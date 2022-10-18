// NPC.h: interface for the NPC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPC_H__6A6486A6_D04B_4978_88CE_BD1C88F269F0__INCLUDED_)
#define AFX_NPC_H__6A6486A6_D04B_4978_88CE_BD1C88F269F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Const.h>
#include <Struct.h>
#include <Timer.h>
#include <RatioSelector.h>
#include <misc.h>
#include <SCNPC.h>
#include <RegenParser.h>

#include "Character.h"
//#include "BattleRecord.h"
#include "NPCAttrForUpdate.h"
#include "VarMsg.h"
#include "UnitRegenInfo.h"
#include "UnitGroupInfo.h"
#include "AppliedNpcDependencyChain.h"


struct AI_MSG;
class State;
class AIMessageQueue;
class Character;
class NPCStateManager;
class TargetManager;
class UnitListOfGroup;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<NPC>
class NPC : public Character, public SCNPC
{
	enum { NPC_SKILL_COOLTIME_INIT_NUM = 10 }; 
public :
	typedef RatioSelector<SLOTCODE>		SkillSelector;

public:		NPC();
	virtual	~NPC();

	virtual	BOOL				Init();
	virtual	BOOL				Update( DWORD dwDeltaTick );
    virtual VOID                Reuse();
protected:
	virtual	VOID				Release();
	virtual	VOID				SetInitialState( DWORD dwStateID, BYTE byMoveType=0, DWORD dwMoveAreaID=0, LPARAM param1=0 );
public:
	virtual	VOID				OnEnterField( GameField& IN rField, const WzVector& IN rwzVec, INT iAngle=-1 );
	virtual	VOID				OnLeaveField();
	virtual	VOID				OnEnterObject( const Sector& IN rSector );
	virtual	VOID				OnLeaveObject( const Sector& IN rSector );
    virtual	void                SendMoveInfo(Player* pEnterPlayer, const Sector& IN rSector); //_NA_0_20100817_HELLON_LAST_TRIGGER

            BASE_NPCINFO*       GetBaseInfo() const;
public:		VOID				SetBaseInfo( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID, DWORD dwStateID=2, LPARAM param1=0 );
	//////////////////////////////////////////////////////////////////////////
	//	<SERIALIZER RENDER INFO>
public:		VOID				GetNPCRenderInfo( MONSTER_RENDER_INFO* OUT pRenderInfo );
	virtual	VOID				GetSummonedRenderInfo( SUMMONED_RENDER_INFO* OUT pRenderInfo );
	virtual	VOID				GetTotemNPCRenderInfo( TOTEMNPC_RENDER_INFO* OUT pRenderInfo );

	//////////////////////////////////////////////////////////////////////////
	//	<ATTRIBUTEs>
public:
    virtual Attributes&         GetAttr();
    virtual const Attributes&   GetAttr() const;

    virtual LEVELTYPE           GetLevel() const;
    virtual LEVELTYPE           GetDisplayLevel() const;
            BYTE                GetGrade() const;
            FLOAT               GetGradeExpRatio();
	virtual	FLOAT				GetAttackRange() const;
    virtual FLOAT               GetNormalRangeBonus() const;
    virtual FLOAT               GetSkillRangeBonus() const;
    virtual FLOAT               GetSightRange() const;
    virtual eARMOR_TYPE         GetArmorType() const;
    virtual eMELEE_TYPE         GetMeleeType() const;
    virtual eATTACK_TYPE        GetWeaponBaseAttType() const;
    virtual eATTACK_TYPE        GetWeaponMagicalAttType() const;
    virtual DWORD               GetHP() const;
    virtual DWORD               GetMP() const;
	virtual	FLOAT				GetHPRatio() const;
	virtual	FLOAT				GetMPRatio() const;
    virtual DWORD               GetMaxHP() const;
    virtual DWORD               GetMaxMP() const;
    virtual const TCHAR *       GetCharName() const;
    virtual INT                 GetRegenHP() const;
    virtual INT                 GetRegenMP() const;
    virtual DWORD               GetPhysicalAvoidValue() const;
    virtual FLOAT               GetPhysicalAttackSpeed() const;
    virtual INT                 GetMoveSpeedRatio() const;
    virtual INT                 GetAttSpeedRatio() const;
    virtual FLOAT               GetBodyRadius() const;

    virtual DWORD               GetMaxSD() const;
    virtual INT                 GetRegenSD() const;
    virtual FLOAT               GetSDRatio() const;
    virtual VOID                SetSD( DWORD dwSD );
    virtual DWORD               GetSD() const;

	virtual	VOID				SetAttr( eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, INT iValue );
	virtual	VOID				SetHP( DWORD dwHP );
    virtual VOID                SetMP( DWORD dwMP );
	virtual	VOID				SetHPAttackPowerAsParty( DWORD dwCurMemberNum );
	// 일단은 무시. 렙업하는 NPC도 있겠지?;
    virtual EXPTYPE             GetExp() const;
	inline eTARGET_SEARCH_TYPE  GetSearchType() const;  //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
	BOOL						IsHPMPRegenNPC();	// 자동 HP 리젠되는 NPC인가?
    BOOL                        IsSDRegenNPC();
	//////////////////////////////////////////////////////////////////////////
	//	<BATTLE CONTROL>
public:
    inline  NPCStateManager*    GetStateManager() const;
    inline  TargetManager*      GetTargetManager() const;
    inline  SkillSelector&      GetSkillSelector();
    inline  VOID                ChangeNpcDependencyChain( const INpcLinkActionD& rNewAction );

	virtual	VOID				SetTargetChar( Character* pChar );
	virtual	Character*			GetTargetChar();
public:		Character*			SelectSkillTarget( SLOTCODE SkillCode, BOOL bHeal=FALSE );
			eUSER_RELATION_TYPE	IsFriend( Character* pTarget );

public:
	virtual	VOID				StatusResist();		// 상태저항
	virtual	BOOL				IsDelegate()	{ return TRUE; }


//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual	VOID				OnRecover( INT recoverHP, INT recoverMP, INT recoverSD = 0, eRECOVER_TYPE Type = eRECOVER_TYPE_NORMAL, Character* pAttacker = NULL );

    void SendHPChange();
    void SendAreaDamage(const DAMAGETYPE damage);

public:

	virtual	VOID				SpecialAction();	// 특수행동 //_NA_0_20100514_NPC_DEAD_STATE_TIME(삭제)
            VOID                SpecialAction(const SPECIAL_CONDITION& special_condition);
            SPECIAL_CONDITION*  GetDrawLotsSpecialAction();
public:		BOOL				IsSpecialAction( eNPC_SPECIAL_ACTION_TYPE eType );
			ENUM_STATD_ID		GetCurStateID();
			/*FLOAT				GetDistToTarget() const;*/
			FLOAT				GetDistToNewTarget( Character* pNewTargetChar ) const;
	//////////////////////////////////////////////////////////////////////////
	//	<SKILL CONTROL>
	// 스킬 관련
#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
protected:
#else
private:
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL
	virtual	VOID				AddDefaultSkill();		// NPC가 사용가능한 스킬들을 등록한다.
public:		BOOL				UseSkill( SLOTCODE SkillCode );

	virtual	RC::eSKILL_RESULT	CanUseSkill( SLOTCODE SkillCode, BOOL bCoolTimerReset = TRUE );
            VOID				SelectSkill( Character* pTarget, SLOTCODE SkillCode
#ifdef _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
                , WORD SelectedSkillDelay = 0 
#endif // _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY
                );
            VOID				SelectSkill();
            SLOTCODE            GetSelectedSkill() const;
			VOID				ResetSelectedSkill();
            WORD                GetSelectedSkillDelay() const;
			BOOL				UseResurrectionSkill();
	virtual	BOOL				CheckSkillRange( Character* pTarget, const WzVector& vTargetPos, FLOAT fSkillRange );

	//////////////////////////////////////////////////////////////////////////
	//	<POSITION INFORMATION>
public:		BOOL				IsSamePositionWithTarget();
			BOOL				IsTooCloseToTarget();		// 너무 가까우면 멀어지기
			BOOL				NeedToRetreatFromTarget();	// target으로부터 멀어질 필요가 있는 경우
public:
    virtual BOOL                IsOutOfWanderRadius();
    virtual BOOL                IsOutOfRegenLocationLimit( WzVector &vTargetPos );
    virtual VOID                SetRegenPos( const WzVector& regenPos );
public:		VOID				GetTrackPos( WzVector& wvDestPos, FLOAT fAttackRangeRatio = 0.9f );
	virtual	BOOL				ExecuteThrust( BOOL bForcedMove, WzVector* pwvMoveDistance, WzVector& wvPosAfterThrust, FLOAT fMoveDistance, BOOL bDownAfterThrust );
			BOOL				MoveAndBroadcast( WzVector* pwvDestPos, eCHAR_MOVE_STATE moveState );
			BOOL				ThrustMoveAndBroadcast( WzVector* pwvDestPos, eCHAR_MOVE_STATE moveState, BOOL& bTouchedSomething, WzVector& wvRotatedVector );

	//////////////////////////////////////////////////////////////////////////
	//	<JUMP CONTROL>
public:		BOOL				IsOnGoodPositionToJump();
			BOOL				CanJumpDist( Character *pTarget );
			BOOL				SetJumpPos( Character *pTarget );
            WzVector&           GetJumpPos();

	//////////////////////////////////////////////////////////////////////////
	//	<STATE & AI CONTROL>
	virtual	VOID				ChangeState( DWORD dwStateID, LPARAM param1 = 0, LPARAM param2 = 0, LPARAM param3 = 0 );
public:		VOID				SendAIMessage( AI_MSG* pMsg, WORD wSize, DWORD dwDelay = 0 );
			VOID				SendAIMessageToGroupExceptMe( AI_MSG* pMsg, WORD wSize, DWORD dwDelay = 0 );
			VOID				SendAIMessageAroundExceptMe( AI_MSG* pMsg, WORD wSize, DWORD dwDelay = 0 );

			VOID				GroupCmdAttackToGroup( Character* pTargetChar );
			VOID				GroupCmdStopAttackToGroup();
protected:	VOID				AIMsgProgress();
public:
	virtual	VOID				OnAttack( Character *pTarget, SLOTCODE skillcode, DAMAGETYPE damage );

protected:
    virtual void _Damaged(DamageArgs* damage_arg);

public:

	virtual	VOID				AddBattlePoint( Character* pAttacker, eBATTLE_POINT_TYPE ePointType, INT iBattlePoint );
	Character*					SearchTarget();

	virtual	BOOL				OnResurrection( FLOAT fRecoverExp, FLOAT fRecoverHP, FLOAT fRecoverMP, Player *pHealer=NULL );
	virtual	VOID				OnDead();

public:
	//////////////////////////////////////////////////////////////////////////
	//	<UNIT CONTROL>
    REGENID GetRegenID();
    CODETYPE GetTriggerAnimationCode();
    void SetTriggerAnimationCode(CODETYPE code);
	bool IsLeader(); //{ return m_pUnitRegenInfo->GetID() == m_pUnitGroupInfo->GetLeaderRegenID(); }
    // (WAVERIX) (090617) (BUG-FIX) 그룹 멤버들은 자발적으로 후퇴 못하는 상황 감소시키기 작업
    NPC* GetLeaderInField();
    bool IsFollower();
public:		bool				IsLeaderAlive();// { return m_pUnitListOfGroup->IsAlive(m_pUnitGroupInfo->GetLeaderRegenID()); }
public:
    bool                IsGroupMember();
    void                SetUnitGroupInfo( UnitGroupInfo* p );
    void                SetUnitRegenInfo( UnitRegenInfo* p );
    UnitRegenInfo*      GetUnitRegenInfo();

	//////////////////////////////////////////////////////////////////////////
	//	<GM CONTROL>
    VOID                SetGMIdleOn( BOOL bIdleOn );
    BOOL                ISGMIdleOn() const;
    ns_trigger::TriggerNpcLinkInGameSlots* GetTriggerLinks();
    // _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eTARGET_SEARCH_TYPE FindSearchTypeByAttitude(const BYTE attitude);
    void SetSearchType(const eTARGET_SEARCH_TYPE search_type, const DWORD object_key);
    Character* SearchTargetFriend();

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<FIELDs>
protected:	enum { eAIMESSAGE_TEMP_SIZE = 120, eAIMESSAGE_DEFAULT_SIZE = 250 };	// AI 관련
protected:
	static	VarMsg2				m_VarMsg2;
protected:	BASE_NPCINFO*		m_pBaseInfo;	// NPCInfoParser에 있는 정보를 공유하는 포인터이므로 삭제하지 말것

protected:	NPCStateManager*	m_pNPCStatusManager;
			TargetManager*		m_pTargetManager;
protected:	INpcLinkActionD		m_LinkedAction;

protected:	AIMessageQueue*		m_pAIMessageQueue;
			AIMessageQueue*		m_pTempMessageQueue;

protected:struct {	// SKILL CONTROL FIELDs
			SkillSelector		m_SkillSelector;
			util::ITimerBase	m_pSkillSelectTimer[1];
			SLOTCODE			m_SelectedSkillCode;
			WORD				m_wSelectedSkillDelay;
		  };
protected:	NPCAttrForUpdate	m_Attr;	// 파라미터
			DWORD				m_dwHP;
			DWORD				m_dwMP;
            DWORD				m_dwSD;
            //
			INT					m_iAngle;
			
			BOOL				m_bDestroy;
			WzVector			m_vJumpPos;
			BOOL				m_bGMIdleOn;
protected:struct {	// UNIT CONTROL FIELDs
			UnitListOfGroup*	m_pUnitListOfGroup;
			UnitRegenInfo*		m_pUnitRegenInfo;
			UnitGroupInfo*		m_pUnitGroupInfo;
		  };
          //----------------------------------------------------------------------------------------
          // NOTE: trigger informantion, need code arrangement
          //_NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
          ns_trigger::TriggerNpcLinkInGameSlots trigger_links_;
          //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
          // 트리거를 통한 타켓 검색 변경을 위해 각NPC가 정보를 가진다. 
          eTARGET_SEARCH_TYPE search_type_;
          REGENID search_taget_unitID_;
          DWORD search_taget_mapobjectID_;
          MONSTERCODE search_taget_monstercode_;

          //_NA_0_20100817_HELLON_LAST_TRIGGER
          CODETYPE trigger_animation_code_;
          //----------------------------------------------------------------------------------------
};

#define __GAMESERVER_NPC_INL
#include "./NPC.inl"
#undef __GAMESERVER_NPC_INL

#endif