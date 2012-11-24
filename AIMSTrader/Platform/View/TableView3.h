#ifndef TABLEVIEW3_H
#define TABLEVIEW3_H

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
#include "Platform/View/TableViewItem2.h"

template<class View, class ViewItem, class Model>
class TableView3 : public QTableWidget
{
    protected:
        Model* _model;
        ViewItem* _clickedItem;
        int _numRows;
        int _numCols;
        QStringList _header;
        QHash<int, int> _trueColumnToViewColumnMap;
        QHash<int, int> _viewColumnToTrueColumnMap;
        int _headerColumnClicked;
        int _numVisibleCols;
        bool _isSortingAllowed;

    protected:
         QMenu* _headerMenu;
         QMenu* _windowsMenu;
         QAction* removeAction;
         QList<QAction*> _customizeActions;
         QSignalMapper* _signalMapper;

    public:
        TableView3(QWidget* parent = 0);
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
       void editHeader(const QString);

    protected:
       const int getViewColumn(const int trueColumnModel);
       void removeHeader();
       void customizeHeader();
       //void modifyHeader(const int);
       void updateItem(const int, const int, const QVariant&, const int role = Qt::EditRole);
       void updateItemSpecial(const int row, const int col, const QVariant& value, const int role = Qt::EditRole);
       void insertItem(const int row, const int column, ViewItem* item);
       //void addRow(int row);
       void contextMenuHelper(QContextMenuEvent *event);


    protected:
        bool eventFilter(QObject *obj, QEvent *event);
};

template<class View, class ViewItem, class Model>
TableView3<View, ViewItem, Model>::TableView3(QWidget* parent):QTableWidget(parent)
{
    _model = new Model();
    _numRows = 0;
    _numCols = _model->getDataModelNumColumns();
    //_trueColumnToViewColumnMap.reserve(_numCols);
    _headerColumnClicked = -1;
    //_numVisibleCols = model->getDataModelDefaultNumColumns();
    _isSortingAllowed  = true;
    //QList<ModelColumn> modelColumns = model->getDataModelColumns();

//    int j = 0;
//    for(int i=0;i<modelColumns.size();++i)
//    {
//        if(model->IsDefault(modelColumns[i]))
//        {
//            _trueColumnToViewColumnMap[int(modelColumns[i])] = j;
//            _viewColumnToTrueColumnMap[j++] = int(modelColumns[i]);
//        }
//    }

    setupMenu();
    setView();
    setupLooks();
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::setupLooks()
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

    connect(hHeader, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

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

    //setSortingEnabled(_isSortingAllowed);

    setShowGrid(false);
    //setAlternatingRowColors(true);
    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::resizeEvent(QResizeEvent* event)
{
//    QSize size = event->size();
//    QHeaderView* hHeader = QTableWidget::horizontalHeader();
//    if(int num = hHeader->count() - hHeader->hiddenSectionCount())
//    {
//        hHeader->setDefaultSectionSize(size.rwidth()/num);
//    }
    QTableWidget::resizeEvent(event);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::closeEvent(QCloseEvent *event)
{
    hide();
}

//template<class View, class ViewItem, class Model>
//TableCellItem<ViewItem>* TableView<View, ViewItem, Model, ModelColumn>::getTableItem(const int row, const int column)
//{
//    return static_cast<TableCellItem<ViewItem>*>(item(row, column));
//}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::editHeader(const QString column)
{
    _header<<column;
}

//template<class View, class ViewItem, class Model>
//ViewItem* TableView3<View, ViewItem, Model>::addItemInView()
//{
//    setSortingEnabled (false);
//    ViewItem* nItem = new ViewItem(_numCols);
//    //_viewItems.push_back(nItem);

//    insertItem(nItem);

//    setSortingEnabled (_isSortingAllowed);
//    return nItem;
//}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::insertItem(ViewItem* item)
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

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::insertItem(const int row, const int column, ViewItem* item)
{
    setItem(row, column, item);
}


template<class View, class ViewItem, class Model>
const int TableView3<View, ViewItem, Model>::getViewColumn(const int trueColumnModel)
{
    if(_trueColumnToViewColumnMap.contains(trueColumnModel))
    {
        return _trueColumnToViewColumnMap[trueColumnModel];
    }
    return -1;
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::setView()
{
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::setHeaders()
{
    QList<QString> columnNames = _model->getDataModelColumnNames();
    int numCols = columnNames.size();
    for(int i=0 ; i<numCols ; ++i)
    {
         _header <<  columnNames[i];
    }

    setHorizontalHeaderLabels(_header);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::setupMenu()
{
    _headerMenu = new QMenu("HeaderMenu", this);
    _windowsMenu = new QMenu("More", this);
     removeAction = new QAction("Remove", this);
     _signalMapper = new QSignalMapper(this);

     QList<QString> columnNames = _model->getDataModelColumnNames();

     for(int i=0;i<columnNames.size();++i)
     {
         QAction* nAction = new QAction(columnNames[i], this);
         nAction->setCheckable(true);

         if(_model->IsDefault(i))
         {
            nAction->setChecked(true);
         }

         _customizeActions.append(nAction);
         _signalMapper->setMapping(nAction, i);
         connect(_customizeActions[i], SIGNAL(triggered()), _signalMapper, SLOT(map()));
     }

     _headerMenu->addAction(removeAction);
     _headerMenu->addSeparator();
     _headerMenu->addMenu(_windowsMenu);
     _windowsMenu->addActions(_customizeActions);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::removeHeader()
{
    //setSortingEnabled (false);
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
     //setSortingEnabled (_isSortingAllowed);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::customizeHeader()
{

}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::headerContextMenuEvent(QContextMenuEvent*  event)
{
    _headerColumnClicked = horizontalHeader()->logicalIndexAt(event->x(),event->y()); 
    event->accept();
    _headerMenu->exec(event->globalPos());
}

template<class View, class ViewItem, class Model>
bool TableView3<View, ViewItem, Model>::eventFilter(QObject *obj, QEvent *event)
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

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::addHeader(const QString& header)
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

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::updateItem(const int row, const int col, const QVariant& value, const int role)
{
    item(row,col)->setData(role, value);
}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::updateItemSpecial(const int row, const int col, const QVariant& value, const int role)
{
    double oldValue = item(row,col)->data(role).toDouble();
    item(row,col)->setData(role, value);

    double newValue = value.toDouble();


    if(oldValue > 0 && newValue < 0)
    {
        item(row,col)->setData(Qt::TextColorRole, Qt::red);
    }
    else if(oldValue < 0 && newValue > 0)
    {
        item(row,col)->setData(Qt::TextColorRole, Qt::green);
    }

    if(newValue == 0)
        item(row,col)->setData(Qt::TextColorRole, Qt::white);

}

template<class View, class ViewItem, class Model>
void TableView3<View, ViewItem, Model>::contextMenuHelper(QContextMenuEvent *event)
{
    _clickedItem = static_cast<ViewItem*>(itemAt(QPoint(event->x(), event->y())));
}


#endif // TABLEVIEW3_H
