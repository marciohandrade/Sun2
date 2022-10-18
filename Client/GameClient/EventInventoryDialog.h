#pragma once
#include "uiBase.h"
#include <SCItemSlotContainer.h>

#define EVENT_INVENTORY_REFRESH_DELAY	1000

class ItemUnitRenderer;
class InterfaceManager;

class EventInventoryDialog : public uiBase, public SCItemSlotContainer
{
public:
	EventInventoryDialog(InterfaceManager* pUIMan);
	virtual ~EventInventoryDialog(void);

	enum PACKET_STATUS
	{
		NONE = 0,
		EVENT_PACKET_STATUS_REFRESH   = 1<<1,
		EVENT_PACKET_STATUS_MOVE_ITEM = 1<<2,
	};

	// �⺻ enum
	enum eEVENT_POS
	{
		EVENT_BT_SLOT_01 = 0,
		EVENT_BT_SLOT_02, 
		EVENT_BT_SLOT_03, 
		EVENT_BT_SLOT_04, 
		EVENT_BT_SLOT_05, 
		EVENT_BT_SLOT_06, 
		EVENT_BT_SLOT_07, 
		EVENT_BT_SLOT_08, 
		EVENT_BT_SLOT_09, 
		EVENT_BT_SLOT_10, 
		EVENT_BT_SLOT_11, 
		EVENT_BT_SLOT_12, 
		EVENT_BT_SLOT_13, 
		EVENT_BT_SLOT_14, 
		EVENT_BT_SLOT_15, 
		EVENT_BT_SLOT_16, 
		EVENT_BT_SLOT_17, 
		EVENT_BT_SLOT_18, 
		EVENT_BT_SLOT_19, 
		EVENT_BT_SLOT_20, 
		EVENT_BT_SLOT_21, 
		EVENT_BT_SLOT_22, 
		EVENT_BT_SLOT_23, 
		EVENT_BT_SLOT_24, 
		EVENT_BT_SLOT_25, 
		EVENT_BT_REFRESH,
		EVENT_BT_EXIT,

		Dialog_Max
	};

	enum 
	{
		eEVENT_SLOT_MAX = EVENT_BT_SLOT_25+1,
		
	};

	// �⺻ virtual methods
	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );

	virtual VOID			OnRenderSolarDialog();

	virtual VOID			OnShowWindow(BOOL val);
	virtual void            UpdatePosition();

	// ������ ����
	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ���Կ� ��ü �߰�
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	virtual VOID							UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot );

	virtual VOID			OnInsert( SCSlot & IN rSlotIn );
	virtual VOID			OnDelete( SCSlot & IN rSlotIn );

	// ������ ���� �޼ҵ�
public:
	VOID					OnLbuttonClickSlot(SI_MESSAGE * pMessage);
	virtual VOID			ReFreshRenderSlot();

	VOID					InitRenderer();
	VOID					FlushUnitDraw();

	virtual ItemUnitRenderer *
							GetItemUnitRender() const;
	
	// ���� �������������� ��ġ
	POSTYPE					GetRealSlotPos( POSTYPE pos );
	// �������̽� ��Ʈ�� ��ġ.
	POSTYPE					GetRelativeSlotPos( POSTYPE pos );

    DWORD					GetPacketStatus() { return this->m_packetStatus; }

	VOID					SendItemMoveMsg( POSTYPE FromPos );

	VOID					SendRequestFetchEventItem();

	// �⺻ proected methods
protected:
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

	// ��Ʈ�� ���� �Ӽ�
protected:

	// ������ ���� �Ӽ�
protected:
	POSTYPE					m_TabNum;
	POSTYPE					m_TabIndex;
	ItemUnitRenderer *		m_pItemUnitRenderer;

	util::Timer *			m_pPressRefreshDelay;
	POSTYPE					m_iPendingMovePos;

	// �⺻ attributes
private:
	static WzID				m_wzId[Dialog_Max];
	TCHAR					m_szBuf[256];
};
