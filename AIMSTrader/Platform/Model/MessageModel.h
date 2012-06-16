#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "Platform/Utils/Singleton.h"
#include "Platform/Model/DataModel.h"
enum MessageModelColumn
{
    MessageModelDateTime = 0,
    MessageModelReporter,
    MessageModelReport,
    MessageModelType
};

class MessageModel : public DataModel<MessageModelColumn>, public Singleton<MessageModel>
{
    friend class Singleton<MessageModel>;

    public:
        MessageModel()
        {
            add(MessageModelDateTime, "DateTime", true);
            add(MessageModelReporter, "Reporter", true);
            add(MessageModelReport, "Message", true);
        }
};

#endif // MESSAGEMODEL_H
