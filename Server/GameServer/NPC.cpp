// NPC.cpp: implementation of the NPC class.
//  @history
//  ~09/12/29
//  09/12/29|waverix|change NPC moving controllers by '_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_'
//                  |TARGETS={ ExecuteThrust, (Thrust)MoveAndBroadcast }|
//
//==================================================================================================
#include "stdafx.h"
#include "./NPC.h"

// Utility
#include <PerfCheck.h>
//==================================================================================================
// ProgramCommon
#include <PublicMath.h>
#include <SkillInfoParser.h>
#include <NPCInfoParser.h>
#include <AIParamParser.h>
#include <Battle.h>
#include <PacketControl/WarMessages_Sync.h>
//==================================================================================================
#include "GameServerEx.h"

#include "GameZone.h"
#include "GameField.h"
#include "GameRoom.h"
#include "MovingPlanner.h"

#include "CharacterFormula.h"

#include "MoveStateControl.h"
#include "NPCStateManager.h"
#include "AIMessageQueue.h"
#include "TargetManager.h"
#include "StatusManager.h"

#include "State.h"
#include "EtcStatus.h"
#include "RecoverStatus.h"
#include "ChaoState.h"

#include "Skill.h"
#include "UnitListOfGroup.h"
#include "UnitRegenInfo.h"

#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "Summoned.h"

//==================================================================================================

#include "GameParty.h"
#include "GamePartyManager.h"
#include "Player.h"
#include "TargetFinder.h"

//==================================================================================================
// Construction/Destruction

NPC::NPC()
:	m_pUnitRegenInfo(NULL)
,	m_pUnitListOfGroup(NULL)
,	m_pUnitGroupInfo( NULL )
{
    SetObjectType(NPC_OBJECT);
    // CHANGES: f110307.2L
    ;{
        ns_object::SearchReferences ref_option = { 0, };
        ref_option.hp = reinterpret_cast<int*>(&m_dwHP);
        _RegisterSearchReference(ref_option);
    };

	m_pAIMessageQueue = new AIMessageQueue(eAIMESSAGE_DEFAULT_SIZE);
	m_pTempMessageQueue = new AIMessageQueue(eAIMESSAGE_TEMP_SIZE);

	m_pNPCStatusManager = new NPCStateManager(this);
	m_Attr.SetOwner( this );
	m_pTargetManager = new TargetManager(this);
}

NPC::~NPC()
{
	SAFE_DELETE( m_pAIMessageQueue );	
	SAFE_DELETE( m_pTempMessageQueue );
	SAFE_DELETE( m_pNPCStatusManager );
	SAFE_DELETE( m_pTargetManager );
}

VOID NPC::Release()
{
	m_LinkedAction = NULL;
	// OnDead에서 하면 죽은 후에 부활이 안된다. 같은 적을 공격하던 애를 부활시키기 때문에..
	SetTargetChar( NULL );	

	Character::Release();
	m_Attr.Clear();	

	m_pNPCStatusManager->Release();
	m_SkillSelector.Release();

	// AIMessage 초기화
	m_pAIMessageQueue->Clear();
	m_pTempMessageQueue->Clear();
    m_pSkillSelectTimer->Stop();
    // CHANGES, f100827.4L
	FreeAllSkillCoolTime( TRUE );
    // CHANGES: f110311.4L add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.Disconnect(NULL);
}

BOOL NPC::Init()
{
	m_SelectedSkillCode		= SKILLCODE_INVALID;
	m_wSelectedSkillDelay	= 0;
	m_KillerType			= MAX_OBJECT;
	m_dwKillerObjectKey		= 0;
	m_bDestroy				= FALSE;
	m_iAngle				= -1;
	m_bGMIdleOn				= FALSE;
	m_pUnitRegenInfo		= NULL;
	m_pUnitListOfGroup		= NULL;
	m_pUnitGroupInfo		= NULL;

    // CHANGES: f110311.4L add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.ns_trigger::TriggerNpcLinkInGameSlots::~TriggerNpcLinkInGameSlots();
    new (&trigger_links_) ns_trigger::TriggerNpcLinkInGameSlots(this);
    // NOTE: need replacement, _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER = {
    search_type_ = eRARGET_NOT_SEARCH;
    search_taget_unitID_ = 0;
    search_taget_monstercode_ = 0;
    search_taget_mapobjectID_ = 0;
    //}
    trigger_animation_code_ = 0;

	if (!Character::Init())
	{
		return FALSE;
	}

	return TRUE;
}

VOID NPC::SetBaseInfo( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID, DWORD dwStateID, LPARAM param1 ) 
{ 
	
	m_pBaseInfo = NPCInfoParser::Instance()->GetNPCInfo( NPCCode );
	ASSERT( m_pBaseInfo );

	// HP, MP 회복률
	int iRecoverHP = 0;	
    int iRecoverMP = 0;
    int iRecoverSD = 0;

    if ( IsHPMPRegenNPC() )
	{
		iRecoverHP = CalcNPCHPRecover( m_pBaseInfo->m_dwMaxHP );
		iRecoverMP = CalcNPCMPRecover( m_pBaseInfo->m_dwMaxMP );
	}

    if ( IsSDRegenNPC() )
    {
        iRecoverSD = CalcNPCSDRecover( m_pBaseInfo->m_dwMaxSD );
    }

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // 여기서 시간을 세팅해주면 스킬사용에 선딜이 발생한다.
    // 어차피 스킬사용 후마다 SetTime해주고, '확률'이 존재하므로 초기화땐 0으로해도 무방
    m_pSkillSelectTimer->SetTimer( 0 );
#else
    m_pSkillSelectTimer->SetTimer( m_pBaseInfo->m_wSkillUpdateTime );
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

	m_Attr.SetCharInfo( m_pBaseInfo, iRecoverHP, iRecoverMP ,iRecoverSD );
	m_Attr.Update();

	// FSM 초기화
	SetInitialState( dwStateID, byMoveType, dwMoveAreaID, param1 );

	// 스크립트에서 얻어온 스피드는 0.001을 곱해준다.
	m_pMoveStateControl->Init( this, CMS_WALK );
	SetHP( m_pBaseInfo->m_dwMaxHP );	
	SetMP( m_pBaseInfo->m_dwMaxMP );	
    SetSD( m_pBaseInfo->m_dwMaxSD );
	m_pTargetManager->Init();

    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    search_type_ = FindSearchTypeByAttitude(m_pBaseInfo->m_byAttitude);
	// 사용 가능한 스킬들을 등록한다.
	AddDefaultSkill();
}

// implemented by _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
// 공격 성향과 검색 타입의 변환
eTARGET_SEARCH_TYPE NPC::FindSearchTypeByAttitude(const BYTE attitude)
{
    switch (attitude)
    {
    case ATTACK_ATTITUDE_AFTER:
        return eRARGET_NOT_SEARCH;

    case ATTACK_ATTITUDE_NEARES_FIRST:
        return eRARGET_SEARCH_NEAREST;

    case ATTACK_ATTITUDE_ONE_TARGET:
        return eRARGET_SEARCH_NONTARGET_LOW_HP;

    case ATTACK_ATTITUDE_LOW_HP_FIRST:
        return eRARGET_SEARCH_LOW_HP;

    case ATTACK_ATTITUDE_LOW_LEVEL_FIRST:
        return eRARGET_SEARCH_LOW_LEVEL;

    case ATTACK_ATTITUDE_HIGH_MP_FIRST:
        return eRARGET_SEARCH_HIGH_MP;

        //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
    case ATTACK_ATTITUDE_NO_ATTACK:
        return eRARGET_SEARCH_NOT_SEARCH_AND_TARGET_CHNAGE;

    default:
        SUNLOG(eCRITICAL_LOG, "[NPC::FindSearchTypeByAttitude]존재하지 않는 공격 성향 "
                              "MonsterCode[%d] attitude[%d]", 
                              m_pBaseInfo->m_MonsterCode, attitude);
        return eRARGET_NOT_SEARCH;
    }
}

void NPC::SetSearchType(eTARGET_SEARCH_TYPE search_type, DWORD object_key) 
{
    search_type_ = search_type;
    search_taget_unitID_ = 0;
    search_taget_mapobjectID_ = 0;
    search_taget_monstercode_ = 0;

    switch (search_type)
    {
    case eRARGET_SEARCH_MAPOBJECTID:
        search_taget_mapobjectID_ = object_key;
        break;

    case eRARGET_SEARCH_UNITID:
        search_taget_unitID_ = static_cast<REGENID>(object_key);
        break;

    case eRARGET_SEARCH_MONSTERCODE:
        search_taget_monstercode_ = static_cast<MONSTERCODE>(object_key);
        break;
    
    default:
        break;
    }
}

//==================================================================================================

BOOL NPC::IsHPMPRegenNPC()
{
	if ( !m_pBaseInfo )	return FALSE;

	switch ( m_pBaseInfo->m_byGrade )
	{
	case eNPC_ELITE:	case eNPC_LEADER:	case eNPC_MIDDLEBOSS:
	case eNPC_BOSS:		case eNPC_SUMMON_NPC:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL NPC::IsSDRegenNPC()
{
    if (!m_pBaseInfo)	
        return FALSE;

    // 서버쪽에서는 NPC 등급 보다는 객체 타입으로 평가 [2/23/2010 lst1024]
    if (IsEqualObjectKind(SSQMONSTER_OBJECT) == 0)
        return TRUE;

    return FALSE;
}

//==================================================================================================

VOID NPC::GetNPCRenderInfo( MONSTER_RENDER_INFO * OUT pRenderInfo )
{
	pRenderInfo->m_dwObjectKey			= GetObjectKey();
	pRenderInfo->m_dwMonsterCode		= GetBaseInfo()->m_MonsterCode;

    GetPos( &pRenderInfo->m_wvPos );
    pRenderInfo->m_dwMaxHP				= GetMaxHP();
    pRenderInfo->m_dwHP					= GetHP();
	pRenderInfo->m_wMoveSpeedRatio		= (WORD)m_Attr.GetMoveSpeedRatio();
	pRenderInfo->m_wAttSpeedRatio		= m_Attr.GetAttSpeedRatio();

	pRenderInfo->m_wAngle = 0;
	if ( m_iAngle >= 0 )	pRenderInfo->m_wAngle = (WORD)m_iAngle;

	// STATE_INFO를 채운다.
	GetStatusManager()->SerialzieStatusInfo( pRenderInfo->m_StateTotalInfo );
}

VOID NPC::GetSummonedRenderInfo( SUMMONED_RENDER_INFO * OUT pRenderInfo )
{
	ASSERT( !"[NPC::GetSummonedRenderInfo] 일루 호출되면 안된다!" );
	SUNLOG( eCRITICAL_LOG, "[NPC::GetSummonedRenderInfo] ObjectType[%d] 일루 호출되면 안된다!", GetObjectType() );
}

VOID NPC::GetTotemNPCRenderInfo( TOTEMNPC_RENDER_INFO * OUT pRenderInfo )
{
	ASSERT( !"[NPC::GetTotemNPCRenderInfo] 일루 호출되면 안된다!" );
	SUNLOG( eCRITICAL_LOG, "[NPC::GetTotemNPCRenderInfo] ObjectType[%d] 일루 호출되면 안된다!", GetObjectType() );
}

//==================================================================================================
// profiling
struct sProfileBlock
{
	LARGE_INTEGER	start_counter, end_counter;
	INT64			elapsedClock;
	INT64			elapsedMilliSecond;
};

#ifdef _SUSUNLOVE_TEST_CODE_
	#define PROFILING_NPC_UPDATE_START()						\
		QueryPerformanceCounter(&profile_storage.start_counter)

	#define PROFILING_NPC_UPDATE_CHECK()						\
		QueryPerformanceCounter(&profile_storage.end_counter)

	#define IF_PROFILING_NPC_UPDATE_OVERTHAN( millisecond )		\
		profile_storage.elapsedClock = end_counter.QuadPart - start_counter.QuadPart;		\
		profile_storage.elapsedMilliSecond = profile_storage.elapsedClock * 1000 / g_pGameServer->m_freq.QuadPart;	\
		if ( profile_storage.elapsedMilliSecond >= (millisecond) )

#else
	#define PROFILING_NPC_UPDATE_START()
	#define PROFILING_NPC_UPDATE_CHECK()
	#define IF_PROFILING_NPC_UPDATE_OVERTHAN( millisecond )

#endif

//==================================================================================================

BOOL
NPC::Update( DWORD dwDeltaTick )
{
	//	need Time Profiling
	static sProfileBlock profile_storage;

	PROFILING_NPC_UPDATE_START();

#if WAVERIX_CODE_DELETE_RESERVED
    static MONSTERCODE npcCode_s;                   // (WAVERIX) debug code
    npcCode_s = this->GetBaseInfo()->m_MonsterCode; // (WAVERIX) debug code
#endif
    if (Character::Update(dwDeltaTick) == FALSE)
        return FALSE;

	GameField* const pField = GetField();
	if ( !pField )
		return TRUE;
	GameZone* const pZone	= pField->GetGameZone();
	const sPOLICY_ZONE& ZonePolicy	= pZone->Policyment;
	const SECTOR_IDX sector_index	= GetSectorIndex();

	BOOLEAN need_proc_state	= FALSE;
	//////////////////////////////////////////////////////////////////////////
	//	<AI CONTROL>
	FLOWCONTROL_START()
    {
		AIMsgProgress();		//	큐에 쌓인 AI 메세지 처리

		Sector* const pSector = pField->GetSector( sector_index );
		if ( !pSector )
			break;

		need_proc_state = (ZonePolicy.FIELD & ZonePolicy.ENABLE_NPC_UPDATE_REF_SKIP) ||
			              pSector->GetReferenceCount();

        if (need_proc_state == false)
        {
            if (Policyment.FIELD & Policyment.PCC_CONTINUING_NPC_UPDATE) {
                need_proc_state = true;
            }
        }

	#ifndef _SUSUNLOVE_TEST_CODE_
		if (	need_proc_state )
	#endif
		{
			const DWORD start_tick		= GetTickCount();

	#ifdef _SUSUNLOVE_TEST_CODE_
			PROFILING_NPC_UPDATE_START();
			BOOLEAN status_result = FALSE;
			PROFILE_TIME_TEST(
				status_result = PlanEvent->Update( dwDeltaTick, need_proc_state );
				)
			PROFILING_NPC_UPDATE_CHECK();
			IF_PROFILING_NPC_UPDATE_OVERTHAN( 30 )
			{
				SUNLOG( eCRITICAL_LOG, "[PathExp_Progress] ObjKey[%d] Tick[%I64d]", GetObjectKey(), profile_storage.elapsedMilliSecond );
			}

	#else
			const BOOLEAN status_result	= PlanEvent->Update( dwDeltaTick, need_proc_state );
	#endif

			if ( !status_result )
				return FALSE;
		}

    }
    FLOWCONTROL_END;

	//////////////////////////////////////////////////////////////////////////
	//	<MOVING CONTROL>
	if ( !need_proc_state )
		PlanEvent->Update( dwDeltaTick, FALSE );

    // 이동 중 발생한 트리거 액션에 의해 필드 퇴장이 일어 났는지 검사한다.
    if (GetField() == NULL)
    {
        return FALSE;
    }

	// sector_index																	<- 이동 전 섹터 인덱스
	const WzVector* const pwvCurPos		= GetPosPtr();
	const SECTOR_IDX new_sector_index	= pField->GetSectorIndex( pwvCurPos );	//	<- 이동 후 섹터 인덱스

	pField->CheckAndApplyChangeSector( this, new_sector_index );

	return TRUE;
}

//==================================================================================================

Character* NPC::SelectSkillTarget( SLOTCODE SkillCode, BOOL bHeal )
{
	SkillScriptInfo *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );
	if ( !pSkillInfo )	return NULL;

	GameField *pField = GetField();
	if ( !pField ) return NULL;

	// 나중에 수정좀 봐야함...
	switch ( pSkillInfo->m_byTarget )
	{
	case SKILL_TARGET_ENEMY:
    case SKILL_TARGET_REACHABLE_ENEMY:
		// 대상이 적일 경우 현재 타겟을 그대로 유지한다.
		return GetTargetChar();

	case SKILL_TARGET_FRIEND:	
    case SKILL_TARGET_REACHABLE_FRIEND:
        {
            TargetFinderArg find_arg;

            find_arg.owner_ = this;
            find_arg.search_type_ = eRARGET_SEARCH_LOW_HPRATIO;
            find_arg.target_type_ = SKILL_TARGET_FRIEND;
            find_arg.unitID_ = 0;
            find_arg.mapobjectID_ = 0;
            find_arg.npc_code_ = 0;

            return pField->SearchTarget(find_arg);
        }

    case SKILL_TARGET_ME:	
    case SKILL_TARGET_REACHABLE_ME:
		return this;
	
    case SKILL_TARGET_FRIEND_CORPSE:
        {
            TargetFinderArg find_arg;

            find_arg.owner_ = this;
            find_arg.search_type_ = eRARGET_SEARCH_CORPSE;
            find_arg.target_type_ = SKILL_TARGET_FRIEND;
            find_arg.unitID_ = 0;
            find_arg.mapobjectID_ = 0;
            find_arg.npc_code_ = 0;

            return pField->SearchTarget(find_arg);
        }

    case SKILL_TARGET_SUMMON:
		// 소환을 할 경우 따로 타겟이 필요가 없으며 현재 타겟을 그대로 유지하면 된다.
		return GetTargetChar();
	
    case SKILL_TARGET_AREA:
		return GetTargetChar();
	
    default :
		SUNLOG( eFULL_LOG, "[NPC::SelectSkillTarget] Invalid TargetType[%d] NPCCode[%d] ", pSkillInfo->m_byTarget, GetBaseInfo()->m_MonsterCode );
	}
	return GetTargetChar();
}

VOID NPC::ChangeState( DWORD dwStateID, LPARAM param1, LPARAM param2, LPARAM param3 )
{
	m_pNPCStatusManager->ChangeState( dwStateID, param1, param2, param3 );
}

// note2 : 모호함을 해결하기 위해 함수를 제거함 - arycoat. 2013.09.13
//float NPC::GetDistToTarget() const
//{
//    // (NOTE) 생각하면 생각할 수록 무대뽀 처리를 수행하는 걸로 판단되는 함수다.
//    // 도대체 무슨 근거를 두고 처리를 하는가? 물론 본인의 경우, 이렇게 한 경우가 없지 않아 있는데,
//    // 그 사례에 따르면, 이것은 그때 그때 즉흥적으로 할 경우에나 나올 법한 방식이다.
//    // 시나리오: 이러한 계산이 필요하다. 없어? 추가... 없어? 추가...
//
//    // 좀 더 따져 봐야겠지만, 최대한 이런 것들을 사용하는 사례를 변경시키도록 해본다.
//    // 이것 외에 거리 계산 관련 in Character, in NPC, etc...
//	Character *pTarget = m_pTargetManager->GetCurTarget();
//
//	return GetDist( pTarget );
//}

float NPC::GetDistToNewTarget( Character* pNewTargetChar ) const
{
	return GetDist( pNewTargetChar );
}

//==================================================================================================
// for AI

VOID NPC::SetInitialState( DWORD dwStateID, BYTE byMoveType, DWORD dwMoveAreaID, LPARAM param1 )
{
	if ( byMoveType )
	{
		if ( !dwMoveAreaID )
		{
			dwMoveAreaID = m_pBaseInfo->m_dwMoveAreaID;
		}
	}
	else
	{
		byMoveType = m_pBaseInfo->m_byMoveAttitude;
		dwMoveAreaID = m_pBaseInfo->m_dwMoveAreaID;
	}

#ifdef _SUSUNLOVE_TEST_CODE_
	byMoveType = eMOVE_ATTITUDE_WANDER;
#endif
    // 변신시 기존 상태를 제거한다.
    m_pNPCStatusManager->ResetStateObject();
	// 완전 정지형
	if ( byMoveType == eMOVE_ATTITUDE_ETERNAL_STOP )
	{
		m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_STOP_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_STOP_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_STOP_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_STOP_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
		m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
		m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
	}
    // 완전 정지형(공격안함, dead시 사라지지 않음.)
    else if ( byMoveType == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK )
    {
        //상태 전이가 없는 상태
        m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_NO_TRANS );
    }
	// 스폰 정지형
	else if ( byMoveType == eMOVE_ATTITUDE_SPAWN_STOP )
	{
		m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_SPAWN_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_RETURN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
		m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
		m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_RETURN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
		m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
		m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
        m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK_AREA, STATE_ID_TRACK_AREA );
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
	}
	// 패트롤
	else if ( byMoveType == eMOVE_ATTITUDE_PATROL )
	{
		m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_PATROL );
		m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_PATROL );
		m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
		m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
		m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_RETURN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
		m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
		m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
        //_NA_0_20100817_HELLON_LAST_TRIGGER
        m_pNPCStatusManager->AddStateObject( STATE_ID_STOP_IDLE, STATE_ID_STOP_IDLE );
	}
    // added by _NA_0_20100222_UNIT_TRIGGERS_PATHLIST = {
    else if (byMoveType == eMOVE_ATTITUDE_PATHLIST)
    {
        m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_PATHLIST );
        m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_PATHLIST );
        m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
        m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
        m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
        m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_RETURN );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
        m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
        m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
        //_NA_0_20100817_HELLON_LAST_TRIGGER
        m_pNPCStatusManager->AddStateObject( STATE_ID_STOP_IDLE, STATE_ID_STOP_IDLE );
    }
    else if (byMoveType == eMOVE_ATTITUDE_FOLLOW)
    {
        m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_FOLLOW );
        m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_FOLLOW );
        m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
        m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
        m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
        m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_FOLLOW );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
        m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
        m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
        //_NA_0_20100817_HELLON_LAST_TRIGGER
        m_pNPCStatusManager->AddStateObject( STATE_ID_STOP_IDLE, STATE_ID_STOP_IDLE );
    }
    //} end added ai list '_NA_0_20100222_UNIT_TRIGGERS_PATHLIST'
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    else if (byMoveType == eMOVE_ATTITUDE_ATTACK_GROUND)
    {
        // 어택 그라운드 형

        m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_SPAWN_IDLE );
        m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_TRACK_AREA );
        m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
        m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
        m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
        m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
        m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
        m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_RETURN );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_TRACK_AREA );
        m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
        m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
        m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK_AREA, STATE_ID_TRACK_AREA );
    }
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
	else
	{
        // 자유 이동형 외 기타

		m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_IDLE );
		m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_WANDER );
		m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
		m_pNPCStatusManager->AddStateObject( STATE_ID_HELP, STATE_ID_HELP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
		m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
		m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
		m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_RETURN );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
		m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
		m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );
		m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );
        m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
        m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_SPECIAL_DEAD_ACTION );
        //_NA_0_20100817_HELLON_LAST_TRIGGER
        m_pNPCStatusManager->AddStateObject( STATE_ID_STOP_IDLE, STATE_ID_STOP_IDLE );
		// 특정영역 우선검색
		if ( byMoveType == eMOVE_ATTITUDE_SEARCH_AREA )
        {
			m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK_AREA, STATE_ID_TRACK_AREA );

			// 특정영역으로 달려가야 하므로 초기상태를 달려가는 상태로 설정한다.
			dwStateID = STATE_ID_TRACK_AREA;
		}
	}

	// 초기상태 설정
	m_pNPCStatusManager->Init( dwStateID, byMoveType, dwMoveAreaID, param1 );
}

VOID NPC::SendAIMessage( AI_MSG *pMsg, WORD wSize, DWORD dwDelay )
{
	m_pAIMessageQueue->SendAIMessage( pMsg, wSize, dwDelay );
}

VOID NPC::SendAIMessageToGroupExceptMe( AI_MSG *pMsg, WORD wSize, DWORD dwDelay )
{
	if ( m_pUnitListOfGroup )
		m_pUnitListOfGroup->SendAIMessage( pMsg, wSize, dwDelay, GetObjectKey() );
}

VOID NPC::SendAIMessageAroundExceptMe( AI_MSG *pMsg, WORD wSize, DWORD dwDelay )
{
	GameField *pField = GetField();
	if ( !pField )	return;

	pField->SendAIMessageAroundExceptMe( this, pMsg, wSize, dwDelay );
}


// 주변에서 적을 찾아서 그 포인터를 리턴한다.
Character* NPC::SearchTarget()
{
	GameField *pField = GetField();
	if ( !pField ) return NULL;

    TargetFinderArg find_arg;

    find_arg.owner_ = this;
    find_arg.search_type_ = search_type_;
    find_arg.target_type_ = SKILL_TARGET_ENEMY;
    find_arg.unitID_ = search_taget_unitID_;
    find_arg.mapobjectID_ = search_taget_mapobjectID_;
    find_arg.npc_code_ = search_taget_monstercode_;

    return pField->SearchTarget(find_arg);
}

Character* NPC::SearchTargetFriend()
{
    GameField* field = GetField();
    if (field == NULL) 
        return NULL;

    TargetFinderArg find_arg;

    find_arg.owner_ = this;
    find_arg.search_type_ = search_type_;
    find_arg.target_type_ = SKILL_TARGET_FRIEND;
    find_arg.unitID_ = search_taget_unitID_;
    find_arg.mapobjectID_ = search_taget_mapobjectID_;
    find_arg.npc_code_ = search_taget_monstercode_;

    return field->SearchTarget(find_arg);
}

VOID NPC::AIMsgProgress()
{
//	LARGE_INTEGER firstcounter,nextcounter;
//	__int64  elapsedclock, elapsedmillsecond;

	AI_MSG *pRecvMsg;
	WORD wSize;

	DWORD dwCurTick = GetTickCount();

	pRecvMsg = (AI_MSG*)m_pAIMessageQueue->RecvAIMessage( wSize ); 
	while ( pRecvMsg )
	{
		//QueryPerformanceCounter(&firstcounter);

		if ( dwCurTick > pRecvMsg->dwDeliveryTick )
		{
			m_pNPCStatusManager->GetCurState()->OnAIMsg( pRecvMsg );
		}
		else
		{
			// 메세지 딜레이만큼 지나지 않았으면 임시 큐에 넣고 처리하지 않는다.
			// 딜레이값 다시 넣어줘야함...좃치안타 -_-;
			m_pTempMessageQueue->SendAIMessage( pRecvMsg, wSize, pRecvMsg->dwDeliveryTick - dwCurTick );			
		}

		m_pAIMessageQueue->Completion( wSize );

		//QueryPerformanceCounter(&nextcounter);

		//elapsedclock = nextcounter.QuadPart - firstcounter.QuadPart;
		//elapsedmillsecond = elapsedclock * 1000 / g_pGameServer->m_freq.QuadPart;
		//if ( elapsedmillsecond >= 10 )
		//{
		//	SUNLOG( eCRITICAL_LOG, "[NPC::AIMsgProgress] 업데이트가 너무 오래걸림! ObjKey[%d] CurState[%d] MsgID[%d] Tick[%d]", 
		//		GetObjectKey(), m_pNPCStatusManager->GetCurState()->GetStateID(), pRecvMsg->dwMsgID, elapsedmillsecond );
		//}

		pRecvMsg = (AI_MSG*)m_pAIMessageQueue->RecvAIMessage( wSize );
	}

	// 임시 큐에 있던 메세지 다시 집어 넣기
	pRecvMsg = (AI_MSG*)m_pTempMessageQueue->RecvAIMessage( wSize );
	while ( pRecvMsg )
	{
		// 딜레이값 다시 넣어줘야함...좃치안타 -_-;
		m_pAIMessageQueue->SendAIMessage( pRecvMsg, wSize, pRecvMsg->dwDeliveryTick - dwCurTick );
		m_pTempMessageQueue->Completion( wSize );
		pRecvMsg = (AI_MSG*)m_pTempMessageQueue->RecvAIMessage( wSize );
	}
}

VOID NPC::SetTargetChar( Character *pTarget )
{
	m_pTargetManager->SetCurTarget( pTarget );
}

Character* NPC::GetTargetChar()
{
    Character* target = m_pTargetManager->GetCurTarget();
    if (obj_util::IsValid(target, __FUNCTION__, __LINE__) == false)
    {
        SetTargetChar(NULL);
        return NULL;
    }

	return target;
}

VOID NPC::GroupCmdAttackToGroup( Character* pTargetChar )
{
	// 그룹의 리더라면
	if ( IsLeader() )
	//if ( GetGroupState().IsLeader() )
	{
		AI_MSG_GROUP_C0MMAND AIGroupCmdMsg;
		AIGroupCmdMsg.byType = GROUP_CMD_TYPE_ATTACK;
		AIGroupCmdMsg.dwTargetKey = pTargetChar->GetObjectKey();

		SendAIMessageToGroupExceptMe( &AIGroupCmdMsg, sizeof(AI_MSG_GROUP_C0MMAND) );
	}
}

VOID NPC::GroupCmdStopAttackToGroup()
{
	// 그룹의 리더라면
	if ( IsLeader() )
	//if ( GetGroupState().IsLeader() )
	{
		AI_MSG_GROUP_C0MMAND AIGroupCmdMsg;
		AIGroupCmdMsg.byType = GROUP_CMD_TYPE_STOP_ATTACK;

		SendAIMessageToGroupExceptMe( &AIGroupCmdMsg, sizeof(AI_MSG_GROUP_C0MMAND) );
	}
}

//==================================================================================================
// moving supporter

BOOL NPC::IsSamePositionWithTarget()
{
	WzVector vTargetPos, vMyPos;
    if (GetTargetChar() == NULL)
        return FALSE;
	GetTargetChar()->GetPos( &vTargetPos );
	GetPos( &vMyPos );

	if ( vMyPos == vTargetPos )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID NPC::GetTrackPos( WzVector &wvDestPos, float fAttackRangeRatio )
{
	// 이전에 추적하던 타겟슬롯을 Release한다.
	GetTargetChar()->FreeEnemySlot( m_pTargetManager->GetTrackSlot() );

	// 새로운 타겟슬롯을 찾아서 정보를 갱신한다.
	WzVector vMyPos;	GetPos( &vMyPos );
	BYTE byTrackSlot = GetTargetChar()->AllocEnemySlot( &vMyPos, wvDestPos, fAttackRangeRatio );
	m_pTargetManager->SetTrackSlot( byTrackSlot );
}

BOOL NPC::OnResurrection( float fRecoverExp, float fRecoverHP, float fRecoverMP, Player *pHealer )
{
	if ( Character::OnResurrection( fRecoverExp, fRecoverHP, fRecoverMP, pHealer ) )
	{
		ChangeState( STATE_ID_IDLE );

		return TRUE;
	}

	return FALSE;
}


VOID NPC::OnDead()
{
	SASSERT( GetHP() == 0, "GetHP() != 0" );

	//	(WAVERIX)(080923)(NOTE) 처리 변경할 시간이 없다. 기존 로직 형태로 묻어간다.

    //!~ too many state change try! - arycoat 2014.08.14
    State* pState = GetStateManager()->GetCurState();
    if (!pState || pState->GetStateID() == STATE_ID_DEAD) return;
    //~!

	ChangeState(STATE_ID_DEAD, m_pBaseInfo->m_dwDeadWaitingTime);

	SetTargetChar( NULL );
	m_pAIMessageQueue->Clear();

	Character::OnDead();
}

BOOL
NPC::ExecuteThrust(BOOL bForcedMove,
                   WzVector* pwvMoveDistance, WzVector& wvPosAfterThrust, float fMoveDistance,
                   BOOL is_down_after_thrust) // 밀리기 후에 다운 상태로 넘어갈 것인가?
{
    // (CHANGES) (091229) (WAVERIX) code-arrangement for NPC moving control
    GameField* pField = GetField();
    if (pField == NULL)
        return false;

    // 완전 정지형인 경우 밀리기가 안된다.
    if (m_pBaseInfo->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP ||
        m_pBaseInfo->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
    {
        return false;
    }

    // TODO: 밀리는 방향이 없는 경우 문제가 생긴다. 랜덤으로??
    if (pwvMoveDistance->x == 0.0f && pwvMoveDistance->y == 0.0f) {
        pwvMoveDistance->x = 1.0f;
        WzVector wvTemp = *pwvMoveDistance;
        int angle = State::GetStateRandomNo() % 360;
        Math_ZRotateVector(&wvTemp, *pwvMoveDistance, angle);
        *pwvMoveDistance = *pwvMoveDistance * fMoveDistance;
    }

    // NPC 밀리기 처리
    if (pField->FindPathThrust(this, pwvMoveDistance, bForcedMove) == false)
        return false;

    // (NOTE) FindPathThrust에 따른 무의미한 검증이 되어야 할 것이다.
    if (IsMoving() == false)
        return false;

    // 밀리기 후의 이동 좌표 얻기
    wvPosAfterThrust = GetPathExplorer()->GetTargetPos();

    // NPC에게 밀리기 메세지를 보냄
    AI_MSG_THRUST thrust_msg;
    thrust_msg.bDownAfterThrust = !!is_down_after_thrust;
    SendAIMessage(&thrust_msg, sizeof(thrust_msg));

    return true;
}

BOOL NPC::IsTooCloseToTarget()
{
    if (GetTargetChar() == NULL)
        return FALSE;

	// 타겟이 본인일 경우(셀프힐)
	if ( GetTargetChar() == this )		return FALSE;

	// 너무 가까운 경우.. 오히려 멀어지자
	// (!) Player의 공격거리랑 차이가 많이 나는 경우는 플레이어가 다가가면 무조건 멀어지는 경향을 보일 수 있다.
	//		--> 적당히 기본 공격거리를 통일하든지 허용 범위를 조절하든지 해야할듯

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

	WzVector vCurPos, vTargetPos;
	GetPos( &vCurPos );	GetTargetChar()->GetPos( &vTargetPos );
	float fDistToTarget2 = VectorLength2( &(vCurPos-vTargetPos) );

	// 타겟과의 거리가 MAX_ATTACK_RANGE_LIMIT이상이면 그냥 공격->장거리인 애들이 너무 멀어지는걸 방지
	if ( fDistToTarget2 < stAICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT*stAICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL NPC::NeedToRetreatFromTarget()
{
    Character* target = GetTargetChar();
    if (target == NULL)
        return FALSE;

	if ( GetSelectedSkill() != SKILLCODE_NORMAL_ATTACK ) return FALSE;

	// 공격거리가 5m는 넘어야 후퇴 공격을 한다.
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	if ( GetAttackRange()>=stAICommonInfo.m_fRETREAT_MIN_ATTACKRANGE_LIMIT )
	{
		float fDistToTarget = GetDist(target);
		float fMinAttackDist = GetAttackRange() * stAICommonInfo.m_fRETREAT_MIN_DISTANCE_LIMIT_RATIO;

		if (fDistToTarget < fMinAttackDist)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 타겟보다 적당히 높은 위치에 있고 수평 거리도 충분히 가까워야 점프할 수 있으므로 체크한다.
BOOL NPC::IsOnGoodPositionToJump()
{
	if ( !GetTargetChar() )
		return FALSE;

	WzVector wvCurPos, wvTargetPos;
	GetPos( &wvCurPos );	GetTargetChar()->GetPos( &wvTargetPos );

	// 높이가 너무 높거나 너무 낮으면 실패
	float fHeight = wvCurPos.z - wvTargetPos.z;
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	if ( fHeight < stAICommonInfo.m_fJUMP_MIN_HEIGHT || fHeight > stAICommonInfo.m_fJUMP_MAX_HEIGHT )
	{
		return FALSE;
	}

	// 수평 거리가 너무 멀거나 너무 가까우면 실패
	float fDistance = sqrt( ( wvTargetPos.x - wvCurPos.x ) * ( wvTargetPos.x - wvCurPos.x ) +
							( wvTargetPos.y - wvCurPos.y ) * ( wvTargetPos.y - wvCurPos.y ) );

	if ( fDistance < stAICommonInfo.m_fJUMP_MIN_DISTANCE || fDistance > stAICommonInfo.m_fJUMP_MAX_DISTANCE )
	{
		return FALSE;
	}

	m_vJumpPos = wvTargetPos;

	return TRUE;
}


// 길찾기를 하고 패킷을 날림
BOOL
NPC::MoveAndBroadcast(WzVector* pwvDestPos, eCHAR_MOVE_STATE moveState)
{
    // (CHANGES) (091229) (WAVERIX) code-arrangement for NPC moving control
    GameField* pField = GetField();
    if (pField == NULL)
        return false;

#ifdef _SUSUNLOVE_TEST_CODE_
    LARGE_INTEGER firstcounter,nextcounter;
    QueryPerformanceCounter(&firstcounter);
#endif

    // 길찾기... 실패시 리턴
    if (pField->FindPath(this, pwvDestPos) == false) 
        return false;

#ifdef _SUSUNLOVE_TEST_CODE_
    QueryPerformanceCounter(&nextcounter);
    __int64 elapsedclock = nextcounter.QuadPart - firstcounter.QuadPart;
    __int64 elapsedmillsecond = elapsedclock * 1000000 / g_pGameServer->m_freq.QuadPart;
    if (elapsedmillsecond >= 20) {
        //SUNLOG(eCRITICAL_LOG, "[PathExp_FindPath] ObjKey[%d] Tick[%I64d]", GetObjectKey(), elapsedmillsecond);
    }
#endif

    SetMoveState(moveState); // changes NPC move state = { RUN/WALK/etc... }

    { //broadcast
        const WzVector* const pMobPos = GetPosPtr();
        MSGSUB_SYNC_MOVE_BRD msg;
        msg.m_dwObjectKey = GetObjectKey();
        msg.m_byState     = (BYTE)moveState;  // RUN or WALK
        msg.m_ForcedMove  = false;
        msg.m_wvCurPos    = *pMobPos;
        msg.m_wvDestPos   = *pwvDestPos;
        SendPacketAroundForSync(&msg);
    };

    return true;
}

BOOL NPC::CanJumpDist( Character *pTarget )
{
	if ( !pTarget )	return FALSE;

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	WzVector vCurPos, vTargetPos;
	GetPos( &vCurPos );	pTarget->GetPos( &vTargetPos );

	// 타겟과의 거리가 일정 범위에 들어야 점프를 한다.
	float fDistToTarget = VectorLength( &(vCurPos-vTargetPos) );
	if ( fDistToTarget < stAICommonInfo.m_fJUMP_ATTACK_MIN_DISTANCE || fDistToTarget > stAICommonInfo.m_fJUMP_ATTACK_MAX_DISTANCE )
		return FALSE;

	return TRUE;
}

BOOL NPC::SetJumpPos( Character *pTarget )
{
	if ( !pTarget )		return FALSE;

	GameField *pField = GetField();
	if ( !pField ) 		return FALSE;

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	WzVector vCurPos, vTargetPos, vNormal, vDistanceWantToMove, vDestPosAfterThrust;

	// 현재 위치
	GetPos( &vCurPos );	pTarget->GetPos( &vTargetPos );

	// 점프 목적지 계산
	VectorNormalize( &vNormal, &( vCurPos-vTargetPos ) );	vNormal.z = 0;
	float fAttackRange = max( stAICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT, GetAttackRange()*stAICommonInfo.m_fJUMP_ATTACK_RANGE_RATIO );
	m_vJumpPos = vTargetPos + vNormal * fAttackRange;
	vDistanceWantToMove = m_vJumpPos - vCurPos;
	float fWantToMoveLength = VectorLength2( &vDistanceWantToMove );

	// 길찾기 가능한지 체크
	if ( !pField->FindPathThrust( this, &vDistanceWantToMove ) )
	{
		return FALSE;
	}

	// FindPath후 설정된 거리가 의도한 거리에 못미치면 장애물에 닿은 것으로 간주한다.
	vDestPosAfterThrust = GetPathExplorer()->GetTargetPos();
	float fMoveLength = VectorLength2( &(vDestPosAfterThrust - vCurPos) );

	// Thrust 거리가 의도한 거리만큼 안되는 경우
	if ( fMoveLength < fWantToMoveLength * 0.95f )
	{
		StopMoving();
		return FALSE;
	}

	SetMoveState( CMS_MONSTER_ATTACKJUMP );

	return TRUE;
}

BOOL
NPC::ThrustMoveAndBroadcast(WzVector* pwvDestPos, eCHAR_MOVE_STATE moveState,
                            BOOL& bTouchedSomething,
                            WzVector& wvRotatedVector)
{
    GameField* pField = GetField();
    if (pField == NULL)
        return false;

    const WzVector& wvCurPos = *GetPosPtr();
    WzVector& wvDistanceWantToMove = *pwvDestPos - wvCurPos;
    float fWantToMoveLength = VectorLength2(&wvDistanceWantToMove);

    // 목적지까지 길찾기 경로를 셋팅한다.
#ifdef _SUSUNLOVE_TEST_CODE_
    LARGE_INTEGER firstcounter,nextcounter;
    QueryPerformanceCounter(&firstcounter);

    BOOL bResult = false;
    //  PROFILE_TIME_TEST(bResult = pField->FindPathThrust(this, &wvDistanceWantToMove);)
    PROFILE_TIME_TEST(bResult = pField->FindPath(this, pwvDestPos);)
    if (bResult == false)
        return false;

    QueryPerformanceCounter(&nextcounter);

    __int64 elapsedclock = nextcounter.QuadPart - firstcounter.QuadPart;
    __int64 elapsedmillsecond = elapsedclock * 1000000 / g_pGameServer->m_freq.QuadPart;
    if (elapsedmillsecond >= 20) {
        SUNLOG(eCRITICAL_LOG, "[PathExp_Thrust] ObjKey[%d] Tick[%I64d]",
               GetObjectKey(), elapsedmillsecond);
    }
    else {
        SUNLOG(eCRITICAL_LOG, "[PathExp_Thrust] No Delay!! ObjKey[%d] Tick[%I64d]",
               GetObjectKey(), elapsedmillsecond);
    }
#else

    if (pField->FindPathThrust(this, &wvDistanceWantToMove) == false) {
        return false;
    }

#endif

    // FindPath후 설정된 거리가 의도한 거리에 못미치면 장애물에 닿은 것으로 간주한다.
    const WzVector& wvDestPosAfterThrust = GetPathExplorer()->GetTargetPos();
    float fMoveLength = VectorLength2(&(wvDestPosAfterThrust - wvCurPos));

    // Thrust 거리가 의도한 거리만큼 안되는 경우
    if (fMoveLength < fWantToMoveLength * 0.95f)
    {
        // 장애물에 닿았다는 표시를 하고 지금 가려는 방향에서 49도 회전시킨 벡터를 구해둔다.
        bTouchedSomething = true;
        const float calc_cos = g_SinTable.FCOS(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE);
        const float calc_sin = g_SinTable.FSIN(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE);

        wvRotatedVector.x = wvDistanceWantToMove.x * calc_cos + wvDistanceWantToMove.y * calc_sin;
        wvRotatedVector.y = -wvDistanceWantToMove.x * calc_sin + wvDistanceWantToMove.y * calc_cos;         
        wvRotatedVector.z = 0.0f;

        // (NOTE) (WAVERIX) 좀 더 따져볼 필요 있다. 이동하지 않을 경우 생기는 문제는?
        // 만약 이동은 하고 성공처리시에는?

        // 이동 거리가 너무 짧으면 이동하지 말고(패킷 송신 없이) 그냥 리턴
        if (fMoveLength < AIParamParser::AICommonInfo.m_fMIN_MOVABLE_DISTANCE *
                          AIParamParser::AICommonInfo.m_fMIN_MOVABLE_DISTANCE)
        {
            StopMoving();
            return false;
        }
    }

    SetMoveState(moveState);

    {
        MSGSUB_SYNC_MOVE_THRUST_BRD msg;
        msg.m_dwObjectKey = GetObjectKey();
        msg.m_byState     = moveState;
        msg.m_wTileIndex  = GetPathExplorer()->GetTile();
        //  (WAVERIX) (081209) (BUG-FIX) 무조건 달리기 수정
        //msg.m_byState   = CMS_RUN;  //캐릭터의 경우는 무조건 뛰기만 존재!!(몬스터와 같이 쓰는 패킷이므로 값은 넣어줌)
        msg.m_wvCurPos    = wvCurPos;
        msg.m_wvDestPos   = wvDestPosAfterThrust;

        SendPacketAroundForSync(&msg);
        WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_npcmove", this, &msg)));
    };

    return true;
}


float NPC::GetAttackRange() const
{
	float fAttackRange = GetNormalRangeBonus();

	if ( GetSelectedSkill() == SKILLCODE_INVALID )
	{
        // 이 함수는 '적이 공격사거리내에 들어왔는가?'를 체크할 때에도 사용하고 이 때엔, 대개 해당 분기를 타게된다.
        // 추적상태에서 불렸을 시에 이부분으로
        // 이 분기문에 들어오는 것은 일어날 수 있는 일이므로 ASSERT 제거
		//ASSERT( GetSelectedSkill() != SKILLCODE_INVALID );
		fAttackRange += 2.5f;
	}
	else if ( GetSelectedSkill() == SKILLCODE_NORMAL_ATTACK )
	{
		// 일반 공격시 NPC의 기본 공격 사거리를 리턴
		fAttackRange += m_pBaseInfo->m_fAttRange;
	}
	else
	{
		// 스킬 공격시 해당 스킬의 공격 사거리를 리턴
		SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( GetSelectedSkill() );
		if ( NULL == pBaseSkillInfo )
		{
			SUNLOG( eCRITICAL_LOG,"[NPC::GetAttackRange] SkillInfo is NULL [%d]\n", GetSelectedSkill() );
			return 1.0f;
		}

		fAttackRange = pBaseSkillInfo->m_wSkillRange/10.f + GetSkillRangeBonus();
	}

	return fAttackRange + GetBodyRadius();
}

BOOL NPC::CheckSkillRange( Character *pTarget, const WzVector &vTargetPos, float fSkillRange )
{
	fSkillRange += GetBodyRadius();

	return Character::CheckSkillRange( pTarget, vTargetPos, fSkillRange );
}

//==================================================================================================

eUSER_RELATION_TYPE NPC::IsFriend( Character *pTarget )
{
    if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return USER_RELATION_NEUTRALIST;
    }

	// 본인인지 체크한다.
	if ( this == pTarget )	return USER_RELATION_FRIEND;

	// 본인과 타겟의 소환된 여부를 확인한다.
	Character* pMySummoner		= NULL; 
	Character* pTargetSummoner	= NULL;

#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    if (this->IsEqualObjectKind(SUMMON_OBJECT)) {
        pMySummoner = GetField()->FindCharacter(this->GetSummonerKey());
    }

    if (pTarget->IsEqualObjectKind(SUMMON_OBJECT)) {
        pTargetSummoner = pTarget->GetField()->FindCharacter(pTarget->GetSummonerKey());
    }
#else 
	SummonedNPCs* pMySummon		= SummonManager::Instance()->FindSummonNPCs( m_dwSummonerKey );
	SummonedNPCs* pTargetSummon = SummonManager::Instance()->FindSummonNPCs( pTarget->GetSummonerKey() );

	if ( pMySummon )			
	{
		pMySummoner = pMySummon->GetSummoner();
		if ( pMySummoner == this )
			pMySummoner =  NULL;
	}
	if ( pTargetSummon )		
	{
		pTargetSummoner = pTargetSummon->GetSummoner();
		if ( pTargetSummoner == pTarget )
			pTargetSummoner =  NULL;
	}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	if ( pMySummoner && pTargetSummoner )
	{
		// 본인소환자의 타입과 타겟소환자의 타입이 같으면 아군이 된다.
		return pMySummoner->IsFriend( pTargetSummoner );
	}
	else if ( pMySummoner )
	{
		// 본인소환자의 타입과 타겟의 타입이 같으면 아군이 된다.
		return pMySummoner->IsFriend( pTarget );
	}
	else if ( pTargetSummoner )
	{
		// 본인 타입과 타겟소환자의 타입이 같으면 아군이 된다.
		return IsFriend( pTargetSummoner );
	}

	if ( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
	{
		//	SSQMonster는 몬스터들 끼리도 적으로 인식한다.
		//	트랩 계열 NPC들을 공격하는 것을 방지하기 위해서 이들만 친구로 인식하도록 한다.
        if (pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
            return USER_RELATION_FRIEND;
		return USER_RELATION_ENEMY;
	}
	else
	{
        // 같은 몬스터끼리는 아군으로 판정한다.
        if (pTarget->IsEqualObjectKind(NPC_OBJECT))
        {
            NPC* const npc = static_cast<NPC*>(pTarget);
            const BYTE grade = npc->GetGrade();
            // CHANGES: f110525.9L, add iff relations about eNPC_MOVE_SYNC_MERCHANT
            if (grade != eNPC_FRIEND_MAPOBJECT_NPC &&
                grade != eNPC_FRIEND_MONSTER &&
                grade != eNPC_MOVE_SYNC_MERCHANT)
            {
                return USER_RELATION_FRIEND;
            }
            else if ((grade == eNPC_MOVE_SYNC_MERCHANT) &&
                     (npc->Policyment.FIELD & npc->Policyment.PCC_PROTECTED_NPC) == 0)
            {   // CHANGES: f110704.5L, add iff relations to support the custom map unit.
                // see the NpcExtraInfo.NpcType and ExtraNPCManager::CheckProtectedNpc
                // the concept of 'protected' is based on player view.
                return USER_RELATION_FRIEND;
            }
            else
            {
                return USER_RELATION_ENEMY;
            }
        }
        else
        {
            return USER_RELATION_ENEMY;
        }
    }

	return USER_RELATION_NEUTRALIST;
}

VOID NPC::OnAttack( Character *pTarget, SLOTCODE skillcode, DAMAGETYPE damage )
{
	Character::OnAttack( pTarget, skillcode, damage );

	// 최초로 공격을 가한 넘에게 초기 포인트를 주기위해 AddAttackPoint를 호출한다.
	m_pTargetManager->AddBattlePoint( pTarget, BATTLE_POINT_DAMAGE, 0 );
}



void
NPC::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;

    //	(WAVERIX)(081013)(NOTE) 너무 길고 중복이 많다. 디파인 제거 작업이 끝나면 정리하기로 한다.
    if ( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
    {
        int damage = damage_arg->damage_;
        if (damage) {
            damage = (damage >> 1);
            if (damage == 0) {
                damage = 1;
            }
            damage_arg->damage_ = damage;
        }
    }

    if (damage_arg->damage_ == 0) {
        return;
    }

#ifndef _NA_0_20100819_AGGRO_RENEWAL
    if ( pAttacker ) 
    {
        DAMAGETYPE wResultDamage = damage_arg->damage_;
        
        DWORD hp = GetHP();
        if ( hp <= damage_arg->damage_ )		
            wResultDamage = (DAMAGETYPE)hp;

        // 계산된 데미지 추가? 실제 적용 데미지를 추가해야 하는것이 아닌가? [2/17/2010 lst1024]
        m_pTargetManager->AddBattlePoint( pAttacker, BATTLE_POINT_DAMAGE, wResultDamage ); 
    }
#endif

    Character::_Damaged(damage_arg);

    if ( pAttacker && IsDead() )
    {
        Player* pAttackPlayer = pAttacker->GetPlayerOwner();
        if ( pAttackPlayer )
        {
            ChaoState& rChaoState = pAttackPlayer->GetChaoState();
            rChaoState.KillMonster( this );                         // 몬스터가 카오 플레이어로부터 공격을 받아서 죽었을 경우

            if (damage_arg->option_.is_mirror_ == false)
            {
                Attributes& rAtt = pAttackPlayer->GetAttr();
                const int AbsorbHP = rAtt.GetAbsorbHP();
                const int AbsorbMP = rAtt.GetAbsorbMP();

                if ( AbsorbHP )
                    pAttackPlayer->OnRecover( AbsorbHP, 0 );        // HP 흡수
                if ( AbsorbMP )
                    pAttackPlayer->OnRecover( 0, AbsorbMP );        // MP 흡수
            }
        }
    }
}

VOID NPC::AddBattlePoint( Character* pAttacker, eBATTLE_POINT_TYPE ePointType, int iBattlePoint )
{
	m_pTargetManager->AddBattlePoint( pAttacker, ePointType, iBattlePoint );
}

BOOL NPC::IsSpecialAction( eNPC_SPECIAL_ACTION_TYPE eType )
{
	// 조건 검사
	for ( int i = 0; i < 2; ++i )
	{
		SPECIAL_CONDITION *pSpecialCondition = &GetBaseInfo()->m_SpecialCondition[i];
		if ( pSpecialCondition->wActionType == eType )
			return TRUE;
	}

	return FALSE;
}

SPECIAL_CONDITION* NPC::GetDrawLotsSpecialAction()
{
    // TODO: need compare with the custom logic 'SpecialAction()'
    for ( int i = 0; i < BASE_NPCINFO::MAX_SPECIAL_NUM; ++i )
    {
        SPECIAL_CONDITION& special_condition = GetBaseInfo()->m_SpecialCondition[i];

        if (CheckCondition(special_condition.byCondition, special_condition.wConditionParam))
        {
            if (DrawLots(special_condition.byActionRate))
            {
                return &special_condition;
            }
        }
    }

    return NULL;
}

VOID NPC::SpecialAction(const SPECIAL_CONDITION& special_condition)
{
    switch (special_condition.wActionType)
    {
    case eNPC_SPECIAL_ACTION_HELPREQUEST:
        {
            // 공격대상이 없으면 지원요청 무효
            if ( !GetTargetChar() )	return;

            if ( !GetStateManager()->IsRequestHelp() )
            {
                AI_MSG_HELP_REQUEST AIHelpMsg;
                AIHelpMsg.dwAttackerKey = GetTargetChar()->GetObjectKey();
                AIHelpMsg.dwTargetKey = GetObjectKey();

                SendAIMessageAroundExceptMe( &AIHelpMsg, sizeof(AIHelpMsg) );

                GetStateManager()->SetRequestHelp( TRUE );
            }
        }
        break;
    case eNPC_SPECIAL_ACTION_TRANSFORMATION:
    case eNPC_SPECIAL_ACTION_SELP_DESTRUCTION:
        {
            UseSkill( special_condition.wActionParam );
        }
        break;
    case eNPC_SPECIAL_ACTION_SKILL:
        {
            // 공격대상이 없으면 스킬구사 무효
            if ( !GetTargetChar() )	return;

            SelectSkill( GetTargetChar(), special_condition.wActionParam );
            ChangeState( STATE_ID_TRACK );
        }
        break;
    }    
}

//_NA_0_20100514_NPC_DEAD_STATE_TIME(삭제)
VOID NPC::SpecialAction()
{
	SPECIAL_CONDITION *pSpecialCondition = NULL;

	// 조건 검사
	for ( int i = 0; i < 2; ++i )
	{
		pSpecialCondition = &GetBaseInfo()->m_SpecialCondition[i];

		if ( CheckCondition( pSpecialCondition->byCondition, pSpecialCondition->wConditionParam ) )
		{
			// 확률 체크
			if ( DrawLots( pSpecialCondition->byActionRate ) )
				break;
		}
		pSpecialCondition = NULL;
	}

	if ( !pSpecialCondition )	return;

	switch ( pSpecialCondition->wActionType )
	{
	case eNPC_SPECIAL_ACTION_HELPREQUEST:
		{
			// 공격대상이 없으면 지원요청 무효
			if ( !GetTargetChar() )	return;

			if ( !GetStateManager()->IsRequestHelp() )
			{
				AI_MSG_HELP_REQUEST AIHelpMsg;
				AIHelpMsg.dwAttackerKey = GetTargetChar()->GetObjectKey();
				AIHelpMsg.dwTargetKey = GetObjectKey();

				SendAIMessageAroundExceptMe( &AIHelpMsg, sizeof(AIHelpMsg) );

				GetStateManager()->SetRequestHelp( TRUE );
			}
		}
		break;
	case eNPC_SPECIAL_ACTION_TRANSFORMATION:
	case eNPC_SPECIAL_ACTION_SELP_DESTRUCTION:
		{
			UseSkill( pSpecialCondition->wActionParam );
		}
		break;
	case eNPC_SPECIAL_ACTION_SKILL:
		{
			// 공격대상이 없으면 스킬구사 무효
			if ( !GetTargetChar() )	return;

			SelectSkill( GetTargetChar(), pSpecialCondition->wActionParam );
			ChangeState( STATE_ID_TRACK );
		}
		break;
	}
}

VOID NPC::StatusResist()
{
    // (NOTE) if a monster has a resistance about specific states,
    // the monster enable released status by a release ratio of a probability.
    const BASE_NPCINFO* const npc_info = GetBaseInfo();
    // (CHANGES) (f100628.3L) (WAVERIX) change to a more cheap routine
    for (int i = 0; i < _countof(npc_info->m_ResistanceCondition); ++i)
    {
        const RESISTANCE_CONDITION& resist_condition = npc_info->m_ResistanceCondition[i];
        const eCHAR_STATE_TYPE state_id = \
            static_cast<eCHAR_STATE_TYPE>(resist_condition.m_wStateCode);
        if (state_id == eCHAR_STATE_INVALID) {
            continue;
        };
        if (m_pStatusManager->FindStatus(state_id) == false) {
            continue;
        };
        if (DrawLots(resist_condition.m_byRatio) == false) {
            continue;
        };
        if (m_pStatusManager->Remove(state_id))
        {
            MSG_CG_BATTLE_MONSTER_STATUS_RESIST_BRD msg_brd;
            msg_brd.m_dwMonsterObjKey = GetObjectKey();
            BOOST_STATIC_ASSERT(sizeof(msg_brd.m_ScriptIndex) == sizeof(BYTE));
            msg_brd.m_ScriptIndex = static_cast<BYTE>(i);
            SendPacketAround(&msg_brd, sizeof(msg_brd));
        };
    }
}

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
VOID NPC::OnRecover( INT recoverHP, INT recoverMP, INT recoverSD, eRECOVER_TYPE Type, Character* pAttacker )
{	
	DWORD oldHP = GetHP();

	// 실제 회복을 시킨다.
//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    Character::OnRecover( recoverHP, recoverMP, recoverSD, Type, pAttacker );

	// HP 변화
	if ( oldHP != GetHP() )
        SendHPChange();
}

void NPC::SendHPChange()    //_NA001385_20090924_DOMINATION 함수로 변경
{
    MSG_CG_STATUS_CHANGE_HP_BRD HPMsg;
    HPMsg.m_dwObjectKey = GetObjectKey();
    HPMsg.m_dwTargetHP  = GetHP();
    //여기는 몬스터나 npc 들의 정보다. 몬스터들의 ReserveHP는 항상 꽉 차있다.
    HPMsg.m_dwReserveHP = GetMaxHP() - GetHP();
    SendPacketAround(&HPMsg, sizeof(HPMsg), FALSE);
}

void NPC::SendAreaDamage(const DAMAGETYPE damage)
{
    MSG_CG_TRIGGER_AREADAMAGE_BRD msg;
    msg.m_dwObjectKey = GetObjectKey();
    msg.m_wDamage = damage;
    msg.m_dwHP = GetHP();
    SendPacketAround(&msg, sizeof(msg), FALSE);
}

float NPC::GetHPRatio() const
{
	const INT maxHP = m_Attr.GetMaxHP();
	if ( maxHP <= 0 )
		return 0;

	return (float)m_dwHP / maxHP;
}

float NPC::GetMPRatio() const
{
	const INT maxMP = m_Attr.GetMaxMP();
	if ( maxMP <= 0 )
		return 0;

	return (float)m_dwMP / maxMP;
}

float NPC::GetSDRatio() const
{
    const INT maxSD = m_Attr.GetMaxSD();
    if ( maxSD <= 0 )
        return 0;

    return (float)m_dwSD / maxSD;
}

VOID NPC::SetHP( DWORD dwHP )
{ 
	const DWORD maxHP = (DWORD)m_Attr.GetMaxHP();
	m_dwHP = BOUND_CHECK( 0, dwHP, maxHP );
}

VOID NPC::SetAttr( eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, int iValue ) 
{ 
	if ( eAttrType <= eATTR_TYPE_INVALID || eAttrType >= eATTR_MAX ) 
	{
		SUNLOG( eCRITICAL_LOG, "[NPC::SetAttr] eAttrType[%d] is Invalid", eAttrType );
		return;
	}
	m_Attr[eAttrType][eAttrKind] = iValue; 
	m_Attr.UpdateEx(); 
}


bool NPC::IsLeaderAlive() 
{
	if ( !m_pUnitListOfGroup || !m_pUnitGroupInfo ) return false;
	return m_pUnitListOfGroup->IsAlive(m_pUnitGroupInfo->GetLeaderRegenID()); 
}

bool NPC::IsLeader() 
{ 
	if ( !m_pUnitRegenInfo || !m_pUnitGroupInfo ) return false;
	return m_pUnitRegenInfo->GetID() == m_pUnitGroupInfo->GetLeaderRegenID(); 
}

// (WAVERIX) (090617) (BUG-FIX) 그룹 멤버들은 자발적으로 후퇴 못하는 상황 감소시키기 작업
NPC*
NPC::GetLeaderInField()
{
    if (m_pUnitRegenInfo == 0 || m_pUnitGroupInfo == 0)
        return 0;
    REGENID leaderID = m_pUnitGroupInfo->GetLeaderRegenID();
    BOOLEAN isLeader = m_pUnitRegenInfo->GetID() == leaderID;
    if (isLeader)
        return this;
    const UNIT_MEMBER_HASH& rList = m_pUnitListOfGroup->GetUnitMemberHash();
    UNIT_MEMBER_HASH::const_iterator it = rList.find(leaderID);
    if (it != rList.end())
        return it->second;
    return 0;
}

// NOTE: f110503.3L, logic improvements
void NPC::SetHPAttackPowerAsParty(DWORD number_of_party_members)
{
    if (number_of_party_members == 0) {
        return;
    };
    switch (eOBJECT_TYPE npc_object_type = GetObjectType())
    {
    case MONSTER_OBJECT:
    case MAPNPC_OBJECT:
        break;
    default:
        return;
    };
    //
    const Attributes& attributes = m_Attr;
    //----------------------------------------------------------------------------------------------
    // NOTE: increase minimum and maximum attack power
    ;{
        bool is_melee_type = (this->GetWeaponBaseAttType() != eATTACK_TYPE_RANGE);
        int min_attack_power = is_melee_type ? attributes.GetBaseMeleeMinAttPower()
                                                   : attributes.GetBaseRangeMinAttPower();
        int max_attack_power = is_melee_type ? attributes.GetBaseMeleeMaxAttPower()
                                                   : attributes.GetBaseRangeMaxAttPower();
        ;
        const int additional_option_attack_power = ns_formula::CalcAttackPowerIncreaseAsParty(
            number_of_party_members,
            ((min_attack_power + max_attack_power) >> 1));
        if (AttrValue* attr_value = &m_Attr[eATTR_OPTION_PHYSICAL_ATTACK_POWER]) {
            (*attr_value)[eATTR_KIND_BASE] = additional_option_attack_power;
        };
    };
    //----------------------------------------------------------------------------------------------
    // NOTE: increase skill attack power, in cases of EP1, increase magical option attack power
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    if (const BASE_NPCINFO* npc_info = m_pBaseInfo)
    {
        int skill_attack_power = npc_info->skill_power;// GetAttr().GetBaseSkillAttackPower();
        ;   skill_attack_power = ns_formula::CalcAttackPowerIncreaseAsParty(
                number_of_party_members, skill_attack_power);
        if (AttrValue* attr_value = &m_Attr[eATTR_ADD_SKILL_ATTACK_POWER]) {
            (*attr_value)[eATTR_KIND_BASE] = skill_attack_power;
        };
    };
#else
    ;{
        // 마법 옵션 공격력 상승
        const int min_attack_power = attributes.GetBaseMagicMinAttPower();
        const int max_attack_power = attributes.GetBaseMagicMaxAttPower();
        const int additional_magic_option_attack_power = ns_formula::CalcAttackPowerIncreaseAsParty(
            number_of_party_members, ((min_attack_power + max_attack_power) >> 1));
        if (AttrValue* attr_value = &m_Attr[eATTR_OPTION_MAGICAL_ATTACK_POWER]) {
            (*attr_value)[eATTR_KIND_BASE] = additional_magic_option_attack_power;
        };
    };
#endif
    //----------------------------------------------------------------------------------------------
    // NOTE: increase maximum HP quantity
    DWORD last_update_cur_hp = GetHP();
    if (const BASE_NPCINFO* npc_info = m_pBaseInfo)
    {
        float increasing_ratio = \
            ns_formula::CalcHPIncreaseRatioAsParty(number_of_party_members);

        DWORD npc_info_max_hp = npc_info->m_dwMaxHP;
        const DWORD max_hp = util::LL_Round(npc_info_max_hp * increasing_ratio);
        ;     DWORD cur_hp = last_update_cur_hp;
        if (cur_hp)
        {
            DWORD cur_max_hp = GetMaxHP();
            if (cur_max_hp)
            {
                float hp_relative_ratio = cur_hp / static_cast<float>(cur_max_hp);
                DWORD calculated_hp = util::LL_Round(hp_relative_ratio * max_hp);
                cur_hp = max(calculated_hp, 1);
            };
        };
        //
        if (AttrValue* attr_value = &m_Attr[eATTR_MAX_HP]) {
            (*attr_value)[eATTR_KIND_BASE] = max_hp;
        };
        //
        last_update_cur_hp = cur_hp;
    };
    //----------------------------------------------------------------------------------------------
    // NOTE: the latest changes apply to npc attributes = { last_update_cur_hp, ... }
    m_Attr.Update();
    this->SetHP(last_update_cur_hp);
    //----------------------------------------------------------------------------------------------
};

ENUM_STATD_ID NPC::GetCurStateID()
{
    // 2014.02.04 / 민경승 / 정의되어있지 않던 NPC::GetCurStateID()를 정의.
    // return형을 DWORD형에서 ENUM_STATD_ID로 수정. 해당 함수는 ENUM으로 정의된 stateID값과의 비교에 
    // 주로 사용되기 때문.
    return m_pNPCStatusManager->GetCurState()->GetStateID();
}