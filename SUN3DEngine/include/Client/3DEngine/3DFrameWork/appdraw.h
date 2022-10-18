// appdraw.h

#ifndef _3DFRAMEWORK_APPDRAW_H_
#define _3DFRAMEWORK_APPDRAW_H_

#include "appbase.h"
#include <client/3dengine/3drenderer/drawbase.h>

// APP ���� �Ķ����
struct St_AppCreateParam
{
    char m_lpszDllPath[MAX_PATH];           // 3DRenderer.dll �� �ִ� ���
    char m_lpszShaderPath[MAX_PATH];        // Shader ���ϵ�( *.vso, *.pso)�� �ִ� ���
    char m_lpszIrradiancePath[MAX_PATH];    // Irradiance ���
    DWORD m_dwDrawCreateOption;             // DrawCreate ���� �ɼǰ�
    void* m_pDiscAccess;                    // Renderer ���� ����� CDiscAccess ������
};

// �ʱ�ȭ ������
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
    // ������/�Ҹ���
    CAppDraw();
    virtual ~CAppDraw();

    // ������ ����
    virtual BOOL Create( HINSTANCE hInstance, 
                         LPCSTR lpszAppName,
                         WNDPROC WndProc, 
                         LPCSTR lpszIconName,
                         int nCmdShow,
                         const St_AppCreateParam* pacp );

    // ������ ����
    virtual void Destroy();

    // ���� ����
    virtual int Run();
    // ���� �ڵ�(�� ������ ȣ��)
    virtual int HeartBeat();

    // ������ ���ν���
    virtual LRESULT WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

private:
    // 3D ���� ���ϸ� ���
    int Get3DSetupFileName( char* buf, int len );

    // 3D ���� ���� �ε�
    void Load3DSetupInfo();
    // 3D ���� ���� ����
    void Save3DSetupInfo();

protected:
    St_ScreenInitialParameters m_ScreenParam;

    CDrawBase* m_pDraw;
    BOOL m_bActive;
};

#endif // _3DFRAMEWORK_APPDRAW_H_
