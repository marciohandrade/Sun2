#pragma once
//=============================================================================================================================
/// GameZone class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- GameServer���� ���� �����ϴ� �ֻ��� �߻� Ŭ����
	@note
		- 
	@history 
		- 2006.7.26 : Solar->hash_map���� ���� + �����丵
		- 2006.8.2 : �������� �����丵 ( �޸� ����ȭ, ���� ����ȭ, Smart Code? )
*/
//=============================================================================================================================
#include <hash_map>

class GameZone;

class GameZoneManager
{
#ifdef _NA_0_20100416_RELOAD_MAPOBJECT
    friend class ZoneReloader;
#endif

	typedef STLX_HASH_MAP<DWORD, GameZone *>	GAMEZONE_HASH;
	typedef std::pair<DWORD, GameZone *>		GAMEZONE_PAIR;
public:
	GameZoneManager(void);
	~GameZoneManager(void);
	static void ReservedZonePool( DWORD poolSize );

	// ���� �����ϰ� �ʱ�ȭ �Ѵ�
    GameZone*  CreateZone(eZONETYPE zoneType, ZONEKEY zoneKey, MAPCODE mapCode,
                          eZONE_RULESET::eRules rules = eZONE_RULESET::ZONE_RULE_EMPTY);
	// ����, ��й�ȣ, �������� ���� �����Ѵ�.

	VOID		Release();

	// Update�߿� DestroyZone(ZONEKEY Key)�� ����ϸ� �ȵȴ�
	VOID		DestroyZone( ZONEKEY Key );
	VOID		DestroyZoneAll();

	VOID		ReadyPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	BOOL		JoinPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos );
	VOID		WarpPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos=NULL );
	VOID		LeavePlayer( ZONEKEY Key, Player * pPlayer );

	// �ʵ� ���� üũ
    // (f100623.1L) change an interface to support a multi-checksum checker
    bool FieldVersionCheck(ZONEKEY zone_key, FIELDCODE field_code,
                           EngineFileChecksum* stored_checksums,
                           const EngineFileChecksum& inputed_checksums) const;
	//BOOL		FieldVersionCheck( ZONEKEY Key, MAPCODE FieldCode, DWORD checksum, DWORD * OUT pCurCheckSum );

	VOID		Update( DWORD dwDeltaTick );

	GameZone *	GetZone( ZONEKEY Key );
	VOID		Display( BOOL bLog = FALSE );
	VOID		DisplayPoolInfo();

	GameZone *	GetFieldVillageZone( MAPCODE MapCode );

private:
	VOID		_DestroyZone(  GAMEZONE_HASH::iterator it );
	VOID		_AddZone( GameZone * pZone );
	VOID		_RemoveZone( ZONEKEY Key );

private:
	GAMEZONE_HASH *	m_pZoneHash;

	class ZonePool;
	static ZonePool s_pool;

	typedef STLX_HASH_MAP<GameZone*, GameZone*> ZONEPOOL_HASH;
	ZONEPOOL_HASH	m_ZonePoolHash;
};


extern GameZoneManager g_GameZoneManager;









