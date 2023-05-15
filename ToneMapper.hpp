#pragma once

#include "Note.hpp"

#include <vector>

float note_to_frequency(const Note n, float A4 = 440.0f);

class ToneMapper {
    float smpl_rate;
    float A4_freq;
    Note lo, hi;
    std::vector<float> tones;
    std::vector<float> sin_params;

    size_t index(const Note& n);
public:
    ToneMapper(const Note& lo,
               const Note& hi,
               size_t sample_rate=44100,
               float A4_frequency=440.0f);

    float get_tone(const Note& n);
    float get_sin_param(const Note& n);
};
