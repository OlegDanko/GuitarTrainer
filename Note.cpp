#include "Note.hpp"

#include <array>
#include <map>
#include <sstream>

ETone &operator++(ETone &val) {
    switch (val) {
    case A:             val = As; break;
    case As:        val = B; break;
    case B:             val = C; break;
    case C:             val = Cs; break;
    case Cs:        val = D; break;
    case D:             val = Ds; break;
    case Ds:        val = E; break;
    case E:             val = F; break;
    case F:             val = Fs; break;
    case Fs:        val = G; break;
    case G:             val = Gs; break;
    case Gs:        val = A; break;
    case Tones_Count:   val = Tones_Count; break;
    }
    return val;
}

ETone &operator--(ETone &val) {
    switch (val) {
    case A:             val = Gs; break;
    case As:        val = A; break;
    case B:             val = As; break;
    case C:             val = B; break;
    case Cs:        val = C; break;
    case D:             val = Cs; break;
    case Ds:        val = D; break;
    case E:             val = Ds; break;
    case F:             val = E; break;
    case Fs:        val = F; break;
    case G:             val = Fs; break;
    case Gs:        val = G; break;
    case Tones_Count:   val = Tones_Count; break;
    }
    return val;
}

std::string std::to_string(ETone val) {
    switch (val) {
    case A:         return "A";
    case As:    return "A#";
    case B:         return "B";
    case C:         return "C";
    case Cs:    return "C#";
    case D:         return "D";
    case Ds:    return "D#";
    case E:         return "E";
    case F:         return "F";
    case Fs:    return "F#";
    case G:         return "G";
    case Gs:    return "G#";
    default:        return "";
    }
}

std::istream &operator>>(std::istream &str, ETone &tone) {
    std::map<std::string, ETone> tones_map {
                                           {"A",  A},
                                           {"A#", As},
                                           {"B",  B},
                                           {"C",  C},
                                           {"C#", Cs},
                                           {"D",  D},
                                           {"D#", Ds},
                                           {"E",  E},
                                           {"F",  F},
                                           {"F#", Fs},
                                           {"G",  G},
                                           {"G#", Gs} };

    std::array<char, 3> key{'\0', '\0', '\0'};
    str >> key.at(0);
    key.at(0) = std::toupper(key.at(0));

    if(!within_incl(key.at(0), 'A', 'G')) {
        str.setstate(std::ios::failbit);
        return str;
    }

    if(str.peek() == '#')
        str >> key.at(1);

    if(auto it = tones_map.find(&key.at(0)); tones_map.end() != it)
        tone = it->second;
    else
        str.setstate(std::ios::failbit);

    return str;
}

Note::Note(ETone tone, int octave)
    : tone(tone)
    , octave(octave) {}

Note::Note(const std::string &str) {
    std::stringstream ss(str);
    if(!(ss >> tone >> octave).fail())
        return;
    tone = Tones_Count;
    octave = -1;
}

Note Note::none() {
    return {Tones_Count, -1};
}

Note Note::operator+(int val) {
    auto div_r = std::div(octave * Tones_Count + tone + val, Tones_Count);
    return {static_cast<ETone>(div_r.rem), div_r.quot};
}

Note Note::operator-(int val) {
    return *this + (-val);
}

bool Note::operator==(const Note &val) const {
    return tone == val.tone && octave == val.octave;
}

bool Note::operator!=(const Note &val) const {
    return !((*this) == val);
}

int Note::operator-(const Note &note) const {
    return (octave - note.octave) * 12 + (tone - note.tone);
}

Note &operator++(Note &val) {
    if(++(val.tone) == C)
        ++(val.octave);
    return val;
}

Note &operator--(Note &val) {
    if(--(val.tone) == B)
        --(val.octave);
    return val;
}

std::string std::to_string(const Note &val) {
    return std::to_string(val.tone) + std::to_string(val.octave);
}

std::ostream &operator<<(std::ostream &str, const Note &val) {
    return str << std::to_string(val);
}

Note Note::operator++(int) {
    auto val = *this;
    ++(*this);
    return val;
}

std::size_t std::hash<Note>::operator()(const Note &val) const {
    union {
        std::array<int, 2> to;
        size_t hash;
    } hash_union{.to = {val.tone, val.octave}};
    return hash_union.hash;
}
