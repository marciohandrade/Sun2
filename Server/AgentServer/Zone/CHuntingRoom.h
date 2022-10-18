#pragma once

#include "RoomInterface.h"
#include <map>

struct PLAYER_HISTORY
{
	DWORD		m_dwGUID;
	DWORD		m_dwJoinTime;		// ���￡ ������ �ð�
};

typedef STLX_MULTIMAP<DWORD, PLAYER_HISTORY>	PLAYER_HISTORY_MAP;
typedef std::pair<DWORD, PLAYER_HISTORY>		PLAYER_HISTORY_PAIR;
typedef PLAYER_HISTORY_MAP::iterator			PLAYER_HISTORY_ITER;


class CHuntingRoom : public RoomInterface
{
	friend class ZoneManager;

	_DECLARE_POOL(CHuntingRoom)
public:
	CHuntingRoom();
	~CHuntingRoom();

	virtual BOOL				Process();

	virtual VOID				ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode = 0, DWORD dwAreaID = 0, WzVector * pwvStartPos = NULL, bool bFirstJoin = false );

	virtual RC::eROOM_RESULT	Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession );
	VOID						SerializeLoad( CHUNTING_ROOMINFO & OUT rRoomInfo );

	inline virtual eZONETYPE	GetType()		{ return eZONETYPE_CHUNTING; }
	inline BOOL					IsCompetition()	{ return m_bCompetition; }
	inline VOID					SetCompetition( BOOL bCompetition )	{ m_bCompetition = bCompetition; }
	VOID						CompleteCompetition();

	VOID						EnterTeam( RoomInterface *pPreviousRoom, BOOL bCompetitionStart );

	VOID						SetLinkedLobby( BOOL bLink )	{ m_bLinkedLobby = bLink; }
	BOOL						GetLinkedLobby()				{ return m_bLinkedLobby; }

	WORD						GetPlayerHistoryNum( DWORD dwGUID );

	inline const ADDITIONAL_CHUNTINGINFO * GetAdditionalCHuntingInfo()	{ return &m_AdditionalInfo; }

	VOID						Link2ndParty( WORD wPartyKey )	{ m_Link2ndPartyKey = wPartyKey; }
	WORD						GetLink2ndParty()				{ return m_Link2ndPartyKey; }

protected:
	virtual VOID				On_create();
	virtual VOID				On_destroy();
	virtual VOID				On_readyuser( User * pUser );
	virtual VOID				On_enteruser( User * pUser );
	virtual VOID				On_leaveuser( User * pUser );
	virtual RC::eROOM_RESULT	_CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD );

	inline VOID SetAdditionalCHuntingInfo(const ADDITIONAL_CHUNTINGINFO * IN pInfo)	{ memcpy(&m_AdditionalInfo, pInfo, sizeof(m_AdditionalInfo)); }

private:
	VOID						CheckMinLimitUser();
	VOID						ExpelAll();	// ���� ��� �߹�

private:
	ADDITIONAL_CHUNTINGINFO		m_AdditionalInfo;
	BOOL						m_bCompetition;		// ���� �������ΰ�
	BOOL						m_bLinkedLobby;		// ��ũ�� �κ� �ֳ�

	PLAYER_HISTORY_MAP			m_mapPlayerHistory;		// ���� ���￡ ������ �÷��̾� ����

	WORD						m_Link2ndPartyKey;
};












