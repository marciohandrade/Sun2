//=======================================================================================================================
/** 로딩씬
	@author
			유재영
	@since
			
	@remarks
	 - 멥 로딩
	 - 로딩후 Nexe Scene처리
*/

#ifndef		_LOADSCENE_H
#define		_LOADSCENE_H

// 
// TODO : 리소스 로드시 인증관련한 함수를 만듬 (테스트 함수라 할수있음..)
// comment by Jae young


#include "SceneBase.h"

#define MAX_LOADING_TEXT_LINE   20

class SceneBase;

class LoadScene : public SceneBase
{
public:
	enum LOADSCENE_STATE
	{

		LoadScene_State_NONE = 0,	// 아무런 동작 준비가 안된상태 초기상태
		LoadScene_State_Start,
		LoadScene_State_Completed,
	};
	MAPID				m_GoingToLobbyMapID;


public:
	LoadScene();
	~LoadScene();

	void				Activate();						//	활성화 되었을 때
	BOOL				Init();							//	초기화, 로딩
	BOOL				Process( DWORD dwTick );			//	메인 계산
	BOOL				Render( DWORD dwTick );			//	메인 랜더링
	void				Release();							//	종료
	void				ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리
	void				RenderLoadingText();
	virtual void        IncreaseLoadingProcess();

public:

	LOADSCENE_STATE		GetLoadState(void) const	{ return m_eLoadState;	}


	MAPID				GetGoingToMapID() const;
	MAPID				GetGoingToFieldID() const;

    void				SetNextSceneAfterLoaded( SCENE_TYPE eNextSceneType, MAPID MapID, FIELDID FieldID );
    void                SetNextSceneAfterLoaded( MAPID MapID, FIELDID FieldID );

	SCENE_TYPE			GetNextSceneAfterLoaded()	{ return m_eNetxtSceneAfterLoaded;}

	void				SetGoingToMovePos(const WzVector * pWzVector);
    void				UpdateGoingToMovePos();

    void				SetGoingToAreaPos(DWORD dwAreaID);
    void				UpdateGoingToAreaPos();

	void				InitText();
	void				ReleaseText();
	void				LoadText();
	void				LoadTextString(int index);
#ifdef _NHN_USA_CHANNELING
	int					GetRePortZone(){return m_nReportZone;}
	void				SetPortalFlag(BOOL bFlag){m_bPortalFlag = bFlag;}
#endif//_NHN_USA_CHANNELING

protected:
	// 데이터로딩
	void				LoadCharacter();
	void				LoadMonster();
	BOOL				LoadUI() {}
	BOOL				LoadNPC() {}

	void				LoadMap();
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	void                LoadStatue();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

protected:
	void				OnRecvCG_CONNECTION( MSG_BASE * pMsg );
	void				OnRecvCG_CHARINFO( MSG_BASE * pMsg );
	void				OnRecvCG_MAP( MSG_BASE * pMsg );
	void				OnRecvCG_SYNC( MSG_BASE * pMsg );
	void				OnRecvCG_CONVERSATION( MSG_BASE * pMsg );
	
	// 텍스트 출력
protected:
	void				RenderText();

    void				SetGoingToMap(MAPID MapID, FIELDID FieldID);
	void				SetLoadState(LOADSCENE_STATE eState){ m_eLoadState = eState; }
	
#ifdef _NA_000000_20140520_MAPLOAD_FAIL_TO_FTP
    void                SendMapLoadFailMessage();
    void                WriteUserInfo(FILE* file_pointer);
    void                UserEncryptSystem(const char* Key,size_t KeyLength,char* InputData,char* OutputData);
#endif //_NA_000000_20140520_MAPLOAD_FAIL_TO_FTP

private:
	ENUM_ALPHABLEND		m_eAlphablendMode;

	BOOL				m_bPacketSend;

	LOADSCENE_STATE		m_eLoadState;
	MAPID				m_GoingToMapID;
	FIELDID				m_GoingToFieldID;
	WzVector			m_WZVGoingToMapPos;
    BOOL                m_bGoingToMapPos;
    BOOL                m_bGoingToAreaPos;
    DWORD               m_GoingToAreaCode;

	SCENE_TYPE			m_eNetxtSceneAfterLoaded;
	BASE_PLAYERINFO		m_GoingToCreateHero;
	
	int                 m_iMaxTextLine;	
	TCHAR               **m_ppText;
	BOOL                m_bLoadMap;
private:
	//	CRITICAL_SECTION		m_cs;
	HANDLE				m_hThread;
	unsigned			m_wthreadID;
	BYTE				m_byLoadStart;
	float				m_fPercentStep;
	int					m_iDelayLoadCnt;
#ifdef _NHN_USA_CHANNELING
	int					m_nReportZone;
	BOOL				m_bPortalFlag;
#endif//_NHN_USA_CHANNELING
};

extern	LoadScene		g_LoadScene;

#endif	//	_LOADSCENE_H


