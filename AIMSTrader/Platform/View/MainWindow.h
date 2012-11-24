#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <AimsTraderDefs/typedefs.h>
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
class StrategyPositionView3;
class QTableView;
class StrategyPositionModel2;
class OpenOrderView2;
class StrategyView2;
class StrategyView3;
class InstrumentViewSubscriber;
class InstrumentViewWidget;

class MainWindow: public QMainWindow//, public Singleton<MainWindow>
{
    //friend class Singleton<MainWindow>;
    Q_OBJECT
    private:
        StrategyView3* _strategyView;
        SpecialDockWidget* _dockForStrategyView;

        InstrumentViewWidget* _instrumentViewWidget;
        SpecialDockWidget* _dockForInstrumentView;

        OpenOrderWidget* _openOrderView;
        SpecialDockWidget* _dockForOpenOrderView;

        //StrategyPositionView* _positionView;
        //SpecialDockWidget* _dockForPositionView;

        StrategyPositionView3* _positionView;
        SpecialDockWidget* _dockForPositionView;

//        OpenOrderView2* _orderView2;
//        SpecialDockWidget* _dockForOrderView2;

        MessageView* _messageView;
        SpecialDockWidget* _dockForMessageView;

        QMenu* _dockWidgetMenu;
        QMenu* _mainWindowMenu;
        QSignalMapper* _mapper;
        QList<QDockWidget*> _extraDockWidgets;
        QAction* _createDockWidgetAction;
        QMenu* _destroyDockWidgetMenu;
        static MainWindow* _instance;
    private:
        //QMenuBar* _pMenuBar;

    private:
//        QMenu* _windowMenu;
//        QMenu* _viewsMenu;
//        QAction* _minimize;
//        QAction* _instrumentViewDisplay;
//        QAction* _strategyViewDisplay;
//        QAction* _openOrderViewDisplay;
//        QAction* _messageViewDisplay;
//        QAction* _positionViewDisplay;

    public:
        MainWindow(QWidget* parent=0);
        void init();

    public:
        void setup(const QMap<QString, QSize> &customSizeHints);

    private:
        void setupMenu();

    public:
       //~MainWindow();

    public:
        StrategyView3* getStrategyView();
        InstrumentViewWidget* getInstrumentView();
        OpenOrderWidget* getOpenOrderView();
        StrategyPositionView3* getPositionView();
        //StrategyPositionView2* getPositionView2();

        MessageView* getMessageView();
        static MainWindow& mainWindow()
        {
            if(_instance == NULL)
            {
                _instance = new MainWindow();
            }
            return *_instance;
        }


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

//extern MainWindow& mainWindow();

#endif // MAINWINDOW_H
