#ifndef __STYLE_CONTAINER_H__
#define __STYLE_CONTAINER_H__

//----------------------------------------------------------------------------
/** ��Ÿ�� â Ŭ����
	@author
			���翵 (y2jinc@webzen.co.kr)
	@since
			
	@remarks
	-2005.08.29 �߰� 
*/


#pragma once

#include "uiBase.h"
#include <SCItemSlotContainer.h>
#include "StyleSlotContainer.h"

class CControlWZ;
class ItemUnitRenderer;

class StyleContainer : public StyleSlotContainer, public uiBase
{
	enum eSTYLE_QUICK_POS
	{
		STYLE_QUICK_BT00 = 0,
		STYLE_QUICK_BT01,
		STYLE_QUICK_BT02,
		STYLE_QUICK_BT03,
		STYLE_QUICK_MAX,
	};	

	enum 
	{ 
		STYLE_START_SLOT = 0,
		STYLE_MAX_SLOT = STYLE_QUICK_BT03+1
	};

public:
	StyleContainer();
	virtual ~StyleContainer();

	virtual VOID			Init( CDrawBase * pDrawBase );
	virtual VOID			Release();

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// ���Կ� ��ü �߰�
	virtual BOOL							DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );									// ������ ��ü ����

	// for solarDialog
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );	

	virtual VOID			OnShowWindow(BOOL val);


	VOID					FlushUnitDraw();
	VOID					InitItemRender();
	ItemUnitRenderer *		GetItemUnitRender() const;

	BOOL					IsExistSameSerialItem(DBSERIAL Serial);
	BOOL					IsExistSameCodeItem( CODETYPE ItemCode );
	VOID					ReFreshRenderSlot();

	int						GetCurTabNum() const { return m_TabNum; }

protected:

	virtual CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );
	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnRenderSolarDialog();
	VOID					getControlPos( POSTYPE AtPos, RECT & rectOut );

	static WzID				m_wzId[STYLE_QUICK_MAX];
	ItemUnitRenderer *		m_pItemUnitRenderer;
	int						m_TabNum;
	RECT					m_rcSelectCurStyle;
};

#endif // __STYLE_CONTAINER_H__