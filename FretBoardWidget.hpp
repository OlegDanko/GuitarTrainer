#pragma once

#include "FretWidget.hpp"

const auto fret_position_none = std::numeric_limits<size_t>::max();

class FretBoardWidget : public DrawableWidgetBase
{
    Q_OBJECT
public:
    FretBoardWidget(const std::vector<FretWidget*>& frets,
                    QWidget *parent = nullptr);
    ~FretBoardWidget();
};
