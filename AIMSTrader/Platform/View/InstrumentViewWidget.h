#ifndef INSTRUMENTVIEWWIDGET_H
#define INSTRUMENTVIEWWIDGET_H

#include <QWidget>

class InstrumentView2;
class SearchLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class OrderEntryDialog;

class InstrumentViewWidget : public QWidget
{
    Q_OBJECT
    private:
        InstrumentView2* _instrumentView;
        SearchLineEdit* _searchLineEdit;
        QListWidget* _listWidget;
        int _numInstrumentsInListWidget;

    private:
        QMenu* _instrumentMenuA;
        QAction* _addNewInstrument;
        QAction* _closeAll;
        QAction* _buyAction;
        QAction* _sellAction;
        QMenu* _instrumentMenuB;
        QAction* _symbolLookup;
        OrderEntryDialog* _orderEntryDialog;

    public:
        InstrumentViewWidget(QWidget* parent = 0);

    private:
        void setupActions();

    public:
        void resizeEvent(QResizeEvent *);

    private slots:
        void updateSearch(const QString&);
        void onInstrumentSelection(QListWidgetItem* );
};

#endif // INSTRUMENTVIEWWIDGET_H
