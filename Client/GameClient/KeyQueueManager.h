//------------------------------------------------------------------------------
/**

	하나의 키로 여러개의 작업을 할때 사용 
	@author
		-김지완 
	@since
		-2005.11.03
	@remarks
		-특정 키에 대해서 여러가지 동작을 해야 할경우 eDoType를 추가 한다.

*/
#ifndef		_KEY_QUEUE_MANAGER_H
#define		_KEY_QUEUE_MANAGER_H


#pragma once

#include "nArray.h"

typedef void (*fn_CallBack)(DWORD,DWORD);

struct keyMsg
{
    fn_CallBack DoSomething;
    BYTE        bPriority;  //우선 순위 현재 안쓰임 
    DWORD       dwType;		//메세지 타입 
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
	인풋을 잡아낼 키 
*/
inline void KeyQueueManager::SetKey(BYTE key)
{
	m_key=key;
}

//------------------------------------------------------------------------------
/**
	키는 눌리고 노드가 아무것도 없을때 할것
*/
inline void KeyQueueManager::SetDefaultFun(fn_CallBack func)
{
	m_defulatFunction=func;
}


#endif