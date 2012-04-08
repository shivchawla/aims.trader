#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Platform/typedefs.h>

class StrategyView;
class InstrumentView;
class OpenOrderView;
class PositionView;
class QSplitter;
class QMenuBar;
class QConsoleWidget;
class QAction;
class QTextEdit;
class DockWidget;
class QDialog;
class QMessageBox;

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

        PositionView* positionView;
        DockWidget* dockForPositionView;

        QTextEdit* messageView;
        DockWidget* dockForMessageView;

    private:
        QMenuBar* menuBar;
        //QConsoleWidget* console;

    private:
        //QSplitter* splitter;

    private:
        QMenu* windowMenu;
        QMenu* viewsMenu;
        QAction* minimize;
        QAction* instrumentViewDisplay;
        QAction* strategyViewDisplay;
        QAction* openOrderViewDisplay;
        QAction* messageViewDisplay;
        QAction* positionViewDisplay;

    private:
       // QMessageBox * messageBox;

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
        static MainWindow* mainWindow();

    public:
        StrategyView* getStrategyView();
        InstrumentView* getInstrumentView();
        OpenOrderView* getOpenOrderView();
        PositionView* getPositionView();

        //void dragMoveEvent(QDragMoveEvent * event);
        //void dropEvent(QDropEvent * event);
        //void dragEnterEvent(QDragEnterEvent * event);

    public:
        void closeEvent(QCloseEvent *);
        void stop();

    public slots:
        void onLog(const String&);

    private slots:
       // void minimize();
        void alterInstrumentView();
        void alterStrategyView();
        void alterOpenOrderView();
        void alterMessageView();
        void alterPositionView();

        //void hideInstrumentView();
        //void hideStrategyView();
        //void hideOpenOrderView();
        //void hideConsoleView();
};

#endif // MAINWINDOW_H
