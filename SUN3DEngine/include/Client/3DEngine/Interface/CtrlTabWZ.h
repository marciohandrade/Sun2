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

	//. ����/�ε� �Լ���.
	virtual void			OnLoad(HANDLE hHandle);
	virtual void			OnSave(HANDLE hHandle);

public:
	virtual void			SetTabPosition(ENUM_TAB_POS eTabpos);			//. Tab�� ��ġ�� �����Ѵ�.
	ENUM_TAB_POS			GetTabPosition(){return m_eTabPos;}

	virtual bool			SetItemCount(int nCnt);							//. �ּҴ� 1�̴�.
	virtual int				GetItemCount(){return m_nItemCnt;}
	
	virtual void			SetItemButtonImageFormat(SI_TFORMAT* pFormat);	//. Item ��ư�� �̹��� ������ ���� ���ش�.
	virtual SI_TFORMAT*		GetItemButtonImageFormat();						//. Item ��ư�� �̹��� ������ �Ѱ��ش�.

	virtual int				GetCurSel(){return m_nSelItem;}
	virtual void			SetCurSel(int nSel);

	virtual void			SetItemButtonIsOutline(bool blIs);				//. Item ��ư�� �ܰ��� �ֳ�. ����.
	virtual bool			GetItemButtonIsOutline();
	virtual void			SetItemButtonOutlineColor(COLORREF rgb);		//. ITem ��ư�� �Ͱ��� Į�� ����.
	virtual COLORREF		GetItemButtonOutlineColor();

	virtual void			SetItemWidth(long lnW);							//. Tab button �� ���̸� �������ش�.
	virtual long			GetItemWidth(){return m_lnItemWidth;}			//. Tab button �� ���̸� �����´�.
	
	virtual short			GetHeightRatio(){return m_nSelectRatio;}		//. Tab Height or Base Height Ratio.
	virtual void			SetHeightRatio(short nRatio);
	
	virtual int				GetItemUseIDCount(int IIndex);					//. Item button�� ��ġ���� �ش�.
	virtual DWORD			GetItemUseIDIndex(int IIndex, int DIndex);		//. Item button�� ��ġ���� ������ ����Ÿ�� ��ġ���� �ش�.
	virtual void			AddItembelongID(int IIndex, DWORD dwID);		//. Item �ȿ� ����� ID �߰�.
	virtual void			RemoveItembelongID(int IIndex, DWORD dwID);		//. Item �ȿ� �������  ID ����.
	virtual void			RemoveItemUseIDAll(int IIndex);

protected:
	void					SelectItemButton(int nIndex);

	//. Mouse ���� �Լ���.
	virtual bool			Message_MouseLButtonDown(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseLButtonUp(SI_MESSAGE* pMsg);
	virtual bool			Message_MouseMove(SI_MESSAGE* pMsg);

	void					DrawTabPosTop();
	void					DrawTabPosLeft();
	void					DrawTabPosBottom();
	void					DrawTabPosRight();

protected:
	STD_TAB_MAP				m_stdTabMab;

	short					m_nItemCnt;										//.���� ����. �⺻ 1�� ����.
	int						m_nSelItem;										//.���õ� Item Index.

	bool					m_blItemOutline;
	COLORREF				m_rgbItemColor;

	long					m_lnItemWidth;
	short					m_nSelectRatio;
	float					m_fHeightRatio;

	ENUM_TAB_POS			m_eTabPos;
};
