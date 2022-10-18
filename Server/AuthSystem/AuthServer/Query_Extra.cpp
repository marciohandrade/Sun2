#include "stdafx.h"
#include "./Query_Extra.h"
#include "./ServerDataManager.h"
#include "./AuthAgentServerSession.h"
#include "SunAuth/AuthProtocol_GA.h"
#include <atlconv.h>
// (WAVERIX) (090816) (ADD-IN) by __NA000000_SHOP_USERID_50_PASSING_PROC__

#include "QueryManager.h"
#include "AccountManager.h"
#include <Struct.h>

QueryUserId::QueryUserId()
    : QueryObject(QUERYTYPE_USERID)
    , query_user_id_info_(0)
    , overrun_detector_(0)
{
    m_recordSet.RegisterVariable(account_id_, "UserID");
}

QueryUserId::~QueryUserId()
{
    if(query_user_id_info_ != 0) {
        TAllocDelete(QueryUserIdInfo, query_user_id_info_);
        query_user_id_info_ = 0;
    }
}


void
QueryUserId::AlignQueryUserIdInfo(const QueryUserIdInfo* query_info)
{
    if (query_user_id_info_ == NULL) {
        query_user_id_info_ = TAllocNew(QueryUserIdInfo);
    };
    *query_user_id_info_ = *query_info;
    ZeroMemory(account_id_, sizeof(account_id_));
}

void
QueryUserId::onResult()
{
    QueryObject::onResult();

    ASSERT(overrun_detector_ == 0 && query_user_id_info_);

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    BOOLEAN success = true;

    if(success && m_recordSet.GetRecordCount() == 0)
        success = false;

    if(success && account_id_[0] == L'\0')
        success = false;

    account_id_[_countof(account_id_) - 1] = L'\0';

    char account_id[ID_MAX_LEN + 1] = { 0, };
    if(success) {
        int ret = WideCharToMultiByte(CP_ACP, 0,
                                      account_id_, -1,
                                      account_id, sizeof(account_id),
                                      NULL, NULL);
        if(ret != 0)
            success = account_id[_countof(account_id) - 1] == 0;
    }

    //if(success) {   // (WAVERIX) (NOTE) Why unicode?
    //    char* dest_it = account_id;
    //    wchar_t* src_it = account_id_;
    //    for(int loop = ID_MAX_LEN; loop; --loop)
    //        *dest_it++ = char(*src_it++);
    //    account_id[_countof(account_id) - 1] = '\0';
    //};

    MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK msgACK;
    msgACK.Clear();
    msgACK.game_agent_id_ = agent_id_;
    //msgACK.action_type_ <- unhandled
    msgACK.result_ = success;

    if(success) {
        strncpy(query_user_id_info_->user_id_, account_id, _countof(query_user_id_info_->user_id_));
        query_user_id_info_->user_id_[_countof(query_user_id_info_->user_id_) - 1] = '\0';
    } else {
        ZeroMemory(query_user_id_info_->user_id_, sizeof(query_user_id_info_->user_id_));
    }
    msgACK.user_info_ = *query_user_id_info_;

    ServerData* pServerData = ServerDataManager::Instance()->FindServerData(agent_id_);
    if(pServerData == 0)
        return;

    AuthAgentServerSession* pAuthAgentServerSession = pServerData->GetAuthAgentSession();
    if(pAuthAgentServerSession == 0)
        return;

    pAuthAgentServerSession->SendPacket(&msgACK, msgACK.GetSize());
#endif //__NA000000_SHOP_USERID_50_PASSING_PROC__
}


QueryUpdateTokenKey::QueryUpdateTokenKey()
: QueryObject(QUERYTYPE_TOKEN_UPDATE)
{
	m_recordSet.RegisterParameter(&query_result_, "", adInteger, adParamReturnValue, sizeof(int), "");
}

QueryUpdateTokenKey::~QueryUpdateTokenKey()
{
	
}

void
QueryUpdateTokenKey::onResult()
{
	QueryObject::onResult();

	if(query_result_ != 0)
	{
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] S_Set_Token_Key fail [%d]", query_result_);
	}
}


QuerySelectPCRoomIPList::QuerySelectPCRoomIPList()
: QueryObject(QUERYTYPE_PCROOM_IPLIST)
{    
    m_recordSet.RegisterVariable(recoder_set_.pc_room_ip_, "IpAddress");    
}

QuerySelectPCRoomIPList::~QuerySelectPCRoomIPList()
{

}

void QuerySelectPCRoomIPList::onResult()
{
    QueryObject::onResult();

#ifdef __PCBANG_IP_UPDATE

    
    USES_CONVERSION;    
    char* pc_room_ip = NULL;    
    DWORD pc_ip = 0;
    
    AccountManager* const account_manager = AccountManager::Instance();
    PCRoomIPManager* const pc_room_manager = account_manager->GetPCRoomManager();

    PCRoomIPList* const pc_room_list = pc_room_manager->CreatePCROOMIPList();

    if(pc_room_list == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] PC List Object Create fail");
        return;
    }  

    pc_room_list->SetListStatus(PCRoomIPList::list_loading);
    pc_room_list->SetPCRoomGUID(pc_room_guid_);
    
    while(!m_recordSet.IsEndRecord())
    {
        pc_room_ip = W2A(recoder_set_.pc_room_ip_);
        pc_room_list->AddIPList(::inet_addr(pc_room_ip));
        m_recordSet.Next();
    }

    pc_room_list->SetListStatus(PCRoomIPList::list_loaded);    
    
#endif

}















