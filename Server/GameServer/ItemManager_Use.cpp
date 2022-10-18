#include "stdafx.h"
#include "./ItemManager.h"

#include "SkillInfoParser.h"

#include "InventorySlotContainer.h"
#include "GameZone.h"
#include "GameField.h"

#include "Monster.h"

#include "AchievementManager.h"
#include "CurrencyInfoParser.h"
#include "StatusManager.h"
//_NA_004035_20120227_COSTUME_ITEM
#include "Player.h"

#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
#include "AbilityStatus.h"
#endif //_NA_006689_20130503_CANNOTUSE_BUFFITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
#include "MiniGame.h"
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

bool ItemManager::IsExtraPacketUse(
    const SLOTIDX item_slot_type, const POSTYPE item_slot_position) const
{
    if (ValidPos(item_slot_type, item_slot_position, true) == false)
    {
        return false;
    }
    
    const SCSlotContainer* const item_slot_contaioner = GetItemSlotContainer(item_slot_type);
    const SCItemSlot* const item_slot = 
        static_cast<const SCItemSlot*>(item_slot_contaioner->GetValidSlotPtr(NULL, item_slot_position));
    if (item_slot == NULL)
    {
        return false;
    }
    
    const BASE_ITEMINFO* const item_info = item_slot->GetItemInfo();
    switch (item_info->m_wType)
    {
    case eITEMTYPE_SOCKETEXTRACT:
    case eITEMTYPE_STAT_RESET:
    case eITEMTYPE_TOP_OF_TRIAL:
    case eITEMTYPE_TOP_OF_TRIAL_CHARGE:
    case eITEMTYPE_PORTAL_QUEST_ITEM:
    case eITEMTYPE_PET_NAME_CARD:
    case eITEMTYPE_CHANGE_GUILD_NAME:
    case eITEMTYPE_RANK_DOWN:
    case eITEMTYPE_HAMMER_OF_ALCHEMIST:
    case eITEMTYPE_PET:
    case eITEMTYPE_ETHER_DEVICE:
    case eITEMTYPE_GUILDMARK_SEAL:
    case eITEMTYPE_SSQ:
    case eITEMTYPE_CHAT_SHOUT_CONSUME:
    case eITEMTYPE_RIDER:
    case eITEMTYPE_RIDER_PARTS:
    case eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL:
    case eITEMTYPE_CHANGE_CHARACTER_FACE:
    case eITEMTYPE_CHANGE_CHARACTER_HAIR:
    case eITEMTYPE_CHANGE_CHARACTER_HEIGHT:
    case eITEMTYPE_BILL:
    case eITEMTYPE_ENCHANT:
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    case eITEMTYPE_AWAKENING_ALLROUND_MATERIAL:
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        return true;
    }

    return false;
}

//==================================================================================================
// (WAVERIX) (090709) (CHANGES) ::Use �ڵ� ����
RC::eITEM_RESULT
ItemManager::Use(SLOTIDX atIndex, POSTYPE atPos, SLOTCODE OUT& itemCode,
                 SkillInfo* inputed_skill_info_data, BYTE byAttackPropensity)
{
    if(!ValidPos(atIndex, atPos, TRUE))
        return RC::RC_ITEM_INVALIDPOS;

    SCSlotContainer* pAtContainer = GetItemSlotContainer(atIndex);
    SCItemSlot& rAtSlot = (SCItemSlot&)pAtContainer->GetSlot(atPos);

    RC::eITEM_RESULT rcResult = player_->CanUseItem(rAtSlot.GetItemInfo());
    if(rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    itemCode = rAtSlot.GetItemCode();
    const BASE_ITEMINFO* const pInfo = rAtSlot.GetItemInfo();

    
    // very usable variables
    const eITEMTYPE item_type = eITEMTYPE(pInfo->m_wType);
    const eWASTETYPE item_waste_type = eWASTETYPE(pInfo->m_byWasteType);

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (item_type == eITEMTYPE_SPA_WARP_SCROLL && player_->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::RC_ITEM_EVENT_ALREADY_ANOTHER_TRANSACTION;
    } 
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
//----------------------------------------------------------------------------------------------
    // ������ ���� �������� �׾�� ��밡��!(����ó��)
    if(item_type != eITEMTYPE_RECALL_ALLIANCE)
    {
        if(!ValidState()) 
        {
            return RC::RC_ITEM_INVALIDSTATE;
        }

        // ���������������� �ϰ�쿡�� ���� ���¿��� ��� ���ϰ� ��
        if(item_type == eITEMTYPE_REPAIR)
        {
            if ((player_ != NULL) && 
                (player_->GetStatusManager() != NULL) && 
                (player_->GetStatusManager()->Status.IsDragonTransforming() == true))
            {
                return RC::RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION;
            };
        }
    }

    // ������ �迭�� ���� ó��
    switch(item_type)
    {
#if defined (_CHINA) || defined (_RUSSIA)
    case eITEMTYPE_LOTTO:
        if( pInfo->m_LimitUseLevel != 0 && player_->GetLevel() < pInfo->m_LimitUseLevel )
            return RC::RC_ITEM_CANNOT_USE_PLAYER_LEVEL;
        break;
#endif
    case eITEMTYPE_SAVE_COORD:
        rcResult = nsSlot::ItemFunctionCoordinate(this).SaveCoord(rAtSlot);
        break;
    case eITEMTYPE_WAYPOINT:
        return nsSlot::ItemFunctionCoordinate(this).FireUpDateTime(rAtSlot);
    case eITEMTYPE_REPAIR:
        if (nsSlot::DurabilityImpl* const impl = player_->GetItemManager()->GetDurabilityImpl()) {
            if (impl->RepairAllUsingByRepairsItem() == false) {
                rcResult = RC::RC_ITEM_DONT_EXIST_TO_REPAIR;
            };
        };
        break;
    case eITEMTYPE_EXTRATAB:
        {
            BASE_PLAYERINFO* pPlayerInfo = player_->GetCharInfo();
            if(pPlayerInfo->m_ExtraInventoryTabCount >= MAX_CHARGE_INVENTORY_TAB_NUM)
            {
                rcResult = RC::RC_ITEM_CANNOT_ADD_EXTRAINVENTORY;
            }
            else
            {
                // f100816.4L, invoke a tab change event if an extra inventory number is changed
                // �κ��丮 Max Size���� = TabNum* Num Of Slot Per Tab
                CInventorySlotContainer* inventory = \
                    static_cast<CInventorySlotContainer*>(GetItemSlotContainer(SI_INVENTORY));
                BYTE tab_counts = ++pPlayerInfo->m_ExtraInventoryTabCount;
                ;    tab_counts += MAX_INVENTORY_TAB_NUM;
                // assert(MAX_INVENTORY_TAB_NUM + pInfo->m_ExtraInventoryTabCount <= 10)
                inventory->SetMaxSlotNum(tab_counts * MAX_INVENTORY_SLOT_PER_TAB);
                // set inventory sorting insert configuration
                inventory->UpdateInventoryConfiguration(pPlayerInfo->m_InventoryConfig);
                //++pPlayerInfo->m_ExtraInventoryTabCount;
                //GetItemSlotContainer(SI_INVENTORY)->SetMaxSlotNum(
                //    (MAX_INVENTORY_TAB_NUM + pPlayerInfo->m_ExtraInventoryTabCount) *
                //    MAX_INVENTORY_SLOT_PER_TAB);

                MSG_CG_ITEM_EXTRAINVENTORY_ADD_CMD cmd;
                cmd.m_ExtraInventoryTabCount = pPlayerInfo->m_ExtraInventoryTabCount;
                player_->SendPacket(&cmd, sizeof(cmd));
            }
        }
        break;
    case eITEMTYPE_RECALL_ALLIANCE:
        rcResult = nsSlot::ItemFunctionCoordinate(this).EnabledRecallAlliance();
        break;
    case eITEMTYPE_TORCH:
    case eITEMTYPE_MINE:
        if(!inputed_skill_info_data)
            return RC::RC_ITEM_CANNOT_SKILL_ACTION;
        rcResult = nsSlot::ItemFunctionSummon(this).CreateTotemNPC(
            rAtSlot, &inputed_skill_info_data->main_target_pos_);

        // ������ ���� ��ų�̹Ƿ� ���� ���� ���θ� ����.
        // �÷��̾��� ���� ���۵� ���� ���.
        if(rcResult == RC::RC_ITEM_SUCCESS)
            player_->SetForceAttack(byAttackPropensity == ATTACK_PROPENSITY_FORCE);
        break;
    case eITEMTYPE_TOTEM:
        rcResult = nsSlot::ItemFunctionSummon(this).CreateTotemNPC(rAtSlot);
        break;
    case eITEMTYPE_GUILDWAREHOUSE_SLOTCASH:
        return nsSlot::ItemFunctionPlayerProperty(this).ExtendGuildWarehouseCashSlot(atIndex, atPos);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case eITEMTYPE_GUILDFACILITY_RESET:
        return nsSlot::ItemFunctionPlayerProperty(this).InitGuildFacility(atPos, 1);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    case eITEMTYPE_RECALL:
        rcResult = nsSlot::ItemFunctionSummon(this).SummonMonsterItemUse(rAtSlot);
        break;
    case eITEMTYPE_EXTEND_CASH_SLOT:
        rcResult = nsSlot::ItemFunctionPlayerProperty(this).ExtendCashSlot(rAtSlot);
        break;
    // implemented by _NA_000587_20100928_DOMINATION_BUFF_ITEM
    case eITEMTYPE_DOMINATION_BUFF: // ������ ������ ���� �������϶� ���� ������ �˻��Ѵ�.
        {
            Monster* const buff_target = 
                static_cast<Monster* const>(
                    player_->GetField()->FindObject(
                        MONSTER_OBJECT, inputed_skill_info_data->main_target_key_));
            if (buff_target == NULL)
            {
                rcResult = RC::RC_ITEM_UNAVAILABLE_TARGET;
                break;
            }

            // ������ ����ڰ� �Ʊ����� �˻��Ѵ�.
            if (player_->IsFriend(buff_target) != USER_RELATION_FRIEND)
            {
                rcResult = RC::RC_ITEM_NOT_FRIEND;
                break;
            }
            
            // ������ ���� �������� �� �ɼǰ� 1�� ���� ����� ���´�.
            // ���� ������ ��� ����� ��ũ��Ʈ�� ������ ���� ��ġ�ϴ��� �˻��Ѵ�.
            {
                const BASE_NPCINFO* const buff_target_info = buff_target->GetBaseInfo();
                if (buff_target_info == NULL)
                {
                    rcResult = RC::RC_ITEM_UNAVAILABLE_TARGET;
                    break;
                }

                const DWORD buff_target_code = pInfo->m_iOptionValue[0];
                if (buff_target_info->m_MonsterCode != buff_target_code)
                {
                    rcResult = RC::RC_ITEM_UNAVAILABLE_TARGET;
                    break;
                }
            }

            // ��ų ��Ÿ��� ����ؼ� ������ ��� �Ÿ��� �˻��Ѵ�.
            // (hukim)(todo) ��ų�� �ߵ��ϱ� ���� ������ ��� ���� ���θ� �˻��ϴ� ���̹Ƿ� 
            // ���Ÿ����� Ư�� ��� ���� ����ϴ� ������(��ų�� ���� �Ǿ� �ִ�)�� ������ �þ�ٸ� 
            // Character::CanUseSkillItem()�� �˻� ������ �����ϰų� Use() �Լ� ���ο��� SkillExcute()
            // ������ �˻� ������ �����ؼ� ���� ����ȭ �ϴ°͵� ����غ��� �ϴ�.
            {
                const SkillInfoParser* const skill_info_parser = SkillInfoParser::Instance();
                const SkillScriptInfo* const item_skill_script_info = 
                    skill_info_parser->GetSkillInfo(pInfo->m_wSkillCode);
                if (item_skill_script_info == NULL)
                {
                    rcResult = RC::RC_ITEM_CANNOT_SKILL_ACTION;
                    break;
                }

                inputed_skill_info_data->main_target_pos_ = *buff_target->GetPosPtr();

                const float item_skill_range = item_skill_script_info->m_wSkillRange / 10.0f;
                if (!player_->CheckSkillRange(
                    buff_target, inputed_skill_info_data->main_target_pos_, item_skill_range))
                {
                    rcResult = RC::RC_ITEM_CANNOT_SKILL_ACTION;
                    break;
                }
            }
        }
        break;
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // ������� ĳ�� �����
    case eITEMTYPE_HEIM_LOTTO_TICKET:
    {
        rAtSlot.SetLock(TRUE);

        // Agent ������ ��û
        MSG_AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN syn;
        syn.char_guid = player_->GetCharGuid();
        syn.index     = atIndex;
        syn.pos       = atPos;
        g_pGameServer->SendToServer(AGENT_SERVER, &syn, (WORD)sizeof(syn));

        return RC::RC_ITEM_PROCESS_PENDING;
    }
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET
#ifdef _NA_002050_20110216_ADD_GENDER
    case eITEMTYPE_EXTEND_CHARACTER_SLOT:
        return nsSlot::ItemFunctionItemProperty(this).UseExtendCharSlotItem(atIndex, atPos);
#endif

    //_NA_004035_20120227_COSTUME_ITEM
    case eITEMTYPE_COSTUME:
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    case eITEMTYPE_COSTUME_DECORATE:
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
        {
            // �ڽ�Ƭ ����..
            Player::CostumeHandler& costume_handler = player_->GetCostumeHandler();
            if (costume_handler.Use(itemCode,atPos) == false)
            {
                return RC::RC_ITEM_FAILED;
            }
        }
        break;
    case eITEMTYPE_CHARACTER_NAME_CHANGE:
        {
            rAtSlot.SetLock(TRUE);

            MSG_DG_CHARNAME_CHANGE_CMD msg;
            msg.char_guid = player_->GetCharGuid();
            msg.slot_index = atIndex;
            msg.item_pos = atPos;
            player_->SendToGameDBPServer(&msg, sizeof(msg));

            return RC::RC_ITEM_PROCESS_PENDING;
        } /*break;*/
#ifdef _NA_006372_20130108_BADGE_ITEM
    case eITEMTYPE_RENDER_EFFECT_ITEM:
        {
            nsSlot::ItemFunctionToggle& toggle_item_handler_badge = \
                nsSlot::ItemFunctionToggle(player_->GetItemManager(), kToggleItem_Badge);
            //
            if(toggle_item_handler_badge.Use(itemCode, atPos) == false) {
                return RC::RC_ITEM_FAILED;
            }
        } break;
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    case eITEMTYPE_PCBANG: 
        {
            // �ǽù濡���� �ǽù�ȿ�� �������� ����� �� ����.
            if (!player_->GetPCRoomSts() && player_->GetReservedValue()) {
                return RC::RC_ITEM_FAILED;
            }
#ifdef _RU_006860_20130806_COSTUME_PCBANG_EFFECT
            //�ڽ�Ƭ ����
            //�ڽ�Ƭ�� �����ϰ� �ٽ������Ѵ�.
            if ( player_->GetIsEquippedCostume() )
            {
                Player::CostumeHandler& costume_handler = player_->GetCostumeHandler();
                SLOTCODE costume_item_code = player_->GetCostumeItemCode();
                POSTYPE costume_item_pos = player_->GetCostumeItemPos();

                CInventorySlotContainer* inventory = \
                    static_cast<CInventorySlotContainer*>(GetItemSlotContainer(SI_INVENTORY));
                SCItemSlot& item_slot = 
                    static_cast<SCItemSlot&>( inventory->GetSlot(costume_item_pos) );

                // �ڽ�Ƭ ����
                if ( costume_handler.Equipped(item_slot, false) == false )
                {
                    //Equipped�� false�� �����ϰ� �Ǿ����� ������, �ȴٸ� ������ �߻�
                }

                // �ڽ�Ƭ ����
                if (costume_handler.Use(costume_item_code, costume_item_pos) == false) {
                    return RC::RC_ITEM_FAILED;
                }
            }
#endif //_RU_006860_20130806_COSTUME_PCBANG_EFFECT
            //���������� ����
            // ���� �������� ���� �������� ������ ���� �ǽù��� �ҵ� ������ �ٽ� ���ش�.
            // ������ ����
            CharacterInfoExManager& extrainfo_manager = player_->GetExtraInfoManager();

            POSTYPE badge_item_pos = extrainfo_manager.UnEquipBadge();

            //������ ����
            nsSlot::ItemFunctionToggle& toggle_item_handler = \
                nsSlot::ItemFunctionToggle(player_->GetItemManager(), kToggleItem_PCBang);
            //
            if(toggle_item_handler.Use(itemCode, atPos) == false) {
                return RC::RC_ITEM_FAILED;
            }

            if (badge_item_pos != 0)
            {
                extrainfo_manager.EquipBadge(badge_item_pos);
            }

            player_->GetItemManager()->UpdateItemAttributes(true, false);
        } break;
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    case eITEMTYPE_DAILYQUEST_RESET:
        {
            CTime current_time  = util::TimeSync::_time64(NULL);

            HonorManager* character_honor_manager = player_->GetHonorManager();
            CharacterInfoExManager& character_extra_info_manager = player_->GetExtraInfoManager();
            CTime previous_time = character_extra_info_manager.GetDailyQuestResetUseTime();

            if (current_time.GetYear()  == previous_time.GetYear() &&
                current_time.GetMonth() == previous_time.GetMonth() &&
                current_time.GetDay()   == previous_time.GetDay())
            {
                character_honor_manager->SendDailyQuestResetUseTime();
                return RC::RC_ITEM_CANNOT_USE;
            }
            else
            {
                character_honor_manager->DailyQuestReset();
                character_extra_info_manager.SetDailyQuestResetUseTime(current_time);
                character_honor_manager->SendDailyQuestResetUseTime();
            }
        } break;
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    case eITEMTYPE_MINIGAME_BINGO_BALL:
        {
            MiniGameBingoActionParam action_param(eMINIGAME_TYPE_BINGO, pInfo->m_iOptionValue[0]);
            RC::eMINIGAME_RESULT minigame_result =
                player_->GetExtraInfoManager().GetMiniGameManager().DoAction(action_param);
            if (minigame_result != RC::RC_MINIGAME_ACTION_SUCCESS)
            {
                MSG_CG_CHARINFO_MINIGAME_NAK nak_packet;
                nak_packet.result_code = minigame_result;
                player_->SendPacket(&nak_packet, sizeof(nak_packet));
                return RC::RC_ITEM_FAILED_EXIST_SEPARATE_NAK_PACKET;
            }

            MSG_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET info_packet;
            memcpy(&info_packet.minigame_bingo_infos,
                &(player_->GetExtraInfoManager().GetMiniGameManager().GetMiniGameBingoInfos()), 
                sizeof(info_packet.minigame_bingo_infos));
            player_->SendPacket(&info_packet, sizeof(info_packet));
        } break;
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    //----------------------------------------------------------------------------------------------
    // USE���� ������� �ʰ� ���� ����ϱ� ���� ��Ŷ�� �����Ѵ�.
    case eITEMTYPE_SOCKETEXTRACT:
    case eITEMTYPE_STAT_RESET:
    case eITEMTYPE_TOP_OF_TRIAL:
    case eITEMTYPE_TOP_OF_TRIAL_CHARGE:
    case eITEMTYPE_PORTAL_QUEST_ITEM:
    case eITEMTYPE_PET_NAME_CARD:
    case eITEMTYPE_CHANGE_GUILD_NAME:
    case eITEMTYPE_RANK_DOWN:   //USE���� ������� �ʰ� ���� ����ϱ� ���� ��Ŷ�� �����Ѵ�.
    case eITEMTYPE_HAMMER_OF_ALCHEMIST: //USE���� ������� �ʰ� ���� ����ϱ� ���� ��Ŷ�� �����Ѵ�.
    case eITEMTYPE_PET: //
    case eITEMTYPE_ETHER_DEVICE:    //__NA_000968_ETHER_EMISSION_DEVICE
    case eITEMTYPE_GUILDMARK_SEAL: //��帶ũ�� ���⼭ ����� �� ����.
    case eITEMTYPE_SSQ:
    case eITEMTYPE_CHAT_SHOUT_CONSUME: // ��ġ�������(�Ҹ�) ������
    case eITEMTYPE_RIDER:
    case eITEMTYPE_RIDER_PARTS:
    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM = {
    case eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL: // ĳ���� ��� �ܸ� ����
    case eITEMTYPE_CHANGE_CHARACTER_FACE: // ĳ���� �� ����
    case eITEMTYPE_CHANGE_CHARACTER_HAIR: // ĳ���� ��� ����
    case eITEMTYPE_CHANGE_CHARACTER_HEIGHT: // ĳ���� Ű ����
    //}
    //!~ _NA004034_20120102_POINT_WALLET_SYSTEM
    case eITEMTYPE_BILL:
    //~! _NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    case eITEMTYPE_AWAKENING_ALLROUND_MATERIAL:
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    //----------------------------------------------------------------------------------------------
    }

    if(rcResult != RC::RC_ITEM_SUCCESS && rcResult != RC::RC_ITEM_MONSTER_SUMMON_FAILED)
        return rcResult;

    // �Ҹ� ������ ��� ����
    switch(item_waste_type)
    {
    case eITEMWASTE_HPPOTION:
    case eITEMWASTE_MPPOTION:
    case eITEMWASTE_HPMP_POTION:
        {
            if (player_->CanUsePotion() == FALSE)
            {
                return RC::RC_ITEM_PORTION_USE_LIMIT;
            }    
            // (WAVERIX) (NOTE) �ܺο� �ִ� ���� ���η� �����Դ�. ���� ����ʰ� �þ�� �ʴ�
            // �� �ϴ� �̰��� �α�� �Ѵ�.
            int recover = 0;
            int periodic_time = 0;
            if (pInfo->m_wTimes > 0)
            {
                if (pInfo->m_wHealHP > 0)
                {
                    recover = pInfo->m_wHealHP / pInfo->m_wTimes;
                }
                if (pInfo->m_wHealHPTime > 0)
                {
                    periodic_time = pInfo->m_wHealHPTime / pInfo->m_wTimes;
                }
            }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            //ȸ�� ���� ��Ÿ�� ���� ����
            util::Timer* timer = player_->GetItemCoolTimer(pInfo->m_wCoolTimeType);
            if (timer)
            {
                DWORD cur_interval = timer->GetBaseIntervalTime();
                double bonus_cooltime = player_->GetAttr().GetValueAppliedAttr(cur_interval, eATTR_RECOVER_POTION_COOLTIME_RATIO);
                timer->SetBonusIntervalTime(bonus_cooltime - cur_interval);
            }

            //ȸ�� ���� ȸ�� ���� ����
            recover = player_->GetAttr().GetValueAppliedAttr(recover, eATTR_RECOVER_POTION_RECOVERY_RATIO);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

            if (item_waste_type == eITEMWASTE_HPPOTION)
            {
                player_->UseHPPotion(recover, pInfo->m_wHealHPTime, periodic_time);
            }
            else if (item_waste_type == eITEMWASTE_MPPOTION)
            {
                player_->UseMPPotion(recover, pInfo->m_wHealHPTime, periodic_time);
            }
            else
            {
                player_->UseHPPotion(recover, pInfo->m_wHealHPTime, periodic_time);
                player_->UseMPPotion(recover, pInfo->m_wHealHPTime, periodic_time);
            }
        }
        break;
    case eITEMWASTE_RETURNSCROLL:
        if(player_->GetGameZoneState() != ePRS_AT_FIELD )
            return RC::RC_ITEM_CAN_USE_FIELD;
        break;
    case eITEMWASTE_QUESTACCEPT:
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
        break;
    case eITEMWASTE_UPSTONE:    // UP ȹ��
        player_->GetCharInfo()->m_UserPoint += pInfo->m_wHealHP;
        break;
    case eITEMWASTE_STAT_POINT_UP:
        player_->StatPointUpItemUse();
        break;
    case eITEMWASTE_PET_FEED:
        rcResult = nsSlot::ItemFunctionPet(this).UsePetFeed(rAtSlot);
        if(RC::RC_ITEM_SUCCESS != rcResult)
            return rcResult;
        break;
    case eITEMWASTE_ETHER_BULLET:
        rcResult = nsSlot::ItemFunctionEtherWeapon(this).ChargeEtherBullet(rAtSlot);
        if(RC::RC_ITEM_SUCCESS != rcResult)
            return rcResult;
        break;
    case eITEMWASTE_EXP_POTION:
        // ����ġ �������� ����... - arycoat 2012.07.09
        //if(pInfo->m_dwExp <= MAX_EXP_POTION_VALUE)
            player_->AddExp(pInfo->m_dwExp    );
        break;
    case eITEMWASTE_CHAO_TIME_DECREASE:
        rcResult = nsSlot::ItemFunctionPlayerProperty(this).UseChaoDecItem(pInfo);
        if(RC::RC_ITEM_SUCCESS != rcResult)
            return rcResult;
        break;
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
    case eITEMWASTE_FUNCTION:
    case eITEMWASTE_FUNCTION_HOLDUP_BUFF_POST_DEAD:
        {
            RC::eITEM_RESULT canuseitem_result = CanUseSkillItem(pInfo);
            if( canuseitem_result != RC::RC_ITEM_SUCCESS)
                return canuseitem_result;
        }break;
#endif //_NA_006689_20130503_CANNOTUSE_BUFFITEM---------------------------------------------
    }

    // ��ų ó��
    static const WORD ItemTypeListOfSkipUseSkills[5] = {
        eITEMTYPE_TOTEM,
        eITEMTYPE_RECALL,
        eITEMTYPE_TORCH,
        eITEMTYPE_MINE,
        eITEMTYPE_FIRECRACKER,
    };

    // ��ų ó��
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    //�̵���ũ���� ���ܷ� ó���Ѵ�. skillcode���� ���ڵ尪�� �־ ����ϰ� �ֱ⶧����
    if(pInfo->m_wSkillCode != 0 && item_type != eITEMTYPE_SPA_WARP_SCROLL)
#else
    if(pInfo->m_wSkillCode)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    {
        BOOLEAN processed = 0;
        // ��ų ����
        if(processed == 0 && item_waste_type == eITEMWASTE_LEARN_SKILL) {
            processed = true;
            //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
            return RC::RC_ITEM_CANNOT_LEARN_SKILL;
        }
        //__NA001390_090915_RIDING_SYSTEM__
        if(processed == 0 && item_type == eITEMTYPE_WAX) {
            // (WAVERIX) (NOTE) skill code ������ �ν� ���� ������ ���ؼ� ����ϰ� �ִ�.
            // ��ų ó���� �Ѿ���� �� �ٸ� ���� �̺�Ʈ�� �䱸�� �� �����Ƿ�, ����� ��Ų��.
            processed = true;
        }
    #ifdef _NA002217_100728_EP2_PERK_SYSTEM_
        if (processed == false && item_type == eITEMTYPE_PERK_CASH_ITEM) {
            // NOTE: a cash perk regist item process start from a PerkManager.
            // this module is processed about a waste item only.
            processed = true;
        }
    #endif
        // ��ų ���
        if(processed == 0)
        {
            BOOLEAN need_skip = 0;
            for(const WORD* it = ItemTypeListOfSkipUseSkills;
                it != &ItemTypeListOfSkipUseSkills[_countof(ItemTypeListOfSkipUseSkills)]; ++it)
            {
                if(eITEMTYPE(*it) != item_type)
                    continue;
                //finded!
                need_skip = true;
                break;
            }

            if(need_skip == 0)
            {
                // make arguments - by __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__
                SkillOption skill_option;
                skill_option.Clear();
                if(item_waste_type == eITEMWASTE_FUNCTION_HOLDUP_BUFF_POST_DEAD)
                    skill_option.AddOption(skill_option.eOption_HoldupBuffPostDead);
                // + attack_propensity
                // + event_code

                if (!nsSlot::ItemFunctionSkill(this).SkillExecute(
                        pInfo->m_wSkillCode, inputed_skill_info_data, skill_option,
                        byAttackPropensity, pInfo->m_byEventCode))
                {
                    return RC::RC_ITEM_CANNOT_SKILL_ACTION;
                }
            }
            processed = true;
        } // end of '��ų ���'
    }; // end of '��ų ó��'
    //
    // AC���� �̺�Ʈ ó��
    if(WORD code_of_AC = pInfo->m_wACCode)
    {
#ifdef _NA_001990_ACSYSTEM_ADD
        // �Ϲݾ�����(ȸ��������...)�� AC�� �����Ҽ� �ְ� �Ǿ���.
        player_->GetACManager()->ProcessItemUse(pInfo->m_Code, pInfo->m_wACCode, pInfo->m_byACReferenceID);
#else
		AchievementManager* pACManager = player_->GetACManager();
		if(!pACManager->IncConditionValue(code_of_AC, pInfo->m_byACReferenceID, pInfo->m_Code))
		{
			return RC::RC_ITEM_ALREADY_REGISTER_AC_ITEM;
		}
#endif //_NA_001990_ACSYSTEM_ADD
		//���� ����� ������ ���� ����Ǿ��ٰ� �ٷ� �ݿ��Ǵ� ���� �ƴ϶�
		//�������� ������ ���� ����޾ƾ� �ǹǷ� ���⼭ �������� �ǹ̰� ���� 
		//�Ϲ� ������ �α׸� �����.
		GAMELOG->LogItem(ITEM_USE, player_, &rAtSlot);
        
    }
    else
    {
        GAMELOG->LogItem(ITEM_USE, player_, &rAtSlot);
    }

    // �Ҹ� �������̸� �����Ѵ�.
    if(pInfo->IsMaterialWaste())
    {
        if(rAtSlot.GetNum() <= 1)
        {
            BOOL rt = pAtContainer->DeleteSlot(atPos, NULL);
            ASSERT(rt && "User():���� ���� �����̳ʿ��� ������ �� ���� �������Դϴ�.\n");
        }
        else
        {
            pAtContainer->UpdateSlot(rAtSlot.GetPos(), -1);
        }
    }

    player_->ResetUseItem(pInfo->m_wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT ItemManager::UseItemByType(
    const eITEMTYPE item_type, 
    const SLOTIDX item_container_type, 
    const POSTYPE item_position, 
    const bool use_response, 
    const TCHAR* called_function)
{
    // ������ ���¸� �˻��Ѵ�.
    SCItemSlotContainer* const inventory = GetItemSlotContainer(item_container_type);
    SCItemSlot& item_slot = 
        static_cast<SCItemSlot&>(inventory->GetSlot(item_position));
    if (item_slot.IsLocked() || 
        item_slot.IsBlocked() ||
        item_slot.IsUserBlocked() || 
        !item_slot.IsContainValidItem())
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|������ ���°� �ǹٸ��� �ʽ��ϴ�.|��������ġ = %d, ĳ�����̸� = %s|"), 
               called_function, 
               item_position, 
               player_->GetCharName());
        return RC::RC_ITEM_INVALIDSTATE;
    }

    // ������ ������ �˻��Ѵ�.
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    if (item_info->m_wType != item_type)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|������ ������ �ǹٸ��� �ʽ��ϴ�.|���������� = %d, ��������ġ = %d, ĳ�����̸� = %s|"), 
               called_function, 
               item_info->m_wType, 
               item_position, 
               player_->GetCharName());
        return RC::RC_ITEM_INVALID_TYPE;
    }

    // ���� ������ ����Ͽ� �������� ����Ѵ�.
    SLOTCODE used_item_code = 0;
    const RC::eITEM_RESULT use_result = Use(item_container_type, item_position, used_item_code);
    if (use_result != RC::RC_ITEM_SUCCESS)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�������� ����� �� �����ϴ�.|��������ġ = %d, ĳ�����̸� = %s|"), 
               called_function, 
               item_position, 
               player_->GetCharName());
        return use_result;
    }

    // ��û�ڿ��� ������ ������.
    // ���� Ŭ���̾�Ʈ ������ ������ ��� ������ ������ ������ �Ҹ� ������ ����Ͽ� 
    // ������ ���̱� ������ �����ϰ� �Ҹ� �������� ��쿡�� ������.
    if (use_response && item_info->IsMaterialWaste())
    {
        MSG_CG_ITEM_USE_ACK use_ack;
        use_ack.m_atIndex = item_container_type;
        use_ack.m_atPos = item_position;
        player_->SendPacket(&use_ack, sizeof(use_ack));
    }

    return RC::RC_ITEM_SUCCESS;
}

//==================================================================================================

// �Ҹ� �������� ����
RC::eITEM_RESULT ItemManager::DeleteWastedItem(SLOTIDX atIndex, POSTYPE atPos)
{
	SCSlotContainer* pAtContainer = GetItemSlotContainer(atIndex);
	SCItemSlot & rAtSlot = (SCItemSlot &)pAtContainer->GetSlot(atPos);
	
	SLOTCODE ItemCode = rAtSlot.GetItemCode();
	const BASE_ITEMINFO* const pInfo = rAtSlot.GetItemInfo();
	
	WORD wCoolTimeType = pInfo->m_wCoolTimeType;
	if(pInfo->IsMaterialWaste())
	{
		if( rAtSlot.GetNum() <= 1 )
		{
			BOOL rt = pAtContainer->DeleteSlot( atPos, NULL );
			if(!rt)
			{
				SUNLOG(eCRITICAL_LOG, "�Ҹ� ������ ���� ����(DB���� �ݿ��Ǿ���)[%u][%u][%d][%d][%d]", 
				player_->GetCharGuid(), player_->GetGuildGuid(), pInfo->m_Code, pInfo->m_wType, atIndex, atPos);
				return RC::RC_ITEM_FAILED;
			}
		}
		else
		{
			pAtContainer->UpdateSlot(rAtSlot.GetPos(), -1);
		}
	}

	player_->ResetUseItem( wCoolTimeType );

	MSG_CG_ITEM_USE_ACK ackMsg;
	ackMsg.m_atIndex = atIndex;
	ackMsg.m_atPos = atPos;
	player_->SendPacket( &ackMsg, sizeof( ackMsg ) );
	return RC::RC_ITEM_SUCCESS;
}


#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
RC::eITEM_RESULT ItemManager::CanUseSkillItem(const BASE_ITEMINFO *const pInfo)
{
    //��ų�� �ߵ��ϴ� �������� ���, �ڽſ��� �ɸ� �����Ƽ�� �����۽�ų�� �����Ƽ��
    //���� ��, �����۽�ų�� �� ������ ���, �ڽ��� ������ �����Ƽ�� �Ȱɷ����� ��츸
    //�������ش�.
    const SkillInfoParser* const skill_info_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* const item_skill_script_info = 
        skill_info_parser->GetSkillInfo(pInfo->m_wSkillCode);

    if (item_skill_script_info == NULL)
        return RC::RC_ITEM_SUCCESS;
    item_skill_script_info->SetFirst();
    //�����ۿ� ���� ��ų�� �����Ƽ����
    const BASE_ABILITYINFO* ability_info = item_skill_script_info->GetNext();
    if(ability_info == NULL)
        return RC::RC_ITEM_SUCCESS;
    const eCHAR_STATE_TYPE state_id = static_cast<eCHAR_STATE_TYPE>(ability_info->m_wStateID);
    //�����Ű���� �����Ƽ�� ���� ����ID�� ����, �ڽſ��� �ɸ� �����Ƽ
    AbilityStatus* ability_status = \
        static_cast<AbilityStatus*>(player_->GetStatusManager()->FindStatus(state_id));

    if (ability_status == NULL)
        return RC::RC_ITEM_SUCCESS;
    if (ability_status->IsAbilityStatus())
    {
        //�����۽�ų �����Ƽ�� ù��° �����Ƽ�� �񱳴���� �ȴ�.
        const double current_player_ability_value = ability_status->GetAbilityInfo()->m_iParam[1];
        const double item_skill_ability_value = ability_info->m_iParam[1];
        //���� �ɸ� �����Ƽ���� �� ũ�� �����ۻ�� �ź�
        if(abs(current_player_ability_value) > abs(item_skill_ability_value))
            return RC::RC_ITEM_CANNOT_USE_ALREADY_APPLY_HIGHRANK_BUFF;
    }

    return RC::RC_ITEM_SUCCESS;
}
#endif  //_NA_006689_20130503_CANNOTUSE_BUFFITEM