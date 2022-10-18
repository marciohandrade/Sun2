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

	// Ű �ݳ� 
	for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(it->second);
		assert(pObject);
		
		RestoreKey(pObject);
		(pObject)->Destroy();

		// ������ �̰͵� �����ϱ� ��������...
		// TODO: g_pHero ����ϴ� �κп� ��� NULL üũ�ϱ�
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
	�������Ʈ �����
*/
BOOL ObjectManager::DeletePet( DWORD dwObjectKey )
{
	Object_Map::iterator	it;
	it = m_hashmapObject.find( dwObjectKey );

	Object * pObject = NULL;

	if( it == m_hashmapObject.end() )
	{
		// ����!
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
	�������Ʈ�� �����Ѵ�. ���ȣ�����ϱ� ���� �ߺ�����
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
	�����������ִ��� üũ��
*/
void
ObjectManager::CheckPetSummoned(Object * pObject)
{
	//�÷��̾,���ΰ���ü�ϰ�� 
	if(pObject->IsKindOfObject(PLAYER_OBJECT) || pObject == g_pHero )
	{
		Player *pPlayer = static_cast<Player*>(pObject);
	
		//���ȯ���¶��
		if(pPlayer && pPlayer->IsPetReturn())
		{
			m_vPetContainer.push_back(pPlayer->GetPetObjectKey());
		}
	}
}
//------------------------------------------------------------------------------
/**
    ĳ���� ������ �����͸� ���ܵΰ� ��� ���� ����
    �̶�, �̹� ĳ���� ����� ���� �� �ִٸ� ���� ���� ����
*/
void
ObjectManager::Destroy( const std::vector<DWORD> &vdwKeys)
{
    Object_Map::iterator	it;
    Object * pObject = NULL;
	
	InitPetContainer();//1.�������̳� �ʱ�ȭ

    // Ű �ݳ� 
    for( it = m_hashmapObject.begin() ; it != m_hashmapObject.end() ;)
    {
        pObject = static_cast<Object *>(it->second);
        assert(pObject);

		pObject->SetForceRemove( true );

        for (size_t i=0; i<vdwKeys.size(); ++i)
        {
            // FIXME: ������Ʈ Ű�� �����Ѱ�?!
            if (vdwKeys[i] == pObject->GetObjectKey())
            {
				pObject->SetForceRemove( false );
                break;
            }
        }

        // ���ΰ��� ��Ƽ������ ä���� ���� �ʾƼ� �Ǽ��� ���� �����ϴ� �� ����
        if (g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey())
        {
			pObject->SetForceRemove( false );
        }
		
		CheckPetSummoned(pObject);//2.������ üũ ����
		
        RestoreKey(pObject);
        (pObject)->Destroy();
        // ������ �̰͵� �����ϱ� ��������...
        // TODO: g_pHero ����ϴ� �κп� ��� NULL üũ�ϱ�
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
	
	DeletePetContainer();//3.������
}

//------------------------------------------------------------------------------
/*!
	const.h ����.
	PLAYER_OBJECT_KEY		= 0,		//	�÷��̾�	: 1 ~ 5�� ( �ִ� 5���� )
	MONSTER_OBJECT_KEY		= 50000,	//	����		: 5�� ~ 10�� ( �ִ� 5����, 5���� ���� ��� )
	NPC_OBJECT_KEY			= 100000,	//	NPC			: 10�� ~ 15�� ( �ִ� 5����, 10���� ���� ��� )
	NONCHARACTER_OBJECT_KEY	= 150000,	//	???			: 15�� ~ 20�� ( �ִ� 5����, 15���� ���� ��� )
	MAP_OBJECT_KEY			= 200000,	//  �� ������Ʈ	: 20�� ~ 25�� 
	ITEM_OBJECT_KEY			= 250000,	//	������		: 25���� ���� ���( �������� ���� ���� )
	PET_OBJECT_KEY			= 300000,   //  �������Ʈ	: 30���� (Ŭ�󿡼������)
*/
void ObjectManager::InitKeyGenerator()
{
	if( GENERALPARAM->IsNet() )
	{
		//!! 0 �� Ű�� ���°����� �Ѵ�!!
		m_pPetKeyGenerator = new util::CObjKeyGenerator<DWORD>(PET_OBJECT_KEY+1, ONLYCLIENT_OBJECT_KEY-1);
        m_pBattleZoneKeyGenerator = new util::CObjKeyGenerator<DWORD>(ONLYCLIENT_BATTLEZONE_OBJECT_KEY+1, MAX_OBJECT_KEY-1);
	}
	else	// �̱۸�忡�� ���////////////////////////////////////
	{

		//!! 0 �� Ű�� ���°����� �Ѵ�!!
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
// �̰��� �̱۸�带 ���ؼ� �ӽ÷� Ű�� �����Ҷ� ����ϴ� �Լ�
DWORD ObjectManager::GenerateKeyAtSingleMode(eOBJECT_TYPE objType)
{
	if(GENERALPARAM->IsNet())
	{
		//if (objType == PET_OBJECT)
		//{
		//	return m_pPetKeyGenerator->GetKey();
		//}
	}
	else	// �̱۸�忡�� ���////////////////////////////////////
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
	else // �̱۸�忡�� ���////////////////////////////////////
	{
		if ( pObject->IsKindOfObject(PLAYER_OBJECT))
		{	
            if (dwKey < ONLYCLIENT_OBJECT_KEY)  //!< dwKey >= ONLYCLIENT_OBJECT_KEY�� Ű�Ҵ����� �ʴ´�.
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
			Snprintf(szWarning, INTERFACE_STRING_LENGTH-1, "Object Key�� 0 �Դϴ�(ObjectType == %d). ��ũ������ ���� �� �������� �������ּ���.(���߹����� �޽���)", objType);
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

	InitPetContainer();//1.�������̳� �ʱ�ȭ

	if( it == m_hashmapObject.end() )
	{
		// ����!
		return FALSE;
	}

    Object* pObject = it->second;

#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	// ���߿��� �ٲ���
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
	
	CheckPetSummoned(pObject);//2.������ üũ ����

	RestoreKey(pObject);
	pObject->Destroy();

	if( pObject == g_pHero)
		g_pHero = NULL;

	delete (pObject);

	m_hashmapObject.erase( dwObjectKey );
	
	DeletePetContainer();//3.������

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
	InitPetContainer();//1.�������̳� �ʱ�ȭ

	CheckPetSummoned(pObject);//2.������ üũ ����

	Delete( g_pHero->GetObjectKey() );
	g_pHero = NULL;	
	
	DeletePetContainer();//3.������

	return TRUE;
}

#endif

//----------------------------------------------------------------------------
/** ������ ������Ʈ Ÿ�Ը� ����� ���� �Լ�
	ex) DeleteObjectsByType(PLAYER_OBJECT);
*/
void			
ObjectManager::DeleteObjectsByType( eOBJECT_TYPE eObjectType  )
{
	InitPetContainer();//1.�������̳� �ʱ�ȭ

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
			
			CheckPetSummoned(pObject);//2.������ üũ ����

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

	// Ű �ݳ� 
	for( itr = m_hashmapObject.begin() ; itr != m_hashmapObject.end() ;)
	{
		pObject = static_cast<Object *>(itr->second);

		if ( pObject->IsKindOfObject( eObjectType ) )
		{
			CheckPetSummoned(pObject);//2.������ üũ ����

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

	DeletePetContainer();//3.������
}

//----------------------------------------------------------------------------
/** �ڽŸ� ���ܵ�ä ��� �÷��̾ ������.
*/
void			
ObjectManager::DeletePlayersWithoutHero( DWORD dwHeroKey )
{
	InitPetContainer();//1.�������̳� �ʱ�ȭ

	Object_Map::iterator	itr;
	Object * pObject = NULL;

	DWORD	dwTargetObjectType = 0;

	// Ű �ݳ� 
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
			CheckPetSummoned(pObject);//2.������ üũ ����

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

	DeletePetContainer();//3.������

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
	
	InitPetContainer();//1.�������̳� �ʱ�ȭ

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
			CheckPetSummoned(pObject);//2.������ üũ ����

			RestoreKey(pObject);
			(pObject)->Destroy();
			delete (pObject);			

			m_hashmapObject.erase(it++);
		}
	}

	DeletePetContainer();//3.������

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

	// �̸����� ã��
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
	//Todo:�ӵ� ���� 

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



