#include "SunClientPrecompiledHeader.h"
#include "selectcompositemenu.h"
#include "ItemETCCompositeDlg.h"
#include "ItemManager.h"
#include "globalfunc.h"
#include "interfacemanager.h"

#include "CompositeNPCInfoParser.h"

#ifdef __NA_001358_ENCHANT_UNIFICATION
#else //__NA_001358_ENCHANT_UNIFICATION
	
	#include "EnchantDialog2.h"
	
#endif //__NA_001358_ENCHANT_UNIFICATION


// 기타 조합 창
#include "ItemETCCompositeDlg.h"
#include "uiItemCompositeMan/uiItemCompositeMan.h"

#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#include "uiEquipmentAwakeningMan/uiEquipmentAwakeningMan.h"
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiEquipmentEvolutionMan/uiEquipmentEvolutionMan.h"
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

WzID SelectCompositeMenu::m_wzId[] = 
{
	StrToWzID("TI02"), //eSELECTMENU_SELECT1
	StrToWzID("TI05"), //eSELECTMENU_SELECT2
	StrToWzID("B001"), //eSELECTMENU_SELECT3	
	StrToWzID("B002"), //eSELECTMENU_SELECT4
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#else
	StrToWzID("B003"), //eSELECTMENU_SELECT5			 
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	StrToWzID("B999"), //eSELECTMENU_BUTTON_EXIT,
    StrToWzID("P000"), //eSELECTMENU_PICTURE_BACK,
};

SelectCompositeMenu::SelectCompositeMenu(void)
{
}

SelectCompositeMenu::~SelectCompositeMenu(void)
{
}

VOID 
SelectCompositeMenu::Init( CDrawBase * pDrawBase )
{
#ifdef _NA_007450_20140805_CRYSTALIZATION_RENEWAL_NPC_SELECT_HEIGHT_ADJUST
    RECT dialog_size;
    GetDialogWZ()->GetDlgSize(&dialog_size);
    GetDialogWZ()->MoveWindowWZ(
        (float)dialog_size.left, 
        (float)dialog_size.top, 
        (float)(dialog_size.right/* - dialog_size.left*/),
        (float)(dialog_size.bottom / 2) + 25);
    CControlWZ* background_picture = getControl(eSELECTMENU_PICTURE_BACK);
    if (background_picture)
    {
        RECT rect_bg = background_picture->GetSize();
        background_picture->MoveWindowWZ(
            (float)rect_bg.left,
            (float)rect_bg.top,
            (float)rect_bg.right,
            (float)(rect_bg.bottom / 2));
    }
#endif // _NA_007450_20140805_CRYSTALIZATION_RENEWAL_NPC_SELECT_HEIGHT_ADJUST

	for ( int i = 0; i < MAX_SELECT; ++i)
	{
		m_pBtnSelectMenu[i] = (CCtrlButtonWZ *)getControl(i);
		assert(m_pBtnSelectMenu[i]);
	}
}


VOID SelectCompositeMenu::Release()
{

}

VOID SelectCompositeMenu::MessageProc( SI_MESSAGE * pMessage )
{
	POSTYPE pos = 255;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch (pos)
			{
			case eSELECTMENU_SELECT1:
			case eSELECTMENU_SELECT2:
			case eSELECTMENU_SELECT3:
			case eSELECTMENU_SELECT4:
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#else
			case eSELECTMENU_SELECT5:
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
				{
					btnClickSelectMenu(pos);	
				}
				break;

			case eSELECTMENU_BUTTON_EXIT:
				{
					ShowDialog(FALSE);
				}
				break;
			}
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}

}

//-------------------------------------------------------------------------------------------
/**
*/
VOID 
SelectCompositeMenu::OnShowWindow(BOOL val)
{
	if (val)
	{
        //MoveWindowWZ(0.0f, 0.0f);

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_item_selectmenu;
		msg.wParam=InterfaceManager::DIALOG_SELECT_COMPOSITE_MENU;
		msg.DoSomething=GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		initCompositeMenuInfo();

        EnchantDialog* enchant_dialog_ptr = static_cast<EnchantDialog*>(ItemManager::Instance()->GetItemContainerDialog(SI_ENCHANT));
        if ((enchant_dialog_ptr != NULL) && (enchant_dialog_ptr->IsVisible() == TRUE))
        {
            enchant_dialog_ptr->ShowWindow(FALSE);
        }
	}
	else
	{
		m_VecSelectComposeInfo.clear();
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_item_selectmenu);
	}
}

VOID			
SelectCompositeMenu::SetOpenNPC(CODETYPE code)
{
	this->m_NPCCode = code;
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID			
SelectCompositeMenu::initCompositeMenuInfo()
{
	BASE_CompositeNPCInfo * pInfo = 
		CompositeNPCInfoParser::Instance()->GetCompositeNPCInfo(this->m_NPCCode);

	for (int i = 0; i < MAX_COMPOSE_PARAM; ++i)
	{
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		if ( i >= MAX_SELECT )
		{
			continue;
		}
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

		sSelectComposeInfo sComposeInfo = { eITEMCOMPOSITE_NONE, 0 };

		m_pBtnSelectMenu[i]->ShowWindowWZ(WZ_HIDE);

		if ( !pInfo || !pInfo->m_dwParams[i] )
		{
			m_VecSelectComposeInfo.push_back(sComposeInfo);
			continue;
		}

		m_pBtnSelectMenu[i]->ShowWindowWZ(WZ_SHOW);
		sComposeInfo.eCompositeType = (eITEMCOMPOSITECODE)pInfo->m_dwParams[i];
		sComposeInfo.dwStrID = 
			ItemManager::Instance()->GetCompositeNameID(sComposeInfo.eCompositeType);

		g_InterfaceManager.GetInterfaceString( 
			sComposeInfo.dwStrID, 
			this->m_szBuf, 
			INTERFACE_STRING_LENGTH);

		GlobalFunc::SetCaption(m_pBtnSelectMenu[i], this->m_szBuf);
		m_VecSelectComposeInfo.push_back(sComposeInfo);
	}

#ifdef _GS_GSP_BLOCK_CONTENTS_
	if(m_pBtnSelectMenu[eSELECTMENU_SELECT2])
		m_pBtnSelectMenu[eSELECTMENU_SELECT2]->ShowWindowWZ(WZ_HIDE);
#endif//_GS_GSP_BLOCK_CONTENTS_
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
SelectCompositeMenu::btnClickSelectMenu(POSTYPE pos)
{
	if ( pos >= this->m_VecSelectComposeInfo.size() )
	{
		return;
	}

	OpenCompositeFoward( this->m_VecSelectComposeInfo[pos].eCompositeType );
    if (IsVisible())
    {
        SolarDialog::ShowWindow(false);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
CControlWZ * 
SelectCompositeMenu::getControl( POSTYPE AtPos )
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


//-------------------------------------------------------------------------------------------
/**
*/
POSTYPE	
SelectCompositeMenu::getCtrlIDToPos( WzID wzId )
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

//-------------------------------------------------------------------------------------------
/**
*/
SolarDialog *
OpenCompositeFoward(eITEMCOMPOSITECODE eCode)
{
	SolarDialog* pReturnDlg = NULL;

	switch (eCode)
	{
	case eITEMCOMPOSITE_LOW_ENCHANT:		
	case eITEMCOMPOSITE_MEDIUM_ENCHANT:
	case eITEMCOMPOSITE_HIGH_ENCHANT:		
	case eITEMCOMPOSITE_HEIM_ENCHANT:
	case eITEMCOMPOSITE_CASH_ENCHANT:		
		{
#ifdef __NA_001358_ENCHANT_UNIFICATION
			// no operation
#else //__NA_001358_ENCHANT_UNIFICATION
			// 인첸트 창
			pReturnDlg = g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ENCHANT);
			g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_ENCHANT, TRUE);

            EnchantDialog* enchantDlg = static_cast<EnchantDialog*>(pReturnDlg);
            assert(enchantDlg);

            enchantDlg->SetCashEnchant(eCode == eITEMCOMPOSITE_CASH_ENCHANT);
	
#endif //__NA_001358_ENCHANT_UNIFICATION
		}
		break;

    case eITEMCOPOSITE_ZARD:			
        {
            uiSocketComposite* ui_socket_composite_ptr = 
                GET_CAST_DIALOG(uiSocketComposite, IM_SOCKET_MANAGER::SOCKET_COMPOSITE);

            if(ui_socket_composite_ptr)
            {
                ui_socket_composite_ptr->ShowDialog(TRUE);	
            }
        }
        break;

	case eITEMCOMPOSITE_RANK_DOWN:		
	case eITEMCOMPOSITE_EXTRACT_SOCKET:
	case eITEMCOMPOSITE_ADD_SOCKET:		
		{
			// 기타 조합 창
			ItemETCCompositeDlg * pDlg = 
				(ItemETCCompositeDlg *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_ITEM_ETC_COMPOSE);
			if ( pDlg )
			{
				pDlg->SetCurrentCompositeType( eCode );
				pDlg->ShowDialog(TRUE);
			}

			pReturnDlg = pDlg;
		}
		break;

	//case eITEMCOMPOSITE_LOW_RING:
	//case eITEMCOMPOSITE_HIGH_RING:	
	//case eITEMCOMPOSITE_LOW_NECKLACE:
	//case eITEMCOMPOSITE_HIGH_NECKLACE:
	//case eITEMCOPOSITE_PROTECTION_CRYSTAL:
	//case eITEMCOPOSITE_SHANING_CRYSTAL:	
	//case eITEMCOPOSITE_SOUL_SHEILD_LOW:	
	//case eITEMCOPOSITE_SOUL_SHEILD_HIGH:	
	//case eITEMCOPOSITE_SOUL_ARMLET_LOW:	
	//case eITEMCOPOSITE_SOUL_ARMLET_HIGH:	
	//case eITEMCOPOSITE_SOUL_WING_LOW:		
	//case eITEMCOPOSITE_SOUL_WING_HIGH:	
	//case eITEMCOPOSITE_SOUL_EYE_LOW:		
	//case eITEMCOPOSITE_SOUL_EYE_HIGH:		
	//case eITEMCOPOSITE_EXTRASTONE_CREATE:
	//	{
	//		// 조합 창.
	//		uiItemCompositeMan * pCompositeMan = 
	//			(uiItemCompositeMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE);

	//		pReturnDlg = NULL;
	//		if ( pCompositeMan )
	//		{				
	//			pReturnDlg = pCompositeMan->ShowItemComposite(
	//				ITEM_COMPOSITE_CATEGORY_COMPOSITE, 
	//				(eITEMCOMPOSITECODE)eCode );
	//		}				
	//	}
	//	break;

    case eITEMCOPOSITE_CRYSTALIZE:
        {
            // 확인 메시지 창 보여주기 hsh
            // 2230 : 결정화 시스템이 개편되어 더 이상 강화 상인이 아이템을 결정화 해주지 않습니다.
            // \n\n스킬창(K)의 액션/감정 탭을 열어 [결정화] 액션을 사용하시면 인벤토리 내에서 보다 편리하게 아이템을 결정화 할 수 있습니다. 
            // \n\n자세한 내용은 정보열람실(H)를 확인하세요.
            GlobalFunc::VerifyMessage(2230, 0, NULL, true);
        }
        break;

#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	case eITEMCOMPOSITE_EQUIPMENT_WAKEUP:
		{
			// 장비 각성 창
			uiEquipmentWakeupMan * ui_equipment_wakeup_manager = 
				(uiEquipmentWakeupMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_EQUIPMENT_WAKEUP);

			pReturnDlg = NULL;
			if ( ui_equipment_wakeup_manager )
			{
				pReturnDlg = ui_equipment_wakeup_manager->ShowEquipmentAwakeningDialog();
			}
		}
		break;
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	case eITEMCOMPOSITE_EQUIPMENT_EVOLUTION:
		{
			// 장비 진화 창
			uiEquipmentEvolutionMan * ui_equipment_evolution_manager = 
				(uiEquipmentEvolutionMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_EQUIPMENT_EVOLUTION);

			pReturnDlg = NULL;
			if ( ui_equipment_evolution_manager )
			{
				pReturnDlg = ui_equipment_evolution_manager->ShowEquipmentEvolutionDialog();
			}
		}
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

	case eITEMCOPOSITE_EXTRASTONE_USE:	
	case eITEMCOMPOSITE_NONE:	
		break;
	}

	return pReturnDlg;
}
