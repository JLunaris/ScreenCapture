#pragma once

#include <QWidget>
#include <QPixmap>
#include <QRubberBand>
#include <QRect>
#include <optional>

class CaptureWindow : public QWidget
{
    enum class CaptureState
    {
        BeforeSelection,    // 选区前（等待开始）
        Selecting,          // 选区中（鼠标拖动绘制）
        SelectionDone,      // 选区结束（完成绘制）
        MovingSelection,    // 移动已有选区中
        MoveSelectionDone,  // 移动选区结束
    };
    using enum CaptureState;

private:
    QPixmap background;
    QRubberBand *rubberBand {new QRubberBand {QRubberBand::Rectangle, this}};
    QPoint origin; // 选区时，用于记录选区的起点
    CaptureState state {BeforeSelection};
    QPoint lastPos; // 移动选区时，用于记录上一次鼠标事件的位置

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit CaptureWindow(QPixmap &&background, QWidget *parent = nullptr);
};

