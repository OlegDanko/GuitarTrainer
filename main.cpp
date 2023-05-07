//#include "MainWindow.hpp"

#include "MyWidget.hpp"

#include <iostream>
#include <thread>
#include <future>
#include <optional>
#include <math.h>

#include <QApplication>

#include <portaudio.h>
#include <fftw3.h>


#define SAMPLE_RATE (44100)

struct paData {
    size_t samples_per_frame = 1024;
    size_t fft_size = samples_per_frame / 2 + 1;
    size_t frames_to_draw = 1;
    int ret = 0;
//    MyWidget widget{samples_per_frame, frames_to_draw};
    MyWidget widget{fft_size, frames_to_draw};
    std::atomic_bool running{true};
    std::thread thread;
    std::mutex mtx;
    std::condition_variable cv;
    std::optional<std::vector<double>> samples_opt;

    paData() {
        thread = std::thread([this]{
            std::list<std::vector<double>> samples_list;
            while (running.load()) {
                if(std::unique_lock lk(mtx); true) {
                    cv.wait(lk, [this]{ return samples_opt.has_value()
                                                || running == false; });
                    if(!running) return;
                    samples_list.push_back(std::move(samples_opt.value()));
                    if(samples_list.size() > 8)
                        samples_list.pop_front();
                    samples_opt.reset();

                }

                std::vector<double> samples;
                for(auto& s : samples_list)
                    std::move(s.begin(), s.end(), std::back_inserter(samples));

                std::vector<fftw_complex> fft(samples.size()/2+1);
                auto p = fftw_plan_dft_r2c_1d(samples.size(),
                                              &samples.front(),
                                              &fft.front(),
                                              FFTW_ESTIMATE);
                fftw_execute(p);
                fftw_destroy_plan(p);

                auto fft_it = fft.begin();

                std::vector<double> fft_real;
                std::transform(fft.begin(), fft.end(),
                               std::back_inserter(fft_real), [](auto v){
                    return sqrt(v[0]*v[0] + v[1]*v[1]);
                });

//                std::cout << "printed" << std::endl;
                widget.setSamples(std::move(fft_real));
//                widget.setSamples(std::move(samples));
            }
        });
    }
    ~paData() {
        running = false;
        cv.notify_one();
        thread.join();
    }

    void on_reveived(const float* samples_arr, size_t size) {
        std::vector<double> samples(size);
        size_t n = 0;
        std::generate(samples.begin(), samples.end(),
                      [&n, &samples_arr]{ return samples_arr[n++]; });

        std::lock_guard lk(mtx);
        samples_opt = samples;
        cv.notify_one();
    }
};

class PortAudioWrapper {
    bool initialized{false};
public:
    PortAudioWrapper() {
        auto err = Pa_Initialize();
        initialized = err == paNoError;
        if(!initialized) {
            std::cout << "PortAudio error during initialization: "
                      << Pa_GetErrorText(err) << std::endl;
        }
    }
    ~PortAudioWrapper() {
        if(!initialized)
            return;
        auto err = Pa_Terminate();
        if(err != paNoError)
            std::cout <<  "PortAudio error during termination: "
                      << Pa_GetErrorText(err) << std::endl;
    }
    bool is_initialized() {
        return initialized;
    }
};

int pa_callback(const void *input,
                void *output,
                unsigned long frameSize,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void *userData) {
    auto data = reinterpret_cast<paData*>(userData);
    data->on_reveived(reinterpret_cast<const float*>(input), frameSize);
    return data->ret;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    paData data;

    if(PortAudioWrapper pa; pa.is_initialized()) {
        PaStream *stream;
        PaError err;

        err = Pa_OpenDefaultStream(&stream,
                                   1,
                                   0,
                                   paFloat32,
                                   SAMPLE_RATE,
                                   data.samples_per_frame,
                                   pa_callback,
                                   &data);

        if( err != paNoError ) return -1;

        err = Pa_StartStream( stream );
        if( err != paNoError ) return -1;

        data.widget.show();
        return a.exec();
    }
}
