//------------------------------------------------------------------------------
/**
월드 상의 오브젝트 특성을 정의 
@author

@since

@remarks
- SceneBase에 기본적으로 할일을 올려주는 작업 진행중. ( 재영 )
  . 유닛 process / Render
  . Object Process / Render
  . Packet Parse --> 각 Scene에서 Overide 하여 사용.

- 
*/
//=======================================================================================================================



#ifndef		_SceneBase_H
#define		_SceneBase_H

#include "GameFramework.h"

struct BASE_PLAYERINFO;
struct CHARINFO_FOR_PACKET;


#define MAX_SCENE_NAME	0xFF


#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "GameFunc.h"


enum
{
	IMG_LOADING_BACKGROUND = 0,
	IMG_LOADING_BAR,
	IMG_LOADING_BAR_START,
	IMG_LOADING_BAR_CENTER,
	IMG_LOADING_BAR_END,

	IMG_MAX_COUNT,
};

enum
{
    IMG_LOADING_KALBUS	= 1000, //!< 탑돌때는 마지막 이미지라고 생각한다.
};


enum	eFADE_STATE
{
	eFADE_STATE_NONE,
	eFADE_STATE_IN,
	eFADE_STATE_OUT,
};


class SceneBase
{
protected:
	BOOL		m_bEnableRender;	//	프로세스가 한번도 실행되지 않은 상태로 랜더링을 실행할 경우가 있다.
									//	그래서 프로세스가 실행될때 TRUE 로 셋팅되고,

									//	랜더링시 이 변수가 FALSE 이면 랜더링하지 않는다

	HANDLE				m_hTexture[IMG_MAX_COUNT]; // 로딩 화면 

	HANDLE	m_hLoadingGageTexture; // 로딩 게이지 

	HANDLE	m_hLoadingGageBackTexture; // 로딩 게이지 Back

	
protected:
	// Scene 상의 Global protected Func들을 등록하자 

public:

	SceneBase( void ) : m_SceneType(SCENE_TYPE_NONE), m_bEnableRender(FALSE),m_fLoadingCount(0.0f),m_fMaxCount(1.0f),m_bChangeImage(FALSE),m_bUsePlayerCollision(FALSE),m_eFadeState(eFADE_STATE_NONE)
	{ 
		InitLoadingImg();
	}
	virtual ~SceneBase( void ) {}

	BOOL IsEnablePkScene();
    BOOL InPVPScene();

	//	각 씬의 내용
	virtual void Activate( void ) {}						//	활성화 되었을 때
	virtual BOOL Init( void );
	virtual DWORD				AddBattleTimeSerial()	{ return 0;}
	virtual DWORD				GetBattleTimeSerial()   { return 0;}

	virtual BOOL Process( DWORD dwTick );					//	메인 계산
	virtual void StartFadeIn();
	
	virtual void SetViewport();                             //  뷰포트      
	virtual BOOL Render( DWORD dwTick );					//	메인 랜더링
	virtual BOOL PreInterfaceRender(DWORD dwTick);          //  인터페이스 이전 그릴것(2D)
	virtual BOOL PostInterfaceRender(DWORD dwTick);         //  인터페이스 이후 그릴것(2D)

	virtual void Release( void );							//	종료
    void         UserInterfaceManagersParsePacket( MSG_BASE * pMsg );
    void         UserInterfaceManagersParsePacketWorld (MSG_BASE * pMsg);
	virtual void ParsePacket( MSG_BASE * pMsg );			//  네트워크 패킷 처리
	virtual void ParsePacketInChat( MSG_BASE * pMsg );		//	체팅 서버 패킷 처리

    virtual BOOL PeekChangeScene();
    virtual void ChangeSceneReady(DWORD waitingSec, SCENE_TYPE sceneType);
    virtual void ClearSceneReady();
    virtual DWORD GetChangeSceneTickOver();
    virtual DWORD ProcessChangeSceneOverTick(DWORD dwTick);
    virtual DWORD GetCurrentTick();

	virtual BOOL  SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam ,SI_MESSAGE *stMessage);

    virtual void  OnConnectWorldServer() {}
	virtual void  OnDeviceReset();

    virtual void  PopupExp(INT64 /*expoffset*/, int bonusExpRatio = 0, int ExpType = 0 ) {}

	virtual void  IncreaseLoadingProcess();

	void	InitLoadingImg();

	void	LoadingImg( BOOL bIsChangeImage = TRUE ,int iForceIndex = -1);	//	0번째 배경 인덱스도 있기때문에 -1로 바꿉니다.

	void	LoadingCI( BOOL bIsChangeImage = TRUE );

	void	ReleaseLoadingImg();
	void	RenderLoadingImg();

	void	RenderLoadingGage();

	void	ChangeImage(bool bCheck, int iLoadingImg = -1)			{m_bChangeImage = bCheck; m_iForceIndex = iLoadingImg; }

	void    SetReadyDeadFlag(BOOL bFlag);    

	void	ShowBillingAlarm(DWORD dwRestTerm);
	void	ShowDisconnectAlarm(DWORD dwErrCode);
	void    loadInitMessage();
	void    EnablePlayerCollision(BOOL bUse){m_bUsePlayerCollision=bUse;}
	BOOL    IsEnablePlayerCollision(){return m_bUsePlayerCollision;}
	virtual void ProcessFade(DWORD dwTick);

	eFADE_STATE GetFadeState();

	const TCHAR *GetRevengeName(){ return m_ptszRevengeName;}

	SCENE_TYPE	 GetType() const
	{
		return m_SceneType;
	}

	DWORD		GetpacketStatus()
	{
		return m_packetStatus;
	}

	virtual int OnEndRendering( DWORD dwTick );

#ifdef __NA_0_20080415_PROCESS_END_PROECSS
	virtual int OnEndProcess( DWORD dwTick );
#endif


	virtual RELATIONKIND::TYPE GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy ) { return RELATIONKIND::Friend; }
	virtual void	InserCenterTextMessage( char * szText, DWORD eType ) {}
    virtual void    InsertKillMessage(
        TCHAR* first_user_id,
        DWORD first_user_color,
        TCHAR* second_user_id,
        DWORD second_user_color,
        TCHAR* format_string)  // 전장 킬 메세지
    {
    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    virtual void    InsertMonsterKillMessage(
        TCHAR* first_user_id,
        DWORD first_user_color,
        TCHAR* monster_id,
        DWORD second_user_color,
        TCHAR* format_string)  // 골드러시 킬 메세지
    {
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

private:
	void loadLoadingMessage();

protected:
	// 게임 서버 패킷 Proc
	virtual void OnRecvCG_CONNECTION( MSG_BASE * pMsg );
	virtual void OnRecvCG_CHARINFO( MSG_BASE * pMsg );
	virtual void OnRecvCG_MAP( MSG_BASE * pMsg );
	virtual void OnRecvCG_SYNC( MSG_BASE * pMsg );
	virtual void OnRecvCG_BATTLE( MSG_BASE * pMsg );
	virtual void OnRecvCG_SKILL( MSG_BASE * pMsg );
	virtual void OnRecvCG_CONVERSATION( MSG_BASE * pMsg );
	virtual void OnRecvCG_ETC( MSG_BASE * pMsg );
    virtual void OnRecvCG_GM( MSG_BASE * pMsg );
	virtual void OnRecvCG_Chao( MSG_BASE * pMsg );



	virtual void OnRecvCG_Party( MSG_BASE * pMsg);



	void OnRecvCG_ArmorKit(MSG_BASE * pMsg);


	// 채팅 서버 패킷 Proc
	virtual void OnRecvCW_CHAT( MSG_BASE * pMsg );
	virtual	void OnRecvCW_CRYPTOGRAPHY( MSG_BASE * pMsg );
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
	virtual void OnRecvCW_NOTIFICATION( MSG_BASE* message );
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM



	void OnRecvCG_CRYPTOGRAPHY(MSG_BASE * pMsg);


#ifdef _JBH_ADD_AGE_LIMIT_MARK
	virtual void	RenderGameRatingMark();
	virtual void	LoadingGameRatingMarkTexture();
	virtual void	ReleaseGameRatingMarkTexture();
#endif

	FTextParser		m_FTextParser;
	FTextSentence	m_FTextSentence;
	FTextRender		m_FTextRender;


	FTextParser		m_TipFTextPraser;
	FTextSentence	m_TipFTextSentence;
	FTextRender		m_TipFTextRender;

    // 4:3 비율 이미지를 기본으로 사용하고 wide 일경우에는 높이를 짤라서 사용한다.
    const static int        kLoadingImageWidth = 1024;
    const static int        kLoadingImageHeight = 768;

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	HANDLE	m_hGameRatingMarkTexture;
#endif //_JBH_ADD_AGE_LIMIT_MARK

	// Auth 서버 패킷 Proc

	// 큐매니저 proc

protected:
	virtual	void	OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_ACK( MSG_BASE * pMsg );
	virtual	void	OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_NAK( MSG_BASE * pMsg );


public:
	virtual void KeyManagerProc(DWORD wParam,DWORD lParam);

protected:
	SCENE_TYPE	m_SceneType;
    DWORD       m_packetStatus;             //패킷 상태 관리-지완 추가 
    BOOL        m_bChangeSceneReady;        //ChangeSceneReady
    DWORD       m_dwChangeSceneTick;        //ChangeSceneReady가 호출된 시점의 Tick
    DWORD       m_dwChangeSceneOverTick;    //
    DWORD       m_dwChangeSceneSecond;      //
    SCENE_TYPE  m_ChangeSceneType;          //

    DWORD       m_dwBaseTick;               // Tick
	BOOL		m_bReadyDeadFlag;
	TCHAR		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
	float		m_fLoadingCount;
	float       m_fMaxCount;
	BOOL        m_bUsePlayerCollision;      //플레이어간의 충돌 처리를 할것인가.패스 엔진이 적용된 맵만 가능함 
    DWORD       m_dwCurFadeTime;
	BOOL        m_bFristRender;
	BOOL		m_bChangeImage;
	eFADE_STATE m_eFadeState;

	int			m_iForceIndex;


private:
	// Attributes
	// ...
};
//------------------------------------------------------------------------------
/**
    각종 공용 초기화 
*/
inline BOOL SceneBase::Init() 
{
	m_dwCurFadeTime=0;
    m_packetStatus=0;
    m_bChangeSceneReady = FALSE;
    m_dwBaseTick = 0;
    m_dwChangeSceneTick = 0;
    m_dwChangeSceneOverTick = 0;
    m_dwChangeSceneSecond = 0;
	m_bReadyDeadFlag				= FALSE;
	m_bUsePlayerCollision =FALSE;	
	m_bFristRender=TRUE;
	ZeroMemory(m_ptszRevengeName,sizeof(m_ptszRevengeName));

    return TRUE; 
}

inline void SceneBase::SetReadyDeadFlag(BOOL bFlag)
{
	m_bReadyDeadFlag				= bFlag;
}
//------------------------------------------------------------------------------
/**
*/
inline BOOL SceneBase::PeekChangeScene()
{
    return m_bChangeSceneReady;
}

//------------------------------------------------------------------------------
/**
*/
inline void SceneBase::ChangeSceneReady(DWORD waitingSec, SCENE_TYPE sceneType)
{
    m_bChangeSceneReady = TRUE;
    m_dwChangeSceneTick = GetCurrentTick();
    m_dwChangeSceneOverTick = GetCurrentTick() + (waitingSec * 1000);
    m_ChangeSceneType = sceneType;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD SceneBase::GetChangeSceneTickOver()
{
    return m_dwChangeSceneOverTick;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD SceneBase::ProcessChangeSceneOverTick(DWORD dwTick)
{
    m_dwChangeSceneTick += dwTick;
    return m_dwChangeSceneTick;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD SceneBase::GetCurrentTick()
{
    return m_dwBaseTick;
}

//------------------------------------------------------------------------------
/**
*/
inline void SceneBase::ClearSceneReady()
{
    m_bChangeSceneReady = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline int SceneBase::OnEndRendering( DWORD /*dwTick*/ )
{
	return 0;
}


extern int g_iLoadingImage;


//#define _WRITE_SCENE_LOG
#ifdef _WRITE_SCENE_LOG
static void xxInitSceneLog(int t)
{
	if( FILE* fp = fopen("c:\\ddafds.txt", "w") )
	{
		fprintf(fp, "init\n");
		fclose(fp);
	}
};


static void xxWriteSceneLog( const char* message, ... )
{

	char buffer[2048];

	va_list args; 
	va_start(args, message); 
	_vsntprintf( buffer, 2048-1, message, args ); 
	va_end(args); 

	if( FILE* fp = fopen("c:\\ddafds.txt", "a") )
	{
		fprintf(fp, buffer);
		fprintf(fp, "\n");
		fclose(fp);
	}
};
#else
#define xxInitSceneLog
#define xxWriteSceneLog
#endif // _WRITE_SCENE_LOG



#endif	//	_SceneBase_H
