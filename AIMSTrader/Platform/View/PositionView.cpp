#include "Platform/View/PositionView.h"
#include "Platform/Model/PositionModel.h"
#include "Platform/View/PositionViewItem.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

PositionView::PositionView(QWidget* parent=0):TableView<PositionView>(parent)
{
    _numCols = PositionViewItem::getNumItems();
    //row for total stats
    _numRows=0;
    setPositionView();
}

void PositionView::setPositionView()
{
    setMinimumSize(_numCols*80, 200);
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();

    //set the total
    PositionViewItem* newItem = new PositionViewItem();
    _positionMap[0][0] = newItem;
    int currentRow = _numRows++;
    insertRow(currentRow);
    int numItems = PositionViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        setItem(currentRow,i,newItem->getTableItem(i));
    }

    newItem->update("Total", Strategy);
    //newItem->update(QString::number(tickerId), TickerID);

}

void PositionView::setHeaders()
{
    _header<<"Strategy"
           <<"TickerID"
           <<"InstrumentId"
           <<"Exchange"
           <<"TradePrice"
           <<"LastPrice"
           <<"Quantity"
           <<"PositionValue"
           <<"Commission"
           <<"GrossProfit"
           <<"NetProfit";
    setHorizontalHeaderLabels(_header);
}


PositionViewItem* PositionView::getPositionViewItem(const StrategyId strategyId, const PositionId positionId)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(positionId)!=0)
        {
            return _positionMap[strategyId][positionId];
        }
    }
    return NULL;
}

PositionView::~PositionView()
{}

void PositionView::update()
{}

void PositionView::addPosition(const StrategyId strategyId, const PositionId positionId, const TickerId tickerId)
{
      PositionViewItem* newItem = new PositionViewItem();
      _positionMap[strategyId][positionId] = newItem;
      int currentRow = _numRows++;
      insertRow(currentRow);
      int numItems = PositionViewItem::getNumItems();
      for(int i=0;i<numItems;++i)
      {
          setItem(currentRow,i,newItem->getTableItem(i));
      }


      newItem->update(QString::number(strategyId), Strategy);
      newItem->update(QString::number(positionId), PositionID);

      //get startegy name from strategyManager
      newItem->update(Service::Instance()->getInstrumentManager()->getInstrumentId(tickerId), InstrumentId);
      newItem->update(StrategyManager::getStrategyName(strategyId), Strategy);

}

void PositionView::onExecutionUpdate(const StrategyId strategyId, const PositionId positionId, const int quantity, const double avgFillPrice, const double positionValue, const double commission)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(positionId)!=0)
        {
            _positionMap[strategyId][positionId]->updatePositionValue(positionValue);
            _positionMap[strategyId][positionId]->update(QString::number(commission), Commission);
            _positionMap[strategyId][positionId]->update(QString::number(quantity), Quantity);
            _positionMap[strategyId][positionId]->update(QString::number(avgFillPrice), TradePrice);
        }
   }
}

void PositionView::onTradeUpdate(const StrategyId strategyId, const PositionId positionId, const double positionValue, const double netProfit)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(positionId)!=0)
        {
            _positionMap[strategyId][positionId]->updatePositionValue(positionValue);
            _positionMap[strategyId][positionId]->updateNetProfit(netProfit);
        }
    }
}

void PositionView::removePosition(const StrategyId strategyId, const PositionId positionId)
{
    if(_positionMap.count(strategyId))
    {
        if(_positionMap[strategyId].count(positionId))
        {
            PositionViewItem* item  = _positionMap[strategyId][positionId];
            int rowNum = row(item->getTableItem(0));
            _positionMap[strategyId].erase(positionId);
            removeRow(rowNum);
            _numRows--;
        }
    }
}

