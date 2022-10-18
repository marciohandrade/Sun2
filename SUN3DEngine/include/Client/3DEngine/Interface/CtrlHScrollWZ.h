#pragma once

#include "CtrlButtonWZ.h"

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlHScrollWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlHScrollWZ : public CControlWZ  
{
public:
	CCtrlHScrollWZ();
	virtual ~CCtrlHScrollWZ();
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();

	virtual bool			OnDraw(bool bl);
	
	virtual void			OnInitialize();

	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);	

	virtual void			SetStyle(DWORD dwStyle);
	virtual void			UpdateWindowWZ(HWND hParent);
	virtual void			ReleaseImage();

	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

	virtual void			EnableWindowWZ(bool bState);

public:
	virtual void			SetChangeSizeThumb(int nSizeRate);			//. ��� ��ư ũ�⸦ �÷��ش�.

	virtual void			SetScrollRangeWZ(int nMinPos, int nMaxPos);
	virtual void			GetScrollRangeWZ(int* pnMinPos, int* pnMaxPos);
	virtual void			SetScrollRangeWZ(float fMinPos, float fMaxPos);
	virtual void			GetScrollRangeWZ(float* pfMinPos, float* pfMaxPos);

	virtual int				SetScrollPosWZ(int nPos);					//. ������ -1:Min���� �۴�,1:Max���� ũ��. 0: OK
	virtual int				GetScrollPosWZ(){return (int)m_fCurPos; }
	virtual int				SetScrollPosWZ(float fPos);					//. ������ -1:Min���� �۴�,1:Max���� ũ��. 0: OK

	virtual void			SetArrowLeftBTImageFormat(SI_TFORMAT* pFormat);		
	virtual void			SetArrowRightBTImageFormat(SI_TFORMAT* pFormat);
	virtual void			SetArrowThumbBTImageFormat(SI_TFORMAT* pFormat);

	virtual SI_TFORMAT*		GetArrowLeftBTImageFormat();		
	virtual SI_TFORMAT*		GetArrowRightBTImageFormat();
	virtual SI_TFORMAT*		GetArrowThumbBTImageFormat();	

	virtual void			SetArrowLeftBTIsOutline(bool blIs);
	virtual bool			GetArrowLeftBTIsOutline();
	virtual void			SetArrowLeftBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetArrowLeftBTOutlineColor();
    virtual void			SetArrowLeftSize(RECT& rect);
    virtual RECT			GetArrowLeftSize();

	virtual void			SetArrowRightBTIsOutline(bool blIs);
	virtual bool			GetArrowRightBTIsOutline();
	virtual void			SetArrowRightBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetArrowRightBTOutlineColor();
    virtual void			SetArrowRightSize(RECT& rect);
    virtual RECT			GetArrowRightSize();

	virtual void			SetArrowThumbBTIsOutline(bool blIs);
	virtual bool			GetArrowThumbBTIsOutline();
	virtual void			SetArrowThumbBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetArrowThumbBTOutlineColor();
    virtual void			SetArrowThumbSize(RECT& rect);
    virtual RECT			GetArrowThumbSize();

protected:
	//. Mouse ���� �Լ���.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

protected:	
	short					m_nMinSizeX;		//. ũ���� �ּ� X��.
		
	int						m_nSpaceLength;		//. ������ ���� ����.

	float					m_fMinPos;
	float					m_fMaxPos;
	float					m_fCurPos;			//. ���� ��ġ (����)	

	RECT					m_rtShaftLeft;		//. ������ Left ���� . topx, topy, width, Height
	RECT					m_rtShaftRight;		//. ������ Right ���� . topx, topy, width, Height

	bool					m_blThumbDrag;		//. ���� �巹�� �ϴ� ������ üũ.
	POINT					m_poLBD;			//. ���콺 ���� ��ư ���������� ��ǥ.
	
	CCtrlButtonWZ			m_cBTArrowLeft;		//. ȭ��ǥ Left
	CCtrlButtonWZ			m_cBTArrowRight;	//. ȭ��ǥ Right
	CCtrlButtonWZ			m_cBTThumb;			//. ��(Thumb) 
};
