//------------------------------------------------------------------------------
/** Item

    @author
		유재영
    @since

    @remarks
        - 
*/

#pragma once
#include "object.h"
#include <SCItemSlot.h>
//#include "StructInPacket.h"


class Item
	: public Object
{
private:
	ITEM_RENDER_INFO::eFIELDITEMTYPE m_ItemType;			// ITEM_RENDER_INFO::eFIELDITEMTYPE 참조

	BOOL			m_bIsSendPickPacket;

	BOOL            m_bHide;
	DWORD           m_dwMonsterID;           // 이 몬스터가 살아있는 동안은 드롭하지않는다.

	BOOL            m_bOpened;
	DWORD           m_dwOpenDelay;
	MONEY           m_dwMoney;

	SCItemSlot		m_ItemSlot;
	DWORD			m_dwOwnerKey;

	DWORD	m_dwDisplayTime;
	DWORD	m_dwHideTick;
	BOOL	m_bProcessTick;
	BOOL	m_bDisappear;
	BYTE	m_Alpha;
	BYTE	m_BackgroundAlpha;

    bool    is_can_get_item_;
    DWORD   expire_time_for_owner_;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    bool    is_dice_item_;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

public:
	Item();
	virtual ~Item();

	virtual BOOL	Create(DWORD dwObjectKey, DWORD dwID );
	virtual BOOL	Process( DWORD dwTick );
	virtual BOOL	ProcessInterpolation( DWORD dwTick );	

	virtual BOOL	Render();
	virtual void	RenderInfo( bool bMouseOver );

	BOOL            IsMoney() { return m_dwMoney > 0; }
	SCItemSlot&     GetItemSlot() { return m_ItemSlot; }
	WzID			GetCurrentDropAnimation();

	void			SetDisplayTime( BOOL bProcess = FALSE );

	BOOL            IsOpened() { return m_bOpened; }

	void			SetItemType( ITEM_RENDER_INFO::eFIELDITEMTYPE Type ) { m_ItemType = Type; }
	ITEM_RENDER_INFO::eFIELDITEMTYPE GetItemType() { return m_ItemType; }

	void			SetSendPickPacket( BOOL bSendPacket );
	BOOL			GetSendPickPacket();

	DWORD			GetOwner() const { return m_dwOwnerKey; }
	void			SetOwner( DWORD dwOwnerKey) { m_dwOwnerKey = dwOwnerKey; }

	BOOL            IsHideItem() { return m_bHide; }
	void            HideItem() { m_bHide = TRUE; }


	void            SetDropMonsterID(DWORD dwID) { m_dwMonsterID = dwID; }

	WzID			GetCurrentIdleAnimation();
	WzID			GetCurrentOpenAnimation();

	void            Open();

	void			SetItem( const ITEMSTREAMEX & ItemStream);
	void            SetMoney( MONEY dwMoney ) { m_dwMoney = dwMoney; }

	BOOL			SetPosition( const WzVector& wvPos, int iTileIndex = -1);
    bool            is_can_get_item() {return is_can_get_item_;}
    void            set_is_can_get_item(bool is_can_get) {is_can_get_item_ = is_can_get;}
    void            set_expire_time_for_owner(DWORD remain_time);
    bool            UpdateOwner();

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    bool            is_dice_item(){ return is_dice_item_; }
    void            set_is_dice_item(bool is_dice_item){ is_dice_item_ = is_dice_item; }
    void            SetDiceAppearance(APPEARANCEINFO* appearance_info);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
private:

	DWORD			_getItemNameRGB(const char* szNumber);
};
