#pragma once
#ifndef TABLEITEM_H
#define TABLEITEM_H
#include <QTableWidgetItem>
#include <QDebug>
#include <QDateTime>

template <class ViewItem>
class TableCellItem : public QTableWidgetItem
{
    private:
        QVariant _data;
        QString _stringData;
        ViewItem* _parent;

    public:
        TableCellItem(ViewItem* parent=0);

    public:
        ///destructor
        ~TableCellItem()
        {}

//        void updateItem(const QVariant& data)
//        {
//            _data = data;
//            _stringData = QString::number()
//            _stringData = data.toString();
//            //setText(QString::number(value));
//            setData(Qt::EditRole, _stringData);
//        }

        void updateItem(const int value)
        {
            _data = value;
            _stringData = QString::number(value);
            setData(Qt::EditRole, value);
        }

        void updateItem(const uint value)
        {
            _data = value;
            _stringData = QString::number(value);
            setData(Qt::EditRole, _stringData);
        }

        void updateItem(const float value)
        {
            _data = value;
            _stringData = QString::number(value, 'f', 2);
            setData(Qt::EditRole, _stringData);
        }

        void updateItem(const double value)
        {
            _data = value;
            _stringData = QString::number(value, 'f', 2);
            setData(Qt::EditRole, _stringData);
        }

        void updateItem(const std::string& str)
        {
            _data = _stringData = QString::fromStdString(str);
            setData(Qt::EditRole, _stringData);
        }

        void updateItemSpecial(const double newValue)
        {
            double oldValue = _data.toDouble();
            _data = newValue;
            _stringData = QString::number(newValue,'f', 2);
            setData(Qt::EditRole, _stringData);

            if(oldValue >= 0 && newValue < 0)
            {
                setData(Qt::TextColorRole, Qt::red);
            }
            else if(oldValue <= 0 && newValue > 0)
            {
                setData(Qt::TextColorRole, Qt::green);
            }

            if(!newValue)
                setData(Qt::TextColorRole, Qt::white);
        }

        void updateItem(const QDateTime& dateTime)
        {
            _data = dateTime;
            _stringData = dateTime.toString(Qt::ISODate);
            setData(Qt::EditRole, _stringData);
        }

        void updateItem(const QString& str)
        {
            _data = _stringData = str;
            setData(Qt::EditRole, str);
        }

        const ViewItem* parent() const
        {
            return _parent;
        }

        ViewItem* parent()
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
