#include "UtilityPanel.h"
#include "CaptureWidget.h"
#include <QWidget>
#include <QToolButton>

UtilityPanel::UtilityPanel(CaptureWidget *parent)
        : QToolBar(parent)
{
//    setStyleSheet(R"(/* 按钮悬停/按下效果 */
//QToolBar QToolButton:hover,
//QToolBar QToolButton:pressed {
//    background: #e0e0e0;
//    border: 2px solid #a0a0a0;
//    border-radius: 4px;
//})");

    setStyleSheet(R"(
QToolBar {
    background-color: rgb(236, 240, 250);
}

QToolBar QToolButton:hover {
    border: 1px solid rgb(164, 164, 164)
}

    )");
    m_close->setIcon(QIcon{QPixmap{":/Close"}});
    m_copy->setIcon(QIcon{QPixmap{":/Finish"}});
    addWidget(m_close);
    addWidget(m_copy);
}
