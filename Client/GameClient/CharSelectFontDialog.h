#pragma once
#include "solardialog.h"
//------------------------------------------------------------------------------
/**

    캐릭터 선택 다이얼 로그 
    @author

    @since

    @remarks
        -2005.10.19 상하 인터페이스 분리-지완 
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
