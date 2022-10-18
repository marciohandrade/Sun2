//------------------------------------------------------------------------------
/**

	�ϳ��� Ű�� �������� �۾��� �Ҷ� ��� 
	@author
		-������ 
	@since
		-2005.11.03
	@remarks
		-Ư�� Ű�� ���ؼ� �������� ������ �ؾ� �Ұ�� eDoType�� �߰� �Ѵ�.

*/
#ifndef		_KEY_QUEUE_MANAGER_H
#define		_KEY_QUEUE_MANAGER_H


#pragma once

#include "nArray.h"

typedef void (*fn_CallBack)(DWORD,DWORD);

struct keyMsg
{
    fn_CallBack DoSomething;
    BYTE        bPriority;  //�켱 ���� ���� �Ⱦ��� 
    DWORD       dwType;		//�޼��� Ÿ�� 
    DWORD       wParam;
    DWORD       lParam;
};

class nBucket;



class KeyQueueManager
{
public:
	KeyQueueManager();
	~KeyQueueManager();

	void Init();
	void Reset();
	void Release();

	void Update();
    void KeyQueueAction();
	BOOL PushBack(keyMsg & msg);
    BOOL PushFront(keyMsg & msg);
	BOOL DeleteMsg(DWORD dwType);
	void SetKey(BYTE key);
	void SetDefaultFun(fn_CallBack func);
	int  Size();

	BOOL PushKeyMsg( eDoType DoType, fn_CallBack DoSomething, DWORD wParam = 0, DWORD lParam = 0 );

protected:	
	nArray<keyMsg>  m_MsgArray;
	fn_CallBack     m_defulatFunction;  
	BYTE			m_key;
};


inline int KeyQueueManager::Size()
{
	return m_MsgArray.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline KeyQueueManager::KeyQueueManager()
{
	m_defulatFunction=NULL;
}

//------------------------------------------------------------------------------
/**
*/
inline KeyQueueManager::~KeyQueueManager()
{

}

//------------------------------------------------------------------------------
/**
	��ǲ�� ��Ƴ� Ű 
*/
inline void KeyQueueManager::SetKey(BYTE key)
{
	m_key=key;
}

//------------------------------------------------------------------------------
/**
	Ű�� ������ ��尡 �ƹ��͵� ������ �Ұ�
*/
inline void KeyQueueManager::SetDefaultFun(fn_CallBack func)
{
	m_defulatFunction=func;
}


#endif