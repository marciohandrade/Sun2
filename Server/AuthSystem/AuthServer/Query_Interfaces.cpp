#include "stdafx.h"
#include "./Query_Interfaces.h"
#include "QueryManager.h"
#include "AccountManager.h"
#include "AuthFrame.h"
#include "LoginFrontServerSession.h"

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__

using namespace nsHanAuth;

extern TCHAR g_LoginErrorString[AUTHRESULT_MAX][MAX_AUTHRESULT_MESSAGE_LEN + 1];

HanAuthQueryCallback HanAuthQueryCallback_Global;

void
HanAuthQueryCallback::AuthResult(nsHanAuth::INDEX_ID_T query_slot_index, int hanAuth_result,
                                 const char user_id[ID_MAX_LEN + 1] /* etc ... */)
{
    NORTHA_DEBUGLOG(__FUNCTION__" mode %d\n", _AUTH_NORTHA_TEST_MODE);
    QueryManager* pQueryManager = QueryManager::Instance();
    QuerySlot slot;
    if(!FlowControl::FCAssert(HanAuthQueryManager::GetSlot(query_slot_index, &slot) != 0))
        return;
    if(hanAuth_result == HAN_AUTHSVR_OK)
    {
        //pQueryManager->QueryLoginUser();
        SUNLOG(eFULL_LOG, "["__FUNCTION__"] HAN_AUTHSVR_OK - %u %s\n", query_slot_index, user_id);
        //pQueryManager->QueryLoginUser(slot.session_index_, const_cast<char*>(user_id), "",
        //    slot.front_id_, 0, slot.client_ip_, slot.local_ip_);
        //AccountInfo* pAccountInfo = AccountManager::Instance()->FindWithLoginFrontUserID(slot.front_id_);
        /*if(pAccountInfo != 0)*/ {
            pQueryManager->QueryLoginUserByAuthTokenNorthA(slot.session_index_,
                                                           const_cast<char*>(user_id),
                                                           slot.front_id_, 0,
                                                           slot.client_ip_,
                                                           slot.local_ip_);
        }
    }
    else
    {
        // (WAVERIX) (090813) (CHANGES) HanAuthForSvr의 실패에 대해서도 클라이언트에 그 사유를 알린다.
        MSG_SF_AUTH_ID_PASSWORD_CHECK_ACK msgFail;
        msgFail.m_dwFrontUserID = slot.front_id_;
        msgFail.m_byFailCount = 0;

        AuthFrame* const pAuthFrame = AuthFrame::Instance();
        LoginFrontServerSession* front =
            (LoginFrontServerSession*)pAuthFrame->FindSession(slot.session_index_);
        if(front == 0)
            return;

        if(hanAuth_result < 0) {
            msgFail.m_byResult = AUTHRESULT_INVALIDACCOUNT;
        } else {
            switch(hanAuth_result)
            {
            case HAN_AUTHSVR_PARAM_INVALID:
            case HAN_AUTHSVR_IP_INVALID:
            case HAN_AUTHSVR_MEMBERID_INVALID:
            case HAN_AUTHSVR_PASSWORD_INCORRECT:
            case HAN_AUTHSVR_PASSWORD_MISMATCHOVER:
            case HAN_AUTHSVR_COOKIE_SETTINGERR:
            case HAN_AUTHSVR_COOKIE_NOTEXIST:
            case HAN_AUTHSVR_COOKIE_TIMEOUT:
                msgFail.m_byResult = AUTHRESULT_INVALIDACCOUNT;
                break;
            case HAN_AUTHSVR_MEMBERID_NOTEXIST:
                msgFail.m_byResult = AUTHRESULT_NOTALLOWEDACCOUNT;
                break;
            case HAN_AUTHSVR_SYSTEM_ERROR:
                msgFail.m_byResult = AUTHRESULT_SYSTEMERROR;
                break;
            default:
                msgFail.m_byResult = AUTHRESULT_INVALIDACCOUNT;
                break;
            }
        };

        ::_tcsncpy(msgFail.m_szInfo, g_LoginErrorString[msgFail.m_byResult],
                   _countof(msgFail.m_szInfo));
        front->SendPacket(&msgFail, sizeof(msgFail));

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] != HAN_AUTHSVR_OK - %u ret : %d\n",
               query_slot_index, hanAuth_result);
    }

    HanAuthQueryManager::ReleaseSlot(query_slot_index);
}

#endif //__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__


