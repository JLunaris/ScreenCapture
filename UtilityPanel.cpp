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

        QToolBar QToolButton {
            border: none;
            padding-left: 2px;
            padding-right: 1px;
        }  /* 关键：取消按下按钮后的下沉效果 */

        QToolBar QToolButton:hover {
            border: 1px solid rgb(164, 164, 164)
        }

        QToolBar QToolButton:checked,
        QToolBar QToolButton:pressed {
            border: 1px solid rgb(164, 164, 164);
            background-color: rgb(210, 215, 225);
        }
    )");

    setIconSize(QSize {20, 24});

    // 配置按钮
    {
        BasicSelection *basicSelection {parent->m_selection->basicSelection()};
        Selection *selection {parent->m_selection};

        m_rectangle->setIcon(QIcon {QPixmap {":/Rectangle"}});
        m_rectangle->setCheckable(true);
        addWidget(m_rectangle);
        connect(m_rectangle, &QToolButton::clicked,
                this, [basicSelection, selection, this]() {
                    basicSelection->setPaintingMode(BasicSelection::PaintingMode::Rectangle);
                    selection->setMode(Selection::PaintingMode);
                    m_rectangle->setChecked(true);
                    m_ellipse->setChecked(false);
                    m_line->setChecked(false);
                    m_pencil->setChecked(false);
                });

        m_ellipse->setIcon(QIcon {QPixmap {":/Ellipse"}});
        m_ellipse->setCheckable(true);
        addWidget(m_ellipse);
        connect(m_ellipse, &QToolButton::clicked,
                this, [basicSelection, selection, this]() {
                    basicSelection->setPaintingMode(BasicSelection::PaintingMode::Ellipse);
                    selection->setMode(Selection::PaintingMode);
                    m_rectangle->setChecked(false);
                    m_ellipse->setChecked(true);
                    m_line->setChecked(false);
                    m_pencil->setChecked(false);
                });

        m_line->setIcon(QIcon {QPixmap {":/Line"}});
        m_line->setCheckable(true);
        addWidget(m_line);
        connect(m_line, &QToolButton::clicked,
                this, [basicSelection, selection, this]() {
                    basicSelection->setPaintingMode(BasicSelection::PaintingMode::Line);
                    selection->setMode(Selection::PaintingMode);
                    m_rectangle->setChecked(false);
                    m_ellipse->setChecked(false);
                    m_line->setChecked(true);
                    m_pencil->setChecked(false);
                });

        m_pencil->setIcon(QIcon {QPixmap {":/Pencil"}});
        m_pencil->setCheckable(true);
        addWidget(m_pencil);
        connect(m_pencil, &QToolButton::clicked,
                this, [basicSelection, selection, this]() {
                    basicSelection->setPaintingMode(BasicSelection::PaintingMode::Pencil);
                    selection->setMode(Selection::PaintingMode);
                    m_rectangle->setChecked(false);
                    m_ellipse->setChecked(false);
                    m_line->setChecked(false);
                    m_pencil->setChecked(true);
                });

        m_redo->setIcon(QIcon {QPixmap {":/Redo"}});
        addWidget(m_redo);
        connect(m_redo, &QToolButton::clicked,
                basicSelection, &BasicSelection::removeItem);

        m_save->setIcon(QIcon {QPixmap {":/Save"}});
        addWidget(m_save);

        m_close->setIcon(QIcon {QPixmap {":/Close"}});
        addWidget(m_close);
        connect(m_close, &QToolButton::clicked,
                parent, &CaptureWidget::backToMainWindow);

        m_copy->setIcon(QIcon {QPixmap {":/Finish"}});
        addWidget(m_copy);
        connect(m_copy, &QToolButton::clicked,
                parent, &CaptureWidget::renderCaptureImage);
        connect(m_copy, &QToolButton::clicked,
                parent, &CaptureWidget::copyImageToClipboard);
        connect(m_copy, &QToolButton::clicked,
                parent, &CaptureWidget::backToMainWindow);
    }
}

CaptureWidget *UtilityPanel::parentCaptureWidget() const
{
    return static_cast<CaptureWidget *>(parentWidget());
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