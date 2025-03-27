#include "CaptureWindow.h"
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>

void CaptureWindow::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter {this};
    painter.drawPixmap(0, 0, background);
    painter.fillRect(rect(), QColor(0, 0, 0, 80));
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{
    if (state == BeforeSelection) {
        origin = event->pos();
        rubberBand->setGeometry(QRect {origin, QSize {}});
        rubberBand->show();
        state = Selecting;
    } else if (state == SelectionDone or state == MoveSelectionDone) {
        QPoint eventPos {event->pos()};
        if (rubberBand->geometry().contains(eventPos, true)) {
            state = MovingSelection;
            lastPos = eventPos;
        }
    }
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos {event->pos()};
    if (state == Selecting) {
        rubberBand->setGeometry(QRect {origin, eventPos}.normalized());
    } else if (state == MovingSelection) {
        int deltaX {eventPos.x() - lastPos.x()};
        int deltaY {eventPos.y() - lastPos.y()};
        QRect newRect {rubberBand->geometry()};
        QRect screenRect {QApplication::primaryScreen()->geometry()};

        if (screenRect.contains(newRect.translated(deltaX, 0))) {
            newRect.translate(deltaX, 0);
        }
        if (screenRect.contains(newRect.translated(0, deltaY))) {
            newRect.translate(0, deltaY);
        }
        rubberBand->setGeometry(newRect);
        lastPos = eventPos;
    }
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (state == Selecting) {
        state = SelectionDone;
    } else if (state == MovingSelection) {
        state = MoveSelectionDone;
    }
}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{
    rubberBand->setGeometry(QRect {});
}
