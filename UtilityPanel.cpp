#include "UtilityPanel.h"
#include "CaptureWidget.h"
#include <QWidget>
#include <QToolButton>
#include <QRect>

UtilityPanel::UtilityPanel(CaptureWidget *parent)
        : QToolBar(parent)
{
    setStyleSheet(R"(
        QToolBar {
            background-color: rgb(236, 240, 250);
        }

        QToolBar QToolButton:hover {
            border: 1px solid rgb(164, 164, 164)
        }
    )");

    setIconSize(QSize {20, 24});

    m_rectangle->setIcon(QIcon {QPixmap {":/Rectangle"}});
    addWidget(m_rectangle);

    m_ellipse->setIcon(QIcon {QPixmap {":/Ellipse"}});
    addWidget(m_ellipse);

    m_horizontalLine->setIcon(QIcon {QPixmap {":/HLine"}});
    addWidget(m_horizontalLine);

    m_pencil->setIcon(QIcon {QPixmap {":/Pencil"}});
    addWidget(m_pencil);

    m_redo->setIcon(QIcon {QPixmap {":/Redo"}});
    addWidget(m_redo);

    m_save->setIcon(QIcon {QPixmap {":/Save"}});
    addWidget(m_save);

    m_close->setIcon(QIcon {QPixmap {":/Close"}});
    addWidget(m_close);

    m_copy->setIcon(QIcon {QPixmap {":/Finish"}});
    addWidget(m_copy);
}

void UtilityPanel::safelyMove(QPointF point)
{
    QRectF parentRect {parentWidget()->rect()};

    // 禁止越界
    if (point.x() < 0) { point.setX(0); }
    if (point.x() + width() > parentRect.right()) { point.setX(parentRect.right() - width()); }
    if (point.y() < 0) { point.setY(0); }
    if (point.y() + height() > parentRect.bottom()) { point.setY(parentRect.bottom() - height()); }

    move(point.toPoint());
}