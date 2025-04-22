#include "BasicSelection.h"
#include <QPainter>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "Selection.h"
#include <print>

void BasicSelection::removeItem() noexcept
{
    if (m_scene == nullptr)
        return;

    if(!m_items.empty()){
        m_scene->removeItem(m_items.top());
        delete m_items.top();
        m_items.pop();
    }
}

void BasicSelection::mousePressEvent(QMouseEvent *event)
{
    switch (m_paintingMode) {
        using
        enum PaintingMode;
    case NonPainting:
        event->ignore();
        break;
    case Rectangle:
    case Ellipse:
    case Line:
    case Pencil:
        m_originPoint = event->position();
        break;
    }
}

void BasicSelection::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_paintingMode) {
        using
        enum PaintingMode;
    case NonPainting:
        event->ignore();
        break;
    case Rectangle: {
        QPointF eventPos {event->position()};
        if (m_paintingProcess == PaintingProcess::BeforePainting) {
            m_paintingProcess = PaintingProcess::Painting;
            auto *item {m_scene->addRect(QRectF {m_originPoint, eventPos}, QPen {m_penColor, m_penWidth})};
            m_items.push(item);
            item->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusable);
            m_scene->setFocusItem(item);
        } else {
            static_cast<QGraphicsRectItem *>(m_scene->focusItem())->
                    setRect(QRectF {m_originPoint, eventPos}.normalized());
        }
        break;
    }
    case Ellipse: {
        QPointF eventPos {event->position()};
        if (m_paintingProcess == PaintingProcess::BeforePainting) {
            m_paintingProcess = PaintingProcess::Painting;
            auto *item {m_scene->addEllipse(QRectF {m_originPoint, eventPos}, QPen {m_penColor, m_penWidth})};
            m_items.push(item);
            item->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusable);
            m_scene->setFocusItem(item);
        } else {
            static_cast<QGraphicsEllipseItem *>(m_scene->focusItem())->
                    setRect(QRectF {m_originPoint, eventPos}.normalized());
        }
        break;
    }
    case Line: {
        QPointF eventPos {event->position()};
        if (m_paintingProcess == PaintingProcess::BeforePainting) {
            m_paintingProcess = PaintingProcess::Painting;
            auto *item {m_scene->addLine(QLineF {m_originPoint, eventPos}, QPen {m_penColor, m_penWidth})};
            m_items.push(item);
            item->setFlag(QGraphicsItem::GraphicsItemFlag::ItemIsFocusable);
            m_scene->setFocusItem(item);
        } else {
            static_cast<QGraphicsLineItem *>(m_scene->focusItem())->
                    setLine(QLineF {m_originPoint, eventPos});
        }
        break;
    }
    }
}

void BasicSelection::mouseReleaseEvent(QMouseEvent *event)
{
    switch (m_paintingMode) {
        using
        enum PaintingMode;
    case NonPainting:
        event->ignore();
        break;
    case Rectangle:
    case Ellipse:
    case Line:
        m_paintingProcess = PaintingProcess::BeforePainting;
        break;
    }
}

BasicSelection::BasicSelection(Selection *parent)
        : QGraphicsView(parent)
{
    setFrameShape(QFrame::Shape::NoFrame); // 关键：移除边框
    viewport()->setAutoFillBackground(false); // 关键：将视口的autoFillBackground设为false
    setRenderHint(QPainter::RenderHint::Antialiasing); // 关键：设置view渲染时抗锯齿
}

BasicSelection::PaintingMode BasicSelection::paintingMode() const
{
    return m_paintingMode;
}

void BasicSelection::setPaintingMode(BasicSelection::PaintingMode mode)
{
    // debug模式断言：不允许从绘画模式切换到非绘画模式
    if (m_paintingMode != PaintingMode::NonPainting)
        assert(mode != PaintingMode::NonPainting);

    // 从非绘画模式切换到绘画模式时，初始化scene
    if (m_paintingMode == PaintingMode::NonPainting and mode != PaintingMode::NonPainting) {
        m_scene = new QGraphicsScene {0, 0, static_cast<qreal>(width()), static_cast<qreal>(height()), this};
        setScene(m_scene);
    }

    m_paintingMode = mode;
    if (mode != PaintingMode::NonPainting) {
        setCursor(QCursor {Qt::CursorShape::CrossCursor});
    }
}
