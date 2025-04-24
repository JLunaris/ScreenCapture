#include "CaptureWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QPointF>
#include <QApplication>
#include <QPainterPath>
#include <QBrush>
#include <print>
#include <stdexcept>
#include <QClipboard>
#include <QDebug>

//void CaptureWidget::paintEvent(QPaintEvent *event)
//{
//    QPainter painter {this};
//    painter.drawPixmap(0, 0, background);
//
//    if (m_state != BeforeSelection) {
//        QPainterPath overlayPath;
//        overlayPath.addRect(rect());
//
//        QPainterPath highlightPath;
//        highlightPath.addRect(selection->geometry());
//
//        overlayPath = overlayPath.subtracted(highlightPath);
//        painter.fillPath(overlayPath, QBrush {QColor {0, 0, 0, 80}});
//    }
//}
//
//void CaptureWidget::mousePressEvent(QMouseEvent *event)
//{
//    if (m_state == BeforeSelection) {
//        origin = event->pos();
//        selection->setGeometry(QRect {origin, QSize {}});
//        selection->show();
//    } else if (m_state == SelectionDone) {
//        QPoint eventPos {event->pos()};
//        const QRect &bandRect {selection->geometry()};
//        if (bandRect.contains(eventPos, true)) {
//            m_state = MovingSelection;
//            relativePos.toTop = eventPos.y() - bandRect.top();
//            relativePos.toBottom = bandRect.bottom() - eventPos.y();
//            relativePos.toLeft = eventPos.x() - bandRect.left();
//            relativePos.toRight = bandRect.right() - eventPos.x();
//            lastPos = eventPos;
//        }
//    }
//}
//
//void CaptureWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    QPoint eventPos {event->pos()};
//    if (m_state == BeforeSelection) {
//        m_state = Selecting;
//        selection->setGeometry(QRect {origin, eventPos}.normalized());
//        update(rect()); // 重新绘制整个区域
//    } else if (m_state == Selecting) {
//        QRect newRect {QRect {origin, eventPos}.normalized()};
//        QRect updatedRect {selection->geometry().united(newRect)}; // 需要更新的部分 = 旧选区 ∪ 新选区
//        // 见 https://doc.qt.io/qt-6/qrect.html#united
//        selection->setGeometry(newRect);
//        update(updatedRect);
//    } else if (m_state == MovingSelection) {
//        int deltaX {eventPos.x() - lastPos.x()};
//        int deltaY {eventPos.y() - lastPos.y()};
//
//        QRect screenRect {QApplication::primaryScreen()->geometry()};
//        const QRect &bandRect {selection->geometry()};
//        QRect newBandRect {bandRect};
//
//        /* 下面的if语句中，"更新后越界"的else分支是有必要的：
//         * 当鼠标快速移动到屏幕边缘时，选区并没有完全移动到边缘，而是留有一段距离。而当鼠标移动较慢时，这个现象不会出现。
//         * 问题出在处理选区移动时的边界检测逻辑上。当鼠标快速移动时，单次事件的位移量(deltaX/deltaY)可能过大，
//         * 导致当前逻辑直接放弃移动，而非移动到允许的最大距离。*/
//
//        if (deltaY < 0) { // 向上移动
//            int screenTop {screenRect.top()};
//            int bandTop {bandRect.top()};
//            if (bandTop + deltaY >= screenTop) { // 更新后未越界
//                newBandRect.translate(0, deltaY);
//                lastPos.ry() = eventPos.y();
//            } else { // 更新后越界
//                newBandRect.moveTop(screenTop); // 修正选区位置
//                lastPos.ry() = screenTop + relativePos.toTop; // 修正lastPos偏移量
//            }
//        } else if (deltaY > 0) { // 向下移动
//            int screenBottom {screenRect.bottom()};
//            int bandBottom {bandRect.bottom()};
//            if (bandBottom + deltaY <= screenBottom) { // 更新后未越界
//                newBandRect.translate(0, deltaY);
//                lastPos.ry() = eventPos.y();
//            } else { // 更新后越界
//                newBandRect.moveBottom(screenBottom);
//                lastPos.ry() = screenBottom - relativePos.toBottom; // 修正lastPos偏移量
//            }
//        }
//
//        if (deltaX < 0) { // 向左移动
//            int screenLeft {screenRect.left()};
//            int bandLeft {bandRect.left()};
//            if (bandLeft + deltaX >= screenLeft) { // 更新后未越界
//                newBandRect.translate(deltaX, 0);
//                lastPos.rx() = eventPos.x();
//            } else { // 更新后越界
//                newBandRect.moveLeft(screenLeft);
//                lastPos.rx() = relativePos.toLeft - screenLeft; // 修正lastPos偏移量
//            }
//        } else if (deltaX > 0) { // 向右移动
//            int screenRight {screenRect.right()};
//            int bandRight {bandRect.right()};
//            if (bandRight + deltaX <= screenRight) { // 更新后未越界
//                newBandRect.translate(deltaX, 0);
//                lastPos.rx() = eventPos.x();
//            } else { // 更新后越界
//                newBandRect.moveRight(screenRight);
//                lastPos.rx() = screenRight - relativePos.toRight; // 修正lastPos偏移量
//            }
//        }
//
//        selection->move(newBandRect.topLeft());
//    }
//}
//
//void CaptureWidget::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (m_state == Selecting || m_state == MovingSelection) {
//        m_state = SelectionDone;
//    }
//}
//


void CaptureWidget::updatePosOfUtilityPanel()
{
    if (m_utilityPanel) {
        QPointF point {QRectF {m_selection->basicGeometry()}.bottomRight()}; // 选区右下角的坐标
        point.ry() += 3; // 向下平移
        point.rx() -= m_utilityPanel->width(); // 向左平移
        m_utilityPanel->safelyMove(point);
    } else throw std::logic_error {"m_utilityPanel has not been created yet!"};
}


void CaptureWidget::renderCaptureImage()
{
    // 获取屏幕的设备像素比(DPR)
    QScreen *screen {QGuiApplication::primaryScreen()};
    qreal ratio {screen->devicePixelRatio()};

    // 将像素比作为缩放因子
    QTransform transform;
    transform.scale(ratio, ratio); // 缩放率: ratio

    // 获取选区部分的截图
    m_captureImage = m_background.copy(transform.mapRect(m_selection->basicGeometry())).toImage();

    // 渲染涂鸦部分
    QPainter painter {&m_captureImage};
    painter.setRenderHint(QPainter::Antialiasing);
    m_selection->basicSelection()->render(&painter, m_captureImage.rect(),
                                          transform.mapRect(m_selection->basicSelection()->rect()));
}


void CaptureWidget::copyImageToClipboard() const
{
    if (m_captureImage.isNull())
        return;

    QClipboard *clipboard {QGuiApplication::clipboard()};
    clipboard->setImage(m_captureImage);
}


void CaptureWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_state == BeforeSelection) {
        m_origin = event->position();
        m_selection = new Selection {this};
        m_selection->setBasicGeometry(QRectF {m_origin, m_origin}.toRect());
        m_selection->show(); // Note: 必须显式show()才能可见
        update(); // 更新整个CaptureWidget以显示出遮罩
        m_state = Selecting;
    }
}


void CaptureWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF eventPos {event->position()};
    if (m_state == Selecting) {
        m_selection->setBasicGeometry(QRectF {m_origin, eventPos}.normalized().toRect());
    }
}


void CaptureWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_state == Selecting) {
        m_utilityPanel = new UtilityPanel {this};
        m_utilityPanel->show();
        m_utilityPanel->setFixedHeight(28);
        connect(m_selection, &Selection::geometryChanged, this, &CaptureWidget::updatePosOfUtilityPanel);
        updatePosOfUtilityPanel();
        m_state = SelectionDone;
    }
}


void CaptureWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.setRenderHint(QPainter::Antialiasing); // 关键：设定抗锯齿，否则改变选区尺寸或位置时会产生锯齿(需要仔细观察)
    painter.drawPixmap(rect(), m_background);
    if (m_selection) {
        QPainterPath overlayPath;
        overlayPath.addRect(rect());

        QPainterPath highlightPath;
        highlightPath.addRect(m_selection->basicGeometry());

        overlayPath = overlayPath.subtracted(highlightPath);
        painter.fillPath(overlayPath, QBrush {QColor {0, 0, 0, 80}});
    }
}


CaptureWidget::CaptureWidget(QPixmap &&background)
        : m_background(std::move(background))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint); // 无边框 | 始终显示在顶部
    setWindowTitle("截图工具");
    setWindowIcon(QIcon {":/Icon"});
    QCursor cursor {QPixmap {":/RedCursor"}, 0, 0};
    setCursor(cursor);
}
