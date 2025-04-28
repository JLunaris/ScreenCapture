#pragma once

#include "Selection.h"
#include "UtilityPanel.h"

#include <QWidget>
#include <QPixmap>
#include <QRect>
#include <QPoint>
#include <QPointF>
#include <QImage>

//class CaptureWidget : public QWidget
//{
//    enum class CaptureState
//    {
//        BeforeSelection,    // 选区前（等待开始）
//        Selecting,          // 选区中（鼠标拖动绘制）
//        SelectionDone,      // 选区结束（完成绘制）
//        MovingSelection,    // 移动已有选区中
//    };
//    using enum CaptureState;
//
//private:
//    QPixmap background;
//    Selection *selection {new Selection {this}};
//
//    CaptureState m_state {BeforeSelection};
//    QPoint origin; // 选区时，用于记录选区的起点
//    QPoint lastPos; // 移动选区时，用于记录上一次鼠标事件的位置(不一定是真的)
//    struct { int toTop, toBottom, toLeft, toRight; } relativePos {}; // 移动选区时，记录鼠标相对选区四周的距离
//
//protected:
//    void paintEvent(QPaintEvent *event) override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
//    void mouseReleaseEvent(QMouseEvent *event) override;
//
//public:
//    explicit CaptureWidget(QPixmap &&background, QWidget *parent = nullptr);
//};

class CaptureWidget : public QWidget
{
    Q_OBJECT

    friend class UtilityPanel;

public:
    enum class CaptureState
    {
        BeforeSelection,    // 选区前（等待开始）
        Selecting,          // 选区中（鼠标拖动绘制）
        SelectionDone,      // 选区结束（完成绘制）
    };
    using enum CaptureState;

private:
    QPixmap m_background;
    Selection *m_selection {nullptr};
    UtilityPanel *m_utilityPanel {nullptr};
    QImage m_captureImage;

    CaptureState m_state {CaptureState::BeforeSelection}; // 执行状态
    QPointF m_origin; // 选区的起点

private Q_SLOTS:
    void updatePosOfUtilityPanel();

public Q_SLOTS:
    void renderCaptureImage();
    void copyImageToClipboard() const;
    void saveImageToUserPath();

Q_SIGNALS:
    void backToMainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

public:
    explicit CaptureWidget(QPixmap &&background);
};
