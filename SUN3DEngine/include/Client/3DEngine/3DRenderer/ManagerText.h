#pragma once

#ifndef _3DRENDERER_MANAGERTEXT_H_
#define _3DRENDERER_MANAGERTEXT_H_

#include "managerbase.h"
#include <programcommon/wzhashtable.h>
#include <hash_map>
#include <string>

class CWzArchive;

enum ENUM_DRAWTEXTSTYLE
{
	TP_HLEFT	= 0x0001,
	TP_HRIGHT	= 0x0002,
	TP_HCENTER	= 0x0004,
	TP_VTOP		= 0x0010,
	TP_VCENTER	= 0x0020,
	TP_VBOTTOM	= 0x0040,
	TP_WORDBREAK = 0x0080,
    TP_UNDERLINE = 0x0100,
};

enum ENUM_FONT_KIND
{
	EFK_DEFAULT,
	EFK_EXTEND,
	EFL_MAKEFONT,
};

enum ENUM_TEXT_SHAPE
{
	ETS_BASE,
	ETS_SHADOW,
	ETS_OUTLINE,
};
typedef struct
{
	WzID m_wiFont;
	int m_iHeight;
	BOOL m_bBold;	
	char *m_lpszFontName;	
	BYTE lfCharSet;
	BYTE lfQuality;
	BYTE lfPitchAndFamily;

} St_FontMake;


#define MAX_NUM_FONT	(32)

extern St_FontMake g_FontMake[MAX_NUM_FONT];

#ifdef UNICODE
#define DrawText DrawTextW
#else
#define DrawText DrawTextA
#endif

#ifdef UNICODE
#define GetTextExtentPoint GetTextExtentPointW
#else
#define GetTextExtentPoint GetTextExtentPointA
#endif


class CManagerText : public CManagerBase
{
private:
	CWzHashTable<int, WzID> m_htFontFind;
	int m_iCurrentFont;
	int m_iNumDefaultFont;


public:
	CManagerText();
	virtual ~CManagerText();

	virtual BOOL Create( CDrawBase* pParentDraw, void* pParam );

	virtual void Clear();
	virtual void Destroy();
	virtual void OnLostDevice();
	virtual void OnResetDevice();


protected:
	void FillLogFont( St_FontMake *pfm, LOGFONT *plf );

	virtual BOOL CreateFont( int iIndex, LOGFONT *plf ) PURE;
	virtual void DestroyFont( int iIndex ) PURE;


public:
	WZRND_API int GetCurrentFontNumber() { return m_iCurrentFont; }
	WZRND_API int GetNumberOfDefaultFont() { return m_iNumDefaultFont; }

	WZRND_API BOOL StartToUseFont( WzID wiFont);
	WZRND_API BOOL StartToUseFont( LPCSTR lpszWzIDFont);
	WZRND_API void EndUseFont();


	// 등록된 폰트의 정보 구조체를 넘겨준다.	
	WZRND_API BOOL GetFontInfo( int iFont, OUT St_FontMake *pFontMake);
	WZRND_API BOOL GetFontInfoWzID( WzID wiFont, OUT St_FontMake *pFontMakeInfo );


	WZRND_API void DrawTextA( LPCSTR lpszText, int x, int y, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, BOOL bStdToReal = TRUE );
	WZRND_API void DrawTextW( LPCWSTR lpwszText, int x, int y, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, BOOL bStdToReal = TRUE );

	//
	WZRND_API virtual HDC GetFontDCByIndex( int nIndex ) PURE;

	WZRND_API virtual void DrawTextA( LPCSTR lpszText, RECT* prtRegion, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, ENUM_TEXT_SHAPE eTextShape = ETS_BASE, WzColor wcLine = WzColor_RGBA( 0, 0, 0, 255), BOOL bStdToReal = TRUE ) PURE;
	WZRND_API virtual void DrawTextW( LPCWSTR lpwszText, RECT* prtRegion, WzColor wcText, WzColor wcBack = WzColor_RGBA( 0, 0, 0, 0), DWORD dwStyle = TP_HLEFT, ENUM_TEXT_SHAPE eTextShape = ETS_BASE, WzColor wcLine = WzColor_RGBA( 0, 0, 0, 255), BOOL bStdToReal = TRUE) PURE;

	WZRND_API virtual BOOL BeginSpriteText() { return TRUE; }
	WZRND_API virtual BOOL EndSpriteText(WzVector wvPos, WzVector wvRot, WzVector wvScale) { return TRUE; }

	WZRND_API virtual BOOL BeginSpriteTextWithDepth( float fDepth ) { return TRUE; }
	WZRND_API virtual BOOL EndSpriteTextWithDepth() { return TRUE; }


	WZRND_API void GetTextExtentPointA( LPCSTR lpszText, int iLength, SIZE* psiText );
	WZRND_API void GetTextExtentPointW( LPCWSTR lpwszText, int iLength, SIZE* psiText );


	// 텍스처에 사각영역 정보를 얻는다.
	WZRND_API BOOL GetTextRect( LPCSTR lpszText, IN OUT RECT* pRect, DWORD dwStyle = TP_HLEFT, LPCSTR lpszWzIDFont = NULL );

	// CrawCtrl.cpp 에서 유일하게 사용중
	WZRND_API HDC GetFontDCByWzID( WzID wzID ) { return GetFontDCByIndex( m_htFontFind.Get( wzID ) ); };


protected:
	int GetFontIndex( WzID nID ) { return m_htFontFind.Get(nID); }



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 이후부터는 Extended Font의 기능들

	// @name 외부에서 읽어들이는 확장 폰트 정보
private:
	stdext::hash_map<WzID,St_FontMake*> m_stdHMExtendedFontInfo;
	char	m_szFontFileFolderPath[MAX_PATH];
	stdext::hash_map<std::string, int> m_stdHMMakeFont;				// 사용자가 등록 시킨 폰트 리스트를 가지고 있는다.

public:
	WZRND_API void SetFontFolderPath( LPCSTR lpszPath);
	WZRND_API BOOL LoadFontResource( LPCSTR lpszFileName);		// 원도우 폰트가 아닌 사용자가 지정한 폰트 파일을 등록 시킨다.
	WZRND_API BOOL AddExtendedFont( WzID wiFontType, int iHeight, BOOL bBold, LPCSTR lpszFontName, BYTE byCharSet = HANGUL_CHARSET, BYTE byQuality = PROOF_QUALITY, BYTE byPandF = FIXED_PITCH | FF_MODERN,LOGFONT* plfont=NULL);	// 확장 폰트 추가
	// 원도위 폰트에 등록된 Font 정보만 만든다.  그 외에는 이함수 호출 전에 원도우 폰트에 등록 해야 한다.			
	WZRND_API void DeleteExtendedFont( int iIndex );				// 확장폰트 삭제

	// 2005/04/19 성천 추가.
	WZRND_API BOOL DeleteExtendedFont( WzID wiFont );				// 확장폰트 삭제
	WZRND_API void DeleteAllExtendedFont();						// 확장폰트 모두 삭제
	WZRND_API int GetNumberOfExtendedFont() { return (int)m_stdHMExtendedFontInfo.size(); }	// 확장폰트 개수 얻기
	WZRND_API BOOL GetExtendedFontInfo( int iIndex, St_FontMake *pFontMakeInfo );			// 확장폰트 정보 얻기


protected:
	BOOL SerializeExtendedFontInfo( CWzArchive *pArchive );

public:
	WZRND_API BOOL LoadExtendedFontInfo( LPCSTR lpszFileName, BYTE byCharSet = DEFAULT_CHARSET ); // 확장폰트 Load
	WZRND_API BOOL SaveExtendedFontInfo( LPCSTR lpszFileName );	// 확장폰트 Save
};

#endif // _3DRENDERER_MANAGERTEXT_H_
