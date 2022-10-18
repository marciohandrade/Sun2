#ifndef		_LOGINSCENE_H
#define		_LOGINSCENE_H
//------------------------------------------------------------------------------
/**
    @class LoginScene

    로그인 씬
    인증서버를 통해서 클라이언트 로그인을 성공하면,
    게임 서버로 접속한다.

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

	virtual void Activate();						//	활성화 되었을 때
	virtual BOOL Init();							//	초기화, 로딩
	virtual BOOL Process( DWORD dwTick );			//	메인 계산
	virtual void SetViewport();
	virtual BOOL Render( DWORD dwTick );			//	메인 랜더링
	virtual void Release();							//	종료
	virtual void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	void RenderSunAnimation(DWORD dwTick);

    BOOL IsConnectedToAuthServer();
    BOOL IsReadyForQuery();
    void SetPendingConfrim(BOOL bConfirm);
    void SetConnectedToAuthServer();
    DWORD GetEncKey();                              /// 인증서버 암호화 키
    WORD* GetServerStatusLists();
    BYTE  GetServerCounts();

    void Reconnect();                               /// 재접속
    void Disconnect();                              /// 인증서버 접속해제
	BOOL Connect();                                 /// 인증서버 접속

	void LoadSunAnimation();
	void ReleaseSunAnimation();

	BOOL PreInterfaceRender(DWORD dwTick);
	BOOL PostInterfaceRender(DWORD dwTick);

    void SetLoginProgressState(int state ) { m_LoginProgressState = state; }
    int GetLoginProgressState() { return m_LoginProgressState; }
    int m_LoginProgressState;

protected:
    void OnRecvCL_Auth( MSG_BASE *pMsg );           /// 인증 패킷 종합처리

protected:
    void CallBack_Confirm(bool bYes);               /// 인증 콜백

protected:
    BOOL    isConnected;                            /// 인증서버에 접속했는가
    BOOL    isConfirmed;                            /// 인증서버에 다음 패킷을 보내도 되는가의 허가를 받았는가
    int     accountTries;                           /// 인증 오류인 경우, 세 번까지 재시도 하고 횟수 초과인 경우, 연결을 종료 합니다.

    BOOL    bReconnect;                             /// 재접속 타이머
    BOOL    bReconnectTimer;                        ///
    DWORD   dwReconnectTick;                        ///
    DWORD   dwReconnectTickOver;                    ///

    BOOL    onceAutoConnectionTry;                  /// 자동 로그인 테스트
    DWORD   dwEncKey;                               /// 인증서버 암호화 키

    BYTE    bySvrCnt;                               // 월드 (채널) 총 개수
    WORD    szServerStatusList[MAX_SVR];            // 월드 (채널) 상태 목록

    BOOL    bSceneInitialized;
	HANDLE handle; 

	int		save_mouse_pointer_x_;					//마우스 포인트 저장용
	int		save_mouse_pointer_y_;					//마무스 포인트 저장용
	float	delay_time_mouseMove_;					//대기시간
	bool	same_position_to_mouse_;				//마우스가 같은 좌표에 있음 (post - now)
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


