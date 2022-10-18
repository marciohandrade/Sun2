#pragma once

#include "ControlWZ.h"

//--------------------------------------------------------------
// CCtrlButtonWZ Class
//--------------------------------------------------------------
class CCtrlButtonWZ : public CControlWZ  
{
public:
    CCtrlButtonWZ();
    virtual ~CCtrlButtonWZ();

    virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);	
    virtual bool			OnDraw(bool bl);

    //. 저장/일기 관련 함수들
    virtual void			OnLoad(HANDLE hHandle);
    virtual void			OnSave(HANDLE hHandle);

    //. Text 관련 함수들.
    virtual	void			GetTextWZA(OUT LPSTR pszText, int nLength);	
    virtual	void			GetTextWZW(OUT LPWSTR pwszText, int nLength);

    virtual	void			SetTextWZA(LPCSTR pszText, bool bSelected=false);
    virtual	void			SetTextWZW(LPCWSTR pwszText, bool bSelected=false);

    virtual void			SetTextID(DWORD dwStrID);
    virtual	DWORD			GetTextID();

    virtual void			SetFontID(DWORD dwFontID);
    virtual DWORD			GetFontID();

    virtual bool            IsTextReducing() const { return m_bTextReducing; }
    virtual void            SetTextReducing(bool bReducing) { m_bTextReducing = bReducing; }
    virtual bool            IsMoneyType() const { return m_bMoneyType; }
    virtual void            SetMoneyType(bool bMoneyType) { m_bMoneyType = bMoneyType; }
    virtual bool            IsUseDefault() const { return m_bUseDefault; }
    virtual void            SetUseDefault(bool bDfault) { m_bUseDefault = bDfault; }


    //. Color 관련 함수들.
    void					SetTextColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextColor = rgbColor;}	
    COLORREF				GetTextColor(){return m_rgbTextColor;}

    void					SetTextMouseOnColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextMouseOnColor = rgbColor;}	
    COLORREF				GetTextMouseOnColor(){return m_rgbTextMouseOnColor;}

    void					SetTextMouseDownColor(COLORREF rgbColor = RGBA(0,0,0,255)){m_rgbTextMouseDownColor = rgbColor;}	
    COLORREF				GetTextMouseDownColor(){return m_rgbTextMouseDownColor;}

    void					SetTextOutlineColor(COLORREF rgbColor){m_rgbTextOutlineColor = rgbColor;}
    COLORREF				GetTextOutlineColor(){return m_rgbTextOutlineColor;}

    void					SetTextDisableColor(COLORREF rgbColor){m_rgbTextDisableColor = rgbColor;}
    COLORREF				GetTextDisableColor(){return m_rgbTextDisableColor;}

    virtual void			SetTextKind(ENUM_TEXT_KIND eTK);
    virtual ENUM_TEXT_KIND  GetTextKind();

    //. 마우스 클릭 했을 때 Text 옵셋을 준다.
    void					SetTextOffsetX(long lnX){m_lnOffsetX = lnX;}	
    long					GetTextOffsetX(){return m_lnOffsetX;}

    void					SetTextOffsetY(long lnY){m_lnOffsetY = lnY;}
    long					GetTextOffsetY(){return m_lnOffsetY;}


protected:
    COLORREF				m_rgbTextColor;						//. Text 기본 칼라
    COLORREF				m_rgbTextMouseOnColor;				//. Text 마우스 On
    COLORREF				m_rgbTextMouseDownColor;			//. Text Mouse Click
    COLORREF				m_rgbTextOutlineColor;				//. 테두리 모양 일때 외각 라인 칼라
    COLORREF				m_rgbTextDisableColor;				//. Disable 상태 일때의 칼라

    DWORD					m_dwFontID;							//. 사용할 폰트 ID
    ENUM_TEXT_KIND			m_eTextKind;						//. 기본 모양, 그림자 모야, 테두리 모양

    //. 마우스 클릭 시 Offset 만큼 이동 시킨다..
    long					m_lnOffsetX;
    long					m_lnOffsetY;

    COLORREF				m_rgbCurrColor;						//. 현재 그릴 Color

private:
    //! ver 2.1 문자열이 캡션 영역을 넘어 갔을대 "..." 으로 표시 할건지여부
    bool                    m_bTextReducing;

    //! ver 2.1 머니 타입 일 경우 "," 출력 여부
    bool                    m_bMoneyType;

    //! ver 2.1 (기본 버튼으로 사용여부, 엔터키 눌렀을때 해당 버튼이 눌러진것처럼 사용)
    bool                    m_bUseDefault;
};