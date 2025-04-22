#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <stack>

class Selection;

class BasicSelection : public QGraphicsView
{
    Q_OBJECT

public:
    enum class PaintingMode
    {
        NonPainting, // 非绘画
        Rectangle, // 矩形
        Ellipse, // 椭圆
        Line, // 直线
        Pencil, // 画笔
    };

private:
    enum class PaintingProcess
    {
        BeforePainting,
        Painting,
    };

private:
    QGraphicsScene *m_scene {nullptr};
    std::stack<QGraphicsItem *> m_items;

    PaintingMode m_paintingMode {PaintingMode::NonPainting};
    QColor m_penColor {Qt::red};
    qreal m_penWidth {0};

    PaintingProcess m_paintingProcess {PaintingProcess::BeforePainting}; // 绘画过程
    QPointF m_originPoint; // 绘画的起点

public Q_SLOTS:
    void removeItem() noexcept;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit BasicSelection(Selection *parent = nullptr);
    [[nodiscard]] PaintingMode paintingMode() const;
    void setPaintingMode(BasicSelection::PaintingMode mode);
};
