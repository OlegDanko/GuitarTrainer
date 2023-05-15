#pragma once

#include "QueueBWNR.hpp"

#include <vector>
#include <memory>

class ToneMapper;
struct Note;

class AudioSynthesizer {
    using buffer_t = std::vector<float>;
    using buffer_prt_t = std::shared_ptr<buffer_t>;

    ToneMapper& tone_mapper;
    size_t frame_size;
    QueueBWNR<buffer_prt_t> queue;
    buffer_prt_t empty_buf;

public:
    AudioSynthesizer(ToneMapper& tm, size_t frame_size);
    void append_note(const Note& n, size_t samples_count);

    buffer_prt_t get_buffer();
};
