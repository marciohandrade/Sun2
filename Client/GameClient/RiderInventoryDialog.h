
#pragma once

#include "uibase.h"
#include "RiderContainer.h"
#include "uiInventoryMan/uiInventoryMan.h"

//------------------------------------------------------------------------------
/**

    ���̴� Ŭ����

    @author		����

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


	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );	// ���Կ� ��ü �߰�
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	
	void			 SetInvenPos(POSTYPE pos){m_InvenPos = pos;}	
	POSTYPE			 GetInvenPos(){return m_InvenPos; }	
	
	RiderContainer* GetSlotContainer();
	void			 RenderRider();
	virtual void	 OnShowWindow(BOOL val);
	virtual VOID     Process(DWORD dwTick);
    void             UpdateData();
    
    //���÷��̿� ���̴� ����
    void             DeleteViewRider();
    
    //���÷��̿� ���̴� ��ȯ������ġ
    POSTYPE          GetRiderSummonPos();
    
    //�����Ƿκ��� ���̴�����
    void             CopySlotRider(POSTYPE pos);
    
    //���ΰ����̴��κ��� ����
    void             CopyHeroRider(Rider *pRIder);
    
    //�����Ǻ��� ��ȯ�� ���̴�Lock����
    void             UnlockSlotRider(BOOL bRemove=FALSE);
    
    //���罽���Ƿκ��ͼ�ȯ�ȶ��̴��ΰ�(�Ⱓ�������۸����)
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
    BOOL                    m_bSlotRider;       //�Ⱓ����(�������̴�����ȯ�Ǿ���?)
    POSTYPE                 m_SummmonPos;       //��ȯ����ġ  
    Rider*		            m_pDisplayRider;    //���÷��̿���̴�

    
    
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
