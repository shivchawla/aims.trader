#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class StrategyView;
class InstrumentView;
class QSplitter;
class QMenuBar;
class QConsoleWidget;
class QAction;

class MainWindow: public QMainWindow
{
    Q_OBJECT
    private :
        static MainWindow* _mainWindow;

    private:
        StrategyView* strategyView;
        QDockWidget* dockForStrategyView;

        InstrumentView* instrumentView;
        QDockWidget* dockForInstrumentView;

    private:
        QMenuBar* menuBar;
        QConsoleWidget* console;
        QDockWidget* dockForConsole;

    private:
        QSplitter* splitter;

    private:
        QMenu* windowMenu;
        QMenu* viewsMenu;
        QAction* minimize;
        QAction* instrumentViewDisplay;
        QAction* strategyViewDisplay;
        QAction* openOrderViewDisplay;
        QAction* consoleViewDisplay;

    private:
        MainWindow();
        void init();

    public:
        void static setUpMainWindow();

    private:
        void setupMenu();

    public:
        ~MainWindow();

    public:
        static MainWindow* mainWindow()
        {
            if(!_mainWindow)
            {
                _mainWindow=new MainWindow();
            }
            return _mainWindow;
        }

    public:
        StrategyView* getStrategyView();
        InstrumentView* getInstrumentView();

        //void dragMoveEvent(QDragMoveEvent * event);
        //void dropEvent(QDropEvent * event);
        //void dragEnterEvent(QDragEnterEvent * event);

    private slots:
       // void minimize();
        void alterInstrumentView();
        void alterStrategyView();
        void alterOpenOrderView();
        void alterConsoleView();
        //void hideInstrumentView();
        //void hideStrategyView();
        //void hideOpenOrderView();
        //void hideConsoleView();
};

#endif // MAINWINDOW_H
