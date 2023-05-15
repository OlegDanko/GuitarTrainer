#include "ToneMapper.hpp"

#include <algorithm>
#include <cmath>

float note_to_frequency(const Note n, float A4) {
    static const float a{std::pow(2.0f, 1/12.0f)};
    return A4 * std::pow(a, n - Note(A, 4));
}

size_t ToneMapper::index(const Note &n) {
    auto id = n - lo;
    return n - lo;
}

ToneMapper::ToneMapper(const Note &lo, const Note &hi, size_t sample_rate, float A4_frequency)
    : smpl_rate(sample_rate)
    , A4_freq(A4_frequency)
    , lo(lo)
    , hi(hi)
{
    Note note = lo;
    std::generate_n(std::back_inserter(tones), (hi - lo) + 1, [&, this]{
        return note_to_frequency(note++, A4_freq);
    });
    std::transform(tones.begin(), tones.end(),
                   std::back_inserter(sin_params), [this](auto freq) {
                       auto samples_per_wave = smpl_rate / freq;
                       return 2*M_PIf / samples_per_wave;
                   });
}

float ToneMapper::get_tone(const Note &n) {
    return tones.at(index(n));
}

float ToneMapper::get_sin_param(const Note &n) {
    return sin_params.at(index(n));
}
