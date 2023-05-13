#include "DrawableWidgetBase.hpp"

#include <QPainter>
#include <QStyleOption>

DrawableWidgetBase::DrawableWidgetBase(QWidget *parent) : QWidget(parent) {}

void DrawableWidgetBase::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
