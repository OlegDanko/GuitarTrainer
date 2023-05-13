#pragma once

#include "DrawableWidgetBase.hpp"
#include "Note.hpp"

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <sstream>
#include <set>

class FretWidget : public DrawableWidgetBase
{
    Q_OBJECT

    std::vector<QWidget*> strings;
    std::vector<bool> states;
    void setStringsRangeState(size_t string_from, size_t string_to, bool state);
public:
    explicit FretWidget(QWidget *parent = nullptr);
    FretWidget(const std::vector<Note>&, QWidget *parent = nullptr);
    ~FretWidget();
    void setStringsStates(const std::vector<bool>& states_new);
    void setStringState(size_t string_num, bool state);
    void activateStringStateExclusive(size_t string_num);
public slots:
signals:
    void stringStateChangedSignal(size_t n, bool state);
};
