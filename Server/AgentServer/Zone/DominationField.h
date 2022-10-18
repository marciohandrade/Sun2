#pragma once
//=============================================================================================================================
/*

DOMINATION_EVENT_END:
 ���� ����(���Ȧ ����)

DOMINATION_EVENT_SYSTEM_RESET:
 ���� ���
 ���� ���� ����
 ���� ��� ����
 �ʵ� �ı�, ���� (Ʈ���� ����)

DOMINATION_EVENT_WAIT:
 ���� ����
 ���� Ÿ�̸� �ð� ����(�������)
- ���� ������ �ð� ����ȭ

DOMINATION_EVENT_START:
 ���� ����
 ���� Ÿ�̸� ����
- ���� ������ �ð� ����ȭ

// ���°� 
DOMINATION_EVENT_ACTION_START           : ��Žð�          [��Ű���, �������ð���, ���尡��(��)] 
DOMINATION_EVENT_ACTION_END             : ��ų�            [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SELECT_ATTACKGUILD     : ���� ��� ����    [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SELECT_ATTACKGUILD_END : ���� ��� ���� �� [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SYSTEM_RESET           : �ý��۸���        [��źҰ�, �������úҰ�, ����Ұ�(��,��)]
DOMINATION_EVENT_WAIT                   : ���� ���         [��źҰ�, �������úҰ�, ���尡��(��,��)]
DOMINATION_EVENT_START                  : ���� ����         [��źҰ�, �������úҰ�, ���尡��(��,��)]
DOMINATION_EVENT_END                    : ���� ��           [��Ű���, �������ð���, ���尡��(��)]
DOMINATION_EVENT_ACTION_START2          : ���� ���� �ʴ� ����
DOMINATION_EVENT_MAX                    : ���� ����

*/
//=============================================================================================================================

#include "ZoneInterface.h"

typedef std::pair<GUILDGUID, INT> GUILD_COUNT;
typedef std::pair<DWORD, DWORD>    OUTSIDER;    //<key, count>
typedef STLX_LIST<OUTSIDER>        OUTSIDERS;
typedef OUTSIDERS::iterator        OUTSIDERS_IT;

typedef std::pair<GUILDGUID, GUILDGUID> KICK_GUILD;

class DominationField : public ZoneInterface
{
    _DECLARE_POOL(DominationField)

    enum { empty_guild_idx = -1 };
    enum { offense_guild, defense_guild, max_guild };
    enum { OUTSIDER_UPDATE_TIME = 100, EVENT_UPDATE_TIME = 500, KICK_TIME = 10*1000 }; 
    // 0.1��, 0.5��, 10��

public:
    DominationField(void);
    virtual ~DominationField(void);

    VOID Create(KEYTYPE key, MAPCODE MapCode, ServerSession* pServerSession);
    inline virtual eZONETYPE GetType() { return eZONETYPE_DOMINATION_FIELD; }

    virtual BOOL Process();
    virtual VOID ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode = 0,
                            DWORD dwAreaID = 0, WzVector * pwvStartPos = NULL, bool bFirstJoin = false );
    VOID SetGuildGuid(GUILDGUID offense, GUILDGUID defense, eDOMINATION_EVENT_TYPE eSetType);
    VOID KickUser(DWORD playerKey);

protected:
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_readyuser(User* pUser);
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);
    virtual RC::eROOM_RESULT _CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);


private:
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    typedef STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> EventInfos;
    //�ļ��� ���� ���� �ʵ��� ������ �̺�Ʈ�� ������ EventInfos�� ����
    VOID _SetEventInfos(MAPCODE map_code); 
    //�ڽ��� EventInfos���� ���� ������ ���¸� ������
    eDOMINATION_EVENT_STATE _GetCurEventState(const WORD day_of_week, const DWORD time) const;
    EventInfos event_infos_;
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    RC::eROOM_RESULT _GuildCount(User* pUser, INT count = 1);
    std::pair<BOOL,INT> _Counting(GUILDGUID guildGuid, INT count );

    BOOL _ReturnToEntrance(User* pUser);
    INT  _GetGuildCountIndex(GUILDGUID guildGuid);
    VOID _ReSetDomination();
    VOID _KickUser(GUILDGUID guildGuid);
    VOID _UpdateTimeEvent();

    BYTE _CountingPlayer(GUILDGUID guild_guid);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void _KickUsers_ExceptWinGuild(GUILDGUID winner_guild);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

public:
    static BOOL is_gm_observer;         // gm�� �������� ���� ��ų���ΰ�. gm ��ɾ�� ���.

private:
    GUILD_COUNT guild_count_[max_guild];
    OUTSIDERS   outSider_;              // �߸� ���� ���� 
    OUTSIDERS   outSiderOb_;            // ������ ����(GM)

    util::Timer	outsider_updatetimer_;  // outSider_ ó�� Ÿ�̸�    //_0_20100419_DOMINATION_RETURN_TO_ENTRANCE_BUG

    BOOL        bReCreate_;    
    util::Timer	kickTimer_;            // ���� ���ð� (10��),
    KICK_GUILD  pairKickGuild_;    

    util::Timer	eventUpdateTimer_;     // �д��� �̺�Ʈ (�ʴ����� üŷ)
    eDOMINATION_EVENT_STATE curEventState_;
};

