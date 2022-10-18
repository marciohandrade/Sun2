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


    BOOL    bDestroy;                             /// 타이머
    BOOL    bDestroyTimer;                        ///
    DWORD   dwDestroyTick;                        ///
    DWORD   dwDestroyTickOver;                    ///


};
