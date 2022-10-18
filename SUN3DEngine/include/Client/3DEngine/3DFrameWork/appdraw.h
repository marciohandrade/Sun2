// appdraw.h

#ifndef _3DFRAMEWORK_APPDRAW_H_
#define _3DFRAMEWORK_APPDRAW_H_

#include "appbase.h"
#include <client/3dengine/3drenderer/drawbase.h>

// APP 생성 파라미터
struct St_AppCreateParam
{
    char m_lpszDllPath[MAX_PATH];           // 3DRenderer.dll 이 있는 경로
    char m_lpszShaderPath[MAX_PATH];        // Shader 파일들( *.vso, *.pso)이 있는 경로
    char m_lpszIrradiancePath[MAX_PATH];    // Irradiance 경로
    DWORD m_dwDrawCreateOption;             // DrawCreate 관련 옵션값
    void* m_pDiscAccess;                    // Renderer 에서 사용할 CDiscAccess 포인터
};

// 초기화 변수들
struct St_ScreenInitialParameters
{
    St_ScreenFormat m_ScreenFormat;
    St_RenderOption m_RenderOption;
};

//------------------------------------------------------------------------------
/**
    @class CAppDraw
*/
class CAppDraw : public CAppBase  
{
public:
    // 생성자/소멸자
    CAppDraw();
    virtual ~CAppDraw();

    // 윈도우 생성
    virtual BOOL Create( HINSTANCE hInstance, 
                         LPCSTR lpszAppName,
                         WNDPROC WndProc, 
                         LPCSTR lpszIconName,
                         int nCmdShow,
                         const St_AppCreateParam* pacp );

    // 윈도우 제거
    virtual void Destroy();

    // 메인 루프
    virtual int Run();
    // 심장 박동(매 프레임 호출)
    virtual int HeartBeat();

    // 윈도우 프로시저
    virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

private:
    // 3D 설정 파일명 얻기
    int Get3DSetupFileName( char* buf, int len );

    // 3D 설정 정보 로딩
    void Load3DSetupInfo();
    // 3D 설정 정보 저장
    void Save3DSetupInfo();

protected:
    St_ScreenInitialParameters m_ScreenParam;

    CDrawBase* m_pDraw;
    BOOL m_bActive;
};

#endif // _3DFRAMEWORK_APPDRAW_H_
