#include "SunClientPrecompiledHeader.h"
#include "rewarddialog.h"
#include "MissionRewardParser.h"
#include "IconMgr.h"
#include "IconImage.h"
#include "ItemInfoParser.h"
#include "ResourceManager.h"
#include "SolarDialog.h"
#include "Mouse.h"
#include "SCItemSlot.h"
#include "Chatdialog.h"
#include "RewardInfoList.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include "ItemTypeList.h"
#include "ItemInfoParser.h"
#include "GameOptionParser.h"
#include "SceneBase.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TitleListParser.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

#include "Hero.h"

WzID RewardDialog::m_wzId[] = 
{
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    StrToWzID("C000"), //REWARDDIALOG_ITEM1_CHECK
    StrToWzID("C001"), //REWARDDIALOG_ITEM2_CHECK
    StrToWzID("C002"), //REWARDDIALOG_ITEM3_CHECK
    StrToWzID("C003"), //REWARDDIALOG_ITEM4_CHECK
    StrToWzID("C004"), //REWARDDIALOG_ITEM5_CHECK
    StrToWzID("TI00"), //REWARDDIALOG_OK_BUTTON
    StrToWzID("S013"), //REWARDDIALOG_MONEY_TEXT
    StrToWzID("T001"), //REWARDDIALOG_MONEY_VALUE_TEXT
    StrToWzID("T002"), //REWARDDIALOG_EXP_STRING_TEXT
    StrToWzID("T003"), //REWARDDIALOG_EXP_VALUE_TEXT
    StrToWzID("S003"), //REWARDDIALOG_TITLE_TEXT
    StrToWzID("S001"), //REWARDDIALOG_TITLE_VALUE_TEXT
    StrToWzID("P024"), //REWARDDIALOG_TITLE_PICTURE
    StrToWzID("LS00"), //REWARDDIALOG_MISSION_TEXT
    StrToWzID("S000"), //REWARDDIALOG_DIALOG_TITLE_TEXT
#else
	StrToWzID("B999"),
	StrToWzID("C000"),
	StrToWzID("C001"),
	StrToWzID("C002"),
	StrToWzID("C003"),
	StrToWzID("C004"),
	StrToWzID("T000"),
	StrToWzID("TI00"),
	StrToWzID("TI01"),
	StrToWzID("T001"),
	StrToWzID("T002"),
	StrToWzID("T003"),
	StrToWzID("BT00"),
	StrToWzID("LS00"),
	StrToWzID("T999"),	
#endif //_NA_003027_20111013_HONOR_SYSTEM
};

RewardDialog::RewardDialog(void)
{
	for (int a = 0; a < MAX_REWARD_ITEM; a++)
	{
		ZeroMemory(&m_RewardItem[a],sizeof(MISSION_REWARD_ITEM));
		
	}

	m_dwRewardIndex = 0;
	m_bSendPacket = FALSE;
	m_DialogKey = 0;
}

RewardDialog::~RewardDialog(void)
{
}


void RewardDialog::Init(CDrawBase *pBase)
{
   //UpdatePosition();
   RECT rc;
   rc.top = rc.left = rc.right = rc.bottom = 100;
   m_FTextRender.Init(g_pSunRenderer,rc);
   m_eRewardViewType = REWARD_VIEW_TYPE_NORMAL;
}


void RewardDialog::UpdateTitle()
{
	TCHAR szTitle[INTERFACE_STRING_LENGTH];

	CCtrlStaticWZ * pTitleControl = (CCtrlStaticWZ *)getControl(REWARDDIALOG_DIALOG_TITLE_TEXT);
	if ( pTitleControl )
	{		
		int iStringIndex = 4538;

		switch (m_eRewardViewType)
		{
		case REWARD_VIEW_TYPE_NORMAL:
			{
				// 4538 미션 보상
				iStringIndex = 4538;
			}			
			break;
		case REWARD_VIEW_TYPE_AC_PREVIEW:
			{
				// 90034 에어리어 컨퀘스트 보상 미리보기
				iStringIndex = 90034;
			}
			break;
		case REWARD_VIEW_TYPE_AC_REWARD:
			{
				// 952 보상을 선택해 주십시오.
				iStringIndex = 952;
			}
			break;		
		}

		g_InterfaceManager.GetInterfaceString( iStringIndex, szTitle, INTERFACE_STRING_LENGTH );

		GlobalFunc::SetCaption(pTitleControl, szTitle);
	}
}


CControlWZ * RewardDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}

POSTYPE	RewardDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

//----------------------------------------------------------------------------
/**
*/
VOID 
RewardDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		m_bSendPacket=FALSE;
	}
}

VOID RewardDialog::MessageProc( SI_MESSAGE * pMessage )
{

	POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

	CControlWZ * pControl = getControl(pos);
	if (!pControl)
	{
		return;
	}

	// 패킷을 보낸후에는 더이상 UI에 반응하지 않는다!
	if (m_bSendPacket)
	{
		return;
	}


	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
        {
			switch(pos)
			{
			case REWARDDIALOG_ITEM1_CHECK:
			case REWARDDIALOG_ITEM2_CHECK:
			case REWARDDIALOG_ITEM3_CHECK:
			case REWARDDIALOG_ITEM4_CHECK:
			case REWARDDIALOG_ITEM5_CHECK:
				{
					OnClickItem(pos - REWARDDIALOG_ITEM1_CHECK);
				}
				break;			
			case REWARDDIALOG_OK_BUTTON:
				{
					switch ( m_eRewardViewType )
					{
					case REWARD_VIEW_TYPE_NORMAL:
						{
							OnClickOK();			
						}
						break;

					case REWARD_VIEW_TYPE_AC_PREVIEW:
						{
							ShowDialog(FALSE);
						}
						break;

                    case REWARD_VIEW_TYPE_AC_REWARD:
                        {
                            uiAreaConquestMan* ui_ac_manager = 
                                GET_DIALOG_MANAGER(uiAreaConquestMan, UIMAN_AREACONQUEST);
                            if (ui_ac_manager != NULL)
                            {
                                ui_ac_manager->SEND_CG_AREACONQUEST_REWARD_SYN();
                            }
                            ShowDialog(FALSE);
                        }
                        break;
					}

				}
				break;
			}
			break;
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}

}


VOID RewardDialog::NetworkProc( MSG_BASE * pMsg )
{

}

void RewardDialog::SetRewardMoney(LONGLONG money)
{
	
	m_RewardMoney = money;

	CCtrlStaticWZ *pControl = (CCtrlStaticWZ *)getControl(REWARDDIALOG_MONEY_VALUE_TEXT);

	TCHAR str[256];

	TCHAR szMoney[256] = {0,};
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
	Sprintf(szMoney, _T("%I64u"), money);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, szMoney, &nFmt, str, sizeof(str));

	GlobalFunc::SetCaption( pControl, str );
//	pControl->SetCaption(str);
}

void RewardDialog::SetRewardExp(EXPTYPE exp)
{
	m_RewardExp = exp;

	CCtrlStaticWZ *pControl = (CCtrlStaticWZ *)getControl(REWARDDIALOG_EXP_VALUE_TEXT);

	TCHAR str[256];

	TCHAR exp_string[256] = {0, };
	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 1 };
	Sprintf(exp_string,_T("%I64u"),exp);
	::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, exp_string, &nFmt, str, sizeof(str));

	GlobalFunc::SetCaption( pControl, str );
//	pControl->SetCaption(str);
}
void RewardDialog::RewardItemActiveForClass()
{
	if(!g_pHero) return; 

	for (int a = 0; a < RewardInfo::MAX_DROP_INDEX_NUM; ++a)
	{
		//슬롯에 아이템이 존재할때
		if (m_RewardItem[a].pImage && m_RewardItem[a].param.dwCode)
		{
			BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo(m_RewardItem[a].param.dwCode);

			//무기나 장비아이템에 한에서만 체크한다
			if(pInfo && (pInfo->IsArmor() || pInfo->IsWeapon()))
			{
				eCHAR_TYPE eCharEqClass[eCHAR_TYPE_MAX] = {	
						(pInfo->m_wEqClass1)?(eCHAR_BERSERKER):(eCHAR_TYPE_MAX),	//(B)버서커
						(pInfo->m_wEqClass2)?(eCHAR_DRAGON):(eCHAR_TYPE_MAX),		//(D)드래곤
						(pInfo->m_wEqClass3)?(eCHAR_SHADOW):(eCHAR_TYPE_MAX),		//(S)쉐도우
						(pInfo->m_wEqClass4)?(eCHAR_VALKYRIE):(eCHAR_TYPE_MAX),		//(V)발키리
                        (pInfo->m_wEqClass5)?(eCHAR_ELEMENTALIST):(eCHAR_TYPE_MAX),		//(E)엘리멘탈리스트
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                        (pInfo->m_wEqClass6)?(eCHAR_MYSTIC):(eCHAR_TYPE_MAX),       //(M)미스틱//_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                        (pInfo->m_wEqClass7)?(eCHAR_HELLROID):(eCHAR_TYPE_MAX),		//(H)
#else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                        (pInfo->m_wEqClass6)?(eCHAR_MYSTIC):(eCHAR_TYPE_MAX),       //(M)미스틱//_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        (pInfo->m_wEqClass8)?(eCHAR_WITCHBLADE):(eCHAR_TYPE_MAX),   //(W)위치블레이드//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                        };

				//자신의클래스와 일치하는지 체크한다
				for( int i = 0 ; i < eCHAR_TYPE_MAX ; ++i )
				{
					//정보셋팅해주고 리턴한다.
					if(eCharEqClass[i]==g_pHero->GetClass())
					{
						for (int n = 0; n < m_iMaxSelectItem; ++n)
						{
							m_RewardItem[n].bSelected = FALSE;
						}
						m_RewardItem[a].bSelected = TRUE;
						return;
					}
				}	
			}
		}

	}

}
void RewardDialog::UpdateRewardInfos(DWORD dwIndex, int nArea, int nACHIVEMENT)
{
	m_dwRewardIndex = dwIndex;

	RewardInfo *pReward = RewardInfoList::Instance()->FindRewardInfo(m_dwRewardIndex);

	if (!pReward)
	{
		assert(!"RewardDialog::SetMissionReward 잘못된 미션넘버");
		return;
	}

	InitializeRewardItem();

	BYTE bRefType = pReward->GetRefType();
	BOOL bRandomItem = FALSE;

	switch(bRefType)
	{
	case RewardInfo::REF_TYPE_SELECT:
		{
			m_bCanSelectItem = TRUE;
			bRandomItem = FALSE;
		}
		break;
	case RewardInfo::REF_TYPE_RANDOM:
		{
			m_bCanSelectItem = TRUE;
			bRandomItem = TRUE;
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_ITEMCODE:
		{
			m_bCanSelectItem = FALSE;
			bRandomItem = FALSE;
		}
		break;
	case RewardInfo::REF_TYPE_EVENT_DROPINFO:
		{
			m_bCanSelectItem = FALSE;
			bRandomItem = TRUE;
		}
		break;
	}

	SetRewardMoney(pReward->GetHime());

	if( g_pHero)
	{
		if( g_pHero->IsReachMaxLevel())
		{
			SetRewardExp(0);
		}
		else
		{
			SetRewardExp(pReward->GetExp());
		}
	}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    SetTitleReward(pReward->GetTitle());
#endif //_NA_003027_20111013_HONOR_SYSTEM

	if (m_bCanSelectItem)
	{
		m_iMaxSelectItem = pReward->GetSelectNum();
	}
	else
	{
		m_iMaxSelectItem = 0;
	}

	for (int a = 0; a< RewardInfo::MAX_DROP_INDEX_NUM; a++) 
	{
		if (bRandomItem)
		{
			if (pReward->GetTypeCode(a))
			{
				SetRandomRewardItem(a);
			}
		}
		else 
		{
			MISSION_REWARD_ITEM_PARAM param;

			ZeroMemory(&param,sizeof(param));

			param.dwCode = pReward->GetTypeCode(a);

			if (param.dwCode) 
			{
				BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo(param.dwCode);
				if (pInfo) 
				{
#ifdef _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT
                    if (pInfo->m_byDupNum > 1) 
#else
  #ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
                    bool is_wing_item = (pInfo->item_rule_info.is_accessory_wings == true) &&
                        (pInfo->item_rule_info.is_accessory_special == true);
					if (!pInfo->IsArmor() && !pInfo->IsWeapon() && is_wing_item == false)
  #else
                    if (!pInfo->IsArmor() && !pInfo->IsWeapon())
  #endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
#endif //_NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT
					{
						param.dwCount = pReward->GetTypeIndex(a);					
						param.iSet = 0;
						param.iRank = 0;
						param.iEnchant = 0;
					}
					else
					{
						param.dwCount = 1;

						ItemType * pItemType = NULL; 
						pItemType = ItemTypeList::Instance()->FindItemType( pReward->GetTypeIndex(a) );
						if ( pItemType )
						{
                            param.iSet = pItemType->GetItemType(); 
							param.iEnchant = pItemType->GetEnchant();
							param.iRank = pItemType->GetRank();
						}
					}
				}
			}

			SetRewardItem(a,param);
		}
	}

	if(m_bCanSelectItem)
	{
		//선택아이템에 한에서만 선택활성화를한다.
		RewardItemActiveForClass();
	}

	m_FTextRender.ClearSentence();

	TCHAR szString[512];
	if (pReward->GetDCode())
	{
		g_InterfaceManager.GetInterfaceString(pReward->GetDCode(),szString,512);

		if ( m_FTextParser.Parser( szString, m_FTextSentence) )
		{
#ifdef _INTERNATIONAL_UI
			m_FTextRender.SetGap(FTextRender::DEFAULT_CHARACTER_GAP,0);
#endif//_INTERNATIONAL_UI
			m_FTextRender.Append(m_FTextSentence);
		}		
	}
}

//#endif // end of __NA_20061103_REWARD_STRUCT_CHANGE

void RewardDialog::SetACReward(DWORD dwRewardIndex, int nArea, int nACHIVEMENT)
{
	m_eRewardViewType = REWARD_VIEW_TYPE_AC_REWARD;

	UpdateTitle();

	UpdateRewardInfos(dwRewardIndex, nArea, nACHIVEMENT);
}

void RewardDialog::SetMissionReward(DWORD dwRewardIndex)
{	
	m_eRewardViewType = REWARD_VIEW_TYPE_NORMAL;

	UpdateTitle();

	UpdateRewardInfos(dwRewardIndex);
}

void RewardDialog::InitializeRewardItem()
{
	for (int a = 0; a < MAX_REWARD_ITEM; a++)
	{
		ZeroMemory(&m_RewardItem[a],sizeof(MISSION_REWARD_ITEM));
	}
}

void RewardDialog::SetRewardItem(int index,MISSION_REWARD_ITEM_PARAM param)
{
	if (param.dwCode == 0)
	{
		return;
	}

	if (index < 0)
	{
		index = 0;
	}
	if (index >= MAX_REWARD_ITEM)
	{
		index = MAX_REWARD_ITEM - 1;
	}


	DeleteItem(index);

	m_RewardItem[index].param = param;
	
#ifdef _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT
    RESOURCECODE code = 0;

    if ((param.iSet == KindOfItemType::Etheria) ||
        (param.iSet == KindOfItemType::EtheriaWithDivine))
    {
        BASE_ITEMINFO *item_info = ItemInfoParser::Instance()->GetItemInfo(param.dwCode);
        if (item_info && item_info->m_wType == eITEMTYPE_PET && g_InterfaceManager.pet_etheria_icon()) 
        {
            // 펫+에테리아 아이템일 경우
            m_RewardItem[index].pImage = g_InterfaceManager.pet_etheria_icon();
            return;
        }
        else
        {
            // 에테리아 아이템일 경우
            code = ResourceManager::Instance()->GetItemSlotImageResourceCode(HardCode::ITEMCODE_of_ETHERIA);
        }
    }
    else
    {
        code = ResourceManager::Instance()->GetItemSlotImageResourceCode(param.dwCode);
    }
#else
    RESOURCECODE code = ResourceManager::Instance()->GetItemSlotImageResourceCode(param.dwCode);
#endif //_NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT

	if (code != 0)
	{
		m_RewardItem[index].pImage = ICONMGR()->Add(code);
	}
}

void RewardDialog::OnUpdateSolarDialog()
{

	CCtrlListWZ *pListCtrl = (CCtrlListWZ *)getControl(REWARDDIALOG_MISSION_TEXT);

	if (pListCtrl)
	{
		m_FTextRender.SetRect( pListCtrl->GetSizeRect() );
	}
}

void RewardDialog::SetRandomRewardItem(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index >= MAX_REWARD_ITEM)
	{
		index = MAX_REWARD_ITEM - 1;
	}


	DeleteItem(index);

	m_RewardItem[index].param.dwCode = RANDOM_ITEM_CODE;
	m_RewardItem[index].param.dwCount = 1;
	m_RewardItem[index].param.iSet = 0;
	m_RewardItem[index].bRandom = TRUE;

	RESOURCECODE code = ResourceManager::Instance()->GetItemSlotImageResourceCode(RANDOM_ITEM_CODE);

	if (code != 0)
	{
		m_RewardItem[index].pImage = ICONMGR()->Add(code);
	}
}

void RewardDialog::DeleteItem(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index >= MAX_REWARD_ITEM)
	{
		index = MAX_REWARD_ITEM - 1;
	}

	if (m_RewardItem[index].pImage)
	{
		ICONMGR()->RemoveIcon(m_RewardItem[index].pImage);
		m_RewardItem[index].pImage = NULL;
	}



}

void RewardDialog::OnRenderSolarDialog()
{
	

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	for (int a = 0; a < MAX_REWARD_ITEM; a++)
	{
		if (m_RewardItem[a].pImage && m_RewardItem[a].param.dwCode)
		{
			CControlWZ *pControl = getControl(REWARDDIALOG_ITEM1_CHECK + a);
			if (pControl) {
				RECT rc = pControl->GetSizeRect();
				
				
				if (m_RewardItem[a].bSelected)				
				{
					//rc.top -= 10;
					//rc.bottom -= 10;
				}

				m_RewardItem[a].pImage->Render(&rc);

#ifdef _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT
                if (m_RewardItem[a].param.iSet == KindOfItemType::Shell)
                {
                    m_RewardItem[a].pImage->RenderCoverImage(&rc, WzColor_RGBA(0, 0, 0, 175));
                }
#endif //_NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT

				if (m_RewardItem[a].bSelected)
				{
					g_InterfaceManager.RenderActivateMark( rc.left - 13, rc.top - 13);
				}

				if (rc.left <= iMouseX && rc.right >= iMouseX)
				{
					if (rc.top <= iMouseY && rc.bottom >= iMouseY)
					{
						SCItemSlot item;

						item.SetCode((SLOTCODE)m_RewardItem[a].param.dwCode);
						item.SetEnchant(m_RewardItem[a].param.iEnchant);
						item.SetRank((eRANK)m_RewardItem[a].param.iRank);
                        nsSlot::ItemTypeChanger::ChangeItemByType(&item, m_RewardItem[a].param.iSet);
                        

						uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
						if(pUIMan)
						{
							pUIMan->RegisterSlotInfoTooltip(&item);
							pUIMan->SetTooltipDialogID(GetDialogKey());
						}

					}
				}
				TCHAR szCount[128];
				Sprintf(szCount,_T("%d"),m_RewardItem[a].param.dwCount);
				WzColor color = WzColor_RGBA(255,255,255,255);

			
				g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("mn12") );
				g_pSunRenderer->x_pManagerTextOut->DrawText(szCount,&rc,color);
				g_pSunRenderer->x_pManagerTextOut->EndUseFont();
			}
		}
	}

	

	m_FTextRender.Render();
}

int RewardDialog::GetSelectedItemCount()
{
	int count = 0;

	for (int a = 0; a < MAX_REWARD_ITEM; a++)
	{
		if (m_RewardItem[a].bSelected)
		{
			count++;
		}
	}

	return count;

}

void RewardDialog::DeselectItem(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index >= MAX_REWARD_ITEM)
	{
		index = MAX_REWARD_ITEM - 1;
	}

	m_RewardItem[index].bSelected = FALSE;

}

void RewardDialog::SelectItem(int index)
{
	int maxselectitem = 0;

	RewardInfo *pReward = RewardInfoList::Instance()->FindRewardInfo(m_dwRewardIndex);

	if (!pReward)
	{
		return;
	}
	
	// 이미 선택할수 있는 최대 갯수 이상의 아이템을 선택했으면
	// 기존 에 선택한 아이템 하나를 선택해제한다.
	if (GetSelectedItemCount() >= m_iMaxSelectItem)
	{
		for (int a = 0; a < MAX_REWARD_ITEM; a++)		
		{
			if (m_RewardItem[a].bSelected)
			{
				DeselectItem(a);
				break;
			}

		}
	}
	
	
	m_RewardItem[index].bSelected = TRUE;

}

void RewardDialog::OnClickItem(int index)
{
	if (index < 0)
	{
		index = 0;
	}
	if (index >= MAX_REWARD_ITEM)
	{
		index = MAX_REWARD_ITEM - 1;
	}

	if (m_bCanSelectItem)
	{
		if (m_RewardItem[index].param.dwCode)
		{
			if (m_RewardItem[index].bSelected)
			{
				DeselectItem(index);
			}
			else
			{
				SelectItem(index);
			}
		}
	}
}

void RewardDialog::OnClickOK()
{

	if(m_bSendPacket)
		return;

	RewardInfo *pReward = RewardInfoList::Instance()->FindRewardInfo(m_dwRewardIndex);

	if (!pReward)
	{
		return;
	}

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


	if (GetSelectedItemCount() < m_iMaxSelectItem )
	{
		//	아이템을 더 선택하십시오.
		g_InterfaceManager.GetInterfaceString( eST_MORE_SELECT_ITEM, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return;
	}

	// 보상주세용
	MSG_CG_TRIGGER_REWARD_SYN SendPacket;

	SendPacket.m_Num = (WORD)m_dwRewardIndex;
//	SendPacket.m_Type = m_bSecond ? 1 : 0;
	SendPacket.m_SelectedItem = 0;

	for (int a = 0; a < MAX_REWARD_ITEM; a++)
	{
		if (m_RewardItem[a].bSelected)
		{
			SendPacket.m_SelectedItem |= (1 << a);
		}

	}

	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

	m_bSendPacket = TRUE;
}

void RewardDialog::OnClickCancel()
{
	if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE)
	{
		// 닫아부려
		ShowDialog(FALSE);
	}
}

// 다시 패킷을 보낼수 있게 해준다.
void RewardDialog::EnableSendPacket()
{

	m_bSendPacket = FALSE;
	
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void RewardDialog::SetTitleReward(WORD reward_title_code)
{
    CControlWZ* control_ptr = NULL;

    TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(reward_title_code);
    if ((reward_title_code == 0) || 
        (title_info == NULL))
    {
        // 하이드 칭호관련 컨트롤
        control_ptr = getControl(REWARDDIALOG_TITLE_PICTURE);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        control_ptr = getControl(REWARDDIALOG_TITLE_TEXT);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        control_ptr = getControl(REWARDDIALOG_TITLE_VALUE_TEXT);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }
    else
    {
        // 쇼 칭호관련 컨트롤
        control_ptr = getControl(REWARDDIALOG_TITLE_PICTURE);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        control_ptr = getControl(REWARDDIALOG_TITLE_TEXT);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        control_ptr = getControl(REWARDDIALOG_TITLE_VALUE_TEXT);
        if (control_ptr != NULL)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
            // 타이틀 세팅
            control_ptr->SetTextID(title_info->title_name_code);
        }
    }
}
#endif //_NA_003027_20111013_HONOR_SYSTEM