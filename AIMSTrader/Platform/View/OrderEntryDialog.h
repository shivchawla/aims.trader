#pragma once
#ifndef ORDEENTRYDIALOG_H
#define ORDEENTRYDIALOG_H

class QTableWidget;
class QComboBox;
class QSpinBox;
class QFormLayout;
class QScrollBar;
class QDialogButtonBox;
class QLabel;
class QDoubleSpinBox;
class QLineEdit;

#include <QDialog>
#include "AimsTraderDefs/typedefs.h"
#include "InteractiveBroker/Shared/Order.h"


class OrderEntryDialog: public QDialog
{
    Q_OBJECT
    private:
        QDialogButtonBox* _buttonBox;
        QComboBox* spreadTypeBox;
        QComboBox* orderSideBox;
        QSpinBox* quantityBox;
        QLineEdit* symbol;
        QComboBox* expiryBox;
        QComboBox* strikeBox;
        QComboBox* instrumentTypeBox;
        QDoubleSpinBox* priceBox;
        QComboBox* orderTypeBox;
        QComboBox* exchangeBox;
        QComboBox* goodTillBox;
        double limitprice;

    public:
        OrderEntryDialog(QWidget* parent=0);

    private slots:
        void accept();
        void reject();
        void onChangeOrderSide(const int);
        void onChangeOrderType(const int);

    private:
        void setOrderSide(const OrderSide side = BUY);
        void setOrderSide(const QString side = "BUY");
        void setQuantity(const int quantity = 100);
        void setSymbol(const QString&);
        void setLimitPrice(const double);
        void setOrderType(const OrderType type = LMT);

    public:
        const int getQuantity();
        const double getLimitPrice();
        const OrderSide getOrderSide();
        const OrderType getOrderType();

    public:
        void setupDialog(const TickerId, const Order& order);

};

#endif // ORDEENTRYDIALOG_H
