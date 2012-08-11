#pragma once
#ifndef TABLEITEM_H
#define TABLEITEM_H
#include <QTableWidgetItem>
#include <QDebug>

template <class ViewItem>
class TableCellItem : public QTableWidgetItem
{
    private:
         ViewItem* _parent;

    public:
        TableCellItem(ViewItem* parent=0);

    public:
        ///destructor
        ~TableCellItem()
        {}

        void updateItem(const double value)
        {
            //setText(QString::number(value));
            setData(Qt::EditRole, value);
        }

        void updateItem(const int value)
        {
             //setText(QString::number(value));
             setData(Qt::EditRole, value);
        }

        void updateItem(const uint value)
        {
             //setText(QString::number(value));
             setData(Qt::EditRole, value);
        }

        void updateItem(const float value)
        {
             //setText(QString::number(value));
             setData(Qt::EditRole, value);
        }

        void updateItem(const std::string& str)
        {
            if(str=="")
            {
                qDebug()<<"Problem";
            }
             //setText(QString::fromStdString(str));
             setData(Qt::EditRole, QString::fromStdString(str));
        }

        void updateItem(const QString& str)
        {
            if(str=="")
            {
                qDebug()<<"Problem";
            }

             //setText(str);
             setData(Qt::EditRole, str);
        }

        const ViewItem* parent() const
        {
            return _parent;
        }

        const ViewItem* parent()
        {
            return _parent;
        }
};

///constructor
template<class ViewItem>
TableCellItem<ViewItem>::TableCellItem(ViewItem* parent):QTableWidgetItem()
{
    _parent = parent;
    setTextAlignment(Qt::AlignCenter);
    //setBackgroundColor(Qt::black);
    setForeground(Qt::white);
    setText("");
    setFont(QFont("Cambria"));
    //setFont();
    //setSizeHint();
}

#endif // TABLEITEM_H
