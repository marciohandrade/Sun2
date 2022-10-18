#pragma once
//=============================================================================================================================
/// MissionRoom 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 9. 20
	@remark
		- 미션방에 대한 존 정보 관리
	@note
		- 
	@history 
		- 
	@usage
		- 
*/
//=============================================================================================================================

#include "roominterface.h"

class MissionRoom : public RoomInterface
{
	_DECLARE_POOL(MissionRoom)
public:
	MissionRoom(void);
	virtual ~MissionRoom(void);

	virtual RC::eROOM_RESULT Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession );
	inline virtual eZONETYPE GetType() { return eZONETYPE_MISSION; }
    virtual RC::eROOM_RESULT MigratePortalUserFirstEnter(KEYTYPE key,
                                                 const RoomArgumentForRoomSetup& rRoomArgument,
                                                 User* pUser, ServerSession* pServerSession);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    RC::eROOM_RESULT Create(User* user, MAPCODE map_code, KEYTYPE mission_key, ServerSession* pServerSession);

protected:
	virtual VOID On_create();
	virtual VOID On_destroy();
	virtual VOID On_readyuser( User * pUser );
	virtual VOID On_enteruser( User * pUser );
	virtual VOID On_leaveuser( User * pUser );
	virtual RC::eROOM_RESULT _CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD );
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    RC::eROOM_RESULT _CanCreate(User* user, MAPCODE map_code, const BASE_ROOMINFO& base_room_info, eZONETYPE zone_type);
    RC::eROOM_RESULT _SetBaseRoomInfo(User* user, BASE_ROOMINFO& base_room_info, MAPCODE map_code);
};
