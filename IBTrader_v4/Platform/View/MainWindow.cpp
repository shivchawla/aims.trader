#include "Platform/View/MainWindow.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/StrategyView.h"
#include <QSplitter>
#include <QMenuBar>
#include "Platform/Utils/qconsolewidget.h"
#include <QDockWidget>
#include <QAction>

MainWindow* MainWindow::_mainWindow = NULL;

MainWindow::MainWindow():QMainWindow()
{
    init();
}

void MainWindow::init()
{
    resize(1300,1024);

    dockForInstrumentView = new QDockWidget("InstrumentView", this);
    instrumentView = new InstrumentView(dockForInstrumentView);
    dockForInstrumentView->show();
    dockForInstrumentView->setWidget(instrumentView);
    dockForInstrumentView->move(0,0);
    dockForInstrumentView->setAllowedAreas(Qt::NoDockWidgetArea);

    dockForStrategyView = new QDockWidget("StrategyView", this);
    strategyView = new StrategyView(dockForStrategyView);
    dockForStrategyView->setWidget(strategyView);
    dockForStrategyView->show();
    dockForStrategyView->move(0,250);
    dockForStrategyView->setAllowedAreas(Qt::NoDockWidgetArea);

    //instrumentView->show();
    //strategyView->show();
    dockForConsole = new QDockWidget(this);
    console = new QConsoleWidget(dockForConsole);
    console->setMinimumSize(400,200);
    dockForConsole->setWidget(console);
    dockForConsole->move(0,500);
    dockForConsole->setAllowedAreas(Qt::NoDockWidgetArea);
    dockForConsole->show();

    setupMenu();

    //setLayout(gridLayout);
   // instrumentView->show();
    instrumentView->show();
    strategyView->show();
    console->show();

    show();
}

void MainWindow::setupMenu()
{
    windowMenu = new QMenu("&Window");
    minimize = new QAction("&Minimize",this);

    windowMenu->addAction(minimize);

    instrumentViewDisplay = new QAction("&Instruments",this);
    strategyViewDisplay = new QAction("&Strategy",this);
    openOrderViewDisplay = new QAction("&OpenOrders",this);
    consoleViewDisplay = new QAction("&Console",this);
    instrumentViewDisplay->setCheckable(true);
    strategyViewDisplay->setCheckable(true);
    openOrderViewDisplay->setCheckable(true);
    consoleViewDisplay->setCheckable(true);
    instrumentViewDisplay->setChecked(true);
    strategyViewDisplay->setChecked(true);
    openOrderViewDisplay->setChecked(true);
    consoleViewDisplay->setChecked(true);

    viewsMenu = new QMenu("&Views");

    windowMenu->addSeparator();
    windowMenu->addMenu(viewsMenu);

    viewsMenu->addAction(instrumentViewDisplay);
    viewsMenu->addAction(strategyViewDisplay);
    viewsMenu->addAction(openOrderViewDisplay);
    viewsMenu->addAction(consoleViewDisplay);
    //viewsMenu->addAction(minimize);

    menuBar = new QMenuBar();
    menuBar->addMenu(windowMenu);

     connect(minimize, SIGNAL(triggered()), this, SLOT(showMinimized()));
     connect(instrumentViewDisplay,SIGNAL(triggered()), this, SLOT(alterInstrumentView()));
     connect(strategyViewDisplay,SIGNAL(triggered()), this, SLOT(alterStrategyView()));
     connect(openOrderViewDisplay,SIGNAL(triggered()), this, SLOT(alterOpenOrderView()));
     connect(consoleViewDisplay,SIGNAL(triggered()),this,SLOT(alterConsoleView()));
     //connect(instrumentViewDisplay,SIGNAL(triggered()), instrumentView, SLOT(switchView()));
}

MainWindow::~MainWindow()
{
    delete strategyView;
    delete instrumentView;
    delete splitter;
}

InstrumentView* MainWindow::getInstrumentView()
{
    return instrumentView;
}

StrategyView* MainWindow::getStrategyView()
{
    return strategyView;
}

void MainWindow::setUpMainWindow()
{
    if(!_mainWindow)
    {
        _mainWindow = new MainWindow();
    }
}

/*void MainWindow::minimize()
{
    showMinimized();
}*/

void MainWindow::alterInstrumentView()
{
    if(dockForInstrumentView->isHidden())
    {
     //instrumentView->show();
     dockForInstrumentView->show();
     instrumentViewDisplay->setChecked(true);
    }
    else
    {
        //instrumentView->hide();
        dockForInstrumentView->hide();
        instrumentViewDisplay->setChecked(false);
    }

}

void MainWindow::alterStrategyView()
{
    if(dockForStrategyView->isHidden())
    {
        dockForStrategyView->show();
        strategyViewDisplay->setChecked(true);
    }
    else
    {
        dockForStrategyView->hide();
        strategyViewDisplay->setChecked(false);
    }
}

void MainWindow::alterOpenOrderView()
{
    /*if(dockForOpenOrderView->isHidden())
    {
        dockForOpenOrderView->show();
        openOrderViewDisplay->setChecked(true);
    }
    else
    {
        dockForOpenOrderView->hide();
        openOrderViewDisplay->setChecked(true);
    }*/
}

void MainWindow::alterConsoleView()
{
    if(dockForConsole->isHidden())
    {
        dockForConsole->show();
        consoleViewDisplay->setChecked(true);
    }
    else
    {
        dockForConsole->hide();
        consoleViewDisplay->setChecked(false);

    }
}

/*void MainWindow::hideInstrumentView()
{
    instrumentView->hide();
}

void MainWindow::hideStrategyView()
{
    strategyView->hide();
}

void MainWindow::hideOpenOrderView()
{
    //openOrderView->hide();
}

void MainWindow::hideConsoleView()
{
    console->hide();
}
*/

