#pragma once
#include "solardialog.h"
//------------------------------------------------------------------------------
/**

    ĳ���� ���� ���̾� �α� 
    @author

    @since

    @remarks
        -2005.10.19 ���� �������̽� �и�-���� 
        - 
*/

class CharSelectFontDialog : public SolarDialog
{
public:
	CharSelectFontDialog(void);
	virtual ~CharSelectFontDialog(void);
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual VOID	Update();

	virtual VOID	OnShowWindow(BOOL val);
};
