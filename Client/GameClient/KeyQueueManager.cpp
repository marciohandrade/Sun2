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
	Ư�� Ű�� �������� ��ϵ� �޽������� ó�� �ϰ� �޽����� ���� ��� �⺻ �ൿ�� �Ѵ�.
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
	�޽����� ��� �Ѵ�.
*/
BOOL KeyQueueManager::PushBack(keyMsg & msg)
{
	// �ִ� Ű��� ���� �ʴ´�.
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
    // �ִ� Ű��� ���� �ʴ´�.
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
	�ش� Ÿ�԰� ��ġ �ϴ� �޽����� �����.Ű �Է� ���� Ư�� ���ǿ� ���� �޽����� ���� �ɶ� 
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
	�޼����� ����Ѵ�.
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
