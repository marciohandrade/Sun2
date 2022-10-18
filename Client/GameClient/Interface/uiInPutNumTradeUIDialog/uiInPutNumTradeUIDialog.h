#pragma once
#include "uiBase.h"

class uiInPutNumTradeUIDialog : public uiBase
{
	typedef std::string::iterator STRING_ITRTYPE;

    enum eDIALOG_POS
    {
        DLGPOS_NUM0 = 0,
		DLGPOS_NUM1,
		DLGPOS_NUM2,
		DLGPOS_NUM3,
		DLGPOS_NUM4,
		DLGPOS_NUM5,
		DLGPOS_NUM6,
		DLGPOS_NUM7,
		DLGPOS_NUM8,
		DLGPOS_NUM9,
		DLGPOS_NUM00,
		DLGPOS_NUM000,

		DLGPOS_EDIT,
		DLGPOS_BACK,
		DLGPOS_CLEAR,
		DLGPOS_INPUT,
		DLGPOS_EXIT,

		DIALOG_MAX
	};

public:
	uiInPutNumTradeUIDialog( InterfaceManager * pUIMan );
	~uiInPutNumTradeUIDialog( void );

    virtual void            Init( CDrawBase * pDrawBase ); 
    virtual void            Release(); 

    virtual void	        OnShowWindow( BOOL val );

    virtual void	        NetworkProc( MSG_BASE * pMsg ); 
    virtual void	        MessageProc( SI_MESSAGE * pMessage );
	virtual void			MouseOver_MsgProc( SI_MESSAGE * pMessage );

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

private:
    void                    InitControls();
	void					SetDigit( BYTE digit );
	void					DeleteEndString( void );
	void					StringClear( void );
	void					SendMoney( void );
	void					UpdateEdit( void );
	void					UpdateEditText( const char * szText );
	void					CalculateThousandDigit( void );
	void					DeleteComma( void );


private:
	CCtrlStaticWZ *			m_pEditText;
	std::string				m_MoneyString;
};

inline POSTYPE uiInPutNumTradeUIDialog::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }

    return DIALOG_MAX;
}

inline CControlWZ * uiInPutNumTradeUIDialog::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );
    return GetControlWZ( m_wzId[AtPos] );
}
