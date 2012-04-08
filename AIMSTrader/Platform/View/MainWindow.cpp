#include "Platform/View/MainWindow.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/StrategyView.h"
#include "Platform/View/OpenOrderView.h"

#include <QSplitter>
#include <QMenuBar>
#include "Platform/Utils/qconsolewidget.h"
#include <QDockWidget>
#include <QAction>
#include <QTextEdit>
#include "Platform/View/DockWidget.h"

MainWindow* MainWindow::_mainWindow = NULL;

MainWindow::MainWindow():QMainWindow()
{
    init();
}

void MainWindow::init()
{
    resize(1300,1024);

    dockForInstrumentView = new DockWidget("Market Data", this);
    instrumentView = new InstrumentView(dockForInstrumentView);
    dockForInstrumentView->show();
    dockForInstrumentView->setWidget(instrumentView);
    dockForInstrumentView->move(0,0);
    dockForInstrumentView->setAllowedAreas(Qt::NoDockWidgetArea);

    dockForStrategyView = new DockWidget("Strategy", this);
    strategyView = new StrategyView(dockForStrategyView);
    dockForStrategyView->setWidget(strategyView);
    dockForStrategyView->show();
    dockForStrategyView->move(0,250);
    dockForStrategyView->setAllowedAreas(Qt::NoDockWidgetArea);

    dockForConsole = new DockWidget("Messages",this);
    console = new QTextEdit(dockForConsole);
    console->setMinimumSize(400,200);
    console->setReadOnly(true);
    dockForConsole->setWidget(console);
    dockForConsole->move(0,500);
    dockForConsole->setAllowedAreas(Qt::NoDockWidgetArea);
    dockForConsole->show();

    dockForOpenOrderView = new DockWidget("Open Orders", this);
    openOrderView = new OpenOrderView(dockForOpenOrderView);
    dockForOpenOrderView->setWidget(openOrderView);
    dockForOpenOrderView->show();
    dockForOpenOrderView->move(220,500);
    dockForOpenOrderView->setAllowedAreas(Qt::NoDockWidgetArea);


    setupMenu();

    //setLayout(gridLayout);
   // instrumentView->show();
    instrumentView->show();
    strategyView->show();
    console->show();
    openOrderView->show();

    //show();
}

void MainWindow::setupMenu()
{
    windowMenu = new QMenu("&Window");
    minimize = new QAction("&Minimize",this);

    windowMenu->addAction(minimize);

    instrumentViewDisplay = new QAction("&Instruments",this);
    connect(dockForInstrumentView,SIGNAL(visibilityChanged(bool)),instrumentViewDisplay,SLOT(setChecked(bool)));

    strategyViewDisplay = new QAction("&Strategy",this);
    connect(dockForStrategyView,SIGNAL(visibilityChanged(bool)),strategyViewDisplay,SLOT(setChecked(bool)));

    openOrderViewDisplay = new QAction("&OpenOrders",this);
    connect(dockForOpenOrderView,SIGNAL(visibilityChanged(bool)),openOrderViewDisplay,SLOT(setChecked(bool)));

    consoleViewDisplay = new QAction("&Console",this);
    connect(dockForConsole,SIGNAL(visibilityChanged(bool)),consoleViewDisplay,SLOT(setChecked(bool)));

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
    //connect(instrumentView,SIGNAL(closed()),this,SLOT(alterInstrumentView()));
    //connect(strategyView,SIGNAL(closed()),this,SLOT(alterStrategyView()));
    //connect(openOrderView,SIGNAL(closed()),this,SLOT(alterOpenOrderView()));
}


MainWindow::~MainWindow()
{
    delete strategyView;
    delete instrumentView;
    //delete openOrderView;
    delete console;
    delete openOrderViewDisplay;
    delete instrumentViewDisplay;
    delete strategyViewDisplay;
    delete consoleViewDisplay;
    //delete splitter;
}


InstrumentView* MainWindow::getInstrumentView()
{
    return instrumentView;
}

StrategyView* MainWindow::getStrategyView()
{
    return strategyView;
}

OpenOrderView* MainWindow::getOpenOrderView()
{
    return openOrderView;
}


void MainWindow::setUpMainWindow()
{
    if(!_mainWindow)
    {
        _mainWindow = new MainWindow();
    }
}

void MainWindow::onLog(const QString& output)
{
    console->setText(output);
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
    if(dockForOpenOrderView->isHidden())
    {
        dockForOpenOrderView->show();
        openOrderViewDisplay->setChecked(true);
    }
    else
    {
        dockForOpenOrderView->hide();
        openOrderViewDisplay->setChecked(true);
    }
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
