#include "Platform/View/MainWindow.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/StrategyView.h"
#include "Platform/View/OpenOrderView.h"
<<<<<<< HEAD
#include "Platform/View/PositionView.h"
#include "Platform/Startup/Service.h"
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

#include <QSplitter>
#include <QMenuBar>
#include "Platform/Utils/qconsolewidget.h"
#include <QDockWidget>
#include <QAction>
#include <QTextEdit>
<<<<<<< HEAD
#include <QDialog>
#include <QMessageBox>
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
#include "Platform/View/DockWidget.h"

MainWindow* MainWindow::_mainWindow = NULL;

MainWindow* MainWindow::mainWindow()
{
    if(!_mainWindow)
    {
        _mainWindow = new MainWindow();
        _mainWindow->show();
    }
    return _mainWindow;
}

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

<<<<<<< HEAD
    dockForMessageView = new DockWidget("Messages",this);
    messageView = new QTextEdit(dockForMessageView);
    messageView->setMinimumSize(1000,200);
    messageView->setReadOnly(true);
    dockForMessageView->setWidget(messageView);
    dockForMessageView->move(0,500);
    dockForMessageView->setAllowedAreas(Qt::NoDockWidgetArea);

    dockForOpenOrderView = new DockWidget("Open Orders", this);
    openOrderView = new OpenOrderView(dockForOpenOrderView);
    dockForOpenOrderView->setWidget(openOrderView);
    dockForOpenOrderView->show();
    dockForOpenOrderView->move(220,200);
    dockForOpenOrderView->setAllowedAreas(Qt::NoDockWidgetArea);

    dockForPositionView = new DockWidget("Positions", this);
    positionView = new PositionView(dockForPositionView);
    dockForPositionView->setWidget(positionView);
    dockForPositionView->show();
    dockForPositionView->move(220,500);
    dockForPositionView->setAllowedAreas(Qt::NoDockWidgetArea);

=======
    dockForConsole = new DockWidget("Messages",this);
    console = new QTextEdit(dockForConsole);
    console->setMinimumSize(400,200);
    console->setReadOnly(true);
    dockForConsole->setWidget(console);
    dockForConsole->move(0,500);
    dockForConsole->setAllowedAreas(Qt::NoDockWidgetArea);
    dockForConsole->show();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

    dockForOpenOrderView = new DockWidget("Open Orders", this);
    openOrderView = new OpenOrderView(dockForOpenOrderView);
    dockForOpenOrderView->setWidget(openOrderView);
    dockForOpenOrderView->show();
    dockForOpenOrderView->move(220,500);
    dockForOpenOrderView->setAllowedAreas(Qt::NoDockWidgetArea);


    setupMenu();

    //setLayout(gridLayout);
   // instrumentView->show();
<<<<<<< HEAD
    dockForInstrumentView->show();
    dockForStrategyView->show();
    dockForMessageView->show();
    dockForOpenOrderView->hide();
    dockForPositionView->show();
=======
    instrumentView->show();
    strategyView->show();
    console->show();
    openOrderView->show();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

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

<<<<<<< HEAD
    messageViewDisplay = new QAction("&Messages",this);
    connect(dockForMessageView,SIGNAL(visibilityChanged(bool)),messageViewDisplay,SLOT(setChecked(bool)));

    positionViewDisplay = new QAction("&Positions",this);
    connect(dockForPositionView,SIGNAL(visibilityChanged(bool)),positionViewDisplay,SLOT(setChecked(bool)));
=======
    consoleViewDisplay = new QAction("&Console",this);
    connect(dockForConsole,SIGNAL(visibilityChanged(bool)),consoleViewDisplay,SLOT(setChecked(bool)));
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

    instrumentViewDisplay->setCheckable(true);
    strategyViewDisplay->setCheckable(true);
    openOrderViewDisplay->setCheckable(true);
    messageViewDisplay->setCheckable(true);
    positionViewDisplay->setCheckable(true);

    //instrumentViewDisplay->setChecked(true);
    //strategyViewDisplay->setChecked(true);
    //openOrderViewDisplay->setChecked(true);
    //messageViewDisplay->setChecked(true);
    //positionViewDisplay->setChecked(true);

    viewsMenu = new QMenu("&Views");

    windowMenu->addSeparator();
    windowMenu->addMenu(viewsMenu);

    viewsMenu->addAction(instrumentViewDisplay);
    viewsMenu->addAction(strategyViewDisplay);
    viewsMenu->addAction(openOrderViewDisplay);
    viewsMenu->addAction(messageViewDisplay);
    viewsMenu->addAction(positionViewDisplay);
    //viewsMenu->addAction(minimize);

    menuBar = new QMenuBar();
    menuBar->addMenu(windowMenu);

    connect(minimize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(instrumentViewDisplay,SIGNAL(triggered()), this, SLOT(alterInstrumentView()));
    connect(strategyViewDisplay,SIGNAL(triggered()), this, SLOT(alterStrategyView()));
    connect(openOrderViewDisplay,SIGNAL(triggered()), this, SLOT(alterOpenOrderView()));
<<<<<<< HEAD
    connect(messageViewDisplay,SIGNAL(triggered()), this,SLOT(alterMessageView()));
    connect(positionViewDisplay,SIGNAL(triggered()), this,SLOT(alterPositionView()));

=======
    connect(consoleViewDisplay,SIGNAL(triggered()),this,SLOT(alterConsoleView()));
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    //connect(instrumentView,SIGNAL(closed()),this,SLOT(alterInstrumentView()));
    //connect(strategyView,SIGNAL(closed()),this,SLOT(alterStrategyView()));
    //connect(openOrderView,SIGNAL(closed()),this,SLOT(alterOpenOrderView()));
}


MainWindow::~MainWindow()
{
    delete strategyView;
    delete instrumentView;
<<<<<<< HEAD
    delete openOrderView;
    delete messageView;
    delete openOrderViewDisplay;
    delete instrumentViewDisplay;
    delete strategyViewDisplay;
    delete messageViewDisplay;
    delete positionViewDisplay;
    delete dockForPositionView;
=======
    //delete openOrderView;
    delete console;
    delete openOrderViewDisplay;
    delete instrumentViewDisplay;
    delete strategyViewDisplay;
    delete consoleViewDisplay;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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

<<<<<<< HEAD
PositionView* MainWindow::getPositionView()
{
    return positionView;
}
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

void MainWindow::setUpMainWindow()
{
    if(!_mainWindow)
    {
        _mainWindow = new MainWindow();
    }
}

<<<<<<< HEAD
void MainWindow::onLog(const String& output)
=======
void MainWindow::onLog(const QString& output)
{
    console->setText(output);
}

/*void MainWindow::minimize()
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    messageView->insertHtml(output);
}

void MainWindow::alterInstrumentView()
{
    if(dockForInstrumentView->isHidden())
    {
     //instrumentView->show();
     dockForInstrumentView->show();
     //instrumentViewDisplay->setChecked(true);
    }
    else
    {
        //instrumentView->hide();
        dockForInstrumentView->hide();
        //instrumentViewDisplay->setChecked(false);
    }
}

void MainWindow::alterStrategyView()
{
    if(dockForStrategyView->isHidden())
    {
        dockForStrategyView->show();
        //strategyViewDisplay->setChecked(true);
    }
    else
    {
        dockForStrategyView->hide();
        //strategyViewDisplay->setChecked(false);
    }
}

void MainWindow::alterOpenOrderView()
{
    if(dockForOpenOrderView->isHidden())
    {
        dockForOpenOrderView->show();
        //openOrderViewDisplay->setChecked(true);
    }
    else
    {
        dockForOpenOrderView->hide();
<<<<<<< HEAD
        //openOrderViewDisplay->setChecked(false);
=======
        openOrderViewDisplay->setChecked(true);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    }
}

void MainWindow::alterPositionView()
{
    if(dockForPositionView->isHidden())
    {
        dockForPositionView->show();
        //positionViewDisplay->setChecked(true);
    }
    else
    {
<<<<<<< HEAD
        dockForPositionView->hide();
        //positionViewDisplay->setChecked(false);
    }
}


void MainWindow::alterMessageView()
{
    if(dockForMessageView->isHidden())
    {
        dockForMessageView->show();
        //messageViewDisplay->setChecked(true);
    }
    else
    {
        dockForMessageView->hide();
        //messageViewDisplay->setChecked(false);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    /*messageBox = new QMessageBox(this);
    messageBox->addButton(QMessageBox::Ok);
    messageBox->addButton(QMessageBox::Cancel);
    messageBox->show();*/
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("AIMSTrader"),
                               tr("Do you really want to close"),
                          QMessageBox::Ok | QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        stop();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::stop()
{
    //here we should close whatever we want to close
    Service::Instance()->stopServices();
}
=======
        dockForConsole->hide();
        consoleViewDisplay->setChecked(false);
    }
}
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
