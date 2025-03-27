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

        QRect screenRect {QApplication::primaryScreen()->geometry()};
        const QRect &bandRect {rubberBand->geometry()};
        QRect newBandRect {bandRect};

        if (deltaY < 0) { // 向上移动
            int screenTop {screenRect.top()};
            int bandTop {bandRect.top()};
            if (bandTop + deltaY >= screenTop) { // 更新后未越界
                newBandRect.translate(0, deltaY);
                lastPos.ry() = eventPos.y();
            } else { // 更新后越界
                newBandRect.moveTop(screenTop);
            }
        } else if (deltaY > 0) { // 向下移动
            int screenBottom {screenRect.bottom()};
            int bandBottom {bandRect.bottom()};
            if (bandBottom + deltaY <= screenBottom) { // 更新后未越界
                newBandRect.translate(0, deltaY);
                lastPos.ry() = eventPos.y();
            } else { // 更新后越界
                newBandRect.moveBottom(screenBottom);
            }
        }

        if (deltaX < 0) { // 向左移动
            int screenLeft {screenRect.left()};
            int bandLeft {bandRect.left()};
            if (bandLeft + deltaX >= screenLeft) { // 更新后未越界
                newBandRect.translate(deltaX, 0);
                lastPos.rx() = eventPos.x();
            } else { // 更新后越界
                newBandRect.moveLeft(screenLeft);
            }
        } else if (deltaX > 0) { // 向右移动
            int screenRight {screenRect.right()};
            int bandRight {bandRect.right()};
            if (bandRight + deltaX <= screenRight) { // 更新后未越界
                newBandRect.translate(deltaX, 0);
                lastPos.rx() = eventPos.x();
            } else { // 更新后越界
                newBandRect.moveRight(screenRight);
            }
        }

        rubberBand->move(newBandRect.topLeft());
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
