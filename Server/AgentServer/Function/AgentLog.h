#pragma once
//=======================================================================================================================
/// AgentLog
/**
	@history
			- 2006.5.15 : [taiyo] 버그 수정 및 위험한 문자열 처리 수정
*/
//=======================================================================================================================

#include <BaseLog.h>
#include <SessionData.h>
#include <ActionData.h>
#include <AdminData.h>
#include <UserSessions/User.h>
#include <PacketStruct_CG.h>

class AgentLog : public BaseLog
{
public:
	AgentLog();
    virtual ~AgentLog() {}

private:
	SessionData		m_SessionData;	
	ActionData		m_ActionData;
	AdminData		m_AdminData;
	TCHAR			m_pTmpBuffer[MAX_ADDINFO_SIZE+1];
	void			SetRoomKey( SessionData & rSessionData, UserSession * pUserSession );
public:
	//////////////////////////////////////////////////////////////////////////
	// 세션 로그 
	// 로그인 실패
	//void	UserLoginFail( UserSession * pUser );
	// 로그인 
	void	UserLogin( UserSession * pUser );
#ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
    void	UserLogin( UserSession * pUser, const char* mac_address );
#endif
	// 로그아웃
	void	UserLogout(  UserSession * pUserSession );
	// 캐릭터 접속
	void	ConnectChar( User * pUser );
	// 캐릭터 접속해제
	void	DisconnectChar( User * pUser );
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    // 스피드핵 검사 로그
    void LogSpeedHackSuspect(User* user, TCHAR* additional_info);
    // 스피드핵 검사 판정에 의한 강제 접속 종료
    void LogSpeedHackSuspectDisconnection(User* user);
#endif
    void LogDominationFieldEnterCount(int log_type, User* user, BYTE count, MAPCODE map_code);

	//////////////////////////////////////////////////////////////////////////
	// 유저액션 로그
	// 캐릭터 생성 / 삭제
	void CreateCharacter(const bool is_create, User* user, const TCHAR* char_name, const TCHAR* add_info);
	// 마을로 이동
	void	MoveVillage( User* pUser, WORD wFromMapCode, WORD wToMapCode, BOOL bRoom );

	void	TrackRevenge( User* pMoveUser, User* pTrackUser, WzVector &vCurPos, WzVector &vDestPos );

	void	Portal( User* pMoveUser, MAPCODE ToMapCode, WzVector &vCurPos, WzVector &vDestPos );
    void WriteCharRename(User* user, SLOTIDX slot_index, const TCHAR* pszName);

	//////////////////////////////////////////////////////////////////////////
	// GM로그
	void	ReqCharInfo( User* pGM, MSG_CG_GM_CHARINFO_ACK& msg );
	void	DisconnectCmd( User* pGM, User* pFindUser );
	void	LogGM( int nLogType, User * pGM, User * pFindUser = NULL );

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    //////////////////////////////////////////////////////////////////////////
    // WZDAT 로그
    void LogWzDat( int log_type, const TCHAR* add_info = NULL );
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
};















