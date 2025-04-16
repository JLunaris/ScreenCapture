#pragma once

#include <QWidget>
#include <QToolBar>
#include <QToolButton>
#include <QPointF>

class CaptureWidget; // forward declaration

class UtilityPanel : public QToolBar
{
    Q_OBJECT

    QToolButton *m_rectangle {new QToolButton {this}};
    QToolButton *m_ellipse {new QToolButton {this}};
    QToolButton *m_horizontalLine {new QToolButton {this}};
    QToolButton *m_pencil {new QToolButton {this}};
    QToolButton *m_redo {new QToolButton {this}};
    QToolButton *m_save {new QToolButton {this}};
    QToolButton *m_close {new QToolButton {this}};
    QToolButton *m_copy {new QToolButton {this}};

public:
    explicit UtilityPanel(CaptureWidget *parent);
    void safelyMove(QPointF point);
};

