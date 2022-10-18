#ifndef __LOGIN_DIALOG_H__
#define __LOGIN_DIALOG_H__
//------------------------------------------------------------------------------
/**
@class LoginScene

�α��� ��
���������� ���ؼ� Ŭ���̾�Ʈ �α����� �����ϸ�,
���� ������ �����Ѵ�.

(C) 2005 ckbang
*/
#include "solardialog.h"

class LoginDialog : public SolarDialog
{
	enum eLOGIN_POS
	{	
        LOGIN_ID = 0,
		LOGIN_PW,
        LOGIN_CONNECT,
        LOGIN_REGISTER,
        LOGIN_EXIT,
		LOGIN_MAX,
	};	
public:
	LoginDialog();
	~LoginDialog();

    enum eLOGIN_TXT_FIELD
    {
        TXT_FIELD_ID = 0,
        TXT_FIELD_PW,
        TXT_FIELD_ALL,
        TXT_FIELD_MAX,
    };

    VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg ){}
    void            Clear(eLOGIN_TXT_FIELD tf=TXT_FIELD_ALL);
    void            execLoginQuery(char *strID=NULL, char *strPW=NULL);

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[LOGIN_MAX];

	VOID			showWindow(BOOL val);

	// �ÿ��� �ӽ� 
	BOOL			m_bSendPacket;

    void            makePasswordCryption(char *pw);

    CDrawBase *		m_pDraw;
};

#endif // __LOGIN_DIALOG_H__