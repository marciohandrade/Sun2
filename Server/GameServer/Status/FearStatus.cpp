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
	m_UpdateTick.SetTimer(0, true, false);	// �ٷ� ȣ��

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
    
	// �������� ���·� �����.
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

        //�ٸ� �÷��̾�鿡�� ��ε� ĳ����(�ڽŵ� ����)
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
                // ��Ȥ�� Ÿ���� ���󰡾� �ϹǷ� �̵��߿� �ٽ� RunAway�� ȣ���Ѵ�
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
	//	-	�� ���� FindPath�� ȣ���ϴ� �ش� �Լ����� ó���ϴ� ���� ��ǥ� ���õ� �͸� �־ ��ܿ��� ��ŵ��Ų��.
	//	-	���� ��ǥ ������ �ƴ� �ٸ� ������ ���� ���� ��Ȳ�� ������ ó���ؾ� �� ���̴�.
	if( pStatusManager->AnimationDelay.NeedSkipProcessByAnimationDelay( this ) )
		return;

	// �̵��� �Ÿ��� ���Ѵ�.
	AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
	float MOVE_DISTANCE = (float)dRandom( stAICommonInfo.m_fMIN_MOVE_DISTANCE, stAICommonInfo.m_fMAX_MOVE_DISTANCE );
    
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (GetAbilityID() == eABILITY_CHARMED) // ��Ȥ
    {
        const Character* const attacker = GetAttacker();
        if (attacker == NULL)
        {
            return;
        }
        if (owner->IsEqualObjectKind(PLAYER_OBJECT))    // Ÿ�� ��ġ ����
        {
            m_bTouchedSomething = FALSE;
            m_vTargetPos = *attacker->GetPosPtr();
        }
        MOVE_DISTANCE *= -1; // ���� ����
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	WzVector wvCurPos, wvDestPos, wvMovement;

	owner->GetPos( &wvCurPos );

	GameField *pField = owner->GetField();
	if( !pField ) return;	

	// ���� ��ã�⿡�� ���𰡿� ���� ��� 60�� ȸ����Ű��, �׷��� �ʴٸ� ������ �������� ������
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
		// ��ֹ��� ��Ҵٴ� ǥ�ø� �ϰ� ���� ������ ���⿡�� 60�� ȸ����Ų ���͸� ���صд�.
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
		msg.m_byState		= CMS_RUN;	//ĳ������ ���� ������ �ٱ⸸ ����!!(���Ϳ� ���� ���� ��Ŷ�̹Ƿ� ���� �־���)
		msg.m_ForcedMove	= TRUE;
		msg.m_wvCurPos		= wvCurPos;
		msg.m_wvDestPos		= wvDestPos;

		owner->SendPacketAroundForSync( &msg );
	};
}


