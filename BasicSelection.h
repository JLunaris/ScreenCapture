#pragma once

#include <QWidget>

class BasicSelection : public QWidget
{
protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit BasicSelection(QWidget *parent = nullptr);
};
