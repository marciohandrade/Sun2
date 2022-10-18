/*!
 * \file	ObjectManager.h
 * \brief	������Ʈ�� ����,����,����  
 * \bug		
 * \todo	����ȭ �ʿ�
 * \author	���ǥ (lammy@webzen.co.kr)
 * \version	2008/12/30	���ǥ - doxygen command �ۼ�
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
 * \brief	������Ʈ�� ����,����,���� 
 * \author	���ǥ (lammy@webzen.co.kr)
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
	/// �⺻ ������
	ObjectManager( void );
	/// �Ҹ���
	~ObjectManager( void );

protected:
	/// ������Ʈ �����ϴ� �ؽ� ��
	Object_Map				m_hashmapObject;
	/// ������Ʈ�� �����ϴ� ���丮
	ObjectFactory*			m_pObjectFactory;
	
	///	�����
	std::vector<DWORD>		m_vPetContainer;

    OBJ_KEY_GENERATOR*	m_pPetKeyGenerator; //! ��
    OBJ_KEY_GENERATOR*	m_pBattleZoneKeyGenerator;  //!< ��Ʋ�� �÷��̾�

	/// �̱� ����� Ű������ ���������� Ŭ���̾�Ʈ���� Ű�� �����ؾ� �� �ʿ䰡 ������쿡 ���Ǵ� ����
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
	/// Ű ���� ����.
	void			InitKeyGenerator( void );
	void			RestoreKey( Object * pObject );
	void			ReleaseKeyGenerator();
	void SafeRestoreKey(OBJ_KEY_GENERATOR* generator, DWORD key);

	/// ����� �����Լ�
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

	MapObject *		GetTerrainObject( DWORD dwID );//������ �Էµ� ���̵�� ������Ʈ�� ã�´�

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
