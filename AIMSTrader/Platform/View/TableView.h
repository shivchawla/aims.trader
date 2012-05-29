#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableWidget>
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>
#include <QLabel>
#include <algorithm>
#include "Platform/View/TableItem.h"

template<class View>
class TableView : public QTableWidget
{
    protected:
        int _numRows;
        int _numCols;
        QStringList _header;

    public:
        TableView(QWidget* parent);
        ~TableView();

    public:
       void init();
       void resizeEvent(QResizeEvent* event);
       void closeEvent(QCloseEvent* event);
       void updateItem(int row, int column, const QString& data);
       TableItem* getTableItem(const int, const int);
       void editHeader(const QString);
};

template <class View>
TableView<View>::TableView(QWidget* parent):QTableWidget(parent)
{
    _numRows=0;
    _numCols=0;
    init();
}

template <class View>
TableView<View>::~TableView()
{}

template <class View>
void TableView<View>::init()
{
    setUpdatesEnabled(true);
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //setRowCount(20);
     //resize(1000,500);
    //setSizePolicy();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    //setBackgroundRole(1);
    hHeader->setResizeMode(QHeaderView::Interactive);
    hHeader->setMovable(true);
    //hHeader->setStretchLastSection(true);
    hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //hHeader->setDefaultSectionSize();
    QHeaderView* vHeader = QTableWidget::verticalHeader();
    vHeader->setDefaultSectionSize(15);


    QPalette p = palette();
    //p.setColor(QPalette::Base, Qt::black);
    setPalette(p);

    setSortingEnabled(true);

    setShowGrid(false);
    setAlternatingRowColors(true);
}

/*void TableView::moveEvent(QMoveEvent * event)
{
    QPoint position = event->pos();
}*/

template <class View>
void TableView<View>::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setDefaultSectionSize(size.rwidth()/(hHeader->count()));

}

template <class View>
void TableView<View>::closeEvent(QCloseEvent *event)
{
    hide();
}

template <class View>
void TableView<View>::updateItem(int row, int column, const QString& data)
{
    TableItem *item = getTableItem(row, column);
    if (!item)
    {
        item = new TableItem();
        setItem(row, column, item);
    }
    item->setText(data);
    emit itemChanged(item);
}

template <class View>
TableItem* TableView<View>::getTableItem(const int row, const int column)
{
    return static_cast<TableItem*>(item(row, column));
}

/*template<class View>
void TableView<View>::closeEvent(QCloseEvent *event)
{
    emit closed();
}*/

template <class View>
void TableView<View>::editHeader(const QString column)
{
    _header<<column;
}


#endif // TABLEVIEW_H
