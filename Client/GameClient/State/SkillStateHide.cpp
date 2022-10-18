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
			//��������
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

		//ù��°�̺�Ʈ�� �߻�ü�� ������.
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
	//safecode:���̵��̺�Ʈ���� �� ���������� ��Ŷ�� ����Դٸ� ����ó��
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

			// �̹� �ڽ��� ����ü���� ������ �ִ°�쿡�� �ٷ� ��������ϱ⶧��
			g_ObjectManager.RefreshPlayerHideState();
		}

		pPlayer->SetHideReserve(FALSE);
	}

}



