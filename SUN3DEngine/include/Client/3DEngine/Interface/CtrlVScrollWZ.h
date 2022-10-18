#pragma once

#include "CtrlButtonWZ.h"


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlVScrollWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
class CCtrlVScrollWZ : public CControlWZ  
{
public:
	CCtrlVScrollWZ();
	virtual ~CCtrlVScrollWZ();
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();

	virtual bool			OnDraw(bool bl);
	
	virtual void			OnInitialize();

	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	
	virtual void			UpdateWindowWZ(HWND hParent);
	virtual void			ReleaseImage();
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);
	
	virtual void			EnableWindowWZ(bool bState);

public:
	virtual void			SetChangeSizeThumb(int nSizeRate);			//. ��� ��ư ũ�⸦ �÷��ش�.(Min:5~Max:100);100�̸� ���� Tthum

	virtual void			SetScrollRangeWZ(int nMinPos, int nMaxPos);
	virtual void			GetScrollRangeWZ(int* pnMinPos, int* pnMaxPos);
	virtual void			SetScrollRangeWZ(float fMinPos, float fMaxPos);
	virtual void			GetScrollRangeWZ(float* pfMinPos, float* pfMaxPos);

	virtual int				SetScrollPosWZ(int nPos);					//. ������ -1:Min���� �۴�,1:Max���� ũ��. 0: OK
	virtual int				GetScrollPosWZ(){return (int)m_fCurPos; }
	virtual int				SetScrollPosWZ(float fPos);					//. ������ -1:Min���� �۴�,1:Max���� ũ��. 0: OK

	virtual void			SetArrowUPBTImageFormat(SI_TFORMAT* pFormat);		
	virtual void			SetArrowDownBTImageFormat(SI_TFORMAT* pFormat);
	virtual void			SetArrowThumbBTImageFormat(SI_TFORMAT* pFormat);

	virtual SI_TFORMAT*		GetArrowUPBTImageFormat();		
	virtual SI_TFORMAT*		GetArrowDownBTImageFormat();
	virtual SI_TFORMAT*		GetArrowThumbBTImageFormat();	

	virtual void			SetArrowUPBTIsOutline(bool blIs);
	virtual bool			GetArrowUPBTIsOutline();
	virtual void			SetArrowUPBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetArrowUPBTOutlineColor();
    virtual void			SetArrowUPSize(RECT& rect);
    virtual RECT			GetArrowUPSize();

	virtual void			SetArrowDownBTIsOutline(bool blIs);
	virtual bool			GetArrowDownBTIsOutline();
	virtual void			SetArrowDownBTOutlineColor(COLORREF rgb);
	virtual COLORREF		GetArrowDownBTOutlineColor();
    virtual void			SetArrowDownSize(RECT& rect);
    virtual RECT			GetArrowDownSize();

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
		
	RECT					m_rtShaftUp;		//. ������ Up ���� . topx, topy, width, Height
	RECT					m_rtShaftDown;		//. ������ Down ���� . topx, topy, width, Height

	bool					m_blThumbDrag;		//. ���� �巹�� �ϴ� ������ üũ.
	POINT					m_poLBD;			//. ���� ��ư ���������� ��ǥ.

	CCtrlButtonWZ			m_cBTArrowUp;		//. ȭ��ǥ up
	CCtrlButtonWZ			m_cBTArrowDown;		//. ȭ��ǥ down
	CCtrlButtonWZ			m_cBTThumb;			//. ��(Thumb) 
};
