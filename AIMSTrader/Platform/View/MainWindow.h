#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Platform/typedefs.h>

class StrategyView;
class InstrumentView;
class OpenOrderView;
<<<<<<< HEAD
class PositionView;
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
class QSplitter;
class QMenuBar;
class QConsoleWidget;
class QAction;
class QTextEdit;
class DockWidget;
<<<<<<< HEAD
class QDialog;
class QMessageBox;
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

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
<<<<<<< HEAD

        PositionView* positionView;
        DockWidget* dockForPositionView;

        QTextEdit* messageView;
        DockWidget* dockForMessageView;
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

    private:
        QMenuBar* menuBar;
        //QConsoleWidget* console;
<<<<<<< HEAD
=======
        QTextEdit* console;
        DockWidget* dockForConsole;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

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
<<<<<<< HEAD
        static MainWindow* mainWindow();
=======
        static MainWindow* mainWindow()
        {
            if(!_mainWindow)
            {
                _mainWindow=new MainWindow();
                _mainWindow->show();
            }
            return _mainWindow;
        }
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

    public:
        StrategyView* getStrategyView();
        InstrumentView* getInstrumentView();
        OpenOrderView* getOpenOrderView();
<<<<<<< HEAD
        PositionView* getPositionView();
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

        //void dragMoveEvent(QDragMoveEvent * event);
        //void dropEvent(QDropEvent * event);
        //void dragEnterEvent(QDragEnterEvent * event);

<<<<<<< HEAD
    public:
        void closeEvent(QCloseEvent *);
        void stop();

    public slots:
        void onLog(const String&);
=======
    public slots:
        void onLog(const QString&);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

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
