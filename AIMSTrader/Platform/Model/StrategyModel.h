#ifndef STRATEGYMODEL_H
#define STRATEGYMODEL_H
#include "Platform/typedefs.h"
#include "Platform/Utils/Enum.h"

enum StrategyModelColumn{
 StrategyID=0,
 StrategyName,
TotalTrades,
 WinningTrades,
 LosingTrades,
ProfitFactor,
NetProfit,
 TotalBought,
TotalSold,
Investment
};
/*class StrategyModelColumn: public Enum<StrategyModelColumn>
{
    private:
        explicit StrategyModelColumn(int value):Enum<StrategyModelColumn>(value){}

    public:
        static const StrategyModelColumn StrategyID=0;
        static const StrategyModelColumn StrategyName=1;
        static const StrategyModelColumn TotalTrades=2;
        static const StrategyModelColumn WinningTrades=3;
        static const StrategyModelColumn LosingTrades=4;
        static const StrategyModelColumn ProfitFactor=5;
        static const StrategyModelColumn NetProfit=6;
        static const StrategyModelColumn TotalBought=7;
        static const StrategyModelColumn TotalSold=8;
        static const StrategyModelColumn Investment=9;
};

/*class Test_Enum: public Enum<Test_Enum> {

private:
 explicit Test_Enum(int Value): Enum<Test_Enum>(Value) { }

public:
  static const Test_Enum enum_11;
  static const Test_Enum enum_12;
  static const Test_Enum enum_18;
  static const Test_Enum enum_20;
  static const Test_Enum enum_21;
};

Test_Enum.cpp

#include "Test_Enum.h"

Enum<Test_Enum>::instances_list Enum<Test_Enum>::s_instances;

const Test_Enum Test_Enum::enum_11(11);
const Test_Enum Test_Enum::enum_12(12);
const Test_Enum Test_Enum::enum_18(18);
const Test_Enum Test_Enum::enum_20(20);
const Test_Enum Test_Enum::enum_21(21);
*/


#endif // STRATEGYMODEL_H
