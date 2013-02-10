#include "Platform/View/InstrumentViewWidget.h"
#include "Platform/View/InstrumentView2.h"
#include "Platform/View/SearchLineEdit.h"
#include "Platform/View/IODatabase.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/View/OrderEntryDialog.h"
#include <QVBoxLayout>

InstrumentViewWidget::InstrumentViewWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    _instrumentView = new InstrumentView(this);
    _searchLineEdit = new SearchLineEdit(this);
    layout->addWidget(_searchLineEdit);
    layout->addWidget(_instrumentView);
    setLayout(layout);

    _numInstrumentsInListWidget = 0;
    _listWidget = new QListWidget(this);
    //_listWidget->pai (Qt::TransparentMode);
    //_listWidget->setWindowOpacity(0);
    int height = _searchLineEdit->size().height();
    int viewWidth = _searchLineEdit->size().width();
    //_searchLineEdit->setGeometry(QRect(pos().x(),pos().y(), viewWidth, headerHeight));
    _listWidget->setGeometry(QRect(_searchLineEdit->pos().x(), _searchLineEdit->pos().y() + height, viewWidth, _numInstrumentsInListWidget*height));

    _instrumentView->show();
    _searchLineEdit->show();
    _listWidget->hide();
    _listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _listWidget->setWindowFlags(Qt::SubWindow);

    connect(_searchLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateSearch(const QString&)));
    connect(_listWidget,SIGNAL(itemClicked(QListWidgetItem* )), this, SLOT(onInstrumentSelection(QListWidgetItem*)));

    for(int i=0;i<5;++i)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setForeground(Qt::black);
        item->setBackground(Qt::white);
        item->setSelected(false);
        item->setFont(QFont("Cambria"));
        _listWidget->insertItem(i,item);
    }
}

void InstrumentViewWidget::updateSearch(const QString& symbol)
{
    if(symbol.length()>0)
    {
        QList<InstrumentData> instruments = IODatabase::ioDatabase().getInstrumentsWithSimilarSymbol(symbol);

        int numItems = _listWidget->count();

        _numInstrumentsInListWidget = instruments.length();
        if(_numInstrumentsInListWidget==0)
        {
            _listWidget->hide();
            return;
        }

        int height = _searchLineEdit->size().height();
        _listWidget->resize(_listWidget->width(), _numInstrumentsInListWidget*height);

         for(int i=0 ; i<numItems ; ++i)
        {
            if(_numInstrumentsInListWidget > i)
            {
                QString rowData = instruments[i].symbol;
                rowData.append('\t').append(instruments[i].fullName);
                rowData.append('\t').append(instruments[i].exchangeCode);
                _listWidget->item(i)->setData(Qt::DisplayRole, rowData);
            }
            else
            {
                _listWidget->item(i)->setData(Qt::DisplayRole, "");
            }
        }
        _listWidget->show();
    }
    else
    {
        _listWidget->hide();
        _searchLineEdit->setText("");
    }
}

void InstrumentViewWidget::onInstrumentSelection(QListWidgetItem* item)
{
    QStringList symbolData = item->data(Qt::DisplayRole).toString().split('\t',QString::SkipEmptyParts);
    TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(symbolData[0]);
    _instrumentView->addInstrument(tickerId);
    _listWidget->hide();
    _searchLineEdit->show();
}

void InstrumentViewWidget::resizeEvent(QResizeEvent* event)
{
    int height = _searchLineEdit->size().height();
    int width = _searchLineEdit->size().width();
    //_searchLineEdit->setGeometry(QRect(pos().x(),pos().y(), viewWidth, headerHeight));
    _listWidget->setGeometry(QRect(_searchLineEdit->pos().x(), _searchLineEdit->pos().y() + height, width, _numInstrumentsInListWidget*height));
    QWidget::resizeEvent(event);

}

void InstrumentViewWidget::onAddInstrment(const TickerId tickerId)
{
    _instrumentView->addInstrument(tickerId);
}

