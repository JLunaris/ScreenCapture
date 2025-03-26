#include "CaptureWindow.h"
#include <QPainter>

void CaptureWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter {this};
    painter.drawPixmap(0, 0, background);
    painter.fillRect(rect(), QColor(0, 0, 0, 80));
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{

}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{


}
