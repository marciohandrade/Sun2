#include "SunClientPrecompiledHeader.h"
#include "uitooltipman.h"
#include "Interfacemanager.h"
#include "Application.h"
#include "GameFunc.h"
#include "ItemInfoParser.h"
#include "SkillInfoParser.h"
#include "SkillInventoryStoreParser.h"
#include "itemcomposelistparser.h"
#include "SCItemSlot.h"
#include "StyleSlot.h"
#include "StyleInfoParser.h"
#include "QuickSlot.h"
#include "StyleContainer.h"
#include "QuickContainer.h"
#include "HeroEquipmentContainer.h"
#include "SummonSkillContainer.h"
#include "ItemManager.h"
#include "StateInfoParser.h"
#include "ObjectManager.h"
#include "MouseHandler.h"
#include "ItemFormula.h"
#include "CursorChangeTransact.h"
#include "SkillTextInfoParser.h"
#include "Character.h"
#include "Hero.h"
#include "CooltimeManager.h"
#include "SCSlotStruct.h"
#include "mouse.h"
#include "OptionListParser.h"
#include "MapInfoParser.h"
#include "SetItemOptionInfoParser.h"
#include "InstanceDungeonScene.h"
#include "GraphicResourceList.h"
#include "FateItemInfoParser.h"
#include "ItemTypeList.h"
#include "PetInfoParser.h"
#include "LotteryInfoParser.h"

#include "SkillMain.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "SolarDateTime.h"
#include "GameConst_Color.h"

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
#include "EnduranceDIalog.h"
#endif//__NA_001062_20080623_STRENGTH_PENALTY

#include "QuestManager_Concrete.h"
#include "uiVendorSell/uiVendorSell_def.h"
#include "SkillSystem/PerkInfoParser.h"
#include "SCPerkSlot.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "Char_SkillDialog.h"
#ifdef _NA_002935_20110704_ITEM_SCORE
#include "ItemScoreParser.h"
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#include "FameListParser.h"
#include "ReputeListParser.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "CurrencyInfoParser.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _DH_BATTLEZONE2_
#include "BattlezoneMissionTreeParser.h"
#endif//_DH_BATTLEZONE2_
#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildSystem/GuildSystemData.h"
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL

#include "ssqscene.h"
#include "RiderContainer.h"

#include <ItemOptionHeader.h>
#include <boost/foreach.hpp>

#include "ShopDialog.h"
#include "NPCInfoParser.h"
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
#include "EnchantParser.h"
#endif //_NA_006731_20130521_ENCHANT_ADD_OPTION


#ifdef _INTERNATIONAL_UI
#define TOOLTIP_TEXT_WIDTH_INTERVAL	5
#else
#define TOOLTIP_TEXT_WIDTH_INTERVAL	16
#endif//_INTERNATIONAL_UI

#ifdef _DH_TOOLTIP_LIMIT
#define TOOLTIP_MAXLISTCNT 15
#endif//_DH_TOOLTIP_LIMIT
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
#include "uiPerkDialog/uiPerkDialog.h"
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
#include "ItemCrystalizeListParser.h"
#include "uiInventoryMan/uiInventoryMan.h"
#include "InventoryDialog.h"
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "ChaosZoneInfoParser.h"
  #include "ChaosSystem/GuildMissionTreeParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

const int cMaxGauge = 100;

#if defined(_JAPAN)
		const int ColWidth1 = 100;
		const int ColWidth2 = 150;
		const int ColWidth3 = 200;
#elif defined(_RUSSIA)
        const int ColWidth1 = 130;
        const int ColWidth2 = 130;
        const int ColWidth3 = 180;
#else
		const int ColWidth1 = 87;
		const int ColWidth2 = 130;
		const int ColWidth3 = 180;
#endif

enum ITEM_MIXTURE_TYPE				// �ͽ��� Ÿ��
{
	IMT_EVERYTHING					= 0x00,		// ��ΰ���
	IMT_INCHANT						= 0x01,		// ��æ�常 ����
	IMT_RANKUP						= 0x02,		// ��ũ���� ����
	IMT_CRYSTALLIZATION				= 0x04,		// ����ȭ�� ����
	IMT_MIXTURE						= 0x08,		// ���ո� ����
	IMT_EXTRA_STONE					= 0x10,		// ����Ʈ���� ����
	IMT_SOCKET_FILL					= 0x20,		// ���� ä��� ����
	IMT_SOCKET_REMOVE_IMPOSSIBLE	= 0x40,		// ���� ���� �Ұ���
	IMT_IMPOSSIBLE_EVERYTHING		= 0xff,		// ��� �Ұ���
};

// Ŭ���̾�Ʈ���� Quick���Կ��� Deactive �������� �Ǵ��Ҷ��� ����.�ļ�..-_-;
#define QUICKSLOT_DEACTIVE_FATEITEM_STATUS_VALUE	55555

MATERIAL_COMPOSE_HASH* uiToolTipMan::m_pMaterialComposeHash = NULL;

//------------------------------------------------------------------------------ 
//! static function
void uiToolTipMan::Callback_Tooltip(CDialogWZ* dialog_ptr, SI_ToolTip* tooltip,int x, int y)
{
    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if ((tooltip_manager != NULL) && (tooltip != NULL))
    {
        tooltip_manager->RegisterTooltipWithStringCode(tooltip->m_dwStrID);
    }
}

//------------------------------------------------------------------------------ 
uiToolTipMan::uiToolTipMan(InterfaceManager *pUIMan) : uiBaseMan(pUIMan)
{
	Clear();
	_ListUpComposeList();
}

void    uiToolTipMan::Clear()
{
	m_bTooltipTestMode		= FALSE;
	m_iTooltipCounts		= 0;
	for(int i = 0; i < 4; ++i)
	{
		m_hSpecialTooltipTexture4LuckyItem[i] = INVALID_HANDLE_VALUE;
	}

	m_hSpecialTooltipTextureRandomItem = INVALID_HANDLE_VALUE;

	m_dwIconCode = 0;
	m_hIconOnTooltip = INVALID_HANDLE_VALUE;
    m_hExpGaugeTexture = INVALID_HANDLE_VALUE;

    for(int index = kTooltipTexture_Start; index < kTooltipTexture_Max; ++index)
    {
        m_hTooltipTexture[index] = INVALID_HANDLE_VALUE;
    }

	memset( &m_rect, 0, sizeof( m_rect ) );
}

void    uiToolTipMan::OnRelease()
{
	if (m_pMaterialComposeHash)
	{
		MATERIAL_COMPOSE_LIST * pComposeList = NULL;
		m_pMaterialComposeHash->SetFirst();
		while(pComposeList = m_pMaterialComposeHash->GetNext())
		{
			SAFE_DELETE( pComposeList );
		}
		m_pMaterialComposeHash->RemoveAll();
		SAFE_DELETE(m_pMaterialComposeHash);
	}	
}

void    uiToolTipMan::OnInitialize()
{
	m_TooltipDialogID = 0;
	
    LoadTextureExpGauge();

    InitTextures();
}

void	uiToolTipMan::Render()
{
	if(m_bTooltipTestMode)
	{
		RenderTooltip( 10, 100);
	}
	else
	{
		const int TOOLTIP_OFFSET = 34;

		//���� ���콺 Ŀ���� �� ���¿����� ���� ������
		if( MouseCursor::Instance()->IsShowCursor() )
		{
			RenderTooltip(
				Mouse::Instance()->GetMouseX() + TOOLTIP_OFFSET,
				Mouse::Instance()->GetMouseY() + TOOLTIP_OFFSET );
		}
        else
        {
            // Ŀ���� ������ ��ϵ� ���� ����
            this->InitTooltip();
        }
		
	}
}


void uiToolTipMan::RenderEx( const RECT& baseRect )
{
	RenderTooltipEx( baseRect );
}


#define		BUG_FIX_ITEM_TOOLTIP_RANK_OPTION	//	2006�� 2�� 13�� �Ӵ���
												//	�Ǽ��縮 ������ ������ ��ũ �ɼ� �׸� ��µ� �ؽ�Ʈ �ڵ尡 ���
												//	�ϴ� ��ũ��Ʈ�� ���� ������ �迭�� �ϵ� �ڵ�

/*
	ID		��Ʈ��	ũ��	��Ÿ��	��Ÿ
	s210	����ü	8		Bold	
	st10	����ü	8		Bold
	m212	����ü	10		Bold
	mn12	����ü	10		Bold
	tt12	����ü	12		Bold
	m216	����ü	14
	mi16	�鱹ȭ	14
	sn24	�鱹ȭ	24
	zn44	�鱹ȭ	44
*/

static const int TOOLTIP_TEXT_WIDTH = 220 + 20;
static const int MIN_TOOLTIP_WIDTH = 50;
static const int TOOLTIP_START_Y = 10;

static const int TOOLTIP_WIDTH_WIDE = 420 + 20;

const WzID			FontID_SkillName	= StrToWzID("m212");
const WzID			FontID_SkillInfo	= StrToWzID("mn12");

const WzID			FontID_ItemName		= StrToWzID("m212");
const WzID			FontID_ItemInfo		= StrToWzID("mn12");

const WzID			FontID_8	= StrToWzID("s210");
const WzID			FontID_10	= StrToWzID("m212");	
const WzID			FontID_12	= StrToWzID("tt12");
const WzID			FontID_12_1	= StrToWzID("mn12");	
const WzID			FontID_14	= StrToWzID("m216");	
const WzID			FontID_24	= StrToWzID("sn24");
const WzID			FontID_14_1 = StrToWzID("mi16");	// �鱹ȭ

const WzColor		WzCurSkillLevelColor = WzColor_RGBA( 188, 240, 120, 255 );
const WzColor		WzCurSkillColor = WzColor_RGBA( 136, 221, 178, 255 );
const WzColor		WzCurSkillDescColor = WzColor_RGBA( 247, 251, 190, 255 );

const WzColor		WzNextSkillLevelColor = WzColor_RGBA( 244, 120, 82, 255 );
const WzColor		WzNextSkillColor = WzColor_RGBA( 190, 80, 61, 255 );
const WzColor		WzNextSkillDescColor = WzColor_RGBA( 229, 104, 75, 255 );

const WzColor		WzSkillErrorColor = WzColor_RGBA( 229, 75, 75, 255 );
const WzColor		WzSkillDisableColor = WzColor_RGBA( 150, 150, 150, 255 );
const WzColor		WzDivineNameColor = WzColor_RGBA( 176, 118, 242, 255 );
const WzColor		WzEliteNameColor = WzColor_RGBA(250,248,75,255);
const WzColor		WzLuckyEliteColor = WzColor_RGBA(255,255,0,255);
const WzColor		WzUniqueNameColor = WzColor_RGBA(183,0,0,255);
const WzColor		WzNameColor = WzColor_RGBA( 253, 183, 0, 255 );
const WzColor		WzTitleColor = WzColor_RGBA( 179, 244, 166, 255 );
const WzColor		WzNormalColor = WzColor_RGBA( 255, 255, 255, 255 );
const WzColor		WzSpaceColor = WzColor_RGBA( 255, 255, 255, 0 );
const WzColor		WzWaitItemColor		= WzColor_RGBA( 128, 128, 128, 255 );
const WzColor		WzInstanceDungeonItemColor		= WzColor_RGBA( 253, 250, 1, 255 );
const WzColor		WzLimitedItemColor		= WzColor_RGBA( 167, 167, 167, 255 );
const WzColor		WzErrorColor = WzColor_RGBA( 204, 0, 0, 255 );
const WzColor		WzDescriptionColor = WzColor_RGBA(80,240,120,255);
const WzColor		WzEffectColor	= WzColor_RGBA( 51, 240, 51, 255 );
const WzColor		WzPcBangColor		= WzColor_RGBA( 211, 191 , 60 , 255);
const WzColor		WzNonPcBangColor	= WzColor_RGBA(116 , 116 ,116 ,170);
const WzColor		WzDescColor		= WzColor_RGBA( 255, 255, 153, 255 );
const WzColor		WzDisableColor	= WzColor_RGBA( 153, 153, 153, 255 );
const WzColor		WzSkillNameColor = WzColor_RGBA( 249, 165, 44, 255 );
const WzColor		WzSkillDescColor = WzColor_RGBA( 222, 239, 74, 255 );
const WzColor		WzChargeItemDateColor = WzColor_RGBA( 222, 239, 74, 255 );
const WzColor		WzSetItemColor = WzColor_RGBA(146, 135, 80, 255);
const WzColor		WzFullSetItemColor = WzColor_RGBA(71, 80, 183, 255);
const WzColor		WzExerciseColor = WzColor_RGBA(2, 207, 251, 255);
const WzColor       WzPassiveTextColor = WzColor_RGBA( 255, 255, 255, 255);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
const WzColor       WzAwakeningColor = WzColor_RGBA(210, 163, 0, 255);
const WzColor       WzAwakeningEffectColor = WzColor_RGBA(0, 255, 0, 255);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

const int			iSkillNameLineHeight = 25;

const int			iTitleLineHeight = 14 + 2;
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
   const int			iSpaceLineHeight = 8;
#else
   const int			iSpaceLineHeight = 12;
#endif //_SCJ_SETITEM_TOOLTIP_REFORM

const float			c_fDuraRate			= 5.f;
const int			iNormalLineHeight	= 12 + 4;
const int			iLineHeight			= 24;
const int			iNameLineHeight		= 19;
const int			iLargeFontHeight	= 27;
const int			iSmallFontHeight	= 12;
const int			iSimpleTooltipHeight		= 23;

enum eTOOLTIP_BUFF_DESC_TYPE
{
	eTOOLTIP_BUFF_DESC_TYPE0 = 0,	// ��� Parameter�� �������� �ʴ³��
	eTOOLTIP_BUFF_DESC_TYPE1,		// Option 1 �� ����
	eTOOLTIP_BUFF_DESC_TYPE2,		// Parameter1 �� ����
	eTOOLTIP_BUFF_DESC_TYPE3,		// Parameter2 �� ���� (Param1�� ���� ��ȭ����)
	eTOOLTIP_BUFF_DESC_TYPE4,		// Parameter2 �� ���� (Param1�� ���� ��ȭ����)
	eTOOLTIP_BUFF_DESC_TYPE5,		// Parameter1, Parameter2 ����
	eTOOLTIP_BUFF_DESC_TYPE6,		// Parameter2, Option2 ����
};


//----------------------------------------------------------------------------------------
#define MODFLOAT(ratio) ((ratio)<0 ? (-0.5f):(0.5f)) //(Ŭ��&������ ����,����Ʈ ��ġ����ȭ(�ݿø�����))


//----------------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableShowBuffTooltip(SKILL_EFFECT_IMAGE *pImage)
{	
	if(!_isEnableShowTooltip())
		return FALSE;

	assert(pImage);

	if(!pImage) return FALSE;
	
	return TRUE;

}
#ifdef _INTERNATIONAL_DATE_TOOLTIP
//----------------------------------------------------------------------------------------
int uiToolTipMan::GetStringMonth(int nMonth)
{
	//11071: 1�� Jan	 
	int nStringCode = 11071 + nMonth - 1;
	nStringCode = max(nStringCode, 11071);
    nStringCode = min(nStringCode, 11082);

	return nStringCode;
}
#endif//_INTERNATIONAL_DATE_TOOLTIP
//----------------------------------------------------------------------------------------
void uiToolTipMan::_addBuffNameForTooltip(BASE_STATEINFO* pStateInfo)
{
	TCHAR	szString[INTERFACE_STRING_LENGTH + 1] = {0,};
	g_InterfaceManager.GetInterfaceString(pStateInfo->m_dwNamecode,szString,INTERFACE_STRING_LENGTH);
	TCHAR	szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
	int		iStringCode = 0;
#ifdef _NA_006819_20130719_STATE_ICON_RENDER
	switch(pStateInfo->m_byIconKind)
	{
	case eSTATE_KIND_DEBUFF:    iStringCode = 11396; break;    //(���� �̻�)
    case eSTATE_KIND_ITEM:      iStringCode = 11393; break;    //(��ȭ)
	case eSTATE_KIND_CASH:      iStringCode = 11394; break;    //(ĳ��)
    case eSTATE_KIND_ETC:       iStringCode = 11395; break;    //(Ư��)
	case eSTATE_KIND_SHIELD:    iStringCode = 11397; break;    //(�߿�)
    case eSTATE_KIND_SKILL:     iStringCode = 11400; break;    //(��ų)
	}
#else
    switch(pStateInfo->m_byIconKind)
    {
    case eSTATE_KIND_DEBUFF: //(���� �̻�)
        iStringCode = 11396;
        break;
    case eSTATE_KIND_SKILL:  //(��ų)
        iStringCode = 11393;
        break;
    case eSTATE_KIND_CASH:   //(ĳ��)
        iStringCode = 11394;
        break;
    case eSTATE_KIND_SHIELD: //(����)
        iStringCode = 11397;
        break;
    case eSTATE_KIND_ETC:    //(��Ÿ)
        iStringCode = 11395;
        break;
    }
#endif // _NA_006819_20130719_STATE_ICON_RENDER

	DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
	sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

	g_InterfaceManager.GetInterfaceString(iStringCode, szTemp);
	Snprintf(szFinal, INTERFACE_STRING_LENGTH, "%s%s", szString, szTemp);
	RegisterTooltip(szFinal,-1,
				iSkillNameLineHeight,
				FontID_SkillName,
				WzSkillNameColor,
				0,
				1,
				sp_tooltip);
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void uiToolTipMan::_addBuffNameForTooltip( BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE* pImage )
{
    TCHAR	szString[INTERFACE_STRING_LENGTH + 1] = {0,};
    g_InterfaceManager.GetInterfaceString(pStateInfo->m_dwNamecode,szString,INTERFACE_STRING_LENGTH);
    TCHAR	szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
    TCHAR	szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
    int		iStringCode = 0;

    switch(pStateInfo->m_byIconKind)
    {
    case eSTATE_KIND_DEBUFF:    iStringCode = 11396; break;    //(���� �̻�)
    case eSTATE_KIND_ITEM:      iStringCode = 11393; break;    //(��ȭ)
    case eSTATE_KIND_CASH:      iStringCode = 11394; break;    //(ĳ��)
    case eSTATE_KIND_ETC:       iStringCode = 11395; break;    //(Ư��)
    case eSTATE_KIND_SHIELD:    iStringCode = 11397; break;    //(�߿�)
    case eSTATE_KIND_SKILL:     iStringCode = 11400; break;    //(��ų)
    }

    int effect_num = 0;
    if (pImage)
    {
        Character *pChr = (Character *)g_ObjectManager.GetObject(pImage->dwOwnerID);
        if (pChr)
        {
            SKILL_EFFECT* skill_effect_ptr = pChr->FindSkillEffect(static_cast<eCHAR_STATE_TYPE>(pImage->dwStatusID));
            if (skill_effect_ptr)
            {
                effect_num = skill_effect_ptr->overlap_status_count_;
            }
        }
    }

    DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
    sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

    g_InterfaceManager.GetInterfaceString(iStringCode, szTemp);
    if (effect_num > 1)
    {
        Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, "%sx%d", szTemp, effect_num);
    }
    Snprintf(szFinal, INTERFACE_STRING_LENGTH, "%s%s", szString, szTemp);
    RegisterTooltip(szFinal,-1,
        iSkillNameLineHeight,
        FontID_SkillName,
        WzSkillNameColor,
        0,
        1,
        sp_tooltip);
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//----------------------------------------------------------------------------------------
int	uiToolTipMan::_getBuffDescType(DWORD dwStatusID)
{
	switch(dwStatusID) 
	{
		// ��� Parameter�� �������� �ʴ³��
		default:
		case eCHAR_STATE_CHAOS:
		case eCHAR_STATE_BLIND:
		case eCHAR_STATE_DOWN:
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
        case eCHAR_STATE_UPPERDOWN:
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
		case eCHAR_STATE_DELAY:
		case eCHAR_STATE_SEALING:
		case eCHAR_STATE_STUN:
		case eCHAR_STATE_STONE:
		case eCHAR_STATE_SLEEP:
		case eCHAR_STATE_FROZEN:
		case eCHAR_STATE_SEQUELA:
		case eCHAR_STATE_REFLECT_FEAR:
		case eCHAR_STATE_PROTECTION:
		case eCHAR_STATE_HOLDING:
		case eCHAR_STATE_TRANSFORMATION:
		case eCHAR_STATE_STEEL :
		case eCHAR_STATE_TRANSPARENT:
		case eCHAR_STATE_FEAR:
		case eCHAR_STATE_BLUR:
			return eTOOLTIP_BUFF_DESC_TYPE0;

		// Option 1
		case eCHAR_STATE_REFLECT_STUN:
		case eCHAR_STATE_REFLECT_FROZEN:
		case eCHAR_STATE_REFLECT_DAMAGE:
			return eTOOLTIP_BUFF_DESC_TYPE1;

	    // Parameter1
		case eCHAR_STATE_REFLECT_SLOW:
		case eCHAR_STATE_REFLECT_SLOWDOWN:
			return eTOOLTIP_BUFF_DESC_TYPE2;

		// Parameter2 Param1�� ���� ��ȭ����
		case eCHAR_STATE_POISON :
		case eCHAR_STATE_WOUND :
		case eCHAR_STATE_FIRE_WOUND :
		case eCHAR_STATE_FETTER :
		case eCHAR_STATE_SLOWDOWN :
		case eCHAR_STATE_HIGH_SPIRIT :
		case eCHAR_STATE_SPEEDING :
		case eCHAR_STATE_INCREASE_SKILLRANGE :
		case eCHAR_STATE_PRECISION :
		case eCHAR_STATE_BUF_RANGE_DAMAGE:
			return eTOOLTIP_BUFF_DESC_TYPE3;

		// Parameter2 Param1�� ���� ��ȭ����
		case eCHAR_STATE_PANIC:
		case eCHAR_STATE_LOW_SPIRIT :
		case eCHAR_STATE_WEAKENING :
		case eCHAR_STATE_ATTACK_FAIL:
		case eCHAR_STATE_FRUSTRATION:
		case eCHAR_STATE_HP_REDUCE:
		case eCHAR_STATE_MP_REDUCE:
		case eCHAR_STATE_LOW_STRENGTH:
		case eCHAR_STATE_DICLINE:
		case eCHAR_STATE_MAGIC_EXPOSE:
		case eCHAR_STATE_ABSORB:
		case eCHAR_STATE_HPRATE_REDUCE:
		case eCHAR_STATE_MPRATE_REDUCE:
		case eCHAR_STATE_ANGER:
		case eCHAR_STATE_DEFENSE:
		case eCHAR_STATE_FIGHTING:
		case eCHAR_STATE_BALANCE:
		case eCHAR_STATE_VITAL_POWER:
		case eCHAR_STATE_MEDITATION:
		case eCHAR_STATE_CONCENTRATION:
		case eCHAR_STATE_HP_INCREASE:
		case eCHAR_STATE_MP_INCREASE:
		case eCHAR_STATE_HPRATE_INCREASE:
		case eCHAR_STATE_MPRATE_INCREASE:
		case eCHAR_STATE_MAGIC_DEFENSE:		 
		case eCHAR_STATE_STAT_STR:
		case eCHAR_STATE_STAT_DEX:
		case eCHAR_STATE_STAT_VIT:
		case eCHAR_STATE_STAT_INT:
		case eCHAR_STATE_STAT_SPI:
		case eCHAR_STATE_MAGIC_ATTACK_INCREASE:
		case eCHAR_STATE_MAGIC_ATTACK_DECRESE:
		case eCHAR_STATE_STAT_STR_DECRESE:
		case eCHAR_STATE_STAT_DEX_DECRESE:
		case eCHAR_STATE_STAT_VIT_DECRESE:
		case eCHAR_STATE_STAT_INT_DECRESE:
		case eCHAR_STATE_STAT_SPI_DECRESE:
			return eTOOLTIP_BUFF_DESC_TYPE4;

		// Parameter1, Parameter2
		case eCHAR_STATE_SP_BONUS:
			return eTOOLTIP_BUFF_DESC_TYPE5;

		// Parameter2,Option2
		case eCHAR_STATE_MAGIC_SHIELD:	    
			return eTOOLTIP_BUFF_DESC_TYPE6;
	}

	return eTOOLTIP_BUFF_DESC_TYPE0;
}

//----------------------------------------------------------------------------------------
void uiToolTipMan::_addBuffDescForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage, BOOL bShowDelStr)
{
	TCHAR	szFormat[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szParam[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szParam2[INTERFACE_STRING_LENGTH] = {0,};
	
	SKILL_EFFECT *pEffect = NULL;

	Character *pChr = (Character *)g_ObjectManager.GetObject(pImage->dwOwnerID);
	if (pChr)
	{
		pEffect = pChr->GetSkillEffect(pImage->iEffectID);
	}
			
	if (!pEffect) return;
		
	g_InterfaceManager.GetInterfaceString(pStateInfo->m_dwDescCode,szFormat,INTERFACE_STRING_LENGTH);

	switch(_getBuffDescType(pImage->dwStatusID)) 
	{
	case eTOOLTIP_BUFF_DESC_TYPE0:
		StrnCopy(szText,szFormat,INTERFACE_STRING_LENGTH);
		break;

	case eTOOLTIP_BUFF_DESC_TYPE1:
		Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iOption1);
		Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam);
		break;
	
	case eTOOLTIP_BUFF_DESC_TYPE2:
		Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[0]);
		Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam);
		break;

	case eTOOLTIP_BUFF_DESC_TYPE3:
		Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
		Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam);
		break;		 

	case eTOOLTIP_BUFF_DESC_TYPE4:
		{
			if (pEffect->AbilityInfo.m_iParam[0] == 1) 
			{
				Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
			}
			else
			{
				Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld%% "),pEffect->AbilityInfo.m_iParam[1]);

			}
			Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam);

		}
		break;

	case eTOOLTIP_BUFF_DESC_TYPE5:
		Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[0]);
		Snprintf(szParam2,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
		Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam,szParam2);
		break;

	case eTOOLTIP_BUFF_DESC_TYPE6:	    
		Snprintf(szParam,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
		Snprintf(szParam2,INTERFACE_STRING_LENGTH-1,_T("%ld "),pEffect->AbilityInfo.m_iOption2);
		Snprintf(szText,INTERFACE_STRING_LENGTH-1,szFormat,szParam,szParam2);
		break;

	}

	AddEmptyLineForTooltip();

	if(TRUE == bShowDelStr)
	{
		TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
		if(0 == pStateInfo->m_bDelType)
		{
			//11392 (���� �Ұ��� �� ���� �Դϴ�.)
			g_InterfaceManager.GetInterfaceString(11392, szTemp);
		}
		else
		{
			//11391 (���� ���� �� ���� �Դϴ�.)
			g_InterfaceManager.GetInterfaceString(11391, szTemp);
		}

		Snprintf(szFinal, INTERFACE_STRING_LENGTH, "%s%s", szText, szTemp);

		RegisterTooltipLineChange(szFinal,
			_tcslen(szFinal),
			iNormalLineHeight,
			FontID_SkillInfo,
			WzSkillDescColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
	else 
	{
		RegisterTooltipLineChange(szText,
			_tcslen(szText),
			iNormalLineHeight,
			FontID_SkillInfo,
			WzSkillDescColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}

//----------------------------------------------------------------------------------------
void uiToolTipMan::_addBuffRemainTimeForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMin[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szSec[INTERFACE_STRING_LENGTH] = {0,};
	
	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;

	SKILL_EFFECT *pEffect = NULL;

	DWORD dwRemainTime = 0;
	DWORD dwRemainSecond = 0;
	DWORD dwRemainMinute = 0;

	Character *pChr = (Character *)g_ObjectManager.GetObject(pImage->dwOwnerID);
	if (pChr)
	{
		pEffect = pChr->GetSkillEffect(pImage->iEffectID);
	}
			
	if(!pEffect) return;

	if(pEffect->iRemainTime > 0)
	{
		dwRemainTime = pEffect->iRemainTime / 1000;
		dwRemainSecond = dwRemainTime % 60;
		dwRemainMinute = dwRemainTime / 60;

		//	�����ð� : %d�� %d��
		g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME_TEXT, szTime, INTERFACE_STRING_LENGTH );
		g_InterfaceManager.GetInterfaceString( eST_MINUTE_TEXT, szMin, INTERFACE_STRING_LENGTH );
		g_InterfaceManager.GetInterfaceString( eST_SECOND_TEXT, szSec, INTERFACE_STRING_LENGTH );

		if (dwRemainMinute > 0)
		{
			Snprintf( szMessage,INTERFACE_STRING_LENGTH-1, _T("%s : %d%s %d%s"), szTime, dwRemainMinute, szMin, dwRemainSecond, szSec );
		}
		else if (dwRemainSecond > 0)
		{
			Snprintf( szMessage,INTERFACE_STRING_LENGTH-1, _T("%s : %d%s"), szTime, dwRemainSecond, szSec );
		}
		else
			return;

		RegisterTooltip( szMessage,
				-1,
				iNormalLineHeight,
				FontID_SkillInfo,
				WzNormalColor,
				TOOLTIP_BG_WZCOLOR,
				dwTooltipExtraStyle);

	}
}

//----------------------------------------------------------------------------------------
void uiToolTipMan::RegisterSkillEffectInfoTooltip(SKILL_EFFECT_IMAGE *pImage, BOOL bShowDelStr /*= FALSE*/)
{
	if(!_isEnableShowBuffTooltip(pImage)) return;

	InitTooltip();

	BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(pImage->dwStatusID);
	
	if (pInfo)
	{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        _addBuffNameForTooltip(pInfo, pImage);
#else
		_addBuffNameForTooltip(pInfo);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		_addBuffRemainTimeForTooltip(pInfo, pImage);
		_addBuffDescForTooltip(pInfo, pImage, bShowDelStr);
	}
}

//-----------------------------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableShowItemTooltip(const SCSlot *pSlot)
{
	if(!pSlot)
		return FALSE;

	if(!_isEnableShowTooltip())
		return FALSE;

	assert(ST_ITEM == pSlot->GetSlotType() );
	if (ST_ITEM != pSlot->GetSlotType())
	{
		return FALSE;
	}

	if (!ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode()))
        return FALSE;


	return TRUE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemEnchant(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if(!pItemSlot)
		return FALSE;

	if(pItemSlot->GetEnchant() <= 0)
		return FALSE;

	
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    if(pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemSlot->IsWingItem())
#else
    if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
    {
		return TRUE;
    }
	return FALSE;
}


//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemSocket(const BASE_ITEMINFO *pItemInfo)
{
	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemType(const BASE_ITEMINFO *pItemInfo)
{
	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemPet(const BASE_ITEMINFO *pItemInfo)
{
	if(pItemInfo->m_wType == eITEMTYPE_PET)
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemEquipClass(const BASE_ITEMINFO *pItemInfo)
{
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    bool is_wing_item = (pItemInfo->item_rule_info.is_accessory_wings == true) &&
        (pItemInfo->item_rule_info.is_accessory_special == true);
	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon() || (is_wing_item == true))
#else
    if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemDurability(const BASE_ITEMINFO *pItemInfo)
{
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    //������ �������� ���ٸ� �̷��� �ؾ���
    //�� �Լ��� ����ϴ� ���� ���� �� ���Ƽ� �ϴ� �־�� (2011.7.20 ������)
    bool is_wing_item = (pItemInfo->item_rule_info.is_accessory_wings == true) &&
        (pItemInfo->item_rule_info.is_accessory_special == true);
    if(pItemInfo->IsArmor() || pItemInfo->IsWeapon() || (is_wing_item == true))
#else
    if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableItemOption(const BASE_ITEMINFO *pItemInfo)
{
	if( pItemInfo->IsArmor()  || 
        pItemInfo->IsWeapon() ||
        pItemInfo->IsAccessory() || 
        pItemInfo->IsRiderPartsItem()||
        pItemInfo->IsSpecialAccessory())
		return TRUE;

	for(int i = 0; i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
	{
        if( pItemInfo->IsExistEquipOptions() )
            return TRUE;
        if( pItemInfo->IsExistInvenOptions() )
            return TRUE;
	}

#ifdef _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	if( pItemInfo->m_wType == eITEMTYPE_COSTUME || 
		pItemInfo->m_wType == eITEMTYPE_COSTUME_DECORATE )
#else
    if( pItemInfo->m_wType == eITEMTYPE_COSTUME )
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    {
        return TRUE;
    }
#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    if( pItemInfo->m_wType == eITEMTYPE_PCBANG )
    {
        return TRUE;
    }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

	return FALSE;
}


BOOL uiToolTipMan::_isEnableChargeItemDesc(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
    if (ItemManager::IsChargeItem(pItemSlot) == true)
    {
        return TRUE;
    }

    if ((pItemInfo != NULL) && 
        (pItemInfo->IsChargeItem() == true))
    {
        return TRUE;
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
/** �Ǽ��縮 �÷��� �ɼ�üũ�κ�

    Ǯ�¾������� �����ѻ��¿��� �÷�����Ʈ�ɼ��� ������ �Ǽ��縮�� ������� ������ �׷��ִ� �ڵ��̴�

    1:armor������ ��Ʈ�����ۿɼ������� SpecialItemCode�ڵ尡 �����ϴ���üũ
    2:�ڵ尡 �����Ѵٸ�  ���ڵ�� �Ǽ��縮�����ڵ�� ��ġ�ϴ��� üũ
    3:�Ѵٸ´ٸ� ���� ��Ţ�� Ǯ��üũ���Ѵ�    

*/
void uiToolTipMan::_addItemFullSetPlusOptionForTooltip(const BASE_ITEMINFO* pItemInfo)
{
    //�÷�����Ʈ�ɼ� ���������� üũ�Ѵ�( ==2)
	if ((pItemInfo->m_bySetOptionType != 2) || 
       (pItemInfo->set_group_code == 0))
    {
        return;
    }

    //safe_code: ��Ţ�����̳ʿ� ��Ʈ�ɼ��ļ�
    EquipmentContainer* equip_info_ptr = static_cast<EquipmentContainer*>(ItemManager::Instance()->GetContainer(SI_EQUIPMENT));
    SetItemOptionInfoParser* setitem_info_ptr = SetItemOptionInfoParser::Instance();

    if ((equip_info_ptr == NULL) && 
       (setitem_info_ptr== NULL) )
    {
        return;
    }

    bool is_exgist_item = false;
    bool is_fullset = false;
    bool is_valid_plus_option = false;

    //1:armor������ ��Ʈ������ �ɼ��� SpecialItemCode�� üũ�Ѵ�
    SCItemSlot& armor_slot = (SCItemSlot &)equip_info_ptr->GetSlot(eEQUIPCONTAINER_ARMOR);
    SETITEM_OPTION_INFO* acc_set_option = setitem_info_ptr->Find(pItemInfo->set_group_code);
    const BASE_ITEMINFO* armor_item_info = NULL;
    SETITEM_OPTION_INFO* armor_set_option = NULL;

    //�������� ��Ţ�� ���� �����ϴ��� üũ
    //if (equip_info_ptr->IsEmpty((POSTYPE)pItemInfo->m_wEqPos) == FALSE)
    //{
    //    is_exgist_item = true;
    //}
    if (ItemManager::Instance()->FindFirstItemSlot(SI_EQUIPMENT, pItemInfo->m_Code) != NULL)
    {
        is_exgist_item = true;
    }
     
    if (is_exgist_item && 
        armor_slot.IsContainValidItem())
    {
        armor_item_info =  armor_slot.GetItemInfo();
        
        if (armor_item_info)
        {
            armor_set_option = setitem_info_ptr->Find(armor_item_info->set_group_code);

            if (armor_set_option && 
               (armor_set_option->m_SpecialEquipOptionItem.m_wItemCode != 0) &&
               (armor_set_option->m_SpecialEquipOptionItem.m_wItemCode == pItemInfo->m_Code))
            {
                //������ ������ �Ƹӿ� ���� �Ǽ��縮�� ����
                is_valid_plus_option = true;
            }
        }   
    
    }

    //2:��ȿ�ϴٸ� Ǯ������ üũ�Ѵ�(armor������ �����͵� ��ȿ��)
    if (is_valid_plus_option)
    {
        //Ǯ�� �������� üũ
        int set_item_number = 0; 

        BYTE equip_pos = eEQUIPCONTAINER_WEAPON;

        const BASE_ITEMINFO* slot_item = NULL;

        ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = equip_info_ptr->GetEquipmentManageInfo();

        for (int equip_pos = 0; equip_pos < equip_info_ptr->GetMaxSlotNum(); ++equip_pos)
        {
            //���� ��ȿ���� üũ
            SCItemSlot& item_slot = (SCItemSlot&)equip_info_ptr->GetSlot(equip_pos);
            if (item_slot.IsContainValidItem() == FALSE)
            {
                continue;
            }

            slot_item = ItemInfoParser::Instance()->GetItemInfo(item_slot.GetCode());   

            //������ �����Ͱ� ��ȿ���� üũ
            if (slot_item == NULL)
            {
                continue;
            }
            // ARMOR�� üũ    
            if (slot_item->IsArmor() == FALSE)
            {
               continue;  
            }

            // ��ƮŸ���� ������ üũ
            if (armor_item_info->set_group_code != slot_item->set_group_code)
            {
                continue;
            }

            // �ش� ������ Ȱ��ȭ �Ǿ� �ִ��� üũ
            ns_formula::EquipSlotsManageInfo::Node* equip_node = equip_slot_manage_info->slots;
            if (equip_node[item_slot.GetPos()].activated == false)
            {
                continue;
            }

            set_item_number++;
        }
        
        if (armor_set_option->m_byActiveFullOptionNum <= set_item_number)
        {
            is_fullset = true;
        }
    }

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp0[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp1[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szSetOpt[INTERFACE_STRING_LENGTH + 1] = {0,};

	for(int i = 0; i < MAX_SETITEM_SPECIAL_OPTION; ++i)
	{
		BYTE use_type = acc_set_option->m_SpecialEquipOptionItem.m_ActiveOption[i].m_byUseType;
		int  option_idx = acc_set_option->m_SpecialEquipOptionItem.m_ActiveOption[i].m_nOption;
		BYTE option_kind = acc_set_option->m_SpecialEquipOptionItem.m_ActiveOption[i].m_byOptionKind;

		if((use_type == 0) &&
          (option_idx == 0))
		{
			continue;
		}
		

		BASE_OptionList* option_list_ptr = OptionListParser::Instance()->GetOptionList(option_kind);
		//�ɼǸ�.
		g_InterfaceManager.GetInterfaceString(option_list_ptr->mdwNameCode, szTemp, INTERFACE_STRING_LENGTH);

		//%d���̻� �ɼ�:
		TCHAR szTempFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
		Snprintf(szTempFinal, INTERFACE_STRING_LENGTH, szTemp0, i*2+2);

        Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s  %+d"), szTemp, option_idx);
        //�ɼǸ� ���̻�
        GlobalFunc::AddOptionSuffix(use_type, option_list_ptr, szInterfaceString);

		if (is_fullset)
		{
			RegisterTooltip(szInterfaceString,
				-1,
				iSmallFontHeight,
				FontID_ItemInfo,
				WzSetItemColor,
				TOOLTIP_BG_WZCOLOR,
				c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
		}
		else
		{
			RegisterTooltip(szInterfaceString,
				-1,
				iSmallFontHeight,
				FontID_ItemInfo,
				WzDisableColor,
				TOOLTIP_BG_WZCOLOR,
				c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
		}

	}
	AddEmptyLineForTooltip();
}



//------------------------------------------------------------------------------
/** Ż�� �ν̷���
*/
void uiToolTipMan::_addItemRiderWaxLevel(SCItemSlot* pItemSlot)
{
    //��þƮ������ �ν������̴�.
    BYTE byWaxLevel= pItemSlot->GetEnchant();

    if(byWaxLevel>=0 && byWaxLevel<LIMIT_SUBTYPE)
    {
        TCHAR szLevel[INTERFACE_STRING_LENGTH + 1] = {0,};
        g_InterfaceManager.GetInterfaceString(6467+byWaxLevel,szLevel);
        RegisterTooltip(szLevel, _tcslen(szLevel),
            iNormalLineHeight,
            FontID_ItemInfo,
            WzDescColor,
            TOOLTIP_BG_WZCOLOR,
            0);

        AddEmptyLineForTooltip();
    }
   
}
//------------------------------------------------------------------------------
/** ���̴��� ��Ʈ�ɼ� �����κ�(��Ʈ��������(���������) Ǯ�¸�������)
*/
void uiToolTipMan::_addItemRiderSetOptionForTooltip(const BASE_ITEMINFO* pItemInfo)
{
    SCSlotContainer * pContainer = ItemManager::Instance()->GetContainer( SI_RIDER);
    SETITEM_OPTION_INFO* pSetItemOpt = SetItemOptionInfoParser::Instance()->Find(pItemInfo->set_group_code);

    if(!pContainer || !pSetItemOpt)
    {
        return;
    }
    //////////////////////////////////////////////////////////////////////////
    // ���̴� ������üũ
    //////////////////////////////////////////////////////////////////////////
    int iEquipNum =0;
    for(int i=0;i<MAX_RIDER_SLOT;++i)
    {
        SCItemSlot & rSlot = (SCItemSlot&)pContainer->GetSlot(i);
        
        if(rSlot.IsContainValidItem())
        {
            const BASE_ITEMINFO	*pItemInfoTemp = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
            if(pItemInfoTemp)
            {

                if((pItemInfo->set_group_code == pItemInfoTemp->set_group_code) && (1 == pItemInfo->m_bySetOptionType) && (1 == pItemInfoTemp->m_bySetOptionType))

                {
                    iEquipNum++;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // ���̴� Ǯ��Ʈ �ɼ�
    //////////////////////////////////////////////////////////////////////////
    TCHAR szInterfaceString[INTERFACE_STRING_LENGTH + 1] = {0,};
    TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
    TCHAR szTemp1[INTERFACE_STRING_LENGTH + 1] = {0,};
    for(int i = 0; i < MAX_SETITEM_FULL_OPTION; ++i)
    {
        BYTE btOptKnd = pSetItemOpt->m_FullOption[i].m_byOptionKind;
        BASE_OptionList* pOptList = OptionListParser::Instance()->GetOptionList(btOptKnd);
        if(pOptList)
        {
            if(0 == pOptList->mdwNameCode)
                continue;
            g_InterfaceManager.GetInterfaceString(pOptList->mdwNameCode
                , szTemp
                , INTERFACE_STRING_LENGTH);

            BYTE byUseType = pSetItemOpt->m_FullOption[i].m_byUseType;

            int byValue    = pSetItemOpt->m_FullOption[i].m_nOption;

            if((0 == byUseType) && (0 == byValue))
                continue;

            Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %s %+d"), szTemp,szTemp1, byValue);
            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(byUseType, pOptList, szInterfaceString);

            if(pSetItemOpt->m_byActiveFullOptionNum < iEquipNum)
            {
                assert(!"SetItemOptionInfo Error");
            }

            //��Ʈ������ �� �����ִٸ� Ȱ��ȭ.
            if(pSetItemOpt->m_byActiveFullOptionNum == iEquipNum)
            {
                RegisterTooltip(szInterfaceString,
                    -1,
                    iSmallFontHeight,
                    FontID_ItemInfo,
                    WzFullSetItemColor,
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            }
            else
            {
                RegisterTooltip(szInterfaceString,
                    -1,
                    iSmallFontHeight,
                    FontID_ItemInfo,
                    WzDisableColor,
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            }
        }
    }
    AddEmptyLineForTooltip();
}

bool uiToolTipMan::_addItemCustomizingTooltip(SCItemSlot* slot_item, const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL ||
       (slot_item->IsContainValidItem() == FALSE))
    {
        return false;
    }

    //1. Shell�������̳� Etheria������ �������
    if (slot_item->IsShellItem() ||
        slot_item->IsEtheriaItem())
    {
        TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
        bool is_shell_item = slot_item->IsShellItem();
        int string_code;

        if (is_shell_item == true)
        {
            // 6116:��� �Ұ���
            g_InterfaceManager.GetInterfaceString(6116, message,INTERFACE_STRING_LENGTH);
        }
        else
        {
            _GetTradeInfoString(message, slot_item, item_info, true);
        }

        RegisterTooltipLineChange(message, _tcslen(message), 
                                  iLargeFontHeight,
                                  FontID_ItemInfo,
                                  slot_item->IsShellItem() ? (WzErrorColor):(WzDescColor), 
                                  TOOLTIP_BG_WZCOLOR, 
                                  0);

        if (slot_item->IsShellItem())
        {
            // ���Ŭ���� ��½� ��ĭ����
            if (_addItemEquipClassForTooltip(slot_item, item_info))
            {
                AddEmptyLineForTooltip();
            }

            // 6097:���׸��ư� ���������Դϴ�. ���׸��Ƹ� �������ּ���
            string_code = slot_item->IsPetItem() ? (6098):(6097); 
            g_InterfaceManager.GetInterfaceString(string_code, message,INTERFACE_STRING_LENGTH);

            RegisterTooltipLineChange(message, _tcslen(message), iNormalLineHeight, FontID_ItemInfo,
                                      WzDescColor,TOOLTIP_BG_WZCOLOR,0);

            AddEmptyLineForTooltip();

            return true;
        }

        return false;
    }

    //2. ���׸��� ���վ������� �ƴ϶��? return
    if (slot_item->GetImageItemInfo()->m_Code == item_info->m_Code)
    {
        return false;
    }
      

    //3. +  (���:1,2,3 ����Ʈ..) + ���� + ���׸���
    TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR type_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR etheria_string[INTERFACE_STRING_LENGTH] = {0,};
    
    bool is_grade_show = (item_info->IsWeapon() ||
                          item_info->IsArmor());
        
    g_InterfaceManager.GetInterfaceString(30421, etheria_string);

    // ���⿡�׸����϶� ���ǥ��
    if (is_grade_show)
    {
        int string_code = 0;
        TCHAR grade_name[INTERFACE_STRING_LENGTH] = {0,};
        WORD item_level = slot_item->GetItemInfo()->m_LV;
        string_code = GameFunc::GetItemGradeStringCode(grade_name, item_level, item_info->sub_type_detailed);
        /*
        //�Ϲݵ�޾�����
        if (item_level < 17)
        {
            TCHAR grade[128] ={0,};
            // 30422 %d ���
            g_InterfaceManager.GetInterfaceString(30422, grade, 127);
            _snprintf(grade_name, INTERFACE_STRING_LENGTH-1 , grade, item_level);
        }//��Ƽ����Ʈ
        else if (item_level == 17)
        {
            string_code = 30424;
        }//����Ʈ 1���
        else if (item_level == 21)
        {
            // 30428 : Ÿ���� ����Ʈ1���
            string_code = (item_info->m_wSubType == 3) ? (30428):(30423);
        }//����Ʈ 1.5���
        else if (item_level == 25)
        {
            string_code = 30425;
        }//����Ʈ 2���
        else if (item_level == 26)
        {
            string_code = 30426;
        }*/
            
        //�Ϲݵ�޾������̾ƴϴ�
        if (string_code != 0)
        {
            g_InterfaceManager.GetInterfaceString(string_code, grade_name);
        }
        
        // Ȳ��(m_NCode) + ��յ���(m_SCNCode)
        g_InterfaceManager.GetInterfaceString(item_info->m_SCNCode, type_string);
                
        _snprintf(item_name, INTERFACE_STRING_LENGTH -1 ,"+%s %s %s",
                  grade_name,       //+���      
                  type_string,      //����
                  etheria_string);  //���׸���
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(item_info->m_NCode, type_string);

        _snprintf(item_name, INTERFACE_STRING_LENGTH -1 ,"+%s %s",
                  type_string,      //�����۸�
                  etheria_string);  //���׸���

    }

    RegisterTooltip(item_name, _tcslen(item_name),
                    iLargeFontHeight,
                    FontID_ItemInfo,
                    WzDisableColor,			
                    TOOLTIP_BG_WZCOLOR,
                    0);
 
    return false;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiToolTipMan::_addItemSetOptionForTooltip(const BASE_ITEMINFO* pItemInfo)
{
	WORD wEqPos = pItemInfo->m_wEqPos;

	if(1 == pItemInfo->GetApplySetItemOptionType())
	{
		SETITEM_OPTION_INFO* pSetItemOpt = SetItemOptionInfoParser::Instance()->Find(pItemInfo->set_group_code);

		if (pSetItemOpt)
		{
//////////////////////////////////////////////////////////////////////////
//������ ������ �ɼ��� �ִ��� üũ
			BOOL bCheck = FALSE;
			for(int i = 0; i < MAX_SETITEM_OPTION_LEVEL; ++i)
			{
				SET_ITEM_OPTION* pSetOpt = SetItemOptionInfoParser::Instance()->GetSetItemOption(pItemInfo->set_group_code, 
					(POSTYPE)wEqPos, 
					(eSETITEM_OPTION_LEVEL)(i+1));

                //safecode
                if(pSetOpt)
                {
                    if((0 != pSetOpt->m_byUseType) && (0 != pSetOpt->m_nOption))
                    {
                        bCheck = TRUE;
                        break;
                    }

                }
			}

			for(int j = 0; j < MAX_SETITEM_FULL_OPTION; j++)
			{
				if(0 != pSetItemOpt->m_FullOption[j].m_byOptionKind)
				{
					bCheck = TRUE;
					break;
				}
			}

			if(FALSE == bCheck) //������ �����ٰ� ���ٸ� ����.
				return;
//////////////////////////////////////////////////////////////////////////

			TCHAR szInterfaceString[INTERFACE_STRING_LENGTH + 1] = {0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
			TCHAR szTemp1[INTERFACE_STRING_LENGTH + 1] = {0,};			

			AddEmptyLineForTooltip();

            if(pItemInfo->IsRiderPartsItem())
            {
                _addItemRiderSetOptionForTooltip(pItemInfo);
                return;
            }

			//////////////////////////////////////////////////////////////////////////
			// ��Ʈ ������ �������� �� ���� üũ
			//////////////////////////////////////////////////////////////////////////
            int apply_set_option_level = 0;         // ��Ʈ������ �ɼ� ����
            POSTYPE number_set_item = 0;            // ����� ��Ʈ������ ����
            SCItemSlot* pTempSetItemSlot[MAX_EQUIPMENT_SLOT_NUM];
            EquipmentContainer* itemslot_container = 
                static_cast<EquipmentContainer*>(ItemManager::Instance()->GetContainer(SI_EQUIPMENT));
            SCItemSlotContainer::SameSetItemSlotsResult setitem_result = 
                itemslot_container->GetSameSetItemSlots(pTempSetItemSlot, _countof(pTempSetItemSlot),
                                                        pItemInfo);
            
            // ���� ������ Ȱ��ȭ�Ǿ� �ִ��� üũ
            ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = itemslot_container->GetEquipmentManageInfo();
            for (int index = 0; index < setitem_result.number_of_same_set_items; ++index)
            {
                ns_formula::EquipSlotsManageInfo::Node* equip_node = equip_slot_manage_info->slots;
                SCItemSlot* equip_slot = pTempSetItemSlot[index];
                if (equip_node[equip_slot->GetPos()].activated == true)
                {
                    number_set_item++;
                }
            }
            apply_set_option_level = (number_set_item / 2);
            
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
            //////////////////////////////////////////////////////////////////////////
            // ��Ʈ ������
            //////////////////////////////////////////////////////////////////////////
            TCHAR str_set_item[INTERFACE_STRING_LENGTH + 1] = {0,};
            //11313 -----��Ʈ ������-----
            g_InterfaceManager.GetInterfaceString(11313, str_set_item, INTERFACE_STRING_LENGTH);
            RegisterTooltip(str_set_item,
                -1,
                iSmallFontHeight,
                FontID_ItemInfo,
                WzSetItemColor,
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            //���⼭ ������ ������ �߰� (������)
            static int test_int = 8;
            for( int i=0; i<test_int; ++i)
            {
                RegisterTooltip(str_set_item,
                    -1,
                    iSmallFontHeight,
                    FontID_ItemInfo,
                    WzSetItemColor,
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            }
            AddEmptyLineForTooltip();
#endif //_SCJ_SETITEM_TOOLTIP_REFORM

			//////////////////////////////////////////////////////////////////////////
			// ��Ʈ �ɼ�
			//////////////////////////////////////////////////////////////////////////
			TCHAR str_set_option[INTERFACE_STRING_LENGTH + 1] = {0,};            
			TCHAR str_some_count_option[INTERFACE_STRING_LENGTH + 1] = {0,};            
            //11313 -----��Ʈ�ɼ�-----
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM            
            g_InterfaceManager.GetInterfaceString(11344, str_set_option, INTERFACE_STRING_LENGTH);
#else
            g_InterfaceManager.GetInterfaceString(11313, str_set_option, INTERFACE_STRING_LENGTH);
#endif //_SCJ_SETITEM_TOOLTIP_REFORM	            
            //11314 %d���̻� �ɼ�
            g_InterfaceManager.GetInterfaceString(11314, str_some_count_option, INTERFACE_STRING_LENGTH);

            RegisterTooltip(str_set_option,
                -1,
                iSmallFontHeight,
                FontID_ItemInfo,
                WzSetItemColor,
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

			BOOL bEmptyLine = FALSE;
			for(int i = 0; i < MAX_SETITEM_OPTION_LEVEL; ++i)
			{
				SET_ITEM_OPTION* pSetOpt = SetItemOptionInfoParser::Instance()->GetSetItemOption(pItemInfo->set_group_code, 
											(POSTYPE)wEqPos, 
											(eSETITEM_OPTION_LEVEL)(i+1));

                //safecode
                if(!pSetOpt) continue;

				BASE_OptionList* pOptList = OptionListParser::Instance()->GetOptionList(pSetOpt->m_byOptionKind);
				if(pOptList)
				{
					//�ɼǸ�.
					g_InterfaceManager.GetInterfaceString( pOptList->mdwNameCode
						, szTemp
						, INTERFACE_STRING_LENGTH);

					if((0 == pSetOpt->m_byUseType) && (0 == pSetOpt->m_nOption))
                    {
						continue;
                    }

					//%d���̻� �ɼ�:
					TCHAR szTempFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
					Snprintf(szTempFinal, INTERFACE_STRING_LENGTH, str_some_count_option, i*2+2);

                    Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %s %s %+d"),
                        szTempFinal,
                        szTemp,
                        szTemp1,
                        pSetOpt->m_nOption);
                    //�ɼǸ� ���̻�
                    GlobalFunc::AddOptionSuffix(pSetOpt->m_byUseType, pOptList, szInterfaceString);

					if(apply_set_option_level > i)
					{
						RegisterTooltip(szInterfaceString,
							-1,
							iSmallFontHeight,
							FontID_ItemInfo,
							WzSetItemColor,
							TOOLTIP_BG_WZCOLOR,
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
                            c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
#else
                            0);
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
							
					}
					else
					{
						RegisterTooltip(szInterfaceString,
							-1,
							iSmallFontHeight,
							FontID_ItemInfo,
							WzDisableColor,
							TOOLTIP_BG_WZCOLOR,
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
                            c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
#else
                            0);
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
							
					}

					bEmptyLine = TRUE;
				}
			}

			if(bEmptyLine)
				AddEmptyLineForTooltip();

			//////////////////////////////////////////////////////////////////////////
			// Ǯ��Ʈ �ɼ�
			//////////////////////////////////////////////////////////////////////////
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
            TCHAR str_full_set_option[INTERFACE_STRING_LENGTH + 1] = {0,};
            //11343 Ǯ��Ʈ�ɼ�
            g_InterfaceManager.GetInterfaceString(11343, str_full_set_option, INTERFACE_STRING_LENGTH);
            if(pSetItemOpt->m_byActiveFullOptionNum <= number_set_item)
            {
                RegisterTooltip(str_full_set_option,
                    -1,
                    iSmallFontHeight,
                    FontID_ItemInfo,
                    WzSetItemColor,
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            }
            else
            {
                RegisterTooltip(szInterfaceString,
                    -1,
                    iSmallFontHeight,
                    FontID_ItemInfo,
                    WzDisableColor,
                    TOOLTIP_BG_WZCOLOR,
                    c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
            }
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
			for(int i = 0; i < MAX_SETITEM_FULL_OPTION; ++i)
			{
				BYTE btOptKnd = pSetItemOpt->m_FullOption[i].m_byOptionKind;
				BASE_OptionList* pOptList = OptionListParser::Instance()->GetOptionList(btOptKnd);
				if(pOptList)
				{
					if(0 == pOptList->mdwNameCode)
						continue;
					g_InterfaceManager.GetInterfaceString(pOptList->mdwNameCode
						, szTemp
						, INTERFACE_STRING_LENGTH);

					BYTE byUseType = pSetItemOpt->m_FullOption[i].m_byUseType;

					int byValue    = pSetItemOpt->m_FullOption[i].m_nOption;

					if((0 == byUseType) && (0 == byValue))
                    {
						continue;
                    }

                    Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %s %+d"), szTemp, szTemp1, byValue);
                    //�ɼǸ� ���̻�
                    GlobalFunc::AddOptionSuffix(byUseType, pOptList, szInterfaceString);

                    if(pSetItemOpt->m_byActiveFullOptionNum < number_set_item)
                    {
                        assert(!"SetItemOptionInfo Error");
                    }

                    //��Ʈ������ �� �����ִٸ� Ȱ��ȭ.
                    if(pSetItemOpt->m_byActiveFullOptionNum <= number_set_item)
					{
						RegisterTooltip(szInterfaceString,
							-1,
							iSmallFontHeight,
							FontID_ItemInfo,
							WzFullSetItemColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
					}
					else
					{
						RegisterTooltip(szInterfaceString,
							-1,
							iSmallFontHeight,
							FontID_ItemInfo,
							WzDisableColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
					}
				}
			}

			AddEmptyLineForTooltip();
		}

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DWORD uiToolTipMan::_getItemNameRGB(const char* szNumber)
{
	return WzColor_ChangeAlpha(strtol(szNumber,0,16),255);
}

//------------------------------------------------------------------------------
/** �� �⺻�ɼ����
*/
void uiToolTipMan::_addItemPetBaseOpForTooltip(const BASE_ITEMINFO* pItemInfo,BOOL bCashShop)
{
	TCHAR 	szBaseString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR 	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR 	szTemp1[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR 	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	WzColor BeginColor = WzColor_RGBA( 0, 255, 0, 255 );
	WzColor EndColor   = WzColor_RGBA( 0, 255, 0, 255 );

	//ĳ�����϶��� ������	
	if(bCashShop)	BeginColor	  = WzExerciseColor;							

	int nMaxBaseOp	= 0;
	for(int i=0;i<5;++i)
	{
		if( pItemInfo->m_wOptionIndex[i]!= 0) ++nMaxBaseOp;
	}

	 int iStringCode = ( nMaxBaseOp>1 ? (11426):(11423));
	
	 g_InterfaceManager.GetInterfaceString(iStringCode,szBaseString,INTERFACE_STRING_LENGTH);
	
	//0.�����ɼ�
	for(int i=0;i<nMaxBaseOp;++i)
	{
		//1.�ɼ�����
		DWORD	dwBaseOptionKind = pItemInfo->m_wOptionIndex[i];	

		//���������� break!
		if( dwBaseOptionKind == 0) break;

		if(nMaxBaseOp>1)
			Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s %d:"), szBaseString, i+1);	
		else
			Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s"), szBaseString, i+1);	

		RegisterTooltip( szString,-1,iNormalLineHeight,	FontID_ItemInfo,BeginColor,TOOLTIP_BG_WZCOLOR,0);

		//�������� ������ ��ġ�� �׸���.
		BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(dwBaseOptionKind);

		//1.�ɼ��� �׸���.
		if(pOption)
		{
            //2.�ɼǼ�ġ
			int iBaseValue = pItemInfo->m_iOptionValue[i];

            int option_name_code = pOption->mdwNameCode;
            if ((g_ITEM_OPTION[dwBaseOptionKind] == eATTR_AUTO_ITEM_PICK_UP) && 
                (iBaseValue >= 100))
            {
                option_name_code = 158445; // ������ �ڵ� �ݱ�(100 ���� ������ '���� �ڵ� �ݱ�')
            }
            g_InterfaceManager.GetInterfaceString(option_name_code, szTemp1, INTERFACE_STRING_LENGTH);

			//3.õ����üũ
			iBaseValue = ConvertOptValForSpecialCase( dwBaseOptionKind, pItemInfo->m_byOptionType[i],iBaseValue );	

			//����ó��:�ڵ��ݱ�� ��ġ�� �����
			if(	g_ITEM_OPTION[dwBaseOptionKind]==eATTR_AUTO_ITEM_PICK_UP)
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s"), szTemp1);
			}
			else
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s %+d"), szTemp1, iBaseValue);
			}
			
            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(pItemInfo->m_byOptionType[i], pOption, szString);
			this->RegisterTooltip(szString,	-1,iNameLineHeight,FontID_ItemInfo,EndColor,TOOLTIP_BG_WZCOLOR,	c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
		}
	}
}
//------------------------------------------------------------------------------
/** �� �߰��ɼ����
bGrade =  0:�߰��ɼǾ��� 1:�߰��ɼ�1�ߵ�(60%�̻�) 2:�߰��ɼ�2�ߵ�(100%)
*/                 
void uiToolTipMan::_addItemPetAddForTooltip(const BASE_ITEMINFO* pItemInfo,BOOL bCashShop,BYTE bGrade)
{
	const int	arStringIndex[2]  = {11433 ,11434 };//�߰��ɼ�1,�߰��ɼ�2

	const int	arStringGrade[2]  = {11435 ,11436 };//(ģ�е� 60%),(ģ�е� 100%)

	const int	arOptionIndex[2]  = {MAX_PET_OPTION_EXTENT_COUNT-2,MAX_PET_OPTION_EXTENT_COUNT-1};

	WzColor		BeginColor[2],EndColor[2];

	//Here is..��Ʈ�� ����
	if(bCashShop)
	{
		BeginColor[0]  = BeginColor[1] = WzExerciseColor;
		EndColor[0]	= EndColor[1]   = WzColor_RGBA( 0, 255, 0, 255 );
	}
	else
	{
		switch(bGrade)
		{
		case 1://�߰��ɼ�1�ߵ�
			{
				BeginColor[0] = EndColor[0] = WzColor_RGBA( 0, 255, 0, 255 );
				BeginColor[1] = EndColor[1] = WzSkillDisableColor;
			}
			break;
		case 2://�߰��ɼ�2�ߵ�
			{
				BeginColor[0] = EndColor[0] = WzSkillDisableColor;
				BeginColor[1] = EndColor[1] = WzColor_RGBA( 0, 255, 0, 255 );
			}
			break;
		default://�߰��ɼǹ�����
			{
				BeginColor[0] = EndColor[0] = WzSkillDisableColor;
				BeginColor[1] = EndColor[1] = WzSkillDisableColor;
			}
			break;
		}
	}

	//Here is..��Ʈ�׸���
	TCHAR 	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR 	szTemp1[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR 	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	sPet_Script_Info* pPetParserInfo = PetInfoParser::Instance()->GetPetInfo(pItemInfo->m_wSkillCode);

	if(!pPetParserInfo) return;

	for(int i=0;i<2;++i)
	{	
		//1.�߰��ɼ�1~2
		g_InterfaceManager.GetInterfaceString(arStringIndex[i],szString,INTERFACE_STRING_LENGTH);
		RegisterTooltip( szString,	-1,	iNormalLineHeight,FontID_ItemInfo,BeginColor[i],TOOLTIP_BG_WZCOLOR,0);

		for(int j=0;j<MAX_PET_OPTION_COUNT;++j)
		{
			//�ɼ�����,Ÿ��
			DWORD dwOptionKind = pPetParserInfo->m_PetOptionInfo[ arOptionIndex[i] ].m_wOptionKind[j];
			BYTE dwOptionType = pPetParserInfo->m_PetOptionInfo[ arOptionIndex[i] ].m_byValueType[j];  

			BASE_OptionList* pAddOption = OptionListParser::Instance()->GetOptionList(dwOptionKind);	

			//������ Skip
			if(!pAddOption || dwOptionKind==0) continue;

			//2.�ɼǹߵ�( ģ�е�60%,ģ�е� 100% )
			g_InterfaceManager.GetInterfaceString(arStringGrade[i],szTemp2,INTERFACE_STRING_LENGTH);

			//3.�ɼ�����
			g_InterfaceManager.GetInterfaceString(pAddOption->mdwNameCode,szTemp1,INTERFACE_STRING_LENGTH);

			//4.�ɼǼ�ġ
			int iAddOptionValue =  pPetParserInfo->m_PetOptionInfo[arOptionIndex[i]].m_iValue[j];

            int option_name_code = pAddOption->mdwNameCode;
            if ((g_ITEM_OPTION[dwOptionKind] == eATTR_AUTO_ITEM_PICK_UP) && 
                (iAddOptionValue >= 100))
            {
                option_name_code = 158445; // ������ �ڵ� �ݱ�(100 ���� ������ '���� �ڵ� �ݱ�')
            }
            g_InterfaceManager.GetInterfaceString(option_name_code, szTemp1, INTERFACE_STRING_LENGTH);

			//5.õ����üũ
			iAddOptionValue = ConvertOptValForSpecialCase( dwOptionKind, dwOptionType,iAddOptionValue );

			//����ó��:�ڵ��ݱ�� ��ġ�� �����
			if(	g_ITEM_OPTION[dwOptionKind]==eATTR_AUTO_ITEM_PICK_UP)
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s"), szTemp1);
			}
			else
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s %+d"), szTemp1 ,iAddOptionValue);
			}

            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(dwOptionType, pAddOption, szString);
			_tcscat(szString,szTemp2);

			RegisterTooltipLineChange(szString, _tcslen(szString),
				iNameLineHeight,
				FontID_ItemInfo,
				EndColor[i],			
				TOOLTIP_BG_WZCOLOR,
				0);	
		}
	}

}
#ifdef __CN_PET_OPTION_APPLY_FLAG
void uiToolTipMan::_addItemPetAddOption(const BASE_ITEMINFO *pItemInfo,BOOL bCashShop,int iIntimacy)
{
	
	sPet_Script_Info* pPetParserInfo = PetInfoParser::Instance()->GetPetInfo(pItemInfo->m_wSkillCode);

	if(!pPetParserInfo) return;

    // ���� �ɼǾ��������� Default����
    if (pPetParserInfo->m_eApplyOptionFlag==sPet_Script_Info::ePET_OPTION_FLAG_NONE)
    {
        if (iIntimacy==cMaxGauge)
        {
            //�߰��ɼ�2�ߵ�:100%�϶�
            _addItemPetAddForTooltip(pItemInfo,FALSE,2);
        }
        else if (iIntimacy>=60)
        {	
            //�߰��ɼ�1�ߵ�:60%�϶�
            _addItemPetAddForTooltip(pItemInfo,FALSE,1);
        }
        else
        {
            //�߰��ɼǾ���
            _addItemPetAddForTooltip(pItemInfo,FALSE,0);
        }

        return;
    }
   	
    TCHAR 	szString[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR 	szTemp1[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR 	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	//�߰��ɼ�(Cash Shop�������Ѹ���)
	if(bCashShop)
	{
		g_InterfaceManager.GetInterfaceString(11437,szString,INTERFACE_STRING_LENGTH);
		RegisterTooltip( szString,	-1,	iNormalLineHeight,FontID_ItemInfo,WzExerciseColor,TOOLTIP_BG_WZCOLOR,0);
	}

	//���� Ȱ��ȭ �������
	int iLevel = PetInfoParser::Instance()->GetOptionLevel(pItemInfo->m_wSkillCode,iIntimacy);

	//STEP1:�������� ����
	for(int lv=0;lv<MAX_PET_OPTION_EXTENT_COUNT;++lv)
	{	
		//Ȱ��ȭ ������
		WzColor TextColor =  (lv<=iLevel || bCashShop ? (WzColor_RGBA( 0, 255, 0, 255 )):(WzSkillDisableColor));

		for(int i=0;i<MAX_PET_OPTION_COUNT;++i)
		{
			//�ɼ�����,Ÿ��,��
			DWORD dwOptionKind	= pPetParserInfo->m_PetOptionInfo[lv].m_wOptionKind[i];
			BYTE dwOptionType	= pPetParserInfo->m_PetOptionInfo[lv].m_byValueType[i];  
			int iAddOptionValue =  pPetParserInfo->m_PetOptionInfo[lv].m_iValue[i];

			BASE_OptionList* pAddOption = OptionListParser::Instance()->GetOptionList(dwOptionKind);	

			//������ Skip
			if(!pAddOption || dwOptionKind==0) continue;

			//2.ģ�е�( x %)
			g_InterfaceManager.GetInterfaceString(1416,szTemp1,INTERFACE_STRING_LENGTH);
			Snprintf(szTemp2,INTERFACE_STRING_LENGTH-1,_T("(%s %d%%)"),szTemp1,pPetParserInfo->m_byIntimacyMin[lv]);

			//3.�ɼ�����
            int option_name_code = pAddOption->mdwNameCode;
            if ((g_ITEM_OPTION[dwOptionKind] == eATTR_AUTO_ITEM_PICK_UP) && 
                (iAddOptionValue >= 100))
            {
                option_name_code = 158445; // ������ �ڵ� �ݱ�(100 ���� ������ '���� �ڵ� �ݱ�')
            }
            g_InterfaceManager.GetInterfaceString(option_name_code, szTemp1, INTERFACE_STRING_LENGTH);
			
			//5.õ����üũ
			iAddOptionValue = ConvertOptValForSpecialCase( dwOptionKind, dwOptionType,iAddOptionValue );

			if(	g_ITEM_OPTION[dwOptionKind]==eATTR_AUTO_ITEM_PICK_UP)
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s"), szTemp1 );
			}
			else
			{
				Snprintf(szString,INTERFACE_STRING_LENGTH-1, _T("%s %+d"), szTemp1 ,iAddOptionValue);
			}
			

            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(dwOptionType, pAddOption, szString);

			_tcscat(szString,szTemp2);

			RegisterTooltipLineChange(szString, _tcslen(szString),iNameLineHeight,FontID_ItemInfo,TextColor,TOOLTIP_BG_WZCOLOR,0);	
		}
	}
}
#endif//__CN_PET_OPTION_APPLY_FLAG
//------------------------------------------------------------------------------
/** �κ��丮 ������ ����
*/
void uiToolTipMan::_addItemPetForToolTip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if(!g_pHero) return;
		
	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp1[INTERFACE_STRING_LENGTH] = {0,};
	
	//---------------------------------------------------------------------------------------------
	// ĳ�������� or �Ϲݾ�����
	//---------------------------------------------------------------------------------------------
	g_InterfaceManager.GetInterfaceString(pItemInfo->m_SCNCode,szString,INTERFACE_STRING_LENGTH);
	
	this->RegisterTooltip(szString,-1,iNormalLineHeight,FontID_ItemInfo,WzNormalColor,TOOLTIP_BG_WZCOLOR,0);
	
	//---------------------------------------------------------------------------------------------
	//		�ŷ�����
	//---------------------------------------------------------------------------------------------
	_addItemTradeInfoForTooltip(pItemSlot, pItemInfo);


	//---------------------------------------------------------------------------------------------
	//		�⺻����
	//---------------------------------------------------------------------------------------------
	SCPetInfo* pPetBaseInfo = pItemSlot->GetPetInfo();			
		
	
	const int iPetBaseCode[2] =
	{
		11421,	//11421:������
		11422	//11422:ģ�е�
	};
	
	int iPetBaseValue[2] = 
	{
		pPetBaseInfo->GetFullness(),//������
		pPetBaseInfo->GetIntimacy() //ģ�е�
	};

	for(int i=0;i<2;++i)
	{
		g_InterfaceManager.GetInterfaceString(iPetBaseCode[i],szTemp1,INTERFACE_STRING_LENGTH);

		Snprintf(szString,INTERFACE_STRING_LENGTH-1,_T(szTemp1),iPetBaseValue[i],cMaxGauge);

		this->RegisterTooltip(szString,	-1,iNormalLineHeight,FontID_ItemInfo,WzNormalColor,	TOOLTIP_BG_WZCOLOR,	0);
	}

	//�����ɼ�:11423
	_addItemPetBaseOpForTooltip(pItemInfo, FALSE);
	
	//---------------------------------------------------------------------------------------------
	//		�߰��ɼ�
	//---------------------------------------------------------------------------------------------
	g_InterfaceManager.GetInterfaceString(11424,szString,INTERFACE_STRING_LENGTH);
	
	this->RegisterTooltip(szString,-1,iLargeFontHeight,FontID_ItemInfo,WzNormalColor,TOOLTIP_BG_WZCOLOR,c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

#ifdef __CN_PET_OPTION_APPLY_FLAG
	_addItemPetAddOption(pItemInfo,FALSE ,iPetBaseValue[1]);
#else
	if(iPetBaseValue[1]==cMaxGauge)
	{
		//�߰��ɼ�2�ߵ�:100%�϶�
		_addItemPetAddForTooltip(pItemInfo,FALSE,2);
	}
	else if(iPetBaseValue[1]>=60)
	{	
		//�߰��ɼ�1�ߵ�:60%�϶�
		_addItemPetAddForTooltip(pItemInfo,FALSE,1);
	}
	else
	{
		//�߰��ɼǾ���
		_addItemPetAddForTooltip(pItemInfo,FALSE,0);
	}
#endif//__CN_PET_OPTION_APPLY_FLAG
	//---------------------------------------------------------------------------------------------
	//		���������
	//---------------------------------------------------------------------------------------------
	g_InterfaceManager.GetInterfaceString(11427,szString,INTERFACE_STRING_LENGTH);
	this->RegisterTooltip(szString,-1,iLargeFontHeight,FontID_ItemName,WzNormalColor,TOOLTIP_BG_WZCOLOR,c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
	
	
	g_InterfaceManager.GetInterfaceString(11428,szString,INTERFACE_STRING_LENGTH);
	RegisterTooltipLineChange(szString,_tcslen(szString),iNormalLineHeight,FontID_ItemInfo,WzNormalColor,TOOLTIP_BG_WZCOLOR,0);

    AddEmptyLineForTooltip();

    GameFunc::GetItemDescription(szString, pItemSlot, pItemInfo);
    RegisterTooltipLineChange(szString, _tcslen(szString),
        iNormalLineHeight,
        FontID_ItemInfo,
        WzEffectColor,			
        TOOLTIP_BG_WZCOLOR,
        0);

    AddEmptyLineForTooltip();

	//---------------------------------------------------------------------------------------------
	//		�������
	//---------------------------------------------------------------------------------------------
	if (_isEnableChargeItemDesc(pItemSlot, pItemInfo))
		_addChargeItemDescForTooltip(pItemSlot, pItemInfo);

    //---------------------------------------------------------------------------------------------
    //		C/P���� ������
    //---------------------------------------------------------------------------------------------
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    _addItemCashTypeTooltip(pItemSlot, pItemInfo);
#endif//_NA000424_GLOBAL_CASH_ITEM_RENEWAL_

	AddEmptyLineForTooltip();
}
//------------------------------------------------------------------------------
/** ��������� ����
*/
void uiToolTipMan::_addItemPetFeedForToolTip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	//��� ����
	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetInterfaceString(pItemInfo->m_TDNCode,szString,INTERFACE_STRING_LENGTH);

	this->RegisterTooltipLineChange(szString, _tcslen(szString),
		iLargeFontHeight,
		FontID_ItemInfo,
		WzNormalColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}

void uiToolTipMan::RegisterPersonalWaypoint(int nEnableCode)
{
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	InitTooltip();

	// 11063 - ���� ���� : %d\nŬ���� ���� ���� ��������Ʈ�� �̿��� �� �ֽ��ϴ�.
	// 11061 - �̿� ����	,	11062 - �̿� �Ұ���
	g_InterfaceManager.GetInterfaceString(11063, szTemp, INTERFACE_STRING_LENGTH);	
	g_InterfaceManager.GetInterfaceString(nEnableCode, szTemp2, INTERFACE_STRING_LENGTH);

	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();

	GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg, szString, StrToWzID("mn12"), _tcslen(szString), 200);

	for (size_t i = 0; i < vstrMsg.size(); ++i)
	{
		TCHAR* pStr = const_cast<TCHAR*>(vstrMsg[i].c_str());

		RegisterTooltip(pStr);
	}
}

void uiToolTipMan::_addChargeItemCoordinateExForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if ( pItemSlot->IsProgressTime() )
	{
		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szKindAMount[INTERFACE_STRING_LENGTH] = {0,};	
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};	

		AddEmptyLineForTooltip();	

		SYSTEMTIME systime;
		util::YYYYMMDDHHMMSSToSYSTEMTIME( pItemSlot->GetDateTime(), systime );

		// 5874   %4d �� %2d�� %2d�� %2d�� %2d�� ����
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 5874, systime);

		RegisterTooltipLineChange(
			szInterfaceString, 
			_tcslen(szInterfaceString),
			iNormalLineHeight,
			FontID_ItemInfo,
			WzChargeItemDateColor,
			TOOLTIP_BG_WZCOLOR,
			0);

		if( pItemSlot && !pItemSlot->IsFateItem()) 
			AddEmptyLineForTooltip();
	}
	else
	{
        if(pItemSlot->GetItemInfo()->IsRiderPartsItem() && pItemSlot->IsExpiredWaitTime())
        {
            return;
        }

        //�������������ǰ�� Ŭ���� �ߵ�
        if (pItemSlot->CanEquip() == false)
		{
			//���콺 ��Ŭ���� Ÿ�̸Ӱ� �����մϴ�.
			TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0, };
			GetUIMan()->GetInterfaceString( 6157, szInterfaceString, INTERFACE_STRING_LENGTH );

			RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
				iNormalLineHeight,
				FontID_ItemInfo,
				WzChargeItemDateColor,
				TOOLTIP_BG_WZCOLOR,
				0);

			AddEmptyLineForTooltip();

            DWORD remain_hour = (DWORD)(pItemSlot->GetDateTime()/(60*60));	
            DWORD remain_min = (DWORD)((pItemSlot->GetDateTime()/60) % 60);

            if (remain_hour > 0)
            {
                // 5872	�����ð�: %d�� %2d��
                g_InterfaceManager.GetInterfaceStringFormat(
                    szInterfaceString, 
                    INTERFACE_STRING_LENGTH, 
                    5872, 
                    remain_hour, 
                    remain_min);
            }
            else
            {
                // 5873	�����ð� : %2d��
                g_InterfaceManager.GetInterfaceStringFormat(
                    szInterfaceString, 
                    INTERFACE_STRING_LENGTH, 
                    5873, 
                    remain_hour, 
                    remain_min);
            }
            RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
                iNormalLineHeight,
                FontID_ItemInfo,
                WzChargeItemDateColor,
                TOOLTIP_BG_WZCOLOR,
                0);

            AddEmptyLineForTooltip();
		}
	}
}
void uiToolTipMan::_addWaypointItemTooltip()
{
	int nSlotCount = GlobalFunc::GetUsedWayPointSlotCount();
	
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	//InitTooltip();

	// 11060	���� ������ ��ǥ����: %d
	g_InterfaceManager.GetInterfaceString( 11060, szTemp, INTERFACE_STRING_LENGTH );	

	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, MAX_CHAR_WAYPOINT_NUM - nSlotCount);

	RegisterTooltip( szString );
}



//------------------------------------------------------------------------------
/** �ǵ��󽽷� ������ ���ֱ̹׸��� �ε������̶� ���ָ̹� �׸���.
*/
void uiToolTipMan::RegisterPandoraItemName(SCItemSlot * pItemSlot)
{
	const BASE_ITEMINFO	*pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pItemSlot->GetCode());
	
	if(pItemInfo)
	{
		TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetItemName(pItemInfo->m_Code, szString, INTERFACE_STRING_LENGTH);
		
		this->RegisterTooltip(szString,
			-1,
			iLineHeight,
			FontID_ItemInfo,
			WzNormalColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}
//------------------------------------------------------------------------------
/** �ζǾ����� �����׸���(�⺻���� + �����Ⱦ������� ������Ϻ����ش�)
*/
void uiToolTipMan::_addItemLottoItemInfoTooltip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	//---------------------------------------------------------------------------------------------
	// �ζǾ������� �������ִ� ������ ����Ʈ(�����ÿ���Ȯ�ΰ���)
	//---------------------------------------------------------------------------------------------
	if(pItemSlot->GetLottoInfo()->IsIdentity())
	{
		AddEmptyLineForTooltip();

		TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};

		SCLottoInfo* pLottoInfo= pItemSlot->GetLottoInfo();

		if(!pLottoInfo) return;

		//�ζ� �׷�����
		const LotteryIndexGroup*  pLotteryGroup =  LotteryInfoParser::Instance()->GetLotteryIndexGroup(pLottoInfo->GetLottoIndex());
		
		if(!pLotteryGroup) return;

		for(int i=0;i<MAX_PANDORA_SLOT_NUM;++i)
		{
			//1.�����������
			MiniSlot SlotInfo = pLottoInfo->GetMiniSlot(i);

			if(!SlotInfo.ExistItem) continue;	

			//2.�׷��ε��������� �����������ڵ���
			const MiniSlotInfo* Minislot =  pLotteryGroup->GetMiniSlotInfo(SlotInfo.InfoPos);
            if (Minislot == NULL)
            {
                continue;
            }
		
			if(Minislot->Random==FALSE)
			{
                g_InterfaceManager.GetItemName(Minislot->Index, szString, INTERFACE_STRING_LENGTH);
			}
            else
            {
                //3. ? �����ϰ��
                g_InterfaceManager.GetInterfaceString(11412, szString, INTERFACE_STRING_LENGTH);
            }

			this->RegisterTooltip(szString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				WzNormalColor,
				TOOLTIP_BG_WZCOLOR,
				0);
		}

	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
void uiToolTipMan::_addItemNameForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, BYTE byEnchant /*= 0*/, BOOL bRenderIcon /*= FALSE*/)
{
	if(!g_pHero)
		return;

	const	TCHAR	*szSpace = _T(" ");

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0, };

	TCHAR szLimitedItemStr[INTERFACE_STRING_LENGTH] = {0, };

    int font_height = iLargeFontHeight;

    if (pItemSlot &&
        pItemInfo &&
        pItemSlot->IsContainValidItem())
    {
        if ((pItemSlot->IsEtheriaItem() == false) &&
            (pItemSlot->GetImageItemInfo()->m_Code != pItemInfo->m_Code))
        {
            font_height = iNameLineHeight;
        }
    }
	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};
	
	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};

	DWORD dwTooltipExtraStyle = 0;

	WzColor textColor = WzNormalColor;

    // ���� ���� ���� ������
    int socket_number = -1;
    if (pItemSlot)
    {
        dwTooltipExtraStyle =c_dwTOOLTIP_EXTRA_INFO_DT_LEFT;
        switch(pItemSlot->GetSocketIdentifyType())
        {
        case eSOCKET_UNIDENTIFIED_ITEM://��Ȯ��
        case eSOCKET_IDENTIFIED_ITEM://���ϰ���Ȯ�ε�
            {
                socket_number = pItemSlot->GetSocketNum();
                
            /* RegisterTooltipImage(kTooltipTexture_Socket, kTexture_Rectangle_Socket,
                    textures_rectangles_[kTexture_Rectangle_Socket].right,
                    textures_rectangles_[kTexture_Rectangle_Socket].bottom,
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);*/
                dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE;
            }
            break;
        case eSOCKET_IDENTIFY_NOT_PROCESS: // ���� �ƴ�
        case eSOCKET_FAIL_IDENTIFY_PROCESS:// ���� 0���� ������
        default:
            socket_number = -1;
            break;
        }

        if(pItemSlot->IsFateItem() && pItemSlot->GetFateIdentify() == false)
        {
            socket_number = -1;
        }
    }

	// ����, ���� ��ü �̸�( ���� + ���� )�� ��� -> (�ձ�) + (��հ�)
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() )
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
        // ���յ� ������ �̸��� ����.
        GameFunc::GetItemName(pItemSlot, pItemInfo, szTemp, INTERFACE_STRING_LENGTH);

		if(pItemSlot && pItemSlot->IsDivine())
		{
			textColor = WzDivineNameColor;	// ���� ����
		}


		if(pItemSlot)
		{
			const BASE_ITEMINFO * pItemInfo = pItemSlot->GetItemInfo();
			if ( pItemInfo )
			{
				switch ( (eITEMSUBTYPE)pItemInfo->m_wSubType )
				{
                case eITEMSUBTYPE_ELITE_SSQ:
				case eITEMSUBTYPE_ELITE:
					textColor = WzEliteNameColor;
					break;
				case eITEMSUBTYPE_UNIQUE:
					textColor = WzUniqueNameColor;
					break;

				default:
					break;
				}
			}
		}

		if(pItemSlot && pItemSlot->IsDestroyItem())	// �μ��� �������̸�
		{
			// 47969: �ν���
			g_InterfaceManager.GetInterfaceString(47969,szTemp3,INTERFACE_STRING_LENGTH);
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1,_T("%s %s"),szTemp3, szTemp);
		}
		else
		{
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1,_T("%s"),szTemp);
		}

        if(_isEnableItemEnchant(pItemSlot, pItemInfo))
		{
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), pItemSlot->GetEnchant());
			_tcscat(szInterfaceString,szTemp);
		}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
        else if(byEnchant &&
            (pItemInfo->IsArmor() || pItemInfo->IsWeapon() ||
            (pItemSlot && pItemSlot->IsWingItem())))
#else
		else if(byEnchant && (pItemInfo->IsArmor() || pItemInfo->IsWeapon()))
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
		{
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), byEnchant);
			_tcscat(szInterfaceString,szTemp);
		}
	}
	else
	{

		if(pItemSlot)
		{
			const BASE_ITEMINFO * pItemInfo = pItemSlot->GetItemInfo();
			if ( pItemInfo )
			{
				switch ( (eITEMSUBTYPE)pItemInfo->m_wSubType )
				{
                case eITEMSUBTYPE_ELITE_SSQ:
				case eITEMSUBTYPE_ELITE:
					textColor = WzEliteNameColor;
					break;
				case eITEMSUBTYPE_UNIQUE:
					textColor = WzUniqueNameColor;
					break;

				default:
					break;
				}
			}
		}

        //	������ �̸��� ��� 
        GameFunc::GetRealItemName(pItemSlot, pItemInfo, szTemp, INTERFACE_STRING_LENGTH);
		Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1,_T("%s"),szTemp);
        GameFunc::GetCustomizeInfo(pItemSlot, szInterfaceString);
	}


	if (pItemSlot &&
        pItemSlot->IsContainValidItem() &&
        pItemSlot->IsExpiredWaitTime())
	{
		textColor = WzWaitItemColor;	// ���� ����
	}

	if (pItemSlot &&
        pItemSlot->IsContainValidItem() &&
        pItemSlot->IsLimited())
	{
		//	11283	����Ƽ��
		ZeroMemory(szTemp, INTERFACE_STRING_LENGTH);
		g_InterfaceManager.GetInterfaceString(11283, szTemp, INTERFACE_STRING_LENGTH);
		Snprintf(szLimitedItemStr, INTERFACE_STRING_LENGTH-1, _T("%s "), szTemp);
		
		this->RegisterTooltip(szLimitedItemStr,
			-1,
			font_height,
			FontID_ItemName,
			WzLimitedItemColor,
			TOOLTIP_BG_WZCOLOR,
            dwTooltipExtraStyle
            );
		dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE;
	}


	
	if( pItemInfo && pItemInfo->m_wSubType == eITEMSUBTYPE_FATE)
	{
		textColor = c_LuckyItemColor;
	}
		
		//����1:�����ۻ������̵�������� (����ũ,����Ʈ,��Ű,���)�ǻ��� ���õȴ�
		if(pItemInfo->m_bUse)
        {
			textColor = _getItemNameRGB(pItemInfo->m_szColor);
        }
		
		//����2:����Ƽ�忡 ���������ɰ�� ���Ͽ켱�����̴�
		if(pItemSlot && pItemSlot->IsLimited())
		{			
			sLIMITED_ITEM_INFO *pLimitItemInfo  = LimitedItemInfoParser::Instance()->GetLimitedItemInfo(pItemSlot->GetLimitedIndex());
			
			if(pLimitItemInfo)
			{
				if(pLimitItemInfo->m_bUse)
					textColor = _getItemNameRGB(pLimitItemInfo->m_szColor);
			}
			else
			{
				assert(pLimitItemInfo);
			}
		}

		DWORD eSpecialTooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

		/*if(pItemInfo && pItemInfo->IsRandomItem())
		{
			eSpecialTooltip &= (~eSPECIAL_TOOLTIP_NOMAL_ITEM);
			eSpecialTooltip |= eSPECIAL_TOOLTIP_RANDOM_ITEM;			
		}*/

		DWORD dwIconCode = 0;
		if(TRUE == bRenderIcon)
		{
			eSpecialTooltip |= eSPECIAL_TOOLTIP_ITEM_ON_TT_TEXT;
			dwIconCode = pItemInfo->m_wVICode;
			dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DT_CENTER;
		}

#ifdef _DEV_VER
//#ifdef _SCJ_SHOW_ITEM_ID
        if (CGeneralGameParam::Instance()->IsShowItemId())
        {
            char buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy( buffer_string, szInterfaceString, INTERFACE_STRING_LENGTH-1 ); 
            Snprintf( szInterfaceString, INTERFACE_STRING_LENGTH-1, _T("%s <ITEMID:%ld>"),
                buffer_string, pItemInfo->m_Code );
        }
//#endif //_SCJ_SHOW_ITEM_ID
#endif //#ifdef _DEV_VER

		this->RegisterTooltip(szInterfaceString,
			-1,
			font_height,
			FontID_ItemName,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			dwTooltipExtraStyle,
			eSpecialTooltip,
			dwIconCode );

	m_PossessionItemColor = textColor;
	
	if( pItemInfo->IsPotion() )
	{
		//	������ ��� ��Ÿ�� ������ ���ķ� ǥ��

		//	���� ��Ÿ�� : %d ��
		const int		iCoolTime = 11173;

		util::Timer *pTimer = xGetCooltimeManager()->GetItemCooltime(pItemInfo->m_wCoolTimeType);
		if (pTimer && !pTimer->IsExpired(FALSE))	// ��Ÿ���� ��������
		{
			dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;

			float fRemainRate = 1.0f - pTimer->GetProgressRatio();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            float fRemainTime = 0.0f;
            // ���ü� ȸ�� ���� ��Ÿ�� ����
            if (g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_RECOVER_POTION_COOLTIME_RATIO) != 0)
            {
                fRemainTime = (pItemInfo->m_dCoolTime + (pTimer->GetIntervalTime() - pTimer->GetBaseIntervalTime())) * fRemainRate;
            }
            else
            {
                fRemainTime = pItemInfo->m_dCoolTime * fRemainRate;
            }
#else
			float fRemainTime = pItemInfo->m_dCoolTime * fRemainRate;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

			DWORD dwCoolTime = (DWORD)(fRemainTime / 1000);
			g_InterfaceManager.GetInterfaceString( iCoolTime, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp, dwCoolTime );

			textColor = WzNormalColor;	// ���� ����
			if(pItemSlot && pItemSlot->IsExpiredWaitTime())
			{
				textColor = WzWaitItemColor;	// ���� ����
			}
			this->RegisterTooltip( szInterfaceString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				textColor,
				TOOLTIP_BG_WZCOLOR,
				dwTooltipExtraStyle);
			
		}
	}

    if (socket_number > -1)
    {
        int texture_index = kTexture_Rectangle_Socket0 + socket_number;
        RegisterTooltipImage(kTooltipTexture_Socket, texture_index,
            m_TooltipTextureRectAngles[texture_index].right,
            m_TooltipTextureRectAngles[texture_index].bottom,
            c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT);
    }
}

//------------------------------------------------------------------------------
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
void uiToolTipMan::_addItemAwakeningForTooltip( SCItemSlot* item_slot, const BASE_ITEMINFO* item_info )
{
	if ( item_slot == NULL )
	{
		return;
	}

	if ( item_info == NULL )
	{
		return;
	}

	const AwakeningInfo* awakening_info = AwakeningInfoParser::Instance()->FindData(item_slot->GetCode());

	if ( awakening_info == NULL )
	{
		return;
	}

	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR awakening_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR max_awakening_string[INTERFACE_STRING_LENGTH] = {0, };
	//_itoa(item_slot->GetAwakening(), wakeup_string, 10);
	_itoa(item_slot->GetAwakening(), awakening_string, 10);
	_itoa(MAX_AWAKENING_GRADE, max_awakening_string, 10);

	// 3988 : �������� (%s/%s)
	g_InterfaceManager.GetInterfaceString(3988, format_string);
	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, awakening_string, max_awakening_string);

	TCHAR white_star_string[8] = {0, };
	TCHAR black_star_string[8] = {0, };

	// 3977 : ��
	g_InterfaceManager.GetInterfaceString(3977, white_star_string);
	// 3976 : ��
	g_InterfaceManager.GetInterfaceString(3976, black_star_string);

	strcat( result_string, " ");
	for ( int i = 0; i < MAX_AWAKENING_GRADE; ++i )
	{
		if ( i < item_slot->GetAwakening() )
		{
			strcat( result_string, black_star_string);
		}
		else
		{
			strcat( result_string, white_star_string );
		}
	}

	WzColor text_color = WzAwakeningColor;
	if (item_slot)
	{
		this->RegisterTooltip(result_string,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			text_color,
			TOOLTIP_BG_WZCOLOR,
			0);
	}

	if ( item_slot->GetAwakening() > 0 )
	{
		text_color = WzAwakeningEffectColor;
		if ( item_slot->GetAwakening() <= MAX_AWAKENING_GRADE )
		{
			std::map<BYTE, DWORD>::const_iterator iter;
			iter = awakening_info->awakening_level_info_map.find(item_slot->GetAwakening());
			if ( iter == awakening_info->awakening_level_info_map.end() )
			{
				return;
			}
			DWORD awakening_effect = (iter->second);
			TCHAR awakening_effect_string[INTERFACE_STRING_LENGTH] = {0, };
			_itoa(awakening_effect, awakening_effect_string, 10);
			TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
			if ( item_info->IsWeapon() )
			{
				// 3964 : ���� ���ݷ� ���� %s
				g_InterfaceManager.GetInterfaceString(3964, format_string);
				Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, awakening_effect_string);
			}
			else if ( item_info->IsArmor() )
			{
				// 3965 : ���� ���� %s
				g_InterfaceManager.GetInterfaceString(3965, format_string);
				Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, awakening_effect_string);
			}
		}

		this->RegisterTooltip(result_string,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			text_color,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemTypeForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetInterfaceString(pItemInfo->m_SCNCode,szInterfaceString,INTERFACE_STRING_LENGTH);
	

	WzColor	textColor = WzNormalColor;
	if(pItemSlot && pItemSlot->IsExpiredWaitTime())
	{
		textColor = WzWaitItemColor;	// ���� ����
	}
	this->RegisterTooltip(szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		textColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemLimitLevelForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if(!g_pHero)
		return;

	WORD LimitLV = pItemInfo->m_LimitEqLevel;
	if(LimitLV > 0)
	{
		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};

		WzColor wzTextColor = WzNormalColor;
		//	���� ����
		g_InterfaceManager.GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );		
		Snprintf(szText,INTERFACE_STRING_LENGTH-1, _T("%s %ld"), szInterfaceString, LimitLV);


		if(pItemSlot && pItemSlot->IsLimited())
		{	
			int nRatio  = pItemSlot->GetLimitValueRatio();

			LimitLV += (WORD)((float)LimitLV * nRatio / 100.0f+MODFLOAT(nRatio));
		}


		if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
		{
			wzTextColor = WzWaitItemColor;
		}
		else if ( g_pHero->GetLevel() < LimitLV ) 
		{	
			wzTextColor = WzErrorColor;
		}
		else
		{
			wzTextColor = WzNormalColor;						
		}

		this->RegisterTooltip(	szText,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			wzTextColor,
			TOOLTIP_BG_WZCOLOR,
			0);


		if(pItemSlot && pItemSlot->IsLimited())
		{
			ZeroMemory(szText, INTERFACE_STRING_LENGTH);
			Snprintf(szText,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), LimitLV);
			
			if(wzTextColor != WzErrorColor)
				wzTextColor = WzCurSkillLevelColor;
			this->RegisterTooltip(	szText,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				wzTextColor,
				TOOLTIP_BG_WZCOLOR,
				c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
		}

		AddEmptyLineForTooltip();
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemNextGradeLimitLevelForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	if(!g_pHero)
		return;

	WORD LimitLV = pItemInfo->m_LimitEqLevel;
	if(LimitLV > 0)
	{
		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};

		WzColor wzTextColor = WzNormalColor;


		if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			wzTextColor = WzWaitItemColor;

		//���� ��� ���� ��ġ
		g_InterfaceManager.GetInterfaceString( eST_NEXT_GRADE_LIMIT_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
		this->RegisterTooltip(	szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			wzTextColor,
			TOOLTIP_BG_WZCOLOR,
			0);

		//	���� ����
		g_InterfaceManager.GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
		Snprintf(szText,INTERFACE_STRING_LENGTH-1, _T("%s %ld"), szInterfaceString, LimitLV);


		if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			wzTextColor = WzWaitItemColor;
		else if ( g_pHero->GetLevel() < LimitLV ) 
		{	
			wzTextColor = WzErrorColor;
		}
		else
		{
			wzTextColor = WzNormalColor;						
		}

		this->RegisterTooltip(	szText,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			wzTextColor,
			TOOLTIP_BG_WZCOLOR,
			0);

		AddEmptyLineForTooltip();
	}
}

//------------------------------------------------------------------------------
bool uiToolTipMan::_addItemEquipClassForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTempFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

	// ���� ����
    bool all_class = false;
	if (pItemInfo->m_wEqClass1 && 
		pItemInfo->m_wEqClass2 && 
		pItemInfo->m_wEqClass3 && 
		pItemInfo->m_wEqClass4 && 
		pItemInfo->m_wEqClass5 && 
        pItemInfo->m_wEqClass6 && 
        pItemInfo->m_wEqClass7 && 
        pItemInfo->m_wEqClass8)
	{
		// ��� ����̸� ������
		all_class = true;
	}

    bool all_gender = (pItemInfo->gender_type == GenderType::kAll) ? true : false;

    if ((all_class == true) && 
        (all_gender == true))
    {
        return false;
    }
	
	int		iClassNameCode[eCHAR_TYPE_MAX] = {
												11382, //(B)
												11385, //(D)
												11388, //(S)
												11383, //(V)
												11384, //(E)
                                                2568, //(M) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                                                2569, //(H) //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                                                10008, //(W) //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
												};

	WORD		wCharEqClass[eCHAR_TYPE_MAX] = {
											pItemInfo->m_wEqClass1,
											pItemInfo->m_wEqClass2,
											pItemInfo->m_wEqClass3,
											pItemInfo->m_wEqClass4,
											pItemInfo->m_wEqClass5,
                                            pItemInfo->m_wEqClass6, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                                            pItemInfo->m_wEqClass7, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                                            pItemInfo->m_wEqClass8, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
											};

    WzColor wcTextColor = WzErrorColor;
    BOOL bCanEquip = ItemManager::Instance()->CanEquipClass(g_pHero, pItemInfo);
    
    if (all_class != true)
    {
        for( int i = 0 ; i < eCHAR_TYPE_MAX ; ++i )
        {
            //	Ŭ���� ���� �ִ� ĳ���� �̸��� ����
            if( wCharEqClass[i] )
            {
                //11381	��� ���� Ŭ���� :
                g_InterfaceManager.GetInterfaceString(11381, szTemp);

                g_InterfaceManager.GetInterfaceString( iClassNameCode[i], szTemp2);
                Snprintf(szTempFinal,INTERFACE_STRING_LENGTH-1,_T("%s "), szTemp2);
                _tcscat(szInterfaceString, szTempFinal);
            }
        }
    }

    const int iGenderStringCode[GenderType::kFemale] = {
                11389,  //(M)
                11390}; //(F)};
    if ((g_pHero != NULL) &&
        (all_gender != true))
    {
        bool can_equip = pItemInfo->IsCanUseGender(static_cast<BYTE>(g_pHero->GetGender()));
        bCanEquip &= (can_equip == true) ? TRUE : FALSE; // Equip�� ���� �ϴ��� gender���� �¾ƾ� �ᱹ TRUE�� �ȴ�.

        g_InterfaceManager.GetInterfaceString(iGenderStringCode[pItemInfo->gender_type - 1], szTemp);
        _tcscat(szInterfaceString, szTemp);
    }

    if(pItemSlot && pItemSlot->IsExpiredWaitTime())
    {
        wcTextColor = WzWaitItemColor;	// ���� ����
    }
    else if(pItemSlot && pItemSlot->IsLimited() && ( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0))
    {
        wcTextColor = WzErrorColor;
    }
    else if(bCanEquip)
    {
        wcTextColor = WzNormalColor;
    }

	if( 0 != strlen(szInterfaceString))
	{
		TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

		_addItemLimitInfoForTooltipMakeStr(pItemSlot, 11381, szInterfaceString, wcTextColor);
	}
	return true;
}
//------------------------------------------------------------------------------
/** DESC:���� �ӵ�
*/
void uiToolTipMan::_addItemPhyAttackSpeedForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if( pItemInfo->m_byMaterialType != eITEM_MATERIAL_TYPE_CAN_EQUIP ) return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	WzColor	wcTextColor = WzNormalColor;
	if(pItemSlot && pItemSlot->IsExpiredWaitTime())
		wcTextColor = WzWaitItemColor;	// ���� ����

	_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_PHYATTACK_SPEED_STRING_INDEX, pItemInfo->m_wPhyAttSpeed, wcTextColor);
}
//------------------------------------------------------------------------------
/** DESC:���� ���ݷ�
*/
void uiToolTipMan::_addItemPhyAttackPowerForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	DWORD dwPhyMinAttackPower = 0;
	DWORD dwPhyMaxAttackPower = 0;

	DWORD dwLimitedPhyMinAttackPower = 0;
	DWORD dwLimitedPhyMaxAttackPower = 0;

	
	DWORD dwPhyMinDamage = 0;
	DWORD dwPhyMaxDamage = 0;

	BYTE byEnchant = 0;
	if(pItemSlot)
		byEnchant = pItemSlot->GetEnchant();

	DWORD WeightDamage = CalcAttackPower( byEnchant, pItemInfo->m_LV );

	if(pItemSlot && pItemSlot->IsDivine())
	{
		dwPhyMinDamage = pItemInfo->m_wDivPhyMinDamage;
		dwPhyMaxDamage = pItemInfo->m_wDivPhyMaxDamage;
	}
	else if( pItemInfo->IsElite() )
	{
		dwPhyMinDamage = pItemInfo->m_wElitePhyMinDamage;
		dwPhyMaxDamage = pItemInfo->m_wElitePhyMaxDamage;
	}
	else if( pItemInfo->IsUnique() )
	{
		dwPhyMinDamage = pItemInfo->m_wUniPhyMinDamage;
		dwPhyMaxDamage = pItemInfo->m_wUniPhyMaxDamage;
	}
	else
	{
		dwPhyMinDamage = pItemInfo->m_wMinDamage;
		dwPhyMaxDamage = pItemInfo->m_wMaxDamage;
	}

	dwPhyMinAttackPower = dwPhyMinDamage + WeightDamage;
	dwPhyMaxAttackPower = dwPhyMaxDamage + WeightDamage;


	if(pItemSlot && pItemSlot->IsLimited())
	{
		int nRatio = pItemSlot->GetDamageRatio();
		dwLimitedPhyMinAttackPower = dwPhyMinDamage + (WORD)((float)dwPhyMinDamage * nRatio / 100.0f+MODFLOAT(nRatio)) + WeightDamage;
		dwLimitedPhyMaxAttackPower = dwPhyMaxDamage + (WORD)((float)dwPhyMaxDamage * nRatio / 100.0f+MODFLOAT(nRatio)) + WeightDamage;
	}

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	// ����
	const AwakeningInfo* awakening_info = AwakeningInfoParser::Instance()->FindData(pItemSlot->GetCode());
	if ( awakening_info )
	{
		std::map<BYTE, DWORD>::const_iterator iter;
		iter = awakening_info->awakening_level_info_map.find(pItemSlot->GetAwakening());
		if ( iter != awakening_info->awakening_level_info_map.end() )
		{
			dwPhyMinAttackPower += (iter->second);
			dwPhyMaxAttackPower += (iter->second);
		}
	}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM


	// �������ݷ�
	if (dwPhyMinAttackPower==0 && dwPhyMaxAttackPower==0) return;

	WzColor	textColor = WzNormalColor;
	if(pItemSlot && pItemSlot->IsExpiredWaitTime())
	{
		textColor = WzWaitItemColor;	// ���� ����
	}
	else if(pItemSlot && pItemSlot->IsLimited() && ( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0))
	{
		textColor = WzErrorColor;
	}

	_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_PHYATTACK_STRING_INDEX, dwPhyMinAttackPower, textColor,"-", dwPhyMaxAttackPower, textColor);

	if(pItemSlot && pItemSlot->IsLimited())
	{
		ZeroMemory(szInterfaceString, INTERFACE_STRING_LENGTH);
		Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld-%ld"),
			dwLimitedPhyMinAttackPower, dwLimitedPhyMaxAttackPower);
		if(textColor != WzErrorColor)
			textColor = WzCurSkillLevelColor;
		this->RegisterTooltip(szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
	}



}
//------------------------------------------------------------------------------
/** ���� ���ݷ�
*/
void uiToolTipMan::_addItemMagicAttackPowerForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{ 
    return;
}

//------------------------------------------------------------------------------
/** ���� ����
*/
void uiToolTipMan::_addItemPhyDefenseForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	if ( !pItemSlot || !pItemInfo )
		return;

	// ���� ����
	DWORD dwPhyDefense = 0;
	DWORD dwLimitedPhyDefense = 0;

	BYTE byEnchant = 0;

	if(pItemSlot)
	{
		byEnchant = pItemSlot->GetEnchant();
	}


	if(pItemSlot && pItemSlot->IsDivine())
	{
		dwPhyDefense = CalcPhyDef( pItemInfo->m_wDivPhyDef, byEnchant, pItemInfo->m_LV );		

		if(pItemSlot->IsLimited())
		{
			int nRatio = pItemSlot->GetDamageRatio();
			dwLimitedPhyDefense = CalcPhyDef( (DWORD)pItemInfo->m_wDivPhyDef + (DWORD)((float)pItemInfo->m_wDivPhyDef * nRatio / 100.0f+MODFLOAT(nRatio)),
				byEnchant, pItemInfo->m_LV );		
		}

	}
	else if( pItemInfo->IsElite() )
	{

		dwPhyDefense = CalcPhyDef( pItemInfo->m_wElitePhyDef, byEnchant, pItemInfo->m_LV );	

		if(pItemSlot->IsLimited())
		{
			int nRatio = pItemSlot->GetDamageRatio();
			dwLimitedPhyDefense = CalcPhyDef( (DWORD)pItemInfo->m_wElitePhyDef + (DWORD)((float)pItemInfo->m_wElitePhyDef * nRatio / 100.0f+MODFLOAT(nRatio)),
				byEnchant, pItemInfo->m_LV );		
		}

	}
	else if( pItemInfo->IsUnique() )
	{
		dwPhyDefense = CalcPhyDef( pItemInfo->m_wUniPhyDef, byEnchant, pItemInfo->m_LV );				

		if(pItemSlot->IsLimited())
		{
			int nRatio = pItemSlot->GetDamageRatio();
			dwLimitedPhyDefense = CalcPhyDef( (DWORD)pItemInfo->m_wUniPhyDef + (DWORD)((float)pItemInfo->m_wUniPhyDef * nRatio / 100.0f+MODFLOAT(nRatio)),
				byEnchant, pItemInfo->m_LV );		
		}

	}
	else if( pItemSlot->IsFateItem())
	{
		int nRatio = pItemSlot->GetFateAtkDefRatio();

        const float apply_rates = (nRatio / 100.0f);
        float defense_power = static_cast<float>(pItemInfo->m_wPhyDef);
        const float correction_value = (nRatio >= 0.0f) ? +0.5f : -0.5f;

        defense_power += (apply_rates * defense_power + correction_value);
        dwPhyDefense = ns_formula::CalcPhyDef(static_cast<DWORD>(defense_power), byEnchant, pItemInfo->m_LV);

		//dwPhyDefense = CalcPhyDef((DWORD)(pItemInfo->m_wPhyDef * nRatio /100.0f+MODFLOAT(nRatio)), byEnchant, pItemInfo->m_LV);
	}
	else
	{
		dwPhyDefense = CalcPhyDef( pItemInfo->m_wPhyDef, byEnchant, pItemInfo->m_LV );

		if( pItemSlot->IsLimited())
		{
			int nRatio = pItemSlot->GetDamageRatio();
			dwLimitedPhyDefense = CalcPhyDef( (DWORD)pItemInfo->m_wPhyDef + (DWORD)((float)pItemInfo->m_wPhyDef * nRatio / 100.0f+MODFLOAT(nRatio)),
				byEnchant, pItemInfo->m_LV );		
		}

	}

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	// ����
	const AwakeningInfo* awakening_info = AwakeningInfoParser::Instance()->FindData(pItemSlot->GetCode());
	if ( awakening_info )
	{
		dwPhyDefense += AwakeningInfoParser::Instance()->GetOptionValue(pItemSlot->GetCode(), pItemSlot->GetAwakening());		
	}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	
	//���� ���ٸ� ����
	if( !dwPhyDefense ) return;
	
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	WzColor	textColor = WzNormalColor;
	if(pItemSlot && pItemSlot->IsExpiredWaitTime())
	{
		textColor = WzWaitItemColor;	// ���� ����
	}
	else if(pItemSlot && pItemSlot->IsLimited() && ( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0))
	{
		textColor = WzErrorColor;
	}

	_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_PHY_DEFENSE_STRING_INDEX, dwPhyDefense, textColor);

	if(pItemSlot && pItemSlot->IsLimited())
	{
		ZeroMemory(szInterfaceString, INTERFACE_STRING_LENGTH);
		Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), dwLimitedPhyDefense);
		if(textColor != WzErrorColor)
			textColor = WzCurSkillLevelColor;
		this->RegisterTooltip(szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
	}



}
//------------------------------------------------------------------------------
/** ���� ����
*/
void uiToolTipMan::_addItemMagicDefenseForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
    return;
}

//------------------------------------------------------------------------------
float uiToolTipMan::_getLimitItemDura(float maxDura)
{
	if(maxDura <= 0.f) return 0.f;

	float rate = maxDura * c_fDuraRate / 100.0f;

	return rate;
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemDurabilityForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	if ( !pItemSlot || !pItemInfo )
		return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	float	CurrentDura			= 0;
	float	CurrentLimitedDura	= 0;

	int iItemMaxDura = pItemSlot->GetMaxDura();

	if( pItemSlot->IsFateItem() )
	{
		iItemMaxDura = pItemSlot->GetFateDuraMax();
	}

	if(pItemSlot)
	{
		CurrentDura = pItemSlot->GetDura();

		if(pItemSlot->IsLimited())
		{
			CurrentLimitedDura = pItemSlot->GetDura();
			if(CurrentLimitedDura > pItemSlot->GetLimitedDuraMax())
				CurrentLimitedDura = pItemSlot->GetLimitedDuraMax();
		}
		
		if(CurrentDura > iItemMaxDura)
		{
			CurrentDura = (float)iItemMaxDura;
		}
	}
	else
	{
		CurrentDura = pItemInfo->m_Dura;
	}

	WzColor	textColor = WzNormalColor;

	if( pItemSlot->IsExpiredWaitTime())
	{
		textColor = WzWaitItemColor;	// ���� ����
	}
	else if( pItemSlot->IsLimited() && ( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0))
	{
		textColor = WzErrorColor;
	}
	else if( ( _getLimitItemDura((float)pItemInfo->m_Dura) > pItemSlot->GetDura()))
	{
		textColor = WzErrorColor;	// ���� ����
	}

	//TODO:���� v0815�� ���� ��������ġ������ ���󺯰��� �޸��Ѵ�.
	#ifdef __NA_001062_20080623_STRENGTH_PENALTY
		EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));
		
		if(pEnduranceDlg)
		{
			textColor = pEnduranceDlg->GetEnduraLevelColor(*pItemSlot);
		}
	#endif//__NA_001062_20080623_STRENGTH_PENALTY
	
	if(pItemSlot->IsLimited())
		_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_DURABILITY_STRING_INDEX, (DWORD)pItemInfo->m_Dura, textColor, " / ", pItemInfo->m_Dura, textColor);
	else
		_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_DURABILITY_STRING_INDEX, CurrentDura, textColor, " / ", iItemMaxDura, textColor);


	if( pItemSlot->IsLimited())
	{
		ZeroMemory(szInterfaceString, INTERFACE_STRING_LENGTH);
		Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %.0f / %d"), 
			CurrentLimitedDura, pItemSlot->GetLimitedDuraMax());
		if( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0)
			textColor = WzErrorColor;
		if(textColor != WzErrorColor)
			textColor = WzCurSkillLevelColor;

		//TODO:���� v0815�� ���� ��������ġ������ ���󺯰��� �޸��Ѵ�.
	#ifdef __NA_001062_20080623_STRENGTH_PENALTY
		EnduranceDIalog* pEnduranceDlg = static_cast<EnduranceDIalog *>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENDURANCE));

		if(pEnduranceDlg)
		{
			textColor = pEnduranceDlg->GetEnduraLevelColor(*pItemSlot);
		}
	#endif//__NA_001062_20080623_STRENGTH_PENALTY

		this->RegisterTooltip(szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
	}
 

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemLimitedItemRepairCountForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	if(pItemSlot == NULL || !pItemSlot->IsLimited() )	return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	//	11311	������ �ʱ�ȭ ����(ĳ�þ�����) %dȸ
	g_InterfaceManager.GetInterfaceString(11311, szTemp, INTERFACE_STRING_LENGTH);			
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1,szTemp, pItemSlot->GetLimitedRepairCount());

	WzColor	textColor = WzNormalColor;
	if(pItemSlot->IsLimited() && ( pItemSlot->GetDura() <= 0 || pItemSlot->GetLimitedDuraMax() <= 0))
		textColor = WzErrorColor;
	else if((_getLimitItemDura((float)pItemInfo->m_Dura) > pItemSlot->GetDura()))
		textColor = WzErrorColor;	// ���� ����
	

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		textColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}

void uiToolTipMan::_addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, DWORD wStat1, WzColor wzTextColor)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(eStringIndex ,szTemp ,INTERFACE_STRING_LENGTH);			
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s"),szTemp);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(": %ld"),wStat1);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE|c_dwTOOLTIP_EXTRA_CELL1));
}

void uiToolTipMan::_addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, const char *eString, WzColor wzTextColor)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(eStringIndex ,szTemp ,INTERFACE_STRING_LENGTH);			
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s"),szTemp);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(": %s"),eString);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE|c_dwTOOLTIP_EXTRA_CELL1));
}

void uiToolTipMan::_addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, DWORD wStat1, WzColor wzTextColor, char *divstr, DWORD wStat2, WzColor wzTextColor2)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(eStringIndex ,szTemp ,INTERFACE_STRING_LENGTH);			
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s"),szTemp);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	wzTextColor = wzTextColor2;
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(": %ld%s%ld"),wStat1,divstr,wStat2);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE|c_dwTOOLTIP_EXTRA_CELL1));
}

void uiToolTipMan::_addItemLimitInfoForTooltipMakeStr(SCItemSlot* pItemSlot, int eStringIndex, float wStat1, WzColor wzTextColor, char *divstr, DWORD wStat2, WzColor wzTextColor2)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString(eStringIndex ,szTemp ,INTERFACE_STRING_LENGTH);			
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s"),szTemp);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	wzTextColor = wzTextColor2;
	Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(": %.0f%s%ld"),wStat1,divstr,wStat2);

	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE|c_dwTOOLTIP_EXTRA_CELL1));
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitStrInfoForTooltip(SCItemSlot* pItemSlot, 
                                                  const BASE_ITEMINFO *pItemInfo, 
                                                  PlayerAttributes *pCharInfo, 
                                                  LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/,
                                                  PlayerAttributes* simulation_player_attributes /*= NULL*/)
{
	//---------------------------------------------------------------------------------------------
	// �� ����
	//---------------------------------------------------------------------------------------------
	
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;

	if (pItemInfo->IsWeapon())
	{
		WORD wLimitStrWeapon = 0;
		WORD wLimitLimitedItemStrWeapon = 0;
		WORD wLimitStr = pItemInfo->m_wLimitStr;

		if ( wLimitStr > 0 )
		{
			if (pItemSlot && (pItemSlot->IsDivine()))
			{
				wLimitStr = pItemInfo->m_wDivLimitStr;
			}
			WORD wLimitStrWeapon = 0;

			wLimitLimitedItemStrWeapon	= GlobalFunc::CalcLimitStat(wLimitStr, pItemSlot, limitstat_composite_type);
			wLimitStrWeapon				= GlobalFunc::CalcLimitStat(wLimitStr, pItemSlot, limitstat_composite_type, TRUE);
			

			if (wLimitStrWeapon)
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if(pItemSlot && pItemSlot->IsLimited())
				{
					if ( pCharInfo->GetSTR() < wLimitLimitedItemStrWeapon )
						wzTextColor = WzErrorColor;
				}
				else if ( pCharInfo->GetSTR() < wLimitStrWeapon )
				{
					if ( pCharInfo->GetSTR() + pCharInfo->GetDecreaseLimitStat() < wLimitStrWeapon )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_STR, 
                                                                pItemSlot, 
                                                                simulation_player_attributes, 
                                                                wLimitLimitedItemStrWeapon, 
                                                                wLimitStrWeapon,
                                                                wzTextColor);
                }
				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITSTR_STRING_INDEX, wLimitStrWeapon, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitLimitedItemStrWeapon);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}


				return TRUE;
			}
		}
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    else if (pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		WORD wLimitStrArmor = 0;
		WORD LimitStr = pItemInfo->m_wLimitStr;
		if ( LimitStr > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				// ����� �������̴�.
				LimitStr = pItemInfo->m_wDivLimitStr;
			}
			
			DWORD wLimitLimitedStrArmor = GlobalFunc::CalcLimitStat(LimitStr, pItemSlot, limitstat_composite_type);
			wLimitStrArmor = GlobalFunc::CalcLimitStat(LimitStr, pItemSlot, limitstat_composite_type, TRUE);

			if (wLimitStrArmor)
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if(pItemSlot && pItemSlot->IsLimited())
				{
					if ( (DWORD)pCharInfo->GetSTR() < wLimitLimitedStrArmor )
						wzTextColor = WzErrorColor;
				}
				else if ( pCharInfo->GetSTR() < wLimitStrArmor )
				{
					if ( pCharInfo->GetSTR() + pCharInfo->GetDecreaseLimitStat() < wLimitStrArmor )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_STR, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        (WORD)wLimitLimitedStrArmor, 
                        wLimitStrArmor,
                        wzTextColor);
                }
				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITSTR_STRING_INDEX, wLimitStrArmor, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitLimitedStrArmor);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitDexInfoForTooltip(SCItemSlot* pItemSlot, 
                                                  const BASE_ITEMINFO *pItemInfo, 
                                                  PlayerAttributes *pCharInfo, 
                                                  LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/,
                                                  PlayerAttributes* simulation_player_attributes /*= NULL*/)
{
	//---------------------------------------------------------------------------------------------
	// ��ø����. 
	//---------------------------------------------------------------------------------------------
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;

	if (pItemInfo->IsWeapon())
	{
		WORD wLimitDex = pItemInfo->m_wLimitDex;
		if ( wLimitDex > 0)
		{
			WORD wLimitDexWeapon = 0;
			WORD wLimitLimitedDexWeapon = 0;


			if (pItemSlot && (pItemSlot->IsDivine()))
			{					
				wLimitDex = pItemInfo->m_wDivLimitDex;
			}

		    wLimitLimitedDexWeapon  = GlobalFunc::CalcLimitStat(wLimitDex, pItemSlot, limitstat_composite_type);
			wLimitDexWeapon			= GlobalFunc::CalcLimitStat(wLimitDex, pItemSlot,limitstat_composite_type,TRUE);

			if (wLimitDexWeapon)
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetDEX() < wLimitLimitedDexWeapon)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetDEX() < wLimitDexWeapon )
				{
					if ( pCharInfo->GetDEX() + pCharInfo->GetDecreaseLimitStat() < wLimitDexWeapon )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_DEX, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitLimitedDexWeapon, 
                        wLimitDexWeapon,
                        wzTextColor);
                }
				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITDEX_STRING_INDEX, wLimitDexWeapon, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitLimitedDexWeapon);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    else if (pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		WORD wLimitDex = pItemInfo->m_wLimitDex;
		if ( wLimitDex )
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{					
				wLimitDex = pItemInfo->m_wDivLimitDex;
			}

			DWORD wLimitDexWeapon	= GlobalFunc::CalcLimitStat(wLimitDex, pItemSlot, limitstat_composite_type);
			wLimitDex				= GlobalFunc::CalcLimitStat(wLimitDex, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if (wLimitDex)
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if((DWORD)pCharInfo->GetDEX() < wLimitDexWeapon)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetDEX() < wLimitDex )
				{
					if ( pCharInfo->GetDEX() + pCharInfo->GetDecreaseLimitStat() < wLimitDex )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_DEX, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        (WORD)wLimitDexWeapon, 
                        wLimitDex,
                        wzTextColor);
                }

				const int LIMITDEX_STRING_INDEX = 11004;
				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITDEX_STRING_INDEX, wLimitDex, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitDexWeapon);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}

	}

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitIntInfoForTooltip(SCItemSlot* pItemSlot, 
                                                  const BASE_ITEMINFO *pItemInfo, 
                                                  PlayerAttributes *pCharInfo, 
                                                  LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/,
                                                  PlayerAttributes* simulation_player_attributes /*= NULL*/)
{
	//---------------------------------------------------------------------------------------------
	// ���� ����
	//---------------------------------------------------------------------------------------------

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;

	if (pItemInfo->IsWeapon())
	{
		WORD wLimitInt = pItemInfo->m_wLimitInt;
		if( wLimitInt > 0 ) 
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				wLimitInt = pItemInfo->m_wDivLimitInt;
			}
			
			WORD wLimitItemLimitInt	= GlobalFunc::CalcLimitStat(wLimitInt, pItemSlot, limitstat_composite_type);
			wLimitInt				= GlobalFunc::CalcLimitStat(wLimitInt, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if ( wLimitInt )
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetINT() < wLimitItemLimitInt)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetINT() < wLimitInt )
				{
					if ( pCharInfo->GetINT() + pCharInfo->GetDecreaseLimitStat() < wLimitInt )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_INT, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitInt, 
                        wLimitInt,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITINT_STRING_INDEX, wLimitInt, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitInt);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    else if (pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		WORD wLimitInt = pItemInfo->m_wLimitInt;
		if ( wLimitInt > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				wLimitInt = pItemInfo->m_wDivLimitInt;
			}
			
			WORD wLimitItemLimitInt	= GlobalFunc::CalcLimitStat(wLimitInt, pItemSlot, limitstat_composite_type);
			wLimitInt				= GlobalFunc::CalcLimitStat(wLimitInt, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if (wLimitInt)
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetINT() < wLimitItemLimitInt)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetINT() < wLimitInt )
				{
					if ( pCharInfo->GetINT() + pCharInfo->GetDecreaseLimitStat() < wLimitInt )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_INT, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitInt, 
                        wLimitInt,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITINT_STRING_INDEX, wLimitInt, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitInt);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitVitInfoForTooltip(SCItemSlot* pItemSlot, 
                                                  const BASE_ITEMINFO *pItemInfo, 
                                                  PlayerAttributes *pCharInfo, 
                                                  LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/,
                                                  PlayerAttributes* simulation_player_attributes /*= NULL*/)
{
	//---------------------------------------------------------------------------------------------
	// ü�� ����
	//---------------------------------------------------------------------------------------------
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;

	if (pItemInfo->IsWeapon())
	{
		WORD LimitVit = pItemInfo->m_wLimitVit;
		if(LimitVit > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				LimitVit = pItemInfo->m_wDivLimitVit;
			}
			
			WORD wLimitItemLimitVit	= GlobalFunc::CalcLimitStat(LimitVit, pItemSlot, limitstat_composite_type);
			LimitVit				= GlobalFunc::CalcLimitStat(LimitVit, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if ( LimitVit )
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetVIT() < wLimitItemLimitVit)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetVIT() < LimitVit )
				{
					if ( pCharInfo->GetVIT() + pCharInfo->GetDecreaseLimitStat() < LimitVit )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_VIT, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitVit, 
                        LimitVit,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITVIT_STRING_INDEX, LimitVit, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitVit);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}
		}
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    else if (pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		WORD LimitVit = pItemInfo->m_wLimitVit;
		if(LimitVit > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				LimitVit = pItemInfo->m_wDivLimitVit;
			}

			WORD wLimitItemLimitVit	= GlobalFunc::CalcLimitStat(LimitVit, pItemSlot, limitstat_composite_type);
			LimitVit				= GlobalFunc::CalcLimitStat(LimitVit, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if ( LimitVit )
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetVIT() < wLimitItemLimitVit)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetVIT() < LimitVit )
				{
					if ( pCharInfo->GetVIT() + pCharInfo->GetDecreaseLimitStat() < LimitVit )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_VIT, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitVit, 
                        LimitVit,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITVIT_STRING_INDEX, LimitVit, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitVit);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}


				return TRUE;
			}
		}
	}


	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitSprInfoForTooltip(SCItemSlot* pItemSlot, 
                                                  const BASE_ITEMINFO *pItemInfo, 
                                                  PlayerAttributes *pCharInfo, 
                                                  LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/,
                                                  PlayerAttributes* simulation_player_attributes /*= NULL*/)
{
	//---------------------------------------------------------------------------------------------
	// ���ŷ� ����
	//---------------------------------------------------------------------------------------------

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;

	if (pItemInfo->IsWeapon())
	{
		WORD LimitSpr = pItemInfo->m_wLimitSpr;
		if(LimitSpr > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				LimitSpr = pItemInfo->m_wDivLimitSpr;
			}
			
			WORD wLimitItemLimitSpr	= GlobalFunc::CalcLimitStat(LimitSpr, pItemSlot, limitstat_composite_type);
			LimitSpr				= GlobalFunc::CalcLimitStat(LimitSpr, pItemSlot, limitstat_composite_type,TRUE);
			
			

			if ( LimitSpr )
			{

				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetSPR() < wLimitItemLimitSpr)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetSPR() < LimitSpr )
				{
					if ( pCharInfo->GetSPR() + pCharInfo->GetDecreaseLimitStat() < LimitSpr )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_SPR, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitSpr, 
                        LimitSpr,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITSPR_STRING_INDEX, LimitSpr, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitSpr);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}


				return TRUE;
			}

		}
	}
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    else if (pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		WORD LimitSpr = pItemInfo->m_wLimitSpr;
		if(LimitSpr > 0)
		{
			if(pItemSlot && (pItemSlot->IsDivine()))
			{
				LimitSpr = pItemInfo->m_wDivLimitSpr;
			}

			WORD wLimitItemLimitSpr	= GlobalFunc::CalcLimitStat(LimitSpr, pItemSlot, limitstat_composite_type);
			LimitSpr				= GlobalFunc::CalcLimitStat(LimitSpr, pItemSlot, limitstat_composite_type,TRUE);
			

			if ( LimitSpr )
			{

				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					wzTextColor = WzWaitItemColor;
				}
				else if (pItemSlot && (pItemSlot->IsLimited()))
				{
					if(pCharInfo->GetSPR() < wLimitItemLimitSpr)
						wzTextColor = WzErrorColor; 
				}
				else if ( pCharInfo->GetSPR() < LimitSpr )
				{
					if ( pCharInfo->GetSPR() + pCharInfo->GetDecreaseLimitStat() < LimitSpr )
					{
						wzTextColor = WzErrorColor; 
					}
					else
					{
						// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
						wzTextColor = WzEffectColor; 
					}
				}
				else
				{
					wzTextColor = WzNormalColor;						
				}
                if ((simulation_player_attributes != NULL) && (wzTextColor == WzErrorColor))
                {
                    wzTextColor = _GetSimulationLimitedInfoColor(eATTR_SPR, 
                        pItemSlot, 
                        simulation_player_attributes, 
                        wLimitItemLimitSpr, 
                        LimitSpr,
                        wzTextColor);
                }

				_addItemLimitInfoForTooltipMakeStr(pItemSlot, eST_LIMITSPR_STRING_INDEX, LimitSpr, wzTextColor);

				if(pItemSlot && pItemSlot->IsLimited())
				{
					Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitSpr);
					if(wzTextColor != WzErrorColor)
						wzTextColor = WzCurSkillLevelColor;
					RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
				}

				return TRUE;
			}

		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemLimitSkillInfoForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/)
{
	if(!g_pHero)
		return FALSE;

	//---------------------------------------------------------------------------------------------
	// ���õ� ����
	//---------------------------------------------------------------------------------------------

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	WzColor wzTextColor = WzNormalColor;
	BOOL bAddLine = FALSE;
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    if( pItemInfo->IsWeapon() || pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
    if( pItemInfo->IsWeapon() || pItemInfo->IsArmor() )
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	
	{
		const int	c_iMax_EqClass = 5;
		int	iItemEqClass[c_iMax_EqClass] = { pItemInfo->m_wEqClass1, pItemInfo->m_wEqClass2, pItemInfo->m_wEqClass3, pItemInfo->m_wEqClass4, pItemInfo->m_wEqClass5 };
		const int	c_iNumLimitSkill = 2;
		const int	c_iLimitSkillNCode = 11162;
		int			iLimitSkill1 = 0;
		int			iLimitSkill2 = 0;
		int			iLimitEqClass = 0;
		for( int i = 0 ; i < c_iMax_EqClass ; ++i )
		{
			if( iItemEqClass[i] )
			{
				iLimitEqClass = i;
				break;
			}
		}

		//---------------------------------------------------------------------------------------------
		//	ù��°
		//---------------------------------------------------------------------------------------------
		if( pItemInfo->m_wLimitSkil1 )
		{
			WORD	wLimitSkill1 = pItemInfo->m_wLimitSkil1;
			WORD	wLimitItemLimitSkill1 = 0;
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
            if( pItemInfo->IsWeapon() || pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
			if( pItemInfo->IsWeapon() || pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
			{
				if(pItemSlot && (pItemSlot->IsDivine()))
				{
					wLimitSkill1 = pItemInfo->m_wDivLimitSkill1;
				}
				
				wLimitItemLimitSkill1		= GlobalFunc::CalcLimitStat(wLimitSkill1, pItemSlot, limitstat_composite_type);
				wLimitSkill1				= GlobalFunc::CalcLimitStat(wLimitSkill1, pItemSlot, limitstat_composite_type,TRUE);
				

			}
		
			iLimitSkill1 = c_iLimitSkillNCode + (iLimitEqClass * c_iNumLimitSkill);

			WORD wExpComp = pCharInfo->GetExperty1();


			if(pItemSlot->IsLimited())
			{
				wLimitSkill1 = wLimitItemLimitSkill1;
			}

			if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			{
				wzTextColor = WzWaitItemColor;
			}
			else if( g_pHero->GetClass() != (iLimitEqClass + 1) )
			{
				wzTextColor = WzErrorColor;
			}
			else if ( wExpComp < wLimitSkill1 )
			{
				if ( wExpComp + pCharInfo->GetDecreaseLimitStat() < wLimitSkill1 )
				{
					wzTextColor = WzErrorColor; 
				}
				else
				{
					// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
					wzTextColor = WzEffectColor; 
				}
			}
			else
			{
				wzTextColor = WzNormalColor;
			}

			_addItemLimitInfoForTooltipMakeStr(pItemSlot, iLimitSkill1, wLimitSkill1, wzTextColor);

			if(pItemSlot && pItemSlot->IsLimited())
			{
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitSkill1);
				if(wzTextColor != WzErrorColor)
					wzTextColor = WzCurSkillLevelColor;
				RegisterTooltip( szInterfaceString,
					-1,
					iNormalLineHeight,
					FontID_ItemInfo,
					wzTextColor,
					TOOLTIP_BG_WZCOLOR,
					c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
			}

			bAddLine = TRUE;
		}

		//---------------------------------------------------------------------------------------------
		//	�ι�°
		//---------------------------------------------------------------------------------------------
		if( pItemInfo->m_wLimitSkil2 )
		{
			WORD	wLimitSkill2 = pItemInfo->m_wLimitSkil2;
			WORD  wLimitItemLimitSkill2 = 0;

#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
            if( pItemInfo->IsWeapon() || pItemInfo->IsArmor() || (pItemSlot && pItemSlot->IsWingItem()))
#else
            if( pItemInfo->IsWeapon() || pItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
			{
				if(pItemSlot && (pItemSlot->IsDivine()))
				{
					wLimitSkill2 = pItemInfo->m_wDivLimitSkill2;
				}
				
				wLimitItemLimitSkill2	= GlobalFunc::CalcLimitStat(wLimitSkill2, pItemSlot, limitstat_composite_type);
				wLimitSkill2			= GlobalFunc::CalcLimitStat(wLimitSkill2, pItemSlot, limitstat_composite_type,TRUE);
			}
			
			iLimitSkill2 = c_iLimitSkillNCode + (iLimitEqClass * c_iNumLimitSkill) + 1;

			WORD wExpComp2 = pCharInfo->GetExperty2();

			if(pItemSlot->IsLimited())
			{
				wLimitSkill2 = wLimitItemLimitSkill2;
			}

			if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			{
				wzTextColor = WzWaitItemColor;
			}
			else if( g_pHero->GetClass() != (iLimitEqClass + 1) )
			{
				wzTextColor = WzErrorColor;
			}
			else if ( wExpComp2 < wLimitSkill2 )
			{
				if ( wExpComp2 + pCharInfo->GetDecreaseLimitStat() < wLimitSkill2 )
				{
					wzTextColor = WzErrorColor; 
				}
				else
				{
					// ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
					wzTextColor = WzEffectColor; 
				}
			}
			else
			{
				wzTextColor = WzNormalColor;
			}

			_addItemLimitInfoForTooltipMakeStr(pItemSlot, iLimitSkill2, wLimitSkill2, wzTextColor);

			if(pItemSlot && pItemSlot->IsLimited())
			{
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T(" -> %ld"), wLimitItemLimitSkill2);
				if(wzTextColor != WzErrorColor)
					wzTextColor = WzCurSkillLevelColor;
				RegisterTooltip( szInterfaceString,
					-1,
					iNormalLineHeight,
					FontID_ItemInfo,
					wzTextColor,
					TOOLTIP_BG_WZCOLOR,
					c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
			}


			bAddLine = TRUE;
		}

	}

	return bAddLine;
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemLimitInfoForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	if(!g_pHero)
		return;

	if( pItemInfo->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP )
	{
        PlayerAttributes* simulation_player_attributes = NULL;
		PlayerAttributes* pCharInfo = g_pHero->GetPlayerAttribute();

		if(!pCharInfo) 
            return;

		BOOL bAddLine = FALSE;

        simulation_player_attributes = uiCharacterStatusMan::GetSimulationPlayerAttributes();

		if(_addItemLimitStrInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, 
            kLimitStatCompositeNormal,
            simulation_player_attributes) == TRUE)	
        {
            bAddLine = TRUE;
        }

		if(_addItemLimitDexInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, 
            kLimitStatCompositeNormal, 
            simulation_player_attributes) == TRUE)	
        {
            bAddLine = TRUE;
        }

		if(_addItemLimitIntInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, 
            kLimitStatCompositeNormal, 
            simulation_player_attributes) == TRUE)		
        {
            bAddLine = TRUE;
        }

		if(_addItemLimitVitInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, 
            kLimitStatCompositeNormal, 
            simulation_player_attributes) == TRUE)		
        {
            bAddLine = TRUE;
        }

		if(_addItemLimitSprInfoForTooltip(pItemSlot, pItemInfo, pCharInfo,
            kLimitStatCompositeNormal, 
            simulation_player_attributes) == TRUE)		
        {
            bAddLine = TRUE;
        }
			
		if(_addItemLimitSkillInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))
        {
            bAddLine = TRUE;
        }

		if(bAddLine) AddEmptyLineForTooltip();
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemNextGradeLimitInfoForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo, LimitStatCompositeType limitstat_composite_type)
{
	if(!g_pHero)
		return;

    if (pItemInfo == NULL)
    {
        return;
    }    

    BYTE charge_subtype = eCHARGE_SUB_GENERAL;
    if (pItemSlot == NULL)
    {
        charge_subtype = pItemInfo->m_ChargeSubType;
    }
    else
    {
        charge_subtype = pItemSlot->GetItemRuleInfo().charge_sub_type;
    }

	if (charge_subtype == eCHARGE_SUB_FATE)
        return;


	if( pItemInfo->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP )
	{
		PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();
		if(!pCharInfo) return;

		BOOL bAddLine = FALSE;

		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
		WzColor wzTextColor = WzNormalColor;
	
		if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			wzTextColor = WzWaitItemColor;

		//���� ��� ���� ��ġ
		g_InterfaceManager.GetInterfaceString( eST_NEXT_GRADE_LIMIT_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
		this->RegisterTooltip(	szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			wzTextColor,
			TOOLTIP_BG_WZCOLOR,
			0);

		
		if(_addItemLimitStrInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))		bAddLine = TRUE;

		if(_addItemLimitDexInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))		bAddLine = TRUE;

		if(_addItemLimitIntInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))		bAddLine = TRUE;

		if(_addItemLimitVitInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))		bAddLine = TRUE;

		if(_addItemLimitSprInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))		bAddLine = TRUE;
			
		if(_addItemLimitSkillInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, limitstat_composite_type))	bAddLine = TRUE;

		if(bAddLine) AddEmptyLineForTooltip();
	}
}



BOOL uiToolTipMan::_addItemAccOptionForTooltip(SCItemSlot* pItemSlot
												, const WORD* iOptionKind
												, const int* iOption 
												, const BYTE* iType 
												, BOOL bPCBang 
												, const BYTE* btOptionExEffect /*= NULL*/
												, const BYTE* btOptionExEffect_PCBang /*= NULL*/)
	
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szSubMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTempExercise[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};

	//if(iOptionKind[0] <= 0)		return FALSE;

	for(int i = 0; i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
	{
		if(iOptionKind[i] <= 0)		
            continue;

#ifdef _NA_003740_20111122_RANDOM_ITEM
        bool is_enable_randomize = pItemSlot->EnableRandomize();
        RandomValueGrade::Value random_grade = pItemSlot->GetRandomOptionGrade(i);
#endif //_NA_003740_20111122_RANDOM_ITEM

		if( BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(iOptionKind[i]) )
        {
			g_InterfaceManager.GetInterfaceString( pOption->mdwNameCode, szTemp, INTERFACE_STRING_LENGTH );

			WzColor TextColor = WzEffectColor;
			WzColor TextExerciseColor = WzExerciseColor;
			if ( bPCBang )
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					TextColor = WzWaitItemColor;
				}
				else if( g_pHero && g_pHero->GetPCBangMode() )
				{
					TextColor = WzPcBangColor;
					TextExerciseColor = WzExerciseColor;
				}
				else
				{
					TextColor = WzNonPcBangColor;
					TextExerciseColor = WzNonPcBangColor;
				}
				//1099	[PC��] %s +%d 
				g_InterfaceManager.GetInterfaceString(1099, szSubMessage ,INTERFACE_STRING_LENGTH);
				Snprintf( szInterfaceString,INTERFACE_STRING_LENGTH-1, szSubMessage , szTemp, iOption[i]);
			}
			else
			{
				Snprintf( szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s %+d"), szTemp, iOption[i]);
			}

            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(iType[i], pOption, szInterfaceString);

			if(NULL != btOptionExEffect)
			{
				if(bPCBang)
				{
					if(eITEM_CON_STATE_INVEN == (eITEM_CON_STATE)btOptionExEffect_PCBang[i])
					{
						//11387	Ive
						g_InterfaceManager.GetInterfaceString(11387, szTempExercise);
						RegisterTooltip( szTempExercise,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextExerciseColor,
							TOOLTIP_BG_WZCOLOR,
							0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE, -1.0f, -3.0f);
                        }
#endif//_NA_003740_20111122_RANDOM_ITEM


						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
					else
					{
                        bool head = true;

						////�Ǽ��縮�� "Use"ǥ�� ���ش�
						////11386	Use
						//g_InterfaceManager.GetInterfaceString(11386, szTempExercise);
						//RegisterTooltip( szTempExercise,
						//	-1,
						//	iNormalLineHeight,
						//	FontID_ItemInfo,
						//	TextExerciseColor,
						//	TOOLTIP_BG_WZCOLOR,
						//	0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                0, -1.0f, -3.0f);

                            head = false;
                        }
#endif //_NA_003740_20111122_RANDOM_ITEM

						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
                            head ? 0 : c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
				}
				else
				{
					if(eITEM_CON_STATE_INVEN == (eITEM_CON_STATE)btOptionExEffect[i])
					{
						//11387	Ive
						g_InterfaceManager.GetInterfaceString(11387, szTempExercise);
						RegisterTooltip( szTempExercise,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextExerciseColor,
							TOOLTIP_BG_WZCOLOR,
							0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE, -1.0f, -3.0f);
                        }
#endif//_NA_003740_20111122_RANDOM_ITEM

						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
					else
					{
                        bool head = true;

						////�Ǽ��縮�� "Use"ǥ�� ���ش�
						////11386	Use
						//g_InterfaceManager.GetInterfaceString(11386, szTempExercise);
						//RegisterTooltip( szTempExercise,
						//	-1,
						//	iNormalLineHeight,
						//	FontID_ItemInfo,
						//	TextExerciseColor,
						//	TOOLTIP_BG_WZCOLOR,
						//	0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                0, -1.0f, -3.0f);
                            head = false;
                        }
#endif

						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
                            head ? 0 : c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
				}
			}
			else
			{

#ifdef _NA_003740_20111122_RANDOM_ITEM
                if( is_enable_randomize == true )
                {
                    DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                        RegisterTooltipImage(
                        kTooltipTexture_RandomItemGrade,
                        image_index,
                        m_TooltipTextureRectAngles[image_index].right,
                        m_TooltipTextureRectAngles[image_index].bottom,
                        0, -1.0f, -3.0f);

                    RegisterTooltip( szInterfaceString,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        TextColor,
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                }
                else
                {
                    RegisterTooltip( szInterfaceString,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        TextColor,
                        TOOLTIP_BG_WZCOLOR,
                        0);
                }
#else
				RegisterTooltip( szInterfaceString,
					-1,
					iNormalLineHeight,
					FontID_ItemInfo,
					TextColor,
					TOOLTIP_BG_WZCOLOR,
					0);
#endif //_NA_003740_20111122_RANDOM_ITEM
			}
		}
        else
        {
            TCHAR temp[256];
            wsprintf( temp, "unknown OptionIndex [%d]", iOptionKind[i] );
            RegisterTooltip( temp, 
                -1,
                iNormalLineHeight,
                FontID_ItemInfo,
                WzErrorColor,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
	}
	return TRUE;
}


//------------------------------------------------------------------------------
BOOL uiToolTipMan::_addItemNativeOptionForTooltip(SCItemSlot* pItemSlot
													, const WORD* iOptionKind
													, const int* iOption 
													, const BYTE* iType 
													, BOOL bPCBang
													, const BYTE* btOptionExEffect /*= NULL*/
													, const BYTE* btOptionExEffect_PCBang /*= NULL*/)
	
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTempExercise[INTERFACE_STRING_LENGTH + 1] = {0,};

	TCHAR szSubMessage[INTERFACE_STRING_LENGTH] = {0,};

	//if(iOptionKind[0] <= 0)		return FALSE;

	for(int i = 0; i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
	{
		if(iOptionKind[i] <= 0)
            continue;

        int option_value = iOption[i];

#ifdef _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		if( pItemSlot->GetItemInfo()->m_wType == eITEMTYPE_COSTUME || 
			pItemSlot->GetItemInfo()->m_wType == eITEMTYPE_COSTUME_DECORATE )
#else
        if( pItemSlot->GetItemInfo()->m_wType == eITEMTYPE_COSTUME )
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
        {
            // �ۼ������� ��ġ�� ����Ұ�쿡�� 10���� ������
            if( iType[i] != VALUE_TYPE_VALUE )
            {
                option_value /= 10;
            }
        }
#endif //_NA_004035_20120227_COSTUME_ITEM

		if( BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(iOptionKind[i]) )
		{

#ifdef _NA_003740_20111122_RANDOM_ITEM
            bool is_enable_randomize = pItemSlot->EnableRandomize();
            RandomValueGrade::Value random_grade = pItemSlot->GetRandomOptionGrade(i);
#endif //_NA_003740_20111122_RANDOM_ITEM
            
            if (pOption->mdwNameCode == 0)
            {
                continue;
            }

			g_InterfaceManager.GetInterfaceString( pOption->mdwNameCode, szTemp2, INTERFACE_STRING_LENGTH );

			WzColor TextColor = WzEffectColor;
			WzColor TextExerciseColor = WzExerciseColor;
			if( bPCBang )
			{
				if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
				{
					TextColor = WzWaitItemColor;
				}
				else if( g_pHero && g_pHero->GetPCBangMode() )
				{
					TextColor = WzPcBangColor;
					TextExerciseColor = WzExerciseColor;
				}
				else
				{
					TextColor = WzNonPcBangColor;
					TextExerciseColor = WzNonPcBangColor;
				}

				//1150	[PC��] %s %s %+d
				Snprintf( szInterfaceString, INTERFACE_STRING_LENGTH-1, g_InterfaceManager.GetInterfaceString(1150), "", szTemp2 ,option_value);
			}
			else
			{
				if( option_value >= 0)
                {
					Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %+d"), szTemp2, option_value);
                }
				else
                {
					Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %d"), szTemp2, option_value);
                }
			}

            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(iType[i], pOption, szInterfaceString);

			if(NULL != btOptionExEffect)
			{
				if(bPCBang)
				{
					if(eITEM_CON_STATE_INVEN == (eITEM_CON_STATE)btOptionExEffect_PCBang[i])
					{
						//11387	Ive
						g_InterfaceManager.GetInterfaceString(11387, szTempExercise);
						RegisterTooltip( szTempExercise,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							WzExerciseColor,
							TOOLTIP_BG_WZCOLOR,
							0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE, -1.0f, -3.0f);
                        }
#endif //_NA_003740_20111122_RANDOM_ITEM

						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
					else
					{
#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                0, -1.0f, -3.0f);

                            RegisterTooltip( szInterfaceString,
                                -1,
                                iNormalLineHeight,
                                FontID_ItemInfo,
                                TextColor,
                                TOOLTIP_BG_WZCOLOR,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                        }
                        else
                        {
                            RegisterTooltip( szInterfaceString,
                                -1,
                                iNormalLineHeight,
                                FontID_ItemInfo,
                                TextColor,
                                TOOLTIP_BG_WZCOLOR,
                                0);
                        }

#else
						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							0);
#endif //_NA_003740_20111122_RANDOM_ITEM
					}
				}
				else
				{
					if(eITEM_CON_STATE_INVEN == (eITEM_CON_STATE)btOptionExEffect[i])
					{
						//11387	Ive
						g_InterfaceManager.GetInterfaceString(11387, szTempExercise);
						RegisterTooltip( szTempExercise,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							WzExerciseColor,
							TOOLTIP_BG_WZCOLOR,
							0);

#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE, -1.0f, -3.0f);
                        }
#endif //_NA_003740_20111122_RANDOM_ITEM

						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
					}
					else
					{
#ifdef _NA_003740_20111122_RANDOM_ITEM
                        if( is_enable_randomize == true )
                        {
                            DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                            RegisterTooltipImage(
                                kTooltipTexture_RandomItemGrade,
                                image_index,
                                m_TooltipTextureRectAngles[image_index].right,
                                m_TooltipTextureRectAngles[image_index].bottom,
                                0, -1.0f, -3.0f);

                            RegisterTooltip( szInterfaceString,
                                -1,
                                iNormalLineHeight,
                                FontID_ItemInfo,
                                TextColor,
                                TOOLTIP_BG_WZCOLOR,
                                c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                        }
                        else
                        {
                            RegisterTooltip( szInterfaceString,
                                -1,
                                iNormalLineHeight,
                                FontID_ItemInfo,
                                TextColor,
                                TOOLTIP_BG_WZCOLOR,
                                0);
                        }
#else
						RegisterTooltip( szInterfaceString,
							-1,
							iNormalLineHeight,
							FontID_ItemInfo,
							TextColor,
							TOOLTIP_BG_WZCOLOR,
							0);
#endif //_NA_003740_20111122_RANDOM_ITEM
					}
				}
			}
			else
			{

#ifdef _NA_003740_20111122_RANDOM_ITEM
                if( is_enable_randomize == true )
                {
                    DWORD image_index = kTooltipTexture_RandomOptionGradeRactangle0 + (random_grade-RandomValueGrade::kNone);
                    RegisterTooltipImage(
                        kTooltipTexture_RandomItemGrade,
                        image_index,
                        m_TooltipTextureRectAngles[image_index].right,
                        m_TooltipTextureRectAngles[image_index].bottom,
                        0, -1.0f, -3.0f);

                    RegisterTooltip( szInterfaceString,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        TextColor,
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                }
                else
                {
                    RegisterTooltip( szInterfaceString,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        TextColor,
                        TOOLTIP_BG_WZCOLOR,
                        0);
                }
#else
                RegisterTooltip( szInterfaceString,
                    -1,
                    iNormalLineHeight,
                    FontID_ItemInfo,
                    TextColor,
                    TOOLTIP_BG_WZCOLOR,
                    0);

#endif //_NA_003740_20111122_RANDOM_ITEM
			}
		}
        else
        {
            TCHAR temp[256];
            wsprintf( temp, "unknown OptionIndex [%d]", iOptionKind[i] );
            RegisterTooltip( temp, 
                -1,
                iNormalLineHeight,
                FontID_ItemInfo,
                WzErrorColor,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
	}

	return TRUE;
}



//------------------------------------------------------------------------------
void uiToolTipMan::_addItemOptionInfoForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo)
{
	//---------------------------------------------------------------------------------------------
	//		����,��,�Ǽ��縮�� �ɼ� ��ġ
	//---------------------------------------------------------------------------------------------
	BOOL bAddLine = FALSE;
	//---------------------------------------------------------------------------------------------
	//		���� �ɼ�
	//---------------------------------------------------------------------------------------------
	if( pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
	{
		bAddLine |= _addItemAccOptionForTooltip(pItemSlot
				, &pItemInfo->m_wOptionIndex[0]
				, &pItemInfo->m_iOptionValue[0] 
				, &pItemInfo->m_byOptionType[0] 
				, FALSE 
				, &pItemInfo->m_byOptionExEffect[0]
				, &pItemInfo->m_byPCOptionExEffect[0]);
	}
	else
	{
		bAddLine |= _addItemNativeOptionForTooltip(pItemSlot
				,&pItemInfo->m_wOptionIndex[0]
				,&pItemInfo->m_iOptionValue[0]
				,&pItemInfo->m_byOptionType[0]
				,FALSE
				,&pItemInfo->m_byOptionExEffect[0]
				,&pItemInfo->m_byPCOptionExEffect[0]);
	}

	if( pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
	{
		bAddLine |= _addItemAccOptionForTooltip(pItemSlot
				, &pItemInfo->m_wPCOptionIndex[0]
				, &pItemInfo->m_iPCOptionValue[0] 
				, &pItemInfo->m_byPCOptionType[0] 
				, TRUE
				, &pItemInfo->m_byOptionExEffect[0]
				, &pItemInfo->m_byPCOptionExEffect[0]);	
	}
	else
	{
		bAddLine |= _addItemNativeOptionForTooltip(pItemSlot
				,&pItemInfo->m_wPCOptionIndex[0]
				,&pItemInfo->m_iPCOptionValue[0]
				,&pItemInfo->m_byPCOptionType[0]
				,TRUE
				,&pItemInfo->m_byOptionExEffect[0]
				,&pItemInfo->m_byPCOptionExEffect[0]);
	}

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTempExercise[INTERFACE_STRING_LENGTH + 1] = {0,};

	
	if(pItemSlot)
	{
		WzColor wzTextColor = WzEffectColor;
		if( pItemSlot->IsExpiredWaitTime() )
			wzTextColor = WzWaitItemColor;

#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
        //---------------------------------------------------------------------------------------------
        //		��æƮ �߰� �ɼ�
        //---------------------------------------------------------------------------------------------
        const BASE_ITEMINFO* item_info = pItemSlot->GetItemInfo();
        if (item_info && item_info->IsSpecialAccessory() == false )
        {
            // ��æƮ ��� ������ ���
            const MATERIALS_FOR_LEVEL* enchant_info = EnchantParser::Instance()->GetEnchantInfo(
                item_info->IsWeapon(), item_info->m_LV, pItemSlot->GetEnchant());
            if (enchant_info)
            {
                for (int i = 0; i < MATERIALS_FOR_LEVEL::kMaxAddOptionNumber; ++i)
                {
                    // �߰� �ɼ� ǥ��
                    DWORD option_kind = enchant_info->add_option_kind_[i];
                    BYTE option_type = enchant_info->add_option_type_[i];
                    int option_value = enchant_info->add_option_value_[i];
                    if (option_kind <= 0)
                    {
                        continue;
                    }
                    if (BASE_OptionList* base_option = OptionListParser::Instance()->GetOptionList(option_kind))
                    {
                        if ((base_option == NULL) || (base_option->mdwNameCode == 0))
                        {
                            continue;
                        }
                        g_InterfaceManager.GetInterfaceString(base_option->mdwNameCode, szTemp, INTERFACE_STRING_LENGTH);

                        if (option_value >= 0)
                        {
                            Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %+d"), szTemp, option_value);
                        }
                        else
                        {
                            Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH, _T("%s %d"), szTemp, option_value);

                        }

                        //�ɼǸ� ���̻�
                        GlobalFunc::AddOptionSuffix(option_type, base_option, szInterfaceString);

                        RegisterTooltip( szInterfaceString,
                            -1,
                            iNormalLineHeight,
                            FontID_ItemInfo,
                            WzEffectColor,
                            TOOLTIP_BG_WZCOLOR,
                            0);
                    }
                }
            }
        }
#endif //_NA_006731_20130521_ENCHANT_ADD_OPTION

		//---------------------------------------------------------------------------------------------
		//		����Ʈ�� ���� ȿ�� �ɼ�
		//---------------------------------------------------------------------------------------------

        int iAttrValue=0;
        const sEXTRASTONE_OPT_INFO *pExtraStoneInfo=NULL;
        if(pItemSlot->GetItemRuleInfo().IsEnableExtraOption())
        {
            iAttrValue = pItemSlot->GetNOptionAttrValue();
            pExtraStoneInfo = pItemSlot->GetNOptionAttrDesc();
        }
		
		if(iAttrValue && pExtraStoneInfo) 
		{
			g_InterfaceManager.GetInterfaceString(pExtraStoneInfo->m_dwOptionNCode, szTemp, INTERFACE_STRING_LENGTH);
		
			if(IS_NUMBERIC(pExtraStoneInfo->m_byValueType))
			{
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s +%d"), szTemp, iAttrValue);
			}
			else
			{
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, _T("%s +%d%%"), szTemp, iAttrValue);
			}
			
			RegisterTooltip(szInterfaceString,
					-1,
					iNormalLineHeight,
					FontID_ItemInfo,
					wzTextColor,
					TOOLTIP_BG_WZCOLOR,
					0);

			bAddLine = TRUE;
		}

		//---------------------------------------------------------------------------------------------
		//		��ũ �ɼ�
		//---------------------------------------------------------------------------------------------
		if (RANK_E < pItemSlot->GetRank())
		{
			for ( BYTE rank = (BYTE)RANK_D; rank <= pItemSlot->GetRank(); ++rank)
			{
				// ��ũ �Ӽ��� �����Ѵ�.
				BYTE byValue = pItemSlot->GetRankAttrValue((eRANK)rank);

				const sRANK_OPTION * pRankOption = pItemSlot->GetRankAttrDesc( (eRANK)rank );
				if (pRankOption)
				{
					g_InterfaceManager.GetInterfaceString( pRankOption->m_OptionNCode, szTemp2, INTERFACE_STRING_LENGTH);

					if(IS_NUMBERIC(pRankOption->m_NumericType))
					{
						Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, _T("%s +%d"), szTemp2, byValue);
					}
					else
					{
						// �ۼ�Ʈ 
						Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, _T("%s +%d%%"), szTemp2, byValue);
					}

					RegisterTooltip(szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_ItemInfo,
						wzTextColor,
						TOOLTIP_BG_WZCOLOR,
						c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

					bAddLine = TRUE;
				}
			}
		}

		//---------------------------------------------------------------------------------------------
		//		���� �ɼ�
		//---------------------------------------------------------------------------------------------
        if (pItemSlot && pItemSlot->GetSocketIdentifyType() == eSOCKET_UNIDENTIFIED_ITEM)//��Ȯ��
        {
            // 2187: ���� ��Ȯ��
            g_InterfaceManager.GetInterfaceString(2188, szTemp, INTERFACE_STRING_LENGTH);
            RegisterTooltipLineChange(szTemp, _tcslen(szTemp),
                iNormalLineHeight,
                FontID_ItemInfo,
                wzTextColor,
                TOOLTIP_BG_WZCOLOR,
                0);
            bAddLine = TRUE;
        }
        else if (pItemSlot && pItemSlot->GetSocketIdentifyType() == eSOCKET_IDENTIFIED_ITEM)//���ϰ���Ȯ�ε�
        {
            if (bAddLine)
            {
                AddEmptyLineForTooltip();
            }

            const int zard_options_max = sSOCKET_OPTION::OptionPointInfo::kMaxListSize;
            TCHAR socket_buffer_string[zard_options_max][INTERFACE_STRING_LENGTH] = {0,};
            TCHAR socket_options_string[INTERFACE_STRING_LENGTH] = {0};
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR option_format_string[INTERFACE_STRING_LENGTH] = {0,};
            // 12118: [%s %d ����Ʈ]
            g_InterfaceManager.GetInterfaceString(12118, option_format_string);

            for(int sock = SOCKET_1 ; sock < pItemSlot->GetSocketNum() ; ++sock)
            {
                for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
                    i < sSOCKET_OPTION::OptionPointInfo::kMaxListSize; ++i)
                {
                    socket_buffer_string[i][0] = '\0';//�ʱ�ȭ ""
                }

                const sSOCKET_OPTION* socket_option = pItemSlot->GetSocketOption((eSOCKET)sock);
                if (socket_option != NULL)
                {
                    TCHAR string_grade[128] = {0,}, string_name[128] = {0,}, string_option[128] = {0,};
                    /*g_InterfaceManager.GetInterfaceString(string_grade, )
                    socket_option->zard_grade_;
                    socket_option->name_code_;
                    socket_option->option_kind_;
                    socket_option->option_value_;
*/
                    if (socket_option->zard_grade > 0)
                    {
                        // 12119 : �ϱ�, 12120 �߱� 12121 ��� 12122 �ֻ��
                        g_InterfaceManager.GetInterfaceString((12118 + socket_option->zard_grade), 
                            string_grade, INTERFACE_STRING_LENGTH);
                    }
                    else
                    {   // �߸��� ����� ���
                        g_InterfaceManager.GetInterfaceString(0, string_grade, INTERFACE_STRING_LENGTH);
                        assert(string_grade);
                    }

                    const BASE_ITEMINFO* socket_item =
                        ItemInfoParser::Instance()->GetItemInfo(socket_option->item_code);
                    if (socket_item != NULL)
                    {   // ��� ������ �̸�
                        g_InterfaceManager.GetInterfaceString(socket_item->m_NCode, string_name,
                            INTERFACE_STRING_LENGTH);
                    }

                    bool is_check = false;

                    const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();

                    int socket_option_name_code = 0;
                    if (socket_option != NULL)
                    {
                        // ���� �ɼ��� �� �ɼ� ����Ʈ�� ����
                        for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
                            i < socket_option->option_point_list_size; ++i)
                        {
                            // ����Ʈ ������ ��ȿ���� �˻��Ѵ�.
                            const sSOCKET_OPTION::OptionPointInfo* const option_point_info = socket_option->GetPointInfo(i);
                            if (option_point_info == NULL)
                            {
                                continue;
                            }

                            // �ɼ� ������ ��ȿ���� �˻��Ѵ�.
                            const ZardOptionInfo* const zard_option_info = option_point_info->zard_option;
                            if (zard_option_info == NULL || 
                                zard_option_info->attr_option_code == eATTR_TYPE_INVALID || 
                                zard_option_info->item_option_code == eATTR_TYPE_INVALID)
                            {
                                continue;
                            }

                            const BASE_OptionList* const zard_option_desc = OptionListParser::Instance()->GetOptionList(zard_option_info->item_option_code);
                            if (zard_option_desc == NULL)
                            {
                                continue;
                            }
                            // %s�� �� �ɼ� ���� �̸�
                            socket_option_name_code = zard_option_desc->mdwNameCode;
                            //option_point_info->option_point; // %d�� �� �ɼ� ����Ʈ
                            TCHAR option_string[INTERFACE_STRING_LENGTH] = {0,};
                            TCHAR comma_string[INTERFACE_STRING_LENGTH] = {0,};

                            g_InterfaceManager.GetInterfaceString(socket_option_name_code, option_string);
                            sprintf(comma_string, "%c%s", (is_check && i < (zard_options_max - 1))?',':' ', option_format_string);
                            sprintf(socket_buffer_string[i], comma_string, option_string, option_point_info->option_point);
                            is_check = true;  
                        }
                    }
                    if (is_check == false)
                    {
                    }
                    else
                    {
                        sprintf(socket_options_string, "%s%s%s%s%s", socket_buffer_string[0], socket_buffer_string[1],
                            socket_buffer_string[2],socket_buffer_string[3],socket_buffer_string[4]);
                        sprintf(szInterfaceString, _T("%s %s %s"), string_grade, string_name, socket_options_string);
                    }
                    /*
                    // ��� �ɼ� �̸�
                    g_InterfaceManager.GetInterfaceString(socket_option->name_code, string_option,
                        INTERFACE_STRING_LENGTH);
                    
                    if (socket_option->option_type == VALUE_TYPE_VALUE)
                    {
                        Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, _T("%s %s %s +%d"), 
                            string_grade, string_name, string_option, socket_option->option_value);
                    }
                    else
                    {
                        Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, _T("%s %s %s +%d%%"), 
                            string_grade, string_name, string_option, socket_option->option_value);
                    }
                    */

#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
                    RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_SocketEmpty,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right - 4.0f,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom - 4.0f,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT, -2.0f, 0.0f);
					RegisterTooltipImage(socket_option->option_image, kTexture_Rectangle_SocketItem,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right - 8.5f,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom - 8.5f,
                        (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE |
                        c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
                        -(m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right) + 3.0f, 2.0f);
#else
					RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_SocketEmpty,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right + 0.5f,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom + 0.5f,
                       c_dwTOOLTIP_EXTRA_INFO_DT_LEFT, -1.0f, -3.0f);
                    RegisterTooltipImage(socket_option->option_image, kTexture_Rectangle_SocketItem,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right - 6.5f,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom - 6.5f,
                        (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE |
                        c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
                        -(m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right) + 2.0f, 0.0f);
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
                    RegisterTooltip(szInterfaceString,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        wzTextColor,
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                }
                else
                {
                    /*  RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_Socket0,
                        textures_rectangles_[kTexture_Rectangle_Socket0].right,
                        textures_rectangles_[kTexture_Rectangle_Socket0].bottom,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT|c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE,  
                        -100, 0);*/

#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
                    RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_SocketEmpty,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right-4,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom-4,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT, -2.0f, 0.0f);
#else
                    RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_SocketEmpty,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
                        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT, -1.0f, -3.0f);
#endif //_SCJ_SETITEM_TOOLTIP_REFORM

                    // 2189: ���� ������
                    g_InterfaceManager.GetInterfaceString(2189, szTemp, INTERFACE_STRING_LENGTH);
                    RegisterTooltip(szTemp,
                        -1,
                        iNormalLineHeight,
                        FontID_ItemInfo,
                        wzTextColor,
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                }
                bAddLine = TRUE;
            }
        }

		if(bAddLine && !pItemInfo->m_TDNCode) 
            AddEmptyLineForTooltip();
	}	
}
//------------------------------------------------------------------------------
void uiToolTipMan::_addPoralItemDescForTooltip(SCItemSlot* pItemSlot, CODETYPE MapCode)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMapName[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szBuf[INTERFACE_STRING_LENGTH ] = {0,};

	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)MapCode);
	if (!pMapInfo)
	{
		return;
	}

	GetUIMan()->GetInterfaceString( 
		pMapInfo->dwNCode, 
		szMapName, 
		INTERFACE_STRING_LENGTH );

	// 5935	���� ��ġ : %s (%d, %d)
	GetUIMan()->GetInterfaceString( 
		5935, 
		szBuf, 
		INTERFACE_STRING_LENGTH );

	Snprintf(
		szInterfaceString, 
		INTERFACE_STRING_LENGTH, 
		szBuf, 
		szMapName, 
		pItemSlot->GetX(), 
		pItemSlot->GetY() );
	
	RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzEffectColor,			
		TOOLTIP_BG_WZCOLOR,
		0);
}


//------------------------------------------------------------------------------
void uiToolTipMan::_addItemDescForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
   	if(!pItemInfo->m_TDNCode)
		return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};

	WzColor wzTextColor = WzDescColor;
	if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
		wzTextColor = WzWaitItemColor;
	else if( pItemInfo->m_byWasteType > 0)	// ������ ���� �� �ٸ���
		wzTextColor = WzEffectColor;
	else
		wzTextColor = WzDescColor;

	_addItemAdmissionTimeForToolTip(pItemInfo);

	AddEmptyLineForTooltip();

    GameFunc::GetItemDescription(szInterfaceString, pItemSlot, pItemInfo);

	RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzEffectColor,			
		TOOLTIP_BG_WZCOLOR,
		0);

	AddEmptyLineForTooltip();

	_addItemEtherChargingForToolTip(pItemSlot, pItemInfo);
}
//------------------------------------------------------------------------------
/** ���׸� ������ ����ǥ������
*/
void uiToolTipMan::_addItemEtherChargingForToolTip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{	
	if (!g_pHero)
    {
        return;
    }
	
	if ((pItemSlot == NULL) ||
		!g_pHero->GetEthreActive() || 
		(pItemInfo->m_wType != eITEMTYPE_ETHER_BULLET) ||
		(pItemSlot->GetPos() != g_pHero->GetEthreInfo().byEtherChargePos))
	{
		return;
	}
	
	//�����
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString(11466, szInterfaceString, INTERFACE_STRING_LENGTH);

	RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzNormalColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	AddEmptyLineForTooltip();
	//����߿��� �� ���ݽø��� źȯ�� �Һ��մϴ�.
	g_InterfaceManager.GetInterfaceString(11467, szInterfaceString, INTERFACE_STRING_LENGTH);
	
	RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzDescColor,
		TOOLTIP_BG_WZCOLOR,
		0);
	
}



//------------------------------------------------------------------------------
void uiToolTipMan::_addItemAdmissionTimeForToolTip( const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	int hour = 0;
	int min = 0;
	int sec = 0;
	int checkTime = 0;

	switch ( pItemInfo->m_wType )
	{

	case eITEMTYPE_TOP_OF_TRIAL:
		{
			checkTime = (int)(g_InstanceDungeonScene.GetStartCheckTime() + 
				(g_InstanceDungeonScene.GetStartProcessTick() - clock_function::GetTickCount())) / 1000;

			if(checkTime < 0)
			{
				g_InstanceDungeonScene.Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN();
				return;
			}
		
			hour = checkTime / 3600;
			checkTime %= 3600;
			min = checkTime / 60;
			checkTime %= 60;
			sec = checkTime % 60;

			if(g_InstanceDungeonScene.GetJoinState() == eJION_TOP_STATE_NONADMISSION)
			{
				//	11352	(������� ���� �ð� : %d�ð� %d�� %d��)
				GetUIMan()->GetInterfaceString( 11352, szInterfaceString, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szInterfaceString, hour, min, sec);
			}
			else if(g_InstanceDungeonScene.GetJoinState() == eJION_TOP_STATE_ADMISSION)
			{
				//	11353	(���� ���� �ð� : %d�� %d��)
				GetUIMan()->GetInterfaceString( 11353, szInterfaceString, INTERFACE_STRING_LENGTH );
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szInterfaceString, min, sec);
			}
			else
				return;

			// ���� Į�����ۿ� ���ò� ���⶧���� �ȿ��� �־...
			RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
				iNormalLineHeight,
				FontID_ItemInfo,
				WzInstanceDungeonItemColor,			
				TOOLTIP_BG_WZCOLOR,
				c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
		}
		break;

	case eITEMTYPE_SSQ:
		{
			g_SsqScene.Func_Output_with_internal_timer(szMessage);

			RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
				iNormalLineHeight,
				FontID_ItemInfo,
				WzInstanceDungeonItemColor,			
				TOOLTIP_BG_WZCOLOR,
				c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
		}
		break;

	default:
		break;
	}
}


//------------------------------------------------------------------------------

void uiToolTipMan::_addChargeItemDescForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
    if (pItemInfo == NULL)
    {
        return;
    }

    BYTE charge_subtype = eCHARGE_SUB_GENERAL;
    if (pItemSlot == NULL)
    {
        charge_subtype = pItemInfo->m_ChargeSubType;
    }
    else
    {
        charge_subtype = pItemSlot->GetItemRuleInfo().charge_sub_type;
    }

	switch (charge_subtype)
	{
	case eCHARGE_SUB_FIXED_AMOUNT:		//< ������ : ���� �Ⱓ�� ( 2
	case eCHARGE_SUB_FIXED_AMOUNT_EX:	//< Ȯ�������� : ����� ����
	case eCHARGE_SUB_FIXED_QUANTITY:	//< ������ : ��� �Ⱓ�� ( 3
	case eCHARGE_SUB_FIXED_AMOUNT_WAIT:	//< ������ : ���� �Ⱓ�� + 7�� ���
	case eCHARGE_SUB_FATE:
    case eCHARGE_SUB_RIDER_WITH_PART:
	case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
		_addChargeItemDateForTooltip(pItemSlot, pItemInfo);
    	break;


	case eCHARGE_SUB_COORDINATE:		//< ��ǥ
		_addPoralItemDescForTooltip(pItemSlot, pItemSlot->GetMapCode());
		break;


	default:
	case eCHARGE_SUB_GENERAL:			//< ���� ����
		break;
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addChargeItemDateForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo)
{

	if (pItemSlot->IsExpiredTime())
	{
		_addChargeItemDateFixedExpiredTimeForTooltip();
		return;
	}

    BYTE charge_subtype = pItemSlot->GetItemRuleInfo().charge_sub_type;
	switch (charge_subtype)
	{
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        _addChargeItemDateFixedEX2AmoutForTooltip(pItemSlot, pItemInfo);
        break;
	case eCHARGE_SUB_COORDINATE_EX:
		_addChargeItemCoordinateExForTooltip(pItemSlot, pItemInfo);
		break;

    case eCHARGE_SUB_RIDER_WITH_PART:
        {
            //���̴�����  -> Ȯ������ 
            //���̴�������-> ������
            if (pItemInfo->IsRiderPartsItem())
            {
                _addChargeItemDateFixedEXAmoutForTooltip(pItemSlot, pItemInfo);
            }
            else
            {
                _addChargeItemDateFixedAmoutForTooltip(pItemSlot, pItemInfo);
            }
        }
        break;


	case eCHARGE_SUB_FIXED_AMOUNT:		//< ������ : ���� �Ⱓ�� ( 2
		_addChargeItemDateFixedAmoutForTooltip(pItemSlot, pItemInfo);		
		break;

	case eCHARGE_SUB_FIXED_AMOUNT_EX:	//< Ȯ�������� : ����� ����
		_addChargeItemDateFixedEXAmoutForTooltip(pItemSlot, pItemInfo);
		break;

	case eCHARGE_SUB_FIXED_QUANTITY:	//< ������ : ��� �Ⱓ�� ( 3
		_addChargeItemDateFixedQUANTITYAmoutForTooltip(pItemSlot, pItemInfo);
		break;

	case eCHARGE_SUB_FIXED_AMOUNT_WAIT:	//< ������ : ���� �Ⱓ�� + 7�� ���
	case eCHARGE_SUB_FATE:
		{
			if (pItemSlot->IsExpiredWaitTime())
            {
				_addChargeItemDateFixedWaitItemTimeForTooltip(pItemSlot, pItemInfo);
            }
			else
            {
				_addChargeItemDateFixedAmoutForTooltip(pItemSlot, pItemInfo);
            }
		}
		break;


	default:
	case eCHARGE_SUB_GENERAL:			//< ���� ����
		break;
	}
}


void uiToolTipMan::_addChargeItemDateFixedExpiredTimeForTooltip( void )
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};

	AddEmptyLineForTooltip();

	//	5314	���Ⱓ�� ����� �������Դϴ�.
	GetUIMan()->GetInterfaceString( 5314, szInterfaceString, INTERFACE_STRING_LENGTH );
	
	RegisterTooltipLineChange(
		szInterfaceString, 
		_tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzChargeItemDateColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}



void uiToolTipMan::_addChargeItemDateFixedWaitItemTimeForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szKindAMount[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};	

    if (pItemSlot &&
       (pItemSlot->CanUseMixture(MIXTURE_PERIOD_EXTEND) == false))
	{
		return;
	}

	AddEmptyLineForTooltip();	

	if( pItemSlot && pItemSlot->IsFateItem() )
	{
		//11372	���Ⱓ ���� ��� ����	
		g_InterfaceManager.GetInterfaceString(11372,szInterfaceString);
	}
	else
	{
		SYSTEMTIME systime;
		util::YYYYMMDDHHMMSSToSYSTEMTIME( pItemSlot->GetDateTime(), systime );

		// 5895 %4d �� %2d�� %2d�� %2d�� %2d�� ���� ���� �����ϱ� ���� ��� �Ҹ�˴ϴ�.
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 5895, systime);
	}

	RegisterTooltipLineChange(
		szInterfaceString, 
		_tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzChargeItemDateColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	AddEmptyLineForTooltip();
}

void uiToolTipMan::_addChargeItemDateFixedAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szKindAMount[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};	

	AddEmptyLineForTooltip();	


	if( pItemSlot && pItemSlot->IsFateItem() && pItemSlot->GetFatePeriod() == INFINITE_ITEM_DAY)
	{
		//1287 ������
		g_InterfaceManager.GetInterfaceString(1287, szInterfaceString);
	}
	else
	{
		SYSTEMTIME systime;
		util::YYYYMMDDHHMMSSToSYSTEMTIME( pItemSlot->GetDateTime(), systime );

		// 5874   %4d �� %2d�� %2d�� %2d�� %2d�� ����
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 5874, systime);
	}

	RegisterTooltipLineChange(
		szInterfaceString, 
		_tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzChargeItemDateColor,
		TOOLTIP_BG_WZCOLOR,
		0);


	if( pItemSlot && !pItemSlot->IsFateItem()) 
		AddEmptyLineForTooltip();

}

void uiToolTipMan::_addChargeItemDateFixedEXAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szKindAMount[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	AddEmptyLineForTooltip();	

	if ( pItemSlot->IsProgressTime() )
	{
		SYSTEMTIME systime;
		util::YYYYMMDDHHMMSSToSYSTEMTIME( pItemSlot->GetDateTime(), systime );

		// 5874   %4d �� %2d�� %2d�� %2d�� %2d�� ���� 
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 5874, systime);
	}
	else
	{		
        BOOL bActiveMsg = (pItemSlot && 
						  (pItemSlot->CanEquip() == false));
        
        BOOL bExpiredTime = pItemSlot->IsExpiredWaitTime();

        if(pItemSlot->GetItemInfo()->IsRiderPartsItem() && bExpiredTime)
        {
            bActiveMsg = FALSE;
        }

        //�������������ǰ�� Ŭ���� �ߵ�
		if(bActiveMsg)
		{
			//���콺 ��Ŭ���� Ÿ�̸Ӱ� �����մϴ�.
			GetUIMan()->GetInterfaceString( 6157, szInterfaceString, INTERFACE_STRING_LENGTH );
            
            RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
                iNormalLineHeight,
                FontID_ItemInfo,
                WzChargeItemDateColor,
                TOOLTIP_BG_WZCOLOR,
                0);
		}
		//else
		{
			DWORD remain_hour = (DWORD)(pItemSlot->GetDateTime()/(60*60));	
			DWORD remain_min = (DWORD)((pItemSlot->GetDateTime()/60) % 60);

            if(pItemSlot->GetItemInfo()->IsRiderPartsItem() && bExpiredTime)
            {
                remain_min = remain_hour =0;
            }


			if (remain_hour)
			{
				// 5872	�����ð�: %d�� %2d�� 
				GetUIMan()->GetInterfaceString( 5872, szTemp, INTERFACE_STRING_LENGTH );

				Snprintf(
					szInterfaceString, 
					INTERFACE_STRING_LENGTH-1, 
					szTemp, 
					remain_hour, 
					remain_min );
			}
			else
			{
				// 5873	�����ð� : %2d��
				GetUIMan()->GetInterfaceString( 5873, szTemp, INTERFACE_STRING_LENGTH );

				Snprintf(
					szInterfaceString, 
					INTERFACE_STRING_LENGTH-1, 
					szTemp, 
					remain_min );
			}
		}
	}

	RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzChargeItemDateColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	AddEmptyLineForTooltip();
}


void uiToolTipMan::_addChargeItemDateFixedEX2AmoutForTooltip( SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo )
{
    TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szKindAMount[INTERFACE_STRING_LENGTH] = {0,};	
    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

    AddEmptyLineForTooltip();	

    if ( pItemSlot->IsProgressTime() )
    {
        SYSTEMTIME systime;
        util::YYYYMMDDHHMMSSToSYSTEMTIME( pItemSlot->GetDateTime(), systime );

        // 5874   %4d �� %2d�� %2d�� %2d�� %2d�� ���� 
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 5874, systime);
    }
    else
    {		

        BOOL bExpiredTime = pItemSlot->IsExpiredWaitTime();

        DWORD remain_hour = (DWORD)(pItemSlot->GetDateTime()/(60*60));	
        DWORD remain_min = (DWORD)((pItemSlot->GetDateTime()/60) % 60);

        if(pItemSlot->GetItemInfo()->IsRiderPartsItem() && bExpiredTime)
        {
            remain_min = remain_hour =0;
        }

        if (remain_hour)
        {
            // 5872	�����ð�: %d�� %2d�� 
            GetUIMan()->GetInterfaceString( 5872, szTemp, INTERFACE_STRING_LENGTH );

            Snprintf(
                szInterfaceString, 
                INTERFACE_STRING_LENGTH-1, 
                szTemp, 
                remain_hour, 
                remain_min );
        }
        else
        {
            // 5873	�����ð� : %2d��
            GetUIMan()->GetInterfaceString( 5873, szTemp, INTERFACE_STRING_LENGTH );

            Snprintf(
                szInterfaceString, 
                INTERFACE_STRING_LENGTH-1, 
                szTemp, 
                remain_min );
        }
    }

    RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
        iNormalLineHeight,
        FontID_ItemInfo,
        WzChargeItemDateColor,
        TOOLTIP_BG_WZCOLOR,
        0);

    AddEmptyLineForTooltip();
}


void uiToolTipMan::_addChargeItemDateFixedQUANTITYAmoutForTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};

	DWORD remain_hour = (DWORD)(pItemSlot->GetDateTime()/(60*60));	
	DWORD remain_min = (DWORD)((pItemSlot->GetDateTime()/60) % 60);
	if (remain_hour)
	{
		// 5872	�����ð�: %d�� %2d�� 
		GetUIMan()->GetInterfaceString( 5872, szTemp, INTERFACE_STRING_LENGTH );

		Snprintf(
			szInterfaceString, 
			INTERFACE_STRING_LENGTH-1, 
			szTemp, 
			remain_hour, 
			remain_min );
	}
	else
	{
		// 5873	�����ð� : %2d��
		GetUIMan()->GetInterfaceString( 5873, szTemp, INTERFACE_STRING_LENGTH );

		Snprintf(
			szInterfaceString, 
			INTERFACE_STRING_LENGTH-1, 
			szTemp, 
			remain_min );
	}					

	AddEmptyLineForTooltip();	

	RegisterTooltipLineChange(
		szInterfaceString, 
		_tcslen(szInterfaceString),
		iNormalLineHeight,
		FontID_ItemInfo,
		WzChargeItemDateColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	AddEmptyLineForTooltip();
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addChargeItemInfoForToolTipInCashShop(const BASE_ITEMINFO *pItemInfo, DWORD dwCoolTime, DWORD64 SellEndDate)
{
	// ���� �����ۼ����� �����ִ� ����
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

	// ���� ��Ÿ��--------------------
	if(dwCoolTime)
	{
		int iHour = dwCoolTime / 60;
		int iMin = dwCoolTime % 60;

		if(iHour && iMin)
		{
			// 155373	���� ��Ÿ��: %d�ð� %d��
			g_InterfaceManager.GetInterfaceString( 155373 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp, iHour, iMin );
		}
		else if(iHour)
		{
			// 155374	���� ��Ÿ��: %d�ð�
			g_InterfaceManager.GetInterfaceString( 155374 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp , iHour);
		}
		else if(iMin)
		{
			// 155375	���� ��Ÿ��: %d��
			g_InterfaceManager.GetInterfaceString( 155375 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp , iMin );
		}

		this->RegisterTooltip( szInterfaceString,
			-1,
			iNormalLineHeight,
			FontID_ItemInfo,
			WzNormalColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}


    // 0�̸� �Ǹ����ᳯ¥ ��ǥ��
    if( SellEndDate != 0 )
    {
	    ZeroMemory(szInterfaceString, sizeof(szInterfaceString));

	    SYSTEMTIME systime;
	    util::YYYYMMDDHHMMSSToSYSTEMTIME( SellEndDate, systime );

	    // 155382	�Ǹ� ������: %4d �� %2d�� %2d�� %2d�� %2d��
        GlobalFunc::GetDateString(szInterfaceString, INTERFACE_STRING_LENGTH, 155382, systime);
    	
	    RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
		    iNormalLineHeight,
		    FontID_ItemInfo,
		    WzChargeItemDateColor,
		    TOOLTIP_BG_WZCOLOR,
		    0);

	    AddEmptyLineForTooltip();
    }
}

//------------------------------------------------------------------------------
bool uiToolTipMan::_addChargeItemInfoForToolTip(const BASE_ITEMINFO *pItemInfo)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};

	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	// ��� ��Ÿ��
	if(pItemInfo->m_dCoolTime)
	{
		DWORD dwCoolTime = (DWORD)(pItemInfo->m_dCoolTime / 1000);	//(��)
		if(dwCoolTime)
		{
			int iMin = dwCoolTime / 60;
			int iSec = dwCoolTime % 60;

			if(iMin && iSec)
			{
				//11181	�� ��� ���ð�: %d�� %d��
				g_InterfaceManager.GetInterfaceString(11181 , szTemp ,INTERFACE_STRING_LENGTH );
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp, iMin, iSec );
			}
			else if(iMin)
			{
				//11180	�� ��� ���ð�: %d��
				g_InterfaceManager.GetInterfaceString(11180 , szTemp ,INTERFACE_STRING_LENGTH );
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp, iMin);
			}
			else if(iSec)
			{
				//11173   �� ��� ���ð�: %d��
				g_InterfaceManager.GetInterfaceString(11173 , szTemp ,INTERFACE_STRING_LENGTH );
				Snprintf(szInterfaceString,INTERFACE_STRING_LENGTH-1, szTemp , iSec );
			}

			this->RegisterTooltip( szInterfaceString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				WzNormalColor,
				TOOLTIP_BG_WZCOLOR,
				0);

			//_addEmptyLineForTooltip();	
			return true;
		}
	}
	return false;
	// ���� �� ����
	// �߰� ����
}

//------------------------------------------------------------------------------
bool CanTradeSell(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, eITEM_TRADESELL_TYPE trade_sell_type)
{

    if (pItemSlot != NULL)
    {
        return pItemSlot->CanTradeSell(trade_sell_type);
    }

    if (pItemInfo != NULL)
    {
        return SCItemSlot::CanTradeSell(pItemInfo, trade_sell_type);
    }
    return false;
}
//------------------------------------------------------------------------------
void uiToolTipMan::_GetTradeInfoString(TCHAR* trade_info_string, 
                                      SCItemSlot* slot_item, 
                                      const BASE_ITEMINFO* item_info, 
                                      bool is_customizing_item/* = false*/)
{
    if ((trade_info_string == NULL) ||
        (slot_item == NULL) ||
        (item_info == NULL))
    {
        return;
    }

    bool is_do_not_all = CanTradeSell(slot_item, item_info, eITEM_TRADESELL_DONTALL);    
    if (is_do_not_all && 
        (slot_item->IsDestroyItem() == FALSE))    // �μ��� �������� �ƴϸ�
    {
        //�ŷ�, �Ǹ�, �ٴڿ� ������, ������ ĳ���� ����(�����̵�), ���� �Ұ���
        g_InterfaceManager.GetInterfaceString(1349, trade_info_string, INTERFACE_STRING_LENGTH);
    }
    else if (slot_item->IsDestroyItem() != FALSE)
    {
        // 48969 ����, ��� �Ұ���
        g_InterfaceManager.GetInterfaceString(48969, trade_info_string, INTERFACE_STRING_LENGTH);
    }
    else
    {
        bool is_first = true;
        eITEM_TRADESELL_TYPE trade_type_check_list[] = {eITEM_TRADESELL_USED_ITEM, 
            eITEM_TRADESELL_TRADE, 
            eITEM_TRADESELL_SELL, 
            eITEM_TRADESELL_DROP, 
            eITEM_TRADESELL_DELETE, 
            eITEM_TRADESELL_SLOT, 
            eITEM_TRADESELL_WAREHOUSE, 
            eITEM_TRADESELL_GUILDWAREHOUSE};
        DWORD trade_check_string_list[] = {6270,    // ����� �ŷ� �Ұ���
            21005,   // �ŷ�
            124,     // �Ǹ�
            1345,    // �ٴڿ� ������
            1346,    // ����
            11026,   // ���� �̵�
            1347,    // â�� �̵�
            1378};   // ���â�� ����
        int trade_type_check_num = _countof(trade_type_check_list);

        TCHAR info_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
        bool check_trade;
        for (int check_index = 0; check_index < trade_type_check_num; ++check_index)
        {
            check_trade = CanTradeSell(slot_item, item_info, trade_type_check_list[check_index]);
            if ((is_customizing_item == true) &&
                (trade_type_check_list[check_index] == eITEM_TRADESELL_SELL))
            {
                check_trade = false;
            }
            if (check_trade == false)
            {
                g_InterfaceManager.GetInterfaceString(trade_check_string_list[check_index], 
                    info_string);

                if (is_first == false)
                {
                    Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, _T(", %s"), info_string);
                }
                else
                {
                    Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, _T("%s"), info_string);
                    is_first = false;
                }

                _tcscat(trade_info_string, temp_string);
            }
        }

        if(is_first == false)
        {
            // 1348 �Ұ��� 
            g_InterfaceManager.GetInterfaceString(1348, info_string, INTERFACE_STRING_LENGTH);
#ifdef _CHINA
            // �߱� ��ȣ �ɻ�� ���Ͽ� �Ұ����� ������ �����ִ� �۾�
            Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, _T("%s"), trade_info_string);
            Snprintf(trade_info_string, INTERFACE_STRING_LENGTH - 1, _T("%s %s"), info_string, temp_string);
#else
            Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, _T(" %s"), info_string);
            _tcscat(trade_info_string, temp_string);
#endif // _CHINA
        }
    }
}

void uiToolTipMan::_addItemTradeInfoForTooltip( SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo )
{
    if ((pItemSlot == NULL) &&
        (pItemInfo == NULL))
    {
        return;
    }

    if ((pItemSlot != NULL) && 
        (pItemSlot->IsEtheriaItem()))
    {
        return;
    }

    if (CanTradeSell(pItemSlot, pItemInfo, eITEM_TRADESELL_DOALL))
    {
        return;
    }

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
    _GetTradeInfoString(szInterfaceString, pItemSlot, pItemInfo);
	
	if(_tcslen(szInterfaceString) > 0)
	{
		WzColor wzTextColor = WzDescColor;
		if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
			wzTextColor = WzWaitItemColor;
		RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
			iNormalLineHeight,
			FontID_ItemInfo,
			wzTextColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addItemPriceForTooltip(SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo, BOOL bBuyPrice/* = TRUE*/, LimitStatCompositeType prpurchase /* = 1 */)
{
    bool bNotSellItem = (bBuyPrice == FALSE) && 
                        (pItemSlot->CanTradeSell(eITEM_TRADESELL_SELL) == false);

    ShopDialog* shop_dialog = static_cast<ShopDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP));

    if (pItemSlot->CanTradeSell(eITEM_TRADESELL_SELL) == false &&
        (prpurchase == kItemPurchasedByItem || 
         prpurchase == kItemPurchasedByPoint || 
         prpurchase == kItemPurchasedByCurrencyPoint))
    {
        bNotSellItem = false;
    }
		
	if( bNotSellItem )
		return;


	//---------------------------------------------------------------------------------------------
	//		����
	//---------------------------------------------------------------------------------------------
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMoney[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMoneyFinal[INTERFACE_STRING_LENGTH] = {0,};

	MONEY PriceOfGoods = 0;
    DWORD tooltip_type = 0;

	WzColor wzTextColor = WzNormalColor;
	if( pItemSlot && pItemSlot->IsExpiredWaitTime() )
		wzTextColor = WzWaitItemColor;

	if(bBuyPrice)	// ���Ű���
	{
		PriceOfGoods = pItemSlot->GetPriceForBuy();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        // ��� �ü� ȿ��
        if (g_pHero && 
            g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO) != 0 && 
            PriceOfGoods > 0)
        {
            int ratio = g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_SHOP_BUY_HEIM_RATIO);
            bool is_plus = false;
            if (ratio > 0)
                is_plus = true;
            ratio = abs(ratio);
            MONEY percent_value = PriceOfGoods / 100 * ratio;
            if (is_plus == true)
            {
                PriceOfGoods += percent_value;
            }
            else
            {
                PriceOfGoods -= percent_value;
            }
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
		g_InterfaceManager.GetInterfaceString(eST_BUY_PRICE_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
	}
	else				// �ǸŰ���
	{
		PriceOfGoods = pItemSlot->GetPriceForSell();

        switch (prpurchase)
        {
        case kItemPurchasedByItem: // ������(������ �̸����� �� ��)
            g_InterfaceManager.GetInterfaceString(47640, szInterfaceString, INTERFACE_STRING_LENGTH); // 47640 ���Ű���:
            if (shop_dialog)
            {
                TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
                CODETYPE item_code = shop_dialog->currency_item_code();
                DWORD cost_item_number = shop_dialog->get_price();
                g_InterfaceManager.GetItemName(item_code, item_name);
                g_InterfaceManager.GetInterfaceString(47642, szMoney, INTERFACE_STRING_LENGTH); // 47642 %s ��
                Sprintf(szTemp, " %s %s", item_name, szMoney);
                PriceOfGoods = cost_item_number;

                RegisterTooltip(szInterfaceString, -1, iNormalLineHeight, FontID_ItemInfo, wzTextColor,
                    TOOLTIP_BG_WZCOLOR, 0); // ���Ű���:
                RegisterTooltip(" ", -1, iNormalLineHeight, FontID_ItemInfo, wzTextColor,
                    TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE); // ����

                tooltip_type = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE;

                const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
                if (item_info && item_code)
                {
                    RegisterTooltipImage(item_info->m_wVICode, kTexture_Rectangle_SocketItem,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
                        m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
                        (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE));
                }
            }
            break;
        case kItemPurchasedByPoint: 
            g_InterfaceManager.GetInterfaceString(47640, szInterfaceString, INTERFACE_STRING_LENGTH); // 47640 ���Ű���:
            if (shop_dialog)
            {
                RegisterTooltip(szInterfaceString, -1, iNormalLineHeight, FontID_ItemInfo, wzTextColor,
                    TOOLTIP_BG_WZCOLOR, 0);
                RegisterTooltip(" ", -1, iNormalLineHeight, FontID_ItemInfo, wzTextColor,
                    TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE); // ����

                tooltip_type = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE;

                PriceOfGoods = shop_dialog->get_price();

                RegisterTooltipImage(32950, kTexture_Rectangle_SocketItem,
                    m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
                    m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
                    (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE));
                // 47643: %s ����Ʈ
                g_InterfaceManager.GetInterfaceString(47643, szTemp, INTERFACE_STRING_LENGTH);
            }
            break;
        case kItemPurchasedByCurrencyPoint:
            {
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
                g_InterfaceManager.GetInterfaceString(47640, szInterfaceString, INTERFACE_STRING_LENGTH); // 47640 ���Ű���:
                if (shop_dialog)
                {
                    CODETYPE currency_code = shop_dialog->currency_item_code();
                    CurrencyInfoParser::CurrencyInfo* currency_info =  CurrencyInfoParser::Instance()->GetCurrencyInfo(currency_code);                    
                    if (currency_info != NULL)
                    {
                        TCHAR currency_name[INTERFACE_STRING_LENGTH] = {0,};
                        DWORD currency_price = shop_dialog->get_price();

                        g_InterfaceManager.GetInterfaceString(currency_info->currency_name_code, 
                            currency_name);
                        g_InterfaceManager.GetInterfaceString(47642, szMoney, INTERFACE_STRING_LENGTH); // 47642 %s ��
                        Sprintf(szTemp, " %s %s", currency_name, szMoney);
                        PriceOfGoods = currency_price;

                        RegisterTooltip(szInterfaceString, -1, iNormalLineHeight, FontID_ItemInfo, 
                            wzTextColor, TOOLTIP_BG_WZCOLOR, 0); // ���Ű���:
                        RegisterTooltip(" ", -1, iNormalLineHeight, FontID_ItemInfo, wzTextColor,
                            TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE); // ����

                        tooltip_type = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE;

                        RegisterTooltipImage(currency_info->currency_icon_code, kTexture_Rectangle_SocketItem,
                            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
                            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
                            (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE));
                    }
                }
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
            }
            break;
        case kLimitStatCompositeItemPurchased:
            //11025	�� ���� ���� : %s ����
            g_InterfaceManager.GetInterfaceString(11025,szTemp);
            wzTextColor = WzEffectColor;
            break;
        default:
            g_InterfaceManager.GetInterfaceString(eST_SELL_PRICE_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
            break;
        }
	}

	NUMBERFMT nFmt = {0, 0, 3, _T("."), _T(","), 1};
	Sprintf(szMoney, _T("%I64u"), PriceOfGoods);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, szMoneyFinal, sizeof(szMoneyFinal));
	Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, szTemp, szMoneyFinal );

	RegisterTooltip(szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		wzTextColor,
		TOOLTIP_BG_WZCOLOR,
		tooltip_type);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    _addItemHonorRatingForPrice(pItemSlot, pItemInfo, shop_dialog, prpurchase);
#endif //_NA_003027_20111013_HONOR_SYSTEM

	AddEmptyLineForTooltip();
}

//------------------------------------------------------------------------------
#ifdef _NA_002935_20110704_ITEM_SCORE
void uiToolTipMan::_addItemScoreForTooltip(SCItemSlot* item_slot, LimitStatCompositeType limitstat_composite_type)
{
    if (item_slot == NULL)
    {
        return;
    }

    bool is_pc_room = false;
    // limitstat_composite_type�� ���� ������ OtherPlayerEquipmentContainer ���� ȣ��Ǵ� ���̰�,
    // ����� pc�������� �˱� ���� �Ű������� �뵵�ν� �Ѿ�´�.
    if (limitstat_composite_type == kItemOtherEquipPcRoomEnable)
    {
        is_pc_room = true;
    }
    else if (limitstat_composite_type == kItemOtherEquipPcRoomDisable)
    {
        is_pc_room = false;
    }
    else
    {
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
        is_pc_room = g_pHero->GetPCRoomStatus()==CHANGE_PCROOM_STATUS::eStatus_PcRoom;	
#else
        is_pc_room = !!(g_pHero->GetPCBangMode());
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED
    }

    float item_score = ItemScoreParser::Instance()->CalculateSingleScore(*item_slot, is_pc_room);
    int item_score_temp = static_cast<int>(item_score + 0.5f);
    TCHAR	temp[INTERFACE_STRING_LENGTH]={0,};
    _itot(item_score_temp,temp,10);

    if (item_score > 0)
    {
        TCHAR	message_text[INTERFACE_STRING_LENGTH]={0,};
        TCHAR	temp_text[INTERFACE_STRING_LENGTH]={0,};

        //11469 ��� ���� : %s
        g_InterfaceManager.GetInterfaceString(11469, temp_text, INTERFACE_STRING_LENGTH);
        Snprintf(message_text,INTERFACE_STRING_LENGTH-1, temp_text, temp);

        RegisterTooltip(message_text,
            -1,
            iNormalLineHeight,
            FontID_ItemInfo,
            WzNormalColor,
            TOOLTIP_BG_WZCOLOR,
            0);

        AddEmptyLineForTooltip();
    }

}
#endif //_NA_002935_20110704_ITEM_SCORE

//------------------------------------------------------------------------------
#ifdef _NA_003027_20111013_HONOR_SYSTEM
void uiToolTipMan::_addItemHonorRatingForPrice( SCItemSlot* pItemSlot,const BASE_ITEMINFO *pItemInfo, ShopDialog* shop_dialog, LimitStatCompositeType prpurchase)
{
    if ((prpurchase != kItemPurchasedByItem) && 
        (prpurchase != kItemPurchasedByPoint) &&
        (prpurchase != kLimitStatCompositeNormal) && 
        (prpurchase != kItemPurchasedByCurrencyPoint))
    {
        return;
    }

    if ((pItemSlot == NULL) || 
        (shop_dialog == NULL))
    {
        return;
    }

    if (pItemSlot->GetSlotIdx() != SI_NPCSTORE)
    {
        return;
    }

    DWORD shop_id = shop_dialog->GetShopID();
    SHOPINFO* shop_info = ShopInfoParser::Instance()->GetShopList(shop_id);
    if (shop_info == NULL)
    {
        return;
    }

    BYTE tab_index = shop_dialog->GetCurrentTab();
    POSTYPE slot_position = pItemSlot->GetPos();
    slot_position -= (tab_index * SHOPINFO::_MAX_SELLITEM_NUM);
    const SHOPINFO::SHOP_ITEM* shop_item_info = shop_info->GetItemForIndex(tab_index, slot_position);
    if (shop_item_info == NULL)
    {
        return;
    }

    WzColor tooltip_color = WzNormalColor;
    TCHAR tooltip_message[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR honor_name[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR honor_rating[INTERFACE_STRING_LENGTH] = {'\0', };
#if defined(_NA004034_20120102_POINT_WALLET_SYSTEM) && defined(_NA_003923_20120130_GUILD_RENEWAL)
    if ((shop_info->GetShopType() == SHOP_TYPE_OF_CURRENCY) && 
        (shop_info->GetCurrencyItemCode() == 1))
    {
        // ȭ�� ������� ó��
        if (shop_item_info->fame_rating_ > 0)
        {
            // 6869 ��������: ��巹�� %s �̻� �ʿ�
            Snprintf(honor_rating, INTERFACE_STRING_LENGTH - 1, "%d", shop_item_info->fame_rating_);
            g_InterfaceManager.GetInterfaceStringFormat(
                tooltip_message, 
                INTERFACE_STRING_LENGTH - 1, 
                6869, 
                honor_rating);

            if (GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGrade < shop_item_info->fame_rating_)
            {
                tooltip_color = WzErrorColor;
            }

            RegisterTooltip(tooltip_message,
                -1,
                iNormalLineHeight,
                FontID_ItemInfo,
                tooltip_color,
                TOOLTIP_BG_WZCOLOR,
                0);
        }

        tooltip_color = WzNormalColor;
        if (shop_item_info->repute_rating_ > 0)
        {
            // 6870 ��������: ���嵵 %s �̻� �ʿ�
            Snprintf(honor_rating, INTERFACE_STRING_LENGTH - 1, "%d", shop_item_info->repute_rating_);
            g_InterfaceManager.GetInterfaceStringFormat(
                tooltip_message, 
                INTERFACE_STRING_LENGTH - 1, 
                6870, 
                honor_rating);

            uiCharacterStatusCurrencyInfo* ui_currency_info = 
                GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
            uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point_info = NULL;
            if (ui_currency_info != NULL)
            {
                currency_point_info = ui_currency_info->GetCurrencyPointInfo(CURRENCY_INFO_DONATION_GUILDCOIN);
            }

            if ((currency_point_info != NULL) && 
                (currency_point_info->currency_point < shop_item_info->repute_rating_))
            {
                tooltip_color = WzErrorColor;
            }

            RegisterTooltip(tooltip_message,
                -1,
                iNormalLineHeight,
                FontID_ItemInfo,
                tooltip_color,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
    }
    else
#endif //defined(_NA004034_20120102_POINT_WALLET_SYSTEM) && defined(_NA_003923_20120130_GUILD_RENEWAL)
    {
        if (shop_item_info->fame_area_ != 0)
        {
            HonorSystem::FamePointInfo* fame_point_info = HonorSystem::GetHonorSystemData().GetFamePointInfo(shop_item_info->fame_area_);
            if (fame_point_info != NULL)
            {
                // 6770 ��������: %s �� %s �̻� �ʿ�
                g_InterfaceManager.GetInterfaceString(fame_point_info->fame_info->fame_name_code, honor_name);
                g_InterfaceManager.GetInterfaceString((6501 + shop_item_info->fame_rating_), honor_rating);
                g_InterfaceManager.GetInterfaceStringFormat(tooltip_message, INTERFACE_STRING_LENGTH - 1, 6770, honor_name, honor_rating);

                if ((FameListParser::GetFameRating(fame_point_info->fame_info, fame_point_info->fame_point_value) < shop_item_info->fame_rating_))
                {
                    tooltip_color = WzErrorColor;
                }

                RegisterTooltip(tooltip_message,
                    -1,
                    iNormalLineHeight,
                    FontID_ItemInfo,
                    tooltip_color,
                    TOOLTIP_BG_WZCOLOR,
                    0);
            }
        }

        tooltip_color = WzNormalColor;
        if (shop_item_info->repute_area_ != 0)
        {
            tooltip_color = WzNormalColor;
            HonorSystem::ReputePointInfo* repute_point_info = HonorSystem::GetHonorSystemData().GetReputePointInfo(shop_item_info->repute_area_);
            if (repute_point_info != NULL)
            {
                // 6770 ��������: %s �� %s �̻� �ʿ� 
                g_InterfaceManager.GetInterfaceString(repute_point_info->repute_info->repute_name_code, honor_name);
                g_InterfaceManager.GetInterfaceString((6508 + shop_item_info->repute_rating_), honor_rating);
                g_InterfaceManager.GetInterfaceStringFormat(tooltip_message, INTERFACE_STRING_LENGTH - 1, 6771, honor_name, honor_rating);

                DWORD repute_rating = static_cast<DWORD>(ReputeListParser::GetReputeRating(
                    repute_point_info->repute_info, 
                    repute_point_info->repute_point_value));
                if (repute_rating < shop_item_info->repute_rating_)
                {
                    tooltip_color = WzErrorColor;
                }            

                RegisterTooltip(tooltip_message,
                    -1,
                    iNormalLineHeight,
                    FontID_ItemInfo,
                    tooltip_color,
                    TOOLTIP_BG_WZCOLOR,
                    0);
            }
        }
    }
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

//------------------------------------------------------------------------------
void uiToolTipMan::AddEmptyLineForTooltip(int iHeight /* = 0*/)
{
	if(0 == iHeight)
		iHeight = iSpaceLineHeight;

	TCHAR szMessage[32] = {0,};
	szMessage[0] = _T(' ');
	szMessage[1] = NULL;
	this->RegisterTooltip( szMessage,
		-1,
		iHeight,
		FontID_8,
		WzSpaceColor,
		TOOLTIP_BG_WZCOLOR,
		0);

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleNameForTooltip(BASE_STYLEINFO* pStyleInfo)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
	
	g_InterfaceManager.GetInterfaceString( pStyleInfo->m_dwStyleNCode, szText, INTERFACE_STRING_LENGTH );
	
	RegisterTooltip(szText,
		-1,
		iLargeFontHeight,
		FontID_SkillName,
		WzSkillNameColor,
		TOOLTIP_BG_WZCOLOR,
		0);

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	// ���� �Ǵ� ��Ÿ��

	DWORD	dwTooltipExtraStyle = 0;
	TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
	
	WzColor textColor = WzSkillDisableColor;
	if(bActivated) textColor = WzCurSkillDescColor;

	//	���� ���ο� ������ �����ؼ� ���� ���
	BIT_ADD( dwTooltipExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE );
	BIT_ADD( dwTooltipExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT );

	if(pStyleInfo->m_wStyleLV == 0)
	{
		const int c_iStyleString = 409;
		g_InterfaceManager.GetInterfaceString( c_iStyleString, szMessage, INTERFACE_STRING_LENGTH );
	}
	else
	{
		g_InterfaceManager.GetInterfaceString( eST_LEVEL, szText, INTERFACE_STRING_LENGTH );	//	����
#ifdef _INTERNATIONAL_UI
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s%d"),  szText,pStyleInfo->m_wStyleLV );
#else
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d%s"), pStyleInfo->m_wStyleLV, szText );
#endif//_INTERNATIONAL_UI(����������û)
		

	}

	RegisterTooltip(szMessage,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					textColor,
					TOOLTIP_BG_WZCOLOR,
					dwTooltipExtraStyle);

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleCurLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(!bActivated) return;
	if(pStyleInfo->m_wStyleLV == 0) return;

	WzColor textColor = WzCurSkillLevelColor;
	
	AddEmptyLineForTooltip();

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString( eST_CUR_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, pStyleInfo->m_wStyleLV, pStyleInfo->m_wMaxLV);
	
	RegisterTooltip(szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					textColor,
					TOOLTIP_BG_WZCOLOR,
					0);
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleNextLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(pStyleInfo->m_wStyleLV == pStyleInfo->m_wMaxLV)
		return;

	if(pStyleInfo->m_wStyleLV == 0) return;

	AddEmptyLineForTooltip();

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	WzColor textColor = WzSkillDisableColor;

	g_InterfaceManager.GetInterfaceString( eST_NEXT_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	

	if(bActivated)
	{
		textColor = WzNextSkillLevelColor;
	}
	
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, pStyleInfo->m_wStyleLV, pStyleInfo->m_wMaxLV);
	
	RegisterTooltip(szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					textColor,
					TOOLTIP_BG_WZCOLOR,
					0);

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSyleNeedsForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(!g_pHero)
		return;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	���� ����
	if(pStyleInfo->m_wRequireLV > 0)
	{
		WzColor	WzTextColor = WzNextSkillColor;

		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

		if(g_pHero->GetCharInfo()->m_LV < pStyleInfo->m_wRequireLV)		
			WzTextColor = WzSkillErrorColor;	// ������ ����ġ�� 
		else if(!bActivated)
		{
			WzTextColor = WzSkillDisableColor;

			if(bActivated && (pStyleInfo->m_wStyleLV > 1))
			{
				WzTextColor = WzNextSkillColor;
			}
		}
		else
			WzTextColor = WzNextSkillColor;

		
		g_InterfaceManager.GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX	, szText, INTERFACE_STRING_LENGTH );
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %d"), szText, pStyleInfo->m_wRequireLV);
		
		RegisterTooltip(szMessage,
						-1,
						iNormalLineHeight,
						FontID_SkillInfo,
						WzTextColor,
						TOOLTIP_BG_WZCOLOR,
						0);
		
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	�ʿ佺ų ����Ʈ
	if(g_pHero->GetCharInfo()->m_iRemainSkill < pStyleInfo->m_byRequireSkillPoint)	// ��ų ����Ʈ�� ������ ���� ǥ��
	{
		const int	iRequireSkillPoint = 70228;		//	�ʿ� ��ų ����Ʈ: %d

		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};

		WzColor	WzTextColor = WzSkillErrorColor;

		//	�ʿ� ��ų ����Ʈ: %d
		g_InterfaceManager.GetInterfaceString( iRequireSkillPoint, szText, INTERFACE_STRING_LENGTH );
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szText, pStyleInfo->m_byRequireSkillPoint);
		
		// �䱸ġ�� �����ϴ��� Ȯ�� ��, �ؽ�Ʈ �� �ٲܰ�
		RegisterTooltip(szMessage,
						-1,
						iNormalLineHeight,
						FontID_SkillInfo,
						WzTextColor,
						TOOLTIP_BG_WZCOLOR,
						0);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleLimitWeaponForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated, BOOL bQuickSlot)
{
	if(!g_pHero)
		return;

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	WzColor WzTextColor = WzSkillErrorColor;

	g_InterfaceManager.GetInterfaceString( eST_LIMIT_WEAPON_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
	
	const int	iWeaponName = 30000;	//	���� �̸� ����( ��� ���� )

	if( pStyleInfo->m_iWeaponDefine > 0 && pStyleInfo->m_iWeaponDefine < eITEMTYPE_PUNCH )
	{
		g_InterfaceManager.GetInterfaceString( (iWeaponName + pStyleInfo->m_iWeaponDefine), szMessage, INTERFACE_STRING_LENGTH );
	}
	else
	{
		// ���� ���� ������ ��� ����
		return;
	}

	if(g_pHero->GetWeaponKind() == pStyleInfo->m_iWeaponDefine)
	{
		WzTextColor = WzCurSkillColor;
	}

	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, szMessage);
		
	if(!bActivated)
	{
		WzTextColor = WzSkillDisableColor;
	
		if(pStyleInfo->m_wStyleLV > 1)
		{
			if(g_pHero->GetWeaponKind() == pStyleInfo->m_iWeaponDefine)
			{
				WzTextColor = WzNextSkillColor;
			}
		}
	}
	
	RegisterTooltip( szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					WzTextColor,
					TOOLTIP_BG_WZCOLOR,
					0);
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addStyleDescForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(pStyleInfo->m_dwStyleDescCode > 0)
	{
		WzColor textColor = WzSkillDisableColor;

		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};

		if(bActivated) 
			textColor = WzCurSkillDescColor;
		else
		{	
			if(pStyleInfo->m_wStyleLV > 1)
			{
				textColor = WzNextSkillDescColor;
			}	
		}


		g_InterfaceManager.GetInterfaceString(pStyleInfo->m_dwStyleDescCode, szString, INTERFACE_STRING_LENGTH);

		RegisterTooltipLineChange(szString, 
			_tcslen(szString),
			iNormalLineHeight,
			FontID_SkillInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}

//------------------------------------------------------------------------------
BOOL uiToolTipMan::_isEnableShowTooltip()
{
	if( !g_pHero )
	{
		return FALSE;
	}

	if( g_pHero->IsShowFullMap() )
	{
		return FALSE;
	}

	//	�������� ��� ������ ó������ �ʰ� ����
	if (MouseHandler::Instance()->IsExistItemAtHand())
		return FALSE;

	return TRUE;
}

//------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipSocketOption(ZardOptionInfo::ItemOptionCode item_option_code)
{
    if (_isEnableShowTooltip() == false)
    {
        return;
    }

    ZardOptionInfo* zard_option_info = SocketOptionParser::Instance()->FindZardOption(item_option_code);
    if (zard_option_info == NULL)
    {
        return;
    }

    BASE_OptionList* base_option_list = OptionListParser::Instance()->GetOptionList(item_option_code);
    if (base_option_list == NULL)
    {
        return;
    }


    InitTooltip();

    //------------------------------------------------------------------------------ 
    TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	str_format[INTERFACE_STRING_LENGTH]={0,};
    static WzColor socket_name_color = WzColor_RGBA(207, 187, 139, 255);

    g_InterfaceManager.GetInterfaceString(base_option_list->mdwNameCode, szText, INTERFACE_STRING_LENGTH );
    RegisterTooltip(szText, -1, iNameLineHeight, FontID_ItemName, socket_name_color, TOOLTIP_BG_WZCOLOR, 0);

    //! �ڵ� ���Ͽɼ� ����
    g_InterfaceManager.GetInterfaceString(3443, szMessage, INTERFACE_STRING_LENGTH );
    RegisterTooltip(szMessage, -1, iNameLineHeight, FontID_ItemInfo, WzNormalColor, TOOLTIP_BG_WZCOLOR, 0);

    BOOST_REVERSE_FOREACH(const ZardOptionInfo::OptionValueMap::value_type& option_value_pair, 
                            zard_option_info->GetOptionValues())
    {
        ZardOptionInfo::OptionValueInfo* option_value = option_value_pair.second;

        if (option_value->point == 0)
        {
            continue;
        }
        
        WzColor text_color = WzColor_RGBA(255,0,0,255);
        if (option_value->point > 0)
        {
            text_color = WzColor_RGBA(0,255,0,255);
            _tcscpy(str_format, _T(" %dp : %s +%d"));
        }
        else
        {
            text_color = WzColor_RGBA(255,0,0,255);
            _tcscpy(str_format, _T("%dp : %s %d"));
        }
        //if ((option_value->type == VALUE_TYPE_PERCENT_PER_CUR) || (option_value->type == VALUE_TYPE_PERCENT_PER_MAX))

        Sprintf(szMessage, str_format, option_value->point, szText, option_value->value);

        //�ɼǸ� ���̻�
        GlobalFunc::AddOptionSuffix(option_value->type, base_option_list, szMessage);
        
        RegisterTooltip(szMessage, -1, iNormalLineHeight, FontID_8, text_color, TOOLTIP_BG_WZCOLOR, 0);
    }

    AddEmptyLineForTooltip();
}
////------------------------------------------------------------------------------
//void uiToolTipMan::RegisterTooltipStyle(BASE_STYLEINFO* curStyleInfo, BASE_STYLEINFO* nextStyleInfo,bool isActivated)
//{
//	if(!_isEnableShowTooltip()) return;
//
//	if (!curStyleInfo)
//    {
//        assert ("��Ÿ�� ������ �ݵ�� �ʿ��մϴ�. (curStyleInfo == NULL)");
//        return;
//    }
//
//	InitTooltip();
//
//	_addStyleNameForTooltip(curStyleInfo);
//	_addStyleLevelForTooltip(curStyleInfo, isActivated);
//
//	_setStyleCurLevInfoForTooltip(curStyleInfo, isActivated);
//	if(isActivated)
//	{
//		_setStyleNextLevInfoForTooltip(nextStyleInfo, isActivated);
//	}
//	else
//	{
//		_setStyleNextLevInfoForTooltip(curStyleInfo, isActivated);
//	}
//
//	_addEmptyLineForTooltip();
//}

//------------------------------------------------------------------------------
void uiToolTipMan::_setStyleCurLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(bActivated)
	{
		_addStyleCurLevelForTooltip(pStyleInfo, bActivated);
		
		_addStyleLimitWeaponForTooltip(pStyleInfo, bActivated, FALSE);
		AddEmptyLineForTooltip();
		_addStyleDescForTooltip(pStyleInfo, bActivated);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_setStyleNextLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
{
	if(pStyleInfo)
	{
		_addStyleNextLevelForTooltip(pStyleInfo, bActivated);
		
		_addSyleNeedsForTooltip(pStyleInfo, bActivated);
		
		_addStyleLimitWeaponForTooltip(pStyleInfo, bActivated, FALSE);
		AddEmptyLineForTooltip();
		_addStyleDescForTooltip(pStyleInfo, bActivated);
	}

}

//------------------------------------------------------------------------------
void uiToolTipMan::_registerSkillslotInfoTooltip(const SCSlot *pSlot, BOOL bFromQuickSlot)
{
	if(!pSlot)
		return;

	if(!_isEnableShowTooltip())
		return;

    int skill_code = pSlot->GetCode();

    if( bFromQuickSlot == TRUE )
    {
        // �������� ��ų�� ��¥��ų
        skill_code = xGetSkillData()->GetRealSkillCode( skill_code );

        if( skill_code == INVALID_SKILL_CODE )
        {
            // �׼�
            skill_code = pSlot->GetCode();
        }
    }

    SkillScriptInfo* pSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo( skill_code );

	assert (pSkillInfo);
	
	if(!pSkillInfo) 
		return;

	if(pSkillInfo->m_SkillCode >= eACTION_START_SKILL_CODE && 
		pSkillInfo->m_SkillCode < (eEMOTION_START_CODE + 1000) )
	{
		RegisterTooltipSkillEtc(pSkillInfo);
		
	}
	else if(bFromQuickSlot)
	{
		_addSkillNameForTooltip(pSkillInfo);
		_addSkillLevelForTooltip(pSkillInfo);
		_addSkillEtherBulletUseInfoTooltip(pSkillInfo);
		_addSkillRemainCoolTimeForTooltip(pSkillInfo);
		_setSkillBaseInfoForTooltip(pSkillInfo, TRUE, TRUE);
		AddEmptyLineForTooltip();
		_addSkillDescForTooltip(pSkillInfo, TRUE);

		AddEmptyLineForTooltip();
	}
	else
	{
		RegisterTooltipSkill(pSkillInfo);

		AddEmptyLineForTooltip();
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillNameForTooltip(SkillScriptInfo* pSkillInfo)
{
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};

	DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
	sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

	//	��ų �̸�
	g_InterfaceManager.GetInterfaceString( pSkillInfo->m_dwSkillNcode, szString, INTERFACE_STRING_LENGTH );
	this->RegisterTooltip(	szString,
							-1,
							iLargeFontHeight,
							FontID_SkillName,
							WzSkillNameColor,
							TOOLTIP_BG_WZCOLOR,
							0,
							sp_tooltip);
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillTypeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)	
{
	if( pSkillInfo->m_bySkillUserType != eSKILL_USER_ACTION && pSkillInfo->m_bySkillUserType != eSKILL_USER_EMOTICON )
	{
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		
		WzColor textColor = WzSkillDisableColor;
		if(bActivated)
		{
			textColor = WzCurSkillDescColor;
		}

		// ������ ����
		DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
		
		g_InterfaceManager.GetInterfaceString( pSkillInfo->m_wSkillAttribute, szString, INTERFACE_STRING_LENGTH);
		this->RegisterTooltip( szString,
			-1,
			iNameLineHeight,
			FontID_SkillInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			dwTooltipExtraStyle);



        if( pSkillInfo->m_bySkillType == SKILL_TYPE_PASSIVE )
        {
            g_InterfaceManager.GetInterfaceString( 406, szString, INTERFACE_STRING_LENGTH);
            this->RegisterTooltip( szString,
                -1,
                iNormalLineHeight,
                FontID_SkillInfo,
                WzPassiveTextColor,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillLevelForTooltip(SkillScriptInfo* pSkillInfo)
{
	// �����Կ� ������ ���� (������ ����)
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
	
	g_InterfaceManager.GetInterfaceString( eST_LEVEL, szTemp, INTERFACE_STRING_LENGTH );	//	���� 
#ifdef _INTERNATIONAL_UI
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s%d"), szTemp,pSkillInfo->m_wSkillLv );		
#else
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%d%s"), pSkillInfo->m_wSkillLv, szTemp );	
#endif//_INTERNATIONAL_UI(����������û)
	
	
	RegisterTooltip(szString,
				-1,
				iNameLineHeight,
				FontID_SkillInfo,
				WzCurSkillDescColor,
				TOOLTIP_BG_WZCOLOR,
				dwTooltipExtraStyle);

}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillRemainCoolTimeForTooltip(SkillScriptInfo* pSkillInfo)
{
	if(!g_pHero)
		return;

	// �����Կ� ������ ���� (������ ����)
	TCHAR	szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	//	���� ��Ÿ�� : %d ��
	
	util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(pSkillInfo->m_SkillCode);
	if (pTimer && !pTimer->IsExpired(FALSE))	// ��Ÿ���� ��������
	{
		DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;

		float fRemainRate = 1.0f - pTimer->GetProgressRatio();
		float fRemainTime = pTimer->GetIntervalTime() * fRemainRate;

		DWORD dwCoolTime = (DWORD)(fRemainTime / 1000);
		g_InterfaceManager.GetInterfaceString( eST_REMAIN_COOLTIME_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
		Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, szTemp, dwCoolTime );

		RegisterTooltip( szInterfaceString,
						-1,
						iNormalLineHeight,
						FontID_SkillInfo,
						WzNormalColor,
						TOOLTIP_BG_WZCOLOR,
						dwTooltipExtraStyle);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillNormalDescForTooltip(SkillScriptInfo* pSkillInfo)
{
	if(pSkillInfo->m_dwSkillDesccode > 0)
	{
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		
		g_InterfaceManager.GetInterfaceString(pSkillInfo->m_dwSkillDesccode, szString, INTERFACE_STRING_LENGTH);

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM        
        if (pSkillInfo->m_SkillCode == 35023)
        {
            // Ư���� ��Ȳ�� �׼�
            TCHAR style_string[INTERFACE_STRING_LENGTH] = {0, };

            uiBase* perk_dialog_ptr = GET_DIALOG(IM_SKILL_MANAGER::SKILL_PERK_DLG);
            if ( perk_dialog_ptr )
            {
                int style = static_cast<uiPerkDialog*>(perk_dialog_ptr)->
                                               get_active_group_number_();
                switch ( style )
                {
                case eStylePageA:
                    {
                        // 11105 B
                        g_InterfaceManager.GetInterfaceString( 11105, style_string, INTERFACE_STRING_LENGTH);
                    }
                    break;
                case eStylePageB:
                    {
                        // 11106 A
                        g_InterfaceManager.GetInterfaceString( 11106, style_string, INTERFACE_STRING_LENGTH);
                    }
                    break;
                default:
                    {
                        // �⺻ �� ���� B�� �Ѵ�.
                        g_InterfaceManager.GetInterfaceString( 11105, style_string, INTERFACE_STRING_LENGTH);
                    }
                }
            }            
            Snprintf(szString, INTERFACE_STRING_LENGTH, szString, style_string);
            RegisterTooltipLineChange(szString,
                _tcslen(szString),
                iNormalLineHeight,
                FontID_SkillInfo,
                WzCurSkillDescColor,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
        else
        {
            RegisterTooltipLineChange(szString,
                _tcslen(szString),
                iNormalLineHeight,
                FontID_SkillInfo,
                WzCurSkillDescColor,
                TOOLTIP_BG_WZCOLOR,
                0);
        }
#else

		RegisterTooltipLineChange(szString,
								_tcslen(szString),
								iNormalLineHeight,
								FontID_SkillInfo,
								WzCurSkillDescColor,
								TOOLTIP_BG_WZCOLOR,
								0);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
	}

}

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
// ��ų�Ŀ���ġ�� ���� �����Ǵ� ��ų���� �����ϱ� �����ϱ� ���� �Լ�
// �����Ǵ� ��� '������ 300(+100)'
// �������� ���� ���'������ 300'
// ������ ǥ�õǴµ� ���
BOOL uiToolTipMan::IsUseSkillPowerAbillity( BASE_SkillTextInfo* skill_text_info, int abillity_index )
{
    if( skill_text_info == NULL )
        return FALSE;

    // ���� ���ǵ� �����Ƽ �ڵ�� �ش� Ÿ���� �ݵ�� 1�ϰ�쿡�� TRUE

    switch( skill_text_info->m_arrayParam[abillity_index].m_dw_AbilityCode )
    {
    case eABILITY_DAMAGE:               // ��ų ������
    case eABILITY_DAMAGE_PER_TIME:      // �ð��� ������

    case eABILITY_CUR_HP_INCREASE:	    // ���� HP ����
    case eABILITY_AGGROPOINT_INCREASE:  // ��׷� ����Ʈ ����

    case eABILITY_WIND_SHIELD:          // ���彯��
    case eABILITY_BUFF_RANGE_DAMAGE:    // ���� ���� ������(��������, ĳ���� �ֺ��� �ð��� ������)

    case eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY: // ����� ������ �߰�
    case eABILITY_BONUS_DAMAGE_PER_STATUS:          // ���º� �߰� ������ �߻�

    case eABILITY_SUCTION_HPMP:    // ����������
    case kAbilityIncreseHeal:                       // ġ���� ����
    case kAbilityComboSkillEffectAddDamage:         // ���� ��ų �ߵ� ȿ�� - ������ �߰�
    case kAbilityIncreseSkillDamage:                // ��ų ������ ����

        return skill_text_info->m_arrayParam[abillity_index].m_dw_ParamType == 1;
    }

    return FALSE;
}
#endif //_NA_0_20120321_SKILLTREE_SECOND_RENEWAL


struct stData
{
    DWORD m_ClassCode;
    DWORD m_SkillCode;
    DWORD m_DisplaySkillCode;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    DWORD m_LinkSkillCode;
    stData() : 
    m_ClassCode(0),
    m_SkillCode(0),
    m_DisplaySkillCode(0),
    m_LinkSkillCode(0)
    {

    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

// ������ ǥ���� ��ų������ �����´�.
// �Ϻ� ��ų�� �ٸ� ��ų������ �̿��� ������ ǥ���ϴ� ��찡 �ֱ⶧��
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
BOOL _ReplaceSkillInfoForTooltip( SkillScriptInfo** pSkillInfo, OUT SkillScriptInfo** link_skill_info)
#else
BOOL _ReplaceSkillInfoForTooltip( SkillScriptInfo** pSkillInfo )
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
{

    static std::vector<stData> s_Array;
    if( s_Array.empty() )
    {
        stData data;
        data.m_ClassCode = eSKILL_MYSTIC_932;
        data.m_SkillCode = 14621;
        data.m_DisplaySkillCode = 14721;
        s_Array.push_back(data);
        data.m_ClassCode = eSKILL_MYSTIC_931;
        data.m_SkillCode = 14601;
        data.m_DisplaySkillCode = 14711;
        s_Array.push_back(data);
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        data.m_ClassCode = eSKILL_HELLROID_1004;    // ���ο����
        data.m_SkillCode = 14771;
        data.m_DisplaySkillCode = 8001;
        s_Array.push_back(data);
        data.m_ClassCode = eSKILL_HELLROID_1006;    // ��ũ�ʽ�����
        data.m_SkillCode = 14791;
        data.m_DisplaySkillCode = 8011;
        s_Array.push_back(data);
        data.m_ClassCode = eSKILL_HELLROID_1011;    // �̽�æ������
        data.m_SkillCode = 14841;                   
        data.m_DisplaySkillCode = 8021;            
        s_Array.push_back(data);                    
        data.m_ClassCode = eSKILL_HELLROID_1013;    // ��Ŭ���ο���
        data.m_SkillCode = 14861;                   
        data.m_DisplaySkillCode = 8031;            
        s_Array.push_back(data);                    
        data.m_ClassCode = eSKILL_HELLROID_1020;    // ��Ŀ�������� 
        data.m_SkillCode = 14931;                   
        data.m_DisplaySkillCode = 8041;            
        s_Array.push_back(data);                    
        data.m_ClassCode = eSKILL_HELLROID_1022;    // �ν�Ʈ����
        data.m_SkillCode = 14951;                   
        data.m_DisplaySkillCode = 8051;            
        s_Array.push_back(data);                    
        data.m_ClassCode = eSKILL_HELLROID_1027;    // �̱׳�����
        data.m_SkillCode = 14011;                   
        data.m_DisplaySkillCode = 8071;            
        s_Array.push_back(data);                    // ����Ʈ���̼ǿ���
        data.m_ClassCode = eSKILL_HELLROID_1029;
        data.m_SkillCode = 14051;
        data.m_DisplaySkillCode = 8081;
        s_Array.push_back(data);
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        data.m_ClassCode = eSKILL_WITCHBLADE_1111;  // ���̽��ҵ�
        data.m_SkillCode = 9110;
        data.m_DisplaySkillCode = 9340;
        data.m_LinkSkillCode = 9360;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_WITCHBLADE_1112;  // �̺��̵�
        data.m_SkillCode = 9120;
        data.m_DisplaySkillCode = 9350;
        data.m_LinkSkillCode = 9370;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_RISINGFORCE;      // ����¡����
        data.m_SkillCode = 9150;
        data.m_DisplaySkillCode = 9380;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_WITCHBLADE_1124;  // ƼŸ�Ͼ�
        data.m_SkillCode = 9240;
        data.m_DisplaySkillCode = 9390;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_WITCHBLADE_1127;  // �꽬
        data.m_SkillCode = 9270;
        data.m_DisplaySkillCode = 9400;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_WITCHBLADE_1130;  // ��������
        data.m_SkillCode = 9300;
        data.m_DisplaySkillCode = 9410;
        s_Array.push_back(data);
        data.m_LinkSkillCode = 0;
        data.m_ClassCode = eSKILL_WITCHBLADE_1132;  // ���׳�ƽ�׶����
        data.m_SkillCode = 9320;
        data.m_DisplaySkillCode = 9420;
        s_Array.push_back(data);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    }

    for( size_t i=0;i!=s_Array.size();++i)
    {
        if( (*pSkillInfo)->m_SkillClassCode == s_Array[i].m_ClassCode )
        {
            int new_skill_code = (*pSkillInfo)->m_SkillCode - s_Array[i].m_SkillCode + s_Array[i].m_DisplaySkillCode;
            SkillScriptInfo* pNewSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( new_skill_code );
            if( pNewSkillInfo == NULL )
                return FALSE;

            *pSkillInfo = pNewSkillInfo;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if (s_Array[i].m_LinkSkillCode != 0)
            {
                new_skill_code = (*pSkillInfo)->m_SkillCode - s_Array[i].m_DisplaySkillCode + s_Array[i].m_LinkSkillCode;
                pNewSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( new_skill_code );
                if( pNewSkillInfo != NULL )
                {
                    *link_skill_info = pNewSkillInfo;
                }
            }
            
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            break;
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillDescForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
    //�ٸ� ��ų������ �̿��ؼ� ������ ǥ���ؾ��ϴ� ��ų�̶�� ��ų������ �ش� ��ų�� �ٲ�ġ���Ѵ�
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    SkillScriptInfo* link_skill_script_info = NULL;
    if ( _ReplaceSkillInfoForTooltip( &pSkillInfo, &link_skill_script_info) == FALSE)
        return;
#else
    if( _ReplaceSkillInfoForTooltip( &pSkillInfo ) == FALSE )
        return;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	if(pSkillInfo->m_dwSkillDesccode > 0)
	{
		pSkillInfo->SetFirst();
		BASE_ABILITYINFO* pAbilityInfo = pSkillInfo->GetNext();


		int iTempAbility[26] = {0,};

		// �����Ƽ ���� �迭�� �����´�
		int index = 0;
		while( pAbilityInfo != NULL )
		{
			int i= index * 5;

			iTempAbility[1+i] = pAbilityInfo->m_wSuccessRate;
			iTempAbility[2+i] = pAbilityInfo->m_iOption1;
			iTempAbility[3+i] = pAbilityInfo->m_iOption2;
			iTempAbility[4+i] = pAbilityInfo->m_iParam[0];
			iTempAbility[5+i] = pAbilityInfo->m_iParam[1];

			pAbilityInfo = pSkillInfo->GetNext();
			++index;
		}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (link_skill_script_info != NULL)
        {
            link_skill_script_info->SetFirst();
            BASE_ABILITYINFO* temp_ability_info = link_skill_script_info->GetNext();

            while ( temp_ability_info != NULL )
            {
                int j = index * 5;

                iTempAbility[1+j] = temp_ability_info->m_wSuccessRate;
                iTempAbility[2+j] = temp_ability_info->m_iOption1;
                iTempAbility[3+j] = temp_ability_info->m_iOption2;
                iTempAbility[4+j] = temp_ability_info->m_iParam[0];
                iTempAbility[5+j] = temp_ability_info->m_iParam[1];

                temp_ability_info = link_skill_script_info->GetNext();
                ++index;
            }
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


		WzColor textColor = WzSkillDisableColor;

		BASE_SkillTextInfo* pSkillTextInfo = SkillTextInfoParser::Instance()->GetSkillTextInfo(pSkillInfo->m_dwSkillDesccode);
		if(pSkillTextInfo && pSkillTextInfo->m_dw_DescCode)
		{
			TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(pSkillTextInfo->m_dw_DescCode, szTemp, INTERFACE_STRING_LENGTH);

            PlayerAttributes* player_attributes = g_pHero->GetPlayerAttribute();		

            int skill_percent_damage = player_attributes->GetSkillPercentDamage();

            const int kParameterNum = 8;
            char parameter_string[kParameterNum][64];
            ZeroMemory(parameter_string, sizeof(parameter_string));
            for(int i = 0; i < kParameterNum; ++i)
            {
                if(pSkillTextInfo->m_arrayParam[i].m_dw_AbilityCode)
                {
                    int getIndex = pSkillTextInfo->m_arrayParam[i].m_dw_ParamNum;

                    if(IS_NUMBERIC(pSkillTextInfo->m_arrayParam[i].m_dw_ParamType))
                    {

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                        if (skill_percent_damage > 0 && IsUseSkillPowerAbillity( pSkillTextInfo, i ) == TRUE )
#else // _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
                        // ���ΰ�쿡�� ��ų�ۼ��������� ������� ����
                        if (skill_percent_damage > 0 && pSkillInfo->m_SkillClassCode != eSKILL_HEALING_HAND )   
#endif // _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

                        {
                            Sprintf(parameter_string[i], "%d(+%d)", 
                                                        iTempAbility[getIndex], 
                                                        int(iTempAbility[getIndex] * (skill_percent_damage / 100.0f)));
                        }
                        else
                        {
                            Sprintf(parameter_string[i], "%d", iTempAbility[getIndex]);
                        }
                    }
                    else
                    {
                        if(iTempAbility[getIndex])	// �Ҽ��� ���
                        {
                            Sprintf(parameter_string[i], "%.1f%%", iTempAbility[getIndex] / 10.0f);
                        }
                    }
                }
                else
                {
                    break;
                }
            }

			if(bActivated) 
            {
				textColor = WzCurSkillDescColor;
            }
			else
			{

                if(pSkillInfo->m_wSkillLv > 0)
                {
                    textColor = WzNextSkillDescColor;
                }
			}

            Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, 
                                                        parameter_string[0], 
                                                        parameter_string[1], 
                                                        parameter_string[2], 
                                                        parameter_string[3], 
                                                        parameter_string[4], 
                                                        parameter_string[5], 
                                                        parameter_string[6], 
                                                        parameter_string[7]);

			RegisterTooltipLineChange(szString,
									_tcslen(szString),
									iNormalLineHeight,
									FontID_SkillInfo,
									textColor,
									TOOLTIP_BG_WZCOLOR,
									0);
		}
	}
}


//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillCurLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if(!bActivated) return;

	AddEmptyLineForTooltip();

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	
	g_InterfaceManager.GetInterfaceString( eST_CUR_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, pSkillInfo->m_wSkillLv, pSkillInfo->m_wMaxLv);
	
	RegisterTooltip(szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					WzCurSkillLevelColor,
					TOOLTIP_BG_WZCOLOR,
					0);
}

void uiToolTipMan::_addChangeUpSkillTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if ( pSkillInfo->m_dwClassDefine < 50		// ��ų�� ClassDefine �� 50 �̻��� ��� ��Ű���� ��ȯ�� ��ų
		&& pSkillInfo->m_dwClassDefine > 10		// ü������ ��ų
		&& g_pHero->GetClassExType() < 10 )		// ĳ���Ͱ� ü�������� ���ƴ���
	{
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString( 11468, szString, INTERFACE_STRING_LENGTH );	

		RegisterTooltip(szString,
			-1,
			iNormalLineHeight,
			FontID_SkillInfo,
			WzSkillErrorColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillNextLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if(pSkillInfo->m_wSkillLv > pSkillInfo->m_wMaxLv)
		return;

	AddEmptyLineForTooltip();

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	WzColor textColor = WzSkillDisableColor;

	g_InterfaceManager.GetInterfaceString( eST_NEXT_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	

	if(bActivated)
	{
		textColor = WzNextSkillLevelColor;
	}
	else
	{
        if(pSkillInfo->m_wSkillLv > 0)
        {
            textColor = WzNextSkillLevelColor;
        }
	}
	
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, pSkillInfo->m_wSkillLv, pSkillInfo->m_wMaxLv);
	
	RegisterTooltip(szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					textColor,
					TOOLTIP_BG_WZCOLOR,
					0);

	
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillNeedsForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bNextSkill)
{
	if(!g_pHero)
		return;

    // ��ųƮ�� ������ ���� m_wRequireSkillStat�� �ٸ��뵵�� ���ǰ� �ֱ⶧���� �� �κ��� �н�
    return;
}


//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillLimitWeaponForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot)
{
	if(!g_pHero)
		return;

	if(g_pHero->GetCharInfo()->m_byClassCode == eCHAR_ELEMENTALIST)	//������ ���������� ����.
		return;
	if(g_pHero->GetCharInfo()->m_byClassCode == eCHAR_MYSTIC)	// �̽�ƽ�� ����
		return;

	if(pSkillInfo->m_iWeaponDefine[0] == -1 ) // ��� ����
		return;

	//---------------------------------------------------------------------------------------------
	//		���� ����
	//---------------------------------------------------------------------------------------------
	if( pSkillInfo->m_bySkillUserType != eSKILL_USER_ACTION && pSkillInfo->m_bySkillUserType != eSKILL_USER_EMOTICON )
	{
		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szTemp3[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

		WzColor		WzTextColor = WzSkillErrorColor;


		g_InterfaceManager.GetInterfaceString( eST_LIMIT_WEAPON_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
		
		BOOL	bSetName = FALSE;
		BOOL	bCheck = FALSE;
		BOOL	bEquip = FALSE;

		const int	iWeaponName = 30000;	//	���� �̸� ����
		
		for(int i = 0 ; i < 4 ; ++i )
		{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if(( pSkillInfo->m_iWeaponDefine[i] > 0 && pSkillInfo->m_iWeaponDefine[i] <= eITEMTYPE_ARCBLADE ) || (pSkillInfo->m_iWeaponDefine[i] == -1))
#else
			if(( pSkillInfo->m_iWeaponDefine[i] > 0 && pSkillInfo->m_iWeaponDefine[i] <= eITEMTYPE_PUNCH ) || (pSkillInfo->m_iWeaponDefine[i] == -1))
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			{
				if( i == 0 )
				{
					bSetName = TRUE;

					ZeroMemory(szTemp2, sizeof(szTemp2));
					if(pSkillInfo->m_iWeaponDefine[i] == eITEMTYPE_PUNCH) // �Ǽ�
					{
						//12017	�Ǽ�
						g_InterfaceManager.GetInterfaceString( 12017 , szTemp2 , INTERFACE_STRING_LENGTH);
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp2, INTERFACE_STRING_LENGTH );
					}
				}
				else if( bSetName )
				{
					ZeroMemory(szMessage, sizeof(szMessage));
					if(pSkillInfo->m_iWeaponDefine[i] == eITEMTYPE_PUNCH) // �Ǽ�
					{
						//12017	�Ǽ�
						g_InterfaceManager.GetInterfaceString( 12017 , szMessage , INTERFACE_STRING_LENGTH);
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp3, INTERFACE_STRING_LENGTH );
						Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T(", %s"), szTemp3 );
					}

					_tcscat( szTemp2, szMessage );
				}
				else
				{
					bSetName = TRUE;

					if(pSkillInfo->m_iWeaponDefine[i] == eITEMTYPE_PUNCH) // �Ǽ�
					{
						//12017	�Ǽ�
						g_InterfaceManager.GetInterfaceString( 12017 , szTemp2 , INTERFACE_STRING_LENGTH);
					}
					else
					{
						g_InterfaceManager.GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp2, INTERFACE_STRING_LENGTH );
					}
				}
				bCheck = TRUE;

				if(g_pHero->GetWeaponKind() == pSkillInfo->m_iWeaponDefine[i])
				{
					// ���� ������ ����� ��ġ�ϸ�
					WzTextColor = WzCurSkillColor;
					bEquip = TRUE;
				}
			}
		}
		//	������ ��¾���
		if( bCheck == FALSE )
		{
			return;
		}

		Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
		

		if(!bActivated)
		{
			WzTextColor = WzSkillDisableColor;

            if(pSkillInfo->m_wSkillLv > 0)
            {
                WzTextColor = WzNextSkillColor;
            }
		}

		if(bQuickSlot)
		{
			// ���������� �ִµ� �ƹ��͵� ���������� ���ߴ�
			if(bCheck && (g_pHero->GetWeaponKind() == eITEMTYPE_PUNCH))
			{
				//���� ���� �ʿ�
				g_InterfaceManager.GetInterfaceString( eST_NEED_WEAPON_STRING_INDEX, szString, INTERFACE_STRING_LENGTH );
			}
			else if(bCheck && bEquip)
			{
				return;	// ���� ���� �����ÿ� ǥ�� ����
			}
		}

		RegisterTooltip( szString,
						-1,
						iNormalLineHeight,
						FontID_SkillInfo,
						WzTextColor,
						TOOLTIP_BG_WZCOLOR,
						0);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillCastingTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if(!g_pHero)
		return;

	if( pSkillInfo->m_bySkillType != SKILL_TYPE_PASSIVE )
	{
		WzColor textColor = WzSkillDisableColor;
		if(bActivated) 
			textColor = WzCurSkillColor;
		else
		{
			if(pSkillInfo->m_wSkillLv > 0)
			{
				textColor = WzNextSkillColor;
			}
		}

		int iMyCharClassCode = g_pHero->GetCharInfo()->m_byClassCode;
		if ((eCHAR_ELEMENTALIST == iMyCharClassCode) || 
            (eCHAR_VALKYRIE == iMyCharClassCode) || // ���� or ��Ű���� �����ð� ǥ��(��Ű���� ��ȯ���� �����ð��� ����)
            (eCHAR_MYSTIC == iMyCharClassCode)) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
		{
			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
			TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

			if( pSkillInfo->m_wSkillCasting > 0 )
			{
				int sec = (pSkillInfo->m_wSkillCasting / 1000);
				int point = (pSkillInfo->m_wSkillCasting % 1000);
				if(point)
				{
					point = (point / 100);
					g_InterfaceManager.GetInterfaceString( 12015, szTemp, INTERFACE_STRING_LENGTH);	//�����ð�:
					g_InterfaceManager.GetInterfaceString( 11303, szTemp2, INTERFACE_STRING_LENGTH);	//��
					Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s %d.%d%s"), szTemp, sec, point, szTemp2 );
				}
				else
				{
					g_InterfaceManager.GetInterfaceString( eST_CASTING_TIME_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
					Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, sec );
				}
			}
			else
			{
				g_InterfaceManager.GetInterfaceString( eST_NONE_CASTING_TIME_STRING_INDEX, szString, INTERFACE_STRING_LENGTH);
			}

			RegisterTooltip( szString,
								-1,
								iNormalLineHeight,
								FontID_SkillInfo,
								textColor,
								TOOLTIP_BG_WZCOLOR,
								0);
		}
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillCoolTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)	// ��Ÿ��
{
	WzColor textColor = WzSkillDisableColor;
	if(bActivated) 
	{
		textColor = WzCurSkillColor;
	}
	else
	{
        if(pSkillInfo->m_wSkillLv > 0)
        {
            textColor = WzNextSkillColor;
        }
	}

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	if( pSkillInfo->m_dwCoolTime > 0 )
	{
		int sec = (pSkillInfo->m_dwCoolTime / 1000);
		int point = (pSkillInfo->m_dwCoolTime % 1000);
		if(point)
		{
			point = (point / 100);
			g_InterfaceManager.GetInterfaceString( 12016, szTemp, INTERFACE_STRING_LENGTH);	//��Ÿ��:
			g_InterfaceManager.GetInterfaceString( 11303, szTemp2, INTERFACE_STRING_LENGTH);	//��
			Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s %d.%d%s"), szTemp, sec, point, szTemp2 );
		}
		else
		{
			g_InterfaceManager.GetInterfaceString( 11156, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, sec );
		}
	
		RegisterTooltip( szString,
							-1,
							iNormalLineHeight,
							FontID_SkillInfo,
							textColor,
							TOOLTIP_BG_WZCOLOR,
							0);
	}
}

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillDistanceForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if( pSkillInfo->m_wSkillRange > 0)
	{
		WzColor textColor = WzSkillDisableColor;
		if(bActivated) 
		{
			textColor = WzCurSkillColor;
		}
		else
		{
            if(pSkillInfo->m_wSkillLv > 0)
            {
                textColor = WzNextSkillColor;
            }
		}

		//��ų ��Ÿ�: %d����
		// ��ȿ �Ÿ�: %d ����
		float fRange=((float)pSkillInfo->m_wSkillRange / 10.0f);

		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString( eST_SKILL_RANGE_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);

		if( pSkillInfo->m_wSkillRange % 10 == 0 )
		{
			TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
			Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, (int)fRange );

			RegisterTooltip( szString, -1, iNormalLineHeight, FontID_SkillInfo, textColor, TOOLTIP_BG_WZCOLOR, 0);

		}
		else
		{
			// %d �� %.1f�� �ٲ۴�
			std::string text = szTemp;
			size_t pos = text.find("%d");
			if( pos != 0 )
			{
				text[pos+1] = 'f';
				text.insert(pos+1, ".1");

				TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
				Snprintf(szString, INTERFACE_STRING_LENGTH-1, text.c_str(), fRange );

				RegisterTooltip( szString, -1, iNormalLineHeight, FontID_SkillInfo, textColor, TOOLTIP_BG_WZCOLOR, 0);
			}
		}

	}
	
}	

//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillSpendForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if(!g_pHero)
		return;

	if(pSkillInfo->m_wMPSpend <= 0) return;

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	WzColor textColor = WzSkillDisableColor;
	if(bActivated) 
	{
		textColor = WzCurSkillColor;
	}
	else
	{
        if(pSkillInfo->m_wSkillLv > 0)
        {
            textColor = WzNextSkillColor;
        }
	}

	int iMyCharClassCode = g_pHero->GetCharInfo()->m_byClassCode;
	if(eCHAR_BERSERKER == iMyCharClassCode)
	{
		g_InterfaceManager.GetInterfaceString( eST_SP_SPEND_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
	}
	else
	{
		g_InterfaceManager.GetInterfaceString( eST_MP_SPEND_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
	}
	
	Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s %d"), szTemp, pSkillInfo->m_wMPSpend );
	
	RegisterTooltip( szString,
					-1,
					iNormalLineHeight,
					FontID_SkillInfo,
					textColor,
					TOOLTIP_BG_WZCOLOR,
					0);

}
//------------------------------------------------------------------------------
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void uiToolTipMan::_addSkillFPSpendForTooltip( SkillScriptInfo* pSkillInfo, BOOL bActivated )
{
    if(!g_pHero)
        return;

    BASE_ABILITYINFO* ability_info = NULL;
    ability_info = pSkillInfo->GetAbilityInfoByIndex(0);
    if (!ability_info)
        return;

    if (ability_info->m_wAbilityID != eABILITY_FP_CHANGE &&  
        ability_info->m_wAbilityID != eABILITY_FP_TOGGLE)
        return;

    TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

    WzColor textColor = WzSkillDisableColor;
    if(bActivated) 
    {
        textColor = WzCurSkillColor;
    }
    else
    {
        if(pSkillInfo->m_wSkillLv > 0)
        {
            textColor = WzNextSkillColor;
        }
    }
    
    int need_fp = ability_info->m_iParam[1]; // �Ҹ� FP
    int string_index = 0;
    int param3 = ability_info->m_iParam[3]; // �ֱ� �ð�

    if (param3 > 0)
    {
        // 12115 : %s�� �� FP %s�� �Ҹ�
        string_index = eST_FP_SPEND_TIME_STRING_INDEX;

        g_InterfaceManager.GetInterfaceString( string_index, szTemp, INTERFACE_STRING_LENGTH );
        TCHAR consume_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR fp_string[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR param3_string[INTERFACE_STRING_LENGTH] = {0, };
        if (param3 % 1000 == 0)
        {
            Snprintf(param3_string, INTERFACE_STRING_LENGTH-1, _T("%d"), param3 / 1000);
        }
        else
        {
            Snprintf(param3_string, INTERFACE_STRING_LENGTH-1, _T("%.1f"), (float)param3 / (float)1000);
        }        
        Snprintf(szString, 
            INTERFACE_STRING_LENGTH-1, 
            szTemp, 
            param3_string,
            _itoa(abs(need_fp), fp_string, 10));
    }
    else
    {
        if (need_fp < 0)
        {
            string_index = eST_FP_SPEND_STRING_INDEX;
        }
        else if (need_fp > 0)
        {
            // 12110 : FP ������ :
            string_index = eST_FP_SPEND_CHARGE_STRING_INDEX;
        }

        g_InterfaceManager.GetInterfaceString( string_index, szTemp, INTERFACE_STRING_LENGTH);
        Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s %d"), szTemp, abs(need_fp));
    }

    RegisterTooltip( szString,
        -1,
        iNormalLineHeight,
        FontID_SkillInfo,
        textColor,
        TOOLTIP_BG_WZCOLOR,
        0);
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//------------------------------------------------------------------------------
void uiToolTipMan::_addSkillEffectForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
    // �̽�ƽ��ų ����Ƽ��
    if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_932 )
    {
        // �׶��Ƽ ��ų�� 14621 ���� �����Ѵ�
        // �׶��Ƽ�� ��ų�� 14721 ���� �����Ѵ�
        int zone_skill_code = pSkillInfo->m_SkillCode - 14621 + 14721;
        SkillScriptInfo* pZoneSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( zone_skill_code );
        if( pZoneSkillInfo == NULL )
        {
            return;
        }

        // ü������������������!!
        pSkillInfo = pZoneSkillInfo;
    }

    // �̽�ƽ��ų ��ũ�������Ʈ�� �Ǹ�ȿ���� ��ũ�������Ʈ�� ǥ���ϱ�����
	if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_931 )
	{
		// ��ũ�������Ʈ ��ų�� 14601 ���� �����Ѵ�
		// ��ũ�������Ʈ�� ��ų�� 14711 ���� �����Ѵ�
		int zone_skill_code = pSkillInfo->m_SkillCode - 14601 + 14711;
		SkillScriptInfo* pZoneSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( zone_skill_code );
		if( pZoneSkillInfo == NULL )
		{
			return;
		}

		// ü������������������!!
		pSkillInfo = pZoneSkillInfo;
	}

	//---------------------------------------------------------------------------------------------
	//	�߰� ȿ��
	//---------------------------------------------------------------------------------------------
	WzColor textColor = WzSkillDisableColor;
	if(bActivated)
	{
		textColor = WzCurSkillColor;
	}
	else
	{
        if(pSkillInfo->m_wSkillLv > 0)
        {
            textColor = WzNextSkillColor;
        }
	}

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};


    g_InterfaceManager.GetInterfaceString( eST_SKILL_EFFECT_STRING_INDEX, szText, INTERFACE_STRING_LENGTH );
    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s "), szText );
    util::_tstring strStates = szMessage;

    std::vector<DWORD> viStateCodes;   // �߰� ���� �ڵ��

    BASE_ABILITYINFO * pAbilityInfo = NULL ;
    pSkillInfo->SetFirst();
    while( pAbilityInfo = pSkillInfo->GetNext() )
    {
		if (pAbilityInfo->m_wStateID)
		{
			//if( pAbilityInfo->m_wStateID == eCHAR_STATE_DARK_OF_LIGHT_ZONE )
			//	continue;

			viStateCodes.push_back(pAbilityInfo->m_wStateID);
		}
    }
	std::vector<DWORD>::iterator itr = std::unique<std::vector<DWORD>::iterator>(viStateCodes.begin(), viStateCodes.end());
	viStateCodes.erase(itr,viStateCodes.end());

    int _start, _end;

    _start = 0;
    _end = viStateCodes.size();
    bool bInsertComma = false;  // �׸� ���� , �߰�
    bool bNonStates = true;     // ä�� �׸��� ����

    if (_end)
    {
		TCHAR	szStateName[INTERFACE_STRING_LENGTH] = {0,};

        for (int i=0, j=0; i<_end; ++i)
        {
            int code = viStateCodes[i];
            BASE_STATEINFO *bs = StateInfoParser::Instance()->GetStateInfo(code);
            if (bs)
            {
                if (j)
                {
                    strStates += _T(", ");
                }

				g_InterfaceManager.GetInterfaceString( bs->m_dwNamecode, szStateName, INTERFACE_STRING_LENGTH );

				strStates += szStateName;


                if (bNonStates)
                    bNonStates = false;

                j++;
            }
        }
    }

    if (!bNonStates)
	{
		RegisterTooltipLineChange((TCHAR *)strStates.c_str(),
			_tcslen(strStates.c_str()),
			iNormalLineHeight,
			FontID_SkillInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}

    
}
//------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipSkill(SkillScriptInfo* curSkillInfo,
                                        SkillScriptInfo* nextSkillInfo, 
                                        bool isActivated)
{
	if(!_isEnableShowTooltip()) 
		return;

	if (!curSkillInfo)
    {
        assert ("��ų������ �ݵ�� �ʿ��մϴ�. (prevSkillInfo == NULL)");
        return;
    }

	InitTooltip();

	_addSkillNameForTooltip(curSkillInfo);
	_addSkillTypeForTooltip(curSkillInfo, isActivated);
	_setSkillCurLevInfoForTooltip(curSkillInfo, isActivated);
	if(isActivated)
	{
		_setSkillNextLevInfoForTooltip(nextSkillInfo, isActivated);
	}
	else
	{
		_setSkillNextLevInfoForTooltip(curSkillInfo, isActivated);
	}
	AddEmptyLineForTooltip();
}

//------------------------------------------------------------------------------
void uiToolTipMan::_setSkillBaseInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot)
{
	_addSkillLimitWeaponForTooltip(pSkillInfo, bActivated, bQuickSlot);
	_addSkillCastingTimeForTooltip(pSkillInfo, bActivated);
	_addSkillCoolTimeForTooltip(pSkillInfo, bActivated);
	_addSkillDistanceForTooltip(pSkillInfo, bActivated);
	_addSkillSpendForTooltip(pSkillInfo, bActivated);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    _addSkillFPSpendForTooltip(pSkillInfo, bActivated);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	_addSkillEffectForTooltip(pSkillInfo, bActivated);
}

//----------------------------------------------------------------------------
void uiToolTipMan::_setSkillCurLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	_addSkillEtherBulletUseInfoTooltip(pSkillInfo);

	if(bActivated)
	{
		_addSkillCurLevelForTooltip(pSkillInfo, bActivated);
		_addSkillNeedsForTooltip(pSkillInfo, TRUE, FALSE);		// CurLev�������� �׻� bActivated = TRUE, bNextSkill = FALSE
		_setSkillBaseInfoForTooltip(pSkillInfo, bActivated, FALSE);
		AddEmptyLineForTooltip();
		_addSkillDescForTooltip(pSkillInfo, bActivated);
	}
}

//----------------------------------------------------------------------------
void uiToolTipMan::_setSkillNextLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
{
	if(pSkillInfo)
	{
		_addSkillNextLevelForTooltip(pSkillInfo, bActivated);

        _UpperSkillEnableTooltip(pSkillInfo);

		_addSkillNeedsForTooltip(pSkillInfo, bActivated);
		
		_setSkillBaseInfoForTooltip(pSkillInfo, bActivated, FALSE);
		AddEmptyLineForTooltip();
		_addSkillDescForTooltip(pSkillInfo, bActivated);
	}
}
//----------------------------------------------------------------------------
/**
*/
void uiToolTipMan::_UpperSkillEnableTooltip(SkillScriptInfo* skill_info)
{
    if (skill_info == NULL)
    {
        return;
    }

    if( skill_info->m_UpperSkillClass == INVALID_SKILL_CLASS_CODE )
    {
        return;
    }

    DWORD skill_name = 0;
    if( xGetSkillData()->GetSkillDataSetForTree().CheckSkillLevel(skill_info->m_UpperSkillClass, skill_info->m_RequireUpperSkillLevel, &skill_name) == false )
    {
        if( skill_name == 0 )
        {
            return;
        }

        static TCHAR result_message[INTERFACE_STRING_LENGTH] = {0,};
        static TCHAR need_message[INTERFACE_STRING_LENGTH] = {0,};
        static TCHAR upper_skill[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(2481, need_message, INTERFACE_STRING_LENGTH);
        g_InterfaceManager.GetInterfaceString(skill_name, upper_skill, INTERFACE_STRING_LENGTH);
        Snprintf(result_message, INTERFACE_STRING_LENGTH - 1, need_message, upper_skill, skill_info->m_wRequireSkillStat[0] );
        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_SkillInfo, WzSkillErrorColor,
            TOOLTIP_BG_WZCOLOR, 0);
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiToolTipMan::RegisterTooltipSkillEtc(SkillScriptInfo* pSkillInfo)
{
	if(!_isEnableShowTooltip()) return;

	if ( !pSkillInfo )
	{
		return;
	}

	InitTooltip();

	_addSkillNameForTooltip(pSkillInfo);
	_addSkillNormalDescForTooltip(pSkillInfo);

	AddEmptyLineForTooltip();
}


//-------------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipForWareNInventoryLock(int iStringCode)
{
	if(!_isEnableShowTooltip()) 
		return;

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};

	InitTooltip();

	switch(iStringCode)
	{
	case 5569:
	case 5570:
		{
			//	Ŭ���� ���� â���� ���/���� ���� ������ �����մϴ�. : ������� %s
			g_InterfaceManager.GetInterfaceString( 5568, szTemp, INTERFACE_STRING_LENGTH );

			// ��� || ����
			g_InterfaceManager.GetInterfaceString( iStringCode, szTemp2, INTERFACE_STRING_LENGTH );

			Snprintf(szString, INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
		}
		break;

	default:
		{
			g_InterfaceManager.GetInterfaceString( iStringCode, szString, INTERFACE_STRING_LENGTH );
		}
		break;
	}

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();

	GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg, szString, FontID_12_1, _tcslen(szString), 200);

	for (size_t i=0; i<vstrMsg.size(); ++i)
	{
		TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());

		RegisterTooltip( pStr,
			-1,
			iSimpleTooltipHeight,
			FontID_12_1,
			WzNormalColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}
//-------------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipFormat(TCHAR* format, int base_value, int over_value)
{
    if (_isEnableShowTooltip() == FALSE)
    {
        return;
    }

    InitTooltip();

    TCHAR str[INTERFACE_STRING_LENGTH] = {NULL, };
    Snprintf(str, INTERFACE_STRING_LENGTH, format, base_value, over_value);

    RegisterTooltip(str, -1, iSimpleTooltipHeight, FontID_12_1, WzNormalColor, TOOLTIP_BG_WZCOLOR, 0);
}

//-------------------------------------------------------------------------------------
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
void uiToolTipMan::RegisterTooltipFormat( TCHAR* format, float base_value, float over_value )
{
	if (_isEnableShowTooltip() == FALSE)
	{
		return;
	}

	InitTooltip();

	TCHAR str[INTERFACE_STRING_LENGTH] = {NULL, };
	Snprintf(str, INTERFACE_STRING_LENGTH, format, base_value, over_value);

	RegisterTooltip(str, -1, iSimpleTooltipHeight, FontID_12_1, WzNormalColor, TOOLTIP_BG_WZCOLOR, 0);
}
#endif // _NA_008472_20150903_IMPROVED_BALANCE

//-------------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipWithStringCode(int iStringCode)
{
	if(!_isEnableShowTooltip()) 
		return;


	InitTooltip();
	
	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString( iStringCode, szString, INTERFACE_STRING_LENGTH );

	RegisterTooltip( szString,
					-1,
					iSimpleTooltipHeight,
					FontID_12_1,
					WzNormalColor,
					TOOLTIP_BG_WZCOLOR,
					0);
}


//-------------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipWithStringCodeByProcessWordWrap( int iStringCode, int size )
{
	if(!_isEnableShowTooltip()) 
		return;

	InitTooltip();

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
	g_InterfaceManager.GetInterfaceString( iStringCode, szString, INTERFACE_STRING_LENGTH );

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();

	GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg, szString, FontID_12_1, _tcslen(szString), size );

	for (size_t i=0; i<vstrMsg.size(); ++i)
	{
		TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());

		RegisterTooltip( pStr,
			-1,
			iSimpleTooltipHeight,
			FontID_12_1,
			WzNormalColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
}


//---------------------------------------------------------------------------------
void  uiToolTipMan::RegisterTooltipSimpleText(TCHAR* pszString)
{
	if(!_isEnableShowTooltip()) 
		return;

	InitTooltip();

	RegisterTooltipLineChange( pszString,
					_tcslen(pszString),
					iSimpleTooltipHeight,
					FontID_12_1,
					WzNormalColor,
					TOOLTIP_BG_WZCOLOR,
					0);
}

//---------------------------------------------------------------------------------
void  uiToolTipMan::_registerItemslotInfoTooltip(const SCSlot *pSlot)
{
	if (!pSlot)
	{
		assert(pSlot);
		return;
	}
	
	assert( pSlot->GetSlotType() == ST_ITEM );

	RegisterTooltipItem(pSlot, FALSE);
}

//-----------------------------------------------------------------------------
void uiToolTipMan::_registerItemslotInfoTooltipForQuick(const SCSlot *pSlot)
{
	if (!pSlot) return;

	assert(ST_ITEM == pSlot->GetSlotType() );

	RegisterTooltipItem(pSlot, FALSE);
}

//----------------------------------------------------------------------------------------
void uiToolTipMan::_registerItemslotInfoTooltipForQuickInDeactive(const SCSlot * pSlot)
{
	if (!pSlot)
    {
        return;
    }

	assert(ST_QUICK == pSlot->GetSlotType() );

	if (pSlot->GetSlotType() != ST_QUICK)
	{
		return;
	}

	QuickSlot * quick_slot = (QuickSlot *)(pSlot);

	static SCItemSlot temp_slot;

	temp_slot.SetCode(quick_slot->GetOrgCode());

    if (temp_slot.IsFateItem())
    {
        //Ŭ���̾�Ʈ���� Deactive �������� ���� �ϱ� ���ؼ� ��������.
	    temp_slot.SetFatePeriod(QUICKSLOT_DEACTIVE_FATEITEM_STATUS_VALUE);
    }

	RegisterTooltipItem(&temp_slot, FALSE);
}

//-----------------------------------------------------------------------------------
void uiToolTipMan::_registerQuickslotInfoTooltip(const SCSlot *pSlot)
{
	assert(pSlot);
	assert(ST_QUICK == pSlot->GetSlotType());

	if (!pSlot)
	{
		return;
	}

	QuickSlot * pQuickSlot = (QuickSlot * )pSlot;

	DWORD _dwFlags = 0;

	switch (pQuickSlot->GetOrgSlotType())
	{
	case ST_ITEM:
		{	
			switch (pQuickSlot->GetStatus())
			{
			case SLOT_RENDER_STATE_ACTIVATED:
				{
					SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer(pQuickSlot->GetOrgSlotIndex());
					if (!pFromContainer)
					{
						assert(pFromContainer);
						return;
					}

					SCSlot & rSlot = pFromContainer->GetSlot(pQuickSlot->GetOrgPos());
					if (rSlot.GetCode() != pQuickSlot->GetOrgCode())
						return;

					_registerItemslotInfoTooltipForQuick(&rSlot);
				}
				break;
			case SLOT_RENDER_STATE_DEACTIVATED:
				{
					_registerItemslotInfoTooltipForQuickInDeactive(pQuickSlot);
				}
				break;
			default:
				break;
			}		
		}
		break;

	case ST_SKILL:
		{
			_registerSkillslotInfoTooltip(pQuickSlot->GetOrgSlot(), TRUE);
		}
		break;
	}
}

//---------------------------------------------------------------------------------
void uiToolTipMan::_registerStyleQuickslotInfoTooltip(const SCSlot *pSlot)
{
	assert(pSlot);
	assert(ST_STYLE_QUICK == pSlot->GetSlotType());

	if (!pSlot)
	{
		return;
	}

	StyleSlot * pStyleSlot = (StyleSlot * )pSlot;

	if(pStyleSlot && ST_SKILL == pStyleSlot->GetOrgSlotType())
	{
		BASE_STYLEINFO * pStyleInfo = (BASE_STYLEINFO *)SkillInfoParser::Instance()->GetStyleInfo(pStyleSlot->GetOrgSlot()->GetCode());

		if(pStyleInfo)
		{
			_addStyleNameForTooltip(pStyleInfo);
			_addStyleLevelForTooltip(pStyleInfo, TRUE);

			AddEmptyLineForTooltip();
			_addStyleDescForTooltip(pStyleInfo, TRUE);
		}
	}
	else
	{
		assert(!"�ǵ����� ���� ����̴�.");
	}
}

//------------------------------------------------------------------------------
// Method : RegisterTooltipItem
// Returns :void
// Parameter : const SCSlot * pSlot
// Parameter : BOOL bBuyPrice
// Parameter : LimitStatCompositeType limitstat_composite_type
// [2010-1-26 by i4u4me] ����
//    ���� : iCompositeType(1�̸� ��ũ��, 2�� ��æƮ , 100�̸� �������籸��) �� ���� �� ���� enum ������ ����
//------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipItem(const SCSlot *pSlot, BOOL bBuyPrice, LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/)	
{

	if(!_isEnableShowItemTooltip(pSlot))
		return;

#ifdef _NA_003740_20111122_RANDOM_ITEM
    SCItemSlot* pItemSlot = (SCItemSlot*)pSlot;
    const BASE_ITEMINFO* pItemInfo = pItemSlot->GetItemInfo();
#else
    const BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo( (CODETYPE)pSlot->GetCode() );
    SCItemSlot* pItemSlot = (SCItemSlot*)pSlot;
#endif


	InitTooltip();


	//---------------------------------------------------------------------------------------------
	//		������ �̸�
	//---------------------------------------------------------------------------------------------
	_addItemNameForTooltip(pItemSlot, pItemInfo);

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	//---------------------------------------------------------------------------------------------
	//		���� ����
	//---------------------------------------------------------------------------------------------
	_addItemAwakeningForTooltip(pItemSlot, pItemInfo);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

	bool bFateItemIdentyfy = pItemSlot && pItemSlot->IsFateItem() && pItemSlot->GetFateIdentify();
	if( bFateItemIdentyfy )
	{
		_addFateItemRankForTooltip(pItemSlot, pItemInfo);
	}


	//---------------------------------------------------------------------------------------------
	//		�ͼ� ������
	//---------------------------------------------------------------------------------------------
	_addIPosessionItemTooltip(pItemSlot, pItemInfo);

    //---------------------------------------------------------------------------------------------
    //		 �� ����(������������ϰ��)
    //---------------------------------------------------------------------------------------------
    if (_addItemCustomizingTooltip(pItemSlot, pItemInfo))
    {
        _addPriceTooltip(bBuyPrice, pItemSlot, pItemInfo, limitstat_composite_type);
        return;
    }

    //---------------------------------------------------------------------------------------------
    //		�� ����
    //---------------------------------------------------------------------------------------------
    if(pItemSlot->IsPetItem())
    {
        _addItemPetForToolTip(pItemSlot, pItemInfo);
        return;

    }

	//---------------------------------------------------------------------------------------------
	//		������ ����
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemType(pItemInfo))
    {
		_addItemTypeForTooltip(pItemSlot, pItemInfo);
	}
	//---------------------------------------------------------------------------------------------
	//		�⺻����
	//---------------------------------------------------------------------------------------------
	if(pItemInfo->IsWeapon())
	{
		_addItemEquipClassForTooltip(pItemSlot, pItemInfo);
		_addItemPhyAttackSpeedForTooltip(pItemSlot, pItemInfo);
		_addItemPhyAttackPowerForTooltip(pItemSlot, pItemInfo);
		_addItemMagicAttackPowerForTooltip(pItemSlot, pItemInfo);
        _addItemPhyAttRateForTooltip(pItemSlot, pItemInfo);
		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);
		_addItemLimitedItemRepairCountForTooltip(pItemSlot, pItemInfo);
		_addItemEtherEmissionDevice(pItemSlot, pItemInfo);

		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if(pItemInfo->IsArmor())
	{
		_addItemEquipClassForTooltip(pItemSlot, pItemInfo);


		if( pItemSlot && pItemSlot->IsFateItem() &&!pItemSlot->GetFateIdentify())
		{
			_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);
	
	
			//������ DeActive ������ ���� ȣ������ �ʴ´�.
			if( pItemSlot->GetFatePeriod() != QUICKSLOT_DEACTIVE_FATEITEM_STATUS_VALUE)
			{
				_addFateItemDescriptionForTooltip();
			}
	
			return;
		}


		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
        _addItemPhyAttAvoidForTooltip(pItemSlot, pItemInfo);
		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);

#ifdef _CHINA
		_addItemLimitedItemRepairCountForTooltip(pItemSlot, pItemInfo);
#endif

		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if(pItemInfo->IsSpecialAccessory())
	{
		_addItemEquipClassForTooltip(pItemSlot, pItemInfo);
		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if (pItemInfo->IsRing())
	{
		if ( _addItemEquipClassForTooltip(pItemSlot, pItemInfo))
		{
			AddEmptyLineForTooltip();	// ���� �� �� �߰�
		}		
	}
#ifdef _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	else if( pItemInfo->m_wType == eITEMTYPE_COSTUME || 
		     pItemInfo->m_wType == eITEMTYPE_COSTUME_DECORATE )
#else
    else if( pItemInfo->m_wType == eITEMTYPE_COSTUME )
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    {
        if ( _addItemEquipClassForTooltip(pItemSlot, pItemInfo))
        {
            AddEmptyLineForTooltip();	// ���� �� �� �߰�
        }		
    }
#endif //_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    else if( pItemInfo->m_wType == eITEMTYPE_PCBANG )
    {
        AddEmptyLineForTooltip();
    }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
//_NA_006372_20130108_BADGE_ITEM
    else if (pItemInfo->m_wType == eITEMTYPE_RENDER_EFFECT_ITEM)
    {
        if ( _addItemEquipClassForTooltip(pItemSlot, pItemInfo))
        {
            AddEmptyLineForTooltip();	// ���� �� �� �߰�
        }
    }


	//---------------------------------------------------------------------------------------------
	//		���Ѽ�ġ
	//---------------------------------------------------------------------------------------------
	_addItemLimitInfoForTooltip(pItemSlot, pItemInfo);
	_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);

	//---------------------------------------------------------------------------------------------
	//		�ɼ�
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemOption(pItemInfo))
    {
		_addItemOptionInfoForTooltip(pItemSlot, pItemInfo);
    }

	_addItemTradeInfoForTooltip(pItemSlot, pItemInfo);

	//---------------------------------------------------------------------------------------------
	//      �� ������ �ɼ�
	//---------------------------------------------------------------------------------------------
    _addItemSetOptionForTooltip(pItemInfo);


	//---------------------------------------------------------------------------------------------
	//		�ζ� ������ ����
	//---------------------------------------------------------------------------------------------
	if(pItemSlot->IsLottoItem())
	{
		_addItemLottoItemInfoTooltip(pItemSlot, pItemInfo);

	}


	//---------------------------------------------------------------------------------------------
	//		����
	//---------------------------------------------------------------------------------------------
	_addItemDescForTooltip(pItemSlot, pItemInfo);
    _addPerkCashItemOption(pItemInfo);

    //---------------------------------------------------------------------------------------------
    //		���̴��ν�����
    //---------------------------------------------------------------------------------------------
    if (pItemInfo->IsRiderPartsItem())
    {
        _addItemRiderWaxLevel(pItemSlot);
    }



	_addItemComposeList(pItemInfo);

	_addItemFullSetPlusOptionForTooltip(pItemInfo);

	if (pItemSlot->IsWayPointItem())
	{
		if (pItemInfo->IsChargeItem())
		{
			_addChargeItemInfoForToolTip(pItemInfo);
		}
		//��ǥ ���� ���� ����
		_addWaypointItemTooltip();		

		if (_isEnableChargeItemDesc(pItemSlot, pItemInfo))
        {
			_addChargeItemDescForTooltip(pItemSlot, pItemInfo);		
        }
	}
	else
	{
		if (_isEnableChargeItemDesc(pItemSlot, pItemInfo))
        {
			_addChargeItemDescForTooltip(pItemSlot, pItemInfo);	
        }

		if (pItemInfo->IsChargeItem())
		{
			if (_addChargeItemInfoForToolTip(pItemInfo))
			{
				AddEmptyLineForTooltip();
			}			
		}
	}


    //---------------------------------------------------------------------------------------------
    //		��� ����
    //---------------------------------------------------------------------------------------------
#ifdef _NA_002935_20110704_ITEM_SCORE
    //limitstat_composite_type�� Ư���� ����� �Ű����� �뵵�� �����.
    _addItemScoreForTooltip(pItemSlot, limitstat_composite_type);
#endif //_NA_002935_20110704_ITEM_SCORE

	//---------------------------------------------------------------------------------------------
	//		���� ��� ���� ��ġ
    // iCompositeType: 1�� ��ũ��, 2�� ��æƮ
	//---------------------------------------------------------------------------------------------
    if (limitstat_composite_type == kLimitStatCompositeEnchant)
    {
        _addItemNextGradeLimitInfoForTooltip(pItemSlot, pItemInfo, limitstat_composite_type);
    }

	//---------------------------------------------------------------------------------------------
	//		����
	//---------------------------------------------------------------------------------------------
    _addPriceTooltip(bBuyPrice, pItemSlot, pItemInfo, limitstat_composite_type);
	
    //---------------------------------------------------------------------------------------------
    //		C/P���� ������
    //---------------------------------------------------------------------------------------------
#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    _addItemCashTypeTooltip(pItemSlot, pItemInfo);
#endif//_NA000424_GLOBAL_CASH_ITEM_RENEWAL_
}

//----------------------------------------------------------------------------------
void uiToolTipMan::_addPriceTooltip(BOOL buy_price,
                                    SCItemSlot* item_slot, 
                                    const BASE_ITEMINFO* item_info,
                                    LimitStatCompositeType composite_type)
{
    if (buy_price)	// ���Ű���
    {
        _addItemPriceForTooltip(item_slot, item_info, TRUE);
    }
    else if (g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_SHOP))
    {
        _addItemPriceForTooltip(item_slot, item_info, FALSE, composite_type);
    }
}
//----------------------------------------------------------------------------------
void uiToolTipMan::RegisterTooltipVendorItemUnitPrice(SCSlot *Item_Slot)
{
    if (Item_Slot == NULL)
    {
        return;
    }
    uiSCItemSlot* vendor_Item_slot = static_cast<uiSCItemSlot*>(Item_Slot);

    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR money_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string1[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string2[INTERFACE_STRING_LENGTH] = {0,};
    
    DURATYPE item_num = vendor_Item_slot->GetNum();
    MONEY item_money = vendor_Item_slot->GetMoney();
    MONEY unit_price = item_num != 0 ? (item_money / item_num) : item_money;

    NUMBERFMT num_format = {0, 0, 3, _T("."), _T(","), 1};
    Sprintf(temp_string1, _T("%I64u"), unit_price);
    ::GetNumberFormat(LOCALE_SYSTEM_DEFAULT, NULL, 
                      temp_string1, &num_format, money_string, sizeof(money_string));
    
    g_InterfaceManager.GetInterfaceString(2166, temp_string1);
    g_InterfaceManager.GetInterfaceString(999, temp_string2);
    Sprintf(tooltip_string, _T("%s : %s %s"), temp_string1, money_string, temp_string2);

    RegisterTooltip(tooltip_string, -1, iNormalLineHeight, FontID_ItemInfo, WzNormalColor,
                    TOOLTIP_BG_WZCOLOR, 0);

    AddEmptyLineForTooltip();
}


void  uiToolTipMan::RegisterSlotInfoTooltip(const SCSlot *pSlot)
{
	if (!pSlot)
	{
		assert(0);
		return;
	}
	
	
	switch(pSlot->GetSlotType())
	{
	case ST_ITEM:
		{
			_registerItemslotInfoTooltip(pSlot);
		}
		break;

	case ST_SKILL:	// ��ųâ
		{
			_registerSkillslotInfoTooltip(pSlot, FALSE);	
		}
		break;

	case ST_QUICK:
		{
			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Use );
			_registerQuickslotInfoTooltip(pSlot);	
		}
		break;

	case ST_STYLE_QUICK:
		{
			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Use );
			_registerStyleQuickslotInfoTooltip(pSlot);
		}
		break;
	}

	

}

//----------------------------------------------------------------------------------
void uiToolTipMan::RegisterHeroItemInfoTooltipFromSlot( const SCSlot* slot )
{
	if( !g_pHero )
	{
		return;
	}

	if( !slot || slot->GetSlotType() != ST_ITEM )
	{
		return;
	}

	const SCItemSlot *itemSlot = static_cast<const SCItemSlot*>( slot );

	const BASE_ITEMINFO *itemInfo = ItemInfoParser::Instance()->GetItemInfo( itemSlot->GetCode() );
	assert( itemInfo );

	if( !itemInfo )
	{
		return;
	}
	
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    // Ư�� �Ǽ������� �� ���� ����
    if( itemInfo->IsArmor() || itemInfo->IsWeapon() /*|| (itemSlot && itemSlot->IsWingItem())*/)
#else
    if( itemInfo->IsArmor() || itemInfo->IsWeapon() )
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
	{
		HeroEquipmentContainer* hec = g_pHero->GetHeroEquipContainer();
		assert( hec );

		if( hec->ValidPos( (POSTYPE)itemInfo->m_wEqPos ) )
		{
			SCSlot& equipSlot = hec->GetSlot( (POSTYPE)itemInfo->m_wEqPos );

			RegisterSlotInfoTooltip( &equipSlot );
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiToolTipMan::RegisterItemslotInfoTooltipForShop(const SCSlot *pSlot, DWORD shop_type)
{
	const BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode());
	if (!pItemInfo) return;

    switch (shop_type)
    {
    case kLimitStatCompositeItemPurchased:
        {
            RegisterTooltipItem(pSlot, FALSE, kLimitStatCompositeItemPurchased);  // 100�� ������ �������� �籸�� �Ҷ� �ĺ���
        }
        break;
    case SHOP_TYPE_OF_ITEM: // ������
        {
            RegisterTooltipItem(pSlot, FALSE, kItemPurchasedByItem);  // 102�� ȭ������� �������� ���� �Ҷ� �ĺ���
        }
        break;
    case SHOP_TYPE_OF_ACCUMULATE: // ���� ����Ʈ
        {
            RegisterTooltipItem(pSlot, FALSE, kItemPurchasedByPoint);  // 103�� ��������Ʈ �������� ���� �Ҷ� �ĺ���
        }
        break;
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    case SHOP_TYPE_OF_CURRENCY:
        {
            RegisterTooltipItem(pSlot, FALSE, kItemPurchasedByCurrencyPoint);  // 104�� ȭ������Ʈ �������� ���� �Ҷ� �ĺ���
        }
        break;
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
    default: // SHOP_TYPE_OF_NARMAL �Ϲ�
        {
            RegisterTooltipItem(pSlot, TRUE);
        }
        break;
    }
}


//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::InitTooltip()
{
	this->m_iTooltipCounts = 0;

	ZeroMemory(this->m_Tooltips, (sizeof(cTooltipInfo) * MAX_SLOTINFO_LINE));
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RegisterTooltipLineChange(LPCTSTR szText,
                                            int iBufSize,
                                            int yInterval,
                                            WzID fontID,
                                            WzColor color,
                                            WzColor shadowColor,
											DWORD dwTooltipExtraStyle)
{
    int iCount = 0;
    int iOldCount = 0;
    int iCurSize = 0;

    SIZE TextSize;
    BOOL bDraw = FALSE;
    BOOL bMultiByte = FALSE;
    BOOL bOldMultiByte = FALSE;

    SIZE tooltip_size;
    CalcSize(tooltip_size);
    tooltip_size.cx = max(tooltip_size.cx, (TOOLTIP_TEXT_WIDTH - TOOLTIP_TEXT_WIDTH_INTERVAL));

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( fontID );

    TCHAR szBuf[2];

    while(iCount < iBufSize)
	{
        if( _ismbblead(szText[iCount]))
        {
            szBuf[0] = szText[iCount];
            szBuf[1] = szText[iCount + 1];

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szBuf,2,&TextSize);

            iCurSize += TextSize.cx;

            iCount += 2;	
            bDraw = TRUE;
            bMultiByte = TRUE;
        }
        else		
        {
            if (szText[iCount] == '\\' && 
                szText[iCount+1] == 'n')
            {
                szBuf[0] = szText[iCount];
                szBuf[1] = szText[iCount + 1];
                g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szBuf,2,&TextSize);
                iCurSize += TextSize.cx;
                iCount += 2;
            }
            else
            {
                g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(&szText[iCount],1,&TextSize);
                iCurSize += TextSize.cx;
                iCount++;
            }

            bDraw = TRUE;
            bMultiByte = FALSE;

            
        }

        if( iCurSize >= ( tooltip_size.cx - 25 ) )
        {

           int BackCount =  RegisterTooltip( &szText[iOldCount],
							 (iCount - iOldCount),
							 yInterval,
							 fontID,
							 color,
							 shadowColor,
							 dwTooltipExtraStyle);

		   iCount -= BackCount;
		   iOldCount = iCount;
           bDraw = FALSE;
           iCurSize = 0;
        }

    }

    // �������� �־��ֱ�~
    if (bDraw) {
        RegisterTooltip(&szText[iOldCount],
            -1,
            yInterval,
            fontID,
            color,
            shadowColor,
			dwTooltipExtraStyle);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::CalcSize( SIZE& size )
{
	const int textGap = 20;

    int width = MIN_TOOLTIP_WIDTH;
	int height = TOOLTIP_START_Y;

	int	preWidth = 0;
	int	preYInterval = 0;
	int cellcx = 0;

    for( int i = 0; i < m_iTooltipCounts; ++i )
    {
        if (i >= MAX_SLOTINFO_LINE)
        {
            break;
        }

        cTooltipInfo& tooltipInfo = this->m_Tooltips[i];
        SIZE textSize;
        int textLength;

        if (tooltipInfo.m_eSpecialToolTip & eSPECIAL_TOOLTIP_IMAGE)
        {
            textLength = 0;
            textSize.cx = 0;
            textSize.cy = 0;
            if (BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE) ||
                BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_ABSOLUTE_IMAGE))
            {
                tooltipInfo.m_iTextYInterval = 0;
            }
            else
            {
                textSize.cx = static_cast<int>(tooltipInfo.image_infomation.width);
                textSize.cy = static_cast<int>(tooltipInfo.image_infomation.height);
            }
        }
        else
        {
            g_pSunRenderer->x_pManagerTextOut->StartToUseFont( tooltipInfo.m_fontID );
            textLength = (int)_tcslen( tooltipInfo.m_szSlotInfoMessage );
    //        if( !BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
    //        {
    //            textLength += 3;
    //        }
    //        else
    //        {
    //            if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL1 ) ||
    //                BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL2 ) ||
    //                BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL3 ))
    //            {
    //                textLength += 3;
    //            }
    //        }
            g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( tooltipInfo.m_szSlotInfoMessage, textLength, &textSize );
        }

		if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
		{
			int totWidth = preWidth + textGap + textSize.cx;
			if( width < totWidth )
			{
				width = totWidth;
			}

			if( tooltipInfo.m_iTextYInterval > preYInterval )
			{
				height += ( tooltipInfo.m_iTextYInterval - preYInterval );
				preYInterval = tooltipInfo.m_iTextYInterval;
			}

			if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL1 ))
			{
				cellcx = (cellcx>ColWidth1)?cellcx:ColWidth1;
			}
			else if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL2 ))
			{
				cellcx = (cellcx>ColWidth2)?cellcx:ColWidth2;
			}
			else if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL3 ))
			{
				cellcx = (cellcx>ColWidth3)?cellcx:ColWidth3;
			}
			if(cellcx > 0)
            {
                cellcx -= textGap;
                preWidth = 0;
            }

			preWidth += cellcx + textSize.cx;

			cellcx = 0; // ������ �ʱ�ȭ

			if(width < preWidth)
			{
				width = preWidth;
			}
		}
		else
		{
			if( width < textSize.cx )
			{
				width = textSize.cx;
			}

			preYInterval = tooltipInfo.m_iTextYInterval;
			height += tooltipInfo.m_iTextYInterval;
			preWidth = textSize.cx;
		}
    }

//#ifdef _K2P_WORD_LENGTH_BUG_IN_JAPAN
//    width += 15;
//#endif
	width += 20;

	size.cx = width + 9; 
    size.cy = height;
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RecalcRect( int x, int y, const SIZE& size, BOOL drawBkgnd )
{
	POINT screenpt = g_pApplication->Get2DModeMapping();

    int windowWidth = screenpt.x;
    int windowHeight = screenpt.y - 18;	// �ణ �Ʒ��ʿ� ������ ���̻ڴϱ� ��������.

	int sx, sy;

	if( drawBkgnd )
	{
		sx = x;
		sy = y;
	}
	else
	{
		sx = x - (size.cx / 2);
		sy = y;
	}

    if( sx >= windowWidth - size.cx )
    {
        sx = windowWidth - size.cx; 
    }

    if( sy >= windowHeight - size.cy )
    {
		sy = windowHeight - size.cy; 
    }

	m_rect.left = sx;
	m_rect.top = sy;
	m_rect.right = m_rect.left + size.cx;
	m_rect.bottom = m_rect.top + size.cy;
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RecalcRectFromBase( const RECT& baseRect, const SIZE& size )
{
	const int gap = 2;

	POINT screenpt = g_pApplication->Get2DModeMapping();

    int windowWidth = screenpt.x;
    int windowHeight = screenpt.y - 18;	// �ణ �Ʒ��ʿ� ������ ���̻ڴϱ� ��������.

	int sx, sy;

	if( baseRect.right + size.cx + gap > windowWidth )
	{
		sx = baseRect.left - size.cx - gap;
	}
	else
	{
		sx = baseRect.right + gap;
	}

    if( baseRect.top + size.cy > windowHeight )
    {
		sy = windowHeight - size.cy; 
    }
	else
	{
		sy = baseRect.top;
	}

	m_rect.left = sx;
	m_rect.top = sy;
	m_rect.right = m_rect.left + size.cx;
	m_rect.bottom = m_rect.top + size.cy;
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RenderTooltip( int x, int y, BOOL drawBkgnd )
{
    if( this->m_iTooltipCounts == 0 )
	{
		m_dwIconCode = 0;
		m_hIconOnTooltip = INVALID_HANDLE_VALUE;
        return;
	}

	g_pApplication->Set2DMode();

	SIZE size;
	CalcSize( size );

	RecalcRect( x, y, size, drawBkgnd );

	DrawTooltip( drawBkgnd );

	if( !m_bTooltipTestMode )
	{
		this->InitTooltip();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RenderTooltipEx( const RECT& baseRect, BOOL drawBkgnd )
{
    if( this->m_iTooltipCounts == 0 )
	{
		m_dwIconCode = 0;
		m_hIconOnTooltip = INVALID_HANDLE_VALUE;
        return;
	}

	g_pApplication->Set2DMode();

	SIZE size;
	CalcSize( size );

	RecalcRectFromBase( baseRect, size );

	DrawTooltip( drawBkgnd );

	if( !m_bTooltipTestMode )
	{
		this->InitTooltip();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::DrawTooltip( BOOL drawBkgnd )
{
	if( drawBkgnd )
	{
		cTooltipInfo& tooltipInfo = this->m_Tooltips[0];
		g_InterfaceManager.RenderRect2( m_rect, WzColor_RGBA( 0, 0, 30, 160 ), tooltipInfo.m_eSpecialToolTip );
	}

	RECT txtRect;
    txtRect.left = m_rect.left + 10;
    txtRect.right = m_rect.right - 10;
    txtRect.top = m_rect.top + TOOLTIP_START_Y;
	txtRect.bottom = txtRect.top + this->m_Tooltips[0].m_iTextYInterval;

	//	��ü ���� ����Ʈ�� ���鼭 �Ѱ��� ó��
	int preWidth = 0;
	int preYInterval = 0;
	int cellcx = 0;

    int textLength;
    SIZE textSize;
    BOOL zTest;

    for( int i = 0; i < this->m_iTooltipCounts; ++i )
    {
        cTooltipInfo& tooltipInfo = this->m_Tooltips[i];
        if (tooltipInfo.m_eSpecialToolTip & eSPECIAL_TOOLTIP_IMAGE)
        {
            textLength = 0;
            textSize.cx = 0;
            textSize.cy = 0;

            if (BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE) ||
                BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_ABSOLUTE_IMAGE))
            {
                tooltipInfo.m_iTextYInterval = 0;
            }
            else
            {
                textSize.cx = static_cast<int>(tooltipInfo.image_infomation.width);
                textSize.cy = static_cast<int>(tooltipInfo.image_infomation.height);
            }
        }
        else
        {
            g_pSunRenderer->x_pManagerTextOut->StartToUseFont( tooltipInfo.m_fontID );
            textLength = (int)_tcsclen( tooltipInfo.m_szSlotInfoMessage );

            g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( tooltipInfo.m_szSlotInfoMessage, textLength, &textSize );
        }
        zTest = g_pSunRenderer->IsEnableZTest();


        //	�ؽ�Ʈ ���� ��Ÿ��
		DWORD dwTextStyle = TP_HLEFT;

		if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_CENTER ) )
		{
			dwTextStyle = TP_HCENTER;
		}
		else if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT ) )
		{
			dwTextStyle = TP_HRIGHT;
		}

		//	������ �ٲ��� ����� �ɼ��� ���� ���
        if( i > 0 && BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
		{
			txtRect.top -= preYInterval;
			txtRect.bottom -= preYInterval;
		}

		RECT drawRect = txtRect;

		if( i > 0 && BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) && (dwTextStyle == TP_HLEFT))
		{
			drawRect.left += preWidth;

			if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL1 ))
			{
				cellcx = ColWidth1;
			}
			else if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL2 ))
			{
				cellcx = ColWidth2;
			}
			else if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL3 ))
			{
				cellcx = ColWidth3;
			}

			if(cellcx > 0)
			{
				drawRect.left = txtRect.left + cellcx;
				drawRect.right = txtRect.right + cellcx;
			}
		}
        else
        {
            preWidth = 0;
        }

		drawRect.left -= 1;
		drawRect.right += 1;

        if (tooltipInfo.m_eSpecialToolTip & eSPECIAL_TOOLTIP_EXP_GAUGE)
        {
            float exp_percentage = 0;
            sscanf(tooltipInfo.m_szSlotInfoMessage, "%f", &exp_percentage);
            RenderSpecialTooltipTexture4ExpGauge(&drawRect, exp_percentage);
        }

        g_pSunRenderer->x_pManagerTextOut->DrawText(tooltipInfo.m_szSlotInfoMessage,
													&drawRect,
                                                    tooltipInfo.m_Color,
													WzColor_RGBA( 0, 0, 0, 0),
													(dwTextStyle), 
													ETS_OUTLINE);
        if (tooltipInfo.m_eSpecialToolTip & eSPECIAL_TOOLTIP_IMAGE)
        {
            FRECT position = {(float)drawRect.left + tooltipInfo.image_infomation.x, 
                (float)drawRect.top + tooltipInfo.image_infomation.y, 
                tooltipInfo.image_infomation.width, tooltipInfo.image_infomation.height};

            if (BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_ABSOLUTE_IMAGE))
            {
                position.left = tooltipInfo.image_infomation.x;
                position.top = tooltipInfo.image_infomation.y;
            }
            if (BIT_CHECK(tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT))
            {
                position.left = drawRect.right - position.right;
            }

            this->RenderTexture(tooltipInfo.image_infomation.image_texture_number,
                tooltipInfo.image_infomation.image_frame_number,
                m_TooltipTextureRectAngles[tooltipInfo.image_infomation.image_frame_number], position,
                tooltipInfo.m_dwExtraStyle, tooltipInfo.m_Color);
        }

        g_pSunRenderer->SetZBufferTest( zTest );

		//	������ �ٲ��� ����� �ɼ��� ���� ���
		if( i > 0 && BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
		{
			//	���� ���� ũ��� ���� ���� ũ���� �� ū������ �����ش�
			if( tooltipInfo.m_iTextYInterval > preYInterval )
			{
				preYInterval = tooltipInfo.m_iTextYInterval;
			}
		}
		else
		{
			preYInterval = tooltipInfo.m_iTextYInterval;
		}

		txtRect.top += preYInterval;
		txtRect.bottom += preYInterval;
		
        if( BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL1 ) || 
            BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL2 ) ||
            BIT_CHECK( tooltipInfo.m_dwExtraStyle, c_dwTOOLTIP_EXTRA_CELL3 ))
        {
            preWidth = cellcx + textSize.cx;
        }
        else
        {
            preWidth += cellcx + textSize.cx;
        }
		cellcx = 0;	// ������ �ʱ�ȭ
    }

	for(int i = 0; i < m_iTooltipCounts; ++i)
	{
        if (this->m_Tooltips[i].m_eSpecialToolTip & eSPECIAL_TOOLTIP_NOMAL_ITEM )
        {
            RenderSpecialTooltipTexture4LuckyItem( m_rect );
        }

		/*if( eSPECIAL_TOOLTIP_RANDOM_ITEM & this->m_Tooltips[i].m_eSpecialToolTip )
		{
			RenderSpecialTooltipTexture4RandomItem( m_rect );
		}*/

		if(eSPECIAL_TOOLTIP_ITEM_ON_TT_TEXT & this->m_Tooltips[i].m_eSpecialToolTip)
		{
			float fIconX = (float)m_rect.left + (float)((m_rect.right - m_rect.left)/2) - (ICON_IMAGE_SIZE/2.f);
			float fIconY = (float)m_rect.top + 10.f;
			RenderIconOnTooltip(this->m_Tooltips[i].m_dwIconCode, fIconX, fIconY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void uiToolTipMan::RenderSpecialTooltipTexture4LuckyItem(RECT rc)
{
	//��Ű ������ ���� �ؽ��� ID
	int iTextureID[4] = {200053, 200055, 200052, 200054};

	ENUM_ALPHABLEND eAlphaBlend = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
	BOOL bTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->SetZBufferTest(FALSE);

	for(int i = 0; i < 4; ++i)
	{

		if(INVALID_HANDLE_VALUE == m_hSpecialTooltipTexture4LuckyItem[i])
		{
			RESOURCE_INFO* pInfo = GraphicResourceList::Instance()->GetItemInfo(iTextureID[i]);
			m_hSpecialTooltipTexture4LuckyItem[i] = 
				g_pSunRenderer->x_pManagerTexture->LoadTexture(pInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
		}
		else
		{
			float fx = 0;
			float fy = 0;
			St_TextureContainer *ptc = 
				g_pSunRenderer->x_pManagerTexture->GetTextureInfo( m_hSpecialTooltipTexture4LuckyItem[i] );

            if (ptc == NULL)
            {
                break;
            }

			if(0 == i)
			{
				fx = (float)rc.left - ptc->m_nImageWidth/2 - 1;
				fy = (float)rc.top - ptc->m_nImageHeight/2 - 1;
			}
			else if(1 == i)
			{
				fx = (float)rc.right - ptc->m_nImageWidth/2;
				fy = (float)rc.top - ptc->m_nImageHeight/2 - 1;
			}
			else if(2 == i)
			{
				fx = (float)rc.left - ptc->m_nImageWidth/2 - 1;
				fy = (float)rc.bottom - ptc->m_nImageHeight/2;
			}
			else if(3 == i)
			{
				fx = (float)rc.right - ptc->m_nImageWidth/2;
				fy = (float)rc.bottom - ptc->m_nImageHeight/2;
			}

			g_pSunRenderer->RenderTexture( m_hSpecialTooltipTexture4LuckyItem[i]
											, fx
											, fy
											, (float)ptc->m_nImageWidth
											, (float)ptc->m_nImageHeight);
		}
	}
	g_pSunRenderer->SetZBufferTest(bTest);
	g_pSunRenderer->SetAlphaBlendMode(eAlphaBlend);
}


////////////////////////////////////////////////////////////////////////////
//void uiToolTipMan::RenderSpecialTooltipTexture4RandomItem(RECT rc)
//{
//	//��Ű ������ ���� �ؽ��� ID
//	int iTextureID = {40006};
//	const int R1WIDTH = 20;	// image Width
//	const int R1HEIGHT = 20; // image Height
//	const int iRevision = 7; // ����ũ��� �̹��� �׵θ����� ���� ������
//	static bool bAssert = false;
//
//	ENUM_ALPHABLEND eAlphaBlend = g_pSunRenderer->GetAlphaBlendMode();
//	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
//	BOOL bTest = g_pSunRenderer->IsEnableZTest();
//	g_pSunRenderer->SetZBufferTest(FALSE);
//
//	if(INVALID_HANDLE_VALUE == m_hSpecialTooltipTextureRandomItem)
//	{
//		RESOURCE_INFO* pInfo = GraphicResourceList::Instance()->GetItemInfo(iTextureID);
//		if(pInfo != NULL)
//		{
//			m_hSpecialTooltipTextureRandomItem = 
//				g_pSunRenderer->x_pManagerTexture->LoadTexture(pInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
//		}
//		if(pInfo == NULL || m_hSpecialTooltipTextureRandomItem == INVALID_HANDLE_VALUE)
//		{	// ������������ ���� �ؽ�ó 40006�� ������ ���߽��ϴ�.
//			if(bAssert == false)
//			{
//				assert(!"uiToolTipMan::RenderSpecialTooltipTexture4RandomItem() - Can not to get texture [40006] for RandomItem");
//				bAssert = true;
//			}
//		}		
//	}
//	else
//	{
//		St_TextureContainer *ptc = 
//		g_pSunRenderer->x_pManagerTexture->GetTextureInfo( m_hSpecialTooltipTextureRandomItem );
//
//		if(ptc == NULL)
//		{	// ���� �������� ���� �ؽ�ó������ 40006�� ������ ���߽��ϴ�.
//			if(bAssert == false)
//			{
//				assert(!"uiToolTipMan::RenderSpecialTooltipTexture4RandomItem() - Can not find textureInfo [40006] for RandomItem");
//				bAssert = true;
//			}
//		}
//		else
//		{
//			float fWidth = (float)ptc->m_nImageWidth;
//			float fHeight = (float)ptc->m_nImageHeight;
//			int R2WIDTH = (int)(fWidth - R1WIDTH);
//			int R2HEIGHT = (int)(fHeight - R1HEIGHT);
//			int irectWidth = rc.right - rc.left - R1WIDTH;
//			int irectHeight = rc.bottom - rc.top - R1HEIGHT;	
//			
//			rc.top = rc.top - iRevision;
//			rc.left = rc.left - iRevision;
//			rc.right = rc.right + iRevision;
//			rc.bottom = rc.bottom + iRevision;
//
//            RECT rtViewRect[8]	= {	// (x, y, width, height) �ѷ��� �簢��
//                { rc.left,			  rc.top,				R1WIDTH,	R1HEIGHT },		//������
//                { rc.left,			  (rc.top+R1HEIGHT),	R1WIDTH,	irectHeight },	//��  ��
//                { rc.left,			  (rc.bottom-R1HEIGHT),	R1WIDTH,	R1HEIGHT },		//������
//                { (rc.left+R1WIDTH),  rc.top,				irectWidth, R1HEIGHT },		//������
//                { (rc.left+R1WIDTH),  (rc.bottom-R1HEIGHT), irectWidth, R1HEIGHT },		//������
//                { (rc.right-R1WIDTH), rc.top,				R1WIDTH,    R1HEIGHT },		//������
//                { (rc.right-R1WIDTH), (rc.top+R1HEIGHT),	R1WIDTH,    irectHeight },	//��  ��
//                { (rc.right-R1WIDTH), (rc.bottom-R1HEIGHT),	R1WIDTH,    R1HEIGHT },		//������
//            };
//            RECT rtImgPartRect[8] = {// (x, y, width, height) �о�� �簢��
//                { 0,	   0,		 R1WIDTH, R1HEIGHT },	//������
//                { 0,	   R1HEIGHT, R1WIDTH, 1 },			//��  ��
//                { 0,	   R2HEIGHT, R1WIDTH, R1HEIGHT },	//������
//                { R1WIDTH, 0,		 1,		  R1HEIGHT },	//������
//                { R1WIDTH, R2HEIGHT, 1,		  R1HEIGHT },	//������
//                { R2WIDTH, 0,		 R1WIDTH, R1HEIGHT },	//������
//                { R2WIDTH, R1HEIGHT, R1WIDTH, 1 },			//��  ��
//                { R2WIDTH, R2HEIGHT, R1WIDTH, R1HEIGHT },	//������
//            };
//
//			for(int i = 0; i < 8; ++i)
//			{	
//				g_pSunRenderer->RenderTexture( m_hSpecialTooltipTextureRandomItem
//												, (float)rtViewRect[i].left
//												, (float)rtViewRect[i].top
//												, (float)rtViewRect[i].right
//												, (float)rtViewRect[i].bottom
//												, (float)rtImgPartRect[i].left
//												, (float)rtImgPartRect[i].top
//												, (float)rtImgPartRect[i].right
//												, (float)rtImgPartRect[i].bottom);
//			}
//		}
//	}
//	g_pSunRenderer->SetZBufferTest(bTest);
//	g_pSunRenderer->SetAlphaBlendMode(eAlphaBlend);
//}
//------------------------------------------------------------------------------
/**
*/
void
uiToolTipMan::RegisterTooltip(const cTooltipInfo& tooltipInfo)
{

	if (m_iTooltipCounts >= MAX_SLOTINFO_LINE )
	{
		return;
	}

    this->m_Tooltips[this->m_iTooltipCounts] = tooltipInfo;
    this->m_iTooltipCounts++;
}

//------------------------------------------------------------------------------
/**
*/
int uiToolTipMan::RegisterTooltip(LPCTSTR szText
							  , int iBufSize
							  , int yInterval
							  , WzID fontID
							  , WzColor color
							  , WzColor shadowColor
							  , DWORD dwTooltipExtraStyle
							  , DWORD eSpecialTooltip /*= eSPECIAL_TOOLTIP_NONE*/
							  , DWORD dwIconCode /*= 0*/
							  )
{
    assert(this->m_iTooltipCounts < MAX_SLOTINFO_LINE && this->m_iTooltipCounts >= 0);

	if (!szText)
	{
		return 0;
	}

	int backcount = 0;

	int StrLen = _tcslen(szText);
	if( StrLen == 0 )
		return 0;

	if (m_iTooltipCounts >= MAX_SLOTINFO_LINE )
	{
		return 0;
	}
	int iTooltipCnt = 1;
	int iRowStart[128] = {0,}; //������ ���� �迭 ��ġ.
	for(int i = 0; i < StrLen; ++i)
	{
		if('\\' == szText[i])
		{
			if('n' == szText[i+1])
			{
				iRowStart[iTooltipCnt] = i + 2;//2�� "\n��ŵ�ҷ���"
                ++i;
				iTooltipCnt++;
			}
		}
	}

	if(1 == iTooltipCnt) // ���๮�ڰ� ���ٸ�
	{
		cTooltipInfo* pTooltipInfo = &this->m_Tooltips[this->m_iTooltipCounts];

		ZeroMemory(pTooltipInfo->m_szSlotInfoMessage, MAX_SLOTINFO_LENGTH);

		if (iBufSize == -1)
		{
			StrnCopy(pTooltipInfo->m_szSlotInfoMessage, szText, MAX_SLOTINFO_LENGTH);
		}
		else
		{
  #ifdef	_INTERNATIONAL_UI

			char * tok = " ";

			std::string	str( szText );
			std::string temp( str.substr( 0, iBufSize ) );

            size_t len = temp.length();
            if (len < str.length())
            {
                size_t pos = len;
                if( temp.compare( len - 1, 1, tok ) != 0 )
                {
                    pos = temp.find_last_of( tok, pos );
                    if (pos < len)
                    {
                        backcount = len - pos - 1;
                        temp.erase( pos + 1, len );
                    }
                }
            }

			StrnCopy( pTooltipInfo->m_szSlotInfoMessage, temp.c_str(), MAX_SLOTINFO_LENGTH );
  #else

			StrnCopy( pTooltipInfo->m_szSlotInfoMessage, szText, iBufSize );
  #endif//_INTERNATIONAL_UI
		}

		pTooltipInfo->m_fontID = fontID;
		pTooltipInfo->m_Color = color;
		pTooltipInfo->m_ShadowColor = shadowColor;
		pTooltipInfo->m_iTextYInterval = yInterval;
		pTooltipInfo->m_dwExtraStyle = dwTooltipExtraStyle;
		pTooltipInfo->m_eSpecialToolTip = eSpecialTooltip;
		pTooltipInfo->m_dwIconCode = dwIconCode;

		this->m_iTooltipCounts++;
	}
	else //���๮�ڰ� �ִٸ�
	{
		for(int i = 0; i < iTooltipCnt; ++i)
		{
            if (m_iTooltipCounts >= MAX_SLOTINFO_LINE)
            {
                break;
            }

			cTooltipInfo* pTooltipInfo = &this->m_Tooltips[this->m_iTooltipCounts];
			TCHAR szTextFinal[MAX_SLOTINFO_LENGTH + 1] = {0,};

			ZeroMemory(pTooltipInfo->m_szSlotInfoMessage, MAX_SLOTINFO_LENGTH);

			if(iBufSize == -1)
			{
				if(0 == iRowStart[i + 1])
					StrnCopy(szTextFinal, &szText[iRowStart[i]], StrLen - iRowStart[i]);
				else
					StrnCopy(szTextFinal, &szText[iRowStart[i]], (iRowStart[i + 1] - 2) - iRowStart[i]); //2�� "\n"

				StrnCopy(pTooltipInfo->m_szSlotInfoMessage, szTextFinal, MAX_SLOTINFO_LENGTH);
			}
			else
			{
	#ifdef	_INTERNATIONAL_UI
				char * tok = " ";

				std::string	str( szText );
				std::string temp( str.substr( 0, iBufSize ) );

                size_t len = temp.length();
                if (len < str.length())
                {
                    size_t pos = len;
                    if( temp.compare( len - 1, 1, tok ) != 0 )
                    {
                        pos = temp.find_last_of( tok, pos );
                        if (pos < len)
                        {
                            backcount = len - pos - 1;
                            temp.erase( pos + 1, len );
                        }
                    }
                }

				TCHAR szTemp[MAX_SLOTINFO_LENGTH + 1] = {0,};
				StrnCopy(szTemp, temp.c_str(), MAX_SLOTINFO_LENGTH);
				
				//���ۻ���� �Ѱ�ٸ� ����������.
				if(iBufSize <= iRowStart[i])
					break;

				//gs_bug_fix:������ ���̻����������ʴ´ٸ� break�Ѵ�.
				if(0 == iRowStart[i + 1])
				{
					backcount = iBufSize - iRowStart[i];
					break;
				}
				else
				{
					//gs_bug_fix: ���������� ���ۻ������ ũ�ٸ�  ���߰� backcount���� �����Ѵ�.
					if((iRowStart[i + 1] - 2)>iBufSize)
					{
						//gs_bug_fix:���۰����� �ƴҶ�����
						if(i!=0)
						{
							backcount	 = iBufSize - iRowStart[i];
							break;
						}
						StrnCopy(szTextFinal, &szTemp[iRowStart[i]], iBufSize - iRowStart[i]); //2�� "\n"
					}
					else
					{
						StrnCopy(szTextFinal, &szTemp[iRowStart[i]], (iRowStart[i + 1] - 2) - iRowStart[i]); //2�� "\n"
					}
				}
				StrnCopy(pTooltipInfo->m_szSlotInfoMessage, szTextFinal, MAX_SLOTINFO_LENGTH);
	#else
				//���ۻ���� �Ѱ�ٸ� ����������.
				if(iBufSize <= iRowStart[i])
				{
					
					break;
				}
					
				//gs_bug_fix:������ ���̻����������ʴ´ٸ� break�Ѵ�.
				if(0 == iRowStart[i + 1])
				{
					backcount = iBufSize - iRowStart[i];
					break;
				}
				else
				{
					//gs_bug_fix: ���������� ���ۻ������ ũ�ٸ�  ���߰� backcount���� �����Ѵ�.
					if((iRowStart[i + 1] - 2)>iBufSize)
					{
						//gs_bug_fix:���۰����� �ƴҶ�����
						if(i!=0)
						{
							backcount	 = iBufSize - iRowStart[i];
							break;
						}
						StrnCopy(szTextFinal, &szText[iRowStart[i]], iBufSize - iRowStart[i]); //2�� "\n"
					}
					else
					{
						StrnCopy(szTextFinal, &szText[iRowStart[i]], (iRowStart[i + 1] - 2) - iRowStart[i]); //2�� "\n"
					}
					
				}
				StrnCopy( pTooltipInfo->m_szSlotInfoMessage, szTextFinal, iBufSize );
	#endif//_INTERNATIONAL_UI
			}
			pTooltipInfo->m_fontID = fontID;
			pTooltipInfo->m_Color = color;
			pTooltipInfo->m_ShadowColor = shadowColor;
			pTooltipInfo->m_iTextYInterval = yInterval;
			pTooltipInfo->m_dwExtraStyle = dwTooltipExtraStyle;
			pTooltipInfo->m_eSpecialToolTip = eSpecialTooltip;
			pTooltipInfo->m_dwIconCode = dwIconCode;

			this->m_iTooltipCounts++;
		}
	}


	return backcount;
}

//------------------------------------------------------------------------------
/**
*/
int uiToolTipMan::RegisterTooltipImage(DWORD texture_number, DWORD rectangle_number, float width, float height, 
                                       DWORD extra_style, float x /* = 0.0f */, float y /* = 0.0f */, WzColor color /* = WzColor_RGBA(0,0,0,0) */)// width��, height�� Ȯ���� ũ��
{
    assert(this->m_iTooltipCounts < MAX_SLOTINFO_LINE && this->m_iTooltipCounts >= 0);
    if (m_iTooltipCounts >= MAX_SLOTINFO_LINE)
    {
        return 0;
    }

    cTooltipInfo* pTooltipInfo = &this->m_Tooltips[this->m_iTooltipCounts];

    strcpy(pTooltipInfo->m_szSlotInfoMessage,"");
    pTooltipInfo->m_fontID = StrToWzID("mn12");
    pTooltipInfo->m_Color = color;          // ��Į��//TOOLTIP_BG_WZCOLOR
    pTooltipInfo->m_ShadowColor = 0;    // ���Ŀ� ���İ����� ����ص� �ɵ�
    pTooltipInfo->m_iTextYInterval = (int)height + 3; // �̹����� height�� // 3�� ����
    pTooltipInfo->m_dwExtraStyle = extra_style;
    pTooltipInfo->m_eSpecialToolTip = eSPECIAL_TOOLTIP_IMAGE;
    pTooltipInfo->image_infomation.image_texture_number = texture_number;
    pTooltipInfo->image_infomation.image_frame_number = rectangle_number;
    pTooltipInfo->image_infomation.width = width;
    pTooltipInfo->image_infomation.height = height;
    pTooltipInfo->image_infomation.x = x;
    pTooltipInfo->image_infomation.y = y;
    pTooltipInfo->m_dwIconCode = 0;
    this->m_iTooltipCounts++;

    return 1;//�׸��� ����
}

void uiToolTipMan::RegisterTiredSystemToolTip(int iTime)
{
    int RemainTime = TIMERPARAM->GetMaxTiredTime() - iTime;
	
	if( RemainTime < 0) 
	{
		RemainTime = 0;
	}
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	InitTooltip();
	// 238	�Ƿε�	
	TCHAR szTemp[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString(eST_FATIGUE , szTemp ,INTERFACE_STRING_LENGTH);
	this->RegisterTooltip( szTemp , -1 ,
		iLargeFontHeight ,
		FontID_12 ,
		WzTitleColor ,
		TOOLTIP_BG_WZCOLOR,
		0);

	int iHour , iMinute;

	iHour = RemainTime / 60;
	iMinute = RemainTime % 60;

	// 239	%d�ð� %d�� ���ҽ��ϴ�.
	g_InterfaceManager.GetInterfaceString(eST_REMAIN_FATIGUE_TIME , szTemp ,INTERFACE_STRING_LENGTH);
	Sprintf( szMessage , szTemp , iHour , iMinute );
	RegisterTooltip(szMessage ,
		_tcslen(szMessage) ,
		iNormalLineHeight ,
		FontID_SkillInfo ,
		DEFAULT_FONT_WZCOLOR ,
		TOOLTIP_BG_WZCOLOR ,
		0 );

	AddEmptyLineForTooltip();
}



//------------------------------------------------------------------------------
/**
*/
void uiToolTipMan::RegisterChaoSystemToolTip(int iTime,BOOL bChao)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	InitTooltip();

	//iTime=iTime/1000;

	if(bChao)
	{
		//5909	ī�� ����
		g_InterfaceManager.GetInterfaceString(5909 , szTemp , INTERFACE_STRING_LENGTH);
		this->RegisterTooltip( szTemp , -1 ,
			iLargeFontHeight ,
			FontID_12 ,
			WzTitleColor ,
			TOOLTIP_BG_WZCOLOR,
			0);

	}
	else
	{
		//5910	����ī�� ����
		g_InterfaceManager.GetInterfaceString(5910, szTemp ,INTERFACE_STRING_LENGTH);
		this->RegisterTooltip( szTemp , -1 ,
			iLargeFontHeight ,
			FontID_12 ,
			WzTitleColor ,
			TOOLTIP_BG_WZCOLOR,
			0);
	}

	int iHour , iMinute;

	iHour = iTime / 3600;	
	iMinute = (iTime-iHour*3600)/60;


	// 239	%d�ð� %d�� ���ҽ��ϴ�.
	g_InterfaceManager.GetInterfaceString(eST_REMAIN_FATIGUE_TIME , szTemp ,INTERFACE_STRING_LENGTH);
	Snprintf( szMessage, INTERFACE_STRING_LENGTH-1,  szTemp , iHour , iMinute );
	RegisterTooltip(szMessage ,
		_tcslen(szMessage) ,
		iNormalLineHeight ,
		FontID_SkillInfo ,
		DEFAULT_FONT_WZCOLOR ,
		TOOLTIP_BG_WZCOLOR ,
		0 );
}


//------------------------------------------------------------------------------
/**
*/
void uiToolTipMan::RegisterChannelInfoTooltip(LPCTSTR pszMessage , int iJoinMemeber_Num )
{
	
	TCHAR szMessage[INTERFACE_STRING_LENGTH] , szResult[INTERFACE_STRING_LENGTH];
	InitTooltip();
	// ä�� �� : %s
	g_InterfaceManager.GetInterfaceString(eST_CHANNEL_ROOM_NAME , szMessage , INTERFACE_STRING_LENGTH );
	Sprintf ( szResult , szMessage , pszMessage);
	this->RegisterTooltip(	szResult,
		-1,
		iLargeFontHeight,
		FontID_SkillName,
		c_Chat_ChannelColor,
		TOOLTIP_BG_WZCOLOR,
		0);

	//���� �ο�: %d
	g_InterfaceManager.GetInterfaceString(eST_CHANNEL_JOIN_NUMBER , szMessage , INTERFACE_STRING_LENGTH );
	Sprintf( szResult , szMessage , iJoinMemeber_Num );
	RegisterTooltipLineChange(szResult,
		_tcslen(szResult),
		iNormalLineHeight,
		FontID_SkillInfo,
		c_Chat_ChannelColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}


void uiToolTipMan::RegisterTooltipItemForPackage(LPCTSTR szName, DWORD *dwItemCode)
{
	if(!_isEnableShowTooltip())
		return;

	InitTooltip();

	this->RegisterTooltip(szName,
		-1,
		iLargeFontHeight,
		FontID_ItemName,
		WzTitleColor,
		TOOLTIP_BG_WZCOLOR,
		0);


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    //! ��Ű�� ������ ���� â�� ����
#else
	//---------------------------------------------------------------------------------------------
	//		������ �̸�
	//---------------------------------------------------------------------------------------------
	for(int i = 0; i < MAX_PACKAGE_ITEM_SIZE; ++i)
	{
		const BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode[i]);

		if (!pItemInfo)
			break;

		_addItemNameForTooltip(NULL, pItemInfo);
	}


#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL



}

void uiToolTipMan::RegisterTooltipItemForCashShop(CODETYPE itemCode, 
                                                  DWORD dwCoolTime, 
                                                  DWORD64 SellEndDate, 
                                                  BYTE byEnchant, 
                                                  BYTE byRank,
                                                  bool is_show_date)
{
	if(!_isEnableShowTooltip())
		return;

	const BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(itemCode);
	if (!pItemInfo)
        return;

	InitTooltip();

    // ������ �����۽���
    SCItemSlot* item_slot = NULL; 
    SCItemSlot tooltip_slot;
    item_slot = &tooltip_slot;
    if ((tooltip_slot.GetCode() != itemCode) ||
        (tooltip_slot.GetEnchant() != byEnchant) ||
        (tooltip_slot.GetRank() != byRank))
    {
        tooltip_slot.Clear();
        tooltip_slot.SetCode(SLOTCODE(itemCode));
        const nsSlot::ItemRuleInfo& item_rule_info = tooltip_slot.GetItemRuleInfo();
        if (item_rule_info.IsEnableEnchant()) {
            tooltip_slot.SetEnchant(byEnchant);
        }
        nsSlot::ItemTypeChanger::ChangeItemByType(&tooltip_slot, byRank);
    }
    //---------------------------------------------------------------------------------------------
	//		������ �̸�
	//---------------------------------------------------------------------------------------------
	_addItemNameForTooltip(item_slot, pItemInfo, byEnchant);

	//---------------------------------------------------------------------------------------------
	//		��ũ
	//---------------------------------------------------------------------------------------------
	// ĳ�ü��������� Rank ���� �뵵 �����Ͽ� Rank�� ��� ���� �ʵ��� ����
    // �������� Rank��ü�� ���ǰ� ���� �ʾ���.
    //if(_isEnableItemRankUp(pItemInfo))
	//{
	//	_addItemRankForTooltip(item_slot, pItemInfo, byRank);
	//}

    //---------------------------------------------------------------------------------------------
    //		 �� ����(������������ϰ��)
    //---------------------------------------------------------------------------------------------
    if (_addItemCustomizingTooltip(item_slot, pItemInfo))
    {
        return;
    }

	//---------------------------------------------------------------------------------------------
	//		������ ����
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemType(pItemInfo))
    {
		_addItemTypeForTooltip(item_slot, pItemInfo);
	}


	//---------------------------------------------------------------------------------------------
	//		��ɼ� ����
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemPet(pItemInfo))
	{
		_addItemPetBaseOpForTooltip( pItemInfo,TRUE);
#ifdef __CN_PET_OPTION_APPLY_FLAG
		_addItemPetAddOption(pItemInfo,TRUE);
#else
		_addItemPetAddForTooltip( pItemInfo,TRUE);
#endif//__CN_PET_OPTION_APPLY_FLAG
	}


	//---------------------------------------------------------------------------------------------
	//		�⺻����
	//---------------------------------------------------------------------------------------------
	if(pItemInfo->IsWeapon())
	{
		_addItemEquipClassForTooltip(item_slot, pItemInfo);
		_addItemPhyAttackSpeedForTooltip(item_slot, pItemInfo);
		_addItemPhyAttackPowerForTooltip(item_slot, pItemInfo);
		_addItemMagicAttackPowerForTooltip(item_slot, pItemInfo);
		_addItemDurabilityForTooltip(item_slot, pItemInfo);
		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if(pItemInfo->IsArmor())
	{
		_addItemEquipClassForTooltip(item_slot, pItemInfo);


		if( pItemInfo->m_wSubType == eITEMSUBTYPE_FATE)
		{
			_addItemLimitLevelForTooltip(item_slot, pItemInfo);
			return;
		}

		
		_addItemPhyDefenseForTooltip(item_slot, pItemInfo);
		_addItemMagicDefenseForTooltip(item_slot, pItemInfo);
		_addItemDurabilityForTooltip(item_slot, pItemInfo);
		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if(pItemInfo->IsSpecialAccessory())
	{
		_addItemEquipClassForTooltip(item_slot, pItemInfo);
		_addItemPhyDefenseForTooltip(item_slot, pItemInfo);
		_addItemMagicDefenseForTooltip(item_slot, pItemInfo);
		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}

	//---------------------------------------------------------------------------------------------
	//		���Ѽ�ġ
	//---------------------------------------------------------------------------------------------
	_addItemLimitInfoForTooltip(item_slot, pItemInfo);
	_addItemLimitLevelForTooltip(item_slot, pItemInfo);

	//---------------------------------------------------------------------------------------------
	//		�ɼ�
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemOption(pItemInfo))
		_addItemOptionInfoForTooltip(item_slot, pItemInfo);


    //---------------------------------------------------------------------------------------------
	//		�ŷ�����
	//---------------------------------------------------------------------------------------------
	_addItemTradeInfoForTooltip(item_slot, pItemInfo);
    
    //---------------------------------------------------------------------------------------------
    //��Ʈ �ɼ�
    //---------------------------------------------------------------------------------------------
    _addItemSetOptionForTooltip(pItemInfo);
	
    //---------------------------------------------------------------------------------------------
	//		����
	//---------------------------------------------------------------------------------------------
	_addItemDescForTooltip(item_slot, pItemInfo);

    
    if (is_show_date)
    {
        _addChargeItemInfoForToolTipInCashShop(pItemInfo, dwCoolTime, SellEndDate);
    }
}
//------------------------------------------------------------------------------ 
void uiToolTipMan::RegisterTooltipItemForCashShopForLotto(LPCTSTR name, LPCTSTR desc, LPCTSTR caution, 
                                                          DWORD64 end_date, bool is_show_date /*= true*/)
{
    if (_isEnableShowTooltip() == FALSE)
    {
        return;
    }

    InitTooltip();

    //! �̸� ���
    RegisterTooltip(name, _tcslen(name), iLargeFontHeight, FontID_ItemName, WzNameColor, TOOLTIP_BG_WZCOLOR, 0);
    AddEmptyLineForTooltip();

    //40025	���� ������
    TCHAR msg[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(40025, msg, INTERFACE_STRING_LENGTH);
    RegisterTooltip(msg, _tcslen(msg), iNormalLineHeight, FontID_ItemInfo, WzNormalColor, TOOLTIP_BG_WZCOLOR, 0);

    // ����
    RegisterTooltipLineChange(desc, _tcslen(desc), iNormalLineHeight, FontID_ItemInfo, WzEffectColor, TOOLTIP_BG_WZCOLOR, 0);
    AddEmptyLineForTooltip();

    // ���� ����
    RegisterTooltipLineChange(caution, _tcslen(caution), iNormalLineHeight, FontID_ItemInfo, WzErrorColor, TOOLTIP_BG_WZCOLOR, 0);
    AddEmptyLineForTooltip();

    if (is_show_date)
    {
        _addChargeItemInfoForToolTipInCashShop(NULL, 0, end_date);
    }
}
//------------------------------------------------------------------------------ 
void uiToolTipMan::RegisterTooltip_ItemStatChange(const BASE_ITEMINFO* pItemInfo, SCSlot *pSlot)
{
	InitTooltip();

	SCItemSlot		*pItemSlot = (SCItemSlot *)pSlot;


	//�̸�
	_addItemNameForTooltip(pItemSlot, pItemInfo);

	AddEmptyLineForTooltip();

	if(pItemInfo->IsWeapon())
	{
		_addItemEquipClassForTooltip(pItemSlot, pItemInfo);
		_addItemPhyAttackSpeedForTooltip(pItemSlot, pItemInfo);
		_addItemPhyAttackPowerForTooltip(pItemSlot, pItemInfo);
		_addItemMagicAttackPowerForTooltip(pItemSlot, pItemInfo);
		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);
		_addItemLimitedItemRepairCountForTooltip(pItemSlot, pItemInfo);
		_addItemEtherEmissionDevice(pItemSlot, pItemInfo);

		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}
	else if(pItemInfo->IsArmor())
	{
		_addItemEquipClassForTooltip(pItemSlot, pItemInfo);

		if( pItemSlot && pItemSlot->IsFateItem() &&!pItemSlot->GetFateIdentify())
		{
			_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);

			//������ DeActive ������ ���� ȣ������ �ʴ���.
			if( pItemSlot->GetFatePeriod() != QUICKSLOT_DEACTIVE_FATEITEM_STATUS_VALUE)
			{
				_addFateItemDescriptionForTooltip();
			}

			return;
		}


		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);


#ifdef _CHINA
		_addItemLimitedItemRepairCountForTooltip(pItemSlot, pItemInfo);
#endif

		AddEmptyLineForTooltip();	// ���� �� �� �߰�
	}

	//---------------------------------------------------------------------------------------------
	//		���Ѽ�ġ
	//---------------------------------------------------------------------------------------------
	_addItemLimitInfoForTooltip(pItemSlot, pItemInfo);
	_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);

	//---------------------------------------------------------------------------------------------
	//		�ɼ�
	//---------------------------------------------------------------------------------------------
	if(_isEnableItemOption(pItemInfo))
		_addItemOptionInfoForTooltip(pItemSlot, pItemInfo);

	if(pItemSlot->GetSlotType())
		_addItemTradeInfoForTooltip(pItemSlot, pItemInfo);

	//---------------------------------------------------------------------------------------------
	//      �� ������ �ɼ�
	//---------------------------------------------------------------------------------------------
	_addItemSetOptionForTooltip(pItemInfo);

	//---------------------------------------------------------------------------------------------
	//		����
	//---------------------------------------------------------------------------------------------
	_addItemDescForTooltip(pItemSlot, pItemInfo);


	if (_isEnableChargeItemDesc(pItemSlot, pItemInfo))
		_addChargeItemDescForTooltip(pItemSlot, pItemInfo);



	if(pItemInfo->IsChargeItem())
	{
		if (_addChargeItemInfoForToolTip(pItemInfo))
		{
			AddEmptyLineForTooltip();
		}			
	}
}

void uiToolTipMan::RenderIconOnTooltip(DWORD dwIconCode, float x, float y)
{
	ENUM_ALPHABLEND eAlphaBlend = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
	BOOL bTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->SetZBufferTest(FALSE);

	if ( m_dwIconCode != dwIconCode )
	{
		m_hIconOnTooltip = INVALID_HANDLE_VALUE;
	}

	if(INVALID_HANDLE_VALUE == m_hIconOnTooltip )
	{
		RESOURCE_INFO* pInfo = GraphicResourceList::Instance()->GetItemInfo(dwIconCode);
		if(NULL == pInfo)
		{
			assert(!"uiToolTipMan::RenderIconOnTooltip::(NULL == RESOURCE_INFO)");
			return;
		}

		m_hIconOnTooltip = g_pSunRenderer->x_pManagerTexture->LoadTexture(pInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
		m_dwIconCode = dwIconCode;
	}

	if ( INVALID_HANDLE_VALUE != m_hIconOnTooltip)
	{
		g_pSunRenderer->RenderTexture( m_hIconOnTooltip, 
										x,
										y,
										ICON_IMAGE_SIZE,
										ICON_IMAGE_SIZE,
										0.0f,
										0.0f,
										ICON_IMAGE_SIZE,
										ICON_IMAGE_SIZE);
	}

	g_pSunRenderer->SetZBufferTest(bTest);
	g_pSunRenderer->SetAlphaBlendMode(eAlphaBlend);
}

void uiToolTipMan::RegisterTooltipItem_OnText(const BASE_ITEMINFO* pItemInfo)
{
	InitTooltip();

	AddEmptyLineForTooltip(50);

	_addItemNameForTooltip(NULL, pItemInfo, 0, TRUE);
	_addItemDescForTooltip(NULL, pItemInfo);
}

void uiToolTipMan::RegisterTooltipNPC_OnText(BASE_NPCINFO* pNPCInfo)
{
	InitTooltip();

	if(NULL == pNPCInfo)
		return;
	
	TCHAR szFinal[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1] = {0,};
#ifdef _DH_NPC_GROUP_NAME
    if (pNPCInfo->group_name != 0)
    {
        g_InterfaceManager.GetInterfaceString(pNPCInfo->group_name, szTemp);
        g_InterfaceManager.GetInterfaceString(pNPCInfo->m_NCode, szMessage);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, szMessage, INTERFACE_STRING_LENGTH-1); 
            Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, pNPCInfo->m_NCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
        Snprintf(szFinal, INTERFACE_STRING_LENGTH, "<%s> %s", szTemp, szMessage);
    }
    else
#endif//_DH_NPC_GROUP_NAME
    {
	    g_InterfaceManager.GetInterfaceString(pNPCInfo->m_NCode, szFinal);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, szFinal, INTERFACE_STRING_LENGTH-1); 
            Snprintf(szFinal, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, pNPCInfo->m_MonsterCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
    }

	DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
	sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

	this->RegisterTooltip(	szFinal,
		-1,
		iLargeFontHeight,
		FontID_SkillName,
		WzNormalColor,
		TOOLTIP_BG_WZCOLOR,
		c_dwTOOLTIP_EXTRA_INFO_DT_CENTER,
		sp_tooltip
		);

	AddEmptyLineForTooltip();

	BOOL bGrade = TRUE;
	switch(pNPCInfo->m_byGrade)
	{
	case eNPC_GENERAL: //11501	�Ϲ�
	case eNPC_MAPOBJECT_NPC:
		g_InterfaceManager.GetInterfaceString(11501, szMessage);
		break;
	case eNPC_ELITE: //11503	����
		g_InterfaceManager.GetInterfaceString(11503, szMessage);
		break;
	case eNPC_LEADER: //11502	����
		g_InterfaceManager.GetInterfaceString(11502, szMessage);
		break;
	case eNPC_MIDDLEBOSS: //11504	ĸƾ
		g_InterfaceManager.GetInterfaceString(11504, szMessage);
		break;
	case eNPC_BOSS: //11505	����
		g_InterfaceManager.GetInterfaceString(11505, szMessage);
		break;
	case eNPC_LUCKY_MONSTER: //11500	��Ű
		g_InterfaceManager.GetInterfaceString(11500, szMessage);
		break;

	case eNPC_MERCHANT_NPC:
		bGrade = FALSE;
		break;

	default:
		break;
	}

	if(TRUE == bGrade)
	{
		//11453	���
		g_InterfaceManager.GetInterfaceString(11453, szTemp);
		Sprintf(szFinal, "%s : %s", szTemp, szMessage);

		this->RegisterTooltip(	szFinal,
			-1,
			iLargeFontHeight,
			FontID_SkillName,
			WzEffectColor,
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

		//351	����
		g_InterfaceManager.GetInterfaceString(351, szTemp);
		Sprintf(szFinal, "%s : %d", szTemp, pNPCInfo->m_DisplayLevel);

		this->RegisterTooltip(	szFinal,
			-1,
			iLargeFontHeight,
			FontID_SkillName,
			WzEffectColor,
			TOOLTIP_BG_WZCOLOR,
			c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
	}

    if (TRUE == bGrade) //����
    {
        bool is_complet = false;
        for (int i = 1; i < MAX_QUEST_COMPL_NUM; ++i)
        {
            if (is_complet == true)
            {
                break;
            }
            Quest* quest_pointer = g_pQMGR_Real->FindQuest(i);
            if (NULL == quest_pointer)
            {
                quest_pointer = g_pQMGR_Temp->FindQuest(i);
                if (NULL == quest_pointer)
                    continue;
            }

            for (int k = 0; k < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++k)
            {
                QUEST_CONDITION_KILLMONSTER_INFO* quest_condition_killmonster_info = 
                    (QUEST_CONDITION_KILLMONSTER_INFO*)quest_pointer->GetQuestInfo()->FindCompleteCondition(eQUEST_CONDITION_KILLMONSTER);
                if (NULL == quest_condition_killmonster_info)
                    continue;

                CODETYPE monster_code = quest_condition_killmonster_info->GetMonsterCode(k);
                if (pNPCInfo->m_MonsterCode != monster_code)
                    continue;

                DWORD map_code = quest_pointer->GetKillMonsterMapCode(k);
                const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)map_code);
                if (map_info)
                {
                    g_InterfaceManager.GetInterfaceString(map_info->dwNCode, szMessage);
                    g_InterfaceManager.GetInterfaceString(11454, szTemp);//11454	�������
                    Sprintf(szFinal, "%s : %s", szTemp, szMessage);

                    this->RegisterTooltip(szFinal,
                        -1,
                        iLargeFontHeight,
                        FontID_SkillName,
                        WzEffectColor,
                        TOOLTIP_BG_WZCOLOR,
                        c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

                    is_complet = true;
                    break;
                }
            }
        }
    }
    else //NPC
    {
        EXTRA_NPCINFO* extra_npc_info = NPCInfoParser::Instance()->GetExtraInfo(pNPCInfo->m_MonsterCode);
        if (extra_npc_info == NULL)
        {
            return;
        }

        const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(extra_npc_info->m_MapCode);
        if(pMapInfo)
        {
            g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMessage);
            g_InterfaceManager.GetInterfaceString(11455, szTemp);//11455 ��������

            if (pMapInfo->byMKind == eZONETYPE_MISSION)
            {
                TCHAR mission_string[INTERFACE_STRING_LENGTH + 1] = {0,};
                g_InterfaceManager.GetInterfaceString(2921, mission_string); // 11455 [�̼�]
                Sprintf(szFinal, "%s : %s%s", szTemp, mission_string, szMessage);
            }
            else
            {
                Sprintf(szFinal, "%s : %s", szTemp, szMessage);
            }

            this->RegisterTooltip(szFinal,
                -1,
                iLargeFontHeight,
                FontID_SkillName,
                WzEffectColor,
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);
        }
    }
}

void uiToolTipMan::RegisterTooltip_Reward(const BASE_ITEMINFO* pItemInfo)
{
	InitTooltip();

	_addItemNameForTooltip(NULL, pItemInfo);
	_addItemDescForTooltip(NULL, pItemInfo);
}


void uiToolTipMan::_addFateItemDescriptionForTooltip()
{

	RegisterTooltip(_T("-----------------------------"),
		-1,
		iNormalLineHeight,
		FontID_SkillInfo,
		WzNormalColor,
		TOOLTIP_BG_WZCOLOR,
		0);


	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	//11369	�����ܿ� ���콺�� �÷����� ��Ŭ���� �Ͻø� ����Ʈ�������� ����Ͻ� �� �ֽ��ϴ�.
	g_InterfaceManager.GetInterfaceString(11369 , szMessage);
	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();
	GlobalFunc::ProcessWordWrapByFontSize(&vstrMsg, szMessage, FontID_12_1, _tcslen(szMessage), 140, false);

	for( unsigned int i = 0; i < vstrMsg.size(); ++i )
	{
		RegisterTooltip((TCHAR *)vstrMsg[i].c_str(),
			-1,
			iNormalLineHeight,
			FontID_SkillInfo,
			WzDescriptionColor,
			TOOLTIP_BG_WZCOLOR,
			0);
	}
	AddEmptyLineForTooltip();
}

//------------------------------------------------------------------------------
/** �ۼ�Ʈ����ó�� �ش�ɼ��ڵ尡 �������� TRUE�������մϴ�.
    ��ũ��Ʈ�� TYPE_NUBERIC�̶������� �ۼ�Ʈ�� ������ ���ܿ� ���˴ϴ�.
*/
BOOL uiToolTipMan::_IsexceptionPercentageType( int OptionKind)
{
	switch( OptionKind )
	{
	case 52: //����ý����κ�Ȱ��Ȯ��	
	case 56: //���¹���Ȯ��	
	case 60: //��ųȿ������Ȯ��	
	case 43: //������ �ݻ�
	case 44: //���� ����
	case 45: //����ġ ����
	case 46: //���߰��� Ȯ��
	case 53: //������ �ι� Ȯ��
	case 8:  //�������°��� 
		return TRUE;
	}

	return FALSE;
}


BOOL uiToolTipMan::_registerTooltipFateItem(SCItemSlot * pItemSlot, const BASE_ITEMINFO * pItemInfo,  BOOL bBuyPrice, LimitStatCompositeType limitstat_composite_type /*= kLimitStatCompositeNormal*/)
{
	if( !pItemSlot->IsFateItem() )
		return FALSE;

	InitTooltip();

	//---------------------------------------------------------------------------------------------
	//		������ �̸�
	//---------------------------------------------------------------------------------------------
	_addItemNameForTooltip(pItemSlot, pItemInfo);
	_addItemEquipClassForTooltip(pItemSlot, pItemInfo);
	_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);

	if( !pItemSlot->GetFateIdentify())
	{
		_addFateItemDescriptionForTooltip();
	}
	else
	{
		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);
		_addItemTradeInfoForTooltip(pItemSlot, pItemInfo);
		_addChargeItemDateFixedAmoutForTooltip(pItemSlot, pItemInfo);

	}


	return TRUE;
}

void uiToolTipMan::_addFateItemRankForTooltip(SCItemSlot * pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if(pItemSlot->IsFateItem())
	{
		int FateItemRank = 0;
		sFATE_ITEM_INFO * pFateItemInfo =  FateItemInfoParser::Instance()->GetFateItemInfoByRefItemCode(pItemInfo->m_Code);
		if( pFateItemInfo )
		{
			DWORD dwTargetItemDefRatio = pItemSlot->GetFateAtkDefRatio();
			for( int i=0; i <MAX_FATE_ATK_DEF_RATIO_COUNT; ++i)
			{
				if( pFateItemInfo->m_wAtkDef_Min[i]	<= dwTargetItemDefRatio && pFateItemInfo->m_wAtkDef_Max[i] >= dwTargetItemDefRatio)
				{
					FateItemRank = i;
				}
			}
		}

		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] ={0,};
		WzColor textColor = WzColor_RGB(255,255,255);
		switch( FateItemRank )
		{
		case 0:
			{
				//11373	�ϱ�
				g_InterfaceManager.GetInterfaceString(11373, szInterfaceString);
				textColor = WzColor_RGB(255,255,255);
			}
			break;
		case 1:
			{
				//11375	�߱�
				g_InterfaceManager.GetInterfaceString(11375, szInterfaceString);
				textColor = WzColor_RGB(255,255,0);
			}
			break;
		case 2:
			{
				//11374	���
				g_InterfaceManager.GetInterfaceString(11374, szInterfaceString);
				textColor = WzColor_RGB(255,165,0);
			}
			break;
		case 3:
			{
				//11376	�ֻ��
				g_InterfaceManager.GetInterfaceString(11376, szInterfaceString);
				textColor = WzColor_RGB(255,0,0);
			}
			break;
		}

        AddEmptyLineForTooltip();	// ���� �� �� �߰�
		DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
		RegisterTooltip(szInterfaceString,
			-1,
			iSmallFontHeight,
			FontID_ItemInfo,
			textColor,
			TOOLTIP_BG_WZCOLOR,
			dwTooltipExtraStyle);
	}
}

//------------------------------------------------------------------------------ 
void uiToolTipMan::_addItemPhyAttRateForTooltip(SCItemSlot* item_slot, const BASE_ITEMINFO* item_info)
{
    if ((item_slot == NULL) || (item_info == NULL))
    {
        return;
    }
    if (item_info->m_wPhyAttRate <= 0)
    {
        return;
    }
 
    //! 11306	���� ������
    TCHAR str[INTERFACE_STRING_LENGTH] = {NULL, };
    Sprintf(str, "%d%%", item_info->m_wPhyAttRate);
    _addItemLimitInfoForTooltipMakeStr(item_slot, 11306, str, WzNormalColor);
}
//------------------------------------------------------------------------------ 
void uiToolTipMan::_addItemPhyAttAvoidForTooltip(SCItemSlot* item_slot, const BASE_ITEMINFO* item_info)
{
    if ((item_slot == NULL) || (item_info == NULL))
    {
        return;
    }
    if (item_info->m_wPhyAvoid <= 0)
    {
        return;
    }
 
    //! 11307 ��� ������
    TCHAR str[INTERFACE_STRING_LENGTH] = {NULL, };
    Sprintf(str, "%d%%", item_info->m_wPhyAvoid);
    _addItemLimitInfoForTooltipMakeStr(item_slot, 11307, str, WzNormalColor);

}
//------------------------------------------------------------------------------ 
void uiToolTipMan::_addItemEtherEmissionDevice(SCItemSlot * pItemSlot, const BASE_ITEMINFO * pItemInfo)
{
	if(pItemSlot == NULL  )	
		return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	WzColor textColor = WzNormalColor;
	if( pItemSlot->IsEquipEtherDevice() )
	{
		textColor = WzEffectColor;
		//11464	���׸� ���� ��ġ ����
		Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(11464));
	}
	else
	{
		textColor = WzErrorColor;
		//11465	���׸� ���� ��ġ ������
		Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(11465));
	}


	RegisterTooltip( szInterfaceString,
		-1,
		iNormalLineHeight,
		FontID_ItemInfo,
		textColor,
		TOOLTIP_BG_WZCOLOR,
		0);
}
void uiToolTipMan::_addSkillEtherBulletUseInfoTooltip(SkillScriptInfo* curSkillInfo)
{
    //1101 ���ķ� ��� ����
    //1201 ��Ȱ (��� �Ұ��� ǥ������ �ʵ��� ����)
	if( !curSkillInfo)
		return;

	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
	WzColor textColor = WzNormalColor;
	if( curSkillInfo->m_byEtherBulletInfo)
	{
		textColor = WzEffectColor;
		//11462	���׸� źȯ ��밡��
		Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(11462));

        RegisterTooltip( szInterfaceString,
            -1,
            iNormalLineHeight,
            FontID_ItemInfo,
            textColor,
            TOOLTIP_BG_WZCOLOR,
            0);
	}
}


int uiToolTipMan::ConvertOptValForSpecialCase( int optKind,int opType, int val )
{	
	//�꿡���� optKind(2,5)�϶��� 
	//VALUE_TYPE_PERCENT_PER_CUR or VALUE_TYPE_RATIO_VALUE���� õ�з��� ����
	switch( opType ) 
	{
	case VALUE_TYPE_PERCENT_PER_CUR: //����������
	case VALUE_TYPE_RATIO_VALUE:	 //����ý����κ�Ȱ��Ȯ��	
		{
			val = (int)( val / 10.0f );
		}
		break;
	}

	return val;
}

void uiToolTipMan::_ListUpComposeList()
{
	if (m_pMaterialComposeHash == NULL)
	{
		m_pMaterialComposeHash = new MATERIAL_COMPOSE_HASH;

		m_pMaterialComposeHash->Initialize(131);
		_COMPOSELIST_HASH* pComposeListHash = ItemComposeListParser::Instance()->GetHashTable();

		pComposeListHash->SetFirst();
		sITEM_COMPOSE_LIST*		pComposeInfo = pComposeListHash->GetNext();
		MATERIAL_COMPOSE_LIST*	pMeterial = NULL;
		while (pComposeInfo)//���ո���Ʈ�� �����˻��Ѵ�.
		{
			for (int i = 0; i < pComposeInfo->m_byMatCount; ++i)
			{
				SLOTCODE MatItemCode = pComposeInfo->m_pMaterial[i].m_ItemCode;
				pMeterial = m_pMaterialComposeHash->GetData(MatItemCode);
				if (pMeterial == NULL)
				{
					// ���ο� ���� ���
					pMeterial = new MATERIAL_COMPOSE_LIST;
					m_pMaterialComposeHash->Add(pMeterial, MatItemCode);
					pMeterial->m_MeterialItem = MatItemCode;
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
					pMeterial->m_ComposeList.insert(pComposeInfo->m_pCategoryCode[1]);
#else
                    pMeterial->m_ComposeList.insert(pComposeInfo->m_Category2NCode);
#endif
				}
				else
				{	// ���� ��� - ���Ǵ� ���� ����(set�� �̿��Ͽ� �ߺ��ϰ�� ���ܵ�)
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
					pMeterial->m_ComposeList.insert(pComposeInfo->m_pCategoryCode[1]);	
#else
                    pMeterial->m_ComposeList.insert(pComposeInfo->m_Category2NCode);
#endif
				}
			}
			pComposeInfo = pComposeListHash->GetNext();
		}
	}	
}

void uiToolTipMan::_addItemComposeList(const BASE_ITEMINFO* pItemInfo)
{
	if (m_pMaterialComposeHash)
	{
		MATERIAL_COMPOSE_LIST*	pMeterial = m_pMaterialComposeHash->GetData(pItemInfo->m_Code);
		if (pMeterial)
		{
			TCHAR	szMessage[INTERFACE_STRING_LENGTH]	= {0, };
			TCHAR	szTemp[INTERFACE_STRING_LENGTH]		= {0, };
			TCHAR	szList[INTERFACE_STRING_LENGTH]		= {0, };
			TCHAR	szItem[INTERFACE_STRING_LENGTH]		= {0, };

			g_InterfaceManager.GetInterfaceString(11420, szTemp, INTERFACE_STRING_LENGTH-1);

			COMPOSE_LIST::iterator itr		= pMeterial->m_ComposeList.begin();
			COMPOSE_LIST::iterator itrEnd	= pMeterial->m_ComposeList.end();
			g_InterfaceManager.GetInterfaceString(*(itr++), szItem, INTERFACE_STRING_LENGTH-1);
			strcat(szList, szItem);

#ifdef _DH_TOOLTIP_LIMIT
			int iListCnt = 0;
#endif//_DH_TOOLTIP_LIMIT
			while ( itr != itrEnd)
			{
				g_InterfaceManager.GetInterfaceString(*(itr++), szItem, INTERFACE_STRING_LENGTH-1);
				strcat(szList, _T(", "));
				strcat(szList, szItem);

#ifdef _DH_TOOLTIP_LIMIT
				++iListCnt;

				if(iListCnt >= TOOLTIP_MAXLISTCNT && itr != itrEnd)
				{
					strcat(szList, _T(" ... "));
					break;
				}
#endif//_DH_TOOLTIP_LIMIT
			}
			_snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szList);

			RegisterTooltipLineChange(szMessage, _tcslen(szMessage),
				iNormalLineHeight,
				FontID_ItemInfo,
				WzEffectColor,			
				TOOLTIP_BG_WZCOLOR,
				0);
			AddEmptyLineForTooltip();
		}	
	}	
}

#ifdef _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
void uiToolTipMan::_addItemCashTypeTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO* pItemInfo)
{
    WzColor textColor = WzColor_RGBA(216, 0, 255, 255);
    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0,};

    if (pItemSlot->GetOptionTag().value)// 1 == pŸ��
    {   // 2141 C-Coin
        g_InterfaceManager.GetInterfaceString(2142, tooltip_string, INTERFACE_STRING_LENGTH);
    }
    else if (pItemInfo->m_ChargeType == eCHARGE_CHARGE)// cĳ��
    {   // 2142 P-Coin
        g_InterfaceManager.GetInterfaceString(2141, tooltip_string, INTERFACE_STRING_LENGTH);
    }
    else
    {
        // �Ϲݾ�����
        return;
    }
    this->RegisterTooltip(tooltip_string, -1, iNormalLineHeight, FontID_ItemInfo, textColor,
        TOOLTIP_BG_WZCOLOR, 0);
}
#endif//_NA000424_GLOBAL_CASH_ITEM_RENEWAL_

void uiToolTipMan::_addIPosessionItemTooltip(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo)
{
	if ( pItemSlot->IsPossessionItem() )
	{
		WzColor textColor;
		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};

		if ( pItemSlot->GetIdentifyPossession() )
		{// �ͼ� ������ ������
			textColor = WzErrorColor;
			g_InterfaceManager.GetInterfaceString(11379, szInterfaceString, INTERFACE_STRING_LENGTH);

			this->RegisterTooltip(szInterfaceString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				textColor,
				TOOLTIP_BG_WZCOLOR,
				0);

			// �ŷ�, ������ �Ұ� ���� �߰�
			// ...
			TCHAR szString[INTERFACE_STRING_LENGTH] = {0, };
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0, };
			ZeroMemory(szInterfaceString, sizeof(szInterfaceString));

#ifdef _CHINA
            // �߱� ��ȣ �ɻ�� ���� �Ұ����� �տ� �����ִ� �۾�
            // 1348 : �Ұ���
            g_InterfaceManager.GetInterfaceString(1348, szString, INTERFACE_STRING_LENGTH);
            ZeroMemory(szTemp, sizeof(szTemp));
            Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s "), szString);
            _tcscat( szInterfaceString, szTemp );
#endif // _CHINA
			
			//�Ǹ� �Ұ���
			//�ŷ�		21005 
			g_InterfaceManager.GetInterfaceString(21005, szString, INTERFACE_STRING_LENGTH);

			ZeroMemory(szTemp, sizeof(szTemp));
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szString);
			_tcscat( szInterfaceString, szTemp );

			//�ٴڿ� ������ �Ұ���
			//�ٴڿ� ������	1345
			g_InterfaceManager.GetInterfaceString(1345, szString, INTERFACE_STRING_LENGTH);

			ZeroMemory(szTemp, sizeof(szTemp));
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(", %s"), szString);
			_tcscat( szInterfaceString, szTemp );

#ifdef _CHINA
            // �߱� ��ȣ �ɻ�� ���� �Ұ����� �տ� �����ִ� �۾�
#else
			//�Ұ���		1348
			g_InterfaceManager.GetInterfaceString(1348, szString, INTERFACE_STRING_LENGTH);

			ZeroMemory(szTemp, sizeof(szTemp));
			Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(" %s"), szString);

			_tcscat( szInterfaceString, szTemp );
#endif // _CHINA

			if(pItemSlot && pItemSlot->IsDestroyItem())	// �μ��� �������̸�, tcscat�� �ƴ϶� �����.
			{
				//����, ��� �Ұ���
				g_InterfaceManager.GetInterfaceString(48969, szInterfaceString, INTERFACE_STRING_LENGTH);
			}

			this->RegisterTooltip(szInterfaceString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				textColor,
				TOOLTIP_BG_WZCOLOR,
				0);
		}
		else
		{
			textColor = m_PossessionItemColor;//WzNormalColor(���)
			g_InterfaceManager.GetInterfaceString(11378, szInterfaceString, INTERFACE_STRING_LENGTH);

			this->RegisterTooltip(szInterfaceString,
				-1,
				iNormalLineHeight,
				FontID_ItemInfo,
				textColor,
				TOOLTIP_BG_WZCOLOR,
				0);
		}
		AddEmptyLineForTooltip();
	}
}

void uiToolTipMan::RegisterTooltipMapName(CCtrlStaticWZ* pControl, TCHAR* pszMapName)
{
	InitTooltip();

	this->RegisterTooltip(pszMapName,
		-1,
		iNormalLineHeight,
		pControl->GetFontID(),
		WzNormalColor,
		TOOLTIP_BG_WZCOLOR,
		0);
	AddEmptyLineForTooltip();
}

void uiToolTipMan::LoadTextureExpGauge()
{
    if (m_hExpGaugeTexture == INVALID_HANDLE_VALUE)
    {
        m_hExpGaugeTexture = 
            g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg02.tga", 
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
}

void uiToolTipMan::RenderSpecialTooltipTexture4ExpGauge(RECT* drawRect, float exp_percentage)
{
    if (drawRect == NULL)
    {
        return;
    }

    // ����ġ ������ �ؽ��� ��ǥ
    const float background_gauge_x[3] = {102.5f, 106.5f, 111.5f};
    const float exp_gauge_x[3] = {115.5f, 119.5f, 124.5f};
    const float gauge_y = 693.5f;
    const float gauge_width = 2.0f;
    const float gauge_height = 12.0f;

    // ����ġ ������ ��� ���
    float dest_x = static_cast<float>(drawRect->left);
    float dest_y = static_cast<float>(drawRect->top + 2);
    float dest_width = gauge_width + 1.0f;
    float dest_height = gauge_height + 1.0f;
    // left
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        background_gauge_x[0], gauge_y, gauge_width, gauge_height);
    // center
    dest_x += dest_width;
    dest_width = static_cast<float>(drawRect->right - drawRect->left) - (dest_width * 2);
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        background_gauge_x[1], gauge_y, gauge_width, gauge_height);
    // right
    dest_x += dest_width;
    dest_width = gauge_width;
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        background_gauge_x[2], gauge_y, gauge_width, gauge_height);

    if (exp_percentage <= 0.0f)
    {
        return;
    }

    // ����ġ ������ ���
    dest_x = static_cast<float>(drawRect->left);
    dest_y = static_cast<float>(drawRect->top + 2);
    dest_width = gauge_width;
    dest_height = gauge_height;
    // left
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        exp_gauge_x[0], gauge_y, gauge_width, gauge_height);
    // center
    dest_x += dest_width;
    dest_width = static_cast<float>(drawRect->right - drawRect->left) - (dest_width * 2);
    dest_width = dest_width / 100.0f * exp_percentage;
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        exp_gauge_x[1], gauge_y, gauge_width, gauge_height);
    // right
    dest_x += dest_width;
    dest_width = gauge_width;
    g_pSunRenderer->RenderTexture(m_hExpGaugeTexture, 
        dest_x, dest_y, dest_width, dest_height, 
        exp_gauge_x[2], gauge_y, gauge_width, gauge_height);
}

void uiToolTipMan::RegisterTooltipPerkOptionDesc(int desc_code, int width)
{
    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };

    g_InterfaceManager.GetInterfaceString(desc_code, 
        tooltip_string, 
        INTERFACE_STRING_LENGTH);

    std::vector<util::_tstring> tooltip_string_list;
    tooltip_string_list.clear();

    GlobalFunc::ProcessWordWrapByFontSize(&tooltip_string_list, 
        tooltip_string, 
        FontID_12_1, 
        _tcslen(tooltip_string), 
        width);

    for (size_t i=0; i<tooltip_string_list.size(); ++i)
    {
        TCHAR *pStr = const_cast<TCHAR *>(tooltip_string_list[i].c_str());

        RegisterTooltip( pStr,
            -1,
            iNormalLineHeight,
            FontID_SkillInfo,
            WzNormalColor,
            TOOLTIP_BG_WZCOLOR,
            0);
    }
}

void uiToolTipMan::RegisterTooltipPerkBenefitOption(const PerkInfo* perk_info)
{
    if (perk_info == NULL)
    {
        return;
    }

    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
    bool is_have_option = false;

    BASE_OptionList* option_list = NULL;
    DWORD ontion_type = eATTR_TYPE_INVALID;
    BYTE ontion_value_type = 0;
    DWORD ontion_value = 0;
    for (int index = 0; index < PerkInfo::kNumberOfBenefitOptions; ++index)
    {
        ontion_type = perk_info->benefit_option_[index].item_option_;
        if (ontion_type == eATTR_TYPE_INVALID)
        {
            continue;
        }

        option_list = OptionListParser::Instance()->GetOptionList(ontion_type);
        if (option_list == NULL)
        {
            continue;
        }
        ontion_value_type = perk_info->benefit_option_[index].value_type_;//��ġŸ��
        ontion_value = perk_info->benefit_option_[index].value_;//��

        g_InterfaceManager.GetInterfaceString(option_list->mdwNameCode, 
            temp_string, 
            INTERFACE_STRING_LENGTH);

        Snprintf(tooltip_string, INTERFACE_STRING_LENGTH, _T("%s : %+d"), temp_string, ontion_value);
        //�ɼǸ� ���̻�
        GlobalFunc::AddOptionSuffix(ontion_value_type, option_list, tooltip_string);

        RegisterTooltip(tooltip_string, 
            -1, 
            iNormalLineHeight, 
            FontID_SkillInfo, 
            WzCurSkillLevelColor, 
            TOOLTIP_BG_WZCOLOR, 
            0);
        is_have_option = true;
    }

    if (is_have_option == true)
    {
        AddEmptyLineForTooltip();
    }    
}

void uiToolTipMan::RegisterTooltipPerkPenaltyOption(const PerkInfo* perk_info)
{
    if (perk_info == NULL)
    {
        return;
    }

    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };

    bool is_register_penalty_desc = false;
    bool is_register_duplicate_penalty_desc = false;
    PerkInfo::OptionNode* perk_option_node[3] = {
        const_cast<PerkInfo::OptionNode*>(&perk_info->penalty_2x_option_[0]), 
        const_cast<PerkInfo::OptionNode*>(&perk_info->penalty_3x_option_[0]),
        const_cast<PerkInfo::OptionNode*>(&perk_info->penalty_4x_option_[0])};
    int penalty_string[3] = {6144, 6155, 6156};
    BASE_OptionList* option_list = NULL;
    DWORD ontion_type = eATTR_TYPE_INVALID;
    BYTE ontion_value_type = 0;
    DWORD ontion_value = 0;
    for (int duplicate_index = 0; duplicate_index < 3; ++duplicate_index)
    {
        is_register_duplicate_penalty_desc = false;
        for (int index = 0; index < PerkInfo::kNumberOfPenaltyOptions; ++index)
        {
            ontion_type = perk_option_node[duplicate_index][index].item_option_;
            if (ontion_type == eATTR_TYPE_INVALID)
            {
                continue;
            }

            option_list = OptionListParser::Instance()->GetOptionList(ontion_type);
            if (option_list == NULL)
            {
                continue;
            }

            if (is_register_penalty_desc != true)
            {
                is_register_penalty_desc = true;
                // Perk���ۿ� ����
                RegisterTooltipPerkOptionDesc(perk_info->penalty_description_code_);
                AddEmptyLineForTooltip();
            }

            if (is_register_duplicate_penalty_desc != true)
            {
                is_register_duplicate_penalty_desc = true;
                // �� ��ø �г�Ƽ ��Ʈ���߰�
                g_InterfaceManager.GetInterfaceString(penalty_string[duplicate_index], 
                    tooltip_string, 
                    INTERFACE_STRING_LENGTH);
                RegisterTooltip(tooltip_string, 
                    -1, 
                    iNormalLineHeight, 
                    FontID_SkillInfo, 
                    WzCurSkillDescColor, 
                    TOOLTIP_BG_WZCOLOR, 
                    0);
            }

            ontion_value_type = perk_option_node[duplicate_index][index].value_type_;//��ġŸ��
            ontion_value = perk_option_node[duplicate_index][index].value_;//��

            g_InterfaceManager.GetInterfaceString(option_list->mdwNameCode, 
                temp_string, 
                INTERFACE_STRING_LENGTH);

            Snprintf(tooltip_string, INTERFACE_STRING_LENGTH, _T("%s : %+d"), temp_string, ontion_value);
            //�ɼǸ� ���̻�
            GlobalFunc::AddOptionSuffix(ontion_value_type, option_list, tooltip_string);

            RegisterTooltip(tooltip_string, 
                -1, 
                iNormalLineHeight, 
                FontID_SkillInfo, 
                WzNextSkillLevelColor, 
                TOOLTIP_BG_WZCOLOR, 
                0);
        }
    }

    if (is_register_penalty_desc == true)
    {
        AddEmptyLineForTooltip();
    }    
}

void uiToolTipMan::RegisterTooltipPerkInfo(const SCPerkSlot* perk_slot, const POSTYPE slot_position)
{
    if (perk_slot == NULL)
    {
        return;
    }

    const PerkInfo* perk_info = NULL;
    DWORD64 perk_exp = 0;
    bool is_active_slot = perk_slot->is_active_slot();
    if ((is_active_slot == true) && 
        (perk_slot->GetLinkSlot() != NULL))
    {
        perk_info = perk_slot->GetLinkSlot()->perk_info();
        perk_exp = perk_slot->GetLinkSlot()->GetPerkExp();
    }
    else
    {
        perk_info = perk_slot->perk_info();
        perk_exp = perk_slot->GetPerkExp();
    }
    if (perk_info == NULL)
    {
        // ����ִ� ĳ�ý����� ��� 'ĳ�� ������'(40022) ���� ���
        for (int index = 0; index < PerkDataList::kNumberOfCashSlots; ++index)
        {
            if (slot_position == PerkDataList::kCashSlotPositions[index])
            {
                RegisterTooltipWithStringCode(40022);
                break;
            }
        }
        return;
    }    

    // Perk ����
    // Perk�̸�
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };

    g_InterfaceManager.GetInterfaceString(perk_info->name_code_, 
        tooltip_string, 
        INTERFACE_STRING_LENGTH);
    RegisterTooltip(tooltip_string, 
        -1, 
        iLargeFontHeight, 
        FontID_SkillName, 
        WzCurSkillDescColor, 
        TOOLTIP_BG_WZCOLOR, 
        0);

    // Perk����
    int perk_level = perk_info->perk_id_.level_;
    g_InterfaceManager.GetInterfaceString(eST_LEVEL, temp_string, INTERFACE_STRING_LENGTH);//����
#ifdef _INTERNATIONAL_UI
    Snprintf(tooltip_string, INTERFACE_STRING_LENGTH - 1, _T("%s%d"), temp_string, perk_level);
#else
    Snprintf(tooltip_string, INTERFACE_STRING_LENGTH - 1, _T("%d%s"), perk_level, temp_string);
#endif//_INTERNATIONAL_UI(����������û)
    RegisterTooltip(tooltip_string, 
        -1,
        iNormalLineHeight,
        FontID_SkillInfo,
        WzCurSkillDescColor,
        TOOLTIP_BG_WZCOLOR,
        0);

    bool is_cash_perk = (perk_info->item_code_ != 0);
    PerkID next_perk_id = perk_info->perk_id_;
    next_perk_id.level_ += 1;
    const PerkInfo* next_level_perk_info = PerkInfoParser::Instance()->FindPerkInfo(next_perk_id);

    // Perk����ġ
    // ������ ��� �� �ۼ�Ʈ ��� //- ĳ������ ��� ����ġ �����
    if (is_cash_perk == false)
    {
        double exp_percentage = 0;
        if (next_level_perk_info != NULL)
        {
            DWORD64 next_exp = perk_info->next_level_exp_;
            exp_percentage = (static_cast<double>(perk_exp - perk_info->exp_value_) / next_exp * 100.f);
            exp_percentage = min(exp_percentage, 99.99f);
        }
        
        Snprintf(tooltip_string, INTERFACE_STRING_LENGTH - 1, _T("%.2f%%"), exp_percentage);
        RegisterTooltip(tooltip_string, 
            -1,
            iNormalLineHeight,
            FontID_SkillInfo,
            WzCurSkillDescColor,
            TOOLTIP_BG_WZCOLOR,
            c_dwTOOLTIP_EXTRA_INFO_DT_CENTER, 
            eSPECIAL_TOOLTIP_NOMAL_ITEM | eSPECIAL_TOOLTIP_EXP_GAUGE);
    }
    AddEmptyLineForTooltip();

    // Perkȿ�� ����
    RegisterTooltipPerkOptionDesc(perk_info->option_description_code_);

    AddEmptyLineForTooltip();

    // ���� ���� Perkȿ�� �ɼ�
    // 6298 ���� ���� �ɼ�:
    if (next_level_perk_info != NULL)
    {
        g_InterfaceManager.GetInterfaceString(6298, tooltip_string, INTERFACE_STRING_LENGTH);
        RegisterTooltip(tooltip_string, 
            -1,
            iNormalLineHeight,
            FontID_SkillInfo,
            WzCurSkillDescColor,
            TOOLTIP_BG_WZCOLOR,
            0);
    }    
    RegisterTooltipPerkBenefitOption(perk_info);

    // ���� ���� Perkȿ�� �ɼ�
    // 6299 ���� ���� �ɼ�:
    if (next_level_perk_info != NULL)
    {
        g_InterfaceManager.GetInterfaceString(6299, tooltip_string, INTERFACE_STRING_LENGTH);
        RegisterTooltip(tooltip_string, 
            -1,
            iNormalLineHeight,
            FontID_SkillInfo,
            WzCurSkillDescColor,
            TOOLTIP_BG_WZCOLOR,
            0);
        RegisterTooltipPerkBenefitOption(next_level_perk_info);
    }

    // Perk���ۿ� �ɼ�
    RegisterTooltipPerkPenaltyOption(perk_info);

    // ĳ���ܽ����� ��� ���ᳯ¥ ���
    if (is_cash_perk == true)
    {
        SYSTEMTIME systime;
        DWORD64 date_time = perk_exp;
        util::YYYYMMDDHHMMSSToSYSTEMTIME(date_time, systime);

        // 5874   %4d �� %2d�� %2d�� %2d�� %2d�� ���� 
        GlobalFunc::GetDateString(tooltip_string, INTERFACE_STRING_LENGTH, 5874, systime);

        RegisterTooltipLineChange(tooltip_string, _tcslen(tooltip_string),
            iNormalLineHeight,
            FontID_ItemInfo,
            WzChargeItemDateColor,
            TOOLTIP_BG_WZCOLOR,
            0);

        AddEmptyLineForTooltip();
    }
}

void uiToolTipMan::_addPerkCashItemOption(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return;
    }

    const PerkInfo* perk_info = PerkInfoParser::Instance()->FindPerkInfo(item_info->m_Code);
    if (perk_info == NULL)
    {
        return;
    }

    RegisterTooltipPerkOptionDesc(perk_info->option_description_code_);
    AddEmptyLineForTooltip();
    RegisterTooltipPerkBenefitOption(perk_info);
    RegisterTooltipPerkPenaltyOption(perk_info);
}

//------------------------------------------------------------------------------
void uiToolTipMan::InitTextures()
{
    // �ִϸ��̼� �ؽ��� �ε�
    m_hTooltipTexture[kTooltipTexture_Socket] = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\Socket_tip.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

#ifdef _NA_003740_20111122_RANDOM_ITEM
    m_hTooltipTexture[kTooltipTexture_RandomItemGrade] = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\Common_bg01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    m_hTooltipTexture[kTooltipTexture_PartyObtain] = g_pSunRenderer->x_pManagerTexture->LoadTexture(
        "Data\\Interface\\all_split01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

    float texture_frames[kTexture_Rectangles_Max][4] = 
    {   // x, y, width, height ������ ����
        {0.0f, 90.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket0, Socket��Ȯ�� �ؽ��� [?]
        {0.0f, 72.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket1, Socket1�� �ؽ��� [1S]
        {0.0f, 54.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket2, Socket2�� �ؽ��� [2S]
        {0.0f, 36.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket3, Socket3�� �ؽ��� [3S]
        {0.0f, 18.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket4, Socket4�� �ؽ��� [4S]
        {0.0f, 0.0f, 67.0f, 18.0f},// kTexture_Rectangle_Socket5, Socket5�� �ؽ��� [5S]
        {88.0f, 0.0f, 22.0f, 22.0f},// kTexture_Rectangle_SocketEmpty, Socket �� ���� �ؽ���
        {0.0f, 0.0f, 32.0f, 32.0f},// kTexture_Rectangle_SocketItem, Socket ������ �ؽ���
#ifdef _NA_003740_20111122_RANDOM_ITEM
        {840.0f, 44.0f, 22.0f, 22.0f}, // kTexture_Ractangle_RandomOptionGrade0,
        {840.0f, 0.0f, 22.0f, 22.0f},  // kTexture_Ractangle_RandomOptionGrade1,
        {862.0f, 0.0f, 22.0f, 22.0f},  // kTexture_Ractangle_RandomOptionGrade2,
        {884.0f, 0.0f, 22.0f, 22.0f},  // kTexture_Ractangle_RandomOptionGrade3,
        {840.0f, 22.0f, 22.0f, 22.0f}, // kTexture_Ractangle_RandomOptionGrade4,
        {862.0f, 22.0f, 22.0f, 22.0f}, // kTexture_Ractangle_RandomOptionGrade5,
        {884.0f, 22.0f, 22.0f, 22.0f}, // kTexture_Ractangle_RandomOptionGrade6,
#endif _NA_003740_20111122_RANDOM_ITEM
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
        {474.0f, 0.0f, 32.0f, 32.0f},// kTexture_Rectangle_PartyObtain, ��κй� �ؽ���
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

    };

    // �ؽ��� �ڸ��� �� ��ġ ���� Rect ����
    for (int i = kTexture_Rectangles_Start; i < kTexture_Rectangles_Max; ++i)
    {
        m_TooltipTextureRectAngles[i].left = texture_frames[i][0];
        m_TooltipTextureRectAngles[i].top = texture_frames[i][1];
        m_TooltipTextureRectAngles[i].right = texture_frames[i][2];
        m_TooltipTextureRectAngles[i].bottom = texture_frames[i][3];
    }
    m_hTooltipTexture[kTooltipTexture_Handle] = INVALID_HANDLE_VALUE;
}


void uiToolTipMan::RenderTexture(DWORD texture_number, DWORD rectangle_number, 
                                 FRECT size, FRECT position, DWORD extra_style, WzColor color)// �Ѱ��� �ؽ��� �׸���
{
    if (rectangle_number < kTexture_Rectangles_Start || rectangle_number >= kTexture_Rectangles_Max)
    {
        return; // ���� ������ ���� �ʽ��ϴ�.
    }

    if (BIT_CHECK(extra_style, c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE))
    {
        RESOURCE_INFO* image_Info = 
            GraphicResourceList::Instance()->GetItemInfo(texture_number);

        if (image_Info)
        {
            m_hTooltipTexture[kTooltipTexture_Handle] = 
                // TODO: ��� �ε� �ؽ��� �ϸ� ���ҽ� ī��Ʈ�� �ö󰡰� �� �ذ� �ʿ�.!!!!
                g_pSunRenderer->x_pManagerTexture->LoadTexture(image_Info->szFilename, 
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
            texture_number = kTooltipTexture_Handle;
        }
    }
    else if (texture_number < kTooltipTexture_Start || texture_number >= kTooltipTexture_Max)
    {
        return; // �ؽ��� ������ ���� �ʽ��ϴ�.
    }

    WzColor* add_colors = NULL;
    WzColor colors[8] = {0,};

    if (color != WzColor_RGBA(0,0,0,0))
    {
        for (int i = 0; i < 8; ++i)
        {
            colors[i] = color;
        }
        add_colors = colors;
    }

    if (m_hTooltipTexture[texture_number] != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->RenderTexture(m_hTooltipTexture[texture_number],
        position.left, position.top, position.right, position.bottom,
        size.left, size.top, size.right, size.bottom, add_colors);
    }
}

//------------------------------------------------------------------------------ 
WzColor uiToolTipMan::_GetSimulationLimitedInfoColor(eATTR_TYPE attr_type, 
                                                    SCItemSlot* item_slot, 
                                                    PlayerAttributes* player_attributes, 
                                                    WORD limit_item_value,
                                                    WORD limit_value,
                                                    WzColor default_text_color)
{
    int attr_value = 0;
    switch (attr_type)
    {
        case eATTR_STR: attr_value = player_attributes->GetSTR(); break;
        case eATTR_DEX: attr_value = player_attributes->GetDEX(); break;
        case eATTR_VIT: attr_value = player_attributes->GetVIT(); break;
        case eATTR_INT: attr_value = player_attributes->GetINT(); break;
        case eATTR_SPR: attr_value = player_attributes->GetSPR(); break;
        default:    return default_text_color;
    }
    
    if (item_slot && item_slot->IsExpiredWaitTime())
    {
        //default_text_color = WzWaitItemColor;
    }
    else if(item_slot && item_slot->IsLimited())
    {
        if (attr_value < limit_item_value)
        {
            //default_text_color = WzErrorColor;
        }
    }
    else if (attr_value < limit_value)
    {
        if (attr_value + player_attributes->GetDecreaseLimitStat() < limit_value)
        {
            //default_text_color = WzErrorColor; 
        }
        else
        {
            // ���Ѽ�ġ ���ҿɼǿ� ���� ���밡��
            //default_text_color = WzEffectColor; 
        }
    }
    else
    {
        default_text_color = WzColor_RGBA(20,255,20,255);
    }

    return default_text_color;
}

#ifdef _DH_BATTLEZONE2_
void uiToolTipMan::RegisterTooltipBattlezoneMap(const BattlezoneMissionTreeInfo* mission_info)
{
    if (_isEnableShowTooltip() == false)
    {
        return;
    }

    InitTooltip();

    //------------------------------------------------------------------------------ 
    TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	str_format[INTERFACE_STRING_LENGTH]={0,};
    static WzColor name_color = WzColor_RGBA(207, 207, 0, 255);
    static WzColor discription_color = WzColor_RGBA(255, 255, 255, 255);
    static WzColor green = WzColor_RGBA(0, 207, 0, 255);
    static WzColor orange = WzColor_RGBA(255, 187, 0, 255);

    //�̸�
    g_InterfaceManager.GetInterfaceString(mission_info->mission_string_code, szText, INTERFACE_STRING_LENGTH );
    RegisterTooltip(szText, -1, iNameLineHeight, FontID_ItemName, name_color, TOOLTIP_BG_WZCOLOR, 0);

#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    if (mission_info->item_obtain_ == kObtainType_Enable)
    {
        RegisterTooltipImage(kTooltipTexture_PartyObtain,
            kTexture_Rectangle_PartyObtain,
            16,
            16,
            c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT);
    }
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN

    AddEmptyLineForTooltip();

    //����
    if (mission_info->tooltip_string_code != 0)
    {
        g_InterfaceManager.GetInterfaceString(mission_info->tooltip_string_code, szText, INTERFACE_STRING_LENGTH );

        ////////////////
        util::_tstring strMapDesc = szText;

        const TCHAR* cstrMapDesc;
        util::_tstring tempstrMapDescSpatial;
        std::vector<util::_tstring> strMapDescSpatial;
        cstrMapDesc = strMapDesc.c_str();
        tempstrMapDescSpatial.clear();
        for (size_t strIter = 0; strIter<strMapDesc.size(); ++strIter)
        {
            if (cstrMapDesc[strIter] == _T('#'))
            {
                strMapDescSpatial.push_back(tempstrMapDescSpatial);
                tempstrMapDescSpatial.clear();
            }
            else
            {
                tempstrMapDescSpatial += strMapDesc[strIter];
            }
        }

        if (tempstrMapDescSpatial.size())
        {
            strMapDescSpatial.push_back(tempstrMapDescSpatial);
            tempstrMapDescSpatial.clear();
        }

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        if (strMapDescSpatial.size())
        {
            for (size_t i=0; i<strMapDescSpatial.size(); ++i)
            {
                sprintf(string, strMapDescSpatial[i].c_str());
                RegisterTooltipLineChange(string, _tcslen(string), 
                    iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
            }
        }
        else
        {
            sprintf(string, strMapDesc.c_str());
            RegisterTooltipLineChange(string, _tcslen(string), iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
        }
        /////////////////

        //RegisterTooltipLineChange(szText, _tcslen(szText), iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
        AddEmptyLineForTooltip();
    }

    //���尡�� ����
    const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)mission_info->map_code);
    if (mapInfo)
    {
        // 1780 ���巹��
        g_InterfaceManager.GetInterfaceString(1780, szText, INTERFACE_STRING_LENGTH );
        sprintf(str_format, "%s: %d-%d", szText, mapInfo->minLV, mapInfo->maxLV);
        RegisterTooltip(str_format, -1, iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
    }

    //���� ����
    // 646	���巹��
    g_InterfaceManager.GetInterfaceString(646, szText, INTERFACE_STRING_LENGTH );
    sprintf(str_format, "%s: %d-%d", szText, mission_info->min_level, mission_info->max_level);
    RegisterTooltip(str_format, -1, iNameLineHeight, FontID_ItemName, orange, TOOLTIP_BG_WZCOLOR, 0);

    //������� �����ð�
    if (mission_info->kind_of_room == PartyOfTopOfTrialByNormal
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
        || (mission_info->kind_of_room == PartyOfTopOfTrialLight_normal)
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
        )

    {
        AddEmptyLineForTooltip();
        //11352	(������� ���� �ð� : %d�ð� %d�� %d��)
        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(mission_info->require_item[0]);//����ð��� 0���� ����
        if (item_info)
        {
            _addItemAdmissionTimeForToolTip(item_info);
        }
    }

    //������ �ʿ��� ������
    bool is_have_require_item = false;
    int find_item_number = 0;

    const BASE_ITEMINFO* item_info = NULL;
    WzColor find_color = WzColor_RGBA(0,0,0,0);

    for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
    {
        if (mission_info->require_item[i] != 0)
        {
            item_info = ItemInfoParser::Instance()->GetItemInfo(mission_info->require_item[i]);
            if (item_info)
            {
                //������ ã��
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, static_cast<SLOTCODE>(mission_info->require_item[i]));
                if (item_slot && item_slot->GetNum() > 0)
                {
                    //nop
                    is_have_require_item = true;
                    find_item_number = i;
                    break;
                }
            }
        }
    }

    if (item_info)
    {
        if (is_have_require_item == false)
        {
            find_color = WzColor_RGBA(255,0,0,255);
        }

        AddEmptyLineForTooltip();
        //2488 ������ �ʿ��� ������
        g_InterfaceManager.GetInterfaceString(2488, szText, INTERFACE_STRING_LENGTH);
        sprintf(str_format, "%s ", szText);
        RegisterTooltip(str_format, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, 0);

        RegisterTooltipImage(item_info->m_wVICode, kTexture_Rectangle_SocketItem,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
            (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
            0.0f, m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].top - 3.0f, find_color);

        g_InterfaceManager.GetItemName(mission_info->require_item[find_item_number], szMessage, INTERFACE_STRING_LENGTH);
        RegisterTooltip(szMessage, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
#ifdef _DEV_VER
    //#ifdef _SCJ_SHOW_ITEM_ID
        for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
        {
            if (mission_info->require_item[i] != 0)
            {
                item_info = ItemInfoParser::Instance()->GetItemInfo(mission_info->require_item[i]);
                if (item_info)
                {
                    if (CGeneralGameParam::Instance()->IsShowItemId())
                    {
                        char buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("<ITEMID:%ld>"), item_info->m_Code);
                        RegisterTooltip(szMessage, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                    }
                }
            }
        }
    //#endif //_SCJ_SHOW_ITEM_ID
#endif //#ifdef _DEV_VER
    }
}
#endif//_DH_BATTLEZONE2_

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
void uiToolTipMan::RegisterTooltipCurrencyDescription(
    const CurrencyInfoParser::CurrencyInfo* currency_info)
{
    if (currency_info == NULL)
    {
        return;
    }

    InitTooltip();

    // ȭ��� ��� �� ������ ���
    TCHAR tooltip_message[INTERFACE_STRING_LENGTH] = {'\0', };
    g_InterfaceManager.GetInterfaceString(
        currency_info->currency_name_code, tooltip_message);

    if (currency_info->currency_icon_code != 0)
    {
        RegisterTooltip(" ",
            -1,
            iLargeFontHeight,
            FontID_ItemName,
            WzNormalColor,
            TOOLTIP_BG_WZCOLOR,
            c_dwTOOLTIP_EXTRA_INFO_DT_CENTER,
            eSPECIAL_TOOLTIP_NOMAL_ITEM | eSPECIAL_TOOLTIP_ITEM_ON_TT_TEXT, 
            currency_info->currency_icon_code);

        AddEmptyLineForTooltip();
    }    

    RegisterTooltip(tooltip_message, 
        -1,
        iNameLineHeight,
        FontID_ItemName,
        WzNormalColor,
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_CENTER);

    AddEmptyLineForTooltip();

    // �ִ뺸�� ���� ���
    TCHAR max_point[INTERFACE_STRING_LENGTH] = {'\0', };
    Snprintf(tooltip_message, INTERFACE_STRING_LENGTH - 1, "%d", currency_info->currency_max_point);
    NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
    ::GetNumberFormat(
        LOCALE_SYSTEM_DEFAULT, 
        NULL, 
        tooltip_message, 
        &nFmt, 
        max_point, 
        sizeof(max_point));
    g_InterfaceManager.GetInterfaceStringFormat(
        tooltip_message, INTERFACE_STRING_LENGTH, 70702, max_point);
    RegisterTooltip(tooltip_message, -1, 20, FontID_12_1, WzTitleColor);

    AddEmptyLineForTooltip();

    // ���� ���
    g_InterfaceManager.GetInterfaceString(
        currency_info->currency_description_code, tooltip_message);
    RegisterTooltipLineChange(tooltip_message, 
        _tcslen(tooltip_message), 
        iNormalLineHeight, 
        FontID_ItemInfo, 
        WzDescColor);

    AddEmptyLineForTooltip();
}
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

void uiToolTipMan::RegisterTooltipCrystalization(SCItemSlot* item_slot_ptr)
{
    if (item_slot_ptr == NULL)
        return;

    if (item_slot_ptr->IsContainValidItem() == FALSE)
        return;

    TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };

    InitTooltip();

    _addItemNameForTooltip(item_slot_ptr, item_slot_ptr->GetItemInfo());

    BASE_ITEMINFO * base_item_info = ItemInfoParser::Instance()->GetItemInfo(item_slot_ptr->GetCode());
    if (base_item_info == NULL)
    {
        // 2162 : ����ȭ �� �� ���� ������
        g_InterfaceManager.GetInterfaceString(2162, result_message);
        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 0, 0, 255),
            TOOLTIP_BG_WZCOLOR, 0);

        AddEmptyLineForTooltip();

        return;
    }

    if (item_slot_ptr->CanUseMixture(MIXTURE_CRYSTALIZE) == false)
    {
        // 2162 : ����ȭ �� �� ���� ������
        g_InterfaceManager.GetInterfaceString(2162, result_message);
        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 0, 0, 255),
            TOOLTIP_BG_WZCOLOR, 0);

        AddEmptyLineForTooltip();

        return;
    }

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode((*item_slot_ptr), (*item_slot_ptr).GetEnchant());
#else
    SLOTCODE slot_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode((*item_slot_ptr));
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
    if (slot_code == 0)
    {   
        // 2162 : ����ȭ �� �� ���� ������
        g_InterfaceManager.GetInterfaceString(2162, result_message);
        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 0, 0, 255),
            TOOLTIP_BG_WZCOLOR, 0);

        AddEmptyLineForTooltip();

        return;
    }

    sITEM_CRYSTALIZE_LIST* item_crystalize_list = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo(slot_code);
    if (item_crystalize_list == NULL)
    {
        // 2162 : ����ȭ �� �� ���� ������
        g_InterfaceManager.GetInterfaceString(2162, result_message);
        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 0, 0, 255),
            TOOLTIP_BG_WZCOLOR, 0);

        AddEmptyLineForTooltip();

        return;
    }
    
    // 2163 : ����ȭ �� ���� ȹ�� ������
    g_InterfaceManager.GetInterfaceString(2163, result_message);
    RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 255, 0, 255),
        TOOLTIP_BG_WZCOLOR, 0);

    AddEmptyLineForTooltip();

    for (BYTE i = 0; i < sITEM_CRYSTALIZE_LIST::MAX_MATERIALS; ++i)
    {
        TCHAR item_name[MAX_ITEMNAME_LENGTH] = {0, };

        g_InterfaceManager.GetItemName(item_crystalize_list->m_pResult[i].m_MaterialCode, item_name);

        BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_crystalize_list->m_pResult[i].m_MaterialCode);
        if (!pItemInfo)
        {
            continue;
        }

        RegisterTooltipImage(kTooltipTexture_Socket,kTexture_Rectangle_SocketEmpty,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right + 4.0f,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom + 4.0f,
            c_dwTOOLTIP_EXTRA_INFO_DT_LEFT, -2.0f, -2.0f);

        RegisterTooltipImage(pItemInfo->m_wVICode, kTexture_Rectangle_SocketItem,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right - 3.0f,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom - 3.0f,
            /*(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE)*/(c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE |
            c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
            -(m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right) - 4.0f , 1.0f);

        MONEY MaterialMoney = item_crystalize_list->m_pResult[i].m_MaterialMoney;
        MONEY ObtainNum;
        if (0 == MaterialMoney)
        {
            ObtainNum = item_crystalize_list->m_pResult[i].m_MaterialCount;
        }
        else
        {
            ObtainNum = item_slot_ptr->GetPriceForBuy() / MaterialMoney + item_crystalize_list->m_pResult[i].m_MaterialCount;
        }
        
        Snprintf(result_message, INTERFACE_STRING_LENGTH-1, "%s  ", item_name);

        RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(255, 255, 255, 255),
            TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);

        TCHAR ea_string[INTERFACE_STRING_LENGTH] = {0, };
        // 155376 : %d��
        g_InterfaceManager.GetInterfaceString(155376, ea_string);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		// ����ȭ�� ���� ���� �ݿ�
		SLOTCODE temp_crystalize_code = ItemCrystalizeListParser::Instance()->GetCrystalizeCode( (*item_slot_ptr), 0 );
		sITEM_CRYSTALIZE_LIST* list_info = ItemCrystalizeListParser::Instance()->GetCrystalizeListInfo( temp_crystalize_code );
		if ( list_info )
		{
			ObtainNum += (list_info->m_pResult[i].m_MaterialCount * item_slot_ptr->GetAwakening());
		}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
        Snprintf(result_message,INTERFACE_STRING_LENGTH-1, ea_string, ObtainNum);

        RegisterTooltip( result_message,
            -1,
            iNormalLineHeight,
            FontID_ItemInfo,
            WzColor_RGBA(255, 255, 255, 255),
            TOOLTIP_BG_WZCOLOR,
            (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE/*|c_dwTOOLTIP_EXTRA_CELL2*/|c_dwTOOLTIP_EXTRA_CELL3));
    

        if (item_crystalize_list->m_pResult[i].m_ResultRatio < 100)
        {
            TCHAR ratio_string[INTERFACE_STRING_LENGTH] = {0, };
            // 2164 : Ȯ�� ����
            g_InterfaceManager.GetInterfaceString(2164, ratio_string);
            
            Snprintf(result_message, INTERFACE_STRING_LENGTH-1, "%s%s%s", "     ", ratio_string, "     ");
            RegisterTooltip(result_message, -1, iNormalLineHeight, FontID_ItemInfo, WzColor_RGBA(0, 255, 0, 255),
                TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
        }
    }

    // 5848 : ����ȭ �� ����ȭ �� �������� �������\n ���� ���� �������� �����˴ϴ�.
    g_InterfaceManager.GetInterfaceString(5848, result_message);
    RegisterTooltip( result_message,
        -1,
        iLineHeight,
        FontID_ItemInfo,
        WzColor_RGBA(255, 0, 0, 255),
        TOOLTIP_BG_WZCOLOR,
        (c_dwTOOLTIP_EXTRA_INFO_DT_LEFT));

    AddEmptyLineForTooltip();
}

#ifdef _NA_008334_20150608_SONNENSCHEIN
void uiToolTipMan::RegisterTooltipGuildMissionTree(const GuildMissionInfo* mission_info)
{
    const ChaosZoneInfo* find_info = ChaosZoneInfoParser::Instance()->FindData(mission_info->map_code);

    if ((mission_info == NULL) || (find_info == NULL))
    {
        return;
    }

    

    if (_isEnableShowTooltip() == false)
    {
        return;
    }

    InitTooltip();

    //------------------------------------------------------------------------------ 
    TCHAR	string[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	string_format[INTERFACE_STRING_LENGTH]={0,};
    static WzColor name_color = WzColor_RGBA(207, 207, 0, 255);
    static WzColor green = WzColor_RGBA(0, 207, 0, 255);
    static WzColor orange = WzColor_RGBA(255, 187, 0, 255);
    static WzColor discription_color = WzColor_RGBA(255, 255, 255, 255);

    //�̸�
    g_InterfaceManager.GetInterfaceString(mission_info->sub_group_string, string, INTERFACE_STRING_LENGTH);
    RegisterTooltip(string, -1, iNameLineHeight, FontID_ItemName, name_color, TOOLTIP_BG_WZCOLOR, 0);

    AddEmptyLineForTooltip();


    // �̼ǳ���
    if (mission_info->mission_string_code != 0)
    {
        g_InterfaceManager.GetInterfaceString(mission_info->mission_string_code, string, INTERFACE_STRING_LENGTH );

        ////////////////
        util::_tstring strMapDesc = string;

        const TCHAR* cstrMapDesc;
        util::_tstring tempstrMapDescSpatial;
        std::vector<util::_tstring> strMapDescSpatial;
        cstrMapDesc = strMapDesc.c_str();
        tempstrMapDescSpatial.clear();
        for (size_t strIter = 0; strIter<strMapDesc.size(); ++strIter)
        {
            if (cstrMapDesc[strIter] == _T('#'))
            {
                strMapDescSpatial.push_back(tempstrMapDescSpatial);
                tempstrMapDescSpatial.clear();
            }
            else
            {
                tempstrMapDescSpatial += strMapDesc[strIter];
            }
        }

        if (tempstrMapDescSpatial.size())
        {
            strMapDescSpatial.push_back(tempstrMapDescSpatial);
            tempstrMapDescSpatial.clear();
        }

        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        if (strMapDescSpatial.size())
        {
            for (size_t i=0; i<strMapDescSpatial.size(); ++i)
            {
                sprintf(string, strMapDescSpatial[i].c_str());
                RegisterTooltipLineChange(string, _tcslen(string), 
                    iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
            }
        }
        else
        {
            sprintf(string, strMapDesc.c_str());
            RegisterTooltipLineChange(string, _tcslen(string), iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
        }
        /////////////////

        //RegisterTooltipLineChange(szText, _tcslen(szText), iNameLineHeight, FontID_ItemName, discription_color, TOOLTIP_BG_WZCOLOR, 0);
        AddEmptyLineForTooltip();
    }

    //������ �ʿ��� ������
    bool is_have_require_item = false;
    int find_item_number = 0;

    const BASE_ITEMINFO* item_info = NULL;
    WzColor find_color = WzColor_RGBA(0,0,0,0);

    for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
    {
        if (find_info->require_item[i] != 0)
        {
            item_info = ItemInfoParser::Instance()->GetItemInfo(find_info->require_item[i]);
            if (item_info)
            {
                //������ ã��
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, static_cast<SLOTCODE>(find_info->require_item[i]));
                if (item_slot && item_slot->GetNum() > 0)
                {
                    is_have_require_item = true;
                    find_item_number = i;
                    break;
                }
            }
        }
    }

    if (item_info)
    {
        if (is_have_require_item == false)
        {
            find_color = WzColor_RGBA(255,0,0,255);
        }

        //2488 ������ �ʿ��� ������
        g_InterfaceManager.GetInterfaceString(2488, string, INTERFACE_STRING_LENGTH);
        sprintf(string_format, "%s ", string);
        RegisterTooltip(string_format, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, 0);
        AddEmptyLineForTooltip();

        RegisterTooltipImage(item_info->m_wVICode, kTexture_Rectangle_SocketItem,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].right,
            m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].bottom,
            (c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE),
            0.0f, m_TooltipTextureRectAngles[kTexture_Rectangle_SocketEmpty].top - 1.f, find_color);

        g_InterfaceManager.GetItemName(find_info->require_item[find_item_number], string, INTERFACE_STRING_LENGTH);
        RegisterTooltip(string, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);

#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_ITEM_ID
        for (int i = 0; i < MAX_REQUIRE_ITEM; ++i)
        {
            if (find_info->require_item[i] != 0)
            {
                item_info = ItemInfoParser::Instance()->GetItemInfo(find_info->require_item[i]);
                if (item_info)
                {
                    if (CGeneralGameParam::Instance()->IsShowItemId())
                    {
                        char buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
                        Snprintf(string, INTERFACE_STRING_LENGTH-1, _T("<ITEMID:%ld>"), item_info->m_Code);
                        RegisterTooltip(string, -1, iNameLineHeight, FontID_ItemName, green, TOOLTIP_BG_WZCOLOR, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE);
                    }
                }
            }
        }
        //#endif //_SCJ_SHOW_ITEM_ID
#endif //#ifdef _DEV_VER
    }

}
#endif//_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
void uiToolTipMan::RegisterTooltipGuildMissionTree_OneDayMission(const GuildMissionInfo* mission_info, DWORD end_time, BYTE event_state)
{
    RegisterTooltipGuildMissionTree(mission_info);

    TCHAR	string[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	temp_string1[INTERFACE_STRING_LENGTH]={0,};
    TCHAR	temp_string2[INTERFACE_STRING_LENGTH]={0,};
    static WzColor orange = WzColor_RGBA(255, 187, 0, 255);    

    // 11142 ������� :
    g_InterfaceManager.GetInterfaceString(11142, temp_string1, INTERFACE_STRING_LENGTH-1);

    if (event_state == eGUILDMISSION_MATCHED)
    {
        // 11143 ������
        g_InterfaceManager.GetInterfaceString(11143, temp_string2, INTERFACE_STRING_LENGTH-1);
    }
    else
    {
        // 11144 �����
        g_InterfaceManager.GetInterfaceString(11144, temp_string2, INTERFACE_STRING_LENGTH-1);
    }

#ifdef _DEV_VER
    sprintf(string, "%s%s (%d)", temp_string1, temp_string2, event_state);
#else
    sprintf(string, "%s%s", temp_string1, temp_string2);
#endif //_DEV_VER
    RegisterTooltip(string, -1, iNameLineHeight, FontID_ItemName, orange, TOOLTIP_BG_WZCOLOR, 0);

    if (event_state == eGUILDMISSION_MATCHED)
    {
        // �������̸� �����ð� ǥ��
        DWORD remain_time = 0;
        if (end_time > GetTickCount())
        {
            remain_time = end_time - GetTickCount();
        }

        DWORD minTick   = 0;
        DWORD secTick   = 0;

        if (remain_time)
        {
            minTick = (remain_time / SEC_PER_TICK) / 60;
            secTick = (remain_time / SEC_PER_TICK) % 60;
        }

        Snprintf(temp_string1, INTERFACE_STRING_LENGTH-1, "%d", minTick);
        Snprintf(temp_string2, INTERFACE_STRING_LENGTH-1, "%d", secTick);

        // 11145 �����ð� : %s�� %s��
        g_InterfaceManager.GetInterfaceStringFormat(
            string, INTERFACE_STRING_LENGTH, 11145, temp_string1, temp_string2);
        RegisterTooltip(string, -1, iNameLineHeight, FontID_ItemName, orange, TOOLTIP_BG_WZCOLOR, 0);
    }
}
#endif//_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

