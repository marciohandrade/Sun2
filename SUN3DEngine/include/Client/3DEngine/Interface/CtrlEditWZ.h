#pragma once


#include "ControlWZ.h"


//--------------------------------------------------------------
// CCtrlBaseEdit Class	기본 함수 연결 기준 클래스이다.
// 기본 EDit와 Direct Edit와 관리 차원의 상위 Edit 링크 개념으로만 사용함.
//--------------------------------------------------------------
class CCtrlBaseEdit : public CControlWZ  
{
public:
    CCtrlBaseEdit(){}
    virtual ~CCtrlBaseEdit(){}

public:

    //. 폰트 관련 함수.
    virtual void			SetFontID(DWORD dwFontID){;}			//. Font ID 설정한다.
    virtual DWORD			GetFontID(){return 0;}					//. Font ID 얻어온다.	

    virtual LOGFONTA*		GetFontInfo(){return NULL;}				//. 폰트 정보를 가져온다.
    virtual void			SetFontInfo(LOGFONTA* lplf){}

    //. 색상 관련 함수.
    virtual void			SetColorCaret(COLORREF color){;}		//. 캐럿 색을 설정한다.
    virtual void			SetColorText(COLORREF color){;}			//. 문자색을 설정한다.

    virtual void			SetColorSelBack(COLORREF color){;}
    virtual void			SetColorSelText(COLORREF color){;}

    virtual void			SetColorCompText(COLORREF color){;}
    virtual void			SetColorCompBack(COLORREF color){;}
    virtual void			SetColorCompCaret(COLORREF color){;}
    virtual void			SetColorIndicatorIme(COLORREF color){;}
    virtual void			SetColorIndicatorEng(COLORREF color){;}
    virtual void			SetColorIndicatorBK(COLORREF color){;}

    virtual COLORREF		GetFontColor(){return NULL;}
    virtual COLORREF		GetCaretColor(){return NULL;}
    virtual COLORREF		GetSelBackColor(){return NULL;}
    virtual COLORREF		GetSelTextColor(){return NULL;}

    virtual COLORREF		GetCompTextColor(){return NULL;}
    virtual COLORREF		GetCompBackColor(){return NULL;}
    virtual COLORREF		GetCompCaretColor(){return NULL;}

    virtual COLORREF		GetIndicatorImeColor(){return NULL;}
    virtual COLORREF		GetIndicatorEngColor(){return NULL;}
    virtual COLORREF		GetIndicatorBKColor(){return NULL;}

    //. 텍스트 관련함수.
    //virtual void			GetTextWZA(OUT LPSTR pszText, int nLength){}
    //virtual void			GetTextWZW(OUT LPWSTR pwszText, int nLength){}

    //virtual void			SetTextWZA(LPCTSTR pszText, bool bSelected=false){}
    //virtual void			SetTextWZW(LPCTSTR pwszText, bool bSelected=false){}

    virtual void			SetLimitText(DWORD dwLimit){}			//. 문장의 길이를 제한한다.
    virtual void			ClearText(){}							//. 문장을 초기화 한다.

    //. 포커스 관련함수.	
    virtual void			SetFocusWZ(){}	

    //. 기타 함수들.
    virtual HWND			GetEditHwnd(){WzAssert(!"DxEditBox랑 연동 안되서 지원안함.");return NULL;}
    virtual void			SetTextNCRect(int nTopThick, int nBottomThick, int nLeftThick, int nRightThick){}	//. 글씨를 출력할 설정. 해당 두께 만큼 전체 크기에서 빼준다.

    virtual void			SetUseIndicator(bool bl){}
    virtual bool			GetUseIndicator(){return false;}

    virtual void			SetTextOutlineColor(COLORREF rgbColor){}
    virtual COLORREF		GetTextOutlineColor(){return RGBA(0,0,0,255);}

    virtual void			SetTextKind(ENUM_TEXT_KIND eTK){};
    virtual ENUM_TEXT_KIND  GetTextKind(){return ETK_BASE;};

};

//--------------------------------------------------------------
// CCtrlEditWZ Class
//--------------------------------------------------------------
class CCtrlEditWZ : public CCtrlBaseEdit  
{//. 현재 사용 안함.
public:
    CCtrlEditWZ();
    virtual ~CCtrlEditWZ();

    virtual void			OnInitialize();

    virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
    virtual void			OnDestroy();
    virtual void			OnDoAction();
    virtual bool			OnDraw(bool bl);

    //. 로드 / 저장 관련 함수.
    virtual void			OnLoad(HANDLE hHandle);
    virtual void			OnSave(HANDLE hHandle);

    virtual void			SetStyle(DWORD dwStyle);
    virtual void			UpdateWindowWZ(HWND hParent);			//. 이미지 정보 갱신을 할때 호출.
    virtual void			ReleaseImage();				

    virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
    virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);

    virtual bool			HaveFocus(){return m_blHasFocus;}
    virtual void			OnFocusIn();
    virtual void			OnFocusOut();

public:		
    //. 폰트 관련 함수.
    virtual void			SetFontID(DWORD dwFontID);				//. Font ID 설정한다.
    virtual DWORD			GetFontID();							//. Font ID 얻어온다.	

    virtual LOGFONTA*		GetFontInfo(){return &m_lplf;}			//. 폰트 정보를 가져온다.
    virtual void			SetFontInfo(LOGFONTA* lplf){WzAssert(!"지원 안함. ID로 사용하셈");}

    //. 색상 관련 함수.
    virtual void			SetColorCaret(COLORREF color);			//. 캐럿 색을 설정한다.
    virtual void			SetColorText(COLORREF color);			//. 문자 색을 설정한다.

    virtual void			SetColorSelBack(COLORREF color){m_rgbSelBackColor = color;}	
    virtual void			SetColorSelText(COLORREF color){m_rgbSelTextColor = color;}

    virtual void			SetColorCompText(COLORREF color){m_rgbCompColor = color;}
    virtual void			SetColorCompBack(COLORREF color){m_rgbCompWinColor = color;}
    virtual void			SetColorCompCaret(COLORREF color){m_rgbCompCaretColor = color;}
    virtual void			SetColorIndicatorIme(COLORREF color){m_rgbIndicatorImeColor = color;}
    virtual void			SetColorIndicatorEng(COLORREF color){m_rgbIndicatorEngColor = color;}
    virtual void			SetColorIndicatorBK(COLORREF color){m_rgbIndicatorBkColor = color;}

    virtual COLORREF		GetFontColor();
    virtual COLORREF		GetCaretColor(){return m_rgbCaretColor;}
    virtual COLORREF		GetSelBackColor(){return m_rgbSelBackColor;}
    virtual COLORREF		GetSelTextColor(){return m_rgbSelTextColor;}

    virtual COLORREF		GetCompTextColor(){return m_rgbCompColor;}
    virtual COLORREF		GetCompBackColor(){return m_rgbCompWinColor;}
    virtual COLORREF		GetCompCaretColor(){return m_rgbCompCaretColor;}

    virtual COLORREF		GetIndicatorImeColor(){return m_rgbIndicatorImeColor;}
    virtual COLORREF		GetIndicatorEngColor(){return m_rgbIndicatorEngColor;}
    virtual COLORREF		GetIndicatorBKColor(){return m_rgbIndicatorBkColor;}

    //. 텍스트 관련함수.
    virtual	void			GetTextWZA(OUT LPSTR pszText, int nLength);
    virtual	void			GetTextWZW(OUT LPWSTR pwszText, int nLength);

    virtual	void			SetTextWZA(LPCSTR pszText, bool bSelected=false);
    virtual	void			SetTextWZW(LPCWSTR pwszText, bool bSelected=false);

    virtual void			SetLimitText(DWORD dwLimit);			//. 문장의 길이를 제한한다.
    virtual void			ClearText();							//. 문장을 초기화 한다.

    //. 포커스 관련함수.	
    virtual void			SetFocusWZ();	

    //. 기타 함수들.
    virtual HWND			GetEditHwnd(){WzAssert(!"DxEditBox랑 연동 안되서 지원안함.");return NULL;}
    virtual void			SetTextNCRect(int nTopThick, int nBottomThick, int nLeftThick, int nRightThick);	//. 글씨를 출력할 설정. 해당 두께 만큼 전체 크기에서 빼준다.

public:
    LRESULT					EditSubProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

protected:	
    void					ReleaseFont();
    void					ReleaseHDC();
    void					ClearBuffer24Bit(PBYTE pbyBuffer, short nRed, short nGreen, short nBlue);	//24 비트 기준만 처리한다.
    bool					IsEditFocus();
    void					SetAlphaBuffer(void* pVoid);
    void					SetDCToTexture();
    void					CreateBackBuffer(int nWidth, int nHeight);
    void					RegisterLogFont(LOGFONTA* lplf);
protected:
    //. Mouse 관련 함수들.
    virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseLButtonDoubleClick(SI_MESSAGE* pMsg);

    virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);	

    //. KeyDown 관련 메세지.	
    virtual bool			Message_KeyDown(SI_MESSAGE* pMsg);

private:
    HWND					m_hWndEdit;
    WNDPROC					m_OldEditProc;

    int						m_nLimit;				//. Text Limit 

    HDC						m_hdc;
    HBITMAP					m_hBmp,m_hOldBmp;	
    BOOL					m_blPassword;
    BOOL					m_blComposition;

    BYTE*					m_pFirst;
    void*					m_pHandleText;

    HFONT					m_hFont;				//폰트 핸들.
    HFONT					m_hOldFont;

    LOGFONTA				m_lplf;					//폰트 정보 구조체 API에서 지원하는 폰트 정보 구조체

    COLORREF				m_rgbTextColor;			//텍스트의 칼라 정보를 셋팅.	
    COLORREF				m_rgbCaretColor;
    COLORREF				m_rgbSelBackColor;		//.선택된 영역의 색 설정.
    COLORREF				m_rgbSelTextColor;		//.선택된 영역의 텍스트 색설정.

    // Color of various IME elements      
    COLORREF				m_rgbCompColor;         // Composition string color
    COLORREF				m_rgbCompWinColor;      // Composition string window color
    COLORREF				m_rgbCompCaretColor;    // Composition string caret color

    COLORREF				m_rgbIndicatorImeColor; // Indicator text color for IME
    COLORREF				m_rgbIndicatorEngColor; // Indicator text color for English
    COLORREF				m_rgbIndicatorBkColor;  // Indicator text background color

    bool					m_blCaretState;			//. 현제 캐럿 상태.
    bool					m_blOldCaretState;

    int						m_nAR, m_nAG, m_nAB;    //. 알파처리 할 배경 색깔.
    //	STD_STRING	m_stdStrFontID;			//. 폰트 아디.
    DWORD					m_dwFontID;			
    RECT					m_rcText;				//. Text의 영역.
    RECT					m_rcTextBorder;

};
