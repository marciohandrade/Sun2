#pragma once
//==================================================================================================
/// GameZone class
/**
    @author	Kim Min Wook < taiyo@webzen.com >
    @since	2004. 12. 29
    @remark
        - GameServer에서 존을 관리하는 최상위 추상 클래스
    @note
        - 
    @history 
        - 2006.8.16 : GameZone 리팩토링 완료(Map클래스와의 연관 제거, 리로스MGR연결.. etc)
*/
//
//==================================================================================================

#include "GameZoneCommon.h"
#include <boost/bind.hpp>

//==================================================================================================
//  <TYPEDEFINITIONs>
class Map;
class Player;
class Monster;
class GameField;

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
class GameBattleResult;

//==================================================================================================
//  <GameZone>
//
class GameZone
{
#ifdef _NA_0_20100416_RELOAD_MAPOBJECT
    friend class ZoneReloader;
#endif

protected:
    explicit GameZone(void);
    void operator = (const GameZone&);
public:
    typedef ::_PLAYER_HASH _PLAYER_HASH;
    typedef _PLAYER_HASH::iterator _PLAYER_HASH_ITR;
    virtual ~GameZone(void);

    void Init(ZONEKEY zoneKey, MAPCODE mapCode,
              eZONE_RULESET::eRules rules/* = eZONE_RULESET::ZONE_RULE_EMPTY*/);
    void Release();

    // 몬스터가 죽었을 때 발생하는 이벤트 처리
    virtual void OnKillMonster(Monster* /*pMonster*/) {}


    // 방생성/삭제시 자식 클래스에서 호출되는 가상함수
    virtual void OnCreate();
    virtual void OnDestroy() {}

    virtual void Update(DWORD dwDeltaTick);

    virtual eZONETYPE GetZoneType() = 0;
    virtual void SetZone(Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos) = 0;

    virtual void Ready(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos)=0;
    virtual BOOL Join(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos);
    virtual void Leave(Player* pPlayer);

    virtual void LeaveForWarp(Player* pPlayer) { Leave(pPlayer); }

    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone(Player* /*pPlayer*/) { return RC::RC_STATUS_UNKNOWN; }

    virtual void Warp(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos=NULL);

    // Field간 이동
    virtual BOOL MoveField(Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos)
    { return Join(pPlayer, FieldCode, AreaID, pwvStartPos); }

    // 파티장 근처로 가기
    virtual RC::ePARTY_RESULT WarpAtPartyMaster(Player* /*pPlayer*/) { return RC::RC_PARTY_INVALID_ROOMTYPE; }
    virtual RC::ePARTY_RESULT SummonReqAtPartyMaster(Player* /*pPlayer*/, DWORD /*dwObjKey*/) { return RC::RC_PARTY_INVALID_ROOMTYPE; }

    virtual BOOL CanPVP(Player* /*pAttacker*/, Player* /*pTarget*/, OUT RC::eSKILL_RESULT* pErrCode) { *pErrCode = RC::RC_SKILL_SUCCESS; return TRUE; }
    virtual BOOL CanPVP(Player* /*pAttacker*/, Player* /*pTarget*/, OUT RC::eBATTLE_RESULT* pErrCode) { *pErrCode = RC::RC_BATTLE_SUCCESS; return TRUE; }
    virtual eUSER_RELATION_TYPE IsFriendPVP(Player* /*pAttacker*/, Player* /*pTarget*/) { return USER_RELATION_NEUTRALIST; }
    //_NA_008706_20160229_TEXTLOG_PK
    virtual void KillPlayer(Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0) { __UNUSED((pAttacker, pDeadPlayer, bSummoned, damage_value)); }
    virtual BOOL IsPvPAtt(Player* /*pPlayer*/, WORD /*PTATroubld*/) { return FALSE; }

    virtual void GetArea(GameField* pField, Player* pPlayer, MAPCODE FieldCode, WzID& AreaID, WzVector& rwvStartPos);

    DWORD GetEntryAreaID();
    GameField* GetEntryField() const;
    MAPCODE GetEntryFieldCode() const;
    // (f100623.1L) change an interface to support a multi-checksum checker
    virtual bool CheckMapVersion(FIELDCODE field_code,
                                 EngineFileChecksum* stored_checksums,
                                 const EngineFileChecksum& inputed_checksums) const;
    //virtual BOOL CheckMapVersion(FIELDCODE fieldCode, DWORD checksum, DWORD* OUT pCurCheckSum = NULL);

    virtual BOOL IsApplyPenalty() { return TRUE; }

    virtual float GetBonusExp(Player* /*pPlayer*/) { return 0; }
    virtual BOOL CanApplySuperMasterBenefit(Player* /*pPlayer*/) { return FALSE; }

    virtual void SetSpecialType(eROOM_SPECIAL_TYPE /*eType*/) {}

    template <typename OPERATOR_T>
    void ForeachPlayer(OPERATOR_T& op)
    {
        for(_PLAYER_HASH::iterator it = m_PlayerHash.begin(); it != m_PlayerHash.end();)
        {
            Player* const player = it->second;
            if (obj_util::IsValid(player, __FUNCTION__, __LINE__))
            {
                op(player);
                ++it;
            }
            else
            {
                // 맵과 백터 구분 주의
                m_PlayerHash.erase(it++);
            }            
        }
    }
    
    template <typename FUNC_T, typename OBJ_T>
    void ForeachPlayer(FUNC_T func, const OBJ_T& obj)
    {
        for(_PLAYER_HASH::iterator it = m_PlayerHash.begin(); it != m_PlayerHash.end();)
        {
            Player* const player = it->second;
            if (obj_util::IsValid(player, __FUNCTION__, __LINE__))
            {
                boost::bind(func, obj, _1)(*it);
                ++it;
            }
            else
            {
                // 맵과 백터 구분 주의
                m_PlayerHash.erase(it++);
            }            
        }
    }

    const _PLAYER_HASH& GetRefPlayerList() const 
    { 
        return m_PlayerHash; 
    };

    void SendPacketToAll(MSG_BASE_FORWARD* pMsg, WORD wSize, DWORD dwMyObjKey = 0);
    
    DWORD GetPlayerNum() const
    { 
        return (DWORD)m_PlayerHash.size(); 
    }
    
    Player* FindPlayer(const PLAYERKEY key) const
    {
        _PLAYER_HASH::const_iterator it = m_PlayerHash.find(key);
        if (it == m_PlayerHash.end()) 
        {
            return NULL;
        }
        Player* const player = it->second;
        if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
        {
            return NULL;
        }
        return player;
    }
    
    GameField* FindGameField(const FIELDCODE code) const
    {
        _GAMEFIELD_HASH::const_iterator it = m_GameFieldHash.find(code);
        if (it == m_GameFieldHash.end())
        {
            return NULL;
        }
        return it->second;
    }

    const _GAMEFIELD_HASH& GetFieldHash() const 
    { 
        return m_GameFieldHash; 
    }

    DWORD GetTotalMonsterNum(DWORD& OUT dwDeadNPCNum);
    
    //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    WORD GetNumberOfExpectedPlayers() const 
    { 
        return m_NumberOfExpectedPlayers; 
    }
    
    void PrepareNumberOfExpectedPlayers(const WORD number_of_expected_players)
    {
        m_NumberOfExpectedPlayers = number_of_expected_players;
    }
    
    BOOLEAN UpdateNumberOfExpectedPlayers(WORD* number_of_active_players); //Get & Replace

#ifdef _NA_20081121_ABUMAN_NIJOT_EVENT
    bool IsAllSameGuildMember(); //현재 게임존에 있는 모든 유저가 같은 길드원인가?
#endif

    virtual void ResetTrigger();
    void CreateTrigger();
    void DestroyTrigger();
    BOOL TriggerActive(WzID triggerID);
    void TriggerInactive(WzID triggerID);
    void AllTriggerInactive();
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual GameBattleResult* GetGameBattleResult() const 
    { 
        return m_pBattleResult; 
    }


public:
    virtual void ChangeHandler(const eZONETYPE kindOfZone,
                               const eINSTANCE_DUNGEON_KIND kindOfIndun = INDUN_KIND_MAX);
    sPOLICY_ZONE Policyment;    // (PROPERTY) Zone Logic Controller
    const eZONE_RULESET Ruleset;
    //----------------------------------------------------------------------------------------------
    //  (Fields)
    //
    __PROPERTY(ZONEKEY, Key);
    __PROPERTY(MAPCODE, MapCode);
    __PTR_PROPERTY(sMAPINFO, MAPINFO);
private:
    WORD m_NumberOfExpectedPlayers;  //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    WORD m_NumberOfExpectedPlayersPrev;
    BOOL m_bCreateTrigger;

protected:
    _GAMEFIELD_HASH m_GameFieldHash; // 앞의 __PTR_PROPERTY 매크로에 의해 protected다... 의도한 건지 모르겠다.
    _PLAYER_HASH m_PlayerHash;

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameBattleResult*   m_pBattleResult;
};