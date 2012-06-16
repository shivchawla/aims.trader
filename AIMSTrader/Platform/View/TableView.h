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
        int _visibleColumns;

    protected:
         //CustomizeHeaderDialog* _dialog;
         QMenu* _headerMenu;
         QMenu* _windowsMenu;
         QAction* removeAction;
         QList<QAction*> _customizeActions;
         QSignalMapper* _signalMapper;

    public:
        TableView(QWidget* parent);
        ~TableView();

    private:
        void setView();
        void setupMenu();
        void setHeaders();
        //void populateHeaderListWidget();
        void headerContextMenuEvent(QContextMenuEvent*  );

    public:
       void setupLooks();
       void resizeEvent(QResizeEvent* event);
       void closeEvent(QCloseEvent* event);
       TableCellItem<ViewItem>* getTableItem(const int, const int);
       void editHeader(const QString);

    protected:
       ViewItem* addItemInView();
       void addColumnInView();
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
    _numCols = model->getDataModelDefaultNumColumns();
    _trueColumnToViewColumnMap.reserve(_numCols);
    //_viewColumnToTrueColumnMap.reserve(_numCols);
    _headerColumnClicked = -1;
    _visibleColumns = _numCols;

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
    //populateHeaderListWidget();
    setView();
    setupLooks();
}

template<class View, class ViewItem, class Model, class ModelColumn>
TableView<View, ViewItem, Model, ModelColumn>::~TableView()
{}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setupLooks()
{
    setUpdatesEnabled(true);
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setResizeMode(QHeaderView::Interactive);
   // hHeader->setMovable(true);
    //hHeader->setHighlightSections(false);
    //hHeader->setSortIndicatorShown(false);

    //hHeader->setContextMenuPolicy(Qt::DefaultContextMenu);
    //hHeader->setContextMenuPolicy(Qt::ActionsContextMenu);
   // hHeader->addAction(removeAction);
    //hHeader->addAction(customizeAction);
    hHeader->installEventFilter(this);


    //hHeader->setStretchLastSection(true);
    hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHeaderView* vHeader = QTableWidget::verticalHeader();
    vHeader->setDefaultSectionSize(15);

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    setPalette(p);

    setSortingEnabled(true);

    setShowGrid(false);
    //setAlternatingRowColors(true);
}

/*void TableView::moveEvent(QMoveEvent * event)
{
    QPoint position = event->pos();
}*/

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setDefaultSectionSize(size.rwidth()/(hHeader->count()));
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
    ViewItem* nItem = new ViewItem(_numCols);
    _viewItems.push_back(nItem);
    int currentRow = _numRows++;
    insertRow(currentRow);
    for(int i=0;i<_numCols;++i)
    {
        setItem(currentRow,i,nItem->getTableItem(i));
    }
    return nItem;
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::addColumnInView()
{
    for(int i=0;i<_numRows;++i)
    {
        _viewItems[i]->addCell();
    }

    insertColumn(_numCols++);
    _visibleColumns++;
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
    setColumnCount(_numCols);
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

//template<class View, class ViewItem, class Model, class ModelColumn>
//void TableView<View, ViewItem, Model, ModelColumn>::populateHeaderListWidget()
//{
//    Model* model = Model::Instance();
//    int sourceNumCols = model->getSourceModelNumColumns();

//    for(int i=0;i<sourceNumCols;++i)
//    {
//        _dialog->addItemInSource(model->getSourceModelColumns()[i], model->getSourceModelColumnNames()[i]);
//    }

//    int defaultNumCols = model->getDefaultModelNumColumns();
//    for(int i=0;i<defaultNumCols;++i)
//    {
//        _dialog->addItemInTarget(model->getDefaultModelColumns()[i], model->getDefaultModelColumnNames()[i]);
//    }
//}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::setupMenu()
{
    //_dialog = new CustomizeHeaderDialog(this);
    //_dialog->hide();
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
    hideColumn(_headerColumnClicked);
    _visibleColumns--;
    if(_viewColumnToTrueColumnMap.contains(_headerColumnClicked))
    {
        int trueColumn = _viewColumnToTrueColumnMap[_headerColumnClicked];
        _customizeActions[trueColumn]->setChecked(false);
    }

    QSize size = horizontalHeader()->size();
    int x=1;
    if(_visibleColumns)
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/_visibleColumns);
    }
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::customizeHeader()
{

}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::headerContextMenuEvent(QContextMenuEvent*  event)
{
    //QTableWidgetItem* item = itemAt(event->x(),event->y());
    _headerColumnClicked = horizontalHeader()->logicalIndexAt(event->x(),event->y());
    //_headerColumnClicked = itemAt(event->x(),event->y())->column();
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
            //_headerRightClicked = true;
            QContextMenuEvent* e = static_cast<QContextMenuEvent*>(event);
            headerContextMenuEvent(e);
            return true;
        }
    }
}

template<class View, class ViewItem, class Model, class ModelColumn>
void TableView<View, ViewItem, Model, ModelColumn>::modifyHeader(const int fullModelColumn)
{
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
            _visibleColumns++;
        }
        else
        {
            _trueColumnToViewColumnMap[fullModelColumn] = _numCols;
            _viewColumnToTrueColumnMap[_numCols] = fullModelColumn;
             addColumnInView();
            _header << Model::Instance()->getColumnName(fullModelColumn);
            setHorizontalHeaderLabels(_header);
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
            _visibleColumns--;
        }
    }

    QSize size = horizontalHeader()->size();
    if(_visibleColumns)
    {
        horizontalHeader()->setDefaultSectionSize(size.rwidth()/_visibleColumns);
    }
}

#endif // TABLEVIEW_H
