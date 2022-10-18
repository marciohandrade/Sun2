#pragma once


#ifdef _NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL
#else

#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"


class uiDominationTowerManagerDlg: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_MANAGE_NOTICE_TEXT,
		DLGPOS_REMAINPOINT_TEXT,

		DLGPOS_LEFTGATE_DEC_BUTTON,
		DLGPOS_LEFTGATE_INC_BUTTON,
		DLGPOS_CENTERGATE_DEC_BUTTON,
		DLGPOS_CENTERGATE_INC_BUTTON,
		DLGPOS_RIGHTGATE_DEC_BUTTON,
		DLGPOS_RIGHTGATE_INC_BUTTON,		

        DLGPOS_POINT_ADD_BUTTON,
        DLGPOS_POINT_ADD_EFFECT,

		DLGPOS_LEFTSTAT1_PIC,
		DLGPOS_LEFTSTAT2_PIC,
		DLGPOS_LEFTSTAT3_PIC,
		DLGPOS_LEFTSTAT4_PIC,
		DLGPOS_LEFTSTAT5_PIC,

		DLGPOS_CENTERSTAT1_PIC,
		DLGPOS_CENTERSTAT2_PIC,
		DLGPOS_CENTERSTAT3_PIC,
		DLGPOS_CENTERSTAT4_PIC,
		DLGPOS_CENTERSTAT5_PIC,

		DLGPOS_RIGHTSTAT1_PIC,
		DLGPOS_RIGHTSTAT2_PIC,
		DLGPOS_RIGHTSTAT3_PIC,
		DLGPOS_RIGHTSTAT4_PIC,
		DLGPOS_RIGHTSTAT5_PIC,		

		//TODO:성문그래프추가
		DLGPOS_LEFTSTAT1_GRAPH_PIC,
		DLGPOS_LEFTSTAT2_GRAPH_PIC,
		DLGPOS_LEFTSTAT3_GRAPH_PIC,
		DLGPOS_LEFTSTAT4_GRAPH_PIC,
		DLGPOS_LEFTSTAT5_GRAPH_PIC,

		DLGPOS_CENTERSTAT1_GRAPH_PIC,
		DLGPOS_CENTERSTAT2_GRAPH_PIC,
		DLGPOS_CENTERSTAT3_GRAPH_PIC,
		DLGPOS_CENTERSTAT4_GRAPH_PIC,
		DLGPOS_CENTERSTAT5_GRAPH_PIC,

		DLGPOS_RIGHTSTAT1_GRAPH_PIC,
		DLGPOS_RIGHTSTAT2_GRAPH_PIC,
		DLGPOS_RIGHTSTAT3_GRAPH_PIC,
		DLGPOS_RIGHTSTAT4_GRAPH_PIC,
		DLGPOS_RIGHTSTAT5_GRAPH_PIC,

		DLGPOS_SETTING_BUTTON,
		DLGPOS_INIT_BUTTON,
		DLGPOS_CANCEL_BUTTON,

		DLGPOS_CLOSE_BUTTON,

		DIALOG_MAX,
	};

	enum eSTAT_POINT
	{
		STATPOINT_LEFTSTAT1,
		STATPOINT_LEFTSTAT2,
		STATPOINT_LEFTSTAT3,
		STATPOINT_LEFTSTAT4,
		STATPOINT_LEFTSTAT5,

		STATPOINT_CENTERSTAT1,
		STATPOINT_CENTERSTAT2,
		STATPOINT_CENTERSTAT3,
		STATPOINT_CENTERSTAT4,
		STATPOINT_CENTERSTAT5,

		STATPOINT_RIGHTSTAT1,
		STATPOINT_RIGHTSTAT2,
		STATPOINT_RIGHTSTAT3,
		STATPOINT_RIGHTSTAT4,
		STATPOINT_RIGHTSTAT5,

		STATPIC_MAX,
	};

	enum
	{
		eMAXPOINTBYWALL = 5,		
		eLEFT = 0,
		eCENTER = 1,
		eRIGHT = 2,
	};

public:
	uiDominationTowerManagerDlg(InterfaceManager *pUIMan);
	virtual ~uiDominationTowerManagerDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void MessageProc( SI_MESSAGE * pMessage );
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

	void SetDefenseWall(BYTE dwTotalPoint, BYTE* pDefenseWall);
    void SetAnimation();
    void AnimateEffect();
    void InitPoint(BYTE byTotalPoint = 0);
    void ShowPictureControlByPoint();

private:
	POSTYPE getCtrlIDToPos( WzID wzId );
	bool CheckPointIntergrity();	
public:

private:
	static WzID m_wzId[DIALOG_MAX];
	CCtrlStaticWZ* m_pNoticeText;
	CCtrlStaticWZ* m_pRemainText;
	CCtrlButtonWZ* m_pLeftDecButton;
	CCtrlButtonWZ* m_pLeftIncButton;
	CCtrlButtonWZ* m_pCenterDecButton;
	CCtrlButtonWZ* m_pCenterIncButton;
	CCtrlButtonWZ* m_pRightDecButton;
	CCtrlButtonWZ* m_pRightIncButton;
	// TODO:picture 컨트롤로 교체 요청
	CCtrlButtonCheckWZ* m_pStatPointPic[STATPIC_MAX];
	CCtrlPictureWZ* m_pStatGraphPic[STATPIC_MAX];
    CCtrlPictureWZ* m_pAddEffect;
    CCtrlButtonWZ* add_block_button_;
    DWORD animate_now_tick_;
    DWORD animate_current_tick_;
    DWORD animate_end_tick_;
	CCtrlButtonWZ* m_pSettingButton;
	CCtrlButtonWZ* m_pInitButton;
	CCtrlButtonWZ* m_pCancelButton;
	CCtrlButtonWZ* m_pCloseButton;

	BYTE m_byTotalPoint;
	BYTE m_byRemainPoint;
	BYTE m_byDefenseWall[MAX_DOMINATION_DEFENSEWALL_NUM];
};

inline POSTYPE uiDominationTowerManagerDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}

#endif //_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL