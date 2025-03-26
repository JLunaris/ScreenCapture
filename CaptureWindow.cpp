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
//    painter.fillRect(rect(), QColor(0, 0, 0, 80));
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{
    if (state == BeforeSelection) {
        origin = event->pos();
        rubberBand->setGeometry(QRect {origin, QSize {}});
        rubberBand->show();
        state = Selecting;
    }
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos {event->pos()};
    if (state == Selecting) {
        rubberBand->setGeometry(QRect {origin, eventPos}.normalized());
    }
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (state == Selecting)
        state = SelectionDone;
}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{
    rubberBand->setGeometry(QRect {});
}
