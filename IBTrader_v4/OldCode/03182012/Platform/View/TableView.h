#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableWidget>
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>
#include <algorithm>

template <class View>
class TableView : public QTableWidget
{
    protected:
        int _numRows;
        int _numCols;
        QStringList _header;

    public:
        TableView();
        ~TableView();

    public:
       void init();
       void resizeEvent(QResizeEvent* event);
};

template<class View>
TableView<View>::TableView():QTableWidget()
{
    _numRows=0;
    _numCols=0;
    init();
}

template<class View>
TableView<View>::~TableView()
{}

template<class View>
void TableView<View>::init()
{
    verticalHeader()->hide();
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    //setRowCount(20);
    resize(1200,500);

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
    //setShowGrid(false);
}

template<class View>
void TableView<View>::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();
    QHeaderView* hHeader = QTableWidget::horizontalHeader();
    hHeader->setDefaultSectionSize(size.rwidth()/(hHeader->count()));
}

#endif // TABLEVIEW_H
