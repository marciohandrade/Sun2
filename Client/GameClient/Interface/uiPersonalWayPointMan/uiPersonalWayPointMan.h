#pragma once
//------------------------------------------------------------------------------
//		�۾���	:	������
//		uiPrivateWayPointMan class
//		�Ϻ� ���� ��������Ʈ �Ŵ���
//------------------------------------------------------------------------------

#include "uiBaseMan.h"

#include "boost/shared_ptr.hpp"

class uiPersonalWayPointWindow;
class SCItemSlot;

struct IM_PERSONAL_WAYPOINT_MANAGER 
{
	static const DWORD WAYPOINT_DIALOG;
};

class uiPersonalWayPointMan : public uiBaseMan
{
	/*
	- ó�� â�� ���� �����κ��� ��������Ʈ ��ü ������ �����´�
			- ��������Ʈ ���� ������ ����
			- ���� ���� true
			- ��������Ʈ ������ ���� �� â ���� ����( ������ ����, ���Ⱓ�� ���� ��Ȱ��ȭ )
			- â ���÷���

	- ���� â�� ���� ���� ���� true �̸� �ٷ� ���÷���
			- ���� ���� false �̰� ���� ��� ���̸� ���
			- �̿�ð��� �����ų�(������ ��) ��Ʋ���� ��� ��� ��ư ��Ȱ��ȭ
					{// ������ �ʵ����� üũ
						if (GameFramework::GetCurrentScene() && 
						(SCENE_TYPE_VILLAGE != GameFramework::GetCurrentScene()->GetType() &&
						SCENE_TYPE_FIELD != GameFramework::GetCurrentScene()->GetType() ) )
					}
			- ���� �ð����� ��  ������ư ��Ȱ��ȭ
	
	- ��������Ʈ ����
			- ��������Ʈ ����� 10�� �� ��� -> 10�� ���� �޽��� ���
			- ������ ��������Ʈ ���̺� �۽�
	
	- ��������Ʈ ����
			- ��������Ʈ ����� 0�� �� ��� -> ����
			- ���� �ð����� - �ð����� �޽��� ���
			- ������ ��������Ʈ ���� �۽�

	- �̵�
	- ����ڵ�

	//////////////////////////////////////////////////////////////////////////

	������ ���� �÷��� ( ���̾�α׿� ���� )
	��������Ʈ ���� ���̺�( ���̾�α׿� ���� )

	*/
public:
    uiPersonalWayPointMan(InterfaceManager *pUIMan);

public:
	virtual void			OnInitialize();

	void					InitWayPoint();
    
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
	void					_processErrorCode(DWORD dwErrorCode);
	void					SEND_CG_CHAR_WAYPOINT_INFO_LOAD_SYN();

	void					ShowWayPointWindow(bool bShow);

	SCItemSlot*				GetActiveWaypointItem();

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
    static void HotKeyTogglePersonalwarpDialog();
#endif   //_NA_000000_20121210_REFACTORY_GAME_USER

private:
    uiPersonalWayPointWindow*					m_pUIWayPoint;
};

