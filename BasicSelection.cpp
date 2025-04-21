#include "BasicSelection.h"
#include <QPainter>
#include <QBrush>
#include "Selection.h"

BasicSelection::BasicSelection(Selection *parent)
        : QGraphicsView(parent)
{
    setScene(nullptr);
    setFrameShape(QFrame::Shape::NoFrame); // 关键：移除边框
    viewport()->setAutoFillBackground(false); // 关键：将视口的autoFillBackground设为false
}

void BasicSelection::paintEvent(QPaintEvent *event)
{

}

void BasicSelection::mousePressEvent(QMouseEvent *event)
{
    switch (m_paintingMode) {
        using
        enum PaintingMode;
    case NonPainting:
        event->ignore();
        break;
    }
//    setPaintingMode(BasicSelection::PaintingMode::Rectangle);
}

void BasicSelection::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_paintingMode) {
        using
        enum PaintingMode;
    case NonPainting:
        event->ignore();
        break;
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
    }
}

void BasicSelection::setPaintingMode(BasicSelection::PaintingMode mode)
{
    m_paintingMode = mode;
    if (mode != PaintingMode::NonPainting) {
        setCursor(QCursor {Qt::CursorShape::CrossCursor});
    }
}

BasicSelection::PaintingMode BasicSelection::paintingMode() const
{
    return m_paintingMode;
}