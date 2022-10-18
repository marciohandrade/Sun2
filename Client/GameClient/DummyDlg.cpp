//---------------------------------------------------------------------------
//
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "dummydlg.h"


WzID DummyDlg::m_wzId[] = 
{
	0,
};


DummyDlg::DummyDlg(void)
{

}

DummyDlg::~DummyDlg(void)
{

}


VOID	
DummyDlg::Init( CDrawBase * pDrawBase )
{

}


VOID	
DummyDlg::Release()
{

}


VOID	
DummyDlg::MessageProc( SI_MESSAGE * pMessage )
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



VOID	
DummyDlg::NetworkProc( MSG_BASE * pMsg )
{

}


VOID	
DummyDlg::OnShowWindow(BOOL val)
{

}


CControlWZ *	
DummyDlg::getControl( POSTYPE AtPos )
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


POSTYPE			
DummyDlg::getCtrlIDToPos( WzID wzId )
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