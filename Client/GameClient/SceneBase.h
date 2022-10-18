//------------------------------------------------------------------------------
/**
���� ���� ������Ʈ Ư���� ���� 
@author

@since

@remarks
- SceneBase�� �⺻������ ������ �÷��ִ� �۾� ������. ( �翵 )
  . ���� process / Render
  . Object Process / Render
  . Packet Parse --> �� Scene���� Overide �Ͽ� ���.

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
    IMG_LOADING_KALBUS	= 1000, //!< ž������ ������ �̹������ �����Ѵ�.
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
	BOOL		m_bEnableRender;	//	���μ����� �ѹ��� ������� ���� ���·� �������� ������ ��찡 �ִ�.
									//	�׷��� ���μ����� ����ɶ� TRUE �� ���õǰ�,

									//	�������� �� ������ FALSE �̸� ���������� �ʴ´�

	HANDLE				m_hTexture[IMG_MAX_COUNT]; // �ε� ȭ�� 

	HANDLE	m_hLoadingGageTexture; // �ε� ������ 

	HANDLE	m_hLoadingGageBackTexture; // �ε� ������ Back

	
protected:
	// Scene ���� Global protected Func���� ������� 

public:

	SceneBase( void ) : m_SceneType(SCENE_TYPE_NONE), m_bEnableRender(FALSE),m_fLoadingCount(0.0f),m_fMaxCount(1.0f),m_bChangeImage(FALSE),m_bUsePlayerCollision(FALSE),m_eFadeState(eFADE_STATE_NONE)
	{ 
		InitLoadingImg();
	}
	virtual ~SceneBase( void ) {}

	BOOL IsEnablePkScene();
    BOOL InPVPScene();

	//	�� ���� ����
	virtual void Activate( void ) {}						//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL Init( void );
	virtual DWORD				AddBattleTimeSerial()	{ return 0;}
	virtual DWORD				GetBattleTimeSerial()   { return 0;}

	virtual BOOL Process( DWORD dwTick );					//	���� ���
	virtual void StartFadeIn();
	
	virtual void SetViewport();                             //  ����Ʈ      
	virtual BOOL Render( DWORD dwTick );					//	���� ������
	virtual BOOL PreInterfaceRender(DWORD dwTick);          //  �������̽� ���� �׸���(2D)
	virtual BOOL PostInterfaceRender(DWORD dwTick);         //  �������̽� ���� �׸���(2D)

	virtual void Release( void );							//	����
    void         UserInterfaceManagersParsePacket( MSG_BASE * pMsg );
    void         UserInterfaceManagersParsePacketWorld (MSG_BASE * pMsg);
	virtual void ParsePacket( MSG_BASE * pMsg );			//  ��Ʈ��ũ ��Ŷ ó��
	virtual void ParsePacketInChat( MSG_BASE * pMsg );		//	ü�� ���� ��Ŷ ó��

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

	void	LoadingImg( BOOL bIsChangeImage = TRUE ,int iForceIndex = -1);	//	0��° ��� �ε����� �ֱ⶧���� -1�� �ٲߴϴ�.

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
        TCHAR* format_string)  // ���� ų �޼���
    {
    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    virtual void    InsertMonsterKillMessage(
        TCHAR* first_user_id,
        DWORD first_user_color,
        TCHAR* monster_id,
        DWORD second_user_color,
        TCHAR* format_string)  // ��巯�� ų �޼���
    {
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

private:
	void loadLoadingMessage();

protected:
	// ���� ���� ��Ŷ Proc
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


	// ä�� ���� ��Ŷ Proc
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

    // 4:3 ���� �̹����� �⺻���� ����ϰ� wide �ϰ�쿡�� ���̸� ©�� ����Ѵ�.
    const static int        kLoadingImageWidth = 1024;
    const static int        kLoadingImageHeight = 768;

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	HANDLE	m_hGameRatingMarkTexture;
#endif //_JBH_ADD_AGE_LIMIT_MARK

	// Auth ���� ��Ŷ Proc

	// ť�Ŵ��� proc

protected:
	virtual	void	OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_ACK( MSG_BASE * pMsg );
	virtual	void	OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_NAK( MSG_BASE * pMsg );


public:
	virtual void KeyManagerProc(DWORD wParam,DWORD lParam);

protected:
	SCENE_TYPE	m_SceneType;
    DWORD       m_packetStatus;             //��Ŷ ���� ����-���� �߰� 
    BOOL        m_bChangeSceneReady;        //ChangeSceneReady
    DWORD       m_dwChangeSceneTick;        //ChangeSceneReady�� ȣ��� ������ Tick
    DWORD       m_dwChangeSceneOverTick;    //
    DWORD       m_dwChangeSceneSecond;      //
    SCENE_TYPE  m_ChangeSceneType;          //

    DWORD       m_dwBaseTick;               // Tick
	BOOL		m_bReadyDeadFlag;
	TCHAR		m_ptszRevengeName[MAX_CHARNAME_LENGTH+1];
	float		m_fLoadingCount;
	float       m_fMaxCount;
	BOOL        m_bUsePlayerCollision;      //�÷��̾�� �浹 ó���� �Ұ��ΰ�.�н� ������ ����� �ʸ� ������ 
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
    ���� ���� �ʱ�ȭ 
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
