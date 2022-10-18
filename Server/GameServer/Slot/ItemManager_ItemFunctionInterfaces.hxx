#ifndef GAMESERVER_ITEMMANAGER_ITEM_FUNCTION_INTERFACES_HXX
    #error "can't direct include this file"
#endif

class ItemManager;
class MiniSlotInfo; // LotteryInfoParser
class MATERIALS_FOR_LEVEL; // EnchantParser

//==================================================================================================
namespace nsSlot {
;
//==================================================================================================
// Sub-implements of the ItemManager about item activity fuctionalities

// interfaces
struct IItemFunctionDropProtector;
struct IItemFunctionCoordinate;
struct IItemFunctionSummon;
struct IItemFunctionBattlezoneEnterance;
struct IItemFunctionPlayerProperty;
struct IItemFunctionItemProperty;
struct IItemFunctionEnchant;
struct IItemFunctionPet;
struct IItemFunctionItemLimitation;
struct IItemFunctionLottery;
struct IItemFunctionEtherWeapon;
struct IItemFunctionIdentity;
struct IItemFunctionCustomization;
struct IItemFunctionSkill;
struct IItemFunctionSocket;
struct IItemFunctionToggle; //_NA_006372_20130108_BADGE_ITEM

// implements
class ItemFunctionDropProtector;
class ItemFunctionCoordinate;
class ItemFunctionSummon;
class ItemFunctionBattlezoneEnterance;
class ItemFunctionPlayerProperty;
class ItemFunctionItemProperty;
class ItemFunctionEnchant;
class ItemFunctionPet;
class ItemFunctionItemLimitation;
class ItemFunctionLottery;
class ItemFunctionEtherWeapon;
class ItemFunctionIdentity;
class ItemFunctionCustomization;
class ItemFunctionSkill;
class ItemFunctionSocket;
class ItemFunctionToggle; //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
class ItemFunctionAwakening;
class ItemFunctionEvolution;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================

struct nsSlot::IItemFunctionDropProtector
{
    // relations = { eITEMTYPE_HOLDDROP, eITEMTYPE_HOLDDROP2 }
    virtual bool ProtectDrop(Player* killer) = 0;
};

struct nsSlot::IItemFunctionCoordinate
{
    // relations = { eITEMTYPE_SAVE_COORD };
    virtual RC::eITEM_RESULT SaveCoord(SCItemSlot& item_slot) = 0;
    // relations = { eITEMTYPE_SAVE_COORD, eITEMTYPE_CLEAR_COORD }
    virtual RC::eITEM_RESULT ClearCoord(POSTYPE clear_pos, POSTYPE coordinate_pos) = 0;
    // relations = { eITEMTYPE_SAVE_COORD, eITEMTYPE_PORTAL }
    virtual RC::eITEM_RESULT Portal(POSTYPE coordinate_pos, POSTYPE portal_pos) = 0;
    // relations = { eITEMTYPE_RECALL_ALLIANCE } <- origin:CanUseRecalAlliance
    virtual RC::eITEM_RESULT EnabledRecallAlliance() = 0;
    // rune stone, relations = { eITEMTYPE_WAYPOINT }
    virtual RC::eITEM_RESULT FireUpDateTime(SCItemSlot& item_slot) = 0;
};

struct nsSlot::IItemFunctionSummon
{
    // relations = { eITEMTYPE_TORCH, eITEMTYPE_MINE, eITEMTYPE_TOTEM }
    virtual RC::eITEM_RESULT CreateTotemNPC(SCItemSlot& item_slot, WzVector* dest_pos = NULL) = 0;
    // relations = { eITEMTYPE_RECALL }
    virtual RC::eITEM_RESULT SummonMonsterItemUse(SCItemSlot& item_slot) = 0;
};

struct nsSlot::IItemFunctionBattlezoneEnterance
{
    // relations = { eITEMTYPE_TOP_OF_TRIAL, eITEMTYPE_TOP_OF_TRIAL_CHARGE }
    virtual RC::eITEM_RESULT CanUseItemForTopOfTrial(POSTYPE item_pos,
        MAPCODE& OUT result_map_code, BOOL& OUT result_used_charge_item) = 0;
    virtual RC::eITEM_RESULT CanUseItemForTopOfTrial(SLOTCODE item_code, BYTE item_num) = 0;
    // relations = { eITEMTYPE_PORTAL_QUEST_ITEM }
    virtual RC::eITEM_RESULT CanUseItemForPortalQuest(POSTYPE item_pos,
        MAPCODE& OUT result_map_code) = 0;
    // relations = { eITEMTYPE_SSQ }
    virtual RC::eITEM_RESULT CanUseItemForSSQField(POSTYPE item_pos,
        MAPCODE& OUT result_map_code, SLOTCODE& OUT result_item_code,
        const MAPCODE expected_map_code) = 0;
};

struct nsSlot::IItemFunctionPlayerProperty
{
    // relations = { eITEMTYPE_STAT_RESET }, 2nd = { eItemWASTE_INIT_~~ }
    virtual RC::eITEM_RESULT InitStatItemUse(POSTYPE pos, WORD& OUT result_decreasing_value) = 0;
    // relations = { eITEMTYPE_EXTEND_CASH_SLOT }
    virtual RC::eITEM_RESULT ExtendCashSlot(SCItemSlot& item_slot) = 0;
    // relations = { eITEMTYPE_GUILDWAREHOUSE_SLOTCASH }
    virtual RC::eITEM_RESULT ExtendGuildWarehouseCashSlot(SLOTIDX slot_index, POSTYPE item_pos) = 0;
    // relations = { eITEMWASTE_CHAO_TIME_DECREASE }
    virtual RC::eITEM_RESULT UseChaoDecItem(const BASE_ITEMINFO* item_info) = 0;
};

struct nsSlot::IItemFunctionItemProperty
{
#ifndef _NA_000251_20100727_SOCKET_SYSTEM
    // relations = { eITEMTYPE_RANK_DOWN }
    virtual RC::eITEM_RESULT UseRankDownItem(POSTYPE ChargeItemPos, POSTYPE TargetPos) = 0;
#endif
    // relations = { eITEMTYPE_HAMMER_OF_ALCHEMIST } <-- unused anymore
    virtual RC::eITEM_RESULT UseHammerOfAlchemist(POSTYPE item_pos, POSTYPE target_item_pos) = 0;
#ifdef _NA_002050_20110216_ADD_GENDER
    // relations = { eITEMTYPE_EXTEND_CHARACTER_SLOT }
    // 캐릭터 슬롯 확장 아이템을 사용한다.
    virtual RC::eITEM_RESULT UseExtendCharSlotItem(
        const SLOTIDX inven_type, const POSTYPE inven_position) = 0;
#endif
};

struct nsSlot::IItemFunctionEnchant
{
    virtual RC::eITEM_RESULT Enchant(
        const POSTYPE target_item_pos,
        const POSTYPE down_guard_item_pos, 
        const eENCHANT_OPTION enchant_option) = 0;
    virtual RC::eITEM_RESULT ChkBlockEnchant() const = 0;
    virtual RC::eITEM_RESULT ChkSlotEnchant(SCItemSlot& target_item_slot) const = 0;
    virtual RC::eITEM_RESULT ChkSuccessRate(
        const uint8_t enchant, 
        const int enchant_grade,
        bool is_elite, 
        bool is_unique, 
        const eENCHANT_OPTION enchant_option
    ) const = 0;
    virtual RC::eITEM_RESULT ChkEnchantCost(
        const MATERIALS_FOR_LEVEL* enchant_info,
        const MONEY enchant_fare,
        const eENCHANT_OPTION enchant_option
    ) const = 0;
    virtual RC::eITEM_RESULT DeleteEnchantCost(
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option) = 0;
    virtual RC::eITEM_RESULT ApplyEnchantSuccess(
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option) = 0;
    virtual RC::eITEM_RESULT ApplyEnchantFail(
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        const MONEY enchant_fare,
        const eENCHANT_OPTION enchant_option, 
        const bool use_down_guard
    ) = 0;
    virtual void LogEnchant(
        int log_type,
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option,
        BYTE down_level = 0) = 0;
    // 엑스트라 스톤 사용
    virtual RC::eITEM_RESULT UseExtraStone(POSTYPE extra_stone_pos,
        POSTYPE target_item_pos, BYTE& OUT result_value_index) = 0;
};

struct nsSlot::IItemFunctionPet
{
    virtual RC::eITEM_RESULT UsePetItem(POSTYPE item_pos) = 0;
    // relations = {}, 2nd = { eITEMWASTE_PET_FEED }
    virtual RC::eITEM_RESULT UsePetFeed(SCItemSlot& item_slot) = 0;
    // relations = { eITEMTYPE_PET_NAME_CARD }
    virtual RC::eITEM_RESULT UseChangePetNameCard(
        POSTYPE item_pos, const util::StrArrayForPetName& pet_name) = 0;

};

struct nsSlot::IItemFunctionItemLimitation
{
    // 'The Bible of Hermesism'
    // relations = { eITEMTYPE_STAT_CHANGE }, unused anymore
    virtual RC::eITEM_RESULT ChangeItemStat(CODETYPE item_code, BOOLEAN clear) = 0;
    // TODO: change from 'POSTYPE ConIndex' to 'SLOTIDX slot_index' -or- remove
    // relations = { eITEMTYPE_STAT_CHANGE }, unused anymore
    virtual RC::eITEM_RESULT ChangeItemStat_SelectItem(
        POSTYPE ConIndex, POSTYPE item_pos,
        POSTYPE target_item_pos,
        MSG_CG_ITEM_STAT_CHANGE_USE_ACK* result_msg) = 0;
};

struct nsSlot::IItemFunctionLottery
{
    struct ConcretizationResult
    {
        enum eResult {
            eResult_None = 0,
            eResult_RemoveLotteryItem = 1 << 0,
        };
        int result_value;
        ITEMSTREAMEX* changed_lottery_item_stream;
        INVENTORY_TOTAL_INFO* total_info;
    };
    // item_slot.IsLottoItem()
    //
    virtual RC::eITEM_RESULT PandoraBoxOpen(
        POSTYPE lottery_item_pos, ITEMSTREAMEX* const result_stream) = 0;
    virtual RC::eITEM_RESULT PandoraBoxClose(
        POSTYPE lottery_item_pos) = 0;
    //__NA001283_20090223_COLLECTION_CLASS
    //(lst1024)(090226)채집 매니저에서 사용위해 private -> public 변경
    virtual SCItemSlot* MakeItemByLotteryInfo(
        const MiniSlotInfo& slotinfo, BOOL random, SCItemSlot* const working_slot) = 0;
    // CHANGES: f110613.3L, added the lottery renewal logic
    virtual RC::eITEM_RESULT ExtractLotteryItems(bool moved_all,
        POSTYPE lottery_item_pos, POSTYPE slot_index_in_lottery, const POSTYPE empty_slot_count,
        ConcretizationResult* const concretization_result) = 0;
};

struct nsSlot::IItemFunctionEtherWeapon
{
    // relations = { eITEMTYPE_ETHER_DEVICE }
    virtual RC::eITEM_RESULT EquipEtherDeviceToWeapon(
        POSTYPE device_item_pos, POSTYPE target_item_pos) = 0;
    // relations = {}, 2nd = { eITEMWASTE_ETHER_BULLET }, unused
    virtual RC::eITEM_RESULT ChargeEtherBullet(SCItemSlot& bullet_slot) = 0;
};

struct nsSlot::IItemFunctionIdentity
{
    // relations = { item_slot.IsFateItem() }
    virtual RC::eITEM_RESULT IdentifyFateItem(POSTYPE lucky_item_pos) = 0;
    // relations = { item_info.m_bPossessionItem }
    virtual RC::eITEM_RESULT IdentifyPossessionItem(
        POSTYPE item_pos, ITEMSTREAMEX& OUT result_item_stream) = 0;

};

struct nsSlot::IItemFunctionCustomization
{
    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    // relations = { eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL,
    //               eITEMTYPE_CHANGE_CHARACTER_FACE,
    //               eITEMTYPE_CHANGE_CHARACTER_HAIR,
    //               eITEMTYPE_CHANGE_CHARACTER_HEIGHT,
    //               eITEMTYPE_CHANGE_CHARACTER_GENDER }
    // 캐릭터 외모 변경 아이템을 사용한다.
    virtual RC::eITEM_RESULT UseChangeCharacterAppearanceItem(
        const POSTYPE item_position,
        CharacterAppearanceInfo& INOUT character_appearance) = 0;
};

struct nsSlot::IItemFunctionSkill
{
    virtual bool SkillExecute(SLOTCODE skill_code, SkillInfo* skill_info_data,
        const SkillOption& added_skill_option,
        BYTE attack_propensity = ATTACK_PROPENSITY_NORMAL, // reserved replace
        BYTE event_code = 0) = 0; // reserved replace
};

struct nsSlot::IItemFunctionSocket
{
    virtual RC::eITEM_RESULT FillSocket(
        const POSTYPE zard_position, const POSTYPE target_position) = 0;
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    virtual RC::eITEM_RESULT ExtractSocket(POSTYPE TargetPos) = 0;  // 사용 안됨.
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    // 소멸의 룬을 사용하여 소켓에 박힌 쟈드를 제거한다.
    virtual RC::eITEM_RESULT ExtractSocket(
        const POSTYPE extract_item_position,
        const POSTYPE zard_owner_position,
        const BYTE socket_number) = 0;
    virtual RC::eITEM_RESULT AddSocket(POSTYPE target_item_pos) = 0;
};

struct nsSlot::IItemFunctionToggle
{
    virtual bool Use(const SLOTCODE item_code, const POSTYPE item_pos) = 0;
    virtual bool Equip(SCItemSlot& item_slot, const bool is_equipped) = 0;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================
#pragma warning(push)
#pragma warning(disable: 4512)
// to fake low-level copy constructor implement based on implicit compiler gereration.
//
class nsSlot::ItemFunctionDropProtector : public nsSlot::IItemFunctionDropProtector
{
public:
    ItemFunctionDropProtector(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_HOLDDROP, eITEMTYPE_HOLDDROP2 }
    virtual bool ProtectDrop(Player* killer);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionCoordinate : public nsSlot::IItemFunctionCoordinate
{
public:
    ItemFunctionCoordinate(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_SAVE_COORD }
    virtual RC::eITEM_RESULT SaveCoord(SCItemSlot& item_slot);
    // relations = { eITEMTYPE_SAVE_COORD, eITEMTYPE_CLEAR_COORD }
    virtual RC::eITEM_RESULT ClearCoord(POSTYPE clear_pos, POSTYPE coordinate_pos);
    // relations = { eITEMTYPE_SAVE_COORD, eITEMTYPE_PORTAL }
    virtual RC::eITEM_RESULT Portal(POSTYPE coordinate_pos, POSTYPE portal_pos);
    // relations = { eITEMTYPE_RECALL_ALLIANCE } <- origin:CanUseRecalAlliance
    virtual RC::eITEM_RESULT EnabledRecallAlliance();
    // rune stone, relations = { eITEMTYPE_WAYPOINT }
    virtual RC::eITEM_RESULT FireUpDateTime(SCItemSlot& item_slot);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionSummon : public nsSlot::IItemFunctionSummon
{
public:
    ItemFunctionSummon(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_TORCH, eITEMTYPE_MINE, eITEMTYPE_TOTEM }
    virtual RC::eITEM_RESULT CreateTotemNPC(SCItemSlot& item_slot, WzVector* dest_pos = NULL);
    // relations = { eITEMTYPE_RECALL }
    virtual RC::eITEM_RESULT SummonMonsterItemUse(SCItemSlot& item_slot);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionBattlezoneEnterance : public nsSlot::IItemFunctionBattlezoneEnterance
{
public:
    ItemFunctionBattlezoneEnterance(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_TOP_OF_TRIAL, eITEMTYPE_TOP_OF_TRIAL_CHARGE }
    virtual RC::eITEM_RESULT CanUseItemForTopOfTrial(POSTYPE item_pos,
        MAPCODE& OUT result_map_code, BOOL& OUT result_used_charge_item);
    virtual RC::eITEM_RESULT CanUseItemForTopOfTrial(SLOTCODE item_code, BYTE item_num);
    // relations = { eITEMTYPE_PORTAL_QUEST_ITEM }
    virtual RC::eITEM_RESULT CanUseItemForPortalQuest(POSTYPE item_pos,
        MAPCODE& OUT result_map_code);
    // relations = { eITEMTYPE_SSQ }
    virtual RC::eITEM_RESULT CanUseItemForSSQField(POSTYPE item_pos,
        MAPCODE& OUT result_map_code, SLOTCODE& OUT result_item_code,
        const MAPCODE expected_map_code);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionPlayerProperty : public nsSlot::IItemFunctionPlayerProperty
{
public:
    ItemFunctionPlayerProperty(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_STAT_RESET }, 2nd = { eItemWASTE_INIT_~~ }
    virtual RC::eITEM_RESULT InitStatItemUse(POSTYPE pos, WORD& OUT result_decreasing_value);
    // relations = { eITEMTYPE_EXTEND_CASH_SLOT }
    virtual RC::eITEM_RESULT ExtendCashSlot(SCItemSlot& item_slot);
    // relations = { eITEMTYPE_GUILDWAREHOUSE_SLOTCASH }
    virtual RC::eITEM_RESULT ExtendGuildWarehouseCashSlot(SLOTIDX slot_index, POSTYPE item_pos);
    // relations = { eITEMWASTE_CHAO_TIME_DECREASE }
    virtual RC::eITEM_RESULT UseChaoDecItem(const BASE_ITEMINFO* item_info);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // relations = { eITEMTYPE_GUILDFACILITY_RESET }
    virtual RC::eITEM_RESULT InitGuildFacility(POSTYPE pos, WORD decrease_value);   // 길드 초기화 아이템 처리
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionItemProperty : public nsSlot::IItemFunctionItemProperty
{
public:
    ItemFunctionItemProperty(ItemManager* item_manager) : root_(item_manager) {}
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    // relations = { eITEMTYPE_RANK_DOWN }
    virtual RC::eITEM_RESULT UseRankDownItem(POSTYPE ChargeItemPos, POSTYPE TargetPos);
#endif
    // relations = { eITEMTYPE_HAMMER_OF_ALCHEMIST } <-- unused anymore
    virtual RC::eITEM_RESULT UseHammerOfAlchemist(POSTYPE item_pos, POSTYPE target_item_pos);
#ifdef _NA_002050_20110216_ADD_GENDER
    // relations = { eITEMTYPE_EXTEND_CHARACTER_SLOT }
    // 캐릭터 슬롯 확장 아이템을 사용한다.
    virtual RC::eITEM_RESULT UseExtendCharSlotItem(
        const SLOTIDX inven_type, const POSTYPE inven_position);
#endif
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionEnchant : public nsSlot::IItemFunctionEnchant
{
public:
    ItemFunctionEnchant(ItemManager* item_manager) : root_(item_manager) {}
    virtual RC::eITEM_RESULT Enchant(
        const POSTYPE target_item_pos, 
        const POSTYPE down_guard_item_pos, 
        const eENCHANT_OPTION enchant_option);
    virtual RC::eITEM_RESULT ChkBlockEnchant() const;
    virtual RC::eITEM_RESULT ChkSlotEnchant(SCItemSlot& target_item_slot) const;
    virtual RC::eITEM_RESULT ChkSuccessRate(
        const uint8_t enchant, 
        const int enchant_grade,
        bool is_elite, 
        bool is_unique, 
        const eENCHANT_OPTION enchant_option
    ) const;
    virtual RC::eITEM_RESULT ChkEnchantCost(
        const MATERIALS_FOR_LEVEL* enchant_info,
        const MONEY enchant_fare,
        const eENCHANT_OPTION enchant_option
    ) const;
    virtual RC::eITEM_RESULT DeleteEnchantCost(
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option
    );
    virtual RC::eITEM_RESULT ApplyEnchantSuccess(
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option
    );
    virtual RC::eITEM_RESULT ApplyEnchantFail(
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        const MONEY enchant_fare,
        const eENCHANT_OPTION enchant_option, 
        const bool use_down_guard
    );
    virtual void LogEnchant(
        int log_type,
        SCItemSlot& target_item_slot,
        const MATERIALS_FOR_LEVEL* enchant_info,
        MONEY enchant_fare,
        eENCHANT_OPTION enchant_option,
        BYTE down_level = 0
    );
    // 엑스트라 스톤 사용
    virtual RC::eITEM_RESULT UseExtraStone(POSTYPE extra_stone_pos,
        POSTYPE target_item_pos, BYTE& OUT result_value_index);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionPet : public nsSlot::IItemFunctionPet
{
public:
    ItemFunctionPet(ItemManager* item_manager) : root_(item_manager) {}
    virtual RC::eITEM_RESULT UsePetItem(POSTYPE item_pos);
    // relations = {}, 2nd = { eITEMWASTE_PET_FEED }
    virtual RC::eITEM_RESULT UsePetFeed(SCItemSlot& item_slot);
    // relations = { eITEMTYPE_PET_NAME_CARD }
    virtual RC::eITEM_RESULT UseChangePetNameCard(
        POSTYPE item_pos, const util::StrArrayForPetName& pet_name);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionItemLimitation : public nsSlot::IItemFunctionItemLimitation
{
public:
    ItemFunctionItemLimitation(ItemManager* item_manager) : root_(item_manager) {}
    // 'The Bible of Hermesism'
    // relations = { eITEMTYPE_STAT_CHANGE }, unused anymore
    virtual RC::eITEM_RESULT ChangeItemStat(CODETYPE item_code, BOOLEAN clear);
    // TODO: change from 'POSTYPE ConIndex' to 'SLOTIDX slot_index' -or- remove
    // relations = { eITEMTYPE_STAT_CHANGE }, unused anymore
    virtual RC::eITEM_RESULT ChangeItemStat_SelectItem(
        POSTYPE ConIndex, POSTYPE item_pos,
        POSTYPE target_item_pos,
        MSG_CG_ITEM_STAT_CHANGE_USE_ACK* result_msg);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionLottery : public nsSlot::IItemFunctionLottery
{
public:
    ItemFunctionLottery(ItemManager* item_manager) : root_(item_manager) {}
    // item_slot.IsLottoItem()
    //
    virtual RC::eITEM_RESULT PandoraBoxOpen(
        POSTYPE lottery_item_pos, ITEMSTREAMEX* const result_stream);
    virtual RC::eITEM_RESULT PandoraBoxClose(
        POSTYPE lottery_item_pos);
    //__NA001283_20090223_COLLECTION_CLASS
    //(lst1024)(090226)채집 매니저에서 사용위해 private -> public 변경
    virtual SCItemSlot* MakeItemByLotteryInfo(
        const MiniSlotInfo& slotinfo, BOOL random, SCItemSlot* const working_slot);
    // CHANGES: f110613.3L, added the lottery renewal logic
    virtual RC::eITEM_RESULT ExtractLotteryItems(bool moved_all,
        POSTYPE lottery_item_pos, POSTYPE slot_index_in_lottery, const POSTYPE empty_slot_count,
        ConcretizationResult* const concretization_result);
private:
    RC::eITEM_RESULT AcquireItemsFromLottery(
        bool moved_all, SCItemSlot& lottery_item, POSTYPE slot_index_in_lottery,
        ConcretizationResult* const concretization_result);
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionEtherWeapon : public nsSlot::IItemFunctionEtherWeapon
{
public:
    ItemFunctionEtherWeapon(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { eITEMTYPE_ETHER_DEVICE }
    virtual RC::eITEM_RESULT EquipEtherDeviceToWeapon(
        POSTYPE device_item_pos, POSTYPE target_item_pos);
    // relations = {}, 2nd = { eITEMWASTE_ETHER_BULLET }, unused
    virtual RC::eITEM_RESULT ChargeEtherBullet(SCItemSlot& bullet_slot);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionIdentity : public nsSlot::IItemFunctionIdentity
{
public:
    ItemFunctionIdentity(ItemManager* item_manager) : root_(item_manager) {}
    // relations = { item_slot.IsFateItem() }
    virtual RC::eITEM_RESULT IdentifyFateItem(POSTYPE lucky_item_pos);
    // relations = { item_info.m_bPossessionItem }
    virtual RC::eITEM_RESULT IdentifyPossessionItem(
        POSTYPE item_pos, ITEMSTREAMEX& OUT result_item_stream);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionCustomization : public nsSlot::IItemFunctionCustomization
{
public:
    ItemFunctionCustomization(ItemManager* item_manager) : root_(item_manager) {}
    //_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    // relations = { eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL,
    //               eITEMTYPE_CHANGE_CHARACTER_FACE,
    //               eITEMTYPE_CHANGE_CHARACTER_HAIR,
    //               eITEMTYPE_CHANGE_CHARACTER_HEIGHT,
    //               eITEMTYPE_CHANGE_CHARACTER_GENDER }
    // 캐릭터 외모 변경 아이템을 사용한다.
    virtual RC::eITEM_RESULT UseChangeCharacterAppearanceItem(
        const POSTYPE item_position,
        CharacterAppearanceInfo& INOUT character_appearance);
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionSkill : public nsSlot::IItemFunctionSkill
{
public:
    ItemFunctionSkill(ItemManager* item_manager) : root_(item_manager) {}

    virtual bool SkillExecute(SLOTCODE skill_code, SkillInfo* skill_info_data,
        const SkillOption& added_skill_option,
        BYTE attack_propensity = ATTACK_PROPENSITY_NORMAL, // reserved replace
        BYTE event_code = 0); // reserved replace
private:
    //
    ItemManager* const root_;
};

class nsSlot::ItemFunctionSocket : public nsSlot::IItemFunctionSocket
{
public:
    ItemFunctionSocket(ItemManager* item_manager) : root_(item_manager) {}
    virtual RC::eITEM_RESULT FillSocket(
        const POSTYPE zard_position, const POSTYPE target_position);
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    virtual RC::eITEM_RESULT ExtractSocket(POSTYPE TargetPos);  // 사용 안됨.
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    // 소멸의 룬을 사용하여 소켓에 박힌 쟈드를 제거한다.
    virtual RC::eITEM_RESULT ExtractSocket(
        const POSTYPE extract_item_position,
        const POSTYPE zard_owner_position,
        const BYTE socket_number);
    virtual RC::eITEM_RESULT AddSocket(POSTYPE target_item_pos);
private:
    //
    ItemManager* const root_;
};

#ifdef _NA_006372_20130108_BADGE_ITEM
class nsSlot::ItemFunctionToggle : public nsSlot::IItemFunctionToggle
{
public:
    ItemFunctionToggle(ItemManager* item_manager, ToggleItemType toggle_item_type) 
        : root_(item_manager), toggle_item_type_(toggle_item_type) 
    {}

    virtual bool Use(const SLOTCODE item_code, const POSTYPE item_pos);
    virtual bool Equip(SCItemSlot& item_slot, const bool is_equipped);

    bool Equip( POSTYPE item_pos, const bool is_equipped);
    bool CheckItemType(const BASE_ITEMINFO* item_info);

private:
    const ToggleItemType toggle_item_type_;
    ItemManager* const root_;
};
#endif //_NA_006372_20130108_BADGE_ITEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
class nsSlot::ItemFunctionAwakening
{
public:
    ItemFunctionAwakening(ItemManager* item_manager) 
        : root_(item_manager)
    {}

    RC::eITEM_RESULT ChkBlockAwakening() const;

    RC::eITEM_RESULT ChkInvaild(const POSTYPE equip_item_pos, 
                                const POSTYPE material_item_pos, 
                                const POSTYPE additive_item_pos) const;

    RC::eITEM_RESULT ChkSlotAwakening(const SCItemSlot& equip_item, 
                                      const SCItemSlot& material_item, 
                                      const SCItemSlot& additive_item) const;

    RC::eITEM_RESULT DeleteMaterialAndAdditive(SCItemSlot& equip_item, 
                                               SCItemSlot& material_item, 
                                               SCItemSlot& additive_item);

    RC::eITEM_RESULT Awakening(const POSTYPE equip_item_pos, 
                               const POSTYPE material_item_pos, 
                               const POSTYPE additive_item_pos);

    RC::eITEM_RESULT ApplyAwakening(bool success,
                                    SCItemSlot& equip_item_slot, 
                                    const CODETYPE material_item_code,
                                    const BYTE material_awakening,
                                    const CODETYPE additive_item_code);
private:
    const bool GetResult(const SCItemSlot& equip_item_slot, 
                         const CODETYPE material_item_code, 
                         const BYTE material_awakening) const;

    ItemManager* const root_;
};

class nsSlot::ItemFunctionEvolution
{
public:
    ItemFunctionEvolution(ItemManager* item_manager)
        : root_(item_manager)
    {}


    RC::eITEM_RESULT ChkBlockEvolution() const;

    RC::eITEM_RESULT ChkInvaild(const POSTYPE equip_item_pos, 
                                const POSTYPE additive_item_pos) const;

    RC::eITEM_RESULT ChkSlotEvolution(const SCItemSlot& equip_item, 
                                      const SCItemSlot& additive_item) const;

    RC::eITEM_RESULT DeleteAdditive(const CODETYPE& equip_item_code,
                                    SCItemSlot& additive_item);

    RC::eITEM_RESULT DeleteEquipment(const POSTYPE equip_item_pos);

    RC::eITEM_RESULT Evolution(const POSTYPE equip_item_pos, 
                               const POSTYPE additive_item_pos);

    RC::eITEM_RESULT ApplyEvolution(bool success,
                                    SCItemSlot& equip_item_slot, 
                                    const CODETYPE additive_item_code);

    RC::eITEM_RESULT ObtainItem(const BYTE result_enchant, 
                                const CODETYPE result_item_code);

private:
    const BYTE GetResultEnchant(const BYTE prev_enchant) const;
    const bool GetResult(const CODETYPE& equip_item_code) const;

    ItemManager* const root_;
};
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#pragma warning(pop)
//==================================================================================================
//==================================================================================================
//==================================================================================================


//==================================================================================================
