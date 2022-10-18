#pragma once

/*
#include <hash_map>

class Player;

struct SORT_VIEW_DATA
{
    Player* m_pOwner;
    Player* m_pPlayer;

    bool m_bReferedPlayer;

    void Init(Player* pOwner, Player* pPlayer, bool bReferedPlayer)
    {
        m_pOwner = pOwner;
        m_pPlayer = pPlayer;
        m_bReferedPlayer = bReferedPlayer;
    }
};

class View
{
    static const BYTE c_MAX_DEFAULT_VIEW_OBJ_NUM = 100;
    static const DWORD c_VIEWPORT_UPDATE_DELAY_TIME = 1000;

public:
    View(void);
    ~View(void);

    void InitView(Player * pOwner);
    void ReleaseView();

    typedef STLX_LIST<SORT_VIEW_DATA> sortview_list;
    typedef STLX_HASH_MAP<DWORD, Player *> view_hash;
    typedef view_hash::iterator view_hash_it;
    typedef STLX_SET<Player *> __player_set;
    typedef __player_set::iterator __player_set_it;

    void Update();

    void EnterField(BOOL bUsingViewport);
    void LeaveField();
    void LeaveField(Player * pLeavedPlayer);

    BOOL PushReferedPlayer(DWORD key, Player * pPlayer) { std::pair<view_hash::iterator,bool> rt=m_ReferedList.insert(view_hash::value_type(key,pPlayer)); return rt.second; }
    void PopReferedPlayer(DWORD key){ m_ReferedList.erase(key); }

    inline void SendPacketAround(MSG_BASE_FORWARD* pMsg, WORD wSize, const BOOLEAN bSendToMe, const GUILDGUID other_guid_guid = INVALID_GUILDGUID);
    void SendExPacketAround(const DWORD numberOfMsgs, BYTE** const IN ppMsg, WORD* const pwSize, const BOOLEAN bSendToMe, const GUILDGUID other_guid_guid);

    BOOL IsUsingViewPort(){ return m_bUsingViewport; }

private:
    void UpdateView();
    void _Update_User(Sector * pSector);
    void __Update_User(OBJECT_PAIR rPair);
    void _UpdateSector();
    void _MakeCurView();

    void EnterNeighbor();
    void LeaveNeighbor(BOOL bPopReferedPlayer);

    void _SendAllPlayersRenderInfo();
    void _SendAllPlayersEquipInfo();

    template< class Operator > void For_eachEnterMembers(Operator & Opr);
    template< class Operator > void For_eachLeaveMembers(Operator & Opr);

    Player* m_pOwner;
    BOOL m_bUsingViewport;
    WORD m_MaxViewObj;
    SimpleTimer m_ViewUpdateTimer;

    view_hash* m_pPreView;
    view_hash* m_pCurView;
    view_hash m_ReferedList;
    sortview_list m_sortViewList;
    DWORD m_dwSentSize;
    DWORD m_dwLastSendTick;

    static __player_set m_setCurrSector;
    static __player_set m_setPrevSector;
    static __player_set m_setEnterSector;
    static __player_set m_setLeaveSector;

    friend class Player; // (WAVERIX)(080527)(Logic Change) 구조 개선 중 임시값 좀더 access control 유연하게 변경하자.
};

template< class Operator >
void View::For_eachEnterMembers(Operator & Opr)
{
    __player_set_it it = m_setEnterSector.begin(), end = m_setEnterSector.end();
    for(; it != end ; ++it)
    {
        Player * pAround = *it;
        Opr(pAround);
    }
}

template< class Operator >
void View::For_eachLeaveMembers(Operator & Opr)
{
    __player_set_it it = m_setLeaveSector.begin(), end = m_setLeaveSector.end();
    for(; it != end ; ++it)
    {
        Player * pAround = *it;
        Opr(pAround);
    }
}


inline void
View::SendPacketAround(MSG_BASE_FORWARD* pMsg, WORD wSize, const BOOLEAN bSendToMe, const GUILDGUID other_guid_guid)
{
    SendExPacketAround(1, reinterpret_cast<BYTE**>(&pMsg), &wSize, bSendToMe, other_guid_guid);
}

*/
