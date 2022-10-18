#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateHelroidSpin.h"
#include "Character.h"
#include <../../ProgramCommon/SkillInfoParser.h>

#include "StateKeyboardJump.h"

#include "Hero.h"
#include "CooltimeManager.h"
#include <NumericValues.h>
#include "Camera.h"


SkillStateHellroidSpin::SkillStateHellroidSpin()
{
}

SkillStateHellroidSpin::~SkillStateHellroidSpin()
{

}

#ifdef _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX
BOOL SkillStateHellroidSpin::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
    Character* pCharacter = (Character *)pOwner;

    DWORD CurrentTime = GetCurrentTime();
    if ( m_EndTime <= CurrentTime )
    {
        if (!(pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE2) ||
            pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE3) ||
            pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE4)))
        {
            return TRUE;
        }
    }

    return FALSE;
}
#endif //_NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX

STATECHANGE::TYPE SkillStateHellroidSpin::OnStart(Object* pOwner, DWORD CurTime)
{
    m_MoveState = 0;
    m_AnimateState = 0;
    m_StartTime = GetCurrentTime();
    m_EndTime = m_StartTime + 5000;


    m_MoveTargetPosition = pOwner->GetVisiblePos();

    // 
    Character* pCharacter = (Character *)pOwner;
    if( pCharacter != NULL )
    {
        if( pCharacter->IsMyHero() == TRUE )
        {
            if( SendSkillPacket() == FALSE )
            {
                assert(!"스킬사용 패킷 전송실패");
                return STATECHANGE::FAILED;
            }

            // 오프라인 처리
            if( GENERALPARAM->IsNet() == FALSE )
            {
                if( util::Timer* pTimer = xGetCooltimeManager()->GetSkillCooltime(m_dwSkillCode) )
                {
                    pTimer->Reset();
                }
            }
        }

        ProcessStartAttackSerial(pCharacter);
        StartSkillAnimation(pCharacter, TRUE );
        //ProcessStartAnimation(pCharacter);
        ProcessStartDirection(pCharacter);
        ProcessStartSkillEffect(pCharacter);

        return STATECHANGE::SUCCESS;
    }

    return STATECHANGE::FAILED;
}

STATEPROCESS::TYPE SkillStateHellroidSpin::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    if( GetAnimationRate() == 100.0f )
        SetAnimationRate(0.0f);


    Character* pCharacter = (Character*)pOwner;


    if( pCharacter == g_pHero )
    {
        // 히어로가 키보드 이동을 할떄
        WzVector NewDirection;

        g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );

        if( StateKeyboardJump::GetJumpDirection( NewDirection, g_pHero->GetMoveFlag(), g_pHero->GetDirection()) == true )
        {
            if (SetTargetPosition(g_pHero, NewDirection) == TRUE)
            {
                m_MoveState = 1;
            }
            else
            {
                m_MoveState = 0;
            }
        }
        else
        {
            m_MoveState = 0;
        }
    }

    // 정지상태가 아니라면
    if( m_MoveState != 0 )
    {
        // 이동 진행
        WzVector CurrentPosition = pCharacter->GetVisiblePos();
        pCharacter->Move( CurrentPosition, m_MoveTargetPosition, CMS_RUN, TRUE );   // 목표좌표 설정

        float move_speed_modifier = pCharacter->GetMoveSpeedModifier();

        if (FALSE == ProcessMove(pCharacter, GetBaseMoveSpeedAsState(CMS_RUN) * move_speed_modifier, dwTick)) // 이동
        {
            pCharacter->StopMove();
            m_MoveState = 0;
        }
    }
    else
    {
        pCharacter->StopMove();
    }

    SendKeyboardMovePacket(pOwner, dwTick );

    CalculateTurnAccelerate(pOwner, dwTick);

    DWORD CurrentTime = GetCurrentTime();
    if( m_EndTime <= CurrentTime )
    {
        if (pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE2) ||
            pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE3) ||
            pCharacter->FindSkillEffect(eCHAR_STATE_BUF_RANGE_DAMAGE4))
        {
            return STATEPROCESS::HOLD;
        }
        else
        {
            return STATEPROCESS::END;
        }
    }

    return STATEPROCESS::HOLD;
}

void SkillStateHellroidSpin::SetHeroKeyboardMove()
{
    // 히어로가 키보드 이동을 할떄
    WzVector NewDirection;

    WzVector Direction = g_Camera.GetCameraDirection();
    Direction.z = 0.0f;
    VectorNormalize(&Direction, &Direction );
    Direction *= -1.0f;


    if( StateKeyboardJump::GetJumpDirection( NewDirection, g_pHero->GetMoveFlag(), Direction ) == true )
    {
        if (SetTargetPosition(g_pHero, NewDirection) == TRUE)
        {
            m_MoveState = 1;
        }
        else
        {
            m_MoveState = 0;
        }
    }
}

void SkillStateHellroidSpin::RecvKeyboardMovePacket(Object* pOwner, BYTE MoveState, const WzVector& CurrentPosition, float fAngle )
{
    Player* pPlayer = (Player*)pOwner;

    // 히어로가 키보드 이동을 할떄
    WzVector NewDirection;

    pPlayer->SetAngle( fAngle );

    if( StateKeyboardJump::GetJumpDirection( NewDirection, MoveState, pPlayer->GetDirection()) == true )
    {
        if (SetTargetPosition(pPlayer, NewDirection) == TRUE)
        {
            m_MoveState = 1;
        }
        else
        {
            m_MoveState = 0;
        }
    }
    else
    {
        m_MoveState = 0;
    }
}


BOOL SkillStateHellroidSpin::SendKeyboardMovePacket(Object* pObject, DWORD dwTick)
{
    if (pObject != g_pHero)
    {		
        return FALSE;
    }


    BOOL rt = FALSE;
    bool is_need_distance_check = false;

    g_pHero->AddKeyboardMovePacketSendTime(dwTick); 

    if (g_pHero->GetMoveFlag() != g_pHero->GetOldMoveFlag())
    {
        rt= TRUE;

        if (g_pHero->KBMove.is_first_kbmove == false)
        {
            is_need_distance_check = true;
        }
    }
    else if (g_pHero->GetKeyboardMovePacketSendTime() >= g_pHero->GetKeyboardMovePacketSendDuration())
    {
        rt= TRUE;
    }


#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if ((rt == FALSE) && (g_pHero->KBMove.last_sent_move_flag != g_pHero->GetMoveFlag()))
    {
        rt = TRUE;
        if (g_pHero->KBMove.is_first_kbmove == false)
        {
            is_need_distance_check = true;
        }
    }

    //! 20도  20/180*3.14 = 0.34f
    float ang_gab = abs(g_pHero->KBMove.ang - g_pHero->GetAngle()); 
    if ((rt == FALSE) && (ang_gab > 0.34f)) 
    {
        rt= TRUE;
        is_need_distance_check = true;
    }
    if ((rt == TRUE) && (is_need_distance_check == true))
    {
        float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier();

        float dist_gab = (fMoveSpeed / 0.01f) * (fMoveSpeed / 0.01f) * 10.0f; 


        if (g_pHero->KBMove.last_sent_move_flag != g_pHero->GetMoveFlag())
        {
            dist_gab = min(2, dist_gab);
        }
        else
        {
            dist_gab = min(6.0f, dist_gab);
            //if (ang_gab < WZ_PI / 4.0f) dist_gab = min(10, dist_gab);
            //else if (ang_gab < WZ_PI / 2.0f) dist_gab = min(7, dist_gab);
            //else if (ang_gab < WZ_PI / 1.5f) dist_gab = min(5, dist_gab);
            //else dist_gab = min(3, dist_gab);
        }

        WzVector gab = g_pHero->KBMove.pos - g_pHero->GetPosition(); 
        float gab_len = VectorLength(&gab);
        if (gab_len < dist_gab)
        {
            g_pHero->KBMove.debug_move_speed = fMoveSpeed;
            g_pHero->KBMove.debug_kbmove_skip_count += 1;
            rt = FALSE;
        }
        g_pHero->KBMove.debug_kbmove_gab = gab_len;
    }
    if (rt)
    {
        DWORD tick_gab = clock_function::GetTickCount() - g_pHero->KBMove.sent_tick;
        if (tick_gab > 100)
        {
            return SEND_CG_SYNC_KBMOVE_SYN();
        }
    }
#else
    if(rt)
    {
        MSG_CG_SYNC_KBMOVE_SYN Packet;
        Packet.m_byCategory = CG_SYNC;
        Packet.m_byProtocol = CG_SYNC_KBMOVE_SYN;
        Packet.m_byMoveState = (BYTE)g_pHero->GetMoveFlag();
        Packet.wvCurPos = g_pHero->GetPosition();
        Packet.m_wAngle = (WORD) (g_pHero->GetAngle() * 180.0f / WZ_PI);
        Packet.m_wTileIndex = g_pHero->GetPathExplorer()->GetTile();

        OutputDebugStr("Send keyboardPosition\n");

        Packet.Encode();

        g_pHero->SendPacket(&Packet,sizeof(Packet));
        g_pHero->SetOldMoveFlag(g_pHero->GetMoveFlag());
        g_pHero->SetKeyboardMovePacketSendDuration(500);
        g_pHero->SetKeyboardMovePacketSendTime(0);
        WzVector vDummy;
        vDummy.x = 0.0f;
        vDummy.y = 0.0f;
        vDummy.z = 0.0f;

        g_pHero->SetOldKeyboardDirection(&vDummy); 
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

    return rt;
}

void SkillStateHellroidSpin::CalculateTurnAccelerate(Object* pPlayerObject, DWORD dwTick)
{
    Player* pPlayer = (Player*)pPlayerObject;

    const  float l_fSpeedAccelerator = 0.002f;
    float  fTurnSpeed = 0.0f;
    float  fMaxTurnSpeed = 0.04f;

    if (pPlayer == g_pHero)
    {
        int oldturnflag = g_pHero->GetOldMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);
        int newturnflag = g_pHero->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);

        if (oldturnflag == newturnflag && newturnflag)
        {
            if (newturnflag & KEYBOARDMOVE_LEFTTURN)
            {
                pPlayer->m_fTurnAccelerate += l_fSpeedAccelerator;
            }
            else 
            {
                pPlayer->m_fTurnAccelerate -= l_fSpeedAccelerator;
            }
        }
        else
        {
            pPlayer->m_fTurnAccelerate *= 0.7f;
            if (fabs(pPlayer->m_fTurnAccelerate) <= (l_fSpeedAccelerator * 0.1f))
            {
                pPlayer->m_fTurnAccelerate = 0.0f;
            }
        }

        if (pPlayer->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN))
        {
            if (g_pHero->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
            {
                pPlayer->m_fTurnAccelerate = l_fSpeedAccelerator;
            }
            else 
            {
                pPlayer->m_fTurnAccelerate = -l_fSpeedAccelerator;
            }
        }
        else
        {
            pPlayer->m_fTurnAccelerate = 0.0f;
        }		
    }
    else
    {
        if (pPlayer->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN))
        {
            if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
            {
                pPlayer->m_fTurnAccelerate = l_fSpeedAccelerator;
            }
            else 
            {
                pPlayer->m_fTurnAccelerate = -l_fSpeedAccelerator;
            }
        }
        else
        {
            pPlayer->m_fTurnAccelerate = 0.0f;
        }
    }

    fTurnSpeed += pPlayer->m_fTurnAccelerate;

    if (fTurnSpeed >= fMaxTurnSpeed)
    {
        fTurnSpeed = fMaxTurnSpeed;
    }
    else if (fTurnSpeed <= -fMaxTurnSpeed)
    {
        fTurnSpeed = -fMaxTurnSpeed;
    }


    if (fabs(fTurnSpeed) > 0.0f) 
    {
        pPlayer->Rotate(fTurnSpeed * dwTick);		
    }

}

BOOL SkillStateHellroidSpin::SetTargetPosition(Object* pOwner, const WzVector& NewDirection)
{
    WzVector temp_position;
    int tile_index = -1;

    for (int i = 0; i < 5; ++i)
    {
        temp_position = pOwner->GetPosition() + NewDirection * static_cast<float>(21 - (4 * i));

        tile_index = g_pSunTerrain->x_pPathFinder->GetTileToStand(temp_position, NULL, -1.0f, 100.0f);

        if (tile_index > 0)
        {
            break;
        }
    }

    if (tile_index > 0)
    {
        m_MoveTargetPosition = temp_position;
        return TRUE;
    }

    return FALSE;
}