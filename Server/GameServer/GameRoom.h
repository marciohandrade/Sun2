#pragma once

#include "GameZone.h"
#include "RankingSystemCtl.h"

class GameRoom : public GameZone
{
protected:
	explicit GameRoom(void);
	void operator = (const GameRoom&);

public:
	virtual	~GameRoom(void);

	virtual	eZONETYPE	GetZoneType() = 0;
	virtual	void		SetZone( Object* pObject, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos ) = 0;

	virtual void		OnCreate();
	virtual void		OnDestroy();
    virtual void        Update( DWORD dwDeltaTick );

	virtual	void		LeaveForWarp( Player* pPlayer );
	virtual BOOL		MoveField( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );

	virtual	void		Ready( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos )=0;
	virtual	BOOL		Join( Player* pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector* pwvStartPos );
	virtual	void 		Leave( Player* pPlayer );

	inline	void		SetMasterUserKey( DWORD dwUserKey )		{ m_MasterUserKey = dwUserKey; }
	inline	DWORD		GetMasterUserKey()						{ return m_MasterUserKey; }
	virtual	void		ChangeMaster(DWORD dwNextMasterUserKey );

	virtual	float		GetBonusExp( Player* pPlayer );
	virtual	BOOL		CanApplySuperMasterBenefit( Player* pPlayer );

	inline const TCHAR*					GetRoomTitle()		{ return m_pszRoomTitle; }
	inline const BASE_ROOMINFO*			GetBaseRoomInfo()	{ return &m_RoomInfo; }
	virtual const ADDITIONAL_ROOMINFO*	GetAdditionalInfo()	{ static ADDITIONAL_ROOMINFO info; return &info; }
	virtual const ADDITIONAL_PVPINFO*	GetAdditionalPVPInfo()	{ static ADDITIONAL_PVPINFO info; return &info; }

	virtual	void		SetSpecialType( eROOM_SPECIAL_TYPE eType )	{ m_RoomInfo.m_SpecialType = static_cast<BYTE>(eType); }

	inline	void		SetRoomTitle(const TCHAR* title)			{ _tcsncpy(m_pszRoomTitle, title, MAX_ROOMTITLE_LENGTH*sizeof(TCHAR)); m_pszRoomTitle[MAX_ROOMTITLE_LENGTH]='\0'; }
	inline	void		SetRoomPWD(const TCHAR* pw)					{ _tcsncpy(m_pszRoomPWD, pw, MAX_ROOMPASSWORD_LENGTH*sizeof(TCHAR)); m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH]='\0'; }
	inline	void		SetBaseRoomInfo(const BASE_ROOMINFO* pInfo) { memcpy(&m_RoomInfo, pInfo, sizeof(BASE_ROOMINFO)); }

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	inline	RankingSystemCtl*	GetRankingSystemCtl()	{ return (Policyment.FIELD & Policyment.ENABLE_RANKING) ? m_pRankingSystemCtl : 0; }
#endif // _NA_008012_20150130_RANKING_SYSTEM

	virtual eUSER_RELATION_TYPE	IsFriendPVP( Player *pPlayer,Player *pTarget ) { __UNUSED((pPlayer, pTarget)); return USER_RELATION_NEUTRALIST; }

    //!~ _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    void SetPartyZoneType(PartyZoneType party_zone_type) { party_zone_type_ = party_zone_type; };
    PartyZoneType GetPartyZoneType() { return party_zone_type_; };
    //~!

private:	
    DWORD				m_MasterUserKey;	//< 방장
	TCHAR				m_pszRoomTitle[MAX_ROOMTITLE_LENGTH+1];
	TCHAR				m_pszRoomPWD[MAX_ROOMPASSWORD_LENGTH+1];
	BASE_ROOMINFO		m_RoomInfo;
	//	<ChangeHandler>
	//	(NOTE)	Specific Process Handler를 교체한다.

public:
	virtual	void				ChangeHandler( const eZONETYPE kindOfRoom, const eINSTANCE_DUNGEON_KIND kindOfIndun = INDUN_KIND_MAX );
    PartyZoneType party_zone_type_; //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

#ifdef _NA_008012_20150130_RANKING_SYSTEM
protected:	
    RankingSystemCtl*	m_pRankingSystemCtl;
#endif // _NA_008012_20150130_RANKING_SYSTEM
};


