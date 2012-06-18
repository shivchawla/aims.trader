#ifndef MESSAGEVIEW_H
#define MESSAGEVIEW_H
#include "Platform/View/TableView.h"
#include "Platform/View/MessageViewItem.h"
#include "Platform/Model/MessageModel.h"

class MessageView : public TableView<MessageView, MessageViewItem, MessageModel, MessageModelColumn>
{
    Q_OBJECT
    public:
        MessageView(QWidget* parent=0);

    public:
        void resizeEvent(QResizeEvent *event);


     public slots:
        void reportEvent(const String& dateTime, const String& reporter, const String& report, const int messageType = 0);
};

//#include "Platform/typedefs.h"
//#include <QWidget>

//class QLabel;
//class QTextEdit;

//class MessageView : public QWidget
//{
//    Q_OBJECT
//    private:
//        QLabel* _label;
//        QTextEdit* _editor;

//    public:
//        MessageView(QWidget* parent = 0);

//     public slots:
//        void reportEvent(const String& dateTime, const String& reporter, const String& message, const int);

//};


#endif // MESSAGEVIEW_H