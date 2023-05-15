#pragma once

#include "QueueBWNR.hpp"
#include "Note.hpp"

#include <vector>
#include <memory>
#include <QObject>

class ToneMapper;

class AudioSynthesizer : public QObject {
    Q_OBJECT
    using buffer_t = std::vector<float>;
    using buffer_prt_t = std::shared_ptr<buffer_t>;

    struct BufferNotePair {
        buffer_prt_t buffer;
        Note note;
    };

    ToneMapper& tone_mapper;
    size_t frame_size;
    QueueBWNR<BufferNotePair> queue;
    buffer_prt_t empty_buf;

public:
    AudioSynthesizer(ToneMapper& tm, size_t frame_size);
    void append_note(const Note& n, size_t samples_count);

    buffer_prt_t get_buffer();

public slots:
signals:
    void noteChangedSignal(Note n);
};
