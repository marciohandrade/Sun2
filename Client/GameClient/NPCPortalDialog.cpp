#include "SunClientPrecompiledHeader.h"
#include "npcportaldialog.h"
#include "MapInfoParser.h"
#include "LimitedMapPortParser.h"
#include "Map.h"
#include "LoadScene.h"
#include "Application.h"
#include "NPCInfoParser.h"
#include "AskNpcPortal.h"
#include "ResultCode.h"
#include "VillageScene.h"
#include "hero.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiAssistMan/uiAssistMan.h"
//----------------------------------------------------------------------------
/**
*/
WzID NPCPortalDialog::m_wzId[] = 
{
	StrToWzID("T001"), //PORTAL_TITLE = 0,
	StrToWzID("L001"), //PORTAL_LIST,
	StrToWzID("B999"), //PORTAL_BTN_EXIT,
	StrToWzID("B002"), //PORTAL_BTN_OK,
	StrToWzID("B003"), //PORTAL_BTN_CANCEL,
};


//----------------------------------------------------------------------------
/**
*/
NPCPortalDialog::NPCPortalDialog(void)
{
    ClearPendingMoney();
}


//----------------------------------------------------------------------------
/**
*/
NPCPortalDialog::~NPCPortalDialog(void)
{

}


//----------------------------------------------------------------------------
/**
*/
VOID	
NPCPortalDialog::Init( CDrawBase * pDrawBase )
{
	m_pVillageMapInfo = NULL;

	m_pTitleCtrl	= (CCtrlStaticWZ *)getControl(PORTAL_TITLE);
	assert(m_pTitleCtrl);
	m_pListCtrl		= (CCtrlListWZ *)getControl(PORTAL_LIST);
	assert(m_pListCtrl);   
	
	m_pListCtrl->DeleteItemAll();
    m_pListCtrl->SetBeginDrawIndex(0);
    m_pListCtrl->SetUseReport(false);
    m_pListCtrl->SetColorSelBar(RGBA(0,0,150,150));
    m_pListCtrl->SetReportUseCnt(0);
}


//----------------------------------------------------------------------------
/**
*/
VOID	
NPCPortalDialog::Release()
{

}


//----------------------------------------------------------------------------
/**
*/
VOID	
NPCPortalDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch (pMessage->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
        {
			POSTYPE pos = this->getCtrlIDToPos(pMessage->dwCtrlID);
			this->BtnClick(pos);
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID	
NPCPortalDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
		if (!m_pVillageMapInfo)
		{
			ShowWindow(FALSE);
		}

		this->m_pListCtrl->SetCurSel(0);

        ClearPendingMoney();

		//UpdatePosition();
	}
	else
	{
		this->m_pVillageMapInfo = NULL;
		this->m_packetStatus = 0;
		this->m_vecPoralList.clear();
	}
}

////----------------------------------------------------------------------------
///**
//*/
//void    
//NPCPortalDialog::UpdatePosition()
//{
//	//POINT pt = g_pApplication->Get2DModeMapping(); 
//
//	//// dialog size
//	//RECT rcDialog;
//	//GetDialogWZ()->GetDlgSize(&rcDialog);
//
//	//long dWidth, dHeight;
//	//dWidth = rcDialog.right / 2;
//	//dHeight = rcDialog.bottom / 2;
//
//
//	////	화면 가운데 설정
//	//MoveWindowWZ( (float)((pt.x / 2) - dWidth), (float)((pt.y / 2) - dHeight) );
//}

//----------------------------------------------------------------------------
/**
*/
VOID			
NPCPortalDialog::SetCurVillageCode( CODETYPE NPCCode, MAPCODE MapCode )
{
	TCHAR szMapName[256] = {0,};
	TCHAR szNPCName[256] = {0,};
	TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};

	m_pVillageMapInfo = MapInfoParser::Instance()->FindMapInfo(MapCode);
	assert(m_pVillageMapInfo);
	if (!m_pVillageMapInfo)
	{
		return;
	}

	g_InterfaceManager.GetInterfaceString(this->m_pVillageMapInfo->dwNCode, szMapName, 256);
	
	BASE_NPCINFO * pNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( NPCCode );
	
	if ( pNPCInfo )
	{
		g_InterfaceManager.GetInterfaceString(pNPCInfo->m_NCode, szNPCName, 256);
#ifdef _DEV_VER
        //#ifdef _SCJ_SHOW_NPC_ID
        if (CGeneralGameParam::Instance()->IsShowNpcId())
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
            StrnCopy(buffer_string, szNPCName, INTERFACE_STRING_LENGTH-1); 
            Snprintf(szNPCName, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
                buffer_string, pNPCInfo->m_MonsterCode);
        }
        //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER
		Snprintf(szResult,INTERFACE_STRING_LENGTH-1, _T("%s   <%s>"), szNPCName, szMapName);
		GlobalFunc::SetCaption( m_pTitleCtrl, szResult );
		m_pTitleCtrl->SetTextColor(RGBA(204, 184, 148, 255));
	}   	

	RegisterPoralList();
	RefreshPortalList();

	this->m_pListCtrl->SetCurSel(0);
}

//----------------------------------------------------------------------------
/**
*/
VOID			
NPCPortalDialog::RegisterPoralList()
{
	if (m_pListCtrl)
	{
		m_pListCtrl->DeleteItemAll();
	}

	if ( m_pVillageMapInfo )
	{
		sMapPort * pMapPort = 
			LimitedMapPortParser::Instance()->FindVillage( m_pVillageMapInfo->MapCode );

		if ( !pMapPort )
			return;

		for ( int i = 0; i < eVILLAGE_MAX; ++i )
		{
			if ( 0 == pMapPort->m_VillagePort[i].m_MapCode )
			{
				continue;
			}

			m_vecPoralList.push_back(pMapPort->m_VillagePort[i]);
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
CControlWZ *	
NPCPortalDialog::getControl( POSTYPE AtPos )
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


//----------------------------------------------------------------------------
/**
*/
POSTYPE			
NPCPortalDialog::getCtrlIDToPos( WzID wzId )
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
NPCPortalDialog::BtnClick( POSTYPE pos )
{
	switch (pos)
	{
	case PORTAL_BTN_CANCEL:
	case PORTAL_BTN_EXIT:
		{
			ShowDialog(FALSE);
		}
		break;

	case PORTAL_BTN_OK:
		{
			assert( this->m_pVillageMapInfo );
			if ( m_pVillageMapInfo )
			{
				int index = this->m_pListCtrl->GetCurSel();
				if (-1 == index )
				{
					return;
				}

				this->popupAskPortal(
					m_vecPoralList[index].m_MapCode, 
					m_vecPoralList[index].m_Fare
					);
			}
		}
		break;

	case PORTAL_LIST:
		{
			assert( this->m_pVillageMapInfo );
			if ( m_pVillageMapInfo )
			{
				int index = this->m_pListCtrl->GetCurSel();
				if (-1 == index )
				{
					return;
				}

				this->popupAskPortal(
					m_vecPoralList[index].m_MapCode, 
					m_vecPoralList[index].m_Fare
					);
			}
		}
		break;

	default:
		break;
	};
}

VOID			
NPCPortalDialog::SEND_CG_ZONE_VILLAGE_MOVE_SYN(MAPCODE MapCode, MONEY Fare)
{
	if ( this->m_vecPoralList.size() <= 0 ) 
	{
		return;
	}

	if ( !MapCode )
	{
		return;
	}

    MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN   sync;
    sync.m_wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndexVer2(g_pMap->GetCurrentFieldID(), MapCode);
    assert (sync.m_wIndex);
    if (!sync.m_wIndex)
        return;

	g_VillageScene.SetRqstFieldMoveMapCode(MapCode);

    m_PendingMapCode = MapCode;
    m_PendingFare = Fare;

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));

}

//----------------------------------------------------------------------------
/**
*/
VOID			
NPCPortalDialog::RefreshPortalList()
{
	TCHAR	szBuffer[256] = {0,};
	TCHAR	szMapName[256] = {0,};
	TCHAR	szResult[INTERFACE_STRING_LENGTH] = {0,};
	
	DWORD	dwTextColor				= RGBA( 240, 201, 121, 255 );//RGBA( 235, 192, 38, 255 );
	DWORD	dwSelectColor			= RGBA( 0, 0, 255,255 );
	DWORD	dwBGColor				= RGBA( 32, 32, 32, 0 );
	DWORD	dwSetColorSelBarOnText	= RGBA( 255, 255, 255, 255 );

	int iSize = this->m_vecPoralList.size();

	this->m_pListCtrl->DeleteItemAll();

	for ( int i = 0; i < iSize; ++i)
	{
		int iIndex = this->m_pListCtrl->InsertItemBack();

        const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(m_vecPoralList[i].m_MapCode);
		assert(pMapInfo);
		if (pMapInfo)
		{
			g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMapName, 256);
			g_InterfaceManager.GetInterfaceString(51201, szBuffer, 256);
			Snprintf( szResult,INTERFACE_STRING_LENGTH-1, szBuffer, szMapName);

			GlobalFunc::SetItem( m_pListCtrl, iIndex, 0, szResult , StrToWzID("mn12"), DT_VCENTER |  DT_CENTER, dwBGColor, dwTextColor, dwSetColorSelBarOnText );
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
NPCPortalDialog::popupAskPortal(MAPCODE MapCode, MONEY Fare)
{
    WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndexVer2(g_pMap->GetCurrentFieldID(), MapCode);

    if (wIndex)
    {
        sMAP_ENTERENCE_PORTAL* entPortal =
            MapInfoParser::Instance()->GetMapPortal( wIndex );

        if (entPortal && g_pHero)
        {
            if (entPortal->dwHeim > g_pHero->GetMoney())
            {
				TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
				TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(eST_NEED_HEIM_MOVE_VILLAGE, szMessage, INTERFACE_STRING_LENGTH);
				Snprintf( szResult, INTERFACE_STRING_LENGTH-1, szMessage, entPortal->dwHeim );

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_VerifyLock( szResult );

                return;
            }

            Fare = entPortal->dwHeim;
        }
    }

	uiAssistMan* pMan = (uiAssistMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST);
	if(pMan)
	{
		pMan->ShowAskNPCPortal(MapCode, Fare);
	}
}