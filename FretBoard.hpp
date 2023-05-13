#pragma once

#include "FretBoardWidget.hpp"

const std::vector<Note> standard_tuning {
    {"E4"}, {"B3"}, {"G3"}, {"D3"}, {"A2"}, {"E2"},
};

class FretBoard {
    std::vector<FretWidget*> fret_widgets;
    std::vector<size_t> string_positions;
    std::vector<Note> tuning;
    FretBoardWidget* widget{nullptr};
    size_t frets_num;

public:
    FretBoard(size_t frets_num) : FretBoard(standard_tuning, frets_num) {}

    FretBoard(const std::vector<Note>& tuning = standard_tuning, size_t frets_num = 13)
        : tuning(tuning)
        , frets_num(frets_num)
    {
        auto fret_tuning = tuning;
        std::generate_n(std::back_inserter(fret_widgets), frets_num, [&, this]{
            auto fret = new FretWidget(fret_tuning);
            for(auto& t : fret_tuning) ++t;
            return fret;
        });
    }

    QWidget* getWidget() {
        if(!widget)
            widget = new FretBoardWidget(fret_widgets);

        return widget;
    }
};
