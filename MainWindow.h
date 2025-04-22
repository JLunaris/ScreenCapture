#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPushButton *m_snipButton;
    QFrame *m_ctrlFrame;
    QFrame *m_pictureFrame;

private Q_SLOTS:
    void captureScreen();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
};
