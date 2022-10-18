//------------------------------------------------------------------------------
//  SkillQueueManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SkillQueueManager.h"
#include "Input.h"
#include "ResourceManager.h"
#include "uiskillqueueman/uiskillqueueman.h"
#include "SkillQueueDialog.h"
#include "interfacemanager.h"
#include "SkillInfoParser.h"
//------------------------------------------------------------------------------
/**
*/
void SkillQueueManager::Init()
{

}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillQueueManager::ClearAll()
{

	for(int i=0;i<Size();i++)
	{
		ICONMGR()->RemoveIcon(m_SkillArray[i].pImage);
	}


	m_SkillArray.Clear();

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void SkillQueueManager::Release()
{
	ClearAll();
}

//------------------------------------------------------------------------------
/**
*/
void SkillQueueManager::RenderNode()
{
	for(int i=0;i<Size();i++)
	{
		SKILL_IMAGE_NODE node=m_SkillArray[i];

		if(node.pImage)
		{
			SkillQueueDialog *pDlg = GET_CAST_DIALOG(SkillQueueDialog, IM_SKILL_QUEUE_MANAGER::SKILL_QUEUE);

			RECT rc=pDlg->GetSkillRect(i);		
			IconImage *pImage=node.pImage;

			pImage->RenderCoverImage(&rc,WzColor_RGBA(255,255,255,(int)(255*0.6f)));
		}
	}

}


//------------------------------------------------------------------------------
/**
*/

void SkillQueueManager::Update()
{
	
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillQueueManager::PushBack(DWORD dwSkillId,BOOL bForceAttack)
{

		if(IsErase(dwSkillId))//취소 
		{
			return FALSE;
		}
		else
		{
			if(GAMEOPTION->GetSkillQueueing())
			{
				if(m_SkillArray.Size()<SKILL_QUEUE_MAX)
				{
					SKILL_IMAGE_NODE node;
					node.dwSkillID=dwSkillId;
					node.bForceAttack=bForceAttack;
					DWORD dwIconID =ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(dwSkillId);
					node.pImage = ICONMGR()->Add(dwIconID);
					node.dwIconID = dwIconID;
					m_SkillArray.PushBack(node);
				}

			}
			else
			{
				if(m_SkillArray.Size()<1)
				{
					SKILL_IMAGE_NODE node;
					node.dwSkillID=dwSkillId;
					node.bForceAttack=bForceAttack;
					DWORD dwIconID =ResourceManager::Instance()->GetItemSkillSlotImageResourceCode(dwSkillId);
					node.pImage = ICONMGR()->Add(dwIconID);
					node.dwIconID = dwIconID;
					m_SkillArray.PushBack(node);
				}
			}

			return TRUE;
		}

	
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD  SkillQueueManager::GetFrontSkillID()
{
	assert(m_SkillArray.Size());

	return m_SkillArray.Front().dwSkillID;
}

//------------------------------------------------------------------------------
/**
*/
BOOL  SkillQueueManager::GetFrontForceAttack()
{
	assert(m_SkillArray.Size());

	return m_SkillArray.Front().bForceAttack;
}


BOOL SkillQueueManager::Pop()
{
	if(m_SkillArray.Size()==0)
		return FALSE;

	ICONMGR()->RemoveIcon(m_SkillArray[0].pImage);
	m_SkillArray.Erase(0);


	return TRUE;
}

#ifdef _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL
void SkillQueueManager::SkillQueueMoveCancle()
{
    //이동 시 스킬큐 해제(활성화된 지역지정스킬 제외)
    if (m_SkillArray.Size() < 1)
    {
        return;
    }
    
    SKILL_IMAGE_NODE node = m_SkillArray[0];
    
    m_SkillArray.Clear();

    // 첫번째 큐가 지역지정 스킬이면 남김
    BASE_SKILLINFO *skill_info = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)node.dwSkillID);
    if ((skill_info != NULL) &&
        (skill_info->m_byTarget == SKILL_TARGET_AREA))
    {
        m_SkillArray.PushBack(node);
    }
}
#endif //_NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL
