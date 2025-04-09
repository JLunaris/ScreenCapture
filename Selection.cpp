#include "Selection.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QPoint>

void SelectionSingleMargin::mousePressEvent(QMouseEvent *event)
{
    if (!active()) return;

    m_lastEventPos = mapTo(parentWidget()->parentWidget(), event->pos());
    const QRect &basicGeometry {parentSelection()->basicGeometry()};
    switch (m_type) {
        using
        enum Type;
    case left:
        m_virtualPointer = basicGeometry.topLeft(); // 虚拟鼠标指针 为 左上角
        m_fixedExtremePoint = basicGeometry.bottomRight(); // 固定端点 为 右下角
        // 两个对角确定一个矩形！
        break;
    }
}

void SelectionSingleMargin::mouseMoveEvent(QMouseEvent *event)
{
    if (!active()) return;

    const QPoint eventPosInGrandparent {mapTo(parentWidget()->parentWidget(), event->pos())};
    const QRect grandparentRect {parentSelection()->parentWidget()->rect()};

    switch (m_type) {
        using
        enum Type;
    case left:
        int deltaX {eventPosInGrandparent.x() - m_lastEventPos.x()};

        // 越界检查
        int newX {m_virtualPointer.x() + deltaX};
        if (newX < 0 or newX > grandparentRect.right()) {

        }

        m_virtualPointer += QPoint {deltaX, 0}; // 移动虚拟鼠标指针
        QPoint newTopLeft {m_virtualPointer};
        parentSelection()->setBasicGeometry(QRect {newTopLeft, m_fixedExtremePoint}.normalized());
        break;
    }

    m_lastEventPos = eventPosInGrandparent;
}

void SelectionSingleMargin::mouseReleaseEvent(QMouseEvent *event) {}

SelectionSingleMargin::SelectionSingleMargin(Selection *parent, Type type)
        : QWidget(parent), m_type {type}
{
    int parentWidth {parent->width()};
    int parentHeight {parent->height()};
    int marginWidth {parent->m_marginWidth};
    int lorHeight {parentHeight - 2 * marginWidth}; // left or right marginWidget's height
    int tobWidth {parentWidth - 2 * marginWidth}; // top or bottom marginWidget's width

    switch (type) {
        using
        enum Type;
    case left:
        setGeometry(QRect {0, marginWidth, marginWidth, lorHeight});
        setCursor(QCursor {Qt::SizeHorCursor});
        break;
    case right:
        setGeometry(QRect {marginWidth + tobWidth, marginWidth, marginWidth, lorHeight});
        setCursor(QCursor {Qt::SizeHorCursor});
        break;
    case top:
        setGeometry(QRect {marginWidth, 0, tobWidth, marginWidth});
        setCursor(QCursor {Qt::SizeVerCursor});
        break;
    case bottom:
        setGeometry(QRect {marginWidth, marginWidth + lorHeight, tobWidth, marginWidth});
        setCursor(QCursor {Qt::SizeVerCursor});
        break;
    case topLeft:
        setGeometry(QRect {0, 0, marginWidth, marginWidth});
        setCursor(QCursor {Qt::SizeFDiagCursor});
        break;
    case topRight:
        setGeometry(QRect {marginWidth + tobWidth, 0, marginWidth, marginWidth});
        setCursor(QCursor {Qt::SizeBDiagCursor});
        break;
    case bottomLeft:
        setGeometry(QRect {0, marginWidth + lorHeight, marginWidth, marginWidth});
        setCursor(QCursor {Qt::SizeBDiagCursor});
        break;
    case bottomRight:
        setGeometry(QRect {marginWidth + tobWidth, marginWidth + lorHeight, marginWidth, marginWidth});
        setCursor(QCursor {Qt::SizeFDiagCursor});
        break;
    }
}

Selection *SelectionSingleMargin::parentSelection() const
{
    return static_cast<Selection *>(parentWidget());
}

void SelectionSingleMargin::setActive(bool active)
{
    m_active = active;
}

bool SelectionSingleMargin::active() const
{
    return m_active;
}

void Selection::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    QPen pen {m_frameColor};
    pen.setWidth(2); // 宽度设为0 可以避免 在移动选区时线条有时粗有时细
    painter.setPen(pen);
    painter.drawRect(rect());
}

void Selection::resizeEvent(QResizeEvent *event)
{
    // 调整basicSelection
    {
        int width {rect().width() - 2 * m_extension};
        int height {rect().height() - 2 * m_extension};
        m_basicSelection->resize(width, height);
    }

    // 调整marginWidget
    {
        int newHeight {rect().height()};
        int newWidth {rect().width()};
        int lorHeight {newHeight - 2 * m_marginWidth};
        int tobWidth {newWidth - 2 * m_marginWidth};

        m_leftMargin->resize(m_marginWidth, lorHeight);
        m_rightMargin->setGeometry(newWidth - m_marginWidth, m_marginWidth, m_marginWidth, lorHeight);
        m_topMargin->resize(tobWidth, m_marginWidth);
        m_bottomMargin->setGeometry(m_marginWidth, m_marginWidth + lorHeight, tobWidth, m_marginWidth);

        m_topRightMargin->move(m_marginWidth + tobWidth, 0);
        m_bottomLeftMargin->move(0, m_marginWidth + lorHeight);
        m_bottomRightMargin->move(m_marginWidth + tobWidth, m_marginWidth + lorHeight);
    }
}

void Selection::mousePressEvent(QMouseEvent *event)
{
    if (m_mode == NonPaintingMode) {
        m_lastEventPos = mapToParent(event->pos());
        m_virtualPointer = basicGeometry().topLeft();
    } else if (m_mode == PaintingMode) {

    }
}

void Selection::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mode == NonPaintingMode) {
        const QPoint eventPosInParent {mapToParent(event->pos())};
        QPoint delta {eventPosInParent - m_lastEventPos};
        m_virtualPointer += delta; // 移动虚拟鼠标指针
        QPoint newTopLeft {m_virtualPointer};
        moveBasic(newTopLeft);
        m_lastEventPos = eventPosInParent;
    } else if (m_mode == PaintingMode) {

    }
}

void Selection::mouseReleaseEvent(QMouseEvent *event)
{

}

Selection::Selection(QWidget *parent)
        : QWidget(parent)
{
    // resize according to m_basicSelection
    {
        int width {m_basicSelection->rect().width() + 2 * m_extension};
        int height {m_basicSelection->rect().height() + 2 * m_extension};
        resize(width, height);
    }

    m_basicSelection->move(m_extension, m_extension);

    // init MarginWidget
    {
        int lorHeight {height() - 2 * m_marginWidth}; // left or right marginWidget's height
        int tobWidth {width() - 2 * m_marginWidth}; // top or bottom marginWidget's width

        using
        enum SelectionSingleMargin::Type;

        m_leftMargin = new SelectionSingleMargin {this, left};
        m_rightMargin = new SelectionSingleMargin {this, right};
        m_topMargin = new SelectionSingleMargin {this, top};
        m_bottomMargin = new SelectionSingleMargin {this, bottom};
        m_topLeftMargin = new SelectionSingleMargin {this, topLeft};
        m_topRightMargin = new SelectionSingleMargin {this, topRight};
        m_bottomLeftMargin = new SelectionSingleMargin {this, bottomLeft};
        m_bottomRightMargin = new SelectionSingleMargin {this, bottomRight};
    }

    setCursor(QCursor {Qt::SizeAllCursor});
    setMinimumWidth(m_extension * 2); // 设定最小宽度，避免因过小而无法点击到Margin
    setMinimumHeight(m_extension * 2); // 同理
}

Selection::Mode Selection::mode() const
{
    return m_mode;
}


void Selection::setMode(Selection::Mode mode)
{
    m_mode = mode;
}


QRect Selection::basicGeometry() const
{
    QPoint topLeft {geometry().topLeft() + QPoint {m_extension, m_extension}};
    QPoint bottomRight {geometry().bottomRight() - QPoint {m_extension, m_extension}};
    return QRect {topLeft, bottomRight};
}


void Selection::setBasicGeometry(QRect rect)
{
    QRect parentRect {parentWidget()->rect()};
    // 禁止越界
    if (rect.left() < 0) { rect.setLeft(0); }
    if (rect.right() > parentRect.right()) { rect.setRight(parentRect.right()); }
    if (rect.top() < 0) { rect.setTop(0); }
    if (rect.bottom() > parentRect.bottom()) { rect.setBottom(parentRect.bottom()); }

    QPoint topLeft {rect.topLeft() - QPoint {m_extension, m_extension}};
    QPoint bottomRight {rect.bottomRight() + QPoint {m_extension, m_extension}};
    setGeometry(QRect {topLeft, bottomRight});
}


void Selection::setBasicGeometry(int x, int y, int w, int h)
{
    setBasicGeometry(QRect {x, y, w, h});
}


QRect Selection::basicRect() const
{
    return QRect {0, 0, m_basicSelection->width(), m_basicSelection->height()};
}

int Selection::basicWidth() const
{
    return m_basicSelection->width();
}

int Selection::basicHeight() const
{
    return m_basicSelection->height();
}

void Selection::moveBasic(QPoint point)
{
    QRect parentRect {parentWidget()->rect()};
    // 禁止越界
    if (point.x() < 0) { point.rx() = 0; }
    if (point.x() + basicWidth() > parentRect.right()) { point.rx() = parentRect.right() - basicWidth(); }
    if (point.y() < 0) { point.ry() = 0; }
    if (point.y() + basicHeight() > parentRect.bottom()) { point.ry() = parentRect.bottom() - basicHeight(); }

    move(point - QPoint {m_extension, m_extension});
}

void Selection::moveBasic(int x, int y)
{
    moveBasic(QPoint {x, y});
}