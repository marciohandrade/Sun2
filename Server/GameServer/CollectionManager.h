
#pragma once

//=======================================================================================================================
/// CollectionManager class
/**
@author
Lee Su Tek

@Memo


*/
//=======================================================================================================================


#include <SolarHashTable.h>
#include "StructInPacket.h"

class  GameField;
class  CollectionStateInfo;
struct CollectionInfo;


typedef util::SolarHashTable<CollectionStateInfo*>				ACTIVE_COLLECTION_HASH;
typedef util::SolarHashTable<CollectionStateInfo*>::iterator	ACTIVE_COLLECTION_HASH_IT;

class CollectionManager
{
public:
	CollectionManager(GameField* pField);
	~CollectionManager() {}

	void Release();

	// ä�� ���� �Ѱ�?
	// true ; ä������ ����. ( ä�������� ���δ� �˼����� )
	// false; ������ ä�����̰ų�, �̹� ä���Ǿ��� ���.
	bool IsCollectable(DWORD Objkey) { return NULL == _FindStateInfo(Objkey) ? true : false; }

	//
	void Update();
	//---------------------------------------------------------------------------------------------
	RC::eCOLLECT_RESULT Collect( DWORD ObjKeyCollection, Player* pPlayer,WzVector& wvClientCurPos );
    RC::eCOLLECT_RESULT Success( Player* pPlayer );
    RC::eCOLLECT_RESULT GmCancel( WzID CollectionID );
	RC::eCOLLECT_RESULT Cancel( Player* pPlayer );
	RC::eCOLLECT_RESULT Cancel( CollectionStateInfo* pStateInfo );
	RC::eCOLLECT_RESULT ChkCondision( Player* pPlayer, WzID CollectionID );

private:
    typedef std::pair<bool,DWORD> PairIsStampReturnValue;
    // CHANGES: f110527.3L, memory usage optimization
    struct CollectionStateInfoManager;
    //
	RC::eITEM_RESULT	_RewardItem( Player* pPlayer, CollectionInfo* pInfo, INVENTORY_TOTAL_INFO& OUT TotalInfo );

	bool				_Collect( Collection* pCollection, Player* pPlayer );
	RC::eCOLLECT_RESULT _ChkDistance( Player* pPlayer, WzVector& wvClientCurPos, WzVector& wvCollectionPos );

	void _Collected( CollectionStateInfo* pStateInfo );
	void _Respawn( CollectionStateInfo* pStateInfo );
	void _Cancel( CollectionStateInfo* pStateInfo );

	inline CollectionStateInfo* const _FindStateInfo( DWORD ObjKey ) { return m_Hash.GetData(ObjKey); }
    
    // _NA001385_20090924_DOMINATION_STAMP 
    // ���ι��� ��� ���ι��� ������ ���� �ϵ��� ����. [5/13/2010 lst1024]
    PairIsStampReturnValue _IsStamp(WzID CollectionID);
    VOID _StartStamp(Player* pPlayer, WzID CollectionID);
    VOID _CancelStamp(Player* pPlayer, WzID CollectionID);
    
    template<typename PacketType> 
    VOID _SendStampPacket(PacketType& BrdMsg, Player* pPlayer, WzID CollectionID);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    bool _IsSealingStrongPoint(WzID collection_id);
    void _StartSealing(Player* player, WzID collection_id);
    void _CancelSealing(Player* player, WzID collection_id);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008334_20150608_SONNENSCHEIN
    // �ҳٻ��ο� �������� Ȯ���Ѵ�
    BOOL _IsSonnenScheinStamp(WzID collection_id);
    VOID _StartSonnenScheinStamp(Player* player, WzID collection_id);
    VOID _CancelSonnenScheinStamp(Player* player, WzID collection_id);
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	// ���ϱ��̼ǿ� �������� Ȯ���Ѵ�.
	BOOL _IsOneDaysGuildmissionStamp(WzID collection_id);
	VOID _StartOneDaysGuildmissionStamp(Player* player, WzID collection_id);
	VOID _CancelOneDaysGuildmissionStamp(Player* player, WzID collection_id);
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
private:
	GameField* m_pField;
	ACTIVE_COLLECTION_HASH m_Hash;				// Ȱ��ȭ�� ä����.
};