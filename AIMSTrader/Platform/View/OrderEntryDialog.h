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
#include "Platform/typedefs.h"

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
        void onChangeOrderSide(int index);
        void onChangeOrderType(QString);

    private:
        void setOrderSide(const QString);
        void setQuantity();
        void setSymbol(const QString&);
        void setLimitPrice(const double);

    public:
        const int getQuantity();
        const double getLimitPrice();
        const String getOrderSide();

    public:
        void setupDialog(QString& action, const TickerId);

};

#endif // ORDEENTRYDIALOG_H
