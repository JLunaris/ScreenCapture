#pragma once

#include <QWidget>
#include <QPixmap>

class CaptureWindow : public QWidget
{
    QPixmap background;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit CaptureWindow(QPixmap &background,QWidget *parent = nullptr);
};

