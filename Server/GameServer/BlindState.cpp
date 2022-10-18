#include "StdAfx.h"
#include ".\blindstate.h"

// (NOTE) implemented by __NA_S00015_20080828_NEW_HOPE_SHADOW

BlindState::BlindState(void)
{
}

BlindState::~BlindState(void)
{
}

void
BlindState::Init()
{

}

void
BlindState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    DWORD interval = static_cast<DWORD>(param1);
    //m_BlindTimer.SetTimer(interval);
    m_BlindTimer.SetTimer(m_BlindTimer.eStaticTimer_Blind, interval);
}

void
BlindState::OnExit()
{
    State::OnExit();
}


BOOL
BlindState::OnUpdate(DWORD dwDeltaTick)
{
    if (m_BlindTimer.IsExpired())
    {
		m_pNPC->ChangeState(STATE_ID_IDLE);
        return true;
    }

    if (!m_pNPC->IsMoving()) 
    {
        WzVector wvDestPos = m_pNPC->GetRandomPosInAround(2);

        if (m_pNPC->MoveAndBroadcast(&wvDestPos, CMS_RUN) == false) {
            // OnUpdate(..) 함수는 false로 리턴하면 안된다.
            //return false;
        }
    }

    return true;
}

