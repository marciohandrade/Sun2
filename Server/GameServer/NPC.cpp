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
	// OnDead���� �ϸ� ���� �Ŀ� ��Ȱ�� �ȵȴ�. ���� ���� �����ϴ� �ָ� ��Ȱ��Ű�� ������..
	SetTargetChar( NULL );	

	Character::Release();
	m_Attr.Clear();	

	m_pNPCStatusManager->Release();
	m_SkillSelector.Release();

	// AIMessage �ʱ�ȭ
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

	// HP, MP ȸ����
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
    // ���⼭ �ð��� �������ָ� ��ų��뿡 ������ �߻��Ѵ�.
    // ������ ��ų��� �ĸ��� SetTime���ְ�, 'Ȯ��'�� �����ϹǷ� �ʱ�ȭ�� 0�����ص� ����
    m_pSkillSelectTimer->SetTimer( 0 );
#else
    m_pSkillSelectTimer->SetTimer( m_pBaseInfo->m_wSkillUpdateTime );
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

	m_Attr.SetCharInfo( m_pBaseInfo, iRecoverHP, iRecoverMP ,iRecoverSD );
	m_Attr.Update();

	// FSM �ʱ�ȭ
	SetInitialState( dwStateID, byMoveType, dwMoveAreaID, param1 );

	// ��ũ��Ʈ���� ���� ���ǵ�� 0.001�� �����ش�.
	m_pMoveStateControl->Init( this, CMS_WALK );
	SetHP( m_pBaseInfo->m_dwMaxHP );	
	SetMP( m_pBaseInfo->m_dwMaxMP );	
    SetSD( m_pBaseInfo->m_dwMaxSD );
	m_pTargetManager->Init();

    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    search_type_ = FindSearchTypeByAttitude(m_pBaseInfo->m_byAttitude);
	// ��� ������ ��ų���� ����Ѵ�.
	AddDefaultSkill();
}

// implemented by _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
// ���� ����� �˻� Ÿ���� ��ȯ
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
        SUNLOG(eCRITICAL_LOG, "[NPC::FindSearchTypeByAttitude]�������� �ʴ� ���� ���� "
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

    // �����ʿ����� NPC ��� ���ٴ� ��ü Ÿ������ �� [2/23/2010 lst1024]
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

	// STATE_INFO�� ä���.
	GetStatusManager()->SerialzieStatusInfo( pRenderInfo->m_StateTotalInfo );
}

VOID NPC::GetSummonedRenderInfo( SUMMONED_RENDER_INFO * OUT pRenderInfo )
{
	ASSERT( !"[NPC::GetSummonedRenderInfo] �Ϸ� ȣ��Ǹ� �ȵȴ�!" );
	SUNLOG( eCRITICAL_LOG, "[NPC::GetSummonedRenderInfo] ObjectType[%d] �Ϸ� ȣ��Ǹ� �ȵȴ�!", GetObjectType() );
}

VOID NPC::GetTotemNPCRenderInfo( TOTEMNPC_RENDER_INFO * OUT pRenderInfo )
{
	ASSERT( !"[NPC::GetTotemNPCRenderInfo] �Ϸ� ȣ��Ǹ� �ȵȴ�!" );
	SUNLOG( eCRITICAL_LOG, "[NPC::GetTotemNPCRenderInfo] ObjectType[%d] �Ϸ� ȣ��Ǹ� �ȵȴ�!", GetObjectType() );
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
		AIMsgProgress();		//	ť�� ���� AI �޼��� ó��

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

    // �̵� �� �߻��� Ʈ���� �׼ǿ� ���� �ʵ� ������ �Ͼ� ������ �˻��Ѵ�.
    if (GetField() == NULL)
    {
        return FALSE;
    }

	// sector_index																	<- �̵� �� ���� �ε���
	const WzVector* const pwvCurPos		= GetPosPtr();
	const SECTOR_IDX new_sector_index	= pField->GetSectorIndex( pwvCurPos );	//	<- �̵� �� ���� �ε���

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

	// ���߿� ������ ������...
	switch ( pSkillInfo->m_byTarget )
	{
	case SKILL_TARGET_ENEMY:
    case SKILL_TARGET_REACHABLE_ENEMY:
		// ����� ���� ��� ���� Ÿ���� �״�� �����Ѵ�.
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
		// ��ȯ�� �� ��� ���� Ÿ���� �ʿ䰡 ������ ���� Ÿ���� �״�� �����ϸ� �ȴ�.
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

// note2 : ��ȣ���� �ذ��ϱ� ���� �Լ��� ������ - arycoat. 2013.09.13
//float NPC::GetDistToTarget() const
//{
//    // (NOTE) �����ϸ� ������ ���� ����� ó���� �����ϴ� �ɷ� �ǴܵǴ� �Լ���.
//    // ����ü ���� �ٰŸ� �ΰ� ó���� �ϴ°�? ���� ������ ���, �̷��� �� ��찡 ���� �ʾ� �ִµ�,
//    // �� ��ʿ� ������, �̰��� �׶� �׶� ���������� �� ��쿡�� ���� ���� ����̴�.
//    // �ó�����: �̷��� ����� �ʿ��ϴ�. ����? �߰�... ����? �߰�...
//
//    // �� �� ���� ���߰�����, �ִ��� �̷� �͵��� ����ϴ� ��ʸ� �����Ű���� �غ���.
//    // �̰� �ܿ� �Ÿ� ��� ���� in Character, in NPC, etc...
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
    // ���Ž� ���� ���¸� �����Ѵ�.
    m_pNPCStatusManager->ResetStateObject();
	// ���� ������
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
    // ���� ������(���ݾ���, dead�� ������� ����.)
    else if ( byMoveType == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK )
    {
        //���� ���̰� ���� ����
        m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_NO_TRANS );
    }
	// ���� ������
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
	// ��Ʈ��
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
        // ���� �׶��� ��

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
        // ���� �̵��� �� ��Ÿ

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
		// Ư������ �켱�˻�
		if ( byMoveType == eMOVE_ATTITUDE_SEARCH_AREA )
        {
			m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK_AREA, STATE_ID_TRACK_AREA );

			// Ư���������� �޷����� �ϹǷ� �ʱ���¸� �޷����� ���·� �����Ѵ�.
			dwStateID = STATE_ID_TRACK_AREA;
		}
	}

	// �ʱ���� ����
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


// �ֺ����� ���� ã�Ƽ� �� �����͸� �����Ѵ�.
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
			// �޼��� �����̸�ŭ ������ �ʾ����� �ӽ� ť�� �ְ� ó������ �ʴ´�.
			// �����̰� �ٽ� �־������...��ġ��Ÿ -_-;
			m_pTempMessageQueue->SendAIMessage( pRecvMsg, wSize, pRecvMsg->dwDeliveryTick - dwCurTick );			
		}

		m_pAIMessageQueue->Completion( wSize );

		//QueryPerformanceCounter(&nextcounter);

		//elapsedclock = nextcounter.QuadPart - firstcounter.QuadPart;
		//elapsedmillsecond = elapsedclock * 1000 / g_pGameServer->m_freq.QuadPart;
		//if ( elapsedmillsecond >= 10 )
		//{
		//	SUNLOG( eCRITICAL_LOG, "[NPC::AIMsgProgress] ������Ʈ�� �ʹ� �����ɸ�! ObjKey[%d] CurState[%d] MsgID[%d] Tick[%d]", 
		//		GetObjectKey(), m_pNPCStatusManager->GetCurState()->GetStateID(), pRecvMsg->dwMsgID, elapsedmillsecond );
		//}

		pRecvMsg = (AI_MSG*)m_pAIMessageQueue->RecvAIMessage( wSize );
	}

	// �ӽ� ť�� �ִ� �޼��� �ٽ� ���� �ֱ�
	pRecvMsg = (AI_MSG*)m_pTempMessageQueue->RecvAIMessage( wSize );
	while ( pRecvMsg )
	{
		// �����̰� �ٽ� �־������...��ġ��Ÿ -_-;
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
	// �׷��� �������
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
	// �׷��� �������
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
	// ������ �����ϴ� Ÿ�ٽ����� Release�Ѵ�.
	GetTargetChar()->FreeEnemySlot( m_pTargetManager->GetTrackSlot() );

	// ���ο� Ÿ�ٽ����� ã�Ƽ� ������ �����Ѵ�.
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

	//	(WAVERIX)(080923)(NOTE) ó�� ������ �ð��� ����. ���� ���� ���·� �����.

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
                   BOOL is_down_after_thrust) // �и��� �Ŀ� �ٿ� ���·� �Ѿ ���ΰ�?
{
    // (CHANGES) (091229) (WAVERIX) code-arrangement for NPC moving control
    GameField* pField = GetField();
    if (pField == NULL)
        return false;

    // ���� �������� ��� �и��Ⱑ �ȵȴ�.
    if (m_pBaseInfo->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP ||
        m_pBaseInfo->m_byMoveAttitude == eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK)
    {
        return false;
    }

    // TODO: �и��� ������ ���� ��� ������ �����. ��������??
    if (pwvMoveDistance->x == 0.0f && pwvMoveDistance->y == 0.0f) {
        pwvMoveDistance->x = 1.0f;
        WzVector wvTemp = *pwvMoveDistance;
        int angle = State::GetStateRandomNo() % 360;
        Math_ZRotateVector(&wvTemp, *pwvMoveDistance, angle);
        *pwvMoveDistance = *pwvMoveDistance * fMoveDistance;
    }

    // NPC �и��� ó��
    if (pField->FindPathThrust(this, pwvMoveDistance, bForcedMove) == false)
        return false;

    // (NOTE) FindPathThrust�� ���� ���ǹ��� ������ �Ǿ�� �� ���̴�.
    if (IsMoving() == false)
        return false;

    // �и��� ���� �̵� ��ǥ ���
    wvPosAfterThrust = GetPathExplorer()->GetTargetPos();

    // NPC���� �и��� �޼����� ����
    AI_MSG_THRUST thrust_msg;
    thrust_msg.bDownAfterThrust = !!is_down_after_thrust;
    SendAIMessage(&thrust_msg, sizeof(thrust_msg));

    return true;
}

BOOL NPC::IsTooCloseToTarget()
{
    if (GetTargetChar() == NULL)
        return FALSE;

	// Ÿ���� ������ ���(������)
	if ( GetTargetChar() == this )		return FALSE;

	// �ʹ� ����� ���.. ������ �־�����
	// (!) Player�� ���ݰŸ��� ���̰� ���� ���� ���� �÷��̾ �ٰ����� ������ �־����� ������ ���� �� �ִ�.
	//		--> ������ �⺻ ���ݰŸ��� �����ϵ��� ��� ������ �����ϵ��� �ؾ��ҵ�

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

	WzVector vCurPos, vTargetPos;
	GetPos( &vCurPos );	GetTargetChar()->GetPos( &vTargetPos );
	float fDistToTarget2 = VectorLength2( &(vCurPos-vTargetPos) );

	// Ÿ�ٰ��� �Ÿ��� MAX_ATTACK_RANGE_LIMIT�̻��̸� �׳� ����->��Ÿ��� �ֵ��� �ʹ� �־����°� ����
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

	// ���ݰŸ��� 5m�� �Ѿ�� ���� ������ �Ѵ�.
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

// Ÿ�ٺ��� ������ ���� ��ġ�� �ְ� ���� �Ÿ��� ����� ������� ������ �� �����Ƿ� üũ�Ѵ�.
BOOL NPC::IsOnGoodPositionToJump()
{
	if ( !GetTargetChar() )
		return FALSE;

	WzVector wvCurPos, wvTargetPos;
	GetPos( &wvCurPos );	GetTargetChar()->GetPos( &wvTargetPos );

	// ���̰� �ʹ� ���ų� �ʹ� ������ ����
	float fHeight = wvCurPos.z - wvTargetPos.z;
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	if ( fHeight < stAICommonInfo.m_fJUMP_MIN_HEIGHT || fHeight > stAICommonInfo.m_fJUMP_MAX_HEIGHT )
	{
		return FALSE;
	}

	// ���� �Ÿ��� �ʹ� �ְų� �ʹ� ������ ����
	float fDistance = sqrt( ( wvTargetPos.x - wvCurPos.x ) * ( wvTargetPos.x - wvCurPos.x ) +
							( wvTargetPos.y - wvCurPos.y ) * ( wvTargetPos.y - wvCurPos.y ) );

	if ( fDistance < stAICommonInfo.m_fJUMP_MIN_DISTANCE || fDistance > stAICommonInfo.m_fJUMP_MAX_DISTANCE )
	{
		return FALSE;
	}

	m_vJumpPos = wvTargetPos;

	return TRUE;
}


// ��ã�⸦ �ϰ� ��Ŷ�� ����
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

    // ��ã��... ���н� ����
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

	// Ÿ�ٰ��� �Ÿ��� ���� ������ ���� ������ �Ѵ�.
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

	// ���� ��ġ
	GetPos( &vCurPos );	pTarget->GetPos( &vTargetPos );

	// ���� ������ ���
	VectorNormalize( &vNormal, &( vCurPos-vTargetPos ) );	vNormal.z = 0;
	float fAttackRange = max( stAICommonInfo.m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT, GetAttackRange()*stAICommonInfo.m_fJUMP_ATTACK_RANGE_RATIO );
	m_vJumpPos = vTargetPos + vNormal * fAttackRange;
	vDistanceWantToMove = m_vJumpPos - vCurPos;
	float fWantToMoveLength = VectorLength2( &vDistanceWantToMove );

	// ��ã�� �������� üũ
	if ( !pField->FindPathThrust( this, &vDistanceWantToMove ) )
	{
		return FALSE;
	}

	// FindPath�� ������ �Ÿ��� �ǵ��� �Ÿ��� ����ġ�� ��ֹ��� ���� ������ �����Ѵ�.
	vDestPosAfterThrust = GetPathExplorer()->GetTargetPos();
	float fMoveLength = VectorLength2( &(vDestPosAfterThrust - vCurPos) );

	// Thrust �Ÿ��� �ǵ��� �Ÿ���ŭ �ȵǴ� ���
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

    // ���������� ��ã�� ��θ� �����Ѵ�.
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

    // FindPath�� ������ �Ÿ��� �ǵ��� �Ÿ��� ����ġ�� ��ֹ��� ���� ������ �����Ѵ�.
    const WzVector& wvDestPosAfterThrust = GetPathExplorer()->GetTargetPos();
    float fMoveLength = VectorLength2(&(wvDestPosAfterThrust - wvCurPos));

    // Thrust �Ÿ��� �ǵ��� �Ÿ���ŭ �ȵǴ� ���
    if (fMoveLength < fWantToMoveLength * 0.95f)
    {
        // ��ֹ��� ��Ҵٴ� ǥ�ø� �ϰ� ���� ������ ���⿡�� 49�� ȸ����Ų ���͸� ���صд�.
        bTouchedSomething = true;
        const float calc_cos = g_SinTable.FCOS(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE);
        const float calc_sin = g_SinTable.FSIN(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE);

        wvRotatedVector.x = wvDistanceWantToMove.x * calc_cos + wvDistanceWantToMove.y * calc_sin;
        wvRotatedVector.y = -wvDistanceWantToMove.x * calc_sin + wvDistanceWantToMove.y * calc_cos;         
        wvRotatedVector.z = 0.0f;

        // (NOTE) (WAVERIX) �� �� ������ �ʿ� �ִ�. �̵����� ���� ��� ����� ������?
        // ���� �̵��� �ϰ� ����ó���ÿ���?

        // �̵� �Ÿ��� �ʹ� ª���� �̵����� ����(��Ŷ �۽� ����) �׳� ����
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
        //  (WAVERIX) (081209) (BUG-FIX) ������ �޸��� ����
        //msg.m_byState   = CMS_RUN;  //ĳ������ ���� ������ �ٱ⸸ ����!!(���Ϳ� ���� ���� ��Ŷ�̹Ƿ� ���� �־���)
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
        // �� �Լ��� '���� ���ݻ�Ÿ����� ���Դ°�?'�� üũ�� ������ ����ϰ� �� ����, �밳 �ش� �б⸦ Ÿ�Եȴ�.
        // �������¿��� �ҷ��� �ÿ� �̺κ�����
        // �� �б⹮�� ������ ���� �Ͼ �� �ִ� ���̹Ƿ� ASSERT ����
		//ASSERT( GetSelectedSkill() != SKILLCODE_INVALID );
		fAttackRange += 2.5f;
	}
	else if ( GetSelectedSkill() == SKILLCODE_NORMAL_ATTACK )
	{
		// �Ϲ� ���ݽ� NPC�� �⺻ ���� ��Ÿ��� ����
		fAttackRange += m_pBaseInfo->m_fAttRange;
	}
	else
	{
		// ��ų ���ݽ� �ش� ��ų�� ���� ��Ÿ��� ����
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

	// �������� üũ�Ѵ�.
	if ( this == pTarget )	return USER_RELATION_FRIEND;

	// ���ΰ� Ÿ���� ��ȯ�� ���θ� Ȯ���Ѵ�.
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
		// ���μ�ȯ���� Ÿ�԰� Ÿ�ټ�ȯ���� Ÿ���� ������ �Ʊ��� �ȴ�.
		return pMySummoner->IsFriend( pTargetSummoner );
	}
	else if ( pMySummoner )
	{
		// ���μ�ȯ���� Ÿ�԰� Ÿ���� Ÿ���� ������ �Ʊ��� �ȴ�.
		return pMySummoner->IsFriend( pTarget );
	}
	else if ( pTargetSummoner )
	{
		// ���� Ÿ�԰� Ÿ�ټ�ȯ���� Ÿ���� ������ �Ʊ��� �ȴ�.
		return IsFriend( pTargetSummoner );
	}

	if ( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
	{
		//	SSQMonster�� ���͵� ������ ������ �ν��Ѵ�.
		//	Ʈ�� �迭 NPC���� �����ϴ� ���� �����ϱ� ���ؼ� �̵鸸 ģ���� �ν��ϵ��� �Ѵ�.
        if (pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
            return USER_RELATION_FRIEND;
		return USER_RELATION_ENEMY;
	}
	else
	{
        // ���� ���ͳ����� �Ʊ����� �����Ѵ�.
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

	// ���ʷ� ������ ���� �ѿ��� �ʱ� ����Ʈ�� �ֱ����� AddAttackPoint�� ȣ���Ѵ�.
	m_pTargetManager->AddBattlePoint( pTarget, BATTLE_POINT_DAMAGE, 0 );
}



void
NPC::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;

    //	(WAVERIX)(081013)(NOTE) �ʹ� ��� �ߺ��� ����. ������ ���� �۾��� ������ �����ϱ�� �Ѵ�.
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

        // ���� ������ �߰�? ���� ���� �������� �߰��ؾ� �ϴ°��� �ƴѰ�? [2/17/2010 lst1024]
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
            rChaoState.KillMonster( this );                         // ���Ͱ� ī�� �÷��̾�κ��� ������ �޾Ƽ� �׾��� ���

            if (damage_arg->option_.is_mirror_ == false)
            {
                Attributes& rAtt = pAttackPlayer->GetAttr();
                const int AbsorbHP = rAtt.GetAbsorbHP();
                const int AbsorbMP = rAtt.GetAbsorbMP();

                if ( AbsorbHP )
                    pAttackPlayer->OnRecover( AbsorbHP, 0 );        // HP ���
                if ( AbsorbMP )
                    pAttackPlayer->OnRecover( 0, AbsorbMP );        // MP ���
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
	// ���� �˻�
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
            // ���ݴ���� ������ ������û ��ȿ
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
            // ���ݴ���� ������ ��ų���� ��ȿ
            if ( !GetTargetChar() )	return;

            SelectSkill( GetTargetChar(), special_condition.wActionParam );
            ChangeState( STATE_ID_TRACK );
        }
        break;
    }    
}

//_NA_0_20100514_NPC_DEAD_STATE_TIME(����)
VOID NPC::SpecialAction()
{
	SPECIAL_CONDITION *pSpecialCondition = NULL;

	// ���� �˻�
	for ( int i = 0; i < 2; ++i )
	{
		pSpecialCondition = &GetBaseInfo()->m_SpecialCondition[i];

		if ( CheckCondition( pSpecialCondition->byCondition, pSpecialCondition->wConditionParam ) )
		{
			// Ȯ�� üũ
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
			// ���ݴ���� ������ ������û ��ȿ
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
			// ���ݴ���� ������ ��ų���� ��ȿ
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

	// ���� ȸ���� ��Ų��.
//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    Character::OnRecover( recoverHP, recoverMP, recoverSD, Type, pAttacker );

	// HP ��ȭ
	if ( oldHP != GetHP() )
        SendHPChange();
}

void NPC::SendHPChange()    //_NA001385_20090924_DOMINATION �Լ��� ����
{
    MSG_CG_STATUS_CHANGE_HP_BRD HPMsg;
    HPMsg.m_dwObjectKey = GetObjectKey();
    HPMsg.m_dwTargetHP  = GetHP();
    //����� ���ͳ� npc ���� ������. ���͵��� ReserveHP�� �׻� �� ���ִ�.
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

// (WAVERIX) (090617) (BUG-FIX) �׷� ������� �ڹ������� ���� ���ϴ� ��Ȳ ���ҽ�Ű�� �۾�
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
        // ���� �ɼ� ���ݷ� ���
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
    // 2014.02.04 / �ΰ�� / ���ǵǾ����� �ʴ� NPC::GetCurStateID()�� ����.
    // return���� DWORD������ ENUM_STATD_ID�� ����. �ش� �Լ��� ENUM���� ���ǵ� stateID������ �񱳿� 
    // �ַ� ���Ǳ� ����.
    return m_pNPCStatusManager->GetCurState()->GetStateID();
}