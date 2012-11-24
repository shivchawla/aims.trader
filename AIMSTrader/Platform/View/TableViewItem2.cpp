#include "Platform/View/TableViewItem2.h"

TableViewItem2::TableViewItem2(): QTableWidgetItem()
{
    setTextAlignment(Qt::AlignCenter);
    //setBackgroundColor(Qt::black);
    setForeground(Qt::white);
    setText("");
    setFont(QFont("Cambria"));
}
