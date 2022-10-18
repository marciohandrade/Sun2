#include "SunClientPrecompiledHeader.h"
#include ".\tooltipdialog.h"
#include "ItemInfoParser.h"
#include "InterfaceManager.h"

WzID TooltipDialog::m_wzId[] = 
{
	StrToWzID("L000"),
};

TooltipDialog::TooltipDialog(void)
{
}

TooltipDialog::~TooltipDialog(void)
{
}



VOID TooltipDialog::Init( CDrawBase * pDrawBase )
{
}

VOID TooltipDialog::Release()
{

}

VOID TooltipDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch (pMessage->eResultMsg)
	{
	case 0:
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


VOID TooltipDialog::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * TooltipDialog::getControl( POSTYPE AtPos )
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

POSTYPE	TooltipDialog::getCtrlIDToPos( WzID wzId )
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


VOID TooltipDialog::ShowToolTipInfo(int x, int y, const TOOLTIP_INFO & info)
{
	MoveWindowWZ(x, y);

	switch (info.eToolTipType)
	{
	case TOOLTIP_TYPE_ITEM:
		{
			_showItemToolTip(info.dwItemCode);
		}
		break;

	case TOOLTIP_TYPE_ETC:
		{
			_showETCToolTip(info.szInfo);
		}
		break;
	}

//	g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_TOOLTIP, TRUE);
}

VOID TooltipDialog::HideToolTipInfo()
{
//	g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_TOOLTIP, FALSE);
}

RECT TooltipDialog::CalcClipRect(int x, int y)
{
	RECT rc = {0,};
	return rc;
}


VOID TooltipDialog::_showItemToolTip(DWORD dwItemCode)
{
	CCtrlListWZ * pList = (CCtrlListWZ *)getControl(TOOLTIP_LIST);

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);

	if (!pItemInfo )
	{
		return;
	}

	// 보여줄 위치를 계산한다.
	RECT rcClient;
	GetDialogWZ()->GetClientRect( &rcClient );

	// 정보를 채운다. 
	
	pList->DeleteItemAll();

	int idx = pList->InsertItemBack();
	int iSubIdx = 0;

	pList->SetItemTextWZA(idx, (char *)pItemInfo->m_pszName);
	
}


VOID TooltipDialog::_showETCToolTip(TCHAR * szInfo)
{

}
