#include "Platform/View/TableView.h"
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>

TableView::TableView():QTableView()
{
    init();
}

TableView::~TableView()
{}

void TableView::init()
{
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //setRowCount(20);
    resize(1200,500);

    //setSizePolicy();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* hHeader = QTableView::horizontalHeader();
    //setBackgroundRole(1);
    hHeader->setResizeMode(QHeaderView::Interactive);
    hHeader->setMovable(true);
    hHeader->setStretchLastSection(true);
    hHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void TableView::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();
    QHeaderView* hHeader = QTableView::horizontalHeader();
    hHeader->setDefaultSectionSize(size.rwidth()/hHeader->count());
}

