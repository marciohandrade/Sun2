#pragma once

/*
AgentParty에 귀속되는 클래스, 비정상 접속 유저가 발생하게 되면 해당 유저가 속해있는 파티가 
비정상 접속 유저 정보를 관리하게 되는데 그 기능을 담당하는 클래스(2011.12.23)
*/

struct ClosingUserInfo;

class ClosingUserList;
class ClosingUserListManager;
class AgentParty;

typedef STLX_LIST<ClosingUserInfo> UserList;

class ClosingUserList
{
public:     
    ClosingUserList();
    ~ClosingUserList();

    void Init(
        WORD PartyKey
        );

    void Clear();
    
    void Update(
        AgentParty* const pAgentParty
        );

    bool UpdateChar(
        ClosingUserInfo& closing_user_info
        );

    void AddClosingUser(
        const DWORD char_guid, 
        const TCHAR* char_name, 
        PartyReturnInfo& rInfo,
        const _PARTY_MEMBER_INFO& member_info
        );

    void RemoveClosingUser(
        const DWORD char_guid
        );

    void RemoveClosingUser(
        const TCHAR* char_name
        );
    
    const ClosingUserInfo* FindClosingUser(
        const DWORD char_guid
        );

    const ClosingUserInfo* FindClosingUser(
        const TCHAR* char_name
        );

    BYTE GetClosingUserCount()const;

    void AddClosingUserInfoToMemberList(
        PARTY_MEMBER_TOTAL_INFO& member_list
        );


private:
    ClosingUserInfo* _FindEditableClosingUser(
        const DWORD char_guid
        );
private:
    WORD party_key_;
    UserList closing_user_list_;
};

struct ClosingUserInfo
{
    DWORD char_guid_;
    ITimeout time_out_;
    sSTRING_BUFFER_CHAR_NAME char_name_;
    WzVector return_pos_;
    FIELDCODE return_field_code_;
    eZONETYPE return_zone_type_;
    KEYTYPE return_zone_key_;
    bool return_transaction_;
    _PARTY_MEMBER_INFO member_info_;

    ClosingUserInfo()
        : char_guid_(0)
        , return_transaction_(false)
    {
        ZeroMemory(&member_info_, sizeof(member_info_));
    }

    ClosingUserInfo(const DWORD char_guid)
        : char_guid_(char_guid)
        , return_transaction_(false)
    {
        time_out_.SetNextTimeoutByCurTickBased(10 * 60 * 1000);
        char_name_.GetBUFFER()[0] = _T('\0');
    }

    inline void SetCharName(const TCHAR* char_name) 
    { 
        char_name_.CopyFrom(char_name); 
    }
    
    inline TCHAR* const GetCharName()const
    { 
        return char_name_; 
    }

    inline bool CanSendRequestReturnParty()const
    {
        return (return_transaction_ == false);
    }

    inline bool IsStateReturnTransaction()const
    {
        return return_transaction_;
    }
};