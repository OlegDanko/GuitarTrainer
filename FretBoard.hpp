#pragma once

#include "FretBoardWidget.hpp"

#include <unordered_set>

const std::vector<Note> standard_tuning {
    {"E4"}, {"B3"}, {"G3"}, {"D3"}, {"A2"}, {"E2"},
};

class FretBoard : public QObject {
    Q_OBJECT

    std::vector<FretWidget*> fret_widgets;
    std::vector<size_t> string_positions;
    std::vector<Note> tuning;
    FretBoardWidget* widget{nullptr};
    size_t frets_num;

    struct FretPosition{FretWidget* widget; size_t string; };
    std::unordered_map<Note, std::vector<FretPosition>> note_position_map;
    std::list<FretPosition> frets_down_set;
public:
    FretBoard(size_t frets_num);

    FretBoard(const std::vector<Note>& tuning = standard_tuning, size_t frets_num = 13);

    QWidget* getWidget();

public slots:
    void noteSet(Note note);

};
