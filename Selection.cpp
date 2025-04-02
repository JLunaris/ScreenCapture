#include "Selection.h"
#include <QPaintEvent>
#include <QPainter>

void Selection::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
//    painter.fillRect(rect(), Qt::GlobalColor::yellow);
}

Selection::Selection(QWidget *parent)
        : QWidget(parent)
{

}