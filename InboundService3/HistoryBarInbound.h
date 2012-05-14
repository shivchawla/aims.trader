#pragma once
#include <QUuid>
#include "InboundBase.h"
#include "Requestor.h"

class HistoryBarInbound : public InboundBase
{
private:
	QUuid instrumentId;
	Requestor *requestor;

public:
	HistoryBarInbound();
	HistoryBarInbound(QUuid instrumentId, Requestor* const requestor);
	HistoryBarInbound(const HistoryBarInbound* inbound);
	~HistoryBarInbound(void);

	void Begin();
};

