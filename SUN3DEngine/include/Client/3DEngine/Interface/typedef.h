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

// Control�� ������ ���� �Ѵ�.
#define CTRL_KIND_DIALOG        0x00000000  // �⺻ �ٴ� Control
#define CTRL_KIND_CLIENT        0x00000001  // �⺻ �ٴ� Control
#define CTRL_KIND_MAINBAR       0x00000002  // Window�� Title Bar
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

// Dialog ���� Plug�� ������ �ִ´�.
#define DLG_PTITLEBAR           0x00000001  // Title bar �ֳ� or ����.
#define DLG_PMINIBUTTON         0x00000002  // Mini Button ��ư.
#define DLG_PMAXBUTTON          0x00000004  // MAX  Button ��ư.
#define DLG_PEXITBUTTON         0x00000008  // EXit Button ��ư.
#define DLG_PMOVE               0x00000010  // Dialog Move �Ұų� ���ų�.

// ABGR ���·� ���
#define RGBA( r, g, b, a )      ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))

// ���� ���� �����´�.
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

// iwz�����̳� STring ���� ������ ���� ��ų�� ���� �ִ� ������ ADD Or Del->NEW ����.
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
    WZ_CUSTOM_DRAW,     // �ܺ� ���μ����� �ش� Dialog�� �׸��� ������ ������ �׸��� �������� ���õȴ�.
};

// ��Ȱ��state
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

    // Dialog �޼���.
    RT_MSG_MINIBTCLICK,
    RT_MSG_MAXBTCLICK,
    RT_MSG_EXITBTCLICK,
    
    // ���콺 �⺻ �޼���.
    RT_MSG_LBUTTONDOWN,
    RT_MSG_LBUTTONUP,
    RT_MSG_LBUTTONCLICK,        // ������ ���� ���콺 Ŭ�� �ߴ�.
    RT_MSG_LBUTTONDCLICK,
    RT_MSG_RBUTTONDOWN,
    RT_MSG_RBUTTONUP,
    RT_MSG_RBUTTONCLICK,        // ������ ������ ���콺 Ŭ�� �ߴ�.
    RT_MSG_RBUTTONDCLICK,
    RT_MSG_DLGMOVE,
    RT_MSG_MOUSEOVER,           // ���콺 ��ǥ�� ���� ���� ������.

    // Scroll �޼���.
    RT_MSG_SB_LINELEFT,         // ���� ȭ��ǥ�� ������.
    RT_MSG_SB_LINERIGHT,        // ������ ȭ��ǥ�� ������.
    RT_MSG_SB_LINETOP,          // ���� ȭ��ǥ�� ������.
    RT_MSG_SB_LINEBOTTOM,       // �Ʒ��� ȭ��ǥ�� ������.
    RT_MSG_SB_PAGELEFT,         // ���� ������ ������.
    RT_MSG_SB_PAGERIGHT,        // ������ ������ ������.
    RT_MSG_SB_PAGETOP,          // ���� ������ ������.
    RT_MSG_SB_PAGEBOTTOM,       // �Ʒ��� ������ ������.
    RT_MSG_SB_THUMBTRACK,       // ���� �巡�� �ϰ� �ִ�.
    RT_MSG_SB_THUMBPOSITION,    // ���� �巡������ ���콺�� ���Ҵ�.
    RT_MSG_SB_ENDSCROLL,        // ��ũ���� ���� �Ǿ���.

    // tab �޼���.
    RT_MSG_TCN_SELCHANGE,       // ���õ� �׸��� �ٲ���.
    RT_MSG_TCN_SELCHANGING,     // ���õ� �׸��� �ٲ�.

    // Edit �޼���.
    RT_MSG_EDIT_COLORCHANGE,    // Control Color Change �ƴ�.
    RT_MSG_EDIT_ENTERKEYDOWN,   // Enter Key �Է��� ���Դ�.
    RT_MSG_EDIT_TABKEYDOWN,     // Tab Key �Է��� ���Դ�.

    RT_MSG_EDIT_CHARDOWN,       // ���ڰ� �ԷµǾ������ IME���� ���� �Է��� ��������
                                // Enter Key , Tab Key �� ������ ������ Ű�� �Է�������� 

    RT_MSG_EDIT_KEYDOWN         // Ű �Է½� �߻�
};

// �׸��� ���¸� ��Ÿ����.
enum ENUM_DRAWSTATE                 
{
    E_BASE,                     // �⺻ ����
    E_ON,                       // ���콺 ON
    E_CLICK,                    // ���콺 Click 
    E_DISABLE,                  // ��Ȱ��ȭ ����
    E_CHECK,                    // üũ ��ư
    E_NUM,
};

// ��Ʈ ���� (���� ��� ����)
enum ENUM_UIFONT_KIND
{
    FT_BASE_DEF,
    FT_BASE_SMA,
    FT_BASE_LAR,
    FT_EXP_01,
    FT_EXP_02,
    FT_NUM,
};

// �۾� ��� 
enum ENUM_TEXT_KIND
{
    ETK_BASE,                   // �� �⺻ ���.
    ETK_SHADOW,                 // �׸��� �۾�.
    ETK_OUTLINE,                // ����(�׵θ�) �۾�.
    ETK_NUM,
};

// Message Struct
// �޼����� ���� ��� ����ü ���� ���� �� ������ �Ѱ��ش�.
// Control�� ����Ʈ�� ó�� ���� �� ��� �߰��� �ൿ �̺�Ʈ�� �߻� �ϸ� ��� �޼��� ������ ���� �ϰ�
// Lock�� �ɾ� �ξ�� �Ѵ�. ��� Return �� �ϴ� ���� �ٸ� Control���� ������ ���� �Ǿ�� �ѱ� ����.
struct SI_MESSAGE                   
{
    // �Ѿ�� �޼��� ����.
    HWND hWnd;
    long lnReceiveMsg;
    WPARAM wParam;
    LPARAM lParam;
    POINT point;

    // �Ѿ�� �޼����� ó�� ��� ����. 
    DWORD dwDlgID;                  // Dlg ID
    DWORD dwCtrlID;                 // Ctrl ID
    DWORD dwKind;                   // Ctrl�� ����
    DWORD dwValue;                  // Ư������ �Ѱ��ش�.  
    ENUM_DIALOG_LIST eCKinds;
    ENUM_RESULT_MSG eResultMsg;     // Ư�� �ൿ�� ������ ��� �޼����� �Ѱ��ش�.

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

// Rectangle ���� ����ü�̴�. Empty Rect, Fill Rect ���� �׷��ش�.
struct SI_RECTANGLE
{
    bool m_blFill;          // ���� ä��ų� ���ų�.
    RECT m_rtRect;          // �׸� ����.
    int m_nWidth;           // ���� ����.
    COLORREF m_coColor;     // ����.
    DWORD m_dwID;           // ID.
    bool m_blUse3dEffect;   // 3d ȿ�� ���.
    bool m_bl3dState;       // �����̳� �����̳�.

    SI_RECTANGLE()
    {
        memset( this, 0, sizeof( SI_RECTANGLE ) );

        m_coColor = RGBA( 0, 0, 0, 255 );
        m_nWidth = 1;
    }
};

// ToolTip ����
struct SI_ToolTip
{   
    bool m_blUse;           // ��� ����
    DWORD m_dwStrID;        // String ID
    DWORD m_dwFontID;       // Font ID

    // Text�� ����� ��� ������ ���ڻ�.
    COLORREF m_coText;      // Text Color  
    COLORREF m_coBack;      // ��� Color

    DWORD m_dwDlgID;        // ToolTip�� Dialog ID�� ���� �Ǿ� ���� ��� , Dialog ID

    // �ڽ��� LeftTop�� �������� ��ġ ������ ������ �ִ´�.
    int m_nPosX;
    int m_nPosY;

    //! ver 2.1 Text�� ����Ұ�� �׵θ� ����
    COLORREF m_coOutline;   // �׵θ� ����(�Ϲ� �����ϰ�쿡�� ���)
    int      m_nThickness;   // �׵θ��β�.(�Ϲ� �����ϰ�쿡�� ���)

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

// Float Rect ����ü.
typedef St_FRect    SI_FRECT;




// Control Info Struct
struct SCTRL_INFO
{   
    DWORD m_dwCtrlID;       // ��Ʈ�� �Ƶ�.
    DWORD m_dwStyle;        // ��Ÿ��.
    DWORD m_dwKind;         // ����.

    DWORD m_dwCaptionID;    // String ID
    SI_FRECT m_sSizeRect;   // Randering size

    SI_FRECT m_sSrcRect;    // Textrue Src ������ �������ִ´�.    

    // �ʱ�ȭ.
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