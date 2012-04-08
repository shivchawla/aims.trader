#include "Platform/View/DockWidget.h"
#include <QDockWidget>

DockWidget::DockWidget(QWidget* parent):QDockWidget(parent)
{}

DockWidget::~DockWidget()
{}

/*void DockWidget::closeEvent()
{
    hide();
}*/

DockWidget::DockWidget(const QString& title, QWidget* parent):QDockWidget(title, parent)
{}
