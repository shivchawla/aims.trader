#include "stdafx.h"
#include "Helper.h"
#include "Session.h"
#include <stdio.h>
#include "DataManager.h"

APISession* APISession::s_pInstance = NULL;
APISession::APISession(void)
: m_hSession(0)
, m_hLastRequest(0)
{
    m_hSession = ATCreateSession();
    s_pInstance = this;
    m_SessionReady = false;
}

APISession::~APISession(void)
{
    ATDestroySession(m_hSession);
    s_pInstance = NULL;
}

void APISession::CreateSession()
{
   m_hSession = ATCreateSession();
}

void APISession::ShutdownSession()
{
    ATShutdownSession(m_hSession);
}

bool APISession::Init(const ATGUID& apiUserid, const std::string& serverIpAddress, uint32_t serverPort, const wchar16_t* userid, const wchar16_t* password)
{
    if(Helper::StringLength(userid) >= _countof(m_userid) || Helper::StringLength(password) >= _countof(m_password))
        return false;

    Destroy();

    Helper::CopyString(m_userid, userid);
    Helper::CopyString(m_password, password);

    bool rc = ATSetAPIUserId(m_hSession, (LPATGUID)&apiUserid);

    if(rc == true)
        rc = ATInitSession(m_hSession, serverIpAddress.c_str(), serverIpAddress.c_str(), serverPort, ATSessionStatusChangeCallback);

    WaitForSession();

    return rc;
}

bool APISession::Destroy()
{
    ATShutdownSession(m_hSession);
    return true;
}

/*static*/ void APISession::ATSessionStatusChangeCallback(uint64_t hSession, ATSessionStatusType statusType)
{
    std::string strStatusType;
    switch(statusType)
    {
    case SessionStatusConnected: strStatusType = "SessionStatusConnected"; break;
    case SessionStatusDisconnected: strStatusType = "SessionStatusDisconnected"; break;
    case SessionStatusDisconnectedDuplicateLogin: strStatusType = "SessionStatusDisconnectedDuplicateLogin"; break;
    default: break;
    }

    printf("RECV Status change [%s]\n", strStatusType.c_str());

    if(statusType == SessionStatusConnected)
    {
        APISession::s_pInstance->m_hLastRequest = ATCreateLoginRequest(hSession, APISession::s_pInstance->m_userid, APISession::s_pInstance->m_password, APISession::ATLoginResponseCallback);
        bool rc = ATSendRequest(hSession, APISession::s_pInstance->m_hLastRequest, DEFAULT_REQUEST_TIMEOUT, APISession::ATRequestTimeoutCallback);

        printf("SEND (%llu): Login request [%s] (rc=%d)\n", APISession::s_pInstance->m_hLastRequest, Helper::ConvertString(APISession::s_pInstance->m_userid,
            Helper::StringLength(APISession::s_pInstance->m_userid)).c_str(), rc);
    }
}

/*static*/ void	APISession::ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse)
{
    std::string strLoginResponseType;
    switch(pResponse->loginResponse)
    {
    case LoginResponseSuccess: strLoginResponseType = "LoginResponseSuccess"; break;
    case LoginResponseInvalidUserid: strLoginResponseType = "LoginResponseInvalidUserid"; break;
    case LoginResponseInvalidPassword: strLoginResponseType = "LoginResponseInvalidPassword"; break;
    case LoginResponseInvalidRequest: strLoginResponseType = "LoginResponseInvalidRequest"; break;
    case LoginResponseLoginDenied: strLoginResponseType = "LoginResponseLoginDenied"; break;
    case LoginResponseServerError: strLoginResponseType = "LoginResponseServerError"; break;
    default: strLoginResponseType = "unknown"; break;
    }

    s_pInstance->setSessionReady(pResponse->loginResponse==LoginResponseSuccess);

    printf("RECV (%llu): Login response [%s]\n", hRequest, strLoginResponseType.c_str());

}

/*static*/ void	APISession::ATServerTimeUpdateCallback(LPATTIME pServerTime)
{
    printf("RECV: Server time update [%0.2d:%0.2d:%0.2d %0.2d/%0.2d/%0.4d\n",
        pServerTime->wHour, pServerTime->wMinute, pServerTime->wSecond,
        pServerTime->wMonth, pServerTime->wDay, pServerTime->wYear);
}

/*static*/ void	APISession::ATRequestTimeoutCallback(uint64_t hOrigRequest)
{
    printf("(%I64u): Request timed-out\n", hOrigRequest);
    DataManager::Instance()->requestTimeout(hOrigRequest);

}

void APISession::WaitForSession()
{
    mutex.lock();
    if(!m_SessionReady)
    {
        waitCondition.wait(&mutex);
    }
    mutex.unlock();
}

void APISession::setSessionReady(const bool isReady)
{
    mutex.lock();
    m_SessionReady = isReady;
    waitCondition.wakeAll();
    mutex.unlock();
}

bool APISession::IsSessionReady()
{
    bool isSessionReady;
    mutex.lock();
    isSessionReady  = m_SessionReady;
    mutex.unlock();
    return isSessionReady;
}


