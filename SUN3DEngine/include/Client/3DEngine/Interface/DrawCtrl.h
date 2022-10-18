#pragma once


#include "typedef.h"
#include <list>

///////////////////////////////////////////////////////////////////////////////////////////
//날짜. 2004 / 2/ 6 
//이름 : CDrawCtrl
//설명 :	-Draw 관련 처리를 하는 클래스이다.
//			-외부에서 클래스를 포인터로 가져온다. 하지만 만약 Draw 클래스가 수정을 하거나 
//			-다른 클래스가 넘어올 경우에 이 클래스만 수정 하면 되는 설계 목적으로 만듬.
////////////////////////////////////////////////////////////////////////////////////////////
typedef std::list<VOID*> STD_LIST_PTR;

class CDrawCtrl  
{
public:
	CDrawCtrl();	
	virtual ~CDrawCtrl();

	void		OnRelease(void* pPtr);	

	//. Draw 관련 함수들.
	void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect, const SI_FRECT& sSrcRect);
    void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect, const SI_FRECT& sSrcRect, float depth);
	void		DrawTexture(VOID* pVoid, const SI_FRECT& sDesRect);

	void		DrawRectangle(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255), int nThickness = 1);	//. Rect(long)으로 영역 그려준다.
	void		DrawFillRectangle(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255), const SI_Gradation* pGradation = NULL);

	void		DrawRectangle(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255), int nThickness = 1); //. SI_FRECT(float)으로 영역 그려준다.
	void		DrawFillRectangle(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255), const SI_Gradation* pGradation = NULL);

	void		DrawShadowConvex(const SI_FRECT& rtRect,int nThickness = 2);
	void		DrawShadowConcave(const SI_FRECT& rtRect,int nThickness = 2);

	void		Draw3dShapeConvex(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255));
	void		Draw3dShapeConcave(const SI_FRECT& rtRect, COLORREF color = RGBA(0,0,0,255));

	void		Draw3dShapeConvex(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255));
	void		Draw3dShapeConcave(const RECT& rtRect, COLORREF color = RGBA(0,0,0,255));

	void		DrawLine(const SI_FRECT& rtRect,COLORREF color = RGBA(0,0,0,255));
	
	void		BegineTransparency(float fdegree);												//. 투명 수치를 설정하면 EndTransparency() 할때 까지 alpha 적용.
	void		EndTransparency();

	//. 텍스트 출력 함수.
	void		TextOutWZ(LPCSTR pszText, float fx, float fy, COLORREF coText = RGBA(0,0,0,255));
	void		DrawText(LPCSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255));	
    void		DrawText_Depth(LPCSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255), float depth = 0.0f);	
	void		DrawTextW(LPCWSTR pszText, RECT* prtRegin, DWORD dwStyle, COLORREF coText = RGBA(0,0,0,255), 
                            ENUM_TEXT_KIND eTK = ETK_BASE, COLORREF coLine = RGBA(0,0,0,255));
	
	void*		LoadImage(LPCSTR pFName);														//. 그림 파일을 읽어온다.

	//. 폰트 출력 관련 함수.
	void		SetDefaultFontID(DWORD dwFontID);												//. Default 폰트 설정
	DWORD		GetDefaultFontID();																

	bool		BeginUseFont(DWORD dwFontID);													//. 폰트 설정 시작 함수.
	void		EndUseFont();																	//. 폰트 설정 끝 함수

	//. DrawClass 초기화.
	void		SetDrawClass(void* pClass);
    void*		GetDrawClass() { return m_pDraw; };

	//. 실제 위치를 얻어온다.
	float		GetRealX(float x);																//. Real->Std 좌표를 가져온다.	
	float		GetRealY(float y);																//. Real->Std 좌표를 가져온다.

	//. 화면 좌표를 얻어온다.
	float		GetStdX(float x);																//. Std->Real 좌표를 가져온다.	
	float		GetStdY(float x);																//. Std->Real 좌표를 가져온다.	
	//string -> WZID로 변형해서 넘겨준다.
	DWORD		ChangeStrToDWORD(LPCSTR pszID);													//. string -> WZID로 변형해서 넘겨준다.
	
	void		SetExePath( LPCSTR pPath );														//. 실행경로를 가지고 있는다.
	const char*	GetExePath(){return m_szPath;}

	//. 빈 텍스쳐 생성 함수.
	VOID*		GenerateEmptyTexture( int iWidth, int iHeight, int iNumComponents);

	//. 텍스쳐의 정보를 변경할때 사용함수.
	VOID*		LockTexture(HANDLE hTexture, long *plPitch, int *piNumComponents);
	void		UnlockTexture( HANDLE hTexture);
	
	//. 배경 칼라 설정 함수.
	void		SetBackGroundColor(COLORREF color = RGBA(255,255,255,255));
	void		GetTextExtentPoint(LPCSTR lpszText, int iLength, SIZE* psiText);
	void		GetTextExtentPointW(LPCWSTR pwszText, int iLength, SIZE* psiText);

	//. 폰트 관련 함수들.

	HDC			GetFontDC(DWORD dwFontID);														//. Font DC 핸들을 얻어온다.
	BOOL		GetFontLogFont(DWORD dwFontID, LOGFONTA* pLogFont);								//. 아디에 해당하는 LogFont 정보 얻기

	//. Default 폰트
	int			GetNumberOfDefaultFont();														//. 디폴트 폰트 개수 얻기
	BOOL		GetDefaultFontInfo(int iIndex,DWORD* pdwFontID, LOGFONTA* pLogFont);			//. 디폴트폰트 정보 얻기

	//. 확장 폰트.
	BOOL		AddExtendedFont(DWORD dwFontID, LOGFONTA* pLogFont);							//. 확장폰트 추가
	void		DeleteExtendedFont(DWORD dwFontID);												//. 확장폰트 삭제
	void		DeleteAllExtendedFont(void);													//. 확장폰트 모두 삭제
	int			GetNumberOfExtendedFont(void);													//. 확장폰트 개수 얻기
	BOOL		GetExtendedFontInfo(int iIndex, DWORD* pdwFontID, LOGFONTA* pLogFont);			//. 확장폰트 정보 얻기
	BOOL		LoadExtendedFontInfo(LPCSTR pszPath);
	BOOL		SaveExtendedFontInfo(LPCSTR pszPath);
	
	//. 파일 병합 관련	
	HANDLE		FileOpen(LPCSTR pszFileName,	DWORD dwDesiredAccess,	DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition);
	
	DWORD		FileRead(HANDLE hHandle, LPVOID lpBuffer, DWORD dwNumberOfByteToRead);
	DWORD		FileWrite(HANDLE hHandle, LPVOID lpBuffer, DWORD dwNumberOfByteToWrite);
	DWORD		GetFileSize(HANDLE hHandle);
	void		FileClose(HANDLE hHandle);
	void		SetDiscAccess(VOID* pPtr);
	
    //------------------------------------------------------------------------------
    // Returns :int : 텍스트 영역의 길이
    // Parameter : LPCSTR message : 문자열
    // Parameter : const WzID font_id : 폰트
    //------------------------------------------------------------------------------
    int        GetTextWidth(LPCSTR message, const WzID font_id);
    //------------------------------------------------------------------------------
    // Returns :bool : 말줄임이 적용되었으면
    // Parameter : OUT std::string* reduce_string : 말줄임이 적용된 문자열
    // Parameter : LPCSTR message : 말줄임을 적용할 문자열
    // Parameter : const RECT& area : 텍스트 영역
    // Parameter : const WzID font_id : 폰트
    //------------------------------------------------------------------------------
    bool        MakeReduceText(OUT std::string* reduce_string, LPCSTR message, const RECT& area, const WzID font_id);

private:
	VOID*		m_pDraw;																		//. Draw Engine 포인터를 가지고 있다.
	VOID*		m_pDiscAccess;																	//. 파일 Packer 사용일 경우 이 포인트로 파일에 접근한다.
	char		m_szPath[MAX_PATH];																//. 리소스 파일 폴더 경로.	

	DWORD		m_dwCurUseFontID;
	DWORD		m_dwDefaultFontID;														
	
	bool		m_blHyper;																		//. 최적화 모드 체크.															//. 최적화를 위해 Text도 한번에 모아서 찍는다.
	float		m_fTransparency;																//. 투명도.
public:
	char		m_szVersion[10];																//. 버젼 
	
};

