// appbase.h

#ifndef _3DFRAMEWORK_APPBASE_H_
#define _3DFRAMEWORK_APPBASE_H_

enum ENUM_HBR
{
    HBR_OK = 0,         // 정상
    HBR_INACTIVE,       // 비활성화
    HBR_EXIT = 0xFF00,  // 종료
};

//------------------------------------------------------------------------------
/**
    @class CAppBase
*/
class CAppBase
{
public:
    // 생성자/소멸자
    CAppBase();
    virtual ~CAppBase();

    // 윈도우 생성
    virtual BOOL Create( HINSTANCE hInstance,
                         LPCSTR lpszAppName,
                         WNDPROC WndProc, 
                         LPCSTR lpszIconName,
                         int nCmdShow );

    // 윈도우 제거
    virtual void Destroy();

    // 메인 루프
    virtual int Run();
    // 심장 박동(매 프레임 호출)
    virtual int HeartBeat() = 0;

    // 윈도우 프로시저
    virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

    // 윈도우 핸들 얻기
    HWND GetHWnd();
    // 인스턴스 핸들 얻기
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
