#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Platform/typedefs.h>
#include "Platform/Utils/Singleton.h"
#include "Platform/View/SubMainWindow.h"

class MessageView;
class StrategyView;
class InstrumentView;
class OpenOrderView;
class OpenOrderWidget;
class StrategyPositionView;
class QSplitter;
class QMenuBar;
class QConsoleWidget;
class QAction;
class QTextEdit;
class SpecialDockWidget;
class QDialog;
class QMessageBox;
class QSignalMapper;

class MainWindow: public QMainWindow, public Singleton<MainWindow>
{
    friend class Singleton<MainWindow>;
    Q_OBJECT
    private:
        StrategyView* _strategyView;
        SpecialDockWidget* _dockForStrategyView;

        InstrumentView* _instrumentView;
        SpecialDockWidget* _dockForInstrumentView;

        OpenOrderWidget* _openOrderView;
        SpecialDockWidget* _dockForOpenOrderView;

        StrategyPositionView* _positionView;
        SpecialDockWidget* _dockForPositionView;

        MessageView* _messageView;
        SpecialDockWidget* _dockForMessageView;

        QMenu* _dockWidgetMenu;
        QMenu* _mainWindowMenu;
        QSignalMapper* _mapper;
        QList<QDockWidget*> _extraDockWidgets;
        QAction* _createDockWidgetAction;
        QMenu* _destroyDockWidgetMenu;

    private:
        QMenuBar* _pMenuBar;

    private:
        QMenu* _windowMenu;
        QMenu* _viewsMenu;
        QAction* _minimize;
        QAction* _instrumentViewDisplay;
        QAction* _strategyViewDisplay;
        QAction* _openOrderViewDisplay;
        QAction* _messageViewDisplay;
        QAction* _positionViewDisplay;

    public:
        MainWindow(QWidget* parent=0);
        void init();

    public:
        void setup(const QMap<QString, QSize> &customSizeHints);

    private:
        void setupMenu();

    public:
        ~MainWindow();

    public:
        StrategyView* getStrategyView();
        InstrumentView* getInstrumentView();
        OpenOrderWidget* getOpenOrderView();
        StrategyPositionView* getPositionView();
        MessageView* getMessageView();

    public:
        void closeEvent(QCloseEvent *);
        void stop();

    private slots:
        void alterInstrumentView();
        void alterStrategyView();
        void alterOpenOrderView();
        void alterMessageView();
        void alterPositionView();

    protected:
        void showEvent(QShowEvent *event);

    public slots:
        void actionTriggered(QAction *action);
        void saveLayout();
        void loadLayout();
        void setCorner(int id);
        void switchLayoutDirection();
        void setDockOptions();

        void createDockWidget();
        void destroyDockWidget(QAction *action);

    private:
        //void setupToolBar();
        void setupMenuBar();
        void setupDockWidgets(const QMap<QString, QSize> &customSizeHints);


};

#endif // MAINWINDOW_H
