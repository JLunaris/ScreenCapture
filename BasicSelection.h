#pragma once

#include <QWidget>

class BasicSelection : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit BasicSelection(QWidget *parent = nullptr);
};
