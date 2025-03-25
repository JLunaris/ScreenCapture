#include "MainWindow.h"

#include <QSize>
#include <QIcon>
#include <QPalette>
#include <QHBoxLayout>
#include <QPushButton>
#include <QColor>
#include <QFrame>
#include <QGuiApplication>
#include <QScreen>
#include <QPixmap>
#include <QThread>
#include <chrono>

void MainWindow::captureScreen()
{
    // hide MainWindow
    hide();
    using namespace std::chrono_literals;
    QThread::sleep(150ms);

    // grab window
    QScreen *screen {QGuiApplication::primaryScreen()};
    QPixmap pixmap {screen->grabWindow()};

    // show captured window in full screen
    QLabel *label {new QLabel {nullptr, Qt::FramelessWindowHint}};
    label->setPixmap(pixmap);
    label->showFullScreen();


}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ctrlFrame->setGeometry(0, 0, centralWidget()->width(), 50);
    pictureFrame->setGeometry(0, 50, centralWidget()->width(), centralWidget()->height() - 50);
}


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
{
    setWindowTitle("截图工具");
    setMinimumSize(QSize {500, 200});
    setWindowIcon(QIcon {":/Icon"});
    setCentralWidget(new QWidget);

    // setFont
    [this] {
        QFont font {"Microsoft YaHei", 11};
        setFont(font);
    }();

    // setPalette
    [this] {
        QPalette palette;
        palette.setColor(QPalette::Active, QPalette::Window, QColor {248, 249, 252});
        palette.setColor(QPalette::Inactive, QPalette::Window, QColor {249, 249, 249});
        setPalette(palette);
    }();


    // CentralWidget
    [this] {
        // ctrlFrame
        [this] {
            ctrlFrame = new QFrame {this->centralWidget()};

            // setPalette
            [this] {
                QPalette framePalette;
                framePalette.setColor(QPalette::Active, QPalette::Window, QColor {238, 244, 249});
                framePalette.setColor(QPalette::Inactive, QPalette::Window, QColor {243, 243, 243});
                ctrlFrame->setPalette(framePalette);
                ctrlFrame->setAutoFillBackground(true);
            }();

            // hBoxLayout
            [this] {
                QHBoxLayout *hBoxLayout {new QHBoxLayout {ctrlFrame}};
                snipButton = new QPushButton {"创建截图"};
                hBoxLayout->addWidget(snipButton, 1);
                snipButton->setStyleSheet(R"(
                    QPushButton {
                            background-color: white;
                            border: 1px solid rgb(220, 225, 229);
                            border-radius: 8px;            /* 圆角半径 */
                            padding: 5px 10px;             /* 内边距（上下 5px，左右 10px） */
                        }

                        /* 鼠标悬停效果 */
                        QPushButton:hover {
                            background-color: rgb(245, 245, 245);
                        }

                        /* 按钮按下效果 */
                        QPushButton:pressed {
                            background-color: rgb(230, 230, 230);
                            border: 1px solid rgb(200, 200, 200);
                        }
                )");
                snipButton->setFont(QFont {"微软雅黑"});
            }();
        }();

        pictureFrame = new QFrame {this->centralWidget()};
        pictureFrame->setFrameStyle(QFrame::Shape::Box);
    }();

    connect(snipButton, &QPushButton::clicked, this, &MainWindow::captureScreen);
}
