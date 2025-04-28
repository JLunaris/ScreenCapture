#pragma once

#include "BasicSelection.h"

#include <QWidget>
#include <QRect>
#include <QCursor>
#include <QPoint>


class Selection; // forward declaration

class SelectionSingleMargin : public QWidget
{
    Q_OBJECT

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
    QPoint m_lastEventPos; // 鼠标移动时，记录上次鼠标移动事件的位置(相对于Selection的父控件的坐标系)
    // 当虚拟指针将要超出Selection的父控件的范围时，修正鼠标移动事件的位置。
    QPoint m_fixedExtremePoint; // 鼠标移动时，记录固定端点(相对于Selection的父控件的坐标系)
    QPoint m_virtualPointer; // 虚拟鼠标指针(相对于Selection的父控件的坐标系)

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    SelectionSingleMargin(Selection *parent, Type type);
    [[nodiscard]] Selection *parentSelection() const;
    void setActive(bool active);
    [[nodiscard]] bool active() const;
};


class Selection : public QWidget
{
    Q_OBJECT

public:
    enum class Mode
    {
        NonPaintingMode, // 可移动、可resize、不可绘画
        PaintingMode, // 不可移动、不可resize、可绘画
    };
    using enum Mode;

    friend class SelectionSingleMargin;

private:
    BasicSelection *m_basicSelection {new BasicSelection {this}};
    SelectionSingleMargin *m_leftMargin, *m_rightMargin, *m_topMargin, *m_bottomMargin;
    SelectionSingleMargin *m_topLeftMargin, *m_topRightMargin, *m_bottomLeftMargin, *m_bottomRightMargin;

    int m_extension {4}; // 向外扩展几个像素
    int m_marginWidth {8}; // 边框厚度
    QColor m_frameColor {107, 173, 246}; // 边框颜色
    Mode m_mode {NonPaintingMode}; // 模式

    QPoint m_lastEventPos; // 鼠标移动时，记录上次鼠标移动事件的位置(相对于父控件坐标系)
    QPoint m_virtualPointer; // 虚拟鼠标指针(相对于父控件坐标系)

Q_SIGNALS:
    void geometryChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit Selection(QWidget *parent = nullptr);

    [[nodiscard]] Mode mode() const;
    void setMode(Mode mode);

    [[nodiscard]] QRect basicGeometry() const;
    void setBasicGeometry(QRect rect);
    void setBasicGeometry(int x, int y, int w, int h);

    [[nodiscard]] QRect basicRect() const;
    [[nodiscard]] int basicWidth() const;
    [[nodiscard]] int basicHeight() const;

    void moveBasic(QPoint point);
    void moveBasic(int x, int y);

    [[nodiscard]] BasicSelection *basicSelection() const;
};
