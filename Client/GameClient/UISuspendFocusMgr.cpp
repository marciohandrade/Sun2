//------------------------------------------------------------------------------
//  UISuspendFocusMgr.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
//#include ".\uisuspendfocusmgr.h"
//#include "InterfaceManager.h"
//#include "SolarDialog.h"
//#include "globalfunc.h"
////-------------------------------------------------------------------------------------------
///**
//*/
//UISuspendFocusMgr::UISuspendFocusMgr(void)
//{
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//UISuspendFocusMgr::~UISuspendFocusMgr(void)
//{
//	ClearFocus();
//}
//
//
////-------------------------------------------------------------------------------------------
///**
//	SetFocusCtrl 호출시 또는 유저의 마우스 입력에 의해서 호출
//	현재는 Edit 컨트롤에 의해서만 의미가 있다.
//*/
//BOOL	 
//UISuspendFocusMgr::ChangeFocusTransact(SolarDialog * pDlg, WzID wzControlID)
//{
//	assert(pDlg);
//	if (!pDlg)
//	{
//		return FALSE;
//	}
//
//	WzID DialogID = (WzID)pDlg->GetDialogWZ()->GetDlgID();
//	WzID ControlID = wzControlID;
//
//	CControlWZ * pControl = pDlg->GetDialogWZ()->GetControlWZ( ControlID );
//	if ( !pControl )
//	{
//		RequestFocusTransact(pDlg);
//	}
//	else
//	if ( CTRL_KIND_EDIT != pControl->GetCtrlInfo()->m_dwKind )
//	{
//		// 에디터가 아닌 다른 컨트롤
//		RequestFocusTransact(pDlg);
//	}
//	else
//	{ 
//		// 정상적으로 에디터로 이동할때.
//
//		// 같은 다이얼로그의 정보가 있냐?
//		FOCUS_ITR itr = find(DialogID);
//		if (this->m_SuspendFocusDeque.end() != itr) 
//		{
//			// 있다면 갱신
//			update(DialogID, ControlID);
//		}
//		else
//		{
//			// 없다면 새로 넣음.
//			add(DialogID, ControlID);
//		}
//	}
//
//	return TRUE;
//}
//
//void	
//UISuspendFocusMgr::RequestFocusTransact(SolarDialog * pDlg)
//{
//	assert(pDlg);
//	if (!pDlg)
//	{
//		return;
//	}
//
//	if ( 0 == m_SuspendFocusDeque.size() )
//	{
//		return;
//	}
//
//	this->returnPendingFocus();
//}
//
////-------------------------------------------------------------------------------------------
///** 다이얼로그를 하이드 할때 호출해준다.
//*/
//void	
//UISuspendFocusMgr::RemoveFocus(SolarDialog * pDlg)
//{
//	if ( !pDlg )
//	{
//		return;
//	}
//
//	remove(pDlg->GetDialogWZ()->GetDlgID());
//
//	if (m_SuspendFocusDeque.size())
//	{
//		// 펜딩된 포커스가 있다.
//		this->returnPendingFocus();
//	}
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void	
//UISuspendFocusMgr::ForceChangeFocus(SolarDialog * pDlg, WzID wzControlID)
//{
//	if ( !this->isValidFocus( pDlg, wzControlID) )
//	{
//		return;
//	}
//
//	this->ClearFocus();
//	this->add(pDlg->GetDialogWZ()->GetDlgID(), wzControlID);
//	this->returnPendingFocus();
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void		
//UISuspendFocusMgr::ClearFocus()
//{
//	FOCUS_ITR itr = m_SuspendFocusDeque.begin();
//
//	while (itr != m_SuspendFocusDeque.end())
//	{
//		UISuspendFocus * pInfo = (*itr);
//
//		SAFE_DELETE( pInfo );
//
//		itr++;
//	}
//
//	m_SuspendFocusDeque.clear();
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//BOOL		
//UISuspendFocusMgr::isValidFocus(SolarDialog * pDlg, WzID ControlID)
//{
//	if ( !pDlg )
//	{
//		return FALSE;
//	}
//
//	CControlWZ * pControl = pDlg->GetControlWZ(ControlID);
//	if ( !pControl )
//	{
//		return FALSE;
//	}
//
//	if ( CTRL_KIND_EDIT != pControl->GetCtrlInfo()->m_dwKind )
//	{
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//FOCUS_ITR	
//UISuspendFocusMgr::find( WzID DialogID)
//{
//	FOCUS_ITR itr = m_SuspendFocusDeque.begin();
//
//	while (itr != m_SuspendFocusDeque.end())
//	{
//		UISuspendFocus * pInfo = (*itr);
//
//		if ( pInfo->wzDialogID == DialogID )
//		{
//			break;
//		}
//
//		itr++;
//	}
//
//	return itr;
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//BOOL		
//UISuspendFocusMgr::add(WzID DialogID, WzID ControlID )
//{
//	if ( this->m_SuspendFocusDeque.end() != find(DialogID) )
//	{
//		return FALSE;
//	}
//
//	UISuspendFocus * pSuspendFocus = new UISuspendFocus(DialogID, ControlID);
//	if ( !pSuspendFocus )
//	{
//		return FALSE;
//	}
//
//	this->m_SuspendFocusDeque.push_front( pSuspendFocus );
//
//	return TRUE;
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void		
//UISuspendFocusMgr::remove( WzID DialogID )
//{
//	FOCUS_ITR itr = find(DialogID);
//
//	if ( itr == m_SuspendFocusDeque.end() )
//	{
//		return;
//	}
//
//	m_SuspendFocusDeque.erase( itr );
//}
//
////-------------------------------------------------------------------------------------------
///**
//*/
//void		
//UISuspendFocusMgr::update( WzID DialogID, WzID ControlID )
//{
//	FOCUS_ITR itr = find(DialogID);
//
//	if ( itr == m_SuspendFocusDeque.end() )
//	{
//		return;
//	}
//
//	UISuspendFocus * pFocus = *itr;
//	SAFE_DELETE(pFocus);
//	m_SuspendFocusDeque.erase( itr );
//
//	add(DialogID, ControlID);
//}
//
////-------------------------------------------------------------------------------------------
///** 펜딩된 다이얼로그의 컨트롤로 포커스를 돌려준다.
//*/
//void		
//UISuspendFocusMgr::returnPendingFocus()
//{
//	if (0 == this->m_SuspendFocusDeque.size()) 
//	{
//		return;
//	}
// 	
//	FOCUS_ITR itr = this->m_SuspendFocusDeque.begin();
//	UISuspendFocus * pFocus = *itr;
//	SolarDialog * pFocusDlg = g_InterfaceManager.GetDialog((WzID) pFocus->wzDialogID);
//	if ( pFocusDlg )
//	{
//		if ( pFocusDlg->IsSkipCheckSuspendFocus() )
//		{
//			return;
//		}
//
//		CCtrlDxEditBox * pFocusEdit = static_cast<CCtrlDxEditBox *>( pFocusDlg->GetControlWZ( pFocus->wzControlID ) );
//
//		if ( pFocusEdit )
//		{
//			g_InterfaceManager.GetDialogManager()->RequestFocus( pFocusDlg->GetDialogWZ() );
//			GlobalFunc::SetFocus(pFocusDlg, pFocusEdit);
//		}
//	}
//}