#pragma once
#include "solardialog.h"
//------------------------------------------------------------------------------
/**
    Ȯ�� ���� ���̾�α� 
    @author
    -��â��
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

    BOOL    bDestroy;                             /// Ÿ�̸�
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///
};
