#include "SunClientPrecompiledHeader.h"
#include "uissqenterwindow.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "GameFunc.h"
#include "ProjectileManagerEx.h"
#include "MouseHandler.h"
#include "InterfaceManager.h"


WzID uiSSQEnterWindow::m_wzID[MAX_SSQ_ENTER_WINDOW] =
{
	StrToWzID("B004"),
	StrToWzID("BT00"),
	StrToWzID("ST01"),
	StrToWzID("PI00"),
	StrToWzID("PI01"),
	StrToWzID("PI02"),
	StrToWzID("PI03"),
	StrToWzID("PI04"),
	StrToWzID("PI05"),

	StrToWzID("ST08"),
	StrToWzID("ST14"),
	StrToWzID("ST09"),
	StrToWzID("ST15"),
	StrToWzID("ST10"),
	StrToWzID("ST16"),
	StrToWzID("ST11"),
	StrToWzID("ST17"),
	StrToWzID("ST12"),
	StrToWzID("ST18"),
	StrToWzID("ST13"),
	StrToWzID("ST19"),

	StrToWzID("B001"),
};



uiSSQEnterWindow::uiSSQEnterWindow( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
}

uiSSQEnterWindow::~uiSSQEnterWindow(void)
{
}

void uiSSQEnterWindow::Init( CDrawBase * pDrawBase )
{
	m_pCreatePortalButton = getControl( CREATE_PORTAL_BUTTON );
	SetLevelCostString();
}

void uiSSQEnterWindow::SetSelLine( int index )
{
	int find = index + SEL_LINE1_PICTURE -1;

	for( int i = SEL_LINE1_PICTURE; i < ENTER_LEVEL1_STATIC; ++i )
	{
		CControlWZ * pControl = getControl( i );

		if( pControl != NULL )
		{
			if( find == i )
			{
				pControl->ShowWindowWZ( WZ_SHOW );
			}
			else
			{
				pControl->ShowWindowWZ( WZ_HIDE );
			}
		}
	}
}

void uiSSQEnterWindow::ClearLine()
{
	for( int i = SEL_LINE1_PICTURE; i < ENTER_LEVEL1_STATIC; ++i )
	{
		CControlWZ * pControl = getControl( i );

		if( pControl != NULL )
			pControl->ShowWindowWZ( WZ_HIDE );
	}
}

int ContrlIndex[6][2] = 
{
	ENTER_LEVEL1_STATIC, ENTER_COST1_STATIC,
	ENTER_LEVEL2_STATIC, ENTER_COST2_STATIC,
	ENTER_LEVEL3_STATIC, ENTER_COST3_STATIC,
	ENTER_LEVEL4_STATIC, ENTER_COST4_STATIC,
	ENTER_LEVEL5_STATIC, ENTER_COST5_STATIC,
	ENTER_LEVEL6_STATIC, ENTER_COST6_STATIC,
};




void uiSSQEnterWindow::SetControl( int index, WORD MinLevel, WORD MaxLevel, MONEY cost )
{
	int level_index	= ContrlIndex[index - 1][0];
	int cost_index	= ContrlIndex[index - 1][1];
	
	CCtrlStaticWZ * pLevel = (CCtrlStaticWZ*)getControl( level_index );
	CCtrlStaticWZ * pCost  = (CCtrlStaticWZ*)getControl( cost_index );
	TCHAR szBuff[MAX_PATH] = { '\0', };

	if( pLevel != NULL && pCost != NULL )
	{
		//Sprintf( szBuff, "%d ~ %d", MinLevel, MaxLevel );
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0, };
		g_InterfaceManager.GetInterfaceString(1757,szTemp);
		Sprintf( szBuff, szTemp, MinLevel, MaxLevel );
		GlobalFunc::SetCaption( pLevel, szBuff );

        GlobalFunc::SetMoneyText(pCost, cost);
	}
}

void uiSSQEnterWindow::SetLevelCostString()
{
	typedef SSQInfoParser::SSQINFO_TABLE	SSQINFO_TABLE;
	const SSQINFO_TABLE& rTable = SSQInfoParser::Instance()->GetRefSSQInfo();

	FOREACH_CONTAINER( SSQINFO_TABLE::value_type& rNode, rTable, SSQINFO_TABLE )
	{
		const SSQInfo * const pSSQInfo = rNode.second;
		SetControl( pSSQInfo->m_Index, pSSQInfo->m_LevelRange[SSQInfo::LV_MIN], pSSQInfo->m_LevelRange[SSQInfo::LV_MAX], pSSQInfo->m_Cost );
	}
}

void uiSSQEnterWindow::Release()
{

}

void uiSSQEnterWindow::Process( DWORD dwTick )
{
}

void uiSSQEnterWindow::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		CanOpenPortal();
	}
	else
	{

	}
}

void uiSSQEnterWindow::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case CREATE_PORTAL_BUTTON:
				{
					if( CanOpenPortal() )
					{
						SendCreatePortal();
						ShowWindow( FALSE );
					}
					else
					{						
						
					}
				}
				break;
			case EXIT_BUTTON:
			case CANCEL_BUTTON:
				{
					ShowWindow( FALSE );
				}
				break;
			}
		}
		break;
	}
}

SSQInfo::INDEX_T uiSSQEnterWindow::CanOpenPortal()
{
	if( g_pHero == NULL )
		return 0;

	CCtrlStaticWZ * pNotice = (CCtrlStaticWZ*)getControl( NOTICE_STATIC );
	if( pNotice )
	{
		TCHAR szNotice[INTERFACE_STRING_LENGTH];
		TCHAR szTemp[INTERFACE_STRING_LENGTH];

		g_InterfaceManager.GetInterfaceString( 1755, szTemp ); 

		Sprintf( szNotice, szTemp, g_pHero->GetName() );


		GlobalFunc::SetCaption( pNotice, szNotice );
	}

	ClearLine();

	LEVELTYPE	level = g_pHero->GetLevel();
	MONEY		money = g_pHero->GetMoney();

	const SSQInfo * pInfo = SSQInfoParser::Instance()->FindSSQInfoEnterableLevel( level );

	//해당레벨에 대한 정보가 없다는것은 정의가 되지않았다고 판단(10이하레벨)
	if( pInfo == NULL )
	{
		//6430:입장할수없는 레벨입니다.
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 6430 ) );		
		return 0;
	}

	SetSelLine( pInfo->m_Index );

	if( money < pInfo->m_Cost )
	{
		//6411: 입장료가 부족합니다.
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 6411 ) );		
		return 0;
	}

	
	
	return pInfo->m_Index;
}

void uiSSQEnterWindow::SendCreatePortal()
{
	MSG_CG_ZONE_EX_SSQ_JOIN_INFO_SYN	SendPacketAck;
	SendPacketAck.m_ItemPos	= MouseHandler::Instance()->GetPendingUseChargeItemPos();
	GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));

}

