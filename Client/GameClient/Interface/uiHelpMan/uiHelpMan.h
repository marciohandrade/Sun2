#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiHelpMan class
//------------------------------------------------------------------------------



#include "HelpInfoParser.h"
#include "uiBaseMan.h"


#define STRSAFE_NO_DEPRECATE
#include "../MemoryINI/AKMemoryIni/MemoryIni.h"

const int	c_iMaxHelpIcon						= 3;


const WzID c_wiHelpMissionFont = StrToWzID("st10");

//-----------------------------------------------------------------------------
// 도움말 (아이콘으로 추가되는가, 하단 알림창인가)
//-----------------------------------------------------------------------------
enum HELP_METHOD
{
	eHM_ICON		= 0,
	eHM_ANNOUNCE,
};

//-----------------------------------------------------------------------------

class uiHelpMenu;
class uiHelpWindow;
class uiHelpAnnounceWindow;
class uiHelpMission;

class uiHelpMonsterInfo; // 싱글모드시 몬스터 이동 및 스킬 보기 기능

class uiHelpMissionTimer;

struct IM_HELP_MANAGER
{
	static const DWORD HELP_MENU_DLG;			
	static const DWORD HELP_WINDOW_DLG;		
	static const DWORD HELP_ANNOUNCE_DLG;	
	static const DWORD HELP_MONSTER_DLG;	
	static const DWORD HELP_MISSION_DLG;
    static const DWORD HELP_MISSION_TIMER_DLG;
};

struct stHelpMissionInfo
{
	int		iTextID;
	DWORD	dwNCode;
	DWORD	dwDCode;
	DWORD	dwImgCode;
};

typedef std::vector<stHelpMissionInfo>		HELP_MISSION_VEC;


struct stHelpReadInfo
{
     DWORD		m_dw_HelpIndex;
	 DWORD		m_dw_Repeat;
};

class uiHelpMan : public uiBaseMan
{
public:
    uiHelpMan(InterfaceManager *pUIMan);
    virtual ~uiHelpMan();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
	virtual	void			OnInitialize();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:

	void					CheckHelpMessage(int iCharStatus, int iParam = 0, BOOL bIsLimited = FALSE);
    void					InitHelpMenuList();
	void					AddNewHelpIndex(DWORD dwHelpIndex);
	void					SetPosition();
	
	void					AddReadHelpIndex(DWORD dwHelpIndex,  BOOL bSave);
	void					SetHelpIndex(DWORD dwHelpIndex, int iCount);
	int						GetReadHelpCount(DWORD dwHelpIndex);
	void					ResetReadInfo(TCHAR* pCharName);
	void					LoadReadInfo();
	void					SaveReadInfo(DWORD dwIndex, int iCount);

	int						GetNewHelpIndex(DWORD dwHelpIndex);
	

	void					ShowHelpMenu(BOOL bShow, BOOL bForce = FALSE);
	void					ShowHelpWindow(int iBtnIndex);					
	void					ShowHelpWindowForIndex(DWORD dwHelpIndex);
#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
	void					ShowHelpTest(DWORD dwHelpIndex);
	void					ShowHelpTestStringCode(DWORD dwStringCode);

	void					ShowHelpAnnounce(DWORD dwHelpIndex,BOOL bIgnoreOption = FALSE);
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

	void					ShowGMMsg(DWORD dwHelpIndex, int iParam = 0, bool bShutDown = false);	// GM 메시지 출력용 함수

	void					ClearHelpMission();
	void					HideHelpMission();
	void					AddHelpMission(int iTextID, DWORD dwNCode, DWORD dwDCode, DWORD dwImgCode);
	void					DelHelpMission(int iTextID);
	void					UpdateHelpMission(int iNewItemTextID = -1);	//

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
#else
	void					ShowQuestHelp(int iParam,eHELP_KIND eKind);
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

	void					ShowHelpMonsterInfo(BOOL bVal); // 싱글모드시 몬스터 이동 및 스킬 보기 기능

#ifdef _SCJ_REMOVE_ANNOUNCE_WINDOW
  #else
    void                    RestoreHelpInfo();
#endif //_SCJ_REMOVE_ANNOUNCE_WINDOW

    void SetMissionTimer(DWORD require_tIme, BYTE progress_type);

private:
	void					_addHelp(int iCharStatus, int iParam = 0);
	void					_addNewHelpVec(DWORD dwHelpIndex);




    //--------------------------------------------------------------------------
    // procedure
    //
public:
    //--------------------------------------------------------------------------
    // member vars
    //
public:
	std::map<DWORD, stHelpReadInfo>		m_mapHelpInfo;
	BOOL								m_bLoadInfo;
	std::vector<DWORD>		m_vecNewHelp;

	HELP_MISSION_VEC	m_vecHelpMissionInfo;
	
private:
    uiHelpMenu*				m_pUIHelpMenu;
	uiHelpWindow*			m_pUIHelpWindow;
	uiHelpAnnounceWindow*	m_pUIHelpAnnounceWindow;
	uiHelpMission*			m_pUIHelpMission; 

	uiHelpMonsterInfo*		m_pUIHelpMonsterInfo;   // 싱글모드시 몬스터 이동 및 스킬 보기 기능

    uiHelpMissionTimer* help_mission_timer_;
	TCHAR					m_szFileName[512];
	MemoryIni				m_MemoryINI;
};

