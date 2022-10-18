#include <stdafx.h>
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "PointWalletManager.h"
#include "PacketStruct_PointSystem.h"
#include <PointSystem/PacketStruct_CG_PointSystem.h>
#include <ItemManager.h>
#include <SCItemSlotContainer.h>
#include <CurrencyInfoParser.h>
#include <GameGuild.h>
#include <GameGuildManager.h>
#include <GuildRankInfoParser.h>
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA
#include <GuildInfoParser.h>
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA

PointWalletManager::PointWalletManager()
{

}

PointWalletManager::~PointWalletManager()
{

}

bool PointWalletManager::init( Player* player )
{
    player_ = player;
    playerinfo_ = player->GetCharInfo();

    Initialize();

    return true;
}

void PointWalletManager::Initialize()
{
    point_wallet_.clear();
}

inline void PointWalletManager::SetPointWallet( DWORD _code, INT _point )
{
    PointSystem::CodeNode* code_node = point_wallet_.find(_code);
    if (code_node == NULL) {
        code_node = point_wallet_.insert(_code);
    }

    if (code_node) {
        code_node->SetValue(_point);
    }
}

inline INT PointWalletManager::GetPointWallet( DWORD _code ) const
{
    PointSystem::CodeNode* code_node = point_wallet_.find(_code);
    return (code_node == NULL ? 0 : code_node->GetValue());
}

INT PointWalletManager::ChangeWalletPoint( DWORD _code, INT _point )
{
    //if(!point_wallet_system_->GetWalletPointInfo(_code))
    //    return 0;

    INT player_point = GetPointWallet(_code);
    player_point += _point;

    SetPointWallet(_code, player_point);

    ;{
        MSG_DG_POINTWALLET_CHANGE_SYN dg_msg(_code, player_point);
        player_->SendToGameDBPServer(&dg_msg, sizeof(dg_msg));

        MSG_CG_POINTWALLET_CHANGE_CMD cg_msg(_code, player_point);
        player_->SendPacket(&cg_msg, sizeof(cg_msg));
    };

    return player_point;
}


void PointWalletManager::ParseCGPacket( PVOID packet )
{
    MSG_CG_POINTWALLET_PACKET* packet2 = \
        static_cast<MSG_CG_POINTWALLET_PACKET*>(packet);

    if((player_->GetUserKey() != packet2->m_dwKey) || !player_->GetField())
        return;

    switch(packet2->packet_type)
    {
        case CG_POINTWALLET_CHANGE_WITH_COUNT_SYN:
        {
            ProcessPacketUseItem(packet);
        } break;

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        case CG_POINTWALLET_GUILDCOIN_DONATION_SYN:
        {
            MSG_CG_POINTWALLET_GUILDCOIN_DONATION_SYN* packet3 = \
                static_cast<MSG_CG_POINTWALLET_GUILDCOIN_DONATION_SYN*>(packet2);

            GuildPointDonation(packet3->point_);
        } break;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
        default:
            break;
    }
}

void PointWalletManager::ParseDGPacket( PVOID packet )
{
    MSG_DG_CHARINFO_POINTWALLET_SYSTEM* packet2 = \
        static_cast<MSG_DG_CHARINFO_POINTWALLET_SYSTEM*>(packet);

    switch(packet2->packet_type)
    {
        case DG_POINTWALLET_SELECT_ACK:
        {
            MSG_DG_POINTWALLET_SELECT_ACK* select_ack = \
                static_cast<MSG_DG_POINTWALLET_SELECT_ACK*>(packet);

            Initialize();
            
            for(int i = 0; i < select_ack->count_; ++i)
            {
                DWORD code  = (DWORD)select_ack->pointwallet_data[i].code_;
                INT   point = (INT)select_ack->pointwallet_data[i].point_;

                SetPointWallet(code, point);
            }

            ;{
                MSG_CG_POINTWALLET_SELECT_CMD msg;
                msg.count_ = select_ack->count_;
                memcpy(msg.pointwallet_data, select_ack->pointwallet_data, \
                    sizeof(select_ack->pointwallet_data[0]) * select_ack->count_);
                player_->SendPacket(&msg, msg.GetSize());
            };
            
        } break;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        case DG_POINTWALLET_GUILDPOINT_DONATION_ACK: 
        {
            MSG_DG_GUILDPOINT_DONATION_ACK* packet2 = \
                static_cast<MSG_DG_GUILDPOINT_DONATION_ACK*>(packet);
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA
            INT max_donation_count = GuildInfoParser::Instance()->GetGuildInfo().max_guild_coin_donation_;
#else
            INT max_donation_count = 5;
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA

            ProcessGuildCoinDonation( 
                packet2->donation_count_of_day_, 
                packet2->donation_item_count_,
                max_donation_count);
        } break;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
    }
}

void PointWalletManager::ProcessPacketUseItem( PVOID packet )
{
    MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN* recv_msg = \
        static_cast<MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_SYN*>(packet);

    SLOTIDX slot_index = recv_msg->slot_index_;
    POSTYPE position = recv_msg->item_position_;
    WORD item_count = recv_msg->item_count_;
    SCItemSlot rAtSlotOut;
    RC::eITEM_RESULT rt = UseItem( slot_index, position, item_count, rAtSlotOut );

    MSG_CG_POINTWALLET_CHANGE_WITH_COUNT_ACK msg(rt);
    if (rt == RC::RC_ITEM_SUCCESS) {
        msg.slot_index_ = rAtSlotOut.GetSlotIdx();
        msg.item_position_ = rAtSlotOut.GetPos();
        msg.item_count_ = rAtSlotOut.GetNum();
    }
    player_->SendPacket(&msg, sizeof(msg));
}

RC::eITEM_RESULT
PointWalletManager::UseItem(SLOTIDX slot_index, POSTYPE position, INT count, \
                            SCItemSlot& OUT rAtSlotOut)
{
    ItemManager* const item_manager = player_->GetItemManager();
    if(!item_manager->ValidState())
        return RC::RC_ITEM_INVALIDSTATE;

    if(!item_manager->ValidPos(slot_index, position, TRUE))
        return RC::RC_ITEM_INVALIDPOS;

    if (slot_index != SI_INVENTORY)
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& rAtSlot = (SCItemSlot&)inventory->GetSlot(position);
    const BASE_ITEMINFO* const pInfo = rAtSlot.GetItemInfo();

    RC::eITEM_RESULT rcResult = player_->CanUseItem(pInfo);
    if(rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    // 아이템 계열에 따른 처리
    if (pInfo->m_wType != eITEMTYPE_BILL)
        return RC::RC_ITEM_INVALIDSTATE;

    if(rAtSlot.GetNum() < count || count < 1)
        return RC::RC_ITEM_FAILED;

    ;{
        // BASE_ITEMINFO::m_MaterialCode 에 변환할 포인트코드가 설정되어있다.
        // BASE_ITEMINFO::m_wHealHP      에 변환할 포인트량이   설정되어있다.
        DWORD wallet_code  = (DWORD)pInfo->m_MaterialCode;
        INT   wallet_point = (INT)pInfo->m_wHealHP;
        INT   exchange_point = wallet_point * count;

        CurrencyInfoParser* const currency_info_parser = CurrencyInfoParser::Instance();
        const CurrencyInfoParser::CurrencyInfo* currency_info = \
            currency_info_parser->GetCurrencyInfo(wallet_code);

        if(!currency_info)
            return RC::RC_ITEM_FAILED;

        PointWalletManager* const pointwallet_manager = player_->GetPointWalletManager();
        INT previous_point = pointwallet_manager->GetPointWallet(wallet_code);

        if((INT)currency_info->currency_max_point < previous_point + exchange_point)
            return RC::RC_ITEM_FAILED_TO_EXCHANGE_CURRENCY_ITEM_BY_LIMIT;

        INT result_point = pointwallet_manager->ChangeWalletPoint(wallet_code, exchange_point);

        GAMELOG->LogPointWallet(player_, POINTWALLET_DEPOSIT_BY_USING_ITEMS, \
            wallet_code, previous_point, exchange_point, result_point);

        GAMELOG->LogItem(ITEM_USE, player_, &rAtSlot, exchange_point, count);

        inventory->UpdateSlot(rAtSlot.GetPos(), -count);
        ;{
            rAtSlotOut = rAtSlot;
            if(rAtSlot.GetNum() < 1) {
                inventory->DeleteSlot(position, NULL);
            }
        };
    };

    return RC::RC_ITEM_SUCCESS;
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
void PointWalletManager::GuildPointDonation( INT donation_item_count )
{
    if (donation_item_count < 1)
        return;

    GUILDGUID guid = player_->GetGuildGuid();
    GameGuild* guild = g_GameGuildManager.FindGuild(guid);

    if (INVALID_GUILDGUID == guid || guild == NULL) 
    {
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_NOT_GUILD_MEMBER;
        player_->SendPacket(&msg, sizeof(msg));
        return;
    }

    BYTE guild_level = guild->GetGuildGrade();
    if (guild_level == GuildRankInfoParser::Instance()->GetGuildMaxGrade())
    {
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_EXCEED_MAX_LEVEL;
        player_->SendPacket(&msg, sizeof(msg));
        return;
    }

    INT guild_coin = GetPointWallet(CURRENCY_INFO_GUILD_COIN);
    if (guild_coin < donation_item_count) 
    {
        // not enough point...
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_NOT_ENOUGH_COIN;
        player_->SendPacket(&msg, sizeof(msg));
        return;
    }

    MSG_DG_GUILDPOINT_DONATION_SYN msg;
    msg.guild_guid_ = player_->GetGuildGuid();
    msg.guild_point_code_ = CURRENCY_INFO_GUILD_COIN;
    msg.donation_item_count_ = donation_item_count; 
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA
    msg.max_guild_coin_donation_ = GuildInfoParser::Instance()->GetGuildInfo().max_guild_coin_donation_;
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA
    player_->SendToGameDBPServer(&msg, sizeof(msg));
}

void PointWalletManager::ProcessGuildCoinDonation( \
    INT donation_count_of_day, INT donation_item_count, INT max_donation_count)
{
    if (donation_count_of_day < 0 /*|| donation_item_count < 0*/)
        return;

    if (donation_count_of_day + donation_item_count > max_donation_count)
    {
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_DONATION_COUNT_OVER;
        msg.today_donation_count_ = donation_count_of_day;
        player_->SendPacket(&msg, sizeof(msg));
        return;
    }

    if (donation_item_count > GetPointWallet(CURRENCY_INFO_GUILD_COIN))
    {
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_NOT_ENOUGH_COIN;
        player_->SendPacket(&msg, sizeof(msg));
        return;
    }

    ChangeWalletPoint(CURRENCY_INFO_GUILD_COIN, -donation_item_count); //포인트 차감
    {
        MSG_CG_POINTWALLET_GUILDCOIN_DONATION_ACK msg;
        msg.result_code_ = RC::RC_ITEM_GUILDCOIN_DONATION_SUCCESS;
        msg.today_donation_count_ = donation_count_of_day + donation_item_count;
        player_->SendPacket(&msg, sizeof(msg));
    };
    
    ChangeWalletPoint(CURRENCY_INFO_DONATION_GUILDCOIN, donation_item_count); // 기부포인트 누적
    {
        MSG_GZ_GUILDPOINT_CHANGE_CMD dg_msg( \
            player_->GetGuildGuid(), player_->GetCharGuid(), \
            CURRENCY_INFO_DONATION_GUILDCOIN, donation_item_count);
        player_->SendToGameDBPServer(&dg_msg, sizeof(dg_msg));

#ifdef _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
        GAMELOG->LogGuild(ACT_GUILD_DONATION_GUILDCOIN, player_, donation_item_count);
#endif //_NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
    };
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM