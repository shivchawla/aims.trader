#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableWidget>
#include <QSize>
#include <QResizeEvent>
#include <QHeaderView>
#include <QLabel>
#include <algorithm>
#include "Platform/View/TableItem.h"

class TableView : public QTableWidget
{
    Q_OBJECT
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

     public slots:
       void switchView();
};

#endif // TABLEVIEW_H
