#pragma once
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

    BOOL    bDestroy;                             /// Ÿ�̸�
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///
};
