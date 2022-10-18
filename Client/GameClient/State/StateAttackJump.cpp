//------------------------------------------------------------------------------
//  StateAttackJump.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateAttackJump.h"
#include "character.h"
#include "monster.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "Hero.h"
#include "HeroActionInput.h"

// �߷°��ӵ�
const int GRAVITY = 20;
// �����ϴ� ����
const float UP_HEIGHT = 2.5f;

const int X_ACCELERATION = 20;

//------------------------------------------------------------------------------
/**
*/
StateAttackJump::StateAttackJump(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateAttackJump::~StateAttackJump(void)
{
}

//------------------------------------------------------------------------------
/**
	������ MOVE �� �ݵ�� ���ư����Ѵ�.
	������ �ʾƿ�~
*/
BOOL StateAttackJump::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{	
	return TRUE;	
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAttackJump::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;
    //Character * pTarget = (Character *)pChr->GetTargetObject();

    pChr->SetFreeMove(TRUE);

    DWORD dwAttackSerial = pChr->GetNextActionAttackSerial();

    pChr->SetCurrentAttackSerial(dwAttackSerial);   
    pChr->SetNextAttackSerial(dwAttackSerial);

    ATTACK_JUMP_INFO * pAttackJump = &pChr->m_AttackJumpInfo;
    


    // ���� �ִϸ��̼� ����
    WzID animationID = pChr->GetCurrentAttackJumpAnimation();
    assert(animationID);
    pChr->StopBlendAnimation();
    pChr->SetAnimation(animationID,FALSE);

    pChr->SetCurrentFrame(0.0f);

    // ��Ʈ�� ó���� ���� �ӽú���
    pChr->m_iHitEvent = 0;
    pChr->m_iHitCount = 0;
    pChr->m_bRightHit = FALSE;

	pAttackJump->m_dwTotalTime = 
        pChr->GetCurrentAction()->ATTACKJUMP.dwEndTime - 
        pChr->GetCurrentAction()->ATTACKJUMP.dwStartTime;
	
	// �Ʒ��� �������� �ϴ� ����
	pAttackJump->m_fDestHeight = 
        pChr->GetCurrentAction()->ATTACKJUMP.vDestPos.wvPos.z - 
        pChr->GetCurrentAction()->ATTACKJUMP.vCurPos.wvPos.z;

	// ������ ��ǥ�� ���� ��ǥ���� ����
	pAttackJump->m_vDist = 
        pChr->GetCurrentAction()->ATTACKJUMP.vDestPos.wvPos - 
        pChr->GetCurrentAction()->ATTACKJUMP.vCurPos.wvPos;
	//pAttackJump->m_vDist.z = 0;


	// ĳ���� ���� ����
	pChr->SetAngle( Math_GetAngleFromVector(&pAttackJump->m_vDist), TRUE );

    if (UP_HEIGHT == 0 && pAttackJump->m_fDestHeight == 0)
    {
        return STATECHANGE::FAILED;
    }

	// ������ ������ ���� �ʱ� �ӵ�
	float fDestLength = VectorLength( &pAttackJump->m_vDist );    
    float fUpHeight = max( UP_HEIGHT, pAttackJump->m_fDestHeight ); 
	pAttackJump->m_fVelocity_Y = sqrt( 2 * GRAVITY * fUpHeight );
    
   

    if( pAttackJump->m_fDestHeight >= UP_HEIGHT )
    {
        pAttackJump->m_fVelocity_X = fDestLength * GRAVITY / pAttackJump->m_fVelocity_Y;
    }
    else
    {
        pAttackJump->m_fVelocity_X = fDestLength * GRAVITY /
            ( 
            pAttackJump->m_fVelocity_Y + 
            sqrt( pAttackJump->m_fVelocity_Y * pAttackJump->m_fVelocity_Y - 2*GRAVITY*pAttackJump->m_fDestHeight ) 
            );
    }

    if ( !GENERALPARAM->IsNet() && g_pHero)
    {
        MSG_CG_SYNC_NPC_JUMPRESULT_BRD SendPacket;

        SendPacket.m_dwObjectKey = g_HeroInput.GetCurrentTarget();
        SendPacket.m_dwTargetObjectKey = g_pHero->GetObjectKey();
        SendPacket.m_byAttackType = 0;
        SendPacket.m_wvCurPos = g_pHero->GetPosition();
        SendPacket.m_wDamage = 1;
        SendPacket.m_wTargetHP = 5;
        
        SendPacket.m_byEffect = 0;				// ���� ����� ���� Ư�� ����Ʈ(ũ��Ƽ���� ������...)

        g_pApplication->AddVirtualPacket( 
            CI_GAMESERVERIDX, (BYTE *)&SendPacket, sizeof(SendPacket));
    }

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateAttackJump::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE ret = STATEPROCESS::HOLD;
	Character*	pChr = (Character*)pOwner;

    ATTACK_JUMP_INFO * pAttackJump = &pChr->m_AttackJumpInfo;

    float fRet = 0.f;

    WzEvent event;	
    ZeroMemory(&event, sizeof(event));    

	DWORD dwRunTime = g_CurTime - pChr->GetCurrentAction()->ATTACKJUMP.dwStartTime ;
    float fMoveRate = (float)dwRunTime / (float)pChr->m_AttackJumpInfo.m_dwTotalTime;
    if (fMoveRate >= 1.0f)
	{
		fMoveRate = 1.0f;
	}
	// ���(x,y)������ �̵��� �Ÿ�
	float fMoveLength = VectorLength( &( pAttackJump->m_vDist * fMoveRate ) );
    float fVecLength = VectorLength( &pAttackJump->m_vDist );

	// fMoveLength�� �������� ���� ���� ���� ����Ѵ�.
	float fResult_Delta_Z = 
        ( 2*pAttackJump->m_fVelocity_X*pAttackJump->m_fVelocity_Y-GRAVITY*fMoveLength ) / 
        ( 2*pAttackJump->m_fVelocity_X*pAttackJump->m_fVelocity_X ) * fMoveLength;

	// �������� ���� ��ǥ�� �����Ѵ�.
	WzVector vResultPos;
    if ( 0 == fVecLength )
    {
        vResultPos = 
            pChr->GetCurrentAction()->ATTACKJUMP.vCurPos.wvPos;	  
    }
    else
    {
        vResultPos = 
            pChr->GetCurrentAction()->ATTACKJUMP.vCurPos.wvPos + pAttackJump->m_vDist * fMoveRate;
	    vResultPos.z = fResult_Delta_Z + pChr->GetCurrentAction()->ATTACKJUMP.vCurPos.wvPos.z;
    }

	float fTotalFrame = pChr->GetTotalFrame();

	if (fMoveRate <= 0.45f) 
	{
        //float fNewFrame = (fTotalFrame * 0.2f)  * (fMoveRate / 0.2f);
        float fNewFrame = fTotalFrame * fMoveRate;
        pChr->SetCurrentFrame(fNewFrame);
        fRet = pChr->ProcessAnimation(0, &event);
	}	
	else if (fMoveRate >= 0.8f)
	{
        fRet = pChr->ProcessAnimation(dwTick, &event);
	}
    else
    {
        fRet = pChr->ProcessAnimation(0, NULL);
    }

	pChr->ProcessInterpolation(dwTick);

    // Ÿ���� Ÿ�̹�
    if (event.m_iNumCustom)
    {
        for (int a = 0; a < event.m_iNumCustom; a++)
        {
            // Ÿ������ ������ġ������ ����ġ������ �����Ѵ�.
            switch(event.m_Custom[a].m_iaParam3[0])
            {
                // 1�� event�� ���� Ÿ�̹�
            case 1:
                {
                    pChr->m_iHitEvent++;
                    if (event.m_Custom[a].m_iaParam3[1] == 1) 
                    {
                        pChr->m_bRightHit = FALSE;
                    }
                    else
                    {
                        pChr->m_bRightHit = TRUE;
                    }

                    if( pChr->m_iHitCount < 1 )
                    {
                        if( pChr->ProcessAttackResult( pChr->GetCurrentAttackSerial(), FALSE, pChr->m_bRightHit ) )
                        {				
                            pChr->m_iHitCount++;									
                        }
                    }
                }
                break;
            }
        }
    }
	
    // process interpolation ������renderpart��ġ�� set�ϹǷ� ��ȿ�ϴ�.
    pChr->GetBasePart()->SetPosition(vResultPos);
    pChr->UpdateOctree();

	if (fMoveRate >= 1.0f)
	{
		pChr->SetPosition(vResultPos);
		
		pChr->SetVisiblePos(vResultPos);

        if ( fRet >= 100.f )
        {
            if (pChr->ProcessNextAction())
            {
                return STATEPROCESS::SUSPEND;
            }
            else
            {
                return STATEPROCESS::END;
            }
        }
	}
	
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAttackJump::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;
	pChr->SetJumpFlag(FALSE);
	pChr->m_fHeight = 0.0f;

    if (pChr->m_iHitCount < pChr->GetShotCount())
    {
#if defined( _DEBUG )
        TCHAR	szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

        //	������Ʈ %d�� ���� Ÿ������ ���ų� ������ ��ҵǾ����ϴ�.
        g_InterfaceManager.GetInterfaceString( eST_STATE_ATTACK_FAIL, szText, INTERFACE_STRING_LENGTH );
        _stprintf( szMessage, szText, pChr->GetObjectKey() );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
#endif

        pChr->ProcessAttackResult( pChr->GetCurrentAttackSerial(), TRUE );
    }

    pChr->m_iHitCount = 0;
    pChr->AddNextAttackSerial();
    pChr->SetAttackStandTime(ATTACK_STAND_TIME);

    pChr->SetFreeMove(FALSE);


    return STATECHANGE::SUCCESS;
}


