#include "SunClientPrecompiledHeader.h"
#include ".\submissioninfodialog.h"
#include "BattleLobby.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//----------------------------------------------------------------------------
/**
*/
WzID SubMissionInfoDialog::m_wzId[] = 
{
	StrToWzID("C001"),	//eSUBMISSION_CHECK_FIELD_1,
	StrToWzID("C002"),	//eSUBMISSION_CHECK_FIELD_2,
	StrToWzID("C003"),	//eSUBMISSION_CHECK_FIELD_3,
};


//----------------------------------------------------------------------------
/**
*/
SubMissionInfoDialog::SubMissionInfoDialog(void)
{

}


//----------------------------------------------------------------------------
/**
*/
SubMissionInfoDialog::~SubMissionInfoDialog(void)
{

}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::Init( CDrawBase * pDrawBase )
{
	this->m_pBattleLobby	 = NULL;
	this->m_pBattleRoomEntry = NULL;

	for ( int i = 0; i < 3; ++i)
	{
		m_pCtrlCheckField[i] = (CCtrlButtonCheckWZ *)getControl(eSUBMISSION_CHECK_FIELD_1+i);
		assert(m_pCtrlCheckField[i]);
	}
    m_ValidMapTextColor   = m_pCtrlCheckField[0]->GetTextColor();
	m_InvalidMapTextColor = RGBA(119,119,119,119); 
}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::Release()
{

}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::MessageProc( SI_MESSAGE * pMessage )
{
	if (!this->m_pBattleLobby || !this->m_pBattleRoomEntry)
	{
		return;
	}

	switch (pMessage->eResultMsg)
	{
	case RT_MSG_LBUTTONDOWN:
		{
			POSTYPE CurPos = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( CurPos )
			{
			case eSUBMISSION_CHECK_FIELD_1:
			case eSUBMISSION_CHECK_FIELD_2:
			case eSUBMISSION_CHECK_FIELD_3:
				{
					if (!this->m_pBattleLobby->IsMaster())
					{
						return;
					}
					
					int FieldIndex = CurPos - eSUBMISSION_CHECK_FIELD_1;

					ModifyMapCode(m_pBattleRoomEntry->GetMapCode(), FieldIndex );
				}
				break;

			default:
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


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::NetworkProc( MSG_BASE * pMsg )
{
	switch (pMsg->m_byProtocol)
	{
	case CG_ZONE_LOBBY_CHANGE_MAP_ACK:
	case CG_ZONE_LOBBY_CHANGE_MAP_NAK:
	case CG_ZONE_LOBBY_CHANGE_MAP_BRD:
		{
			BIT_REMOVE(this->m_packetStatus, PACKET_STATUS_SEND_CONFIG);
		}
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::update()
{
	if ( !this->m_pBattleRoomEntry )
	{
		return;
	}

	updateMapConfig();
}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::render()
{	
	for ( size_t i = 0; i < GetDialogWZ()->GetControlCount(); ++i )
	{
		CControlWZ * pControl = GetDialogWZ()->FindControlWZ(i);
		if ( pControl )
			GetDialogWZ()->OnDrawControlWZ( pControl->GetCtrlID() );
	}

}


//----------------------------------------------------------------------------
/**
*/
VOID	
SubMissionInfoDialog::showWindow(BOOL val)
{
	if (val)
	{
//        m_usSubMissionIdx = 0;

  //      BattleLobby * pBattleDlg = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
		//m_pBattleLobby = pBattleDlg;
		//assert(m_pBattleLobby);
		//this->m_pBattleRoomEntry = &m_pBattleLobby->m_BattleRoomEntry;

		//UpdatePosition();
	}
	else
	{

	}
}

void     
SubMissionInfoDialog::UpdatePosition()
{
	this->MoveWindowWZ(748, 298);
}

//----------------------------------------------------------------------------
/**
*/
VOID			
SubMissionInfoDialog::ModifyMapCode( CODETYPE MapCode, int index )
{
	//if (!BIT_CHECK(this->m_packetStatus, PACKET_STATUS_SEND_CONFIG))
	//{
	//	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(MapCode);
	//	if ( !pMapInfo) 
	//	{
	//		assert(pMapInfo);
	//		return;
	//	}

	//	MapGroup * pMapGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );

	//	if ( !pMapGroup ) 
	//	{
	//		assert(pMapGroup);
	//		return;
	//	}

	//	sMAPINFO * pMapInfo2 = pMapGroup->GetMapInfoIndex(index);
	//	if (!pMapInfo2) 
	//	{
	//		return;
	//	}

	//	BIT_ADD(this->m_packetStatus, PACKET_STATUS_SEND_CONFIG);

	//	MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN SendPacket;
	//	this->m_pBattleLobby->m_PendingMapCode = SendPacket.m_MapCode = pMapInfo2->MapCode;

	//	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof( SendPacket ));
	//}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
SubMissionInfoDialog::updateMapConfig()
{
	if (!this->m_pBattleRoomEntry)
	{
		return;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH];

	int iFieldIndex = 0;
	int i = 0;

    for ( i = 0; i < 3; ++i )
    {
        //		m_pCtrlCheckField[i]->SetTextColor(m_InvalidMapTextColor);
        m_pCtrlCheckField[i]->ShowWindowWZ(WZ_HIDE);
    }

	const MapGroup * pGroupInfo	=  this->m_pBattleRoomEntry->GetCurGroup();
	assert(pGroupInfo);
	if ( !pGroupInfo)
	{
		return;
	}

	sMAPINFO_HASH & rMapInfoHash = const_cast<MapGroup *>(pGroupInfo)->GetMapInfoHash();
	sMAPINFO * pMapInfo = NULL;
	sMAPINFO_HASH::iterator itr = rMapInfoHash.begin();
	for( ; itr != rMapInfoHash.end(); ++itr)
	{
		pMapInfo =  itr->second ;
		if ( pMapInfo->byMType < 3 )	// 스크립트 실수로 인해서 뻑나는 문제 막기 위해서 넣어둔다.
		{
			g_InterfaceManager.GetInterfaceString(pMapInfo->dwNCode, szMessage, INTERFACE_STRING_LENGTH);
			m_pCtrlCheckField[pMapInfo->byMType]->SetTextWZ(szMessage );

//			m_pCtrlCheckField[pMapInfo->byMType]->SetTextColor(m_ValidMapTextColor);
			m_pCtrlCheckField[pMapInfo->byMType]->ShowWindowWZ(WZ_SHOW);
		}
		
	}

	iFieldIndex = this->m_pBattleRoomEntry->GetCurMapInfo()->byMType;

	for (i = 0; i < 3; ++i)
	{
		m_pCtrlCheckField[i]->SetCheckState(FALSE);
	}
	m_pCtrlCheckField[iFieldIndex]->SetCheckState(TRUE);
}

//----------------------------------------------------------------------------
/**
*/
CControlWZ *	
SubMissionInfoDialog::getControl( POSTYPE AtPos )
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
SubMissionInfoDialog::getCtrlIDToPos( WzID wzId )
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
