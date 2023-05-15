#include "FretBoard.hpp"
#include <ranges>

FretBoard::FretBoard(size_t frets_num) : FretBoard(standard_tuning, frets_num) {}

FretBoard::FretBoard(const std::vector<Note> &tuning, size_t frets_num)
    : tuning(tuning)
    , frets_num(frets_num)
{
    auto fret_tuning = tuning;
    std::generate_n(std::back_inserter(fret_widgets), frets_num, [&, this]{
        auto fret = new FretWidget(fret_tuning);
        size_t s{0};
        for(auto& t : fret_tuning) {
            note_position_map[t++].push_back({fret, s++});
        }
        return fret;
    });
}

QWidget *FretBoard::getWidget() {
    if(!widget)
        widget = new FretBoardWidget(fret_widgets);

    return widget;
}

void FretBoard::noteSet(Note note) {
    if(note == Note::none()) {
        auto it = frets_down_set.begin();
        while (it != frets_down_set.end()) {
            it->widget->setStringState(it->string, false);
            it = frets_down_set.erase(it);
        }
        return;
    }
    for(auto& pos : note_position_map[note]) {
        pos.widget->setStringState(pos.string, true);
        frets_down_set.push_back(pos);

    }
}
