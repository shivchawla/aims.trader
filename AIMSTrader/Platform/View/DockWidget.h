#pragma once
#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QDockWidget>

class DockWidget: public QDockWidget
{
    public:
        DockWidget(QWidget* parent);
        ~DockWidget();
        DockWidget(const QString&, QWidget* parent=0);

    public:
        //void closeEvent(QCloseEvent *event);
};

#endif // DOCKWIDGET_H
