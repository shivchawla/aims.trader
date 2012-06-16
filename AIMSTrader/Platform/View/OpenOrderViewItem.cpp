#include "Platform/View/OpenOrderViewItem.h"

OpenOrderViewItem::OpenOrderViewItem(const int numItems):TableViewItem<OpenOrderViewItem>(numItems)
{
//    for(int i=0;i<_numItems;++i)
//    {
//        OpenOrderModelColumn col = OpenOrderModel::Instance()->getColumns()[i];
//        _map[(int)col] = i;
//    }
}

OpenOrderViewItem::~OpenOrderViewItem()
{}

//void OpenOrderViewItem::update(const QString& value, const OpenOrderModelColumn column)
//{
//    int col = (int)column;
//    if(_map.contains(col))
//    {
//        _items[_map[col]]->updateItem(value);
//    }
//}
