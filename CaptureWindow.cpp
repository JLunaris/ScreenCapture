#include "CaptureWindow.h"
#include <QPainter>
#include <QRubberBand>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>
#include <QPainterPath>

void CaptureWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.drawPixmap(0, 0, background);

    if (state != BeforeSelection) {
        QPainterPath overlayPath;
        overlayPath.addRect(rect());

        QPainterPath highlightPath;
        highlightPath.addRect(rubberBand->geometry());

        overlayPath = overlayPath.subtracted(highlightPath);
        painter.fillPath(overlayPath, QBrush {QColor {0, 0, 0, 80}});
    }
}

void CaptureWindow::mousePressEvent(QMouseEvent *event)
{
    if (state == BeforeSelection) {
        origin = event->pos();
        rubberBand->setGeometry(QRect {origin, QSize {}});
        rubberBand->show();
    } else if (state == SelectionDone) {
        QPoint eventPos {event->pos()};
        const QRect &bandRect {rubberBand->geometry()};
        if (bandRect.contains(eventPos, true)) {
            state = MovingSelection;
            relativePos.toTop = eventPos.y() - bandRect.top();
            relativePos.toBottom = bandRect.bottom() - eventPos.y();
            relativePos.toLeft = eventPos.x() - bandRect.left();
            relativePos.toRight = bandRect.right() - eventPos.x();
            lastPos = eventPos;
        }
    }
}

void CaptureWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos {event->pos()};
    if (state == BeforeSelection) {
        state = Selecting;
        rubberBand->setGeometry(QRect {origin, eventPos}.normalized());
        update(rect());
    } else if (state == Selecting) {
        rubberBand->setGeometry(QRect {origin, eventPos}.normalized());
        update(rect());
    } else if (state == MovingSelection) {
        int deltaX {eventPos.x() - lastPos.x()};
        int deltaY {eventPos.y() - lastPos.y()};

        QRect screenRect {QApplication::primaryScreen()->geometry()};
        const QRect &bandRect {rubberBand->geometry()};
        QRect newBandRect {bandRect};

        /* 下面的if语句中，"更新后越界"的else分支是有必要的：
         * 当鼠标快速移动到屏幕边缘时，选区并没有完全移动到边缘，而是留有一段距离。而当鼠标移动较慢时，这个现象不会出现。
         * 问题出在处理选区移动时的边界检测逻辑上。当鼠标快速移动时，单次事件的位移量(deltaX/deltaY)可能过大，
         * 导致当前逻辑直接放弃移动，而非移动到允许的最大距离。*/

        if (deltaY < 0) { // 向上移动
            int screenTop {screenRect.top()};
            int bandTop {bandRect.top()};
            if (bandTop + deltaY >= screenTop) { // 更新后未越界
                newBandRect.translate(0, deltaY);
                lastPos.ry() = eventPos.y();
            } else { // 更新后越界
                newBandRect.moveTop(screenTop); // 修正选区位置
                lastPos.ry() = screenTop + relativePos.toTop; // 修正lastPos偏移量
            }
        } else if (deltaY > 0) { // 向下移动
            int screenBottom {screenRect.bottom()};
            int bandBottom {bandRect.bottom()};
            if (bandBottom + deltaY <= screenBottom) { // 更新后未越界
                newBandRect.translate(0, deltaY);
                lastPos.ry() = eventPos.y();
            } else { // 更新后越界
                newBandRect.moveBottom(screenBottom);
                lastPos.ry() = screenBottom - relativePos.toBottom; // 修正lastPos偏移量
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
                lastPos.rx() = relativePos.toLeft - screenLeft; // 修正lastPos偏移量
            }
        } else if (deltaX > 0) { // 向右移动
            int screenRight {screenRect.right()};
            int bandRight {bandRect.right()};
            if (bandRight + deltaX <= screenRight) { // 更新后未越界
                newBandRect.translate(deltaX, 0);
                lastPos.rx() = eventPos.x();
            } else { // 更新后越界
                newBandRect.moveRight(screenRight);
                lastPos.rx() = screenRight - relativePos.toRight; // 修正lastPos偏移量
            }
        }

        rubberBand->move(newBandRect.topLeft());
    }
}

void CaptureWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (state == Selecting || state == MovingSelection) {
        state = SelectionDone;
    }
}

CaptureWindow::CaptureWindow(QPixmap &&background, QWidget *parent)
        : background(std::move(background)), QWidget(parent, Qt::FramelessWindowHint)
{
    rubberBand->setGeometry(QRect {});
}
