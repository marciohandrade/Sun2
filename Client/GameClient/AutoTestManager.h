/*!
*	\file		AutoTestManager.h
*	\brief		AutoTestManager class
*	\author		¿Ã¡ˆø¨ (kiki@webzen.co.kr)
*	\version	2009/07/02 first release
*/

#ifndef __AUTOTEST_MANAGER_H__
#define __AUTOTEST_MANAGER_H__

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 

#include <Singleton.h>

const int c_iMaxTestIDLen = 64;
const int c_iMaxTestNameLen = 32;

enum eTestCaseState
{
	eTCS_NONE	= 0,
	eTCS_TRY	= 1,
	eTCS_WAIT	= 2,
	eTCS_WORK	= 3,
	eTCS_DONE	= 4,
	eTCS_FAIL	= 5,
};

enum eTestCaseType
{
	eTCT_UNKNOWN			= 0,
	eTCT_GAME_LOADING		= 1,
	eTCT_LOGIN				= 2,
	eTCT_SELECT_SERVER		= 3,
    eTCT_START_GAME			= 4,
    eTCT_PATCH_SERVER       = 5,
	eTCT_GO_TO_MAP			= 6,
	eTCT_OPEN_INVEN			= 7,
	eTCT_CLOSE_INVEN		= 8,
	eTCT_GO_TO_BATTLE_NPC	= 9,
	eTCT_OPEN_BATTLEZONE	= 10,
	eTCT_TRY_CREATE_ROOM	= 11,
	eTCT_MAKE_MISSION		= 12,
	eTCT_START_MISSION		= 13,
	eTCT_LEAVE_MISSION		= 14,
	eTCT_MAKE_HUNTING		= 15,
	eTCT_START_HUNTING		= 16,
	eTCT_LEAVE_HUNTING		= 17,
	eTCT_LOGOUT				= 18,
	eTCT_LAST_DATA			= 19,
};
typedef struct _TESTCASE
{
	UINT	iStepIndex;
	TCHAR	tszTestName[c_iMaxTestNameLen];
	eTestCaseType eTestType;
	eTestCaseState eTestState;
	DWORD	dwParam1;
    char test_string[MAX_PATH];

} TESTCASE;

class AutoTestManagerEx : public util::Singleton<AutoTestManagerEx>
{
public:
    bool m_IsActive;

    struct sAutoLogin
    {
        bool m_Enable;
        std::string m_Account;
        std::string m_ServerName;
        std::string m_Passward;
        std::string m_CharacterName;
    };

    struct sCommand
    {
        bool m_Enable;
        std::vector<std::string> m_CommandArray;
    };

    sAutoLogin m_AutoLogin;
    sCommand m_Command;


    AutoTestManagerEx(){}

    bool LoadTestCase();

    void RunStartGame();
    void RunLogin();
    void RunSelectServer();

    void RunAutoCommand();

    bool IsAutoLogin() { return m_AutoLogin.m_Enable; }
    void SetCompliteLogin() { m_AutoLogin.m_Enable = false; }

    bool IsAutoCommand() { return m_Command.m_Enable; }
    void SetCompliteCommand() { m_Command.m_Enable = false; }

    void SetActive( bool IsActive ) { m_IsActive = IsActive; }
    bool IsActive() { return m_IsActive; }

};

#endif
#endif	//__AUTOTEST_MANAGER_H__