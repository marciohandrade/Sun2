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
	virtual void			SetChangeSizeThumb(int nSizeRate);			//. 가운데 버튼 크기를 늘려준다.

	virtual void			SetScrollRangeWZ(int nMinPos, int nMaxPos);
	virtual void			GetScrollRangeWZ(int* pnMinPos, int* pnMaxPos);
	virtual void			SetScrollRangeWZ(float fMinPos, float fMaxPos);
	virtual void			GetScrollRangeWZ(float* pfMinPos, float* pfMaxPos);

	virtual int				SetScrollPosWZ(int nPos);					//. 리턴이 -1:Min보다 작다,1:Max보다 크다. 0: OK
	virtual int				GetScrollPosWZ(){return (int)m_fCurPos; }
	virtual int				SetScrollPosWZ(float fPos);					//. 리턴이 -1:Min보다 작다,1:Max보다 크다. 0: OK

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
	//. Mouse 관련 함수들.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

protected:	
	short					m_nMinSizeX;		//. 크기의 최소 X값.
		
	int						m_nSpaceLength;		//. 몸통의 여백 길이.

	float					m_fMinPos;
	float					m_fMaxPos;
	float					m_fCurPos;			//. 현재 위치 (비율)	

	RECT					m_rtShaftLeft;		//. 몸통의 Left 영역 . topx, topy, width, Height
	RECT					m_rtShaftRight;		//. 몸통의 Right 영역 . topx, topy, width, Height

	bool					m_blThumbDrag;		//. 썸을 드레그 하는 중인지 체크.
	POINT					m_poLBD;			//. 마우스 왼쪽 버튼 눌렀을때의 좌표.
	
	CCtrlButtonWZ			m_cBTArrowLeft;		//. 화살표 Left
	CCtrlButtonWZ			m_cBTArrowRight;	//. 화살표 Right
	CCtrlButtonWZ			m_cBTThumb;			//. 썸(Thumb) 
};
