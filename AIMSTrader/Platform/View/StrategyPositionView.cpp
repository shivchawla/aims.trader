#include "Platform/View/StrategyPositionView.h"
#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/StrategyPositionViewItem.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

StrategyPositionView::StrategyPositionView(QWidget* parent=0):TableView<StrategyPositionView>(parent)
{
    _numCols = StrategyPositionModel::getDefaultNumColumns();
    //row for total stats
    _numRows=0;
    setPositionView();
}

void StrategyPositionView::setPositionView()
{
    setMinimumSize(_numCols*80, 200);
    setRowCount(_numRows);
    setColumnCount(_numCols);
    setHeaders();

    //set the total
    /*StrategyPositionViewItem* newItem = new StrategyPositionViewItem();
    _positionMap[0][0] = newItem;
    int currentRow = _numRows++;
    insertRow(currentRow);
    int numItems = StrategyPositionViewItem::getNumItems();
    for(int i=0;i<numItems;++i)
    {
        setItem(currentRow,i,newItem->getTableItem(i));
    }

    newItem->update("Total", Strategy);
    //newItem->update(QString::number(tickerId), TickerID);*/

}

void StrategyPositionView::setHeaders()
{
    /*_header<<"Strategy"
           <<"TickerID"
           <<"InstrumentId"
           <<"Exchange"
           <<"TradePrice"
           <<"LastPrice"
           <<"Quantity"
           <<"PositionValue"
           <<"Commission"
           <<"GrossProfit"
           <<"NetProfit";*/

    int cols = StrategyPositionModel::getDefaultNumColumns();

    for(int i=0;i<cols;++i)
    {
        _header<<StrategyPositionModel::getStrategyPositionModelColumnName(StrategyPositionModel::getDefaultModel()[i]);
    }
    setHorizontalHeaderLabels(_header);
}


StrategyPositionViewItem* StrategyPositionView::getPositionViewItem(const StrategyId strategyId, const PositionId positionId)
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

StrategyPositionView::~StrategyPositionView()
{}

void StrategyPositionView::update()
{}

/*void PositionView::addPosition(const StrategyId strategyId, const PositionId positionId, const TickerId tickerId)
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

}*/

void StrategyPositionView::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
      StrategyPositionViewItem* newItem = new StrategyPositionViewItem();
      _positionMap[strategyId][tickerId] = newItem;
      int currentRow = _numRows++;
      insertRow(currentRow);
      int numItems = StrategyPositionViewItem::getNumItems();
      for(int i=0;i<numItems;++i)
      {
          setItem(currentRow,i,newItem->getTableItem(i));
      }

      newItem->update(QString::number(strategyId), Strategy);
      //newItem->update(QString::number(positionId), PositionID);

      //get startegy name from strategyManager
      newItem->update(Service::Instance()->getInstrumentManager()->getInstrumentId(tickerId), InstrumentId);
      newItem->update(StrategyManager::getStrategyName(strategyId), Strategy);
}

void StrategyPositionView::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
           StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];

           item->update(QString::number(sharesBought), Buys);
           item->update(QString::number(sharesSold), Sells);
           item->update(QString::number(netShares), Net);
           item->update(QString::number(avgBought), AvgBT);
           item->update(QString::number(avgSold), AvgSLD);
           item->update(QString::number(totalValueBought), TotalBT);
           item->update(QString::number(totalValueSold), TotalSLD);
           item->update(QString::number(totalValueBought-totalValueSold), NetTotal);
           item->update(QString::number(realizedPnl), RealizedPL);
           item->update(QString::number(runningPnl), UnRealizedPL);
           item->update(QString::number(realizedPnl+runningPnl), PL);
           item->update(QString::number(-totalValueBought+totalValueSold-totalCommision), NetInclCommission);
        }
   }
}

void StrategyPositionView::updatePositionForLastPrice(const StrategyId strategyId, const TickerId tickerId, const double runningPnl, const double pnl)
{
    if(_positionMap.count(strategyId)!=0)
    {
        if(_positionMap[strategyId].count(tickerId)!=0)
        {
            StrategyPositionViewItem* item = _positionMap[strategyId][tickerId];
            item->update(QString::number(pnl), PL);
            item->update(QString::number(runningPnl), UnRealizedPL);
        }
    }
}

void StrategyPositionView::removePosition(const StrategyId strategyId, const TickerId tickerId)
{
    if(_positionMap.count(strategyId))
    {
        if(_positionMap[strategyId].count(tickerId))
        {
            StrategyPositionViewItem* item  = _positionMap[strategyId][tickerId];
            int rowNum = row(item->getTableItem(0));
            _positionMap[strategyId].erase(tickerId);
            removeRow(rowNum);
            _numRows--;
        }
    }
}

