#ifndef __QUICK_CONTAINER_H__
#define __QUICK_CONTAINER_H__

//=======================================================================================================================
/** 단축창 클래스
	@author
			유재영
	@since
			
	@remarks
*/


#pragma once

#include "uiBase.h"
#include <SCItemSlotContainer.h>
#include "QuickSlotContainer.h"



class CControlWZ;
class ItemUnitRenderer;

class QuickContainer : public QuickSlotContainer, public uiBase
{
	enum eQUICK_POS
	{
        QUICK_BT00 = 0,
		QUICK_BT01,
		QUICK_BT02,
		QUICK_BT03,
		QUICK_BT04,
		QUICK_BT05,
		QUICK_BT06,
		QUICK_BT07,
		QUICK_BT08,
		QUICK_BT09,//10
		QUICK_BT10,
		QUICK_BT11,
        QUICK_BT12,
		QUICK_BT13,
		QUICK_BT14,
		QUICK_BT15,
		QUICK_BT16,
		QUICK_BT17,
		QUICK_BT18,
        QUICK_BT19,//20
        QUICK_BT20,
		QUICK_BT21,
		QUICK_BT22,
		QUICK_BT23,
		QUICK_BT24,
		QUICK_BT25,
		QUICK_BT26,
		QUICK_BT27,
		QUICK_BT28,
		QUICK_BT29,//30
		QUICK_BT30,
		QUICK_BT31,
        QUICK_BT32,
		QUICK_BT33,
		QUICK_BT34,
		QUICK_BT35,
		QUICK_BT36,
		QUICK_BT37,
		QUICK_BT38,
        QUICK_BT39,//40
		QUICK_UPBT,
		QUICK_DNBT,
        QUICK_CHANGE_SLOT,
        QUICK_BACKGROUND_SINGLE,
        QUICK_BACKGROUND_DOUBLE,

#ifdef _NA_000000_20130114_RENEWER_UI
		//exp
		QUICK_PICTURE_EXPP002,
		QUICK_PICTURE_EXPP000,

		QUICK_STATIC_TEXT009,
#else
        QUICK_NUMBER_BOTTOM_LINE,
#endif

		QUICK_MAX,
	};	

    enum { QUICK_SLOT_MAX = QUICK_BT39+1, 
           QUICK_SLOT_HALF = QUICK_BT19+1 };

    enum 
    { 
        RECTANGEL_OF_UI,
        RECTANGEL_OF_CONTROL,
        RECTANGLE_MAX    
    };

	enum { QUICK_START_SLOT = 0 };

public:
	QuickContainer();
	virtual ~QuickContainer();

	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	// 아이템 처리
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );					// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );					// 슬롯의 객체 삭제

	// 인터페이스 처리
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );	
	virtual VOID			OnShowWindow(BOOL val);

    virtual void MoveDefaultWindowPos();

	VOID					FlushUnitDraw();
	
	VOID					InitItemRender();
	ItemUnitRenderer *		GetItemUnitRender() const;

	// 실제 슬롯정보관련한 위치
	POSTYPE					GetRealSlotPos( POSTYPE pos );
	// 인터페이스 컨트롤 위치.
	POSTYPE					GetRelativeSlotPos( POSTYPE pos );
	BOOL					IsExistSameSerialItem(DBSERIAL Serial);
	BOOL					IsExistSameCodeItem( CODETYPE Code );
	POSTYPE					GetPosByCode(CODETYPE code , POSTYPE AfterPos = 0);

	VOID					ReFreshRenderSlot();

	int						GetCurTabNum() const { return m_TabNum; }

	VOID					PrevQuickTab();
	VOID					NextQuickTab();
	void					SetCurQuickSlot(int iIndex = -1);
    void                    ChangeSlotLine(bool double_size);
    void                    ToggleChangeSlotSize();
    void                    ToggleSlot();
    void                    UpdateDialogSize();
    bool double_size(){ return double_size_; }
	void					ProcessToolTip(bool bMouseOver, int tooltip_string_code);

    void UpdateEquip(SLOTIDX slot_index_prev, POSTYPE inven_pos_prev, SLOTCODE slot_code_prev,
        SLOTIDX slot_index_next, POSTYPE inven_pos_next, SLOTCODE slot_code_next); // 인벤토리 갱신 때 찾아서 있으면 위치 갱신
    void set_is_item_move(bool is_value){ is_item_move_ = is_value; }
    bool is_item_move(){ return is_item_move_; }
protected:
	virtual CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	VOID					getControlPos( POSTYPE AtPos, RECT & rectOut );

	POSTYPE					m_TabNum;
	POSTYPE					m_TabIndex;
	POSTYPE					m_PrevTabIndex;
	static WzID				m_wzId[QUICK_MAX];
    bool                    double_size_;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    CCtrlPictureWZ*			numbers_of_bottom_line_;
#endif //_NA_000000_20130114_RENEWER_UI
    CCtrlPictureWZ*			background_of_double_slot_;
    CCtrlPictureWZ*			background_of_single_slot_;
    CCtrlButtonWZ*          button_of_double_;//+
    CCtrlButtonWZ*          button_of_single_;//-
    RECT                    rectangles_[RECTANGLE_MAX];
    SI_FRECT                rectangle_of_texture_;
    SI_TFORMAT*             format_of_image_control_;
	ItemUnitRenderer *		m_pItemUnitRenderer;

     bool is_item_move_;

#ifdef _NA_000000_20130114_RENEWER_UI
	float					exp_gauge_size;
	CCtrlPictureWZ*			exp_line_;
	CCtrlStaticWZ *			Text_EXP_Info_;
	TCHAR					m_szRenderInfo[256];
	bool				    is_double_slot_;
public:
	VOID SetHeroExp(float fPercent);
	void RenderText();
 #endif
};



#endif // __QUICK_CONTAINER_H__