#include "SunClientPrecompiledHeader.h"
#include "Mouse.h"
#include "Application.h"
#include "interfacemanager.h"

Mouse::Mouse()
{
	m_pMouseInfo	= NULL;
	m_nWheelMsg		= 0;
	m_nQueSize		= MAX_QUESIZE;
	m_bInited		= FALSE;

	m_iMoveSpeed	= GetSystemMoveSpeed();
	m_iBackupMoveSpeed = m_iMoveSpeed;
	m_iMoveLevel	= GetUserDefaultMoveLevel(m_iMoveSpeed);
	SetMoveLevel(m_iMoveLevel);
	SetMoveRange(0,5);


}

Mouse::~Mouse()
{

	m_iBackupMoveSpeed = m_iBackupMoveSpeed < 1  ? 1: m_iBackupMoveSpeed;
	m_iBackupMoveSpeed = m_iBackupMoveSpeed > 20 ? 20: m_iBackupMoveSpeed;
	SystemParametersInfo(SPI_SETMOUSESPEED,0,(PVOID) m_iBackupMoveSpeed,0);

}

VOID Mouse::Init( HWND hWnd )
{
	m_fScreenWidthRes = 1.0f;
	m_fScreenHeightRes = 1.0f;

	m_nQueSize		= MAX_QUESIZE;
	m_pMouseInfo	= new sMOUSEINFO[m_nQueSize];
	m_nHead = 0;
	m_nTail = 0;


	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( hWnd, &pt );
	m_nMouseOldX = m_nMouseX = pt.x;
	m_nMouseOldY = m_nMouseY = pt.y;
	m_nMouseEventX = 0;
	m_nMouseEventY = 0;

	m_nWheelMsg		= 0;
	m_nCombineKey	= 0;
	m_bInited		= TRUE;
}

VOID Mouse::Release()
{
	SAFE_DELETE_ARRAY( m_pMouseInfo );
}

VOID Mouse::MouseMessageProc( HWND hWnd, DWORD wmsg, WPARAM wParam, LPARAM lParam )
{
	switch(wmsg)
	{
	case WM_MOUSEMOVE:					RecordMouseEvent( hWnd, eME_NONE, wParam, lParam );	break;
	case WM_LBUTTONDOWN:				SetCapture( hWnd ); RecordMouseEvent( hWnd, eME_LDOWN, wParam, lParam );	break;
	case WM_RBUTTONDOWN:				SetCapture( hWnd ); RecordMouseEvent( hWnd, eME_RDOWN, wParam, lParam );	break;
	case WM_MBUTTONDOWN:				SetCapture( hWnd ); RecordMouseEvent( hWnd, eME_MDOWN, wParam, lParam );	break;
	case WM_LBUTTONUP:					ReleaseCapture(); RecordMouseEvent( hWnd, eME_LUP, wParam, lParam );	break;
	case WM_RBUTTONUP:					ReleaseCapture(); RecordMouseEvent( hWnd, eME_RUP, wParam, lParam );	break;
	case WM_MBUTTONUP:					ReleaseCapture(); RecordMouseEvent( hWnd, eME_MUP, wParam, lParam );	break;
	case WM_LBUTTONDBLCLK:				RecordMouseEvent( hWnd, eME_LDBL, wParam, lParam );	break;
	case WM_RBUTTONDBLCLK:				RecordMouseEvent( hWnd, eME_RDBL, wParam, lParam );	break;
	case WM_MBUTTONDBLCLK:				RecordMouseEvent( hWnd, eME_MDBL, wParam, lParam );	break;
	case 0x020A/*WM_MOUSEWHEEL*/:		m_nWheelMsg = int(wParam)>>16;	break;
	}	
}

VOID Mouse::RecordMouseEvent( HWND hWnd, int nEvent, WPARAM wParam, LPARAM lParam )
{
	if( !m_bInited ) return;

	sMOUSEINFO MouseInfo;

	MouseInfo.nEvent	= nEvent;
	MouseInfo.nX		= (int)( (float)LOWORD(lParam) * m_fScreenWidthRes );
	MouseInfo.nY		= (int)( (float)HIWORD(lParam) * m_fScreenHeightRes );
	MouseInfo.nKey		= (int)wParam;
	
	GetNextMouseEvent( hWnd,  &MouseInfo );
}

//----------------------------------------------------------------------------
/**
*/
VOID Mouse::SetPoint(POINT & pt)
{
	this->m_Point = pt;
}

//----------------------------------------------------------------------------
/**
*/
POINT Mouse::ConvertPoint(POINT pt)
{
	POINT ret;
	ret.x		= (int)( (float)pt.x * m_fScreenWidthRes );
	ret.y		= (int)( (float)pt.y * m_fScreenHeightRes );
	return ret;
}


//----------------------------------------------------------------------------
/**
*/
POINT			
Mouse::CheckMousePos()
{
	return  ConvertPoint(this->m_Point);
}

//----------------------------------------------------------------------------
/**
*/
VOID Mouse::GetNextMouseEvent( HWND hWnd, sMOUSEINFO * pMouseInfo )
{
	m_nMouseX	= pMouseInfo->nX;
	m_nMouseY	= pMouseInfo->nY;

	m_nMouseAxisX	= m_nMouseOldX - m_nMouseX;
	m_nMouseAxisY	= m_nMouseOldY - m_nMouseY;

	m_nMouseOldX	= m_nMouseX;
	m_nMouseOldY	= m_nMouseY;

	m_nWheel		= m_nWheelMsg;
	m_nWheelMsg		= 0;

	for( int i = 0 ; i < eME_COUNT ; ++i )
		m_bMouseEvent[i] = FALSE;

	
	ParseCurMouseEvent( pMouseInfo );


	if( m_nMouseAxisX || m_nMouseAxisY )
	{
		if( LButtonPressed() )	m_bLBtnDrag = TRUE;
		if( RButtonPressed() )	m_bRBtnDrag = TRUE;
		if( MButtonPressed() )	m_bMBtnDrag = TRUE;
	}
}

VOID Mouse::ParseCurMouseEvent( sMOUSEINFO* pMouseInfo )
{
	m_bMouseEvent[pMouseInfo->nEvent] = TRUE;
	
	switch( pMouseInfo->nEvent )
	{
	case eME_LDOWN:		m_bLBtnPress	= TRUE;		break;
	case eME_RDOWN:		m_bRBtnPress	= TRUE;		break;
	case eME_MDOWN:		m_bMBtnPress	= TRUE;		break;
	case eME_LUP:		m_bLBtnPress	= FALSE;	m_bLBtnDrag = FALSE;	break;
	case eME_RUP:		m_bRBtnPress	= FALSE;	m_bRBtnDrag = FALSE;	break;
	case eME_MUP:		m_bMBtnPress	= FALSE;	m_bMBtnDrag = FALSE;	break;
	case eME_LDBL:		break;
	case eME_RDBL:		break;
	case eME_MDBL:		break;
	}

	m_nMouseEventX	= pMouseInfo->nX;
	m_nMouseEventY	= pMouseInfo->nY;
	m_nCombineKey	= pMouseInfo->nKey;
}

BOOL Mouse::PushMouseInfo( sMOUSEINFO* pIn )
{
	int nTail = ( m_nTail + 1 ) % m_nQueSize;

	if( nTail == m_nHead ) return FALSE;

	m_pMouseInfo[m_nTail] = *pIn;
	m_nTail = nTail;

	return TRUE;
}

BOOL Mouse::PopMouseInfo( sMOUSEINFO* pOut )
{
	if( m_nTail == m_nHead ) return FALSE;

	*pOut = m_pMouseInfo[m_nHead];

	m_nHead = ( m_nHead + 1 ) % m_nQueSize;

	return TRUE;
}



void Mouse::SetSystemMoveSpeed(int iLevel)
{
	switch(iLevel)
	{
    case 0:
        m_iMoveSpeed = 4;
        break;
	case 1:
		m_iMoveSpeed = 6;
		break;
	case 2:
		m_iMoveSpeed = 8;
		break;
	case 3:
		m_iMoveSpeed = 10;
		break;
	case 4:
		m_iMoveSpeed = 12;
		break;
	case 5:
		m_iMoveSpeed = 14;
		break;
	default:
		m_iMoveSpeed = m_iMoveSpeed;
		break;
	}
	m_iMoveSpeed = m_iMoveSpeed < 1  ? 1: m_iMoveSpeed;
	m_iMoveSpeed = m_iMoveSpeed > 20 ? 20: m_iMoveSpeed;
	SystemParametersInfo(SPI_SETMOUSESPEED,0,(PVOID) m_iMoveSpeed,0);
}


int Mouse::GetSystemMoveSpeed()
{
	int iGetSpeed =0;
	SystemParametersInfo(SPI_GETMOUSESPEED,0,(PVOID)&iGetSpeed,0);
	return iGetSpeed;
}

int	Mouse::GetUserDefaultMoveLevel(int value)
{
	if(value <= 4) return m_MoveScrollRange.iMin;
    else if(value <=6) return 1;
	else if(value <=8) return 2;
	else if(value <=10) return 3;
	else if(value <=12) return 4;
	else if(value <=14) return 5;
	else return m_MoveScrollRange.iMax;
}

int Mouse::GetMoveLevel()
{
	return m_iMoveLevel;
}

void Mouse::SetMoveLevel(int iLevel)
{
	m_iMoveLevel = iLevel;
	SetSystemMoveSpeed(iLevel);
}

void Mouse::SetMoveRange(int min, int max)
{
	m_MoveScrollRange.iMin = min;
	m_MoveScrollRange.iMax = max;
}

int Mouse::GetMoveRangeMax()
{
	return m_MoveScrollRange.iMax;
}

int Mouse::GetMoveRangeMin()
{
	return m_MoveScrollRange.iMin; 
}



