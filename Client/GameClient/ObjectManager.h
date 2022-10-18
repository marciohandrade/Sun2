/*!
 * \file	ObjectManager.h
 * \brief	오브젝트를 생성,삭제,관리  
 * \bug		
 * \todo	최적화 필요
 * \author	김경표 (lammy@webzen.co.kr)
 * \version	2008/12/30	김경표 - doxygen command 작성
 */

#ifndef		_OBJECTMANAGER_H
#define		_OBJECTMANAGER_H
#include <ObjKeyGenerator.h>

class ObjectFactory;
class Object;
class MapObject;
struct APPEARANCEINFO;

typedef		stdext::hash_map<DWORD, Object *>		Object_Map;
typedef		Object_Map::iterator			Object_Map_Itr;
typedef		std::pair<DWORD, Object *>			Object_Map_Pair;

typedef		util::CObjKeyGenerator<DWORD> OBJ_KEY_GENERATOR;

/*!
 * \class	ObjectManager Class
 * \brief	오브젝트를 생성,삭제,관리 
 * \author	김경표 (lammy@webzen.co.kr)
 */

class ObjectManager
{
#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	typedef	std::multimap<DWORD, DWORD>											OBJECT_MULTIMAP_TYPE;
	typedef	std::multimap<DWORD, DWORD>::iterator								OBJECT_MULTIMAP_ITRTYPE;
	typedef std::pair<OBJECT_MULTIMAP_ITRTYPE, OBJECT_MULTIMAP_ITRTYPE>			OBJECT_MULTIMAP_PAIR_TYPE;

	OBJECT_MULTIMAP_TYPE		m_ObjectKindMap;
#endif

public:
	/// 기본 생성자
	ObjectManager( void );
	/// 소멸자
	~ObjectManager( void );

protected:
	/// 오브젝트 관리하는 해쉬 맵
	Object_Map				m_hashmapObject;
	/// 오브젝트를 생성하는 펙토리
	ObjectFactory*			m_pObjectFactory;
	
	///	펫관련
	std::vector<DWORD>		m_vPetContainer;

    OBJ_KEY_GENERATOR*	m_pPetKeyGenerator; //! 팻
    OBJ_KEY_GENERATOR*	m_pBattleZoneKeyGenerator;  //!< 배틀존 플레이어

	/// 싱글 모드의 키생성과 독립적으로 클라이언트에서 키를 생성해야 할 필요가 있을경우에 사용되는 변수
	OBJ_KEY_GENERATOR*	m_pPlayerKeyGenerator;
	OBJ_KEY_GENERATOR*	m_pMonsterKeyGenerator;
	OBJ_KEY_GENERATOR*	m_pNpcKeyGenerator;
	OBJ_KEY_GENERATOR*	m_pItemKeyGenerator;
	OBJ_KEY_GENERATOR*  m_pMapObjectKeyGenerator;
	OBJ_KEY_GENERATOR*	m_pNonCharacterKeyGenerator;



#ifndef _K2P_OBJECT_MANAGER_MODIFIDY
	int   m_iPlayerCount;
	int   m_iMonsterCount;
	int   m_iItemCount;
#endif

protected:
	/// 키 생성 관련.
	void			InitKeyGenerator( void );
	void			RestoreKey( Object * pObject );
	void			ReleaseKeyGenerator();
	void SafeRestoreKey(OBJ_KEY_GENERATOR* generator, DWORD key);

	/// 펫관련 삭제함수
	void			InitPetContainer() { m_vPetContainer.clear(); }
	void			CheckPetSummoned( Object * pObject );
	void			DeletePetContainer();
	BOOL			DeletePet( DWORD dwObjectKey );
	
public:
	BOOL			Create( void );
	void			Destroy( void );
    void            Destroy( const std::vector<DWORD> &vdwKeys);

	Object *		Add( DWORD dwObjectKey, eOBJECT_TYPE objType, DWORD class_code, BOOL bHero = FALSE );
	Object *		AddMapObject( DWORD dwObjectKey, eOBJECT_TYPE objType, APPEARANCEINFO * pAppearInfo, OBJECT_SUB_TYPE subType = OBJECT_SUB_TYPE_NONE );
	BOOL			Delete( DWORD dwObjectKey );

#ifdef _MOD_CHAGNE_DELETE_HERO
	BOOL			DeleteHero();
#endif

	Object *		GetObject(DWORD dwKey);
	Object *		GetObject( const TCHAR *szName );
	void			GetMonsters(std::vector<Monster*>& pMonsterArry);
	void			GetPlayers(std::vector<Player*>& pPlayerArry);
    Player*		    GetPlayer(DWORD object_key);
    Character*		GetCharacter(DWORD object_key);

	MapObject *		GetTerrainObject( DWORD dwID );//툴상의 입력된 아이디로 오브젝트를 찾는다

	void			DeleteObjectsByType( eOBJECT_TYPE eObjectType );
	void			DeletePlayersWithoutHero( DWORD dwHeroKey );
	
#ifdef _MOD_CHAGNE_DELETE_HERO
	void			DeleteAllObjectWithoutHero();
#endif

	int             GetObjectCount(eOBJECT_TYPE objType);

	int				GetAllObjectCnt();

#ifdef	_K2P_OBJECT_MANAGER_MODIFIDY
	void			ProcessUnits( DWORD dwTick );
	DWORD			GetObjectKey( const char * szName );
#endif
	
    DWORD GenerateKeyByBattleZonePlayer(eOBJECT_TYPE obj_type);
    DWORD GenerateKeyByPet(eOBJECT_TYPE obj_type);
	DWORD GenerateKeyAtSingleMode(eOBJECT_TYPE objType);


	Object_Map::iterator GetBegin()
	{
		return m_hashmapObject.begin();
	}
	Object_Map::iterator GetEnd()
	{
		return m_hashmapObject.end();
	}


    virtual void    RestoreObjectName();


    void RefreshPlayerHideState();


	
};

extern ObjectManager	g_ObjectManager;

#endif	//	_OBJECTMANAGER_H
