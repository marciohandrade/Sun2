#pragma once

#include "dlgbasewz.h"
#include "controlwz.h"
#include "CtrlClientWZ.h"
#include <list>

typedef std::list< CControlWZ* >	STD_UI_EDIT_SORT;
typedef std::list<RECT>				STD_UI_INTERSECT_RECT;

//------------------------------------------------------------------------------
//! Qualifier: 해당 다이얼로그이 tooltip 그려지는 시점에서 콜백을 호출한다.
//------------------------------------------------------------------------------
typedef void (*fn_CallbackTooltip)(CDialogWZ* dialog_ptr, SI_ToolTip* tooltip,int x, int y);

class CStringManager;
class CDialogManager;
class CDialogWZ : public CDlgBaseWZ 
{
public:
	//. 상속 받는 함수들.
	CDialogWZ();
	virtual ~CDialogWZ();
	virtual bool							OnCreate(DWORD dwID, DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,void* pDraw,CDialogManager* pPtr);	//Dialog클래스를 초기화 해준다.
	virtual void							OnDoAction();																	//. Action 
	virtual void							OnDraw(bool bl);
	virtual bool							OnDrawControlWZ(DWORD dwCtrlID);												//. Dialog 안에 등록된 Control를 그려준다.
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
	
	void									SetManager(CBaseManager* pPtr){m_pDialogManager = pPtr;}								//내가 속해있는 Manager PTR(포인터)를 가지고있다.
	void									SetStringManager(CStringManager* pPtr){m_pStringManager = pPtr;}

	CControlWZ*								CreateControllWZ(DWORD dwCtrlID,DWORD dwKind, DWORD dwStyle,HWND hParentWnd,HINSTANCE hInstance,RECT* rtSize,void* pDraw);

	void									SetToolTipPtr(SI_ToolTip* pToolTip){m_pToolTipInfo = pToolTip;}
	void									SetToolTipPoint(POINT point){m_poToolTip=point;}	
	
public:
	//. 외부 접근용 함수들.
	virtual void							RequestFocus(CControlWZ* pCtrl);												//. 포커스를 체크 한다.(사용 안할 경우 NULL 셋팅) 

	virtual void							OnInitialize();
	void									DisableInterSectRect(const RECT& rRT){ m_bInterSectRect = rRT; }//02/27
	std::list<RECT>&					    GetIntersectRectList()		{	return m_IntersectRectList; }
	virtual bool							InterSectRect(POINT point);														//. InterSectRect을 체크해준다.
	
	virtual DWORD							GetDlgID(){return m_dwDialogID;}
	virtual bool							SetDlgID(DWORD dwDlgID);

	virtual CControlWZ*						GetClientWZ(){return &m_ctrlClientWZ;}
	virtual	CControlWZ*						GetControlWZ(DWORD dwCtrlID);
	virtual CControlWZ*						FindControlWZ(int nIndex);														//. 인덱스에 해당하는 위치의 ControlWZ 포인터를 찾는다.

	virtual void							SetStyle(DWORD dwStyle);

    //------------------------------------------------------------------------------
    // Qualifier : 깊이가 있는다이얼로그, 통짜 이미지만 에만 적용(타일, 3등분은 적용안됨)
    // Parameter : float depth 0 ~ 1사이
    //------------------------------------------------------------------------------
    virtual void							SetDepth(float depth);

	//. Image 정보 관련 함수들
	virtual void							SetDialogImageFormat(SI_TFORMAT* pFMT);
	virtual void							SetControlImageFormat(DWORD dwCtrlID,SI_TFORMAT* pFMT);

	virtual SI_TFORMAT*						GetDialogImageFormat(DWORD dwKind = CTRL_KIND_CLIENT);
	virtual void							SetDialogPartFormat(SI_TFORMAT* pFMT, DWORD dwKind = CTRL_KIND_CLIENT);
	virtual void							ReleaseImage();	

	//. 저장/ 로드
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

	virtual DWORD							GetControlCount();													//. 등록된 컨트롤의 수

	virtual void							SetMoveRECT(RECT rtR)												//. 이동 영역 설정 박스 설정. 이 안에 클릭시 이동할수 있다.
	{
		m_rtMoveRECT.left	= rtR.left;
		m_rtMoveRECT.top	= rtR.top;
		m_rtMoveRECT.right	= rtR.right;
		m_rtMoveRECT.bottom = rtR.bottom;
	}
	virtual RECT							GetMoveRECT(){return m_rtMoveRECT;}									//. 이동 영역 얻기.

	virtual void							MoveToListIndex(int nOrig, int nDes);								//. Dialog 우선순위를 변경해준다.

	//. 외각 라인 관련 함수.
	virtual void							SetIsOutline(bool blIs){m_ctrlClientWZ.SetIsOutline(blIs);}			//. 외각 테두리를 그릴건가. 말건가.
	virtual bool							GetIsOutline(){return m_ctrlClientWZ.GetIsOutline();}				//. 외각 테두리 사용 여부 확인.
	virtual void							SetOutlineColor(COLORREF rgb){m_ctrlClientWZ.SetOutlineColor(rgb);}	//. 외각 테두리 색 설정.
	virtual COLORREF						GetOutlineColor(){return m_ctrlClientWZ.GetOutlineColor();}			//. 외각 테두리 색 얻기.	

	virtual void							SetTransparency(float fdegree);										//. 0 ~ 1.0 사이값 입력. 투명도를 셋팅한다.

	float                                   GetDlgX(){ return m_fDlgX;}
	float                                   GetDlgY(){ return m_fDlgY;}
	void									ReleaseDialogTexture()	{m_ctrlClientWZ.ReleaseImage();	}

    virtual void                            SetTextReducing(bool bReducing);
    virtual void                            SetTextReducing(DWORD dwCtrlID, bool bReducing);
    
    CBaseManager*                           GetCtrlManager() { return m_pcCtrlManager; }
    void                                    SetCallbackTooltip(fn_CallbackTooltip fn) { m_fnCallbackTooltip = fn; }

   

protected:
    DWORD									m_dwDialogID;
    CBaseManager*							m_pDialogManager;		//. 자기가 포한된 매니져 클래스 포인터
	CStringManager*							m_pStringManager;
	
	RECT									m_bInterSectRect;
	std::list<RECT>					        m_IntersectRectList;

private:
	CBaseManager*							m_pcCtrlManager;		//. Dialog 내부의 컨트롤들을 관리한다.
	CDialogManager*							m_pParentPtr;

	CCtrlClientWZ							m_ctrlClientWZ;

	float									m_fDlgX;
	float									m_fDlgY;

	bool									m_blLBClick;			//. 마우스 왼쪽 버튼 클릭 했나. 안했나.
	//bool									m_blRBClick;
	bool									m_blMoveRgnClick;

	float									m_fLBClickX;
	float									m_fLBClickY;
	
	//float									m_fOldLBClickX;
	//float									m_fOldLBClickY;

	float									m_fMMDistanceX;
	float									m_fMMDistanceY;

	RECT									m_rtMoveRECT;			//. 원도우 바같은 기능을 하기위해 일정한 크기를 가지고 있다가. 이안을 클릭할 경우 이동가능하게 설정
																	//. Default는 클라이언트 전 영역으로설정 된다.
	CDrawCtrl*								m_pDraw;				//. Draw 관련 클래스의 포인터를 가지고 그림을 그린다.

    SI_ToolTip*								m_pToolTipInfo;         //. 표시할 툴팁정보 - 컨트롤이 가지고 있는 툴팁정보에 대한 포인터 
	POINT									m_poToolTip;

	CControlWZ*								m_pControlFocus;		//. 현제 포커스를 가지고 있는 ControlWZ 포인터.
	bool									m_blHasFocus;			

	//bool									m_blLockOnly;			//. 메세지를 온니 나만 잡고 있는다.
	
	float									m_fTransparency;
	
	bool									m_blIsWebCtrl;			//. Web Control 이 등록되어 있나 없나 체크.
	bool									m_blIsEditCtrl;			//. Dialog에 Edit Control이 있나 없나 체크.

    //! OnDraw 에서 tooltip 랜더링시 콜백 호출
    fn_CallbackTooltip                      m_fnCallbackTooltip;

public:
	//! 커서 위치(마우스 이동시 계속 저장해 놓는다.)
	static POINT                            static_mousecursor_pos;

	static STD_UI_EDIT_SORT					static_stdListEditPtr; //. Edit control들을 따로 관리 한다. 

public:
    void SetScale(float width, float height);
   
};

