#include "FretBoardWidget.hpp"
#include <QLine>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <ranges>


FretBoardWidget::FretBoardWidget(const std::vector<FretWidget*>& frets,
                                 QWidget *parent)
    : DrawableWidgetBase(parent)
{
    setObjectName("fret_board");
//    setStyleSheet("background-color: brown;");

    auto layout_ = new QHBoxLayout;
    layout_->setSpacing(0);

    setLayout(layout_);

//    frets.front()->setStyleSheet("background-color: black;");
    frets.front()->setObjectName("fret_zero");
    for(auto f : frets)
        layout()->addWidget(f);
    for(auto [n, f] : frets
                    | std::views::enumerate
                    | std::views::drop(1)) {
        auto rem = n%12;

        if(rem == 0) {
            f->setObjectName("fret_double_dot");
            continue;
        }
        if(std::set<int>{3, 5, 7, 9}.count(rem) > 0) {
            f->setObjectName("fret_dot");
            continue;
        }
        f->setObjectName("fret");
    }
}

FretBoardWidget::~FretBoardWidget() {}


