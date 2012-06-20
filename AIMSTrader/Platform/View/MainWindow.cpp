#include "Platform/View/MainWindow.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/StrategyView.h"
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/OpenOrderWidget.h"
#include "Platform/View/StrategyPositionView.h"
#include "Platform/Startup/Service.h"
#include "Platform/View/MainWindow.h"
#include "Platform/View/SpecialDockWidget.h"
#include "Platform/View/MessageView.h"

#include <QAction>
#include <QLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSignalMapper>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <qdebug.h>
#include <QScrollBar>


Q_DECLARE_METATYPE(QDockWidget::DockWidgetFeatures)

//MainWindow::* MainWindow::_MainWindow:: = NULL;

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
    //setup();
   // setAcceptDrops(true);
}

void MainWindow::setup(const QMap<QString, QSize> &customSizeHints)
{
    setObjectName("MainWindow");
    setWindowTitle("AIMSTrader");

    setupMenuBar();
    setupDockWidgets(customSizeHints);
    init();

    //setCentralWidget(dockForStrategyView);
}

void MainWindow::init()
{
    //resize(1300,1024);

    /*static const struct Set {
        const char * name;
        uint flags;
        Qt::DockWidgetArea area;
    } sets [] = {
        { "Position", 0, Qt::RightDockWidgetArea },
        { "MarketData", 0, Qt::TopDockWidgetArea },
        { "OpenOrders", 0, Qt::TopDockWidgetArea },
        { "Strategy", 0, Qt::BottomDockWidgetArea },
        { "Messages", 0, Qt::BottomDockWidgetArea }
    };*/
  //  SpecialDockWidget *swatch = new SpecialDockWidget(tr(sets[i].name), this, Qt::WindowFlags(sets[i].flags));
//    QString name = QString::fromLatin1(sets[i].name);
//    if (customSizeHints.contains(name))
//        swatch->setCustomSizeHint(customSizeHints.value(name));

//    addDockWidget(sets[i].area, swatch);
//    dockWidgetMenu->addMenu(swatch->menu);


    //dockForInstrumentView = new SpecialDockWidget("Market Data", this, Qt::WindowFlags(Qt::TopDockWidgetArea));
    _instrumentView = new InstrumentView(_dockForInstrumentView);
    _dockForInstrumentView->setWidget(_instrumentView);
    //dockForInstrumentView->move(0,0);
    //dockForInstrumentView->setAllowedAreas(Qt::NoSpecialDockWidgetArea);

    //dockForStrategyView = new SpecialDockWidget("Strategy", this, Qt::WindowFlags(Qt::TopDockWidgetArea));
    _strategyView = new StrategyView(_dockForStrategyView);
    _dockForStrategyView->setWidget(_strategyView);
//    dockForStrategyView->move(0,250);
//    dockForStrategyView->setAllowedAreas(Qt::NoSpecialDockWidgetArea);

    //dockForMessageView = new SpecialDockWidget("Messages",this, Qt::WindowFlags(Qt::BottomDockWidgetArea));
    _messageView = new MessageView(_dockForMessageView);
    //messageView->setMinimumSize(1000,200);
    //_messageView->setReadOnly(true);
    _dockForMessageView->setWidget(_messageView);
//    dockForMessageView->move(0,500);
//    dockForMessageView->setAllowedAreas(Qt::NoSpecialDockWidgetArea);

    //dockForOpenOrderView = new SpecialDockWidget("Open Orders", this, Qt::WindowFlags(Qt::BottomDockWidgetArea));
    _openOrderView = new OpenOrderWidget(_dockForOpenOrderView);
    _dockForOpenOrderView->setWidget(_openOrderView);
    //dockForOpenOrderView->show();
//    dockForOpenOrderView->move(220,200);
//    dockForOpenOrderView->setAllowedAreas(Qt::NoSpecialDockWidgetArea);

    //dockForPositionView = new SpecialDockWidget("Positions", this, Qt::WindowFlags(Qt::BottomDockWidgetArea));
    _positionView = new StrategyPositionView(_dockForPositionView);
    _dockForPositionView->setWidget(_positionView);
//    dockForPositionView->show();
//    dockForPositionView->move(220,500);
//    dockForPositionView->setAllowedAreas(Qt::NoSpecialDockWidgetArea);

    setupMenu();

    //setLayout(gridLayout);
   // instrumentView->show();
    _dockForInstrumentView->show();
    _dockForStrategyView->show();
    _dockForMessageView->show();
    _dockForOpenOrderView->show();
    _dockForPositionView->show();
    show();
}

void MainWindow::setupMenu()
{
    _windowMenu = new QMenu("&Window");
    _minimize = new QAction("&Minimize",this);

    _windowMenu->addAction(_minimize);

    _instrumentViewDisplay = new QAction("&Instruments",this);
    connect(_dockForInstrumentView,SIGNAL(visibilityChanged(bool)),_instrumentViewDisplay,SLOT(setChecked(bool)));

    _strategyViewDisplay = new QAction("&Strategy",this);
    connect(_dockForStrategyView,SIGNAL(visibilityChanged(bool)),_strategyViewDisplay,SLOT(setChecked(bool)));

    _openOrderViewDisplay = new QAction("&OpenOrders",this);
    connect(_dockForOpenOrderView,SIGNAL(visibilityChanged(bool)),_openOrderViewDisplay,SLOT(setChecked(bool)));

    _messageViewDisplay = new QAction("&Messages",this);
    connect(_dockForMessageView,SIGNAL(visibilityChanged(bool)),_messageViewDisplay,SLOT(setChecked(bool)));

    _positionViewDisplay = new QAction("&Positions",this);
    connect(_dockForPositionView,SIGNAL(visibilityChanged(bool)),_positionViewDisplay,SLOT(setChecked(bool)));

    _instrumentViewDisplay->setCheckable(true);
    _strategyViewDisplay->setCheckable(true);
    _openOrderViewDisplay->setCheckable(true);
    _messageViewDisplay->setCheckable(true);
    _positionViewDisplay->setCheckable(true);

    //instrumentViewDisplay->setChecked(true);
    //strategyViewDisplay->setChecked(true);
    //openOrderViewDisplay->setChecked(true);
    //messageViewDisplay->setChecked(true);
    //positionViewDisplay->setChecked(true);

    _viewsMenu = new QMenu("&Views");

    _windowMenu->addSeparator();
    _windowMenu->addMenu(_viewsMenu);

    _viewsMenu->addAction(_instrumentViewDisplay);
    _viewsMenu->addAction(_strategyViewDisplay);
    _viewsMenu->addAction(_openOrderViewDisplay);
    _viewsMenu->addAction(_messageViewDisplay);
    _viewsMenu->addAction(_positionViewDisplay);
    //viewsMenu->addAction(minimize);

    _pMenuBar = new QMenuBar();
    _pMenuBar->addMenu(_windowMenu);

    connect(_minimize, SIGNAL(triggered()), this, SLOT(showMinimized()));
    connect(_instrumentViewDisplay,SIGNAL(triggered()), this, SLOT(alterInstrumentView()));
    connect(_strategyViewDisplay,SIGNAL(triggered()), this, SLOT(alterStrategyView()));
    connect(_openOrderViewDisplay,SIGNAL(triggered()), this, SLOT(alterOpenOrderView()));
    connect(_messageViewDisplay,SIGNAL(triggered()), this,SLOT(alterMessageView()));
    connect(_positionViewDisplay,SIGNAL(triggered()), this,SLOT(alterPositionView()));

    //connect(instrumentView,SIGNAL(closed()),this,SLOT(alterInstrumentView()));
    //connect(strategyView,SIGNAL(closed()),this,SLOT(alterStrategyView()));
    //connect(openOrderView,SIGNAL(closed()),this,SLOT(alterOpenOrderView()));
}


MainWindow::~MainWindow()
{
    delete _instrumentView;
    delete _strategyView;
    delete _openOrderView;
    delete _messageView;
    delete _openOrderViewDisplay;
    delete _instrumentViewDisplay;
    delete _strategyViewDisplay;
    delete _messageViewDisplay;
    delete _positionViewDisplay;
    delete _dockForPositionView;

    //delete splitter;
}

MessageView* MainWindow::getMessageView()
{
    return _messageView;
}

InstrumentView* MainWindow::getInstrumentView()
{
    return _instrumentView;
}

StrategyView* MainWindow::getStrategyView()
{
    return _strategyView;
}

OpenOrderWidget* MainWindow::getOpenOrderView()
{
    return _openOrderView;
}

StrategyPositionView* MainWindow::getPositionView()
{
    return _positionView;
}

//void MainWindow::onLog(const String& output)
//{
//    //_messageView->insertHtml(output);
//    _messageView->verticalScrollBar()->setSliderPosition(
//        _messageView->verticalScrollBar()->maximum());
//}

void MainWindow::alterInstrumentView()
{
    if(_dockForInstrumentView->isHidden())
    {
     //instrumentView->show();
     _dockForInstrumentView->show();
     //instrumentViewDisplay->setChecked(true);
    }
    else
    {
        //instrumentView->hide();
        _dockForInstrumentView->hide();
        //instrumentViewDisplay->setChecked(false);
    }
}

void MainWindow::alterStrategyView()
{
    if(_dockForStrategyView->isHidden())
    {
        _dockForStrategyView->show();
        //strategyViewDisplay->setChecked(true);
    }
    else
    {
        _dockForStrategyView->hide();
        //strategyViewDisplay->setChecked(false);
    }
}

void MainWindow::alterOpenOrderView()
{
    if(_dockForOpenOrderView->isHidden())
    {
        _dockForOpenOrderView->show();
        //openOrderViewDisplay->setChecked(true);
    }
    else
    {
        _dockForOpenOrderView->hide();
        //openOrderViewDisplay->setChecked(false);
    }
}

void MainWindow::alterPositionView()
{
    if(_dockForPositionView->isHidden())
    {
        _dockForPositionView->show();
        //positionViewDisplay->setChecked(true);
    }
    else
    {
        _dockForPositionView->hide();
        //positionViewDisplay->setChecked(false);
    }
}


void MainWindow::alterMessageView()
{
    if(_dockForMessageView->isHidden())
    {
        _dockForMessageView->show();
        //messageViewDisplay->setChecked(true);
    }
    else
    {
        _dockForMessageView->hide();
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

void MainWindow::actionTriggered(QAction *action)
{
    qDebug("action '%s' triggered", action->text().toLocal8Bit().data());
}

//void MainWindow::setupToolBar()
//{
//    for (int i = 0; i < 3; ++i) {
//        ToolBar *tb = new ToolBar(QString::fromLatin1("Tool Bar %1").arg(i + 1), this);
//        toolBars.append(tb);
//        addToolBar(tb);
//    }
//}

void MainWindow::setupMenuBar()
{
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    QAction *action = menu->addAction(tr("Save layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(saveLayout()));

    action = menu->addAction(tr("Load layout..."));
    connect(action, SIGNAL(triggered()), this, SLOT(loadLayout()));

    action = menu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

    menu->addSeparator();

    menu->addAction(tr("&Quit"), this, SLOT(close()));

//    mainWindowMenu = menuBar()->addMenu(tr("Main window"));

//    action = mainWindowMenu->addAction(tr("Animated docks"));
//    action->setCheckable(true);
//    action->setChecked(dockOptions() & AnimatedDocks);
//    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

//    action = mainWindowMenu->addAction(tr("Allow nested docks"));
//    action->setCheckable(true);
//    action->setChecked(dockOptions() & AllowNestedDocks);
//    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

//    action = mainWindowMenu->addAction(tr("Allow tabbed docks"));
//    action->setCheckable(true);
//    action->setChecked(dockOptions() & AllowTabbedDocks);
//    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

//    action = mainWindowMenu->addAction(tr("Force tabbed docks"));
//    action->setCheckable(true);
//    action->setChecked(dockOptions() & ForceTabbedDocks);
//    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

//    action = mainWindowMenu->addAction(tr("Vertical tabs"));
//    action->setCheckable(true);
//    action->setChecked(dockOptions() & VerticalTabs);
//    connect(action, SIGNAL(toggled(bool)), this, SLOT(setDockOptions()));

////    QMenu *toolBarMenu = menuBar()->addMenu(tr("Tool bars"));
////    for (int i = 0; i < toolBars.count(); ++i)
////        toolBarMenu->addMenu(toolBars.at(i)->menu);

    //dockWidgetMenu = menuBar()->addMenu(tr("&Dock Widgets"));
}

void MainWindow::setDockOptions()
{
    DockOptions opts;
    QList<QAction*> actions = _mainWindowMenu->actions();

    if (actions.at(0)->isChecked())
        opts |= AnimatedDocks;
    if (actions.at(1)->isChecked())
        opts |= AllowNestedDocks;
    if (actions.at(2)->isChecked())
        opts |= AllowTabbedDocks;
    if (actions.at(3)->isChecked())
        opts |= ForceTabbedDocks;
    if (actions.at(4)->isChecked())
        opts |= VerticalTabs;

    QMainWindow::setDockOptions(opts);
}

void MainWindow::saveLayout()
{
    QString fileName
        = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::loadLayout()
{
    QString fileName
        = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(fileName)
                        .arg(file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
        ok = restoreGeometry(geo_data);
    if (ok)
        ok = restoreState(layout_data);

    if (!ok) {
        QString msg = tr("Error reading %1")
                        .arg(fileName);
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

QAction *addAction(QMenu *menu, const QString &text, QActionGroup *group, QSignalMapper *mapper,
                    int id)
{
    bool first = group->actions().isEmpty();
    QAction *result = menu->addAction(text);
    result->setCheckable(true);
    result->setChecked(first);
    group->addAction(result);
    QObject::connect(result, SIGNAL(triggered()), mapper, SLOT(map()));
    mapper->setMapping(result, id);
    return result;
}

void MainWindow::setupDockWidgets(const QMap<QString, QSize> &customSizeHints)
{
    qRegisterMetaType<QDockWidget::DockWidgetFeatures>();

    _mapper = new QSignalMapper(this);
    connect(_mapper, SIGNAL(mapped(int)), this, SLOT(setCorner(int)));

//    QMenu *corner_menu = dockWidgetMenu->addMenu(tr("Top left corner"));
//    QActionGroup *group = new QActionGroup(this);
//    group->setExclusive(true);
//    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 0);
//    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 1);

//    corner_menu = dockWidgetMenu->addMenu(tr("Top right corner"));
//    group = new QActionGroup(this);
//    group->setExclusive(true);
//    ::addAction(corner_menu, tr("Top dock area"), group, mapper, 2);
//    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 3);

//    corner_menu = dockWidgetMenu->addMenu(tr("Bottom left corner"));
//    group = new QActionGroup(this);
//    group->setExclusive(true);
//    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 4);
//    ::addAction(corner_menu, tr("Left dock area"), group, mapper, 5);

//    corner_menu = dockWidgetMenu->addMenu(tr("Bottom right corner"));
//    group = new QActionGroup(this);
//    group->setExclusive(true);
//    ::addAction(corner_menu, tr("Bottom dock area"), group, mapper, 6);
//    ::addAction(corner_menu, tr("Right dock area"), group, mapper, 7);

     //dockWidgetMenu->addSeparator();


    _dockForInstrumentView = new SpecialDockWidget("Market Data", this, Qt::WindowFlags(0));
    _dockForInstrumentView->setMaximumSize(300,100000);
    addDockWidget(Qt::LeftDockWidgetArea, _dockForInstrumentView);

    _dockForStrategyView = new SpecialDockWidget("Strategy", this, Qt::WindowFlags(0));
    addDockWidget(Qt::RightDockWidgetArea, _dockForStrategyView);


    _dockForOpenOrderView = new SpecialDockWidget("Open Orders", this, Qt::WindowFlags(0));
    addDockWidget(Qt::RightDockWidgetArea, _dockForOpenOrderView);

    //tabifyDockWidget(dockForStrategyView, dockForOpenOrderView);

    _dockForPositionView = new SpecialDockWidget("Positions", this, Qt::WindowFlags(0));
    addDockWidget(Qt::RightDockWidgetArea, _dockForPositionView);

    _dockForMessageView = new SpecialDockWidget("Messages", this, Qt::WindowFlags(0));
    addDockWidget(Qt::RightDockWidgetArea, _dockForMessageView);


//        QString name = QString::fromLatin1(sets[i].name);
//        if (customSizeHints.contains(name))
//            swatch->setCustomSizeHint(customSizeHints.value(name));

       // addDockWidget(sets[i].area, swatch);
        //dockWidgetMenu->addMenu(swatch->menu);

//    createDockWidgetAction = new QAction(tr("Add dock widget..."), this);
//    connect(createDockWidgetAction, SIGNAL(triggered()), this, SLOT(createDockWidget()));
//    destroyDockWidgetMenu = new QMenu(tr("Destroy dock widget"), this);
//    destroyDockWidgetMenu->setEnabled(false);
//    connect(destroyDockWidgetMenu, SIGNAL(triggered(QAction*)), this, SLOT(destroyDockWidget(QAction*)));

//    dockWidgetMenu->addSeparator();
//    dockWidgetMenu->addAction(createDockWidgetAction);
//    dockWidgetMenu->addMenu(destroyDockWidgetMenu);
}

void MainWindow::setCorner(int id)
{
    switch (id) {
        case 0:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::TopDockWidgetArea);
            break;
        case 1:
            QMainWindow::setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 2:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::TopDockWidgetArea);
            break;
        case 3:
            QMainWindow::setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
            break;
        case 4:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
            break;
        case 5:
            QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
            break;
        case 6:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
            break;
        case 7:
            QMainWindow::setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
            break;
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

class CreateDockWidgetDialog : public QDialog
{
public:
    CreateDockWidgetDialog(QWidget *parent = 0);

    QString objectName() const;
    Qt::DockWidgetArea location() const;

private:
    QLineEdit *m_objectName;
    QComboBox *m_location;
};

CreateDockWidgetDialog::CreateDockWidgetDialog(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout(this);

    layout->addWidget(new QLabel(tr("Object name:")), 0, 0);
    m_objectName = new QLineEdit;
    layout->addWidget(m_objectName, 0, 1);

    layout->addWidget(new QLabel(tr("Location:")), 1, 0);
    m_location = new QComboBox;
    m_location->setEditable(false);
    m_location->addItem(tr("Top"));
    m_location->addItem(tr("Left"));
    m_location->addItem(tr("Right"));
    m_location->addItem(tr("Bottom"));
    m_location->addItem(tr("Restore"));
    layout->addWidget(m_location, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    layout->addLayout(buttonLayout, 2, 0, 1, 2);
    buttonLayout->addStretch();

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    buttonLayout->addWidget(cancelButton);
    QPushButton *okButton = new QPushButton(tr("Ok"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    buttonLayout->addWidget(okButton);

    okButton->setDefault(true);
}

QString CreateDockWidgetDialog::objectName() const
{
    return m_objectName->text();
}

Qt::DockWidgetArea CreateDockWidgetDialog::location() const
{
    switch (m_location->currentIndex()) {
        case 0: return Qt::TopDockWidgetArea;
        case 1: return Qt::LeftDockWidgetArea;
        case 2: return Qt::RightDockWidgetArea;
        case 3: return Qt::BottomDockWidgetArea;
        default:
            break;
    }
    return Qt::NoDockWidgetArea;
}

void MainWindow::createDockWidget()
{
    CreateDockWidgetDialog dialog(this);
    int ret = dialog.exec();
    if (ret == QDialog::Rejected)
        return;

    QDockWidget *dw = new QDockWidget;
    dw->setObjectName(dialog.objectName());
    dw->setWindowTitle(dialog.objectName());
    dw->setWidget(new QTextEdit);

    Qt::DockWidgetArea area = dialog.location();
    switch (area) {
        case Qt::LeftDockWidgetArea:
        case Qt::RightDockWidgetArea:
        case Qt::TopDockWidgetArea:
        case Qt::BottomDockWidgetArea:
            addDockWidget(area, dw);
            break;
        default:
            if (!restoreDockWidget(dw)) {
                QMessageBox::warning(this, QString(), tr("Failed to restore dock widget"));
                delete dw;
                return;
            }
            break;
    }

    _extraDockWidgets.append(dw);
    _destroyDockWidgetMenu->setEnabled(true);
    _destroyDockWidgetMenu->addAction(new QAction(dialog.objectName(), this));
}

void MainWindow::destroyDockWidget(QAction *action)
{
    int index = _destroyDockWidgetMenu->actions().indexOf(action);
    delete _extraDockWidgets.takeAt(index);
    _destroyDockWidgetMenu->removeAction(action);
    action->deleteLater();

    if (_destroyDockWidgetMenu->isEmpty())
        _destroyDockWidgetMenu->setEnabled(false);
}



