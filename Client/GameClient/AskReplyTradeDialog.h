#pragma once
//------------------------------------------------------------------------------
/**
    확인 선택 다이얼로그 
    @author
    -방창규
    @since
    -2005.07.28 ck
    @remarks
*/
#include "AskDialog.h"

class AskReplyTradeDialog:
    public AskDialog
{
public:
    AskReplyTradeDialog(InterfaceManager *pUIMan);
    virtual ~AskReplyTradeDialog(void);

    virtual VOID    Process(DWORD dwTick);

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

    BOOL    bDestroy;                             /// 타이머
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///
};
