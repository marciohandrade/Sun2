#include "stdafx.h"
#include "Gamefield.h"

#include <NPCInfoParser.h>
#include <CollectionParser.h>
//==================================================================================================

#include "./GameServerEx.h"
#include "./ObjectFactory.h"

#include "./Map.h"
#include "./GameZone.h"
#include "./GameInstanceDungeon.h"
#include "./MovingPlanner.h"
#include "./World/GameWarControlManager.h"
#include "./World/HighLoadBalancer.h"
#include "./TriggerManagerEx.h"

#include "./SummonedNPCs.h"
#include "./NpcAI/TargetFinder.h"
#include "./NpcAI/NPCStateManager.h"
#include "./UnitGroupManager.h"
#include "./UnitGroupInfoManager.h"
#include "./DropManager.h"
#include "./EffectManager.h"
#include "./Status/StatusManager.h"

#include "./Player.h"
#include "./Monster.h"
#include "./LuckyMonster.h"
#include "./SSQMonster.h"
#include "./MapObject.h"
#include "./Item.h"
#include "./MapNpc.h"
#include "./TotemNpc.h"
#include "./Summoned.h"
#include "./FriendMonster.h"
#include "./SyncMerchant.h"

#include "./Collection.h"
#include "./CollectionManager.h"
#include "./CrystalWarp.h"
#include "GameMissionRoom.h"

//==================================================================================================
__IMPL_CUSTOMPOOL_PTR(GameField);

const DWORD GameField::SECTOR_SYNC_UPDATE_INTERVAL = 50;

GameField::GameField()
    : instanced_resourse_(false)
	, m_pHLB( 0 )
    , npc_trigger_events_(NULL)
{
	m_pEffectManager	= new EffectManager( this );
	m_pTriggerManager	= new TriggerManagerEx;
	m_pUnitRegenManager	= new UnitRegenManager;
	m_pUnitGroupMgr		= new UnitGroupManager;
	m_pCollectionManager = new CollectionManager(this);

	m_SyncUpdater.SetTimer(SECTOR_SYNC_UPDATE_INTERVAL);
}

GameField::~GameField()
{
	SAFE_DELETE( m_pUnitGroupMgr );
	SAFE_DELETE( m_pUnitRegenManager );
	SAFE_DELETE( m_pEffectManager );
	SAFE_DELETE( m_pTriggerManager );
	SAFE_DELETE( m_pCollectionManager );
    if (npc_trigger_events_) {
        TAllocDelete(ns_trigger::TriggerNpcLinkMatchTable, npc_trigger_events_);
        npc_trigger_events_ = NULL;
    };
}



VOID GameField::Release()
{
	m_pEffectManager->Release();
	m_pTriggerManager->Release();
	m_pCollectionManager->Release();
    //__NA000000_20090320_DELAY_ITEM_DROP_TIME
	g_DropManager.OnFieldRelease(m_pGameZone->GetKey());

	//	(WAVERIX)(080816)(MINOR-FIX)
	//	ObjectFactory::Instance()->		==>		pObjectFactory->

	ObjectFactory* const pObjectFactory = ObjectFactory::Instance();
	{
		// NPC
		for(_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end(); ++it)
		{
			NPC* npc = it->second;
            if (npc != NULL)
            {
                npc->OnLeaveField();
            }		
		}
        for(_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end(); ++it)
        {
            NPC* npc = it->second;
            if (npc != NULL)
            {
                pObjectFactory->FreeObject(npc);
                npc = NULL;
            }            
        }
	}

	{
		// MapObject
		_MAPOBJECT_HASH::iterator it = m_MapObjectHash.begin(), end = m_MapObjectHash.end();
		for( ; it != end ; ++it )
		{
			MapObject* pMapObject = it->second;
			pMapObject->OnLeaveField();
			pObjectFactory->FreeObject( pMapObject );
		}
	}

	{
		// 嬴檜蠱 薯剪
		_ITEM_HASH::iterator end = m_FieldItemHash.end();
		for( _ITEM_HASH::iterator it = m_FieldItemHash.begin(); it != end ; ++it )
		{
			Item* pItem = it->second;
			pItem->OnLeaveField();
			pObjectFactory->FreeObject( pItem );
		}
	}
	{
		// 瓣餵僭 薯剪
		_COLLECTION_HASH::iterator end = m_FieldCollectHash.end();
		for( _COLLECTION_HASH::iterator it = m_FieldCollectHash.begin(); it != end ; ++it )
		{
			Collection* pCollection = it->second;
			pCollection->OnLeaveField();
			pObjectFactory->FreeObject( pCollection );
		}
	}
	m_FieldCollectHash.clear();
    //
	m_NPCHash.clear();
	m_NPCHashForUnitID.clear();
	m_MapObjectHash.clear();
	m_FieldItemHash.clear();
	m_PlayerHash.clear();
	m_CharacterHash.clear();
	
	m_pUnitGroupMgr->Clear();
	m_pUnitRegenManager->Release();

    instanced_resourse_ = false;
    if (npc_trigger_events_) {
        TAllocDelete(ns_trigger::TriggerNpcLinkMatchTable, npc_trigger_events_);
        npc_trigger_events_ = NULL;
    };
    // NOTE: f110531.4L
    HighLoadBalancer::Instance()->UnRegisterBuffer( this );

	Field::Release();
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

//   GameField::BuildSector曖 �躅�
VOID GameField::BuildInstanceInfo()
{
    if (instanced_resourse_) {
        return;
    };
    // お葬剪縑憮 MapObject曖 薯橫蒂 ж朝 掘僥檜 襄營 ж嘎煎 煎註(TriggerManager->Init())
    // 檜瞪縑 儅撩 п撿 ж雖虜, 斜歇唳辦 MapObject 儅撩縑 曖и BuildInstanceInfo()曖 營晦 ��轎檜 嫦儅 й 熱 氈棻.
    // instanced_resourse_ 撲薑擊 譆鼻嬪煎 滲唳 營晦 ��轎擊 虞朝棻.
    // 晦襄 BuildInstanceInfo陛 營晦 ��轎 腎歷湍 睡碟檜 曖紫脹 匙檜塭賊 檜�� 餌檜萄 檜めお蒂 蕨難ж晦
    // 橫溥遴 睡碟檜 氈棻. ъ釭 Init()縑 "お葬剪蒂 舒廓 蟾晦�� ж雖 彊朝棻." 朝 薑曖陛 氈朝 勘煎 爾嬴
    // 僥薯陛 橈橫 爾檣棻. TriggerManagerEx::m_bInitCompleted 高曖 餌辨 寞徹戲煎 爾嬴 BuildInstanceInfo 曖
    // 營晦蒂 虞晦 嬪и 寞ら戲煎 爾罹 霞棻. (輿曖 в蹂) [5/18/2010 lst1024]

    instanced_resourse_ = true;

    if( m_MapObjectHash.size() == 0 )
        AddMapObjectAll();

	//	お葬剪 煎註
    if( m_tszTriggerGroupName[0] != '0')
	{
		TriggerGroupInfo* pGroupInfo = GetFieldInfo()->GetTriggerGroupInfo( m_tszTriggerGroupName );
		ASSERT( pGroupInfo && "お葬剪 斜瑜檜 橈蝗棲棻!!(World.txt霤堅)" );

		if( pGroupInfo )
		{
        #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
            if (npc_trigger_events_ == NULL) {
                npc_trigger_events_ = TAllocNew(ns_trigger::TriggerNpcLinkMatchTable);
            };
            npc_trigger_events_->Init(this);
            m_pTriggerManager->Init(*pGroupInfo, this);
            // post event
            if (npc_trigger_events_->IsExistTriggerAreaEvent() == false) {
                TAllocDelete(ns_trigger::TriggerNpcLinkMatchTable, npc_trigger_events_);
                npc_trigger_events_ = NULL;
            }
        #else
            m_pTriggerManager->Init(*pGroupInfo, this);
        #endif
		}
		else
		{
			SUNLOG( eCRITICAL_LOG
				, "Map:%d,Field:%d,TriggerGroupName:%s"
				, m_pGameZone->GetMapCode()
				, m_pFieldInfo->GetFieldCode()
				, m_tszTriggerGroupName
				);
		}
	}
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

VOID
GameField::Init(
				GameZone* pZone
				, FieldInfo* pFieldInfo
				, const TCHAR* pszGroupName
				, UnitRegenInfoManager* pUnitRegenInfoMgr
				, UnitGroupInfoManager* pUnitGroupInfoMgr
				, bool bPreMapLoading, bool bUsingViewPort, bool bPreRegen
				)
{
	m_pGameZone = pZone;

	m_bPreRegen = bPreRegen;
	if( pszGroupName )
	{
		_tcsncpy( m_tszTriggerGroupName, pszGroupName, MAX_MAPNAME_LENGTH );
		//	(WAVERIX)(080815)(BUG-FIX)	- Buffer Overrun
		//	- (OLD) CHAR m_tszTriggerGroupName[MAX_MAPNAME_LENGTH];		摹樹縑 渠и
		//		m_tszTriggerGroupName[MAX_MAPNAME_LENGTH] = '\0';		籀葬
		//	- (NEW) CHAR m_tszTriggerGroupName[MAX_MAPNAME_LENGTH+1];	(滲唳)
		m_tszTriggerGroupName[MAX_MAPNAME_LENGTH] = '\0';
	}
	else
	{
		m_tszTriggerGroupName[0] = '0';
		m_tszTriggerGroupName[1] = '\0';
	}

    instanced_resourse_ = false;
    // NOTE: f110531.4L
    HighLoadBalancer::Instance()->RegisterBuffer( this );

	Field::Init( pFieldInfo, bPreMapLoading, bUsingViewPort, bPreRegen );

	m_pMovingPlanner->Init( this );

	ASSERT( m_pUnitGroupMgr->GetCount() == 0 );
	m_pUnitGroupMgr->Clear();
	m_pUnitGroupInfoMgr = pUnitGroupInfoMgr;

	m_pUnitRegenManager->Init( this, pUnitRegenInfoMgr, bPreRegen );

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    //葆擊檜賊 Init()縑憮 BuildInstanceInfo()蒂 ��轎ж堅
    //葆擊檜 嬴棲賊 EnterField()縑憮 и棻
    if (pZone->GetZoneType() == eZONETYPE_VILLAGE) {
        BuildInstanceInfo();
    }
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

BOOL GameField::EnterField( Object *pObject, const WzVector *pwzVec, int iAngle )
{
	const eOBJECT_TYPE objectType = pObject->GetObjectType();

	//	(WAVERIX)(080917)(NOTE)
	//	- 檜勘 熱чи 檜嶸朝 嬴鼠楚紫 橾瞪曖 碳榆 婦葬 CharacterHash縑憮 僥薯蒂 撿晦ц擊 陛棟撩檜 氈棻. 匐隸 �� 碳в蹂ж啪 虜菟濠.
	if( NULL != FindObject( objectType, pObject->GetObjectKey() ) ) 
		return FALSE;

    //__NA_276_20070213_UNIT_TRIGGER
    DWORD npc_unit_id = 0;
    if (pObject->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* pNPC = static_cast<NPC*>(pObject);
        npc_unit_id = pNPC->GetRegenID();
        if (npc_unit_id != 0)
        {
            if( NULL != FindNPCForUnitID(npc_unit_id))
                return FALSE;
        }
    }
	//	(WAVERIX)(080917)(BUG-FIX)	Field::EnterField 鼻鷓縑憮 GameField縑 跡煙檜 橈朝 鼻鷓縑憮 籀葬腎朝 鼻�� 寡薯 跡瞳
	_AddObject( pObject );
	const BOOL bEnterField = Field::EnterField( pObject, pwzVec, iAngle );
	if( !bEnterField )
	{
		_DeleteObject( pObject );
		return FALSE;
	}

	GameZone* const pGameZone = GetGameZone();
	if( !pGameZone )	// GameZone檜 橈朝等 撩奢擊 奩�納炾� 氈棻. 斜 檜嶸 塽 顫渡撩擊 匐隸ж塭. (橾欽擎 僥曖л... 闔闔ж雖虜 剩橫陛濠.
		return TRUE;

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    if (pGameZone->GetZoneType() != eZONETYPE_VILLAGE) {
        BuildInstanceInfo();
    }
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

	pGameZone->SetZone( pObject, GetFieldCode(), 0, NULL );
	if((objectType & CHARACTER_OBJECT) == CHARACTER_OBJECT)
	{
		Character* const pCharacter = static_cast<Character*>(pObject);
		pCharacter->ChangePolicy(pGameZone);

        // (CHANGES) (f100514.3L) add the flag to prevent ANY actions that
        // a player do a delayed spiking whether the player request a LEAVE.
        pCharacter->GetStatusManager()->ChangeInterState(eCHAR_STATE_ZONE_TRANSACTION, false);
		//	奢問 碳陛 鼻鷓煎 瞪�紗藷眼�.
		sPOLICY_ZONE& rPolicyment = pGameZone->Policyment;
		if(rPolicyment.FIELD & rPolicyment.ENABLE_SSQ_CONTROL)
		{   //assert( GetTypeOfIndun() == INDUN_KIND_SS_QUEST );
			GameWarControlManager::OnMsg(GameSSQCtrlCmd::CharacterState::Enter(pCharacter),
                                         static_cast<GameInstanceDungeon*>(pGameZone));
		}
	}
#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    if (npc_unit_id != 0 && npc_trigger_events_ != NULL)
    {
        if (npc_trigger_events_->CheckExistAreaEvent(npc_unit_id))
        {
            NPC* npc = static_cast<NPC*>(pObject);
            const sPOLICY_CHARACTER& CharPolicy = npc->Policyment;
            npc->ChangePolicyPostEnterField(CharPolicy.PCC_EXIST_AREA_EVENT_NPC,
                                            CharPolicy.PCC_DISABLE_ALL,
                                            NULL);
        }
    }
#endif

    if (npc_trigger_events_ != NULL && pObject->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* npc = static_cast<NPC*>(pObject);
        MONSTERCODE monster_code = npc->GetBaseInfo()->m_MonsterCode;
        if (npc_trigger_events_->CheckExistMonsterAreaEvent(monster_code))
        {
            const sPOLICY_CHARACTER& CharPolicy = npc->Policyment;
            npc->ChangePolicyPostEnterField(CharPolicy.PCC_EXIST_AREA_EVENT_NPC,
                CharPolicy.PCC_DISABLE_ALL,
                NULL);

            //!~ arycoat 2014.10.16
            const int iTile = npc->GetPathExplorer()->GetTile();
            if (GameField* game_field = npc->GetField()) {
                if (ns_trigger::TriggerNpcLinkMatchTable* table =
                    game_field->GetNpcTriggerEvents())
                {
#ifdef  NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
					npc->Update(1);
#endif //  NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
                    table->OnChangeTile(-1, iTile, npc);
                }
            }
            //~!
        }
    }

	return TRUE;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


BOOL GameField::LeaveField( Object *pObject )
{
	//	(WAVERIX)(080917)(BUG-FIX)	Field::LeaveField 檜�� Object 薯剪ж紫煙 熱薑
	//	幗ぷ 蟾晦�降� в蹂ж棻.
	//	顫塊и 餌錳 婦溼
	if(pObject->IsEqualObjectKind(CHARACTER_OBJECT))
	{
		Character* const pCharacter = static_cast<Character*>( pObject );
        // (CHANGES) (f100514.3L) add the flag to prevent ANY actions that
        // a player do a delayed spiking whether the player request a LEAVE.
        pCharacter->GetStatusManager()->ChangeInterState(eCHAR_STATE_ZONE_TRANSACTION, true);
		GameZone* const pGameZone = GetGameZone();
		sPOLICY_ZONE& rPolicyment = pGameZone->Policyment;
		if( rPolicyment.FIELD & rPolicyment.ENABLE_SSQ_CONTROL )
		{
			GameWarControlManager::OnMsg( GameSSQCtrlCmd::CharacterState::Leave( pCharacter ), static_cast<GameInstanceDungeon*>(pGameZone) );
		}
	}

	const BOOL bLeavedSuccess = Field::LeaveField( pObject );
	_DeleteObject( pObject );
    // NOTE: f110531.4L
    if (m_PlayerHash.empty()) {
        HighLoadBalancer::Instance()->Update(this, true);
    }

	if( !bLeavedSuccess )
		SUNLOG(eCRITICAL_LOG, "Field::LeaveField Fail Error! Caution!!!!!!!!!! %8X",
               pObject->GetObjectType());

	return bLeavedSuccess;
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

MapNpc* GameField::FindMapNpc(const Object::Key object_key) const
{
    const MapObject* const map_object = FindMapObject(object_key);
    if (map_object == NULL)
    {
        return NULL;
    }
    const Object::Key link_obj_key = map_object->GetLinkedNpcObjectKey();
    return static_cast<MapNpc*>(FindObject(MAPNPC_OBJECT, link_obj_key));
}

MapObject* GameField::FindMapObject(const Object::Key object_key) const
{
    _MAPOBJECT_HASH::const_iterator it = m_MapObjectHash.find(object_key);
    if (it != m_MapObjectHash.end())
    {
        Object* const object = it->second;
        if (obj_util::IsValid(object, __FUNCTION__, __LINE__) == false)
        {
            return NULL;
        }
        return it->second;
    }
    return NULL;
}

Character* GameField::FindCharacter(const Object::Key object_key) const
{
    _CHARACTER_HASH::const_iterator it = m_CharacterHash.find(object_key);
    if (it != m_CharacterHash.end())
    {
        Object* const object = it->second;
        if (obj_util::IsValid(object, __FUNCTION__, __LINE__) == false)
        {
            return NULL;
        }
        return it->second;
    }
    return NULL;
}

NPC* GameField::FindNPCForUnitID(const DWORD unit_id) const
{
    _NPC_HASH::const_iterator it = m_NPCHashForUnitID.find(unit_id);
    if (it != m_NPCHashForUnitID.end())
    {
        Object* const object = it->second;
        if (obj_util::IsValid(object, __FUNCTION__, __LINE__) == false)
        {
            return NULL;
        }
        return it->second;
    }
    return NULL;
}

void GameField::_LinkedMapNpc(Object* pObj)
{
    ASSERT(pObj);
    ASSERT(MAPNPC_OBJECT == pObj->GetObjectType());

    MapNpc* pMapNpc = static_cast<MapNpc*>(pObj);
    MapObject* pMapObject = FindMapObject(pMapNpc->GetMapObjectID());

    if (NULL != pMapObject)
        pMapObject->SetLinkedNpcObjectKey(pMapNpc->GetObjectKey());
}

void GameField::_UnLinkedMapNpc(Object* pObj)
{
    ASSERT(pObj);
    ASSERT(MAPNPC_OBJECT == pObj->GetObjectType());

    MapNpc* pMapNpc = static_cast<MapNpc*>(pObj);
    MapObject* pMapObject = FindMapObject(pMapNpc->GetMapObjectID());

    if (NULL != pMapObject)
        pMapObject->SetLinkedNpcObjectKey(0);
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

void GameField::_AddObject( Object* pObj )
{
	if( NULL != FindObject( pObj->GetObjectType(), pObj->GetObjectKey() ) ) 
		return;

	switch( pObj->GetObjectType() )
	{
	case OBJECT_OBJECT:
	case CHARACTER_OBJECT:
	case NONCHARACTER_OBJECT:
	case NPC_OBJECT:
		ASSERT(!"NPC_OBJECT Object add Error!");
		break;
	case PLAYER_OBJECT:
		m_PlayerHash[pObj->GetObjectKey()] = static_cast<Player*>(pObj);
		m_CharacterHash[pObj->GetObjectKey()] = static_cast<Character*>(pObj);
		break;
    case MONSTER_OBJECT:
	case SUMMON_OBJECT:
    case MAPNPC_OBJECT:
	case MERCHANT_OBJECT:
	case LUCKYMON_OBJECT:
	case TOTEMNPC_OBJECT:
	case SSQMONSTER_OBJECT:
    case CRYSTALWARP_OBJECT:
    case FRIEND_MONSTER_OBJECT:
    case SYNC_MERCHANT_OBJECT:
        if (NPC* npc = static_cast<NPC*>(pObj))
        {
            DWORD object_key = npc->GetObjectKey();
            m_NPCHash[object_key] = npc;
            m_CharacterHash[object_key] = npc;
            const DWORD unit_id = npc->GetRegenID();
            if (unit_id == 0) {
                break;
            };
            _NPC_HASH::_Pairib result = \
                m_NPCHashForUnitID.insert(_NPC_HASH::value_type(unit_id, npc));
            if (result.second == false) {
                assert(!"unexpected status");
                SUNLOG(eCRITICAL_LOG, "Can not add m_NPCHashForUnitID map(%u)", unit_id);
                break;
            };
        };
        break;
	case ITEM_OBJECT:
	case MONEY_OBJECT:
		m_FieldItemHash[pObj->GetObjectKey()] = static_cast<Item *>(pObj);
		break;
	case COLLECTION_OBJECT:	
		m_FieldCollectHash[pObj->GetObjectKey()] = static_cast<Collection*>(pObj);
		break;
	default:
		ASSERT(!"default Object add Error!");
		break;
	}

    if (MAPNPC_OBJECT == pObj->GetObjectType())
        _LinkedMapNpc(pObj);
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


bool GameField::_DeleteObject( Object* pObj )
{
    const eOBJECT_TYPE objType = pObj->GetObjectType();

	if( NULL == FindObject( objType, pObj->GetObjectKey() ) ) 
		return false;

    if (MAPNPC_OBJECT == pObj->GetObjectType())
        _UnLinkedMapNpc(pObj);

	switch( pObj->GetObjectType() )
	{
	case OBJECT_OBJECT:
	case CHARACTER_OBJECT:
	case NONCHARACTER_OBJECT:
	case NPC_OBJECT:
		ASSERT(!"NPC_OBJECT Object rem Error!");
		return false;
	case PLAYER_OBJECT:
		m_PlayerHash.erase( pObj->GetObjectKey() );
		m_CharacterHash.erase( pObj->GetObjectKey() );
		break;
	case MONSTER_OBJECT:
	case SUMMON_OBJECT:
	case MERCHANT_OBJECT:
	case MAPNPC_OBJECT:
	case LUCKYMON_OBJECT:
	case TOTEMNPC_OBJECT:
	case SSQMONSTER_OBJECT:
    case CRYSTALWARP_OBJECT:
    case FRIEND_MONSTER_OBJECT:
    case SYNC_MERCHANT_OBJECT:
        if (DWORD object_key = pObj->GetObjectKey())
        {
            m_NPCHash.erase(object_key);
            m_CharacterHash.erase(object_key);
            const DWORD unit_id = static_cast<NPC*>(pObj)->GetRegenID();
            if (unit_id != 0) {
                m_NPCHashForUnitID.erase(unit_id);
            };
        };
        break;
	case ITEM_OBJECT:
	case MONEY_OBJECT:
		m_FieldItemHash.erase( pObj->GetObjectKey() );
		break;
	case COLLECTION_OBJECT:	
		m_FieldCollectHash.erase( pObj->GetObjectKey() );
		break;
	default:
		ASSERT(!"default Object rem Error!");
		return false;
	}

	return true;
}

//==================================================================================================

void GameField::DestroyObject( Object* pObj )
{
    // (NOTE) important, designed by __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__
    pObj->RequestDelete();
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


VOID GameField::AddMapObjectAll()
{
	ASSERT( m_pSharedFieldInfo );

	// 裘螃粽薛お 儅撩
	MAPOBJECT_INFO_HASH & rMapObjectInfoTable = m_pSharedFieldInfo->GetMapObjectInfoHashTable();
	MAPOBJECT_INFO *pMapInfo = NULL;
	MAPOBJECT_INFO_HASH_IT it, end = rMapObjectInfoTable.end();
	for( it = rMapObjectInfoTable.begin() ; it != end ; ++it )
	{
		pMapInfo = (it->second);
		
		// 裘螃粽薛お
		MapObject* pMapObject = static_cast<MapObject *>(ObjectFactory::Instance()->AllocObject(MAP_OBJECT));
		pMapObject->Init();
		pMapObject->SetObjectKey( pMapInfo->dwMapObjectID );
		pMapObject->SetPos(	&pMapInfo->wvPos );
		m_MapObjectHash[pMapObject->GetObjectKey()] = pMapObject;

		DWORD dwCode = pMapInfo->dwExtraInfos[eMAPOBJECT_MONSTER];
		ASSERT(SAFE_NUMERIC_TYPECAST( DWORD, dwCode, WORD ));
		WORD NPCCode = (WORD)dwCode;

        //_NA_0_20100222_UNIT_TRIGGERS_REGEN
        // 裘螃粽薛お縑 翱唸脹 跨蝶攪曖 唳辦 嘐葬 儅撩腎橫 霞棻. 
        // 蕨諼煎 ⑦煩曖 撩 憮粽 嘐暮曖 唳辦 お葬剪 擋暮縑 曖п 儅撩腎橫 霞棻.
        const DWORD dwRegenType = pMapInfo->dwExtraInfos[eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE];
        const bool  bPreRegen   = !((dwRegenType & REGEN_TYPE_TRIGGER) == REGEN_TYPE_TRIGGER);

        if( 0 != dwCode && false != bPreRegen)
		{
			if( !NPCInfoParser::Instance()->GetNPCInfo( NPCCode ) )
			{
				SUNLOG( eCRITICAL_LOG, "[Field:%u]MapObject Monster(%u) Is Not Found", GetFieldCode(), pMapInfo->dwExtraInfos[eMAPOBJECT_MONSTER] );
				SUNLOG( eCRITICAL_LOG, "[Field:%u]MapObject(ID:%u),(ExtraInfoValue)[TYPE(1):%u,MONSTER(2):%u,(Reserved:%u,%u)]", 
					GetFieldCode(), 
					pMapInfo->dwMapObjectID,
					pMapInfo->dwExtraInfos[eMAPOBJECT_TYPE],
					pMapInfo->dwExtraInfos[eMAPOBJECT_MONSTER], 
					pMapInfo->dwExtraInfos[eMAPOBJECT_USE_ALPHA], 
					pMapInfo->dwExtraInfos[eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE]
					 );
			}
			else
			{
				MapNpc* pNPC = CreateMapNPC( NPCCode, &pMapInfo->wvPos, pMapInfo );
				ASSERT( pNPC );
			}
		}

		// (lst1024)(瓣餵) в萄 儅撩衛 螃粽薛お醞 瓣餵僭擎 賅舒 儅撩п 菩棻.
		if( 0 != pMapInfo->CollectionID )
		{
			if( NULL == CollectionParser::Instance()->FindCollectionInfo(pMapInfo->CollectionID) )
			{
				char ch[5] = {0,};

				SUNLOG( eCRITICAL_LOG, 
					"瓣餵僭 薑爾蒂 瓊擊熱 橈蝗棲棻. " 
					"Field[%u] MapObjectKey[%u] CollectionID[%s] "
					"ExtraInfoValue[TYPE(1):%u,MONSTER(2):%u,(Reserved:%u,%u)]", 
					GetFieldCode(), 
					pMapInfo->dwMapObjectID,
					WzIDToString(ch,pMapInfo->CollectionID),
					pMapInfo->dwExtraInfos[eMAPOBJECT_TYPE],
					pMapInfo->dwExtraInfos[eMAPOBJECT_MONSTER], 
					pMapInfo->dwExtraInfos[eMAPOBJECT_USE_ALPHA], 
					pMapInfo->dwExtraInfos[eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE] );
			}
			else
			{
				Collection* pCollection = CreateCollection( pMapInfo );
				ASSERT( pCollection );
			}
		}
	}
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


VOID
GameField::Update(DWORD delta_tick)
{
    if (!IsReady()) {
        return;
    }

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    GameZone* zone = GetGameZone();
    if (zone != NULL && zone->GetZoneType() == eZONETYPE_VILLAGE) 
    {
        m_pTriggerManager->Update();
        if (m_PlayerHash.empty()) {
            return;
        }
    }
    else
    {
        if (m_PlayerHash.empty()) {
            return;
        }
        m_pTriggerManager->Update();
    }
#else

    if (m_PlayerHash.empty()) {
        return;
    }
    m_pTriggerManager->Update();
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

	m_pUnitRegenManager->Update();
	m_pEffectManager->Update( GetTickCount() );
	m_pCollectionManager->Update();

	ObjectFactory* const object_factory = ObjectFactory::Instance();
	
    // 嬴檜蠱
    for (_ITEM_HASH::iterator it = m_FieldItemHash.begin(); it != m_FieldItemHash.end();)
    {
        Item* item = it->second;
        if (item == NULL)
        {
            m_FieldItemHash.erase(it++);
        }
        else if (item->Update(delta_tick) == FALSE)
        {
            Field::LeaveField(item);
            m_FieldItemHash.erase(it++);
            object_factory->FreeObject(item);
            item = NULL;
        }
        else
        {
            ++it;
        }
    }

    // 跨蝶攪
    for (_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end();)
    {
        NPC* npc = it->second;
        if (npc == NULL)
        {
            m_NPCHash.erase(it++);
        }
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        else if (npc->IsDeleted())
        {
            const Object::Key key = npc->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            m_NPCHash.erase(it++);
            m_CharacterHash.erase(key);
        }
//#endif
        else if (npc->Update(delta_tick) == FALSE)
        {
            const CODETYPE monster_object_key = npc->GetObjectKey();
            if (npc->IsEqualObjectKind(MONSTER_OBJECT))
            {
                if (npc->GetUnitRegenInfo()) //< 歜衛
                {
                    ASSERT(npc->GetRegenID());

                    // 楣酈 跨蝶攪曖 唳辦 模資褻勒縑 評塭 棻擠 葬薇顫歜檜 殖塭霞棻.
                    DWORD regen_time = npc->GetUnitRegenInfo()->GetRegenTime();
                    if (npc->IsEqualObjectKind(LUCKYMON_OBJECT))
                    {
                        LuckyMonster* lucky_monster = static_cast<LuckyMonster*>(npc);
                        if (lucky_monster->GetRunAwayRegenTime())
                        {
                            regen_time = lucky_monster->GetRunAwayRegenTime();
                        }	
                    }
                    m_pUnitRegenManager->AddOnWaiting(npc->GetRegenID(), regen_time);
                }
            }

            Field::LeaveField(npc);

            m_NPCHash.erase(it++);
            m_CharacterHash.erase(monster_object_key);	//	(WAVERIX)(080822)(BUG-FIX)	erase陛 橈歷棻.

            //__NA_276_20070213_UNIT_TRIGGER
            const DWORD regen_id = npc->GetRegenID();
            if (regen_id != 0)
            {
                m_NPCHashForUnitID.erase(regen_id);
            }
            object_factory->FreeObject(npc);
            npc = NULL;
        }
        else
        {
            ++it;
        }
    }
	
    Field::Update(delta_tick);
    
    // NOTE: f110531.4L
    if (m_SyncUpdater.IsExpired()) 
    {
        HighLoadBalancer::Instance()->Update(this, true);
    }
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Item* GameField::CreateItem( const WzVector* pvPos, DWORD dwPlayerKey, DWORD dwFromMonsterKey, SCItemSlot & slot )
{
	Item* pFieldItem = static_cast<Item *>(ObjectFactory::Instance()->AllocObject( ITEM_OBJECT ));
	pFieldItem->Init();
	pFieldItem->CreateItem( dwPlayerKey, dwFromMonsterKey, slot );
	if( FALSE == EnterField( pFieldItem, pvPos, 0 ) )
	{
		ObjectFactory::Instance()->FreeObject( pFieldItem );
		SUNLOG( eCRITICAL_LOG, "[Item EnterField] Pos[%f, %f, %f] is failed!", pvPos->x, pvPos->y, pvPos->z );
		return FALSE;
	}
	return pFieldItem;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Item* GameField::CreateMoney( const WzVector* pvPos, DWORD dwPlayerKey, DWORD dwFromMonsterKey, const MONEY& Money)
{
	Item* pFieldMoneyItem = static_cast<Item *>(ObjectFactory::Instance()->AllocObject( MONEY_OBJECT ));
	pFieldMoneyItem->Init();
	pFieldMoneyItem->CreateMoney( dwPlayerKey, dwFromMonsterKey, Money );

	if( FALSE == EnterField( pFieldMoneyItem, pvPos, 0 ) )
	{
		ObjectFactory::Instance()->FreeObject( pFieldMoneyItem );
		SUNLOG( eCRITICAL_LOG, "[Item EnterField] Pos[%f, %f, %f] is failed!", pvPos->x, pvPos->y, pvPos->z );
		return NULL;
	}
	return pFieldMoneyItem;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

#include <GroupParser.h>

Monster* GameField::CreateMonster( WORD NPCCode, const WzVector *pvPos, 
									  WORD wGroupID, int iAngle, BYTE byMoveType, DWORD dwMoveAreaID, WORD MissionAreaID/*嘐暮瞪辨歜衛*/ )
{
	Monster *pSpawnMonster = AllocMonster( NPCCode, byMoveType, dwMoveAreaID );

	if( !pSpawnMonster ) return NULL;

	if( 0 != wGroupID )
	{
		DWORD dwNewGroupID = MAKEDWORD(MissionAreaID,wGroupID);
		UnitGroupInfo* pGroupInfo = m_pUnitGroupInfoMgr->Find( dwNewGroupID );
		if( !pGroupInfo )
		{
			BASE_GROUPINFO* pBaseGroupInfo = GroupParser::Instance()->GetGroupInfo( wGroupID );
			ASSERT(pBaseGroupInfo);
			if( !pBaseGroupInfo )
			{
				SUNLOG( eCRITICAL_LOG, "[Field:%u],Group[%u,%u] pBaseGroupInfo is NULL", GetFieldCode(), dwNewGroupID, wGroupID );
			}
			else
			{
				pGroupInfo = new UnitGroupInfo;
				pGroupInfo->SetID(dwNewGroupID);
				pGroupInfo->SetLeaderRegenID(0);
				m_pUnitGroupInfoMgr->Insert( dwNewGroupID, pGroupInfo );
			}
		}

		if( pGroupInfo && 0 == pGroupInfo->GetLeaderRegenID() )
		{
			BASE_GROUPINFO* pBaseGroupInfo = GroupParser::Instance()->GetGroupInfo( wGroupID );
			ASSERT(pBaseGroupInfo);
			if( pBaseGroupInfo && NPCCode == pBaseGroupInfo->dwLeaderCode )
			{
				pGroupInfo->SetLeaderRegenID(pSpawnMonster->GetObjectKey());
			}
		}
		else
		{
			//SUNLOG( eCRITICAL_LOG, "斜瑜[%u]檜嘐 葬渦[%u]陛 襄營л[%u]2!", wGroupID, pGroupInfo->GetLeaderRegenID(), NPCCode );
		}

		pSpawnMonster->SetUnitGroupInfo( pGroupInfo );
	}

	if( !EnterField( pSpawnMonster, pvPos, iAngle ) )
	{
		ObjectFactory::Instance()->FreeObject( pSpawnMonster );
		return NULL;
	}

	return pSpawnMonster;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Monster* GameField::CreateSummonByNPC( WORD NPCCode, const WzVector* pvPos, Character *pSummoner, SummonedNPCs *pOwnerSummonNPCs )
{
	BASE_NPCINFO* pInfo = NPCInfoParser::Instance()->GetNPCInfo( NPCCode );
	ASSERT( pInfo );
	if( !pInfo )
	{
		SUNLOG( eCRITICAL_LOG, _T("It isn't exist NPC Info for Code[%u]]"), NPCCode );
		return NULL;
	}


	Monster *pMonster = static_cast<Monster*>(ObjectFactory::Instance()->AllocObject( MONSTER_OBJECT ));
    if (pMonster == NULL) {
        return NULL;
    }

	pMonster->InitMonster( NPCCode, 0, 0 );
	// NPC縑 模�素恇� ん檣攪 奢嶸
	pMonster->SetSummonerKey( pSummoner->GetObjectKey() );
	// 模�素睎� 顫啃擊 蹺瞳ж紫煙 NPC曖 鼻鷓蒂 夥羞棻.
	if( pSummoner->GetTargetChar() )
	{
		pMonster->SetTargetChar( pSummoner->GetTargetChar() );
		pMonster->ChangeState( STATE_ID_HELP );
	}

	if( !EnterField( pMonster, pvPos, -1 ) )
	{
		ObjectFactory::Instance()->FreeObject( pMonster );
		return NULL;
	}

	// 模�素睎� 模�素恇鷟� 跨蝶攪 熱 隸陛
	pOwnerSummonNPCs->IncreaseSummoned( NPCCode, pMonster );

	return pMonster;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Summoned* GameField::CreateSummonByPlayer( WORD NPCCode, const WzVector* pvPos, Character *pSummoner, 
										  SummonedNPCs* pOwnerSummonNPCs, DWORD dwExpireTime, SLOTCODE SkillCode, const PLAYER_SUMMON_INFO* const summon_info)
{
	Summoned *pSummoned = static_cast<Summoned*>(ObjectFactory::Instance()->AllocObject( SUMMON_OBJECT ));
	pSummoned->InitSummoned( NPCCode, SkillCode, pSummoner, dwExpireTime );

    if (summon_info != NULL)
    {
        pSummoned->SetHP(summon_info->hp);
        pSummoned->SetMP(summon_info->mp);
        const eSUMMON_COMMAND command = (summon_info->command == eSUMMON_COMMAND_ATTACK) ? 
            eSUMMON_COMMAND_DELEGATE_ATTACK : summon_info->command;
        pSummoned->SetCommandState(command, 0);
    }

	if( !EnterField( pSummoned, pvPos, -1 ) )
	{
		SUNLOG( eCRITICAL_LOG, "[CreateSummonByPlayer EnterField][Key:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
			pSummoned->GetObjectKey(), NPCCode, pvPos->x, pvPos->y, pvPos->z );
		ObjectFactory::Instance()->FreeObject( pSummoned );
		return NULL;
	}
	// 模�素睎� 模�素恇鷟� 跨蝶攪 熱 隸陛
	pOwnerSummonNPCs->IncreaseSummoned( NPCCode, pSummoned );
    // (CHANGES) (100205) (WAVERIX) Ы溯檜橫縑 曖и 模�紡鶺� 唳辦 AI update蒂 嬪п 撮睡 瑙擊 滲唳.
    const sPOLICY_CHARACTER& CharPolicyment = pSummoned->Policyment;
    pSummoned->ChangePolicyPostEnterField(CharPolicyment.PCC_CONTINUING_NPC_UPDATE,
                                          CharPolicyment.PCC_DISABLE_ALL,
                                          pSummoner);

	return pSummoned;
}

//避擎 跨蝶攪蒂 模�納挬�.
//薯剪�� 模�素睊滇� 儅撩и棻.
// __NA_S00015_20080828_NEW_HOPE_SHADOW
Summoned* GameField::CreateSummonDeadMonster( WORD NPCCode, const WzVector* pvPos, Character *pSummoner, SummonedNPCs* pOwnerSummonNPCs, SLOTCODE SkillCode )
{
	Summoned *pSummoned = static_cast<Summoned*>(ObjectFactory::Instance()->AllocObject( SUMMON_OBJECT ));
	pSummoned->InitSummonedDeadMonster( NPCCode, SkillCode, pSummoner );

	if( !EnterField( pSummoned, pvPos, -1 ) )
	{
		SUNLOG( eCRITICAL_LOG, "[CreateSummonByPlayer EnterField][Key:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
			pSummoned->GetObjectKey(), NPCCode, pvPos->x, pvPos->y, pvPos->z );
		ObjectFactory::Instance()->FreeObject( pSummoned );
		return NULL;
	}
	// 模�素睎� 模�素恇鷟� 跨蝶攪 熱 隸陛
	pOwnerSummonNPCs->IncreaseSummoned( NPCCode, pSummoned );
    // (CHANGES) (100205) (WAVERIX) Ы溯檜橫縑 曖и 模�紡鶺� 唳辦 AI update蒂 嬪п 撮睡 瑙擊 滲唳.
    const sPOLICY_CHARACTER& CharPolicyment = pSummoned->Policyment;
    pSummoned->ChangePolicyPostEnterField(CharPolicyment.PCC_CONTINUING_NPC_UPDATE,
                                          CharPolicyment.PCC_DISABLE_ALL,
                                          pSummoner);

	return pSummoned;
}

CrystalWarp* GameField::CreateSummonCrystalWarp( WORD NPCCode, const WzVector* pvPos, DWORD dwGuildGuid, DWORD dwExpireTime )
{
    CrystalWarp* pCrystalWarp = static_cast<CrystalWarp*>(ObjectFactory::Instance()->AllocObject( CRYSTALWARP_OBJECT ));
    pCrystalWarp->InitCrystalWarp( NPCCode, dwGuildGuid, dwExpireTime );

    if( !EnterField( pCrystalWarp, pvPos, -1 ) )
    {
        SUNLOG( eCRITICAL_LOG, "[CreateSummonCrystalWarp] [Key:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
                                pCrystalWarp->GetObjectKey(), NPCCode, pvPos->x, pvPos->y, pvPos->z );
        ObjectFactory::Instance()->FreeObject( pCrystalWarp );
        return NULL;
    }

    return pCrystalWarp;
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


MapNpc* GameField::CreateMapNPC( WORD NPCCode, const WzVector* pvPos, MAPOBJECT_INFO *pMapInfo )
{
	MapNpc *pMapNpc = static_cast<MapNpc*>(ObjectFactory::Instance()->AllocObject( MAPNPC_OBJECT ));
	pMapNpc->InitMapNPC( NPCCode, pMapInfo );

    int dir = static_cast<int>(WZRAD2DEG(pMapInfo->wzRot.z));
    dir %= 360;
    if (dir < 0)
        dir += 360;

	if( !EnterField(pMapNpc, pvPos, dir) )
	{
		SUNLOG( eCRITICAL_LOG, "[CreateMapNPC EnterField][Key:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
			pMapNpc->GetObjectKey(), NPCCode, pvPos->x, pvPos->y, pvPos->z );

		ObjectFactory::Instance()->FreeObject( pMapNpc );
		return NULL;
	}

	return pMapNpc;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

TotemNpc*	GameField::CreateTotemNPC( WORD NPCCode, const WzVector* pvPos, Player* pSummoner, SCItemSlot & IN slot )
{
	TotemNpc *pTotemNpc = static_cast<TotemNpc*>(ObjectFactory::Instance()->AllocObject( TOTEMNPC_OBJECT ));
	pTotemNpc->InitTotemNpc( NPCCode, pSummoner, slot );

	if( !EnterField( pTotemNpc, pvPos, -1 ) )
	{
		SUNLOG( eCRITICAL_LOG, "[CreateTotemNPC EnterField][Key:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
			pTotemNpc->GetObjectKey(), NPCCode, pvPos->x, pvPos->y, pvPos->z );

		ObjectFactory::Instance()->FreeObject( pTotemNpc );
		return NULL;
	}

	return pTotemNpc;
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

//{__NA001283_20090225_COLLECTION_USE
Collection* GameField::CreateCollection( MAPOBJECT_INFO *pMapInfo )
{
	Collection* pCollection = static_cast<Collection*>(ObjectFactory::Instance()->AllocObject( COLLECTION_OBJECT ));
	pCollection->InitCollection( pMapInfo );
	pCollection->SetObjectKey( pMapInfo->dwMapObjectID );
	pCollection->SetPos( &pMapInfo->wvPos );

    int dir = static_cast<int>(WZRAD2DEG(pMapInfo->wzRot.z));
    dir %= 360;
    if (dir < 0)
        dir += 360;

	if( !EnterField( pCollection, &pMapInfo->wvPos, dir ) )
	{
		char ch[5] = {0,};
		SUNLOG( eCRITICAL_LOG, "[CreateCollection EnterField][Key:%u,wzId:%s] Pos[%f, %f, %f] is failed!", 
			pCollection->GetObjectKey(), WzIDToString(ch, pMapInfo->CollectionID), pMapInfo->wvPos.x, pMapInfo->wvPos.y, pMapInfo->wvPos.z );

		ObjectFactory::Instance()->FreeObject( pCollection );
		return NULL;
	}

	return pCollection;
}
//}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Monster*	GameField::SpawnMonster( UnitRegenInfo* pUnitRegenInfo )
{
    int npc_code = pUnitRegenInfo->GetRandomNPCCode();
    const int npc_unit_type = pUnitRegenInfo->GetUnitType();

#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    ;{
        MONSTERCODE replaace_monster_code = \
            AppliedNpcDependencyChain::Instance()->GetRepleaceMonsterCode(GetGameZone(), npc_code, true);

        if (replaace_monster_code) {
            npc_code = replaace_monster_code;
        }
    };
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

	Monster* pSpawnMonster = AllocMonster( npc_code, npc_unit_type, 0 );

	if( !pSpawnMonster )
	{
		SUNLOG( eCRITICAL_LOG, "[SpawnMonster] AllocMonster Fail!" );
		return NULL;
	}

	pSpawnMonster->SetUnitRegenInfo( pUnitRegenInfo );

	if( 0 != pUnitRegenInfo->GetGroupID() )
	{
		UnitGroupInfo* pGroupInfo = m_pUnitGroupInfoMgr->Find( pUnitRegenInfo->GetGroupID() );
		if( pGroupInfo )
			pSpawnMonster->SetUnitGroupInfo( pGroupInfo );
	}

    const WORD pos_index = pUnitRegenInfo->GetRandomPosIndex();
    const WzVector& vStartPos = pUnitRegenInfo->GetStartPos(pos_index);

    NPCStateManager* const state_manager = pSpawnMonster->GetStateManager();
    state_manager->SetMovePosIndex(pos_index);

	if( pUnitRegenInfo->GetDir() > 360 )
	{
		SUNLOG( eCRITICAL_LOG, "[SpawnMonster] Dir[%f] is over!", pUnitRegenInfo->GetDir() );
		pUnitRegenInfo->SetDir( (float)( (int)pUnitRegenInfo->GetDir() % 360 ) );
	}

	if( FALSE == EnterField( pSpawnMonster, &vStartPos, (int)pUnitRegenInfo->GetDir() ) )
	{
		ObjectFactory::Instance()->FreeObject( pSpawnMonster );
		SUNLOG( eCRITICAL_LOG, "[SpawnMonster EnterField][Field:%u,Code:%u] Pos[%f, %f, %f] is failed!", 
			GetFieldCode(), npc_code, vStartPos.x, vStartPos.y, vStartPos.z );
		return NULL;
	}

	return pSpawnMonster;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Monster*	GameField::AllocMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID )
{
	
	BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( NPCCode );
	if( !pBaseNPCInfo )
	{
		SUNLOG( eCRITICAL_LOG, "[AllocMonster] MonterCode(%d)IsNotFound", NPCCode );
		return NULL;
	}

	Monster *pSpawnMonster = NULL;

	ObjectFactory* const pObjectFactory = ObjectFactory::Instance();
	switch( pBaseNPCInfo->m_byGrade )
	{
	case eNPC_LUCKY_MONSTER:
		{
			LuckyMonster* pLuckyMonster = static_cast<LuckyMonster *>( pObjectFactory->AllocObject( LUCKYMON_OBJECT ) );
			pSpawnMonster = pLuckyMonster;
		}
		break;
	case eNPC_SSQ_MONSTER:
		{
			SSQMonster* pMonster = static_cast<SSQMonster *>( pObjectFactory->AllocObject( SSQMONSTER_OBJECT ) );
			pSpawnMonster = pMonster;
		}
		break;
    case eNPC_FRIEND_MONSTER:
        {
            FriendMonster* pFriendMonster = static_cast<FriendMonster*>(pObjectFactory->AllocObject(FRIEND_MONSTER_OBJECT));
            pSpawnMonster = pFriendMonster;
        }
        break;
    case eNPC_MOVE_SYNC_MERCHANT:
        {
            SyncMerchant* pSyncMerchant = static_cast<SyncMerchant*>( pObjectFactory->AllocObject( SYNC_MERCHANT_OBJECT ) );
            pSpawnMonster = pSyncMerchant;
        }
        break;
	default:
		{
			Monster* pMonster = static_cast<Monster *>( pObjectFactory->AllocObject( MONSTER_OBJECT ) );
			pSpawnMonster = pMonster;
		}
		break;
	}

    if (pSpawnMonster == NULL) {
        SUNLOG( eCRITICAL_LOG, "[AllocMonster] MonterCode(%d)/Type(%d) alloc fail", NPCCode, pBaseNPCInfo->m_byGrade );
        return NULL;
    }

	pSpawnMonster->InitMonster( NPCCode, byMoveType, dwMoveAreaID );

	return pSpawnMonster;
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

void GameField::KillAllNPC(Player* player, int range, const FLOAT damage_percent)
{
	const WzVector& player_pos = *player->GetPosPtr();
    const float range_square = float(range * range);

    for (_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end(); ++it)
    {
        NPC* const npc = it->second;
        if (npc == NULL)
        {
            continue;
        }
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        if (npc->IsDeleted())
        {
            const Object::Key key = npc->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            continue;
        }
//#endif
        const WzVector& npc_pos = *npc->GetPosPtr();
        const float dist_square = VectorLength2(&(npc_pos - player_pos));
        if ((dist_square <= range_square) && (player->IsFriend(npc) == USER_RELATION_ENEMY))
        {
            if (damage_percent)
            {
                GmDamageNPC(player, npc, damage_percent);
            }
            else
            {
                KillNPC(player, npc, TRUE);
            }                
        }
    }
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

// _NA_0_20100222_UNIT_TRIGGERS_ADD
VOID    GameField::KillOneMonster( NPC* pNPC, Player* pPlayer, BOOL IsExpItemDrop )
{
    if(pPlayer)
    {
        KillNPC(pPlayer, pNPC, IsExpItemDrop);
    }
    else 
    {
        DestroyObject(pNPC); // (WAVERIX) (090121) leave_field + delete_object
        //pNPC is a garbage
    }
}

//爾鼻橈檜 跨蝶攪蒂 餌塭雖啪 и棻.
void GameField::KillMonster(MONSTERCODE monster_code, Player* player, BOOL is_exp_item_drop)
{
    for (_NPC_HASH::iterator it = m_NPCHash.begin(); it != m_NPCHash.end(); ++it)
    {
        NPC* const npc = it->second;
        if (npc == NULL)
        {
            continue;
        }
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        if (npc->IsDeleted())
        {
            const Object::Key key = npc->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            continue;
        }
//#endif
        if (npc->GetBaseInfo()->m_MonsterCode != monster_code)
        {
            continue;
        }
        KillOneMonster(npc, player, is_exp_item_drop);
    }
}

VOID			GameField::AddExpAllMember( DWORD64 Exp )
{
	_PLAYER_HASH::iterator it = m_PlayerHash.begin(), end = m_PlayerHash.end();
	for( ; it != end ; )
	{
		Player* pPlayer = (Player*)it->second;
		++it;
		if( pPlayer )
		{
			pPlayer->AddExp( Exp, 0, 0, TRUE );
		}
	}
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


VOID	GameField::KillAllItem( Player *pPlayer, int iRange )
{
    // (NOTE) important, designed by __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__
    const WzVector& playerPos = *pPlayer->GetPosPtr();
    const float rangeSquare = float(iRange * iRange);
    _ITEM_HASH::iterator it = m_FieldItemHash.begin(), end = m_FieldItemHash.end();
    for( ; it != end; ++it)
    {
        Item* pItem = it->second;
        const WzVector& itemPos = *pItem->GetPosPtr();
        float fDistSquare = VectorLength2(&(playerPos - itemPos));
        if(fDistSquare <= rangeSquare)
            pItem->RequestDelete();
    }
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

BOOL	GameField::KillNPC(Player* pPlayer, NPC* pNPC, BOOL bIsDrop, BOOLEAN bBrd)
{
    // (WAVERIX) (090121) (CODE-REVISION)
    // - 闡だ橾脹 等檜攪曖 檜鼻檣雖 綠薑鼻瞳檣 翕濛擊 爾檜朝 睡碟檜 襄營и棻.
    // 囀萄 營薑溺擊 熱чж賊 п唸腎嘎煎 除欽�� 營薑溺擊 熱чж晦煎 и棻.
	if( !pPlayer || !pNPC )
        return FALSE;
    //
    if(pNPC->IsDeleteReserved())
        return FALSE;
    //
	if(pNPC->IsAlive() == FALSE)
        return FALSE;

	/*
	// NPC曖 譆渠HP 虜躑 等嘐雖蒂 輿溥堅 ц戲釭, 等嘐雖 顫殮檜 WORD檜嘎煎 譆渠HP蒂 顫殮議蝶たй 陽 熱纂陛 澀跤脹棻!
	// 評塭憮 橾欽 ⑷營 HP蒂 1煎 撢たи �醴� 等嘐雖 100擊 輿橫憮 跨蝶攪蒂 避檜朝 寞衝戲煎 夥紱歷棻.
	pNPC->SetHP( 1 );
	DAMAGETYPE wDamage = 100;
	*/
	// 遴艙ぜ曖 蹂羶戲煎 錳楚渠煎 籀葬л(2006.7.21)
    // (WAVERIX) Dummy Code : Compile Issue
    static NPC* npc_s = 0;
    ;           npc_s = pNPC;
    const DWORD maxHP = npc_s->GetMaxHP();
    const DWORD maxSD = npc_s->GetMaxSD();

    // f100908.1L
    DamageArgs damage_args(pPlayer, eATTACK_TYPE_MELEE);
    ;{
        //damage_args.option_.is_critical_;
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        //damage_args.attack_regist_kind_;
        //damage_args.sd_apply_;
        damage_args.damage_ = maxHP;
    };

	// 薑鼻瞳戲煎 唳я纂, 嬴檜蠱碟寡蒂 籀葬и棻賊,
	if( bIsDrop )
	{
		pNPC->SetHP( 1 );
        pNPC->Damaged(&damage_args);
        //pNPC->Damaged( pPlayer, eATTACK_TYPE_MELEE, wDamage, FALSE ); //蹂晦縑憮 頂睡瞳戲煎 OnDead蒂 ��轎и棻.
	}
	// 欽牖�� NPC蒂 避檜溥堅 и棻賊,
	else
	{
		pNPC->SetHP( 0 );
		pNPC->StopMoving();
		DestroyObject( pNPC );
	}

    // (WAVERIX) (NOTE) 檜 衛薄 п渡 螃粽薛お朝 Free腎橫 氈朝 鼻鷓橾 �捕�檜 堪棻. 餌辨衛 輿曖!
    if (bBrd)
    {
        MSGSUB_BATTLE_PLAYER_ATTACK_BRD msg_brd;
        msg_brd.Clear();
        //msg_brd.dwClientSerial = 0;
        pPlayer->GetPos( &msg_brd.wvCurPos );
        msg_brd.dwAttackerKey = pPlayer->GetObjectKey();
        //msg_brd.style_presentation = 0; //_NA003109_STYLE_PRESENTATION_IMPROVEMENT
        msg_brd.byAttackType  = eATTACK_SEQUENCE_FIRST;
        msg_brd.dwTargetKey   = pNPC->GetObjectKey();
        msg_brd.wDamage = damage_args.damage_;
        msg_brd.dwTargetHP = pNPC->GetHP();
        //msg_brd.byEffect = 0; // GM貲滄橫煎 避檜朝 匙檜雖虜 檜 高縑 渠и 撮た擊 п輿橫撿 л!!(20060115)
        pPlayer->SendPacketAroundForSync(&msg_brd);
    };
	return TRUE;
}

BOOL	GameField::GmDamageNPC(Player* pPlayer, NPC* pNPC, const FLOAT fDamagePercent)
{
    // implemented by _NA001385_20090924_DOMINATION_
    if( !pPlayer || !pNPC )
        return FALSE;
    //
    if(pNPC->IsDeleteReserved())
        return FALSE;
    //
    if(pNPC->IsAlive() == FALSE)
        return FALSE;

    if (fDamagePercent)
    {
        const DWORD curHp = pNPC->GetHP();
        const DWORD maxHp = pNPC->GetMaxHP();
        const FLOAT fDamage  = (FLOAT)maxHp * fDamagePercent;
        const DWORD dwDamage = (DWORD)fDamage;

        pNPC->DecreaseHP(dwDamage);
        pNPC->SendHPChange();
    }
    else
    {
        const DWORD maxHP = pNPC->GetMaxHP();
        const DWORD maxSD = pNPC->GetMaxSD();
        // f100908.1L
        DamageArgs damage_args(pPlayer, eATTACK_TYPE_MELEE);
        ;{
            damage_args.damage_ = maxHP;
        };

        pNPC->SetHP(1);
        pNPC->Damaged(&damage_args);
        //pNPC->Damaged(pPlayer, eATTACK_TYPE_MELEE, wDamage, FALSE);		//蹂晦縑憮 頂睡瞳戲煎 OnDead蒂 ��轎и棻.

        ;{
            MSGSUB_BATTLE_PLAYER_ATTACK_BRD msg_brd;
            msg_brd.Clear();
            //msg_brd.dwClientSerial = 0;
            pPlayer->GetPos(&msg_brd.wvCurPos);
            msg_brd.dwAttackerKey = pPlayer->GetObjectKey();
            //msg_brd.style_presentation = 0; //_NA003109_STYLE_PRESENTATION_IMPROVEMENT
            msg_brd.byAttackType = eATTACK_SEQUENCE_FIRST;
            msg_brd.dwTargetKey = pNPC->GetObjectKey();
            msg_brd.wDamage = damage_args.damage_;
            msg_brd.dwTargetHP = pNPC->GetHP();
            //msg_brd.byEffect = 0;
            pPlayer->SendPacketAroundForSync(&msg_brd);
        };
    }

    return TRUE;
}

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


VOID	GameField::SpawnSector( DWORD dwSectorIndex, Object* pObject )
{
	// 嘐葬 葬薇ц戲賊 葬欐
	if( m_bPreRegen )	return;

	// Ы溯檜橫 蛔濰檣雖 羹觼
	if(pObject->IsEqualObjectKind(PLAYER_OBJECT) == 0)
		return;

	m_pUnitRegenManager->SpawnSector( dwSectorIndex );
}


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式


Object*	GameField::FindObject(const eOBJECT_TYPE object_type, const Object::Key object_key) const
{
    Object* obj = NULL;

    switch(object_type)
    {
    case OBJECT_OBJECT:
    case CHARACTER_OBJECT:
    case NONCHARACTER_OBJECT:
    case NPC_OBJECT: 
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            return NULL;
        }
        break;
    case PLAYER_OBJECT:
        {
            _PLAYER_HASH::const_iterator it = m_PlayerHash.find(object_key);
            if (it != m_PlayerHash.end())
            {
                obj = it->second;
            }
        }
        break;
    case MONSTER_OBJECT:
    case SUMMON_OBJECT:
    case MERCHANT_OBJECT:
    case MAPNPC_OBJECT:
    case LUCKYMON_OBJECT:
    case TOTEMNPC_OBJECT:
    case SSQMONSTER_OBJECT:
    case CRYSTALWARP_OBJECT:
    case FRIEND_MONSTER_OBJECT:
    case SYNC_MERCHANT_OBJECT:
        {
            _NPC_HASH::const_iterator it = m_NPCHash.find(object_key);
            if (it != m_NPCHash.end())
            {
                obj = it->second;
            }
        }
        break;
    case ITEM_OBJECT:
    case MONEY_OBJECT:
        {
            _ITEM_HASH::const_iterator it = m_FieldItemHash.find(object_key);
            if (it != m_FieldItemHash.end())
            {
                obj = it->second;
            }      
        }
        break;
    case COLLECTION_OBJECT:	
        {
            _COLLECTION_HASH::const_iterator it = m_FieldCollectHash.find(object_key);
            if (it != m_FieldCollectHash.end())
            {
                obj = it->second;
            }
        }
        break;
    default:
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Invalid object type.|ObjectType = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                object_type
            );
            return NULL;
        }
    }

    if (obj_util::IsValid(obj, __FUNCTION__, __LINE__) == false)
    {
        return NULL;
    }
    return obj;
}

/*
// (CHANGES) (f100504.1L) change the interface specification to support a complex condition
// the replacement facility for SendXXPacketToSector<IsSendPlayer>
bool IsSendPlayer::ChkCondision(Player* pPlayer)
{
	return RC::RC_COLLECT_SUCCESS == m_pMgr->ChkCondision(pPlayer,m_CollectionID);
}
*/
