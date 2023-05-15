#include "FretWidget.hpp"

#include <QVBoxLayout>
#include <QPushButton>
#include <ranges>
#include <iostream>

const std::vector<Note> no_tuning {
    {}, {}, {}, {}, {}, {},
};

FretWidget::FretWidget(const std::vector<Note>& tones, QWidget *parent)
    : DrawableWidgetBase(parent)
{
    auto layout_ = new QVBoxLayout();
    layout_->setSpacing(0);
    layout_->setMargin(0);
    setLayout(layout_);

    size_t num = 0;
    std::transform(tones.begin(),
                   tones.end(),
                   std::back_inserter(strings),
                   [&, this](const auto& tone){
                       auto btn = new QPushButton(std::to_string(tone).c_str(), this);
                       btn->setObjectName("roundbg");
                       btn->setFixedHeight(32);
                       layout()->addWidget(btn);

                       auto gen_callback = [this](size_t n, bool state) {
                           return [=, this]{ stringStateChangedSignal(n, state); };
                       };

                       connect(btn, &QPushButton::pressed, gen_callback(num, true));
                       connect(btn, &QPushButton::pressed, gen_callback(num, false));

                       return btn;
                   });

    std::fill_n(std::back_inserter(states), tones.size(), false);
}

FretWidget::FretWidget(QWidget *parent)
    : FretWidget(no_tuning, parent) {}

FretWidget::~FretWidget() {}

void FretWidget::setStringsStates(const std::vector<bool>& states_new) {
    if(states.size() != states_new.size())
        throw std::runtime_error("FretWidget::stringsStateChangedSlot: "
                                 "Incorrect number of strings in a new strings state");

    for(auto elem : std::views::zip(states, states_new)
                         | std::views::enumerate) {
        auto [before, after] = std::get<1>(elem);

        if(before == after) continue;
        auto n = std::get<0>(elem);

        setStringState(n, after);
    }
}

void FretWidget::setStringState(size_t string_num, bool state) {
    states.at(string_num) = state;
    strings.at(string_num)->setDown(state);
}

void FretWidget::setStringsRangeState(size_t string_from, size_t string_to, bool state) {
    for(auto num : std::views::iota(string_from, string_to))
        setStringState(num, false);
}

void FretWidget::activateStringStateExclusive(size_t string_num) {
    setStringsRangeState(0, string_num, false);
    setStringState(string_num++, true);
    setStringsRangeState(string_num, strings.size(), false);
}
