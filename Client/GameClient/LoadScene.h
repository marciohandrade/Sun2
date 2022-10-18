//=======================================================================================================================
/** �ε���
	@author
			���翵
	@since
			
	@remarks
	 - �� �ε�
	 - �ε��� Nexe Sceneó��
*/

#ifndef		_LOADSCENE_H
#define		_LOADSCENE_H

// 
// TODO : ���ҽ� �ε�� ���������� �Լ��� ���� (�׽�Ʈ �Լ��� �Ҽ�����..)
// comment by Jae young


#include "SceneBase.h"

#define MAX_LOADING_TEXT_LINE   20

class SceneBase;

class LoadScene : public SceneBase
{
public:
	enum LOADSCENE_STATE
	{

		LoadScene_State_NONE = 0,	// �ƹ��� ���� �غ� �ȵȻ��� �ʱ����
		LoadScene_State_Start,
		LoadScene_State_Completed,
	};
	MAPID				m_GoingToLobbyMapID;


public:
	LoadScene();
	~LoadScene();

	void				Activate();						//	Ȱ��ȭ �Ǿ��� ��
	BOOL				Init();							//	�ʱ�ȭ, �ε�
	BOOL				Process( DWORD dwTick );			//	���� ���
	BOOL				Render( DWORD dwTick );			//	���� ������
	void				Release();							//	����
	void				ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��
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
	// �����ͷε�
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
	
	// �ؽ�Ʈ ���
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


