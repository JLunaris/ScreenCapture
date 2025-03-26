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
    origin = event->pos();
    rubberBand->setGeometry(QRect {origin, QSize {}});
    rubberBand->show();
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    std::printf("mouseMove\n");
    QPoint eventPos {event->pos()};
    rubberBand->setGeometry(QRect {origin, eventPos}.normalized());
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    std::printf("mouseRelease\n");
}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{
    rubberBand->setGeometry(QRect {});
}
