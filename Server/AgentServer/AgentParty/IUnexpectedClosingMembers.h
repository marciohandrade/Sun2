#pragma once

class UnexpectedClosingMembers;
class IUnexpectedClosingMembers;
struct sUClosingUser;
class AgentParty;

typedef IUnexpectedClosingMembers   IUCM_T;

//==================================================================================================

class IUnexpectedClosingMembers
{
public:     
    enum eUSR_STATUS 
    {
        NO_ALIGN        = 0x0001, // 할당되지 않은 상태, 취소된 상태
        NEW_INSERTED    = 0x0002, // 신규 추가
        LEADER_REQUEST  = 0x0010, // 리더가 요청 한번 했다
        LEADER_REQUEST2 = 0x0020, // 리더가 요청 두번 했다 (멤버가 한번 취소)
        MEMBER_REJECT   = 0x0100, // 멤버가 취소했다.
        MEMBER_REJECT2  = 0x0200, // 멤버가 두번째 취소했다. (만료)
        PENDING_STATUS  = 0x1000, // 내부 제어값

        // 외부에서는 이 상태에 대해 제어해서는 안된다.
        MASK_EXT_NOACT      = NO_ALIGN | NEW_INSERTED | PENDING_STATUS,
        MASK_LEADER_REQUEST = LEADER_REQUEST | LEADER_REQUEST2,
    };

    IUnexpectedClosingMembers() : m_wPartyKey(0), m_pMembers(NULL) {}
    ~IUnexpectedClosingMembers();

    void Init(WORD PartyKey);
    void Clear();

    void Update(AgentParty* const pAgentParty);
    BOOL UpdateChar(sUClosingUser& rCUser);

    void AddUClosingUser(const DWORD char_guid, const TCHAR* pCharName, PartyReturnInfo& rInfo);
    void RemoveUClosingUser(const DWORD char_guid);
    sUClosingUser* FindUClosingUser(const DWORD char_guid);

    BOOL ChangeStatus(const DWORD char_guid, const eUSR_STATUS IN rStatus);
    BOOL SetFirstStatus(const DWORD char_guid);

private:
    WORD m_wPartyKey;
    UnexpectedClosingMembers* m_pMembers;
};

//==================================================================================================

struct sUClosingUser
{
    DWORD       CHAR_GUID;
    IUCM_T::eUSR_STATUS STATUS;
    ITimeout    TIMEOUT;
    sSTRING_BUFFER_CHAR_NAME CHAR_NAME;
    WzVector    m_ReturnPos;
    FIELDCODE   m_ReturnFieldCode;
    eZONETYPE   m_ReturnZoneType;
    KEYTYPE     m_ReturnZoneKey;

    sUClosingUser()
        : CHAR_GUID(0)
        , STATUS(IUCM_T::NO_ALIGN) {};
    sUClosingUser(const DWORD char_guid)
        : CHAR_GUID(char_guid)
        , STATUS(IUCM_T::NEW_INSERTED)
    {
        TIMEOUT.SetNextTimeoutByCurTickBased(10 * 60 * 1000);
        CHAR_NAME.GetBUFFER()[0] = _T('\0');
    }

    inline void SetCharName(const TCHAR* pCharName) { CHAR_NAME.CopyFrom(pCharName); }
    inline TCHAR* const GetCharName() { return CHAR_NAME; }
};

