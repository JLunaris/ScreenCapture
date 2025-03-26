#pragma once

#include <QWidget>
#include <QPixmap>
#include <QRubberBand>
#include <QRect>

class CaptureWindow : public QWidget
{
    enum class CaptureState
    {
        BeforeSelection,    // 选区前（等待开始）
        Selecting,          // 选区中（鼠标拖动绘制）
        SelectionDone,      // 选区结束（完成绘制）
        ResizingSelection   // 修改选区大小（调整已有选区）
    };
    using enum CaptureState;

private:
    QPixmap background;
    QRubberBand *rubberBand {new QRubberBand {QRubberBand::Rectangle, this}};
    QPoint origin;
    CaptureState state {BeforeSelection};

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit CaptureWindow(QPixmap &&background, QWidget *parent = nullptr);
};

