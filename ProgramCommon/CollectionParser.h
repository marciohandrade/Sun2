#pragma once
//=======================================================================================================================
/// CollectionParser class
/**
@author
Lee Su Tek
@since
2009. 2. 21
@remarks
@note
@history

*/
//=======================================================================================================================

#include <SolarHashTable.h>
#include <Singleton.h>

struct CollectionInfo
{	
	friend class CollectionParser;

	enum { START = 0, LOOP, END, MAX_ANI };                             // �÷��̾� �ִϸ��̼��ڵ�

    enum { COLLECTABLE = 0, COLLECTING, SUCCESS, MAX_EFFECT };          // ä���� ǥ�� ����Ʈ�ڵ�

    enum 
    {  // ä�� ���� ����
        QUEST = 0,
        AC,
        MAPOBJID,
        STAMP,
        COUNT,
        TRIGGER,
        MAX_CONDITION
    }; 

    enum 
    {   // ���� ��.��
        OBJ_COLLECT = 0, 
        OBJ_STAMP, 
        OBJ_STAMP_USE_GUILD_MASTER,
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        OBJ_BATTLE_GROUND_STRONGPOINT,
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
		//_NA_008334_20150608_SONNENSCHEIN
        OBJ_SONNENSCHEIN_STAMP,
		// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
		OBJ_ONEDYSGUILDMISSION_STAMP,
    };             

	enum { SCRIPT_TYPE_ITEM	= 0, SCRIPT_TYPE_DROP, MAX_SCRIPT_TYPE };   // [�������ڵ�] ��� ���

private:
	CollectionInfo() {}
	~CollectionInfo() {}
    CollectionInfo(const CollectionInfo& rhs) { *this = rhs; }
    /*
    // (CHANGES) (f100604.3L) remove constructor
	const CollectionInfo( const CollectionInfo& rhs )
		: m_CollectionID(rhs.m_CollectionID)
		, m_dwRespawnTime(rhs.m_dwRespawnTime)
		, m_dwCollectingTime(rhs.m_dwCollectingTime)
		, m_byScriptType(rhs.m_byScriptType)
		, m_ItemCode(rhs.m_ItemCode)
		, m_IGType(rhs.m_IGType)
	{
		memcpy( m_wzidAnimation, rhs.m_wzidAnimation, sizeof(m_wzidAnimation) );
		memcpy( m_wzidEffect, rhs.m_wzidEffect, sizeof(m_wzidEffect) );

		memcpy( m_Condition, rhs.m_Condition, sizeof(m_Condition) );
	}
    */

public:
	// ����Ű
	WzID  m_CollectionID;
	
	DWORD m_dwRespawnTime;		    // ������ �ð�
	DWORD m_dwCollectingTime;	    // ä��   �ð�

	WzID m_wzidAnimation[MAX_ANI];	// �ִϸ��̼��ڵ�
	WzID m_wzidEffect[MAX_EFFECT];	// ����Ʈ�ڵ�

	BYTE	 m_byScriptType;	    // ���ὺũ��Ʈ	(0:�����۰���, 1: ������)
	SLOTCODE m_ItemCode;		    // �������ڵ�
	INT		 m_IGType;			    // �����۵��

	DWORD m_Condition[MAX_CONDITION]; // ä�� ����
};


typedef util::SolarHashTable<const CollectionInfo*> COLLECTION_INFO_HASH;

class CollectionParser : public util::Singleton<CollectionParser>, public IParser
{

public:
	CollectionParser();
	virtual ~CollectionParser();

	VOID Init( char * pszPackFileName );
	VOID Release(){ Unload(); };

	VOID Reload();
	VOID Unload();
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

	inline const CollectionInfo* const FindCollectionInfo( WzID CollectionID ) { return m_CollectionInfoHash.GetData(CollectionID); }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    inline const COLLECTION_INFO_HASH& GetCollectionInfoHash() const { return m_CollectionInfoHash; }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	template< class Operator >
	void for_each( Operator& op );

private:
	BOOL _Load( BOOL bReload );
	BOOL _Validating( const CollectionInfo& Collection ) const ;

private:
	char m_tszFileName[MAX_PATH];

	COLLECTION_INFO_HASH	m_CollectionInfoHash;
};

// (CHANGES) (f100320.1L) (WAVERIX) fix up the unsafe code that the for_each using std::for_each \
// change to explicit for each routine, because the COLLECTION_INFO_HASH is the SolarHashtable \
// which is not supported generic type traits
template< class Operator >
void CollectionParser::for_each(Operator& op) {
    COLLECTION_INFO_HASH::iterator it = m_CollectionInfoHash.begin(),
                                   end = m_CollectionInfoHash.end();
    for ( ; it != end; ++it) {
        const CollectionInfo* info = *it;
        op(info);
    }
}
