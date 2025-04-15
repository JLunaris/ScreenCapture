#pragma once

#include <QWidget>
#include <QToolBar>
#include <QToolButton>

class CaptureWidget; // forward declaration

class UtilityPanel : public QToolBar
{
    QToolButton *m_close {new QToolButton {this}};
    QToolButton *m_copy {new QToolButton {this}};

public:
    explicit UtilityPanel(CaptureWidget *parent);
};

