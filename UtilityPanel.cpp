#include "UtilityPanel.h"
#include "CaptureWidget.h"
#include <QWidget>

UtilityPanel::UtilityPanel(CaptureWidget *parent)
        : QWidget(parent)
{
    setStyleSheet("background-color: white;");
}
