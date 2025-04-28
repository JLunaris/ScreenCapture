#include "MainWindow.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <print>

class TestMainWindow : public QMainWindow
{
public:
    explicit TestMainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
            : QMainWindow(parent, flags)
    {
        QGraphicsScene *scene {new QGraphicsScene(0, 0, 200, 200, this)};
        scene->addEllipse(0, 0, 200, 200, QPen(Qt::NoPen), QBrush {QColor {Qt::blue}});
        std::println("{}  {}", scene->sceneRect().width(), scene->sceneRect().height());
        QGraphicsView *view {new QGraphicsView {scene, this}};

        view->resize(200, 200);
        view->setRenderHint(QPainter::RenderHint::Antialiasing); // 关键：设置view渲染时抗锯齿
        view->setFrameShape(QFrame::NoFrame); // 关键：移除边框
        std::println("viewport: {}", view->viewport()->autoFillBackground());
        view->viewport()->setAutoFillBackground(false); // 关键：将视口的autoFillBackground设为false
        auto x {view->viewport()->palette().color(QPalette::ColorRole::Window)};
        std::println("{} {} {}", x.red(), x.green(), x.blue());
        std::println("{}", view->frameWidth());
    }
};


int main(int argc, char *argv[])
{
    QApplication app {argc, argv};
    MainWindow win;
    win.show();

//    TestMainWindow win;
//    win.showMaximized();

    return QApplication::exec();
}
