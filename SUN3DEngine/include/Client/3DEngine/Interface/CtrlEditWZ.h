#pragma once


#include "ControlWZ.h"


//--------------------------------------------------------------
// CCtrlBaseEdit Class	�⺻ �Լ� ���� ���� Ŭ�����̴�.
// �⺻ EDit�� Direct Edit�� ���� ������ ���� Edit ��ũ �������θ� �����.
//--------------------------------------------------------------
class CCtrlBaseEdit : public CControlWZ  
{
public:
    CCtrlBaseEdit(){}
    virtual ~CCtrlBaseEdit(){}

public:

    //. ��Ʈ ���� �Լ�.
    virtual void			SetFontID(DWORD dwFontID){;}			//. Font ID �����Ѵ�.
    virtual DWORD			GetFontID(){return 0;}					//. Font ID ���´�.	

    virtual LOGFONTA*		GetFontInfo(){return NULL;}				//. ��Ʈ ������ �����´�.
    virtual void			SetFontInfo(LOGFONTA* lplf){}

    //. ���� ���� �Լ�.
    virtual void			SetColorCaret(COLORREF color){;}		//. ĳ�� ���� �����Ѵ�.
    virtual void			SetColorText(COLORREF color){;}			//. ���ڻ��� �����Ѵ�.

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

    //. �ؽ�Ʈ �����Լ�.
    //virtual void			GetTextWZA(OUT LPSTR pszText, int nLength){}
    //virtual void			GetTextWZW(OUT LPWSTR pwszText, int nLength){}

    //virtual void			SetTextWZA(LPCTSTR pszText, bool bSelected=false){}
    //virtual void			SetTextWZW(LPCTSTR pwszText, bool bSelected=false){}

    virtual void			SetLimitText(DWORD dwLimit){}			//. ������ ���̸� �����Ѵ�.
    virtual void			ClearText(){}							//. ������ �ʱ�ȭ �Ѵ�.

    //. ��Ŀ�� �����Լ�.	
    virtual void			SetFocusWZ(){}	

    //. ��Ÿ �Լ���.
    virtual HWND			GetEditHwnd(){WzAssert(!"DxEditBox�� ���� �ȵǼ� ��������.");return NULL;}
    virtual void			SetTextNCRect(int nTopThick, int nBottomThick, int nLeftThick, int nRightThick){}	//. �۾��� ����� ����. �ش� �β� ��ŭ ��ü ũ�⿡�� ���ش�.

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
{//. ���� ��� ����.
public:
    CCtrlEditWZ();
    virtual ~CCtrlEditWZ();

    virtual void			OnInitialize();

    virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
    virtual void			OnDestroy();
    virtual void			OnDoAction();
    virtual bool			OnDraw(bool bl);

    //. �ε� / ���� ���� �Լ�.
    virtual void			OnLoad(HANDLE hHandle);
    virtual void			OnSave(HANDLE hHandle);

    virtual void			SetStyle(DWORD dwStyle);
    virtual void			UpdateWindowWZ(HWND hParent);			//. �̹��� ���� ������ �Ҷ� ȣ��.
    virtual void			ReleaseImage();				

    virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
    virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);

    virtual bool			HaveFocus(){return m_blHasFocus;}
    virtual void			OnFocusIn();
    virtual void			OnFocusOut();

public:		
    //. ��Ʈ ���� �Լ�.
    virtual void			SetFontID(DWORD dwFontID);				//. Font ID �����Ѵ�.
    virtual DWORD			GetFontID();							//. Font ID ���´�.	

    virtual LOGFONTA*		GetFontInfo(){return &m_lplf;}			//. ��Ʈ ������ �����´�.
    virtual void			SetFontInfo(LOGFONTA* lplf){WzAssert(!"���� ����. ID�� ����ϼ�");}

    //. ���� ���� �Լ�.
    virtual void			SetColorCaret(COLORREF color);			//. ĳ�� ���� �����Ѵ�.
    virtual void			SetColorText(COLORREF color);			//. ���� ���� �����Ѵ�.

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

    //. �ؽ�Ʈ �����Լ�.
    virtual	void			GetTextWZA(OUT LPSTR pszText, int nLength);
    virtual	void			GetTextWZW(OUT LPWSTR pwszText, int nLength);

    virtual	void			SetTextWZA(LPCSTR pszText, bool bSelected=false);
    virtual	void			SetTextWZW(LPCWSTR pwszText, bool bSelected=false);

    virtual void			SetLimitText(DWORD dwLimit);			//. ������ ���̸� �����Ѵ�.
    virtual void			ClearText();							//. ������ �ʱ�ȭ �Ѵ�.

    //. ��Ŀ�� �����Լ�.	
    virtual void			SetFocusWZ();	

    //. ��Ÿ �Լ���.
    virtual HWND			GetEditHwnd(){WzAssert(!"DxEditBox�� ���� �ȵǼ� ��������.");return NULL;}
    virtual void			SetTextNCRect(int nTopThick, int nBottomThick, int nLeftThick, int nRightThick);	//. �۾��� ����� ����. �ش� �β� ��ŭ ��ü ũ�⿡�� ���ش�.

public:
    LRESULT					EditSubProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

protected:	
    void					ReleaseFont();
    void					ReleaseHDC();
    void					ClearBuffer24Bit(PBYTE pbyBuffer, short nRed, short nGreen, short nBlue);	//24 ��Ʈ ���ظ� ó���Ѵ�.
    bool					IsEditFocus();
    void					SetAlphaBuffer(void* pVoid);
    void					SetDCToTexture();
    void					CreateBackBuffer(int nWidth, int nHeight);
    void					RegisterLogFont(LOGFONTA* lplf);
protected:
    //. Mouse ���� �Լ���.
    virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
    virtual bool			Message_MouseLButtonDoubleClick(SI_MESSAGE* pMsg);

    virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);	

    //. KeyDown ���� �޼���.	
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

    HFONT					m_hFont;				//��Ʈ �ڵ�.
    HFONT					m_hOldFont;

    LOGFONTA				m_lplf;					//��Ʈ ���� ����ü API���� �����ϴ� ��Ʈ ���� ����ü

    COLORREF				m_rgbTextColor;			//�ؽ�Ʈ�� Į�� ������ ����.	
    COLORREF				m_rgbCaretColor;
    COLORREF				m_rgbSelBackColor;		//.���õ� ������ �� ����.
    COLORREF				m_rgbSelTextColor;		//.���õ� ������ �ؽ�Ʈ ������.

    // Color of various IME elements      
    COLORREF				m_rgbCompColor;         // Composition string color
    COLORREF				m_rgbCompWinColor;      // Composition string window color
    COLORREF				m_rgbCompCaretColor;    // Composition string caret color

    COLORREF				m_rgbIndicatorImeColor; // Indicator text color for IME
    COLORREF				m_rgbIndicatorEngColor; // Indicator text color for English
    COLORREF				m_rgbIndicatorBkColor;  // Indicator text background color

    bool					m_blCaretState;			//. ���� ĳ�� ����.
    bool					m_blOldCaretState;

    int						m_nAR, m_nAG, m_nAB;    //. ����ó�� �� ��� ����.
    //	STD_STRING	m_stdStrFontID;			//. ��Ʈ �Ƶ�.
    DWORD					m_dwFontID;			
    RECT					m_rcText;				//. Text�� ����.
    RECT					m_rcTextBorder;

};
