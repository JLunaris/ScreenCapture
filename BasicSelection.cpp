#include "BasicSelection.h"
#include <QPainter>
#include <QBrush>

void BasicSelection::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
//    painter.fillRect(rect(), Qt::GlobalColor::red);
}

BasicSelection::BasicSelection(QWidget *parent)
        : QWidget(parent)
{

}