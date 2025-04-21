#pragma once

#include <QGraphicsView>
#include <QMouseEvent>
#include <QAbstractScrollArea>

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
    PaintingMode m_paintingMode {PaintingMode::NonPainting};

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit BasicSelection(Selection *parent = nullptr);
    void setPaintingMode(PaintingMode mode);
    [[nodiscard]] PaintingMode paintingMode() const;
};
