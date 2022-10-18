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

    //. ����/�ϱ� ���� �Լ���
    virtual void			OnLoad(HANDLE hHandle);
    virtual void			OnSave(HANDLE hHandle);

    //. Text ���� �Լ���.
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


    //. Color ���� �Լ���.
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

    //. ���콺 Ŭ�� ���� �� Text �ɼ��� �ش�.
    void					SetTextOffsetX(long lnX){m_lnOffsetX = lnX;}	
    long					GetTextOffsetX(){return m_lnOffsetX;}

    void					SetTextOffsetY(long lnY){m_lnOffsetY = lnY;}
    long					GetTextOffsetY(){return m_lnOffsetY;}


protected:
    COLORREF				m_rgbTextColor;						//. Text �⺻ Į��
    COLORREF				m_rgbTextMouseOnColor;				//. Text ���콺 On
    COLORREF				m_rgbTextMouseDownColor;			//. Text Mouse Click
    COLORREF				m_rgbTextOutlineColor;				//. �׵θ� ��� �϶� �ܰ� ���� Į��
    COLORREF				m_rgbTextDisableColor;				//. Disable ���� �϶��� Į��

    DWORD					m_dwFontID;							//. ����� ��Ʈ ID
    ENUM_TEXT_KIND			m_eTextKind;						//. �⺻ ���, �׸��� ���, �׵θ� ���

    //. ���콺 Ŭ�� �� Offset ��ŭ �̵� ��Ų��..
    long					m_lnOffsetX;
    long					m_lnOffsetY;

    COLORREF				m_rgbCurrColor;						//. ���� �׸� Color

private:
    //! ver 2.1 ���ڿ��� ĸ�� ������ �Ѿ� ������ "..." ���� ǥ�� �Ұ�������
    bool                    m_bTextReducing;

    //! ver 2.1 �Ӵ� Ÿ�� �� ��� "," ��� ����
    bool                    m_bMoneyType;

    //! ver 2.1 (�⺻ ��ư���� ��뿩��, ����Ű �������� �ش� ��ư�� ��������ó�� ���)
    bool                    m_bUseDefault;
};