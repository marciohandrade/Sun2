/*!
 *	\file		Input.h
 *	\brief		Input class
 *	\note		마우스 스테이트와 키보드 스테이트 분리
 *	\bug		
 *	\todo		- Input::Update(BOOL bRet, BOOL bPreData) 에서 BOOL bRet 는 불필요한 인자 (멤버변수를 그대로 넘겨받아 자기대입하고 있음)
				- Input::UpdateEx() 사용안됨
 *	\author		재영
 */

#ifndef		_INPUT_H
#define		_INPUT_H

#ifndef		DIRECTINPUT_VERSION
#define		DIRECTINPUT_VERSION			0x0800
#endif	//	DIRECTINPUT_VERSION


#include <dinput.h>
#include <dxerr9.h>

typedef std::map<DWORD, util::_tstring>	KEY_SET_MAP;
typedef KEY_SET_MAP::iterator			KEY_SET_ITR;
typedef std::pair<DWORD, util::_tstring>		KEY_SET_PAIR;

typedef std::vector<WORD>	KeyCodeVec;



#define		MAX_KEY_BUFFER				256


//	키보드 버튼의 상태
enum
{
	IKS_NONE = 0x01,
	IKS_DOWN = 0x02,
	IKS_KEY  = 0x04,
	IKS_UP   = 0x08,

	KS_NONE  = 0x10,
	KS_DOWN  = 0x20,
	KS_KEY	 = 0x40,
	KS_UP	 = 0x80,
};

//	마우스 버튼의 상태
enum
{
	IKS_MOUSE_NONE = 0x0100,
	IKS_MOUSE_DOWN = 0x0200,
	IKS_MOUSE_KEY  = 0x0400,	
	IKS_MOUSE_UP   = 0x0800,

	KS_MOUSE_NONE  = 0x1000,
	KS_MOUSE_DOWN  = 0x2000,
	KS_MOUSE_KEY   = 0x4000,	
	KS_MOUSE_UP	   = 0x8000,
};

enum
{
	ID_MOUSE_LBUTTON	= 0,
	ID_MOUSE_RBUTTON	= 1,
	ID_MOUSE_MBUTTON	= 2,
    ID_MOUSE_BUTTON4    = 3,
    ID_MOUSE_BUTTON5    = 4,
    ID_MOUSE_BUTTON6    = 5,
    ID_MOUSE_BUTTON7    = 6,
    ID_MOUSE_BUTTON8    = 7,

	MAX_MOUSE_BUTTON,
};

//	마우스 클릭 시간 검사용
enum
{
	EXT_MOUSE_BUTTON_NONE			= 0,
	EXT_MOUSE_BUTTON_CLICK_SHORT	= 1,
	EXT_MOUSE_BUTTON_CLICK_MID		= 2,
	EXT_MOUSE_BUTTON_CLICK_LONG		= 3,
};

#define		MOUSE_CLICK_TIME_SHORT		250
#define		MOUSE_CLICK_TIME_MID		500
#define		MOUSE_CLICK_TIME_LONG		1000

#define     MOUSE_WHEEL_NONE    0
#define     MOUSE_WHEEL_UP      1
#define     MOUSE_WHEEL_DOWN    2

enum 
{
    MOUSE_LBUTTON = ID_MOUSE_LBUTTON + MAX_KEY_BUFFER,
    MOUSE_RBUTTON,
    MOUSE_MBUTTON,
    MOUSE_BUTTON4,
    MOUSE_BUTTON5,
    MOUSE_BUTTON6,
    MOUSE_BUTTON7,
    MOUSE_BUTTON8,

    MOUSE_WHEEL_BUTTON_UP = MOUSE_WHEEL_UP + MOUSE_BUTTON8,
    MOUSE_WHEEL_BUTTON_DOWN,
};

enum
{
	INPUT_SUCCESS_ALL = 0,
	INPUT_ERROR_KEY = 1,
	INPUT_ERROR_MOUSE = 2,
};

class Input
{
protected:
	LPDIRECTINPUT8			m_pDI;				// DirectInput object
	LPDIRECTINPUTDEVICE8	m_pKeyboard;		// DirectInput keyboard device
	LPDIRECTINPUTDEVICE8	m_pMouse;			// DirectInput mouse device

	BOOL					m_bMouseChange;

public:
	BYTE					m_Key[MAX_KEY_BUFFER];
	BYTE					m_PreKey[MAX_KEY_BUFFER];
    WORD                    m_Event[MAX_KEY_BUFFER];

	DIMOUSESTATE2			m_MouseState;
	DIMOUSESTATE2			m_PreMouseState;

	BOOL					m_bUIMouseInput;

	int						m_MouseX;
	int						m_MouseY;
    int                     m_MouseZ;

	DWORD					m_dwClickTimer_Start[MAX_MOUSE_BUTTON];
	int						m_iClickState[MAX_MOUSE_BUTTON];


	DWORD					m_dwLockKeyboard;

protected:
	BOOL			IsValidDevice();

public:
	/*!
	Input 을 생성한다. GlobalData에서 전역변수로 선언 g_Input
	*/
	Input( void );
	~Input( void );

	/*!
	DirectInput 객체 및 장치를 생성한다.
	Application::Create() 에서 호출됨
	*/
	HRESULT			Create( HWND hWnd );
	void			Destroy( void );
	void			Reset();
	void			ResetKeyBuffer();

	/*!
	키보드와 마우스의 상태를 읽어와 버퍼에 저장한다.
	Application::HeartBeat() 에서 호출됨
	*/
	int				Update( BOOL bRet, BOOL bPreData = TRUE );
	int				UpdateEx( BOOL bRet, BYTE *pKey, DWORD dwBufferSize );

	BOOL			IsPressShiftKey();
	BOOL			IsPressCtrlKey();
	BOOL			IsPressAltKey();
	BOOL			IsPressAnyExtKey();
	BOOL			IsPressAnyKey();
	BOOL			IsPressAnyKeyExceptExt();

	BOOL			IsPressQuickExtKey();


	BOOL			IsDownAltKey();

	BOOL			ScanCurKeyInfo(KeyCodeVec & IN rVecCode);

	BOOL			GetInterfaceInputMouseFlag() { return m_bUIMouseInput; }

	BYTE*			GetKeyBuffer( void ) { return (BYTE*)m_Key; }
	BYTE*			GetPreKeyBuffer( void ) { return (BYTE*)m_PreKey; }
	LPDIMOUSESTATE2	GetMouseBuffer( void ) { return &m_MouseState; }

	BOOL			GetState( int Key, int iKeyState , BOOL bForceInput = FALSE);
	char			GetState( BYTE *pKey, BYTE *pPreKey, int Key );
	void 			InvalidateState( int Key );

	int				GetMouseX() { return m_MouseX;	}
	int				GetMouseY() { return m_MouseY;	}
    int             GetMouseWheel() { return m_MouseZ; }


	void			SetMouseChange(BOOL val);
	BOOL			GetMouseChange();

	int				GetMouseClickState( int iKey );

	BOOL            ConvertCodeToStr(KeyCodeVec & IN rVecCode, util::_tstring & OUT rKeyStrInfo);
	BOOL            ConvertStrToCode(util::_tstring & IN KeyStrInfo, KeyCodeVec & OUT rVecCode );
};

#endif	//	_INPUT_H