#include "stdafx.h"
#include "HistoryBarInbound.h"

HistoryBarInbound::HistoryBarInbound(QUuid id, Requestor* const req)
{
	instrumentId = id;
	requestor = req;
}
HistoryBarInbound :: HistoryBarInbound(const HistoryBarInbound* inbound) {
	instrumentId = inbound->instrumentId;
	requestor = inbound->requestor;
}

HistoryBarInbound::HistoryBarInbound(void)
{
}

HistoryBarInbound::~HistoryBarInbound(void)
{
}

void HistoryBarInbound::Begin() {
}
