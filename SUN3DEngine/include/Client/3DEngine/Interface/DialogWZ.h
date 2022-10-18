#pragma once

#include "dlgbasewz.h"
#include "controlwz.h"
#include "CtrlClientWZ.h"
#include <list>

typedef std::list< CControlWZ* >	STD_UI_EDIT_SORT;
typedef std::list<RECT>				STD_UI_INTERSECT_RECT;

//------------------------------------------------------------------------------
//! Qualifier: �ش� ���̾�α��� tooltip �׷����� �������� �ݹ��� ȣ���Ѵ�.
//------------------------------------------------------------------------------
typedef void (*fn_CallbackTooltip)(CDialogWZ* dialog_ptr, SI_ToolTip* tooltip,int x, int y);

class CStringManager;
class CDialogManager;
class CDialogWZ : public CDlgBaseWZ 
{
public:
	//. ��� �޴� �Լ���.
	CDialogWZ();
	virtual ~CDialogWZ();
	virtual bool							OnCreate(DWORD dwID, DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,void* pDraw,CDialogManager* pPtr);	//DialogŬ������ �ʱ�ȭ ���ش�.
	virtual void							OnDoAction();																	//. Action 
	virtual void							OnDraw(bool bl);
	virtual bool							OnDrawControlWZ(DWORD dwCtrlID);												//. Dialog �ȿ� ��ϵ� Control�� �׷��ش�.
	virtual void							ShowWindowWZ(ENUM_STATEWZ eState);
	virtual void							UpdateWindowWZ(HWND hParent = NULL);

	virtual void							OnFocusIn();
    virtual void							OnFocusOut();
	virtual bool							HaveFocus(){return m_blHasFocus;}

public:		
	void									OnDestroy();
	void									OnRelease();	
	
	bool									RemoveControlWZ(DWORD dwCtrlID);
	bool									SendMessageWZ(SI_MESSAGE* pMsg);
    bool                                    OnKeyDown(OUT SI_MESSAGE* message);
	
	void									SetManager(CBaseManager* pPtr){m_pDialogManager = pPtr;}								//���� �����ִ� Manager PTR(������)�� �������ִ�.
	void									SetStringManager(CStringManager* pPtr){m_pStringManager = pPtr;}

	CControlWZ*								CreateControllWZ(DWORD dwCtrlID,DWORD dwKind, DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);

	void									SetToolTipPtr(SI_ToolTip* pToolTip){m_pToolTipInfo = pToolTip;}
	void									SetToolTipPoint(POINT point){m_poToolTip=point;}	
	
public:
	//. �ܺ� ���ٿ� �Լ���.
	virtual void							RequestFocus(CControlWZ* pCtrl);												//. ��Ŀ���� üũ �Ѵ�.(��� ���� ��� NULL ����) 

	virtual void							OnInitialize();
	void									DisableInterSectRect(const RECT& rRT){ m_bInterSectRect = rRT; }//02/27
	std::list<RECT>&					    GetIntersectRectList()		{	return m_IntersectRectList; }
	virtual bool							InterSectRect(POINT point);														//. InterSectRect�� üũ���ش�.
	
	virtual DWORD							GetDlgID(){return m_dwDialogID;}
	virtual bool							SetDlgID(DWORD dwDlgID);

	virtual CControlWZ*						GetClientWZ(){return &m_ctrlClientWZ;}
	virtual	CControlWZ*						GetControlWZ(DWORD dwCtrlID);
	virtual CControlWZ*						FindControlWZ(int nIndex);														//. �ε����� �ش��ϴ� ��ġ�� ControlWZ �����͸� ã�´�.

	virtual void							SetStyle(DWORD dwStyle);

    //------------------------------------------------------------------------------
    // Qualifier : ���̰� �ִ´��̾�α�, ��¥ �̹����� ���� ����(Ÿ��, 3����� ����ȵ�)
    // Parameter : float depth 0 ~ 1����
    //------------------------------------------------------------------------------
    virtual void							SetDepth(float depth);

	//. Image ���� ���� �Լ���
	virtual void							SetDialogImageFormat(SI_TFORMAT* pFMT);
	virtual void							SetControlImageFormat(DWORD dwCtrlID,SI_TFORMAT* pFMT);

	virtual SI_TFORMAT*						GetDialogImageFormat(DWORD dwKind = CTRL_KIND_CLIENT);
	virtual void							SetDialogPartFormat(SI_TFORMAT* pFMT, DWORD dwKind = CTRL_KIND_CLIENT);
	virtual void							ReleaseImage();	

	//. ����/ �ε�
	virtual void							OnLoad(HANDLE hHandle);
	virtual void							OnSave(HANDLE hHandle);

	virtual void							GetDlgPartSizeRect(RECT* rtSize,DWORD dwKind = CTRL_KIND_CLIENT);
	virtual void							SetDlgPartSizeRect(RECT rtSize,DWORD dwKind = CTRL_KIND_CLIENT);

	virtual bool							MoveWindowWZ(DWORD dwCtrlID, float fx, float fy, float fWidth, float fHeight);
	virtual bool							MoveWindowWZ(float fx, float fy, float fWidth, float fHeight);
	virtual bool							MoveWindowWZ(float fx, float fy);

	virtual void							GetDlgSize(RECT* prtD);
	virtual void							GetClientRect(RECT* prtD);
	virtual void							GetCtrlSize(DWORD dwCtrlID, RECT* prtRect);

	virtual DWORD							GetControlCount();													//. ��ϵ� ��Ʈ���� ��

	virtual void							SetMoveRECT(RECT rtR)												//. �̵� ���� ���� �ڽ� ����. �� �ȿ� Ŭ���� �̵��Ҽ� �ִ�.
	{
		m_rtMoveRECT.left	= rtR.left;
		m_rtMoveRECT.top	= rtR.top;
		m_rtMoveRECT.right	= rtR.right;
		m_rtMoveRECT.bottom = rtR.bottom;
	}
	virtual RECT							GetMoveRECT(){return m_rtMoveRECT;}									//. �̵� ���� ���.

	virtual void							MoveToListIndex(int nOrig, int nDes);								//. Dialog �켱������ �������ش�.

	//. �ܰ� ���� ���� �Լ�.
	virtual void							SetIsOutline(bool blIs){m_ctrlClientWZ.SetIsOutline(blIs);}			//. �ܰ� �׵θ��� �׸��ǰ�. ���ǰ�.
	virtual bool							GetIsOutline(){return m_ctrlClientWZ.GetIsOutline();}				//. �ܰ� �׵θ� ��� ���� Ȯ��.
	virtual void							SetOutlineColor(COLORREF rgb){m_ctrlClientWZ.SetOutlineColor(rgb);}	//. �ܰ� �׵θ� �� ����.
	virtual COLORREF						GetOutlineColor(){return m_ctrlClientWZ.GetOutlineColor();}			//. �ܰ� �׵θ� �� ���.	

	virtual void							SetTransparency(float fdegree);										//. 0 ~ 1.0 ���̰� �Է�. ������ �����Ѵ�.

	float                                   GetDlgX(){ return m_fDlgX;}
	float                                   GetDlgY(){ return m_fDlgY;}
	void									ReleaseDialogTexture()	{m_ctrlClientWZ.ReleaseImage();	}

    virtual void                            SetTextReducing(bool bReducing);
    virtual void                            SetTextReducing(DWORD dwCtrlID, bool bReducing);
    
    CBaseManager*                           GetCtrlManager() { return m_pcCtrlManager; }
    void                                    SetCallbackTooltip(fn_CallbackTooltip fn) { m_fnCallbackTooltip = fn; }

   

protected:
    DWORD									m_dwDialogID;
    CBaseManager*							m_pDialogManager;		//. �ڱⰡ ���ѵ� �Ŵ��� Ŭ���� ������
	CStringManager*							m_pStringManager;
	
	RECT									m_bInterSectRect;
	std::list<RECT>					        m_IntersectRectList;

private:
	CBaseManager*							m_pcCtrlManager;		//. Dialog ������ ��Ʈ�ѵ��� �����Ѵ�.
	CDialogManager*							m_pParentPtr;

	CCtrlClientWZ							m_ctrlClientWZ;

	float									m_fDlgX;
	float									m_fDlgY;

	bool									m_blLBClick;			//. ���콺 ���� ��ư Ŭ�� �߳�. ���߳�.
	//bool									m_blRBClick;
	bool									m_blMoveRgnClick;

	float									m_fLBClickX;
	float									m_fLBClickY;
	
	//float									m_fOldLBClickX;
	//float									m_fOldLBClickY;

	float									m_fMMDistanceX;
	float									m_fMMDistanceY;

	RECT									m_rtMoveRECT;			//. ������ �ٰ��� ����� �ϱ����� ������ ũ�⸦ ������ �ִٰ�. �̾��� Ŭ���� ��� �̵������ϰ� ����
																	//. Default�� Ŭ���̾�Ʈ �� �������μ��� �ȴ�.
	CDrawCtrl*								m_pDraw;				//. Draw ���� Ŭ������ �����͸� ������ �׸��� �׸���.

    SI_ToolTip*								m_pToolTipInfo;         //. ǥ���� �������� - ��Ʈ���� ������ �ִ� ���������� ���� ������ 
	POINT									m_poToolTip;

	CControlWZ*								m_pControlFocus;		//. ���� ��Ŀ���� ������ �ִ� ControlWZ ������.
	bool									m_blHasFocus;			

	//bool									m_blLockOnly;			//. �޼����� �´� ���� ��� �ִ´�.
	
	float									m_fTransparency;
	
	bool									m_blIsWebCtrl;			//. Web Control �� ��ϵǾ� �ֳ� ���� üũ.
	bool									m_blIsEditCtrl;			//. Dialog�� Edit Control�� �ֳ� ���� üũ.

    //! OnDraw ���� tooltip �������� �ݹ� ȣ��
    fn_CallbackTooltip                      m_fnCallbackTooltip;

public:
	//! Ŀ�� ��ġ(���콺 �̵��� ��� ������ ���´�.)
	static POINT                            static_mousecursor_pos;

	static STD_UI_EDIT_SORT					static_stdListEditPtr; //. Edit control���� ���� ���� �Ѵ�. 

public:
    void SetScale(float width, float height);
   
};

