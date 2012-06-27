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

OrderEntryDialog::OrderEntryDialog(QWidget* parent):QDialog(parent, Qt::Sheet)//;,~Qt::WindowMaximizeButtonHint)
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
    orderSideBox->addItem("SHORTSELL");

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
    orderTypeBox->addItem("MKT");
    orderTypeBox->addItem("MKTCLS");
    orderTypeBox->addItem("LMT");
    orderTypeBox->addItem("LMTCLS");
    orderTypeBox->addItem("PEGMKT");
    orderTypeBox->addItem("SCALE");
    orderTypeBox->addItem("STP");
    orderTypeBox->addItem("STPLMT");
    orderTypeBox->addItem("TRAIL");
    orderTypeBox->addItem("REL");
    orderTypeBox->addItem("VWAP");
    orderTypeBox->addItem("TRAILLIMIT");

    formLayout->addRow("OrderType", orderTypeBox);
    connect(orderTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeOrderType(int)));

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

void OrderEntryDialog::setOrderType(const OrderType type)
{
    orderTypeBox->setCurrentIndex(type);
}

void OrderEntryDialog::setOrderSide(const OrderSide side)
{
    quantityBox->setValue(100);
    if(side == BUY)
    {
        orderSideBox->setCurrentIndex(BUY);
    }
    else if(side == SELL)
    {
        orderSideBox->setCurrentIndex(SELL);
    }
    else
    {
        orderSideBox->setCurrentIndex(SHORTSELL);
    }
}

void OrderEntryDialog::setOrderSide(const QString side)
{
    quantityBox->setValue(100);
    if(side == "BUY")
    {
        orderSideBox->setCurrentIndex(BUY);
    }
    else if(side == "SELL")
    {
        orderSideBox->setCurrentIndex(SELL);
    }
    else
    {
        orderSideBox->setCurrentIndex(SHORTSELL);
    }
}

void OrderEntryDialog::setQuantity(const int quantity)
{
      quantityBox->setValue(quantity);
}

void OrderEntryDialog::setSymbol(const QString& sym)
{
    symbol->setText(sym);
}

void OrderEntryDialog::setLimitPrice(const double price)
{
    if(orderSideBox->currentIndex() == BUY)
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

void OrderEntryDialog::onChangeOrderSide(const int side)
{
    setOrderSide((OrderSide)side);
}

void OrderEntryDialog::onChangeOrderType(const int type)
{
    if(type == MKT)
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

void OrderEntryDialog::setupDialog(const TickerId tickerId, const Order order)
{
    ATContract aTcontract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);

    setSymbol(QString::fromStdString(aTcontract.contract.symbol));

    setOrderSide(QString::fromStdString(order.action));

    if(order.action=="BUY")
    {
        setLimitPrice(Service::Instance()->getInstrumentManager()->getAskPrice(tickerId));
    }
    else if(order.action == "SELL")
    {
        setLimitPrice(Service::Instance()->getInstrumentManager()->getBidPrice(tickerId));
    }

    if(order.totalQuantity)
    {
        setQuantity(order.totalQuantity);
    }

    if(order.orderType != "")
    {
        setOrderType(LMT);
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
    if(orderTypeBox->currentIndex() == LMT)
    {
        return priceBox->value();
    }
    return 0;
}

const OrderSide OrderEntryDialog::getOrderSide()
{
    return static_cast<OrderSide>(orderSideBox->currentIndex());
}

const OrderType OrderEntryDialog::getOrderType()
{
    return static_cast<OrderType>(orderTypeBox->currentIndex());
}

