#pragma once

#include "gameroom.h"

class GameLobbyRoom : public GameRoom
{
	__DECL_CUSTOMPOOL_PTR( GameLobbyRoom )
public:
	GameLobbyRoom(void);
	virtual ~GameLobbyRoom(void);

	virtual void		OnCreate();

	virtual VOID		Update( DWORD dwDeltaTick );
	virtual eZONETYPE	GetZoneType() { return eZONETYPE_LOBBY;	}
	virtual VOID		Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual BOOL		Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual	VOID 		Leave( Player * pPlayer );
    // (f100623.1L) change an interface to support a multi-checksum checker
    virtual bool CheckMapVersion(FIELDCODE field_code,
                                 EngineFileChecksum* stored_checksums,
                                 const EngineFileChecksum& inputed_checksums) const;
	//virtual BOOL		CheckMapVersion( MAPCODE FieldCode, DWORD checksum, DWORD * OUT pCurCheckSum = NULL );

	virtual	VOID		SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	virtual VOID		ChangeMaster(DWORD dwNextMasterUserKey ) { SetMasterUserKey( dwNextMasterUserKey ); }
};
