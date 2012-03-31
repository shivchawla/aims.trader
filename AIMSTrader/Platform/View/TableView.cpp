#include "Platform/View/TableView.h"

TableView::TableView(QWidget* parent):QTableWidget(parent)
{
    _numRows=0;
    _numCols=0;
    init();
}

TableView::~TableView()
{}

void TableView::init()
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
    hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    setPalette(p);

    setSortingEnabled(true);

    setShowGrid(false);
}

/*void TableView::moveEvent(QMoveEvent * event)
{
    QPoint position = event->pos();
}*/

void TableView::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setDefaultSectionSize(size.rwidth()/(hHeader->count()));

}

void TableView::closeEvent(QCloseEvent *event)
{
    hide();
}


void TableView::updateItem(int row, int column, const QString& data)
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

TableItem* TableView::getTableItem(const int row, const int column)
{
    return static_cast<TableItem*>(item(row, column));
}

void TableView::switchView()
{
    if(size().rwidth()==0)
    {
        show();
    }
    else
    {
        hide();
    }
}
