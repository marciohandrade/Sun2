/**
 *	@file		Input.cpp
 *	@brief		Input class
 *	@author		재영
 */

#include "SunClientPrecompiledHeader.h"

#include "Input.h"
#include "InterfaceManager.h"
#include "KeyMapMgr.h"
#include "GlobalFunc.h"

#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dxerr9.lib" )
#pragma comment( lib, "dinput8.lib" )

const int kKeyEventMax = 10;

//----------------------------------------------------------------------------
/**
*/
Input::Input( void )
{
	m_pDI			= NULL; 		// DirectInput object
	m_pKeyboard 	= NULL; 		// DirectInput keyboard device
	m_pMouse	 	= NULL; 		// DirectInput mouse device
	
	for( int i = 0 ; i < MAX_KEY_BUFFER ; ++i )
	{
		m_Key[i] = 0;
		m_PreKey[i] = 0;
	}


	memset( &m_MouseState, 0, sizeof(DIMOUSESTATE2) );
	memset( &m_PreMouseState, 0, sizeof(DIMOUSESTATE2) );

	m_bUIMouseInput = FALSE;

	for( int i = 0 ; i < MAX_MOUSE_BUTTON ; ++i )
	{
		m_dwClickTimer_Start[i] = 0;
		m_iClickState[i] = EXT_MOUSE_BUTTON_NONE;
	}

	m_dwLockKeyboard=0;

	m_bMouseChange = FALSE;

}

//----------------------------------------------------------------------------
/**
*/
Input::~Input( void )
{
	Destroy();
}

//----------------------------------------------------------------------------
/**
*/
void 
Input::Reset()
{
	memset( &m_MouseState, 0, sizeof(DIMOUSESTATE2) );
	memset( &m_PreMouseState, 0, sizeof(DIMOUSESTATE2));
	m_dwLockKeyboard=0;

	
	m_bUIMouseInput = FALSE;
	for( int i = 0 ; i < MAX_KEY_BUFFER ; ++i )
	{
		m_Key[i] = 0;
		m_Event[i] =0;
		m_PreKey[i] = 0;
	}

}

//----------------------------------------------------------------------------
/**
*/
void 
Input::ResetKeyBuffer()
{
	memset(m_Key, 0, MAX_KEY_BUFFER);
}

//----------------------------------------------------------------------------
/**
*/
HRESULT 
Input::Create( HWND hWnd )
{
	HRESULT hr;

	// Create a IDirectInput8*
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
		return DXTRACE_ERR( _T("DirectInput8Create"), hr );

	// Create a IDirectInputDevice8* for the keyboard
	if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
		return DXTRACE_ERR( _T("CreateDevice keyboard"), hr );

	// Set the keyboard data format
	if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
		return DXTRACE_ERR( _T("SetDataFormat keyboard"), hr );

	// Set the cooperative level on the keyboard
	if( FAILED( hr = m_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) )
		return DXTRACE_ERR( _T("SetCooperativeLevel keyboard"), hr );


	// 버퍼생성 최대 10개
	DIPROPDWORD  dipdw; 
	dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj = 0; 
	dipdw.diph.dwHow = DIPH_DEVICE; 
	dipdw.dwData = kKeyEventMax; 
	hr = m_pKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	// Acquire the keyboard
	m_pKeyboard->Acquire();





	hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	if( FAILED( hr ) )
	{
		return DXTRACE_ERR( _T("CreateDevice Mouse"), hr );
	}

	// 데이터 포멧 설정
	hr = m_pMouse->SetDataFormat( &c_dfDIMouse2 );
	if( FAILED( hr ) )
	{
		return DXTRACE_ERR( _T("SetDataFormat Mouse"), hr );
	}

	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		return DXTRACE_ERR( _T("SetCooperativeLevel Mouse"), hr );
	}

	m_pMouse->Acquire();



	return S_OK;
}

//----------------------------------------------------------------------------
/**
*/
void Input::Destroy( void )
{
	// Cleanup DirectX input objects
	if( m_pKeyboard )
		m_pKeyboard->Unacquire();
	SAFE_RELEASE( m_pKeyboard );

	if( m_pMouse )
		m_pMouse->Unacquire();

	SAFE_RELEASE( m_pMouse );

	SAFE_RELEASE( m_pDI );
}

//----------------------------------------------------------------------------
/**
*/
int Input::Update( BOOL bRet, BOOL bPreData )
{
    if (!m_pKeyboard || !m_pMouse)
        return INPUT_SUCCESS_ALL;

	m_bUIMouseInput = bRet;


	HRESULT		hr;
	int			iCode = INPUT_SUCCESS_ALL;

	if( bPreData )
	{
		memcpy( m_PreKey, m_Key, MAX_KEY_BUFFER );
	}

	DIDEVICEOBJECTDATA rgdod[kKeyEventMax]; 
	DWORD dwItems = kKeyEventMax; 
	HRESULT hres = m_pKeyboard->GetDeviceData( 
		sizeof(DIDEVICEOBJECTDATA), 
		rgdod, 
		&dwItems, 
		0); 

	if (SUCCEEDED(hres)) 
	{ 
		ZeroMemory(m_Event,sizeof(m_Event));
		if (hres == DI_BUFFEROVERFLOW) 
		{ 			
			ZeroMemory(m_Key,MAX_KEY_BUFFER);
		} 
		else
		{
            DWORD key_code = 0;
			for (size_t a = 0; a < dwItems; a++)
			{
                key_code = rgdod[a].dwOfs;
                if (key_code >= MAX_KEY_BUFFER)
                {
                    continue;
                }

				if (rgdod[a].dwData)
				{
					m_Event[key_code] = KS_DOWN;
				}
				else
				{
					m_Event[key_code] = KS_UP;
				}

				m_Key[key_code] = (BYTE)rgdod[a].dwData;
			}

		}
	}
	else
	{
		m_pKeyboard->Acquire();
		ZeroMemory(m_Key,MAX_KEY_BUFFER);

	}



	if( bPreData )
	{
		memcpy( &m_PreMouseState, &m_MouseState, sizeof(DIMOUSESTATE2) );
	}

	hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseState );
	if( FAILED( hr ) )
	{
		m_pMouse->Acquire();
		iCode |= INPUT_ERROR_MOUSE;
		
		return iCode;
	}

	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	if( m_MouseX < 0 )
	{
		m_MouseX = 0;
	}
	if( m_MouseY < 0 )
	{
		m_MouseY = 0;
	}

    if (m_MouseState.lZ)
        m_MouseZ = m_MouseState.lZ < 0 ? MOUSE_WHEEL_DOWN : MOUSE_WHEEL_UP;
    else
        m_MouseZ = MOUSE_WHEEL_NONE;

	return iCode;
}

//----------------------------------------------------------------------------
/**
*/
int Input::UpdateEx( BOOL bRet, BYTE *pKey, DWORD dwBufferSize )
{
	m_bUIMouseInput = bRet;


	HRESULT		hr;
	int			iCode = INPUT_SUCCESS_ALL;

	// Get the input's device state, and put the state in dims
	ZeroMemory( pKey, dwBufferSize );
	hr = m_pKeyboard->GetDeviceState( dwBufferSize, pKey );
	if( FAILED(hr) ) 
	{
		m_pKeyboard->Acquire();
		iCode |= INPUT_ERROR_KEY;
	}

	memcpy( &m_PreMouseState, &m_MouseState, sizeof(DIMOUSESTATE2) );

	if( m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseState ) == (DIERR_INPUTLOST | DIERR_NOTACQUIRED) )
	{
		m_pMouse->Acquire();
		iCode |= INPUT_ERROR_MOUSE;
	}

	return iCode;
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsPressShiftKey()
{
	return GetState( DIK_LSHIFT, KS_KEY ) || GetState( DIK_RSHIFT, KS_KEY );
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsPressCtrlKey()
{
	return GetState( DIK_LCONTROL, KS_KEY ) || GetState( DIK_RCONTROL, KS_KEY );
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsPressAltKey()
{
	return GetState( DIK_LALT, KS_KEY ) || GetState( DIK_RALT, KS_KEY );
}

//----------------------------------------------------------------------------
/**
*/
BOOL
Input::IsPressAnyExtKey()
{

	BOOL	bPress = (	GetState( DIK_LSHIFT, KS_KEY ) || GetState( DIK_RSHIFT, KS_KEY ) ||
						GetState( DIK_LCONTROL, KS_KEY ) || GetState( DIK_RCONTROL, KS_KEY ) ||
						GetState( DIK_LALT, KS_KEY ) || GetState( DIK_RALT, KS_KEY )
					);
	return bPress;
}


//----------------------------------------------------------------------------
/**
*/
BOOL
Input::IsPressQuickExtKey()
{
    BOOL bPress = (	GetState( DIK_LSHIFT, KS_KEY ) || GetState( DIK_RSHIFT, KS_KEY ));
	return bPress;
}



//----------------------------------------------------------------------------
/**
*/
BOOL
Input::IsPressAnyKey()
{
	for (int i = 0; i < MAX_KEY_BUFFER; ++i)
	{
		if ( m_Key[i] )
			return TRUE;
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsPressAnyKeyExceptExt()
{
	for (int i = 0; i < MAX_KEY_BUFFER; ++i)
	{
		switch (i)
		{
		case DIK_LSHIFT:	case DIK_RSHIFT:
		case DIK_LCONTROL:	case DIK_RCONTROL:
		case DIK_LALT:		case DIK_RALT: 
			{
				continue;
			}
			break;

		default:
			{
				if ( m_Key[i]  )
					return TRUE;
			}
		}
	}

	return FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsDownAltKey()
{
	return GetState( DIK_LALT, KS_DOWN ) || GetState( DIK_RALT, KS_DOWN );
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL 
Input::ScanCurKeyInfo(KeyCodeVec & IN rVecCode)
{
	if ( g_Input.IsPressAnyKey() )
	{
		// 확장키를 받아놓는다.
		KEY_SET_ITR itr = KeyMapMgr::Instance()->GetBegin();
		
		while (itr != KeyMapMgr::Instance()->GetEnd()) 
		{
            WORD wKEY_CODE = (WORD)itr->first;

			switch (wKEY_CODE)
			{
			case DIK_LSHIFT:	case DIK_RSHIFT:
			case DIK_LCONTROL:	case DIK_RCONTROL:
			case DIK_LALT:		case DIK_RALT: 
				{
					if ( g_Input.GetState(wKEY_CODE, KS_KEY , TRUE) || g_Input.GetState(wKEY_CODE, IKS_KEY , TRUE) )
					{
						rVecCode.push_back(wKEY_CODE);
					}
				}
				break;
			default:{}	
			}

			itr++;
		}

		// 확장키를 제외한 키를 받아놓는다.
		itr = KeyMapMgr::Instance()->GetBegin();
		while (itr != KeyMapMgr::Instance()->GetEnd()) 
		{
			WORD wKEY_CODE = (WORD)itr->first;

			switch (wKEY_CODE)
			{
			case DIK_LSHIFT:	case DIK_RSHIFT:
			case DIK_LCONTROL:	case DIK_RCONTROL:
			case DIK_LALT:		case DIK_RALT: 
				{
				}
				break;
			default:
				{
					if ( g_Input.GetState(wKEY_CODE, KS_DOWN , TRUE) || g_Input.GetState(wKEY_CODE, IKS_DOWN , TRUE) )
					{
						rVecCode.push_back(wKEY_CODE);	
						return TRUE;
					}
				}	
			}

			itr++;	   
		}

	}
		
	return FALSE;
}


BOOL Input::GetState( int Key, int iKeyState , BOOL bForceInput)
{
    if (GlobalFunc::BeforeLoadingInputLock())
    {
        //nop
    }
    else if (g_InterfaceManager.IsLockDlg() && bForceInput == FALSE)
	{
		return FALSE;
	}

	if(m_bMouseChange)
	{
		if(Key == MOUSE_LBUTTON) Key = MOUSE_RBUTTON;

		else if(Key == MOUSE_RBUTTON) Key = MOUSE_LBUTTON; 
	}

	unsigned int chKey ;

	if( Key < MAX_KEY_BUFFER )
	{
		//	키보드
		chKey = KS_NONE;

		if (m_Key[Key] & 0x80)
		{
			chKey |= KS_KEY;
		}

		if (m_Event[Key] & KS_DOWN)
		{
			chKey |= KS_DOWN;

		}

		if (m_Event[Key] & KS_UP)
		{
			chKey |= KS_UP;
		}

		if( g_InterfaceManager.GetDialogManager()->GetActiveEditCount() )	
		{
#ifdef _NA_004859_20120518_FORCE_INPUT_SCREENSHOT_KEY
            if( Key != DIK_SYSRQ )
            {
			    if((DIK_LSHIFT != Key) && (DIK_RSHIFT != Key) && (DIK_LCONTROL != Key) && (DIK_RCONTROL != Key) && (DIK_LALT != Key) && (DIK_RALT != Key))
			    {
				    chKey >>= 4;
			    }
            }
#else
            if((DIK_LSHIFT != Key) && (DIK_RSHIFT != Key) && (DIK_LCONTROL != Key) && (DIK_RCONTROL != Key) && (DIK_LALT != Key) && (DIK_RALT != Key))
            {
                chKey >>= 4;
            }

#endif // _NA_004859_20120518_FORCE_INPUT_SCREENSHOT_KEY
		}

		return BOOL( chKey & iKeyState);
	}
	else
	{
		//	마우스
		chKey = KS_MOUSE_NONE;

		if( (Key - MAX_KEY_BUFFER) < 0 && (Key - MAX_KEY_BUFFER) > MAX_MOUSE_BUTTON )
		{
			chKey = IKS_MOUSE_NONE;
		}

		Key = Key - MAX_KEY_BUFFER;

        if (Key >= MAX_MOUSE_BUTTON)
        {
            int wheel_state = (Key - MAX_MOUSE_BUTTON) + 1;
            if ((wheel_state == m_MouseZ) && 
                (m_bUIMouseInput == FALSE))
            {
                return TRUE;
            }
            return FALSE;
        }
		else if( m_PreMouseState.rgbButtons[Key] == 0 && m_MouseState.rgbButtons[Key] )
		{
			//	처음 눌렀을 때
			chKey = KS_MOUSE_DOWN;

			m_dwClickTimer_Start[Key] = timeGetTime();
			m_iClickState[Key] = EXT_MOUSE_BUTTON_CLICK_SHORT;
		}
		else if( m_PreMouseState.rgbButtons[Key] && m_MouseState.rgbButtons[Key] )
		{
			//	계속 누르고 있을 때
			chKey = KS_MOUSE_KEY;

			DWORD	dwTimer = timeGetTime();

			//	마우스 버튼 클릭 시간 체크( 누른 상태로 얼마나 오래 있었나 )
			if( (dwTimer - m_dwClickTimer_Start[Key]) <= MOUSE_CLICK_TIME_SHORT )
			{
				m_dwClickTimer_Start[Key] = dwTimer;
				m_iClickState[Key] = EXT_MOUSE_BUTTON_CLICK_SHORT;
			}
			else if( (dwTimer - m_dwClickTimer_Start[Key]) > MOUSE_CLICK_TIME_LONG )
			{
				m_dwClickTimer_Start[Key] = dwTimer;
				m_iClickState[Key] = EXT_MOUSE_BUTTON_CLICK_LONG;
			}
			else
			{
				m_dwClickTimer_Start[Key] = dwTimer;
				m_iClickState[Key] = EXT_MOUSE_BUTTON_CLICK_MID;
			}
		}
		else if( m_PreMouseState.rgbButtons[Key] && m_MouseState.rgbButtons[Key] == 0 )
		{
			//	누르다가 처음 띄었을 때( 버튼 업 )
			chKey = KS_MOUSE_UP;

			m_dwClickTimer_Start[Key] = timeGetTime();
			m_iClickState[Key] = EXT_MOUSE_BUTTON_NONE;
		}

		if( m_bUIMouseInput )
		{
			chKey >>= 4;
		}

	}
	return BOOL( iKeyState == chKey);
}

void Input::InvalidateState( int Key)
{
	unsigned int chKey;

	if( Key < MAX_KEY_BUFFER )
	{
		//	키보드
		chKey = KS_NONE;

		m_Key[Key] = 0;
		m_Event[Key] = 0;
		
	}
	else
	{
		//	마우스 는 아직 없다
	}

	
}



int Input::GetMouseClickState( int iKey )
{
	iKey = iKey - MAX_KEY_BUFFER;
	if( iKey >= 0 && iKey < MAX_MOUSE_BUTTON )
	{
		return m_iClickState[iKey];
	}

	return EXT_MOUSE_BUTTON_NONE;
}



//-------------------------------------------------------------------------------------------
/**
*/
BOOL	
Input::ConvertCodeToStr(KeyCodeVec & IN rVecCode, util::_tstring & OUT rKeyStrInfo)
{
	util::_tstring strTemp;
 
	int iSize = rVecCode.size();
	for ( int i = 0; i < iSize; ++i)
	{
		strTemp += KeyMapMgr::Instance()->GetKeyString( rVecCode[i] );	
		if (iSize != 1 && i != iSize - 1)
		{
			strTemp += _T("+");
		}
	}

	rKeyStrInfo.clear();
	rKeyStrInfo = strTemp;
	    
	return TRUE;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL				
Input::ConvertStrToCode(util::_tstring  & KeyStrInfo, KeyCodeVec & OUT rVecCode)
{
	TCHAR * pszString =	(TCHAR *)(const TCHAR *)KeyStrInfo.c_str();
	
	std::vector< util::_tstring > tokenVec;

	basic_string<TCHAR>::size_type	indexBegin = 0, indexEnd = 0;
	basic_string<TCHAR>::size_type	notPos = -1;
	int	iDelta = 0;

	while(1) 
	{
		indexEnd = KeyStrInfo.find_first_of(_T("+"), indexBegin);

		if (indexEnd == notPos)
		{
			indexEnd = KeyStrInfo.size();
			iDelta = indexEnd - indexBegin;
			util::_tstring	subStr = KeyStrInfo.substr(indexBegin, iDelta);		
			tokenVec.push_back(subStr);

			break;
		}

		iDelta = indexEnd - indexBegin;
		util::_tstring	subStr = KeyStrInfo.substr(indexBegin, iDelta);		
		tokenVec.push_back(subStr);
		indexBegin = indexEnd+1;
	}
				
	int	iTokenSize= tokenVec.size();
	for ( int i = 0; i < iTokenSize; ++i )
	{
		KEY_SET_ITR itr = KeyMapMgr::Instance()->GetBegin();

		while (itr != KeyMapMgr::Instance()->GetEnd())
		{
			int iCompLen = _tcslen( itr->second.c_str() );
			pszString = (TCHAR *)(const TCHAR *)tokenVec[i].c_str();
			int iOrgLen = _tcslen( tokenVec[i].c_str() );

			if ( iOrgLen != iCompLen )
			{
				itr++;
				continue;
			}

			TCHAR * pszComp = (TCHAR *)(const TCHAR *)itr->second.c_str();
			if ( _Strincmp( pszString, iOrgLen, pszComp, iOrgLen) )
			{
				rVecCode.push_back((WORD)itr->first);
				break;
			}

			itr++;
		}
	}   

	tokenVec.clear();
	
	return TRUE;
}


void Input::SetMouseChange(BOOL val)
{
	m_bMouseChange = val;
};

BOOL Input::GetMouseChange()
{
	return m_bMouseChange;
};


//-------------------------------------------------------------------------------------------
/**
*/
BOOL			
Input::IsValidDevice()
{
	return BOOL( m_pKeyboard && m_pMouse );
}

