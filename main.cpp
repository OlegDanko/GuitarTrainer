#include "FretBoard.hpp"

#include <QApplication>
#include <QFile>
#include <QtWidgets>
#include <thread>
#include <ranges>
#include <chrono>

#include "PortAudioWrapper.hpp"
#include "ToneMapper.hpp"
#include "AudioSynthesizer.hpp"

#define SAMPLE_RATE (44100.0f)

int pa_output_callback(const void *input,
                       void *output,
                       unsigned long frameSize,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData) {
    auto& as = *static_cast<AudioSynthesizer*>(userData);
    auto buf = as.get_buffer();
    auto out_buf = reinterpret_cast<float*>(output);
    for(auto [id, sample] : *buf | std::views::enumerate) {
        out_buf[id] = sample;
    }
    return 0;
}

Q_DECLARE_METATYPE(Note);
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<Note>();

    QFile styleFile(":/style.css");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    app.setStyleSheet(style);

    QWidget widget;
    widget.setLayout(new QVBoxLayout());
    FretBoard board(13);
    widget.layout()->addWidget(board.getWidget());
    widget.layout()->addItem(new QSpacerItem(1000000,
                                             1000000,
                                             QSizePolicy::Expanding));

    ToneMapper tone_mapper(Note{"E2"}, Note{"F5"});
    AudioSynthesizer synth(tone_mapper, 512);
    AudioSynthesizer::connect(&synth, &AudioSynthesizer::noteChangedSignal,
                              &board, &FretBoard::noteSet);

    std::atomic_bool running{true};
    std::thread t;

    if(PortAudioWrapper pa; pa.is_initialized()) {
        PaStream *stream;
        PaError err;

        err = Pa_OpenDefaultStream(&stream,
                                   0,
                                   1,
                                   paFloat32,
                                   SAMPLE_RATE,
                                   512,
                                   pa_output_callback,
                                   &synth);

        if( err != paNoError ) return -1;

        err = Pa_StartStream( stream );
        if( err != paNoError ) return -1;

        t = std::thread([&]{
            using namespace std::chrono_literals;

            std::vector<Note> sequence{{A, 4}, {B, 4}, {C, 4}, {D, 4}};
            auto i = 0;
            auto start = std::chrono::steady_clock::now();
            while(running) {
                synth.append_note(sequence.at(i++), SAMPLE_RATE/2);
                i %= sequence.size();
                start += 1s;
                auto now = std::chrono::steady_clock::now();
                if (now > start) {
                    start = now;
                    continue;
                }
                std::this_thread::sleep_until(start);
            }
        });

        widget.show();
        app.exec();

        running = false;
        t.join();
    }
    return 0;
}
