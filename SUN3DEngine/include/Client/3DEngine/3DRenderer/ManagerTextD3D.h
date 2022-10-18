// managertextd3d.h

#ifndef _3DRENDERER_MANAGERTEXTD3D_H_
#define _3DRENDERER_MANAGERTEXTD3D_H_


#include "managertext.h"
#include "managerd3d.h"


class CManagerTextD3D : public CManagerText, private CManagerD3D  
{
private:
	LPD3DXFONT m_pD3dxFont[2][MAX_NUM_FONT];

	BOOL m_blSpriteText;
	LPD3DXSPRITE m_lpTextSprite;

public:
	CManagerTextD3D();
	~CManagerTextD3D();	// 이 클래스는 상속되지 않음을 의미

private:
	BOOL Create( CDrawBase *pParentDraw, void *pParam );
	void Destroy();
	void Clear();

	void OnLostDevice();
	void OnResetDevice();

private:
	// CMamagerText override
	BOOL CreateFont( int iIndex, LOGFONTA *plf );
	void DestroyFont( int iIndex);

	void DrawTextA( const char* lpszText, RECT* prtRegion, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, ENUM_TEXT_SHAPE eTextShape = ETS_BASE, WzColor wcLine = WzColor_RGBA( 0, 0, 0, 255), BOOL bStdToReal = TRUE );
	void DrawTextW( const WCHAR* lpwszText, RECT* prtRegion, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, ENUM_TEXT_SHAPE eTextShape = ETS_BASE, WzColor wcLine = WzColor_RGBA( 0, 0, 0, 255), BOOL bStdToReal = TRUE );

	BOOL BeginSpriteText();
	BOOL EndSpriteText(WzVector wvPos, WzVector wvRot, WzVector wvScale);

	BOOL BeginSpriteTextWithDepth(float fDepth);
	BOOL EndSpriteTextWithDepth();

	HDC GetFontDCByIndex( int iIndex );

private:
	// 2005/05/31 성천 추가.
	BOOL ResetFont( int iIndex, LOGFONTA* plf );
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CD3DTextMesh 클래스는 ManagerText3D에 붙어 있었지만 사용하지 않아 일단 주석으로 처리해두었음.
//
//class CD3DTextMesh
//{
//private:
//	struct sFontMeshInfo
//	{
//		TCHAR tstrText[100];
//		TCHAR tstrFontName[20];
//		BYTE  bySize;
//		BOOL  bBold;
//		BOOL  bItalic;
//		float fDeviation;		//
//		float fExtrusion;
//		LPD3DXMESH pMeshText;
//	};
//	CWzHandleManager<sFontMeshInfo*> m_hmFontMesh;
//
//public:
//	CD3DTextMesh() {}
//	virtual ~CD3DTextMesh() {}
//
//protected:
//	void Create();
//	void Destroy();
//	void OnResetDevice();
//	void OnLostDevice();
//
//private:
//	virtual LPDIRECT3DDEVICE9 GetDevice() PURE;
//
//	BOOL CreateD3dTextMesh( sFontMeshInfo* pFontMeshInfo );
//
//public:
//	// D3dTextMesh 관련 함수들.
//	HANDLE CreateTextMesh( LPCTSTR pstrText, LPCTSTR pstrFont, DWORD dwSize, BOOL bBold = FALSE, BOOL bItalic = FALSE, float fDeviation = 0.001f, float fExtrusion = 0.1f);
//	BOOL ReleaseTextMesh( HANDLE hHandle );
//	BOOL ReleaseAllTextMesh();
//	BOOL DrawTextMesh( HANDLE hHandle, WzColor wcText, WzVector wvPos , WzVector wvRot , WzVector wvScale ); 
//};


#endif // _3DRENDERER_MANAGERTEXTD3D_H_
