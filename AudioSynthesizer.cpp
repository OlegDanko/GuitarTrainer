#include "AudioSynthesizer.hpp"
#include "ToneMapper.hpp"

#include <cmath>

AudioSynthesizer::AudioSynthesizer(ToneMapper &tm, size_t frame_size)
    : tone_mapper(tm)
    , frame_size(frame_size)
    , queue(10)
    , empty_buf(std::make_shared<buffer_t>(frame_size))
{}

AudioSynthesizer::buffer_prt_t AudioSynthesizer::get_buffer() {
    buffer_prt_t buf;
    if(queue.pop(buf)) {
        return buf;
    }
    return empty_buf;
}

void AudioSynthesizer::append_note(const Note &n, size_t samples_count) {
    size_t i = 0;
    auto sin_param = tone_mapper.get_sin_param(n);
    auto gen_fn = [&] {
        return std::sin(sin_param * i++);
    };

    size_t count = 0;
    while(samples_count > frame_size) {
        ++count;
        auto buf = std::make_shared<buffer_t>(frame_size);
        std::generate(buf->begin(), buf->end(), gen_fn);
        queue.push(std::move(buf));
        samples_count -= frame_size;
    }

    auto buf = std::make_shared<buffer_t>(frame_size);
    std::generate_n(buf->begin(), samples_count, gen_fn);
    queue.push(std::move(buf));
}
