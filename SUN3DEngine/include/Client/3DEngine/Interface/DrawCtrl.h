#pragma once


#include "typedef.h"
#include <list>

///////////////////////////////////////////////////////////////////////////////////////////
//��¥. 2004 / 2/ 6 
//�̸� : CDrawCtrl
//���� :	-Draw ���� ó���� �ϴ� Ŭ�����̴�.
//			-�ܺο��� Ŭ������ �����ͷ� �����´�. ������ ���� Draw Ŭ������ ������ �ϰų� 
//			-�ٸ� Ŭ������ �Ѿ�� ��쿡 �� Ŭ������ ���� �ϸ� �Ǵ� ���� �������� ����.
////////////////////////////////////////////////////////////////////////////////////////////
typedef std::list<VOID*> STD_LIST_PTR;

class CDrawCtrl  
{
public:
	CDrawCtrl();	
	virtual ~CDrawCtrl();

	void		OnRelease(void* pPtr);	

	//. Draw ���� �Լ���.
	void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect, const SI_FRECT& sSrcRect);
    void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect, const SI_FRECT& sSrcRect, float depth);
	void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect);

	void		DrawRectangle(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255), int nThickness = 1);	//. Rect(long)���� ���� �׷��ش�.
	void		DrawFillRectangle(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255), const SI_Gradation* pGradation = NULL);

	void		DrawRectangle(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255), int nThickness = 1); //. SI_FRECT(float)���� ���� �׷��ش�.
	void		DrawFillRectangle(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255), const SI_Gradation* pGradation = NULL);

	void		DrawShadowConvex(const SI_FRECT& rtRect,int nThickness = 2);
	void		DrawShadowConcave(const SI_FRECT& rtRect,int nThickness = 2);

	void		Draw3dShapeConvex(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255));
	void		Draw3dShapeConcave(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255));

	void		Draw3dShapeConvex(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255));
	void		Draw3dShapeConcave(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255));

	void		DrawLine(const SI_FRECT& rtRect,COLORREF color = RGBA(0,0,0,255));
	
	void		BegineTransparency(float fdegree);												//. ���� ��ġ�� �����ϸ� EndTransparency() �Ҷ� ���� alpha ����.
	void		EndTransparency();

	//. �ؽ�Ʈ ��� �Լ�.
	void		TextOutWZ(LPCSTR pszText, float fx, float fy, COLORREF coText = RGBA(0,0,0,255));
	void		DrawText(LPCSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255));	
    void		DrawText_Depth(LPCSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255), float depth = 0.0f);	
	void		DrawTextW(LPCWSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255));
	
	void*		LoadImage(LPCSTR pFName);														//. �׸� ������ �о�´�.

	//. ��Ʈ ��� ���� �Լ�.
	void		SetDefaultFontID(DWORD dwFontID);												//. Default ��Ʈ ����
	DWORD		GetDefaultFontID();																

	bool		BeginUseFont(DWORD dwFontID);													//. ��Ʈ ���� ���� �Լ�.
	void		EndUseFont();																	//. ��Ʈ ���� �� �Լ�

	//. DrawClass �ʱ�ȭ.
	void		SetDrawClass(void* pClass);
    void*		GetDrawClass() { return m_pDraw; };

	//. ���� ��ġ�� ���´�.
	float		GetRealX(float x);																//. Real->Std ��ǥ�� �����´�.	
	float		GetRealY(float y);																//. Real->Std ��ǥ�� �����´�.

	//. ȭ�� ��ǥ�� ���´�.
	float		GetStdX(float x);																//. Std->Real ��ǥ�� �����´�.	
	float		GetStdY(float x);																//. Std->Real ��ǥ�� �����´�.	
	//string -> WZID�� �����ؼ� �Ѱ��ش�.
	DWORD		ChangeStrToDWORD(LPCSTR pszID);													//. string -> WZID�� �����ؼ� �Ѱ��ش�.
	
	void		SetExePath( LPCSTR pPath );														//. �����θ� ������ �ִ´�.
	const char*	GetExePath(){return m_szPath;}

	//. �� �ؽ��� ���� �Լ�.
	VOID*		GenerateEmptyTexture( int iWidth, int iHeight, int iNumComponents);

	//. �ؽ����� ������ �����Ҷ� ����Լ�.
	VOID*		LockTexture(HANDLE hTexture, long *plPitch, int *piNumComponents);
	void		UnlockTexture( HANDLE hTexture);
	
	//. ��� Į�� ���� �Լ�.
	void		SetBackGroundColor(COLORREF color = RGBA(255,255,255,255));
	void		GetTextExtentPoint(LPCSTR lpszText, int iLength, SIZE* psiText);
	void		GetTextExtentPointW(LPCWSTR pwszText, int iLength, SIZE* psiText);

	//. ��Ʈ ���� �Լ���.

	HDC			GetFontDC(DWORD dwFontID);														//. Font DC �ڵ��� ���´�.
	BOOL		GetFontLogFont(DWORD dwFontID, LOGFONTA* pLogFont);								//. �Ƶ� �ش��ϴ� LogFont ���� ���

	//. Default ��Ʈ
	int			GetNumberOfDefaultFont();														//. ����Ʈ ��Ʈ ���� ���
	BOOL		GetDefaultFontInfo(int iIndex,DWORD* pdwFontID, LOGFONTA* pLogFont);			//. ����Ʈ��Ʈ ���� ���

	//. Ȯ�� ��Ʈ.
	BOOL		AddExtendedFont(DWORD dwFontID, LOGFONTA* pLogFont);							//. Ȯ����Ʈ �߰�
	void		DeleteExtendedFont(DWORD dwFontID);												//. Ȯ����Ʈ ����
	void		DeleteAllExtendedFont(void);													//. Ȯ����Ʈ ��� ����
	int			GetNumberOfExtendedFont(void);													//. Ȯ����Ʈ ���� ���
	BOOL		GetExtendedFontInfo(int iIndex, DWORD* pdwFontID, LOGFONTA* pLogFont);			//. Ȯ����Ʈ ���� ���
	BOOL		LoadExtendedFontInfo(LPCSTR pszPath);
	BOOL		SaveExtendedFontInfo(LPCSTR pszPath);
	
	//. ���� ���� ����	
	HANDLE		FileOpen(LPCSTR pszFileName,	DWORD dwDesiredAccess,	DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition);
	
	DWORD		FileRead(HANDLE hHandle, LPVOID lpBuffer, DWORD dwNumberOfByteToRead);
	DWORD		FileWrite(HANDLE hHandle, LPVOID lpBuffer, DWORD dwNumberOfByteToWrite);
	DWORD		GetFileSize(HANDLE hHandle);
	void		FileClose(HANDLE hHandle);
	void		SetDiscAccess(VOID* pPtr);
	
    //------------------------------------------------------------------------------
    // Returns :int : �ؽ�Ʈ ������ ����
    // Parameter : LPCSTR message : ���ڿ�
    // Parameter : const WzID font_id : ��Ʈ
    //------------------------------------------------------------------------------
    int        GetTextWidth(LPCSTR message, const WzID font_id);
    //------------------------------------------------------------------------------
    // Returns :bool : �������� ����Ǿ�����
    // Parameter : OUT std::string* reduce_string : �������� ����� ���ڿ�
    // Parameter : LPCSTR message : �������� ������ ���ڿ�
    // Parameter : const RECT& area : �ؽ�Ʈ ����
    // Parameter : const WzID font_id : ��Ʈ
    //------------------------------------------------------------------------------
    bool        MakeReduceText(OUT std::string* reduce_string, LPCSTR message, const RECT& area, const WzID font_id);

private:
	VOID*		m_pDraw;																		//. Draw Engine �����͸� ������ �ִ�.
	VOID*		m_pDiscAccess;																	//. ���� Packer ����� ��� �� ����Ʈ�� ���Ͽ� �����Ѵ�.
	char		m_szPath[MAX_PATH];																//. ���ҽ� ���� ���� ���.	

	DWORD		m_dwCurUseFontID;
	DWORD		m_dwDefaultFontID;														
	
	bool		m_blHyper;																		//. ����ȭ ��� üũ.															//. ����ȭ�� ���� Text�� �ѹ��� ��Ƽ� ��´�.
	float		m_fTransparency;																//. ����.
public:
	char		m_szVersion[10];																//. ���� 
	
};

