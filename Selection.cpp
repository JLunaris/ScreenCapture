#include "Selection.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QRect>
#include <QPoint>

void SelectionSingleMargin::mousePressEvent(QMouseEvent *event)
{
    if (!active()) return;

    m_lastEventPosInGrandparent = mapTo(parentWidget()->parentWidget(), event->pos());
}

void SelectionSingleMargin::mouseMoveEvent(QMouseEvent *event)
{
    printf("移动");
    if (!active()) return;

    QPoint eventPosInGrandparent {mapTo(parentWidget()->parentWidget(), event->pos())};

    switch (m_type) {
        using
        enum Type;
    case left:
        int deltaX {eventPosInGrandparent.x() - m_lastEventPosInGrandparent.x()};

        break;
    }

    m_lastEventPosInGrandparent = eventPosInGrandparent;
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
        m_mouseRelativePos = event->pos();
    } else if (m_mode == PaintingMode) {

    }
}

void Selection::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mode == NonPaintingMode) {
        QPoint eventPosInParent {mapToParent(event->pos())};
        move(eventPosInParent - m_mouseRelativePos);
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

void Selection::setMode(Selection::Mode mode)
{
    m_mode = mode;
}

Selection::Mode Selection::mode() const
{
    return m_mode;
}