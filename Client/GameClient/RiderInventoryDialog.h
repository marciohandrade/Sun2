
#pragma once

#include "uibase.h"
#include "RiderContainer.h"
#include "uiInventoryMan/uiInventoryMan.h"

//------------------------------------------------------------------------------
/**

    라이더 클래스

    @author		광수

    @since		09.10.15

    @remarks	
*/
typedef map<int,int> USE_TOOLTIP_CTRL_EQUIP_WINDOW;

class SCItemSlot;
class Rider;
class RiderInventoryDialog: public uiBase ,public RiderContainer
{
    enum RIDER_ROTATE_TYPE
    {
        R_ROTATE_NONE=0,
        R_ROTATE_LEFT,
        R_ROTATE_RIGHT,
        MAX_BUTTON
    };


public:
	RiderInventoryDialog(InterfaceManager * pUIMan );
	~RiderInventoryDialog();

public:
	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual void	OnUpdateSolarDialog();
	virtual void	OnRenderSolarDialog();

    virtual void    ReloadRider(SCItemSlot &RiderSlot);

	virtual	ItemUnitRenderer * GetItemUnitRender() const { return m_pItemUnitRenderer; }


	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );	// 슬롯에 객체 추가
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	
	void			 SetInvenPos(POSTYPE pos){m_InvenPos = pos;}	
	POSTYPE			 GetInvenPos(){return m_InvenPos; }	
	
	RiderContainer* GetSlotContainer();
	void			 RenderRider();
	virtual void	 OnShowWindow(BOOL val);
	virtual VOID     Process(DWORD dwTick);
    void             UpdateData();
    
    //디스플레이용 라이더 제거
    void             DeleteViewRider();
    
    //디스플레이용 라이더 소환슬롯위치
    POSTYPE          GetRiderSummonPos();
    
    //슬롯의로부터 라이더복사
    void             CopySlotRider(POSTYPE pos);
    
    //주인공라이더로부터 복사
    void             CopyHeroRider(Rider *pRIder);
    
    //슬롯의부터 소환된 라이더Lock해제
    void             UnlockSlotRider(BOOL bRemove=FALSE);
    
    //현재슬롯의로부터소환된라이더인가(기간제아이템만료시)
    BOOL             IsSlotRider(){return m_bSlotRider;}
        
private:

    void            updateRotate();
    void            rotateRider(float fAngle);
    void			renderToolTip();	
	void			InitRenderer();
	void			InitControls();
	POSTYPE			getCtrlIDToPos( WzID wzId );
	CControlWZ *	getControl( POSTYPE AtPos );
	void			FlushUnitDraw();
    int	            getTooltipString(POSTYPE pos);
    
	
private:
    RIDER_ROTATE_TYPE       m_eRotateType;
    USE_TOOLTIP_CTRL_EQUIP_WINDOW	m_UseTooltipCtrls;
    int				        m_iTooltipStringCode;
	POSTYPE					m_InvenPos;
	CDrawBase *				m_pDraw;
	ItemUnitRenderer *		m_pItemUnitRenderer;
	SCItemSlot *			m_pItemToolTip;
	CCtrlButtonWZ *			m_pCtrlCloseBtn;
	CCtrlStaticWZ*			m_TxtCtrlTitle;
    CCtrlButtonWZ *			m_pRotateBtn[2];

    float                   m_fAngleZ;
    float                   m_fScale;
    BOOL                    m_bSlotRider;       //기간만료(죽은라이더가소환되었냐?)
    POSTYPE                 m_SummmonPos;       //소환된위치  
    Rider*		            m_pDisplayRider;    //디스플레이용라이더

    
    
};

//------------------------------------------------------------------------------
/**
*/
inline	POSTYPE	RiderInventoryDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < RIDER_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return RIDER_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ *	RiderInventoryDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < RIDER_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
