// appbase.h

#ifndef _3DFRAMEWORK_APPBASE_H_
#define _3DFRAMEWORK_APPBASE_H_

enum ENUM_HBR
{
    HBR_OK = 0,         // ����
    HBR_INACTIVE,       // ��Ȱ��ȭ
    HBR_EXIT = 0xFF00,  // ����
};

//------------------------------------------------------------------------------
/**
    @class CAppBase
*/
class CAppBase
{
public:
    // ������/�Ҹ���
    CAppBase();
    virtual ~CAppBase();

    // ������ ����
    virtual BOOL Create( HINSTANCE hInstance,
                         LPCSTR lpszAppName,
                         WNDPROC WndProc, 
                         LPCSTR lpszIconName,
                         int nCmdShow );

    // ������ ����
    virtual void Destroy();

    // ���� ����
    virtual int Run();
    // ���� �ڵ�(�� ������ ȣ��)
    virtual int HeartBeat() = 0;

    // ������ ���ν���
    virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

    // ������ �ڵ� ���
    HWND GetHWnd();
    // �ν��Ͻ� �ڵ� ���
    HINSTANCE GetHInstance();

protected:
    DWORD m_dwInitialWndStyle;
    RECT m_rtInitWnd;

    HINSTANCE m_hInstance;
    HWND m_hWnd;
};

//------------------------------------------------------------------------------
/**
*/
inline HWND CAppBase::GetHWnd()
{
    return m_hWnd; 
}

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE CAppBase::GetHInstance()
{
    return m_hInstance;
}

#endif // _3DFRAMEWORK_APPBASE_H_
