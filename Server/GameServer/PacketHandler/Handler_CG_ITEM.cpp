#include "StdAfx.h"
#include ".\handler_cg_item.h"
#include <Protocol_CG.h>
#include <Protocol_DG.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_AG.h>
#include "Player.h"
#include "ItemManager.h"
#include "PlayerManager.h"
#include "GameServerEx.h"
#include <SCItemSlotContainer.h>
#include "GameField.h"
#include "Item.h"
#include "Sector.h"
#include "ObjectFactory.h"
#include "QuickManager.h"
#include "Map.h"
#include "GameServerEx.h"
#include "GameZone.h"
#include "ItemExpDistributor.h"
#include "StatisticsManager.h"
#include "ItemInfoParser.h"
#include "ExtraNPCManager.h"
#include "FunctionalRestrictManager.h"
#include "GuildWareHouseSlotContainer.h"
#include <ServerOptionParserEx.h>

#include "GameParty/NewGamePartyManager.h"

#include "InventorySlotContainer.h"

#include "Skill.h"
#include "PKScriptParser.h"

#include "GroupInfoParser.h"
#include "PlayerRider.h"


#include "RankUpImplement.h"        // [11/3/2009 namka199]
#include "RandomItemImplement.h"    // [11/3/2009 namka199]
#include "CrystallizeImplement.h"   // [11/3/2009 namka199]

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "AccumulatePointImplement.h"
#endif
#include "./Slot/EtherealizeImpl.h"
#include "ComposeOrDecomposeImplement.h"
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#include "./SocketSystem/SocketSystemImplement.h" // 소켓 시스템 구현
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#include "ImplementUtil.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "UseExtendDateItemImplement.h"
#endif
#ifdef _NA_003740_20111122_RANDOM_ITEM
#include "RandomItemParser.h"
#endif // _NA_003740_20111122_RANDOM_ITEM
#include <AchievementManager.h>

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_ITEM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_ITEM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_MOVE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_PICK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_PICK_MONEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_EXCHANGE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_DROP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_COMBINE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_DIVIDE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_MOTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_BUY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_SELL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_DELETE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_REPAIR_DURA_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_PORTAL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_CLEAR_COORD_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUICK_LINKITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUICK_LINKSKILL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUICK_UNLINK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUICK_MOVE_SYN, PHASE_SERVICE),
        // NOTE: f110216.2L, unused section
        //HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUICK_UPDATE_LINKED_ITEM_CMD, PHASE_SERVICE), 

        // 조합
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_ENCHANT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_ENCHANTUP_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_SOCKET_FILL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_SOCKET_EXTRACT_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_ACCESSORY_CREATE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_COMPOSE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_CRYSTALIZE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_EXTRASTONE_USE_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_INIT_STAT_SYN, PHASE_SERVICE),

        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_INVENTORY_PWD_SETTING_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_INVENTORY_PWD_CHECK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_INVENTORY_PWD_LOCKING_SYN	, PHASE_SERVICE),

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_EXCHANGE_REWARD_ITEM_SYN, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_HAMMER_OF_ALCHEMIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_VALIDATION_OF_REIHALT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_REPURCHASE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_PET_ITEM_SYN, PHASE_SERVICE),	

        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_PANDORA_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_MOVE_PANDORA_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_PANDORABOX_CLOSE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_EQUIP_ETHER_DEVICE_SYN, PHASE_SERVICE),

        { MAKEWORD(CG_ITEM, CG_ITEM_USE_SKILL_ACTION_SYN), PHASE_SERVICE,
          Handler_CG_ITEM::OnCG_ITEM_USE_SYN },

        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_STAT_CHANGE_USE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_STAT_CHANGE_SELECT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_ACTIVE_ETHER_BULLET_SYN, PHASE_SERVICE),	
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_IDENTIFY_POSSESSION_SYN, PHASE_SERVICE),
        //__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN, PHASE_SERVICE),
        //__NA001390_090915_RIDING_SYSTEM__
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_RIDER_CONTROL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_RIDER_CONTROL_PARTS_SYN, PHASE_SERVICE),
        //_NA_0_20091030_RANDOM_ITEM
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_RANDOM_ITEM_SYN, PHASE_SERVICE),

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_ACCUMULATE_POINT_SYN, PHASE_SERVICE),
#endif
        //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN, PHASE_SERVICE),
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_ETHERIA_CONTROL_SYN, PHASE_SERVICE),
        //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN, PHASE_SERVICE), 
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN, PHASE_SERVICE), 
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD, PHASE_SERVICE), 
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_RECREATE_SOCKET_SYN, PHASE_SERVICE), 
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_IDENTIFY_SOCKET_CMD, PHASE_SERVICE), 
#endif // _NA_000251_20100727_SOCKET_SYSTEM
        
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_USE_EXTEND_DATE_SYN, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_RANDOMIZE_SYN, PHASE_SERVICE),

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_AWAKENING_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_EVOLUTION_SYN, PHASE_SERVICE),
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        HANDLER_NODE_INFO(CG_ITEM, CG_ITEM_SPA_COSTUME_CHANGE_SYN, PHASE_SERVICE),
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


static BOOL		CheckProcessInventoryLock( Player* pPlayer, BYTE protocol, DWORD appendData = 0 )
{
	ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
	DWORD checkInvenLock = rPROTECT.IsUsable() ? ULONG_MAX : RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
	if( ULONG_MAX == checkInvenLock )
		return TRUE;	// Pass

	switch( protocol )
	{
	case CG_ITEM_ENCHANT_SYN:
		{
			MSG_CG_ITEM_ENCHANT_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
    case CG_ITEM_SOCKET_FILL_SYN:
		{
			MSG_CG_ITEM_SOCKET_FILL_NAK msgNAK;												
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_SOCKET_EXTRACT_SYN:
		{
			MSG_CG_ITEM_SOCKET_EXTRACT_NAK msgNAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_HAMMER_OF_ALCHEMIST_SYN:
		{
			MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_NAK msgNAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_COMPOSE_SYN:
		{
			MSG_CG_ITEM_COMPOSE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_CRYSTALIZE_SYN:
		{
			MSG_CG_ITEM_CRYSTALIZE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_EXTRASTONE_USE_SYN:
		{
			MSG_CG_ITEM_EXTRASTONE_USE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_PICK_SYN:
		{
			MSG_CG_ITEM_PICK_NAK msgNAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_DROP_SYN:
		{
			MSG_CG_ITEM_DROP_NAK msgNAK;
			msgNAK.m_byCategory	= CG_ITEM;
			msgNAK.m_byProtocol	= CG_ITEM_DROP_NAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_COMBINE_SYN:
		{
			MSG_CG_ITEM_COMBINE_NAK msgNAK;
			msgNAK.m_byCategory	= CG_ITEM;
			msgNAK.m_byProtocol	= CG_ITEM_COMBINE_NAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_DIVIDE_SYN:
		{
			MSG_CG_ITEM_DIVIDE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_USE_SYN:
		{
			MSG_CG_ITEM_USE_NAK msgNAK;
			msgNAK.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_SELL_SYN:
		{
			MSG_CG_ITEM_SELL_NAK msgNAK;
			msgNAK.m_byCategory	= CG_ITEM;
			msgNAK.m_byProtocol	= CG_ITEM_SELL_NAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_BUY_SYN:
		{
			MSG_CG_ITEM_BUY_NAK msgNAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_DROP_MONEY_SYN:
		{
			MSG_CG_ITEM_DROP_MONEY_NAK msgNAK;
			msgNAK.m_byCategory	= CG_ITEM;
			msgNAK.m_byProtocol	= CG_ITEM_DROP_MONEY_NAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_PICK_MONEY_SYN:
		{
			MSG_CG_ITEM_PICK_MONEY_NAK msgNAK;
			msgNAK.m_dwFieldItemObjectKey = appendData;// dwFieldItemObjectKey;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_EXCHANGE_SYN:
		{
			MSG_CG_ITEM_EXCHANGE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_QUICK_LINKITEM_SYN:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_NAK msgNAK;
			msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_QUICK_LINKSKILL_SYN:
		{
			//MSG_CG_ITEM_QUICK_LINKSKILL_NAK msgNAK;
			//msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			//pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
			return TRUE;
		}
	case CG_ITEM_QUICK_UNLINK_SYN:
		{
			//MSG_CG_ITEM_QUICK_UNLINK_NAK msgNAK;
			//msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			//pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
			return TRUE;
		}
	case CG_ITEM_QUICK_MOVE_SYN:
		{
			//MSG_CG_ITEM_QUICK_MOVE_NAK msgNAK;
			//msgNAK.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			//pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
			return TRUE;
		}
	case CG_ITEM_DELETE_SYN:
		{
			MSG_CG_ITEM_DELETE_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_REPAIR_DURA_SYN:
		{
			MSG_CG_ITEM_REPAIR_DURA_NAK msgNAK;
			msgNAK.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_PORTAL_SYN:
		{
			MSG_CG_ITEM_USE_NAK msgNAK;
			msgNAK.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_CLEAR_COORD_SYN:
		{
			MSG_CG_ITEM_USE_NAK msgNAK;
			msgNAK.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
	case CG_ITEM_INIT_STAT_SYN:
		{
			MSG_CG_ITEM_INIT_STAT_NAK msgNAK;
			msgNAK.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	case CG_ITEM_EXCHANGE_REWARD_ITEM_SYN:
		{
			MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_NAK msgNAK;
			msgNAK.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
		}
        break;
#endif

	case CG_ITEM_VALIDATION_OF_REIHALT_SYN:
		{
			MSG_CG_ITEM_VALIDATION_OF_REIHALT_NAK NakMsg;
			NakMsg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &NakMsg, sizeof( NakMsg ));
		}
        break;
	case CG_ITEM_USE_PET_ITEM_SYN:
		{
			MSG_CG_ITEM_USE_PET_ITEM_ACK AckMsg;
			AckMsg.m_Result = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
        break;
	case CG_ITEM_REPURCHASE_NAK:
		{
			MSG_CG_ITEM_REPURCHASE_NAK AckMsg;
			AckMsg.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
        break;
	case CG_ITEM_USE_PANDORA_ITEM_SYN:
	case CG_ITEM_MOVE_PANDORA_ITEM_SYN:
	case CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN:
		return TRUE;
	case CG_ITEM_EQUIP_ETHER_DEVICE_SYN:
		{
			MSG_CG_ITEM_EQUIP_ETHER_DEVICE_ACK AckMsg;
			AckMsg.m_Result	= RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
        break;
	case CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN:
		{
			MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK AckMsg;
			AckMsg.m_ResultCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
        break;
	case CG_ITEM_ACTIVE_ETHER_BULLET_SYN:
		{
			MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK AckMsg;
			AckMsg.m_ResultCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
        break;
	case CG_ITEM_IDENTIFY_POSSESSION_SYN:
		{
			MSG_CG_ITEM_IDENTIFY_POSSESSION_ACK AckMsg;
			AckMsg.m_ResultCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
		}
		break;
        //__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
	case CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN:
		{
			MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK AckMsg;
			AckMsg.m_ResultCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ) );
			break;
		}
    case CG_ITEM_RIDER_CONTROL_SYN:
    case CG_ITEM_RIDER_CONTROL_PARTS_SYN:
    case CG_ITEM_ETHERIA_CONTROL_SYN: //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        {
            // local handling
        }
        break;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    case CG_ITEM_AWAKENING_SYN:
        {
            MSG_CG_ITEM_AWAKENING_ACK ack_msg;
            ack_msg.result_code = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            pPlayer->SendPacket(&ack_msg, sizeof(ack_msg));
        }
        break;
    case CG_ITEM_EVOLUTION_SYN:
        {
            MSG_CG_ITEM_EVOLUTION_ACK ack_msg;
            ack_msg.result_code = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            pPlayer->SendPacket(&ack_msg, sizeof(ack_msg));
        }
        break;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case CG_ITEM_SPA_COSTUME_CHANGE_SYN:
        {
            MSG_CG_ITEM_SPA_COSTUME_CHANGE_NAK nak_packet;
            nak_packet.error_code = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            pPlayer->SendPacket(&nak_packet, sizeof(nak_packet));
        }
        break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
	default:
		{
			SUNLOG( eCRITICAL_LOG, _T("Unexpected Inventory Lock Check SYN %u\n"), protocol );
			break;
		}
	}

	return FALSE;	
}

Handler_CG_ITEM::Handler_CG_ITEM()
{
}

Handler_CG_ITEM::~Handler_CG_ITEM()
{
}


// 지울때 인챈트 관련 소스 다 지울것~!
Handler_CG_ITEM_IMPL( CG_ITEM_ENCHANT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_ENCHANT_SYN * pRecvMsg = (MSG_CG_ITEM_ENCHANT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_ENCHANT_SYN )] Not Exist GameField" );
		return;
	}

	RC::eITEM_RESULT rt;
	if( g_ExtraNPCManager.IsLengthCanUseNPC(pRecvMsg->m_NPCCode, pPlayer, eNPC_TYPE_ITEM_MIX) )
	{
		if (!CheckProcessInventoryLock(pPlayer, CG_ITEM_ENCHANT_SYN))
        {
            return;
        }			
#ifdef _NA_003966_20111227_ADD_ENCHANT
        const POSTYPE down_guard_item_pos = pRecvMsg->down_guard_item_pos;
#else
        const POSTYPE down_guard_item_pos = INVALID_SLOT_POS;
#endif
        rt = pPlayer->GetItemManager()->GetItemFunctionEnchant().Enchant(
            pRecvMsg->m_TargetPos, down_guard_item_pos, (eENCHANT_OPTION)pRecvMsg->m_SuccessRateIndex);
	}
	else
	{
		rt = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
	}

	switch( rt )
	{
	case RC::RC_ITEM_ENCHANT_SUCCESS:
		{
			// 성공
			MSG_CG_ITEM_ENCHANT_SUCCESS_ACK msg;
			//msg.m_TargetPos		= pRecvMsg->m_TargetPos;
			pPlayer->SendPacket(&msg,sizeof(msg));

#ifdef _NA_003966_20111227_ADD_ENCHANT
            AchievementManager* const ac_manager = pPlayer->GetACManager();
            SCSlotContainer* const inventory = pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
            SCItemSlot& target_slot = static_cast<SCItemSlot&>(inventory->GetSlot(pRecvMsg->m_TargetPos));
            ac_manager->ProcessEnchant(target_slot.GetItemCode(), target_slot.GetEnchant());
#endif
		}
		break;
	case RC::RC_ITEM_ENCHANT_FAILED:
		{
			// 실패
			MSG_CG_ITEM_ENCHANT_FAILED_ACK msg;
			msg.m_byErrorCode	= rt; 
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
		break;
	case RC::RC_ITEM_ENCHANT_FAILED_AND_DESTROY:
	case RC::RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN:
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case RC::RC_ITEM_ENCHANT_FAILED_AND_PREVENT:
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		{
			// 실패
			MSG_CG_ITEM_ENCHANT_FAILED_ACK msg;
			msg.m_byErrorCode	= rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
		break;
	default:
		{
			MSG_CG_ITEM_ENCHANT_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
			return;
		}
	}

	g_StatisticsManager.PlusStatisticType( eSTATISTIC_ENCHANT_COUNT, 1 );
}

Handler_CG_ITEM_IMPL( CG_ITEM_ENCHANTUP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
/*
	MSG_CG_ITEM_ENCHANTUP_SYN * pRecvMsg = (MSG_CG_ITEM_ENCHANTUP_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	
	BYTE Enchant = 0;
	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->Enchant( pRecvMsg->m_TargetPos, Enchant );
	switch( rt )
	{
	case RC::RC_ITEM_ENCHANT_SUCCESS:
		{
			// 성공
			MSG_CG_ITEM_ENCHANTUP_ACK msg;
			msg.m_TargetPos		= pRecvMsg->m_TargetPos;
			msg.m_TargetItemEnchant = Enchant;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
		break;
	case RC::RC_ITEM_ENCHANT_DOWN_FAILED:
		{
			// 실패
			// 다운
			MSG_CG_ITEM_ENCHANTUP_ACK msg;
			msg.m_TargetPos		= pRecvMsg->m_TargetPos;
			msg.m_TargetItemEnchant = Enchant;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
		break;
	case RC::RC_ITEM_ENCHANT_CRACK_FAILED:
		{
			// 실패
			// 소멸
			MSG_CG_ITEM_ENCHANTUP_ACK msg;
			msg.m_TargetPos		= pRecvMsg->m_TargetPos;
			msg.m_TargetItemEnchant = 0;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
		break;
	default:
		{
			MSG_CG_ITEM_ENCHANTUP_NAK msg;
			msg.m_dwErrorCode	= rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
	*/
}

Handler_CG_ITEM_IMPL(CG_ITEM_SOCKET_FILL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_SOCKET_FILL_SYN* const fill_syn = 
        static_cast<const MSG_CG_ITEM_SOCKET_FILL_SYN* const>(pMsg);
    if (sizeof(*fill_syn) != wSize) 
    {
        return;
    }

    Player* const requester = PlayerManager::Instance()->FindPlayerByUserKey(fill_syn->m_dwKey);
    if (!requester) 
    {
        return;
    }

    if (!requester->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_SOCKET_FILL_SYN )] Not Exist GameField");
        return;
    }

    if (!CheckProcessInventoryLock(requester, CG_ITEM_SOCKET_FILL_SYN))
    {
        return;
    }
        
    const RC::eITEM_RESULT fill_result =  requester->GetItemManager()->GetItemFunctionSocket().
        FillSocket(fill_syn->m_SocketItemPos, fill_syn->m_TargetPos);
    switch(fill_result)
    {
    case RC::RC_ITEM_SUCCESS:
        {
            MSG_CG_ITEM_SOCKET_FILL_ACK fill_ack;
            {
                fill_ack.m_SocketItemPos = fill_syn->m_SocketItemPos;
                fill_ack.m_TargetPos = fill_syn->m_TargetPos;
                const SCItemSlot& target_slot = 
                    static_cast<const SCItemSlot&>(
                        requester->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot(
                            fill_ack.m_TargetPos));
                target_slot.CopyOut(fill_ack.m_TargetItemStream);
            }
            requester->SendPacket(&fill_ack, sizeof(fill_ack));
        }
        break;
    default:
        {
            MSG_CG_ITEM_SOCKET_FILL_NAK fill_nak;
            {
                const ProtectorInventory& inventory_protector = 
                    requester->GetWarehouseInventoryProtector().INVENTORY();
                if (!inventory_protector.IsUsable())
                {
                    fill_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
                }
                else
                {
                    fill_nak.m_dwErrorCode = fill_result;
                }
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
                // NOTE: from waverix to hukim, need recheck the below code
                fill_nak.target_item_code = 0;
                if (fill_result != RC::RC_ITEM_INVALIDPOS)
                {
                    const SCItemSlot& target_slot = 
                        static_cast<const SCItemSlot&>(
                            requester->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot(
                                fill_syn->m_TargetPos));
                    fill_nak.target_item_code = target_slot.GetItemCode();
                };
#endif // _NA_000251_20100727_SOCKET_SYSTEM
            }
            requester->SendPacket(&fill_nak, sizeof(fill_nak));
            return;
        }
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_SOCKET_EXTRACT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_SOCKET_EXTRACT_SYN* const extract_syn = 
        static_cast<const MSG_CG_ITEM_SOCKET_EXTRACT_SYN* const>(pMsg);
    if (sizeof(*extract_syn) != wSize) 
    {
        return;
    }

    Player* const requester = PlayerManager::Instance()->FindPlayerByUserKey(extract_syn->m_dwKey);
    if (!requester)
    {
        return;
    }

    if (!requester->GetField())
    {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_SOCKET_EXTRACT_SYN )] Not Exist GameField");
        return;
    }

    if (!CheckProcessInventoryLock(requester, CG_ITEM_SOCKET_EXTRACT_SYN))
    {
        return;
    }
        
    const RC::eITEM_RESULT extract_result = \
        requester->GetItemManager()->GetItemFunctionSocket().ExtractSocket(
            extract_syn->m_ChargeIntemPos, extract_syn->m_TargetPos, extract_syn->m_SocketNum);
    if (extract_result == RC::RC_ITEM_EXTRACTSOCKET_SUCCESS)
    {	
        MSG_CG_ITEM_SOCKET_EXTRACT_SUCCESS_ACK extract_ack;
        {
            extract_ack.m_ChargeIntemPos = extract_syn->m_ChargeIntemPos;		    
            extract_ack.m_TargetPos = extract_syn->m_TargetPos;
            const SCItemSlot& socket_owner = 
                static_cast<const SCItemSlot&>(
                    requester->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot(
                        extract_ack.m_TargetPos));
            socket_owner.CopyOut(extract_ack.m_TargetItemStream);
        }
        requester->SendPacket(&extract_ack, sizeof(extract_ack));
        return;
    }
    
    MSG_CG_ITEM_SOCKET_EXTRACT_NAK extract_nak;
    {
        const ProtectorInventory& inven_protector = 
            requester->GetWarehouseInventoryProtector().INVENTORY();
        if (!inven_protector.IsUsable())
        {
            extract_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        }
        else
        {
            extract_nak.m_dwErrorCode = extract_result;
        }
    }
    requester->SendPacket(&extract_nak, sizeof(extract_nak));
}

Handler_CG_ITEM_IMPL(CG_ITEM_HAMMER_OF_ALCHEMIST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN * pRecvMsg = (MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_HAMMER_OF_ALCHEMIST_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_HAMMER_OF_ALCHEMIST_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionItemProperty().
        UseHammerOfAlchemist(pRecvMsg->m_ChargeItemPos, pRecvMsg->m_TargetItemPos);

	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_ACK msg;
            msg.m_ChargeItemPos = pRecvMsg->m_ChargeItemPos;
			msg.m_TargetItemPos	= pRecvMsg->m_TargetItemPos;
			SCItemSlot & rSlot	= (SCItemSlot &)pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot( pRecvMsg->m_TargetItemPos );
			rSlot.CopyOut(msg.m_TargetItemStream);
			pPlayer->SendPacket(&msg,sizeof(msg));			
		}
		break;
	default:
		{
			MSG_CG_ITEM_HAMMER_OF_ALCHEMIST_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
			return;
		}
	}
}

/*
Handler_CG_ITEM_IMPL( CG_ITEM_ACCESSORY_CREATE_SYN )
{
	
	MSG_CG_ITEM_ACCESSORY_CREATE_SYN * pRecvMsg = (MSG_CG_ITEM_ACCESSORY_CREATE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	MSG_CG_ITEM_ACCESSORY_CREATE_ACK amsg;
	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->CreateAccessory( pRecvMsg->m_type, pRecvMsg->m_SocketItemPos, &amsg.m_ItemInfo );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			amsg.m_type = pRecvMsg->m_type;
			amsg.m_SocketItemPos = pRecvMsg->m_SocketItemPos;
			amsg.m_Money = pPlayer->GetMoney();
			pPlayer->SendPacket(&amsg,amsg.GetSize());
		}
		break;
	default:
		{
			MSG_CG_ITEM_ACCESSORY_CREATE_NAK msg;
			msg.m_dwErrorCode	= rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
	
}*/

Handler_CG_ITEM_IMPL( CG_ITEM_COMPOSE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_COMPOSE_SYN * pRecvMsg = (MSG_CG_ITEM_COMPOSE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_COMPOSE_SYN )] Not Exist GameField" );
		return;
	}

	BYTE	bySuccessNum = 0;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    BYTE    byPreventNum = 0;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	MSG_CG_ITEM_COMPOSE_SUCCESS_ACK amsg;

	RC::eITEM_RESULT rt;
	if( g_ExtraNPCManager.IsLengthCanUseNPC(pRecvMsg->m_NPCCode, pPlayer, eNPC_TYPE_ITEM_MIX) )
	{
		if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_COMPOSE_SYN ) )
			return;
#ifdef  _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        rt = pPlayer->GetItemManager()->Compose( pRecvMsg->m_ComposeListCode, pRecvMsg->m_byMakingNum, &amsg.m_ItemInfo, bySuccessNum, byPreventNum);
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        rt = pPlayer->GetItemManager()->Compose( pRecvMsg->m_ComposeListCode, pRecvMsg->m_byMakingNum, &amsg.m_ItemInfo, bySuccessNum );
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
	}
	else
	{
		rt = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
	}

	switch( rt )
	{
	case RC::RC_ITEM_COMPOSE_SUCCESS:
		{
			amsg.m_Money = pPlayer->GetMoney();
			amsg.m_byMakingNum	= pRecvMsg->m_byMakingNum;
			amsg.m_bySuccessNum	= bySuccessNum;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            amsg.m_byPreventNum = byPreventNum;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
			pPlayer->SendPacket(&amsg,amsg.GetSize());
		}
        break;
	case RC::RC_ITEM_COMPOSE_FAILED:
		{
			MSG_CG_ITEM_COMPOSE_FAILED_ACK f_amsg;
            f_amsg.m_Money = pPlayer->GetMoney();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            f_amsg.m_byPreventNum = byPreventNum;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
			pPlayer->SendPacket(&f_amsg,sizeof(f_amsg));
		}
		break;
	default:
		{
			MSG_CG_ITEM_COMPOSE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
			return;
		}
	}

	// 통계 정보 수집(성현창)
	sITEM_COMPOSE_LIST * pListInfo = ItemComposeListParser::Instance()->GetComposeListInfo(pRecvMsg->m_ComposeListCode);
	if( pListInfo->m_ItemType )
	{
		switch( pListInfo->m_ItemType )
		{
			case eITEMTYPE_RING:
			{
				g_StatisticsManager.PlusStatisticType( eSTATISTIC_COMPOSE_RING_LOW, 1 );
				break;
			}
			case eITEMTYPE_NECKLACE:
			{
				g_StatisticsManager.PlusStatisticType( eSTATISTIC_COMPOSE_NECKLACE_LOW, 1 );
				break;
			}
			// 클래스별 특수 악세서리로 변경
			case eITEMTYPE_BERSERKER_SACCESSORY:
			case eITEMTYPE_DRAGON_SACCESSORY:
			case eITEMTYPE_VALKYRIE_SACCESSORY:
			case eITEMTYPE_SHADOW_SACCESSORY:
			case eITEMTYPE_ELEMENTALIST_SACCESSORY:
            case eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            case eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            case eITEMTYPE_ALLCLASS_CHAOSEYE:             //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_BERSERKER_KALVUS_ACCESSORY:    //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_DRAGON_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_MYSTIC_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            case eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
			{
				g_StatisticsManager.PlusStatisticType( eSTATISTIC_COMPOSE_SOUL_SHEILD_LOW, 1 );
				break;
			}
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_CRYSTALIZE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    CrystallizeImplement Implement;
    Implement.DoMessageImplement( pMsg );
    return;
}

Handler_CG_ITEM_IMPL( CG_ITEM_EXTRASTONE_USE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_EXTRASTONE_USE_SYN * pRecvMsg = (MSG_CG_ITEM_EXTRASTONE_USE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_EXTRASTONE_USE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_EXTRASTONE_USE_SYN ) )
		return;

	BYTE byValueIndex = 0;
    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionEnchant().UseExtraStone(
        pRecvMsg->m_ExtraStonePos, pRecvMsg->m_TargetPos, byValueIndex);
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			MSG_CG_ITEM_EXTRASTONE_USE_ACK amsg;
			amsg.m_NOptionAttrValueIndex = byValueIndex;
			pPlayer->SendPacket(&amsg,sizeof(amsg));
		}
		break;
	default:
		{
			MSG_CG_ITEM_EXTRASTONE_USE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL(CG_ITEM_MOVE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_MOVE_SYN* const move_syn = static_cast<const MSG_CG_ITEM_MOVE_SYN* const>(pMsg);
    
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(move_syn->m_dwKey);
    ASSERT(player != NULL);
    if (player == NULL)
    {
        return;  
    }
        
    if (player->GetField() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_ITEM_IMPL( CG_ITEM_MOVE_SYN )] Not Exist GameField"));
        return;
    }

    if (wSize != sizeof(MSG_CG_ITEM_MOVE_SYN)) 
    {
        SUNLOG(eCRITICAL_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_MOVE_SYN"));
        return;
    }

    const SLOTIDX from_slot_type = move_syn->m_fromIndex;
    const POSTYPE from_slot_position = move_syn->m_fromPos;
    const SLOTIDX to_slot_type = move_syn->m_toIndex;
    const POSTYPE to_slot_position = move_syn->m_toPos;

    if ((from_slot_type == SI_INVENTORY || to_slot_type == SI_INVENTORY) && 
        !CheckProcessInventoryLock(player, CG_ITEM_MOVE_SYN))
    {
        return;
    }

    const DURATYPE move_quantity = move_syn->move_quantity_;

    ItemManager* const item_manager = player->GetItemManager();
    // CHANGES: f110418.1L
    RC::eITEM_RESULT move_result = RC::RC_ITEM_SUCCESS;
    if (nsSlot::ItemMoveImpl* impl = item_manager->GetItemMoveImpl()) {
        move_result = impl->Move(from_slot_type, from_slot_position,
                                 to_slot_type, to_slot_position,
                                 move_quantity);
    };

    switch(move_result)
    {
    case RC::RC_ITEM_SUCCESS:
        {
            MSG_CG_ITEM_MOVE_ACK move_ack;
            {
                move_ack.m_fromIndex = from_slot_type;
                move_ack.m_fromPos = from_slot_position;
                move_ack.m_toIndex = to_slot_type;
                move_ack.m_toPos = to_slot_position;
                move_ack.move_quantity_ = move_quantity;
                player->SendPacket(&move_ack, sizeof(move_ack));
            }
            // CHANGES: f110418.1L, moved logging parts to ItemMoveImpl
        }
        break;
    case RC::RC_ITEM_INVALIDPOS:
    default:
        {
            MSG_CG_ITEM_MOVE_NAK move_nak;
            move_nak.m_byCategory = CG_ITEM;
            move_nak.m_byProtocol = CG_ITEM_MOVE_NAK;
            move_nak.m_dwErrorCode = move_result;

            bool process_to_next = true;
            if (process_to_next && (from_slot_type == SI_INVENTORY || to_slot_type == SI_INVENTORY))
            {
                const ProtectorInventory& inventory_protector = 
                    player->GetWarehouseInventoryProtector().INVENTORY();
                if (!inventory_protector.IsUsable())
                {
                    move_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
                    process_to_next = false;
                }
            }

            if (process_to_next && (from_slot_type == SI_WAREHOUSE || to_slot_type == SI_WAREHOUSE))
            {
                ProtectorWarehouse& warehouse_protector = 
                    player->GetWarehouseInventoryProtector().WAREHOUSE();
                if (!warehouse_protector.IsUsable())
                {
                    move_nak.m_dwErrorCode = RC::RC_ITEM_WAREHOUSE_NEED_PASSWORD;
                    process_to_next = false;
                }
            }

            player->SendPacket(&move_nak, sizeof(move_nak));
        }
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_PICK_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_ITEM_PICK_SYN* pRecvMsg = (MSG_CG_ITEM_PICK_SYN *)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(!FlowControl::FCAssert(pPlayer != 0))
        return;

    if(!pPlayer->GetField()) {
        SUNLOG(eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL(CG_ITEM_PICK_SYN)] Not Exist GameField");
        return;
    }

    {
        BOOLEAN bIsValidSize = sizeof(MSG_CG_ITEM_PICK_SYN) == wSize;
        if(bIsValidSize == 0) {
            SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_PICK_SYN"));
            return;
        }
    };

    DWORD dwFieldItemObjectKey = pRecvMsg->m_dwFieldItemObjectKey;

    if(!CheckProcessInventoryLock(pPlayer, CG_ITEM_PICK_SYN))
        return;

    static const SERVER_COMMON_OPTION& ServerOption =
        ServerOptionParserEx::Instance()->GetServerOption();

    MSG_CG_ITEM_PICK_ACK msgACK;

    ItemManager* item_manager = pPlayer->GetItemManager();

#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    RC::eITEM_RESULT ret = item_manager->PickParty(dwFieldItemObjectKey, ServerOption);
    if(ret == RC::RC_ITEM_SUCCESS) 
    {
        // nothing to do..
    }
#else
    RC::eITEM_RESULT ret = item_manager->Pick(dwFieldItemObjectKey, &msgACK.m_ItemInfo);
    if(ret == RC::RC_ITEM_SUCCESS) {
        pPlayer->SendPacket(&msgACK, msgACK.GetSize());

        const sPOLICY_CHARACTER& rPolicyment = pPlayer->Policyment;
        const BOOLEAN isBrdApply = !(rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD_PLAYER);
        if(isBrdApply) {
            // 아이템 습득을 주변에 알린다.
            MSG_CG_ITEM_PICK_BRD msgBRD;
            msgBRD.m_dwObjKey = pPlayer->GetObjectKey();
            msgBRD.m_ItemCode = msgACK.m_ItemInfo.m_Slot[0].m_Stream.Part.wCode;
            if(ServerOption.m_bItemPickUpMsgBrd != 0) {
                pPlayer->SendPacketAround(&msgBRD, sizeof(msgBRD), FALSE);
            }
            if(ServerOption.m_bItemPickUpPartyMsgBrd != 0) {
                WORD party_key = pPlayer->GetPartyState().GetPartyKey();
                if(party_key != 0) {
                    NewGamePartyManager* pPartyManager = NewGamePartyManager::Instance();
                    pPartyManager->SendPacketAllMember(party_key, &msgBRD, sizeof(msgBRD));
                }
            }
        }
    }
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    else { //ret != RC::RC_ITEM_SUCCESS
        MSG_CG_ITEM_PICK_NAK msgNAK;
        msgNAK.m_dwFieldItemObjectKey = dwFieldItemObjectKey;
        msgNAK.m_dwErrorCode = ret;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_DROP_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_DROP_SYN * pRecvMsg = (MSG_CG_ITEM_DROP_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_DROP_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_DROP_SYN ) )
		return;

	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->Drop( pRecvMsg->m_atIndex, pRecvMsg->m_atPos );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			MSG_CG_ITEM_DROP_ACK msg;
			msg.m_byCategory	= CG_ITEM;
			msg.m_byProtocol	= CG_ITEM_DROP_ACK;
			msg.m_atIndex		= pRecvMsg->m_atIndex;
			msg.m_atPos			= pRecvMsg->m_atPos;

			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_DROP_NAK msg;
			msg.m_byCategory	= CG_ITEM;
			msg.m_byProtocol	= CG_ITEM_DROP_NAK;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL(CG_ITEM_COMBINE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_COMBINE_SYN* const combine_syn = 
        static_cast<const MSG_CG_ITEM_COMBINE_SYN* const>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(combine_syn->m_dwKey);
    ASSERT(player != NULL);
    if (player == NULL)
    {
        return;
    }
        
    if (player->GetField() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_ITEM_IMPL( CG_ITEM_COMBINE_SYN )] Not Exist GameField"));
        return;
    }

    if (wSize != sizeof(MSG_CG_ITEM_COMBINE_SYN))
    {
        SUNLOG(eCRITICAL_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_COMBINE_SYN"));
        return;
    }

    if (!CheckProcessInventoryLock(player, CG_ITEM_COMBINE_SYN))
    {
        return;
    }
        
    DURATYPE combine_quantity = combine_syn->combine_quantity_;

    const RC::eITEM_RESULT combine_result = player->GetItemManager()->Combine(combine_syn->m_fromIndex, 
                                                                              combine_syn->m_fromPos, 
                                                                              combine_syn->m_toIndex, 
                                                                              combine_syn->m_toPos, 
                                                                              combine_quantity);
    switch(combine_result)
    {
    case RC::RC_ITEM_SUCCESS:
        {
            MSG_CG_ITEM_COMBINE_ACK combine_ack;
            combine_ack.m_byCategory = CG_ITEM;
            combine_ack.m_byProtocol = CG_ITEM_COMBINE_ACK;
            combine_ack.m_fromIndex = combine_syn->m_fromIndex;
            combine_ack.m_toIndex = combine_syn->m_toIndex;
            combine_ack.m_fromPos = combine_syn->m_fromPos;
            combine_ack.m_toPos = combine_syn->m_toPos;
            combine_ack.combine_quantity_ = combine_quantity;
            player->SendPacket(&combine_ack, sizeof(combine_ack));
        }
        break;
    default:
        {
            MSG_CG_ITEM_COMBINE_NAK combine_nak;
            combine_nak.m_byCategory = CG_ITEM;
            combine_nak.m_byProtocol = CG_ITEM_COMBINE_NAK;
            const ProtectorInventory& inventory_protector = 
                player->GetWarehouseInventoryProtector().INVENTORY();
            if (!inventory_protector.IsUsable())
            {
                combine_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            }
            else
            {
                combine_nak.m_dwErrorCode = combine_result;
            }
            player->SendPacket(&combine_nak, sizeof(combine_nak));
        }
    }
}

Handler_CG_ITEM_IMPL( CG_ITEM_DIVIDE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_DIVIDE_SYN * pRecvMsg = (MSG_CG_ITEM_DIVIDE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_DIVIDE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_DIVIDE_SYN ) )
		return;

	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->Divide( pRecvMsg->m_fromPos, pRecvMsg->m_toPos, pRecvMsg->m_fromNum, pRecvMsg->m_toNum );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			MSG_CG_ITEM_DIVIDE_ACK msg;
			//msg.m_fromPos	= pRecvMsg->m_fromPos;
			//msg.m_toPos		= pRecvMsg->m_toPos;
			//msg.m_fromNum	= pRecvMsg->m_fromNum;
			//msg.m_toNum		= pRecvMsg->m_toNum;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_DIVIDE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

// 소모성 아이템만 처리함

Handler_CG_ITEM_IMPL(CG_ITEM_USE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	const MSG_CG_ITEM_USE_SYN* const item_use_syn = (MSG_CG_ITEM_USE_SYN*)pMsg;

	Player* player = PlayerManager::Instance()->FindPlayerByUserKey(item_use_syn->m_dwKey);
	if (player == NULL)
    {
        return;
    }
    
	if (player->GetField() == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_USE_SYN )] Not Exist GameField");
		return;
	}

	if (!CheckProcessInventoryLock(player, CG_ITEM_USE_SYN))
    {
        return;
    }
	
    SLOTCODE item_code = 0;
	RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;	
    
    ItemManager* const item_manager = player->GetItemManager();
    if (item_manager->IsExtraPacketUse(item_use_syn->m_atIndex, item_use_syn->m_atPos))
    {
        result = RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    else if (item_use_syn->m_byProtocol == CG_ITEM_USE_SKILL_ACTION_SYN)
    {
        // 자기자신이 아닌 타인에게 영향을 주는 스킬의 경우 강제 공격의 여부와 공격 타겟 위치가 필요.  
        const MSG_CG_ITEM_USE_SKILL_ACTION_SYN& recv_msg = \
            static_cast<MSG_CG_ITEM_USE_SKILL_ACTION_SYN&>(*pMsg);
        // TODO, f100831.3L, waverix, need check a packet integrity
        SkillInfo skill_info_data(player, recv_msg);
        result = item_manager->Use(recv_msg.m_atIndex, recv_msg.m_atPos, item_code,
            &skill_info_data, skill_info_data.attack_propensity_);
    }
    else
    {
        result = item_manager->Use( item_use_syn->m_atIndex, item_use_syn->m_atPos, item_code );
    }

	switch (result)
	{
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    case RC::RC_ITEM_FAILED_EXIST_SEPARATE_NAK_PACKET:
        // 실패했지만 별도의 NAK 패킷이 존재한다
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    case RC::RC_ITEM_PROCESS_PENDING:
        // 처리가 비동기로 이루어지며, 즉시 결과를 회신할 수 없는 경우
        return;
	case RC::RC_ITEM_SUCCESS:
		{
            const BASE_ITEMINFO* const item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
			if (item_info == NULL)
            {
                break;
            }
			
            if (item_info->m_wType == eITEMTYPE_GUILDWAREHOUSE_SLOTCASH)
			{
				return;
			}
			
            if (item_info->IsMaterialWaste())
			{
			    MSG_CG_ITEM_USE_ACK item_use_ack;
			    item_use_ack.m_atIndex = item_use_syn->m_atIndex;
			    item_use_ack.m_atPos = item_use_syn->m_atPos;
			    player->SendPacket(&item_use_ack, sizeof(item_use_ack));
			}
			
            if (item_info->m_dwEffectCode != Null_Value && 
                item_use_syn->m_byProtocol != CG_ITEM_USE_SKILL_ACTION_SYN)
			{
				MSG_CG_ITEM_USE_BRD item_use_brd;
				item_use_brd.m_dwObjKey = player->GetObjectKey();
				item_use_brd.m_ItemCode = item_code;
				player->SendPacketAround(&item_use_brd, sizeof(item_use_brd), FALSE);
			}

			if (item_info->m_byWasteType == eITEMWASTE_RETURNSCROLL)
			{
                // added by _NA_0_20100722_AUTO_SYSTEM
                // 마지막 귀환 스크롤 위치로의 이동을 위해 저장해둔다.
                player->SetlastReturnScroll();
				// 귀환스크롤
				MSG_AG_ZONE_RETURN_VILLAGE_ACK return_village_ack;
				return_village_ack.m_wMapCode = player->GetPreVillageMapCode();
				player->SendPacket(&return_village_ack, sizeof(return_village_ack));
			}
			else if (item_info->m_byWasteType == eITEMWASTE_UPSTONE)
			{
				// UP 올라감
				MSG_CG_CHARINFO_USERPOINT_CMD user_point_cmd;
				user_point_cmd.m_TotalUP = player->GetCharInfo()->m_UserPoint;
				player->SendPacket( &user_point_cmd, sizeof(user_point_cmd) );
			}

			if (item_info->m_wType == eITEMTYPE_REPAIR)
			{
				MSG_CG_ITEM_REPAIR_DURA_ACK repair_dura_ack;
				repair_dura_ack.m_bAllRepair = true;
				repair_dura_ack.m_Money = player->GetMoney();
				player->SendPacket(&repair_dura_ack, sizeof(repair_dura_ack));
			}

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            if (item_info->m_wType == eITEMTYPE_SPA_WARP_SCROLL)
            {
                player->RemoveDragonTransformStatus();

                MSG_AG_ZONE_SPA_ENTER_ACK enter_spa_packet;
                enter_spa_packet.m_dwKey = player->GetUserKey();
                enter_spa_packet.spa_mapcode = item_info->m_wSkillCode;

                player->SendPacket(&enter_spa_packet, sizeof(enter_spa_packet));
            }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
		}
		break;
	default:
		{
			MSG_CG_ITEM_USE_NAK item_use_nak;
			const ProtectorInventory& inven_protector = 
                player->GetWarehouseInventoryProtector().INVENTORY();
			if (!inven_protector.IsUsable())
            {
                item_use_nak.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            }
			else
            {
                item_use_nak.m_ErrorCode = result;
            }
			player->SendPacket(&item_use_nak, sizeof(item_use_nak));
		}
	}
}


Handler_CG_ITEM_IMPL(CG_ITEM_SELL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    MSG_CG_ITEM_SELL_SYN* const item_sell_syn = static_cast<MSG_CG_ITEM_SELL_SYN* const>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(item_sell_syn->m_dwKey);
    ASSERT(player != NULL);
    if (player == NULL)
    {
        return;
    }
        
    if (player->GetField() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_ITEM_IMPL( CG_ITEM_SELL_SYN )] Not Exist GameField"));
        return;
    }

    if (wSize != sizeof(MSG_CG_ITEM_SELL_SYN))
    {
        SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_SELL_SYN"));
        return;
    }
    item_sell_syn->Decode();

    const DURATYPE sell_quantity = item_sell_syn->sell_quantity_;

    RC::eITEM_RESULT sell_result;
    if (g_ExtraNPCManager.IsLengthCanUseNPC(item_sell_syn->m_NPCCode, player, eNPC_TYPE_STORE))
    {
        if (!CheckProcessInventoryLock(player, CG_ITEM_SELL_SYN))
        {
            return;
        }
        sell_result = player->GetItemManager()->GetTradingImpl()->Sell(item_sell_syn->m_atIndex, item_sell_syn->m_atPos, sell_quantity);
    }
    else
    {
        sell_result = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
    }

    switch(sell_result)
    {
    case RC::RC_ITEM_SUCCESS:
        {
            MSG_CG_ITEM_SELL_ACK sell_ack;
            sell_ack.m_byCategory = CG_ITEM;
            sell_ack.m_byProtocol = CG_ITEM_SELL_ACK;
            sell_ack.m_atIndex = item_sell_syn->m_atIndex;
            sell_ack.m_atPos = item_sell_syn->m_atPos;
            sell_ack.m_Money = player->GetMoney();
            sell_ack.sell_quantity_ = sell_quantity;
            player->SendPacket(&sell_ack, sizeof(sell_ack));
        }
        break;
    default:
        {
            MSG_CG_ITEM_SELL_NAK sell_nak;
            sell_nak.m_byCategory = CG_ITEM;
            sell_nak.m_byProtocol = CG_ITEM_SELL_NAK;
            const ProtectorInventory& inventory_protector = player->GetWarehouseInventoryProtector().INVENTORY();
            if (!inventory_protector.IsUsable())
            {
                sell_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            }   
            else
            {
                sell_nak.m_dwErrorCode = sell_result;
            }               
            player->SendPacket(&sell_nak, sizeof(sell_nak));
        }
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_BUY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_BUY_SYN* const item_buy_syn = static_cast<const MSG_CG_ITEM_BUY_SYN* const>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(item_buy_syn->m_dwKey);
    ASSERT(player != NULL);
    if (player == NULL)
    {
        return;
    }
    
    if (player->GetField() == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[Handler_CG_ITEM_IMPL( CG_ITEM_BUY_SYN )] Not Exist GameField"));
        return;
    }

    if (wSize != sizeof(MSG_CG_ITEM_BUY_SYN))
    {
        SUNLOG(eCRITICAL_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_BUY_SYN"));
        return;
    }

    MSG_CG_ITEM_BUY_ACK item_buy_ack;

    RC::eITEM_RESULT item_buy_result;
    if (g_ExtraNPCManager.IsLengthCanUseNPC(item_buy_syn->m_NPCCode, player, eNPC_TYPE_STORE))
    {
        if (!CheckProcessInventoryLock(player, CG_ITEM_BUY_SYN))
        {
            return;
        }

        const DURATYPE buy_quantity = item_buy_syn->buy_quantity_;
        item_buy_result = player->GetItemManager()->GetTradingImpl()->Buy(
            item_buy_syn->m_dwShopListID, 
            item_buy_syn->m_ShopTabIndex, 
            item_buy_syn->m_ShopItemIndex, 
            &item_buy_ack, 
            buy_quantity);
    }
    else
    {
        item_buy_result = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
    }

    switch(item_buy_result)
    {
    case RC::RC_ITEM_SUCCESS:
        {
            player->SendPacket(&item_buy_ack, item_buy_ack.GetSize());
        }
        break;
    default:
        {
            MSG_CG_ITEM_BUY_NAK item_buy_nak;
            const ProtectorInventory& inventory_protector = 
                player->GetWarehouseInventoryProtector().INVENTORY();
            if (!inventory_protector.IsUsable())
            {
                item_buy_nak.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            }               
            else
            {
                item_buy_nak.m_dwErrorCode = item_buy_result;
            }
            player->SendPacket(&item_buy_nak, sizeof(item_buy_nak));
        }
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_DROP_MONEY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_DROP_MONEY_SYN * pRecvMsg = (MSG_CG_ITEM_DROP_MONEY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_DROP_MONEY_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_DROP_MONEY_SYN ) )
		return;

	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->DropMoney( pRecvMsg->m_dwDropMoney );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			GAMELOG->LogMoney( MONEY_DROP, pPlayer, pRecvMsg->m_dwDropMoney );
			/* Nothing To Do */
		}
		break;
	default:
		{
			MSG_CG_ITEM_DROP_MONEY_NAK msg;
			msg.m_byCategory	= CG_ITEM;
			msg.m_byProtocol	= CG_ITEM_DROP_MONEY_NAK;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}

}

////////////////////////////////////////////////////////////////////////////
//// (WAVERIX) (090514) (REMOVE) 사용 안 하는 블록 제거
//class TotalLevelSum
//{
//	float				m_limitDistance;
//	Player *			m_pStandardPlayer;
//	LEVELTYPE			m_TotalLevelSum;
//public:
//	TotalLevelSum( Player *	pStandardPlayer ):m_TotalLevelSum(0),m_pStandardPlayer(pStandardPlayer){}
//	~TotalLevelSum(){}
//
//	inline LEVELTYPE GetTotalLevelSum() { return m_TotalLevelSum; }
//	inline VOID	SetDistance(  float limitDistance ) { m_limitDistance = limitDistance;	}
//
//	VOID operator()( Player * pPlayer )
//	{
//		if( m_limitDistance == 0.0f )
//		{
//			m_TotalLevelSum += pPlayer->GetLevel();
//		}
//		else
//		{
//			if( pPlayer->GetDist(m_pStandardPlayer) < m_limitDistance )
//			{
//				m_TotalLevelSum += pPlayer->GetLevel();
//			}
//		}
//	}
//};

Handler_CG_ITEM_IMPL(CG_ITEM_PICK_MONEY_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_PICK_MONEY_SYN * pRecvMsg = (MSG_CG_ITEM_PICK_MONEY_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_PICK_MONEY_SYN )] Not Exist GameField" );
		return;
	}

	BOOL bIsValidSize = (BOOL) ( sizeof(MSG_CG_ITEM_PICK_MONEY_SYN) == wSize );
	if( !bIsValidSize )
	{
		SUNLOG( eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ITEM_PICK_MONEY_SYN") );
		return;
	}
	DWORD dwFieldItemObjectKey = pRecvMsg->m_dwFieldItemObjectKey;

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_PICK_MONEY_SYN, dwFieldItemObjectKey ) )
		return;

	if( !pPlayer->GetItemManager()->ValidState() )
	{
		MSG_CG_ITEM_PICK_MONEY_NAK msg;
		msg.m_dwErrorCode	= RC::RC_ITEM_INVALIDSTATE;
		msg.m_dwFieldItemObjectKey = dwFieldItemObjectKey;
		pPlayer->SendPacket( &msg, sizeof( msg ) );
		return;
	}

	GameField * pField = pPlayer->GetField();
	if( !pField || !pField->IsReady() ) return;

    // CHANGES: f100503.3L, change the interface usage
    // f110415.6L, changes using method to find simple. the internal process of SearchNeighborItem
    // brutely has the object searching algorithm for each items.
    Item* pItem = NULL;
    ;{
        Object* object = pField->FindObject(MONEY_OBJECT, dwFieldItemObjectKey);
        if (object && object->IsEqualObjectKind(MONEY_OBJECT) &&
            object->IsDeleteReserved() == false)
        {
            pItem = static_cast<Item*>(object);
        }
    };
#if SUN_CODE_BACKUP
    Item* pItem = pField->SearchNeighborItem(pPlayer->GetSectorIndex(), dwFieldItemObjectKey);
#endif
	if( !pItem )
	{
		MSG_CG_ITEM_PICK_MONEY_NAK msg;
		msg.m_dwErrorCode	= RC::RC_ITEM_NOTEXISTITEMATFIELD;
		msg.m_dwFieldItemObjectKey = dwFieldItemObjectKey;
		pPlayer->SendPacket( &msg, sizeof( msg ) );
		return;
	}

	ItemManager* pItemManager = pPlayer->GetItemManager();
	if( !pItemManager )
		return;

	RC::eITEM_RESULT rt = pItemManager->CanPick( pItem );
	if( rt != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_PICK_MONEY_NAK msg;
		msg.m_dwFieldItemObjectKey = dwFieldItemObjectKey;
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
		else
			msg.m_dwErrorCode = rt;

		pPlayer->SendPacket(&msg,sizeof(msg));
		return;
	}

	MONEY DropMoney = pItem->GetMoney();
    DWORD dwMonsterKey = pItem->GetFromMonsterKey();
    WORD PartyKey = pPlayer->GetPartyState().GetPartyKey();

    if(!PartyKey || dwMonsterKey == 0)
    {
        pPlayer->PickMoney(DropMoney, dwMonsterKey, pPlayer);
    }
    else
    {
        g_ItemExpDistributor.DistributeMoney( DropMoney, dwMonsterKey, pPlayer );
    }

	pField->DestroyObject( pItem );
}

Handler_CG_ITEM_IMPL( CG_ITEM_EXCHANGE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_EXCHANGE_SYN * pRecvMsg = (MSG_CG_ITEM_EXCHANGE_SYN *)pMsg;
	
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_EXCHANGE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_EXCHANGE_SYN ) )
		return;

	MSG_CG_ITEM_EXCHANGE_ACK amsg;
	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetTradingImpl()->Exchange(
        pRecvMsg->m_atPos, &amsg.m_tagInventory );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			pPlayer->SendPacket( &amsg, amsg.GetSize() );
		}
		break;
	default:
		{
			MSG_CG_ITEM_EXCHANGE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_LINKITEM_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_QUICK_LINKITEM_SYN * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKITEM_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_LINKITEM_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_QUICK_LINKITEM_SYN ) )
		return;

	RC::eQUICK_RESULT rt = pPlayer->GetQuickManager()->LinkItem( pRecvMsg->m_OrgPos, pRecvMsg->m_ToPos );
	switch( rt )
	{
	case RC::RC_QUICK_SUCCESS:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_ACK msg;
			msg.m_OrgPos		= pRecvMsg->m_OrgPos;
			msg.m_ToPos			= pRecvMsg->m_ToPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_QUICK_LINKITEM_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_LINKSKILL_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_QUICK_LINKSKILL_SYN * pRecvMsg = (MSG_CG_ITEM_QUICK_LINKSKILL_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_LINKSKILL_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_QUICK_LINKSKILL_SYN ) )
		return;

	RC::eQUICK_RESULT rt = pPlayer->GetQuickManager()->LinkSkill( pRecvMsg->m_SkillCode, pRecvMsg->m_ToPos );
	switch( rt )
	{
	case RC::RC_QUICK_SUCCESS:
		{
			MSG_CG_ITEM_QUICK_LINKSKILL_ACK msg;
			msg.m_SkillCode		= pRecvMsg->m_SkillCode;
			msg.m_ToPos			= pRecvMsg->m_ToPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_QUICK_LINKSKILL_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_UNLINK_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_QUICK_UNLINK_SYN * pRecvMsg = (MSG_CG_ITEM_QUICK_UNLINK_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
        // TODO: 라이브 로그 과다 출력 문제 임시 수정
        // 클라에서 패치 적용되면 수정되는 문제로 클라 패치 적용전까지 주석 처리
		//SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_UNLINK_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_QUICK_UNLINK_SYN ) )
		return;

	RC::eQUICK_RESULT rt = pPlayer->GetQuickManager()->Unlink( pRecvMsg->m_atPos );
	switch( rt )
	{
	case RC::RC_QUICK_SUCCESS:
		{
			MSG_CG_ITEM_QUICK_UNLINK_ACK msg;
			msg.m_atPos			= pRecvMsg->m_atPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_QUICK_UNLINK_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_MOVE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_QUICK_MOVE_SYN * pRecvMsg = (MSG_CG_ITEM_QUICK_MOVE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_QUICK_MOVE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_QUICK_MOVE_SYN ) )
		return;

	RC::eQUICK_RESULT rt = pPlayer->GetQuickManager()->Move( pRecvMsg->m_fromPos, pRecvMsg->m_toPos );
	switch( rt )
	{
	case RC::RC_QUICK_SUCCESS:
		{
			MSG_CG_ITEM_QUICK_MOVE_ACK msg;
			msg.m_fromPos		= pRecvMsg->m_fromPos;
			msg.m_toPos			= pRecvMsg->m_toPos;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_QUICK_MOVE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_dwErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_dwErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

#if SUN_CODE_BACKUP
// NOTE: f110216.2L, unused section
// 퀵슬롯의 연결된 아이템 정보를 갱신한다. (C->G)
Handler_CG_ITEM_IMPL(CG_ITEM_QUICK_UPDATE_LINKED_ITEM_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));

    const MSG_CG_ITEM_QUICK_UPDATE_LINKED_ITEM_CMD* update_cmd(NULL);    
    Player* requester(NULL);

    if (!util::internal::CheckRequestMessage(pMsg, wSize, update_cmd, requester))
    {
        return;
    }

    int temp_result_code = 0;
    if (!util::internal::CheckRequesterState(requester, __FUNCTION__, temp_result_code))
    {
        return;
    }

    QuickManager* const quick_slot_manager = requester->GetQuickManager();
    quick_slot_manager->UpdateLinkedItem(update_cmd->target_quick_slot_position, 
                                         update_cmd->new_linked_item_container_type, 
                                         update_cmd->new_linked_item_position);
}
#endif

Handler_CG_ITEM_IMPL( CG_ITEM_DELETE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_DELETE_SYN * pRecvMsg = (MSG_CG_ITEM_DELETE_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_DELETE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_DELETE_SYN ) )
		return;

	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->Delete( pRecvMsg->m_AtPos );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			MSG_CG_ITEM_DELETE_ACK msg;
			pPlayer->SendPacket( &msg, sizeof( msg ) );
		}
		break;
	default:
		{
			MSG_CG_ITEM_DELETE_NAK msg;
			ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
			if( !rPROTECT.IsUsable() )
				msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
			else
				msg.m_byErrorCode = rt;
			pPlayer->SendPacket(&msg,sizeof(msg));
		}
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_REPAIR_DURA_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_REPAIR_DURA_SYN* pRecvMsg = (MSG_CG_ITEM_REPAIR_DURA_SYN*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_REPAIR_DURA_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_REPAIR_DURA_SYN ) )
		return;

    RC::eITEM_RESULT rt = RC::RC_ITEM_FAILED;
	if( g_ExtraNPCManager.IsLengthCanUseNPC(pRecvMsg->m_NPCCode, pPlayer, eNPC_TYPE_STORE) )
	{
        if (nsSlot::DurabilityImpl* const impl = pPlayer->GetItemManager()->GetDurabilityImpl()) {
            rt = impl->Repair(pRecvMsg->m_bAllRepair, pRecvMsg->m_SlotIndex, pRecvMsg->m_SlotPos);
        };
	}
	else
	{
		rt = RC::RC_ITEM_LENGTH_CANNOT_USE_NPC;
	}

	if( rt == RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_REPAIR_DURA_ACK ackMsg;
		ackMsg.m_bAllRepair = pRecvMsg->m_bAllRepair;
		ackMsg.m_SlotIndex = pRecvMsg->m_SlotIndex;
		ackMsg.m_SlotPos = pRecvMsg->m_SlotPos;
		ackMsg.m_Money = pPlayer->GetMoney();
		pPlayer->SendPacket( &ackMsg, sizeof(ackMsg) );
	}
	else
	{
		MSG_CG_ITEM_REPAIR_DURA_NAK msg;
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			msg.m_byErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
		else
			msg.m_byErrorCode = rt;
		pPlayer->SendPacket(&msg,sizeof(msg));
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_PORTAL_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_PORTAL_SYN* pRecvMsg = (MSG_CG_ITEM_PORTAL_SYN*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_PORTAL_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_PORTAL_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionCoordinate().Portal(
        pRecvMsg->m_CoordPos, pRecvMsg->m_PortalPos);
	if( rt != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_USE_NAK msg;
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			msg.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
		else
			msg.m_ErrorCode = rt;
		pPlayer->SendPacket(&msg,sizeof(msg));
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_CLEAR_COORD_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_CLEAR_COORD_SYN* pRecvMsg = (MSG_CG_ITEM_CLEAR_COORD_SYN*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_CLEAR_COORD_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_CLEAR_COORD_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionCoordinate().ClearCoord(
        pRecvMsg->m_ClearPos, pRecvMsg->m_CoordPos);
	if( rt != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_USE_NAK msg;
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			msg.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
		else
			msg.m_ErrorCode = rt;
		pPlayer->SendPacket(&msg,sizeof(msg));
	}
}

//초기화 아이템 사용
Handler_CG_ITEM_IMPL( CG_ITEM_INIT_STAT_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_INIT_STAT_SYN* pRecvMsg = (MSG_CG_ITEM_INIT_STAT_SYN*)pMsg;
	
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_INIT_STAT_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_INIT_STAT_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionPlayerProperty().
        InitStatItemUse(pRecvMsg->m_ItemSlotPos, pRecvMsg->m_wDecreaseValue);
	if( rt == RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_INIT_STAT_ACK AckMsg;
		AckMsg.m_ItemSlotPos    = pRecvMsg->m_ItemSlotPos;
        AckMsg.m_wDecreaseValue = pRecvMsg->m_wDecreaseValue;
		pPlayer->SendPacket( &AckMsg, sizeof(AckMsg) );
	}
	else
	{
		MSG_CG_ITEM_INIT_STAT_NAK msg;
		ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
		if( !rPROTECT.IsUsable() )
			msg.m_ErrorCode = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
		else
			msg.m_ErrorCode = rt;
		pPlayer->SendPacket(&msg,sizeof(msg));
	}
}


Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_CHECK_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_CHECK_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_ITEM_INVENTORY_PWD_CHECK_SYN( (MSG_CG_ITEM_INVENTORY_PWD_CHECK_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_ITEM_INVENTORY_PWD_CHECK_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_SETTING_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_SETTING_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_ITEM_INVENTORY_PWD_SETTING_SYN( (MSG_CG_ITEM_INVENTORY_PWD_SETTING_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_ITEM_INVENTORY_PWD_SETTING_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}

Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_LOCKING_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_INVENTORY_PWD_LOCKING_SYN )] Not Exist GameField" );
		return;
	}

	if( g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD ) )
	{
		WarehouseInventoryProtector& rPROTECTOR = pPlayer->GetWarehouseInventoryProtector();
		rPROTECTOR.OnRecvCG_ITEM_INVENTORY_PWD_LOCKING_SYN( (MSG_CG_ITEM_INVENTORY_PWD_LOCKING_SYN*)pMsg, wSize );
	}
	else
	{
		MSG_CG_ITEM_INVENTORY_PWD_LOCKING_NAK msgNAK;
		pPlayer->SendPacket( &msgNAK, sizeof(msgNAK) );
	}
}



#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
Handler_CG_ITEM_IMPL(CG_ITEM_EXCHANGE_REWARD_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN* pRecvMsg = (MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_SYN*)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_EXCHANGE_REWARD_ITEM_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_EXCHANGE_REWARD_ITEM_SYN ) )
		return;

	MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_ACK AckMsg;
	RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetTradingImpl()->ExchangeRewardItem(
        pRecvMsg->m_ItemCode, pRecvMsg->m_Count, &AckMsg.m_ItemInfo );
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{
			pPlayer->SendPacket( &AckMsg, AckMsg.GetSize() );
		}
		break;
	default:
		{
			MSG_CG_ITEM_EXCHANGE_REWARD_ITEM_NAK NakMsg;
			NakMsg.m_ErrorCode = rt;
			pPlayer->SendPacket( &NakMsg, sizeof( NakMsg ) );
		}
	}
}
#endif

Handler_CG_ITEM_IMPL(CG_ITEM_VALIDATION_OF_REIHALT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_VALIDATION_OF_REIHALT_SYN* pRecvMsg = (MSG_CG_ITEM_VALIDATION_OF_REIHALT_SYN*)pMsg;
	
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_VALIDATION_OF_REIHALT_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_VALIDATION_OF_REIHALT_SYN ) )
		return;
	
    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionIdentity().IdentifyFateItem(
        pRecvMsg->m_TargetItemPos);
	
	switch( rt )
	{
	case RC::RC_ITEM_SUCCESS:
		{			
			MSG_CG_ITEM_VALIDATION_OF_REIHALT_ACK AckMsg;			
			AckMsg.m_TargetItemPos = pRecvMsg->m_TargetItemPos;			
			SCItemSlot & rSlot	= (SCItemSlot &)pPlayer->GetItemManager()->GetItemSlotContainer(SI_INVENTORY)->GetSlot( pRecvMsg->m_TargetItemPos );
			rSlot.CopyOut(AckMsg.m_TargetItemStream);
			pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ));
		}
		break;
	default:
		{
			MSG_CG_ITEM_VALIDATION_OF_REIHALT_NAK NakMsg;
			NakMsg.m_dwErrorCode = rt;
			pPlayer->SendPacket( &NakMsg, sizeof( NakMsg ));
		}
	}	
}

Handler_CG_ITEM_IMPL(CG_ITEM_USE_PET_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_USE_PET_ITEM_SYN * pRecvMsg = (MSG_CG_ITEM_USE_PET_ITEM_SYN *)pMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_USE_PET_ITEM_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_USE_PET_ITEM_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionPet().UsePetItem(
        pRecvMsg->m_PetItemPos);
	
	MSG_CG_ITEM_USE_PET_ITEM_ACK AckMeg;
	AckMeg.m_PetItemPos = pRecvMsg->m_PetItemPos;
	AckMeg.m_Result		= rt;
	pPlayer->SendPacket( &AckMeg, sizeof( AckMeg ));	
}

// 판도라 상자를 열때. (복권아이템을 사용하려 한다.)
Handler_CG_ITEM_IMPL(CG_ITEM_USE_PANDORA_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // CHANGES: f110614.3L, logic arrangement
    const MSG_CG_ITEM_USE_PANDORA_ITEM_SYN* const recv_msg =
        static_cast<MSG_CG_ITEM_USE_PANDORA_ITEM_SYN*>(pMsg);
	PLAYER_PTR_USERKEY(Player, player, recv_msg->m_dwKey);
    GameField* const game_field = player->GetField();
    GameZone* const game_zone = (game_field != NULL) ? game_field->GetGameZone() : NULL;
    if (game_field == NULL || game_zone == NULL) {
        return;
    };
    const sPOLICY_ZONE& zone_policy = game_zone->Policyment;
    if (zone_policy.FIELD & zone_policy.ENABLE_SSQ_CONTROL) {
        return;
    };
    if (!CheckProcessInventoryLock(player, CG_ITEM_USE_PANDORA_ITEM_SYN)) {
        return;
    };
    //----------------------------------------------------------------------------------------------
    struct FailResultHandler {
        FailResultHandler(Player* player)
            : player_(player), result_(RC::RC_ITEM_LOTTO_OPEN_SUCCESS) { }
        ~FailResultHandler()
        {
            if (this->result_ != RC::RC_ITEM_LOTTO_OPEN_SUCCESS)
            {
                MSG_CG_ITEM_USE_PANDORA_ITEM_NAK msg_nak;
                msg_nak.m_Result = this->result_;
                player_->SendPacket(&msg_nak, sizeof(msg_nak));
            };
        };
        Player* const player_;
        RC::eITEM_RESULT result_;
    } result_handler(player);
    //----------------------------------------------------------------------------------------------
    MSG_CG_ITEM_USE_PANDORA_ITEM_ACK msg_ack;
    ;{
        msg_ack.m_Identity = false;
        msg_ack.m_InventoryPos = recv_msg->m_InventoryPos;
        ZeroMemory(&msg_ack.m_PandoraItemStream, sizeof(msg_ack.m_PandoraItemStream));
    };
    if (ItemManager* item_manager = player->GetItemManager())
    {
        result_handler.result_ = item_manager->GetItemFunctionLottery().PandoraBoxOpen(\
            recv_msg->m_InventoryPos, &msg_ack.m_PandoraItemStream);
    };
    if (result_handler.result_ != RC::RC_ITEM_LOTTO_OPEN_SUCCESS) {
        return;
    };
    // success process
    msg_ack.m_Identity = true;
    player->SendPacket(&msg_ack, sizeof(msg_ack));
    return;
}

// 판도라 상자를 닫을 때.
Handler_CG_ITEM_IMPL(CG_ITEM_PANDORABOX_CLOSE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // CHANGES: f110614.3L, logic arrangement
    const MSG_CG_ITEM_PANDORABOX_CLOSE_SYN* const recv_msg =
        static_cast<MSG_CG_ITEM_PANDORABOX_CLOSE_SYN*>(pMsg);
	PLAYER_PTR_USERKEY(Player, player, recv_msg->m_dwKey);
    GameField* const game_field = player->GetField();
    GameZone* const game_zone = (game_field != NULL) ? game_field->GetGameZone() : NULL;
    if (game_field == NULL || game_zone == NULL) {
        return;
    };
    const sPOLICY_ZONE& zone_policy = game_zone->Policyment;
    if (zone_policy.FIELD & zone_policy.ENABLE_SSQ_CONTROL) {
        return;
    };
    if (!CheckProcessInventoryLock(player, CG_ITEM_USE_PANDORA_ITEM_SYN)) { // ...
        return;
    };
    //----------------------------------------------------------------------------------------------
    RC::eITEM_RESULT close_result = RC::RC_ITEM_FAILED;
    if (ItemManager* item_manager = player->GetItemManager())
    {
        close_result = item_manager->GetItemFunctionLottery().PandoraBoxClose(\
            recv_msg->m_InventoryPos);
    };
    if (close_result != RC::RC_ITEM_PANDORABOX_CLOSE_SUCCESS)
    {
        MSG_CG_ITEM_PANDORABOX_CLOSE_NAK msg_nak;
		msg_nak.m_Result = close_result;
		player->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // success process
    MSG_CG_ITEM_PANDORABOX_CLOSE_ACK msg_ack;
    player->SendPacket(&msg_ack, sizeof(msg_ack));
    return;
}

Handler_CG_ITEM_IMPL(CG_ITEM_MOVE_PANDORA_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_ITEM_MOVE_PANDORA_ITEM_SYN* recv_msg =
        static_cast<MSG_CG_ITEM_MOVE_PANDORA_ITEM_SYN*>(pMsg);
    PLAYER_PTR_USERKEY(Player, player, recv_msg->m_dwKey);
    GameField* const game_field = player->GetField();
    GameZone* const game_zone = (game_field != NULL) ? game_field->GetGameZone() : NULL;
    if (game_field == NULL || game_zone == NULL) {
        return;
    };
    const sPOLICY_ZONE& zone_policy = game_zone->Policyment;
    if (zone_policy.FIELD & zone_policy.ENABLE_SSQ_CONTROL) {
        return;
    };
    if (!CheckProcessInventoryLock(player, CG_ITEM_MOVE_PANDORA_ITEM_SYN)) {
        return;
    };
    //----------------------------------------------------------------------------------------------
    struct FailResultHandler {
        FailResultHandler(Player* player) : player_(player), result_(RC::RC_ITEM_SUCCESS) { }
        ~FailResultHandler()
        {
            if (this->result_ != RC::RC_ITEM_SUCCESS &&
                this->result_ != RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS)
            {
                MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK msg_nak;
                msg_nak.m_Result = this->result_;
                player_->SendPacket(&msg_nak, sizeof(msg_nak));
            };
        };
        Player* const player_;
        RC::eITEM_RESULT result_;
    } result_handler(player);
    //----------------------------------------------------------------------------------------------
    if (player->GetBehave() != PLAYER_BEHAVE_DIALOG_OPEN_STATE) {
        result_handler.result_ = RC::RC_ITEM_INVALIDSTATE;
        return;
    };
    //----------------------------------------------------------------------------------------------
    MSG_CG_ITEM_MOVE_PANDORA_ITEM_ACK msg_ack;
    //----------------------------------------------------------------------------------------------
    // lottery_item -> inventory, concretization
    nsSlot::ItemFunctionLottery::ConcretizationResult concretization_arg = { NULL, };
    concretization_arg.changed_lottery_item_stream = &msg_ack.m_PandoraItemStream;
    concretization_arg.total_info = &msg_ack.m_InventoryInfo;
    //
    ItemManager* const item_manager = player->GetItemManager();
    SCSlotContainer* const inventory_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    result_handler.result_ = item_manager->GetItemFunctionLottery().ExtractLotteryItems(
        false, recv_msg->m_InventoryPos, recv_msg->m_PandoraItemPosFrom, 
        inventory_container->GetEmptySlotNum(), &concretization_arg);
    //----------------------------------------------------------------------------------------------
    if ((result_handler.result_ != RC::RC_ITEM_SUCCESS) &&
        (result_handler.result_ != RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS))
    {
        return;
    };
    ;{
        msg_ack.m_Result = result_handler.result_;
        msg_ack.m_InventoryPos = recv_msg->m_InventoryPos;
        msg_ack.m_MoveInventory = 0; // unused;
        msg_ack.m_Remove = (concretization_arg.result_value &
                            concretization_arg.eResult_RemoveLotteryItem) != 0;
    };
    //
    player->SendPacket(&msg_ack, msg_ack.GetSize());

    return;
}


Handler_CG_ITEM_IMPL(CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN* recv_msg = 
        static_cast<MSG_CG_ITEM_MOVE_ALL_PANDORA_ITEM_SYN*>(pMsg);
    PLAYER_PTR_USERKEY(Player, player, recv_msg->m_dwKey);
    GameField* const game_field = player->GetField();
    GameZone* const game_zone = (game_field != NULL) ? game_field->GetGameZone() : NULL;
    if (game_field == NULL || game_zone == NULL) {
        return;
    };
    const sPOLICY_ZONE& zone_policy = game_zone->Policyment;
    if (zone_policy.FIELD & zone_policy.ENABLE_SSQ_CONTROL) {
        return;
    };
    if (!CheckProcessInventoryLock(player, CG_ITEM_MOVE_PANDORA_ITEM_SYN)) {
        return;
    };
    //----------------------------------------------------------------------------------------------
    struct FailResultHandler {
        FailResultHandler(Player* player) : player_(player), result_(RC::RC_ITEM_SUCCESS) { }
        ~FailResultHandler()
        {
            if (this->result_ != RC::RC_ITEM_SUCCESS &&
                this->result_ != RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS)
            {
                MSG_CG_ITEM_MOVE_PANDORA_ITEM_NAK msg_nak;
                msg_nak.m_Result = this->result_;
                player_->SendPacket(&msg_nak, sizeof(msg_nak));
            };
        };
        Player* player_;
        RC::eITEM_RESULT result_;
    } result_handler(player);
    //----------------------------------------------------------------------------------------------
    if (player->GetBehave() != PLAYER_BEHAVE_DIALOG_OPEN_STATE) {
        result_handler.result_ = RC::RC_ITEM_INVALIDSTATE;
        return;
    };
    //----------------------------------------------------------------------------------------------
    MSG_CG_ITEM_MOVE_PANDORA_ITEM_ACK msg_ack;
    //----------------------------------------------------------------------------------------------
    // lottery_item -> inventory, concretization
    nsSlot::ItemFunctionLottery::ConcretizationResult concretization_arg = { NULL, };
    concretization_arg.changed_lottery_item_stream = &msg_ack.m_PandoraItemStream;
    concretization_arg.total_info = &msg_ack.m_InventoryInfo;
    //
    ItemManager* const item_manager = player->GetItemManager();
    SCSlotContainer* const inventory_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    result_handler.result_ = item_manager->GetItemFunctionLottery().ExtractLotteryItems(
        true, recv_msg->m_InventoryPos, 0, inventory_container->GetEmptySlotNum(), &concretization_arg);
    //----------------------------------------------------------------------------------------------
    if ((result_handler.result_ != RC::RC_ITEM_SUCCESS) &&
        (result_handler.result_ != RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS))
    {        
        return;
    };
    ;{
        msg_ack.m_Result = result_handler.result_;
        msg_ack.m_InventoryPos = recv_msg->m_InventoryPos;
        msg_ack.m_MoveInventory = 0; // unused;
        msg_ack.m_Remove = (concretization_arg.result_value &
                            concretization_arg.eResult_RemoveLotteryItem) != 0;
    };
    //
    player->SendPacket(&msg_ack, msg_ack.GetSize());

    return;
}

Handler_CG_ITEM_IMPL(CG_ITEM_REPURCHASE_SYN)
{	
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_CG_ITEM_REPURCHASE_SYN, pRecvMsg, pMsg);
	PLAYER_PTR_USERKEY(Player, pPlayer, pRecvMsg->m_dwKey);

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_REPURCHASE_SYN )] Not Exist GameField" );
		return;
	}
	
	RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;

	if( g_ExtraNPCManager.IsLengthCanUseNPC(pRecvMsg->m_NPCCode, pPlayer, eNPC_TYPE_STORE) )
	{
		MSG_CG_ITEM_REPURCHASE_ACK msg;
		result = pPlayer->GetItemManager()->GetTradingImpl()->Repurchase(
            pRecvMsg->m_RepurchasePos, &msg.m_TotalInfo);
		if(RC::RC_ITEM_SUCCESS == result)
		{
			msg.m_Money = pPlayer->GetMoney();
			pPlayer->SendPacket( &msg, msg.GetSize() );
			return;
		}
	}
	
	ProtectorInventory& rPROTECT = pPlayer->GetWarehouseInventoryProtector().INVENTORY();
	
	MSG_CG_ITEM_REPURCHASE_NAK msg;
	msg.m_ErrorCode = (rPROTECT.IsUsable()) ? result : RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
	pPlayer->SendPacket(&msg,sizeof(msg));
	
}

Handler_CG_ITEM_IMPL(CG_ITEM_EQUIP_ETHER_DEVICE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_EQUIP_ETHER_DEVICE_SYN * pRecvMsg = (MSG_CG_ITEM_EQUIP_ETHER_DEVICE_SYN *)pMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_EQUIP_ETHER_DEVICE_SYN )] Not Exist GameField" );
		return;
	}

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_EQUIP_ETHER_DEVICE_SYN ) )
		return;

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionEtherWeapon().
        EquipEtherDeviceToWeapon(pRecvMsg->m_DeviceItemPos, pRecvMsg->m_WeaponItemPos);

	MSG_CG_ITEM_EQUIP_ETHER_DEVICE_ACK AckMsg;
	AckMsg.m_DeviceItemPos		= pRecvMsg->m_DeviceItemPos;
	AckMsg.m_WeaponItemPos		= pRecvMsg->m_WeaponItemPos;
	AckMsg.m_Result				= rt;
	pPlayer->SendPacket( &AckMsg, sizeof( AckMsg ));
}

Handler_CG_ITEM_IMPL(CG_ITEM_STAT_CHANGE_USE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_STAT_CHANGE_USE_SYN *pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_USE_SYN *)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );

	ASSERT( pPlayer );
	if( !pPlayer ) return;

	MSG_CG_ITEM_STAT_CHANGE_USE_ACK ack;
    DWORD result = pPlayer->GetItemManager()->GetItemFunctionItemLimitation().\
        ChangeItemStat_SelectItem(pRecvMsg->m_atIndex, pRecvMsg->m_atPos,
            pRecvMsg->m_ChangeItemPos, &ack);
	if( result != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_STAT_CHANGE_USE_NAK nack;
		nack.m_ErrorCode = result;
		pPlayer->SendPacket( &nack, sizeof( MSG_CG_ITEM_STAT_CHANGE_USE_NAK));
		return;
	}

	ack.m_atPos = pRecvMsg->m_atPos;
	ack.m_ChangeItemPos = pRecvMsg->m_ChangeItemPos;
	pPlayer->SendPacket( &ack, sizeof( MSG_CG_ITEM_STAT_CHANGE_USE_ACK));
}

Handler_CG_ITEM_IMPL(CG_ITEM_STAT_CHANGE_SELECT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_STAT_CHANGE_SELECT_SYN *pRecvMsg = (MSG_CG_ITEM_STAT_CHANGE_SELECT_SYN *)pMsg;
	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );

	ASSERT( pPlayer );
	if( !pPlayer ) return;

	MSG_CG_ITEM_STAT_CHANGE_SELECT_ACK msg;
    DWORD result = pPlayer->GetItemManager()->GetItemFunctionItemLimitation().ChangeItemStat(
        pRecvMsg->m_SelectItemCode, pRecvMsg->m_bSelect);
	if( RC::RC_ITEM_SUCCESS != result )
	{
		MSG_CG_ITEM_STAT_CHANGE_SELECT_NAK nack;
		nack.m_ErrorCode = result;
		pPlayer->SendPacket( &nack, sizeof( MSG_CG_ITEM_STAT_CHANGE_SELECT_NAK));
		return;
	}
	
	::ZeroMemory( &msg.m_InventoryInfo, sizeof( INVENTORY_TOTAL_INFO));
	pPlayer->GetItemManager()->SerializeInventoryTotalInfo( msg.m_InventoryInfo, SERIALIZE_LOAD);
	pPlayer->SendPacket(&msg, sizeof(MSG_CG_ITEM_STAT_CHANGE_SELECT_ACK));
}


Handler_CG_ITEM_IMPL(CG_ITEM_USE_MOTION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_USE_MOTION_SYN *pRecvMsg = (MSG_CG_ITEM_USE_MOTION_SYN *)pMsg;

	Player *pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	//.//

	ItemManager* pItemMng = pPlayer->GetItemManager();

	if( !pItemMng->ValidPos( pRecvMsg->m_atIndex, pRecvMsg->m_atPos, TRUE ) )
	{
		MSG_CG_ITEM_USE_MOTION_NAK nack;
		nack.m_byErrorCode = RC::RC_ITEM_INVALIDPOS;
		pPlayer->SendPacket( &nack, sizeof( MSG_CG_ITEM_USE_MOTION_NAK));
		return;
	}



	SCSlotContainer * pAtContainer = pItemMng->GetItemSlotContainer( pRecvMsg->m_atIndex );
	SCItemSlot & rAtSlot = (SCItemSlot &)pAtContainer->GetSlot( pRecvMsg->m_atPos );

	RC::eITEM_RESULT rcResult = pPlayer->CanUseItem( rAtSlot.GetItemInfo() );
	if( rcResult != RC::RC_ITEM_SUCCESS )
	{
		MSG_CG_ITEM_USE_MOTION_NAK nack;
		nack.m_byErrorCode = rcResult;
		pPlayer->SendPacket( &nack, sizeof( MSG_CG_ITEM_USE_MOTION_NAK));
		return;
	}

	//.//

	MSG_CG_ITEM_USE_MOTION_BRD brdMsg;
	brdMsg.m_dwAttackerKey   = pPlayer->GetObjectKey();
	brdMsg.m_ItemCode		 = rAtSlot.GetItemCode();
	brdMsg.m_wvMainTargetPos = pRecvMsg->m_wvMainTargetPos;

	pPlayer->SendPacket( &brdMsg, sizeof(brdMsg) );
	pPlayer->SendPacketAround( &brdMsg, sizeof(brdMsg), FALSE );


}

Handler_CG_ITEM_IMPL(CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN * pRecvMsg = (MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN *)pMsg;	
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_USE_CHANGE_PET_NAME_ITEM_SYN ) )
		return;
    
	//클라 안믿어.
	pRecvMsg->m_PetName[MAX_PET_NAME_LENGTH] = '\0';

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionPet().UseChangePetNameCard(
        pRecvMsg->m_ItemPos, pRecvMsg->m_PetName);

	MSG_CG_ITEM_USE_CHANGE_PET_NAME_ITEM_ACK AckMeg;	
	AckMeg.m_ResultCode	= rt;
	pPlayer->SendPacket( &AckMeg, sizeof( AckMeg ));	
}

Handler_CG_ITEM_IMPL( CG_ITEM_ACTIVE_ETHER_BULLET_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_ACTIVE_ETHER_BULLET_SYN * pRecvMsg = (MSG_CG_ITEM_ACTIVE_ETHER_BULLET_SYN *)pMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	MSG_CG_ITEM_ACTIVE_ETHER_BULLET_ACK		AckMsg;
	MSG_CG_ITEM_ETHER_ACTIVE_STATE_BRD		BrdMsg;

	AckMsg.m_ResultCode		= RC::RC_ITEM_SUCCESS;
	
	BrdMsg.m_dwPlayerKey	= pPlayer->GetObjectKey();
	BrdMsg.m_bActive		= pRecvMsg->m_bActive;

	//활성화
	if( pRecvMsg->m_bActive )
	{
		if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_ACTIVE_ETHER_BULLET_SYN ) )
			return;

		if( pPlayer->m_EtherManager.IsActive() )
		{
			AckMsg.m_ResultCode = RC::RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET;
			pPlayer->SendPacket( &AckMsg, sizeof(AckMsg));
			return;
		}
		
		if( !pPlayer->m_EtherManager.ActiveBulletItem( pRecvMsg->m_ActiveBulletItemPos ) )
		{
			AckMsg.m_ResultCode = RC::RC_ITEM_CAN_NOT_ACTIVE_ETHER_BULLET;
			pPlayer->SendPacket( &AckMsg, sizeof(AckMsg));
			return;
		}

		tagEtherBulletInfo* pBulletInfo = pPlayer->m_EtherManager.GetBulletOption();
		if( pBulletInfo )
		{
			AckMsg.m_BulletEffectCode = pBulletInfo->m_byEffectCode;
			BrdMsg.m_BulletEffectCode = pBulletInfo->m_byEffectCode;
		}
		else
		{
			ASSERT(FALSE);
			SUNLOG(eCRITICAL_LOG, "Not find BulletOption");
		}
	}
	//비활성화
	else
	{
		if( !pPlayer->m_EtherManager.IsActive() )
		{
			AckMsg.m_ResultCode = RC::RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET;
			pPlayer->SendPacket( &AckMsg, sizeof(AckMsg));
			return;
		}
		if( !pPlayer->m_EtherManager.InActiveBulletItem( pRecvMsg->m_ActiveBulletItemPos ) )
		{
			AckMsg.m_ResultCode = RC::RC_ITEM_CAN_NOT_INACTIVE_ETHER_BULLET;
			pPlayer->SendPacket( &AckMsg, sizeof(AckMsg));
			return;
		}
	}

	//SUCCESS	
	pPlayer->SendPacket( &AckMsg, sizeof(AckMsg));
    pPlayer->SendPacketAround( &BrdMsg, sizeof(BrdMsg), FALSE );
}

Handler_CG_ITEM_IMPL( CG_ITEM_IDENTIFY_POSSESSION_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_ITEM_IDENTIFY_POSSESSION_SYN * pRecvMsg = (MSG_CG_ITEM_IDENTIFY_POSSESSION_SYN *)pMsg;	
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return;

	if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_IDENTIFY_POSSESSION_SYN ) )
		return;

	MSG_CG_ITEM_IDENTIFY_POSSESSION_ACK AckMeg;	

    RC::eITEM_RESULT rt = pPlayer->GetItemManager()->GetItemFunctionIdentity().\
        IdentifyPossessionItem(pRecvMsg->m_ItemPos, AckMeg.m_PossessionItemStream);
	
	AckMeg.m_ResultCode	= rt;
	AckMeg.m_PossessionItemPos = pRecvMsg->m_ItemPos;
	
	
	pPlayer->SendPacket( &AckMeg, sizeof( AckMeg ));
}

//{__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
Handler_CG_ITEM_IMPL( CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // (WAVERIX) (090918) (BUG-FIX) 수퍼방장 아이템 사용 요청에 대한 처리가 성공/실패가 구분 없이
    // 이뤄지는 문제 수정
    MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN* pRecvMsg =
        static_cast<MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN *>(pMsg);

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    if(!FlowControl::FCAssert(pPlayer != 0))
        return;

    if( !CheckProcessInventoryLock( pPlayer, CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_SYN ) )
        return;

    ItemManager* pItemManager = pPlayer->GetItemManager();
    //if(!pItemManager) -> assert (pPlayer->GetItemManager() != 0); <- pre-aligned
    //  return;

    BOOLEAN success = true;
    RC::eITEM_RESULT rt = RC::RC_ITEM_SUCCESS;
    if(success && !pItemManager->ValidState())
    { success = false, rt = RC::RC_ITEM_INVALIDSTATE; }

    const POSTYPE requested_fireup_item_pos = pRecvMsg->m_FireUpItemPos;

    SCSlotContainer* pContainer = 0;
    if(success && !pItemManager->ValidPos(SI_INVENTORY, requested_fireup_item_pos))
    { success = false, rt =  RC::RC_ITEM_INVALIDPOS; }

    MSG_CG_ITEM_FIXED_AMOUNT_EX_ITEM_FIRE_UP_ACK msgACK;
    if(success == 0) {
        msgACK.m_ResultCode    = rt;
        msgACK.m_FireUpItemPos = requested_fireup_item_pos;
        pPlayer->SendPacket(&msgACK, sizeof(msgACK));
        return;
    }

    pContainer = pItemManager->GetItemSlotContainer(SI_INVENTORY);

    SCItemSlot& rFireUpItemSlot = (SCItemSlot&)pContainer->GetSlot(requested_fireup_item_pos);
    //if(success && !rFireUpItemSlot.GetItemInfo()) <- assert(rFireUpItemSlot.GetItemInfo() != 0)
    //{ success = false, rt = RC::RC_ITEM_NOINFO; }
    const BASE_ITEMINFO* pFireupItemInfo = rFireUpItemSlot.GetItemInfo();
    if(success && pFireupItemInfo->m_Code == 0)
    { success = false, rt = RC::RC_ITEM_NOINFO; }

    //확장 정액 타입인가
    if(success && pFireupItemInfo->m_ChargeSubType != eCHARGE_SUB_FIXED_AMOUNT_EX)
    { success = false, rt = RC::RC_ITEM_INVALID_TYPE; }

    if(success) {
        int slot_pos = 0;
        for(; slot_pos < MAX_INVENTORY_SLOT_NUM; ++slot_pos) {
            if(pContainer->IsEmpty(slot_pos))
                continue;
            if(slot_pos == requested_fireup_item_pos)
                continue;

            SCItemSlot& rItemSlot = (SCItemSlot &)pContainer->GetSlot(slot_pos);

            const BASE_ITEMINFO* pSlotItemInfo = rItemSlot.GetItemInfo();
            if(rItemSlot.GetSerial() != 0 && 
                pSlotItemInfo->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX &&
                pSlotItemInfo->m_wType == pFireupItemInfo->m_wType && 
                rItemSlot.IsProgressTime() )
            {
                break;
            }
        }

        if(success && slot_pos != MAX_INVENTORY_SLOT_NUM)
        { success = false, rt = RC::RC_ITEM_ALREADY_FIREUP_ITEM_SAME_TYPE; }
    }

    if(success) {
        assert(rt == RC::RC_ITEM_SUCCESS);
        if(success && !rFireUpItemSlot.FireUp_DateTime())
        { success = false, rt = RC::RC_ITEM_FAILED; }
    }

    if(success) {
        //(shogen)(090814) Log
        GAMELOG->LogItem( ITEM_CHARGE_FIRST_EQUIP, pPlayer, &rFireUpItemSlot );

        msgACK.m_ResultCode    = rt;
        msgACK.m_FireUpItemPos = requested_fireup_item_pos;
        pPlayer->SendPacket(&msgACK, sizeof(msgACK));

        static_cast<CInventorySlotContainer*>(pContainer)->OnUpdate(
            rFireUpItemSlot, rFireUpItemSlot.GetNum());

        pItemManager->SendItemChange(rFireUpItemSlot);

        return;
    }

    assert(success == false);
    // current status { fail }
    msgACK.m_ResultCode    = rt;
    msgACK.m_FireUpItemPos = requested_fireup_item_pos;
    pPlayer->SendPacket(&msgACK, sizeof(msgACK));
}
//}__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER

//==================================================================================================

Handler_CG_ITEM_IMPL(CG_ITEM_RIDER_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_ITEM_RIDER_CONTROL_SYN* pRecvMsg =
        static_cast<MSG_CG_ITEM_RIDER_CONTROL_SYN*>(pMsg);

    Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(!FlowControl::FCAssert(pPlayer != 0))
        return;

    MSG_CG_ITEM_RIDER_CONTROL_BRD msgBRD;
    msgBRD.Clear();

    const RiderControl& header = pRecvMsg->header;
    BOOLEAN success = header.request < header.eReq_Counts;
    if(success == 0) {
        msgBRD.SetUnknownFailed();
        pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
        return;
    }

    //버그수정:5799 문상현 2010-02-03 모든국가공통 인벤락걸린상타에서 소환물 타고 있을때 공격못하는 오류 수정
    if(header.eReq_SummonBattleRelease != header.request) //전투를 위한 소환해제는 인벤락을 무시한다.
    {
        success = !!CheckProcessInventoryLock(pPlayer, CG_ITEM_RIDER_CONTROL_SYN);
        if(success == 0) {
            IPlayerRiderD::SetDefaultFailResult(header, &msgBRD.header, &msgBRD.data);
            pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
            return;
        }
    }
    

    // 트랜잭션이나 이미 소환되어 있는 상태 고려할 것.
    switch(header.request)
    {
    case header.eReq_SummonStart:
    case header.eReq_SummonCancel:
    case header.eReq_Summon:
    case header.eReq_SummonRelease:
    case header.eReq_SummonBattleRelease:
    //case header.eReq_WaxGloss:
        { // accept fields
        }
        break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "0|Can't Accept Request|%s|%d|",
                   pPlayer->GetCharName(), header.request);
            msgBRD.SetUnknownFailed();
            pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
            return;
        }
        break;
    }

    assert(success != 0);
    if(pPlayer->Rider.IsValidState() == 0)
        pPlayer->Rider.Init(pPlayer);

    PlayerRider& rider = pPlayer->Rider.Get();
    PlayerRider::eResult result = rider.Route(header, &msgBRD.header, &msgBRD.data);

    if(result == rider.eResult_1toN)
        pPlayer->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
    else
        pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
}

Handler_CG_ITEM_IMPL(CG_ITEM_RIDER_CONTROL_PARTS_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN* pRecvMsg =
        static_cast<MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN*>(pMsg);

    Player* const pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    if(!FlowControl::FCAssert(pPlayer != 0))
        return;

    MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD msgBRD;
    msgBRD.Clear();

    const RiderControl& header = pRecvMsg->header;
    BOOLEAN success = header.request < header.eReq_Counts;
    if(success == 0) {
        msgBRD.SetUnknownFailed();
        pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
        return;
    }

    success = !!CheckProcessInventoryLock(pPlayer, CG_ITEM_RIDER_CONTROL_PARTS_SYN);
    if(success == 0) {
        IPlayerRiderD::SetDefaultFailResult(header, &msgBRD.header, &msgBRD.data);
        pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
        return;
    }

    // 트랜잭션이나 이미 소환되어 있는 상태 고려할 것.
    assert(success != 0);
    PlayerRider::eResult result = PlayerRider::eResult_Fail;
    switch(header.request)
    {
    case header.eReq_EquipParts:
    case header.eReq_UnEquipParts:
        {
            if(pPlayer->Rider.IsValidState() == 0)
                pPlayer->Rider.Init(pPlayer);

            PlayerRider& rider = pPlayer->Rider.Get();
            result = rider.Route(header, &msgBRD.header, &msgBRD.data);
        }
        break;
    case header.eReq_WaxGloss: // < 
        { // accept fields
            result = PlayerRider::WaxGloss(header, &msgBRD.header, &msgBRD.data, pPlayer);
        }
        break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "0|Can't Accept Request|%s|%d|",
                pPlayer->GetCharName(), header.request);
            msgBRD.SetUnknownFailed();
            result = PlayerRider::eResult_Fail;
        }
        break;
    }

    if(result == PlayerRider::eResult_1toN)
        pPlayer->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
    else
        pPlayer->SendPacket(&msgBRD, msgBRD.GetSize());
}

//_NA_0_20091030_RANDOM_ITEM
Handler_CG_ITEM_IMPL(CG_ITEM_USE_RANDOM_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    RandomItemImplement Implement;
    Implement.DoMessageImplement( pMsg );
}

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
Handler_CG_ITEM_IMPL(CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    AccumulatePointImplement Implement;
    Implement.DoMessageImplement( pMsg );
}

Handler_CG_ITEM_IMPL(CG_ITEM_USE_ACCUMULATE_POINT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    AccumulatePointImplement Implement;
    Implement.DoMessageImplement( pMsg );
}
#endif

//_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
Handler_CG_ITEM_IMPL(CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // (NOTE) (f100331.2L)
    const MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN* recv_msg = \
        static_cast<MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_SYN*>(pMsg);
    if (sizeof(*recv_msg) != wSize) { // pass
        assert(sizeof(*recv_msg) != wSize);
        return;
    }
    //
    const DWORD user_key = recv_msg->m_dwKey;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (player == NULL || player->GetField() == NULL) { // low level
        return;
    }
    //
    MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK msg_ack;
    // reply to client whatever the result is succeeds or fails.
    bool succeed = player->UpdateInventoryConfig(recv_msg->changed);
    __UNUSED(succeed);
    msg_ack.changed = player->GetCharInfo()->m_InventoryConfig;
    msg_ack.result = RC::RC_ITEM_SUCCESS;
    player->SendPacket(&msg_ack, sizeof(msg_ack));
}

//_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
Handler_CG_ITEM_IMPL(CG_ITEM_ETHERIA_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const MSG_CG_ITEM_ETHERIA_CONTROL_SYN* recv_msg = \
        static_cast<MSG_CG_ITEM_ETHERIA_CONTROL_SYN*>(pMsg);
    if (sizeof(*recv_msg) != wSize) { // pass
        assert(sizeof(*recv_msg) != wSize);
        return;
    }
    //
    const DWORD user_key = recv_msg->m_dwKey;
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (player == NULL || player->GetField() == NULL) { // low level
        return;
    }
    //
    MSG_CG_ITEM_ETHERIA_CONTROL_ACK msg_ack;
    const EtheriaControl& header = recv_msg->header;
    bool success = (header.request < header.eReq_Counts);
    if (success == false)
    {
        msg_ack.SetUnknownFailed();
        player->SendPacket(&msg_ack, msg_ack.GetSize());
        return;
    }
    //
    nsSlot::EtheriaFailHandler etheria_fail_handler(&msg_ack.header, &msg_ack.data, header, player);
    //
    success = CheckProcessInventoryLock(player, CG_ITEM_ETHERIA_CONTROL_SYN) != false;
    if (success == false)
    {
        etheria_fail_handler.SetFailResult(msg_ack.header.eRet_ItemFailed,
                                           RC::RC_ITEM_INVENTORY_NEED_PASSWORD, false);
        player->SendPacket(&msg_ack, msg_ack.GetSize());
        return;
    }
    //
    assert(success != 0);
    nsSlot::EtheriaImpl etherealizer;
    player->SetupEtherealizer(&etherealizer);
    nsSlot::EtheriaImpl::eResult notify_result = etherealizer.eResult_Fail;
    switch (header.request)
    {
    case header.eReq_CastingStart:
    case header.eReq_CastingCancel:
        notify_result = etherealizer.Casting(header, &msg_ack.header, &msg_ack.data);
        break;
    case header.eReq_Extract:
        notify_result = etherealizer.ExtractToEtheria(header, &msg_ack.header, &msg_ack.data);
        break;
    case header.eReq_Combine:
        notify_result = etherealizer.CombineWithEtheria(header, &msg_ack.header, &msg_ack.data);
        break;
    default:
        success = false;
        etheria_fail_handler.SetFailResult(msg_ack.header.eRet_Failed, 0, false);
        break;
    }
    //
    if (success == false) {
        etheria_fail_handler.OutputLog();
    }
    if (notify_result != etherealizer.eResult_1toN) {
        player->SendPacket(&msg_ack, msg_ack.GetSize());
    }
    else {
        MSG_CG_ITEM_ETHERIA_CONTROL_BRD msg_brd;
        msg_brd.header = msg_ack.header;
        msg_brd.data = msg_ack.data;
        player->SendPacketAround(&msg_brd, msg_brd.GetSize());
    }
}

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
Handler_CG_ITEM_IMPL(CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN) // 캐릭터 외모 변경 아이템 사용 요청을 처리한다.
{
    __TOUCHED((pServerSession, pMsg, wSize));
    
    MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN* const change_syn = 
        static_cast<MSG_CG_ITEM_CHANGE_CHARACTER_APPEARANCE_SYN* const>(pMsg);

    // 패킷 크기를 검사한다.
    if (sizeof(*change_syn) != wSize) 
    {
        assert(sizeof(*change_syn) != wSize);
        return;
    }

    // 플레이어를 검사한다.
    const PlayerManager* const player_manager = PlayerManager::Instance();
    Player* const player = player_manager->FindPlayerByUserKey(change_syn->m_dwKey);
    {
        if (player == NULL)
        {
            return;
        }

        if (player->GetField() == NULL)
        {
            return;
        }
    }

    // 캐릭터 외모 변경 아이템을 사용한다.
    ItemManager* const item_manager = player->GetItemManager();
    const RC::eITEM_RESULT use_result =
        item_manager->GetItemFunctionCustomization().UseChangeCharacterAppearanceItem(\
            change_syn->item_position_, change_syn->character_appearance_);

    // 로그를 기록한다.
    const eSUN_LOG_LEVEL log_type = RC::ItemResultUtil::IsSuccess(use_result) ?
        eFULL_LOG : eCRITICAL_LOG;
    ::MessageOut(log_type, 
                 _T("|%s|Result:%d|PlayerObjectKey:%u|PlayerCharName:%s|"), 
                 __FUNCTION__, 
                 use_result, 
                 player->GetObjectKey(), 
                 player->GetCharName());
            
    // 캐릭터 외모 변경 결과를 응답한다.
    MSG_CG_CHARINFO_CHANGE_APPEARANCE_BRD change_brd;
    {
        change_brd.player_key_ = player->GetObjectKey();
        change_brd.result_ = RC::ItemResultUtil::EnumToValue(use_result);
        change_brd.character_appearance_ = change_syn->character_appearance_;
        change_brd.item_position_ = change_syn->item_position_;
        player->SendPacketAround(&change_brd, change_brd.GetSize());
    }
}

Handler_CG_ITEM_IMPL(CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN) // 아이템 합성 또는 분해 요청을 처리한다.
{
    __UNUSED(pServerSession);
    ComposeOrDecomposeImplement implement;
    implement.HandleComposeOrDecompose(pMsg, wSize);
}

// 성공한 합성 또는 분해 내역 요청을 처리한다.
Handler_CG_ITEM_IMPL(CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN) 
{
    __UNUSED(pServerSession);
#ifdef _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
#else
    ComposeOrDecomposeImplement implement;
    implement.HandleGetHistories(pMsg, wSize);
#endif // _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
}

// 합성 또는 분해 내역 수정 요청을 처리한다.
Handler_CG_ITEM_IMPL(CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD) 
{
    __UNUSED(pServerSession);
    ComposeOrDecomposeImplement implement;
    implement.HandleModifyHistories(pMsg, wSize);
}

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
Handler_CG_ITEM_IMPL(CG_ITEM_IDENTIFY_SOCKET_CMD) // 아이템 소켓 확인 요청을 처리한다.
{
    __UNUSED(pServerSession);
    SocketSystemImplement implement;
    implement.HandleIdentifySocketMessage(pMsg, wSize);
}

Handler_CG_ITEM_IMPL(CG_ITEM_RECREATE_SOCKET_SYN) // 소켓 다시 생성 요청을 처리한다.
{
    __UNUSED(pServerSession);
    SocketSystemImplement impl;
    impl.HandleRecreateSocket(pMsg, wSize);
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
Handler_CG_ITEM_IMPL(CG_ITEM_USE_EXTEND_DATE_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    UseExtendDateItemImplement implement;
    implement.Implement((MSG_CG_ITEM_USE_EXTEND_DATE_SYN *)pMsg);
}
#endif

// _NA_003740_20111122_RANDOM_ITEM
Handler_CG_ITEM_IMPL(CG_ITEM_RANDOMIZE_SYN) 
{
    __UNUSED(pServerSession);
#ifdef _NA_003740_20111122_RANDOM_ITEM
    using namespace util::internal;

    const MSG_CG_ITEM_RANDOMIZE_SYN* request(NULL);
    Player* requester(NULL);
    if (CheckRequestMessage(pMsg, wSize, request, requester) == false)
    {
        return;
    }

    struct ErrorSender 
    {
        explicit ErrorSender(Player& requester) : requester_(requester), result(RC::RC_ITEM_SUCCESS)
        {
        }
        ~ErrorSender()
        {
            if (result != RC::RC_ITEM_SUCCESS)
            {
                MSG_CG_ITEM_USE_NAK response;
                response.m_ErrorCode = static_cast<BYTE>(result);
                requester_.SendPacket(&response, sizeof(response));
            }
        }
        int result;
    private:
        Player& requester_;
    };
    ErrorSender error_sender(*requester);

    if (CheckRequesterState(requester, __FUNCTION__, error_sender.result) == false)
    {
        return;
    }

    ItemManager* const item_manager = requester->GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    const RandomItemParser* const parser = RandomItemParser::Instance();
    
    SCItemSlot* request_item(NULL);
    SCItemSlot* target_item(NULL);
    if (parser->EnableItemRandomize(
        *inventory, 
        request->request_item_pos, 
        request->target_item_pos, 
        request_item, target_item) == false)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|아이템을 사용한 랜덤화가 불가능 합니다.|RequestItemPos = %d, TargetItemPos = %d, 캐릭터이름 = %s|"), 
            __FUNCTION__, 
            request->request_item_pos, 
            request->target_item_pos, 
            requester->GetCharName()
        );
        error_sender.result = RC::RC_ITEM_INVALID_VALUE;
        return;
    }

    const RandomValueTypeGrade::Value type_grade = 
        static_cast<RandomValueTypeGrade::Value>(request_item->GetItemInfo()->m_wSubType);
    if (target_item->Randomize(type_grade) == false)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|아이템 랜덤화에 실패 했습니다.|ItemCode = %d, 캐릭터이름 = %s|"), 
            __FUNCTION__, 
            target_item->GetItemInfo()->m_Code, 
            requester->GetCharName()
            );
        error_sender.result = RC::RC_ITEM_INVALID_VALUE;
        return;
    }
    item_manager->SendItemChange(*target_item);

    // 로그를 기록한다.
    {
        TCHAR add_info[MAX_ADDINFO_SIZE] = { 0 };
        target_item->RandomOptionToString<_countof(add_info)>(type_grade, add_info);
        GAMELOG->LogItem(ITEM_RANDOMIZE_OPTION, requester, target_item, 0, 1, NULL, NULL, add_info);
    }

    const RC::eITEM_RESULT using_result = item_manager->UseItemByType(
        eITEMTYPE_RANDOMIZE_ITEM, SI_INVENTORY, request->request_item_pos, true, __FUNCTION__);
    if (using_result != RC::RC_ITEM_SUCCESS)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|랜덤화 아이템 사용에 실패 했습니다.|UsingResult = %d, ItemCode = %d, 캐릭터이름 = %s|"), 
            __FUNCTION__, 
            using_result, 
            request_item->GetItemInfo()->m_Code, 
            requester->GetCharName()
        );
        error_sender.result = using_result;
        return;
    }

    MSG_CG_ITEM_RANDOMIZE_ACK response;
    response.Init();
    response.request_item_pos = request->request_item_pos;
    response.target_item_pos = request->target_item_pos;
    requester->SendPacket(&response, response.GetSize());
#endif // _NA_003740_20111122_RANDOM_ITEM
}

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
Handler_CG_ITEM_IMPL( CG_ITEM_AWAKENING_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_ITEM_AWAKENING_SYN * pRecvMsg = (MSG_CG_ITEM_AWAKENING_SYN *)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer ) return ;
    if( !pPlayer->GetField() )
    {
        return;
    }

    MSG_CG_ITEM_AWAKENING_ACK msg;
    if (!CheckProcessInventoryLock(pPlayer, CG_ITEM_AWAKENING_SYN))
    {
        return;
    }			
    msg.result_code = pPlayer->GetItemManager()->GetItemFunctionAwakening().Awakening(
        pRecvMsg->equip_item_pos, pRecvMsg->material_item_pos, pRecvMsg->additive_item_pos);

    pPlayer->SendPacket(&msg,sizeof(msg));

}

Handler_CG_ITEM_IMPL( CG_ITEM_EVOLUTION_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_ITEM_EVOLUTION_SYN * pRecvMsg = (MSG_CG_ITEM_EVOLUTION_SYN *)pMsg;

    Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer ) return ;
    if( !pPlayer->GetField() )
    {
        return;
    }

    MSG_CG_ITEM_EVOLUTION_ACK msg;
    if (!CheckProcessInventoryLock(pPlayer, CG_ITEM_EVOLUTION_SYN))
    {
        return;
    }			
    msg.result_code = pPlayer->GetItemManager()->GetItemFunctionEvolution().Evolution(
        pRecvMsg->equip_item_pos, pRecvMsg->additive_item_pos);

    pPlayer->SendPacket(&msg, sizeof(msg));

}
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
Handler_CG_ITEM_IMPL( CG_ITEM_SPA_COSTUME_CHANGE_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_ITEM_SPA_COSTUME_CHANGE_SYN* syn_msg = static_cast<MSG_CG_ITEM_SPA_COSTUME_CHANGE_SYN*>(pMsg);

    Player * player = PlayerManager::Instance()->FindPlayerByUserKey( syn_msg->m_dwKey );
    ASSERT( player );
    if( !player ) return ;
    if( !player->GetField() )
    {
        return;
    }

    if (!CheckProcessInventoryLock(player, CG_ITEM_SPA_COSTUME_CHANGE_SYN)) {
        return;
    }	

    RC::eITEM_RESULT result = 
            player->GetPlayerSpaInfo()->SetSpaInfo(syn_msg->slotcontainer_type, syn_msg->item_pos);

    if (result == RC::RC_ITEM_SUCCESS)
    {
        MSG_CG_ITEM_SPA_COSTUME_CHANGE_BRD brd_msg;
        brd_msg.spa_ticket_code = player->GetPlayerSpaInfo()->GetSpaTicketCode();
        brd_msg.player_object_key = player->GetObjectKey();
        player->SendPacketAround(&brd_msg, sizeof(brd_msg));
    }
    else
    {
        MSG_CG_ITEM_SPA_COSTUME_CHANGE_NAK nak_msg;
        nak_msg.error_code = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM