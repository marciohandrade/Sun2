#pragma once

#include "ControlWZ.h"


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlStaticWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlStaticWZ : public CControlWZ  
{
public:
	CCtrlStaticWZ();
	virtual ~CCtrlStaticWZ();
	
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();
	
	virtual bool			OnDraw(bool bl);
	virtual void			OnInitialize();
	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);
	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	
	//. 이미지 관련 함수들.	
	virtual void			ReleaseImage();

	//. 저장/로딩 관련 함수들
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	//. Text 관련 함수들.
	virtual	void			GetTextWZA(OUT CHAR* pszText, int nLength);	
    virtual	void			GetTextWZW(OUT WCHAR* pwszText, int nLength);	

	virtual	void			SetTextWZA(LPCSTR pszText, bool bSelected=false);
	virtual	void			SetTextWZW(LPCWSTR pwszText, bool bSelected=false);

	virtual void			SetTextID(DWORD dwStrID);
	virtual	DWORD			GetTextID();

	virtual void			SetFontID(DWORD dwFontID);
	virtual DWORD			GetFontID();

    // [2010-3-10 by i4u4me]  
    virtual bool            IsTextReducing() const { return m_bTextReducing; }
    virtual void            SetTextReducing(bool bReducing) { m_bTextReducing = bReducing; }
    virtual bool            IsMoneyType() const { return m_bMoneyType; }
    virtual void            SetMoneyType(bool bMoneyType) { m_bMoneyType = bMoneyType; }

	//. 글씨 모양의 종류 설정.(기본, 그림자, 테두리)
	virtual void			SetTextKind(ENUM_TEXT_KIND eTK);
	virtual ENUM_TEXT_KIND  GetTextKind();

	void					SetTextColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextColor = rgbColor;}	
	COLORREF				GetTextColor(){return m_rgbTextColor;}
	
	void					SetTextOutlineColor(COLORREF rgbColor){m_rgbTextOutlineColor = rgbColor;}
	COLORREF				GetTextOutlineColor(){return m_rgbTextOutlineColor;}

	virtual void			ChangeCaptionString();

protected:
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg){return true;};

protected:
	COLORREF				m_rgbTextColor;
	COLORREF				m_rgbTextOutlineColor;

	STD_STRING				m_stdStrFontID;
	DWORD					m_dwFontID;
	ENUM_TEXT_KIND			m_eTextKind;	

private:
    void					CheckNewLine( IN OUT LPSTR szText ); 

    //! 문자열이 캡션 영역을 넘어 갔을대 "..." 으로 표시 할건지여부
    bool                    m_bTextReducing;

    //! 머니 타입 일 경우 "," 출력 여부
    bool                    m_bMoneyType;
};



//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlImageStaticWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlImageStaticWZ : public CCtrlStaticWZ
{
public:
	CCtrlImageStaticWZ();
	virtual ~CCtrlImageStaticWZ();

	virtual bool			OnDraw(bool bl);

	//. 저장/일기 관련 함수들
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual void			ReleaseImage();

    virtual bool            IsUseDefault() const { return m_bUseDefault; }
    virtual void            SetUseDefault(bool bDfault) { m_bUseDefault = bDfault; }

	void					SetTextMouseOnColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextMouseOnColor = rgbColor;}	
	COLORREF				GetTextMouseOnColor(){return m_rgbTextMouseOnColor;}

	void					SetTextMouseDownColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextMouseDownColor = rgbColor;}	
	COLORREF				GetTextMouseDownColor(){return m_rgbTextMouseDownColor;}

	void					SetTextDisableColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextDisableColor = rgbColor;}	
	COLORREF				GetTextDisableColor(){return m_rgbTextDisableColor;}
		
	void					SetTextOffsetX(long lnX){m_lnOffsetX = lnX;}
	long					GetTextOffsetX(){return m_lnOffsetX;}

	void					SetTextOffsetY(long lnY){m_lnOffsetY = lnY;}
	long					GetTextOffsetY(){return m_lnOffsetY;}

protected:
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

protected:
	COLORREF				m_rgbTextMouseOnColor;	
	COLORREF				m_rgbTextMouseDownColor;
	COLORREF				m_rgbTextDisableColor;	
	COLORREF				m_rgbCurrColor;	

	//. 마우스 클릭 시 Offset 만큼 이동 시킨다..
	long					m_lnOffsetX;
	long					m_lnOffsetY;

    //! ver 2.1
    bool                    m_bUseDefault;
};
