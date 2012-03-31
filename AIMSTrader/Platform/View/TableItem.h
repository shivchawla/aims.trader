#ifndef TABLEITEM_H
#define TABLEITEM_H
#include <QTableWidgetItem>

class TableItem : public QTableWidgetItem
{
    public:
        TableItem();
        ~TableItem();

    public:
        void updateItem(const double value);
        void updateItem(const int value);
        void updateItem(const long value);
        void updateItem(const float value);
        void updateItem(const std::string& str);
        void updateItem(const QString& str);

    public:
       // void setData(int role, const QVariant &value);
};

#endif // TABLEITEM_H
