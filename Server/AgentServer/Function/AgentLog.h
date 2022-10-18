#pragma once
//=======================================================================================================================
/// AgentLog
/**
	@history
			- 2006.5.15 : [taiyo] ���� ���� �� ������ ���ڿ� ó�� ����
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
	// ���� �α� 
	// �α��� ����
	//void	UserLoginFail( UserSession * pUser );
	// �α��� 
	void	UserLogin( UserSession * pUser );
#ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
    void	UserLogin( UserSession * pUser, const char* mac_address );
#endif
	// �α׾ƿ�
	void	UserLogout(  UserSession * pUserSession );
	// ĳ���� ����
	void	ConnectChar( User * pUser );
	// ĳ���� ��������
	void	DisconnectChar( User * pUser );
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    // ���ǵ��� �˻� �α�
    void LogSpeedHackSuspect(User* user, TCHAR* additional_info);
    // ���ǵ��� �˻� ������ ���� ���� ���� ����
    void LogSpeedHackSuspectDisconnection(User* user);
#endif
    void LogDominationFieldEnterCount(int log_type, User* user, BYTE count, MAPCODE map_code);

	//////////////////////////////////////////////////////////////////////////
	// �����׼� �α�
	// ĳ���� ���� / ����
	void CreateCharacter(const bool is_create, User* user, const TCHAR* char_name, const TCHAR* add_info);
	// ������ �̵�
	void	MoveVillage( User* pUser, WORD wFromMapCode, WORD wToMapCode, BOOL bRoom );

	void	TrackRevenge( User* pMoveUser, User* pTrackUser, WzVector &vCurPos, WzVector &vDestPos );

	void	Portal( User* pMoveUser, MAPCODE ToMapCode, WzVector &vCurPos, WzVector &vDestPos );
    void WriteCharRename(User* user, SLOTIDX slot_index, const TCHAR* pszName);

	//////////////////////////////////////////////////////////////////////////
	// GM�α�
	void	ReqCharInfo( User* pGM, MSG_CG_GM_CHARINFO_ACK& msg );
	void	DisconnectCmd( User* pGM, User* pFindUser );
	void	LogGM( int nLogType, User * pGM, User * pFindUser = NULL );

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    //////////////////////////////////////////////////////////////////////////
    // WZDAT �α�
    void LogWzDat( int log_type, const TCHAR* add_info = NULL );
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
};















