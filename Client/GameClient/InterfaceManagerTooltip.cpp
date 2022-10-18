#include "SunClientPrecompiledHeader.h"
//#include ".\interfacemanager.h"
//
//#include "Application.h"
//#include "GameFunc.h"
//#include "TooltipDialog.h"
//#include "ItemInfoParser.h"
//#include "SkillInfoParser.h"
//#include "SkillInventoryStoreParser.h"
//#include "SCItemSlot.h"
//#include "StyleSlot.h"
//#include "StyleInfoParser.h"
//#include "QuickSlot.h"
//#include "StyleContainer.h"
//#include ".\QuickContainer.h"
//#include "HeroEquipmentContainer.h"
//#include "SummonSkillContainer.h"
//#include "ItemManager.h"
//#include "StateInfoParser.h"
//#include "ObjectManager.h"
//#include "MouseHandler.h"
//#include "ItemFormula.h"
//#include "CursorChangeTransact.h"
//#include "SkillTextInfoParser.h"
//#include "Character.h"
//#include "Hero.h"
//
//#include "OptionListParser.h"
//
//
//#pragma warning ( push )
//#pragma warning ( disable : 4244)
//
//#ifndef DEF_REDSKY_MODIFY_TOOLTIP_20060719
//
//#define		BUG_FIX_TOOLTIP_BOX_WIDTH		//	2006년 2월 13일 임대진
//											//	툴팁의 박스 크기가 고정되어 있어 출력할 내용이 박스 가로 크기 보다 클 경우,
//											//	글자가 겹쳐서 출력되던 버그 수정
//
//
//#define		BUG_FIX_ITEM_TOOLTIP_RANK_OPTION	//	2006년 2월 13일 임대진
//												//	악세사리 아이템 툴팁의 랭크 옵션 항목에 출력될 텍스트 코드가 없어서
//												//	일단 스크립트를 받을 때까지 배열로 하드 코딩
//
//
//
//
///*
//	ID		폰트명	크기	스타일	기타
//	s210	굴림체	8		Bold	
//	st10	굴림체	8		Bold
//	m212	굴림체	10		Bold
//	mn12	굴림체	10		Bold
//	tt12	굴림체	12		Bold
//	m216	굴림체	14
//	mi16	들국화	14
//	sn24	들국화	24
//	zn44	들국화	44
//*/
//
//static const int TOOLTIP_TEXT_WIDTH = 220 + 20;
//static const int TOOLTIP_WIDTH = 50;
//
//static const int TOOLTIP_WIDTH_WIDE = 420 + 20;
//
//const WzID			FontID_SkillName	= StrToWzID("m212");
//const WzID			FontID_SkillInfo	= StrToWzID("mn12");
//
//const WzID			FontID_ItemName		= StrToWzID("m212");
//const WzID			FontID_ItemInfo		= StrToWzID("mn12");
//
//const WzID			FontID_8	= StrToWzID("s210");
//const WzID			FontID_10	= StrToWzID("m212");	
//const WzID			FontID_12	= StrToWzID("tt12");
//const WzID			FontID_12_1	= StrToWzID("mn12");	
//const WzID			FontID_14	= StrToWzID("m216");	
//const WzID			FontID_24	= StrToWzID("sn24");
//const WzID			FontID_14_1 = StrToWzID("mi16");	// 들국화
//
//const WzColor		WzCurSkillLevelColor = WzColor_RGBA( 188, 240, 120, 255 );
//const WzColor		WzCurSkillColor = WzColor_RGBA( 136, 221, 178, 255 );
//const WzColor		WzCurSkillDescColor = WzColor_RGBA( 247, 251, 190, 255 );
//
//const WzColor		WzNextSkillLevelColor = WzColor_RGBA( 244, 120, 82, 255 );
//const WzColor		WzNextSkillColor = WzColor_RGBA( 190, 80, 61, 255 );
//const WzColor		WzNextSkillDescColor = WzColor_RGBA( 229, 104, 75, 255 );
//
//const WzColor		WzSkillErrorColor = WzColor_RGBA( 229, 75, 75, 255 );
//const WzColor		WzSkillDisableColor = WzColor_RGBA( 150, 150, 150, 255 );
//
//
//const WzColor		WzDivineNameColor = WzColor_RGBA( 176, 118, 242, 255 );
//const WzColor		WzEliteNameColor = WzColor_RGBA(250,248,75,255);
//const WzColor		WzUniqueNameColor = WzColor_RGBA(183,0,0,255);
//const WzColor		WzNameColor = WzColor_RGBA( 253, 183, 0, 255 );
//const WzColor		WzTitleColor = WzColor_RGBA( 179, 244, 166, 255 );
//const WzColor		WzNormalColor = WzColor_RGBA( 255, 255, 255, 255 );
//const WzColor		WzSpaceColor = WzColor_RGBA( 255, 255, 255, 0 );
//
//const WzColor		WzErrorColor = WzColor_RGBA( 204, 0, 0, 255 );
//
//const WzColor		WzEffectColor	= WzColor_RGBA( 51, 240, 51, 255 );
//const WzColor		WzDescColor		= WzColor_RGBA( 255, 255, 153, 255 );
//const WzColor		WzDisableColor	= WzColor_RGBA( 153, 153, 153, 255 );
//const WzColor		WzSkillNameColor = WzColor_RGBA( 249, 165, 44, 255 );
//const WzColor		WzSkillDescColor = WzColor_RGBA( 222, 239, 74, 255 );
//const int			iSkillNameLineHeight = 25;
//
//const int			iTitleLineHeight = 14 + 2;
//const int			iSpaceLineHeight = 12;
//
//const float			c_fDuraRate			= 5.f;
//const int			iNormalLineHeight	= 12 + 4;
//const int			iLineHeight			= 24;
//const int			iNameLineHeight		= 19;
//const int			iLargeFontHeight	= 27;
//const int			iSmallFontHeight	= 12;
//const int			iSimpleTooltipHeight		= 23;
//
//enum eTOOLTIP_BUFF_DESC_TYPE
//{
//	eTOOLTIP_BUFF_DESC_TYPE0 = 0,	// 어떠한 Parameter도 참조하지 않는놈들
//	eTOOLTIP_BUFF_DESC_TYPE1,		// Option 1 값 참조
//	eTOOLTIP_BUFF_DESC_TYPE2,		// Parameter1 값 참조
//	eTOOLTIP_BUFF_DESC_TYPE3,		// Parameter2 값 참조 (Param1에 따라 변화없음)
//	eTOOLTIP_BUFF_DESC_TYPE4,		// Parameter2 값 참조 (Param1에 따라 변화있음)
//	eTOOLTIP_BUFF_DESC_TYPE5,		// Parameter1, Parameter2 참조
//	eTOOLTIP_BUFF_DESC_TYPE6,		// Parameter2, Option2 참조
//};
//
//BOOL InterfaceManager::_isEnableShowBuffTooltip(SKILL_EFFECT_IMAGE *pImage)
//{	
//	if(!_isEnableShowTooltip())
//		return FALSE;
//
//	assert(pImage);
//
//	if(!pImage) return FALSE;
//	
//	return TRUE;
//
//}
//
////----------------------------------------------------------------------------------------
//void InterfaceManager::_addBuffNameForTooltip(BASE_STATEINFO* pStateInfo)
//{
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//
//	GetInterfaceString(pStateInfo->m_dwNamecode,szString,INTERFACE_STRING_LENGTH);
//	RegisterTooltip(szString,-1,
//					iSkillNameLineHeight,
//					FontID_SkillName,
//					WzSkillNameColor,
//					0);
//}
//
////----------------------------------------------------------------------------------------
//int	InterfaceManager::_getBuffDescType(DWORD dwStatusID)
//{
//	switch(dwStatusID) 
//	{
//		// 어떠한 Parameter도 참조하지 않는놈들
//		default:
//		case eCHAR_STATE_CHAOS:
//		case eCHAR_STATE_BLIND:
//		case eCHAR_STATE_DOWN:
//		case eCHAR_STATE_DELAY:
//		case eCHAR_STATE_SEALING:
//		case eCHAR_STATE_STUN:
//		case eCHAR_STATE_STONE:
//		case eCHAR_STATE_SLEEP:
//		case eCHAR_STATE_FROZEN:
//		case eCHAR_STATE_SEQUELA:
//		case eCHAR_STATE_REFLECT_FEAR:
//		case eCHAR_STATE_PROTECTION:
//		case eCHAR_STATE_HOLDING:
//		case eCHAR_STATE_TRANSFORMATION:
//		case eCHAR_STATE_STEEL :
//		case eCHAR_STATE_TRANSPARENT:
//		case eCHAR_STATE_FEAR:
//		case eCHAR_STATE_BLUR:
//			return eTOOLTIP_BUFF_DESC_TYPE0;
//
//		// Option 1
//		case eCHAR_STATE_REFLECT_STUN:
//		case eCHAR_STATE_REFLECT_FROZEN:
//		case eCHAR_STATE_REFLECT_DAMAGE:
//			return eTOOLTIP_BUFF_DESC_TYPE1;
//
//	    // Parameter1
//		case eCHAR_STATE_REFLECT_SLOW:
//		case eCHAR_STATE_REFLECT_SLOWDOWN:
//			return eTOOLTIP_BUFF_DESC_TYPE2;
//
//		// Parameter2 Param1에 따라 변화없음
//		case eCHAR_STATE_POISON :
//		case eCHAR_STATE_WOUND :
//		case eCHAR_STATE_FIRE_WOUND :
//		case eCHAR_STATE_FETTER :
//		case eCHAR_STATE_SLOWDOWN :
//		case eCHAR_STATE_HIGH_SPIRIT :
//		case eCHAR_STATE_SPEEDING :
//		case eCHAR_STATE_INCREASE_SKILLRANGE :
//		case eCHAR_STATE_PRECISION :
//		case eCHAR_STATE_BUF_RANGE_DAMAGE:
//			return eTOOLTIP_BUFF_DESC_TYPE3;
//
//		// Parameter2 Param1에 따라 변화있음
//		case eCHAR_STATE_PANIC:
//		case eCHAR_STATE_LOW_SPIRIT :
//		case eCHAR_STATE_WEAKENING :
//		case eCHAR_STATE_ATTACK_FAIL:
//		case eCHAR_STATE_FRUSTRATION:
//		case eCHAR_STATE_HP_REDUCE:
//		case eCHAR_STATE_MP_REDUCE:
//		case eCHAR_STATE_LOW_STRENGTH:
//		case eCHAR_STATE_DICLINE:
//		case eCHAR_STATE_MAGIC_EXPOSE:
//		case eCHAR_STATE_ABSORB:
//		case eCHAR_STATE_HPRATE_REDUCE:
//		case eCHAR_STATE_MPRATE_REDUCE:
//		case eCHAR_STATE_ANGER:
//		case eCHAR_STATE_DEFENSE:
//		case eCHAR_STATE_FIGHTING:
//		case eCHAR_STATE_BALANCE:
//		case eCHAR_STATE_VITAL_POWER:
//		case eCHAR_STATE_MEDITATION:
//		case eCHAR_STATE_CONCENTRATION:
//		case eCHAR_STATE_HP_INCREASE:
//		case eCHAR_STATE_MP_INCREASE:
//		case eCHAR_STATE_HPRATE_INCREASE:
//		case eCHAR_STATE_MPRATE_INCREASE:
//		case eCHAR_STATE_MAGIC_DEFENSE:		 
//		case eCHAR_STATE_STAT_STR:
//		case eCHAR_STATE_STAT_DEX:
//		case eCHAR_STATE_STAT_VIT:
//		case eCHAR_STATE_STAT_INT:
//		case eCHAR_STATE_STAT_SPI:
//		case eCHAR_STATE_MAGIC_ATTACK_INCREASE:
//		case eCHAR_STATE_MAGIC_ATTACK_DECRESE:
//		case eCHAR_STATE_STAT_STR_DECRESE:
//		case eCHAR_STATE_STAT_DEX_DECRESE:
//		case eCHAR_STATE_STAT_VIT_DECRESE:
//		case eCHAR_STATE_STAT_INT_DECRESE:
//		case eCHAR_STATE_STAT_SPI_DECRESE:
//			return eTOOLTIP_BUFF_DESC_TYPE4;
//
//		// Parameter1, Parameter2
//		case eCHAR_STATE_SP_BONUS:
//			return eTOOLTIP_BUFF_DESC_TYPE5;
//
//		// Parameter2,Option2
//		case eCHAR_STATE_MAGIC_SHIELD:	    
//			return eTOOLTIP_BUFF_DESC_TYPE6;
//	}
//
//	return eTOOLTIP_BUFF_DESC_TYPE0;
//}
//
////----------------------------------------------------------------------------------------
//void InterfaceManager::_addBuffDescForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage)
//{
//	TCHAR	szFormat[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szParam[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szParam2[INTERFACE_STRING_LENGTH] = {0,};
//	
//	SKILL_EFFECT *pEffect = NULL;
//
//	Character *pChr = (Character *)g_ObjectManager.GetObject(pImage->dwOwnerID);
//	if (pChr)
//	{
//		pEffect = pChr->GetSkillEffect(pImage->iEffectID);
//	}
//			
//	if (!pEffect) return;
//		
//	GetInterfaceString(pStateInfo->m_dwDescCode,szFormat,INTERFACE_STRING_LENGTH);
//
//	switch(_getBuffDescType(pImage->dwStatusID)) 
//	{
//	case eTOOLTIP_BUFF_DESC_TYPE0:
//		StrCopy(szText,szFormat);
//		break;
//
//	case eTOOLTIP_BUFF_DESC_TYPE1:
//		Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iOption1);
//		Sprintf(szText,szFormat,szParam);
//		break;
//	
//	case eTOOLTIP_BUFF_DESC_TYPE2:
//		Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iParam[0]);
//		Sprintf(szText,szFormat,szParam);
//		break;
//
//	case eTOOLTIP_BUFF_DESC_TYPE3:
//		Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
//		Sprintf(szText,szFormat,szParam);
//		break;		 
//
//	case eTOOLTIP_BUFF_DESC_TYPE4:
//		{
//			if (pEffect->AbilityInfo.m_iParam[0] == 1) 
//			{
//				Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
//			}
//			else
//			{
//				Sprintf(szParam,_T("%ld%% "),pEffect->AbilityInfo.m_iParam[1]);
//
//			}
//			Sprintf(szText,szFormat,szParam);
//
//		}
//		break;
//
//	case eTOOLTIP_BUFF_DESC_TYPE5:
//		Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iParam[0]);
//		Sprintf(szParam2,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
//		Sprintf(szText,szFormat,szParam,szParam2);
//		break;
//
//	case eTOOLTIP_BUFF_DESC_TYPE6:	    
//		Sprintf(szParam,_T("%ld "),pEffect->AbilityInfo.m_iParam[1]);
//		Sprintf(szParam2,_T("%ld "),pEffect->AbilityInfo.m_iOption2);
//		Sprintf(szText,szFormat,szParam,szParam2);
//		break;
//
//	}
//
//	_addEmptyLineForTooltip();
//
//	RegisterTooltipLineChange(szText,
//							_tcslen(szText),
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							WzSkillDescColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//}
//
////----------------------------------------------------------------------------------------
//void InterfaceManager::_addBuffRemainTimeForTooltip(BASE_STATEINFO* pStateInfo, SKILL_EFFECT_IMAGE *pImage)
//{
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTime[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szMin[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szSec[INTERFACE_STRING_LENGTH] = {0,};
//	
//	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//
//	SKILL_EFFECT *pEffect = NULL;
//
//	DWORD dwRemainTime = 0;
//	DWORD dwRemainSecond = 0;
//	DWORD dwRemainMinute = 0;
//
//	Character *pChr = (Character *)g_ObjectManager.GetObject(pImage->dwOwnerID);
//	if (pChr)
//	{
//		pEffect = pChr->GetSkillEffect(pImage->iEffectID);
//	}
//			
//	if(!pEffect) return;
//
//	if(pEffect->iRemainTime > 0)
//	{
//		dwRemainTime = pEffect->iRemainTime / 1000;
//		dwRemainSecond = dwRemainTime % 60;
//		dwRemainMinute = dwRemainTime / 60;
//
//		//	남은시간 : %d분 %d초
//		g_InterfaceManager.GetInterfaceString( eST_REMAIN_TIME_TEXT, szTime, INTERFACE_STRING_LENGTH );
//		g_InterfaceManager.GetInterfaceString( eST_MINUTE_TEXT, szMin, INTERFACE_STRING_LENGTH );
//		g_InterfaceManager.GetInterfaceString( eST_SECOND_TEXT, szSec, INTERFACE_STRING_LENGTH );
//
//		if (dwRemainMinute > 0)
//		{
//			Sprintf( szMessage, _T("%s : %d%s %d%s"), szTime, dwRemainMinute, szMin, dwRemainSecond, szSec );
//		}
//		else if (dwRemainSecond > 0)
//		{
//			Sprintf( szMessage, _T("%s : %d%s"), szTime, dwRemainSecond, szSec );
//		}
//		else
//			return;
//
//		RegisterTooltip( szMessage,
//				-1,
//				iNormalLineHeight,
//				FontID_SkillInfo,
//				WzNormalColor,
//				TOOLTIP_BG_WZCOLOR,
//				dwTooltipExtraStyle);
//
//	}
//}
//
////----------------------------------------------------------------------------------------
//void InterfaceManager::RegisterSkillEffectInfoTooltip(SKILL_EFFECT_IMAGE *pImage)
//{
//	if(!_isEnableShowBuffTooltip(pImage)) return;
//
//	InitTooltip();
//
//	BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(pImage->dwStatusID);
//	
//	if (pInfo)
//	{
//		_addBuffNameForTooltip(pInfo);
//		_addBuffRemainTimeForTooltip(pInfo, pImage);
//		_addBuffDescForTooltip(pInfo, pImage);
//	}
//}
//
////-----------------------------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableShowItemTooltip(const SCSlot *pSlot)
//{
//	if(!_isEnableShowTooltip())
//		return FALSE;
//
//	assert(ST_ITEM == pSlot->GetSlotType() );
//	if (ST_ITEM != pSlot->GetSlotType())
//	{
//		return FALSE;
//	}
//
//	if (!ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode()))
//        return FALSE;
//
//
//	return TRUE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemRankUp(BASE_ITEMINFO *pItemInfo)
//{
//	//if(pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemInfo->IsAccessory())
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//		return TRUE;
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemEnchant(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo)
//{
//	if(pItemSlot->GetEnchant() <= 0)
//		return FALSE;
//
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
//		return TRUE;
//	
//	return FALSE;
//}
//
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemSocket(BASE_ITEMINFO *pItemInfo)
//{
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
//		return TRUE;
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemType(BASE_ITEMINFO *pItemInfo)
//{
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
//		return FALSE;
//
//	return TRUE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemEquipClass(BASE_ITEMINFO *pItemInfo)
//{
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
//		return TRUE;
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemDurability(BASE_ITEMINFO *pItemInfo)
//{
//	if(pItemInfo->IsArmor() || pItemInfo->IsWeapon())
//		return TRUE;
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableItemOption(BASE_ITEMINFO *pItemInfo)
//{
//	//if( pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemInfo->IsAccessory())
//	if( pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//		return TRUE;
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemNameForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo)
//{
//	if(!g_pHero)
//		return;
//
//	const	TCHAR	*szSpace = _T(" ");
//
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//	
//	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
//
//	DWORD dwTooltipExtraStyle = 0;
//
//	WzColor textColor = WzNormalColor;
//
//	// 무기, 방어구만 전체 이름( 네임 + 종류 )을 사용 -> (합금) + (양손검)
//	if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() )
//	{
//		//GetInterfaceString(eST_DIVINE_STRING_INDEX,szString,INTERFACE_STRING_LENGTH);
//		GetInterfaceString(pItemInfo->m_NCode,szTemp,INTERFACE_STRING_LENGTH);
//		GetInterfaceString(pItemInfo->m_SCNCode,szTemp2,INTERFACE_STRING_LENGTH);
//			
//		//if(pItemSlot->GetSet() == 1) // 디바인
//		//{
//		//	Sprintf(szInterfaceString,_T("%s %s %s"),szString, szTemp, szTemp2);
//		//	textColor = WzDivineNameColor;
//		//}
//		//else
//		//{
//		//	Sprintf(szInterfaceString,_T("%s %s"),szTemp, szTemp2);
//		//}
//
//		//if(pItemSlot->GetSet() == 1) // 디바인
//		if(pItemSlot->IsDivine())
//		{
//			textColor = WzDivineNameColor;	// 색만 변경
//		}
//
//		Sprintf(szInterfaceString,_T("%s %s"),szTemp, szTemp2);
//
//		if(_isEnableItemEnchant(pItemSlot, pItemInfo))
//		{
//			Sprintf(szTemp,_T("+%d"),pItemSlot->GetEnchant());
//			_tcscat(szInterfaceString,szTemp);
//		}
//	}
//	else	
//	{
//		//	아이템 이름만 출력 
//		GetInterfaceString(pItemInfo->m_NCode,szTemp,INTERFACE_STRING_LENGTH);
//		Sprintf(szInterfaceString,_T("%s"),szTemp);
//	}
//
//	this->RegisterTooltip(szInterfaceString,
//			-1,
//			iLargeFontHeight,
//			FontID_ItemName,
//			textColor,
//			TOOLTIP_BG_WZCOLOR,
//			dwTooltipExtraStyle);
//
//
//	if( pItemInfo->IsPotion() )
//	{
//		//	물약일 경우 쿨타임 오른쪽 정렬로 표시
//
//		//	남은 쿨타임 : %d 초
//		const int		iCoolTime = 11173;
//
//		util::Timer *pTimer = g_pHero->GetSCPlayer().GetItemCoolTimer(pItemInfo->m_wCoolTimeType);
//		if (pTimer && !pTimer->IsExpired(FALSE))	// 쿨타임이 남았으면
//		{
//			dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//
//			float fRemainRate = 1.0f - pTimer->GetProgressRatio();
//			float fRemainTime = pItemInfo->m_dCoolTime * fRemainRate;
//
//			DWORD dwCoolTime = (fRemainTime / 1000);
//			GetInterfaceString( iCoolTime, szTemp, INTERFACE_STRING_LENGTH);
//			Sprintf(szInterfaceString, szTemp, dwCoolTime );
//
//			this->RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			dwTooltipExtraStyle);
//		}
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemRankForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	// 랭크는 오른쪽 정렬을 해야함...
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//
//	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//
//	//if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() || pItemInfo->IsAccessory() )
//	if (pItemInfo->IsWeapon() || pItemInfo->IsArmor() || pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//	{
//		eRANK ItemRank = pItemSlot->GetRank();
//
//		int iRankIndex = eST_RANK_E_STRING_INDEX  + (pItemSlot->GetRank() - RANK_E);
//
//		if (iRankIndex > eST_RANK_PS_STRING_INDEX)
//		{
//			iRankIndex = eST_RANK_PS_STRING_INDEX;
//		}
//
//		if (iRankIndex < eST_RANK_E_STRING_INDEX)
//		{
//			iRankIndex = eST_RANK_E_STRING_INDEX;
//		}
//
//		GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
//		GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
//		Sprintf(szInterfaceString, szTemp, szTemp2);
//
//		RegisterTooltip(szInterfaceString,
//				-1,
//				iSmallFontHeight,
//				FontID_ItemInfo,
//				WzNormalColor,
//				TOOLTIP_BG_WZCOLOR,
//				dwTooltipExtraStyle);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemSocketForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	if(!pItemSlot->GetSocketNum())	return;
//	
//	// 오른쪽 정렬
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//
//	GetInterfaceString(eST_SOCKET_NUM_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);	
//	Sprintf(szInterfaceString, szTemp, pItemSlot->GetSocketNum());
//	
//	RegisterTooltip(szInterfaceString,
//				-1,
//				iSmallFontHeight,
//				FontID_ItemInfo,
//				WzNormalColor,
//				TOOLTIP_BG_WZCOLOR,
//				dwTooltipExtraStyle);
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemTypeForTooltip(BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//
//	GetInterfaceString(pItemInfo->m_SCNCode,szInterfaceString,INTERFACE_STRING_LENGTH);
//	
//    this->RegisterTooltip(szInterfaceString,
//        -1,
//        iNormalLineHeight,
//        FontID_ItemInfo,
//        WzNormalColor,
//		TOOLTIP_BG_WZCOLOR,
//		0);
//	
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemLimitLevelForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	if(!g_pHero)
//		return;
//	
//	//if (pItemInfo->IsAccessory() )
//	//if (pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//	WORD LimitLV = pItemInfo->m_LimitEqLevel;
//	if(LimitLV > 0)
//	{
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
//
//		WzColor wzTextColor = WzNormalColor;
//		//const int LIMIT_LEVEL_STRING_INDEX = 11005;	//	레벨 제한
//		GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
//				
//		//WORD wLimitAccessoryLV = CalcLimitLVAccessory( pItemInfo->m_LimitEqLevel, (eRANK)pItemSlot->GetRank() );
//		//Sprintf(szText, _T("%s %ld"), szInterfaceString, wLimitAccessoryLV);
//		Sprintf(szText, _T("%s %ld"), szInterfaceString, LimitLV);
//
//		if ( g_pHero->GetLevel() < LimitLV ) 
//		{	
//			wzTextColor = WzErrorColor;
//		}
//		else
//		{
//			wzTextColor = WzNormalColor;						
//		}
//
//		this->RegisterTooltip(	szText,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			wzTextColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		_addEmptyLineForTooltip();
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemNextGradeLimitLevelForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	if(!g_pHero)
//		return;
//
//	//if (pItemInfo->IsAccessory() )
//	//if (pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//	WORD LimitLV = pItemInfo->m_LimitEqLevel;
//	if(LimitLV > 0)
//	{
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szText[INTERFACE_STRING_LENGTH] = {0,};
//
//		WzColor wzTextColor = WzNormalColor;
//
//		//다음 등급 제한 수치
//		GetInterfaceString( eST_NEXT_GRADE_LIMIT_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
//		this->RegisterTooltip(	szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			wzTextColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		//const int LIMIT_LEVEL_STRING_INDEX = 11005;	//	레벨 제한
//		GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
//				
//		//WORD wLimitAccessoryLV = CalcLimitLVAccessory( pItemInfo->m_LimitEqLevel, (eRANK)(pItemSlot->GetRank() + 1));
//		//Sprintf(szText, _T("%s %ld"), szInterfaceString, wLimitAccessoryLV);
//		Sprintf(szText, _T("%s %ld"), szInterfaceString, LimitLV);
//
//		if ( g_pHero->GetLevel() < LimitLV ) 
//		{	
//			wzTextColor = WzErrorColor;
//		}
//		else
//		{
//			wzTextColor = WzNormalColor;						
//		}
//
//		this->RegisterTooltip(	szText,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			wzTextColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		_addEmptyLineForTooltip();
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemEquipClassForTooltip(BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//
//	// 착용 제한
//	if (pItemInfo->m_wEqClass1 && 
//		pItemInfo->m_wEqClass2 && 
//		pItemInfo->m_wEqClass3 && 
//		pItemInfo->m_wEqClass4 && 
//		pItemInfo->m_wEqClass5 )
//	{
//		// 모두 허용이면 하지마
//	}
//	else
//	{
//		int		iClassNameCode[eCHAR_TYPE_MAX] = {
//												eST_LIMITCLASS_BER_STRING_INDEX,
//												eST_LIMITCLASS_DRA_STRING_INDEX,
//												eST_LIMITCLASS_SHA_STRING_INDEX,
//												eST_LIMITCLASS_VAL_STRING_INDEX,
//												eST_LIMITCLASS_ELM_STRING_INDEX,
//												};
//
//		WORD		wCharEqClass[eCHAR_TYPE_MAX] = {
//												pItemInfo->m_wEqClass1,
//												pItemInfo->m_wEqClass2,
//												pItemInfo->m_wEqClass3,
//												pItemInfo->m_wEqClass4,
//												pItemInfo->m_wEqClass5,
//												};
//
//		WzColor wcTextColor = WzErrorColor;
//		for( int i = 0 ; i < eCHAR_TYPE_MAX ; i++ )
//		{
//			//	클래스 제한 있는 캐릭터 이름들 나열
//			if( wCharEqClass[i] )
//			{
//				GetInterfaceString( iClassNameCode[i], szTemp, INTERFACE_STRING_LENGTH );
//				Sprintf(szTemp2,_T("%s "), szTemp );
//				_tcscat(szInterfaceString,szTemp2);
//
//				BOOL bCanEquip = ItemManager::Instance()->CanEquipClass(pItemInfo);
//				if(bCanEquip)
//				//if( i == (g_pHero->GetCharInfo()->m_byClassCode - 1) )
//				{
//					wcTextColor = WzNormalColor;
//				}
//			}
//		}	
//
//		RegisterTooltip(szInterfaceString,
//						-1,
//						iNormalLineHeight,
//						FontID_ItemInfo,
//						wcTextColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//	}	
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemPhyAttackSpeedForTooltip(BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	if( pItemInfo->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP )
//	{
//		GetInterfaceString(eST_PHYATTACK_SPEED_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
//		Sprintf(szInterfaceString,_T("%s %d"),szTemp,pItemInfo->m_wPhyAttSpeed);
//		
//		RegisterTooltip(szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemPhyAttackPowerForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	DWORD dwPhyMinAttackPower = 0;
//	DWORD dwPhyMaxAttackPower = 0;
//	//DWORD dwPhyMinDamage = pItemInfo->m_wMinDamage;
//	//DWORD dwPhyMaxDamage = pItemInfo->m_wMaxDamage;
//
//	DWORD dwPhyMinDamage = 0;
//	DWORD dwPhyMaxDamage = 0;
//
//	DWORD WeightDamage = CalcAttackPower( pItemSlot->GetEnchant(), pItemInfo->m_LV );
//
//	if(pItemSlot->IsDivine())
//	{
//		dwPhyMinDamage = pItemInfo->m_wDivPhyMinDamage;
//		dwPhyMaxDamage = pItemInfo->m_wDivPhyMaxDamage;
//
//		//dwPhyMinDamage = CalcMinAttackPowerForDivineWeapon(dwPhyMinDamage, (eITEMTYPE)pItemInfo->m_wType, pItemInfo->m_LV );
//		//dwPhyMaxDamage = CalcMaxAttackPowerForDivineWeapon(dwPhyMaxDamage, (eITEMTYPE)pItemInfo->m_wType, pItemInfo->m_LV );
//	}
//	else
//	{
//		dwPhyMinDamage = pItemInfo->m_wMinDamage;
//		dwPhyMaxDamage = pItemInfo->m_wMaxDamage;
//	}
//
//	dwPhyMinAttackPower = dwPhyMinDamage + WeightDamage;
//	dwPhyMaxAttackPower = dwPhyMaxDamage + WeightDamage;
//
//
//	// 물리공격력
//	if (dwPhyMinAttackPower || dwPhyMaxAttackPower)
//	{
//		//const int PHYATTACK_STRING_INDEX = 358;
//		GetInterfaceString(eST_PHYATTACK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//		Sprintf(szInterfaceString,_T("%s %ld-%ld"),szTemp,dwPhyMinAttackPower,dwPhyMaxAttackPower);			
//		
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemMagicAttackPowerForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	//DWORD dwMagicMinAttackPower = pItemInfo->m_wMagicAttMinDamgage;
//	//DWORD dwMagicMaxAttackPower = pItemInfo->m_wMagicAttMaxDamgage;
//	DWORD dwMagicMinAttackPower = 0;
//	DWORD dwMagicMaxAttackPower = 0;
//
//	DWORD WeightDamage = CalcAttackPower( pItemSlot->GetEnchant(), pItemInfo->m_LV );
//
//	if(pItemSlot->IsDivine())
//	{
//		dwMagicMinAttackPower = pItemInfo->m_wDivMagicMinDamage;
//		dwMagicMaxAttackPower = pItemInfo->m_wDivMagicMaxDamage;
//	}
//	else
//	{
//		dwMagicMinAttackPower = pItemInfo->m_wMagicAttMinDamgage;
//		dwMagicMaxAttackPower = pItemInfo->m_wMagicAttMaxDamgage;
//	}
//
//	dwMagicMinAttackPower = dwMagicMinAttackPower + WeightDamage; 
//	dwMagicMaxAttackPower = dwMagicMaxAttackPower + WeightDamage; 
//
//	// 마법 공격력
//	if (dwMagicMinAttackPower || dwMagicMaxAttackPower)
//	{
//		GetInterfaceString(eST_MAGATTACK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//		Sprintf(szInterfaceString,_T("%s %ld-%ld"),szTemp,dwMagicMinAttackPower,dwMagicMaxAttackPower);
//
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemPhyDefenseForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	// 물리 방어력
//	//DWORD dwPhyDefense = pItemInfo->m_wPhyDef;
//	DWORD dwPhyDefense = 0;
//	//int iCalcValue = CalcPhyDef( pItemInfo->m_wPhyDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//
//	if( pItemSlot->IsDivine())
//	{
//		//	디바인 아이템
//		//dwPhyDefense = iCalcValue + pItemInfo->m_wDivPhyDef;
//		dwPhyDefense = CalcPhyDef( pItemInfo->m_wDivPhyDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//	}
//	else
//	{
//		//dwPhyDefense = iCalcValue;
//		dwPhyDefense = CalcPhyDef( pItemInfo->m_wPhyDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//	}
//
//	
//	if( dwPhyDefense )
//	{
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//		GetInterfaceString(eST_PHY_DEFENSE_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//		Sprintf(szInterfaceString,_T("%s %ld"), szTemp, dwPhyDefense);
//
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemMagicDefenseForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	// 마법 방어력
//	//DWORD dwMagicDefense = pItemInfo->m_wMagicDef;
//	DWORD dwMagicDefense = 0;
//	//int iCalcValue = CalcPhyDef( pItemInfo->m_wMagicDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//
//	if( pItemSlot->IsDivine() )
//	{
//		//	디바인 아이템
//		//dwMagicDefense = iCalcValue + pItemInfo->m_wDivMagicDef;			
//		dwMagicDefense = CalcPhyDef( pItemInfo->m_wDivMagicDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//	}
//	else
//	{
//		//dwMagicDefense = iCalcValue;
//		dwMagicDefense = CalcPhyDef( pItemInfo->m_wMagicDef, pItemSlot->GetEnchant(), pItemInfo->m_LV );
//	}
//	
//	//dwMagicDefense += iCalcValue;
//
//	if( dwMagicDefense )
//	{
//		const int MAG_DEFRATING_STRING_INDEX = 368;
//
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//		GetInterfaceString(MAG_DEFRATING_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//		Sprintf(szInterfaceString,_T("%s : %ld"), szTemp, dwMagicDefense);
//
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//	}
//}
//
////------------------------------------------------------------------------------
//float InterfaceManager::_getLimitItemDura(float maxDura)
//{
//	if(maxDura <= 0.f) return 0.f;
//
//	float rate = maxDura * c_fDuraRate / 100;
//
//	return rate;
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemDurabilityForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	float	CurrentDura = pItemSlot->GetDura();
//	if( CurrentDura > pItemInfo->m_Dura )
//	{
//		CurrentDura = pItemInfo->m_Dura;
//	}
//	GetInterfaceString(eST_DURABILITY_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//	Sprintf(szInterfaceString,_T("%s %.0f / %d"),szTemp, CurrentDura, pItemInfo->m_Dura);
//
//	if(_getLimitItemDura((float)pItemInfo->m_Dura) > pItemSlot->GetDura())
//	//if( pItemSlot->GetfDura() == 0.f )
//	{
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzErrorColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//	else
//	{
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitStrInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	//---------------------------------------------------------------------------------------------
//	// 힘 제한
//	//---------------------------------------------------------------------------------------------
//	
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if (pItemInfo->IsWeapon())
//	{
//		WORD wLimitStrWeapon = 0;
//
//		WORD wLimitStr = pItemInfo->m_wLimitStr;
//		if ( wLimitStr > 0 )
//		{
//			if (pItemSlot->IsDivine() )
//			{
//				//wLimitStr = CalcLimitStrForDivineWeapon(pItemInfo->m_wLimitStr, pItemInfo->GetFirstAvailableClass(), (eITEMTYPE)pItemInfo->m_wType);
//				wLimitStr = pItemInfo->m_wDivLimitStr;
//			}
//
//			wLimitStrWeapon = CalcLimitStatWeapon(wLimitStr, iEnchant, (eRANK)iRank, pItemInfo->m_LV);
//
//			if (wLimitStrWeapon)
//			{
//				//if ( ItemManager::Instance()->GetSTR(pCharInfo) < wLimitStrWeapon )
//				if ( pCharInfo->GetSTR() < wLimitStrWeapon )
//				{
//					wzTextColor = WzErrorColor;
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//				
//				//const int LIMITSTR_STRING_INDEX = 11003;
//				GetInterfaceString(eST_LIMITSTR_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"),szTemp,wLimitStrWeapon);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//	else if (pItemInfo->IsArmor())
//	{
//		WORD wLimitStrArmor = 0;
//		WORD LimitStr = pItemInfo->m_wLimitStr;
//		if ( LimitStr > 0)
//		{
//			//if( pItemSlot->GetSet() == 1 )
//			if(pItemSlot->IsDivine())
//			{
//				// 디바인 아이템이다.
//				//LimitStr = CalcLimitStrForDivineArmor( pItemInfo->m_wLimitStr, pItemInfo->GetFirstAvailableClass(), pItemInfo->m_LV );
//				LimitStr = pItemInfo->m_wDivLimitStr;
//			}
//
//			wLimitStrArmor = CalcLimitStatArmor(LimitStr, iEnchant, (eRANK)iRank, pItemInfo->m_LV);
//
//			if (wLimitStrArmor)
//			{
//				//if ( ItemManager::Instance()->GetSTR(pCharInfo) < wLimitStrArmor )
//				if ( pCharInfo->GetSTR() < wLimitStrArmor )
//				{
//					wzTextColor = WzErrorColor;
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				//const int LIMITDEX_STRING_INDEX = 11003;
//				GetInterfaceString(eST_LIMITSTR_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, wLimitStrArmor);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitDexInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	//---------------------------------------------------------------------------------------------
//	// 민첩제한. 
//	//---------------------------------------------------------------------------------------------
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if (pItemInfo->IsWeapon())
//	{
//		WORD wLimitDex = pItemInfo->m_wLimitDex;
//		if ( wLimitDex > 0)
//		{
//			WORD wLimitDexWeapon = 0;
//
//			if (pItemSlot->IsDivine() )
//			{					
//				//wLimitDex = CalcLimitDexForDivineWeapon(pItemInfo->m_wLimitDex, pItemInfo->GetFirstAvailableClass(), (eITEMTYPE)pItemInfo->m_wType);
//				wLimitDex = pItemInfo->m_wDivLimitDex;
//			}
//
//			wLimitDexWeapon = CalcLimitStatWeapon(wLimitDex, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//
//			if (wLimitDexWeapon)
//			{
//				//if ( ItemManager::Instance()->GetDEX(pCharInfo) < wLimitDexWeapon )
//				if ( pCharInfo->GetDEX() < wLimitDexWeapon )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				//const int LIMITDEX_STRING_INDEX = 11004;
//				GetInterfaceString(eST_LIMITDEX_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString,_T("%s %ld"),szTemp,wLimitDexWeapon);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//	else if (pItemInfo->IsArmor())
//	{
//		WORD wLimitDex = pItemInfo->m_wLimitDex;
//		if ( wLimitDex )
//		{
//			//if (pItemSlot->GetSet() == 1 )
//			if(pItemSlot->IsDivine())
//			{					
//				//wLimitDex = CalcLimitDexForDivineArmor(wLimitDex, pItemInfo->GetFirstAvailableClass(), pItemInfo->m_LV);
//				wLimitDex = pItemInfo->m_wDivLimitDex;
//			}
//
//			wLimitDex = CalcLimitStatArmor(wLimitDex, iEnchant, (eRANK)iRank, pItemInfo->m_LV);
//
//			if (wLimitDex)
//			{
//				//if ( ItemManager::Instance()->GetDEX(pCharInfo) < wLimitDex )
//				if ( pCharInfo->GetDEX() < wLimitDex )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				const int LIMITDEX_STRING_INDEX = 11004;
//				GetInterfaceString(eST_LIMITDEX_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, wLimitDex);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//
//	}
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitIntInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	//---------------------------------------------------------------------------------------------
//	// 지력 제한
//	//---------------------------------------------------------------------------------------------
//
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if (pItemInfo->IsWeapon())
//	{
//		WORD wLimitInt = pItemInfo->m_wLimitInt;
//		if( wLimitInt > 0 ) 
//		{
//			if( pItemSlot->IsDivine() )
//			{
//				//wLimitInt = CalcLimitIntForDivineWeapon(wLimitInt, pItemInfo->GetFirstAvailableClass(), (eITEMTYPE)pItemInfo->m_wType );
//				wLimitInt = pItemInfo->m_wDivLimitInt;
//			}
//			wLimitInt = CalcLimitStatWeapon(wLimitInt, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//
//			if ( wLimitInt )
//			{
//				//if ( ItemManager::Instance()->GetINT(pCharInfo) < wLimitInt )
//				if ( pCharInfo->GetINT() < wLimitInt )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				//const int LIMITINT_STRING_INDEX = 11017;
//				GetInterfaceString(eST_LIMITINT_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, wLimitInt);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//	else if (pItemInfo->IsArmor())
//	{
//		WORD wLimitInt = pItemInfo->m_wLimitInt;
//		if ( wLimitInt > 0)
//		{
//			//if( pItemSlot->GetSet() == 1 )
//			if(pItemSlot->IsDivine())
//			{
//				//wLimitInt = CalcLimitIntForDivineArmor(wLimitInt, pItemInfo->GetFirstAvailableClass(), pItemInfo->m_LV);
//				wLimitInt = pItemInfo->m_wDivLimitInt;
//			}
//
//			wLimitInt = CalcLimitStatArmor(wLimitInt, iEnchant , (eRANK)iRank, pItemInfo->m_LV);
//
//			if (wLimitInt)
//			{
//				//if ( ItemManager::Instance()->GetINT(pCharInfo) < wLimitInt )
//				if ( pCharInfo->GetINT() < wLimitInt )
//				{
//					wzTextColor = WzErrorColor;
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				//const int LIMITINT_STRING_INDEX = 11017;
//				GetInterfaceString(eST_LIMITINT_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, wLimitInt);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitVitInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	//---------------------------------------------------------------------------------------------
//	// 체력 제한
//	//---------------------------------------------------------------------------------------------
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if (pItemInfo->IsWeapon())
//	{
//		WORD LimitVit = pItemInfo->m_wLimitVit;
//		if(LimitVit > 0)
//		{
//			if(pItemSlot->IsDivine())
//			{
//				//LimitInt = CalcLimitIntForDivineWeapon( LimitVit, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType );
//				LimitVit = pItemInfo->m_wDivLimitVit;
//			}
//			
//			LimitVit = CalcLimitStatWeapon( LimitVit, iEnchant, (eRANK)iRank, pItemSlot->GetItemInfo()->m_LV );
//
//			if ( LimitVit )
//			{
//
//    			//if ( ItemManager::Instance()->GetVIT(pCharInfo) < LimitVit )
//				if ( pCharInfo->GetVIT() < LimitVit )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				GetInterfaceString(eST_LIMITVIT_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, LimitVit);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//	else if (pItemInfo->IsArmor())
//	{
//		WORD LimitVit = pItemInfo->m_wLimitVit;
//		if(LimitVit > 0)
//		{
//			if(pItemSlot->IsDivine())
//			{
//				LimitVit = pItemInfo->m_wDivLimitVit;
//			}
//			
//			LimitVit = CalcLimitStatArmor( LimitVit, iEnchant, (eRANK)iRank, pItemSlot->GetItemInfo()->m_LV );
//
//			if ( LimitVit )
//			{
//				//if ( ItemManager::Instance()->GetVIT(pCharInfo) < LimitVit )
//				if ( pCharInfo->GetVIT() < LimitVit )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				GetInterfaceString(eST_LIMITVIT_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, LimitVit);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//		}
//	}
//
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitSprInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	//---------------------------------------------------------------------------------------------
//	// 정신력 제한
//	//---------------------------------------------------------------------------------------------
//
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if (pItemInfo->IsWeapon())
//	{
//		WORD LimitSpr = pItemInfo->m_wLimitSpr;
//		if(LimitSpr > 0)
//		{
//			if( pItemSlot->IsDivine() )
//			{
//				LimitSpr = pItemInfo->m_wDivLimitSpr;
//			}
//			
//			LimitSpr = CalcLimitStatWeapon( LimitSpr, iEnchant, (eRANK)iRank, pItemSlot->GetItemInfo()->m_LV );
//
//			if ( LimitSpr )
//			{
//				//if ( ItemManager::Instance()->GetSPR(pCharInfo) < LimitSpr )
//				if ( pCharInfo->GetSPR() < LimitSpr )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				GetInterfaceString(eST_LIMITSPR_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, LimitSpr);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//
//		}
//	}
//	else if (pItemInfo->IsArmor())
//	{
//		WORD LimitSpr = pItemInfo->m_wLimitSpr;
//		if(LimitSpr > 0)
//		{
//			if( pItemSlot->IsDivine() )
//			{
//				LimitSpr = pItemInfo->m_wDivLimitSpr;
//			}
//
//			LimitSpr = CalcLimitStatArmor( LimitSpr, iEnchant, (eRANK)iRank, pItemSlot->GetItemInfo()->m_LV );
//
//			if ( LimitSpr )
//			{
//				//if ( ItemManager::Instance()->GetSPR(pCharInfo) < LimitSpr )
//				if ( pCharInfo->GetSPR() < LimitSpr )
//				{
//					wzTextColor = WzErrorColor; 
//				}
//				else
//				{
//					wzTextColor = WzNormalColor;						
//				}
//
//				GetInterfaceString(eST_LIMITSPR_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//				Sprintf(szInterfaceString, _T("%s %ld"), szTemp, LimitSpr);
//
//				RegisterTooltip( szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					wzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//				return TRUE;
//			}
//
//		}
//	}
//
//	return FALSE;
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_addItemLimitSkillInfoForTooltip(SCItemSlot* pItemSlot, BASE_ITEMINFO *pItemInfo, PlayerAttributes *pCharInfo, int iCompositeType /*=0*/)
//{
//	if(!g_pHero)
//		return FALSE;
//	//---------------------------------------------------------------------------------------------
//	// 숙련도 제한
//	//---------------------------------------------------------------------------------------------
//
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	WzColor wzTextColor = WzNormalColor;
//	BOOL bAddLine = FALSE;
//
//	// iCompositeType: 1은 랭크업, 2는 인챈트
//	int iRank = pItemSlot->GetRank();
//	int iEnchant = pItemSlot->GetEnchant();
//
//	if(iCompositeType == 1)
//	{
//		++iRank;
//	}
//	else if(iCompositeType == 2)
//	{
//		++iEnchant;
//	}
//
//	if( pItemInfo->IsWeapon() || pItemInfo->IsArmor() )
//	{
//		const int	c_iMax_EqClass = 5;
//		int	iItemEqClass[c_iMax_EqClass] = { pItemInfo->m_wEqClass1, pItemInfo->m_wEqClass2, pItemInfo->m_wEqClass3, pItemInfo->m_wEqClass4, pItemInfo->m_wEqClass5 };
//		const int	c_iNumLimitSkill = 2;
//		const int	c_iLimitSkillNCode = 11162;
//		int			iLimitSkill1 = 0;
//		int			iLimitSkill2 = 0;
//		int			iLimitEqClass = 0;
//		for( int i = 0 ; i < c_iMax_EqClass ; ++i )
//		{
//			if( iItemEqClass[i] )
//			{
//				iLimitEqClass = i;
//				break;
//			}
//		}
//
//		//---------------------------------------------------------------------------------------------
//		//	첫번째
//		//---------------------------------------------------------------------------------------------
//		if( pItemInfo->m_wLimitSkil1 )
//		{
//			WORD	wLimitSkill1 = pItemInfo->m_wLimitSkil1;
//			if( pItemInfo->IsWeapon() )
//			{
//				if( pItemSlot->IsDivine() )
//				{
//					wLimitSkill1 = pItemInfo->m_wDivLimitSkill1;
//				}
//				wLimitSkill1 = CalcLimitStatWeapon( wLimitSkill1, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//			}
//			else if( pItemInfo->IsArmor() )
//			{
//				if( pItemSlot->IsDivine() )
//				{
//					wLimitSkill1 = pItemInfo->m_wDivLimitSkill1;
//				}
//				wLimitSkill1 = CalcLimitStatArmor( wLimitSkill1, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//			}
//
//			iLimitSkill1 = c_iLimitSkillNCode + (iLimitEqClass * c_iNumLimitSkill);
//
//			GetInterfaceString( iLimitSkill1, szTemp, INTERFACE_STRING_LENGTH );
//			Sprintf( szInterfaceString, szTemp, wLimitSkill1 );
//
//			//WORD wExpComp = ItemManager::Instance()->GetExperty1(pCharInfo);
//			WORD wExpComp = pCharInfo->GetExperty1();
//
//			if( g_pHero->GetClass() != (iLimitEqClass + 1) || wExpComp < wLimitSkill1 )
//			{
//				wzTextColor = WzErrorColor;
//			}
//			else
//			{
//				wzTextColor = WzNormalColor;
//			}
//
//			RegisterTooltip( szInterfaceString,
//				-1,
//				iNormalLineHeight,
//				FontID_ItemInfo,
//				wzTextColor,
//				TOOLTIP_BG_WZCOLOR,
//				0);
//
//			bAddLine = TRUE;
//		}
//
//		//---------------------------------------------------------------------------------------------
//		//	두번째
//		//---------------------------------------------------------------------------------------------
//		if( pItemInfo->m_wLimitSkil2 )
//		{
//			WORD	wLimitSkill2 = pItemInfo->m_wLimitSkil2;
//			if( pItemInfo->IsWeapon() )
//			{
//				if( pItemSlot->IsDivine() )
//				{
//					wLimitSkill2 = pItemInfo->m_wDivLimitSkill2;
//				}
//				wLimitSkill2 = CalcLimitStatWeapon( wLimitSkill2, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//			}
//			else if( pItemInfo->IsArmor() )
//			{
//				if( pItemSlot->IsDivine() )
//				{
//					wLimitSkill2 = pItemInfo->m_wDivLimitSkill2;
//				}
//				wLimitSkill2 = CalcLimitStatArmor( wLimitSkill2, iEnchant, (eRANK)iRank, pItemInfo->m_LV );
//			}
//
//			iLimitSkill2 = c_iLimitSkillNCode + (iLimitEqClass * c_iNumLimitSkill) + 1;
//
//			GetInterfaceString( iLimitSkill2, szTemp, INTERFACE_STRING_LENGTH );
//			Sprintf( szInterfaceString, szTemp, wLimitSkill2 );
//
//			//WORD wExpComp2 = ItemManager::Instance()->GetExperty2(pCharInfo);
//			WORD wExpComp2 = pCharInfo->GetExperty2();
//
//			if( g_pHero->GetClass() != (iLimitEqClass + 1) || wExpComp2 < wLimitSkill2 )
//			{
//				wzTextColor = WzErrorColor;
//			}
//			else
//			{
//				wzTextColor = WzNormalColor;
//			}
//
//			RegisterTooltip( szInterfaceString,
//				-1,
//				iNormalLineHeight,
//				FontID_ItemInfo,
//				wzTextColor,
//				TOOLTIP_BG_WZCOLOR,
//				0);
//
//			bAddLine = TRUE;
//		}
//
//	}
//
//	return bAddLine;
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemLimitInfoForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	if(!g_pHero)
//		return;
//
//	if( pItemInfo->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP )
//	{
//		PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();
//		if(!pCharInfo) return;
//
//		BOOL bAddLine = FALSE;
//
//		if(_addItemLimitStrInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))		bAddLine = TRUE;
//
//		if(_addItemLimitDexInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))		bAddLine = TRUE;
//
//		if(_addItemLimitIntInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))		bAddLine = TRUE;
//
//		if(_addItemLimitVitInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))		bAddLine = TRUE;
//
//		if(_addItemLimitSprInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))		bAddLine = TRUE;
//			
//		if(_addItemLimitSkillInfoForTooltip(pItemSlot, pItemInfo, pCharInfo))	bAddLine = TRUE;
//
//		if(bAddLine) _addEmptyLineForTooltip();
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemNextGradeLimitInfoForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo, int iCompositeType)
//{
//	if(!g_pHero)
//		return;
//
//	if( pItemInfo->m_byMaterialType == eITEM_MATERIAL_TYPE_CAN_EQUIP )
//	{
//		PlayerAttributes *	pCharInfo = g_pHero->GetPlayerAttribute();
//		if(!pCharInfo) return;
//
//		BOOL bAddLine = FALSE;
//
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		WzColor wzTextColor = WzNormalColor;
//	
//		//다음 등급 제한 수치
//		GetInterfaceString( eST_NEXT_GRADE_LIMIT_STRING_INDEX, szInterfaceString, INTERFACE_STRING_LENGTH );
//		this->RegisterTooltip(	szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			wzTextColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		
//		if(_addItemLimitStrInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))		bAddLine = TRUE;
//
//		if(_addItemLimitDexInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))		bAddLine = TRUE;
//
//		if(_addItemLimitIntInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))		bAddLine = TRUE;
//
//		if(_addItemLimitVitInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))		bAddLine = TRUE;
//
//		if(_addItemLimitSprInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))		bAddLine = TRUE;
//			
//		if(_addItemLimitSkillInfoForTooltip(pItemSlot, pItemInfo, pCharInfo, iCompositeType))	bAddLine = TRUE;
//
//		if(bAddLine) _addEmptyLineForTooltip();
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemAccOptionForTooltip(WORD iOptionKind, WORD iOption)
//{
//	int iStringIndex = 0;
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(iOptionKind);
//	if(pOption)
//	{
//		iStringIndex = pOption->mdwNameCode;
//	}
//
//	if(iStringIndex)
//	{
//		g_InterfaceManager.GetInterfaceString( iStringIndex, szTemp, INTERFACE_STRING_LENGTH );
//		Sprintf( szInterfaceString, _T("%s +%d"), szTemp, iOption);
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzEffectColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//}
//
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemNativeOptionForTooltip(WORD iOptionKind, WORD iOption)
//{
//	int iStringIndex = 0;
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};
//
//	BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(iOptionKind);
//	if(pOption)
//	{
//		iStringIndex = pOption->mdwNameCode;
//	}
//
//	if(iStringIndex)
//	{
//		g_InterfaceManager.GetInterfaceString( eST_NATIVE_OPTION_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
//		g_InterfaceManager.GetInterfaceString( iStringIndex, szTemp2, INTERFACE_STRING_LENGTH );
//		Sprintf( szTemp3,szTemp2, iOption);
//		Sprintf( szInterfaceString, _T("%s %s"), szTemp, szTemp3);
//		RegisterTooltip( szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzEffectColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemOptionInfoForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo)
//{
//	//---------------------------------------------------------------------------------------------
//	//		악세사리 옵션
//	//---------------------------------------------------------------------------------------------
//	//if( pItemInfo->IsAccessory() )
//	//{
//	//	if( pItemInfo->m_wOptionKind1 > 0 )
//	//	{		
//	//		_addEmptyLineForTooltip();
//
//	//		_addItemAccOptionForTooltip(pItemInfo->m_wOptionKind1, pItemInfo->m_wOption1);
//
//	//		if( pItemInfo->m_wOptionKind2 > 0 )
//	//		{
//	//			_addItemAccOptionForTooltip(pItemInfo->m_wOptionKind2, pItemInfo->m_wOption2);
//	//		}
//	//		
//	//		_addEmptyLineForTooltip();
//	//	}
//	//}
//	//---------------------------------------------------------------------------------------------
//	//		무기,방어구,악세사리의 옵션 수치
//	//---------------------------------------------------------------------------------------------
//	//else if( pItemInfo->IsArmor() || pItemInfo->IsWeapon() )
//	//if( pItemInfo->IsArmor() || pItemInfo->IsWeapon() || pItemInfo->IsAccessory())
//	{
//		BOOL bAddLine = FALSE;
//		//---------------------------------------------------------------------------------------------
//		//		고유 옵션
//		//---------------------------------------------------------------------------------------------
//
//		//if( pItemInfo->IsAccessory() )
//		if( pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
//		{
//			if( pItemInfo->m_wOptionKind1 > 0 )
//			{		
//				//_addEmptyLineForTooltip();
//
//				_addItemAccOptionForTooltip(pItemInfo->m_wOptionKind1, pItemInfo->m_wOption1);
//
//				if( pItemInfo->m_wOptionKind2 > 0 )
//				{
//					_addItemAccOptionForTooltip(pItemInfo->m_wOptionKind2, pItemInfo->m_wOption2);
//				}
//				
//				//_addEmptyLineForTooltip();
//
//				bAddLine = TRUE;
//			}
//		}
//		else
//		{
//			if( pItemInfo->m_wOptionKind1 > 0 )
//			{		
//				_addItemNativeOptionForTooltip(pItemInfo->m_wOptionKind1, pItemInfo->m_wOption1);		
//
//				if( pItemInfo->m_wOptionKind2 > 0 )
//				{
//					_addItemNativeOptionForTooltip(pItemInfo->m_wOptionKind2, pItemInfo->m_wOption2);
//				}
//
//				bAddLine = TRUE;
//			}
//		}
//
//		TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//
//		//---------------------------------------------------------------------------------------------
//		//		엑스트라 스톤 효과 옵션
//		//---------------------------------------------------------------------------------------------
//		int iAttrValue = pItemSlot->GetNOptionAttrValue();
//		const sEXTRASTONE_OPT_INFO *pExtraStoneInfo = pItemSlot->GetNOptionAttrDesc();
//		if(iAttrValue && pExtraStoneInfo) 
//		{
//			GetInterfaceString(pExtraStoneInfo->m_dwOptionNCode, szTemp, INTERFACE_STRING_LENGTH);
//
//			if( TYPE_NUMERIC == pExtraStoneInfo->m_byValueType )
//			{
//				Sprintf(szInterfaceString, _T("%s +%d"), szTemp, iAttrValue);
//			}
//			else
//			{
//				Sprintf(szInterfaceString, _T("%s +%d%%"), szTemp, iAttrValue);
//			}
//			
//			RegisterTooltip(szInterfaceString,
//					-1,
//					iNormalLineHeight,
//					FontID_ItemInfo,
//					WzEffectColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//			bAddLine = TRUE;
//		}
//
//		//---------------------------------------------------------------------------------------------
//		//		랭크 옵션
//		//---------------------------------------------------------------------------------------------
//		//BYTE byValue = 0;
//		//eATTR_TYPE attr;
//
//		if (RANK_E < pItemSlot->GetRank())
//		{
//			//GetInterfaceString((eST_D_RANKOPTION_STRING_INDEX + (int)pItemSlot->GetRank()), szTemp, INTERFACE_STRING_LENGTH );	// ex) D랭크 옵션: 
//
//			for ( BYTE rank = (BYTE)RANK_D; rank <= pItemSlot->GetRank(); ++rank)
//			{
//				// 랭크 속성이 존재한다.
//				BYTE byValue = pItemSlot->GetRankAttrValue((eRANK)rank);
//
//				const sRANK_OPTION * pRankOption = pItemSlot->GetRankAttrDesc( (eRANK)rank );
//				if (pRankOption)
//				{
//					GetInterfaceString((eST_D_RANKOPTION_STRING_INDEX + (int)rank - 1), szTemp, INTERFACE_STRING_LENGTH );	// ex) D랭크 옵션: 	
//					GetInterfaceString( pRankOption->m_OptionNCode, szTemp2, INTERFACE_STRING_LENGTH);
//
//					if( TYPE_NUMERIC == pRankOption->m_NumericType )
//					{
//						Sprintf(  szInterfaceString, _T("%s %s +%d"), szTemp, szTemp2, byValue);
//					}
//					else
//					{
//						// 퍼센트 
//						Sprintf(  szInterfaceString, _T("%s %s +%d%%"), szTemp, szTemp2, byValue);
//					}
//
//					RegisterTooltip(szInterfaceString,
//						-1,
//						iNormalLineHeight,
//						FontID_ItemInfo,
//						WzEffectColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//
//					bAddLine = TRUE;
//				}
//			}
//		}
//
//		//---------------------------------------------------------------------------------------------
//		//		소켓 옵션
//		//---------------------------------------------------------------------------------------------
//		for( int sock = SOCKET_1 ; sock < pItemSlot->GetSocketNum() ; ++sock )
//		{
//			BYTE byValue = pItemSlot->GetSocketAttrValue((eSOCKET)sock );
//			if( pItemSlot->GetSocketAttrIndex((eSOCKET)sock) == 0 ) continue;
//
//			GetInterfaceString(eST_SOCKET1_OPTION_STRING_INDEX + sock, szTemp, INTERFACE_STRING_LENGTH );	// ex) 소켓1 옵션: 
//
//			const sSOCKET_OPTION * pSocketOption = pItemSlot->GetSocketAttrDesc((eSOCKET)sock);
//			if ( byValue && pSocketOption )
//			{
//				GetInterfaceString(pSocketOption->m_NCode, szTemp2, INTERFACE_STRING_LENGTH);
//
//				if( TYPE_NUMERIC == pSocketOption->m_NumericType )
//				{
//					Sprintf(szInterfaceString, _T("%s %s +%d"), szTemp, szTemp2, byValue);
//				}
//				else
//				{
//					Sprintf(szInterfaceString, _T("%s %s +%d%%"), szTemp, szTemp2, byValue);
//				}
//				
//				//RegisterTooltipText(szTemp);
//				RegisterTooltip(szInterfaceString,
//						-1,
//						iNormalLineHeight,
//						FontID_ItemInfo,
//						WzEffectColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//
//				bAddLine = TRUE;
//			}
//		}
//
//		if(bAddLine) _addEmptyLineForTooltip();
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemDescForTooltip(BASE_ITEMINFO *pItemInfo)
//{
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//
//	//	소모성 아이템은 설명문과 효과
//	//if( pItemInfo->IsCanUseWaste() )
//	//if(pItemInfo->IsPotion())			
//	if( pItemInfo->m_byWasteType > 0)	// 물약은 색을 좀 다르게
//	{
//		_addEmptyLineForTooltip();
//
//		GetInterfaceString(pItemInfo->m_TDNCode,szInterfaceString,INTERFACE_STRING_LENGTH);
//		RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
//								iNormalLineHeight,
//								FontID_ItemInfo,
//								WzEffectColor,			
//								TOOLTIP_BG_WZCOLOR,
//								0);
//
//		//this->RegisterTooltip( szInterfaceString,
//		//	-1,
//		//	iTitleLineHeight,
//		//	FontID_12,
//		//	WzEffectColor,
//		//	TOOLTIP_BG_WZCOLOR,
//		//	0);
//
//		_addEmptyLineForTooltip();
//	}
//	else if( _isEtcItem(pItemInfo) )	//	재료및 기타 아이템
//	{
//		_addEmptyLineForTooltip();
//
//		GetInterfaceString(pItemInfo->m_TDNCode,szInterfaceString,INTERFACE_STRING_LENGTH);
//
//		RegisterTooltipLineChange(szInterfaceString, _tcslen(szInterfaceString),
//								iNormalLineHeight,
//								FontID_ItemInfo,
//								WzDescColor,
//								TOOLTIP_BG_WZCOLOR,
//								0);
//
//		//this->RegisterTooltip( szInterfaceString,
//		//	-1,
//		//	iTitleLineHeight,
//		//	FontID_12,
//		//	WzDescColor,
//		//	TOOLTIP_BG_WZCOLOR,
//		//	0);
//
//		_addEmptyLineForTooltip();
//	}
//
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEtcItem(BASE_ITEMINFO *pItemInfo)
//{
//	//if( pItemInfo->m_wType == eITEMTYPE_ENCHANT || pItemInfo->m_wType == eITEMTYPE_CRYSTAL 
//	//	|| pItemInfo->m_wType == eITEMTYPE_SOCKET || pItemInfo->m_wType == eITEMTYPE_ARROW
//	//	|| pItemInfo->m_wType == eITEMTYPE_SOCKETMATERIAL || pItemInfo->m_wType == eITEMTYPE_ACCESSORYMATERIAL
//	//	|| pItemInfo->m_wType == eITEMTYPE_SACCESSORYMATERIAL )
//	if((pItemInfo->m_wType >= eITEMTYPE_WASTE) && (pItemInfo->m_wType <= eITEMTYPE_RANDOM))
//	{
//		return TRUE;
//	}
//
//	return FALSE;
//	
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addItemPriceForTooltip(SCItemSlot* pItemSlot,BASE_ITEMINFO *pItemInfo, BOOL bBuyPrice/* = TRUE*/)
//{
//
//	//---------------------------------------------------------------------------------------------
//	//		가격
//	//---------------------------------------------------------------------------------------------
//	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	MONEY PriceOfGoods = 0;
//
//	//if (dwFlags & c_dwTOOLTIP_ITEM_INFO_ADINFO_PRICE)
//	if(bBuyPrice)		// 구매가격
//	{	
//		// 구매가격		
//		PriceOfGoods = pItemSlot->GetPriceForBuy();
//		GetInterfaceString(eST_BUY_PRICE_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);			
//
//		Sprintf( szInterfaceString, szTemp, PriceOfGoods );
//		
//		RegisterTooltip(szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		_addEmptyLineForTooltip();
//	}
//	else// if (dwFlags & c_dwTOOLTIP_ITEM_INFO_ADINFO_PRICE_USED)
//	{
//		// 판매가격
//		PriceOfGoods = pItemSlot->GetPriceForSell();
//		GetInterfaceString(eST_SELL_PRICE_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
//
//		Sprintf( szInterfaceString, szTemp, PriceOfGoods );
//		
//		RegisterTooltip(szInterfaceString,
//			-1,
//			iNormalLineHeight,
//			FontID_ItemInfo,
//			WzNormalColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//
//		_addEmptyLineForTooltip();
//	}
//
//	
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addEmptyLineForTooltip()
//{
//	TCHAR szMessage[32] = {0,};
//	szMessage[0] = _T('A');
//	szMessage[1] = NULL;
//	this->RegisterTooltip( szMessage,
//		-1,
//		iSpaceLineHeight,
//		FontID_8,
//		WzSpaceColor,
//		TOOLTIP_BG_WZCOLOR,
//		0);
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleNameForTooltip(BASE_STYLEINFO* pStyleInfo)
//{
//	TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
//	
//	GetInterfaceString( pStyleInfo->m_dwStyleNCode, szText, INTERFACE_STRING_LENGTH );
//	
//	RegisterTooltip(szText,
//		-1,
//		iLargeFontHeight,
//		FontID_SkillName,
//		WzSkillNameColor,
//		TOOLTIP_BG_WZCOLOR,
//		0);
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	// 레벨 또는 스타일
//
//	DWORD	dwTooltipExtraStyle = 0;
//	TCHAR	szText[INTERFACE_STRING_LENGTH]={0,};
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH]={0,};
//	
//	WzColor textColor = WzSkillDisableColor;
//	if(bActivated) textColor = WzCurSkillDescColor;
//
//	//	같은 라인에 오른쪽 정렬해서 레벨 출력
//	BIT_ADD( dwTooltipExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE );
//	BIT_ADD( dwTooltipExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT );
//
//	if(pStyleInfo->m_wStyleLV == 0)
//	{
//		const int c_iStyleString = 409;
//		GetInterfaceString( c_iStyleString, szMessage, INTERFACE_STRING_LENGTH );
//	}
//	else
//	{
//		GetInterfaceString( eST_LEVEL, szText, INTERFACE_STRING_LENGTH );	//	레벨
//		Sprintf( szMessage, _T("%d%s"), pStyleInfo->m_wStyleLV, szText );
//
//	}
//
//	RegisterTooltip(szMessage,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					textColor,
//					TOOLTIP_BG_WZCOLOR,
//					dwTooltipExtraStyle);
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleCurLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(!bActivated) return;
//	if(pStyleInfo->m_wStyleLV == 0) return;
//
//	WzColor textColor = WzCurSkillLevelColor;
//	
//	_addEmptyLineForTooltip();
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	GetInterfaceString( eST_CUR_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
//	Sprintf( szString, szTemp, pStyleInfo->m_wStyleLV, pStyleInfo->m_wMaxLV);
//	
//	RegisterTooltip(szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					textColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleNextLevelForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(pStyleInfo->m_wStyleLV == pStyleInfo->m_wMaxLV)
//		return;
//
//	if(pStyleInfo->m_wStyleLV == 0) return;
//
//	_addEmptyLineForTooltip();
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	WzColor textColor = WzSkillDisableColor;
//
//	GetInterfaceString( eST_NEXT_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
//
//	if(bActivated)
//	{
//		textColor = WzNextSkillLevelColor;
//		//Sprintf( szString, szTemp, pSkillInfo->m_wSkillLv + 1, pSkillInfo->m_wMaxLv);
//	}
//	//else
//	
//	Sprintf( szString, szTemp, pStyleInfo->m_wStyleLV, pStyleInfo->m_wMaxLV);
//	
//	RegisterTooltip(szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					textColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSyleNeedsForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(!g_pHero)
//		return;
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//	레벨 제한
//	if(pStyleInfo->m_wRequireLV > 0)
//	{
//		WzColor	WzTextColor = WzNextSkillColor;
//
//		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
//
//		if(g_pHero->GetCharInfo()->m_LV < pStyleInfo->m_wRequireLV)		
//			WzTextColor = WzSkillErrorColor;	// 레벨에 못미치면 
//		else if(!bActivated)
//		{
//			WzTextColor = WzSkillDisableColor;
//
//			if(bActivated && (pStyleInfo->m_wStyleLV > 1))
//			{
//				WzTextColor = WzNextSkillColor;
//			}
//		}
//		else
//			WzTextColor = WzNextSkillColor;
//
//		
//		GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX	, szText, INTERFACE_STRING_LENGTH );
//		Sprintf( szMessage, "%s %d", szText, pStyleInfo->m_wRequireLV);
//		
//		RegisterTooltip(szMessage,
//						-1,
//						iNormalLineHeight,
//						FontID_SkillInfo,
//						WzTextColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//		
//	}
//
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//	필요스킬 포인트
//	if(g_pHero->GetCharInfo()->m_iRemainSkill < pStyleInfo->m_byRequireSkillPoint)	// 스킬 포인트가 부족할 때만 표시
//	{
//		const int	iRequireSkillPoint = 70228;		//	필요 스킬 포인트: %d
//
//		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
//
//		WzColor	WzTextColor = WzSkillErrorColor;
//
//		//	필요 스킬 포인트: %d
//		GetInterfaceString( iRequireSkillPoint, szText, INTERFACE_STRING_LENGTH );
//		Sprintf( szMessage, szText, pStyleInfo->m_byRequireSkillPoint);
//		
//		// 요구치에 만족하는지 확인 후, 텍스트 색 바꿀것
//		RegisterTooltip(szMessage,
//						-1,
//						iNormalLineHeight,
//						FontID_SkillInfo,
//						WzTextColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleLimitWeaponForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated, BOOL bQuickSlot)
//{
//	if(!g_pHero)
//		return;
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//
//	WzColor WzTextColor = WzSkillErrorColor;
//
//	GetInterfaceString( eST_LIMIT_WEAPON_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
//	
//	const int	iWeaponName = 30000;	//	무기 이름 시작( 양손 도끼 )
//
//	if( pStyleInfo->m_iWeaponDefine > 0 && pStyleInfo->m_iWeaponDefine < eITEMTYPE_PUNCH )
//	{
//		GetInterfaceString( (iWeaponName + pStyleInfo->m_iWeaponDefine), szMessage, INTERFACE_STRING_LENGTH );
//	}
//	else
//	{
//		//GetInterfaceString( eST_NONE_TEXT, szMessage, INTERFACE_STRING_LENGTH );
//		// 무기 제한 없으면 출력 안함
//		return;
//	}
//
//	if(g_pHero->GetWeaponKind() == pStyleInfo->m_iWeaponDefine)
//	{
//		WzTextColor = WzCurSkillColor;
//	}
//
//	Sprintf(szString, szTemp, szMessage);
//		
//	if(!bActivated)
//	{
//		WzTextColor = WzSkillDisableColor;
//	
//		if(pStyleInfo->m_wStyleLV > 1)
//		{
//			if(g_pHero->GetWeaponKind() == pStyleInfo->m_iWeaponDefine)
//			{
//				WzTextColor = WzNextSkillColor;
//			}
//		}
//	}
//	
//	RegisterTooltip( szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					WzTextColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addStyleDescForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(pStyleInfo->m_dwStyleDescCode > 0)
//	{
//		WzColor textColor = WzSkillDisableColor;
//
//		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//
//		if(bActivated) 
//			textColor = WzCurSkillDescColor;
//		else
//		{	
//			if(pStyleInfo->m_wStyleLV > 1)
//			{
//				textColor = WzNextSkillDescColor;
//			}	
//		}
//
//
//		GetInterfaceString(pStyleInfo->m_dwStyleDescCode, szString, INTERFACE_STRING_LENGTH);
//
//		RegisterTooltipLineChange(szString, 
//			_tcslen(szString),
//			iNormalLineHeight,
//			FontID_SkillInfo,
//			textColor,
//			TOOLTIP_BG_WZCOLOR,
//			0);
//	}
//}
//
////------------------------------------------------------------------------------
//BOOL InterfaceManager::_isEnableShowTooltip()
//{
//	if( !g_pHero )
//	{
//		return FALSE;
//	}
//
//	if( g_pHero->IsShowFullMap() )
//	{
//		return FALSE;
//	}
//
//	//	아이템을 들고 있으면 처리하지 않고 리턴
//	if (MouseHandler::Instance()->IsExistItemAtHand())
//		return FALSE;
//
//	return TRUE;
//}
////------------------------------------------------------------------------------
//void InterfaceManager::RegisterTooltipStyle(BASE_STYLEINFO* curStyleInfo, BASE_STYLEINFO* nextStyleInfo,bool isActivated)
//{
//	if(!_isEnableShowTooltip()) return;
//
//	if (!curStyleInfo)
//    {
//        assert ("스타일 정보가 반드시 필요합니다. (curStyleInfo == NULL)");
//        return;
//    }
//
//	InitTooltip();
//
//	_addStyleNameForTooltip(curStyleInfo);
//	_addStyleLevelForTooltip(curStyleInfo, isActivated);
//
//	//if(isActivated)
//	//{
//	//	_addStyleCurLevelForTooltip(curStyleInfo, isActivated);
//	//	
//	//	_addStyleLimitWeaponForTooltip(curStyleInfo, isActivated, FALSE);
//	//	_addEmptyLineForTooltip();
//	//	_addStyleDescForTooltip(curStyleInfo, isActivated);
//
//	//	if(nextStyleInfo)
//	//	{
//	//		_addStyleNextLevelForTooltip(nextStyleInfo, isActivated);
//
//	//		_addSyleNeedsForTooltip(nextStyleInfo, isActivated);
//
//	//		_addStyleLimitWeaponForTooltip(nextStyleInfo, isActivated, FALSE);
//	//		_addEmptyLineForTooltip();
//	//		_addStyleDescForTooltip(nextStyleInfo, isActivated);
//	//	}
//	//}
//	//else
//	//{	
//	//	_addStyleNextLevelForTooltip(curStyleInfo, isActivated);
//	//	
//	//	_addSyleNeedsForTooltip(curStyleInfo, isActivated);
//	//	
//	//	_addStyleLimitWeaponForTooltip(curStyleInfo, isActivated, FALSE);
//	//	_addEmptyLineForTooltip();
//	//	_addStyleDescForTooltip(curStyleInfo, isActivated);
//
//	//}
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
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_setStyleCurLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(bActivated)
//	{
//		_addStyleCurLevelForTooltip(pStyleInfo, bActivated);
//		
//		_addStyleLimitWeaponForTooltip(pStyleInfo, bActivated, FALSE);
//		_addEmptyLineForTooltip();
//		_addStyleDescForTooltip(pStyleInfo, bActivated);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_setStyleNextLevInfoForTooltip(BASE_STYLEINFO* pStyleInfo, BOOL bActivated)
//{
//	if(pStyleInfo)
//	{
//		_addStyleNextLevelForTooltip(pStyleInfo, bActivated);
//		
//		_addSyleNeedsForTooltip(pStyleInfo, bActivated);
//		
//		_addStyleLimitWeaponForTooltip(pStyleInfo, bActivated, FALSE);
//		_addEmptyLineForTooltip();
//		_addStyleDescForTooltip(pStyleInfo, bActivated);
//	}
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_registerSkillslotInfoTooltip(const SCSlot *pSlot, BOOL bQuickSlot)
//{
//	if(!_isEnableShowTooltip())
//		return;
//
//    DWORD _dwFlags = 0;
//
//	SkillScriptInfo* pSkillInfo =
//        (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo(pSlot->GetCode());
//
//	
//	assert (pSkillInfo);
//	
//	if(!pSkillInfo) 
//		return;
//
//	if(pSkillInfo->m_SkillCode >= eACTION_START_SKILL_CODE && 
//		pSkillInfo->m_SkillCode < (eEMOTION_START_CODE + 1000) )
//	{
//		//_dwFlags = c_dwTOOLTIP_INFO_ETC_SKILL;
//		RegisterTooltipSkillEtc(pSkillInfo);
//	}
//	else if(bQuickSlot)
//	{
//		//_dwFlags = c_dwTOOLTIP_INFO_SKILL_QUICKSLOT;
//		_addSkillNameForTooltip(pSkillInfo);
//		_addSkillLevelForTooltip(pSkillInfo);
//		_addSkillRemainCoolTimeForTooltip(pSkillInfo);
//		_setSkillBaseInfoForTooltip(pSkillInfo, TRUE, TRUE);
//		_addEmptyLineForTooltip();
//		_addSkillDescForTooltip(pSkillInfo, TRUE);
//	}
//	else
//	{
//		RegisterTooltipSkill(pSkillInfo);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillNameForTooltip(SkillScriptInfo* pSkillInfo)
//{
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//
//	//	스킬 이름
//	GetInterfaceString( pSkillInfo->m_dwSkillNcode, szString, INTERFACE_STRING_LENGTH );
//	this->RegisterTooltip(	szString,
//							-1,
//							iLargeFontHeight,
//							FontID_SkillName,
//							WzSkillNameColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillTypeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)	
//{
//	if( pSkillInfo->m_bySkillUserType != eSKILL_USER_ACTION && pSkillInfo->m_bySkillUserType != eSKILL_USER_EMOTICON )
//	{
//		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//		
//		WzColor textColor = WzSkillDisableColor;
//		if(bActivated)
//		{
//			textColor = WzCurSkillDescColor;
//		}
//
//		// 오른쪽 정렬
//		DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//		
//		GetInterfaceString( pSkillInfo->m_wSkillAttribute, szString, INTERFACE_STRING_LENGTH);
//		this->RegisterTooltip( szString,
//			-1,
//			iNameLineHeight,
//			FontID_SkillInfo,
//			textColor,
//			TOOLTIP_BG_WZCOLOR,
//			dwTooltipExtraStyle);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillLevelForTooltip(SkillScriptInfo* pSkillInfo)
//{
//	// 퀵슬롯에 나오는 정보 (오른쪽 정렬)
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE | c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//	
//	GetInterfaceString( eST_LEVEL, szTemp, INTERFACE_STRING_LENGTH );	//	레벨
//	Sprintf( szString, _T("%d%s"), pSkillInfo->m_wSkillLv, szTemp );
//	
//	RegisterTooltip(szString,
//				-1,
//				iNameLineHeight,
//				FontID_SkillInfo,
//				WzCurSkillDescColor,
//				TOOLTIP_BG_WZCOLOR,
//				dwTooltipExtraStyle);
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillRemainCoolTimeForTooltip(SkillScriptInfo* pSkillInfo)
//{
//	if(!g_pHero)
//		return;
//
//	// 퀵슬롯에 나오는 정보 (오른쪽 정렬)
//	TCHAR	szInterfaceString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	//	남은 쿨타임 : %d 초
//	
//	util::Timer *pTimer = g_pHero->GetSCCharacter().GetCoolTimer(pSkillInfo->m_SkillCode);
//	if (pTimer && !pTimer->IsExpired(FALSE))	// 쿨타임이 남았으면
//	{
//		DWORD dwTooltipExtraStyle = c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT;
//
//		float fRemainRate = 1.0f - pTimer->GetProgressRatio();
//		float fRemainTime = pSkillInfo->m_dwCoolTime * fRemainRate;
//
//		DWORD dwCoolTime = (fRemainTime / 1000);
//		GetInterfaceString( eST_REMAIN_COOLTIME_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
//		Sprintf(szInterfaceString, szTemp, dwCoolTime );
//
//		RegisterTooltip( szInterfaceString,
//						-1,
//						iNormalLineHeight,
//						FontID_SkillInfo,
//						WzNormalColor,
//						TOOLTIP_BG_WZCOLOR,
//						dwTooltipExtraStyle);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillNormalDescForTooltip(SkillScriptInfo* pSkillInfo)
//{
//	if(pSkillInfo->m_dwSkillDesccode > 0)
//	{
//		//_addEmptyLineForTooltip();
//
//		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//		
//		GetInterfaceString(pSkillInfo->m_dwSkillDesccode, szString, INTERFACE_STRING_LENGTH);
//
//		RegisterTooltipLineChange(szString,
//								_tcslen(szString),
//								iNormalLineHeight,
//								FontID_SkillInfo,
//								WzCurSkillDescColor,
//								TOOLTIP_BG_WZCOLOR,
//								0);
//	}
//
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillDescForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	//if(curSkillInfo->m_dwSkillDesccode > 0)
//	//{
//	//	WzColor textColor = WzDisableColor;
//	//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	//	
//	//	GetInterfaceString(curSkillInfo->m_dwSkillDesccode, szString, INTERFACE_STRING_LENGTH);
//
//	//	if(bActivated) textColor = WzDescColor;
//
//	//	RegisterTooltipLineChange(szString,
//	//							_tcslen(szString),
//	//							iNormalLineHeight,
//	//							FontID_14,
//	//							textColor,
//	//							TOOLTIP_BG_WZCOLOR,
//	//							0);
//	//}
//
//	if(pSkillInfo->m_dwSkillDesccode > 0)
//	{
//		pSkillInfo->SetFirst();
//		BASE_ABILITYINFO	*pAbilityInfo = pSkillInfo->GetNext();
//		int iTempAbility[21] = {0,};
//		int index=0;
//		while( pAbilityInfo != NULL )
//		{
//			int i= index * 4;
//			iTempAbility[1+i] = pAbilityInfo->m_iOption1;
//			iTempAbility[2+i] = pAbilityInfo->m_iOption2;
//			iTempAbility[3+i] = pAbilityInfo->m_iParam[0];
//			iTempAbility[4+i] = pAbilityInfo->m_iParam[1];
//			
//			pAbilityInfo = pSkillInfo->GetNext();
//			index++;
//		}
//
//		WzColor textColor = WzSkillDisableColor;
//
//		BASE_SkillTextInfo* pSkillTextInfo = SkillTextInfoParser::Instance()->GetSkillTextInfo(pSkillInfo->m_dwSkillDesccode);
//		if(pSkillTextInfo && pSkillTextInfo->m_dw_DescCode)
//		{
//			TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//			GetInterfaceString(pSkillTextInfo->m_dw_DescCode, szTemp, INTERFACE_STRING_LENGTH);
//
//			int iParam[16] = {0,};
//			int i=0;
//			int a=0;
//			for(int i=0; i<8; ++i)
//			{
//				if(a >= 16) break;
//
//				if(pSkillTextInfo->m_arrayParam[i].m_dw_AbilityCode)
//				{
//					int getIndex = pSkillTextInfo->m_arrayParam[i].m_dw_ParamNum;
//					if(TYPE_NUMERIC == pSkillTextInfo->m_arrayParam[i].m_dw_ParamType)		//1 수치, 2 퍼센트(10으로 나눈후 출력)
//					{
//						iParam[a] = iTempAbility[getIndex];
//						a++;
//					}
//					else
//					{
//						if(iTempAbility[getIndex])	// 소숫점 출력
//						{
//							iParam[a] = iTempAbility[getIndex] / 10;	
//							a++;
//
//							int point = (iTempAbility[getIndex] % 10);
//							if(point)
//							{
//								point = (point / 10);
//							}
//							
//							iParam[a] = abs(point);
//							a++;
//
//						}
//
//					}
//				}
//				else
//				{
//					break;
//				}
//			}
//
//			if(bActivated) 
//				textColor = WzCurSkillDescColor;
//			else
//			{
//				if(pSkillInfo->m_wSkillLv > 1)
//				{
//					textColor = WzNextSkillDescColor;
//				}
//			}
//
//		// 무식하지만 이렇게밖에는... ㅜㅜ
//			Sprintf(szString, szTemp, iParam[0], iParam[1], iParam[2], iParam[3], iParam[4], iParam[5], iParam[6], iParam[7],
//										iParam[8], iParam[9], iParam[10], iParam[11], iParam[12], iParam[13], iParam[14], iParam[15]);
//
//
//			RegisterTooltipLineChange(szString,
//									_tcslen(szString),
//									iNormalLineHeight,
//									FontID_SkillInfo,
//									textColor,
//									TOOLTIP_BG_WZCOLOR,
//									0);
//		}
//	}
//}
//
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillCurLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(!bActivated) return;
//
//	_addEmptyLineForTooltip();
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	
//	GetInterfaceString( eST_CUR_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
//	Sprintf( szString, szTemp, pSkillInfo->m_wSkillLv, pSkillInfo->m_wMaxLv);
//	
//	RegisterTooltip(szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					WzCurSkillLevelColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillNextLevelForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(pSkillInfo->m_wSkillLv > pSkillInfo->m_wMaxLv)
//		return;
//
//	_addEmptyLineForTooltip();
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	WzColor textColor = WzSkillDisableColor;
//
//	GetInterfaceString( eST_NEXT_SKILL_LEVEL_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );	
//
//	if(bActivated)
//	{
//		textColor = WzNextSkillLevelColor;
//		//Sprintf( szString, szTemp, pSkillInfo->m_wSkillLv + 1, pSkillInfo->m_wMaxLv);
//	}
//	else
//	{
//		if(pSkillInfo->m_wSkillLv > 1)
//		{
//			textColor = WzNextSkillLevelColor;
//		}
//	}
//	
//	Sprintf( szString, szTemp, pSkillInfo->m_wSkillLv, pSkillInfo->m_wMaxLv);
//	
//	RegisterTooltip(szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					textColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//	
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillNeedsForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)	
//{
//	if(!g_pHero)
//		return;
//
//	// 요구 숙련도
//	const int	iSkillType[eCLASS_ALL][2] = {
//												{ 11221, 11221 },	//	eCLASS_NONE
//												{ 420,     414 },	//	eCLASS_BERSERKER
//												{ 415,     413 },	//	eCLASS_DRAGON
//												{   0,       0 },	//	eCLASS_SHADOW
//												{ 416,     417 },	//	eCLASS_VALKYRIE
//												{ 418,     419 },	//	eCLASS_ELEMENTALIST
//											};
//
//	int	iSkillTypeIndex = 0;
//
//	WzColor	WzTextColor = WzNextSkillColor;
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
//
//	PlayerAttributes *pCharInfo = g_pHero->GetPlayerAttribute();
//
//    short sExpert1 = pCharInfo->GetExperty1();
//	short sExpert2 = pCharInfo->GetExperty2();
//
//	//if (dwFlags & c_dwTOOLTIP_INFO_REQUIRED_SKILLSTAT0)
//	{
//		if(pSkillInfo->m_wRequireSkillStat[0] > 0)
//		{
//			//	드래고닉, 오펜시브 등등 캐릭터에 맞게
//			GetInterfaceString( iSkillType[g_pHero->GetClass()][0], szString, INTERFACE_STRING_LENGTH );
//			//	필요 요구 %s : %d
//			GetInterfaceString( eST_REQ_SKILL_STAT_STRING_INDEX, szText, INTERFACE_STRING_LENGTH );
//			Sprintf( szMessage, szText, szString, pSkillInfo->m_wRequireSkillStat[0] );
//
//			// 요구치에 만족하는지 확인 후, 텍스트 색 바꿀것
//			if(sExpert1 < pSkillInfo->m_wRequireSkillStat[0])
//				WzTextColor = WzSkillErrorColor;
//			else if(!bActivated)
//			{
//				WzTextColor = WzSkillDisableColor;
//			
//				if(pSkillInfo->m_wSkillLv > 1)
//				{
//					WzTextColor = WzNextSkillColor;
//				}
//			}
//			else
//				WzTextColor = WzNextSkillColor;
//
//			RegisterTooltip(szMessage,
//							-1,
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							WzTextColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//		}
//	}
//	//else if (dwFlags & c_dwTOOLTIP_INFO_REQUIRED_SKILLSTAT1)
//	{
//		if(pSkillInfo->m_wRequireSkillStat[1] > 0)
//		{
//			//	드래고닉, 오펜시브 등등 캐릭터에 맞게
//			GetInterfaceString( iSkillType[g_pHero->GetClass()][1], szString, INTERFACE_STRING_LENGTH );
//			//	필요 요구 %s : %d
//			GetInterfaceString( eST_REQ_SKILL_STAT_STRING_INDEX, szText, INTERFACE_STRING_LENGTH );
//			Sprintf( szMessage, szText, szString, pSkillInfo->m_wRequireSkillStat[1]);
//
//			// 요구치에 만족하는지 확인 후, 텍스트 색 바꿀것
//			if(sExpert2 < pSkillInfo->m_wRequireSkillStat[1])
//				WzTextColor = WzSkillErrorColor;
//			else if(!bActivated)
//			{
//				WzTextColor = WzSkillDisableColor;
//			
//				if(pSkillInfo->m_wSkillLv > 1)
//				{
//					WzTextColor = WzNextSkillColor;
//				}
//			}
//			else
//				WzTextColor = WzNextSkillColor;
//
//			RegisterTooltip(szMessage,
//							-1,
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							WzTextColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//		}
//	}
//    //else if (dwFlags & c_dwTOOLTIP_INFO_REQUIRED_SKILLLEVEL)
//    {
//		if(pSkillInfo->m_wRequireLv > 0)
//		{
//			if(g_pHero->GetCharInfo()->m_LV < pSkillInfo->m_wRequireLv)		
//				WzTextColor = WzSkillErrorColor;	// 레벨에 못미치면 
//			else if(!bActivated)
//			{
//				WzTextColor = WzSkillDisableColor;
//				if(pSkillInfo->m_wSkillLv > 1)
//				{
//					WzTextColor = WzNextSkillColor;
//				}
//			}
//			else
//				WzTextColor = WzNextSkillColor;
//
//			//	레벨 제한: 
//			GetInterfaceString( eST_LIMITLEVEL_STRING_INDEX	, szText, INTERFACE_STRING_LENGTH );
//			Sprintf( szMessage, "%s %d", szText, pSkillInfo->m_wRequireLv);
//			
//			RegisterTooltip(szMessage,
//							-1,
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							WzTextColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//			
//		}
//    }
//	
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//	필요스킬 포인트
//	const int	iRequireSkillPoint = 70228;		//	필요 스킬 포인트: %d
//	//if (dwFlags & c_dwTOOLTIP_INFO_REQUIRED_SKILLPOINT)
//	{
//		if(g_pHero->GetCharInfo()->m_iRemainSkill < pSkillInfo->m_byRequireSkillPoint)	// 스킬 포인트가 부족할 때만 표시
//		{
//			WzTextColor = WzSkillErrorColor;
//
//			//	필요 스킬 포인트: %d
//			GetInterfaceString( iRequireSkillPoint, szText, INTERFACE_STRING_LENGTH );
//			Sprintf( szMessage, szText, pSkillInfo->m_byRequireSkillPoint);
//			
//			// 요구치에 만족하는지 확인 후, 텍스트 색 바꿀것
//
//			RegisterTooltip(szMessage,
//							-1,
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							WzTextColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//		}
//	}
//}
//
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillLimitWeaponForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot)
//{
//	if(!g_pHero)
//		return;
//
//	//---------------------------------------------------------------------------------------------
//	//		무기 제한
//	//---------------------------------------------------------------------------------------------
//	if( pSkillInfo->m_bySkillUserType != eSKILL_USER_ACTION && pSkillInfo->m_bySkillUserType != eSKILL_USER_EMOTICON )
//	{
//		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szTemp3[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//
//		WzColor		WzTextColor = WzSkillErrorColor;
//
//
//		GetInterfaceString( eST_LIMIT_WEAPON_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
//		
//		BOOL	bSetName = FALSE;
//		BOOL	bCheck = FALSE;
//		BOOL	bEquip = FALSE;
//
//		const int	iWeaponName = 30000;	//	무기 이름 시작
//		
//		for(int i = 0 ; i < 4 ; ++i )
//		{
//			if( pSkillInfo->m_iWeaponDefine[i] > 0 && pSkillInfo->m_iWeaponDefine[i] < eITEMTYPE_PUNCH )
//			{
//				if( i == 0 )
//				{
//					bSetName = TRUE;
//					this->GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp2, INTERFACE_STRING_LENGTH );
//				}
//				else if( bSetName )
//				{
//					this->GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp3, INTERFACE_STRING_LENGTH );
//					Sprintf(szMessage, _T(", %s"), szTemp3 );
//					_tcscat( szTemp2, szMessage );
//				}
//				else
//				{
//					bSetName = TRUE;
//					GetInterfaceString( (iWeaponName + pSkillInfo->m_iWeaponDefine[i]), szTemp2, INTERFACE_STRING_LENGTH );
//				}
//				bCheck = TRUE;
//
//				if(g_pHero->GetWeaponKind() == pSkillInfo->m_iWeaponDefine[i])
//				{
//					// 내가 장착한 무기와 일치하면
//					WzTextColor = WzCurSkillColor;
//					bEquip = TRUE;
//				}
//			}
//		}
//		//	없으면 출력안함
//		if( bCheck == FALSE )
//		{
//			return;
//		}
//
//		Sprintf(szString, szTemp, szTemp2);
//		
//
//		if(!bActivated)
//		{
//			WzTextColor = WzSkillDisableColor;
//
//			if(pSkillInfo->m_wSkillLv > 1)
//			{
//				WzTextColor = WzNextSkillColor;
//			}
//		}
//
//		if(bQuickSlot)
//		{
//			// 무기제한이 있는데 아무것도 무기장착을 안했다
//			if(bCheck && (g_pHero->GetWeaponKind() == eITEMTYPE_PUNCH))
//			{
//				//무기 장착 필요
//				GetInterfaceString( eST_NEED_WEAPON_STRING_INDEX, szString, INTERFACE_STRING_LENGTH );
//			}
//			else if(bCheck && bEquip)
//			{
//				return;	// 무기 장착 만족시엔 표시 안함
//			}
//		}
//
//		RegisterTooltip( szString,
//						-1,
//						iNormalLineHeight,
//						FontID_SkillInfo,
//						WzTextColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillCastingTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(!g_pHero)
//		return;
//
//	if( pSkillInfo->m_bySkillType != SKILL_TYPE_PASSIVE )
//	{
//		WzColor textColor = WzSkillDisableColor;
//		if(bActivated) 
//			textColor = WzCurSkillColor;
//		else
//		{
//			if(pSkillInfo->m_wSkillLv > 1)
//			{
//				textColor = WzNextSkillColor;
//			}
//		}
//
//		int iMyCharClassCode = g_pHero->GetCharInfo()->m_byClassCode;
//		if(eCHAR_ELEMENTALIST == iMyCharClassCode || eCHAR_VALKYRIE ==iMyCharClassCode )	// 엘리 or 발키리만 시전시간 표시(발키리는 소환수에 시전시간이 있음)
//		{
//			TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//			TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//			TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//
//			if( pSkillInfo->m_wSkillCasting > 0 )
//			{
//				int sec = (pSkillInfo->m_wSkillCasting / 1000);
//				int point = (pSkillInfo->m_wSkillCasting % 1000);
//				if(point)
//				{
//					point = (point / 100);
//					GetInterfaceString( 12015, szTemp, INTERFACE_STRING_LENGTH);	//시전시간:
//					GetInterfaceString( 11303, szTemp2, INTERFACE_STRING_LENGTH);	//초
//					Sprintf(szString, "%s %d.%d%s", szTemp, sec, point, szTemp2 );
//				}
//				else
//				{
//					GetInterfaceString( eST_CASTING_TIME_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
//					Sprintf(szString, szTemp, sec );
//				}
//			}
//			else
//			{
//				GetInterfaceString( eST_NONE_CASTING_TIME_STRING_INDEX, szString, INTERFACE_STRING_LENGTH);
//			}
//
//			RegisterTooltip( szString,
//								-1,
//								iNormalLineHeight,
//								FontID_SkillInfo,
//								textColor,
//								TOOLTIP_BG_WZCOLOR,
//								0);
//		}
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillCoolTimeForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)	// 쿨타임
//{
//	WzColor textColor = WzSkillDisableColor;
//	if(bActivated) 
//		textColor = WzCurSkillColor;
//	else
//	{
//		if(pSkillInfo->m_wSkillLv > 1)
//		{
//			textColor = WzNextSkillColor;
//		}
//	}
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};
//
//	if( pSkillInfo->m_dwCoolTime > 0 )
//	{
//		int sec = (pSkillInfo->m_dwCoolTime / 1000);
//		int point = (pSkillInfo->m_dwCoolTime % 1000);
//		if(point)
//		{
//			point = (point / 100);
//			GetInterfaceString( 12016, szTemp, INTERFACE_STRING_LENGTH);	//쿨타임:
//			GetInterfaceString( 11303, szTemp2, INTERFACE_STRING_LENGTH);	//초
//			Sprintf(szString, "%s %d.%d%s", szTemp, sec, point, szTemp2 );
//		}
//		else
//		{
//			GetInterfaceString( 11156, szTemp, INTERFACE_STRING_LENGTH);
//			Sprintf(szString, szTemp, sec );
//		}
//	
//		RegisterTooltip( szString,
//							-1,
//							iNormalLineHeight,
//							FontID_SkillInfo,
//							textColor,
//							TOOLTIP_BG_WZCOLOR,
//							0);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillDistanceForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if( pSkillInfo->m_wSkillRange > 0)
//	{
//		WzColor textColor = WzSkillDisableColor;
//		if(bActivated) 
//		{
//			textColor = WzCurSkillColor;
//		}
//		else
//		{
//			if(pSkillInfo->m_wSkillLv > 1)
//			{
//				textColor = WzNextSkillColor;
//			}
//		}
//
//		//스킬 사거리: %d미터
//		//const int c_iStringRange = 80004;	// 유효 거리: %d 미터
//		TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//		float fRange=((float)pSkillInfo->m_wSkillRange / 10.0f);
//
//		GetInterfaceString( eST_SKILL_RANGE_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
//		Sprintf(szString, szTemp, (int)fRange );
//
//		RegisterTooltip( szString,
//						-1,
//						iNormalLineHeight,
//						FontID_SkillInfo,
//						textColor,
//						TOOLTIP_BG_WZCOLOR,
//						0);
//	}
//	
//}	
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillSpendForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(!g_pHero)
//		return;
//
//	if(pSkillInfo->m_wMPSpend <= 0) return;
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
//
//	//const int iMPSpend = 80005;	//	MP 소모량: %d
//	//const int iSPSpend = 80006;	//	SP 소모량: %d
//
//	WzColor textColor = WzSkillDisableColor;
//	if(bActivated) 
//	{
//		textColor = WzCurSkillColor;
//	}
//	else
//	{
//		if(pSkillInfo->m_wSkillLv > 1)
//		{
//			textColor = WzNextSkillColor;
//		}
//	}
//
//	int iMyCharClassCode = g_pHero->GetCharInfo()->m_byClassCode;
//	if(eCHAR_BERSERKER == iMyCharClassCode || eCHAR_DRAGON == iMyCharClassCode)
//	{
//		GetInterfaceString( eST_SP_SPEND_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
//	}
//	else
//	{
//		GetInterfaceString( eST_MP_SPEND_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH);
//	}
//	
//	Sprintf(szString, "%s %d", szTemp, pSkillInfo->m_wMPSpend );
//	
//	RegisterTooltip( szString,
//					-1,
//					iNormalLineHeight,
//					FontID_SkillInfo,
//					textColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//
//}
//
////------------------------------------------------------------------------------
////void InterfaceManager::_addSkillDamageForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
////{
////	bool			bSpaceLine = false;
////	const int		c_iSkillAttackDamage = 1;
////	const int		c_iSkillDamageParam1 = 0;
////	
////	WzColor textColor = WzSkillDisableColor;
////	if(bActivated) textColor = WzCurSkillColor;
////
////	if( pSkillInfo )
////	{
////		pSkillInfo->SetFirst();
////		BASE_ABILITYINFO	*pAbilityInfo = pSkillInfo->GetNext();
////
////		while( pAbilityInfo != NULL )
////		{
////			if( pAbilityInfo->m_wAbilityID == c_iSkillAttackDamage )
////			{
////				TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
////				TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
////
////				// 스킬 공격력: 
////				GetInterfaceString( eST_SKILL_ATTACK_POWER_STRING_INDEX, szTemp, INTERFACE_STRING_LENGTH );
////				Sprintf( szString, "%s %d", szTemp, pAbilityInfo->m_iParam[c_iSkillDamageParam1] );
////
////				RegisterTooltip(	szString,
////					-1,
////					//_tcslen(szString),
////					iNormalLineHeight,
////					FontID_14,
////					textColor,
////					TOOLTIP_BG_WZCOLOR,
////					0);
////
////				break;
////			}
////
////			pAbilityInfo = pSkillInfo->GetNext();
////		}
////	}
////
////}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_addSkillEffectForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	//---------------------------------------------------------------------------------------------
//	//	추가 효과
//	//---------------------------------------------------------------------------------------------
//	WzColor textColor = WzSkillDisableColor;
//	if(bActivated)
//	{
//		textColor = WzCurSkillColor;
//	}
//	else
//	{
//		if(pSkillInfo->m_wSkillLv > 1)
//		{
//			textColor = WzNextSkillColor;
//		}
//	}
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
//	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
//
//    {
//        GetInterfaceString( eST_SKILL_EFFECT_STRING_INDEX, szText, INTERFACE_STRING_LENGTH );
//        Sprintf( szMessage, _T("%s "), szText );
//        util::_tstring strStates = szMessage;
//
//        std::vector<int>    viStateCodes;   // 추가 상태 코드들
//        viStateCodes.clear();
//
//        BASE_ABILITYINFO * pAbilityInfo = NULL ;
//        pSkillInfo->SetFirst();
//        while( pAbilityInfo = pSkillInfo->GetNext() )
//        {
//			if (pAbilityInfo->m_wStateID)
//			{
//				viStateCodes.push_back(pAbilityInfo->m_wStateID);
//			}
//        }
//
//        int _start, _end;
//
//        _start = 0;
//        _end = viStateCodes.size();
//        bool bInsertComma = false;  // 항목 사이 , 추가
//        bool bNonStates = true;     // 채울 항목이 없음
//
//        if (_end)
//        {
//			TCHAR	szStateName[INTERFACE_STRING_LENGTH] = {0,};
//
//            for (int i=0, j=0; i<_end; ++i)
//            {
//                int code = viStateCodes[i];
//                BASE_STATEINFO *bs = StateInfoParser::Instance()->GetStateInfo(code);
//                if (bs)
//                {
//                    if (j)
//                    {
//                        strStates += _T(", ");
//                    }
//
//					GetInterfaceString( bs->m_dwNamecode, szStateName, INTERFACE_STRING_LENGTH );
//					//strStates += szStateName;
//
//#ifdef	UNICODE
//						TCHAR	szWideString[INTERFACE_STRING_LENGTH];
//						MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStateName, -1, szWideString, -1 );
//						strStates += szWideString;
//#else
//						strStates += szStateName;
//#endif
//
//                    if (bNonStates)
//                        bNonStates = false;
//
//                    j++;
//                }
//            }
//        }
//
//      /*  if (bNonStates)
//        {
//            //	없음
//            this->GetInterfaceString( eST_NONE_TEXT, szText, INTERFACE_STRING_LENGTH );
//            strStates += szText;
//        }*/
//
//
//		if (!bNonStates)
//		{
//			//	공백 라인( 이뿌게 보이기 위해 라인 한 줄 띄운다 ) 띄우고 출력
//			//_addEmptyLineForTooltip();
//
//			RegisterTooltipLineChange((TCHAR *)strStates.c_str(),
//				_tcslen(strStates.c_str()),
//				iNormalLineHeight,
//				FontID_SkillInfo,
//				textColor,
//				TOOLTIP_BG_WZCOLOR,
//				0);
//		}
//
//    }
//}
////------------------------------------------------------------------------------
//void InterfaceManager::RegisterTooltipSkill(SkillScriptInfo* curSkillInfo, SkillScriptInfo* nextSkillInfo, bool isActivated)
//{
//	if(!_isEnableShowTooltip()) 
//		return;
//
//	if (!curSkillInfo)
//    {
//        assert ("스킬정보가 반드시 필요합니다. (prevSkillInfo == NULL)");
//        return;
//    }
//
//	InitTooltip();
//
//	_addSkillNameForTooltip(curSkillInfo);
//	_addSkillTypeForTooltip(curSkillInfo, isActivated);
//	
//	//if(isActivated)
//	//{
//	//	_addSkillCurLevelForTooltip(curSkillInfo, isActivated);
//	//	
//	//	_setSkillBaseInfoForTooltip(curSkillInfo, isActivated, FALSE);
//	//	_addEmptyLineForTooltip();
//	//	_addSkillDescForTooltip(curSkillInfo, isActivated);
//
//	//	if(nextSkillInfo)
//	//	{
//	//		_addSkillNextLevelForTooltip(nextSkillInfo, isActivated);
//
//	//		_addSkillNeedsForTooltip(nextSkillInfo, isActivated);
//	//		
//	//		_setSkillBaseInfoForTooltip(nextSkillInfo, isActivated, FALSE);
//	//		_addEmptyLineForTooltip();
//	//		_addSkillDescForTooltip(nextSkillInfo, isActivated);
//	//	}
//	//}
//	//else
//	//{
//	//	_addSkillNextLevelForTooltip(curSkillInfo, isActivated);
//
//	//	_addSkillNeedsForTooltip(curSkillInfo, isActivated);
//	//	
//	//	_setSkillBaseInfoForTooltip(curSkillInfo, isActivated, FALSE);
//	//	_addEmptyLineForTooltip();
//	//	_addSkillDescForTooltip(curSkillInfo, isActivated);
//	//}
//
//	_setSkillCurLevInfoForTooltip(curSkillInfo, isActivated);
//	if(isActivated)
//	{
//		_setSkillNextLevInfoForTooltip(nextSkillInfo, isActivated);
//	}
//	else
//	{
//		_setSkillNextLevInfoForTooltip(curSkillInfo, isActivated);
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::_setSkillBaseInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated, BOOL bQuickSlot)
//{
//	_addSkillLimitWeaponForTooltip(pSkillInfo, bActivated, bQuickSlot);
//	_addSkillCastingTimeForTooltip(pSkillInfo, bActivated);
//	_addSkillCoolTimeForTooltip(pSkillInfo, bActivated);
//	_addSkillDistanceForTooltip(pSkillInfo, bActivated);
//	_addSkillSpendForTooltip(pSkillInfo, bActivated);
//	_addSkillEffectForTooltip(pSkillInfo, bActivated);
//}
//
////----------------------------------------------------------------------------
//void InterfaceManager::_setSkillCurLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(bActivated)
//	{
//		_addSkillCurLevelForTooltip(pSkillInfo, bActivated);
//		
//		_setSkillBaseInfoForTooltip(pSkillInfo, bActivated, FALSE);
//		_addEmptyLineForTooltip();
//		_addSkillDescForTooltip(pSkillInfo, bActivated);
//	}
//}
//
////----------------------------------------------------------------------------
//void InterfaceManager::_setSkillNextLevInfoForTooltip(SkillScriptInfo* pSkillInfo, BOOL bActivated)
//{
//	if(pSkillInfo)
//	{
//		_addSkillNextLevelForTooltip(pSkillInfo, bActivated);
//
//		_addSkillNeedsForTooltip(pSkillInfo, bActivated);
//		
//		_setSkillBaseInfoForTooltip(pSkillInfo, bActivated, FALSE);
//		_addEmptyLineForTooltip();
//		_addSkillDescForTooltip(pSkillInfo, bActivated);
//	}
//}
//
////----------------------------------------------------------------------------
///**
//*/
//void InterfaceManager::RegisterTooltipSkillEtc(SkillScriptInfo* pSkillInfo)
//{
//	if(!_isEnableShowTooltip()) return;
//
//	if ( !pSkillInfo )
//	{
//		return;
//	}
//
//	InitTooltip();
//
//	_addSkillNameForTooltip(pSkillInfo);
//	_addSkillNormalDescForTooltip(pSkillInfo);
//}
//
//
////-------------------------------------------------------------------------------------
//void InterfaceManager::RegisterTooltipWithStringCode(int iStringCode)
//{
//	if(!_isEnableShowTooltip()) 
//		return;
//
//
//	InitTooltip();
//	//InitializeTooltipText();
//
//	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};
//	GetInterfaceString( iStringCode, szString, INTERFACE_STRING_LENGTH );
//
//	//RegisterTooltipText(szString, -1);
//	RegisterTooltip( szString,
//					-1,
//					iSimpleTooltipHeight,
//					FontID_12_1,
//					WzNormalColor,
//					TOOLTIP_BG_WZCOLOR,
//					0);
//}
//
////---------------------------------------------------------------------------------
//void  InterfaceManager::_registerItemslotInfoTooltip(const SCSlot *pSlot)
//{
//	if (!pSlot)
//	{
//		assert(pSlot);
//		return;
//	}
//	
//	assert( pSlot->GetSlotType() == ST_ITEM );
//
//	RegisterTooltipItem(pSlot, FALSE);
//}
//
////-----------------------------------------------------------------------------
//void InterfaceManager::_registerItemslotInfoTooltipForQuick(const SCSlot *pSlot)
//{
//	if (!pSlot) return;
//
//	assert(ST_ITEM == pSlot->GetSlotType() );
//
//	RegisterTooltipItem(pSlot, FALSE);
//}
//
////----------------------------------------------------------------------------------------
//void InterfaceManager::_registerItemslotInfoTooltipForQuickInDeactive(const SCSlot * pSlot)
//{
//	if (!pSlot) return;
//
//	assert(ST_QUICK == pSlot->GetSlotType() );
//	if (pSlot->GetSlotType() != ST_QUICK)
//	{
//		return;
//	}
//
//	QuickSlot * pQuickSlot = (QuickSlot *)pSlot;
//
//	static SCItemSlot TempItem;
//	TempItem.SetCode( pQuickSlot->GetOrgCode() );
//
//	RegisterTooltipItem(&TempItem, FALSE);
//}
//
////-----------------------------------------------------------------------------------
//void InterfaceManager::_registerQuickslotInfoTooltip(const SCSlot *pSlot)
//{
//	assert(pSlot);
//	assert(ST_QUICK == pSlot->GetSlotType());
//
//	if (!pSlot)
//	{
//		return;
//	}
//
//	QuickSlot * pQuickSlot = (QuickSlot * )pSlot;
//
//	DWORD _dwFlags = 0;
//
//	switch (pQuickSlot->GetOrgSlotType())
//	{
//	case ST_ITEM:
//		{	
//			switch (pQuickSlot->GetStatus())
//			{
//			case SLOT_RENDER_STATE_ACTIVATED:
//				{
//					SCSlotContainer * pFromContainer = ItemManager::Instance()->GetContainer(pQuickSlot->GetOrgSlotIndex());
//					if (!pFromContainer)
//					{
//						assert(pFromContainer);
//						return;
//					}
//
//					SCSlot & rSlot = pFromContainer->GetSlot(pQuickSlot->GetOrgPos());
//					if (rSlot.GetCode() != pQuickSlot->GetOrgCode())
//						return;
//
//					_registerItemslotInfoTooltipForQuick(&rSlot);
//				}
//				break;
//			case SLOT_RENDER_STATE_DEACTIVATED:
//				{
//					_registerItemslotInfoTooltipForQuickInDeactive(pQuickSlot);
//				}
//				break;
//			default:
//				break;
//			}		
//		}
//		break;
//
//	case ST_SKILL:
//		{
//			_registerSkillslotInfoTooltip(pQuickSlot->GetOrgSlot(), TRUE);
//		}
//		break;
//	}
//}
//
////---------------------------------------------------------------------------------
//void InterfaceManager::_registerStyleQuickslotInfoTooltip(const SCSlot *pSlot)
//{
//	assert(pSlot);
//	assert(ST_STYLE_QUICK == pSlot->GetSlotType());
//
//	if (!pSlot)
//	{
//		return;
//	}
//
//	StyleSlot * pStyleSlot = (StyleSlot * )pSlot;
//
//	if(pStyleSlot && ST_SKILL == pStyleSlot->GetOrgSlotType())
//	{
//		BASE_STYLEINFO * pStyleInfo = (BASE_STYLEINFO *)SkillInfoParser::Instance()->GetStyleInfo(pStyleSlot->GetOrgSlot()->GetCode());
//
//		if(pStyleInfo)
//		{
//			_addStyleNameForTooltip(pStyleInfo);
//			_addStyleLevelForTooltip(pStyleInfo, TRUE);
//
//			_addEmptyLineForTooltip();
//			_addStyleDescForTooltip(pStyleInfo, TRUE);
//		}
//	}
//	else
//	{
//		assert(!"의도되지 않은 사용이다.");
//	}
//}
//
////------------------------------------------------------------------------------
//void InterfaceManager::RegisterTooltipItem(const SCSlot *pSlot, BOOL bBuyPrice, int iCompositeType/* = 0*/)	// iCompositeType: 1이면 랭크업, 2면 인챈트
//{
//	if(!_isEnableShowItemTooltip(pSlot))
//		return;
//
//	InitTooltip();
//
//    BASE_ITEMINFO	*pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode());
//	SCItemSlot		*pItemSlot = (SCItemSlot *)pSlot;
//
//	//---------------------------------------------------------------------------------------------
//	//		1. 아이템 이름
//	//---------------------------------------------------------------------------------------------
//	_addItemNameForTooltip(pItemSlot, pItemInfo);
//
//	//---------------------------------------------------------------------------------------------
//	//		2. 랭크
//	//---------------------------------------------------------------------------------------------
//	if(_isEnableItemRankUp(pItemInfo))
//	{
//		_addItemRankForTooltip(pItemSlot, pItemInfo);
//	}
//
//	//---------------------------------------------------------------------------------------------
//	//		3. 소켓
//	//---------------------------------------------------------------------------------------------
//	if(_isEnableItemSocket(pItemInfo))
//	{
//		_addItemSocketForTooltip(pItemSlot, pItemInfo);
//	}
//
//	//---------------------------------------------------------------------------------------------
//	//		4. 아이템 종류
//	//---------------------------------------------------------------------------------------------
//	if(_isEnableItemType(pItemInfo))
//    {
//		_addItemTypeForTooltip(pItemInfo);
//	}
//
//	//---------------------------------------------------------------------------------------------
//	//		5. 기본정보
//	//---------------------------------------------------------------------------------------------
//	if(pItemInfo->IsWeapon())
//	{
//		_addItemEquipClassForTooltip(pItemInfo);
//		_addItemPhyAttackSpeedForTooltip(pItemInfo);
//		_addItemPhyAttackPowerForTooltip(pItemSlot, pItemInfo);
//		_addItemMagicAttackPowerForTooltip(pItemSlot, pItemInfo);
//		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);
//		_addEmptyLineForTooltip();	// 공백 한 줄 추가
//	}
//	else if(pItemInfo->IsArmor())
//	{
//		_addItemEquipClassForTooltip(pItemInfo);
//		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
//		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
//		_addItemDurabilityForTooltip(pItemSlot, pItemInfo);
//		_addEmptyLineForTooltip();	// 공백 한 줄 추가
//	}
//	else if(pItemInfo->IsSpecialAccessory())
//	{
//		_addItemEquipClassForTooltip(pItemInfo);
//		_addItemPhyDefenseForTooltip(pItemSlot, pItemInfo);
//		_addItemMagicDefenseForTooltip(pItemSlot, pItemInfo);
//		_addEmptyLineForTooltip();	// 공백 한 줄 추가
//	}
//
//	//---------------------------------------------------------------------------------------------
//	//		6. 제한수치
//	//---------------------------------------------------------------------------------------------
//	_addItemLimitInfoForTooltip(pItemSlot, pItemInfo);
//	_addItemLimitLevelForTooltip(pItemSlot, pItemInfo);
//
//	//---------------------------------------------------------------------------------------------
//	//		7. 옵션
//	//---------------------------------------------------------------------------------------------
//	if(_isEnableItemOption(pItemInfo))
//		_addItemOptionInfoForTooltip(pItemSlot, pItemInfo);
//
//	//---------------------------------------------------------------------------------------------
//	//		8. 설명
//	//---------------------------------------------------------------------------------------------
//	_addItemDescForTooltip(pItemInfo);
//
//
//	//---------------------------------------------------------------------------------------------
//	//		9. 다음 등급 제한 수치
//	//---------------------------------------------------------------------------------------------
//	if(iCompositeType)
//	{
//		// iCompositeType: 1은 랭크업, 2는 인챈트
//
//		if(iCompositeType == 1)
//		{
//			_addItemNextGradeLimitInfoForTooltip(pItemSlot, pItemInfo, iCompositeType);
//			_addItemNextGradeLimitLevelForTooltip(pItemSlot, pItemInfo);
//		}
//		else if(iCompositeType == 2)
//		{
//			_addItemNextGradeLimitInfoForTooltip(pItemSlot, pItemInfo, iCompositeType);
//		}
//	}
//
//	//---------------------------------------------------------------------------------------------
//	//		10. 가격
//	//---------------------------------------------------------------------------------------------
//	if (bBuyPrice)	// 구매가격
//	{
//		_addItemPriceForTooltip(pItemSlot, pItemInfo, TRUE);
//	}
//	//else if (dwFlags & c_dwTOOLTIP_ITEM_INFO_ADINFO_PRICE_USED)
//	else if (IsShowDialog(DIALOG_SHOP))
//    {
//		_addItemPriceForTooltip(pItemSlot, pItemInfo, FALSE);
//	}
//
//}
//
////----------------------------------------------------------------------------------
//void  InterfaceManager::RegisterSlotInfoTooltip(const SCSlot *pSlot)
//{
//	if (!pSlot)
//	{
//		assert(0);
//		return;
//	}
//	
//	switch(pSlot->GetSlotType())
//	{
//	case ST_ITEM:
//		{
//			_registerItemslotInfoTooltip(pSlot);
//		}
//		break;
//
//	case ST_SKILL:	// 스킬창
//		{
//			_registerSkillslotInfoTooltip(pSlot, FALSE);	
//		}
//		break;
//
//	case ST_QUICK:
//		{
//			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Use );
//			_registerQuickslotInfoTooltip(pSlot);	
//		}
//		break;
//
//	case ST_STYLE_QUICK:
//		{
//			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Use );
//			_registerStyleQuickslotInfoTooltip(pSlot);
//		}
//		break;
//	}
//
//	
//
//}
//
//void RenderItemText(TCHAR *szText,RECT *pRC,WzColor fontcolor)
//{
//	
//	int iTextYInterval = 20;
//
//	//g_pSunRenderer->x_pManagerTextOut->DrawText(szText,pRC,fontcolor,WzColor_RGBA(0,0,0,0),TP_HLEFT);
//
//	RenderShadowedText(szText,pRC,fontcolor);
//	//g_pSunRenderer->x_pManagerTextOut->DrawText(szText,pRC,fontcolor,WzColor_RGBA(0,0,0,0),TP_HLEFT);
//
//	pRC->top += iTextYInterval;
//	pRC->bottom += iTextYInterval;
//
//
//}
//
//void InterfaceManager::RegisterItemslotInfoTooltipForShop(const SCSlot *pSlot)
//{
//    BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(pSlot->GetCode());
//    if (!pItemInfo) return;
//
//	RegisterTooltipItem(pSlot, TRUE);
//}
//
//void
//InterfaceManager::InitTooltip()
//{
//	this->m_iTooltipCoutns = 0;
//
//	ZeroMemory(this->m_Tooltips, (sizeof(cTooltipInfo) * MAX_SLOTINFO_LINE));
//}
//
//void InterfaceManager::InitializeTooltipText()
//{
//	m_iSlotInfoLineCount = 0;
//}
//
//void InterfaceManager::RegisterTooltipText(TCHAR *szText,int iStrLength)
//{
//	assert(m_iSlotInfoLineCount < MAX_SLOTINFO_LINE && m_iSlotInfoLineCount >= 0);
//
//	if (m_iSlotInfoLineCount >= MAX_SLOTINFO_LINE )
//	{
//		return;
//	}
//
//	ZeroMemory(m_szSlotInfoMessage[m_iSlotInfoLineCount],MAX_SLOTINFO_LENGTH);
//
//	if (iStrLength == -1) {
//		_tcsncpy(m_szSlotInfoMessage[m_iSlotInfoLineCount],szText,MAX_SLOTINFO_LENGTH);
//	}
//	else
//	{
//		_tcsncpy(m_szSlotInfoMessage[m_iSlotInfoLineCount],szText,iStrLength);
//
//	}
//	m_iSlotInfoLineCount++;
//	
//}
//
//void InterfaceManager::RegisterTooltipTextLineChange(TCHAR *szText,int stringlength)
//{
//	int iCount = 0;
//	int iOldCount = 0;
//	int iCurSize = 0;
//
//	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );	
//
//	SIZE TextSize;
//	BOOL bDraw = FALSE;
//	BOOL bMultiByte = FALSE;
//	BOOL bOldMultiByte = FALSE;
//	
//	while(iCount < stringlength) {
//
//		if( _ismbblead(szText[iCount]))
//		{
//
//			TCHAR szBuf[2];
//
//			szBuf[0] = szText[iCount];
//			szBuf[1] = szText[iCount + 1];
//
//#ifdef	UNICODE
//			char	szMBString[INTERFACE_STRING_LENGTH];
//			WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, szBuf, 2, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMBString,2,&TextSize);
//#else
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szBuf,2,&TextSize);
//#endif
//			iCurSize += TextSize.cx;
//
//			
//			iCount += 2;	
//			bDraw = TRUE;
//			bMultiByte = TRUE;
//		}
// 		else		
//		{
//#ifdef	UNICODE
//			char	szMBString[INTERFACE_STRING_LENGTH];
//			WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, &szText[iCount], -1, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMBString,1,&TextSize);
//#else
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(&szText[iCount],1,&TextSize);
//#endif
//			iCurSize += TextSize.cx;
//
//			iCount++;
//			bDraw = TRUE;
//			bMultiByte = FALSE;
//		}
//
//		if (iCurSize >= (TOOLTIP_TEXT_WIDTH - 16) )
//		{
//			
//			RegisterTooltipText(&szText[iOldCount],(iCount - iOldCount));
//				
//			iOldCount = iCount;
//			bDraw = FALSE;
//			iCurSize = 0;
//		}
//
//	
//
//		
//
//		
//	}
//
//	// 마지막줄 넣어주기~
//	if (bDraw) {
//		RegisterTooltipText(&szText[iOldCount]);
//		
//		
//	}
//
//
//
//	
//	
//}
//
//void
//InterfaceManager::RegisterTooltipLineChange(TCHAR *szText,
//                                            int iBufSize,
//                                            int yInterval,
//                                            WzID fontID,
//                                            WzColor color,
//                                            WzColor shadowColor,
//											DWORD dwTooltipExtraStyle)
//{
//    int iCount = 0;
//    int iOldCount = 0;
//    int iCurSize = 0;
//
//    SIZE TextSize;
//    BOOL bDraw = FALSE;
//    BOOL bMultiByte = FALSE;
//    BOOL bOldMultiByte = FALSE;
//
//    while(iCount < iBufSize)
//	{
//        if( _ismbblead(szText[iCount]))
//        {
//
//            TCHAR szBuf[2];
//
//            szBuf[0] = szText[iCount];
//            szBuf[1] = szText[iCount + 1];
//
//#ifdef	UNICODE
//			char	szMBString[INTERFACE_STRING_LENGTH];
//			WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, szBuf, 2, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMBString,1,&TextSize);
//#else
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szBuf,2,&TextSize);
//#endif
//
//            iCurSize += TextSize.cx;
//
//            iCount += 2;	
//            bDraw = TRUE;
//            bMultiByte = TRUE;
//        }
//        else		
//        {
//#ifdef	UNICODE
//			char	szMBString[INTERFACE_STRING_LENGTH];
//			WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, &szText[iCount], -1, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szMBString,1,&TextSize);
//#else
//			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(&szText[iCount],1,&TextSize);
//#endif
//            iCurSize += TextSize.cx;
//
//            iCount++;
//            bDraw = TRUE;
//            bMultiByte = FALSE;
//        }
//
//		if (iCurSize >= (TOOLTIP_TEXT_WIDTH - 16) )
//        {
//            RegisterTooltip(&szText[iOldCount],
//                            (iCount - iOldCount),
//                            yInterval,
//                            fontID,
//                            color,
//                            shadowColor,
//							dwTooltipExtraStyle);
//
//            iOldCount = iCount;
//            bDraw = FALSE;
//            iCurSize = 0;
//        }
//
//
//
//
//
//
//    }
//
//    // 마지막줄 넣어주기~
//    if (bDraw) {
//        RegisterTooltip(&szText[iOldCount],
//            -1,
//            yInterval,
//            fontID,
//            color,
//            shadowColor,
//			dwTooltipExtraStyle);
//    }
//}
//
//void
//InterfaceManager::RenderTooltip(int x, int y, BOOL bRenderBackGround)
//{
//    if (this->m_iTooltipCoutns == 0)
//        return;
//
//#ifdef UI_RESOLUTION_VARIATION
//	g_pApplication->Set2DMode();
//#else 
//	g_pSunRenderer->Set2DMode(TRUE);
//#endif
//
//	WzID _fontID = DEFAULT_FONT_WZCOLOR;
//	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( _fontID );
//
//	const int	iTextGap = 20;
//    int sx,sy;
//	int iTextStartY = 10;
//
//    int width = TOOLTIP_WIDTH;	//	가로 양쪽 이쁜 여백 10씩
//	int height = iTextStartY;
//
//	int	iTempWidth = width;
//	int	iPreWidth = 0;
//	int	iPreHeight = 0;
//
//	int		iTextLength = 0;
//	SIZE	TextSize;
//    for (int a=0; a<this->m_iTooltipCoutns; ++a)
//    {
//        cTooltipInfo* tooltipInfo = &this->m_Tooltips[a];
//        assert(tooltipInfo);
//
//		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( tooltipInfo->m_fontID );
//		iTextLength = (int)_tcslen( tooltipInfo->m_szSlotInfoMessage );
//		iTextLength += 3;
//
//#ifdef	UNICODE
//		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, tooltipInfo->m_szSlotInfoMessage, -1, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( szMBString, iTextLength, &TextSize );
//#else
//		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( tooltipInfo->m_szSlotInfoMessage, iTextLength, &TextSize );
//#endif
//
//		if( iTempWidth < TextSize.cx )
//		{
//			iTempWidth = TextSize.cx;
//		}
//
//		if( BIT_CHECK( tooltipInfo->m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
//		{
//			//	가로 크기 검사( 라인 바꾸기가 이미 실행 되서 가로 크기를 여기서 바꾸면 안된다 )
//
//#ifdef	BUG_FIX_TOOLTIP_BOX_WIDTH
//			if( iTempWidth < (TextSize.cx + iTextGap + iPreWidth) )
//			{
//				iTempWidth = TextSize.cx + iTextGap + iPreWidth;
//			}
//#endif
//
//
//			//	현재 라인 크기가 이전 라인 크기보다 클때,
//			//	이전 라인 크기 대신에 현재 라이 크기를 더해서 크기를 구한다.
//			if( iPreHeight < tooltipInfo->m_iTextYInterval )
//			{
//				height += tooltipInfo->m_iTextYInterval - iPreHeight;
//			}
//
//			iPreHeight = height;
//		}
//		else
//		{
//			iPreHeight = height;
//			height += tooltipInfo->m_iTextYInterval;
//		}
//
//		iPreWidth = TextSize.cx;
//    }
//
//	//	위에서 계산된 가장 큰 가로 크기를 얻어온다
//	width = iTempWidth;
//
//	if( bRenderBackGround )
//	{
//		sx = x;
//		sy = y;
//	}
//	else
//	{
//		sx = x - (width / 2);
//		sy = y;
//	}
//
//
//	POINT screenpt = g_pApplication->Get2DModeMapping();
//
//    int windowxsize = screenpt.x;
//    int windowysize = screenpt.y - 18;	// 약간 아래쪽에 나오면 안이쁘니까 내려주자.
//
//    if (sx  >= windowxsize - width)
//    {
//        sx = windowxsize - width; 
//    }
//
//    if (sy  >= windowysize - height)
//    {
//		sy = windowysize - height; 
//    }
//
//    g_pSunRenderer->x_pManagerTextOut->StartToUseFont( _fontID );
//    RECT rc;
//    RECT textrc;
//
//    rc.left = sx;
//    rc.top = sy;
//    rc.right = sx + width + 4;
//    rc.bottom = sy + height;
//
//	if( bRenderBackGround )
//	{
//		RenderRect2(rc, WzColor_RGBA(0,0,30,160));
//	}
//
//    textrc.left = rc.left+10;
//    textrc.right = rc.right - 10;
//    textrc.top = rc.top + iTextStartY;
//	textrc.bottom = textrc.top + this->m_Tooltips[0].m_iTextYInterval - 10;
//
//	//	전체 툴팁 리스트를 돌면서 한개씩 처리
//	DWORD	dwTextStyle = 0;
//	int		ipreHeight = 0;
//    for (int a=0; a<this->m_iTooltipCoutns; ++a)
//    {
//        cTooltipInfo* tooltipInfo = &this->m_Tooltips[a];
//        assert(tooltipInfo);
//
//        if (tooltipInfo->m_fontID != _fontID)
//        {
//            _fontID = tooltipInfo->m_fontID;
//            g_pSunRenderer->x_pManagerTextOut->StartToUseFont( _fontID );
//        }
//
//		iTextLength = (int)_tcsclen( tooltipInfo->m_szSlotInfoMessage );
//
//#ifdef	UNICODE
//		WideCharToMultiByte( CP_ACP, WC_COMPOSITECHECK, tooltipInfo->m_szSlotInfoMessage, -1, szMBString, INTERFACE_STRING_LENGTH, NULL, NULL );
//		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( szMBString, iTextLength, &TextSize );
//#else
//		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( tooltipInfo->m_szSlotInfoMessage, iTextLength, &TextSize );
//#endif
//
//
//	   BOOL bTest = g_pSunRenderer->IsEnableZTest();
//        g_pSunRenderer->SetZBufferTest(FALSE);
//
//		//	텍스트 정렬 스타일
//		dwTextStyle = TP_HLEFT;
//		if( BIT_CHECK( tooltipInfo->m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_CENTER ) )
//		{
//			dwTextStyle = TP_HCENTER;
//		}
//		else if( BIT_CHECK( tooltipInfo->m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT ) )
//		{
//			dwTextStyle = TP_HRIGHT;
//		}
//
//		//	라인을 바꾸지 말라는 옵션이 있을 경우
//
//        
//        if( a > 0 && BIT_CHECK( tooltipInfo->m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
//		{
//			textrc.top -= ipreHeight;
//			textrc.bottom -= ipreHeight;
//		}
//
//		RECT _rc = textrc;
//		_rc.left -= 1;
//		_rc.right += 1;
//
////		g_pSunRenderer->x_pManagerTextOut->DrawText(tooltipInfo->m_szSlotInfoMessage,
//////                                                    _rc.left,
//////                                                    _rc.top,
////													&_rc,
////                                                    WzColor_RGBA(30,30,30,Alpha_WzColor(tooltipInfo->m_Color)),
////													WzColor_RGBA( 0, 0, 0, 0),
////													(dwTextStyle) );
//////													(dwTextStyle | TP_VCENTER) );
////        _rc.top -= 2;
////        _rc.left -= 2;
//        g_pSunRenderer->x_pManagerTextOut->DrawText(tooltipInfo->m_szSlotInfoMessage,
////                                                    _rc.left,
////                                                    _rc.top,
//													&_rc,
//                                                    tooltipInfo->m_Color,
//													WzColor_RGBA( 0, 0, 0, 0),
//													(dwTextStyle), 
//													ETS_OUTLINE);
////													(dwTextStyle | TP_VCENTER) );
//        g_pSunRenderer->SetZBufferTest(bTest);
//
//
//		//	라인을 바꾸지 말라는 옵션이 있을 경우
//		if( a > 0 && BIT_CHECK( tooltipInfo->m_dwExtraStyle, c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE ) )
//		{
//			//	이전 라인 크기와 지금 라인 크기중 더 큰것으로 더해준다
//			if( ipreHeight < tooltipInfo->m_iTextYInterval )
//			{
//				ipreHeight = tooltipInfo->m_iTextYInterval;
//			}
//		}
//		else
//		{
//			ipreHeight = tooltipInfo->m_iTextYInterval;
//		}
//
//		textrc.top += ipreHeight;
//		textrc.bottom += ipreHeight;
//    }
//
//	if(!m_bTooltipTestMode)
//	{
//		this->InitTooltip();
//	}
//
//}
//
//void InterfaceManager::RenderTooltipText(int x,int y)
//{
//	if (m_iSlotInfoLineCount == 0)
//	{
//		return;
//	}
//
//#ifdef UI_RESOLUTION_VARIATION
//	g_pApplication->Set2DMode();
//#else 
//	g_pSunRenderer->Set2DMode(TRUE);
//#endif
//
//	int sx,sy;
//	int width,height;
//
//	int iTextStartY = 10;
//	int iTextYInterval = 20;
//
//	WzColor BackColor = TOOLTIP_BG_WZCOLOR;
//	WzColor FontColor = TOOLTIP_FONT_WZCOLOR;
//
//	sx = x;
//	sy = y;
//	width = TOOLTIP_WIDTH+10;
//	height = iTextStartY + m_iSlotInfoLineCount * iTextYInterval;
//
//	// 1024에 맞추어 코딩을 하면 자동으로 수정됨
//	int windowxsize = 1024;
//	int windowysize = 750;	// 약간 아래쪽에 나오면 안이쁘니까 내려주자.
//
//	if (sx  >= windowxsize - width)
//	{
//		sx = windowxsize - width; 
//	}
//
//	if (sy  >= windowysize - height)
//	{
//		sy = windowysize - height; 
//	}
//
//
//	RECT rc;
//	rc.left = sx;
//	rc.top = sy;
//	rc.right = sx + width;
//	rc.bottom = sy + height;
//
//	RenderRect(rc,BackColor);
//
//	RECT textrc;
//
//	textrc.left = rc.left+10;
//	textrc.right = rc.right+10;
//	textrc.top = rc.top + iTextStartY;
//	textrc.bottom = textrc.top + iTextYInterval;
//	
//	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m212") );
//
//	for (int a = 0; a < m_iSlotInfoLineCount; a++)
//	{
//		RenderItemText(m_szSlotInfoMessage[a],&textrc,FontColor);
//	}
//
//    InitializeTooltipText();
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//InterfaceManager::RegisterTooltip(const cTooltipInfo& tooltipInfo)
//{
//
//	if (m_iTooltipCoutns >= MAX_SLOTINFO_LINE )
//	{
//		return;
//	}
//
//    this->m_Tooltips[this->m_iTooltipCoutns] = tooltipInfo;
//    this->m_iTooltipCoutns++;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//InterfaceManager::RegisterTooltip(TCHAR *szText, int iBufSize, int yInterval, WzID fontID, WzColor color, WzColor shadowColor, DWORD dwTooltipExtraStyle )
//{
//    assert(this->m_iTooltipCoutns < MAX_SLOTINFO_LINE && this->m_iTooltipCoutns >= 0);
//
//	if (!szText)
//	{
//		return;
//	}
//
//	if (m_iTooltipCoutns >= MAX_SLOTINFO_LINE )
//	{
//		return;
//	}
//
//
//    cTooltipInfo* pTooltipInfo = &this->m_Tooltips[this->m_iTooltipCoutns];
//
//    ZeroMemory(pTooltipInfo->m_szSlotInfoMessage, MAX_SLOTINFO_LENGTH);
//    
//    if (iBufSize == -1)
//    {
//        _tcsncpy(pTooltipInfo->m_szSlotInfoMessage, szText, MAX_SLOTINFO_LENGTH);
//    }
//    else
//    {
//        _tcsncpy(pTooltipInfo->m_szSlotInfoMessage, szText, iBufSize);
//    }
//
//    pTooltipInfo->m_fontID = fontID;
//    pTooltipInfo->m_Color = color;
//    pTooltipInfo->m_ShadowColor = shadowColor;
//    pTooltipInfo->m_iTextYInterval = yInterval;
//	pTooltipInfo->m_dwExtraStyle = dwTooltipExtraStyle;
//
//    this->m_iTooltipCoutns++;
//}
//
//void InterfaceManager::RegisterChannelInfoTooltip(TCHAR * pszMessage , int iJoinMemeber_Num )
//{
//	TCHAR szMessage[INTERFACE_STRING_LENGTH] , szResult[INTERFACE_STRING_LENGTH];
//	InitTooltip();
//	// 채널 명 : %s
//	g_InterfaceManager.GetInterfaceString(eST_CHANNEL_ROOM_NAME , szMessage , INTERFACE_STRING_LENGTH );
//	sprintf ( szResult , szMessage , pszMessage);
//	this->RegisterTooltip(	szResult,
//		-1,
//		iLargeFontHeight,
//		FontID_SkillName,
//		CHAT_CHANNEL_FONT_WZCOLOR,
//		TOOLTIP_BG_WZCOLOR,
//		0);
//
//	//참여 인원: %d
//	g_InterfaceManager.GetInterfaceString(eST_CHANNEL_JOIN_NUMBER , szMessage , INTERFACE_STRING_LENGTH );
//	sprintf( szResult , szMessage , iJoinMemeber_Num );
//	RegisterTooltipLineChange(szResult,
//		_tcslen(szResult),
//		iNormalLineHeight,
//		FontID_SkillInfo,
//		CHAT_CHANNEL_FONT_WZCOLOR,
//		TOOLTIP_BG_WZCOLOR,
//		0);
//}
//#endif //DEF_REDSKY_MODIFY_TOOLTIP_20060719
