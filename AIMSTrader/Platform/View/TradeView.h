#ifndef TRADEVIEW_H
#define TRADEVIEW_H

#include "Platform/View/TableView.h"
#include "Platform/View/TradeViewItem.h"
#include "Platform/Model/TradeModel.h"

class TradeView : public TableView<TradeView, TradeViewItem, TradeModel, TradeModelColumn>
{
    Q_OBJECT
    public:
        TradeView(QWidget* parent = 0);

    public slots:
        void addTrade(const OrderDetail&);
};

#endif // TRADEVIEW_H
