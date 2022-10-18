//------------------------------------------------------------------------------
//  SkillStateHide.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateHide.h"
#include "Character.h"
#include "Player.h"
#include "Clone.h"
#include "CloneManager.h"
#include "ObjectManager.h"


SkillStateHide::SkillStateHide()
{
	m_dwCopyTime=0;
	m_iCloneCnt=0;
}

SkillStateHide::~SkillStateHide(void)
{
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateHide::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	
	EnterProcess(pOwner,CurTime,dwTick);

	Player*	pPlayer = (Player*)pOwner;

	CheckEvent();

	if(!pPlayer->GetHideSkill())
	{
		if( m_dwCopyTime < dwTick && m_iHitProcessed==0) 
		{
			//갯수제한
			if(++m_iCloneCnt<7)
			{
				m_dwCopyTime=30;

				pPlayer->CloneEffect(500, 1700, 0);
			}
		}
		else
		{
			m_dwCopyTime-=dwTick;
		}

		//첫번째이벤트에 발사체를 날린다.
		if (CheckNotProcessedHitEvent(1)) 
		{
			SetHidePlayer(pOwner);
			++m_iHitProcessed;
		}
	}

	return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateHide::OnEnd(Object* pOwner, DWORD CurTime)
{
	//safecode:하이드이벤트보다 더 늦은시점에 패킷이 날라왔다면 예외처리
	SetHidePlayer(pOwner);

	return STATECHANGE::SUCCESS;
}
//------------------------------------------------------------------------------
/**
*/
void
SkillStateHide::SetHidePlayer(Object *pObj)
{
	Player*	pPlayer = (Player*)pObj;

	if(pPlayer)
	{
		if(pPlayer->GetHideReserve())
		{
			pPlayer->SetHideSkill(TRUE);

			// 이미 자신이 은신체감지 버프가 있는경우에는 바로 보여줘야하기때문
			g_ObjectManager.RefreshPlayerHideState();
		}

		pPlayer->SetHideReserve(FALSE);
	}

}



