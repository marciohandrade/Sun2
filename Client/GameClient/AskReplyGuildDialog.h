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

class AskReplyGuildDialog:
    public AskDialog
{
public:
    AskReplyGuildDialog(InterfaceManager *pUIMan);
    virtual ~AskReplyGuildDialog(void);

    virtual VOID    Process(DWORD dwTick);

	void	SetInviteOperUserKey(DWORD dwOperObjectKey) {  m_dwOperObjectKey = dwOperObjectKey;}

protected:
    virtual void	OnUpdateSolarDialog();
    virtual void    MessageProc( SI_MESSAGE * pMessage );
    virtual VOID	OnShowWindow( BOOL val );

	void	NET_SEND_CG_GUILD_INVITE_ANSWER_SYN(bool bInviteAccept);
	DWORD	m_dwOperObjectKey;


    BOOL    bDestroy;                             /// Ÿ�̸�
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///


};
