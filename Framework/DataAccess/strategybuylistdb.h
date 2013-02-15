#include <QObject>
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "../Shared/DataAccess/DbBase.h"
#include "Data/strategybuylistdata.h"
#include "DataAccess/DbTask.h"

class ATContract;

class StrategyBuyListDb : public DbBase
{
    private:
        enum StrategyBuyList
        {
            InstrumentId,
            DeactivatedDate
        };

    public:
        StrategyBuyListDb();

        //Methods
        StrategyBuyListData getStrategyBuyListById(const uint strategyId);
        /*QList<StrategyBuyListData> getStrategyBuyLists();
        QList<StrategyBuyListData> getStrategyBuyListsForStrategy(const QString& strategyName);
        uint insertStrategyBuyList(const StrategyBuyListData& data);
        uint insertStrategyBuyList(uint strategyId, uint instrumentId, QDateTime deactivatedDate);
        uint updateStrategyBuyList(const StrategyBuyListData& data);
        uint deleteStrategyBuyList(const uint &id);

        QList<ATContract> getATContractsForStrategy(const QString &strategyName);
        std::string getSecurityTypeForVendor(const quint8 &instrumentType, int vendorCode);*/

};

