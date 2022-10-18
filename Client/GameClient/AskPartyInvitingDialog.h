#pragma once
#include "solardialog.h"
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

class AskPartyInvitingDialog :
    public AskDialog
{
public:
    AskPartyInvitingDialog(void);
    virtual ~AskPartyInvitingDialog(void);

    virtual VOID    Process(DWORD dwTick);

protected:
    virtual void	update();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	showWindow( BOOL val );

    BOOL    bDestroy;                             /// 타이머
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///
};
