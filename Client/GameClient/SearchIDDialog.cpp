#include "SunClientPrecompiledHeader.h"
//#include ".\searchiddialog.h"
//#include "InterfaceManager.h"
//
//
//WzID SearchIDDialog::m_wzId[SRCH_MAX] = {
//	StrToWzID("B001"),
//	StrToWzID("B002"),
//	StrToWzID("E001"),	
//};
//
//SearchIDDialog::SearchIDDialog(void)
//{
//}
//
//
//SearchIDDialog::~SearchIDDialog(void)
//{
//}
//
//
//VOID SearchIDDialog::Init( CDrawBase * pDrawBase )
//{
//	m_eAnswer = Answer_NO;
//}
//
//
//VOID SearchIDDialog::Release()
//{
//
//}
//
//
//VOID SearchIDDialog::MessageProc( SI_MESSAGE * pMessage )
//{
//	switch( pMessage->eResultMsg ) 
//	{
//	case RT_MSG_LBUTTONCLICK:
//		{
//			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
//			{
//			case B001:	// È®ÀÎ
//				{
//					g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_SEARCH_ID, FALSE ); 
//				}
//				break;
//
//			case B002:
//				{
//					g_InterfaceManager.ShowDialog( InterfaceManager::DIALOG_SEARCH_ID, FALSE ); 
//				}
//				break;
//			}		
//		}
//		break;
//
//	case RT_MSG_EDIT_ENTERKEYDOWN:
//		{
//			_asm nop;
////			if( m_bSendPacket )  return;
//
////			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
//		}
//		break;
//
//	default:
//		{
//			SolarDialog::MessageProc( pMessage );
//		}
//		break;
//	}
//}
//
//
//VOID SearchIDDialog::NetworkProc( MSG_BASE * pMsg )
//{
//
//}
//
//
//VOID SearchIDDialog::update()
//{
//
//}
//
//
//CControlWZ * SearchIDDialog::getControl( POSTYPE AtPos )
//{
//	assert( AtPos < SRCH_MAX );
//
//	return GetControlWZ( m_wzId[AtPos] );
//}
//
//POSTYPE	SearchIDDialog::getCtrlIDToPos( WzID wzId )
//{
//	for( POSTYPE i = 0 ; i < SRCH_MAX; ++i )
//	{
//		if( m_wzId[i] == wzId )
//		{
//			return i;
//		}
//	}
//	return SRCH_MAX;
//
//}
