#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QGridLayout>
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/StrategyView.h"

class StrategyModel;
class OpenOrderModel;

class MainFrame : public QWidget
{
   private:
       //QGridLayout* frame;
      // QTableView* view1;
       //QTableView* view2;

    public:
        MainFrame(QWidget *parent = 0);
        ~MainFrame();

    public:
        void linkStrategyModel();

    public:
        void createView(StrategyModel*);
        void createView(OpenOrderModel*);

    public:
       // void init();
        void populateStrategies();
};

#endif // MAINFRAME_H
