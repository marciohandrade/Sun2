#include "stdafx.h"
#include <SinTable.h>
#include <NumericValues.h>
#include <PacketControl/WarMessages_Sync.h>

#include "Ability.h"
#include "FearStatus.h"
#include "AIParamParser.h"
#include "StatusManager.h"
#include "MoveStateControl.h"

VOID FearStatus::Start()
{
	m_UpdateTick.SetTimer(0, true, false);	// 바로 호출

	const Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    
	// 도망가는 상태로 만든다.
    DWORD attacker_object_key  = attacker->GetObjectKey();
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    if (attacker->IsEqualObjectKind(SUMMON_OBJECT) == true) {
        attacker_object_key = attacker->GetSummonerKey();
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
	AI_MSG_RUNAWAY ai_msg;
	ai_msg.dwTargetKey = attacker_object_key;
	ai_msg.dwRunAwayTime = GetApplicationTime();
	ai_msg.wStateID = GetStateID();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    ai_msg.abilityID = GetAbilityID();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	owner->SendAIMessage(&ai_msg, sizeof(ai_msg));

    if (owner->IsEqualObjectKind(PLAYER_OBJECT))
	{
		m_bTouchedSomething = FALSE;
		m_vTargetPos = *attacker->GetPosPtr();
	}

	// Player & Monster Common
	StatusManager* const status_manager = owner->GetStatusManager();
    (status_manager->AnimationDelay).SetAnimationDelay(this, GetApplicationTime());
}

VOID	FearStatus::End()
{
	m_UpdateTick.Stop();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        owner->StopMoving();

        //다른 플레이어들에게 브로드 캐스팅(자신도 포함)
        MSGSUB_SYNC_STOP_BRD msg;
        msg.m_dwObjectKey	= owner->GetObjectKey();
        msg.m_wvCurPos	= *owner->GetPosPtr();
        owner->SendPacketAroundForSync( &msg );
    }
	
	AbilityStatus::End();
}

BOOL	FearStatus::Update( DWORD curTime )
{
	if( m_UpdateTick.IsExpired() )
	{
		m_UpdateTick.SetTimer(UPDATE_INTERVAL, true, false);	// delayed update
        
        Character* const owner = GetOwner();
        if (owner != NULL)
        {
            if (owner->IsEqualObjectKind(PLAYER_OBJECT))
            {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                // 매혹은 타겟을 따라가야 하므로 이동중에 다시 RunAway를 호출한다
                if (!owner->IsMoving() || (GetAbilityID() == eABILITY_CHARMED))
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                if (!owner->IsMoving())
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                {
                    RunAway();
                }
            }
        }
    }

	return AbilityStatus::Update( curTime );
}

VOID	FearStatus::RunAway()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	StatusManager* const pStatusManager = owner->GetStatusManager();
	//	(WAVERIX)(080610)(NOTE)
	//	-	현 시점 FindPath를 호출하는 해당 함수에서 처리하는 것은 좌표등에 관련된 것만 있어서 상단에서 스킵시킨다.
	//	-	만약 좌표 설정이 아닌 다른 내용이 들어가는 경우는 상황을 따져서 처리해야 할 것이다.
	if( pStatusManager->AnimationDelay.NeedSkipProcessByAnimationDelay( this ) )
		return;

	// 이동할 거리를 구한다.
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	float MOVE_DISTANCE = (float)dRandom( stAICommonInfo.m_fMIN_MOVE_DISTANCE, stAICommonInfo.m_fMAX_MOVE_DISTANCE );
    
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (GetAbilityID() == eABILITY_CHARMED) // 매혹
    {
        const Character* const attacker = GetAttacker();
        if (attacker == NULL)
        {
            return;
        }
        if (owner->IsEqualObjectKind(PLAYER_OBJECT))    // 타겟 위치 갱신
        {
            m_bTouchedSomething = FALSE;
            m_vTargetPos = *attacker->GetPosPtr();
        }
        MOVE_DISTANCE *= -1; // 방향 설정
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	WzVector wvCurPos, wvDestPos, wvMovement;

	owner->GetPos( &wvCurPos );

	GameField *pField = owner->GetField();
	if( !pField ) return;	

	// 이전 길찾기에서 무언가에 닿은 경우 60도 회전시키고, 그렇지 않다면 랜덤한 방향으로 포인팅
	if( m_bTouchedSomething )
	{
		m_bTouchedSomething = FALSE;
		wvMovement = m_wvRotatedVector;
	}
	else
	{
		VectorNormalize( &wvMovement, &( wvCurPos - m_vTargetPos ) );
		wvMovement.x *= MOVE_DISTANCE;
		wvMovement.y *= MOVE_DISTANCE;
	}
	wvMovement.z = 2.f;

	wvDestPos = wvCurPos + wvMovement;

	MoveStateControl* const pMoveStateControl = static_cast<Player*>(owner)->GetMoveStateControl();
	float fMoveSpeed = pMoveStateControl->GetMoveSpeed();
	pMoveStateControl->SetMoveState( CMS_RUN );

	if( !pField->FindPath( owner, &wvDestPos, (eCHAR_STATE_TYPE)GetStateID() ) )
	{
		// 장애물에 닿았다는 표시를 하고 지금 가려는 방향에서 60도 회전시킨 벡터를 구해둔다.
		m_bTouchedSomething	= TRUE;
		WzVector wvDistanceWantToMove;
		wvDistanceWantToMove = wvDestPos - wvCurPos;

		const FLOAT cosVal = g_SinTable.FCOS(stAICommonInfo.m_wSEARCH_ROTATE_ANGLE);
		const FLOAT sinVal = g_SinTable.FSIN(stAICommonInfo.m_wSEARCH_ROTATE_ANGLE);
		m_wvRotatedVector.x = wvDistanceWantToMove.x * cosVal + wvDistanceWantToMove.y * sinVal;
		m_wvRotatedVector.y	= -wvDistanceWantToMove.x * sinVal + wvDistanceWantToMove.y * cosVal;
		m_wvRotatedVector.z = 0.0f;
		return;
	}

	{
		MSGSUB_SYNC_MOVE_BRD msg;
		msg.m_dwObjectKey	= owner->GetObjectKey();
		msg.m_byState		= CMS_RUN;	//캐릭터의 경우는 무조건 뛰기만 존재!!(몬스터와 같이 쓰는 패킷이므로 값은 넣어줌)
		msg.m_ForcedMove	= TRUE;
		msg.m_wvCurPos		= wvCurPos;
		msg.m_wvDestPos		= wvDestPos;

		owner->SendPacketAroundForSync( &msg );
	};
}


