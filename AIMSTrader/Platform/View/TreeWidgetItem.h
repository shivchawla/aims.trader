#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QTreeWidgetItem>
template<class ModelColumn>
class TreeWidgetItem : public QTreeWidgetItem
{
    private:

    public:
        TreeWidgetItem(QWidget* parent = 0);

};

template<class ModelColumn>
TreeWidgetItem::TreeWidgetItem(QWidget* parent) :QTreeWidgetItem(parent)
{}


#endif // TREEWIDGETITEM_H
