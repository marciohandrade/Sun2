#include "SunClientPrecompiledHeader.h"
#include "CharacterDialog.h"

WzID CharacterDialog::m_wzId[CHR_MAX] = 
{ 
	StrToWzID("BCLS"),
};


CharacterDialog::CharacterDialog()
{

}
CharacterDialog::~CharacterDialog()
{
}

VOID CharacterDialog::Init( CDrawBase * pDrawBase )
{
	GetDialogWZ()->MoveWindowWZ( 0, 95);
}

VOID CharacterDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case CHR_BCLS:
				{
					ShowWindow( FALSE );
				}
				break;
			}
		}
		break;
	}
}

VOID CharacterDialog::NetworkProc( MSG_BASE * pMsg )
{
}

POSTYPE CharacterDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CHR_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CHR_MAX;
}

CControlWZ * CharacterDialog::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < CHR_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
