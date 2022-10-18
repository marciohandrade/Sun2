// drawadapterfindd3d.h

#ifndef _3DRENDERER_DRAWADAPTERFINDD3D_H_
#define _3DRENDERER_DRAWADAPTERFINDD3D_H_

#include <d3d9.h>
#include "drawunitrenderer.h"


enum ENUM_VPT		///< Vertex Processing Type
{
	VPT_SOFTWARE = 0,
	VPT_MIXED,
	VPT_HARDWARE,
	VPT_PURE_HARDWARE
};

typedef struct
{
	UINT m_uiAdapter;
	D3DDEVTYPE m_DevType;
	D3DCAPS9 m_Caps;
	CWzArray<ENUM_VPT> m_arVertexProcessingType;
	int m_iDisplayModeForFullScreen;
	D3DFORMAT m_DepthStencilFmt;	///< m_iDisplayModeForFullScreen �� �´� Depth, Stencil buffer
} St_DeviceInfo;

typedef struct
{
	/// @name ����̽� �ν�
	UINT m_uiAdapter;	///< ��� ��ȣ
	D3DADAPTER_IDENTIFIER9 m_D3dAdapterIdentifier;	///< ��� �νĿ� ������

	/// @name ����
	CWzArray<D3DDISPLAYMODE> m_arDisplayMode;
	CWzArray<St_DeviceInfo*> m_arDeviceInfo;
	int m_iCurrentDeviceInfo;
	D3DDISPLAYMODE m_dmDspModeForWindowMode;
} St_AdapterInfoD3D;



class CDrawAdapterFindD3d : public CDrawUnitRenderer  
{
public:
	CDrawAdapterFindD3d();
	virtual ~CDrawAdapterFindD3d();
	virtual void Clear();

public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();

protected:
	LPDIRECT3D9 m_lpD3d;

	/// @name --- 3D �׷��� ��ġ ����Ʈ ����
protected:
	CWzArray<St_AdapterInfoD3D*> m_arAdapterInfo;
	int m_iCurrentAdapterInfo;
	BOOL FindAdapters( DWORD dwVenderId = 0, DWORD dwDeviceId = 0);
	BOOL FindDevices( St_AdapterInfoD3D *pAdapterInfo, CWzArray<D3DFORMAT> *parAdapterFormat);

	/// @name --- ���ϴ� �ػ�/����/��� ����
public:
	virtual BOOL SelectMode( St_ScreenFormat ScreenFormat);
	virtual int GetAvailableScreenSettings( St_ScreenSetting *pScreenSettings, int iMaxCount);

public:
	virtual BOOL IsSupportedIndex32();
};


#endif // _3DRENDERER_DRAWADAPTERFINDD3D_H_
