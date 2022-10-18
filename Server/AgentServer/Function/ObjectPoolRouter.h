#pragma once

//
#include <MemoryPoolFactoryV.h>
#include <ObjKeyGenerator.h>

//
// <TYPE DEFINITIONs>
//
enum eAGENT_OBJECT_INDEX
{
	OBJTYPE_EMPTY,
	OBJTYPE_DELEGATOR,
	//OBJTYPE_LINKEDLIST,
	//OBJTYPE_USERINFO,
	OBJTYPE_USER,
	OBJTYPE_TEMPUSER,
	OBJTYPE_USERDELEGATOR,
	//
	//
	//
	//
	OBJTYPE_MAX
};

enum eAGENT_OBJKEY_INDEX
{
	OBJKEYTYPE_PLAYERKEY,
	OBJKEYTYPE_ZONEKEY,
	OBJKEYTYPE_PARTYKEY,
    OBJKEYTYPE_PORTALKEY,
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    OBJKEYTYPE_PARTYROOMKEY,

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    OBJKEYTYPE_BATTLE_GROUND_ROOM_KEY,
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    OBJKEYTYPE_FREE_PVP_ROOM_KEY,
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
	OBJKEYTYPE_MAX,
};

typedef struct sLINKEDLIST_NODE
{
	sLINKEDLIST_NODE*	next;
	sLINKEDLIST_NODE*	prev;
	PVOID				pREF;		// 실제 데이터 참조
	DWORD				dwExtra;	// Object별 특별히 사용할 수 있는 필드
} *sLINKEDLIST_NODE_P;

struct sDELEGATOR_NODE
{
};

//////////////////////////////////////////////////////////////////////////
// 서버모드에 따라 달라질 수 있는 수치
struct sPOOL_SIZE_INFO
{
	DWORD	dwUserPoolBase;
	DWORD	dwUserPoolExtend;
	DWORD	dwTempUserPoolBase;
	DWORD	dwTempUserPoolExtend;
};


typedef Delegator<sDELEGATOR_NODE>		REF_DELEGATOR;
typedef REF_DELEGATOR*					REF_DELEGATOR_P;
typedef util::PoolRouter<OBJTYPE_MAX>			AGENT_OBJECT_POOL_ROUTER;

//////////////////////////////////////////////////////////////////////////
// <CLASS DEFINITION>
//
class ObjectPoolRouter
{
public:
	ObjectPoolRouter();
	~ObjectPoolRouter();

public:
	static ObjectPoolRouter*	Instance();

//////////////////////////////////////////////////////////////////////////
// <OBJECT POOL INTERFACE>
public:
	inline PVOID				Alloc( eAGENT_OBJECT_INDEX eTypeIdx )
	{
		return m_pPoolRouter->Alloc( eTypeIdx );
	}

	inline BOOL					Free( eAGENT_OBJECT_INDEX eTypeIdx, PVOID pInstance )
	{
		return m_pPoolRouter->Free( eTypeIdx, pInstance );
	}

	//inline sLINKEDLIST_NODE_P	AllocList()
	//{
	//	return (sLINKEDLIST_NODE_P)m_pPoolRouter->Alloc( OBJTYPE_LINKEDLIST );
	//}

	//inline BOOL					FreeList( sLINKEDLIST_NODE_P pInst )
	//{
	//	return m_pPoolRouter->Free( OBJTYPE_LINKEDLIST, pInst );
	//}

	inline REF_DELEGATOR_P		AllocToken()
	{
		return (REF_DELEGATOR_P)m_pPoolRouter->Alloc( OBJTYPE_DELEGATOR );
	}

	inline BOOL					FreeToken( REF_DELEGATOR_P pInst )
	{
		return m_pPoolRouter->Free( OBJTYPE_DELEGATOR, pInst );
	}

public:
	BOOL						InitializeByPoolInfo( sPOOL_SIZE_INFO* pSizeInfo );
	inline VOID					Initialize( eAGENT_OBJECT_INDEX eTypeIdx, DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
	{
		m_pPoolRouter->Initialize( (DWORD)eTypeIdx, dwPoolBasicSize, dwPoolExtendSize );
	}

	template<typename _TYPENAME_>
	inline BOOL					Register( DWORD dwIndex )
	{
		return m_pPoolRouter->Register<_TYPENAME_>( dwIndex );
	}

private:
	AGENT_OBJECT_POOL_ROUTER*	m_pPoolRouter;
};

//////////////////////////////////////////////////////////////////////////
// util::CObjKeyGenerator<>,
// util::CObjKeyGenerator<DWORD>
//  계열 ObjectKey Generator Wrapper
class ObjKeyGender
{
public:
	ObjKeyGender();
	~ObjKeyGender();

public:
	static ObjKeyGender*				Instance();
//////////////////////////////////////////////////////////////////////////
// <OBJECT POOL INTERFACE>
public:
	inline util::CObjKeyGenerator<DWORD>*		AllocObjKeyGenerator( eAGENT_OBJKEY_INDEX eObjKeyIdx )
	{
		if( OBJKEYTYPE_MAX > eObjKeyIdx )
		{
			util::CObjKeyGenerator<DWORD>* pKeyGender = m_pObjKeyRouter[eObjKeyIdx];
			return pKeyGender;;
		}
		ASSERT( !"잘못된 Object Key Index로 쿼리를 수행" );
		return NULL;
	}

	inline VOID				Create( eAGENT_OBJKEY_INDEX eObjKeyIdx, DWORD dwKeyStart, DWORD dwKeyEnd )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			pKeyGender->Create( dwKeyStart, dwKeyEnd );
	}

	inline DWORD			GetKey( eAGENT_OBJKEY_INDEX eObjKeyIdx )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			return pKeyGender->GetKey();
		return (DWORD)0;
	}

	inline BOOL				ReserveKey( eAGENT_OBJKEY_INDEX eObjKeyIdx, DWORD dwKey )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			return pKeyGender->ReserveKey( dwKey );
		return FALSE;
	}

	inline VOID				RestoreKey( eAGENT_OBJKEY_INDEX eObjKeyIdx, DWORD dwKey )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			pKeyGender->RestoreKey( dwKey );
	}

	inline BOOL				IsEmpty( eAGENT_OBJKEY_INDEX eObjKeyIdx )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			return pKeyGender->IsEmpty();
		return FALSE;
	}

	inline BOOL				IsExistKey( eAGENT_OBJKEY_INDEX eObjKeyIdx, DWORD dwKey )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			return pKeyGender->IsExistKey( dwKey );
		return FALSE;
	}

	inline size_t			GetSize( eAGENT_OBJKEY_INDEX eObjKeyIdx )
	{
		util::CObjKeyGenerator<DWORD>* pKeyGender = AllocObjKeyGenerator( eObjKeyIdx );
		if( pKeyGender )
			return pKeyGender->GetSize();
		return static_cast<size_t>(-1);
	}

private:
	util::CObjKeyGenerator<DWORD>*	m_pObjKeyRouter[OBJKEYTYPE_MAX];
};

