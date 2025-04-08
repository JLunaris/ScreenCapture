#pragma once

#include <QWidget>
#include <QRect>
#include <QCursor>
#include <QPoint>
#include "BasicSelection.h"

class Selection; // forward declaration

class SelectionSingleMargin : public QWidget
{
public:
    enum class Type
    {
        left,
        right,
        top,
        bottom,
        topLeft,
        topRight,
        bottomLeft,
        bottomRight,
    };

private:
    Type m_type;
    bool m_active {true}; // 是否激活该控件(非激活状态下无法拉伸)
    QPoint m_lastEventPosInGrandparent; // 鼠标移动时，记录上次鼠标移动事件的位置(相当于Selection的父控件的坐标系)
    QPoint virtualPointer; // 虚拟鼠标指针

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
public:
    SelectionSingleMargin(Selection *parent, Type type);
    void setActive(bool active);
    [[nodiscard]] bool active() const;
};


class Selection : public QWidget
{
public:
    enum class Mode
    {
        NonPaintingMode, // 可移动、可resize、不可绘画
        PaintingMode, // 不可移动、不可resize、可绘画
    };
    using
    enum Mode;

    friend class SelectionSingleMargin;

private:
    BasicSelection *m_basicSelection {new BasicSelection {this}};
    SelectionSingleMargin *m_leftMargin, *m_rightMargin, *m_topMargin, *m_bottomMargin;
    SelectionSingleMargin *m_topLeftMargin, *m_topRightMargin, *m_bottomLeftMargin, *m_bottomRightMargin;

    int m_extension {4}; // 向外扩展几个像素
    int m_marginWidth {8}; // 边框厚度
    QColor m_frameColor {66, 133, 244}; // 边框颜色
    Mode m_mode {NonPaintingMode}; // 模式

    QPoint m_mouseRelativePos; // 鼠标单击选区时,在该选区中的坐标

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit Selection(QWidget *parent = nullptr);
    void setMode(Mode mode);
    [[nodiscard]] Mode mode() const;
};
