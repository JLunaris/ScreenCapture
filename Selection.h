#pragma once

#include <QWidget>

class Selection : public QWidget
{



protected:
    void paintEvent(QPaintEvent *event) override;
public:
    explicit Selection(QWidget *parent = nullptr);
};

