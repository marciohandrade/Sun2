#ifndef _AUTH_SERVER_QUERY_EXTRA_H_
#define _AUTH_SERVER_QUERY_EXTRA_H_

#include "Query_Base.h"

// (WAVERIX) (090816) (ADD-IN) by __NA000000_SHOP_USERID_50_PASSING_PROC__
struct QueryUserIdInfo;

class QueryUserId : public QueryObject
{
public:
    QueryUserId();
    virtual ~QueryUserId();

    void AlignQueryUserIdInfo(const QueryUserIdInfo* query_info);
    const QueryUserIdInfo* GetQueryUserIdInfo() const { return query_user_id_info_; };

    void SetGameAgentID(AGENTID agent_ID) { agent_id_ = agent_ID; };

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

private:
    AGENTID agent_id_;
    QueryUserIdInfo* query_user_id_info_;
#pragma pack(push, 1)
    wchar_t account_id_[ID_MAX_LEN + 1];
    const wchar_t overrun_detector_;
#pragma pack(pop)
};

class QueryUpdateTokenKey : public QueryObject
{

public:
	QueryUpdateTokenKey();
	virtual ~QueryUpdateTokenKey();

	void onResult();
	virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

private:
	int query_result_;
};

class QuerySelectPCRoomIPList : public QueryObject
{    
public:    
    struct RECODER_SET
    {
        wchar_t pc_room_ip_[MAX_IP_NAME_LENGTH+1];
    };

public:
    QuerySelectPCRoomIPList();
    virtual ~QuerySelectPCRoomIPList();

    void SetPCRoomIP(DWORD pc_room_guid)  {pc_room_guid_ = pc_room_guid;}
    
    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

private:    
    DWORD               pc_room_guid_;
    RECODER_SET         recoder_set_;
};


#endif //_AUTH_SERVER_QUERY_EXTRA_H_

