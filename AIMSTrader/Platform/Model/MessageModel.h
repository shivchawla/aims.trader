#pragma once
#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "Platform/Utils/Singleton.h"
#include "Platform/Model/DataModel.h"
enum MessageModelColumn
{
    MessageModelDateTime = 0,
    MessageModelReporter,
    MessageModelReport,
    MessageModelType,
    MessageModelId
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
            add(MessageModelType, "Type",false);
            add(MessageModelId,"ID",false);
        }
};

#endif // MESSAGEMODEL_H
