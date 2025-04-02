#include "Selection.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>

void Selection::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    QPen pen {QColor {66, 133, 244}};
    pen.setWidth(0); // 宽度设为0 可以避免 在移动选取时线条有时粗有时细
    painter.setPen(pen);
    painter.drawRect(rect().adjusted(1, 1, -1, -1));
}

Selection::Selection(QWidget *parent)
        : QWidget(parent)
{

}