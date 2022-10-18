#pragma once

#include "uibaseman.h"
#include "uiCashItemShopMan\uiCashItemShopDef.h"
#include "SocketOptionParser.h"
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "CurrencyInfoParser.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

typedef std::set<CODETYPE>			COMPOSE_LIST;

struct MATERIAL_COMPOSE_LIST
{
	SLOTCODE		m_MeterialItem;
	COMPOSE_LIST	m_ComposeList;
};

typedef util::SolarHashTable<MATERIAL_COMPOSE_LIST*> MATERIAL_COMPOSE_HASH;

class SCItemSlot;
class PlayerAttributes;
class SCPerkSlot;
#ifdef _NA_003027_20111013_HONOR_SYSTEM
class ShopDialog;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _DH_BATTLEZONE2_
struct BattlezoneMissionTreeInfo;
#endif//_DH_BATTLEZONE2_

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
struct BASE_SkillTextInfo;
#endif
#ifdef _NA_008334_20150608_SONNENSCHEIN
struct GuildMissionInfo;
#endif //_NA_008334_20150608_SONNENSCHEIN

class uiToolTipMan :
	public uiBaseMan
{
    enum TooltipTextures
    {
        kTooltipTexture_Start = 0,
        kTooltipTexture_Socket = kTooltipTexture_Start, // 소켓 텍스쳐
#ifdef _NA_003740_20111122_RANDOM_ITEM
        kTooltipTexture_RandomItemGrade,
#endif//_NA_003740_20111122_RANDOM_ITEM
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        kTooltipTexture_PartyObtain,    //모두분배 아이콘
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        kTooltipTexture_Handle, // 핸들용 텍스쳐
        kTooltipTexture_Max,
    };    

    enum TooltipTextures_Rectangles
    {
        kTexture_Rectangles_Start = 0,
        kTexture_Rectangle_Socket0 = kTexture_Rectangles_Start, // Socket미확인 텍스쳐 [?]
        kTexture_Rectangle_Socket1, // Socket1개 텍스쳐 [1S]
        kTexture_Rectangle_Socket2, // Socket2개 텍스쳐 [2S]
        kTexture_Rectangle_Socket3, // Socket3개 텍스쳐 [3S]
        kTexture_Rectangle_Socket4, // Socket4개 텍스쳐 [4S]
        kTexture_Rectangle_Socket5, // Socket5개 텍스쳐 [5S]
        kTexture_Rectangle_SocketEmpty, // Socket 빈 슬롯 텍스쳐
        kTexture_Rectangle_SocketItem, // Socket 아이템 텍스쳐

#ifdef _NA_003740_20111122_RANDOM_ITEM
        kTooltipTexture_RandomOptionGradeRactangle0,
        kTooltipTexture_RandomOptionGradeRactangle1,
        kTooltipTexture_RandomOptionGradeRactangle2,
        kTooltipTexture_RandomOptionGradeRactangle3,
        kTooltipTexture_RandomOptionGradeRactangle4,
        kTooltipTexture_RandomOptionGradeRactangle5,
        kTooltipTexture_RandomOptionGradeRactangle6,
#endif //_NA_003740_20111122_RANDOM_ITEM

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        kTexture_Rectangle_PartyObtain,
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

        kTexture_Rectangles_Max,
    };

public:
	uiToolTipMan(InterfaceManager *pUIMan);

    //! CDialogWZ 에 콜백을 셋팅하면
    //! 해당 다이얼로그의 툴팁이 랜더링 되는 시점에 콜백으로 넘어 온다.
    static void Callback_Tooltip(CDialogWZ* dialog_ptr, SI_ToolTip* tooltip,int x, int y);

	//--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
    virtual void            OnInitialize();
	virtual void			Render();
    void                    RenderTexture(DWORD texture_number, DWORD rectangle_number, FRECT size, FRECT position, DWORD extra_style, WzColor color);
    void                    Clear();

	void					RenderEx( const RECT& baseRect );
	const RECT&				GetTooltipRect( void ) const { return m_rect; }


#ifdef _INTERNATIONAL_DATE_TOOLTIP
	int						GetStringMonth(int nMonth);
#endif//_INTERNATIONAL_DATE_TOOLTIP(영문날짜순서)
	//-----------------------------------------------------------------------------------------------
	// 툴팁 관련
	//-----------------------------------------------------------------------------------------------
private:
    void                                _registerStyleQuickslotInfoTooltip(const SCSlot *pSlot);
    void                                _registerQuickslotInfoTooltip(const SCSlot *pSlot);
    void                                _registerItemslotInfoTooltipForQuickInDeactive(const SCSlot *pSlot);
    void                                _registerItemslotInfoTooltipForQuick(const SCSlot *pSlot);
    void                                _registerItemslotInfoTooltip(const SCSlot *pSlot);
    void                                _registerSkillslotInfoTooltip(const SCSlot *pSlot, BOOL bFromQuickSlot);
    BOOL                                _registerTooltipFateItem(SCItemSlot * pItemSlot, const BASE_ITEMINFO * pItemInfo,  BOOL bBuyPrice, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal);
    void                                _addPriceTooltip(BOOL buy_price,SCItemSlot* item_slot, const BASE_ITEMINFO* item_info,LimitStatCompositeType composite_type);

public:
    void                                InitTooltip();
    void                                InitTextures();

    void                                SetTooltipDialogID( DWORD ParentDialogID ) { m_TooltipDialogID = ParentDialogID; }
    DWORD                               GetTooltipDialogID() { return m_TooltipDialogID; }

    void                                AddEmptyLineForTooltip(int iHeight = 0);

    void                                RegisterSlotInfoTooltip(const SCSlot *pSlot);
    void                                RegisterHeroItemInfoTooltipFromSlot( const SCSlot* slot );
    void                                RegisterItemslotInfoTooltipForShop(const SCSlot *pSlot, DWORD bRePurchase = SHOP_TYPE_OF_NARMAL);
    void                                RegisterChannelInfoTooltip(LPCTSTR pszMessage , int iJoinMemeber_Num );
    void                                RegisterSkillEffectInfoTooltip(SKILL_EFFECT_IMAGE *pImage, BOOL bShowDelStr = FALSE);
    void                                RegisterTooltipItemForPackage(LPCTSTR szName, DWORD *dwItemCode);
    void                                RegisterTooltipItemForCashShop(CODETYPE itemCode, DWORD dwCoolTime, DWORD64 SellEndDate, BYTE byEnchant = 0, BYTE byRank = 0, bool is_show_date = true);
    void                                RegisterTooltipItemForCashShopForLotto(LPCTSTR name, LPCTSTR desc, LPCTSTR caution, DWORD64 end_date, bool is_show_date = true);
    
    void                                RegisterTooltipItem(const SCSlot *pSlot, BOOL bBuyPrice, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal);

    // 개당가격 툴팁출력
    void                                RegisterTooltipVendorItemUnitPrice(SCSlot* Item_Slot);
    
    void                                RegisterTooltipSkill(SkillScriptInfo* curSkillInfo,
                                                             SkillScriptInfo* nextSkillInfo=NULL,
                                                             bool isActivated=true);

    //void                                RegisterTooltipStyle(BASE_STYLEINFO* curStyleInfo,
 //                                                            BASE_STYLEINFO* nextStyleInfo=NULL,
    //                                                         bool isActivated=true);
    void                                RegisterTooltipSkillEtc(SkillScriptInfo* pSkillInfo);

    void                                RegisterTooltipWithStringCode(int iStringCode);
    void                                RegisterTooltipFormat(TCHAR* format, int base_value, int over_value);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	void                                RegisterTooltipFormat(TCHAR* format, float base_value, float over_value);
#endif // _NA_008472_20150903_IMPROVED_BALANCE


    void                                RegisterTooltipWithStringCodeByProcessWordWrap( int iStringCode, int size = 200 );


    void                                RegisterTooltipForWareNInventoryLock( int iStringCode);

    void                                RegisterTooltipSimpleText(TCHAR* pszString);

    void                                SetTooltipTestMode(BOOL bTest)    { m_bTooltipTestMode = bTest; }


    void                                RegisterTooltip(const cTooltipInfo& tooltipInfo);

    int                                 RegisterTooltip(LPCTSTR szText,
                                                        int iBufSize=-1,
                                                        int yInterval=20,
                                                        WzID fontID=StrToWzID("m212"),
                                                        WzColor color = WzColor_RGBA(255, 255, 255, 255),
                                                        WzColor shadowColor=TOOLTIP_BG_WZCOLOR,
                                                        DWORD dwTooltipExtraStyle = 1,
                                                        DWORD eSpecialTooltip = eSPECIAL_TOOLTIP_NONE,
                                                        DWORD dwIconCode = 0 );

    void                                RegisterTooltipLineChange(LPCTSTR szText,
                                                        int iBufSize,
                                                        int yInterval=20,
                                                        WzID fontID=StrToWzID("m212"),
                                                        WzColor color=TOOLTIP_FONT_WZCOLOR,
                                                        WzColor shadowColor=TOOLTIP_BG_WZCOLOR,
                                                        DWORD dwTooltipExtraStyle = 0);

    void                                RegisterTiredSystemToolTip(int iTime);
    void                                RegisterChaoSystemToolTip(int iTime,BOOL bChao);
    void                                RegisterTooltip_ItemStatChange(const BASE_ITEMINFO* pItemInfo, SCSlot *pSlot);
    void                                RegisterTooltip_Reward(const BASE_ITEMINFO* pItemInfo);
    void                                RegisterPandoraItemName(SCItemSlot* pItemSlot);
    void                                RegisterPersonalWaypoint(int bEnable);

    void                                RegisterTooltipMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName);

    void                                RegisterTooltipPerkInfo(const SCPerkSlot* perk_slot, const POSTYPE slot_position);

    void                                RegisterTooltipSocketOption(ZardOptionInfo::ItemOptionCode item_option_code);
    void                                RegisterTooltipCrystalization(SCItemSlot* item_lsot_ptr);

#ifdef _DH_BATTLEZONE2_
    void                                RegisterTooltipBattlezoneMap(const BattlezoneMissionTreeInfo* mission_info);
#endif//_DH_BATTLEZONE2_

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    void                                RegisterTooltipCurrencyDescription(const CurrencyInfoParser::CurrencyInfo* currency_info);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    void                                RegisterTooltipGuildMissionTree(const GuildMissionInfo* mission_info);
#endif//_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    void                                RegisterTooltipGuildMissionTree_OneDayMission(const GuildMissionInfo* mission_info, DWORD end_time, BYTE event_state);
#endif//_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

    //void                                RenderSpecialTooltipTexture4RandomItem(RECT rc);
    void                                RenderSpecialTooltipTexture4LuckyItem(RECT rc);

private:
    int                                 RegisterTooltipImage(DWORD texture_number, DWORD rectangle_number, float width, float height, DWORD extra_style, float x = 0.0f, float y = 0.0f, WzColor color = WzColor_RGBA(0,0,0,0));

private:

    // 툴팁용 내부함수 / 이상 상태 버프 
    BOOL                                _isEnableShowTooltip();
    
    void                                _addChargeItemCoordinateExForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addWaypointItemTooltip();

    BOOL                                _isEnableShowBuffTooltip(SKILL_EFFECT_IMAGE *pImage);

    void                                _addBuffNameForTooltip(BASE_STATEINFO* pStateInfo);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void                                _addBuffNameForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE* pImage);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void                                _addBuffDescForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage, BOOL bShowDelStr);
    void                                _addBuffRemainTimeForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage);

    int                                 _getBuffDescType(DWORD dwStatusID);

    // 툴팁용 내부함수 / 스타일
    void                                _addStyleNameForTooltip(BASE_STYLEINFO* pStyleInfo);
    void                                _addStyleLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);

    void                                _addStyleCurLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);
    void                                _addStyleNextLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);
    
    void                                _addSyleNeedsForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);
    void                                _addStyleLimitWeaponForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated, BOOL bQuickSlot);

    void                                _addStyleDescForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);

    void                                _setStyleCurLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);
    void                                _setStyleNextLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated);

    // 툴팁용 내부함수 / 스킬
    
    void                                _addSkillNameForTooltip(SkillScriptInfo* pSkillInfo);
    
    void                                _addSkillTypeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillLevelForTooltip(SkillScriptInfo* pSkillInfo);
    void                                _addSkillRemainCoolTimeForTooltip(SkillScriptInfo* pSkillInfo);

    void                                _addSkillDescForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillNormalDescForTooltip(SkillScriptInfo* pSkillInfo);

    void                                _addSkillCurLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillNextLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addChangeUpSkillTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);

    void                                _addSkillNeedsForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bNextSkill = TRUE);
    void                                _addSkillLimitWeaponForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot);
    void                                _addSkillCastingTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillCoolTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillDistanceForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillSpendForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void                                _addSkillFPSpendForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    //void                                _addSkillDamageForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _addSkillEffectForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    
    void                                _setSkillBaseInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot);
    void                                _setSkillCurLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    void                                _setSkillNextLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated);
    
    
    // 툴팁용 내부함수 / 아이템
    BOOL                                _isEnableShowItemTooltip(const SCSlot *pSlot);
    BOOL                                _isEnableItemEnchant(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemSocket(const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemType(const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemEquipClass(const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemDurability(const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemOption(const BASE_ITEMINFO *pItemInfo);

    void                                _addItemPetForToolTip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemPetFeedForToolTip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo);
    BOOL                                _isEnableItemPet(const BASE_ITEMINFO *pItemInfo);
    void                                _addItemPetBaseOpForTooltip( const BASE_ITEMINFO *pItemInfo,BOOL bCashShop);
    void                                _addItemPetAddForTooltip( const BASE_ITEMINFO *pItemInfo,BOOL bCashShop,BYTE bGrade=0);
 #ifdef __CN_PET_OPTION_APPLY_FLAG
    void                                _addItemPetAddOption(const BASE_ITEMINFO *pItemInfo,BOOL bCashShop,int iIntimacy=0);
 #endif//__CN_PET_OPTION_APPLY_FLAG(펫추가옵션누적)
    
    void                                _addItemLottoItemInfoTooltip( SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo);


    void                                _UpperSkillEnableTooltip(SkillScriptInfo* skill_info);

    BOOL                                _isEnableChargeItemDesc(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemRiderSetOptionForTooltip(const BASE_ITEMINFO* pItemInfo);
    void                                _addItemRiderWaxLevel(SCItemSlot* pItemSlot);


    bool                                _addItemCustomizingTooltip(SCItemSlot* slot_item, const BASE_ITEMINFO* item_info);
    
    void                                _addItemSetOptionForTooltip(const BASE_ITEMINFO* pItemInfo);


    void                                _addItemFullSetPlusOptionForTooltip(const BASE_ITEMINFO* pItemInfo);

    DWORD                               _getItemNameRGB(const char* szNumber);

    void                                _addItemNameForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, BYTE byEnchant = 0, BOOL bRenderIcon = FALSE);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	void                                _addItemAwakeningForTooltip(SCItemSlot* item_slot, const BASE_ITEMINFO* item_info);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
    void                                _addFateItemRankForTooltip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemTypeForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemLimitLevelForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    bool                                _addItemEquipClassForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemPhyAttackSpeedForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemLimitedItemRepairCountForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemPhyAttRateForTooltip(SCItemSlot* item_slot, const BASE_ITEMINFO* item_info);
    void                                _addItemPhyAttAvoidForTooltip(SCItemSlot* item_slot, const BASE_ITEMINFO* item_info);
    void                                _addItemEtherEmissionDevice(SCItemSlot * pItemSlot, const BASE_ITEMINFO * pItemInfo);
    void                                _addSkillEtherBulletUseInfoTooltip(SkillScriptInfo* curSkillInfo);
    void                                _addItemNextGradeLimitLevelForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    
    void                                _addItemPhyAttackPowerForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemMagicAttackPowerForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemPhyDefenseForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    void                                _addItemMagicDefenseForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);

    void                                _addItemDurabilityForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo);
    
    void                                _addItemLimitInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);
    BOOL                                _addItemLimitStrInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal, PlayerAttributes* simulation_player_attributes = NULL);


    BOOL                                _addItemLimitDexInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal, PlayerAttributes* simulation_player_attributes = NULL);
    BOOL                                _addItemLimitIntInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal, PlayerAttributes* simulation_player_attributes = NULL);
    BOOL                                _addItemLimitVitInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal, PlayerAttributes* simulation_player_attributes = NULL);
    BOOL                                _addItemLimitSprInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal, PlayerAttributes* simulation_player_attributes = NULL);
    BOOL                                _addItemLimitSkillInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType stat_composite_type = kLimitStatCompositeNormal);

    void                                _addItemNextGradeLimitInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, LimitStatCompositeType stat_composite_type);
    void                                _addItemOptionInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);

    BOOL                                _addItemAccOptionForTooltip(SCItemSlot* pItemSlot, const WORD* iOptionKind, const int* iOption, const BYTE* iType, BOOL bPCBang = FALSE, const BYTE* btOptionExEffect = NULL, const BYTE* btOptionExEffect_PCBang = NULL);
    BOOL                                _addItemNativeOptionForTooltip(SCItemSlot* pItemSlot, const WORD* iOptionKind, const int* iOption, const BYTE* iType, BOOL bPCBang = FALSE, const BYTE* btOptionExEffect = NULL, const BYTE* btOptionExEffect_PCBang = NULL);

    void                                _addPoralItemDescForTooltip(SCItemSlot* pItemSlot, CODETYPE MapCode);
    void                                _addItemDescForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addItemEtherChargingForToolTip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addItemAdmissionTimeForToolTip(const BASE_ITEMINFO* pItemInfo);

    void                                _addChargeItemDescForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addChargeItemDateForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);

    void                                _addChargeItemDateFixedAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addChargeItemDateFixedEXAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addChargeItemDateFixedEX2AmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
    void                                _addChargeItemDateFixedQUANTITYAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);

    void                                _addChargeItemDateFixedExpiredTimeForTooltip(void);
    void                                _addChargeItemDateFixedWaitItemTimeForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);

    void                                _addChargeItemInfoForToolTipInCashShop(const BASE_ITEMINFO* pItemInfo, DWORD dwCoolTime, DWORD64 SellEndDate);
    bool                                _addChargeItemInfoForToolTip(const BASE_ITEMINFO* pItemInfo);

    void                                _addItemTradeInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);

    //------------------------------------------------------------------------------
    void                                _GetTradeInfoString(TCHAR* trade_info_string, 
                                                           SCItemSlot* slot_item, 
                                                           const BASE_ITEMINFO* item_info, 
                                                           bool is_customizing_item = false);
    WzColor                             _GetSimulationLimitedInfoColor(eATTR_TYPE attr_type, SCItemSlot* item_slot, PlayerAttributes* player_attributes, WORD limit_item_value, WORD limit_value, WzColor default_text_color);

    void                                _addFateItemDescriptionForTooltip();
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    void                                _addItemCashTypeTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo);
#endif//_NA000424_GLOBAL_CASH_ITEM_RENEWAL_

    void                                _addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, const char* wStat1, WzColor Color1);
    void                                _addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, DWORD wStat1, WzColor Color1);
    void                                _addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, DWORD wStat1, WzColor Color1, char *divstr, DWORD wStat2, WzColor Color2);
    void                                _addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, float wStat1, WzColor Color1, char *divstr, DWORD wStat2, WzColor Color2);

    void _addItemPriceForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo, BOOL bBuyPrice = TRUE, LimitStatCompositeType prpurchase = kLimitStatCompositeNormal);
#ifdef _NA_002935_20110704_ITEM_SCORE
    void                                _addItemScoreForTooltip(SCItemSlot* item_slot, LimitStatCompositeType limitstat_composite_type);
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    void                                _addItemHonorRatingForPrice(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo, ShopDialog* shop_dialog, LimitStatCompositeType prpurchase);
#endif //_NA_003027_20111013_HONOR_SYSTEM
    float                               _getLimitItemDura(float maxDura);
    BOOL                                _IsexceptionPercentageType( int OptionKind);

    void                                CalcSize( SIZE& size );
    void                                RecalcRect( int x, int y, const SIZE& size, BOOL drawBkgnd );
    void                                RecalcRectFromBase( const RECT& baseRect, const SIZE& size );

    void                                RenderTooltip(int x, int y, BOOL drawBkgnd = TRUE );
    void                                RenderTooltipEx(const RECT& baseRect, BOOL drawBkgnd = TRUE );

    void                                DrawTooltip( BOOL drawBkgnd );
    int                                 ConvertOptValForSpecialCase( int optKind,int opType, int val );
    void                                _addIPosessionItemTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo);

private:
    void                                _ListUpComposeList();
    void                                _addItemComposeList(const BASE_ITEMINFO* pItemInfo);
private:
    static MATERIAL_COMPOSE_HASH*       m_pMaterialComposeHash;

private:
    void                                LoadTextureExpGauge();
    void                                RenderSpecialTooltipTexture4ExpGauge(RECT* drawRect, float exp_percentage);
    void                                RegisterTooltipPerkOptionDesc(int desc_code, int width = 250);
    void                                RegisterTooltipPerkBenefitOption(const PerkInfo* perk_info);
    void                                RegisterTooltipPerkPenaltyOption(const PerkInfo* perk_info);

    void                                _addPerkCashItemOption(const BASE_ITEMINFO* item_info);


private:
    void                                RenderIconOnTooltip(DWORD dwIconCode, float x, float y);
public:
    void                                RegisterTooltipItem_OnText(const BASE_ITEMINFO* pItemInfo);
    void                                RegisterTooltipNPC_OnText(BASE_NPCINFO* pNPCInfo);

private:
    DWORD                               m_TooltipDialogID;
    cTooltipInfo                        m_Tooltips[MAX_SLOTINFO_LINE];
    int                                 m_iTooltipCounts;
    BOOL                                m_bTooltipTestMode;    // 툴팁 테스트용..(테스트 씬에서 사용)
    TCHAR                               m_szSlotInfoMessage[MAX_SLOTINFO_LINE][MAX_SLOTINFO_LENGTH];

    HANDLE                              m_hSpecialTooltipTexture4LuckyItem[4];
    HANDLE                              m_hSpecialTooltipTextureRandomItem;

    RECT                                m_rect;
    WzColor                             m_PossessionItemColor;

    DWORD                               m_dwIconCode;
    HANDLE                              m_hIconOnTooltip;
    HANDLE                              m_hExpGaugeTexture;


    HANDLE                              m_hTooltipTexture[kTooltipTexture_Max];
    FRECT                               m_TooltipTextureRectAngles[kTexture_Rectangles_Max];


#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    BOOL                                IsUseSkillPowerAbillity( BASE_SkillTextInfo* skill_text_info, int abillity_index );
#endif //_NA_0_20120321_SKILLTREE_SECOND_RENEWAL
};
