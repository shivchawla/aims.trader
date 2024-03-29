#include "Platform/View/MessageView.h"
#include <QScrollBar>
#include <QTextTable>
#include <QDebug>
#include <QHeaderView>
#include <QDateTime>

MessageView::MessageView(QWidget* parent):TableView<MessageView, MessageViewItem, MessageModel, MessageModelColumn>(parent)
{
    _id = 0;
    setAlternatingRowColors(true);
    setShowGrid(true);
    horizontalHeader()->setStretchLastSection(true);
    setSortingEnabled(_isSortingAllowed =  false);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(245, 245, 220));
    p.setColor(QPalette::Foreground, QColor(245, 245, 220));

    setPalette(p);

    horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveHeader()));

    connect(_signalMapper, SIGNAL(mapped(const int)), this, SIGNAL(modifyHeadersClicked(const int)));
    connect(this, SIGNAL(modifyHeadersClicked(const int)), this, SLOT(modifyHeaders(int)));
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)),this, SLOT(onHeaderClick(int)));

}

void MessageView::resizeEvent(QResizeEvent *event)
{
    //horizontalHeader()->resize
    QTableWidget::resizeEvent(event);
}

void MessageView::reportEvent(const QDateTime& dateTime, const String& reporter, const String& report, const MessageType messageType)
{
    MessageViewItem* newItem  = addItemInView();

    newItem->update(dateTime.toString("MM/dd/yyyy hh:mm:ss:zzz"), MessageModelDateTime);
    newItem->update(reporter, MessageModelReporter);
    newItem->update(report, MessageModelReport);
    newItem->update(getMessageType(messageType), MessageModelType);
    newItem->update(++_id, MessageModelId);
    verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
}

void MessageView::onRemoveHeader()
{
    removeHeader();
}

void MessageView::onCustomizeHeader()
{
     //_dialog->show();
}

void MessageView::modifyHeaders(const int column)
{
    modifyHeader(column);
}

void MessageView::onHeaderClick(const int section)
{
    Qt::SortOrder order = horizontalHeader()->sortIndicatorOrder();// == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;

    sortByColumn(section,order);
//    if(section == (int)MessageModelReporter || section == (int)MessageModelReport)
//    {
//        sortByColumn((int)MessageModelId, Qt::AscendingOrder);
//        sortByColumn(section, order);
//    }
//    else
//    {
//        sortByColumn((int)MessageModelId, order);
//    }
}

//#include <QVBoxLayout>
//#include <QLabel>
//#include <QTextEdit>

//MessageView::MessageView(QWidget* parent):QWidget(parent)
//{
//    QVBoxLayout* layout = new QVBoxLayout(this);
//    _editor  = new QTextEdit(this);
//    _label = new QLabel(this);
//    _editor->setFrameShape(QFrame::NoFrame);

//    layout->addWidget(_label);
//    layout->addWidget(_editor);

//    String label;
//    label.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");
//    label.append(HtmlTags::ROW_START);
//    label.append(HtmlTags::FIELD_START).append("DateTime").append(HtmlTags::FIELD_END);
//    label.append(HtmlTags::FIELD_START).append("Reporter").append(HtmlTags::FIELD_END);
//    label.append(HtmlTags::FIELD_START).append("Message").append(HtmlTags::FIELD_END);
//    label.append(HtmlTags::ROW_END);

//    _label->setText(label);
//    _editor->setReadOnly(true);

//     String output;
//     output.append("<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=100%>");

//    //_editor->textCursor().insertTable(1,3);
//    _editor->insertHtml(output);
//}

//void MessageView::reportEvent(const String& dateTime, const String& reporter, const String& message, const int)
//{
//    String output;

//    output.append(HtmlTags::ROW_START);
//    output.append(HtmlTags::FIELD_START).append(dateTime).append(HtmlTags::FIELD_END);
//    output.append(HtmlTags::FIELD_START).append(reporter).append(HtmlTags::FIELD_END);
//    output.append(HtmlTags::FIELD_START).append(message).append(HtmlTags::FIELD_END);
//    output.append(HtmlTags::ROW_END);

//    _editor->insertHtml(output);
//    qDebug()<<_editor->toHtml();

//    //_editor->textCursor().currentTable()->insertRows(1,3);
//    int x=1;
//}
