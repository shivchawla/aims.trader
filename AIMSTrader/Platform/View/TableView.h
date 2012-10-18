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
        std::vector<ViewItem*> _viewItems;
        int _numRows;
        int _numCols;
        QStringList _header;
        QHash<int, int> _trueColumnToViewColumnMap;
        QHash<int, int> _viewColumnToTrueColumnMap;
        int _headerColumnClicked;
        int _numVisibleCols;
        bool _isSortingAllowed;


    protected:
         //CustomizeHeaderDialog* _dialog;
         QMenu* _headerMenu;
         QMenu* _windowsMenu;
         QAction* removeAction;
         QList<QAction*> _customizeActions;
         QSignalMapper* _signalMapper;

    public:
        TableView(QWidget* parent);
        //~TableView();

    private:
        void setView();
        void setupMenu();
        void setHeaders();
        //void populateHeaderListWidget();
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
       //void addColumnInView();
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
    Model* model = Model::Instance();
    _numRows = 0;
    _numCols = model->getDataModelNumColumns();
    _trueColumnToViewColumnMap.reserve(_numCols);
    _headerColumnClicked = -1;
    _numVisibleCols = model->getDataModelDefaultNumColumns();
    _isSortingAllowed  = true;
    QList<ModelColumn> modelColumns = model->getDataModelColumns();

    int j = 0;
    for(int i=0;i<modelColumns.size();++i)
    {
        if(model->IsDefault(modelColumns[i]))
        {
            _trueColumnToViewColumnMap[int(modelColumns[i])] = j;
            _viewColumnToTrueColumnMap[j++] = int(modelColumns[i]);
        }
    }

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
    _viewItems.push_back(nItem);

    insertItem(nItem);

    setSortingEnabled (_isSortingAllowed);
    return nItem;
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::insertItem(ViewItem* item)
{
    int currentRow = _numRows++;
    item->setRow(currentRow);
    insertRow(currentRow);
    for(int i=0;i<_numCols;++i)
    {
        int viewColumn = getViewColumn(i);
        if(viewColumn!=-1)
        {
            setItem(currentRow, viewColumn, item->getTableItem(i));
        }
    }
}

template<class View, class ViewItem, class Model, class ModelColumn>
const int TableView<View, ViewItem, Model, ModelColumn>::getViewColumn(const int trueColumnModel)
{
    if(_trueColumnToViewColumnMap.contains(trueColumnModel))
    {
        return _trueColumnToViewColumnMap[trueColumnModel];
    }
    return -1;
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setView()
{
    setRowCount(_numRows);
    setColumnCount(_numVisibleCols);
    setHeaders();
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setHeaders()
{
    Model* model = Model::Instance();
    QList<ModelColumn> modelColumns = model->getDataModelColumns();
    QList<QString> columnNames = model->getDataModelColumnNames();

    for(int i=0 ; i<modelColumns.size() ; ++i)
    {
        if(model->IsDefault(modelColumns[i]))
        {
            _header <<  columnNames[i];
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

     QList<ModelColumn> modelColumns = Model::Instance()->getDataModelColumns();
     QList<QString> columnNames = Model::Instance()->getDataModelColumnNames();

     for(int i=0;i<columnNames.size();++i)
     {
         QAction* nAction = new QAction(columnNames[i], this);
         nAction->setCheckable(true);

         if(Model::Instance()->IsDefault(modelColumns[i]))
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
    if(_viewColumnToTrueColumnMap.contains(_headerColumnClicked))
    {
        int trueColumn = _viewColumnToTrueColumnMap[_headerColumnClicked];
        _customizeActions[trueColumn]->setChecked(false);
    }

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
void TableView<View, ViewItem, Model, ModelColumn>::modifyHeader(const int fullModelColumn)
{
    setSortingEnabled (false);
    //this is the true mode column
    //check if it's checked
    if(_customizeActions[fullModelColumn]->isChecked())
    {
        //now you gotta add this %##%@#%
        //now check if it was alread there, inthat case just unhide it
        if(_trueColumnToViewColumnMap.contains(fullModelColumn))
        {
            int viewColumn = _trueColumnToViewColumnMap[fullModelColumn];
            showColumn(viewColumn);
            //_numVisibleCols++;
        }
        else
        {
            _trueColumnToViewColumnMap[fullModelColumn] = _numVisibleCols;
            _viewColumnToTrueColumnMap[_numVisibleCols] = fullModelColumn;

            insertColumn(_numVisibleCols);

            setHorizontalHeaderItem(_numVisibleCols, new QTableWidgetItem(Model::Instance()->getColumnName(fullModelColumn)));

            for(int i=0;i<_numRows;++i)
            {
                ViewItem* vItem = static_cast<TableCellItem<ViewItem>*>(item(i,0))->parent();
                setItem(i, _numVisibleCols, vItem->getTableItem(fullModelColumn));
            }

            //hideColumn(_numVisibleCols);
            //showColumn(_numVisibleCols);
            _numVisibleCols++;
            //_header << Model::Instance()->getColumnName(fullModelColumn);
        }
    }
    else
    {
        //remove this
        //for removing just hide it
        //first get the view column
        if(_trueColumnToViewColumnMap.contains(fullModelColumn))
        {
            int viewColumn = _trueColumnToViewColumnMap[fullModelColumn];
            hideColumn(viewColumn);
            //_numVisibleCols--;
        }
    }

    QSize size = horizontalHeader()->size();

    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
    }

//     size = horizontalHeader()->size();

//    if(int num = horizontalHeader()->count()-horizontalHeader()->hiddenSectionCount())
//    {
//        horizontalHeader()->setDefaultSectionSize(size.rwidth()/num);
//    }

    setSortingEnabled (_isSortingAllowed);
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
