// drawsetupd3d.h

#ifndef _3DRENDERER_DRAWSETUPD3D_H_
#define _3DRENDERER_DRAWSETUPD3D_H_

#include "drawadapterfindd3d.h"


class CDrawSetupD3d : public CDrawAdapterFindD3d  
{
public:
	CDrawSetupD3d();
	virtual ~CDrawSetupD3d();
	virtual void Clear();

public:
	virtual BOOL Create( HWND hWnd, DWORD dwVenderId, DWORD dwDeviceId, DWORD dwOption = 0);
	virtual void Destroy();
	virtual void CreateManagers( char *lpszShaderPath);
	virtual void DestroyManagers();

	/// @name setup 관련
protected:
	LPDIRECT3DDEVICE9 m_lpD3dDevice;
	D3DPRESENT_PARAMETERS m_D3dpp;

	/// @name 현재 상태
	D3DCAPS9 m_D3dCaps;
	D3DSURFACE_DESC m_D3dSurfDesc;
	BOOL m_bDeviceLost;
	BOOL m_bSupportGamma;
	D3DGAMMARAMP m_D3dGammaRamp;

	int pixelVersion_;
	int vertexVersion_;

public:
	virtual BOOL Initialize3DEnvironment();
	virtual BOOL ToggleFullScreen( HWND hWndChange = 0);
	virtual BOOL ProcessWindowSizeChange();
	virtual BOOL ChangeScreenFormat(const St_ScreenFormat* pcScreenFormat, 
                                    HWND hWndChange = 0, 
                                    bool is_window_size_changed = false);
	virtual BOOL IsRenderReady() const;

	// @name 감마 정보 
	// 0~255 인덱스, 0~65535값으로 설정
	virtual BOOL GetGammaRamp( WORD* pwOutRed, WORD* pwOutGreen, WORD* pwOutBlue ) const;
	virtual BOOL SetGammaRamp( const WORD* pwColor);
	virtual BOOL SetGammaRamp( const WORD* pwRed, const WORD* pwGreen, const WORD* pwBlue);
	virtual BOOL SetGammaRamp( int iMinIndex, WORD wMinValue, int iMaxIndex, WORD wMaxValue);


    LPDIRECT3DDEVICE9& GetD3dDevice() { return m_lpD3dDevice; }
    const LPDIRECT3DDEVICE9& GetD3dDevice() const { return m_lpD3dDevice; }
    void SetD3dDevice(const LPDIRECT3DDEVICE9& value) { m_lpD3dDevice = value; }

	int GetPixelVersion() { return pixelVersion_;}
	int GetVertexVersion() { return vertexVersion_;}

protected:
	BOOL Reset3DEnvironment();
	void FillPresentParams();

	/// @name 설정 변경
protected:
	virtual void FillRenderCaps();

	/// @name Window 세팅 (API 관련)
protected:
	virtual void PrepareDeviceSetting( BOOL bFullScreen);
	virtual void FinishDeviceSetting( BOOL bFullScreen);
	BOOL FindDepthStencilFormat( BOOL bFullScreen, St_DeviceInfo *pDeviceInfo);

	//type - 0 vertex, 1 pixel
	/*
		lev 1 - 1.1
		lev 2 - 2.0
		lev 3 - 2.a
		lev 4 - 2.b
		lev 5 ~ 3.0 4.0 5.0
	*/
	int GetShaderVersion(int type);
};

#endif // _3DRENDERER_DRAWSETUPD3D_H_
