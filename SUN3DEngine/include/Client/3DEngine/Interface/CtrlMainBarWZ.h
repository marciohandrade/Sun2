#pragma once

////*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//// CCtrlMainBarWZ Class
////*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
//class CCtrlMainBarWZ : public CControlWZ  
//{
//public:
//	CCtrlMainBarWZ();
//	virtual ~CCtrlMainBarWZ();
//	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw) ;
//	virtual void			OnDestroy();
//	virtual void			OnDoAction();
//	virtual bool			OnDraw(bool bl);
//
//	virtual void			OnInitialize();
//	virtual bool			SendMessageWZ(SI_MESSAGE* pMsg);
//
//	virtual void			SetStyle(DWORD dwStyle);
//	virtual void			UpdateWindowWZ(HWND hParent);
//	virtual void			ReleaseImage();
//
//	virtual void			OnLoad(HANDLE hHandle);
//	virtual void			OnSave(HANDLE hHandle);
//
//public:
//	void					GetDlgPartSizeRect(DWORD dwKind,RECT* rtSize);
//	void					SetDlgPartSizeRect(DWORD dwKind,RECT rtSize);
//
//	void					SetPartFormat(DWORD dwKind, SI_TFORMAT* pFMT);
//	void					SetImageFormat(SI_TFORMAT* pFMT);
//	SI_TFORMAT*				GetImageFormat(DWORD dwKind);
//
//	bool					GetCheckLButton(){return m_blLBClick;}
//	float					GetMouseDistanceX(){return m_fMMDistanceX;}
//	float					GetMouseDistanceY(){return m_fMMDistanceY;}
//
//	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
//
//private:
//	CCtrlButtonWZ			m_cCtrlButtonMini;
//	CCtrlButtonWZ			m_cCtrlButtonMax;
//	CCtrlButtonWZ			m_cCtrlButtonExit;
//
//	bool					m_blLBClick;			//마우스 왼쪽 버튼 클릭 했나. 안했나.
//	float					m_fLBClickX;
//	float					m_fLBClickY;
//	
//	float					m_fOldLBClickX;
//	float					m_fOldLBClickY;
//
//	float					m_fMMDistanceX;
//	float					m_fMMDistanceY;	
//};
