//------------------------------------------------------------------------------
/**

	��ų �Է� ť�� 
	@author
		-������ 
	@since
		-2005.12.08
	@remarks
		-max 3�� ���� �ȴ�.
		-�̵��� ��� ���� ����.
		-���� ����Ϸ��� ��ų�� ��� ������ �ȵǸ� ���  ����.
		-������ Ÿ���� ��尡 ���Եɰ�� ������ ��带 ���� 
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
    void            SkillQueueMoveCancle(); //�̵� �� ��ųť ����(Ȱ��ȭ�� ����������ų ����)
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