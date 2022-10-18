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


	// ��ϵ� ��Ʈ�� ���� ����ü�� �Ѱ��ش�.	
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


	// �ؽ�ó�� �簢���� ������ ��´�.
	WZRND_API BOOL GetTextRect( LPCSTR lpszText, IN OUT RECT* pRect, DWORD dwStyle = TP_HLEFT, LPCSTR lpszWzIDFont = NULL );

	// CrawCtrl.cpp ���� �����ϰ� �����
	WZRND_API HDC GetFontDCByWzID( WzID wzID ) { return GetFontDCByIndex( m_htFontFind.Get( wzID ) ); };


protected:
	int GetFontIndex( WzID nID ) { return m_htFontFind.Get(nID); }



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ĺ��ʹ� Extended Font�� ��ɵ�

	// @name �ܺο��� �о���̴� Ȯ�� ��Ʈ ����
private:
	stdext::hash_map<WzID,St_FontMake*> m_stdHMExtendedFontInfo;
	char	m_szFontFileFolderPath[MAX_PATH];
	stdext::hash_map<std::string, int> m_stdHMMakeFont;				// ����ڰ� ��� ��Ų ��Ʈ ����Ʈ�� ������ �ִ´�.

public:
	WZRND_API void SetFontFolderPath( LPCSTR lpszPath);
	WZRND_API BOOL LoadFontResource( LPCSTR lpszFileName);		// ������ ��Ʈ�� �ƴ� ����ڰ� ������ ��Ʈ ������ ��� ��Ų��.
	WZRND_API BOOL AddExtendedFont( WzID wiFontType, int iHeight, BOOL bBold, LPCSTR lpszFontName, BYTE byCharSet = HANGUL_CHARSET, BYTE byQuality = PROOF_QUALITY, BYTE byPandF = FIXED_PITCH | FF_MODERN,LOGFONT* plfont=NULL);	// Ȯ�� ��Ʈ �߰�
	// ������ ��Ʈ�� ��ϵ� Font ������ �����.  �� �ܿ��� ���Լ� ȣ�� ���� ������ ��Ʈ�� ��� �ؾ� �Ѵ�.			
	WZRND_API void DeleteExtendedFont( int iIndex );				// Ȯ����Ʈ ����

	// 2005/04/19 ��õ �߰�.
	WZRND_API BOOL DeleteExtendedFont( WzID wiFont );				// Ȯ����Ʈ ����
	WZRND_API void DeleteAllExtendedFont();						// Ȯ����Ʈ ��� ����
	WZRND_API int GetNumberOfExtendedFont() { return (int)m_stdHMExtendedFontInfo.size(); }	// Ȯ����Ʈ ���� ���
	WZRND_API BOOL GetExtendedFontInfo( int iIndex, St_FontMake *pFontMakeInfo );			// Ȯ����Ʈ ���� ���


protected:
	BOOL SerializeExtendedFontInfo( CWzArchive *pArchive );

public:
	WZRND_API BOOL LoadExtendedFontInfo( LPCSTR lpszFileName, BYTE byCharSet = DEFAULT_CHARSET ); // Ȯ����Ʈ Load
	WZRND_API BOOL SaveExtendedFontInfo( LPCSTR lpszFileName );	// Ȯ����Ʈ Save
};

#endif // _3DRENDERER_MANAGERTEXT_H_
