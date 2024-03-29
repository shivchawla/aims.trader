#pragma once
#include <ActiveTickServerAPI.h>
#include <string>
#include <QMutex>
#include <QWaitCondition>

namespace ActiveTickFeed{

#ifndef APISESSION_H
#define APISESSION_H

#include <ActiveTickServerAPI.h>
#include <string>
#include <QMutex>
#include <QWaitCondition>

class APISession
{
public:
    APISession(/*InboundService* parentServicep*/);
    virtual ~APISession(void);

public:
    uint64_t GetSessionHandle() const { return m_hSession; }

public:
    bool		Init(const ATGUID& apiUserid, const std::string& serverIpAddress, uint32_t serverPort, const wchar16_t* userid, const wchar16_t* password);
    bool		Destroy();
    void        CreateSession();
    void        WaitForSession();
    bool        IsSessionReady();
    void        ShutdownSession();


private:
    static void ATSessionStatusChangeCallback(uint64_t hSession, ATSessionStatusType statusType);
    static void ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse);
    static void ATServerTimeUpdateCallback(LPATTIME pServerTime);
    static void ATRequestTimeoutCallback(uint64_t hOrigRequest);
    void setSessionReady(const bool isReady=false);

private:
    uint64_t		m_hSession;
    uint64_t		m_hLastRequest;
    wchar16_t		m_userid[50];
    wchar16_t		m_password[50];
    bool            m_SessionReady;
    QMutex          mutex;
    QWaitCondition  waitCondition;

private:
    static APISession*	s_pInstance;
};


}
#endif //APISESSION_H
