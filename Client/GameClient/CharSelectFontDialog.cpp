//------------------------------------------------------------------------------
//  CharSelectFontDialog.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "charselectfontdialog.h"
#include "InterfaceManager.h"
#include "GameFramework.h"
#include "Loadscene.h"
#include "CharacterScene.h"
#include "Application.h"

//------------------------------------------------------------------------------
/**
*/
CharSelectFontDialog::CharSelectFontDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
CharSelectFontDialog::~CharSelectFontDialog(void)
{

}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectFontDialog::Init( CDrawBase * pDrawBase )
{
	RECT rcDlgSize;
	this->GetDialogWZ()->GetDlgSize(&rcDlgSize);

	RECT rcInterface;
	POINT pt = g_pApplication->Get2DModeMapping();

	rcInterface.left = rcInterface.top = 0;
	rcInterface.right = pt.x;
	rcInterface.bottom = pt.y;
	
	int x = (rcInterface.right / 2) - (rcDlgSize.right / 2);
	int y = (rcInterface.bottom / 2) - (rcDlgSize.bottom / 2);	

	MoveWindowWZ( (float)x, (float)y );
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectFontDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectFontDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case 0:
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
CharSelectFontDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
		
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectFontDialog::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/**
*/
VOID CharSelectFontDialog::Update()
{
	//MoveWindowWZ(0,0);
}
