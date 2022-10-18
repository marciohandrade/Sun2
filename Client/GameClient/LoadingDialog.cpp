#include "SunClientPrecompiledHeader.h"
#include "loadingdialog.h"

WzID LoadingDialog::m_wzId[LoadingDialog::Loading_Max] = {
	StrToWzID("P001"),
};

LoadingDialog::LoadingDialog(void)
{
}

LoadingDialog::~LoadingDialog(void)
{
}

VOID LoadingDialog::Init( CDrawBase * pDrawBase )
{

}

VOID LoadingDialog::Release()
{

}

VOID LoadingDialog::MessageProc( SI_MESSAGE * pMessage )
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

VOID LoadingDialog::NetworkProc( MSG_BASE * pMsg )
{

}

VOID LoadingDialog::Update()
{
}

CControlWZ * LoadingDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < Loading_Max );

	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE		 LoadingDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Loading_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Loading_Max;
}

