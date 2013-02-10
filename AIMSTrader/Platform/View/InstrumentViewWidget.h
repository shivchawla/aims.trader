#ifndef INSTRUMENTVIEWWIDGET_H
#define INSTRUMENTVIEWWIDGET_H

#include <QWidget>

class InstrumentView;
class SearchLineEdit;
class QListWidget;
class QListWidgetItem;
class QMenu;
class OrderEntryDialog;
#include "Platform/Position/Instrument.h"

class InstrumentViewWidget : public QWidget
{
    Q_OBJECT
    private:
        InstrumentView* _instrumentView;
        SearchLineEdit* _searchLineEdit;
        QListWidget* _listWidget;
        int _numInstrumentsInListWidget;

    public:
        InstrumentViewWidget(QWidget* parent = 0);

    private:
        void setupActions();

    public:
        void resizeEvent(QResizeEvent *);
        void onAddInstrment(const TickerId);

    private slots:
        void updateSearch(const QString&);
        void onInstrumentSelection(QListWidgetItem* );
};

#endif // INSTRUMENTVIEWWIDGET_H
