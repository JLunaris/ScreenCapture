#include "CaptureWindow.h"
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>

void CaptureWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter {this};
    painter.drawPixmap(0, 0, background);
    painter.fillRect(rect(), QColor(0, 0, 0, 80));
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{
    std::printf("mousePress\n");
    QPoint pos {event->pos()};
    if (rubberBandRect.isNull()) {
        rubberBandRect.setTopLeft(pos);
        rubberBand->setGeometry(rubberBandRect);
        rubberBand->show();
    }
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    std::printf("mouseMove\n");
    if(true){
        QPoint pos {event->pos()};
//        rubberBandRect->set
    }

}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    std::printf("mouseRelease\n");
}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{

}
