#ifndef SPECIALDOCKWIDGET_H
#define SPECIALDOCKWIDGET_H

#include <QDockWidget>

class QAction;
class QActionGroup;
class QMenu;

class SpecialDockWidget : public QDockWidget
{
    Q_OBJECT

//    QAction *_closableAction;
//    QAction *_movableAction;
//    QAction *_floatableAction;
//    QAction *floatingAction;
//    QAction *_verticalTitleBarAction;

//    QActionGroup *_allowedAreasActions;
//    QAction *_allowLeftAction;
//    QAction *_allowRightAction;
//    QAction *_allowTopAction;
//    QAction *_allowBottomAction;

//    QActionGroup *_areaActions;
//    QAction *_leftAction;
//    QAction *_rightAction;
//    QAction *_topAction;
//    QAction *_bottomAction;

//    QAction *_changeSizeHintsAction;

    QMenu *_tabMenu;
    QMenu *_splitHMenu;
    QMenu *_splitVMenu;

    QAction *_windowModifiedAction;

public:
    SpecialDockWidget(const QString &colorName, QWidget *parent = 0, Qt::WindowFlags flags = 0);

    QMenu *_menu;
    void setCustomSizeHint(const QSize &size);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void resizeEvent(QResizeEvent *e);
    //bool event(QEvent *event);

private:
    void allow(Qt::DockWidgetArea area, bool allow);
    void place(Qt::DockWidgetArea area, bool place);

private slots:
    void changeClosable(bool on);
    void changeMovable(bool on);
    void changeFloatable(bool on);
    void changeFloating(bool on);
    void changeVerticalTitleBar(bool on);
    void updateContextMenu();

    void allowLeft(bool a);
    void allowRight(bool a);
    void allowTop(bool a);
    void allowBottom(bool a);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void splitInto(QAction *action);
    void tabInto(QAction *action);

    void dockWidgetTopLevelChanged(bool);
};

#endif // SPECIALDOCKWIDGET_H
