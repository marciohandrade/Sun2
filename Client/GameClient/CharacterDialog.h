#ifndef __CHARACTER_DIALOG_H__
#define __CHARACTER_DIALOG_H__

#pragma once

#include "solardialog.h"

class CharacterDialog : public SolarDialog
{
	enum eCHR_POS
	{	
		CHR_BCLS = 0,
		CHR_MAX,
	};

public:
	CharacterDialog();
	virtual ~CharacterDialog();

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[CHR_MAX];
	
};

#endif // __CHARACTER_DIALOG_H__