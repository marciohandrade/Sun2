//------------------------------------------------------------------------------
//  KeyQueueManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "KeyQueueManager.h"
#include "Input.h"

//------------------------------------------------------------------------------
/**
*/
void KeyQueueManager::Init()
{

}

//------------------------------------------------------------------------------
/**
*/
void KeyQueueManager::Reset()
{
	m_defulatFunction=NULL;
	m_MsgArray.Clear();
	m_key=0;
}

//------------------------------------------------------------------------------
/**
*/
void KeyQueueManager::Release()
{
	Reset();
}

//------------------------------------------------------------------------------
/**
	특정 키가 눌렸을때 등록된 메시지들을 처리 하고 메시지가 없을 경우 기본 행동을 한다.
*/

void KeyQueueManager::Update()
{
	if (m_key && (g_Input.GetState(m_key, KS_UP, TRUE) || g_Input.GetState(m_key, IKS_UP, TRUE)))
	{
        KeyQueueAction();
	}
}

void KeyQueueManager::KeyQueueAction()
{
    if (m_MsgArray.Empty() == false)
    {
        keyMsg& msg = m_MsgArray.Back();
        msg.DoSomething(msg.wParam, msg.lParam);
    }
    else if (m_defulatFunction != NULL)
    {
        m_defulatFunction(0,0);
    }
}

//------------------------------------------------------------------------------
/**
	메시지를 등록 한다.
*/
BOOL KeyQueueManager::PushBack(keyMsg & msg)
{
	// 있는 키라면 넣지 않는다.
	for( int i = 0; i < m_MsgArray.Size(); i++ )
	{
		if( m_MsgArray[i].dwType == msg.dwType )
		{
			return FALSE;
		}
	}

	m_MsgArray.PushBack(msg);

	return TRUE;
}
BOOL KeyQueueManager::PushFront(keyMsg & msg)
{
    // 있는 키라면 넣지 않는다.
    for (int i = 0; i < m_MsgArray.Size(); ++i)
    {
        if (m_MsgArray[i].dwType == msg.dwType)
        {
            return FALSE;
        }
    }
    
    m_MsgArray.Insert(0, msg);

    return TRUE;
}

//------------------------------------------------------------------------------
/**
	해당 타입과 매치 하는 메시지를 지운다.키 입력 말고 특정 조건에 의해 메시지가 삭제 될때 
*/
BOOL KeyQueueManager::DeleteMsg(DWORD dwType)
{
	if( m_MsgArray.Empty() )
		return FALSE;

	for(int i=0;i<m_MsgArray.Size();i++)
	{
		if(m_MsgArray[i].dwType==dwType)
		{
			m_MsgArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
	메세지를 등록한다.
*/
BOOL KeyQueueManager::PushKeyMsg( eDoType DoType, fn_CallBack DoSomething, DWORD wParam, DWORD lParam )
{
	keyMsg msg;

	ZeroMemory(&msg,sizeof(msg));
	msg.dwType = DoType;
	msg.DoSomething = DoSomething;
	msg.wParam = wParam;
	msg.lParam = lParam;
	return PushBack( msg );
}
