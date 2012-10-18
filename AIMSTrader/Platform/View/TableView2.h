#ifndef TABLEVIEW2_H
#define TABLEVIEW2_H

#include <QTableView>
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>
#include <QLabel>
#include <algorithm>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include "Platform/View/CustomizeHeaderDialog.h"
#include <QDebug>
#include <QSortFilterProxyModel>

template<class View, class Model>
class TableView2 : public QTableView
{
    protected:
        Model* _model;
        QSortFilterProxyModel* _proxyModel;
        QModelIndex _clickedIdx;
        int _headerColumnClicked;
        int _numVisibleCols;
        bool _isSortingAllowed;

    protected:
         QMenu* _headerMenu;
         QMenu* _windowsMenu;
         QAction* _removeAction;
         QList<QAction*> _customizeActions;
         QSignalMapper* _signalMapper;

    public:
        TableView2(QWidget* parent);

    private:
        void setView();
        void setupMenu();
        void setHeaders();
        void headerContextMenuEvent(QContextMenuEvent*  );
        void addHeader(const QString&);

    public:
       void setupLooks();
       //void resizeEvent(QResizeEvent* event);
       void closeEvent(QCloseEvent* event);
       void editHeader(const QString);
       void resizeEvent(QResizeEvent *event);

    public:
       Model* getModel()
       {
           return _model;
       }

    protected:
       void removeHeader();
       void customizeHeader();
       void modifyHeader(const int);

    protected:
        bool eventFilter(QObject *obj, QEvent *event);

};

template<class View, class Model>
TableView2<View, Model>::TableView2(QWidget* parent):QTableView(parent)
{
    _model = new Model(this);
    _proxyModel = new QSortFilterProxyModel(this);
    _proxyModel->setSourceModel(_model);
    _proxyModel->setDynamicSortFilter(true);
    setModel(_proxyModel);
    _headerColumnClicked = -1;
    _isSortingAllowed  = true;
    setupMenu();
    setupLooks();

}

template<class View, class Model>
void TableView2<View, Model>::setupLooks()
{
    setUpdatesEnabled(true);
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* hHeader = QTableView::horizontalHeader();
    hHeader->setResizeMode(QHeaderView::Interactive);
    hHeader->setStretchLastSection(true);
    hHeader->installEventFilter(this);
    hHeader->setMovable(true);

    QFont f("Verdana");
    f.setBold(true);
    hHeader->setFont(f);

    QSize size = horizontalHeader()->size();

    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }
    //hHeader->setS

    //hHeader->setStretchLastSection(true);
    //hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHeaderView* vHeader = verticalHeader();
    vHeader->setDefaultSectionSize(15);

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::lightGray);
    setPalette(p);

    setSortingEnabled(_isSortingAllowed);
    setSortingEnabled(true);
    setShowGrid(false);
    setAlternatingRowColors(true);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setTextAlignment(Qt::AlignCenter);
    //setBackgroundColor(Qt::black);
    //setForeground(Qt::white);
    //setText("");
    setFont(QFont("Cambria"));
}

template<class View, class Model>
void TableView2<View, Model>::resizeEvent(QResizeEvent* event)
{
    QTableView::resizeEvent(event);
    QSize size = horizontalHeader()->size();

    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }
}

template<class View, class Model>
void TableView2<View, Model>::closeEvent(QCloseEvent *event)
{
    hide();
}

template<class View, class Model>
void TableView2<View, Model>::editHeader(const QString column)
{
}


template<class View, class Model>
void TableView2<View, Model>::setView()
{

}

template<class View, class Model>
void TableView2<View, Model>::setupMenu()
{
    _headerMenu = new QMenu("HeaderMenu", this);
    _windowsMenu = new QMenu("More", this);
    _removeAction = new QAction("Remove", this);
    _signalMapper = new QSignalMapper(this);

     //QList<ModelColumn> modelColumns = _model->getModelColumns();
     QList<QString> columnNames = _model->getModelColumnNames();

     int numCols = _model->columnCount();
     for(int i=0;i<numCols;++i)
     {
         QAction* nAction = new QAction(columnNames[i], this);
         nAction->setCheckable(true);
         nAction->setChecked(true);
         bool isDefault = _model->IsDefault(i);
         if(!isDefault)
         {
            nAction->setChecked(false);
            hideColumn(i);
         }

         _customizeActions.append(nAction);
         _signalMapper->setMapping(nAction, i);
         connect(_customizeActions[i], SIGNAL(triggered()), _signalMapper, SLOT(map()));
     }

     _headerMenu->addAction(_removeAction);
     _headerMenu->addSeparator();
     _headerMenu->addMenu(_windowsMenu);
     _windowsMenu->addActions(_customizeActions);
}

template<class View, class Model>
void TableView2<View, Model>::removeHeader()
{
    if(isSortingEnabled())
    {
        qDebug()<<"Sorting Enabled";
    }
    else
    {
        qDebug()<<"Sorting Disabled";
    }

      hideColumn(_headerColumnClicked);
      _customizeActions[_headerColumnClicked]->setChecked(false);

      QSize size = horizontalHeader()->size();

      if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
      {
          horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
      }
}

template<class View, class Model>
void TableView2<View, Model>::customizeHeader()
{

}

template<class View, class Model>
void TableView2<View, Model>::headerContextMenuEvent(QContextMenuEvent*  event)
{
    _headerColumnClicked = horizontalHeader()->logicalIndexAt(event->x(),event->y());
    event->accept();
    _headerMenu->exec(event->globalPos());
}

template<class View, class Model>
bool TableView2<View, Model>::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
        case QEvent::ContextMenu:
        {
            QContextMenuEvent* e = static_cast<QContextMenuEvent*>(event);
            headerContextMenuEvent(e);
            return true;
        }
    }
}

template<class View, class Model>
void TableView2<View, Model>::modifyHeader(const int fullModelColumn)
{
    if(isColumnHidden(fullModelColumn))
    {
        showColumn(fullModelColumn);
        _customizeActions[fullModelColumn]->setChecked(true);
    }
    else
    {
        hideColumn(fullModelColumn);
        _customizeActions[fullModelColumn]->setChecked(false);
    }

    QSize size = horizontalHeader()->size();

    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }

////     size = horizontalHeader()->size();

////    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
////    {
////        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
////    }

//    setSortingEnabled (_isSortingAllowed);
}

template<class View, class Model>
void TableView2<View, Model>::addHeader(const QString& header)
{
    horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    QSize size = horizontalHeader()->size();

    int count = horizontalHeader()->count();
    int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount();
    for(int i=0;i<count;++i)
    {
        horizontalHeader()->resizeSection(i,size.rwidth()/num);
    }
}

#endif // TABLEVIEW2_H
