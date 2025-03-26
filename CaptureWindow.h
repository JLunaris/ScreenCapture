#pragma once

#include <QWidget>
#include <QPixmap>
#include <QRubberBand>
#include <QRect>

class CaptureWindow : public QWidget
{
    QPixmap background;
    QRubberBand *rubberBand {new QRubberBand {QRubberBand::Rectangle, this}};
    QRect rubberBandRect;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit CaptureWindow(QPixmap &&background, QWidget *parent = nullptr);
};

