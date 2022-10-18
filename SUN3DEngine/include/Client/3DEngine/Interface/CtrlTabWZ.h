#pragma once

#include "ControlWZ.h"


//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
// CCtrlTabWZ Class
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
typedef std::map <DWORD,DWORD> STD_TAB_MAP;
enum ENUM_TAB_POS {ETAB_POS_TOP, ETAB_POS_LEFT, ETAB_POS_BOTTOM, ETAB_POS_RIGHT};
class CCtrlTabWZ : public CControlWZ  
{
public:
	CCtrlTabWZ();
	virtual ~CCtrlTabWZ();
	virtual bool			OnCreate(DWORD dwCtrlID,DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);
	virtual void			OnDestroy();

	virtual bool			OnDraw(bool bl);
	
	virtual void			OnInitialize();

	virtual bool			MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);		

	virtual void			ReleaseImage();

	//. 저장/로드 함수들.
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

public:
	virtual void			SetTabPosition(ENUM_TAB_POS eTabpos);			//. Tab의 위치를 설정한다.
	ENUM_TAB_POS			GetTabPosition(){return m_eTabPos;}

	virtual bool			SetItemCount(int nCnt);							//. 최소는 1이다.
	virtual int				GetItemCount(){return m_nItemCnt;}
	
	virtual void			SetItemButtonImageFormat(SI_TFORMAT* pFormat);	//. Item 버튼의 이미지 정보를 셋팅 해준다.
	virtual SI_TFORMAT*		GetItemButtonImageFormat();						//. Item 버튼의 미미지 정보를 넘겨준다.

	virtual int				GetCurSel(){return m_nSelItem;}
	virtual void			SetCurSel(int nSel);

	virtual void			SetItemButtonIsOutline(bool blIs);				//. Item 버튼의 외곽선 있냐. 없냐.
	virtual bool			GetItemButtonIsOutline();
	virtual void			SetItemButtonOutlineColor(COLORREF rgb);		//. ITem 버튼의 와곽선 칼라 셋팅.
	virtual COLORREF		GetItemButtonOutlineColor();

	virtual void			SetItemWidth(long lnW);							//. Tab button 의 넓이를 셋팅해준다.
	virtual long			GetItemWidth(){return m_lnItemWidth;}			//. Tab button 의 넓이를 가져온다.
	
	virtual short			GetHeightRatio(){return m_nSelectRatio;}		//. Tab Height or Base Height Ratio.
	virtual void			SetHeightRatio(short nRatio);
	
	virtual int				GetItemUseIDCount(int IIndex);					//. Item button의 위치값을 준다.
	virtual DWORD			GetItemUseIDIndex(int IIndex, int DIndex);		//. Item button의 위치값과 가져올 데이타의 위치값을 준다.
	virtual void			AddItembelongID(int IIndex, DWORD dwID);		//. Item 안에 사용할 ID 추가.
	virtual void			RemoveItembelongID(int IIndex, DWORD dwID);		//. Item 안에 사용중인  ID 제거.
	virtual void			RemoveItemUseIDAll(int IIndex);

protected:
	void					SelectItemButton(int nIndex);

	//. Mouse 관련 함수들.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

	void					DrawTabPosTop();
	void					DrawTabPosLeft();
	void					DrawTabPosBottom();
	void					DrawTabPosRight();

protected:
	STD_TAB_MAP				m_stdTabMab;

	short					m_nItemCnt;										//.탭의 갯수. 기본 1로 셋팅.
	int						m_nSelItem;										//.선택된 Item Index.

	bool					m_blItemOutline;
	COLORREF				m_rgbItemColor;

	long					m_lnItemWidth;
	short					m_nSelectRatio;
	float					m_fHeightRatio;

	ENUM_TAB_POS			m_eTabPos;
};
