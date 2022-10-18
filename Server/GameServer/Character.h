#pragma once
#ifndef GAMESERVER_CHARACTER_H
#define GAMESERVER_CHARACTER_H

//==================================================================================================
//
// Character.h: interface for the Character class.
//
//==================================================================================================

#include <SCCharacter.h>
#include <PacketStruct.h>
#include <Attribute.h>
//---------------------------
#include <PartyState.h>
//---------------------------
#include "Object.h"
#include "ObjectPolicy.h"
#include "ObjectList.h"

//==================================================================================================

class CWorldBase;       class CPathExplorer;        struct WzVector;
//
class Skill;            class SkillManager;
class Status;           class StatusManager;
class SummonedNPCs;
class AbilityStatus;    class Ability;
class Zone;             class GameZone;             class Sector;
//
class Attributes;       class MoveStateControl;
//
class ChaosZoneManager;
//
struct STATE_INFO;      struct AI_MSG;

/*
 = Character�� ���� Observers�� �ǹ�
    * Character�� ���� Target���� �����ϰ� �ִ� �ֵ�

 = Character�� ���� Observers�� �뵵
    * ������ �������� ���� �ְ� A�� ġ���ϴ� ��� --> A�� Observers���� �˸���.
    * �������� A�� field�� ������ ��� --> A�� Observers���� �˸���.
*/

//==================================================================================================
// Character
//
class Character : public Object,
                  public ICharacterParameter//,
                  //public SCCharacter
{
    friend class nsAI::MovingPlanner;
public:
    Character();
    virtual ~Character();

    virtual BOOL Init();
    virtual void Reuse();
    virtual BOOL Update(DWORD dwDeltaTick);
protected:
    virtual void Release();

public:
    //==============================================================================================
    //  <Policy Control>
    //  -   ZoneType�� ���� �ش� ĳ������ ó������� �޶����� ��쿡 ���� ���� ��å ó��
    void ChangePolicy(GameZone* const pGameZone);
    // (CHANGES) (100205) (WAVERIX) EnterField ����, Character�� ���� ���λ��� ���� �ʿ��� ���
    // �߰� ���� �����ϱ� ����. Ư�� ���׸� ó���� ��
    void ChangePolicyPostEnterField(const sPOLICY_CHARACTER::eFLAG add_flag,
                                    const sPOLICY_CHARACTER::eFLAG del_flag,
                                    Object* related_object);
    void ClearKillerInfo();

    //==============================================================================================
    //  <SendPacket Methods>
    virtual BOOL SendPacket(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bCrypt = false);
    virtual BOOL SendExPacket(DWORD dwNumberOfMessages, BYTE **pMsg, WORD *pwSize);
    virtual void SendPacketAround(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bSendToMe = true);
    virtual void SendExPacketAround(DWORD dwNumberOfMessages,
                                    BYTE **pMsg, WORD *pwSize, BOOL bSendToMe = true);
    template< typename _MSGSUBTYPE >
    void SendPacketAroundForSync(const _MSGSUBTYPE* const pMsgSub, const BOOLEAN bPostAction = false);
private:
    void SendPacketAroundForSync_(const BYTE _TAG, WAR_TAG_V* const IN pMsg, const WORD wSize,
                                  const BOOLEAN bPostAction = false);
    //
public:
    //==============================================================================================
    //  <Zone>
public:
    virtual void SetField(GameField* pField);
    virtual void SetGameZone(eZONE_STATE eState, GameZone* pZone,
                             MAPCODE EnterFieldCode, WzID AreaID = 0, WzVector* pwvStartPos = NULL);
    eZONE_STATE GetGameZoneState() const;
    GameZone*   GetGameZonePtr() const;
    eZONE_STATE GetGameZone(GameZone*& OUT pZone, MAPCODE& OUT enterFieldCodeOut,
                            WzID* OUT pAreaID = NULL, WzVector* OUT pwvStartPos = NULL);

    ChaosZoneManager* GetChaosZoneManager();
    //==============================================================================================
    // �̵�, ����ȭ ���� + ���
public:
    // �ָ� �̵����� ��� ���ε��� �������� �̵������� ������.
    virtual void SendMoveInfo(Player* pEnterPlayer, const Sector& IN rSector);
    BOOL CanMove(eCHAR_STATE_TYPE eException = eCHAR_STATE_INVALID);
    ///<��ã�� ��� ����
    BOOL SetPath(WzVector* pwvCur, WzVector* pwvDest, int* pTileRoute, int iNumTileRoute);
    BOOL PathProgress(float fMove, ENUM_CHECK_COLLISION CollisionCheck=ECC_NO);

    virtual BOOL ExecuteThrust(BOOL bForcedMove, WzVector* pwvDestPos,
                               WzVector& wvPosAfterThrust, float fMoveDistance,
                               BOOL bDownAfterThrust = false);
    virtual BOOL IsMoving();
    virtual void SetActionDelay(DWORD delay);

    CPathExplorer* GetPathExplorer() const;
    BOOL GetTileAtt(WORD* wTileAtt);
    void StopMoving();
    BOOL IsStandingOnJumpTile();
    // ���� ���ֺ��� �ݿ��� �������� ����ִ� ������ �Ҵ����ش�.
    BYTE AllocEnemySlot(WzVector* pwvEnemyPos, WzVector &wvResultPos, float fAreaRadius);
    void FreeEnemySlot(BYTE bySlot);
    // TODO, waverix, reserve delete that this routine is a temporay save for a summoned.
    BOOL ModifyPosition(BOOL bIsPlayer, WzVector& vCurPos, WzVector& vDestPos,
                        WzVector& vResultCurPos, WzVector& vResultDestPos, SLOTCODE SkillCode = 0);
    // UNDONE | CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
    bool ModifyPosition(SkillInfo* preparing_skill_info_data);

    float GetDist(Character* pTargetCharacter) const;
    float GetDist(const WzVector& targetPos) const;
    float GetDist2(Character* pTargetCharacter) const;    // �Ÿ� ����
    float GetDist2(const WzVector& targetPos) const;
    float GetHeightDiff(const WzVector& targetPos) const;
protected:
    virtual void _SetPosVector(const WzVector* const pwzVector);   // protector
public:
    void SetPos(const WzVector* pwzVec);
    virtual void GetDestPos(WzVector* pWzVecOut) const;
    WzVector GetRandomPosInAround(int iDist, int iLoop = 10);

public:
    //==============================================================================================
    // ICharacterParameter interfaces
    //virtual Attributes& GetAttr() = 0;
    //virtual const Attributes& GetAttr() const = 0;
    ////_NA_001231_20081216_ADD_OPTION_KIND 
    virtual int GetResistBadStatusRatio(WORD state_id = eCHAR_STATE_INVALID);
    virtual Player* GetPlayerOwner();
    //virtual LEVELTYPE GetLevel() const = 0;
    virtual LEVELTYPE GetDisplayLevel() const;
    //virtual float GetAttackRange() const = 0;
    //virtual float GetNormalRangeBonus() const = 0;
    //virtual float GetSkillRangeBonus() const = 0;
    //virtual float GetSightRange() const = 0;
    //virtual eARMOR_TYPE  GetArmorType() const = 0;
    //virtual eMELEE_TYPE  GetMeleeType() const = 0;
    //virtual eATTACK_TYPE GetWeaponBaseAttType() const = 0;       // ���� �⺻ ����Ÿ��(�ٰŸ�, ���Ÿ�, ����)
    //virtual eATTACK_TYPE GetWeaponMagicalAttType() const = 0;    // ���� �Ӽ� ����Ÿ��(��, ��, ...)
    //virtual DWORD GetHP() const = 0;
    //virtual DWORD GetMP() const = 0;
    //virtual DWORD GetSD() const = 0;
    //virtual float GetHPRatio() const = 0;
    //virtual float GetMPRatio() const = 0;
    //virtual float GetSDRatio() const = 0;
    //virtual DWORD GetMaxHP() const = 0;
    //virtual DWORD GetMaxMP() const = 0;
    //virtual DWORD GetMaxSD() const = 0;
    virtual int GetRegenHP() const;
    virtual int GetRegenMP() const;
    virtual int GetRegenSD() const;
    //virtual const TCHAR* GetCharName() const = 0;

    virtual BOOL IsAlive() const;
    virtual BOOL IsDead() const;
    //virtual DWORD GetPhysicalAvoidValue() const = 0;
    //virtual float GetPhysicalAttackSpeed() const = 0;
    //virtual EXPTYPE GetExp() const = 0;
    virtual short GetExperty1() const;
    virtual short GetExperty2() const;
    //virtual int GetMoveSpeedRatio() const = 0;
    //virtual int GetAttSpeedRatio() const = 0;
    virtual float GetBodyRadius() const;
    virtual int   GetMPSpendIncValue() const;
    virtual float GetMPSpendIncRatio() const;

    //virtual void SetAttr(eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, int iValue) = 0;
    //virtual void SetHP(DWORD hp) = 0;
    //virtual void SetMP(DWORD mp) = 0;
    //virtual void SetSD(DWORD sd) = 0;
    ////
    virtual DWORD IncreaseHP(DWORD increment);
    virtual DWORD DecreaseHP(DWORD decrement, int fLimitHP = 0);
    virtual DWORD IncreaseMP(DWORD increment);
    virtual DWORD DecreaseMP(DWORD decrement);
    virtual DWORD IncreaseSD(DWORD increment);
    virtual DWORD DecreaseSD(DWORD decrement);
    //
    //==============================================================================================
    MoveStateControl* GetMoveStateControl() const;
    void SendAttrChange(eATTR_TYPE attr_type, DWORD attr_value);
    static const BOOL IsPCObject(const Character* pChar);
    //}
    void SetMoveState(eCHAR_MOVE_STATE state);

    virtual EXPTYPE AddExp(EXPTYPE exp, DWORD dwTargetObjKey=0, float fBonusRatio=0, BOOL bSendPacket=true);

    void SetShield(int iShieldHP, int iDecreaseMP, float fRatio, float DecreaseMPRatio);
    virtual void UpdateCalcRecover(BOOL bHPUpdate, BOOL bMPUpdate, BOOL bSDUpdate);
public:
    BOOL CheckCondition(BYTE byCondition, WORD wParam);
    BOOL CheckCondition_Under_HP(WORD wPercent);
    BOOL CheckCondition_Under_MP(WORD wPercent);
    BOOL CheckCondition_Same_HP(WORD wPercent);
    BOOL CheckCondition_Same_MP(WORD wPercent);

    //  Observer : ���� ���Ѻ��� �ֵ�
    void SendToObservers(AI_MSG* pMsg, WORD wSize);
    virtual void SendAIMessage(AI_MSG* pMsg, WORD wSize, DWORD dwDelay=0);
    ObjectList& GetObservers();
    const ObjectList& GetObservers() const;

    //  �ڽ��� ��ȯ�� ĳ���� //_NA_000000_20140224_SUMMONER_REFACTORING
    DWORD GetSummonerKey() const;
    void  SetSummonerKey(DWORD dwSummonerKey);
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    virtual eSUMMON_TYPE GetSummonType() const {__UNUSED(); return eSUMMON_MAX;}
#endif
    //==============================================================================================
    //  <Team>
    ChaosZoneTeam  GetTeamKey() const;
    void SetTeamKey(const ChaosZoneTeam team_key);
    //==============================================================================================
    //  <Party>
    PartyState& GetPartyState();
    //==============================================================================================
    // Status
    StatusManager* GetStatusManager() const;
    BOOL IsStatus(eCHAR_STATE_TYPE eStateID);
    //==============================================================================================
    //  <Skill>
    SkillManager* GetSkillManager() const;

    BOOL _ChkNeedHP(WORD wHPSpend);
    BOOL _ChkNeedMP(WORD wMPSpend);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual BOOL ChkNeedFP(PLAYERFP fp_spend) {__UNUSED(fp_spend); return FALSE;}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    static  BOOL finite(WzVector& wvPos);

    RC::eSKILL_RESULT  _CanUseSkill(SLOTCODE SkillCode);
    virtual RC::eSKILL_RESULT CanUseSkillItem(SLOTCODE SkillCode);
    virtual BOOL IsTotemSkillAreaType() { return true; }

    virtual RC::eSKILL_RESULT CanUseSkill(SLOTCODE SkillCode, BOOL bCoolTimerReset = true);
    // CHANGES, f100824.7L, remove a method which is unused on the Character object level
    //virtual bool CanApplyPassiveSkill(SLOTCODE skill_code);
    BOOL CheckAttackRange(Character* pTarget, WzVector& vTargetPos);
    virtual BOOL CheckSkillRange(Character* pTarget, const WzVector& vTargetPos, float fSkillRange);
    BOOL CheckArea(const WzVector& vTargetPos, const WzVector& vMainTargetPos, BYTE byRangeForm, float fArea);
    void AddFightingEnergy(short sCount);
    void DelFightingEnergy(short sCount);
    short GetFightingEnergy() const;

    // ���� ���� ������ ��´�.
    int GetUsedFightingEnergySize() const
    {
        return used_fighting_energy_size_;
    }
    // ���� ���� ������ �����Ѵ�.
    void SetUsedFightingEnergySize(const int used_fighting_energy_size)
    {
        used_fighting_energy_size_ = used_fighting_energy_size;
    }

    void SetSkillCancel(SLOTCODE SkillCode, BOOL bCancel = true);
    void CancelAllSkill();
    //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK Character ���� �߰�
    BOOL CheckHeightRange(Character* pTarget, const WzVector& vTargetPos, float fBaseRange);
    virtual BOOL CheckClassDefine(SLOTCODE SkillCode, BOOL bUseActiveSkill);
    // TODO: need recheck... _NA001385_20090924_DOMINATION_FINDPATH_ATTACK
    BOOL CheckAttackFindPath(const WzVector& vMainTargetPos);
public:
    //==============================================================================================
    //  <Skill+Battle>
    // ��Ȱ�� ��ų�� ȣ��
    virtual BOOL OnResurrection(float fRecoverExp, float fRecoverHP, float fRecoverMP,
                                Player* pHealer = NULL);
    virtual void ApplyPenalty() {} // ����� �г�Ƽ ����(����� �÷��̾ ����)
    virtual void OnDead(); // ������ ȣ��
    virtual BOOL CanAttack() const;
    virtual BOOL CanBeAttacked() const;
    virtual void OnAttack(Character* pTarget, SLOTCODE skillcode, DAMAGETYPE damage); // �����Ҷ�
    virtual void Attacked(); // ���ݹ�����
    //_NA_001231_20081216_ADD_OPTION_KIND
    DAMAGETYPE CalcDecreaseDamage(DAMAGETYPE damage, int nResistVlue, int nResistRatio);
private:
    DAMAGETYPE _ApplyOptionDecDamage(Character* pAttacker, DAMAGETYPE damage,
                                     eATTACK_RESIST attackKind);

    //_NA_0_20100211_SD_EFFECT_MODIFY
    DAMAGETYPE  _ApplyShieldPoint(Character* pAttacker, DAMAGETYPE damage, eSD_APPLY SDApply);

    DAMAGETYPE  _ApplyMagicShield(DAMAGETYPE wDamage);
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
    void WriteMaxDamageLog(Character* pAttacker, DamageArgs* damage_arg);
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG

protected:  // ���� ����(�ڽ� Ŭ����) ȣ���
    // Damage ���� ����
    virtual void _Damaged(DamageArgs* damage_arg);
#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    virtual VOID _WriteMondschienLog(Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value) 
            {__UNUSED(attacker, skill_code, damage_value);}
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
    virtual VOID _WriteBattleMonitorLog(Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value)
            {__UNUSED(attacker, skill_code, damage_value);}
#endif //_NA_008706_20160229_TEXTLOG_PK

public:
    // �ܺ�(Ÿ Ŭ����) ȣ���
    // _Damaged �����Լ�
    void Damaged(DamageArgs* damage_arg);
    //// _Damaged �����Լ�
    //DAMAGETYPE Damaged(Character* pAttacker, eATTACK_TYPE attackType, DAMAGETYPE damage,
    //                   BOOL bCritical, BOOL bMirror = false 
    //                   , int fLimitHP = 0
    //                #ifdef __NA_001244_20090417_ATTACK_RESIST
    //                   , eATTACK_RESIST attackKind = eATTACK_RESIST_SKILL
    //                #endif
    //                   , eSD_APPLY SDApply = eSD_APPLY_EMPTY 
    //                       );

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual void OnRecover(int recoverHP, int recoverMP, int recoverSD = 0,
                           eRECOVER_TYPE Type = eRECOVER_TYPE_NORMAL, Character* pAttacker = NULL);

    void PeriodicRecover(eCHAR_STATE_TYPE eStateID, int recoverHP, int recoverMP,
                         int iApplicationTime, int iPeriodicTime);
    virtual void SetTargetChar(Character* pChar);
    virtual Character* GetTargetChar();
    virtual void AddBattlePoint(Character* pAttacker,
                                eBATTLE_POINT_TYPE ePointType, int iBattlePoint);

    virtual eUSER_RELATION_TYPE IsFriend(Character* pTarget);
//#if !defined(_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM)
    virtual BOOL IsCoincidentWithTargetType(Character* pTarget, eSKILL_TARGET_TYPE eTargetType);
//#endif
    virtual void ChangeState(DWORD dwStateID, LPARAM param1 = 0, LPARAM param2 = 0, LPARAM param3 = 0);
    virtual void ForcedAttack(Character* pTarget);
    virtual BOOL CanResurrection(Character* pTarget);

    //==============================================================================================
    // CHANGES, f100827.4L, r100827.4L, SCCharacter interfaces related to a cool-timer
    void SetSkillCoolTime(SLOTCODE skill_code, WORD cool_time_margin = 0);
    void FreeSkillCoolTime(SLOTCODE skill_code);
    void FreeAllSkillCoolTime(BOOL pool_use);
    void InitializeSkillCoolTime();
    util::Timer* GetSkillCoolTimer(SLOTCODE skill_code);
    //
    bool CheckCoolTimer(SLOTCODE skill_code) const;
    bool CoolTimerReset(SLOTCODE skill_code);

    //==============================================================================================
    // for GameField/Sector grid
    virtual void OnEnterField(GameField& IN rField, const WzVector& IN rwzVec, int iAngle=-1);
    virtual void OnLeaveField();

    void  SetReserveHP(DWORD dwReserveHP); //������ HP
    DWORD GetReserveHP() const;
    void  SetDeadReserveHP(DWORD dwDeadReserveHP); //������ HP
    DWORD GetDeadReserveHP() const;
    eCHAR_DEAD_TYPE GetDeadType() const;
    void SetDeadType(eCHAR_DEAD_TYPE DeadType);
    //__NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE
    void GetEtcStateTotalInfo(ETC_STATE_TOTAL_INFO* OUT rEtcStateTotalInfo);

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    const COOL_TIME_TABLE& get_cool_time_table() const
    {
        return skill_cool_time_table_;
    }
#endif

    //==============================================================================================
    //==============================================================================================
    //==============================================================================================
    // start data fields...
public:
    //==============================================================================================
    // properies
    sPOLICY_CHARACTER Policyment;
    nsAI::IPlanEventObject PlanEvent; // Property
private:
    enum { MAX_ENEMYSLOT_NUM = 12 }; // 4�� ��� ��õ!
    //
    CPathExplorer* m_pPathExplorer; ///< ��ã��� 

    int m_iEnemySlot[MAX_ENEMYSLOT_NUM];// ���� �ѷ��ΰ� �ִ� Position ���¸� �������ִ�.(���� ��ġ�� ���� ������ count�� �þ��.)
    ObjectList m_Observers; // �갡 ���ϴ��� ���Ѻ��� �ִ� �����
protected:
    StatusManager* m_pStatusManager; // ĳ������ ���� ���� ����Ʈ
    SkillManager*  m_pSkillManager;
    DWORD m_dwSummonerKey;                // ��ȯ����Ʈ(SummonedNPCs)�� Ű��, ��ȯ���� ���� �ְ� ��ȯ�� ����� ���� �ִ�.

    ChaosZoneTeam team_key_; // ��Ƽ ���� ������ �� ���� 2013.01.04 / ������ <��Ű�� ������ ��ƼŰ�� �ٸ����� �Ʊ����� �ν�>
    PartyState m_PartyState;
    MoveStateControl* m_pMoveStateControl;
    struct {
        // Zone
        GameZone*   m_pGameZone;
        eZONE_STATE m_eGameZoneState;
        WzID        m_GameAreaID;
        MAPCODE     m_GameFieldCode;
        WzVector    m_GameStartPos;
    };
    struct {
        // ��ų ����(���߿� �������� ���°� ���� ��...)
        int   m_iShieldHP;            // ���� ���忡 ���� HP
        int   m_iShieldMP;            // ���� ���� ����� MP ���� ��ġ
        float decrease_MP_Ratio;      // ���� ���� ����� MP ���� ����
        float m_fShieldAbsorbRatio;   // ���� HP ��� ����
        short m_sFightingEnergyCount; // ���� ����
        int used_fighting_energy_size_; // ���� ���� ����
    };
    // ��� �г�Ƽ ���� ����(�������� ���� ����...)
    EXPTYPE m_DeadExp; // ������� ���غ� ����ġ

    // ���� ������Ʈ Ű
    eOBJECT_TYPE m_KillerType;
    DWORD m_dwKillerObjectKey;

    DWORD m_dwReserveHP; //������ HP
    DWORD m_dwDeadReserveHP; //ĳ���Ͱ� �׾������� ������ HP.
    eCHAR_DEAD_TYPE m_DeadType; //��������(0:��������, 1:����, 2:�׾ ȥ���� ����)
    // f100827.4L
    COOL_TIME_TABLE skill_cool_time_table_;
};

//==================================================================================================

#define __GAMESERVER_CHARACTER_INL
#include "./Character.inl"
#undef __GAMESERVER_CHARACTER_INL

#endif // !defined(GAMESERVER_CHARACTER_H)
