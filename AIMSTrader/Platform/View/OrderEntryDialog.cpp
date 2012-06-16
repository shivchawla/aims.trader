#include "Platform/View/OrderEntryDialog.h"

#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QScrollBar>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"


OrderEntryDialog::OrderEntryDialog(QWidget* parent):QDialog(parent, Qt::Dialog)//;,~Qt::WindowMaximizeButtonHint)
{
    limitprice=0;
    setModal(false);
    QWidget::setWindowTitle("Order Entry");
    QFormLayout* formLayout = new QFormLayout();

    QStringList headerList;
    headerList << "SpreadType" << "Side" << "Quantity" << "Symbol" <<"Expiry" << "Strike" << "InstrumentType" << "Price"
            << "OrderType" << "Exchange" << "GoodTill";

    //add Item
    spreadTypeBox = new QComboBox();
    spreadTypeBox->addItem("STOCK");
    formLayout->addRow("SpreadType",spreadTypeBox);

    orderSideBox = new QComboBox();
    orderSideBox->addItem("BUY");
    orderSideBox->addItem("SELL");
    formLayout->addRow("Side", orderSideBox);
    connect(orderSideBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeOrderSide(int)));

    quantityBox = new QSpinBox();
    quantityBox->setSingleStep(100);
    quantityBox->setMaximum(1000000000);
    quantityBox->setMinimum(0);
    quantityBox->setAlignment(Qt::AlignRight);
    formLayout->addRow("Quantity", quantityBox);

    symbol = new QLineEdit();
    formLayout->addRow("Symbol", symbol);


    expiryBox = new QComboBox();
    formLayout->addRow("Expiry",expiryBox);

    strikeBox = new QComboBox();
    formLayout->addRow("Strike", strikeBox);

    instrumentTypeBox = new QComboBox();
    instrumentTypeBox->addItem("STOCK");
    formLayout->addRow("InstrumentType", instrumentTypeBox);


    priceBox = new QDoubleSpinBox();
    priceBox->setSingleStep(0.01);
    priceBox->setMaximum(100000);
    priceBox->setAlignment(Qt::AlignRight);
    formLayout->addRow("Price", priceBox);

    orderTypeBox = new QComboBox();
    orderTypeBox->addItem("LIMIT");
    orderTypeBox->addItem("MARKET");
    orderTypeBox->addItem("STOP");
    orderTypeBox->addItem("STOPLIMIT");
    orderTypeBox->addItem("MARKET");
    formLayout->addRow("OrderType", orderTypeBox);
    connect(orderTypeBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onChangeOrderType(QString)));

    exchangeBox = new QComboBox();
    exchangeBox->addItem("SMART");
    formLayout->addRow("Exchange", exchangeBox);

    goodTillBox = new QComboBox();
    goodTillBox->addItem("DAY");
    formLayout->addRow("Time In Force", goodTillBox);

    _buttonBox  = new QDialogButtonBox();

     QPushButton* deletePushButton = new QPushButton("Delete", _buttonBox);
    _buttonBox->addButton(deletePushButton, QDialogButtonBox::RejectRole);
     connect(deletePushButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton* confirmPushButton = new QPushButton("Confirm and Send",_buttonBox);
    connect(confirmPushButton, SIGNAL(clicked()), this, SLOT(accept()));
    _buttonBox->addButton(confirmPushButton, QDialogButtonBox::AcceptRole);


    QVBoxLayout* boxLayout = new QVBoxLayout();
    boxLayout->addLayout(formLayout);
    boxLayout->addWidget(_buttonBox);

    setLayout(boxLayout);

}

void OrderEntryDialog::accept()
{


    QDialog::accept();
}

void OrderEntryDialog::reject()
{
    QDialog::reject();
}


void OrderEntryDialog::setOrderSide(const QString side)
{
    if(side=="BUY")
    {
        orderSideBox->setCurrentIndex(0);
        //quantityBox->setValue(100);
    }
    else
    {
        orderSideBox->setCurrentIndex(1);
        //quantityBox->setValue(-100);
    }
}

void OrderEntryDialog::setQuantity()
{
//    if(orderSideBox->currentIndex() == 0)
//    {
//        quantityBox->setValue(100);
//    }
//    else
//    {
//        quantityBox->setValue(-100);
//    }
}

void OrderEntryDialog::setSymbol(const QString& sym)
{
    symbol->setText(sym);
}

void OrderEntryDialog::setLimitPrice(const double price)
{
    if(orderSideBox->currentIndex() == 0)
    {
        limitprice = price-0.1;
        priceBox->setValue(limitprice);
    }
    else
    {
       limitprice=price + 0.1;
       priceBox->setValue(limitprice);
    }
}

void OrderEntryDialog::onChangeOrderSide(int index)
{
//    if(index==0)
//    {
//        quantityBox->setValue(100);
//    }
//    else if(index==1)
//    {
//        quantityBox->setValue(-100);
//    }
}

void OrderEntryDialog::onChangeOrderType(QString type)
{
    if(type=="MARKET")
    {
        priceBox->setValue(0);
        priceBox->setReadOnly(true);
    }
    else
    {
        priceBox->setValue(limitprice);
        priceBox->setReadOnly(false);
    }
}

void OrderEntryDialog::setupDialog(QString& action, const TickerId tickerId)
{
    Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);

    setSymbol(QString::fromStdString(contract.symbol));
    setOrderSide(action);

    if(action=="BUY")
    {
        setLimitPrice(Service::Instance()->getInstrumentManager()->getAskPrice(tickerId));
    }
    else
    {
        setLimitPrice(Service::Instance()->getInstrumentManager()->getBidPrice(tickerId));
    }

    show();
    raise();
    activateWindow();
}


const int OrderEntryDialog::getQuantity()
{
   return quantityBox->value();
}

const double OrderEntryDialog::getLimitPrice()
{
    if(orderTypeBox->currentText()=="LIMIT")
    {
        return priceBox->value();
    }
    return 0;
}

const String OrderEntryDialog::getOrderSide()
{
    return orderSideBox->currentText();
}


