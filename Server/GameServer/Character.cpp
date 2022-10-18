// Character.cpp: implementation of the Character class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <float.h>

#include <3DEngine/3DTerrain/PathExplorer.h>
#include <3DEngine/3DTerrain/3DTerrain.h>
#include <3DEngine/3DTerrain/WorldBase.h>
#include <Global.h>
#include <PublicMath.h>
#include <SinTable.h>
#include <PacketStruct_AG.h>
#include <PacketControl/WarMessages_Sync.h>
#include <ServerOptionParser.h>
#include <SkillInfoParser.h>
#include <ServerOptionParserEx.h>
#include <CoolTimerFactory.h>

#include "Character.h"
#include "FieldServer.h"
#include "FieldInfo.h"
#include "GameField.h"
#include "AITypes.h"
#include "NPC.h"
#include "SyncMerchant.h"
#include "SummonedNPCs.h"
#include "GameParty.h"
#include "GamePartyManager.h"
#include "Map.h"
#include "StatusManager.h"
#include "AbilityStatus.h"
#include "SummonManager.h"
#include "CharacterFormula.h"
#include "AIParamParser.h"
#include "EtcStatus.h"
#include "StatusManager.h"
#include "RecoverStatus.h"
#include "Summoned.h"
#include "MoveStateControl.h"
#include "TargetManager.h"
#include "Skill.h"
#include "GameCHuntingRoom.h"
#include "CHuntingManager.h"
#include "ItemManager.h"
#include "Player.h"
#include "SkillManager.h"
#include "MovementChecker.h"
#include "GameInstanceDungeon.h"
#include "World/ChaosZoneSystem/ChaosZoneManager.h"
#include "MovingPlanner.h"
#include <AreaConquestCommonHeader.h>
#include "GameBattleResult.h" //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "ExtraNPCManager.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/ChaosZone.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

class AIMessageSender
{
public:
    AIMessageSender(AI_MSG* message, WORD message_size) : 
        message_(message), message_size_(message_size)
    {
        ASSERT(message != NULL);
        ASSERT(message_size > 0);
    }

    bool operator() (Object* const object)
    {
        if (object == NULL)
        {
            return false;
        }
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        if (object->IsDeleted())
        {
            return false;
        }
//#endif
        if (message_ == NULL)
        {
            return false;
        }
        if (message_size_ < 1)
        {
            return false;
        }
        Character* const character = dynamic_cast<Character*>(object);
        if (character == NULL)
        {
            return false;
        }
        character->SendAIMessage(message_, message_size_);
        return true;
    }
private:
    AI_MSG*	message_;
    WORD message_size_;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Character::Character() : 
    PlanEvent(m_pPathExplorer, m_pSkillManager, m_pStatusManager), 
    used_fighting_energy_size_(0),
    team_key_(kChaosZoneTeamNone)
{
	SetObjectType(CHARACTER_OBJECT);

	Object::_SetPosVector(m_pPathExplorer->GetPosPtr());
	WzVector wzVec = { 0, 0, 0 };
	SetPos(&wzVec);

	m_pStatusManager	= new StatusManager(this);
	m_pMoveStateControl	= new MoveStateControl;
	m_pSkillManager		= new SkillManager(this);

	m_dwReserveHP		= 0;		//적립된 HP
	m_dwDeadReserveHP	= 0;		//죽었을시 적립된 HP
	m_DeadType = eCHAR_DEAD_NOT_DEAD;
}

Character::~Character()
{
#ifndef _SUSUNLOVE_DRIVE_NOT_FOUND_
	g_pGameServer->m_fnContactTerrain(CT_DELETE_PATH_EXPLORER, m_pPathExplorer);
#endif

	SAFE_DELETE(m_pStatusManager);
	SAFE_DELETE(m_pMoveStateControl);
	SAFE_DELETE(m_pSkillManager);

	m_dwReserveHP = 0;				//적립된 HP
	m_dwDeadReserveHP = 0;			//죽었을시 적립된 HP
}

BOOL Character::Init()
{
	Policyment.SetField(Policyment.PCC_IN_DEF_FIELD);

	ZeroMemory(m_iEnemySlot, sizeof(int)*MAX_ENEMYSLOT_NUM);

	m_PartyState.Init();

	// 스킬관련
	m_iShieldHP				= 0;
	m_iShieldMP				= 0;
    decrease_MP_Ratio       = 0;
	m_fShieldAbsorbRatio	= 0;
	m_sFightingEnergyCount	= 0;
    used_fighting_energy_size_ = 0;

	// 사망 경험치 손해
	m_DeadExp				= 0;

	m_dwSummonerKey			= 0;

	SetGameZone(ePRS_NOT_AT_ZONE, NULL, 0);

	// 길찾기 초기화
	m_pPathExplorer->Clear();
	PlanEvent.Init(this);

	m_dwReserveHP = 0;				//적립된 HP
	m_dwDeadReserveHP = 0;			//죽었을시 적립된 HP
	m_pSkillManager->Init();
    // (CHANGES) (f100514.3L) add an initializer because of the status without a 'GetField' check
    // has somehow corrupted the data integrity.
    m_pStatusManager->Release();

    return Object::Init();	
}

void Character::Release()
{
    // (WAVERIX) (090607) (BUG-FIX) 쉐도우 잘못된 타입 비교 구문 수정
    //    // 섀도우는 한 서버안에서는 계속 소환체를 유지한다.
    //    // 따라서 Release시에만 소환체를 삭제한다.
    //__NA_S00015_20080828_NEW_HOPE_SHADOW
    //    if((GetObjectType() & eCHAR_SHADOW) == eCHAR_SHADOW)
    //        SummonManager::Instance()->LeaveSummonNPCs(this);
    //#endif

	m_Observers.RemoveAll();
	Object::Release();

	PlanEvent.Release();

	m_pStatusManager->Release();
	m_pSkillManager->Release();

	m_dwReserveHP = 0;				//적립된 HP
	m_dwDeadReserveHP = 0;			//죽었을시 적립된 HP
}


BOOL Character::Update(DWORD dwDeltaTick)
{
    if(Object::Update(dwDeltaTick) == false)
        return false;

	DWORD curTick = GetTickCount();
	m_pStatusManager->Update(curTick);

	m_pSkillManager->Update();

	return true;
}


void Character::SetField(GameField* pField)
{
	Object::SetField(pField);
}


void Character::SetPos(const WzVector* pwzVec)
{
	GameField* const pGameField = GetField();
	if(!pGameField)
		return;

	pGameField->GetMovingPlanner()->PathPlan_Reset(this, pwzVec);
}

void Character::GetDestPos(WzVector* pWzVecOut) const
{
	*pWzVecOut = m_pPathExplorer->GetCurTargetPos();
}



//--------------------------------------------------------------------------------------------------

void
Character::SendPacketAround(MSG_BASE_FORWARD* pMsg, WORD wSize, BOOL bSendToMe)
{
	GameField* const pField = GetField();
	if(!pField)
		return;

	const SECTOR_IDX current_sector_index = GetSectorIndex();
	pField->SendPacketAround(current_sector_index, pMsg, wSize, bSendToMe ? 0 : GetObjectKey());
}


//--------------------------------------------------------------------------------------------------

void
Character::SendExPacketAround(DWORD dwNumberOfMessages, BYTE** pMsg, WORD* pwSize, BOOL bSendToMe)
{
	GameField* const pField = GetField();
	if(!pField)
		return;

	const SECTOR_IDX current_sector_index = GetSectorIndex();
	pField->SendExPacketAround(current_sector_index, dwNumberOfMessages, pMsg, pwSize, bSendToMe ? 0 : GetObjectKey());
}

//--------------------------------------------------------------------------------------------------

WzVector Character::GetRandomPosInAround(int iDist, int iLoop)
{
	WzVector vCurPos;	GetPos(&vCurPos);

	if(!GetField())
	{
		SUNLOG(eCRITICAL_LOG, "[Character::GetRandomPosInAround] Field is NULL!");
		return vCurPos;
	}

	WzVector vDestPos;
	if(!GetField()->GetRandomPosInAround(&vCurPos, iDist, vDestPos, iLoop))
	{
		// 그렇게 크리티컬하지 않으므로 로그는 뺀다.
		return vCurPos;
	}

	return vDestPos;
}


//	1차... 좀더 time slicing을 수행해야 하는 구조로 변경해야 한다.
// 길찾기 경로 설정
BOOL
Character::SetPath(WzVector* pwvCur, WzVector* pwvDest, int* pTileRoute, int iNumTileRoute)
{
	const DWORD ERRCODE_INVALID_STATUS		= 100;
	const DWORD ERRCODE_INVALID_POSITION	= 110;
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		GameField* const pGameField = GetField();
		IfFailThenBreak(pGameField, ERRCODE_INVALID_STATUS);
		IfFailThenBreak(WzVector_FiniteCheck(pwvCur) && WzVector_FiniteCheck(pwvDest),
                        ERRCODE_INVALID_POSITION);

		// TODO(WAVERIX) 이 부분 재 검증할 것.
		SetPos(pwvCur);

		//	이전에 존재하던 제약 조건들은 Moving Planner에서 검증하게 될 것이다.
		nsAI::MovingPlanner* const pMovingPlanner = pGameField->GetMovingPlanner();

		return pMovingPlanner->PathPlan_SetPath(this, pwvDest, pTileRoute, iNumTileRoute);
    }
    FLOWCONTROL_END;
	
	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>
	switch(const DWORD errcode = flow.GetErrorCode())
	{
	case ERRCODE_INVALID_POSITION:
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Pos is Invalid x:y:z={%f,%f,%f} | x:y:z={%f,%f,%f}"
			, pwvCur->x, pwvCur->y, pwvCur->z, pwvDest->x, pwvDest->y, pwvDest->z);
		break;
	default:
		SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Error %u", errcode);
		break;
	}
	
	return false;
}


//	1차... 좀더 time slicing을 수행해야 하는 구조로 변경해야 한다.
// 길찾기 수행
BOOL
Character::PathProgress(float fMove, ENUM_CHECK_COLLISION CollisionCheck)
{
	//fMove = 거리 = 시간* 속도
	GameField* const pGameField = GetField();
	const BOOLEAN bRet = pGameField->GetMovingPlanner()->PathPlan_Progress(this, fMove, NULL, CollisionCheck);
	return bRet;
}

//	1차... 좀더 time slicing을 수행해야 하는 구조로 변경해야 한다.
void
Character::StopMoving()
{
	m_pPathExplorer->Stop();
}

//	1차... 좀더 time slicing을 수행해야 하는 구조로 변경해야 한다.
BOOL
Character::IsMoving()
{
	return m_pPathExplorer->IsMoving();
}

DWORD Character::IncreaseHP(DWORD dwIncreament)
{
	const DWORD curHP = GetHP();
	const DWORD calcedHP = curHP + dwIncreament;
	const DWORD maxHP = GetMaxHP();

	const DWORD hp = min(maxHP, calcedHP);
	dwIncreament = hp - curHP;

	SetHP(hp);

	if(dwIncreament)
	{
		m_pStatusManager->ChangeHP();
	}

	return dwIncreament;
}

DWORD Character::DecreaseHP(DWORD dwDecrement, int fLimitHP)
{
	DWORD	curHP = GetHP();
	BOOL	isDead = false;

	if (curHP == 0)
	{
		return 0;
	}

    // removed by __NA_001290_20090525_SHIELD_SYSTEM
	// 쉴드에 의해 방어되는 HP 수치를 구한다.
	//ApplyMagicShield(dwDecrement);

    //__BUG_2572_SPIRIT_SLUG_BUG
	if(fLimitHP > 0 && fLimitHP > ((int)curHP - (int)dwDecrement))
	{
		if(curHP > (DWORD)fLimitHP)
		{
			dwDecrement = curHP - fLimitHP;
		}
		else
		{
			dwDecrement = 0;
		}
	}

    if(curHP <= dwDecrement)	// 남은 HP보다 더 많이 줄어들면 죽는거다~
    {
        dwDecrement = curHP;
        SetHP(0);
        OnDead();
        isDead = true;
    }
    else
    {
        DWORD hp = curHP - dwDecrement;
        SetHP(hp);
    }

	m_pStatusManager->ChangeHP();

	if(isDead)
	{
#ifdef _JAPAN //일본은 지역점령전에서 스스로 부할을 할수 없다.
        GameZone* zone = GetGameZonePtr();
        if(zone)
        {
            const sPOLICY_ZONE& zone_policy = zone->Policyment;
            if (zone_policy.FIELD & zone_policy.ENABLE_DOMINATION_CONTROL) 
            {
                return dwDecrement;
            }
        }
#endif
		int resurrect = random(0, 100); 
		if(resurrect < GetAttr().GetResurrectionRatio())
        {  
            OnResurrection(0, 1.f, 1.f);
        }
	}

	return dwDecrement;
}


DWORD Character::IncreaseMP(DWORD dwIncreament)
{
	const DWORD curMP = GetMP();
	const DWORD calcedMP = curMP + dwIncreament;
	const DWORD maxMP = GetMaxMP();

	DWORD mp = min(maxMP, calcedMP);
	dwIncreament = mp - curMP;

	SetMP(mp);

	return dwIncreament;
}


DWORD Character::DecreaseMP(DWORD dwDecrement)
{
	DWORD curMP = GetMP();
	if (curMP <= dwDecrement)
	{
		dwDecrement = curMP;
		SetMP(0);		
	}
	else
	{
		DWORD mp = curMP - dwDecrement;
		SetMP(mp);
	}

	return dwDecrement;
}

//{__NA_001290_20090525_SHIELD_SYSTEM
DWORD Character::IncreaseSD(DWORD dwIncreament)
{
    const DWORD curSD    = GetSD();
    const DWORD maxSD    = GetMaxSD();
    const DWORD calcedSD = curSD + dwIncreament;
    const DWORD newSD    = min(maxSD, calcedSD);
    const BOOL bAllocStatus = (0==curSD && 0!=newSD);
    const DWORD dwRealIncreament = newSD-curSD;

    SetSD(newSD);
    if(bAllocStatus)
    {
        ALLOC_STATUS_PAIR pairBaseStatus = m_pStatusManager->AllocStatus(eCHAR_STATE_ETC_EXIST_SHELD_POINT);        
        if(pairBaseStatus.second)
            pairBaseStatus.second->SendStatusAddBRD();
    }

    return dwRealIncreament;
}


DWORD Character::DecreaseSD(DWORD dwDecrement)
{
    const DWORD curSD = GetSD();
    if (curSD <= dwDecrement)
    {
        dwDecrement = curSD;
        SetSD(0);
        m_pStatusManager->Remove(eCHAR_STATE_ETC_EXIST_SHELD_POINT);
    }
    else
    {
        const DWORD newSD = curSD - dwDecrement;
        SetSD(newSD);
    }

    return dwDecrement;
}

// 데미지 감소 순서 조정, 소스정리 [8/14/2009 lst1024]
DAMAGETYPE Character::_ApplyMagicShield(DAMAGETYPE wDamage)
{
    if((!m_iShieldMP && decrease_MP_Ratio == 0) || !m_iShieldHP)
        return wDamage;

    //DWORD character_MaxMP = GetMaxMP();
    DWORD decrease_MP = 0;
    
#ifdef _NA_006621_20130408_MAGICSHILD_MP_DECREASE
    decrease_MP = (DWORD)(wDamage * decrease_MP_Ratio);
#else
    decrease_MP = m_iShieldMP;
#endif //_NA_006621_20130408_MAGICSHILD_MP_DECREASE

    if((int)GetMP() < decrease_MP)          // MP가 부족하면 쉴드가 적용 안된다.
        return wDamage;


    int iAbsorbDamage = (int)(wDamage * m_fShieldAbsorbRatio);

    iAbsorbDamage = min(iAbsorbDamage, m_iShieldHP);  // 흡수 데미지는 쉴드HP 보다 낮고
    iAbsorbDamage = max(iAbsorbDamage, 0);            // 0보다 큰 값이다.
    iAbsorbDamage = min(iAbsorbDamage, (int)wDamage); // 현재 데미지 보다 클수 없다.


    m_iShieldHP -= iAbsorbDamage;
    SetMP(GetMP() - decrease_MP);

    if(0 >= m_iShieldHP) {
        m_iShieldHP = 0;
        m_pStatusManager->Remove(eCHAR_STATE_MAGIC_SHIELD);
    }

    return wDamage - iAbsorbDamage;                     // 매직쉴드가 적용된 데미지를 리턴한다.
}

BYTE Character::AllocEnemySlot(WzVector* pwvEnemyPos, WzVector& wvResultPos, float fAreaRadius)
{
	// pwzVec를 기준으로 가장 근접한 InitSlot를 구해준다.
	WzVector vCurPos = m_pPathExplorer->GetPos();
	WzVector diffVec =* pwvEnemyPos - vCurPos;
	int CurAngle = Math_GetAngleFromVector2(&diffVec);
	int UnitAngle = 360 / MAX_ENEMYSLOT_NUM;
	int InitSlot = (int)(CurAngle / UnitAngle);
	if(InitSlot >= MAX_ENEMYSLOT_NUM)	InitSlot -= MAX_ENEMYSLOT_NUM;
	int MidAngle = (int)(UnitAngle* (InitSlot + 0.5f));

	if(InitSlot < 0 || InitSlot > MAX_ENEMYSLOT_NUM)
	{
		SUNLOG(eCRITICAL_LOG, "[AllocEnemySlot] Invalid InitSlot[%d] CurAngle[%d]", InitSlot, CurAngle);
		InitSlot = 0;
	}

	// InitSlot을 기준으로 가장 비어있는 Position을 할당해준다.
	int MinEnemy = m_iEnemySlot[InitSlot];
	int MinSlot, NextSlot, PrevSlot;
	MinSlot = NextSlot = PrevSlot = InitSlot;

	for(int i = 0; i < MAX_ENEMYSLOT_NUM/4; i++)
	{
		if(CurAngle > MidAngle)
		{
			if(++NextSlot >= MAX_ENEMYSLOT_NUM)	NextSlot -= MAX_ENEMYSLOT_NUM;
			if(--PrevSlot < 0)					PrevSlot += MAX_ENEMYSLOT_NUM;
		}
		else
		{
			if(--NextSlot < 0)					NextSlot += MAX_ENEMYSLOT_NUM;
			if(++PrevSlot >= MAX_ENEMYSLOT_NUM)	PrevSlot -= MAX_ENEMYSLOT_NUM;
		}

		if(MinEnemy > m_iEnemySlot[NextSlot])	
		{
			MinSlot = NextSlot;
			MinEnemy = m_iEnemySlot[MinSlot];
		}
		if(MinEnemy > m_iEnemySlot[PrevSlot])	
		{
			MinSlot = PrevSlot;
			MinEnemy = m_iEnemySlot[MinSlot];
		}
	}

	if(MinSlot < 0 || MinSlot > MAX_ENEMYSLOT_NUM)
	{
		SUNLOG(eCRITICAL_LOG, "[AllocEnemySlot] Invalid MinSlot[%d] InitSlot CurAngle[%d]", MinSlot, InitSlot, CurAngle);
		MinSlot = 0;
	}

	// 사용하게 될 MinSlot의 m_iEnemySlot를 하나 늘려준다.
	++m_iEnemySlot[MinSlot];

	// 최종적으로 Enemy(몬스터)가 쫓아야할 좌표를 셋팅해준다.
	WzVector vRelativePos;
	SetVector(&vRelativePos, g_SinTable.FCOS(UnitAngle*MinSlot), g_SinTable.FSIN(UnitAngle*MinSlot), 0);
	wvResultPos = vCurPos + (vRelativePos*fAreaRadius);

	WzVector diffEnemyVec = wvResultPos - *pwvEnemyPos;
	float fMoveLength2 = Math_Vector2DLength2(&diffEnemyVec);
	if(fMoveLength2 < (fAreaRadius * 0.2f * fAreaRadius * 0.2f))
		wvResultPos = vCurPos;

	return (BYTE)MinSlot;
}


void Character::FreeEnemySlot(BYTE bySlot)
{
	if(bySlot == TargetManager::TRACK_SLOT_NONE)		return;
	if(bySlot > MAX_ENEMYSLOT_NUM)					return;

	if(m_iEnemySlot[bySlot] <= 0)
		return;

	--m_iEnemySlot[bySlot];
}


void Character::SendToObservers(AI_MSG* pMsg, WORD wSize)
{
	AIMessageSender op(pMsg, wSize);
	m_Observers.Foreach(op);
}


class RemoveTargetOpr
{
public:
	RemoveTargetOpr(Character* pDeadChar) : m_pDeadChar(pDeadChar) {}
	~RemoveTargetOpr() {}

	void operator () (Object* pObject)
	{
        if(pObject->IsEqualObjectKind(NPC_OBJECT))
		{
			NPC* pNPC = (NPC*)pObject;
			pNPC->GetTargetManager()->RemoveBattleRecord(m_pDeadChar->GetObjectKey());
		}
	}

private:
	Character*	m_pDeadChar;
};


void Character::OnDead()
{
	// 죽은 캐릭터를 타겟에서 제외
	RemoveTargetOpr op(this);
	m_Observers.Foreach(op);

	StopMoving();

	// 죽으면 스킬리스트 초기화
	m_pSkillManager->ReleaseDead();
	DelFightingEnergy(GetFightingEnergy());

	if(Policyment.FIELD & Policyment.PCC_IN_DEF_FIELD)
	{
		// 죽으면 모든 Status를 꺼줘야할까? --> 고민을 해보자. ;;
		// 죽으면 패널티를 적용한다.
		ApplyPenalty();

		if(ePRS_AT_CHUNTING == GetGameZoneState() && GetField())
		{
			Character* pKiller = GetField()->FindCharacter(m_dwKillerObjectKey);
			if(pKiller)
			{
				GameCHuntingRoom* pCHuntingRoom = (GameCHuntingRoom *)GetGameZonePtr();
				if(pCHuntingRoom)
					pCHuntingRoom->GetCHuntingManager()->DeadCharacter(this, pKiller);
			}
		}
	};

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameField* game_field = GetField();
    GameZone* game_zone = GetGameZonePtr();
    if (game_zone && game_field)
    {
        MAPCODE mapcode = game_zone->GetMapCode();
        GameBattleResult* battle_result = game_zone->GetGameBattleResult();
        if (battle_result != NULL)
        {
            Character* killer = game_field->FindCharacter(m_dwKillerObjectKey);
            if (killer != NULL)
            {
                eBATTLE_RECORD_TYPE record_type = IsEqualObjectKind(PLAYER_OBJECT) ? 
                                            BATTLE_RECORD_PLAYER_KILL : BATTLE_RECORD_MONSTER_KILL;

#ifdef _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
                if (killer->IsEqualObjectKind(PLAYER_OBJECT))
                    battle_result->DoRecord(killer, record_type, 0);
                else if (killer->IsEqualObjectKind(SUMMON_OBJECT))
                    battle_result->DoRecord(killer->GetPlayerOwner(), record_type, 0);
#else
                battle_result->DoRecord(killer, record_type, 0);
#endif // _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
                battle_result->DoRecord(this, BATTLE_RECORD_DEAD, 0);
            }
#ifdef _NA_001990_ACSYSTEM_ADD
            if ((IsEqualObjectKind(SUMMON_OBJECT) == false) || (GetPlayerOwner() == NULL))
            {
                // Player가 소환한 소환수에 대해서는 AC 를 체크하지 않는다.
                if (killer && killer->IsEqualObjectKind(PLAYER_OBJECT))
                {
                    Player* pKiller = static_cast<Player*>(killer);
                    AchievementManager* pACManager = pKiller->GetACManager();
                    if(IsEqualObjectKind(PLAYER_OBJECT))
                    {
                        pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_PC_KILL, mapcode );
                        pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_PC_KILL_ACCUMULATED, mapcode );
                    }
                    else
                    {
                        pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_NPC_KILL, mapcode );
                        pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_NPC_KILL_ACCUMULATED, mapcode );
                    }

                    pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_KILL, mapcode );
                    pACManager->IncConditionValue( ACHIEVEMENT_OBJECT_KILL_ACCUMULATED, mapcode );
                }
            }

            if (this->IsEqualObjectKind(PLAYER_OBJECT))
            {
                Player* pPlayer = static_cast<Player*>(this);
                if( game_field->GetMovingPlanner()->CheckAttribute(GetPathExplorer()->GetTile(), PTA_PLAYER_FALLDOWN_TILE) == TRUE )
                {
                    pPlayer->GetACManager()->IncConditionValue( ACHIEVEMENT_OBJECT_FALL, mapcode );                    
                }
            }
#endif //_NA_001990_ACSYSTEM_ADD
        }

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        ;{
            Character* kill = game_field->FindCharacter(m_dwKillerObjectKey);
            Character* dead = game_field->FindCharacter(GetObjectKey());//(this);
            if(kill && dead)
            {
                if (kill->IsEqualObjectKind(PLAYER_OBJECT))
                {
                    Player* kill_user = static_cast<Player*>(kill);
                    AchievementManager* kill_ac_manager = kill_user->GetACManager();

                    WORD object_code = (dead->IsEqualObjectKind(PLAYER_OBJECT) ? 2 : 1 );

                    kill_ac_manager->IncConditionValue( ACHIEVEMENT_OBJECT_KILL_ALL, object_code);
                    kill_ac_manager->IncConditionValue( ACHIEVEMENT_OBJECT_KILL_ALL, 3 );
                }

                if (dead->IsEqualObjectKind(PLAYER_OBJECT))
                {
                    Player* dead_user = static_cast<Player*>(dead);
                    AchievementManager* dead_ac_manager = dead_user->GetACManager();

                    WORD object_code = (kill->IsEqualObjectKind(PLAYER_OBJECT) ? 2 : 1 );

                    dead_ac_manager->IncConditionValue( ACHIEVEMENT_OBJECT_DEAD_ALL, object_code );
                    dead_ac_manager->IncConditionValue( ACHIEVEMENT_OBJECT_DEAD_ALL, 3 ); 
                }
            }
        };
#endif//_NA_003027_20111013_HONOR_SYSTEM
    }

	SetDeadType(eCHAR_DEAD_NORMAL);
    /*
    // (CHANGES) (f100329.2L) (WAVERIX) synchronization control flag based on room \
    // related to PvP, GvG, and NvN
    if (game_zone->Policyment.FIELD & game_zone->Policyment.ENABLE_NvN_SYNC)
    {
    }
    else
    */
    // CHANGES: f110303.3L, added rule to support _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    if (SkillExecutionTransaction::kIsTransaction == 0)
    {
        MSGSUB_SYNC_CHARACTER_DEAD_BRD msg;
        msg.m_ObjectKey = GetObjectKey();
        SendPacketAroundForSync(&msg, true);
    }
}


BOOL Character::OnResurrection(float fRecoverExp, float fRecoverHP, float fRecoverMP, Player* pHealer)
{
	if(IsAlive())	return false;

	// 손해본 경험치 회복
	EXPTYPE RecoverExp = (DWORD)(m_DeadExp * fRecoverExp);
	AddExp(RecoverExp, 0, 0, false);

	// HP, MP 회복
	const DWORD dwRecoverHP = (DWORD)(GetMaxHP() * fRecoverHP);
	const DWORD dwRecoverMP = (DWORD)(GetMaxMP() * fRecoverMP);
    const DWORD dwRecoverSD = (DWORD)GetMaxSD();
    SetSD(dwRecoverSD);
	SetHP(dwRecoverHP);
	SetMP(dwRecoverMP);

	return true;
}


void Character::OnAttack(Character* pTarget, SLOTCODE skillcode, DAMAGETYPE damage)
{
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    m_pStatusManager->Attack(pTarget, damage);
#else
    m_pStatusManager->Attack(damage);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

void Character::Attacked()
{
	m_pStatusManager->Attacked();
}


void
Character::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;
    // CHANGES: f101124.3L, added a damage rule such as a self damage rule
    // if a reflect option is checked
    if (this == pAttacker || damage_arg->option_.is_mirror_) 
    {
        // 자기가 자신한테 데미지를 입혀 죽을 수 있을 경우 남은 피의 1%만 남겨놓는다.
       int cur_hp = GetHP();
       if (cur_hp <= damage_arg->damage_) { // f101124.3L, changed a comarator '<' to '<='
           damage_arg->damage_ = static_cast<DAMAGETYPE>(cur_hp - (cur_hp * 0.01f));
       }
    }

    if (pAttacker)
    {
        m_KillerType = pAttacker->GetObjectType();
        m_dwKillerObjectKey = pAttacker->GetObjectKey();
    }
    else
    {
        m_KillerType = MAX_OBJECT;
        m_dwKillerObjectKey = 0;
    }

    if (damage_arg->option_.is_mirror_ == false)
    {
        // TODO: f100908.1L, need check
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
        m_pStatusManager->Damaged(pAttacker, damage_arg);
#else
        DAMAGETYPE result_damage = damage_arg->damage_;
        m_pStatusManager->Damaged(pAttacker, damage_arg->attack_type_, result_damage);
        damage_arg->damage_ = result_damage;
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    }

    damage_arg->damage_ = _ApplyOptionDecDamage(pAttacker, damage_arg->damage_,
                                                damage_arg->attack_regist_kind_);
#ifdef _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    if (this != pAttacker)
    {
        const DWORD absorb_sd = damage_arg->damage_ * pAttacker->GetAttr().GetAttackDamageAbsorbSDRatio() / 100;
        if (absorb_sd > 0)
        {
#ifdef _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE
			//if(pAttacker->GetDeadType() == eCHAR_DEAD_NOT_DEAD)
			{
				pAttacker->IncreaseSD(absorb_sd);
				MSG_CG_STATUS_CHANGE_SD_CMD cmd_msg;
				cmd_msg.m_dwSD = pAttacker->GetSD();
				pAttacker->SendPacket(&cmd_msg, sizeof(cmd_msg));
			}
#else
			pAttacker->IncreaseSD(absorb_sd);
			MSG_CG_STATUS_CHANGE_SD_CMD cmd_msg;
			cmd_msg.m_dwSD = pAttacker->GetSD();
			pAttacker->SendPacket(&cmd_msg, sizeof(cmd_msg));
#endif //(_NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE)

        }

        const DWORD absorb_hp = damage_arg->damage_ * (pAttacker->GetAttr().GetAttackDamageAbsorbHPRatio()) / 100;
        if (absorb_hp > 0) {
#ifdef _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE
			if(pAttacker->GetHP() != 0)
			{
				pAttacker->IncreaseHP(absorb_hp);
				MSG_CG_STATUS_CHANGE_HP_BRD cmd_msg;
				cmd_msg.m_dwObjectKey = pAttacker->GetObjectKey();
				cmd_msg.m_dwTargetHP = pAttacker->GetHP();
				cmd_msg.m_dwReserveHP = pAttacker->GetReserveHP();
				pAttacker->SendPacket(&cmd_msg, sizeof(cmd_msg));
			}
#else
			pAttacker->IncreaseHP(absorb_hp);
			MSG_CG_STATUS_CHANGE_HP_BRD cmd_msg;
			cmd_msg.m_dwObjectKey = pAttacker->GetObjectKey();
			cmd_msg.m_dwTargetHP = pAttacker->GetHP();
			cmd_msg.m_dwReserveHP = pAttacker->GetReserveHP();
			pAttacker->SendPacket(&cmd_msg, sizeof(cmd_msg));
#endif //(_NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE)
        }
        
    }
#endif // _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY

    // 매직쉴드와 SD에 의해 감소되기 전 피해량
    //어시스트포인트는 SD와 매직쉴드에 준 피해도 포함시킨다
    damage_arg->damage_first_ = damage_arg->damage_;

    damage_arg->damage_ = _ApplyMagicShield(damage_arg->damage_);
    // Shield System
    damage_arg->damage_ = _ApplyShieldPoint(pAttacker, damage_arg->damage_, damage_arg->sd_apply_);

#ifdef _NA_0_20100819_AGGRO_RENEWAL
    // 캐릭터가 NPC이면 최종 데미지를 BattlePoint로 기록
    if (this->IsEqualObjectKind(NPC_OBJECT) && IsAlive())
    {
        NPC* npc = static_cast<NPC*>(this);
        npc->AddBattlePoint(pAttacker, BATTLE_POINT_DAMAGE, damage_arg->damage_);
    }
#endif //_NA_0_20100819_AGGRO_RENEWAL

#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    //캐릭터가 플레이어면 어시스트포인트를 증가시킨다
    //어시스트포인트는 전장에 한해서만 추가될 것이다.
    else if (this->IsEqualObjectKind(PLAYER_OBJECT) && IsAlive())
    {
        Player* player = static_cast<Player*>(this);
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        ChaosZoneType chaoszone_type = player->GetChaosZoneType();
        if ( chaoszone_type == kBattleGround || chaoszone_type == eCHAOSZONE_TYPE_GOLDRUSH) {
            player->GetPlayerAssistMeter()->AddAssistPoint(pAttacker, damage_arg->damage_first_);
        }
#else
        if ( player->IsThereBattleGroundZone()) {
            player->GetPlayerAssistMeter()->AddAssistPoint(pAttacker, damage_for_assist_point);
        }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    }
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

    damage_arg->damage_ = DecreaseHP(damage_arg->damage_, damage_arg->limit_hp_);

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
    _WriteMondschienLog(pAttacker, damage_arg->skill_code_, damage_arg->damage_first_);
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
    _WriteBattleMonitorLog(pAttacker, damage_arg->skill_code_, damage_arg->damage_first_);
#endif //_NA_008706_20160229_TEXTLOG_PK

#ifdef _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB
    // 데미지 적용 정리 : 상태무적(1301) 적용 -> 데미지 반사(220...) -> 추가(보너스) 데미지(231)
    //                    -> 데미지 감소 옵션(전체) -> 데미지 감소 옵션(pvp) -> 데미지 감소 옵션(pvall)
    //                    -> 매직 쉴드 적용 -> SP 적용 -> HP 감소 
    //                    -> HP데미지 복구(피격자의 hp.mp) -> HP데미지 복구(공격자의 hp.mp)

    if (damage_arg->option_.is_mirror_ == false)
    {
        // 기존 mp,sp의 복구가 초기 데미지에서 -> HP감소 데미지로.
        // hp에데미지를 입은 이후 다시 eCHAR_STATE_ABSORB(115)에 의해 복구 한다.
        // 죽어 버렸다면 복구 불가.
        m_pStatusManager->DamagedAbsorb(damage_arg->damage_);
    }
#endif

    Attacked();
    GetStatusManager()->UpdateExpiredTime(eCHAR_STATE_BATTLE, STATE_BATTLE_TIME);

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameZone* game_zone = GetGameZonePtr();
    if (game_zone)
    {
        GameBattleResult* battle_result = game_zone->GetGameBattleResult();
        if (battle_result != NULL)
        {
            battle_result->DoRecord(this, BATTLE_RECORD_DAMAGE, damage_arg->damage_);
#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
            WriteMaxDamageLog(pAttacker, damage_arg);
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
#ifdef _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
            if (pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
                battle_result->DoRecord(pAttacker, BATTLE_RECORD_ATTACK, damage_arg->damage_);
            else if (pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
                battle_result->DoRecord(pAttacker->GetPlayerOwner(), BATTLE_RECORD_ATTACK, damage_arg->damage_);
#else
            battle_result->DoRecord(pAttacker, BATTLE_RECORD_ATTACK, damage_arg->damage_);
#endif // _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
        }
    }

#ifdef _NA_008321_20150601_BATTLE_INFO
    //개발서버나 사내서버일 경우만
    eSERVER_MODE server_mode = g_pGameServer->GetMode();
    if ((server_mode == eSERVER_DEV || server_mode == eSERVER_INTRAOFFICE) &&
         (pAttacker->IsEqualObjectKind(PLAYER_OBJECT) && this->IsEqualObjectKind(PLAYER_OBJECT)))
    {
        MSG_CG_BATTLE_DETAIL_INFORMATION_CMD cmd_packet;
        cmd_packet.damage = damage_arg->damage_first_;
        cmd_packet.skill_code = damage_arg->skill_code_;
        strcpy(cmd_packet.attacker_name, pAttacker->GetCharName());
        strcpy(cmd_packet.target_name, this->GetCharName());

        this->SendPacket(&cmd_packet, sizeof(cmd_packet));
        pAttacker->SendPacket(&cmd_packet, sizeof(cmd_packet));
        SUNLOG(eCRITICAL_LOG,"[DamageNotify] [%s] --> [%s] [Damage:%d(%d)]", cmd_packet.attacker_name, 
               cmd_packet.target_name, cmd_packet.damage, cmd_packet.skill_code);
    }
#endif //_NA_008321_20150601_BATTLE_INFO
}


// 데미지 감소 옵션은 순차적으로 이루어 진다.
// AllvAll -> PvP -> PvAll
DAMAGETYPE Character::_ApplyOptionDecDamage(Character* pAttacker, DAMAGETYPE damage,
                                            eATTACK_RESIST attackKind)
{
    __UNUSED(attackKind);
    Attributes& rAtt = GetAttr();
    int nResistValue  = rAtt.GetDecDamage();
    int nResistRatio = rAtt.GetRatioDecDamage();

#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    if (pAttacker && pAttacker->IsEqualObjectKind(PLAYER_OBJECT) == FALSE)
    {
        nResistValue += rAtt.GetDecreaseDamageNPC();
        nResistRatio += rAtt.GetRatioDecreaseDamageNPC();
    }
    else if (pAttacker && pAttacker->IsEqualObjectKind(PLAYER_OBJECT) == TRUE)
    {
        Player* attacker = static_cast<Player*>(pAttacker);
        eCHAR_TYPE char_class = attacker->GetCharType();

        switch(char_class)
        {
        case eCHAR_BERSERKER:
            nResistValue += rAtt.GetDecreaseDamageBerserker();
            nResistRatio += rAtt.GetRatioDecreaseDamageBerserker();
            break;
        case eCHAR_DRAGON:
            nResistValue += rAtt.GetDecreaseDamageDragonKnight();
            nResistRatio += rAtt.GetRatioDecreaseDamageDragonKnight();
            break;
        case eCHAR_SHADOW:
            nResistValue += rAtt.GetDecreaseDamageShadow();
            nResistRatio += rAtt.GetRatioDecreaseDamageShadow();
            break;
        case eCHAR_VALKYRIE:
            nResistValue += rAtt.GetDecreaseDamageValkyrie();
            nResistRatio += rAtt.GetRatioDecreaseDamageValkyrie();
            break;
        case eCHAR_ELEMENTALIST:
            nResistValue += rAtt.GetDecreaseDamageElementalist();
            nResistRatio += rAtt.GetRatioDecreaseDamageElementalist();
            break;
        case eCHAR_MYSTIC:
            nResistValue += rAtt.GetDecreaseDamageMystic();
            nResistRatio += rAtt.GetRatioDecreaseDamageMystic();
            break;
        case eCHAR_HELLROID:
            nResistValue += rAtt.GetDecreaseDamageHellroid();
            nResistRatio += rAtt.GetRatioDecreaseDamageHellroid();
            break;
        case eCHAR_WITCHBLADE:
            nResistValue += rAtt.GetDecreaseDamageWitchBlade();
            nResistRatio += rAtt.GetRatioDecreaseDamageWitchBlade();
            break;
        }
    }
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4

    damage = CalcDecreaseDamage(damage, nResistValue, nResistRatio); 	// 전체 데감

    if(pAttacker && pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
    {
        // PVP 데감 _NA_001231_20081216_ADD_OPTION_KIND
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        //DoT 효과는 PvP데미지감소효과를 받지 않도록 한다
        if(this->IsEqualObjectKind(PLAYER_OBJECT) &&
            attackKind != eATTACK_RESIST_SKILL_DOT_DAMAGE)
        {
            nResistValue  = rAtt.GetDecreasePVPDamage();
            nResistRatio = rAtt.GetRatioDecreasePVPDamage();

            damage = CalcDecreaseDamage(damage, nResistValue, nResistRatio);
        }
#else
        if(this->IsEqualObjectKind(PLAYER_OBJECT))
        {
            nResistValue  = rAtt.GetDecreasePVPDamage();
            nResistRatio = rAtt.GetRatioDecreasePVPDamage();

            damage = CalcDecreaseDamage(damage, nResistValue, nResistRatio);
        }
#endif //_NA_008486_20150914_TOTAL_BALANCE
    }

    return damage;
}

// 쉴드 시스템
//_NA_0_20100211_SD_EFFECT_MODIFY
DAMAGETYPE 
Character::_ApplyShieldPoint(Character* pAttacker, DAMAGETYPE damage, eSD_APPLY SDApply)
{
    PAIR_DAMAGE retVal(damage, 0);

    if (eSD_APPLY_DO == SDApply &&
        IsPCObject(this) &&
        NULL != pAttacker && 
        IsPCObject(pAttacker))
    {
        DAMAGETYPE& damegeHP = retVal.damageHP;
        DAMAGETYPE& damegeSD = retVal.damageSD;

        damegeSD = DAMAGETYPE(damegeHP * SD_ABSORB_RATIO);
        damegeSD = DecreaseSD(damegeSD); // 리턴값 : 실제 차감된 데미지
        damegeHP -= damegeSD;

        //... 
        //_NA_0_20100211_SD_EFFECT_MODIFY
        // 기획에서 원하는건 SD 감소하는 플레이어의 감소연출이다. 
        // 공격자와 주위 유저들에게는 연출 되지 않는다.
        // 데미지쪽 모든 패킷에 추가하기엔 패킷 낭비다.
        // 허나 이펙트에 따라 연출(2타스킬,두개의 데미지어빌리티)이 
        // 다른 스킬의 처리시 클라에서는 한번에 연출에 대한 정보를 모두 
        // 받길 원한다.
        if (damegeSD != 0 &&
            IsEqualObjectKind(PLAYER_OBJECT))
        {
            Player* pPlayer = (Player*)this;
            pPlayer->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
        }
    }
    //_NA_0_20100211_SD_EFFECT_MODIFY
    return retVal.damageHP;
}

BOOL Character::CanAttack() const
{
	if(IsDead())
	{
		return false;
	}

	// 특정 상태에 걸렸을 경우 공격 불가
	return m_pStatusManager->CanAttack();
}

BOOL Character::CanMove(eCHAR_STATE_TYPE eException)
{
	// 특정 상태에 걸렸을 경우 이동 불가
	return m_pStatusManager->CanMove(eException);
}


BOOL Character::CanBeAttacked() const
{
	if (IsDead())
	{
		return false;
	}

	// 특정 상태에 걸렸을 경우 공격을 받지 않음
	return m_pStatusManager->CanBeAttacked();
}


float Character::GetDist(Character* pTargetCharacter) const
{
	// 타겟과의 거리 구하기
	WzVector vCurPos, vTargetPos;
	GetPos(&vCurPos);
	pTargetCharacter->GetPos(&vTargetPos);

	return VectorLength(&(vTargetPos - vCurPos));
}


float Character::GetDist(const WzVector& vTargetPos) const
{
	// 타겟과의 거리 구하기
	WzVector vCurPos;
	GetPos(&vCurPos);
	
	return VectorLength(&(vTargetPos - vCurPos));
}


float Character::GetDist2(Character* pTargetCharacter) const
{
	// 타겟과의 거리 구하기
	WzVector vCurPos, vTargetPos;
	GetPos(&vCurPos);
	pTargetCharacter->GetPos(&vTargetPos);

	return VectorLength2(&(vTargetPos - vCurPos));
}


float Character::GetDist2(const WzVector& vTargetPos) const
{
	// 타겟과의 거리 구하기
	WzVector vCurPos;
	GetPos(&vCurPos);

	return VectorLength2(&(vTargetPos - vCurPos));
}


float Character::GetHeightDiff(const WzVector& vTargetPos) const
{
	// 타겟과의 높이차 구하기
	WzVector vCurPos;	GetPos(&vCurPos);

	return (vTargetPos.z - vCurPos.z);
}


BOOL Character::IsStandingOnJumpTile()
{
	GameField* const pGameField = GetField();
	if(!pGameField)
		return false;

	// 현재 서있는 타일 속성 얻어내기
	const int iTileNo = m_pPathExplorer->GetTile();
	if(iTileNo < 0)
		return false;

	const WORD tile_attribute = pGameField->GetMovingPlanner()->GetAttribute(iTileNo);

	//	점프 속성이 포함되어 있는지 검사
	return !!(tile_attribute & PTA_ONLY_JUMP);
}


BOOL Character::CheckArea(const WzVector& vTargetPos, const WzVector& vMainTargetPos,
    BYTE byRangeForm, float fArea)
{
	WzVector vAttackerPos;
	GetPos(&vAttackerPos);

	if(byRangeForm == SRF_FOWARD_120)
	{
		// 120도
		WzVector diffVec = vMainTargetPos - vAttackerPos;
		if(!Math_IsPositionInFanshapedArea(&vTargetPos, &vAttackerPos, &diffVec, 120))
		{
			return false;
		}
	}
	else if(byRangeForm == SRF_FOWARD_160)
	{
		// 160도
		WzVector diffVec = vMainTargetPos - vAttackerPos;
		if(!Math_IsPositionInFanshapedArea(&vTargetPos, &vAttackerPos, &diffVec, 160))
		{
			return false;
		}
	}
	else if(byRangeForm == SRF_PIERCE)
	{
		// 관통
		if(Math_IsPositionInOBB(&vAttackerPos, &vMainTargetPos, fArea*2, &vTargetPos))
			return true;
		else
			return false;
	}

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

// 	if(!CheckHeightRange(vTargetPos, fArea)) // _NA001385_20090924_DOMINATION_ETC 이전 루틴에서 이미 체크 했다. (삭제)
// 		return false;

	// SkillArea를 체크한다.(MainTarget을 중심으로 하는 원영역) ... 관통일 경우는 체크하지 않는다.
	if(fArea && Math_Vector2DLength(vMainTargetPos, vTargetPos) >= fArea + stAICommonInfo.m_fRANGE_TOLERANCE)
	{
		return false;
	}

	return true;
}

BOOL Character::CheckHeightRange(Character* pTarget, const WzVector& vTargetPos, float fBaseRange)
{
    float fHeightDiff = fabs(GetHeightDiff(vTargetPos));

    //_NA001385_20090924_DOMINATION_FINDPATH_ATTACK
    GameZone* pZone = GetGameZonePtr();
    if (pZone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[Character::CheckHeightRange] GetGameZonePtr() returned NULL!");
        return false;
    }
    if (NULL != pTarget &&
        eZONETYPE_DOMINATION_FIELD == pZone->GetZoneType() && 
        false != this->IsEqualObjectKind(PLAYER_OBJECT) &&
        false != pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
    {
        // 지역 점령전 MAPNCP의 경우 높이 체크를 한다.
        if(fHeightDiff >= MAX_ATTACK_HEIGHT_IN_DOMINATION) 
            return false;
    }

	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

	// 높이 체크(사거리의 50% 정도를 적용하며 사거리가 짧을 경우에는 허용해준다.)
	float fZAttackRange = max(4, fBaseRange * stAICommonInfo.m_fATTACK_RANGE_HEIGHT_RATIO);
	if(fHeightDiff >= fZAttackRange) 
		return false;

	return true;
}

//_NA001385_20090924_DOMINATION_FINDPATH_ATTACK
BOOL Character::CheckAttackFindPath(const WzVector& vMainTargetPos)
{
    // 지역점령전에서는 벽 안쪽 플레이어는 공격 할 수 없다.
    // 벽(못가는 타일) 위의 NPC는 공격 할수 있다.

    // 해당 경우는 3가지가 있다. 
    // 1. 적군이 보이지 않는 경우. (벽 안쪽이나 성문 안쪽)
    // 2. 적군은 보이지만 적군 위치로는 갈수 없는 경우. (절벽의 반대편)
    // 3. 캐스팅 시작시 보이지만, 완료시 보이지 않는 경우. (성문이 열렸다, 닫힌경우.)

    // 이후 기획 변경. : 공격은 가능 하나 타겟팅은 되지 않는다. (클라이언트에서 처리함.)
    
    // 이후 언젠가.. 
    // 신규 타일 속성이나, 방식의 변경을 통해 공격 판정이나 취소 miss 에 대한 수정이 필요.
    // (현재 문 안쪽을 공격 하는 방식이 버그 같지만, 오래 되어 정당한 플레이 방식임.)

    return true;
#if SUN_CODE_BACKUP
    GameZone* pZone = GetGameZonePtr();
  #if SUN_CODE_RENEWAL
    const sPOLICY_ZONE& ZonePolicy = pZone->Policyment;
    if ((ZonePolicy.FIELD & ZonePolicy.ENABLE_DOMINATION_CONTROL) == 0) {
        return true;
    }
  #else
    if (eZONETYPE_DOMINATION_FIELD != pZone->GetZoneType())
        return true;
  #endif

    GameField* const pGameField = GetField();
    const BOOL bPath = pGameField->CorrectMovableDestPath(this, &vMainTargetPos);
    return bPath;
#endif //
}

BOOL Character::CheckAttackRange(Character* pTarget, WzVector &vTargetPos)
{
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

	WzVector vAttackerPos;	GetPos(&vAttackerPos);
	float fAttackRange = GetAttackRange() + stAICommonInfo.m_fRANGE_TOLERANCE;

	if(!CheckHeightRange(pTarget, vTargetPos, fAttackRange))
		return false;

	if(pTarget)
    {
        if (false != this->IsEqualObjectKind(PLAYER_OBJECT) &&
            false == pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
        {
            if (false == CheckAttackFindPath(vTargetPos))
                return false;
        }

		fAttackRange += pTarget->GetBodyRadius();
    }

	// 공격 사거리 판별(공격자와 타겟과의 거리)
	const float diff_length = Math_Vector2DLength(vAttackerPos, vTargetPos);
	if(diff_length >= fAttackRange)
	{
		return false;
	}

	return true;
}

// 공격자 기준
BOOL Character::CheckSkillRange(Character* pTarget, const WzVector &vTargetPos, float fSkillRange)
{
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();

	WzVector vAttackerPos;	GetPos(&vAttackerPos);
	fSkillRange += stAICommonInfo.m_fRANGE_TOLERANCE;
	
	// 원거리 스킬인 경우에만 SkillRangeBonus 적용
    if(fSkillRange >= 4.f) {
        float skill_range_ratio_bonus = fSkillRange * (GetAttr().GetRatioSkillRangeBonus()/100.0f);
		fSkillRange += GetSkillRangeBonus() + skill_range_ratio_bonus;
    }

	if(!CheckHeightRange(pTarget, vTargetPos, fSkillRange))
		return false;

	if(pTarget)
    {
        if (false != this->IsEqualObjectKind(PLAYER_OBJECT) &&
            false == pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
        {
            if (false == CheckAttackFindPath(vTargetPos))
                return false;
        }

        fSkillRange += pTarget->GetBodyRadius();
    }

	// 스킬 사거리 판별(공격자와 타겟과의 거리)
	if(fSkillRange && Math_Vector2DLength(vAttackerPos, vTargetPos) >= fSkillRange)
		return false;

	return true;
}

void Character::PeriodicRecover(eCHAR_STATE_TYPE eStateID, int recoverHP, int recoverMP, int iApplicationTime, int iPeriodicTime)
{
	m_pStatusManager->AllocStatus(eStateID, iApplicationTime, iPeriodicTime, recoverHP, recoverMP);
}


//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
void Character::OnRecover(int recoverHP, int recoverMP, int recoverSD, eRECOVER_TYPE Type, Character* pAttacker)
{	
	if(recoverHP > 0)
	{
		IncreaseHP(recoverHP);
	}
	else if(recoverHP < 0)
	{
		DecreaseHP(-recoverHP);
	}

	if(recoverMP > 0)
	{
		IncreaseMP(recoverMP);		
	}
	else if(recoverMP < 0)
	{
		DecreaseMP(-recoverMP);
	}

    if(recoverSD > 0)
    {
        IncreaseSD(recoverSD);		
    }
    else if(recoverSD < 0)
    {
        DecreaseSD(-recoverSD);
    }

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameZone* game_zone = GetGameZonePtr();
    if (game_zone && pAttacker)
    {
        GameBattleResult* battle_result = game_zone->GetGameBattleResult();
        if (battle_result != NULL)
        {
#ifdef _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
            if (pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
                battle_result->DoRecord(pAttacker, BATTLE_RECORD_HEAL, recoverHP);
            else if (pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
                battle_result->DoRecord(pAttacker->GetPlayerOwner(), BATTLE_RECORD_HEAL, recoverHP);
#else
            battle_result->DoRecord(pAttacker, BATTLE_RECORD_HEAL, recoverHP);
#endif // _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
        }
    }
}

BOOL Character::CheckCondition(BYTE byCondition, WORD wParam)
{
	switch(byCondition)
	{
	case 1:
		return CheckCondition_Under_HP(wParam);
	case 2:
		return CheckCondition_Under_MP(wParam);
	case 3:
		return CheckCondition_Same_HP(wParam);
	case 4:
		return CheckCondition_Same_MP(wParam);
	case 5:
		return IsDead();
	}

	return false;
}

// HP가 특정 퍼센트 이하이면 true를 리턴한다.
BOOL Character::CheckCondition_Under_HP(WORD wPercent)
{
	if(GetMaxHP() == 0)
	{
		ASSERT(! "MaxHP == 0");
		return false;
	}

	if(IsDead())
		return false;

	return GetHP() * 100 / GetMaxHP() <= wPercent;
}

// MP가 특정 퍼센트 이하이면 true를 리턴한다.
BOOL Character::CheckCondition_Under_MP(WORD wPercent)
{
	if(GetMaxMP() == 0)
	{
		ASSERT(! "MaxMP == 0");
		return false;
	}

	return GetMP() * 100 / GetMaxMP() <= wPercent;
}

// HP가 특정 퍼센트와 동일하면 true를 리턴한다.
BOOL Character::CheckCondition_Same_HP(WORD wPercent)
{
	if(GetMaxHP() == 0)
	{
		ASSERT(! "MaxHP == 0");
		return false;
	}

	return GetHP() * 100 / GetMaxHP() == wPercent;
}

// MP가 특정 퍼센트와 동일하면 true를 리턴한다.
BOOL Character::CheckCondition_Same_MP(WORD wPercent)
{
	if(GetMaxMP() == 0)
	{
		ASSERT(! "MaxMP == 0");
		return false;
	}
	return GetMP() * 100 / GetMaxMP() == wPercent;
}

RC::eSKILL_RESULT
Character::_CanUseSkill(SLOTCODE SkillCode)
{
	SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(SkillCode);

	if(!pBaseSkillInfo)	return RC::RC_SKILL_BASEINFO_NOTEXIST;
	if(!CanAttack())		return RC::RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY;		// 공격할 수 없는 상황이라면

	if(!m_pStatusManager->Status.IsPassConstraint(this, pBaseSkillInfo))
		return RC::RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY;

	// 필요 MP, HP가 있는지 체크한다.
	if(false == _ChkNeedHP(pBaseSkillInfo->m_wHPSpend) || 
		false == _ChkNeedMP(pBaseSkillInfo->m_wMPSpend))
		return RC::RC_SKILL_HPMP_RUN_SHORT;

	// 봉인 상태에 걸렸는지 체크
	RC::eSKILL_RESULT rcResult = m_pStatusManager->CanUseSkill();
	if(rcResult != RC::RC_SKILL_SUCCESS)
		return rcResult;

	return RC::RC_SKILL_SUCCESS;
}

RC::eSKILL_RESULT Character::CanUseSkill(SLOTCODE SkillCode, BOOL bCoolTimerReset)
{
	if(false == CheckCoolTimer(SkillCode))
		return RC::RC_SKILL_COOLTIME_ERROR;	

	return _CanUseSkill(SkillCode);
}

RC::eSKILL_RESULT Character::CanUseSkillItem(SLOTCODE SkillCode)
{
	return _CanUseSkill(SkillCode);
}

BOOL Character::_ChkNeedMP(WORD wMPSpend)
{
	int iMPSpend = (int)(wMPSpend * (1 + GetMPSpendIncRatio()) + GetMPSpendIncValue());
	iMPSpend = max(0, iMPSpend);

	return GetMP() < (DWORD)iMPSpend ? false:true; 
}

BOOL Character::_ChkNeedHP(WORD wHPSpend)
{
	return GetHP() <= wHPSpend ? false:true;
}

// CHANGES, f100827.4L, r100827.4L, SCCharacter interfaces related to a cool-timer
void Character::SetSkillCoolTime(SLOTCODE skill_code, WORD cool_time_margin)
{
    if (skill_code == 0) {
        return;
    };
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* skill_info = NULL;
    if (const ROOT_SKILLINFO* root_skill_info = skill_parser->GetInfo(skill_code))
    {
        if (root_skill_info->IsStyle()) {
            return;
        }
        skill_info = static_cast<const SkillScriptInfo*>(root_skill_info);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find a skill (%d)|"), skill_code);
        return;
    }
    assert(skill_info->m_bySkillType != SKILL_TYPE_PASSIVE);
    //
    util::Timer* timer = GetSkillCoolTimer(skill_code);
    if (timer) {
        return;
    }
    timer = CoolTimerFactory::Instance()->Alloc();
    DWORD interval = skill_info->m_dwCoolTime > cool_time_margin
                   ?    skill_info->m_dwCoolTime - cool_time_margin
                   :    skill_info->m_dwCoolTime;
    timer->SetTimer(interval);
    timer->InitCoolTime();

    skill_cool_time_table_[skill_code] = timer;
}

void Character::FreeSkillCoolTime(SLOTCODE skill_code)
{
    COOL_TIME_TABLE::iterator found = skill_cool_time_table_.find(skill_code);
    if (found == skill_cool_time_table_.end()) {
        return;
    }
    util::Timer* timer = found->second;
    CoolTimerFactory::Instance()->Free(timer);
    skill_cool_time_table_.erase(found);
}

void Character::FreeAllSkillCoolTime(BOOL pool_use)
{
    FOREACH_CONTAINER(const COOL_TIME_TABLE::value_type& node, skill_cool_time_table_,
                      COOL_TIME_TABLE)
    {
        util::Timer* timer = node.second;
        CoolTimerFactory::Instance()->Free( timer );
    };
    skill_cool_time_table_.clear();
}

void Character::InitializeSkillCoolTime()
{
    FOREACH_CONTAINER(const COOL_TIME_TABLE::value_type& node, skill_cool_time_table_,
                      COOL_TIME_TABLE)
    {
        util::Timer* timer = node.second;
        timer->InitCoolTime();
    }
}

util::Timer* Character::GetSkillCoolTimer(SLOTCODE skill_code)
{
    COOL_TIME_TABLE::const_iterator found = skill_cool_time_table_.find(skill_code);
    if (found == skill_cool_time_table_.end()) {
        return NULL;
    }
    util::Timer* timer = found->second;
    return timer;
}

// 쿨타임을 체크한다.
bool Character::CheckCoolTimer(SLOTCODE skill_code) const
{
    COOL_TIME_TABLE::const_iterator found = skill_cool_time_table_.find(skill_code);
    if (found == skill_cool_time_table_.end()) {
        return true;
    };
    util::Timer* timer = found->second;
    if (timer->IsExpired(false) == false) {
        return false;
    }
    return true;
}

bool Character::CoolTimerReset(SLOTCODE skill_code)
{
    COOL_TIME_TABLE::const_iterator found = skill_cool_time_table_.find(skill_code);
    if (found == skill_cool_time_table_.end()) {
        return false; //RC::RC_SKILL_COOLTIME_ERROR
    };
    util::Timer* timer = found->second;
    // 스킬 쿨타임 증감 적용
    const int base_cool_time = timer->GetBaseIntervalTime();
    int calculate_cool_time = base_cool_time;
    if(calculate_cool_time)
    {
        const Attributes& attributes = this->GetAttr();
        int bonus_cool_time = attributes.GetItemBonusSkillCoolTime(); // CHANGES: f110603.2L
        float bonus_cool_time_ratio = attributes.GetRatioBonusSkillCoolTime() / 100.f;
        bonus_cool_time_ratio = max(-1.0f, bonus_cool_time_ratio);

        calculate_cool_time = calculate_cool_time + bonus_cool_time;
        calculate_cool_time = max(0, calculate_cool_time);
        calculate_cool_time = (int)(calculate_cool_time * (1 + bonus_cool_time_ratio));

        int bonus_time = calculate_cool_time - base_cool_time;
        timer->SetBonusIntervalTime(bonus_time);
    }

    timer->Reset();

    return true;
}

BOOL Character::finite(WzVector& wvPos)
{
	if(!_finite(wvPos.x) || !_finite(wvPos.y) || !_finite(wvPos.z))
		return false;

	return true;
}

void Character::SendAttrChange(eATTR_TYPE attrType, DWORD dwValue)
{
	if(Policyment.FIELD & Policyment.PCC_IN_DEF_FIELD)
	{
		MSG_CG_STATUS_ATTR_CHANGE_BRD sendMsg;
		sendMsg.m_dwObjKey = GetObjectKey();
		sendMsg.m_AttrType = (BYTE)attrType;
		sendMsg.m_dwValue = dwValue;

		SendPacketAround(&sendMsg, sizeof(sendMsg));
	};
}

//#if !defined(_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM)
BOOL Character::IsCoincidentWithTargetType(Character* pTarget, eSKILL_TARGET_TYPE eTargetType)
{
	switch(eTargetType)
	{
	case SKILL_TARGET_ME:
	case SKILL_TARGET_REACHABLE_ME:
		{
			if(GetObjectKey() != pTarget->GetObjectKey())	return false;
		}
		break;
	case SKILL_TARGET_FRIEND:	
	case SKILL_TARGET_REACHABLE_FRIEND:
		{
			if(IsFriend(pTarget) != USER_RELATION_FRIEND)	return false;
		}
		break;
	case SKILL_TARGET_ENEMY:	
	case SKILL_TARGET_REACHABLE_ENEMY:	
	case SKILL_TARGET_AREA:
		{
			if(IsFriend(pTarget) != USER_RELATION_ENEMY)	return false;
		}
		break;
	case SKILL_TARGET_FRIEND_CORPSE:
		{
			if(IsAlive())									return false;
			if(IsFriend(pTarget) != USER_RELATION_FRIEND)	return false;
		}
		break;
	}

	return true;
}
//#endif //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM


BOOL Character::IsStatus(eCHAR_STATE_TYPE eStateID)
{
	if(m_pStatusManager->FindStatus(eStateID))	
		return true;
	else
		return false;
}

// TODO, waverix, reserve delete that this routine is a temporay save for a summoned.
BOOL Character::ModifyPosition(	BOOL bIsPlayer, WzVector &vCurPos, WzVector &vDestPos, WzVector &vResultCurPos, WzVector &vResultDestPos, SLOTCODE SkillCode)
{
	// 현재 좌표가 유효한지 체크
	if(!_finite(vCurPos.x) || !_finite(vCurPos.y) || !_finite(vCurPos.z))
	{
		if(bIsPlayer)
			SECURELOG(SECURELOG_PLAYERINFO((Player*)this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
				_T("[ModifyPosition] CurPos is Invalid! x[%f] y[%f] z[%f]"), vCurPos.x, vCurPos.y, vCurPos.z);
		return false;
	}

	// 목적지 좌표가 유효한지 체크
	if(!_finite(vDestPos.x) || !_finite(vDestPos.y) || !_finite(vDestPos.z))
	{
		if(bIsPlayer)
			SECURELOG(SECURELOG_PLAYERINFO((Player*)this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
				_T("[ModifyPosition] DestPos is Invalid! x[%f] y[%f] z[%f]"), vDestPos.x, vDestPos.y, vDestPos.z);
		return false;
	}

	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	WzVector vServerPos, vPosAfterThrust;	GetPos(&vServerPos);
	float fCurPosDiff = Math_Vector2DLength(vServerPos, vCurPos);

	// 목적지 좌표가 0인 경우
	if(vDestPos.x == 0 && vDestPos.y == 0 && vDestPos.z == 0)
	{
		if(bIsPlayer)
			SECURELOG(SECURELOG_PLAYERINFO((Player*)this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
				_T("[ModifyPosition] vDestPos is 0!"));

		vResultCurPos = vServerPos;
		vResultDestPos = vServerPos;
	}
	// 서버가 알고 있는 플레이어의 좌표와 클라이언트가 보낸 좌표가 10m 이상 차이나는 경우 서버를 기준으로 한다.
	else if(fCurPosDiff >= stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE)
	{
		if(bIsPlayer)
			SECURELOG(SECURELOG_PLAYERINFO((Player*)this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_NOT_SYNC_POSITION),
				_T("[ModifyPosition] The Server CurPos is different from the Client CurPos! CurPosDiff[%f]"), fCurPosDiff);
		vResultCurPos = vServerPos;
		vResultDestPos = vDestPos;
	}
	// 클라를 믿고 플레이어의 cur 위치를 맞춰줌
	else
	{
		vResultCurPos = vCurPos;
		vResultDestPos = vDestPos;

		ROOT_SKILLINFO* pRootInfo = SkillInfoParser::Instance()->GetInfo(SkillCode);

		if(pRootInfo && pRootInfo->IsSkill())
		{
			SkillScriptInfo* pBaseSkillInfo = (SkillScriptInfo *)pRootInfo;

			if(pBaseSkillInfo->IsNonStopSkill())
			{
				// 플레이어는 멈추지않고 계속 지나간다.
			}
			else
			{
				// 플레이어 위치를 클라가 보내준 어택 시작점으로 옮긴다.
				SetPos(&vCurPos);
				if(bIsPlayer)
				{
					MovementChecker* pCHECKER = ((Player*)this)->GetMovementChecker();
					pCHECKER->GetServerPathExpUpdate()->SetupLatestState(pCHECKER, .0f, 0, vCurPos, vCurPos);
				}
			}
		}
		else
		{
			SetPos(&vCurPos);
			if(bIsPlayer)
			{
				MovementChecker* pCHECKER = ((Player*)this)->GetMovementChecker();
				pCHECKER->GetServerPathExpUpdate()->SetupLatestState(pCHECKER, .0f, 0, vCurPos, vCurPos);
			}
		}
	}

	// 목적지로 Thrust
	if(vResultCurPos != vResultDestPos)
	{
		WzVector vDiff = vResultDestPos - vResultCurPos;
		float fThrustDist = VectorLength(&vDiff);

		// Thrust 거리가 너무 길면 최대거리(10m)로 보정한다.
		SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();
		if(fThrustDist > stServerOptionInfo.m_byMOVE_SYNC_THRUST_DISTANCE)
		{
			if(bIsPlayer)
				SECURELOG(SECURELOG_PLAYERINFO((Player*)this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE),
					_T("[ModifyPosition] Skill or Normal Attack fThrustDist[%5.2f] is long!"), fThrustDist);
			fThrustDist = (float)(stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE - 1);

			WzVector vNormal;	VectorNormalize(&vNormal, &vDiff);	vNormal.z = 0;
			vResultDestPos = vResultCurPos + vNormal* fThrustDist;
		}

		ExecuteThrust(false, &vDiff, vPosAfterThrust, fThrustDist);
	}

	return true;
}

// CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
bool Character::ModifyPosition(SkillInfo* preparing_skill_info_data)
{
    // for safe-control. if you want modify a SkillInfo, do access to preparing_skill_info_data.
    const SkillInfo& skill_info_data = *preparing_skill_info_data;
    Player* const player_for_logging = skill_info_data.character_->IsEqualObjectKind(PLAYER_OBJECT)
                                     ? static_cast<Player*>(this) : NULL;
    //
    WzVector cur_pos = skill_info_data.cur_pos_;
    const WzVector dest_pos = skill_info_data.dest_pos_;
    // 현재 좌표가 유효한지 체크
    if (WzVector_FiniteCheck(&cur_pos) == false)
    {
        if (player_for_logging) {
            SECURELOG(SECURELOG_PLAYERINFO(player_for_logging, \
                      eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
                      _T("["__FUNCTION__"]|Msg=a current position is invalid ")
                      _T("x:y:z={%4.2f,%4.2f,%4.2f}|"),
                      cur_pos.x, cur_pos.y, cur_pos.z);
        }
        return false;
    };
    // 목적지 좌표가 유효한지 체크
    if (WzVector_FiniteCheck(&dest_pos) == false)
    {
        if (player_for_logging) {
            SECURELOG(SECURELOG_PLAYERINFO(player_for_logging, \
                      eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
                      _T("["__FUNCTION__"]|Msg=a destination position is invalid ")
                      _T("x:y:z={%4.2f,%4.2f,%4.2f}|"),
                      dest_pos.x, dest_pos.y, dest_pos.z);
        }
        return false;
    };
    //
    const SERVER_COMMON_OPTION& server_option = ServerOptionParserEx::Instance()->GetServerOption();
    //
    const WzVector& cur_pos_by_server = *GetPosPtr();
    float difference_between_client_and_server = Math_Vector2DLength(cur_pos_by_server, cur_pos);
    //
    // 목적지 좌표가 0인 경우
    if (dest_pos.x == 0.0f && dest_pos.y == 0.0f && dest_pos.z == 0.0f)
    {
        if (player_for_logging) {
            SECURELOG(SECURELOG_PLAYERINFO(player_for_logging, \
                      eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
                      _T("["__FUNCTION__"]|Msg=a destination position is empty|Action=modified|"));
        }
        preparing_skill_info_data->cur_pos_ = cur_pos_by_server;
        preparing_skill_info_data->dest_pos_ = cur_pos_by_server;
        // updated 'cur_pos', 'dest_pos'
    }
    // 서버가 알고 있는 플레이어의 좌표와 클라이언트가 보낸 좌표가 10m 이상 차이나는 경우 서버를 기준으로 한다.
    else if (difference_between_client_and_server >= server_option.m_byMOVE_SYNC_FAIL_DISTANCE)
    {
        /*if (player_for_logging) {
            SECURELOG(SECURELOG_PLAYERINFO(player_for_logging, \
                      eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_NOT_SYNC_POSITION),
                      _T("["__FUNCTION__"]|Msg=there are a differnce of distance ")
                      _T("between client and server|gap=%4.2f|Action=modified|"),
                      difference_between_client_and_server);
        }*/
        preparing_skill_info_data->cur_pos_ = cur_pos_by_server;
        preparing_skill_info_data->dest_pos_ = cur_pos_by_server;
        // updated 'cur_pos', 'dest_pos'
        cur_pos = cur_pos_by_server;
    }
    // 클라를 믿고 플레이어의 cur 위치를 맞춰줌
    else
    {
        preparing_skill_info_data->cur_pos_ = cur_pos_by_server;
        preparing_skill_info_data->dest_pos_ = cur_pos_by_server;
        // updated 'cur_pos', 'dest_pos'
        if (skill_info_data.root_skill_info_ && skill_info_data.root_skill_info_->IsSkill())
        {
            const SkillScriptInfo* base_skill_info = \
                static_cast<const SkillScriptInfo*>(skill_info_data.root_skill_info_);

            if (base_skill_info->IsNonStopSkill() ||
                base_skill_info->IsAuraSkill())
            {
                // 플레이어는 멈추지않고 계속 지나간다.
            }
            else
            {
                // 플레이어 위치를 클라가 보내준 어택 시작점으로 옮긴다.
                SetPos(&cur_pos);
                if (player_for_logging) {
                    // TODO : need more thinking... currently (10083), this operation is meaningless.
                    MovementChecker* movement = player_for_logging->GetMovementChecker();
                    movement->GetServerPathExpUpdate()->SetupLatestState(movement, .0f, 0,
                        preparing_skill_info_data->cur_pos_, preparing_skill_info_data->cur_pos_);
                }
            }
        }
        else
        {
            SetPos(&cur_pos);
            if (player_for_logging) {
                // TODO : need more thinking... currently (10083), this operation is meaningless.
                MovementChecker* movement = player_for_logging->GetMovementChecker();
                    movement->GetServerPathExpUpdate()->SetupLatestState(movement, .0f, 0,
                        preparing_skill_info_data->cur_pos_, preparing_skill_info_data->cur_pos_);
            }
        } //
    }

    // 목적지로 Thrust
    // 이 로직은 더이상 사용하지 않는다
    // dest_pos의 용도변경 --> 돌진스킬이동처리 다른곳에서 처리하도록 변경됨
    // 의도치 않은 시작위치 보정의 역할만 진행 -> 상단 코드에서 시작위치 보정작업 진행
    // 결국 필요없는 코드
    /*
    if (cur_pos != dest_pos)
    {
        WzVector diff = dest_pos;
        ;        diff -= cur_pos;
        float thrust_distance = VectorLength(&diff);

        // Thrust 거리가 너무 길면 최대거리(10m)로 보정한다.
        //SERVER_COMMON_OPTION& server_option = ServerOptionParserEx::Instance()->GetServerOption();
        if (thrust_distance > server_option.m_byMOVE_SYNC_THRUST_DISTANCE)
        {
            if (player_for_logging) {
                SECURELOG(SECURELOG_PLAYERINFO(player_for_logging, \
                      eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE),
                      _T("["__FUNCTION__"]|Msg=a value for thrusting distance ")
                      _T("by attack request is long|gap=%4.2f|Action=modified|"),
                      thrust_distance);
            }
            thrust_distance = static_cast<float>(server_option.m_byMOVE_SYNC_FAIL_DISTANCE - 1);
            // CHANGES, f100830.5L, a process, clear z axis, is removed. 3D dimentional thrusting.
            WzVector modified_vector;
            VectorNormalize(&modified_vector, &diff);
            ;        modified_vector *= thrust_distance;
            preparing_skill_info_data->dest_pos_ = cur_pos;
            preparing_skill_info_data->dest_pos_ += modified_vector;
        }

        WzVector post_position;
        ExecuteThrust(false, &diff, post_position, thrust_distance);
    }
    */

    return true;
}

void Character::SetMoveState(eCHAR_MOVE_STATE state)
{
	m_pMoveStateControl->SetMoveState(state);
}

void Character::SetSkillCancel(SLOTCODE SkillCode, BOOL bCancel)
{
	m_pSkillManager->CancelActiveSkill(SkillCode);
}

void Character::CancelAllSkill()
{
    m_pSkillManager->CancelAllSkill();
}

// 새로운 유저가 섹터에 들어오거나, 내가 섹터에 들어갈 때 섹터의 유저들에게 나의 이동정보를 보낸다.
void Character::SendMoveInfo(Player* pEnterPlayer, const Sector& IN rSector)
{
	GameField* pField = GetField();

  // (WAVERIX) (090225) (BUG-FIX) 새로 등장하거나 사라지는 모든 캐릭터들의 이동 상태가
  //불량해지는 문제 수정
  if(false == (pField && IsMoving()))
    return;

	const WzVector* pwzCurPos = m_pPathExplorer->GetPosPtr();
	const WzVector* pwzDestPos = m_pPathExplorer->GetTargetPosPtr();

	if(!(pwzCurPos && pwzDestPos))
	{
		SUNLOG(eCRITICAL_LOG, "[SendMoveInfo] PosPtr is NULL");
		return;
	}

	// 남은 거리가 10m 이상인 경우에만 패킷을 보낸다.
	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	float fDistance = VectorLength2(&(*pwzDestPos - *pwzCurPos));
	if(fDistance > stServerOptionInfo.m_byGRID_MOVEINFO_DISTANCE*stServerOptionInfo.m_byGRID_MOVEINFO_DISTANCE)
	{
		MSGSUB_SYNC_MOVE_BRD msg;
		msg.m_dwObjectKey	= GetObjectKey();
		msg.m_byState		= (BYTE)m_pMoveStateControl->GetMoveState();	// RUN or WALK
		msg.m_ForcedMove	= false;
		msg.m_wvCurPos		= *pwzCurPos;
		msg.m_wvDestPos		= *pwzDestPos;

		pEnterPlayer
			?	pEnterPlayer->SendPacketForSync(&msg)
			:	pField->SendPacketAroundForSync(this, &msg)
			;
	}
}

void Character::DelFightingEnergy(short sCount)
{ 
	m_sFightingEnergyCount = max(0, m_sFightingEnergyCount - sCount);

	const DWORD playerField = Policyment.FIELD & (Policyment.PCC_IN_SSQ_FIELD_PLAYER|Policyment.PCC_IN_DEF_FIELD_PLAYER);
	if(playerField)
	{	//	(WAVERIX)(080731) 개선 여지가 존재한다.
		MSG_CG_STATUS_REMOVE_BRD BrdMsg;
		BrdMsg.m_dwTargetKey = GetObjectKey();
		BrdMsg.m_dwStatusCode = eCHAR_STATE_SPCHARGE;

		switch(playerField)
		{
		case Policyment.PCC_IN_DEF_FIELD_PLAYER:
			{
				for(short i = 0; i < sCount; i++)
					SendPacketAround(&BrdMsg, BrdMsg.GetSize());
			}
			return;
		case Policyment.PCC_IN_SSQ_FIELD_PLAYER:
			{
				for(short i = 0; i < sCount; i++)
					this->SendPacket(&BrdMsg, BrdMsg.GetSize());
			}
			return;
		}
	}
}

void Character::SetGameZone(eZONE_STATE eState, GameZone* pZone, MAPCODE EnterFieldCode, WzID AreaID, WzVector* pwvStartPos)
{ 
	m_eGameZoneState = eState; 
	m_pGameZone = pZone; 
	m_GameFieldCode = EnterFieldCode; 
	m_GameAreaID = AreaID; 
	memset(&m_GameStartPos, 0, sizeof(WzVector));
	if(pwvStartPos) m_GameStartPos =* pwvStartPos; 
}

// 타일의 속성값을 빼온다. 
// 타일을 구해올수 없을경우 (false)
// 타일 인덱스에 변화가 있을시 타일 속성을 새로 구해 온다.
BOOL
Character::GetTileAtt(OUT WORD* wTileAtt) 
{
	GameField* const pGameField = GetField();
	if(!pGameField)
		return false;

    const nsAI::TileEvent* tile_event = 0;
    if(PlanEvent->GetTileEvent(&tile_event) == false)
        return false;

    *wTileAtt = tile_event->Attr;
    return true;
}

void	Character::ChangePolicy(GameZone* const pGameZone)
{
	const eZONETYPE ztTYPE = pGameZone->GetZoneType();
	const BOOLEAN isPlayer = IsEqualObjectKind(PLAYER_OBJECT);

	switch(ztTYPE)
	{
	case eZONETYPE_LOBBY			:
	case eZONETYPE_VILLAGE			:
	case eZONETYPE_CHARSELECT		:
	case eZONETYPE_MISSION			:
	case eZONETYPE_PVP				:
	case eZONETYPE_HUNTING			:
	case eZONETYPE_CHUNTING			:
	case eZONETYPE_FIELD			:
	case eZONETYPE_TUTORIAL_FIELD	:
    case eZONETYPE_DOMINATION_FIELD :
    case eZONETYPE_SPA_FIELD        :   //_NA_008069_20150210_ADD_SPA_SYSTEM
		{
			const DWORD addin_field = (isPlayer ? Policyment.PCC_IN_DEF_FIELD_PLAYER : 0);
			Policyment.SetField(
				(Policyment.PCC_IN_DEF_FIELD
				| Policyment.PCC_STATUS_BRD_TO_ALL
				| Policyment.PCC_BATTLE_RECORD_PRIORITY
				| addin_field
				));
			//Policyment.SetField(
			//	(Policyment.PCC_IN_SSQ_FIELD
			//	| addin_field
			//	));
		}
		break;
	case eZONETYPE_INSTANCE			:
		{
			GameInstanceDungeon* const pGameInstanceDungeon = static_cast<GameInstanceDungeon*>(pGameZone);
			const eINSTANCE_DUNGEON_KIND kindOfDungeon = pGameInstanceDungeon->GetTypeOfIndun();
			BOOST_STATIC_ASSERT(INDUN_KIND_MAX == 7);	// 추가 타입 존재시 하단 내용 추가할 것
			switch(kindOfDungeon)
			{
			case INDUN_KIND_TOP_OF_TRIAL	:
			case INDUN_KIND_100LEVEL_QUEST	:
			case INDUN_KIND_EXT_03			:
			case INDUN_KIND_EXT_04			:
			case INDUN_KIND_MAYA_SCROLL		:
				{
					const DWORD addin_field = (isPlayer ? Policyment.PCC_IN_DEF_FIELD_PLAYER : 0);
					Policyment.SetField(
						(Policyment.PCC_IN_DEF_FIELD
						| Policyment.PCC_STATUS_BRD_TO_ALL
						| Policyment.PCC_BATTLE_RECORD_PRIORITY
						| addin_field
						));
				}
				break;
			case INDUN_KIND_SS_QUEST		:
				{
					const DWORD addin_field = (isPlayer ? Policyment.PCC_IN_SSQ_FIELD_PLAYER : 0);
					Policyment.SetField(
						(Policyment.PCC_IN_SSQ_FIELD
						| addin_field
						));
				}
				break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            case INDUN_KIND_CHAOS_ZONE		:
                {
                    const DWORD addin_field = (isPlayer ? Policyment.PCC_IN_DEF_FIELD_PLAYER : 0);
                    Policyment.SetField(
                        (Policyment.PCC_IN_DEF_FIELD
                        | Policyment.PCC_STATUS_BRD_TO_ALL
                        | Policyment.PCC_BATTLE_RECORD_PRIORITY
                        | addin_field
                        ));

                }break;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            default:
				FlowControl::FCAssert(!"Unexpected IndunType");
				break;
			}
		}
		break;
	default:
		FlowControl::FCAssert(!"Unexpected ZoneType");
		return;
	};

    //----------------------------------------------------------------------------------------------
    // CHANGES: f110704.5L, the character types needed separated configurations.
    // NOTE: f110704.5L, added iff relations to support the custom map unit.
    // for detailed subscription, see the NpcExtraInfo.NpcType
    switch (this->GetObjectType())
    {
    case SYNC_MERCHANT_OBJECT: //__NA_276_20070213_UNIT_TRIGGER
        if (SyncMerchant* npc = static_cast<SyncMerchant*>(this))
        {
            const DWORD unit_id = npc->GetRegenID();
            if (unit_id == 0) {
                break;
            };
            // NOTE: f110704.5L, added the iff filter
            // for support the custom eNPC_MOVE_SYNC_MERCHANT types.
            // to extend the above rule set, it needs to modify.
            if (g_ExtraNPCManager.CheckProtectedNpc(npc) == false) {
                break;
            };
            Policyment.SetField(
                (Policyment.FIELD) |
                (Policyment.PCC_PROTECTED_NPC)
                );
        }
        break;
    };
    //----------------------------------------------------------------------------------------------
}


// (CHANGES) (100205) (WAVERIX) EnterField 이후, Character에 대한 세부사항 제어 필요한 경우
// 추가 룰을 설정하기 위함. 특수 사항만 처리할 것
void
Character::ChangePolicyPostEnterField(const sPOLICY_CHARACTER::eFLAG add_flag,
                                      const sPOLICY_CHARACTER::eFLAG del_flag,
                                      Object* related_object)
{
    assert(this->GetField() != NULL);
    // (WARNING) 함부로 설정을 변경시킬 때, 예기치 못한 문제가 발생할 수 있다.
    // 검증된 필요한 것들만 필요에 따라 설정/해제하기 위한 제어 루틴
    if (del_flag != Policyment.PCC_DISABLE_ALL)
    {
        if (del_flag & Policyment.PCC_CONTINUING_NPC_UPDATE) {
            Policyment.SetField(Policyment.FIELD & ~Policyment.PCC_CONTINUING_NPC_UPDATE);
        }
    }

    if (add_flag != Policyment.PCC_DISABLE_ALL)
    {
        // (NOTE) current rule
        //  related_object->IsEqualObjectKind(PLAYER_OBJECT) &&
        //  this->IsEqualObjectKind(SUMMON_OBJECT)    --> summon npc by player
        if (add_flag & Policyment.PCC_CONTINUING_NPC_UPDATE) {
            Policyment.SetField(Policyment.FIELD | Policyment.PCC_CONTINUING_NPC_UPDATE);
        }
        // checked by _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
        if (add_flag & Policyment.PCC_EXIST_AREA_EVENT_NPC) {
            Policyment.SetField(Policyment.FIELD | Policyment.PCC_EXIST_AREA_EVENT_NPC);
        };
    }
}

ChaosZoneManager* Character::GetChaosZoneManager()
{
    if (GetGameZonePtr() && GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)  
    {
        const GameInstanceDungeon* instance_dungeon = 
            static_cast<GameInstanceDungeon*>(GetGameZonePtr());
        if (instance_dungeon && instance_dungeon->GetChaosZoneManager())
        {
            return instance_dungeon->GetChaosZoneManager();
        }
    }

    return NULL;
}

#ifdef _NA_008299_20150519_MAX_DAMAGE_LOG
void Character::WriteMaxDamageLog(Character* pAttacker, DamageArgs* damage_arg)
{
    if (BATTLE_SERVER != g_pGameServer->GetServerType())
    {
        return;
    }
    
    GameZone* game_zone = GetGameZonePtr();
    if (pAttacker == NULL || damage_arg == NULL || game_zone == NULL)
    {
        return;
    }

    CHARGUID target_char_guid = 257252; // u1은빛포르쉐

    Player* attacker_player = NULL;
    if (damage_arg->damage_ < DAMAGETYPE_MAX / 2)
    {
        if (pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
        {
            attacker_player = static_cast<Player*>(pAttacker);
            if (attacker_player == NULL
                || attacker_player->GetCharGuid() != target_char_guid)
            {
                return;
            }
        }
        else if(pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
        {
            attacker_player = pAttacker->GetPlayerOwner();
            if (attacker_player == NULL
                || attacker_player->GetCharGuid() != target_char_guid)
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
    else // if (damage_arg->damage_ >= DAMAGETYPE_MAX / 2)
    {
        if (pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
        {
            attacker_player = static_cast<Player*>(pAttacker);
            if (attacker_player == NULL)
            {
                return;
            }
        }
        else if(pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
        {
            attacker_player = pAttacker->GetPlayerOwner();
            if (attacker_player == NULL)
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    // 소환수
    bool is_summoned = false;
    int summoned_code = 0;
    if (pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
    {
        is_summoned = true;
        Summoned* summoned = static_cast<Summoned*>(pAttacker);
        if (summoned != NULL)
        {
            const BASE_NPCINFO* pInfo = summoned->GetBaseInfo();
            if (pInfo != NULL)
            {
                summoned_code = pInfo->m_MonsterCode;
            }
        }
    }

    // 피격 NPC
    int npc_code = 0;
    if (this->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* npc = static_cast<NPC*>(this);
        if (npc != NULL)
        {
            const BASE_NPCINFO* pInfo = npc->GetBaseInfo();
            if (pInfo != NULL)
            {
                npc_code = pInfo->m_MonsterCode;
            }
        }
    }

    SUNMAXDAMAGELOG(eCRITICAL_LOG, "[MAX_DAMAGE] %s(%d) - %d, %d, %d (%d, %d)", 
        attacker_player->GetCharName(), attacker_player->GetCharGuid(),
        game_zone->GetMapCode(), npc_code, damage_arg->skill_code_,
        is_summoned, summoned_code);

    GAMELOG->LogMaxDamage(ACT_MAX_DAMAGE, this, pAttacker, damage_arg);
}
#endif // _NA_008299_20150519_MAX_DAMAGE_LOG
