#pragma once

#include <QWidget>
#include <QPushButton>

class CaptureWidget; // forward declaration

class UtilityPanel : public QWidget
{
public:
    explicit UtilityPanel(CaptureWidget *parent);
};

