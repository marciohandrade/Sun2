//------------------------------------------------------------------------------
/**

	스킬 입력 큐잉 
	@author
		-김지완 
	@since
		-2005.12.08
	@remarks
		-max 3개 까지 된다.
		-이동시 노드 전부 삭제.
		-현재 사용하려는 스킬이 사용 조건이 안되면 노드  삭제.
		-동일한 타입의 노드가 삽입될경우 기존의 노드를 삭제 
*/
#ifndef		_SKILL_QUEUE_MANAGER_H
#define		_SKILL_QUEUE_MANAGER_H
#pragma once

#include "nArray.h"
#define SKILL_QUEUE_MAX	3
#include "IConMgr.h"
#include "iconimage.h"

typedef struct _SKILL_IMAGE_NODE
{
	DWORD      dwSkillID;
	BOOL       bForceAttack;
	DWORD      dwIconID;
	IconImage *pImage;	
} SKILL_IMAGE_NODE;



class SkillQueueManager
{
public:
	SkillQueueManager();
	~SkillQueueManager();

	void			Init();
	void			Release();
	void			Update();
	void            RenderNode();

	BOOL			PushBack(DWORD dwSkillId,BOOL bForceAttack);
	DWORD			GetFrontSkillID();
	BOOL            GetFrontForceAttack();

	BOOL			ClearAll();
	BOOL            Pop();
	int				Size();
	BOOL            IsErase(DWORD dwSkillId);

#ifdef _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL
    void            SkillQueueMoveCancle(); //이동 시 스킬큐 해제(활성화된 지역지정스킬 제외)
#endif //_NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL

protected:	
	nArray<SKILL_IMAGE_NODE>  m_SkillArray;
};


inline int SkillQueueManager::Size()
{
	return m_SkillArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline SkillQueueManager::SkillQueueManager()
{
}

//------------------------------------------------------------------------------
/**
*/
inline SkillQueueManager::~SkillQueueManager()
{
	
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL SkillQueueManager::IsErase(DWORD dwSkillId)
{
	for(int i=0;i<Size();i++)
	{
		if(m_SkillArray[i].dwSkillID==dwSkillId)
		{
			ICONMGR()->RemoveIcon(m_SkillArray[i].pImage);
			m_SkillArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

#endif