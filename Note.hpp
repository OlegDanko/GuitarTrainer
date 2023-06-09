#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <array>

enum ETone {
    C = 0,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B,
    Tones_Count
};

ETone& operator++(ETone& val);
ETone& operator--(ETone& val);

namespace std {
string to_string(ETone val);
}
std::istream& operator>>(std::istream& str, ETone& tone);

template<typename T, typename L, typename H>
bool within_incl(T val, L lo, H hi) {
    return val >= lo && val <= hi;
}

struct Note {
    ETone tone;
    int octave;
    Note(ETone tone = Tones_Count, int octave = -1);
    Note(const std::string& str);
    static Note none();
    Note operator++(int);
    Note operator+(int val);
    Note operator-(int val);
    int operator-(const Note& note) const;
    bool operator==(const Note& val) const;
    bool operator!=(const Note& val) const;
};

Note& operator++(Note& val);
Note& operator--(Note& val);
std::ostream& operator<<(std::ostream& str, const Note& val);
namespace std {
string to_string(const Note& val);
template<> struct hash<Note> {
size_t operator()(const Note& val) const;
};
}
