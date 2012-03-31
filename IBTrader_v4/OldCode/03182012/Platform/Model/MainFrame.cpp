#include "MainFrame.h"
#include "Platform/Model/OpenOrderModel.h"
//#include "Platform/Model/StrategyModel.h"

MainFrame::MainFrame(QWidget *parent) : QWidget(parent)
{
    /*init();*/
    //populateStrategies();
}

MainFrame::~MainFrame()
{}

/*void MainFrame::init()
{
    //frame = new QGridLayout(this);
    view1 = new OpenOrderView();
    view2 = new StrategyView();
    _openOrderModel = new OpenOrderModel();
    _strategyModel  = new StrategyModel();
    view1->setModel(_openOrderModel);
    view2->setModel(_strategyModel);
    //frame->addWidget(view1,0,0);
    //frame->addWidget(view2,1,0);
    //frame->activate();
    view1->show();
    view2->show();
    //setLayout(frame);
    //show();
}*/

/*void MainFrame::populateStrategies()
{
  Strategy* base = NULL;
  int nStrategies = base->GetRegisteredManufactoringPlantCount();
  for(int i=0;i<nStrategies;++i)
  {
    CBootStrapper<Strategy>* strategies = Strategy::GetRegisteredManufactoringPlant(i);
    //Create an instance of the requested strategy
    Strategy* strategy = strategies->CreateObject();
   // QTableWidgetItem *item = new QTableWidgetItem("shiv");
    //view2->setItem(i,0,item);
    strategy->initialize();
  }

}*/

void MainFrame::createView(StrategyModel* strategyModel)
{
    QTableView* strategyView = new StrategyView();
    //strategyView->setModel(strategyModel);
}

void MainFrame::createView(OpenOrderModel* openOrderModel)
{
    QTableView* _openOrderView = new OpenOrderView();
    _openOrderView->setModel(openOrderModel);
}

