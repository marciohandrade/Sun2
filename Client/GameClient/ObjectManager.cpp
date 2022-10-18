//------------------------------------------------------------------------------
//  ObjectManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "Hero.h"
#include "mapobject.h"
#include "GlobalFunc.h"
#include "Object.h"

ObjectManager	g_ObjectManager;

//------------------------------------------------------------------------------
/**
*/
ObjectManager::ObjectManager( void )
: m_pObjectFactory( new ObjectFactory )
{
	
}


//------------------------------------------------------------------------------
/**
*/
ObjectManager::~ObjectManager( void )
{
	Destroy();
	delete m_pObjectFactory;
}

//------------------------------------------------------------------------------
/**
*/
BOOL ObjectManager::Create( void )
{
	InitKeyGenerator();

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	m_iPlayerCount = 0;
	m_iMonsterCount = 0;
	m_iItemCount = 0;
#endif

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void ObjectManager::Destroy( void )
{
	Object_Map::iterator	it;
	Object * pObject = NULL;

	// 키 반납 
	for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(it->second);
		assert(pObject);
		
		RestoreKey(pObject);
		(pObject)->Destroy();

		// 어차피 이것도 위험하긴 마찬가지...
		// TODO: g_pHero 사용하는 부분에 모두 NULL 체크하기
		if (pObject == g_pHero)
			g_pHero = NULL;

		delete (pObject);			


		m_hashmapObject.erase(it++);
	}

	g_pHero = NULL;
	m_hashmapObject.clear();

#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	m_ObjectKindMap.clear();
#endif

	ReleaseKeyGenerator();

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	m_iPlayerCount = 0;
	m_iMonsterCount = 0;
	m_iItemCount = 0;
#endif
}

//------------------------------------------------------------------------------
/**
	펫오브젝트 지우기
*/
BOOL ObjectManager::DeletePet( DWORD dwObjectKey )
{
	Object_Map::iterator	it;
	it = m_hashmapObject.find( dwObjectKey );

	Object * pObject = NULL;

	if( it == m_hashmapObject.end() )
	{
		// 없당!
		return FALSE;
	}

	pObject = it->second;

	RestoreKey(pObject);
	(pObject)->Destroy();

	if( pObject == g_pHero)
		g_pHero = NULL;

	delete (pObject);

	m_hashmapObject.erase( dwObjectKey );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
	펫오브젝트를 삭제한다. 재귀호출피하기 위해 중복했음
*/
void 
ObjectManager::DeletePetContainer()
{
	if(m_vPetContainer.empty()) return;
	
	for(size_t i=0;i<m_vPetContainer.size();++i)
	{
		DeletePet(m_vPetContainer[i]);
	}

	m_vPetContainer.clear();
}
//------------------------------------------------------------------------------
/**
	펫을가지고있는지 체크후
*/
void
ObjectManager::CheckPetSummoned(Object * pObject)
{
	//플레이어나,주인공객체일경우 
	if(pObject->IsKindOfObject(PLAYER_OBJECT) || pObject == g_pHero )
	{
		Player *pPlayer = static_cast<Player*>(pObject);
	
		//펫소환상태라면
		if(pPlayer && pPlayer->IsPetReturn())
		{
			m_vPetContainer.push_back(pPlayer->GetPetObjectKey());
		}
	}
}
//------------------------------------------------------------------------------
/**
    캐쉬에 저장할 데이터만 남겨두고 모두 강제 삭제
    이때, 이미 캐쉬가 충분히 가득 차 있다면 역시 강제 삭제
*/
void
ObjectManager::Destroy( const std::vector<DWORD> &vdwKeys)
{
    Object_Map::iterator	it;
    Object * pObject = NULL;
	
	InitPetContainer();//1.펫컨테이너 초기화

    // 키 반납 
    for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
    {
        pObject = static_cast<Object *>(it->second);
        assert(pObject);

		pObject->SetForceRemove( true );

        for (size_t i=0; i<vdwKeys.size(); ++i)
        {
            // FIXME: 오브젝트 키는 유일한가?!
            if (vdwKeys[i] == pObject->GetObjectKey())
            {
				pObject->SetForceRemove( false );
                break;
            }
        }

        // 주인공이 파티정보에 채워져 있지 않아서 실수로 강제 삭제하는 것 막기
        if (g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey())
        {
			pObject->SetForceRemove( false );
        }
		
		CheckPetSummoned(pObject);//2.펫유무 체크 삽입
		
        RestoreKey(pObject);
        (pObject)->Destroy();
        // 어차피 이것도 위험하긴 마찬가지...
        // TODO: g_pHero 사용하는 부분에 모두 NULL 체크하기
        if (pObject == g_pHero)
            g_pHero = NULL;

        delete (pObject);			

        m_hashmapObject.erase(it++);
    }
    g_pHero = NULL;
    m_hashmapObject.clear();

    ReleaseKeyGenerator();

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
    m_iPlayerCount = 0;
    m_iMonsterCount = 0;
    m_iItemCount = 0;
#endif
	
	DeletePetContainer();//3.펫제거
}

//------------------------------------------------------------------------------
/*!
	const.h 참조.
	PLAYER_OBJECT_KEY		= 0,		//	플레이어	: 1 ~ 5만 ( 최대 5만개 )
	MONSTER_OBJECT_KEY		= 50000,	//	몬스터		: 5만 ~ 10만 ( 최대 5만개, 5만번 이후 사용 )
	NPC_OBJECT_KEY			= 100000,	//	NPC			: 10만 ~ 15만 ( 최대 5만개, 10만번 이후 사용 )
	NONCHARACTER_OBJECT_KEY	= 150000,	//	???			: 15만 ~ 20만 ( 최대 5만개, 15만번 이후 사용 )
	MAP_OBJECT_KEY			= 200000,	//  맵 오브젝트	: 20만 ~ 25만 
	ITEM_OBJECT_KEY			= 250000,	//	아이템		: 25만번 이후 사용( 아이템이 가장 많음 )
	PET_OBJECT_KEY			= 300000,   //  펫오브젝트	: 30만번 (클라에서만사용)
*/
void ObjectManager::InitKeyGenerator()
{
	if( GENERALPARAM->IsNet() )
	{
		//!! 0 번 키는 없는것으로 한다!!
		m_pPetKeyGenerator = new util::CObjKeyGenerator<DWORD>(PET_OBJECT_KEY+1, ONLYCLIENT_OBJECT_KEY-1);
        m_pBattleZoneKeyGenerator = new util::CObjKeyGenerator<DWORD>(ONLYCLIENT_BATTLEZONE_OBJECT_KEY+1, MAX_OBJECT_KEY-1);
	}
	else	// 싱글모드에서 사용////////////////////////////////////
	{

		//!! 0 번 키는 없는것으로 한다!!
		m_pPlayerKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(PLAYER_OBJECT_KEY+1, MONSTER_OBJECT_KEY-1);

		m_pMonsterKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(MONSTER_OBJECT_KEY, NPC_OBJECT_KEY-1);

		m_pPetKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(PET_OBJECT_KEY+1, ONLYCLIENT_OBJECT_KEY-1);

		m_pNpcKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(NPC_OBJECT_KEY, NONCHARACTER_OBJECT_KEY-1);

		m_pNonCharacterKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(NONCHARACTER_OBJECT_KEY, MAP_OBJECT_KEY-1);

		m_pMapObjectKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(MAP_OBJECT_KEY, ITEM_OBJECT_KEY-1);

		m_pItemKeyGenerator = 
			new util::CObjKeyGenerator<DWORD>(ITEM_OBJECT_KEY, ITEM_OBJECT_KEY+50000-1);

        m_pBattleZoneKeyGenerator = new util::CObjKeyGenerator<DWORD>(ONLYCLIENT_BATTLEZONE_OBJECT_KEY+1, MAX_OBJECT_KEY-1);
	}
}
//------------------------------------------------------------------------------ 
DWORD ObjectManager::GenerateKeyByBattleZonePlayer(eOBJECT_TYPE obj_type)
{
    if(GENERALPARAM->IsNet() == TRUE)
    {
        if (obj_type == PLAYER_OBJECT)
        {
            return m_pBattleZoneKeyGenerator->GetKey();
        }
    }
    return 0;
}
//------------------------------------------------------------------------------ 
DWORD ObjectManager::GenerateKeyByPet(eOBJECT_TYPE obj_type)
{
    if(GENERALPARAM->IsNet() == TRUE)
    {
        if (obj_type == PET_OBJECT)
        {
            return m_pPetKeyGenerator->GetKey();
        }
    }
    return 0;
}
//------------------------------------------------------------------------------ 
// 이것은 싱글모드를 위해서 임시로 키를 생성할때 사용하는 함수
DWORD ObjectManager::GenerateKeyAtSingleMode(eOBJECT_TYPE objType)
{
	if(GENERALPARAM->IsNet())
	{
		//if (objType == PET_OBJECT)
		//{
		//	return m_pPetKeyGenerator->GetKey();
		//}
	}
	else	// 싱글모드에서 사용////////////////////////////////////
	{
		switch (objType)
		{
			case PLAYER_OBJECT:		
				return m_pPlayerKeyGenerator->GetKey();	
				break;

			case SSQMONSTER_OBJECT:
			case MONSTER_OBJECT:	
				return m_pMonsterKeyGenerator->GetKey(); 

			case PET_OBJECT:	
				return m_pPetKeyGenerator->GetKey();

			case NPC_OBJECT:	
				return m_pNpcKeyGenerator->GetKey();	

			case MAP_OBJECT:		
				return m_pMapObjectKeyGenerator->GetKey(); 

			case ITEM_OBJECT:		
				return m_pItemKeyGenerator->GetKey();	

			case NONCHARACTER_OBJECT: 
				return m_pNonCharacterKeyGenerator->GetKey(); 
		}
	}

	return 0;
}



void ObjectManager::SafeRestoreKey(OBJ_KEY_GENERATOR* generator, DWORD key)
{
	if (generator == NULL)
	{
		return;
	}

	if (generator->IsExistKey(key) == FALSE)
	{
		generator->RestoreKey(key);
	}
}

void ObjectManager::RestoreKey(Object * pObject)
{
	if (pObject == NULL)
    {
		return;
    }

	DWORD dwKey = pObject->GetObjectKey();

	if(GENERALPARAM->IsNet())
	{
        if (pObject->IsKindOfObject(PLAYER_OBJECT))
        {	
            if ((dwKey >= ONLYCLIENT_BATTLEZONE_OBJECT_KEY) && (dwKey < MAX_OBJECT_KEY))
            {
                m_pBattleZoneKeyGenerator->RestoreKey(dwKey);
            }
        }
		else if (pObject->IsKindOfObject(PET_OBJECT))
		{
			m_pPetKeyGenerator->RestoreKey(dwKey);
		}
	}
	else // 싱글모드에서 사용////////////////////////////////////
	{
		if ( pObject->IsKindOfObject(PLAYER_OBJECT))
		{	
            if (dwKey < ONLYCLIENT_OBJECT_KEY)  //!< dwKey >= ONLYCLIENT_OBJECT_KEY은 키할당하지 않는다.
            {
                m_pPlayerKeyGenerator->RestoreKey(dwKey);
            }
		}
		else if( pObject->IsKindOfObject(PET_OBJECT) )
		{
			m_pPetKeyGenerator->RestoreKey(dwKey);
		}
		else if ( pObject->IsKindOfObject(MONSTER_OBJECT) )
		{
			m_pMonsterKeyGenerator->RestoreKey(dwKey);
		}
		else if ( pObject->IsKindOfObject(NPC_OBJECT))
		{
			SafeRestoreKey(m_pNpcKeyGenerator, dwKey);
		}
		else if ( pObject->IsKindOfObject(MAP_OBJECT))
		{
			SafeRestoreKey(m_pMapObjectKeyGenerator, dwKey);
		}
		else if ( pObject->IsKindOfObject(NONCHARACTER_OBJECT))
		{
			m_pNonCharacterKeyGenerator->RestoreKey(dwKey);
		}
		else if ( pObject->IsKindOfObject(ITEM_OBJECT))
		{
			m_pItemKeyGenerator->RestoreKey(dwKey);
		}

	}
}


void ObjectManager::ReleaseKeyGenerator()
{
	SAFE_DELETE(m_pPlayerKeyGenerator)
	SAFE_DELETE(m_pMonsterKeyGenerator)
	SAFE_DELETE(m_pNpcKeyGenerator)
	SAFE_DELETE(m_pItemKeyGenerator)
	SAFE_DELETE(m_pMapObjectKeyGenerator)
	SAFE_DELETE(m_pNonCharacterKeyGenerator)
	SAFE_DELETE(m_pPetKeyGenerator)
    SAFE_DELETE(m_pBattleZoneKeyGenerator)
}

//------------------------------------------------------------------------------
/**
*/	
Object* ObjectManager::Add( DWORD dwObjectKey, eOBJECT_TYPE objType, DWORD class_code, BOOL bHero )
{
	Object *pOldObject = GetObject(dwObjectKey);
	assert( pOldObject == NULL && "Conflict Object Key");
	assert( 0 != dwObjectKey );

	eOBJECT_TYPE tempType=objType;

	if (dwObjectKey == 0)
	{
#ifdef _DEBUG
		_asm int 3;
#endif

#ifdef _DEV_VER
		if (GENERALPARAM->GetSpecialMode())
		{
			TCHAR szWarning[INTERFACE_STRING_LENGTH] = {0,};
			Snprintf(szWarning, INTERFACE_STRING_LENGTH-1, "Object Key가 0 입니다(ObjectType == %d). 스크린샷을 찍은 후 서버팀에 문의해주세요.(개발배포용 메시지)", objType);
			GlobalFunc::OutputMsg_SysMsg(szWarning);
		}
#endif
		return NULL;
	}

	Object* pObj;
	if(bHero == FALSE)
	{
		pObj = m_pObjectFactory->MakeNewObject(tempType);
	}
	else 
	{
		pObj = new Hero;
	}

	assert(pObj);
	if( !pObj )
		return NULL;

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	switch (objType)
	{
	case PLAYER_OBJECT:
		m_iPlayerCount++;
		break;

	case SSQMONSTER_OBJECT:
	case MONSTER_OBJECT:
		m_iMonsterCount++;
		break;

	case ITEM_OBJECT:
		m_iItemCount++;
		break;
	}
#endif

	//pObj->SetTransFormResourceCode(rcCode);

	BOOL bRet = pObj->Create(dwObjectKey,class_code);

	if (!bRet)
	{
		delete pObj;
		return NULL;
	}

	m_hashmapObject.insert( Object_Map_Pair(dwObjectKey, pObj) );
#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	m_ObjectKindMap.insert( OBJECT_MULTIMAP_TYPE::value_type( pObj->GetSCObject().GetObjectType(), dwObjectKey ) );
#endif

	return pObj;
}


Object * ObjectManager::AddMapObject( DWORD dwObjectKey, eOBJECT_TYPE objType, APPEARANCEINFO * pAppearInfo,OBJECT_SUB_TYPE subType)
{
	Object *pOldObject = GetObject(dwObjectKey);
	assert( pOldObject == NULL );
	assert( 0 != dwObjectKey );

	Object* pObj;
	pObj = m_pObjectFactory->MakeNewObject(objType,subType);
	assert(pObj);

	if( !pObj )
		return NULL;

	MapObject * pMapObject = static_cast<MapObject *>(pObj);
	assert(pMapObject);
	pMapObject->Create( dwObjectKey, pAppearInfo );
	m_hashmapObject.insert( Object_Map_Pair(dwObjectKey, pObj) );

#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	m_ObjectKindMap.insert( OBJECT_MULTIMAP_TYPE::value_type( pObj->GetSCObject().GetObjectType(), dwObjectKey ) );
#endif
	return pObj;
}

//------------------------------------------------------------------------------
/**
*/
BOOL ObjectManager::Delete( DWORD dwObjectKey )
{
	Object_Map::iterator	it;
	it = m_hashmapObject.find( dwObjectKey );

	InitPetContainer();//1.펫컨테이너 초기화

	if( it == m_hashmapObject.end() )
	{
		// 없당!
		return FALSE;
	}

    Object* pObject = it->second;

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	// 나중에좀 바꾸자
	if (pObject->IsKindOfObject(PLAYER_OBJECT))
	{
		m_iPlayerCount--;
	}
	else if (pObject->IsKindOfObject(MONSTER_OBJECT))
	{
		m_iMonsterCount--;
	}
	else if (pObject->IsKindOfObject(ITEM_OBJECT))
	{
		m_iItemCount--;
	}
#endif

#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	OBJECT_MULTIMAP_PAIR_TYPE range = m_ObjectKindMap.equal_range( pObject->GetSCObject().GetObjectType() );

	for( OBJECT_MULTIMAP_ITRTYPE itr = range.first; itr != range.second; ++itr )
	{
		if( itr->second == dwObjectKey )
		{
			m_ObjectKindMap.erase( itr );	
			break;
		}
	}
#endif
	
	CheckPetSummoned(pObject);//2.펫유무 체크 삽입

	RestoreKey(pObject);
	pObject->Destroy();

	if( pObject == g_pHero)
		g_pHero = NULL;

	delete (pObject);

	m_hashmapObject.erase( dwObjectKey );
	
	DeletePetContainer();//3.펫제거

	return TRUE;
}

#ifdef _MOD_CHAGNE_DELETE_HERO

//------------------------------------------------------------------------------
/**
*/
BOOL			
ObjectManager::DeleteHero()
{
	if ( !g_pHero )
	{
		return FALSE;
	}
	InitPetContainer();//1.펫컨테이너 초기화

	CheckPetSummoned(pObject);//2.펫유무 체크 삽입

	Delete( g_pHero->GetObjectKey() );
	g_pHero = NULL;	
	
	DeletePetContainer();//3.펫제거

	return TRUE;
}

#endif

//----------------------------------------------------------------------------
/** 선택한 오브젝트 타입만 지우기 위한 함수
	ex) DeleteObjectsByType(PLAYER_OBJECT);
*/
void			
ObjectManager::DeleteObjectsByType( eOBJECT_TYPE eObjectType  )
{
	InitPetContainer();//1.펫컨테이너 초기화

#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	if( m_ObjectKindMap.empty() )
		return;

	Object_Map::iterator	Object_itr;
	Object * pObject = NULL;

	OBJECT_MULTIMAP_PAIR_TYPE range = m_ObjectKindMap.equal_range( eObjectType );

	for( OBJECT_MULTIMAP_ITRTYPE itr = range.first; itr != range.second; )
	{
		DWORD key = itr->second;
		Object_itr = m_hashmapObject.find( key );

		if( Object_itr != m_hashmapObject.end() )
		{
			pObject = Object_itr->second;
			
			CheckPetSummoned(pObject);//2.펫유무 체크 삽입

			RestoreKey(pObject);
			(pObject)->Destroy();
			
			if( pObject == g_pHero )
				g_pHero = NULL;

			delete (pObject);

			m_hashmapObject.erase( Object_itr );
			m_ObjectKindMap.erase( itr++ );
		}
	}

#else
	Object_Map::iterator	itr;
	Object * pObject = NULL;

	// 키 반납 
	for( itr = m_hashmapObject.begin() ; itr != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(itr->second);

		if ( pObject->IsKindOfObject( eObjectType ) )
		{
			CheckPetSummoned(pObject);//2.펫유무 체크 삽입

			RestoreKey(pObject);
			(pObject)->Destroy();
			
			if(pObject==g_pHero)
				g_pHero=NULL;

			delete (pObject);

			m_hashmapObject.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
#endif

	DeletePetContainer();//3.펫제거
}

//----------------------------------------------------------------------------
/** 자신만 남겨둔채 모든 플레이어를 지워라.
*/
void			
ObjectManager::DeletePlayersWithoutHero( DWORD dwHeroKey )
{
	InitPetContainer();//1.펫컨테이너 초기화

	Object_Map::iterator	itr;
	Object * pObject = NULL;

	DWORD	dwTargetObjectType = 0;

	// 키 반납 
	for( itr = m_hashmapObject.begin() ; itr != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(itr->second);

		if ( dwHeroKey == pObject->GetObjectKey() )
		{
			itr++;
			continue;
		}

		if ( pObject->IsKindOfObject( PLAYER_OBJECT ) )
		{
			CheckPetSummoned(pObject);//2.펫유무 체크 삽입

			RestoreKey(pObject);
			(pObject)->Destroy();
			delete (pObject);

			m_hashmapObject.erase(itr++);
		}
		else
		{
			itr++;
		}
	}

	DeletePetContainer();//3.펫제거

}

#ifdef _MOD_CHAGNE_DELETE_HERO
//------------------------------------------------------------------------------
/**
*/
void			
ObjectManager::DeleteAllObjectWithoutHero()
{
	Object * pObject = NULL;
	Object_Map::iterator it;
	
	InitPetContainer();//1.펫컨테이너 초기화

	for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(it->second);
		assert(pObject);

		if ( pObject == g_pHero )
		{
			it++;
		}
		else
		{
			CheckPetSummoned(pObject);//2.펫유무 체크 삽입

			RestoreKey(pObject);
			(pObject)->Destroy();
			delete (pObject);			

			m_hashmapObject.erase(it++);
		}
	}

	DeletePetContainer();//3.펫제거

}
#endif


//------------------------------------------------------------------------------
/**
*/

void ObjectManager::GetMonsters(std::vector<Monster*>& pMonsterArry)
{
	pMonsterArry.clear();

	Object * pObject = NULL;
	Object_Map::iterator it;
	BOOL rt=TRUE;
	for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(it->second);
		SUN_ASSERT(pObject);
		if(pObject && pObject->IsKindOfObject(MONSTER_OBJECT))
			pMonsterArry.push_back((Monster*)pObject);

		it++;
	}
}

void ObjectManager::GetPlayers(std::vector<Player*>& pPlayerArry)
{
	pPlayerArry.clear();

	Object * pObject = NULL;
	Object_Map::iterator it;
	BOOL rt=TRUE;
	for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(it->second);
		SUN_ASSERT(pObject);
		if(pObject && pObject->IsKindOfObject(PLAYER_OBJECT))
			pPlayerArry.push_back((Player*)pObject);

		it++;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Object * ObjectManager::GetObject(DWORD dwKey)
{	
	Object_Map_Itr	itr;
	itr = m_hashmapObject.find( dwKey );

	if( itr != m_hashmapObject.end() )
		return itr->second;

	return NULL;

}
//------------------------------------------------------------------------------ 
Player* ObjectManager::GetPlayer(DWORD object_key)
{
    Object* obj = GetObject(object_key);
    if (obj != NULL)
    {
        if (obj->IsKindOfObject(PLAYER_OBJECT) == TRUE)
        {
            return static_cast<Player*>(obj);
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
Character* ObjectManager::GetCharacter(DWORD object_key)
{
    Object* obj = GetObject(object_key);
    if (obj != NULL)
    {
        if (obj->IsKindOfObject(CHARACTER_OBJECT) == TRUE)
        {
            return static_cast<Character*>(obj);
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
Object * ObjectManager::GetObject( const TCHAR * szName )
{
#ifdef _K2P_OBJECT_MANAGER_MODIFIDY

	Object * pObject = NULL;

	OBJECT_MULTIMAP_PAIR_TYPE range = m_ObjectKindMap.equal_range( PLAYER_OBJECT );

	for( OBJECT_MULTIMAP_ITRTYPE itr = range.first; itr != range.second; ++itr )
	{
		pObject = GetObject( itr->second );

		if( pObject != NULL )
		{
			if( Strcmp( szName, pObject->GetName() ) == 0 )
				return pObject;
		}
	}

#else
	Object_Map::iterator	itr;
	Object *				pObject = NULL;

	// 이름으로 찾기
	for (itr = m_hashmapObject.begin(); itr != m_hashmapObject.end(); ++itr)
	{
		pObject = static_cast<Object *>(itr->second);

		if (pObject)
		{
            if (pObject->IsKindOfObject(PLAYER_OBJECT) == TRUE)
            {
                if (Strcmp(szName, pObject->GetName()) == 0)
                {
				    return pObject;
                }
			}
		}
	}
#endif

	return NULL;
}

MapObject * ObjectManager::GetTerrainObject(DWORD dwID)
{
	//Todo:속도 개선 

	Object_Map::iterator	itr;
	Object * pObject = NULL;

	DWORD	dwTargetObjectType = 0;


	for( itr = m_hashmapObject.begin() ; itr != m_hashmapObject.end() ; itr++ )
	{
		pObject = static_cast<Object *>(itr->second);
		if( pObject )
		{
			if(pObject->IsKindOfObject(MAP_OBJECT))
			{
				MapObject *pMapObject=(MapObject *)pObject;
				if(pMapObject->IsTerrain()&&pMapObject->GetID()==dwID)
				{
					return pMapObject;
				}
			}
		}
	}

	return NULL;
}


int  ObjectManager::GetObjectCount(eOBJECT_TYPE objType)
{
#ifdef _K2P_OBJECT_MANAGER_MODIFIDY

	if( m_ObjectKindMap.empty() )
		return 0;
	
	UINT	nCount = 0;

	OBJECT_MULTIMAP_PAIR_TYPE range = m_ObjectKindMap.equal_range( objType );

	for( OBJECT_MULTIMAP_ITRTYPE itr = range.first; itr != range.second; ++itr )
	{
		nCount++;
	}

	return nCount;

#else
	switch (objType)
	{
	case PLAYER_OBJECT:
		return m_iPlayerCount;
		break;

	case SSQMONSTER_OBJECT:
	case MONSTER_OBJECT:
		return m_iMonsterCount;
		break;

	case ITEM_OBJECT:
		return m_iItemCount;
		break;
	}

	return 0;
#endif

}

void ObjectManager::RefreshPlayerHideState()
{
    for( Object_Map::iterator it = m_hashmapObject.begin(); it != m_hashmapObject.end(); ++it)
    {
        Object* pObject = static_cast<Object*>(it->second);

        if( pObject->IsKindOfObject(PLAYER_OBJECT) )
        {
            Player* pPlayer = (Player*)pObject;
            pPlayer->RefreshHideState(FALSE);
        }
    }
}


int ObjectManager::GetAllObjectCnt()
{
	return m_hashmapObject.size();
}

#ifdef	_K2P_OBJECT_MANAGER_MODIFIDY
void	ObjectManager::ProcessUnits( DWORD dwTick )
{
	float		fDistance = 0.f;
	Object_Map_Itr tempitr;
	Object_Map_Itr itr = m_hashmapObject.begin();

	while( itr != m_hashmapObject.end() )
	{		
		Object * pObject = static_cast<Object *>(itr->second);
		if( 0 == pObject )
		{
			assert(!"Object Is NULL!!");
			return;
		}
		
		BOOL bLive = pObject->Process(dwTick);

		if( bLive )
		{
			itr++;
		}
		else
		{
			tempitr = itr;
			itr++;

			Delete( tempitr->first );
		}
	}

}

DWORD	ObjectManager::GetObjectKey( const char * szName )
{
	Object * pObj = GetObject( szName );

	if( pObj != NULL )
		return pObj->GetObjectKey();

	return 0;
}
#endif

//------------------------------------------------------------------------------
void ObjectManager::RestoreObjectName()
{
    Object_Map::iterator begin = m_hashmapObject.begin();
    Object_Map::iterator end = m_hashmapObject.end();

    for (; begin != end; ++begin)
    {
        if (begin->second)
        {
            (begin->second)->RestoreObjectName();
        }
    }
}



