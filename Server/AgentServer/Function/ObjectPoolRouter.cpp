#include "stdafx.h"
#include <ObjectKeyStream.h>
#include "ObjectPoolRouter.h"

#include <UserSessions/User.h>
#include <UserSessions/TempUser.h>

static ObjectPoolRouter		s_ObjectPoolRouter;
static ObjKeyGender			s_ObjKeyGender;

ObjectPoolRouter::ObjectPoolRouter()
{
	m_pPoolRouter = new AGENT_OBJECT_POOL_ROUTER;
}

ObjectPoolRouter::~ObjectPoolRouter()
{
	// object history 확인할 수 있는 좋은 방법이 없는가?
	SAFE_DELETE( m_pPoolRouter );
}

BOOL	ObjectPoolRouter::InitializeByPoolInfo( sPOOL_SIZE_INFO* pSizeInfo )
{
	if( !pSizeInfo )
		return FALSE;

	sPOOL_SIZE_INFO& rINFO = *pSizeInfo;
	if( !rINFO.dwUserPoolBase )			rINFO.dwUserPoolBase = 10;
	if( !rINFO.dwUserPoolExtend )		rINFO.dwUserPoolExtend = 10;
	if( !rINFO.dwTempUserPoolBase )		rINFO.dwTempUserPoolBase = 10;
	if( !rINFO.dwTempUserPoolExtend )	rINFO.dwTempUserPoolExtend = 10;

	//
	//m_pPoolRouter->Register<REF_DELEGATOR>( OBJTYPE_DELEGATOR );
	//m_pPoolRouter->Initialize( OBJTYPE_DELEGATOR, 100, 100 );

	//m_pPoolRouter->Register<sLINKEDLIST_NODE>( OBJTYPE_LINKEDLIST );
	//m_pPoolRouter->Initialize( OBJTYPE_LINKEDLIST, 100, 100 );

	m_pPoolRouter->Register<User>(OBJTYPE_USER);
	m_pPoolRouter->Initialize( OBJTYPE_USER, rINFO.dwUserPoolBase, rINFO.dwUserPoolExtend );

	m_pPoolRouter->Register<TempUser>(OBJTYPE_TEMPUSER);
	m_pPoolRouter->Initialize( OBJTYPE_TEMPUSER, rINFO.dwTempUserPoolBase, rINFO.dwTempUserPoolExtend );

	return TRUE;
}

//VOID				ObjectPoolRouter::CreateInstance()
//{
//	//if( !s_ObjectPoolRouter )
//	//	s_ObjectPoolRouter = new ObjectPoolRouter();
//}
//
//VOID				ObjectPoolRouter::DestroyInstance()
//{
//	//SAFE_DELETE( s_ObjectPoolRouter );
//}

ObjectPoolRouter*	ObjectPoolRouter::Instance()
{
	return &s_ObjectPoolRouter;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ObjKeyGender::ObjKeyGender()
{
    ZeroMemory(m_pObjKeyRouter, sizeof(m_pObjKeyRouter));

    //
    m_pObjKeyRouter[OBJKEYTYPE_PLAYERKEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_PLAYERKEY]->Create(\
        nsKeyInfo::PLAYER_OBJECT_KEY_START+1, nsKeyInfo::PLAYER_OBJECT_KEY_LAST);

    m_pObjKeyRouter[OBJKEYTYPE_ZONEKEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_ZONEKEY]->Create(\
        nsKeyInfo::ZONE_KEY_START+1, nsKeyInfo::ZONE_KEY_LAST);

    m_pObjKeyRouter[OBJKEYTYPE_PARTYKEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_PARTYKEY]->Create(\
        nsKeyInfo::PARTY_KEY_START+1, nsKeyInfo::PARTY_KEY_LAST );

    m_pObjKeyRouter[OBJKEYTYPE_PORTALKEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_PORTALKEY]->Create(\
        nsKeyInfo::PORTAL_KEY_START+1, nsKeyInfo::PORTAL_KEY_LAST);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    // CHANGES: f110831.2L, changes using key range
    m_pObjKeyRouter[OBJKEYTYPE_PARTYROOMKEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_PARTYROOMKEY]->Create(\
        nsKeyInfo::PARTY_ROOM_KEY_START+1, nsKeyInfo::PARTY_ROOM_KEY_LAST);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    m_pObjKeyRouter[OBJKEYTYPE_BATTLE_GROUND_ROOM_KEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_BATTLE_GROUND_ROOM_KEY]->Create(\
        nsKeyInfo::BATTLE_GROUND_ROOM_KEY_START+1, nsKeyInfo::BATTLE_GROUND_ROOM_KEY_LAST);
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    m_pObjKeyRouter[OBJKEYTYPE_FREE_PVP_ROOM_KEY] = new util::CObjKeyGenerator<DWORD>;
    m_pObjKeyRouter[OBJKEYTYPE_FREE_PVP_ROOM_KEY]->Create(\
        nsKeyInfo::FREE_PVP_ROOM_KEY_START+1, nsKeyInfo::FREE_PVP_ROOM_KEY_LAST);
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

}

ObjKeyGender::~ObjKeyGender()
{
	for( INT i=0 ; i<OBJKEYTYPE_MAX ; ++i )
	{
		util::CObjKeyGenerator<DWORD>*& rpGender = m_pObjKeyRouter[i];
		SAFE_DELETE(rpGender);
	}
}

ObjKeyGender*		ObjKeyGender::Instance()
{
	return &s_ObjKeyGender;
}


