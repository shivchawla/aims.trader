#include "Platform/View/SpecialDockWidget.h"
#include "Platform/View/ColorDock.h"
#include "Platform/View/MainWindow.h"
#include "Platform/View/SubMainWindow.h"
#include "TradingApplication.h"


SpecialDockWidget::SpecialDockWidget(const QString &objectName, QWidget *parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags)
{
    //setFeatures(features() & ~DockWidgetFloatable);
    setObjectName(objectName);// + QLatin1String(" Dock Widget"));

//   QPushButton* pb = new QPushButton("Wow",this);
//   pb->resize(10,10);
//   pb->move(10,0);
//           pb->setFlat(true);
//    setTitleBarWidget(pb);
     setWindowTitle(objectName);// + QLatin1String(" [*]"));

    QFrame *swatch = new ColorDock(objectName, this);
    swatch->setFrameStyle(QFrame::Box | QFrame::Sunken);

    setWidget(swatch);
    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(dockWidgetTopLevelChanged(bool)));

    //changeSizeHintsAction = new QAction(tr("Change Size Hints"), this);
    //connect(changeSizeHintsAction, SIGNAL(triggered()), swatch, SLOT(changeSizeHints()));

//    closableAction = new QAction(tr("Closable"), this);
//    closableAction->setCheckable(true);
//    connect(closableAction, SIGNAL(triggered(bool)), SLOT(changeClosable(bool)));

//    movableAction = new QAction(tr("Movable"), this);
//    movableAction->setCheckable(true);
//    connect(movableAction, SIGNAL(triggered(bool)), SLOT(changeMovable(bool)));

//    floatableAction = new QAction(tr("Floatable"), this);
//    floatableAction->setCheckable(true);
//    connect(floatableAction, SIGNAL(triggered(bool)), SLOT(changeFloatable(bool)));

//    verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
//    verticalTitleBarAction->setCheckable(true);
//    connect(verticalTitleBarAction, SIGNAL(triggered(bool)),
//            SLOT(changeVerticalTitleBar(bool)));

//    floatingAction = new QAction(tr("Floating"), this);
//    floatingAction->setCheckable(true);
//    connect(floatingAction, SIGNAL(triggered(bool)), SLOT(changeFloating(bool)));

//    allowedAreasActions = new QActionGroup(this);
//    allowedAreasActions->setExclusive(false);

//    allowLeftAction = new QAction(tr("Allow on Left"), this);
//    allowLeftAction->setCheckable(true);
//    connect(allowLeftAction, SIGNAL(triggered(bool)), SLOT(allowLeft(bool)));

//    allowRightAction = new QAction(tr("Allow on Right"), this);
//    allowRightAction->setCheckable(true);
//    connect(allowRightAction, SIGNAL(triggered(bool)), SLOT(allowRight(bool)));

//    allowTopAction = new QAction(tr("Allow on Top"), this);
//    allowTopAction->setCheckable(true);
//    connect(allowTopAction, SIGNAL(triggered(bool)), SLOT(allowTop(bool)));

//    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
//    allowBottomAction->setCheckable(true);
//    connect(allowBottomAction, SIGNAL(triggered(bool)), SLOT(allowBottom(bool)));

//    allowedAreasActions->addAction(allowLeftAction);
//    allowedAreasActions->addAction(allowRightAction);
//    allowedAreasActions->addAction(allowTopAction);
//    allowedAreasActions->addAction(allowBottomAction);

//    areaActions = new QActionGroup(this);
//    areaActions->setExclusive(true);

//    leftAction = new QAction(tr("Place on Left") , this);
//    leftAction->setCheckable(true);
//    connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

//    rightAction = new QAction(tr("Place on Right") , this);
//    rightAction->setCheckable(true);
//    connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

//    topAction = new QAction(tr("Place on Top") , this);
//    topAction->setCheckable(true);
//    connect(topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

//    bottomAction = new QAction(tr("Place on Bottom") , this);
//    bottomAction->setCheckable(true);
//    connect(bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));

//    areaActions->addAction(leftAction);
//    areaActions->addAction(rightAction);
//    areaActions->addAction(topAction);
//    areaActions->addAction(bottomAction);

//    connect(movableAction, SIGNAL(triggered(bool)), areaActions, SLOT(setEnabled(bool)));

//    connect(movableAction, SIGNAL(triggered(bool)), allowedAreasActions, SLOT(setEnabled(bool)));

//    connect(floatableAction, SIGNAL(triggered(bool)), floatingAction, SLOT(setEnabled(bool)));

//    connect(floatingAction, SIGNAL(triggered(bool)), floatableAction, SLOT(setDisabled(bool)));
//    connect(movableAction, SIGNAL(triggered(bool)), floatableAction, SLOT(setEnabled(bool)));

//    tabMenu = new QMenu(this);
//    tabMenu->setTitle(tr("Tab into"));
//    connect(tabMenu, SIGNAL(triggered(QAction*)), this, SLOT(tabInto(QAction*)));

//    splitHMenu = new QMenu(this);
//    splitHMenu->setTitle(tr("Split horizontally into"));
//    connect(splitHMenu, SIGNAL(triggered(QAction*)), this, SLOT(splitInto(QAction*)));

//    splitVMenu = new QMenu(this);
//    splitVMenu->setTitle(tr("Split vertically into"));
//    connect(splitVMenu, SIGNAL(triggered(QAction*)), this, SLOT(splitInto(QAction*)));

//    windowModifiedAction = new QAction(tr("Modified"), this);
//    windowModifiedAction->setCheckable(true);
//    windowModifiedAction->setChecked(false);
//    connect(windowModifiedAction, SIGNAL(toggled(bool)), this, SLOT(setWindowModified(bool)));

//    menu = new QMenu(objectName, this);
//    menu->addAction(toggleViewAction());
//    QAction *action = menu->addAction(tr("Raise"));
//    connect(action, SIGNAL(triggered()), this, SLOT(raise()));
//    menu->addAction(changeSizeHintsAction);
//    menu->addSeparator();
//    menu->addAction(closableAction);
//    menu->addAction(movableAction);
//    menu->addAction(floatableAction);
//    menu->addAction(floatingAction);
//    menu->addAction(verticalTitleBarAction);
//    menu->addSeparator();
//    menu->addActions(allowedAreasActions->actions());
//    menu->addSeparator();
//    menu->addActions(areaActions->actions());
//    menu->addSeparator();
//    menu->addMenu(splitHMenu);
//    menu->addMenu(splitVMenu);
//    menu->addMenu(tabMenu);
//    menu->addSeparator();
//    menu->addAction(windowModifiedAction);

//    connect(menu, SIGNAL(aboutToShow()), this, SLOT(updateContextMenu()));

//    if(objectName == "Black") {
//        leftAction->setShortcut(Qt::CTRL|Qt::Key_W);
//        rightAction->setShortcut(Qt::CTRL|Qt::Key_E);
//        toggleViewAction()->setShortcut(Qt::CTRL|Qt::Key_R);
//    }
}

void SpecialDockWidget::updateContextMenu()
{
//    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
//    const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
//    const Qt::DockWidgetAreas areas = allowedAreas();

//    closableAction->setChecked(features() & QDockWidget::DockWidgetClosable);
//    if (windowType() == Qt::Drawer) {
//        floatableAction->setEnabled(false);
//        floatingAction->setEnabled(false);
//        movableAction->setEnabled(false);
//        verticalTitleBarAction->setChecked(false);
//    } else {
//        floatableAction->setChecked(features() & QDockWidget::DockWidgetFloatable);
//        floatingAction->setChecked(isWindow());
//        // done after floating, to get 'floatable' correctly initialized
//        movableAction->setChecked(features() & QDockWidget::DockWidgetMovable);
//        verticalTitleBarAction
//            ->setChecked(features() & QDockWidget::DockWidgetVerticalTitleBar);
//    }

//    allowLeftAction->setChecked(isAreaAllowed(Qt::LeftDockWidgetArea));
//    allowRightAction->setChecked(isAreaAllowed(Qt::RightDockWidgetArea));
//    allowTopAction->setChecked(isAreaAllowed(Qt::TopDockWidgetArea));
//    allowBottomAction->setChecked(isAreaAllowed(Qt::BottomDockWidgetArea));

//    if (allowedAreasActions->isEnabled()) {
//        allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
//        allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
//        allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
//        allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
//    }

//    leftAction->blockSignals(true);
//    rightAction->blockSignals(true);
//    topAction->blockSignals(true);
//    bottomAction->blockSignals(true);

//    leftAction->setChecked(area == Qt::LeftDockWidgetArea);
//    rightAction->setChecked(area == Qt::RightDockWidgetArea);
//    topAction->setChecked(area == Qt::TopDockWidgetArea);
//    bottomAction->setChecked(area == Qt::BottomDockWidgetArea);

//    leftAction->blockSignals(false);
//    rightAction->blockSignals(false);
//    topAction->blockSignals(false);
//    bottomAction->blockSignals(false);

//    if (areaActions->isEnabled()) {
//        leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
//        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
//        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
//        bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
//    }

//    tabMenu->clear();
//    splitHMenu->clear();
//    splitVMenu->clear();
//    QList<SpecialDockWidget*> dock_list = qFindChildren<SpecialDockWidget*>(mainWindow);
//    foreach (SpecialDockWidget *dock, dock_list) {
////        if (!dock->isVisible() || dock->isFloating())
////            continue;
//        tabMenu->addAction(dock->objectName());
//        splitHMenu->addAction(dock->objectName());
//        splitVMenu->addAction(dock->objectName());
//    }
}

void SpecialDockWidget::splitInto(QAction *action)
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    QList<SpecialDockWidget*> dock_list = qFindChildren<SpecialDockWidget*>(mainWindow);
    SpecialDockWidget *target = 0;
    foreach (SpecialDockWidget *dock, dock_list) {
        if (action->text() == dock->objectName()) {
            target = dock;
            break;
        }
    }
    if (target == 0)
        return;

    Qt::Orientation o = action->parent() == _splitHMenu
                        ? Qt::Horizontal : Qt::Vertical;
    mainWindow->splitDockWidget(target, this, o);
}

void SpecialDockWidget::tabInto(QAction *action)
{
    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    QList<SpecialDockWidget*> dock_list = qFindChildren<SpecialDockWidget*>(mainWindow);
    SpecialDockWidget *target = 0;
    foreach (SpecialDockWidget *dock, dock_list) {
        if (action->text() == dock->objectName()) {
            target = dock;
            break;
        }
    }
    if (target == 0)
        return;

    mainWindow->tabifyDockWidget(target, this);
}

void SpecialDockWidget::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    //menu->exec(event->globalPos());
}

void SpecialDockWidget::resizeEvent(QResizeEvent *e)
{
    QDockWidget::resizeEvent(e);
}

void SpecialDockWidget::allow(Qt::DockWidgetArea area, bool a)
{
    Qt::DockWidgetAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);

//    if (_areaActions->isEnabled()) {
//        _leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
//        _rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
//        _topAction->setEnabled(areas & Qt::TopDockWidgetArea);
//        _bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
//    }
}

void SpecialDockWidget::place(Qt::DockWidgetArea area, bool p)
{
    if (!p) return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    mainWindow->addDockWidget(area, this);

//    if (_allowedAreasActions->isEnabled()) {
//        _allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
//        _allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
//        _allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
//        _allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
//    }
}

void SpecialDockWidget::setCustomSizeHint(const QSize &size)
{
    if (ColorDock *dock = qobject_cast<ColorDock*>(widget()))
        dock->setCustomSizeHint(size);
}

void SpecialDockWidget::changeClosable(bool on)
{ setFeatures(on ? features() | DockWidgetClosable : features() & ~DockWidgetClosable); }

void SpecialDockWidget::changeMovable(bool on)
{ setFeatures(on ? features() | DockWidgetMovable : features() & ~DockWidgetMovable); }

void SpecialDockWidget::changeFloatable(bool on)
{ setFeatures(on ? features() | DockWidgetFloatable : features() & ~DockWidgetFloatable); }

void SpecialDockWidget::changeFloating(bool floating)
{ setFloating(floating); }

void SpecialDockWidget::allowLeft(bool a)
{ allow(Qt::LeftDockWidgetArea, a); }

void SpecialDockWidget::allowRight(bool a)
{ allow(Qt::RightDockWidgetArea, a); }

void SpecialDockWidget::allowTop(bool a)
{ allow(Qt::TopDockWidgetArea, a); }

void SpecialDockWidget::allowBottom(bool a)
{ allow(Qt::BottomDockWidgetArea, a); }

void SpecialDockWidget::placeLeft(bool p)
{ place(Qt::LeftDockWidgetArea, p); }

void SpecialDockWidget::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }

void SpecialDockWidget::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }

void SpecialDockWidget::placeBottom(bool p)
{ place(Qt::BottomDockWidgetArea, p); }

void SpecialDockWidget::changeVerticalTitleBar(bool on)
{
    setFeatures(on ? features() | DockWidgetVerticalTitleBar
                    : features() & ~DockWidgetVerticalTitleBar);
}

void SpecialDockWidget::dockWidgetTopLevelChanged(bool isFloating)
{
    if(isFloating)
    {	setWindowFlags(Qt::Window);
        // setWindowFlags calls setParent() when changing the flags for a window, causing the widget to be hidden.
        // You must call show() to make the widget visible again
        show();
    }
}
