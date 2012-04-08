#include "Platform/View/TableItem.h"

TableItem::TableItem():QTableWidgetItem()
{
    setTextAlignment(Qt::AlignCenter);
    //setTextAlignment(Qt::AlignVCenter);
    //setBackgroundColor(Qt::black);
    //setForeground(Qt::white);
    setText("");
}

TableItem::~TableItem()
{}

//void TableItem::setData(int role, const QVariant &value)
//{}

void TableItem::updateItem(const double value)
{
    setText(QString::number(value));
}

void TableItem::updateItem(const int value)
{
     setText(QString::number(value));
}

void TableItem::updateItem(const long value)
{
     setText(QString::number(value));
}

void TableItem::updateItem(const float value)
{
     setText(QString::number(value));
}

void TableItem::updateItem(const std::string& str)
{
     setText(QString::fromStdString(str));
}

void TableItem::updateItem(const QString& str)
{
     setText(str);
}


