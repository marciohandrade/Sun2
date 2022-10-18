#include "SunClientPrecompiledHeader.h"
#include "uiinputnumtradeuidialog.h"
#include "uiTradeMan/uiTradeMan.h"
#include "hero.h"
#include "globalfunc.h"
#include "interfacemanager.h"

#define	MAX_EDIT_LENGTH	20
#define MAX_NUMBER_STRING	12

WzID uiInPutNumTradeUIDialog::m_wzId[DIALOG_MAX] =
{
	StrToWzID("B011"),							// DLGPOS_NUM0		  
	StrToWzID("B008"),							// DLGPOS_NUM1
	StrToWzID("BT01"),							// DLGPOS_NUM2
	StrToWzID("B010"),							// DLGPOS_NUM3
	StrToWzID("B006"),							// DLGPOS_NUM4
	StrToWzID("BT00"),							// DLGPOS_NUM5		
	StrToWzID("B007"),							// DLGPOS_NUM6
	StrToWzID("B002"),							// DLGPOS_NUM7
	StrToWzID("B003"),							// DLGPOS_NUM8
	StrToWzID("B004"),							// DLGPOS_NUM9
	StrToWzID("B012"),							// DLGPOS_NUM00
	StrToWzID("B014"),							// DLGPOS_NUM000

	StrToWzID("ST00"),							// DLGPOS_EDIT
	StrToWzID("B005"),							// DLGPOS_BACK
	StrToWzID("B009"),							// DLGPOS_CLEAR
	StrToWzID("B015"),							// DLGPOS_INPUT
	StrToWzID("B001"),							// DLGPOS_EXIT
};

char NumArray[MAX_NUMBER_STRING][5] = 
{
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"00",
	"000"
};


uiInPutNumTradeUIDialog::uiInPutNumTradeUIDialog( InterfaceManager * pUIMan )
:uiBase( pUIMan )
,m_pEditText( NULL )
{
	m_MoneyString.clear();
}

uiInPutNumTradeUIDialog::~uiInPutNumTradeUIDialog()
{
}

void uiInPutNumTradeUIDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init( pDrawBase );

	InitControls();
}

void uiInPutNumTradeUIDialog::Release()
{

}


void uiInPutNumTradeUIDialog::OnShowWindow( BOOL val )
{
	//this->m_bShow = val;
	StringClear();
}

void uiInPutNumTradeUIDialog::NetworkProc( MSG_BASE * pMsg )
{
}

void uiInPutNumTradeUIDialog::MessageProc( SI_MESSAGE * pMessage )
{
	if (this->IsLock())
        return;


    switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_LBUTTONCLICK:
		{
	        POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_NUM0:
			case DLGPOS_NUM00:
			case DLGPOS_NUM000:
			case DLGPOS_NUM1:
			case DLGPOS_NUM2:
			case DLGPOS_NUM3:
			case DLGPOS_NUM4:
			case DLGPOS_NUM5:
			case DLGPOS_NUM6:
			case DLGPOS_NUM7:
			case DLGPOS_NUM8:
			case DLGPOS_NUM9:
				SetDigit( pos );
				break;
			case DLGPOS_BACK:
				DeleteEndString();
				break;
			case DLGPOS_CLEAR:
				StringClear();
				break;
			case DLGPOS_INPUT:
				SendMoney();
				break;
			case DLGPOS_EXIT:
				ShowWindow( FALSE );
				break;
			}
		}
		break;

	}
}

void uiInPutNumTradeUIDialog::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
}

void uiInPutNumTradeUIDialog::InitControls()
{
	m_pEditText = (CCtrlStaticWZ *)getControl( DLGPOS_EDIT );
	if( m_pEditText != NULL )
		m_pEditText->SetTextWZ( "0" );
}


void uiInPutNumTradeUIDialog::SetDigit( POSTYPE pos )
{
	if( m_MoneyString.length() >= MAX_EDIT_LENGTH )
		return;

	if( pos >= MAX_NUMBER_STRING )
		return;

	std::string	temp( NumArray[pos] );

	BOOL IsZero = FALSE;
	if( temp == "0" || temp == "00" || temp == "000" )
	{
		if( m_MoneyString.empty() )
			IsZero = TRUE;
	}

	if( IsZero == FALSE )
		m_MoneyString += temp;

	UpdateEdit();
}

void uiInPutNumTradeUIDialog::CalculateThousandDigit( void )
{
	DeleteComma();
	size_t len = m_MoneyString.length();
	if( len > 3 )
	{
		int count = 1;
		for( int i = len - 1; i > 0; i-- )
		{
			if( count % 3 == 0 )
			{
				m_MoneyString.insert( i, "," ); 
			}

			count++;
		}
	}
}

void uiInPutNumTradeUIDialog::DeleteComma( void )
{
	STRING_ITRTYPE	itr = m_MoneyString.begin();
	STRING_ITRTYPE	end = m_MoneyString.end();

	for( ; itr != end; ++itr )
	{
		if( *itr == ',' )
			m_MoneyString.erase( itr );	
	}
}

void uiInPutNumTradeUIDialog::DeleteEndString()
{
	if( m_MoneyString.empty() )
		return;

	size_t len = m_MoneyString.length();
	m_MoneyString.erase( len - 1 );
	UpdateEdit();
}

void uiInPutNumTradeUIDialog::StringClear( void )
{
	m_MoneyString.clear();
	UpdateEdit();
}

void uiInPutNumTradeUIDialog::SendMoney()
{
	if( m_MoneyString.empty() )
	{
		//	금액을 입력해주세요!!
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_INPUT_MONEY, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
		return;
	}

	DeleteComma();
	MONEY Money = _tcstoui64( m_MoneyString.c_str(), NULL, 10 );

	if( g_pHero && (g_pHero->GetMoney() < Money) )
	{
		//	입력 하실 수 있는 금액은 현재 소지금에 제한 됩니다.
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( 70385, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
	}
	else
	{
		uiTradeMan	* pMan = (uiTradeMan*)GetMan();
		if( pMan )
			pMan->TEST_PutMoney( Money );

		StringClear();
		ShowWindow( FALSE );
	}
}

void uiInPutNumTradeUIDialog::UpdateEdit()
{
	if( m_MoneyString.empty() )
		UpdateEditText( "0" );
	else
	{
		CalculateThousandDigit();
		UpdateEditText( m_MoneyString.c_str() );
	}
}

void uiInPutNumTradeUIDialog::UpdateEditText( const char * szText )
{
	if( m_pEditText != NULL )
		m_pEditText->SetTextWZ( const_cast<char*>( szText ) );
}
