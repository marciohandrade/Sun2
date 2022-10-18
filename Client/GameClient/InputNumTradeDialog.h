#ifndef     _INPUTNUMTRADEDIALOG_H
#define     _INPUTNUMTRADEDIALOG_H
//------------------------------------------------------------------------------
/**
    @class InputNumTradeDialog

    �ŷ����� �Ӵ� �Է�â Ŭ����

    (C) 2005 ckbang
*/
#include "InputNumDialog.h"

class InputNumTradeDialog : public InputNumDialog
{
public:
    InputNumTradeDialog(void);
    virtual ~InputNumTradeDialog(void);

	enum
	{
		TIMER_STAT_NONE = 0,
		TIMER_STAT_IDLE,
		TIMER_STAT_FINISH,
	};

    virtual VOID    Init( CDrawBase * pDrawBase );
    virtual VOID    Release();

public:
    virtual VOID    MessageProc( SI_MESSAGE * pMessage );
	void            Exit();
protected:
    virtual VOID    OnShowWindow( BOOL val );

    void            MoneyProcess();
    bool            checkDigit(const TCHAR* str);
};

//------------------------------------------------------------------------------
#endif  //  _INPUTNUMTRADEDIALOG_H
