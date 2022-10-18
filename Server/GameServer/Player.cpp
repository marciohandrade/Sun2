// Player.cpp: implementation of the Player class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Player.h"
//////////////////////////////////////////////////////////////////////////

// <ProgramCommon>
#include <PublicMath.h>
#include <CharInfoParser.h>
#include <SkillInfoParser.h>
// <Quest>
#include <QuestDefine.h>
#include <QuestStruct.h>
#include <QuestManager.h>
// <Common>
#include <ServerSession.h>
#include <PacketStruct_AG.h>
#include <WayPoint.h>
#include <FileParser.h>
#include <ServerOptionParser.h>
#include <PacketControl/WarMessages_Sync.h>
#include <PacketControl/PacketControlS2S.h>

// <SunWriteLog>
#include <LogTimeFormat.h>

// <Internal GameServer>
#include "GameServerEx.h"
#include "PlayerManager.h"
#include "Map.h"
#include "NPC.h"
#include "GameField.h"
#include "GameZone.h"
#include "GameZoneManager.h"
#include "GameShopManager.h"
#include "CharacterFormula.h"
// <Internal GameServer STATUS CONTROLLER in Player>
#include "MissionManager.h"
#include "ItemManager.h"
#include "TradeSlotContainer.h"
#include "TradeManager.h"
#include "VendorManager.h"
#include "VendorPageManager.h"
#include "SummonManager.h"
#include "SummonedNPCs.h"
#include "SlotManager.h"
#include "SkillManager.h"
#include "QuickManager.h"
#include "QuickStyleManager.h"
#include "TriggerManager.h"
#include "StatusManager.h"
#include "PVPInfo.h"
#include "EventInventorySlotContainer.h"
#include <AreaConquestCommonHeader.h>
#include "FatigueManager.h"
#include "QuestManager_Concrete.h"
#include "QuestInfoManagerEx.h"
#include "ChaoState.h"
#include "CHuntingManager.h"
#include "CompetitionInfo.h"
#include "View.h"
#include "MovementChecker.h"
#include "MoveStateControl.h"

// <ETCs>
#include "EtcStatus.h"
#include "RecoverStatus.h"
#include "GameCHuntingRoom.h"
#include "CoolTimerFactory.h"
#include <ServerOptionParserEx.h>
#include "SkillSlotManager.h"
#include <SkillSystem/PerkManager.h> //_NA002217_100728_EP2_PERK_SYSTEM_

#include <World/GameWarControlManager.h>
#include "GameInstanceDungeon.h"
#include "MovingPlanner.h"
#include "CollectedList.h"
#include "WarehouseSlotContainer.h"
#include "GuildWarehouseSlotContainer.h"

#include "GroupInfoParser.h"
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
#include "AccumulatePoint.h"
#endif

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include <EquipmentSlotContainer.h>
#include "SunRanking/SunRankingGradeManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
#include "DominationInfoParser.h"
#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "GameGuildManager.h"
#include "GameGuild.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
#include "PostManager.h"
#endif

#include "./Slot/EtherealizeImpl.h"


//_NA_004035_20120227_COSTUME_ITEM
#include <SkillAttrCalculator.h>
#include "InventorySlotContainer.h"

//[Costume]=========================================================================================
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
bool Player::CostumeHandler::Use(const SLOTCODE item_code, const POSTYPE item_pos)
{
    CInventorySlotContainer* const inventory_container = 
        static_cast<CInventorySlotContainer*>(owner_->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    
    SCItemSlot& item_slot = 
        static_cast<SCItemSlot&>(inventory_container->GetSlot(item_pos));

    WORD item_type = item_slot.GetItemInfo()->m_wType;
    // ������ Ÿ�� �˻�
    if (item_slot.GetItemInfo() == NULL || 
        (item_type != eITEMTYPE_COSTUME && item_type != eITEMTYPE_COSTUME_DECORATE))
    {
        return false;
    }

    // �ڽ�Ƭ ���� ����..
    if (item_slot.GetItemInfo()->IsCanUseGender(owner_->GetCharInfo()->gender) == false) 
    {
        return false;
    }

    // �ڽ�Ƭ Ŭ���� ����..
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(owner_->GetCharInfo()->m_byClassCode);
    if (item_slot.GetItemInfo()->CheckValidEquipClass(char_type) == false)
    {
        return false;
    }


    // �̹� �ڽ�Ƭ �������� ��� ó��
    if (item_type == eITEMTYPE_COSTUME && owner_->char_info_.is_equipped_costume_ == true)
    {
        // ���� ����..
        if (owner_->char_info_.costume_item_pos_ == item_pos)
        {
            return Equipped(item_slot, false);
        }
        // ���� �ڽ�Ƭ ���� �� ���ο� �ڽ�Ƭ ����..
        else
        {
            SCItemSlot& previous_item_slot = 
                static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.costume_item_pos_));
            const BASE_ITEMINFO* precious_item_info = previous_item_slot.GetItemInfo();
            if (precious_item_info && precious_item_info->m_wType == eITEMTYPE_COSTUME)
            {
                ApplyOption(previous_item_slot, false);
            }
            else
            {
                owner_->char_info_.is_equipped_costume_ = 0;
                owner_->char_info_.costume_item_pos_ = INVALID_POSTYPE_VALUE;
                owner_->char_info_.costume_item_code_ = 0;
                return false;
            }

            return Equipped(item_slot, true);
        }
    }
    else if (item_type == eITEMTYPE_COSTUME_DECORATE && owner_->char_info_.is_equipped_decorate_costume_ == true)
    {
        // ���� ����..
        if (owner_->char_info_.decorate_costume_item_pos_ == item_pos)
        {
            return Equipped(item_slot, false);
        }
        // ���� �ڽ�Ƭ ���� �� ���ο� �ڽ�Ƭ ����..
        else
        {
            SCItemSlot& previous_item_slot = 
                static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.decorate_costume_item_pos_));
            const BASE_ITEMINFO* precious_item_info = previous_item_slot.GetItemInfo();
            if (precious_item_info && precious_item_info->m_wType == eITEMTYPE_COSTUME)
            {
                ApplyOption(previous_item_slot, false);
            }
            else
            {
                owner_->char_info_.is_equipped_decorate_costume_ = 0;
                owner_->char_info_.decorate_costume_item_pos_ = INVALID_POSTYPE_VALUE;
                owner_->char_info_.decorate_costume_item_code_ = 0;
                return false;
            }

            return Equipped(item_slot, true);
        }
    }
    // �ڽ�Ƭ ���������� ��� ó��
    else
    {
        return Equipped(item_slot, true);
    }

    return false;
}

void Player::CostumeHandler::SetCostumeInfo()
{
    if (is_applyed_option_ == false)
    {
        // ���� �ڽ�Ƭ
        if (owner_->char_info_.is_equipped_costume_ == true)
        {
            CInventorySlotContainer* const inventory_container = 
                static_cast<CInventorySlotContainer*>(owner_->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    

            SCItemSlot& item_slot = 
                static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.costume_item_pos_));

            const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
            if (item_info == NULL || item_info->m_wType != eITEMTYPE_COSTUME)
            {
                owner_->char_info_.is_equipped_costume_ = 0;
                owner_->char_info_.costume_item_code_ = 0;
                owner_->char_info_.costume_item_pos_ = INVALID_POSTYPE_VALUE;
                return;
            }
            ApplyOption(item_slot, true);
        }
        
        // ���� �ڽ�Ƭ
        if (owner_->char_info_.is_equipped_decorate_costume_ == true)
        {
            CInventorySlotContainer* const inventory_container = 
                static_cast<CInventorySlotContainer*>(owner_->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    

            SCItemSlot& item_slot = 
                static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.decorate_costume_item_pos_));

            const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
            if (item_info == NULL || item_info->m_wType != eITEMTYPE_COSTUME_DECORATE)
            {
                owner_->char_info_.is_equipped_decorate_costume_ = 0;
                owner_->char_info_.decorate_costume_item_code_ = 0;
                owner_->char_info_.decorate_costume_item_pos_ = INVALID_POSTYPE_VALUE;
                return;
            }
            ApplyOption(item_slot, true);
        }
    }
}

#else
bool Player::CostumeHandler::Use(const SLOTCODE item_code, const POSTYPE item_pos)
{
    CInventorySlotContainer* const inventory_container = 
        static_cast<CInventorySlotContainer*>(owner_->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    
    SCItemSlot& item_slot = 
        static_cast<SCItemSlot&>(inventory_container->GetSlot(item_pos));

    if (item_slot.GetItemInfo() == NULL || item_slot.GetItemInfo()->m_wType != eITEMTYPE_COSTUME)
    {
        return false;
    }

    // �ڽ�Ƭ ���� ����..
    if (item_slot.GetItemInfo()->IsCanUseGender(owner_->GetCharInfo()->gender) == false) 
    {
        return false;
    }

    // �ڽ�Ƭ Ŭ���� ����..
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(owner_->GetCharInfo()->m_byClassCode);
    if (item_slot.GetItemInfo()->CheckValidEquipClass(char_type) == false)
    {
        return false;
    }


    // �̹� ���� ��..
    if (owner_->char_info_.is_equipped_costume_)
    {
        // ���� ����..
        if (owner_->char_info_.costume_item_pos_ == item_pos)
        {
            return Equipped(item_slot, false);
        }

        // ���� �ڽ�Ƭ ���� �� ���ο� �ڽ�Ƭ ����..
        else
        {
            SCItemSlot& previous_item_slot = 
                static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.costume_item_pos_));
            const BASE_ITEMINFO* precious_item_info = previous_item_slot.GetItemInfo();
            if (precious_item_info && precious_item_info->m_wType == eITEMTYPE_COSTUME)
            {
                ApplyOption(previous_item_slot, false);
                //Equipped(previous_item_slot, false);
            }
            else
            {
                owner_->char_info_.is_equipped_costume_ = 0;
                owner_->char_info_.costume_item_pos_ = INVALID_POSTYPE_VALUE;
                owner_->char_info_.costume_item_code_ = 0;
                return false;
            }

            return Equipped(item_slot, true);
        }
    }

    // �� ���� ��..
    else
    {
        return Equipped(item_slot, true);
    }

    return false;
}

void Player::CostumeHandler::SetCostumeInfo()
{
    if (owner_->char_info_.is_equipped_costume_ && is_applyed_option_ == false)
    {
        CInventorySlotContainer* const inventory_container = 
            static_cast<CInventorySlotContainer*>(owner_->GetItemManager()->GetItemSlotContainer(SI_INVENTORY));    

        SCItemSlot& item_slot = 
            static_cast<SCItemSlot&>(inventory_container->GetSlot(owner_->char_info_.costume_item_pos_));

        const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
        if (item_info == NULL || item_info->m_wType != eITEMTYPE_COSTUME)
        {
            owner_->char_info_.is_equipped_costume_ = 0;
            owner_->char_info_.costume_item_code_ = 0;
            owner_->char_info_.costume_item_pos_ = INVALID_POSTYPE_VALUE;
            return;
        }
        ApplyOption(item_slot, true);
    }
}
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

bool Player::CostumeHandler::Equipped(SCItemSlot& item_slot, const bool is_equipped)
{
    if (is_equipped)
    {
        ApplyOption(item_slot, true);
#ifdef _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME
        const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
        if (item_info && 
            (item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX ||
            item_info->m_ChargeSubType == eCHARGE_SUB_COORDINATE_EX ||
            item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX2)
            )
#endif //_NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME
        {
            item_slot.FireUp_DateTime();
        }
    }
    else
    {
        ApplyOption(item_slot, false);
    }

    //�ڽ�Ƭ ���� ����
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    WORD item_type = item_slot.GetItemInfo()->m_wType;
    if (item_type == eITEMTYPE_COSTUME)
    {
        owner_->char_info_.is_equipped_costume_ = is_equipped;
        owner_->char_info_.costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
        owner_->char_info_.costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;

        //��Ŷ����
        {
            // Ŭ������..
            MSG_CG_ITEM_USE_COSTUME_ACK ack_msg;
            ack_msg.Init();
            ack_msg.is_equipped_costume_ = is_equipped;
            ack_msg.costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
            owner_->SendPacket(&ack_msg, ack_msg.GetSize());

            // BRD..
            MSG_CG_ITEM_USE_COSTUME_BRD brd_msg;
            brd_msg.Init();
            brd_msg.object_key_ = owner_->GetObjectKey();
            brd_msg.costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;
            owner_->SendPacketAround(&brd_msg,brd_msg.GetSize(),false);
        }
    }
    else if (item_type == eITEMTYPE_COSTUME_DECORATE)
    {
        owner_->char_info_.is_equipped_decorate_costume_ = is_equipped;
        owner_->char_info_.decorate_costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
        owner_->char_info_.decorate_costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;

        //��Ŷ����
        {
            // Ŭ������..
            MSG_CG_ITEM_USE_DECORATE_COSTUME_ACK ack_msg;
            ack_msg.Init();
            ack_msg.is_equipped_costume_ = is_equipped;
            ack_msg.deco_costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
            owner_->SendPacket(&ack_msg, ack_msg.GetSize());

            // BRD..
            MSG_CG_ITEM_USE_DECORATE_COSTUME_BRD brd_msg;
            brd_msg.Init();
            brd_msg.object_key_ = owner_->GetObjectKey();
            brd_msg.deco_costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;
            owner_->SendPacketAround(&brd_msg,brd_msg.GetSize(),false);
        }
    }
#else
    owner_->char_info_.is_equipped_costume_ = is_equipped;
    owner_->char_info_.costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
    owner_->char_info_.costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;

    //��Ŷ����
    {
        // Ŭ������..
        MSG_CG_ITEM_USE_COSTUME_ACK ack_msg;
        ack_msg.Init();
        ack_msg.is_equipped_costume_ = is_equipped;
        ack_msg.costume_item_pos_ = is_equipped ? item_slot.GetPos() : INVALID_POSTYPE_VALUE;
        owner_->SendPacket(&ack_msg, ack_msg.GetSize());

        // BRD..
        MSG_CG_ITEM_USE_COSTUME_BRD brd_msg;
        brd_msg.Init();
        brd_msg.object_key_ = owner_->GetObjectKey();
        brd_msg.costume_item_code_ = is_equipped ? item_slot.GetItemCode() : 0;
        owner_->SendPacketAround(&brd_msg,brd_msg.GetSize(),false);
    }
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

    // �α׸� ����Ѵ�.
    {
        TCHAR add_info[MAX_ADDINFO_SIZE] = { 0 };
        ::_sntprintf(add_info, MAX_ADDINFO_SIZE, _T("ACTIVE:%u"), (is_equipped ? 1 : 0));
        add_info[MAX_ADDINFO_SIZE-1] = _T('\0');
        GAMELOG->LogItem(ITEM_USE_COSTUME, owner_, &item_slot, 0, 1, NULL, NULL, add_info);
    }
    
    return true;
}

void Player::CostumeHandler::ApplyOption(SCItemSlot& item_slot, const bool is_add)
{
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    if (item_info == NULL || 
        (item_info->m_wType != eITEMTYPE_COSTUME && item_info->m_wType != eITEMTYPE_COSTUME_DECORATE))
    {
        return;
    }

    SkillAttrCalculator skill_calculator(owner_->GetAttr());

    for (int op = 0; op < _countof(item_info->m_wOptionIndex); ++op)
    {
        const int attr_option_index = item_info->m_wOptionIndex[op];
        if (attr_option_index == 0)
        {
            continue;
        }
        if (attr_option_index >= _countof(g_ITEM_OPTION))
        {
            continue;
        }
        const int attr_value = item_info->m_iOptionValue[op];
        if (attr_value == 0)
        {
            continue;
        }
        const int attr_value_type = item_info->m_byOptionType[op];
        if (is_add)
        {
            skill_calculator.AddAttr( \
                g_ITEM_OPTION[attr_option_index], attr_value_type, attr_value, true);
        }
        else
        {
            skill_calculator.DelAttr( \
                g_ITEM_OPTION[attr_option_index], attr_value_type, attr_value, true);
        }
    }

#ifdef _RU_006860_20130806_COSTUME_PCBANG_EFFECT
    //PC����� ���뿩��
    const bool pc_room_status = (owner_->CanApplyPCBangBenefit(true) != false);
    if(pc_room_status == true)
    {
        for (int op = 0; op < _countof(item_info->m_wPCOptionIndex); ++op)
        {
            const int attr_option_index = item_info->m_wPCOptionIndex[op];
            if (attr_option_index == 0)
            {
                continue;
            }
            if (attr_option_index >= _countof(g_ITEM_OPTION))
            {
                continue;
            }
            const int attr_value = item_info->m_iPCOptionValue[op];
            if (attr_value == 0)
            {
                continue;
            }
            const int attr_value_type = item_info->m_byPCOptionType[op];

            //�ǽù�ɼ��� ����� ���¿��� �ٽ� �ǽù�ɼ��� �����Ϸ��� ���¿�
            //�ǽù�ɼ��� ����ް����� �ʴ� ���¿��� �ǽù�ɼǼ�ġ��ŭ�� �����Ϸ��� ���¸� ���� ����.
            if (is_add)
            {
                skill_calculator.AddAttr( \
                    g_ITEM_OPTION[attr_option_index], attr_value_type, attr_value, true);
            }
            else
            {
                skill_calculator.DelAttr( \
                    g_ITEM_OPTION[attr_option_index], attr_value_type, attr_value, true);
            }
        }
    }
#endif //_RU_006860_20130806_COSTUME_PCBANG_EFFECT

    owner_->GetAttr().UpdateEx();

    const BOOL is_block = (is_add ? TRUE : FALSE);
    item_slot.SetUserBlocked(is_block);

    is_applyed_option_ = is_add;
}

//==================================================================================================
Player::Player()
	: RENDER_EFFECT(m_RenderEffect)
	, m_pServerSession ( NULL )
	, m_NextExp( 0 )
    , costume_handler_(this) //_NA_004035_20120227_COSTUME_ITEM
{
    SetObjectType(PLAYER_OBJECT);
    // CHANGES: f110307.2L
    ;{
        ns_object::SearchReferences ref_option = { 0, };
        ref_option.hp = &char_info_.m_iHP;
        _RegisterSearchReference(ref_option);
    };
    // f100827.4L, singleton setting to a linker binding
    new (&attributes_linker_) ns_object::PlayerAttributeLinker(this);
    //
	m_pItemManager			= new ItemManager;
	m_pSlotManager			= new SlotManager;
	m_pMissionManager		= new MissionManager;
	m_pQuestManager			= new QuestManager_Concrete(&g_QuestInfoManager);
	m_pQuickManager			= new QuickManager;
	//m_pStyleManager			= new QuickStyleManager;
	m_pMovementChecker		= new MovementChecker(this);
	m_pPVPInfo				= new PVPInfo(this);
	m_pFatigueManager		= new FatigueManager;
	m_pCompetitionInfo		= new CompetitionInfo(this);
	m_pACManager			= new AchievementManager;
	m_pSkillSlotManager		= new SkillSlotManager;
    perk_manager_ = NULL;
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    perk_manager_ = new nsSkill::PerkManager();
#endif

	m_pEventSlotContainer	= new EventInventorySlotContainer;
	m_pWayPoint				= new WayPoint;
	m_pCharWayPoint			= new CharWayPoint;

	m_pChaoState			= new ChaoState(this);
    m_pInvitateFriend       = new InvitateFriendInfo(this);

    m_Attr.SetOwner(&attributes_linker_);
	m_bExpiredPlayer = TRUE;
	m_PreVillageMapCode = 0;

    //_NA_0_20100722_AUTO_SYSTEM
    m_lastReturnScrollMapCode = 0;  
    ZeroMemory(&m_lastReturnScrollPos, sizeof(m_lastReturnScrollPos));

	m_byDebugInfoFlag = 0;

	m_pCollectedList = new CollectedList;

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    m_pAccumulatePoint = new AccumulatePoint(this);
#endif

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    m_PostManager = new PostManager;
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    honor_manager_ = new HonorManager;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    point_wallet_manager_ = new PointWalletManager;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    battle_ground_player_info_ = new BattleGroundPlayerInfo(this);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    goldrush_playerinfo_ = new GoldRushPlayerInfo(this);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    extra_info_manager_.Init(this);
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    player_assist_meter = new PlayerAssistMeter(this);
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    player_spa_info_ = new PlayerSpaInfo(this);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    random_manager_ = new RandomManager;

    // MUST BE last field
    m_StartCheckCode = eVERIFY_MAGICCODE_INIT;
    m_EndCheckCode = eVERIFY_MAGICCODE_INIT;
}

Player::~Player()
{
	SAFE_DELETE( m_pPVPInfo );
	SAFE_DELETE( m_pEventSlotContainer );
	SAFE_DELETE( m_pSlotManager );
	SAFE_DELETE( m_pItemManager );
	SAFE_DELETE( m_pMissionManager );
	SAFE_DELETE( m_pQuestManager );
	SAFE_DELETE( m_pQuickManager );
	//SAFE_DELETE( m_pStyleManager );
	SAFE_DELETE( m_pMovementChecker );
	SAFE_DELETE( m_pFatigueManager );
	SAFE_DELETE( m_pACManager );
	SAFE_DELETE( m_pCompetitionInfo );
	SAFE_DELETE( m_pWayPoint );
	SAFE_DELETE( m_pCharWayPoint );
    SAFE_DELETE( m_pInvitateFriend );
	SAFE_DELETE( m_pChaoState );
	SAFE_DELETE( m_pSkillSlotManager );
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    SAFE_DELETE(perk_manager_);
#endif
	SAFE_DELETE( m_pCollectedList );
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    SAFE_DELETE( m_pAccumulatePoint );
#endif

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    SAFE_DELETE( m_PostManager );
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    SAFE_DELETE( honor_manager_ )
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    SAFE_DELETE(point_wallet_manager_)
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SAFE_DELETE(battle_ground_player_info_)
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    SAFE_DELETE(goldrush_playerinfo_)
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    SAFE_DELETE(player_assist_meter)
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SAFE_DELETE(player_spa_info_)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    SAFE_DELETE(random_manager_)

    // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.ns_trigger::TriggerPlayerLinkInGameSlots::~TriggerPlayerLinkInGameSlots();

    m_StartCheckCode = eVERIFY_MAGICCODE_RELEASE;
    m_EndCheckCode = eVERIFY_MAGICCODE_RELEASE;
}


BOOL Player::Init()
{
	m_StartInitCode = eVERIFY_MAGICCODE_INIT;
	RENDER_EFFECT.Clear();

    ZeroMemory(&char_info_, sizeof(char_info_));
    char_name_hash_ = 0; // NOTE: f110517.3L, this field is not cleared on 'Release'
	m_bExpiredPlayer = FALSE;
	m_PreVillageMapCode = 0;
    //_NA_0_20100722_AUTO_SYSTEM
    m_lastReturnScrollMapCode = 0;  
    ZeroMemory(&m_lastReturnScrollPos, sizeof(m_lastReturnScrollPos));
    m_dwLastSendTick	= GetTickCount();
	m_dwSentSize		= 0;
	m_pVendorPage		= NULL;

	m_pSlotManager->Init( this );
	m_pItemManager->Init( m_pSlotManager );
	m_pSkillSlotManager->Init( m_pSlotManager );
	m_pQuickManager->Init( m_pSlotManager );
	//m_pStyleManager->Init( m_pSlotManager );
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    perk_manager_->Reuse(this);
#endif
	m_pEventSlotContainer->ClearAll();				//< �̺�Ʈ �κ��丮 Ŭ����
	m_pEventSlotContainer->SetPlayer(this);
	m_pMissionManager->Clear();

	m_pQuestManager->Init(this);
	m_pACManager->Init(this);

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    m_pAccumulatePoint->Init( this );
#endif

	m_pMovementChecker->Reset();
	m_pWayPoint->Clear();
	m_pCharWayPoint->Clear();

    m_pChaoState->Init();

	m_DBPUpdateTimer.SetTimer( _DBP_UPDATE_CYCLE_DELAY );

	m_pMoveStateControl->Init( this, CMS_RUN );

	// ������ ��Ÿ�� ����
	SetItemCoolTime( TRUE, SKILL_COOLTIME_MARGIN );

	// ��Ƽ�� ��ó�� ���� ��Ÿ�� ����
	SetSkillCoolTime( eACTION_START_SKILL_CODE + eACTION_PARTY_MEMBER_SUMMON, SKILL_COOLTIME_MARGIN );
	SetSkillCoolTime( eACTION_START_SKILL_CODE + eACTION_PARTY_MEMBER_WARP, SKILL_COOLTIME_MARGIN );

	StopMoving();
	SetTradeContainer(NULL);
	SetVendorContainer(NULL);
	SetBehave(PLAYER_BEHAVE_IDLE_STATE);

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	SetRankingSystemCtl(NULL);
#endif // _NA_008012_20150130_RANKING_SYSTEM

	m_pPVPInfo->Init();

	m_pCompetitionInfo->Init();
	
	m_bDoingAction		= FALSE;
	m_byVkrReloadCount	= MAX_VALKYRIE_RELOAD_COUNT;
	m_ShapShotLogTimer.SetTimer( SNAPSHOT_LOG_CYCLE );

    if (GetFatigueManager()->IsApplyFatigue() == TRUE) {
        m_FatigueLogTimer.SetTimer( FATIGUE_LOG_CYCLE );
    }

	m_byLastAttackSeq = eATTACK_SEQUENCE_NONE;
	m_byDebugInfoFlag = 0;
	memset( &m_GMStateInfo, 0, sizeof(STATE_GM_INFO) );

	//�߰� �̵��ӵ����� �⺻�� 1
	m_fAddMoveSpeedRatio = 1.0f;

	SetMoveState( CMS_RUN );

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	for(INT i=0;i<MAX_MISSION_NUM+1;++i)
		*(m_pBestMissionTime+i) = INT_MAX;
#endif // _NA_008012_20150130_RANKING_SYSTEM

	m_ItemOwnFlag.Init();

	GetWarehouseInventoryProtector().Init( this );

	m_EndInitCode = eVERIFY_MAGICCODE_INIT;

	m_bIsFallDead = FALSE;

	m_bCheckedStat = FALSE;

	m_PetManager.Init( this );
	m_EtherManager.Init( this );
	m_PlayerEvent.Init();

    // (WAVERIX) (090614) (BUG-FIX) �ʱ�ȭ �ϴ� ���� ���� ���� (����, �̰�... version-439 ��xx xxx ������� �׷�?)
    // (WAVERIX) (090614) (NOTE) �� ������ �������� �� ��.
	//if( Character::Init() )
	//{
	//	// �ʱ�ȭ�ɶ� ���� ���� ����ġ ����
	//	m_NextExp = 0;
	//	return TRUE;
	//}

#ifdef  __CN_OO0510__FATIGUE_MATCHING_UPDATE
	m_wPlayTime = 0;
	m_wOfflineTime = 0;
	m_byAge = 0;
	m_byRealNameAuth = 0;
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

	UnexpectedUserEnterRoom() = FALSE;

	m_GuildWarehouseRight = FALSE;
    etheria_control_option_.Clear();
	m_LoadingCheckTimer.Clear();
	m_listPlayerSummonInfo.clear();
	m_pCollectedList->Clear();
    m_pInvitateFriend->ClearAllFriendList();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    honor_manager_->init(this);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    point_wallet_manager_->init(this);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
    random_manager_->Init();
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    player_assist_meter->Init();
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST

    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.ns_trigger::TriggerPlayerLinkInGameSlots::~TriggerPlayerLinkInGameSlots();
    new (&trigger_links_) ns_trigger::TriggerPlayerLinkInGameSlots(this);
    new (&zone_transaction_info_) ns_object::PlayerZoneTransaction(this); // CHANGES: f110103.2L

    // (WAVERIX) (090614) (NOTE) �� ������ �������� �� ��.
    if(Character::Init())
    {
        // �ʱ�ȭ�ɶ� ���� ���� ����ġ ����
        m_NextExp = 0;
        return TRUE;
    }

	return FALSE;
}

VOID Player::Release()
{
    // CHANGES: f101115.3L
    CloseSharedTransactions();
    //
	RENDER_EFFECT.Clear();

	//SUNLOG( eFULL_LOG, "[%s]Release", GetCharName() );

    m_pWayPoint->Clear();
    m_pCharWayPoint->Clear();

	m_pChaoState->Init();
	m_pACManager->Release();
	m_pQuestManager->Release();
	m_pSlotManager->Release();
	m_pQuickManager->Release();
	//m_pStyleManager->Release();
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    perk_manager_->Reuse(NULL);
#endif
	m_pItemManager->Release();
	m_pSkillSlotManager->Release();

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    m_pAccumulatePoint->Release();
#endif

	// ��Ÿ�� ����
	FreeAllSkillCoolTime( TRUE );
	FreeItemCoolTime( TRUE );


    {   // (WAVERIX) (090607) (BUG-FIX) ������ �߸��� Ÿ�� �� ���� ���� �� Player�ϰ� ���ŷ� ����.
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
        DWORD summoner_key = GetSummonerKey();
        if(summoner_key != 0) // auto-relase key
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
            SummonManager::Instance()->LeaveSummonNPCs(this);
    }

    // (WAVERIX) (090619) (NOTE) �� ���� ��ġ�� �� �� ������ �ʿ� ����.
	Character::Release();
	m_Attr.Clear();

	//////////////////////////////////////////////////////////////////////////
	m_bExpiredPlayer = TRUE;
    m_PreVillageMapCode = 0;
    //_NA_0_20100722_AUTO_SYSTEM
    m_lastReturnScrollMapCode = 0;  
    ZeroMemory(&m_lastReturnScrollPos, sizeof(m_lastReturnScrollPos));


	SetBehave(PLAYER_BEHAVE_IDLE_STATE);

#ifdef _NA_008012_20150130_RANKING_SYSTEM
	SetRankingSystemCtl(NULL);
#endif // _NA_008012_20150130_RANKING_SYSTEM

	m_pPVPInfo->Init();

	m_pCompetitionInfo->Init();

	//m_dwLastSendTick	= GetTickCount();
	m_dwSentSize		= 0;
	m_bDoingAction		= FALSE;
	m_byVkrReloadCount	= MAX_VALKYRIE_RELOAD_COUNT;
	//m_ShapShotLogTimer.SetTimer( SNAPSHOT_LOG_CYCLE );

	m_byDebugInfoFlag = 0;

	//�߰� �̵��ӵ����� �⺻�� 1
	m_fAddMoveSpeedRatio = 1.0f;

	//SetMoveState( CMS_RUN );

	m_PlayerEvent.Release();

//	Destroy();
/*	SetServerSession( NULL );
	SetUserKey( 0 );
	SetState( PLAYER_DESTROYED );*/
	//////////////////////////////////////////////////////////////////////////
	m_LoadingCheckTimer.Clear();

	SSQHandler.Release();	// Init�� �ʿ�ÿ� ����ȴ�.
    Rider.Release();
    Wings.Release(); // NOTE: f110315.2L
	DestroySummonInfo();
	m_pCollectedList->Clear();
    // (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
    trigger_links_.Disconnect(NULL);
    //m_TriggerLink.Delete(); //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    new (&zone_transaction_info_) ns_object::PlayerZoneTransaction(this); // CHANGES: f110103.2L

    m_pInvitateFriend->ClearAllFriendList();
    // (NOTE) �� ������ ������ ��.
    m_StartInitCode = eVERIFY_MAGICCODE_RELEASE;
    m_EndInitCode = eVERIFY_MAGICCODE_RELEASE;
}

VOID Player::Create( ServerSession * pServerSession, DWORD UserKey, 
					const TCHAR * pszID, const CHAR * pszIP, 
					time_t LoginTime_t )
{
	ASSERT( pServerSession );
	ASSERT( pServerSession && pServerSession->GetServerType() == AGENT_SERVER );
	SetServerSession( pServerSession );
	SetUserKey( UserKey );
	SetState( PLAYER_CREATED );
	SetUserID( pszID );
	SetUserIP( pszIP );
	SetLoginTime_t( LoginTime_t );
}
//
//VOID Player::Destroy()
//{
//	SetServerSession( NULL );
//	SetUserKey( 0 );
//	SetState( PLAYER_DESTROYED );
//}

// CHANGES: f101115.3L, make an independent method
// for stable player closing process using shared data.
void Player::CloseSharedTransactions()
{
    const ePLAYER_BEHAVE_STATE behave_state = GetBehave();
    switch (behave_state)
    {
    case PLAYER_BEHAVE_TRADE_STATE:
        {
            SUNLOG(eFULL_LOG, "Release�ðŷ�����[%s]", GetCharName());
            Player* counter_player = GetTradeContainer()->GetCounterPlayer();
            if (counter_player->GetBehave() != PLAYER_BEHAVE_TRADE_STATE) {
                SUNLOG(eCRITICAL_LOG, "Release����2�ŷ�����[%s]�̻�", counter_player->GetCharName());
            }
            g_TradeManager.EndTrade(this);

            MSG_CG_TRADE_CANCEL_CMD cancel_cmd;
            counter_player->SendPacket(&cancel_cmd, sizeof(cancel_cmd));
        }
        break;
    case PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE:
        {
            RC::eVENDOR_RESULT result = g_VendorManager.EndVendor(this);
            if (result != RC::RC_VENDOR_SUCCESS) {
                SUNLOG(eCRITICAL_LOG, "Release�ù��End����[%s]", GetCharName());
            }
            MSG_CG_VENDOR_END_BRD msg_brd;
            msg_brd.m_dwPlayerKey = GetObjectKey();
            SendPacketAround(&msg_brd, sizeof(msg_brd), false);
        }
        break;
    case PLAYER_BEHAVE_VENDOR_OBSERVER_STATE:
        {
#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
            GAMELOG->LogItem(ITEM_VENDOR_VIEW_END, this, NULL);
#endif //_NA_000000_20131031_GAMELOG_REDUCE

            RC::eVENDOR_RESULT result = g_VendorManager.EndViewVendor(this);
            if (result != RC::RC_VENDOR_SUCCESS) {
                SUNLOG(eCRITICAL_LOG, "Release�ù����End����[%s]", GetCharName());
            }
        }
        break;
    };
    //
    if (m_pVendorPage != NULL)
    {
        RC::eVENDOR_LIST_RESULT result = g_VendorPageManager.EndVendorList(this);
        if (result != RC::RC_VENDOR_LIST_SUCCESS) {
            SUNLOG(eCRITICAL_LOG, "Release�� EndVendorList ����[%s]", GetCharName());
        }
    }

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    g_GameShopManager.OnPlayerDisconnect(this);
#endif
}

BOOL Player::Update(const util::ServerTimeInfoPerFrame* time_info_per_frame, BOOL bSectorUpdate)
{
	static Player* pTHIS = NULL;
	pTHIS = Player::VerifyNRestoreTHIS( this );
	if( !pTHIS )
		return FALSE;

	// ���� EnterField ���̸� Update�� �������� �ʴ´�.
	GameField* pField = GetField();
	if( !( pField && pField->IsReady() ) )
		return TRUE;

	// [SusunLove]
	static struct
	{
		eCHAR_TYPE	eCurType;
		DWORD		dwSummonerKey;
		eCHAO_STATE eCharState;
		LEVELTYPE	Level;
		DWORD		UserKey;
		eZONE_STATE eRoomState;
		DWORD		FieldPointer;
		MAPCODE		FieldCode;
		INT			SkillCount;
		DWORD		dwSectorIndex;
	} DEBUG_STK;

	DEBUG_STK.eCurType = GetCharType();
	DEBUG_STK.dwSummonerKey = GetSummonerKey();
	DEBUG_STK.eCharState = GetChaoState().GetChaoState();
	DEBUG_STK.Level = GetLevel();
	DEBUG_STK.UserKey = GetUserKey();
	DEBUG_STK.eRoomState = GetGameZoneState();
	DEBUG_STK.FieldPointer = static_cast<DWORD>(reinterpret_cast<__int64>(pField));
	DEBUG_STK.FieldCode = m_GameFieldCode;
	DEBUG_STK.SkillCount = GetSkillManager()->GetActiveSkillNum();
	DEBUG_STK.dwSectorIndex = GetSectorIndex();

	// <NOTICE WARNING>
	Character* pCHAR = dynamic_cast<Character*>(pTHIS);
    if (pCHAR->Update(time_info_per_frame->delta_tick_) == FALSE)
    {
        SUNLOG( eCRITICAL_LOG, "[%s:%d] Character::Update() fail", __FUNCTION__, __LINE__ );
        // character object released or delete requested
        return FALSE;
    }
	//

	if( m_DBPUpdateTimer.IsExpired(FALSE) )
	{
		/// DBProxy�� ������Ʈ ��Ŷ�� ������.
		SerializeInfoToDBProxy(UPDATE_FACTOR_CHAR_UPDATE);
	}

	m_pItemManager->Update();
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    perk_manager_->Update(time_info_per_frame);
#endif
	// �Ƿε� �ý���
	m_pFatigueManager->Update();

	// �������� ���� ������Ʈ
	m_pCompetitionInfo->Update();

	// ī������ ����
	m_pChaoState->Update();
	m_PetManager.Update();
	m_EtherManager.Update();
#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    m_pAccumulatePoint->Update();
#endif

	// �׼� ������ üũ
	if( IsDoingAction() && IsActionExpired() )
	{
		SetActionDelay( 0 );
	}

	//	(WAVERIX) (081223) (NOTE) ������ �����ߴ��� ���� ���� ������ �ʹ� ����������. �ϰ� ��� �����ϱ�� �Ѵ�.
	//		(WAVERIX) (081004) (NOTE) �� ��õ ����
	//	(PURPOSE) �÷��̾��� �̵� ���� ���� ó���� �̵� �߿� ����ϰ� �־ �̵� �Ϸ�/���� ���¿�����
	//	Sector������ �ȵ� �� �ִ� ����ȭ ���� ����
	BOOLEAN isMoved = FALSE;
	{	//	Move State Update
		WzVector prevPos = *GetPosPtr();
		SECTOR_IDX sectorIndexPrev = GetSectorIndex();

		PlanEvent->Update(time_info_per_frame->delta_tick_, FALSE );

		const WzVector* nextPos = GetPosPtr();
		isMoved = memcmp( &prevPos, nextPos, sizeof(prevPos) ) != 0;
		if( bSectorUpdate )
		{
			SECTOR_IDX sectorIndexLastest = pField->GetSectorIndex( nextPos );
			pField->CheckAndApplyChangeSector( this, sectorIndexLastest );
            // CHANGES: f110926.1L, 
            if (sectorIndexPrev != sectorIndexLastest) {
                this->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedPos);
            };
		}
	};

	if( isMoved )
	{	//	Move Constraints
		const ePLAYER_BEHAVE_STATE behaveState = this->GetBehave();
		switch( behaveState )
		{
		case PLAYER_BEHAVE_TRADE_STATE:
			{	//	�ŷ����� ��� ������ ��� ���
				MSG_CG_TRADE_CANCEL_CMD cmsg;
				Player* pPlayer = GetTradeContainer()->GetCounterPlayer();
				if( pPlayer->GetBehave() != PLAYER_BEHAVE_TRADE_STATE )
				{
					SUNLOG( eCRITICAL_LOG, "Move��TRADE_STATE�����̻�[1:%s][2:%s],%u", 
						GetCharName(), pPlayer->GetCharName(), pPlayer->GetBehave() );
				}
				else
				{	
					pPlayer->SendPacket( &cmsg, sizeof(cmsg) );
				}

				g_TradeManager.EndTrade( this );
				this->SendPacket( &cmsg, sizeof(cmsg) );
			}
			break;
		}
	};

    SendChangedStatus();

#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
    m_pMovementChecker->Update_Moving_Turn(time_info_per_frame->delta_tick_);
#endif

    Rider.Update(this, time_info_per_frame->delta_tick_);
    Wings.Update(this, *time_info_per_frame);

	// ������ �α� �߰�.
	// �����ð��� ������ �� �κ� ȣ��..
	if( m_ShapShotLogTimer.IsExpired() )
	{
		m_ShapShotLogTimer.Reset( SNAPSHOT_LOG_CYCLE );
		GAMELOG->LogSnapShot( this );
	}

    if (GetFatigueManager()->IsApplyFatigue() == TRUE)
    {
        if( m_FatigueLogTimer.IsExpired() )
        {
            m_FatigueLogTimer.Reset( FATIGUE_LOG_CYCLE );
            GAMELOG->LogFatigue( this );
        }
    }

	return TRUE;
}

//��������������������������������������������������������������������������������������������������������������������������������������������������
//	Packet Send, SendEx
//��������������������������������������������������������������������������������������������������
//
BOOL Player::SendPacket(MSG_BASE_FORWARD* forward_msg, WORD msg_size, BOOL need_compress)
{
    __TOUCHED(need_compress);
    forward_msg->m_dwKey = GetUserKey();

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    const size_t kMaxPacketSize =
        max(sizeof(MSG_CG_CHARINFO_CHAR_ITEM_CMD),
        max(sizeof(MSG_CG_CHARINFO_SKILL_CMD),
        max(sizeof(MSG_CG_CHARINFO_SKILL_PERK_CMD),
        max(sizeof(MSG_CG_AREACONQUEST_STREAM_CMD),
        max(sizeof(MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK), 0)))));
#else
    const size_t kMaxPacketSize =
        max(sizeof(MSG_CG_CHARINFO_CHAR_ITEM_CMD),
        max(sizeof(MSG_CG_CHARINFO_SKILL_CMD),
        max(sizeof(MSG_CG_CHARINFO_SKILL_PERK_CMD),
        max(sizeof(MSG_CG_AREACONQUEST_STREAM_CMD), 0))));
#endif
    //----------------------------------------------------------------------------------------------
    // for DEBUGGING
    {
        const DWORD curTick = GetTickCount();
        if (curTick - m_dwLastSendTick >= 1000)
        {
            if (m_dwSentSize > (kMaxPacketSize * 4))
            {
                SUNLOG(eCRITICAL_LOG,
                       "["__FUNCTION__"] C:P={%u:%u} %d bytes have sent per second. UserKey(%d)",
                       forward_msg->m_byCategory, forward_msg->m_byProtocol, m_dwSentSize, GetUserKey() );
            }
            m_dwSentSize = 0;
            m_dwLastSendTick = curTick;
        }
        m_dwSentSize += msg_size;
    };

    // 1) �� �ʿ��� �͸� Server::PacketControl�ʿ� ����ؼ� ����� ��
    // 2) Agent���� ���ŵ� �޽����� ������ ������ �Ϳ� ������� �� ��
    // 3) ���� ���鼭 ���� �޽��� �÷��̾�� �������� �� ��... �װ� ���� xxxAround ���� �Լ��� ����� ��
    nsPacket::PacketInfo packet_info = \
        nsPacket::Server::PacketControl::Pack(GAME_SERVER, AGENT_SERVER, forward_msg, msg_size);
    forward_msg = static_cast<MSG_BASE_FORWARD*>(packet_info.pMsg);
    msg_size = packet_info.Size;
    //
    //----------------------------------------------------------------------------------------------
    // for SENDING SIZE CONTROL
    if (msg_size > kMaxPacketSize)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] C:P={%u:%u} Packet Size[%d] Access!! UserKey[%u]", 
               forward_msg->m_byCategory, forward_msg->m_byProtocol, msg_size, GetUserKey());

        return false; // ...HUMMM...
    };
    //
    //----------------------------------------------------------------------------------------------
    if (GameField* const game_field = GetField()) {
        game_field->SendPacket(this, forward_msg, msg_size);
        return true;
    };
    //
    return m_pServerSession->Send((BYTE*)forward_msg, msg_size);
}

//��������������������������������������������������������������������������������������������������
//
BOOL Player::SendExPacket( DWORD dwNumberOfMessages, BYTE** pMsg, WORD* pwSize )
{
	MSG_BASE_FORWARD* pSendMsg = (MSG_BASE_FORWARD*)pMsg[0];
	pSendMsg->m_dwKey = GetUserKey();

	DWORD TotSize = 0;
	WORD* pwSizeOff = pwSize;
	for( DWORD dwIdx=0 ; dwIdx<dwNumberOfMessages ; ++dwIdx, ++pwSizeOff )
		TotSize += *pwSizeOff;
	if( TotSize > 12000 )
	{
		SUNLOG( eCRITICAL_LOG, "[SendExPacket] TotSize[%u] is Big!! UserKey[%u]C[%d]P[%d]", TotSize, GetUserKey(), pSendMsg->m_byCategory, pSendMsg->m_byProtocol );
	}

	GameField* const pGameField = GetField();
	return ( pGameField )
		?	pGameField->SendExPacket( this, dwNumberOfMessages, pMsg, pwSize ), TRUE
		:	m_pServerSession->SendEx( dwNumberOfMessages, pMsg, pwSize )
		;
}

//��������������������������������������������������������������������������������������������������
//
VOID Player::SendPacketAround( MSG_BASE_FORWARD * pMsg, WORD wSize, BOOL bSendToMe, GUILDGUID OtherGuildGuid)
{
	//if( m_pViewPort->IsUsingViewPort() )
	//{
	//	m_pViewPort->SendPacketAround( pMsg, wSize, bSendToMe, OtherGuildGuid );
	//}
	//else
	//{
		Character::SendPacketAround( pMsg, wSize, bSendToMe );
	//}
}

//��������������������������������������������������������������������������������������������������
//
VOID Player::SendExPacketAround( DWORD dwNumberOfMessages, BYTE** pMsg, WORD* pwSize, BOOL bSendToMe )
{
	//if( m_pViewPort->IsUsingViewPort() )
	//{
	//	m_pViewPort->SendExPacketAround( dwNumberOfMessages, pMsg, pwSize, !!bSendToMe, INVALID_GUILDGUID );
	//}
	//else
	//{
		Character::SendExPacketAround( dwNumberOfMessages, pMsg, pwSize, bSendToMe );
	//}
}


//��������������������������������������������������������������������������������������������������������������������������������
// CHANGES: f110531.4L, remove the broadcast upper boundary limitation using 1toN broadcaster
void Player::SendPacketForSync_(const BYTE _TAG, WAR_TAG_V* const IN msg, const WORD msg_size)
{
    GameField* const game_field = GetField();
    if (game_field == NULL) {
        return;
    };

    MSG_AG_CONNECTION_PACKET_UNICASTER& unicast = MSG_COMPOSE_UNICASTER::InstanceOfBase();
    MSG_CG_SYNC_COMPOSED_STREAM_CMD msg_cmd;
    msg_cmd.m_Header->size = msg_size;
    unicast.Init(&msg_cmd, sizeof(msg_cmd));
    msg->_TAG = _TAG;
    unicast.AppendMsg(reinterpret_cast<uint8_t*>(msg), msg_size);
    uint16_t* const user_begin = unicast.GetUsersStartPtr();
    unicast.AddUser(user_begin, this->GetObjectKey());
    this->SendPacket(&unicast, unicast.GetSendingSize());
    //
    return;
#if 0
	GameField* const pField = GetField();
	if( !pField )
		return;

	const DWORD object_key = GetObjectKey();
	MSG_AG_CONNECTION_PACKET_UNICASTER& rMsgUNI = MSG_COMPOSE_UNICASTER::InstanceOfBase();
	MSG_CG_SYNC_COMPOSED_STREAM_CMD msg;
	msg.m_Header->size = wSize;
	rMsgUNI.Init( &msg, sizeof(msg) );
	pMsg->_TAG = _TAG;
	rMsgUNI.AppendMsg( reinterpret_cast<BYTE*>(pMsg), wSize );
	DWORD* const pStartPtr	= rMsgUNI.GetUsersStartPtr();
	rMsgUNI.AddUser( pStartPtr, object_key );
	SendPacket( &rMsgUNI, rMsgUNI.GetSendingSize() );
#endif
}


//��������������������������������������������������������������������������������������������������
//
BOOL	Player::SendToGuildServer( MSG_BASE_FORWARD * pMsg, WORD wSize )
{
	ASSERT( g_pGameServer );
	pMsg->m_dwKey = GetUserKey();
	return g_pGameServer->SendToServer( GUILD_SERVER, pMsg, wSize );
}

//��������������������������������������������������������������������������������������������������
//
BOOL	Player::SendToGameDBPServer( MSG_BASE_FORWARD * pMsg, WORD wSize )
{
	ASSERT( g_pGameServer );
	pMsg->m_dwKey = GetUserKey();
	return g_pGameServer->SendToServer( GAME_DBPROXY, pMsg, wSize );
}

//
//��������������������������������������������������������������������������������������������������������������������������������������������������

VOID Player::ForceDisconnect( BYTE Reason )
{
	MSG_AG_CONNECTION_UNREGISTER_CMD msg;
	msg.m_byDisonnectCode = Reason;
	SendPacket( &msg, sizeof(msg) );
}

//-------------------------------------------------------------------------------------------------
// ���� ��ȭ
//-------------------------------------------------------------------------------------------------
// !!!!!!
#include "PVPManager.h"
#include "GamePVPRoom.h"

// below interfaces is extracted from the SCPlayer

// Item Cool Timer
void Player::SetItemCoolTime(BOOL bPoolUse, DWORD cool_time_margin)
{
    // ��Ÿ�� Ÿ�̸� �ؽ� ���̺� ����
    CoolTimerFactory* cool_timer_factory = CoolTimerFactory::Instance();
    ItemInfoParser* item_parser = ItemInfoParser::Instance();
    const ITEM_COOLTIME_MAP* script_item_cool_time_table = item_parser->GetItemCoolTimeMap();
    ITEM_COOLTIME_MAP::const_iterator it = script_item_cool_time_table->begin(),
                                      end = script_item_cool_time_table->end();
    for ( ; it != end; ++it)
    {
        const ITEM_COOLTIME_INFO& cool_time_info = it->second;
        util::Timer* timer = cool_timer_factory->Alloc();
        DWORD interval = cool_time_info.m_dwCoolTime > cool_time_margin
                       ?    cool_time_info.m_dwCoolTime - cool_time_margin
                       :    cool_time_info.m_dwCoolTime;
        timer->SetTimer(interval);
        timer->InitCoolTime();
        item_cool_time_table_[cool_time_info.m_wCoolTimeType] = timer;
    }
}

void Player::FreeItemCoolTime(BOOL bPoolUse)
{
    CoolTimerFactory* cool_timer_factory = CoolTimerFactory::Instance();
    FOREACH_CONTAINER(const COOL_TIME_TABLE::value_type& node, item_cool_time_table_,
                      COOL_TIME_TABLE)
    {
        util::Timer* timer = node.second;
        cool_timer_factory->Free(timer);
    }
    item_cool_time_table_.clear();
}

util::Timer* Player::GetItemCoolTimer(WORD wType)
{
    COOL_TIME_TABLE::const_iterator found = item_cool_time_table_.find(wType);
    if (found == item_cool_time_table_.end()) {
        return NULL;
    }
    util::Timer* timer = found->second;
    return timer;
};

void Player::InitializeItemCooltime()
{
    FOREACH_CONTAINER(const COOL_TIME_TABLE::value_type& node, item_cool_time_table_,
                      COOL_TIME_TABLE)
    {
        util::Timer* timer = node.second;
        timer->InitCoolTime();
    }
}

VOID
Player::SetBehave( ePLAYER_BEHAVE_STATE state )
{
	m_BehaveState = state;
	// Agent�� �˸�	
	MSG_AG_STATUS_CHANGE_STATE_CMD cmsg;
	cmsg.m_eBehaveState = m_BehaveState;
	SendPacket( &cmsg, sizeof(cmsg) );
}

nsSync::CSSyncTuner* Player::GetSyncTuner() {
    return m_pMovementChecker->GetSyncTuner();
}

VOID
Player::OnDead()
{
	//	(WAVERIX)(080910)(NOTE)	����� �������������Ƿ� �����Ѵ�. �ش� ��ũ�δ� �������� ������ ����
	//	- (Addin) �ߺ� ó���Ǵ� �κ� ����, Ÿ���� ��� �ڵ� �߰�
	const DWORD DEFAULT_SKIP = 10;

	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
		
		const DWORD object_key = GetObjectKey();
		const DWORD curHP = GetHP();
		SASSERT( curHP == 0, "[Player::OnDead] GetHP() != 0" ); //... �̰��� ���� ó������ �ʰ� �ִ�.

		Character::OnDead();

		{
			// �׾����� ������ HP�� 0���� �Ѵ�.
			SetDeadReserveHP( GetReserveHP() ); //���� ������ HP�� 50%�� �д�.
			SetReserveHP(0);
		};

		{	//	��Ÿ ���� ���ӷ��� ó�� (�ֺ��� �˸��� ���� ����)
			//	#1) ��ȯü�� �����Ѵ�.
			SummonManager::Instance()->LeaveSummonNPCs( this );
			//	#2) __NA_0_PET_SYSTEM
			m_PetManager.OnDead();
		};

		{   //	Agent�� �˸�
			MSG_AG_STATUS_DEAD_CMD msgCMD;
			SendPacket( &msgCMD, sizeof(msgCMD) );
		};

		//	�÷��̾� ��� �α�
		MAPCODE mapcode = 0;
		if( GetGameZonePtr() )
			mapcode = GetGameZonePtr()->GetMapCode();
		GAMELOG->WritePlayerDead( this, mapcode );
		//	LAST!!!
		GameZone* const pGameZone = GetGameZonePtr();
		GameField* const pGameField	= GetField();
		if( !(pGameZone && pGameField) )
			break;

		//	Field���� ��Ÿ �ڵ��.
		const eZONETYPE zone_type = pGameZone->GetZoneType();
		switch( zone_type )
		{
		case eZONETYPE_PVP:
			{
				//	(OLD COMMENT)	!!!!!!!�ӽ�!!!!!!!!
				//	(WAVERIX)(080910)(NOTE)	���� �ӽ��̱� �ٶ�����, ���� ����ϰ� �ִ� �ڵ��.-.-;
				if( ePRS_AT_PVP == GetGameZoneState() )
				{
					GamePVPRoom* const pPVPRoom = static_cast<GamePVPRoom *>( GetGameZonePtr() );
					Character* const pKiller = pGameField->FindCharacter( m_dwKillerObjectKey );
					pPVPRoom->GetPVPManager()->OnPVPResult( pKiller, this );
				}
			}
			break;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        case eZONETYPE_INSTANCE:
            {
                if(ePRS_AT_INSTANCE == GetGameZoneState() && GetField())
                {
                    Character* const killer = GetField()->FindCharacter(m_dwKillerObjectKey);
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
                    ChaosZoneManager* chaoszone_manager = GetChaosZoneManager();
                    if (chaoszone_manager != NULL) {
                        chaoszone_manager->DeadCharacter(this, killer);
                    }
#else
                    if (IsThereBattleGroundZone() == true)
                    {
                        BattleGround* const battle_ground = \
                            static_cast<BattleGround*>(GetCurrentChaosZone());
                        if (battle_ground) {
                            battle_ground->DeadCharacter(this, killer);
                        }
                    }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
                }
            }
            break;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
		//	(WAVERIX)(080910)(NOTE)
		//	��Ÿ Ư�� �̺�Ʈ�� ���⿡ �� ��. �̷��� �ϴ� ����... �Լ� �б⵵ ���߿� ���� ���� �������� �ϱ� �����.
		//	���� ���⿡ Ư�� ��ʷ� �ɱ� �����ϸ� �װ� ���� ������������ ���� ���� ���������̴�.
		//	���⼭ Ư�� �̺�Ʈ�� �ߵ���ų ���� �Ϲ�ȭ�� ȣ���� ������ �� �ֵ��� ����.
		//case 
		}

		if( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD_PLAYER )
		{
			if( zone_type != eZONETYPE_INSTANCE )
				break;

			Character* const pCharacter = pGameField->FindCharacter( m_dwKillerObjectKey );
			GameWarControlManager::OnMsg(
									GameSSQCtrlCmd::CharacterEvent::OnDead( this, pCharacter ) // no-null check
									, static_cast<GameInstanceDungeon*>(pGameZone)
									);
		}

		return;
    }
    FLOWCONTROL_END;

	return;
}

//-------------------------------------------------------------------------------------------------
// ��ġ ��ȭ
//-------------------------------------------------------------------------------------------------

BOOL
Player::ExecuteThrust( BOOL bForcedMove, WzVector *pwvMoveDistance, WzVector &wvPosAfterThrust, FLOAT fMoveDistance, BOOL bDownAfterThrust )
{
	GameField *pField = GetField();
	if( !pField )	
	{
		return FALSE;
	}

	// TODO: �и��� ������ ���� ��� ������ �����. ��������??
	if (pwvMoveDistance->x == 0.0f && pwvMoveDistance->y == 0.0f )
	{
		pwvMoveDistance->x = 1.0f;
		WzVector wvTemp = *pwvMoveDistance;
		Math_ZRotateVector( &wvTemp, *pwvMoveDistance, rand() % 360 );
		*pwvMoveDistance = *pwvMoveDistance * fMoveDistance;
	}

	// Player �и��� ó��
	if( !pField->FindPathThrust( this, pwvMoveDistance, bForcedMove ) )
	{
		return FALSE;
	}

	wvPosAfterThrust = GetPathExplorer()->GetTargetPos();
	SetMoveState( CMS_SWIPE );

	// �̵��� üũ�� ���� ����
	WzVector vCurPos;	GetPos( &vCurPos );
	MovementChecker* pCHECKER = GetMovementChecker();
	pCHECKER->GetServerPathExpUpdate()->SetupLatestState( pCHECKER, .0f, 0, vCurPos, vCurPos );

	return TRUE;
}

//-------------------------------------------------------------------------------------------------
// ������ ����
//-------------------------------------------------------------------------------------------------
RC::eITEM_RESULT Player::CanUseItem(const BASE_ITEMINFO* pItemInfo)
{
	if( !pItemInfo )
		return RC::RC_ITEM_COOLTIME_ERROR;

	if( GetField()==NULL )
	{
		return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
	}

	if( !GetStatusManager()->Status.IsPassConstraint( this, pItemInfo ) )
		return RC::RC_ITEM_UNUSABLE_FUNCTION;

	// ��Ÿ���� ���� ��� üũ
	util::Timer * pTimer = GetItemCoolTimer( pItemInfo->m_wCoolTimeType );
	if( pTimer )
	{
		if( !pTimer->IsExpired( FALSE ) )
		{
			return RC::RC_ITEM_COOLTIME_ERROR;
		}
	}

    //__CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
	if( pItemInfo->m_LimitUseLevel != 0 && GetLevel() < pItemInfo->m_LimitUseLevel )
		return RC::RC_ITEM_CANNOT_USE_PLAYER_LEVEL;

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(!GetBetaKeyInfo()->CanAbilityContent(pItemInfo->m_betakey))
    {
        return RC::RC_ITEM_CANNOT_USE_BETAKEY;
    }
#endif

	return RC::RC_ITEM_SUCCESS;
}

VOID Player::ResetUseItem( WORD wCoolTimeType )
{
	util::Timer * pTimer = GetItemCoolTimer( wCoolTimeType );
	if( pTimer )
	{
		pTimer->Reset();
	}
}


VOID Player::SetActionDelay( DWORD delay )
{
	m_ActionTimer.SetTimer( delay );

	if( delay )
	{
		m_bDoingAction = TRUE;
	}
	else if( m_bDoingAction )
	{ 
		m_bDoingAction = FALSE;

		MSG_CG_SYNC_ACTION_EXPIRED_CMD msg;
		SendPacket( &msg, sizeof(msg) );
	}
}

DWORD Player::GetActionDelay()
{
	return m_ActionTimer.GetIntervalTime();
}

BOOL Player::IsActionExpired()
{
	if( m_ActionTimer.IsExpired() )
	{
		return TRUE;
	}

	return FALSE;
}

VOID Player::ApplyPenalty()
{
	//���� 10���ϴ� �г�ġ ���� ����
	if( GetLevel() < MIN_PENALTY_LEVEL ) return;

	// Zone�� ���� �г�Ƽ�� �� �޴� ��찡 �ִ�.
	GameZone *pZone = GetGameZonePtr();
	if( !pZone ) return;

	// ������ ����
	FLOAT fDuraDecRatio = 0;
	if( ePRS_AT_CHUNTING == GetGameZoneState() )
	{
		GameCHuntingRoom *pCHunting = (GameCHuntingRoom*)GetGameZonePtr();
		fDuraDecRatio = GetDuraDecRatio( GetGameZoneState(), (eCOMPETITION_MODE_TYPE)pCHunting->GetAdditionalCHuntingInfo()->m_Mode );
	}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    else if (ePRS_AT_INSTANCE == GetGameZoneState())
    {
        GameInstanceDungeon* const instance_dugeon = 
            static_cast<GameInstanceDungeon*>(GetGameZonePtr());
        fDuraDecRatio = GetDuraDecRatio(GetGameZoneState(), instance_dugeon->GetChaosZoneType());
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	else
	{
		fDuraDecRatio = GetDuraDecRatio( GetGameZoneState(), (eCOMPETITION_MODE_TYPE)0);
	}

    if (nsSlot::DurabilityImpl* const impl = this->GetItemManager()->GetDurabilityImpl()) {
        impl->DecreaseDuraAll(fDuraDecRatio);
    };
}

BOOL Player::IsPvPAtt(WORD PTATroubld)
{
	if( !GetGameZonePtr() ) return FALSE;
	
	return GetGameZonePtr()->IsPvPAtt(this,PTATroubld);
}

//_NA_008706_20160229_TEXTLOG_PK
VOID Player::KillPlayer(Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/)
{
	if( !GetGameZonePtr() ) return;

	GetGameZonePtr()->KillPlayer(this,pDeadPlayer,bSummoned, damage_value);
}

//���� ���� äũ. 
//
RC::eBATTLE_RESULT Player::ChkAttackPvP( Character* pTargetChar )
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    if (pTargetChar == NULL) {
        return RC::RC_BATTLE_FAILED;
    }
    if (pTargetChar->IsEqualObjectKind(SUMMON_OBJECT)) {
        Character* const summon_owner = GetField()->FindCharacter(pTargetChar->GetSummonerKey());
        return ChkAttackPvP(summon_owner);
    }
#else
	// ��ȯü ��ó��
	SummonedNPCs *pSummonedNPCs = SummonManager::Instance()->FindSummonNPCs( pTargetChar->GetSummonerKey() );
	if( pSummonedNPCs )
	{
		// Ÿ���� ��ȯ�ڱ��� üũ
		Character *pSummoner = pSummonedNPCs->GetSummoner();
		if( pSummoner && pTargetChar != pSummoner )
		{
			return ChkAttackPvP( pSummoner );
		}
	}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	//.//

	RC::eBATTLE_RESULT ErrCode = RC::RC_BATTLE_SUCCESS;

	if(pTargetChar->IsEqualObjectKind(PLAYER_OBJECT))
	{
		// ��ȯ���� �ϰ� ���� ���� ��� �����ڰ� ��ȿ�� ���� ������ �ִ�.
		if( !GetGameZonePtr() ) 
			return RC::RC_BATTLE_FAILED;

		// �� ���� �����ϴ� �꿡 ������.

		GetGameZonePtr()->CanPVP(this,(Player*)pTargetChar,&ErrCode);
	}

	return ErrCode;
}


RC::eSKILL_RESULT Player::ChkSkillPvP( Character* pTargetChar )
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    if (pTargetChar->IsEqualObjectKind(SUMMON_OBJECT)) {
        Character* const summon_owner = GetField()->FindCharacter(pTargetChar->GetSummonerKey());
        if (summon_owner)
        {
            return ChkSkillPvP(summon_owner);
        }
    }
#else
	// ��ȯü ��ó��
	SummonedNPCs *pSummonedNPCs = SummonManager::Instance()->FindSummonNPCs( pTargetChar->GetSummonerKey() );
	if( pSummonedNPCs )
	{
		// Ÿ���� ��ȯ�ڱ��� üũ
		Character *pSummoner = pSummonedNPCs->GetSummoner();
		if( pSummoner && pTargetChar != pSummoner )
		{
			return ChkSkillPvP( pSummoner );
		}
	}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	//.//

	RC::eSKILL_RESULT ErrCode = RC::RC_SKILL_SUCCESS;

	if(pTargetChar->IsEqualObjectKind(PLAYER_OBJECT))
	{
		// ��ȯ���� �ϰ� ���� ���� ��� �����ڰ� ��ȿ�� ���� ������ �ִ�.
		if( !GetGameZonePtr() ) 
			return RC::RC_SKILL_FAILED;

		// �� ���� �����ϴ� �꿡 ������.

		GetGameZonePtr()->CanPVP(this,(Player*)pTargetChar,&ErrCode);
	}

	return ErrCode;
}

/*
// CHANGES, f100828.4L, removed
BASE_PLAYERINFO* Player::GetCharInfo()
{
	m_Attr.GetCharInfo( GetCharInfo() );
	return GetCharInfo();
}
*/

eUSER_RELATION_TYPE Player::IsFriend( Character *pTarget )
{
	if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return USER_RELATION_NEUTRALIST;
    }

	// �������� üũ�Ѵ�.
	if( GetObjectKey() == pTarget->GetObjectKey() )	return USER_RELATION_FRIEND;
	if( this == pTarget )	return USER_RELATION_FRIEND;

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	// ��ȯü ��ó��
	SummonedNPCs *pSummonedNPCs = SummonManager::Instance()->FindSummonNPCs( pTarget->GetSummonerKey() );
	if( pSummonedNPCs )
	{
		// Ÿ���� ��ȯ�ڰ� �Ʊ��̶�� Ÿ�ٵ� �Ʊ��� �ȴ�.
		Character *pSummoner = pSummonedNPCs->GetSummoner();
		if( pSummoner && pTarget != pSummoner )
		{
			return IsFriend( pSummoner );
		}
	}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	if( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD )
	{
		//	SSQMonster�� ���͵� ������ ������ �ν��Ѵ�.
		//	Ʈ�� �迭 NPC���� �����ϴ� ���� �����ϱ� ���ؼ� �̵鸸 ģ���� �ν��ϵ��� �Ѵ�.
        if(pTarget->IsEqualObjectKind(MAPNPC_OBJECT))
            return USER_RELATION_FRIEND;
        if(!pTarget->IsEqualObjectKind(SUMMON_OBJECT))
		    return USER_RELATION_ENEMY;
	}
	//.//

	eOBJECT_TYPE eObjectType = pTarget->GetObjectType();

	switch(eObjectType)
	{
		//.// ����
	case MONSTER_OBJECT:
	case LUCKYMON_OBJECT:
	case SSQMONSTER_OBJECT:
	case TOTEMNPC_OBJECT:	// ��ȯü ��ó���� ���� ��Ȳ.
		return USER_RELATION_ENEMY;
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    case SUMMON_OBJECT:
        {
            Character* summon_owner = NULL;
            GameField* game_field = GetField();
            DWORD summoner_key = pTarget->GetSummonerKey();
            if( game_field == NULL ) {
                summon_owner = PlayerManager::Instance()->FindPlayerByObjKey(summoner_key);
            }
            else {
                summon_owner = game_field->FindCharacter(pTarget->GetSummonerKey());
            }

            if (summon_owner != NULL) {
                return IsFriend(summon_owner);
            }
        } break;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
    case MAPNPC_OBJECT:
    case CRYSTALWARP_OBJECT:
        return pTarget->IsFriend(this); // ������ ȣ��� ���� ��� ���ɼ� �ִ�.
    case FRIEND_MONSTER_OBJECT:
        return USER_RELATION_FRIEND;
    case SYNC_MERCHANT_OBJECT:
        return USER_RELATION_NEUTRALIST;
		//.// PVP
	case PLAYER_OBJECT: 
		{
			// ��ȯ���� �ϰ� ���� ���� ��� �����ڰ� ��ȿ�� ���� ������ �ִ�.
			if( !GetGameZonePtr() ) 
				return USER_RELATION_NEUTRALIST;

			// �� ���� �����ϴ� �꿡 ������.
			return GetGameZonePtr()->IsFriendPVP(this,(Player*)pTarget);
		}
		break;

	default:
		SUNLOG( eCRITICAL_LOG, "Player::IsFriend  Target Ÿ��[%d]", eObjectType );
		break;
	}

	return USER_RELATION_NEUTRALIST;
}

VOID Player::SetDBCharState( eDBCHAR_STATE stat, WORD wMinutes )
{
	/*
	GetCharInfo()->m_byCharState = stat;
	switch( stat )
	{
	case DBCHAR_STATE_CHAR_BLOCK:
		{
			//WZTime::GetTime( 3, GetCharInfo()->m_tszStateTime, MAX_TIMEDATA_SIZE );
			//�߰����� �ð� ���
		}
		break;
	}
	*/
}
CWarehouseSlotContainer * Player::GetWarehouseContainer() const
{ 
	return (CWarehouseSlotContainer *)GetItemManager()->GetItemSlotContainer(SI_WAREHOUSE);	
}

CGuildWarehouseSlotContainer * Player::GetGuildWarehouseContainer()
{ 
	return (CGuildWarehouseSlotContainer *)GetItemManager()->GetItemSlotContainer(SI_GUILDWAREHOUSE);	
}

//��Ȱ
BOOL Player::OnResurrection( FLOAT fRecoverExp, FLOAT fRecoverHP, FLOAT fRecoverMP, Player *pHealer )
{
	if( Character::OnResurrection( fRecoverExp, fRecoverHP, fRecoverMP, pHealer ) )
	{
		if( GetCharType() == eCHAR_BERSERKER )
		{
			SetMP( 0 );
		}	
        else if(GetCharType() == eCHAR_DRAGON)
        {
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
            // �뺯�� ���¿��� ������ 
            // �뺯���� Ǯ���鼭 HP �� �� ����� �Ǿ�� �ϴµ� �̹� ���� ���¿��� HP�� ����� ���� ����
            // ��Ȱ�� ������ ����� �ѹ��� ����
            if (ItemManager* item_manager = GetItemManager())
            {
                item_manager->UpdateItemAttributes(true, false);
            }
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
        }	

		//������ ���� ���� ������ �������뿡�� ��Ȱ�̳�, ������ ��Ȱ�̴�... �̰��� ��Ŀ������ �ش�.
		if( !pHealer )
		{
			DWORD dwReserveHP = GetHP() / 2 + GetDeadReserveHP() / 2;
			if( dwReserveHP >= ( GetMaxHP() - GetHP() ) )
				SetReserveHP( GetMaxHP() - GetHP() );
			else
				SetReserveHP( dwReserveHP  );
		}

		SetDeadReserveHP( 0 );

        //{__NA_001290_20090525_SHIELD_SYSTEM
        const DWORD curSD = GetSD();
        if( curSD )
        {
            ALLOC_STATUS_PAIR pairBaseStatus = m_pStatusManager->AllocStatus( eCHAR_STATE_ETC_EXIST_SHELD_POINT );        
            if( pairBaseStatus.second )
                pairBaseStatus.second->SendStatusAddBRD();
        }
        //}
		// ��ö ��� ����.
        // ��ö ������ ��������� ���� ���� ���� �̹Ƿ� SendStatusAddBRD�� ���� brd�Ѵ�. 
        // �ڻ�(���������̵�) -> �������� -> OnResurrection
        
        int steel_time = REBIRTH_STEEL_MODE_TIME;  

        // ���������� ��ö ���� �ǿ����� ����.
        if (GetGameZonePtr()->GetZoneType() == eZONETYPE_DOMINATION_FIELD)  
        {
            steel_time = DOMINATION_REBIRTH_STEEL_MODE_TIME;
        }
        
        // ���忡���� ��ö ��带 ��� ���� �ʴ´�.(���������)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        if (GetChaosZoneType() == kBattleGround || GetChaosZoneType() == eCHAOSZONE_TYPE_GOLDRUSH) {
            steel_time = 0;
        }
#else
        if (IsThereBattleGroundZone()) {
            steel_time = 0;
        }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

        if (steel_time > 0) 
        {
            ALLOC_STATUS_PAIR steel = 
                m_pStatusManager->AllocStatus(eCHAR_STATE_STEEL, steel_time);        
            if (steel.second)
                steel.second->SendStatusAddBRD();
#ifdef _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : AllocStatus(eCHAR_STATE_STEEL)");
#endif // _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG
        }

        //
		// �÷��̾� ��Ȱ �α� �����
		if( GetField() && GetField()->GetGameZone() )
		{
			GAMELOG->WritePlayerResurrection( this, pHealer );
		}
		// �÷��̾� ��Ȱ �α� ����� ��


		// ��Ƽ�� ���� �ǹǷ� Agent�� ������ ó���Ѵ�.
		MSG_AG_STATUS_RESURRECTION_CMD cmdMsg;
		cmdMsg.m_dwCurHP = GetHP();
		cmdMsg.m_dwCurMP = GetMP();
        cmdMsg.m_dwCurSD = GetSD();
        NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
        SendPacket( &cmdMsg, sizeof(cmdMsg) );

        // (WAVERIX) (090506) (BUG-FIX) DEAD�� RESURRECTION�� ��Ŷ ���� ����
        {
            MSGSUB_SYNC_CHARACTER_RESURRECTION_BRD msg;
            msg.m_dwObjectKey = GetObjectKey();
            msg.m_dwHP = GetHP();
            msg.m_dwMP = GetMP();
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            msg.m_dwSD = GetSD();
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            msg.m_wvCurPos = *GetPosPtr();
            msg.m_dwReserveHP = GetReserveHP();
            SendPacketAroundForSync(&msg, TRUE);
        };
	//	// ��Ȱ�� �˷�����.
	//	MSG_CG_STATUS_RESURRECTION_BRD sendMsg;
	//	sendMsg.m_dwObjectKey = GetObjectKey();
	//	sendMsg.m_dwHP = GetHP();
	//	sendMsg.m_dwMP = GetMP();
	//	GetPos(&sendMsg.m_wvCurPos);
	//	sendMsg.m_dwReserveHP = GetReserveHP();
	//	SendPacketAround( &sendMsg, sizeof(sendMsg) );

		return TRUE;
	}
	else
	{
		MSG_CG_STATUS_RESURRECTION_NAK NakMsg;
		NakMsg.m_dwObjectKey = GetObjectKey();
		NakMsg.m_byErrorCode = 0;		// Error Code ���� �ʿ�
		SendPacket( &NakMsg, sizeof(NakMsg) );
	}

	return FALSE;
}


FLOAT Player::GetHPRatio() const
{
	if( m_Attr.GetMaxHP() <= 0 )
		return 0;
		 
	return (FLOAT)char_info_.m_iHP / m_Attr.GetMaxHP();
}

FLOAT Player::GetMPRatio() const
{
	if( m_Attr.GetMaxMP() <= 0 )
		return 0;

	return (FLOAT)char_info_.m_iMP / m_Attr.GetMaxMP();
}

FLOAT Player::GetSDRatio() const
{
    if( m_Attr.GetMaxSD() <= 0 )
        return 0;

    return (FLOAT)char_info_.m_iSD / m_Attr.GetMaxSD();
}

BOOL Player::CanUsePotion()
{
	// �������� ���� ���Ƚ�� ����
	if( ePRS_AT_CHUNTING == GetGameZoneState() )
	{
		GameCHuntingRoom *pCHuntingRoom = (GameCHuntingRoom *)GetGameZonePtr();
		if( pCHuntingRoom )
        {
			if( !pCHuntingRoom->GetCHuntingManager()->CanUsePotion( this ) )
				return FALSE;
		}
	}
	return TRUE;
}

VOID Player::UseHPPotion( INT recoverHP, INT iApplicationTime, INT iPeriodicTime )
{
	// ���� ���Ƚ��
	if( ePRS_AT_CHUNTING == GetGameZoneState() )
	{
		GameCHuntingRoom *pCHuntingRoom = (GameCHuntingRoom *)GetGameZonePtr();
		if( pCHuntingRoom )
			pCHuntingRoom->GetCHuntingManager()->UsePotion( this );
	}

    // ��� ȸ�� ������ ����ߴٸ�
    const bool is_recover_instantly = (iApplicationTime == 1 && iPeriodicTime == 1);
    if (is_recover_instantly)
    {
        // ��� ȸ�� ���¸� ����Ѵ�.
        PeriodicRecover(eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY, recoverHP, 0, iApplicationTime, iPeriodicTime);
        return;
    }

	PeriodicRecover( eCHAR_STATE_ETC_ITEM_RECOVER_HP, recoverHP, 0, iApplicationTime, iPeriodicTime );
}

VOID Player::UseMPPotion( INT recoverMP, INT iApplicationTime, INT iPeriodicTime )
{
	// ���� ���Ƚ��
	if( ePRS_AT_CHUNTING == GetGameZoneState() )
	{
		GameCHuntingRoom *pCHuntingRoom = (GameCHuntingRoom *)GetGameZonePtr();
		pCHuntingRoom->GetCHuntingManager()->UsePotion( this );
	}

	PeriodicRecover( eCHAR_STATE_ETC_ITEM_RECOVER_MP, 0, recoverMP, iApplicationTime, iPeriodicTime );
}

VOID Player::StatPointUpItemUse()
{
	BASE_PLAYERINFO * pBasePlayerInfo = this->GetCharInfo();
	pBasePlayerInfo->m_iRemainStat += 5;

	//����
	UpdateCharStat();

	//Player���� ���� ������ �뺸�Ѵ�.
	MSG_CG_ITEM_STAT_ONE_CHANGE_CMD CmdMsg;		
	CmdMsg.m_AttrType		= eATTR_TYPE_INVALID;		//<��� ����
	CmdMsg.m_dwValue		= 0;						//<��� ����
	CmdMsg.m_iRemainStat	= GetRemainStatPoint();
	SendPacket( &CmdMsg, sizeof(CmdMsg) );
}

VOID Player::SetPreVillageMapCode( MAPCODE mapCode )
{
	GetCharInfo()->m_iPreRegion = (INT)mapCode;
}

MAPCODE	Player::GetPreVillageMapCode()
{
	return (MAPCODE)GetCharInfo()->m_iPreRegion;
}

//_NA_0_20100722_AUTO_SYSTEM
VOID Player::SetlastReturnScroll()
{
    GameField* game_field = GetField();
    if( game_field == NULL )
        return;

    m_lastReturnScrollMapCode = game_field->GetFieldCode();
    GetPos(&m_lastReturnScrollPos);
}

RC::ePORTAL_RESULT Player::MovelastReturnScroll()
{
    RC::ePORTAL_RESULT retValue = CanPortal();
    if( RC::RC_PORTAL_CAN_SUCCESS == retValue )
    {
        retValue = Portal(ePORTAL_TYPE_ITEM_EX, m_lastReturnScrollMapCode, m_lastReturnScrollPos, 0);
    }

    return retValue;
}


VOID Player::SetStartMapCode( MAPCODE mapCode )
{
	GetCharInfo()->m_iRegion = (INT)mapCode;
}

MAPCODE	Player::GetStartMapCode()
{
	return (MAPCODE)GetCharInfo()->m_iRegion;
}

RC::ePORTAL_RESULT Player::CanPortal()
{
    if( !GetField() )	
        return RC::RC_PORTAL_FIELD_IS_NULL;

    GameZone* pGameZone = GetGameZonePtr();
    if( !pGameZone )
        return RC::RC_PORTAL_FIELD_IS_NULL;

    eZONETYPE ZoneType = pGameZone->GetZoneType();

    if( ZoneType != eZONETYPE_VILLAGE	&&
        ZoneType != eZONETYPE_FIELD		&&
        ZoneType != eZONETYPE_DOMINATION_FIELD &&
        ZoneType != eZONETYPE_SPA_FIELD)    //_NA_008069_20150210_ADD_SPA_SYSTEM
    {
        return RC::RC_PORTAL_UNABLE_CUR_ZONE;
    }

    // ���� â�� ���� �ִ� ���¸� ����ó��..
    CWarehouseSlotContainer* pWareHouse = GetWarehouseContainer();
    if( !pWareHouse || pWareHouse->IsStarted() )
        return RC::RC_PORTAL_INVALIDSTATE;

    // (lst1024)(090627) �߱� ��� â�� �̿��� ���� ���� ����
    // ���� ���â�� ���� �ִ� ���¸� ����ó��..
    CGuildWarehouseSlotContainer* pGuildWareHouse = GetGuildWarehouseContainer();
    if( pGuildWareHouse && pGuildWareHouse->IsOpened() )
        return RC::RC_PORTAL_INVALIDSTATE;

    // �÷��̾��� ���°� �⺻���°� �ƴϸ�( ��:���� ���λ����� ������ ���µ� )
    const ePLAYER_BEHAVE_STATE state = GetBehave();
    if( PLAYER_BEHAVE_IDLE_STATE != state )
        return RC::RC_PORTAL_INVALIDSTATE;

    return RC::RC_PORTAL_CAN_SUCCESS;
}

RC::ePORTAL_RESULT Player::Portal( ePORTAL_TYPE ePortalType, FIELDCODE ToFieldCode, WzVector & vDestPos, POSTYPE atItemPos )
{
	if( !GetField() )	return RC::RC_PORTAL_FAIL;

    // add rule by __NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP
    if( ePortalType != ePORTAL_TYPE_GM )
    {
        if( ToFieldCode == HardCode::MAPCODE_for_GM ||
            ToFieldCode == HardCode::MAPCODE_for_TUTORIAL_FIELD  )
        {
           return RC::RC_PORTAL_UNABLE_DEST_ZONE;
        }
    }

	GameZone* pZone = g_GameZoneManager.GetFieldVillageZone( ToFieldCode ); 
	if( !pZone )	return RC::RC_PORTAL_FAIL; 

	// �̵��Ϸ��� �ʵ带 ã�´�.
	GameField *pToField = pZone->FindGameField( ToFieldCode );
	if( !pToField )	return RC::RC_PORTAL_FAIL;

	// �̵������� ��ǥ�� �ƴϸ� �̵������� �ֺ� ��ǥ�� ã�´�.
	if( vDestPos.x != 0 || vDestPos.y != 0 || vDestPos.z != 0 )
	{
		if( !pToField->IsMovableCoordinates( &vDestPos ) )
		{
			if( !pToField->GetRandomPosInAround( &vDestPos, 7, vDestPos, 30 ) )
				return RC::RC_PORTAL_FAIL;
		}
	}

	// ���� �ʵ� �������� �̵�
	if (pToField == GetField())
	{
		if( GetField()->TeleportObject( this, &vDestPos ) )
			return RC::RC_PORTAL_INSTANT_MOVE;
		else
			return RC::RC_PORTAL_FAIL;
	}
	// �ʵ� �� �̵�
	else
	{
        SLOTCODE item_code = 0;
        SCSlotContainer* pContainer = m_pItemManager->GetItemSlotContainer(SI_INVENTORY);
        // NOTE: f110105.1L, fixed logical clearly that
        // the validation of the container is inconsistent with an empty checking.
        if (pContainer->ValidPos(atItemPos) && !pContainer->IsEmpty(atItemPos))
        {
            item_code = pContainer->GetSlot(atItemPos).GetCode();
        }
#ifdef _NA_20100307_BETAKEY_SYSTEM
        ReCallPlayer(GetCharName(), 0, ToFieldCode, vDestPos, ePortalType, atItemPos, item_code);
#else
		MSG_AG_GM_GO_CMD AGMsg;
		_tcsncpy( AGMsg.m_szCharName, GetCharName(), MAX_CHARNAME_LENGTH );
		AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
		AGMsg.m_RoomKey = 0;
		AGMsg.m_FieldCode = ToFieldCode;
		GetPos( &AGMsg.m_vCurPos );
		AGMsg.m_vDestPos = vDestPos;
		AGMsg.m_PortalType = (BYTE)ePortalType;
		AGMsg.m_atItemPos = atItemPos;
        AGMsg.m_atItemCode = item_code;
		SendPacket( &AGMsg, sizeof(AGMsg) );
#endif
		return RC::RC_PORTAL_CAN_SUCCESS;
	}
}

// implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA
VOID Player::GroupBonusAcceptForMoney( MONEY bonusMoney, MONEY addRatioMoney, eHEIM_ACQUISITION_TYPE eType )
{
    tag_GroupInfo* groupInfo = GroupInfoParser::Instance()->GetGroupInfo();

    FLOAT groupRatio = groupInfo->dwHeimRewardRatio/1000.0f;

    switch( eType )
    {
    case eHEIM_ACQUISITION_PICKUP:  //������ ������ �ֿ��� ���
        if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST ||
            groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION)
            return;
        else 
        {
            if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION_RATIO ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST_RATIO)
            {
                 g_pGameServer->NotifyHeimAcquisition(GetCharGuid(), eHEIM_ACQUISITION_PICKUP, MONEY(addRatioMoney * groupRatio) );
            }
            else
            {
                if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO ||
                    groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION_NORATIO ||
                    groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST_NORATIO)
                {
                    g_pGameServer->NotifyHeimAcquisition(GetCharGuid(), eHEIM_ACQUISITION_PICKUP, MONEY(bonusMoney * groupRatio) );
                }
            }
        }
        break;
    case eHEIM_ACQUISITION_QUEST:  //������ ����Ʈ ���� �޾��� ���
        if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO||
            groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO )
            return;
        else 
        {
            if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST_NORATIO ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_QUEST_RATIO )
            {
                g_pGameServer->NotifyHeimAcquisition(GetCharGuid(), eHEIM_ACQUISITION_QUEST, MONEY(bonusMoney * groupRatio) );
            }
        }
        break;
    case eHEIM_ACQUISITION_MISSION:  //������ �̼� ���� �޾��� ���
        if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO||
            groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO )
            return;
        else
        {
            if( groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION_NORATIO ||
                groupInfo->flHeimApplyScope == GroupInfoParser::ACCEPT_MISSION_RATIO)
            {
                g_pGameServer->NotifyHeimAcquisition(GetCharGuid(), eHEIM_ACQUISITION_MISSION, MONEY(bonusMoney * groupRatio) );
            }
        }
        break;
    }
}
VOID Player::GroupBonusAcceptForEXP( EXPTYPE bonusEXP, EXPTYPE addRatioEXP, eEXP_ACQUISITION_TYPE eType )
{
    tag_GroupInfo* groupInfo = GroupInfoParser::Instance()->GetGroupInfo();
    FLOAT groupRatio = groupInfo->dwExpRewardRatio/1000.0f;

    switch( eType )
    {
    case eEXP_ACQUISITION_HUNTING:  //������� ����ġ�� ����� ���
        if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST )
            return;
        else 
        {
            if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_MISSION_RATIO ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST_RATIO )
            {
                g_pGameServer->NotifyEXPAcquisition(GetCharGuid(), eEXP_ACQUISITION_HUNTING, EXPTYPE(addRatioEXP * groupRatio) );
            }
            else 
            {
                if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO ||
                    groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_MISSION_NORATIO ||
                    groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST_NORATIO )
                {
                    g_pGameServer->NotifyEXPAcquisition(GetCharGuid(), eEXP_ACQUISITION_HUNTING, EXPTYPE(bonusEXP * groupRatio) );
                }
            }
        }
        break;
    case eEXP_ACQUISITION_QUEST:  //����Ʈ �������� ����ġ�� ����� ���
        if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO||
            groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO )
            return;
        else
        {
            if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST_NORATIO||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_QUEST_RATIO)
            {
                g_pGameServer->NotifyEXPAcquisition(GetCharGuid(), eEXP_ACQUISITION_QUEST, EXPTYPE(bonusEXP * groupRatio) );
            }
        }
        break;
    case eEXP_ACQUISITION_MISSION:  //�̼� �������� ����ġ�� ����� ���
        if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_NORATIO||
            groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_HUNTING_RATIO )
            return;
        else
        {
            if( groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_ALL ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_MISSION ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_MISSION_NORATIO ||
                groupInfo->flExpApplyScope == GroupInfoParser::ACCEPT_MISSION_RATIO)
            {
                g_pGameServer->NotifyEXPAcquisition(GetCharGuid(), eEXP_ACQUISITION_MISSION, EXPTYPE(bonusEXP * groupRatio) );
            }
        }
        break;
    }
}

BOOL Player::CanApplyPCBangBenefit(  BOOL bNationCheck  )
{
	if( bNationCheck )
	{
		// �߱������� PC�� ���������� ���� �ʴ´�. ������ �Ϻ�����(����ġ 10%, PC�� �̺�Ʈ)�� ���� �����Ѵ�.
		if( ServerOptionParserEx::Instance()->GetServerOption().m_byPCBangApplyBenefit == FALSE )
			return FALSE;
	}

#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    if (GetPlayerAttr().GetAttrValue(eATTR_PREMIUMSERVICE_PCBANG) > 0) {
        return TRUE;
    }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

	return (!m_PCRoomSts && m_dwReservedValue);
}

VOID Player::SetFallDead( WzVector BeforeDeadPos )
{
	m_bIsFallDead = TRUE;	
	m_dwBeforeFallDeadPos = BeforeDeadPos;
}

RC::eSTATUS_RESULT Player::RebirthFromFallDead()
{
	if( IsAlive() )	
		return RC::RC_STATUS_ALIVE;	

	if( !m_bIsFallDead )	
		return RC::RC_STATUS_NOT_FALL_DEAD;
	
	//��Ȱ
    SetSD( 0 );
	SetHP( 1 );
	SetMP( 0 );
	SetPos( &m_dwBeforeFallDeadPos );
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    SetFP( 0 );
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	m_bIsFallDead = FALSE;

	return RC::RC_STATUS_SUCCESS;
}

BOOL Player::SetInterceptionOption(BYTE byType, BOOL bFlag)
{
	switch(byType)
	{
	case eINTERCEPTION_TRADE:
		{
			GetCharInfo()->m_bTradeFlag = bFlag;	return TRUE;
		}
		break;
	case eINTERCEPTION_WHISPER:
		{
			GetCharInfo()->m_bWhisperFlag = bFlag;	return TRUE;
		}
		break;
	default:
		return FALSE;		
		break;
	}

	return FALSE;
}

BOOL Player::GetInterceptionOption(BYTE byType)
{
	switch(byType)
	{
	case eINTERCEPTION_TRADE:
		{
			return GetCharInfo()->m_bTradeFlag;
		}
		break;
	default:
		return FALSE;
		break;
	}
	return FALSE;
}

//__NA001336_090514_PARTY_NAVI_SUPPORT__
PlayerStatEvent::eFlag
Player::CheckChangedStatus(PlayerStatEvent::eFlag skip_flag)
{
    DWORD flag = PlayerStatEvent::eFlag_None;
    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedHP)
        flag |= PlayerStatEvent::eFlag_ChangedHP;
    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedMP)
        flag |= PlayerStatEvent::eFlag_ChangedMP;
    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedSD)
        flag |= PlayerStatEvent::eFlag_ChangedSD;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (m_PartyEvent & PlayerStatEvent::eFlag_ChangedFP) {
        flag |= PlayerStatEvent::eFlag_ChangedFP;
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    if((skip_flag & PlayerStatEvent::eFlag_ChangedPos) == 0 && GetField())
    {
        static const WzVector radius = { 3.0f, 3.0f, 0.0f };
        const WzVector* pPos = GetPosPtr();
        if(m_LastUpdatedPartyNaviPosBox.IsValidCoordinateXY(pPos) == 0)
        {
            flag |= PlayerStatEvent::eFlag_ChangedPos;
            m_LastUpdatedPartyNaviPosBox.SetInit2D(pPos, &radius);
        }
    }
    return PlayerStatEvent::eFlag(flag);
}

void
Player::SendChangedStatus()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    m_pMovementChecker->GetSyncTuner()->Update();
#endif
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
#else
    if(GetPartyState().IsPartyMember() != 0)
        return;
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE

    // CHANGES: f110926.1L, notify the player navigation information
    // to support world server notification
    MSG_AG_PARTY_NAVI_INFO_BRD msg_navi;
    msg_navi.m_CodeType = msg_navi.OPTION_PARTY_NAVI;
    msg_navi.m_NumberOfNodes = 0;

    // no party member single side notification
    MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD msgHpMp;
    msgHpMp.m_NumberOfNodes = 0;

    // (CHANGES) (f100514.2L) (WAVERIX) fix up a loss which an original hp value over-value state
    // is possible if an old state is removed to replace a new state.
    // reference (BUGFIX) (100107) fix up invalid state that hp is grater than max hp

    typedef MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD::uNode StatChangeNode;
    const WORD playerKey = WORD(GetObjectKey());
    GameField* const game_field = this->GetField();

    if ((m_PartyEvent & PlayerStatEvent::eFlag_ChangedPos) && (game_field != NULL))
    {
        MSG_AG_PARTY_NAVI_INFO_BRD::Node& node = msg_navi.m_Nodes[msg_navi.m_NumberOfNodes++];
        node.fieldCode = game_field->GetFieldCode(); // assert(GetField() != 0)
        node.objectKey = playerKey;
        const WzVector* current_pos = GetPosPtr();
        node.posX = WORD(current_pos->x);
        node.posY = WORD(current_pos->y);
        node.posZ = WORD(current_pos->z);
    };

    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedHP)
    {
        StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
        node.m_HP.event = msgHpMp.eEvent_ChangedHP;
        node.m_HP.objectKey = playerKey;
        node.m_HP.maxHP = GetMaxHP();
        node.m_HP.currentHP = GetHP();
        node.m_HP.reservedHP = GetReserveHP();
        // (CHANGES) (f100514.2L) to pending process related to HP events.
        // (WARNING) the contents of the Reserved HP has problems if a hp is grater than a max hp
        if (node.m_HP.currentHP > node.m_HP.maxHP) {
            node.m_HP.currentHP = node.m_HP.maxHP;
            SetHP(node.m_HP.currentHP);
        }
    }
    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedMP)
    {
        StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
        node.m_MP.event = msgHpMp.eEvent_ChangedMP;
        node.m_MP.objectKey = playerKey;
        node.m_MP.maxMP = GetMaxMP();
        node.m_MP.currentMP = GetMP();
        // (CHANGES) (f100514.2L) to pending process related to MP events.
        if (node.m_MP.currentMP > node.m_MP.maxMP) {
            node.m_MP.currentMP = node.m_MP.maxMP;
            SetMP(node.m_MP.currentMP);
        }
    }

    if(m_PartyEvent & PlayerStatEvent::eFlag_ChangedSD)
    {
        StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
        node.m_SD.event = msgHpMp.eEvent_ChangedSD;
        node.m_SD.objectKey = playerKey;
        node.m_SD.maxSD = GetMaxSD();
        node.m_SD.currentSD = GetSD();
        // (CHANGES) (f100514.2L) to pending process related to MP events.
        if (node.m_SD.currentSD > node.m_SD.maxSD) {
            node.m_SD.currentSD = node.m_SD.maxSD;
            SetSD(node.m_SD.currentSD);
        }
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (m_PartyEvent & PlayerStatEvent::eFlag_ChangedFP)
    {
        StatChangeNode& node = msgHpMp.m_Nodes[msgHpMp.m_NumberOfNodes++];
        node.m_FP.event = msgHpMp.eEvent_ChangedFP;
        node.m_FP.objectKey = playerKey;
        node.m_FP.maxFP = GetMaxFP();
        node.m_FP.currentFP = GetFP();
        // (CHANGES) (f100514.2L) to pending process related to MP events.
        if (node.m_FP.currentFP > node.m_FP.maxFP) {
            node.m_FP.currentFP = node.m_FP.maxFP;
            SetFP(node.m_FP.currentFP);
        }
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    //
    NotifyChangedStatus(PlayerStatEvent::eFlag_None);
    if (msg_navi.m_NumberOfNodes != 0) {
        this->SendPacket(&msg_navi, msg_navi.GetSize());
    };
    if(msgHpMp.m_NumberOfNodes != 0) {
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        msgHpMp.user_count_ = 1;
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        SendPacket(&msgHpMp, msgHpMp.GetSize());
    }
}
//}__NA001336_090514_PARTY_NAVI_SUPPORT__

void
Player::Player_TransOff()
{
    StatusManager* const pStatusManager = GetStatusManager();
    StatusField& rStatus = pStatusManager->Status;

    if(rStatus.IsPlayer_TransOn())
    {
        // �ڿ������� �����ϱ� ���ؼ� ���� ��������� ���� �ð��� ���� �ð����� �����Ѵ�.
        BaseStatus* pStatus = pStatusManager->FindStatus(eCHAR_STATE_TRANSPARENT);
        if(pStatus)
            pStatus->StopStatus();
    }
}

//_KR_0_20091021_FIRENDINVITATION_EXTEND
VOID 
Player::SetInvitatedFriendRecallRequestTime(DWORD FriendGUID, DWORD64 Recalltime)
{
    RECALL_MSGBOX_TIME_MAP_ITER iter = m_RecallMsgBoxTimeMap.find(FriendGUID);

    if(iter == m_RecallMsgBoxTimeMap.end())
    {
        m_RecallMsgBoxTimeMap.insert( RECALL_MSGBOX_TIME_MAP_PAIR( FriendGUID, Recalltime ) );
    }
    else
    {
        (*iter).second = Recalltime;
    }    
}

DWORD64 
Player::GetInvitatedFriendRecallRequestTime(DWORD FriendGUID)
{
    RECALL_MSGBOX_TIME_MAP_ITER iter = m_RecallMsgBoxTimeMap.find(FriendGUID);

    if(iter == m_RecallMsgBoxTimeMap.end())
    {
        return 0;
    }
    return (*iter).second;    
}

// __KR_001355_20090624_GROUPINGSYSTEM_ALL
void Player::NotifyGroupMoneyToPlayer(MONEY amountMoney)
{
    MSG_CG_STATUS_GROUPMONEY_BRD sendMsg;

    _tcsncpy(sendMsg.ptszMemeberName, this->GetCharName(), _countof(sendMsg.ptszMemeberName));
    sendMsg.ptszMemeberName[_countof(sendMsg.ptszMemeberName) - 1] = '\0';
    sendMsg.bonusMoney = amountMoney;

    SendPacket(&sendMsg, sizeof(sendMsg));
}

// this has a different presentation from a complex riding casting
bool Player::SetupEtherealizer(nsSlot::EtheriaImpl* result_instance)
{
    result_instance->SetInstance(this, &etheria_control_option_);
    return true;
}

BOOL Player::ReCallPlayer(char const character_name[MAX_CHARNAME_LENGTH] ,const KEYTYPE room_key, FIELDCODE field_code, 
                          WzVector dest_pos, ePORTAL_TYPE portal_type, POSTYPE item_pos, SLOTCODE item_code)
{ 

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if((!GetBetaKeyInfo()->CanAbilityContent(GetGameZonePtr()->GetMAPINFO()->nBetaKey)) &&
        ePORTAL_TYPE_GM != portal_type)
    {        
        MSG_CG_MAP_RECALL_NAK nak_msg;
        nak_msg.error_code_ = MSG_CG_MAP_RECALL_NAK::RECALL_CANNOT_BETAKEY;
        SendPacket( &nak_msg, sizeof(nak_msg));
        return FALSE;
    }
#endif
    
    MSG_AG_GM_GO_CMD msg;

    strncpy(msg.m_szCharName, character_name, _countof(msg.m_szCharName));
    msg.m_szCharName[_countof(msg.m_szCharName) - 1] = '\0';
    GetPos(&msg.m_vCurPos);
    msg.m_RoomKey = room_key;
    msg.m_FieldCode = field_code;    
    msg.m_vDestPos = dest_pos;
    msg.m_PortalType = static_cast<BYTE>(portal_type);
    msg.m_atItemPos = item_pos;
    msg.m_atItemCode = item_code;    

    SendPacket( &msg, sizeof(msg) );

    return TRUE;
}

bool Player::IsThereBattleGroundZone() const
{
    if (GetGameZonePtr() && GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)  
    {
        const GameInstanceDungeon* instance_dungeon = 
            static_cast<GameInstanceDungeon*>(GetGameZonePtr());
        if (instance_dungeon && instance_dungeon->GetChaosZoneManager())
        {
            if (instance_dungeon->GetChaosZoneType() == kBattleGround)
            {
                return true; 
            }
        }
    }

    return false;
}

ChaosZone* const Player::GetCurrentChaosZone() const
{
    if (GetGameZonePtr() && GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)  
    {
        const GameInstanceDungeon* instance_dungeon = 
            static_cast<GameInstanceDungeon*>(GetGameZonePtr());
        if (instance_dungeon == NULL)
            return NULL;
        
        ChaosZoneManager* chaoszone_manager = instance_dungeon->GetChaosZoneManager();
        if (chaoszone_manager)
        {
            return chaoszone_manager->GetCurrentZone();
        }
    }

    return NULL;
}

ChaosZoneType Player::GetChaosZoneType() const
{
    if (GetGameZonePtr() && GetGameZonePtr()->GetZoneType() == eZONETYPE_INSTANCE)  
    {
        const GameInstanceDungeon* instance_dungeon = 
            static_cast<GameInstanceDungeon*>(GetGameZonePtr());
        if (instance_dungeon && instance_dungeon->GetChaosZoneManager())
        {
            return instance_dungeon->GetChaosZoneType();
        }
    }

    return kChaosZoneTypeNone;
}

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

bool Player::GetTotalEquipmentScore(int& out_total_item_score)
{
    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer* const>(
        GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    if (equip_container == NULL)
    {
        return false;
    }

    const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
        equip_container->GetEquipmentManageInfo();
    if (equip_manage_info == NULL)
    {
        return false;
    }

    const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
        equip_manage_info->item_score_info;

    out_total_item_score = item_score_info.GetTotalScore();
    return true;
}

SunRankingPlayerInfo Player::GetSunRankingPlayerInfo()
{
    SunRankingPlayerInfo out_info;
    strncpy(out_info.char_name, GetCharName(), MAX_CHARNAME_LENGTH);
    out_info.char_guid = GetCharGuid();
    out_info.guild_guid = GetGuildGuid();
    out_info.class_code = GetCharType();

    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        out_info.score[type] = 0;
    }
    return out_info;
}

void Player::OnSendAllPlayersSunRankingGradeInfo( const Sector& IN rSector )
{
    VarMsg2* const pVarMsg = &(m_VarMsg2[SENDINFO_SUN_RANKING_GRADE]);

    BYTE count = 0;
    const BYTE MAX_PACKET_COUNT = 
        MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD::_MAX_SUN_RANKING_GRADE_INFO_SIZE;

    MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD* pGradeInfo = 
        pVarMsg->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD>());

    pVarMsg->SetSize(pGradeInfo->GetSize());

    ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
        if (this == pPlayer)
            continue;   

        SUN_RANKING_GRADE_INFO* pack_info = (SUN_RANKING_GRADE_INFO*)pVarMsg->NextPtr();

        SunRankingGradeManager::Instance()->GetTopGrade(pPlayer->GetCharGuid(),
            pack_info->grade, pack_info->ranking_type);
        pack_info->m_dwObjectKey = pPlayer->GetObjectKey();

        pVarMsg->SetSize(pack_info->GetSize());

        if (MAX_PACKET_COUNT == ++count)
        {
            pGradeInfo->m_byCount = count;
            SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(),
                *pVarMsg->GetMsgSizePtr(), TRUE);

            pGradeInfo = pVarMsg->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD>());
            pVarMsg->SetSize(pGradeInfo->GetSize());

            count = 0;
        }
    }

    if(count)
    {
        pGradeInfo->m_byCount = count;
        SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(), *pVarMsg->GetMsgSizePtr(), TRUE);
    }
}

void Player::OnSendAllPlayersSunRankingGradeInfoToNeighbor( const Sector& IN rSector )
{
    VarMsg2* const pVarMsg = &(m_VarMsg2[SENDINFO_SUN_RANKING_GRADE]);

    ns_object::SearchNodeConstIterator it = rSector.GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = rSector.GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* pPlayer = link_node->select.player_node.player;
        if (obj_util::IsValid(pPlayer) == false)
        {
            continue;
        }
        if (this == pPlayer)
            continue;


        MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD* pGradeInfo = 
            pVarMsg->StartTypePtr(Param2Class<MSG_CG_SYNC_ALLPLAYERS_SUN_RANKING_GRADE_CMD>());

        pVarMsg->SetSize(pGradeInfo->GetSize());
        pGradeInfo->m_byCount = 1;

        SUN_RANKING_GRADE_INFO* pack_info = (SUN_RANKING_GRADE_INFO*)pVarMsg->NextPtr();

        SunRankingGradeManager::Instance()->GetTopGrade(this->GetCharGuid(),
            pack_info->grade, pack_info->ranking_type);
        pack_info->m_dwObjectKey = this->GetObjectKey();

        pVarMsg->SetSize(pack_info->GetSize());
        pPlayer->SendPacket((MSG_BASE_FORWARD*)*pVarMsg->GetMsgPPtr(),
            *pVarMsg->GetMsgSizePtr(), TRUE);
    }
}

#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
VOID Player::RemoveDragonTransformStatus()
{
    // �뺯��State�� End���¿��� Ŭ�󿡰� �뺯�����Ÿ� �˸���
    StatusManager* status_manager = this->GetStatusManager();
    if (status_manager->IsDragonTransforming() == true) {
        status_manager->RemoveDragonTransformStatus();
    }
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
GameGuild* Player::GetGuild() const
{
    GUILDGUID guild_guid = this->GetGuildGuid();
    if (guild_guid == 0) {
        return NULL;
    }

    return g_GameGuildManager.FindGuild(guild_guid);
}
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008132_20150316_MONDSCHEIN_TEXTLOG
VOID Player::_WriteMondschienLog( Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value )
{
    GameZone* game_zone = this->GetGameZonePtr();

    if (game_zone != NULL && game_zone->GetZoneType() == eZONETYPE_DOMINATION_FIELD ) 
    {
        eDOMINATION_TYPE domi_type = DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(game_zone->GetMapCode());
        if (domi_type == DOMINATION_TYPE_MONDSCHIEN ||
            domi_type == DOMINATION_TYPE_NORMAL)
        {
            // Ư����Ȳ ó��
            Player* attacker_player = static_cast<Player*>(attacker);
            GUILDGUID player_guild_guid = attacker_player->GetGuildGuid(); 

            //��尡 ������ ������� �ʴ´�
            if (attacker_player->GetGuildName() == NULL || this->GetGuildName() == NULL) {
                return;
            }

            // (-) �ǰ��ڰ� ������� ���
            if (IsDead() == TRUE) 
            {
                SUNMONDSCHIENLOG(eCRITICAL_LOG, TEXT("[AttackerGuild:%s]\t[Attacker:%s]\t[TargetGuild:%s]\t[Target:%s]\t[Player Dead!][skill:%d]\t%d"), 
                    attacker_player->GetGuildName(), attacker_player->GetCharName(), 
                    this->GetGuildName(), this->GetCharName(), skill_code, damage_value);
            }

            /*
            // (1) �����ڰ� 'Ư����'�� ��� //Ư���� �α� �Ⱦ�����
            if ( _IsLogTargetUser(attacker) == TRUE )
            { 
                // ������ ����!, ��������ġ ǥ��
                if (IsDead() == TRUE) {
                    SUNMONDSCHIENLOG(eCRITICAL_LOG, TEXT("[Attacker:%s]\t[Target:%s]\t[Player Dead!][skill:%d]\t%d"), 
                        attacker_player->GetCharName(), this->GetCharName(), skill_code, damage_value);
                }
                else {
                    SUNMONDSCHIENLOG(eCRITICAL_LOG, TEXT("[Attacker:%s]\t[Target:%s]\t[Damaged!][skill:%d]\t%d"), 
                        attacker_player->GetCharName(), this->GetCharName(), skill_code, damage_value);
                }

                // Attacker(������)�� �����ڵ帮��Ʈ
                    _WriteMondschienLog_StatusList(attacker);
                //Target(�ǰ���)�� �����ڵ帮��Ʈ
                    _WriteMondschienLog_StatusList(this);
            }
            // (2) '�������' �̸鼭 �ǰ��ڰ� ������� ���
            else if (IsDead() == TRUE && _IsLogTargetGuildUser(player_guild_guid)) 
            {
                SUNMONDSCHIENLOG(eCRITICAL_LOG, TEXT("[AttackerGuild:%s]\t[Attacker:%s]\t[TargetGuild:%s]\t[Target:%s]\t[Player Dead!][skill:%d]\t%d"), 
                                 attacker_player->GetGuildName(), attacker_player->GetCharName(), 
                                 this->GetGuildName(), this->GetCharName(), skill_code, damage_value);
            }
            */
        }
    }
}

VOID Player::_WriteMondschienLog_StatusList(Character* user)
{
    StatusManager* status_manager = user->GetStatusManager();
    std::string player_status_list = "x";
    if (status_manager != NULL) {
        player_status_list = status_manager->PrintLogStatusList();
    }
    SUNMONDSCHIENLOG(eCRITICAL_LOG, TEXT("[Target:%s]\t[Target's Status]\t[%s]"), 
        user->GetCharName(), player_status_list.c_str());
}

BOOL Player::_IsLogTargetUser(Character* user)
{
    //Ư���� �α� �ߴ�
    if ( _tcscmp(user->GetCharName(), TEXT("Shuu")) == 0            || 
         _tcscmp(user->GetCharName(), TEXT("z����z")) == 0          ||
         _tcscmp(user->GetCharName(), TEXT("kiss���")) == 0        ||
         _tcscmp(user->GetCharName(), TEXT("���ĭ")) == 0          ||
         _tcscmp(user->GetCharName(), TEXT("ħ��Death��Ŷ�")) == 0 
       )
    {
        return TRUE;
    }
    
    return FALSE;
}

BOOL Player::_IsLogTargetGuildUser( GUILDGUID guild_guid )
{
    /*
    BOOL is_target = FALSE;
    switch(guild_guid)
    {
    case 21419:     // HiddeN���
    case 22702:     // sIlElySiumlIs
    case 21929:     // Sunfighters
    case 21450:     // BandOfBlood
        is_target = TRUE;
        break;
    }

    return is_target;
    */
    //��� ������ ���� �αױ��
    return TRUE;
}

#endif //_NA_008132_20150316_MONDSCHEIN_TEXTLOG

#ifdef _NA_008706_20160229_TEXTLOG_PK
void Player::_WriteBattleMonitorLog( Character* attacker, SLOTCODE skill_code, DAMAGETYPE damage_value )
{
    // [�α� ��� ����]
    // 1. PvP�� ��쿡�� �����ϵ��� �Ѵ�
    if (attacker->IsEqualObjectKind(PLAYER_OBJECT) == false ||
        this->IsEqualObjectKind(PLAYER_OBJECT) == false)
    {
        return;
    }

    // 2. �ʵ忡���� ����Ѵ�
    GameZone* game_zone = this->GetGameZonePtr();
    if (game_zone == NULL || game_zone->GetZoneType() != eZONETYPE_FIELD ) 
    {
        return;
    }
    
    // 3. Ư�� ���� ����
    if (_IsLogTargetUser(attacker) == FALSE)
    {
        return;
    }

    // [�α� ���]
    SUNBATTLEMONITORLOG(eCRITICAL_LOG, TEXT("[Attacker:%s]\t[Target:%s]\t[skill:%d]\t[Damage:%d]"), 
        attacker->GetCharName(), this->GetCharName(), skill_code, damage_value);

    if (this->IsDead() == TRUE)
    {
        SUNBATTLEMONITORLOG(eCRITICAL_LOG, TEXT("[Attacker:%s]\t[Target:%s]\t[Player Dead!]"), 
            attacker->GetCharName(), this->GetCharName());
    }
}
#endif //_NA_008706_20160229_TEXTLOG_PK
