#pragma once

#include <QWidget>

class DrawableWidgetBase : public QWidget
{
    Q_OBJECT
public:
    DrawableWidgetBase(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *) override;
};

