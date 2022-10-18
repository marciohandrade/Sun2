#include "StdAfx.h"
#include "CollectionManager.h"
#include "CollectionParser.h"
#include "QuestManager_Concrete.h"
#include <AreaConquestCommonHeader.h>
#include "LotteryInfoParser.h"
#include "ItemManager.h"
#include <PublicMath.h>
#include "ServerOptionParserEx.h"
#include "MovementChecker.h"
#include "CollectedList.h"
#include <PacketStruct_DG.h>
#include "CollectionStateInfo.h"
#include "Collection.h"
#include "ObjectFactory.h"
#include "GameDominationField.h"
#include "GameGuildManager.h"
#include "GameGuild.h"
#include <SkillAttrCalculator.h>
#include "StatusManager.h"
#include "GameField.h"
#include "TriggerManager.h"
#include "TriggerManagerEx.h"
#include "GameMissionRoom.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <GameInstanceDungeon.h>
#include <World/ChaosZoneSystem/ChaosZone.h>
#include <world/ChaosZoneSystem/BattleGround.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <World/ChaosZoneSystem/SonnenSchein.h>
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <World/ChaosZoneSystem/OneDaysGuildMission.h>
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

//==================================================================================================
// CHANGES: f110527.3L, memory usage optimization
struct CollectionManager::CollectionStateInfoManager
{
    static CollectionStateInfo* Create(Collection* collection,
        CHARGUID collector, WzVector& collector_pos);
    static void Release(CollectionStateInfo* state_info);
};

CollectionStateInfo* CollectionManager::CollectionStateInfoManager::Create(
    Collection* collection, CHARGUID collector, WzVector& collector_pos)
{
    CollectionStateInfo* state_info = TAllocNew(CollectionStateInfo);
    if (state_info->Init(collection, collector, collector_pos) == false)
    {
        Release(state_info);
        return NULL;
    };

    collection->SetStateInfo(state_info);
    return state_info;
}

void CollectionManager::CollectionStateInfoManager::Release(CollectionStateInfo* state_info)
{
    Collection* collection = state_info->GetCollection();
    if (collection) {
        collection->SetStateInfo(NULL);
    };
    state_info->Release();
    TAllocDelete(CollectionStateInfo, state_info);
}

//==================================================================================================
//

CollectionManager::CollectionManager(GameField* pField)
	:m_pField(pField) 
{
	m_Hash.Initialize(MAX_FIELD_COLLECTION_NUM); 
}


void CollectionManager::Release()
{
	ACTIVE_COLLECTION_HASH_IT it = m_Hash.begin(), end = m_Hash.end();
	while( it != end )
	{
		CollectionStateInfoManager::Release(*it);
		++it;
	}
	m_Hash.clear();
}

//{__NA001283_20090221_COLLECTION_SYNC
// ����

// Ŭ���� ä���� ����Ani -> ä��Ani -> �Ϸ�Ani �� ������.
// ������ Ŭ��� ���۽ð��� ����ȭ �ϳ� �Ϸ� �ð��� ����ȭ �����ʴ´�.
// Ÿ�ֿ̹� ���� ������ ä�� �Ϸ� �ϰ� Ŭ��� ä�� ��� �ϴ� ������ �߻��Ѵ�.
// ��Ȯ�� Ÿ�̹��� ���� Ŭ�� �Ϸ�Ani�� ���������� ����ȭ ��Ŷ�� ������,
// ������ �� Ÿ�ֿ̹� �����, �Ϸ� ó���� �Ѵ�.

// ������ �Ϸ� ��Ȳ�� �÷��̾ ����, ����ȭ ��Ŷ�� ���� �����Ѵ�.

// pCollectionInfo <- �����صξ���� ����

// Ŭ�󿡼��� �����̵� ��ҵ� ���� �ϳ��� ������ �������� �����ؾ� �Ѵ�.
// ������ �̰�쿡�� ����ȭ�� ������ �߻��Ѵ�. 
// ������ ������ ����Ǿ Ÿ�̹��� �ʴ´ٸ�, ���� ä������ ������ Ŭ�� �ٰ��ΰ��� ����
// �� ������ ��ģ��.
// ä������ �⺻ Ȱ�� ������ Ÿ������ ���� �ذ��� �ǳ�, 
// ���̳� �ǵ����� ��� ���� �������̹Ƿ� ����ǥ�� �ٽ� ���ܳ���.

RC::eCOLLECT_RESULT CollectionManager::Success(Player* player)
{
    if (player == NULL)
    {
        return RC::RC_COLLECT_FAIL;
    }

    CollectedList*  collect_list = player->GetCollectedList();
    DWORD object_key_collected = 0;
    CollectionInfo* collect_info = collect_list->GetSuccessCollectionInfo(object_key_collected);
    if (collect_info == NULL)
    {
        return RC::RC_COLLECT_NOTEXIST;
    }

    WzID collect_id = collect_info->m_CollectionID;

    //gamelulu_dev_note : ���� ������Ʈ�� ��쿡�� �Ʒ��� ���� ���� ���θ�
    //���� �˻��Ѵ�. ���� ������ ���� ������ ������ ���з� ������ ������� ����.
    const PairIsStampReturnValue is_stamp = _IsStamp(collect_id);
    if (is_stamp.first != false)
    {
        GameZone* pGameZone = player->GetGameZonePtr();
        if (eZONETYPE_DOMINATION_FIELD == pGameZone->GetZoneType())
        {
            GameDominationField* pZone = (GameDominationField*)pGameZone;
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL 
            if (pZone->IsSuccessStamp() == false) {
                return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
            }
            pZone->SuccessStamp(player);
#else
            //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
            if (pZone->IsSuccessStamp() == false)
            {
                return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
            }
            pZone->SuccessStamp();
#endif // _NA_006826_20130722_DOMINATION_RENEWAL 
            GAMELOG->LogDomination(ACT_DOMINATION_STAMP_COMPLETE, player, NULL);
        }
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (_IsSealingStrongPoint(collect_id))
    {
        // ���� �÷��̾ ��Ʋ�׶������� ��ġ�ϴ��� üũ
        if (player->IsThereBattleGroundZone() == false)
        {
            SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
                __FUNCTION__, __LINE__);
            return RC::RC_COLLECT_FAIL;
        }

        BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
        if (battle_ground) {
            battle_ground->SuccessSealing(player, collect_id);
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if (_IsSonnenScheinStamp(collect_id) == TRUE)
    {
        //SonnenSchein������ ���� ���� ó��
        if (player->GetChaosZoneType() == eCHAOSZONE_TYPE_SONNENSCHEIN)
        {
            ChaosZone* zone = player->GetCurrentChaosZone();
            if (zone == NULL) {
                return RC::RC_COLLECT_FAIL;
            }
            SonnenSchein* sonnenschein = static_cast<SonnenSchein*>(zone);
            sonnenschein->SuccessStamp(player);
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	else if (_IsOneDaysGuildmissionStamp(collect_id) == TRUE)
	{
		// ���ϱ��̼� ���� ���� ó��
		if (player->GetChaosZoneType() == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
		{
			ChaosZone* zone = player->GetCurrentChaosZone();
			if (zone == NULL) {
				return RC::RC_COLLECT_FAIL;
			}
			OneDaysGuildMission* onedaysguildmission = static_cast<OneDaysGuildMission*>(zone);
			onedaysguildmission->SuccessStamp(player);
		}
	}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

    // ä���� �����ص� ������ ���� ���� �ʴ� ä������ �ִ�.(����)
    if (collect_info->m_ItemCode != 0)   
    {
        MSG_CG_COLLECT_OBTAIN_CMD obtain_msg;

#ifdef _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
        obtain_msg.quest_code_ = collect_info->m_Condition[CollectionInfo::QUEST];
#endif

        RC::eITEM_RESULT result_code = _RewardItem(player, collect_info, obtain_msg.m_tagInventory);
        if( result_code != RC::RC_ITEM_SUCCESS )
        {
            if( result_code == RC::RC_ITEM_HAVENOTSPACE )
            {
                return RC::RC_COLLECT_FAIL_HAVENOTSPACE;
            }
            else
            {
                return RC::RC_COLLECT_FAIL_REWORD_ITEM;
            }            
        }
        player->SendPacket( &obtain_msg, obtain_msg.GetSize() );
    }

    //ACó��
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(collect_info->m_ItemCode);

    if (item_info)
    {
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        player->GetACManager()->IncConditionValue((int)item_info->m_wACCode,
                                                   item_info->m_byACReferenceID,
                                                   collect_info->m_ItemCode);
#else
        player->GetACManager()->IncConditionValue((int)item_info->m_wACCode,
                                                   item_info->m_byACReferenceID);
#endif
    }

    //ac������ ��� ����ȭ�� ���� DBP�� �ٷ� �����Ѵ�.
    player->SerializeInfoToDBProxy();  
#ifdef _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    //------------------------------------------------------------------------------
    // ä�� Ʈ���� üũ
    // ���� �̼Ƕ� ä�� �����Ͱ� ������ �ϱ⶧����
    // �̼� Ʈ���ſ� ä���ϰ�쿡�� DB �� ������� �ʴ´�.
    DWORD condition_trigger = collect_info->m_Condition[CollectionInfo::TRIGGER];
    if (condition_trigger != 0) // �̼ǳ� ä����
    {
        GameField* const field = player->GetField();
        if (field != NULL)
        {
            TRIGGER_COMPLETE_COLLECTION msg;
            msg.object_key = object_key_collected;
            field->GetTriggerManager()->OnMsg(&msg);
        }
        
        // �̼� ���ο����� �Ѹ��� ä���� �����ϸ� ������ �ο��鵵 ä���� �����Ѱɷ� ����..
        // ä���� ����Ʈ�� ä���� Ű���� �־��ش�.
        const GameField::_PLAYER_HASH& players = field->GetRefPlayerList();
        FOREACH_CONTAINER(
            const GameField::_PLAYER_HASH::value_type& player_pair, players, GameField::_PLAYER_HASH)
        {
            Player* const field_player = player_pair.second;
            if (obj_util::IsValid(field_player, __FUNCTION__, __LINE__) == false)
            {
                continue;
            }
            CollectedList* const field_player_collect_list = field_player->GetCollectedList();
            field_player_collect_list->Add(collect_id);
        }
    }
    else // �ʵ� ä����
    {
        collect_list->Add(collect_id);
        MSG_DG_COLLECT_ADD_SYN msg;
        msg.m_CollectionID = collect_id;
        player->SendToGameDBPServer( &msg, sizeof(msg) );
    }
#else
    collect_list->Add(collect_id);
    MSG_DG_COLLECT_ADD_SYN msg;
    msg.m_CollectionID = collect_id;
    player->SendToGameDBPServer( &msg, sizeof(msg) );
#endif //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER

    return RC::RC_COLLECT_SUCCESS;
}
//}__NA001283_20090221_COLLECTION_SYNC

RC::eCOLLECT_RESULT CollectionManager::GmCancel( WzID CollectionID )
{
	ACTIVE_COLLECTION_HASH_IT it = m_Hash.begin(), end = m_Hash.end();
	while( it != end )
	{	
		CollectionStateInfo* pState = *it;
		if( CollectionID != pState->GetCollectionID() )
        {
            ++it;
            continue;
        }

        Player* const pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pState->GetCollector() );
		if( pPlayer )
		{
			Cancel(pPlayer);
		}
		else
		{
			Collection* pCollection = pState->GetCollection();
			if( pCollection )
			{
				m_pField->LeaveField( pCollection );
				ObjectFactory::Instance()->FreeObject( pCollection );
			}

			_Respawn(pState);
		}
		return RC::RC_COLLECT_SUCCESS;
	}

	return RC::RC_COLLECT_FAIL;
}

VOID CollectionManager::_StartStamp( Player* player, WzID collection_id )
{
    const CollectionInfo* conllection_info = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
    if (conllection_info != NULL)
    {
        StatusManager* status_manager = player->GetStatusManager();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        status_manager->AllocStampStatus(conllection_info->m_dwCollectingTime);
#else
        //���̾� �ٵ�, ��ö ���´� ���ν� �������.
        status_manager->Remove(eCHAR_STATE_STEEL);
        status_manager->Remove(eCHAR_STATE_IMMUNITY_DAMAGE);
        status_manager->AllocStatus(eCHAR_STATE_STAMP, conllection_info->m_dwCollectingTime);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    }

    MSG_CG_DOMINATION_BEGIN_STAMP_BRD brd_msg;
    brd_msg.m_nStampID = collection_id;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(player->GetGuildGuid());
    _tcsncpy(brd_msg.m_stamp_guild_name, pRequestGuild->GetGuildName(), MAX_GUILDNAME_LENGTH);
#else
    _SendStampPacket(brd_msg, player, collection_id);
#endif

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //���� ���۽ð��� �����Ѵ�.
    GameZone* game_zone = player->GetGameZonePtr();
    if (game_zone->GetZoneType() == eZONETYPE_DOMINATION_FIELD )
    {
        GameDominationField* zone = static_cast<GameDominationField*>(game_zone);
        zone->SetStampStartTime();

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        zone->SendPacketToAll(&brd_msg, sizeof(brd_msg));
#endif
    }

    GAMELOG->LogDomination(ACT_DOMINATION_STAMP_START, player, NULL);
}

VOID CollectionManager::_CancelStamp(Player* player, WzID collection_id)
{
    MSG_CG_DOMINATION_CANCLE_STAMP_BRD brd_msg;
    brd_msg.m_nStampID = collection_id;

    if (player == NULL)
    {
        m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
    }
    else
    {
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->Remove(eCHAR_STATE_STAMP);

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
#else
        _SendStampPacket(brd_msg, player, collection_id);
#endif
    }

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //���� ���۽ð��� �ʱ�ȭ �Ѵ�.
#ifdef _NA_008256_20150428_MODIFY_CANCLE_STAMP_LOGIC
    GameZone* game_zone = m_pField->GetGameZone();
#else
    GameZone* game_zone = player->GetGameZonePtr();
#endif // _NA_008256_20150428_MODIFY_CANCLE_STAMP_LOGIC
    if (game_zone->GetZoneType() == eZONETYPE_DOMINATION_FIELD )
    {
        GameDominationField* zone = static_cast<GameDominationField*>(game_zone);
        zone->InitStampCheckTime();
    }
}

template<typename PacketType> 
VOID CollectionManager::_SendStampPacket(PacketType& BrdMsg, Player* pPlayer, WzID CollectionID)
{
    GameGuild* pRequestGuild = g_GameGuildManager.FindGuild(pPlayer->GetGuildGuid());
    GameField* pField = pPlayer->GetField();

    if (NULL != pField &&
        NULL != pRequestGuild)
    {
        const FIELDCODE fieldCode = pField->GetFieldCode();
        pRequestGuild->SendPacketFieldMember(BrdMsg, fieldCode);
    }
}

// 
RC::eCOLLECT_RESULT CollectionManager::Cancel( Player* pPlayer )
{
	ASSERT(pPlayer);

	CollectedList* pCollected =	pPlayer->GetCollectedList();
	DWORD ObjKeyCollection = pCollected->GetObjKeyCollecting();	
	if( 0 == ObjKeyCollection )
		return RC::RC_COLLECT_CHAR_NOTCOLLECTING;


	CollectionStateInfo* pStateInfo = _FindStateInfo(ObjKeyCollection);
	if( !pStateInfo )
		return RC::RC_COLLECT_NOTCOLLECTING;

	if( false == pStateInfo->CanCancel(pPlayer->GetCharGuid()) )
		return RC::RC_COLLECT_FAIL_CANCEL;


    const WzID collectionID = pStateInfo->GetCollectionID();
    ;{
        MSGSUB_COLLECT_CANCEL_BRD msg_brd;
        msg_brd.object_key = ObjKeyCollection;
        msg_brd.collection_id = collectionID;
        msg_brd.collector = pPlayer->GetObjectKey();
        pPlayer->SendPacketAroundForSync(&msg_brd);
    };
    //���� ���
    const PairIsStampReturnValue is_stamp = _IsStamp(collectionID);
    if (is_stamp.first == true)
    {
        _CancelStamp(pPlayer, collectionID);
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (_IsSealingStrongPoint(collectionID))
    {
        _CancelSealing(pPlayer, collectionID);
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if (_IsSonnenScheinStamp(collectionID) == TRUE) {
        _CancelSonnenScheinStamp(pPlayer, collectionID);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	else if (_IsOneDaysGuildmissionStamp(collectionID) == TRUE) {
		_CancelOneDaysGuildmissionStamp(pPlayer, collectionID);
	}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

    pStateInfo->Cancel();
    pCollected->EndCollecting();

	return RC::RC_COLLECT_SUCCESS;
}

// ä���ڰ� ���� ä������ ä�� ���.
RC::eCOLLECT_RESULT CollectionManager::Cancel( CollectionStateInfo* pStateInfo )
{
	ASSERT(pStateInfo);

    const WzID collectionID = pStateInfo->GetCollectionID();
    ;{
        MSGSUB_COLLECT_CANCEL_BRD msg_brd;
        msg_brd.object_key = pStateInfo->GetObjectKey();
        msg_brd.collection_id = collectionID;
        msg_brd.collector = 0;

        const Collection* pCollection = pStateInfo->GetCollection();
        const SECTOR_IDX  sector_idx  = pCollection->GetSectorIndex();
        m_pField->SendPacketAroundForSync(sector_idx, &msg_brd);
    };
    //���� ���
    const PairIsStampReturnValue is_stamp = _IsStamp(collectionID);
    if (is_stamp.first == true)
    {
        _CancelStamp(NULL, collectionID);
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (_IsSealingStrongPoint(collectionID))
    {
        _CancelSealing(NULL, collectionID);
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if (_IsSonnenScheinStamp(collectionID) == TRUE) {
        _CancelSonnenScheinStamp(NULL, collectionID);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	else if (_IsOneDaysGuildmissionStamp(collectionID) == TRUE) {
		_CancelOneDaysGuildmissionStamp(NULL, collectionID);
	}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    pStateInfo->Cancel();

	return RC::RC_COLLECT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

RC::eCOLLECT_RESULT CollectionManager::Collect(DWORD ObjKeyCollection, Player* pPlayer, WzVector& wvClientCurPos)
{
	CollectedList* pCollectedList = pPlayer->GetCollectedList();

	if( false != pCollectedList->IsCollecting() )
	{
		//Ŭ�󿡼� ����Ŭ���� �ι� ���޾� ���ü� ������, 
		//�� ��� NAK�� Ŭ�� �������� Ŭ�������� ä���� ��ҵȴ�.
		//�׷����� ���������� ä����Ҹ� BRD�Ѵ�.

		RC::eCOLLECT_RESULT rc= Cancel(pPlayer);

		SUNLOG( eDEV_LOG, "[CollectionManager::Collect] ä���� ä���õ� �ɸ���[%s] ä����[%d] cancel[%s]",
			pPlayer->GetCharName(), ObjKeyCollection, RC::RC_COLLECT_SUCCESS != rc?"fail":"success" );

		return RC::RC_COLLECT_ALREADY_COLLECTING;
	}

	Collection* pCollection = (Collection*)m_pField->FindObject( COLLECTION_OBJECT, ObjKeyCollection );
	if( NULL == pCollection )
		return RC::RC_COLLECT_NOTEXIST;
	
	if( false == IsCollectable(ObjKeyCollection) )
		return RC::RC_COLLECT_ALREADY_COLLECTED;

	//.//

	RC::eCOLLECT_RESULT rc = RC::RC_COLLECT_FAIL;
	MAPOBJECT_INFO* pObjInfo = pCollection->GetMapObjectInfo();

	rc = _ChkDistance(pPlayer,wvClientCurPos,pObjInfo->wvPos);
	if( RC::RC_COLLECT_SUCCESS != rc )
		return rc;

	rc = ChkCondision( pPlayer, pObjInfo->CollectionID );
	if( RC::RC_COLLECT_SUCCESS != rc )
		return rc;

    if (pPlayer->IsDead())
    {
        const PairIsStampReturnValue is_stamp = _IsStamp(pObjInfo->CollectionID);

        if (false != is_stamp.first)
            return RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else if (_IsSealingStrongPoint(pObjInfo->CollectionID))
        {
            return RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR;
        }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
        else if (_IsSonnenScheinStamp(pObjInfo->CollectionID) == TRUE) {
            return RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR;
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
		else if (_IsOneDaysGuildmissionStamp(pObjInfo->CollectionID) == TRUE) {
			return RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR;
		}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
        else
            return RC::RC_COLLECT_FAIL_REWORD_DEAD_CHAR;
    }

    //.//

	if( false == _Collect(pCollection,pPlayer) )
		return RC::RC_COLLECT_FAIL;

	// ���� �˻������ ���� ä�� ���� ä������ �־�д�.

	pCollectedList->BeginCollecting(ObjKeyCollection);

	//.//
    // ���� ��Ŷ���� Ŭ�� ä������� ���� (�ڽŰ� �ڽ� �ֺ�)
    ;{
        MSGSUB_COLLECT_ACTION_BRD msg_brd;
        msg_brd.object_key = ObjKeyCollection;
        msg_brd.collection_id = pObjInfo->CollectionID;
        msg_brd.collector = pPlayer->GetObjectKey();
        pPlayer->SendPacketAroundForSync(&msg_brd);
    };
    const PairIsStampReturnValue is_stamp = _IsStamp(pObjInfo->CollectionID);
    if (is_stamp.first == true)
    {
        // ���� ������ �����ʵ� ��������� ������. 
        _StartStamp(pPlayer, pObjInfo->CollectionID);
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (_IsSealingStrongPoint(pObjInfo->CollectionID))
    {
        _StartSealing(pPlayer, pObjInfo->CollectionID);
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if (_IsSonnenScheinStamp(pObjInfo->CollectionID) == TRUE) {
        _StartSonnenScheinStamp(pPlayer, pObjInfo->CollectionID);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	else if (_IsOneDaysGuildmissionStamp(pObjInfo->CollectionID) == TRUE) {
		_StartOneDaysGuildmissionStamp(pPlayer, pObjInfo->CollectionID);
	}
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

    //ä���� �����Ǵ� ����
    pPlayer->Player_TransOff();      

#ifdef _DEBUG
	char ch[5] = {0,};
	SUNLOG( eDEV_LOG, "[SEND MSG_CG_COLLECT_ACTION_BRD] SENDER_ [%s][%d] _ "
		"ObjKeyCollection[%d] CollectionID[%s] Collector[%d]",
		pPlayer->GetCharName(),pPlayer->GetCharGuid(), 
		ObjKeyCollection, WzIDToString(ch,pObjInfo->CollectionID), pPlayer->GetObjectKey() );
#endif

	return RC::RC_COLLECT_SUCCESS;
}

bool CollectionManager::_Collect( Collection* pCollection, Player* pPlayer )
{
	CollectionStateInfo* pStateInfo = _FindStateInfo(pCollection->GetObjectKey());
	if( NULL != pStateInfo || 
		NULL != pCollection->GetStateInfo() ) 
		return false;

	WzVector vServerPos;
	pPlayer->GetPos( &vServerPos );

	pStateInfo = CollectionStateInfoManager::Create(pCollection, pPlayer->GetCharGuid(), vServerPos );
	if( NULL == pStateInfo )
		return false;

	if( false == pStateInfo->StartCollect() )
	{
		CollectionStateInfoManager::Release(pStateInfo);
		return false;
	}

	m_Hash.Add(pStateInfo, pCollection->GetObjectKey());

	return true;
}

RC::eCOLLECT_RESULT CollectionManager::_ChkDistance( Player* pPlayer, WzVector& wvClientCurPos, WzVector& wvCollectionPos )
{
	WzVector vResultCurPos;	
    if( FALSE == pPlayer->CorrectPosition(wvClientCurPos, vResultCurPos) )
        return RC::RC_COLLECT_POSITION_INVALID;

    FLOAT fCurPosDiff = Math_Vector2DLength( vResultCurPos, wvCollectionPos );

	if( MAX_COLLECT_DISTANCE_OBJ < fCurPosDiff )
		return RC::RC_COLLECT_DISTANCE_INVALID;

	return RC::RC_COLLECT_SUCCESS;
}

CollectionManager::PairIsStampReturnValue CollectionManager::_IsStamp(WzID CollectionID)
{
    const CollectionInfo* pCollectInfo = 
        CollectionParser::Instance()->FindCollectionInfo(CollectionID); 

    if( NULL != pCollectInfo )
    {
        const DWORD dwStamp = pCollectInfo->m_Condition[CollectionInfo::STAMP];
        const bool is_stamp = CollectionInfo::OBJ_STAMP == dwStamp || 
                              CollectionInfo::OBJ_STAMP_USE_GUILD_MASTER == dwStamp;

        return std::make_pair(is_stamp, dwStamp);
    }

    return std::make_pair(false,0);
}

RC::eCOLLECT_RESULT CollectionManager::ChkCondision( Player* pPlayer, WzID CollectionID )
{
	const CollectionInfo* pCollectInfo = CollectionParser::Instance()->FindCollectionInfo(CollectionID); 
	if( NULL == pCollectInfo )
	{
		return RC::RC_COLLECT_NOTEXIST;
	}

	// 0���� ���� ���� ��.
	const QCODE qCode   = (QCODE)pCollectInfo->m_Condition[CollectionInfo::QUEST];
    const int  iACCode = (int)pCollectInfo->m_Condition[CollectionInfo::AC];
    
    const DWORD dwObjID = pCollectInfo->m_Condition[CollectionInfo::MAPOBJID];
	const DWORD dwCount = pCollectInfo->m_Condition[CollectionInfo::COUNT];
	
	if( 0 != qCode )
	{
		Quest* pQuest = pPlayer->GetQuestManager()->FindQuest(qCode);
		if( NULL == pQuest || QS_NORMAL != pQuest->GetQState() )
        {
			return RC::RC_COLLECT_FAIL_QCONDISION;
        }

        //------------------------------------------------------------------------------
        //
        ItemManager* item_manager = pPlayer->GetItemManager();
        QuestInfo* quest_info = pQuest->GetQuestInfo();
        if (quest_info == NULL) 
        { 
            SUNLOG(eCRITICAL_LOG, _T("[%s:%d] cannot find quest info : %d"), 
                __FUNCTION__, __LINE__, qCode);
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        } 
 
        QUEST_CONDITION_ITEM_INFO* quest_condition_item_info = \
            static_cast<QUEST_CONDITION_ITEM_INFO*>(
            quest_info->FindCompleteCondition(eQUEST_CONDITION_ITEM));
        if (quest_condition_item_info == NULL) 
        { 
            SUNLOG(eCRITICAL_LOG, _T("[%s:%d] cannot find quest complete condition item info : %d"), 
                __FUNCTION__, __LINE__, qCode);
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        } 
 
        for(int i = 0; i < quest_condition_item_info->GetItemArrayCount(); ++i)
        { 
            if(quest_condition_item_info->GetItemCode(i) == pCollectInfo->m_ItemCode)
            { 
                // ����� ������ �������� �ִ��� üũ
                int item_count = quest_condition_item_info->GetItemNum(i);
                int exist_count = item_manager->ExistItem(SI_INVENTORY, pCollectInfo->m_ItemCode, item_count);
                if (exist_count >= item_count)
                { 
                    return RC::RC_COLLECT_FAIL_COUNTCONDISION; 
                } 
            }
        } 
        //
        //------------------------------------------------------------------------------
	}

    if( 0 != iACCode )
	{
		AchievementManager* pACMgr = pPlayer->GetACManager();
		if( NULL == pACMgr || FALSE == pACMgr->CanCollectTreasureBox(iACCode) )
			return RC::RC_COLLECT_FAIL_ACCONDISION;	 
	}

    const PairIsStampReturnValue is_stamp = _IsStamp(CollectionID);

    if (false != is_stamp.first)
    {
        GUILDGUID guildGuid = pPlayer->GetGuildGuid();

        if (INVALID_GUILDGUID == guildGuid ||
            ePRS_AT_DOMINATION_FIELD != pPlayer->GetGameZoneState()) 
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;

        GameDominationField* pZone = (GameDominationField*)pPlayer->GetGameZonePtr();

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        if (guildGuid == pZone->GetDefenseGuild())
        {
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
        }

        // ���������� �ð��� ����Ǳ����� ���������� ���� ���� ���� ���..
        if (pZone->GetOffenseGuild() == 0 && pZone->GetEventState() == DOMINATION_EVENT_END)
        {
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
        }

        const GUILDGUID defense_guild_guild = pZone->GetDefenseGuild();
        GameGuild* const defense_game_guild = g_GameGuildManager.FindGuild(defense_guild_guild);
        if (defense_game_guild)
        {
            GuildRelationSystem::GuildRelationEntry* defense_guild_entry = \
                defense_game_guild->GetRelationEntry();
            if (defense_guild_entry)
            {
                eGUILD_RELATION relation = \
                    defense_guild_entry->GetRelationType(guildGuid);
                if (relation == eGUILD_RELATION_ALLIENCE)
                {
                    return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
                }
            }
        }
#else
        if (guildGuid != pZone->GetOffenseGuild())
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

        // ��� �����͸� ���� ������ ���ι� �߰�. [5/13/2010 lst1024]

        // OBJ_STAMP : ���� ���� ���� ����
        // OBJ_STAMP_USE_GUILD_MASTER : ���� ��� �����͸� ����

        if (is_stamp.second == CollectionInfo::OBJ_STAMP_USE_GUILD_MASTER &&        
            pPlayer->GetCharInfo()->m_eGuildDuty != eGUILD_DUTY_MASTER)
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (_IsSealingStrongPoint(CollectionID))
    {
        // �÷��̾ ���� ��Ʋ�׶������� �ִ��� üũ
        if (pPlayer->IsThereBattleGroundZone() == false)
        {
            SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
                __FUNCTION__, __LINE__);
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        }

        const BattleGround* const battle_ground = \
            static_cast<BattleGround*>(pPlayer->GetCurrentChaosZone());
        if (battle_ground == NULL || battle_ground->CanCheckSealing(pPlayer, CollectionID) == false) {
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
    else if(_IsSonnenScheinStamp(CollectionID) == TRUE)
    {
        GUILDGUID guildGuid = pPlayer->GetGuildGuid();
        if (INVALID_GUILDGUID == guildGuid) {
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
        }

        if (pPlayer->GetChaosZoneType() != eCHAOSZONE_TYPE_SONNENSCHEIN)
        {
            SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in SonnenSchein"), 
                __FUNCTION__, __LINE__);
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        }

        SonnenSchein* sonnenschein = static_cast<SonnenSchein*>(pPlayer->GetCurrentChaosZone());
        if (sonnenschein == NULL) {
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
        }

        if (guildGuid != sonnenschein->GetRoomGuildGuid()) {
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
        }

        //�ҳٻ��� �������� ����üũ
        // Start���¿��� ���ΰ����Ѱ� ������ ���̽�
        // None���� ���ΰ����� GM���� ������ �ҳٻ����� ���� ��
        if (sonnenschein->GetEventState() != SONNENSCHEIN_EVENT_STATE_START &&
            sonnenschein->GetEventState() != SONNENSCHEIN_EVENT_STATE_NONE) {
            return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	else if(_IsOneDaysGuildmissionStamp(CollectionID) == TRUE)
	{
		GUILDGUID guildGuid = pPlayer->GetGuildGuid();
		if (INVALID_GUILDGUID == guildGuid) {
			return RC::RC_COLLECT_FAIL_STAMP_CONDISION;
		}

		if (pPlayer->GetChaosZoneType() != eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
		{
			SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in OneDaysGuildMission"), 
				__FUNCTION__, __LINE__);
			return RC::RC_COLLECT_FAIL_COUNTCONDISION;
		}

		OneDaysGuildMission* onedaysguildmission = static_cast<OneDaysGuildMission*>(pPlayer->GetCurrentChaosZone());
		if (onedaysguildmission == NULL) {
			return RC::RC_COLLECT_FAIL_COUNTCONDISION;
		}
	}
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    else if( 0 != dwCount )
    {
        CollectedList* pCollectedList = pPlayer->GetCollectedList();
        if( NULL == pCollectedList || pCollectedList->GetCount(CollectionID) >= dwCount )
            return RC::RC_COLLECT_FAIL_COUNTCONDISION;
    }
    
    if (0 != dwObjID)
    {
        GameField*  pGameField = pPlayer->GetField();
        if (NULL == pGameField)
            return RC::RC_COLLECT_FAIL_OBJECTID_CONDISION;

        const MapNpc* pMapNpc = pGameField->FindMapNpc(Conv_MapObjectKey(dwObjID));

        // ��ȹ���� �Ǽ��� ������Ʈ�� ID�� �߸� ���Ե� ���. 
        if (NULL == pMapNpc)
            return RC::RC_COLLECT_FAIL_OBJECTID_CONDISION;

        if (pMapNpc->IsAlive())
            return RC::RC_COLLECT_FAIL_OBJECTID_CONDISION;
    }

	return RC::RC_COLLECT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

// ����ũ��Ʈ���� ������ ��õ������,
// ��ȹ�ڰ� ���� ��ũ��Ʈ�� ������ ä����ũ��Ʈ�� �Ϻ� ������ ������ ����.
RC::eITEM_RESULT CollectionManager::_RewardItem( Player* pPlayer, CollectionInfo* pInfo, INVENTORY_TOTAL_INFO& OUT TotalInfo )
{
	ItemManager* pMgr = pPlayer->GetItemManager();

	MiniSlotInfo slotinfo;
	slotinfo.Index  = pInfo->m_ItemCode;
	slotinfo.Random = pInfo->m_byScriptType;
	slotinfo.ItemTypeIndex = pInfo->m_IGType;

    SCItemSlot *pSlot = pMgr->GetItemFunctionLottery().MakeItemByLotteryInfo(
        slotinfo, slotinfo.Random, NULL);
	if( NULL == pSlot || 
		0 == pSlot->GetNum() )
	{
		SUNLOG( eCRITICAL_LOG, "[CollectionManager::_RewardItem] ���� ������ �߸��Ǿ���. [pSlot %s NULL], ItemNum[%d]",
			pSlot?"!=":"==", pSlot?pSlot->GetNum():-1 );

		return RC::RC_ITEM_NOINFO;
	}


	RC::eITEM_RESULT rc = RC::RC_ITEM_FAILED;

	if( FALSE==pSlot->IsOverlap() )
		rc = pMgr->Obtain( *pSlot, pSlot->GetNum(), &TotalInfo);
	else
        rc = pMgr->Obtain( pSlot->GetItemCode(), pSlot->GetNum(), &TotalInfo);

	if( RC::RC_ITEM_SUCCESS != rc )
	{
		SUNLOG( eCRITICAL_LOG, "[CollectionManager::_RewardItem][%s]���� ��������� ���� �����ߴ�. err[%d] ItemCode[%d] Num[%d] Overlap[%s]",
			pPlayer->GetCharName(), rc, pSlot->GetItemCode(), pSlot->GetNum(), TRUE==pSlot->IsOverlap()?"true":"false" );	
	}

	return rc;
}

//

void CollectionManager::_Collected( CollectionStateInfo* pStateInfo )
{
	ASSERT(pStateInfo);

	Player* const pPlayer = PlayerManager::Instance()->FindPlayerByCharKey( pStateInfo->GetCollector() );

	S_TRY
	{
		if( NULL == pPlayer )	
            S_THROW(RC::RC_COLLECT_FAIL_REWORD_EXIT_CHAR);

        //_NA001385_20090924_DOMINATION_STAMP 
		if( pPlayer->IsDead() )	
        {
            const WzID collection_id = pStateInfo->GetCollectionID();
            const PairIsStampReturnValue is_stamp = _IsStamp(collection_id);

            if (false != is_stamp.first)
            {
                S_THROW(RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR);
            }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            else if (_IsSealingStrongPoint(collection_id))
            {
                S_THROW(RC::RC_COLLECT_FAIL_STAMP_DEAD_CHAR);
            }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            else
            {
                S_THROW(RC::RC_COLLECT_FAIL_REWORD_DEAD_CHAR);
            }
        }

		CollectedList* pCollectedList = pPlayer->GetCollectedList();
		if( pStateInfo->GetObjectKey() != pCollectedList->GetObjKeyCollecting() )	
			S_THROW(RC::RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY);

		WzVector vServerPos;
		pPlayer->GetPos( &vServerPos );

		FLOAT fCurPosDiff = Math_Vector2DLength( vServerPos, pStateInfo->GetCollectorPos() );
		if( MAX_COLLECT_DISTANCE_CHAR < fCurPosDiff )
		{
			SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Collected] [%s]�� ä������ ��ȿ �Ÿ��� �����. �Ÿ���[%d]",
				pPlayer->GetCharName(), fCurPosDiff );	

			S_THROW(RC::RC_COLLECT_FAIL_REWORD_POS);
		}

		const MAPOBJECT_INFO* pMapObjectInfo = pStateInfo->GetMapObjectInfo();
		fCurPosDiff = Math_Vector2DLength( vServerPos, pMapObjectInfo->wvPos );
		if( MAX_COLLECT_DISTANCE_OBJ < fCurPosDiff )
			S_THROW(RC::RC_COLLECT_FAIL_REWORD_DISTANCE);
	}
	S_CATCH(DWORD, err)
	{
		// ä���� Ŭ���̾�Ʈ�� ����������� �߻��Ұ��
		// �ʵ忡 �ٸ� �÷��̾ ���ٸ� update�� �̷�� ���� ������, _Collected()��
		// ��� ���� �ʴ´�.
		// �� ��� �ٸ� �÷��̾��� ���Խ� RC_COLLECT_FAIL_REWORD_EXIT_CHAR��
		// ���� �÷��̾� �����Խ� RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY�� ������ �޴´�.
		// �̰�� ä���ڿ� ä������ ���� ���谡 ���� �����̴�.
		// �׷��Ƿ� ä���� �������� ������ �ڽ�(ä����)�� ��Ҹ� �˸���.

		RC::eCOLLECT_RESULT rc = RC::RC_COLLECT_SUCCESS;

		switch(err)
		{
		case RC::RC_COLLECT_FAIL_REWORD_EXIT_CHAR:
		case RC::RC_COLLECT_FAIL_REWORD_WRONG_COLLECTOBJKEY:
			rc = Cancel(pStateInfo);
			SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Collected] Player�� ���� ���� �ʾ� ä���� �ֺ��� ��Ҹ� �˸���.");
			break;

		default:
			{
				rc = Cancel(pPlayer);

				MSG_CG_COLLECT_ACTION_NAK msgNAK;
				msgNAK.m_byErrorCode = err;
				pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

				SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Collected] [%s] ���� ���� �޽����� ������.", pPlayer->GetCharName());
			}
		}

		SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Collected] ä����ó�� ����[%d] cancel[%s]",
			err, RC::RC_COLLECT_SUCCESS != rc?"fail":"success" );

		return;
	}


	// ä������(����) �˸�

	CollectedList* pCollectedList = pPlayer->GetCollectedList();
    CollectionInfo* pCollectionInfo = pStateInfo->GetCollectionInfo();
    pCollectedList->SetSuccessCollectionInfo(pCollectionInfo);
    pCollectedList->EndCollecting();

	Collection* pCollection = pStateInfo->GetCollection();

	if( pCollection )
	{
		m_pField->LeaveField( pCollection );
		ObjectFactory::Instance()->FreeObject( pCollection );
	}

	pStateInfo->StartRespawn();
}


void CollectionManager::_Respawn( CollectionStateInfo* pStateInfo )
{
	MAPOBJECT_INFO* pMapInfo = pStateInfo->GetMapObjectInfo();
	if( NULL == pMapInfo )
	{
		char ch[5] = {0,};
		SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Respawn] pStateInfo ������ �������� ���� Ȱ��ȭ ��ų�� ����[%s].", WzIDToString(ch, pStateInfo->GetCollectionID()) );
		return;
	}

	DWORD ObjKey = pStateInfo->GetObjectKey();

	CollectionStateInfoManager::Release(pStateInfo);
	m_Hash.Remove(ObjKey);

	m_pField->CreateCollection(pMapInfo);
}

void CollectionManager::_Cancel( CollectionStateInfo* pStateInfo )
{
	DWORD ObjKey = pStateInfo->GetObjectKey();

	CollectionStateInfoManager::Release(pStateInfo);
	m_Hash.Remove(ObjKey);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//

void CollectionManager::Update()
{
	BYTE stat = 0; 
    bool delete_iterator = false;

	ACTIVE_COLLECTION_HASH_IT it = m_Hash.begin(), end = m_Hash.end();
	
    while( it != end )
	{
		CollectionStateInfo* pStateInfo = *it;
		if( NULL == pStateInfo )
		{
			SUNLOG( eCRITICAL_LOG, "CollectionManager::Update() �߸��� ���� ������ �����ϰ� �ִ�." );
            ++it;
			continue;
		}

        delete_iterator = false;

		if (pStateInfo->update(&stat))
		{
			switch(stat)
			{
				// ä���Ϸ�
			case CollectionStateInfo::Collecting:
				_Collected(pStateInfo);
				break;

				// �����Ϸ�
			case CollectionStateInfo::Respawning:
                {
                    MAPOBJECT_INFO* pMapInfo = pStateInfo->GetMapObjectInfo();
                    if (pMapInfo)
                    {
                        CollectionStateInfoManager::Release(pStateInfo);
                        it = m_Hash.erase(it);
                        m_pField->CreateCollection(pMapInfo);
                        delete_iterator = true;
                    }
                    else
                    {
                        char ch[5] = {0,};
                        SUNLOG( eCRITICAL_LOG, "[CollectionManager::_Respawn] pStateInfo ������ �������� ���� Ȱ��ȭ ��ų�� ����[%s].", WzIDToString(ch, pStateInfo->GetCollectionID()) );
                    }
                }
				break;
			}
		}

		if( CollectionStateInfo::cancel == stat )
        {
            CollectionStateInfoManager::Release(pStateInfo);
            it = m_Hash.erase(it);
            delete_iterator = true;
        }

        if (delete_iterator == false)
            ++it;
	}
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
bool CollectionManager::_IsSealingStrongPoint(WzID collection_id)
{
    const CollectionInfo* const collection_info = 
        CollectionParser::Instance()->FindCollectionInfo(collection_id); 

    if (collection_info == NULL)
    {
        return false;
    }
    const DWORD stamp = collection_info->m_Condition[CollectionInfo::STAMP];

    return (CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT == stamp) ? true : false;
}

void CollectionManager::_StartSealing(Player* player, WzID collection_id)
{
    if (player->IsThereBattleGroundZone() == false)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    const CollectionInfo* conllection_info = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
    if (conllection_info != NULL)
    {
        StatusManager* status_manager = player->GetStatusManager();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        status_manager->AllocStampStatus(conllection_info->m_dwCollectingTime);
#else
        //���̾� �ٵ�, ��ö ���´� ���ν� �������.
        status_manager->Remove(eCHAR_STATE_STEEL);
        status_manager->Remove(eCHAR_STATE_IMMUNITY_DAMAGE);
        status_manager->AllocStatus(eCHAR_STATE_STAMP, conllection_info->m_dwCollectingTime);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    }

    BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
    if (battle_ground == NULL)
    {
        return;
    }

    if (battle_ground->StartSealing(player, collection_id) == false)
    {
        Cancel(player);
    }
}

void CollectionManager::_CancelSealing(Player* player, WzID collection_id)
{
    GameInstanceDungeon* const instance_dungeon = 
        static_cast<GameInstanceDungeon*>(m_pField->GetGameZone());

    if (instance_dungeon == NULL) {
        return;
    }

    ChaosZoneManager* const chaoszone_manager = instance_dungeon->GetChaosZoneManager();
    if (chaoszone_manager == NULL || chaoszone_manager->GetChaosZoneType() != kBattleGround) {
        return;
    }

    BattleGround* const battle_ground = static_cast<BattleGround*>(chaoszone_manager->GetCurrentZone());
    if (battle_ground == NULL) {
        return;
    }

    if (player)
    {
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->Remove(eCHAR_STATE_STAMP);
    }

    battle_ground->CancelSealing(player, collection_id);
}

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_008334_20150608_SONNENSCHEIN
BOOL CollectionManager::_IsSonnenScheinStamp( WzID collection_id )
{
    const CollectionInfo* pCollectInfo = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
    if( NULL != pCollectInfo )
    {
        const DWORD dwStamp = pCollectInfo->m_Condition[CollectionInfo::STAMP];
        const bool is_stamp = (CollectionInfo::OBJ_SONNENSCHEIN_STAMP == dwStamp);

        return is_stamp;
    }

    return FALSE;
}

VOID CollectionManager::_StartSonnenScheinStamp( Player* player, WzID collection_id )
{
    //���λ��¸� �Ǵ�
    const CollectionInfo* conllection_info = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
    if (conllection_info != NULL)
    {
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->AllocStampStatus(conllection_info->m_dwCollectingTime);
    }

    MSG_CG_SONNENSCHEIN_BEGIN_STAMP_BRD brd_msg;
    brd_msg.stamp_id = collection_id;

    //���� ���۽ð��� �����Ѵ�.
    if(player->GetChaosZoneType() == eCHAOSZONE_TYPE_SONNENSCHEIN)
    {
        //SonnenSchein������ ����
        ChaosZone* chaos_zone = player->GetCurrentChaosZone();
        if (chaos_zone != NULL)
        {
            SonnenSchein* sonnenschein = static_cast<SonnenSchein*>(chaos_zone);
            sonnenschein->SetStampStartTime();

            //packet send
            m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
        }
    }
}

VOID CollectionManager::_CancelSonnenScheinStamp( Player* player, WzID collection_id )
{
    MSG_CG_SONNENSCHEIN_CANCEL_STAMP_BRD brd_msg;
    brd_msg.stamp_id = collection_id;

    if (player == NULL) {
        m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
    }
    else
    {
        StatusManager* status_manager = player->GetStatusManager();
        status_manager->Remove(eCHAR_STATE_STAMP);

        m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
    }

    //���� ���۽ð��� �ʱ�ȭ �Ѵ�.
    //Player�� NULL�� �� �ִ� �Լ��̹Ƿ� GameZone�� ���� ���� SonnenSchein�� �����Ѵ�
    GameInstanceDungeon* const instance_dungeon = 
        static_cast<GameInstanceDungeon*>(m_pField->GetGameZone());
    if (instance_dungeon == NULL) {
        return;
    }
    ChaosZoneManager* const chaoszone_manager = instance_dungeon->GetChaosZoneManager();
    if (chaoszone_manager == NULL || chaoszone_manager->GetChaosZoneType() != eCHAOSZONE_TYPE_SONNENSCHEIN) {
        return;
    }

    SonnenSchein* const sonnenschein = static_cast<SonnenSchein*>(chaoszone_manager->GetCurrentZone());
    if (sonnenschein == NULL) {
        return;
    }

    sonnenschein->InitStampCheckTime();
}

#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
BOOL CollectionManager::_IsOneDaysGuildmissionStamp( WzID collection_id )
{
	const CollectionInfo* pCollectInfo = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
	if( NULL != pCollectInfo )
	{
		const DWORD dwStamp = pCollectInfo->m_Condition[CollectionInfo::STAMP];
		const bool is_stamp = (CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP == dwStamp);

		return is_stamp;
	}

	return FALSE;
}

VOID CollectionManager::_StartOneDaysGuildmissionStamp( Player* player, WzID collection_id )
{
	//���λ��¸� �Ǵ�
	const CollectionInfo* conllection_info = CollectionParser::Instance()->FindCollectionInfo(collection_id); 
	if (conllection_info != NULL)
	{
		StatusManager* status_manager = player->GetStatusManager();
		status_manager->AllocStampStatus(conllection_info->m_dwCollectingTime);
	}

	MSG_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD brd_msg;
	brd_msg.stamp_id = collection_id;

	//���� ���۽ð��� �����Ѵ�.
	if(player->GetChaosZoneType() == eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION)
	{
		// ���ϱ��̼ǿ����� ����
		ChaosZone* chaos_zone = player->GetCurrentChaosZone();
		if (chaos_zone != NULL)
		{
			OneDaysGuildMission* Onedaysguildmission = static_cast<OneDaysGuildMission*>(chaos_zone);
			Onedaysguildmission->SetStampStartTime();

			//packet send
			m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
		}
	}
}

VOID CollectionManager::_CancelOneDaysGuildmissionStamp( Player* player, WzID collection_id )
{
	MSG_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD brd_msg;
	brd_msg.stamp_id = collection_id;

	if (player == NULL) {
		m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
	}
	else
	{
		StatusManager* status_manager = player->GetStatusManager();
		status_manager->Remove(eCHAR_STATE_STAMP);

		m_pField->SendPacketToAll(&brd_msg, sizeof(brd_msg));
	}

	//���� ���۽ð��� �ʱ�ȭ �Ѵ�.
	//Player�� NULL�� �� �ִ� �Լ��̹Ƿ� GameZone�� ���� ���� oneDaysGuildMission�� �����Ѵ�
	GameInstanceDungeon* const instance_dungeon = 
		static_cast<GameInstanceDungeon*>(m_pField->GetGameZone());
	if (instance_dungeon == NULL) {
		return;
	}
	ChaosZoneManager* const chaoszone_manager = instance_dungeon->GetChaosZoneManager();
	if (chaoszone_manager == NULL || chaoszone_manager->GetChaosZoneType() != eCHAOSZONE_TYPE_SONNENSCHEIN) {
		return;
	}

	OneDaysGuildMission* const onedaysguildmission = static_cast<OneDaysGuildMission*>(chaoszone_manager->GetCurrentZone());
	if (onedaysguildmission == NULL) {
		return;
	}

	onedaysguildmission->InitStampCheckTime();
}

#endif // (_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
////////////////////////////////////////////////////////////////////////////////////////////////
