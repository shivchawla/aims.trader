#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class StrategyView;
class InstrumentView;
class OpenOrderView;
class QSplitter;
class QMenuBar;
class QConsoleWidget;
class QAction;
class QTextEdit;
class DockWidget;

class MainWindow: public QMainWindow
{
    Q_OBJECT
    private :
        static MainWindow* _mainWindow;

    private:
        StrategyView* strategyView;
        DockWidget* dockForStrategyView;

        InstrumentView* instrumentView;
        DockWidget* dockForInstrumentView;

        OpenOrderView* openOrderView;
        DockWidget* dockForOpenOrderView;

    private:
        QMenuBar* menuBar;
        //QConsoleWidget* console;
        QTextEdit* console;
        DockWidget* dockForConsole;

    private:
        //QSplitter* splitter;

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
                _mainWindow->show();
            }
            return _mainWindow;
        }

    public:
        StrategyView* getStrategyView();
        InstrumentView* getInstrumentView();
        OpenOrderView* getOpenOrderView();

        //void dragMoveEvent(QDragMoveEvent * event);
        //void dropEvent(QDropEvent * event);
        //void dragEnterEvent(QDragEnterEvent * event);

    public slots:
        void onLog(const QString&);

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
