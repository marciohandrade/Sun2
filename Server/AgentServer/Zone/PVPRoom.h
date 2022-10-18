#pragma once
//=============================================================================================================================
/// PVPRoom 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 12. 6
	@remark
		- PVP방에 대한 존 정보 관리
	@note
		- 
	@history 
		- 
	@usage
		- 
*/
//=============================================================================================================================

#include "roominterface.h"

class PVPRoom : public RoomInterface
{
	_DECLARE_POOL(PVPRoom)

public:
	PVPRoom(void);

	virtual ~PVPRoom(void);

    virtual eZONETYPE GetType() {return eZONETYPE_PVP;};

	virtual RC::eROOM_RESULT Migrate( 
        KEYTYPE key, 
        RoomInterface* pPreviousRoom, 
        ServerSession* pServerSession 
        );

    virtual void ReadyUser(
        User* pUser, 
        eSEND_TYPE type,
        MAPCODE field_code = 0, 
        DWORD areaID = 0, 
        WzVector* pwvStartPos = NULL, 
        bool bFirstJoin = false
        );

    void Link2ndParty(WORD wPartyKey);

    WORD GetLink2ndParty();
    
    bool IsPVPTeamMode();

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    RC::eROOM_RESULT Create(
        User* user, 
        MAPCODE map_code, 
        KEYTYPE mission_key, 
        ServerSession* server_session, 
        const ADDITIONAL_PVPINFO* additional_pvp_info
        );

protected:
	virtual void On_create();
	
    virtual void On_destroy();
	
    virtual void On_readyuser(
        User* pUser
        );

	virtual void On_enteruser(
        User* pUser
        );

	virtual void On_leaveuser(
        User* pUser
        );

	virtual RC::eROOM_RESULT _CanJoinEach(
        User* pUser,
        eZONEPUBLIC RoomPublic,
        TCHAR* pszRoomPWD
        );

	void SetAdditionalPVPInfo(
        const ADDITIONAL_PVPINFO* IN pInfo
        );

	const ADDITIONAL_PVPINFO* GetAdditionalPVPInfo();

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    RC::eROOM_RESULT _SetBaseRoomInfo(
        User* user, 
        BASE_ROOMINFO& base_room_info, 
        MAPCODE map_code
        );

    RC::eROOM_RESULT _CanCreate(
        User* user, 
        MAPCODE map_code, 
        const BASE_ROOMINFO& base_room_info, 
        eZONETYPE zone_type
        );

    bool _SendPVPInfo(User* user);

#if defined( __NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)
    RC::eROOM_RESULT CanJoinGuildPVP(
        User* pUser
        );

    bool IsSpecialType(
        eROOM_SPECIAL_TYPE SpecialType
        );
#endif

private:
	ADDITIONAL_PVPINFO			m_AdditionalPVPInfo;
	WORD						m_Link2ndPartyKey;
};

inline void
PVPRoom::Link2ndParty(WORD wPartyKey)	
{ 
    m_Link2ndPartyKey = wPartyKey; 
}

inline bool
PVPRoom::IsPVPTeamMode()
{ 
    return (m_AdditionalPVPInfo.m_Mode == ePVP_TEAM_MODE); 
}

inline WORD 
PVPRoom::GetLink2ndParty()
{ 
    return m_Link2ndPartyKey;
}

#if defined( __NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)
inline bool 
PVPRoom::IsSpecialType(eROOM_SPECIAL_TYPE SpecialType)
{ 
    return m_RoomInfo.IsSpecialType( SpecialType ); 
}
#endif

inline void 
PVPRoom::SetAdditionalPVPInfo(const ADDITIONAL_PVPINFO * IN pInfo)	
{ 
    memcpy(&m_AdditionalPVPInfo, pInfo, sizeof(m_AdditionalPVPInfo)); 
}

inline const ADDITIONAL_PVPINFO*
PVPRoom::GetAdditionalPVPInfo()
{ 
    return &m_AdditionalPVPInfo; 
}










