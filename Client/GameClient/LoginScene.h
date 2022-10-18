#ifndef		_LOGINSCENE_H
#define		_LOGINSCENE_H
//------------------------------------------------------------------------------
/**
    @class LoginScene

    �α��� ��
    ���������� ���ؼ� Ŭ���̾�Ʈ �α����� �����ϸ�,
    ���� ������ �����Ѵ�.

    (C) 2005 ckbang
*/
#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include  "SceneBase.h"

const int MAX_LOGIN_ANIMATION = 48;

class LoginScene : public SceneBase
{
public:

	LoginScene();
	virtual ~LoginScene();

	virtual void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL Init();							//	�ʱ�ȭ, �ε�
	virtual BOOL Process( DWORD dwTick );			//	���� ���
	virtual void SetViewport();
	virtual BOOL Render( DWORD dwTick );			//	���� ������
	virtual void Release();							//	����
	virtual void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	void RenderSunAnimation(DWORD dwTick);

    BOOL IsConnectedToAuthServer();
    BOOL IsReadyForQuery();
    void SetPendingConfrim(BOOL bConfirm);
    void SetConnectedToAuthServer();
    DWORD GetEncKey();                              /// �������� ��ȣȭ Ű
    WORD* GetServerStatusLists();
    BYTE  GetServerCounts();

    void Reconnect();                               /// ������
    void Disconnect();                              /// �������� ��������
	BOOL Connect();                                 /// �������� ����

	void LoadSunAnimation();
	void ReleaseSunAnimation();

	BOOL PreInterfaceRender(DWORD dwTick);
	BOOL PostInterfaceRender(DWORD dwTick);

    void SetLoginProgressState(int state ) { m_LoginProgressState = state; }
    int GetLoginProgressState() { return m_LoginProgressState; }
    int m_LoginProgressState;

protected:
    void OnRecvCL_Auth( MSG_BASE *pMsg );           /// ���� ��Ŷ ����ó��

protected:
    void CallBack_Confirm(bool bYes);               /// ���� �ݹ�

protected:
    BOOL    isConnected;                            /// ���������� �����ߴ°�
    BOOL    isConfirmed;                            /// ���������� ���� ��Ŷ�� ������ �Ǵ°��� �㰡�� �޾Ҵ°�
    int     accountTries;                           /// ���� ������ ���, �� ������ ��õ� �ϰ� Ƚ�� �ʰ��� ���, ������ ���� �մϴ�.

    BOOL    bReconnect;                             /// ������ Ÿ�̸�
    BOOL    bReconnectTimer;                        ///
    DWORD   dwReconnectTick;                        ///
    DWORD   dwReconnectTickOver;                    ///

    BOOL    onceAutoConnectionTry;                  /// �ڵ� �α��� �׽�Ʈ
    DWORD   dwEncKey;                               /// �������� ��ȣȭ Ű

    BYTE    bySvrCnt;                               // ���� (ä��) �� ����
    WORD    szServerStatusList[MAX_SVR];            // ���� (ä��) ���� ���

    BOOL    bSceneInitialized;
	HANDLE handle; 

	int		save_mouse_pointer_x_;					//���콺 ����Ʈ �����
	int		save_mouse_pointer_y_;					//������ ����Ʈ �����
	float	delay_time_mouseMove_;					//���ð�
	bool	same_position_to_mouse_;				//���콺�� ���� ��ǥ�� ���� (post - now)
	bool    off_ui_login_;
};


extern	LoginScene		g_LoginScene;

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
LoginScene::IsReadyForQuery()
{
    if (!isConfirmed)
        return FALSE;

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
LoginScene::SetPendingConfrim(BOOL bConfirm)
{
    this->isConfirmed = bConfirm;
}

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
LoginScene::IsConnectedToAuthServer()
{
    return isConnected;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
LoginScene::GetEncKey()
{
    return dwEncKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
LoginScene::GetServerCounts()
{
    return this->bySvrCnt;
}

//------------------------------------------------------------------------------
/**
*/
inline
WORD*
LoginScene::GetServerStatusLists()
{
    return &this->szServerStatusList[0];
}

//------------------------------------------------------------------------------
/**
*/
inline
void
LoginScene::SetConnectedToAuthServer()
{
    this->isConnected = true;
}

//------------------------------------------------------------------------------
#endif	//	_LOGINSCENE_H


