#pragma once

//=============================================================================================================================
/// HuntingRoom 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 9. 20
	@remark
		- 헌팅방에 대한 존 정보 관리
	@note
		- 
	@history 
		- 
	@usage
		- 
*/
//=============================================================================================================================

#include "roominterface.h"

class HuntingRoom : public RoomInterface
{
	_DECLARE_POOL(HuntingRoom)
public:
	HuntingRoom(void);
	virtual ~HuntingRoom(void);

	virtual RC::eROOM_RESULT	Migrate( KEYTYPE key, RoomInterface * pPreviousRoom, ServerSession * pServerSession );
	inline virtual eZONETYPE	GetType() { return eZONETYPE_HUNTING; }

	virtual VOID				Invite( User *pUser );

	VOID						SerializeLoad( HUNTING_ROOMINFO & OUT rRoomInfo );

	RC::eROOM_RESULT	Create( User * pUser, 
								KEYTYPE key,
								MAPCODE MapCode, 
								eZONEPUBLIC ePublic, 
								TCHAR * pszRoomTitle, 
								TCHAR * pszRoomPWD, 
								BASE_ROOMINFO & IN BaseRoomInfo,
								ADDITIONAL_ROOMINFO & IN AdditionalRoomInfo,
								ServerSession * pServerSession );

protected:
	virtual RC::eROOM_RESULT	_CanJoinEach( User* pUser, eZONEPUBLIC RoomPublic, TCHAR * pszRoomPWD );
	RC::eROOM_RESULT			CanCreate( User * pUser, MAPCODE MapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic, BASE_ROOMINFO & IN BaseRoomInfo, ADDITIONAL_ROOMINFO & IN AdditionalRoomInfo );
	virtual VOID				On_create();
	virtual VOID				On_destroy();
	virtual VOID				On_readyuser( User * pUser );
	virtual VOID				On_enteruser( User * pUser );
	virtual VOID				On_leaveuser( User * pUser );

	virtual BOOL				RecommendLevel( LEVELTYPE LV );

	inline const ADDITIONAL_ROOMINFO * GetAdditionalInfo()				{ return &m_AdditionalRoomInfo; }
	inline VOID SetAdditionalInfo(const ADDITIONAL_ROOMINFO * IN pInfo) { memcpy(&m_AdditionalRoomInfo, pInfo, sizeof(m_AdditionalRoomInfo)); }

private:
	ADDITIONAL_ROOMINFO	m_AdditionalRoomInfo;
};














