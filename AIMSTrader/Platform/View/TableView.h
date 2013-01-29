#pragma once
#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableWidget>
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>
#include <QLabel>
#include <algorithm>
#include "Platform/View/TableCellItem.h"
#include <QMenu>
#include <QAction>
#include "Platform/View/HeaderListWidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include "Platform/View/CustomizeHeaderDialog.h"

template<class View, class ViewItem, class Model, class ModelColumn>
class TableView : public QTableWidget
{
    protected:
        TableCellItem<ViewItem>* _clickedItem;
        int _numRows;
        int _numCols;
        QStringList _header;
        int _headerColumnClicked;
        bool _isSortingAllowed;
        Model* _model;

    protected:
         QMenu* _headerMenu;
         QMenu* _windowsMenu;
         QAction* removeAction;
         QList<QAction*> _customizeActions;
         QSignalMapper* _signalMapper;

    public:
        TableView(QWidget* parent);

    private:
        void setView();
        void setupMenu();
        void setHeaders();
        void headerContextMenuEvent(QContextMenuEvent*  );
        void addHeader(const QString&);
        void insertItem(ViewItem* );

    public:
       void setupLooks();
       void resizeEvent(QResizeEvent* event);
       void closeEvent(QCloseEvent* event);
       TableCellItem<ViewItem>* getTableItem(const int, const int);
       void editHeader(const QString);

    protected:
       ViewItem* addItemInView();
       const int getViewColumn(const int trueColumnModel);
       void removeHeader();
       void customizeHeader();
       void modifyHeader(const int);

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
};

template<class View, class ViewItem, class Model, class ModelColumn>
TableView<View, ViewItem, Model, ModelColumn>::TableView(QWidget* parent):QTableWidget(parent)
{
    _model = new Model();
    _numRows = 0;
    _numCols = _model->getDataModelNumColumns();
    _headerColumnClicked = -1;
    _isSortingAllowed  = false;

    setupMenu();
    setView();
    setupLooks();
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setupLooks()
{
    setUpdatesEnabled(true);
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setResizeMode(QHeaderView::Interactive);
    hHeader->setStretchLastSection(true);
    hHeader->installEventFilter(this);
    hHeader->setMovable(true);

    QFont f("Verdana");
    f.setBold(true);
    hHeader->setFont(f);
    //hHeader->setS

    //hHeader->setStretchLastSection(true);
    //hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHeaderView* vHeader = QTableWidget::verticalHeader();
    vHeader->setDefaultSectionSize(15);

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::lightGray);
    setPalette(p);

    setSortingEnabled(_isSortingAllowed);

    setShowGrid(false);
    //setAlternatingRowColors(true);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::resizeEvent(QResizeEvent* event)
{
//    QSize size = event->size();
//    QHeaderView* hHeader = QTableWidget::horizontalHeader();
//    if(int num = hHeader->count() - hHeader->hiddenSectionCount())
//    {
//        hHeader->setDefaultSectionSize(size.rwidth()/num);
//    }
    QTableWidget::resizeEvent(event);
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::closeEvent(QCloseEvent *event)
{
    hide();
}

template<class View, class ViewItem, class Model, class ModelColumn>
TableCellItem<ViewItem>* TableView<View, ViewItem, Model, ModelColumn>::getTableItem(const int row, const int column)
{
    return static_cast<TableCellItem<ViewItem>*>(item(row, column));
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::editHeader(const QString column)
{
    _header<<column;
}

template<class View, class ViewItem, class Model, class ModelColumn>
ViewItem* TableView<View, ViewItem, Model, ModelColumn>::addItemInView()
{
    setSortingEnabled (false);
    ViewItem* nItem = new ViewItem(_numCols);

    insertItem(nItem);

    setSortingEnabled (_isSortingAllowed);
    return nItem;
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::insertItem(ViewItem* item)
{
    int currentRow = _numRows++;
    item->setRow(currentRow);

    if(currentRow >= rowCount())
    {
       insertRow(currentRow);
    }

    for(int currentCol = 0 ; currentCol < _numCols ; ++currentCol)
    {
        setItem(currentRow, currentCol, item->getCellItem(currentCol));
    }

    //showRow(currentRow);
}

template<class View, class ViewItem, class Model, class ModelColumn>
const int TableView<View, ViewItem, Model, ModelColumn>::getViewColumn(const int trueColumnModel)
{
//    if(_trueColumnToViewColumnMap.contains(trueColumnModel))
//    {
//        return _trueColumnToViewColumnMap[trueColumnModel];
//    }
    return -1;
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setView()
{
    int defaultNumRows = 0;
    setRowCount(defaultNumRows);
    setColumnCount(_numCols);
    setHeaders();

    int i = 0;
    while(++i < defaultNumRows)
    {
        if(i > 20)
            hideRow(i);
    }
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setHeaders()
{
    //Model* model = Model::Instance();
    QList<ModelColumn> modelColumns = _model->getDataModelColumns();
    QList<QString> columnNames = _model->getDataModelColumnNames();

    for(int i=0 ; i<modelColumns.size() ; ++i)
    {
        _header <<  columnNames[i];
        if(!_model->IsDefault(modelColumns[i]))
        {
            hideColumn(i);
        }
    }
    setHorizontalHeaderLabels(_header);
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setupMenu()
{
    _headerMenu = new QMenu("HeaderMenu", this);
    _windowsMenu = new QMenu("More", this);
     removeAction = new QAction("Remove", this);
     _signalMapper = new QSignalMapper(this);

     QList<ModelColumn> modelColumns = _model->getDataModelColumns();
     QList<QString> columnNames = _model->getDataModelColumnNames();

     for(int i=0;i<_numCols;++i)
     {
         QAction* nAction = new QAction(columnNames[i], this);
         nAction->setCheckable(true);

         if(_model->IsDefault(modelColumns[i]))
         {
            nAction->setChecked(true);
         }

         _customizeActions.append(nAction);
         _signalMapper->setMapping(nAction, int(modelColumns[i]));
         connect(_customizeActions[i], SIGNAL(triggered()), _signalMapper, SLOT(map()));
     }

     _headerMenu->addAction(removeAction);
     _headerMenu->addSeparator();
     _headerMenu->addMenu(_windowsMenu);
     _windowsMenu->addActions(_customizeActions);
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::removeHeader()
{
    setSortingEnabled (false);

    hideColumn(_headerColumnClicked);
    _customizeActions[_headerColumnClicked]->setChecked(false);

    QSize size = horizontalHeader()->size();
    int x=1;
    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }

    setSortingEnabled (_isSortingAllowed);
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::customizeHeader()
{

}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::headerContextMenuEvent(QContextMenuEvent*  event)
{
    _headerColumnClicked = horizontalHeader()->logicalIndexAt(event->x(),event->y());
    event->accept();
    _headerMenu->exec(event->globalPos());
}

template<class View, class ViewItem, class Model, class ModelColumn>
bool TableView<View, ViewItem, Model, ModelColumn>::eventFilter(QObject *obj, QEvent *event)
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

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::modifyHeader(const int modelColumn)
{
    bool isChecked = _customizeActions[modelColumn]->isChecked();
    if(isChecked)
    {
        showColumn(modelColumn);
    }
    else
    {
        hideColumn(modelColumn);
    }

    QSize size = horizontalHeader()->size();
    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::addHeader(const QString& header)
{
    horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    //horizontalHeader()->setStretchLastSection(true);

    QSize size = horizontalHeader()->size();

//    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
//    {
//        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
//    }

    int count = horizontalHeader()->count();
    int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount();
    for(int i=0;i<count;++i)
    {
        horizontalHeader()->resizeSection(i,size.rwidth()/num);
    }

}




#endif // TABLEVIEW_H
