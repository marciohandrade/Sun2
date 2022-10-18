#pragma once

#ifndef _CONTROLSDLL_TYPEDEF_H_
#define _CONTROLSDLL_TYPEDEF_H_

#include <client/3dengine/common/wzcommontypedef.h>
#include <client/3dengine/3drenderer/typedef.h>

#include <string>
#include <list>

typedef std::string STD_STRING;
typedef std::list<DWORD> STD_LIST;

#ifdef WZCONTROLSDLL_EXPORTS
#define WZCTL_API   __declspec( dllexport )
#else
#define WZCTL_API   __declspec( dllimport )
#endif

#define _MAX_UI_STRING_LENGTH 2048

// Control의 종류를 정의 한다.
#define CTRL_KIND_DIALOG        0x00000000  // 기본 바닥 Control
#define CTRL_KIND_CLIENT        0x00000001  // 기본 바닥 Control
#define CTRL_KIND_MAINBAR       0x00000002  // Window의 Title Bar
#define CTRL_KIND_MINI          0x00000004  // Mini button
#define CTRL_KIND_MAX           0x00000008  // Max  button
#define CTRL_KIND_EXIT          0x00000010  // Exit button
#define CTRL_KIND_CHECKBT       0x00000020  // Check Button Control
#define CTRL_KIND_BUTTON        0x00000040  // Button Control
#define CTRL_KIND_TEXT          0x00000080  // Text Control
#define CTRL_KIND_EDIT          0x00000100  // Edit Control
#define CTRL_KIND_LIST          0x00000200  // List Control
#define CTRL_KIND_HSCROLL       0x00000400  // Hscroll Control
#define CTRL_KIND_VSCROLL       0x00000800  // VScroll Control
#define CTRL_KIND_TAB           0x00001000  // Tab Control
#define CTRL_KIND_PICTURE       0x00002000  // Picture Control
#define CTRL_KIND_RECTANGLE     0x00004000  // BOX Control
#define CTRL_KIND_TEXTIMAGE     0x00008000  // TextImage Control
#define CTRL_KIND_WEB           0x00010000  // Web Browser Control

// Dialog 정보 Plug를 가지고 있는다.
#define DLG_PTITLEBAR           0x00000001  // Title bar 있나 or 없나.
#define DLG_PMINIBUTTON         0x00000002  // Mini Button 버튼.
#define DLG_PMAXBUTTON          0x00000004  // MAX  Button 버튼.
#define DLG_PEXITBUTTON         0x00000008  // EXit Button 버튼.
#define DLG_PMOVE               0x00000010  // Dialog Move 할거냐 말거냐.

// ABGR 형태로 등록
#define RGBA( r, g, b, a )      ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))

// 알파 값을 가져온다.
#define GetAValue( rgba )       ((BYTE)((rgba)>>24))

// Control Style

// Edit style
#define CTRL_ES_AUTOHSCROLL     0x00000001
#define CTRL_ES_LEFT            0x00000002  
#define CTRL_ES_CENTER          0x00000004
#define CTRL_ES_RIGHT           0x00000010
#define CTRL_ES_MULTILINE       0x00000020
#define CTRL_ES_PASSWORD        0x00000040  

// Public style
#define CTRL_SHADOW             0x00000080  
#define CTRL_3DSHAPE            0x00000100      

// iwz파일이나 STring 파일 내용을 적용 시킬때 기존 있는 정보에 ADD Or Del->NEW 여부.
enum ENUM_LOAD_UITYPE
{
    ELUIT_NEW,
    ELUIT_ADD,
};

// State 
enum ENUM_STATEWZ
{   
    WZ_NONE,                        
    WZ_SHOW,
    WZ_HIDE,
    WZ_MINISH,
    WZ_CUSTOM_DRAW,     // 외부 프로세스가 해당 Dialog를 그리고 싶을대 내부의 그리기 순서에서 무시된다.
};

// 비활성state
enum 
{
    WZS_DISABLE = 0,
    WZS_ENABLE = 1,
};

// Dlalog ID List
enum ENUM_DIALOG_LIST
{
    WZD_DLG_CLIENT,
    WZD_DLG_MINIBT,
    WZD_DLG_MAXBT,
    WZD_DLG_EXITBT,
    WZD_DLG_TITLE,  
    WZD_DIALOG,
    WZD_BUTTOM,
    WZD_NUM,
};

// Retrun Massge Result
enum ENUM_RESULT_MSG
{
    RT_NONE,

    // Dialog 메세지.
    RT_MSG_MINIBTCLICK,
    RT_MSG_MAXBTCLICK,
    RT_MSG_EXITBTCLICK,
    
    // 마우스 기본 메세지.
    RT_MSG_LBUTTONDOWN,
    RT_MSG_LBUTTONUP,
    RT_MSG_LBUTTONCLICK,        // 영역을 왼쪽 마우스 클릭 했다.
    RT_MSG_LBUTTONDCLICK,
    RT_MSG_RBUTTONDOWN,
    RT_MSG_RBUTTONUP,
    RT_MSG_RBUTTONCLICK,        // 영역을 오른쪽 마우스 클릭 했다.
    RT_MSG_RBUTTONDCLICK,
    RT_MSG_DLGMOVE,
    RT_MSG_MOUSEOVER,           // 마우스 좌표가 영역 위에 있을때.

    // Scroll 메세지.
    RT_MSG_SB_LINELEFT,         // 왼쪽 화살표를 눌렀다.
    RT_MSG_SB_LINERIGHT,        // 오른쪽 화살표를 눌렀다.
    RT_MSG_SB_LINETOP,          // 윗쪽 화살표를 눌렀다.
    RT_MSG_SB_LINEBOTTOM,       // 아래쪽 화살표를 눌렀다.
    RT_MSG_SB_PAGELEFT,         // 왼쪽 몸통을 눌렀다.
    RT_MSG_SB_PAGERIGHT,        // 오른쪽 몸통을 눌렀다.
    RT_MSG_SB_PAGETOP,          // 윗쪽 몸통을 눌렀다.
    RT_MSG_SB_PAGEBOTTOM,       // 아래쪽 몸통을 눌렀다.
    RT_MSG_SB_THUMBTRACK,       // 썸을 드래그 하고 있다.
    RT_MSG_SB_THUMBPOSITION,    // 썸을 드래그한후 마우스를 놓았다.
    RT_MSG_SB_ENDSCROLL,        // 스크롤이 종료 되었다.

    // tab 메세지.
    RT_MSG_TCN_SELCHANGE,       // 선택된 항목이 바뀐후.
    RT_MSG_TCN_SELCHANGING,     // 선택된 항목이 바뀔때.

    // Edit 메세지.
    RT_MSG_EDIT_COLORCHANGE,    // Control Color Change 됐다.
    RT_MSG_EDIT_ENTERKEYDOWN,   // Enter Key 입력이 들어왔다.
    RT_MSG_EDIT_TABKEYDOWN,     // Tab Key 입력이 들어왔다.

    RT_MSG_EDIT_CHARDOWN,       // 문자가 입력되었을경우 IME에서 문자 입력이 끝났을때
                                // Enter Key , Tab Key 를 제외한 나머지 키를 입력했을경우 

    RT_MSG_EDIT_KEYDOWN         // 키 입력시 발생
};

// 그리기 상태를 나타낸다.
enum ENUM_DRAWSTATE                 
{
    E_BASE,                     // 기본 상태
    E_ON,                       // 마우스 ON
    E_CLICK,                    // 마우스 Click 
    E_DISABLE,                  // 비활성화 상태
    E_CHECK,                    // 체크 버튼
    E_NUM,
};

// 폰트 종류 (현재 사용 안함)
enum ENUM_UIFONT_KIND
{
    FT_BASE_DEF,
    FT_BASE_SMA,
    FT_BASE_LAR,
    FT_EXP_01,
    FT_EXP_02,
    FT_NUM,
};

// 글씨 모양 
enum ENUM_TEXT_KIND
{
    ETK_BASE,                   // 걍 기본 모양.
    ETK_SHADOW,                 // 그림자 글씨.
    ETK_OUTLINE,                // 윤곽(테두리) 글씨.
    ETK_NUM,
};

// Message Struct
// 메세지에 대한 결과 구조체 정보 설정 한 다음에 넘겨준다.
// Control의 리스트를 처음 부터 돌 경우 중간에 행동 이벤트가 발생 하면 결과 메세지 정보를 설정 하고
// Lock을 걸어 두어야 한다. 즉시 Return 안 하는 경우는 다른 Control들의 정보를 갱신 되어야 한기 때문.
struct SI_MESSAGE                   
{
    // 넘어온 메세지 정보.
    HWND hWnd;
    long lnReceiveMsg;
    WPARAM wParam;
    LPARAM lParam;
    POINT point;

    // 넘어온 메세지의 처리 결과 정보. 
    DWORD dwDlgID;                  // Dlg ID
    DWORD dwCtrlID;                 // Ctrl ID
    DWORD dwKind;                   // Ctrl의 종류
    DWORD dwValue;                  // 특정값을 넘겨준다.  
    ENUM_DIALOG_LIST eCKinds;
    ENUM_RESULT_MSG eResultMsg;     // 특정 행동을 했을때 결과 메세지를 넘겨준다.

    SI_MESSAGE()
    {
        memset( this, 0, sizeof( SI_MESSAGE ) );
    }

    void Set( HWND hW, UINT iM, WPARAM wP, LPARAM lP )
    {
        memset( this, 0, sizeof( SI_MESSAGE ) );

        hWnd = hW;
        lnReceiveMsg = iM;
        wParam = wP;
        lParam = lP;
        m_blLock = false;
    }

    void LockMsg()
    {
        m_blLock = true;
    }

    void UnLockMsg()
    {
        m_blLock = false;
    }

    bool IsLock()
    {
        return m_blLock;
    }

private:
    bool m_blLock;
};

// Rectangle 정보 구조체이다. Empty Rect, Fill Rect 등을 그려준다.
struct SI_RECTANGLE
{
    bool m_blFill;          // 안을 채울거냐 말거냐.
    RECT m_rtRect;          // 그릴 영역.
    int m_nWidth;           // 선의 굵기.
    COLORREF m_coColor;     // 색상.
    DWORD m_dwID;           // ID.
    bool m_blUse3dEffect;   // 3d 효과 상용.
    bool m_bl3dState;       // 오목이냐 볼록이냐.

    SI_RECTANGLE()
    {
        memset( this, 0, sizeof( SI_RECTANGLE ) );

        m_coColor = RGBA( 0, 0, 0, 255 );
        m_nWidth = 1;
    }
};

// ToolTip 정보
struct SI_ToolTip
{   
    bool m_blUse;           // 사용 여부
    DWORD m_dwStrID;        // String ID
    DWORD m_dwFontID;       // Font ID

    // Text로 사용할 경우 배경색과 문자색.
    COLORREF m_coText;      // Text Color  
    COLORREF m_coBack;      // 배경 Color

    DWORD m_dwDlgID;        // ToolTip이 Dialog ID로 설정 되어 있을 경우 , Dialog ID

    // 박스의 LeftTop을 기준으로 위치 정보를 가지고 있는다.
    int m_nPosX;
    int m_nPosY;

    //! ver 2.1 Text로 사용할경우 테두리 색깔
    COLORREF m_coOutline;   // 테두리 색깔(일반 툴팁일경우에만 사용)
    int      m_nThickness;   // 테두리두께.(일반 툴팁일경우에만 사용)

    void initialize()
    {
        memset( this, 0, sizeof( SI_ToolTip ) );

        m_coBack = RGBA( 255, 255, 255, 255 );
        m_coText = RGBA( 0, 0, 0, 255 );
        m_coOutline = RGBA( 0, 0, 0, 255 );
        m_dwFontID = 0;
        m_nThickness = 1;
    }
};

// Float Rect 구조체.
typedef St_FRect    SI_FRECT;




// Control Info Struct
struct SCTRL_INFO
{   
    DWORD m_dwCtrlID;       // 컨트롤 아디.
    DWORD m_dwStyle;        // 스타일.
    DWORD m_dwKind;         // 종류.

    DWORD m_dwCaptionID;    // String ID
    SI_FRECT m_sSizeRect;   // Randering size

    SI_FRECT m_sSrcRect;    // Textrue Src 영역을 가지고있는다.    

    // 초기화.
    void Initialize()
    {   
        m_dwCtrlID = 0;
        m_dwCaptionID = 0;
        m_dwStyle = 0;
        m_dwKind = 0;     

        memset( &m_sSizeRect, 0, sizeof( SI_FRECT ) );
        memset( &m_sSrcRect, 0, sizeof( SI_FRECT ) );
    }
};

#include "ImageData.h"

#endif // _CONTROLSDLL_TYPEDEF_H_