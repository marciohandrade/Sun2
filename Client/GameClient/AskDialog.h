#pragma once
#include "uiBase.h"
//------------------------------------------------------------------------------
/**

    Ȯ�� ���� ���̾�α� 
    @author
        -������, ��â��
    @since
        -2005.07.28 ck
        -2005.06.30
    @remarks
        - Todo:Ȯ�� ���� ���̾�α��� ��� �ν��Ͻ��� ������ ���� �Ҽ� �־�� ������ �������̽� �޴��� ������ �ʿ� �ϴ�.
*/

class AskDialog :
	public uiBase
{
public:
	enum ASK_POS
	{
		ASK_OK = 0,
        ASK_CANCEL,
        ASK_LIST,
		ASK_MAX,
	};

    enum ASK_STATUS
    {
        ASK_STATUS_NORMAL=0,
        ASK_STATUS_OK,
        ASK_STATUS_CANCEL,
    };

public:
	AskDialog();
	AskDialog(InterfaceManager *pUIMan);
	virtual ~AskDialog(void);

public:
	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

    ASK_STATUS      GetStatus();
	VOID			SetStatus(ASK_STATUS eStatus);

	VOID			GetControls();
    
    //����ó���Ұ�����������
    virtual void	InsertMessage(TCHAR	*szMessage,DWORD bkColor,DWORD TxtColor);

protected:

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
    virtual VOID	OnShowWindow( BOOL val );
    ASK_STATUS      m_status;
    static WzID	    m_wzId[ASK_MAX];

protected:
	CCtrlImageStaticWZ* m_pOKButton;
	CCtrlImageStaticWZ* m_pCancelButton;
	CCtrlListWZ *	m_pListCtrl;
};

inline
AskDialog::ASK_STATUS
AskDialog::GetStatus()
{
    return m_status;
}

inline VOID AskDialog::SetStatus(ASK_STATUS eStatus)
{
	m_status = eStatus;
}
