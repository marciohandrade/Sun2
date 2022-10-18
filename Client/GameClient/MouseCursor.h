#ifndef __MOUSE_CURSOR_H__
#define __MOUSE_CURSOR_H__

//=============================================================================================================================
///  �ý��� ���콺 Ŀ�� Ŭ����
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 14
	@note
		- ���콺 Ŀ�� �߰��� enum eMouseCursor Ÿ���� �߰��ϰ� Init()�� Ŀ���� Load�ؾ� �Ѵ�.
		- �������ͽ� �߰�(eMouseCursor ����)(�翵)
*/
//=============================================================================================================================

#pragma once

#include <Singleton.h>

class MouseCursor : public util::Singleton<MouseCursor>
{
public:
	enum eMouseCursor
	{
		eMouseCursor_Default				  = 0, 		// �Ϲ� Ŀ��
		eMouseCursor_Cursor_Cancle				 , 		// �Ϲ� Ŀ��(�Ϲ�Ŀ���� ���ư���)
		eMouseCursor_Default_Highlight			 ,		// �Ϲ� Ŀ�� ���϶���Ʈ
		eMouseCursor_Default_Click				 ,		// �Ϲ� Ŀ��(Ŭ��)
		eMouseCursor_Open_Box					 ,		// ���ڿ��� Ŀ��
		eMouseCursor_Open_Box_Highlight			 ,		// ���ڿ��� ���϶���Ʈ Ŀ��
		eMouseCursor_Operating_switch			 ,		// ����ġ ���� Ŀ��
		eMouseCursor_Operating_switch_Highlight  ,		// ����ġ ���� ���϶���Ʈ Ŀ��
		eMouseCursor_Attack_Target				 ,		// Į ��� Ŀ��
		eMouseCursor_Attack_Target_Highlight	 ,		// Į ��� ���϶���Ʈ Ŀ��
		eMouseCursor_Use						 ,		// Ŀ�� �����
		eMouseCursor_Use_Highlight				 ,		// Ŀ�� ����� ���϶���Ʈ
		eMouseCursor_Enter_Zone					 ,		// �� ��� Ŀ��
		eMouseCursor_Enter_Zone_Highlight		 ,		// �� ��� Ŀ�� ���϶���Ʈ
		eMouseCursor_Waiting					 ,		// ����� Ŀ��
		eMouseCursor_Conversation				 ,		// ��ȭ Ŀ��
		eMouseCursor_Repair						 ,		// ��ġ��� Ŀ��
		eMouseCursor_Repair_Highlight			 ,		// ��ġ���϶���Ʈ
		eMouseCursor_Buy						 ,		// ���� Ŀ��
		eMouseCursor_Buy_Highlight				 ,		// ���� ���϶���Ʈ
		eMouseCursor_Sell						 ,		// �Ǹ� Ŀ��
		eMouseCursor_Sell_Highlight				 ,		// �Ǹ� ���϶���Ʈ
		eMouseCursor_Composite					 ,		// ����Ŀ�� (��¦�� �ִ�)
		eMouseCursor_Composite_Highlight		 ,		// �������϶���Ʈ
		eMouseCursor_Magic_Scope				 ,		// ���� ��� Ŀ��
		eMouseCursor_Magic_Scope_Highlight		 ,		// ���� ��� ���϶���Ʈ
		eMouseCursor_DO_NOT_GO_TILE				 ,		// ���� ��� ���϶���Ʈ
		eMouseCursor_Waste_Basket				 ,		// ������
		eMouseCursor_Waste_Basket_Hightlight	 ,		// ������ ���϶���Ʈ
		eMouseCursor_Item_Rank_Down				 ,		// ������ ��ũ �ٿ�
		eMouseCursor_Repair_Limited_Item		,		// ����Ƽ�� ������ ����
		eMouseCursor_Repair_Limited_Item_Hightlight		,		// ����Ƽ�� ������ ���� ���϶���Ʈ
		eMouseCursor_Donation,							//��� ���� Ŀ��(X_Mas)
		eMouseCursor_Donation_Highlight,					//
		eMouseCursor_WireHouse_In,		
		eMouseCursor_WireHouse_In_Highlight,
		eMouseCursor_WireHouse_Out,
		eMouseCursor_WireHouse_Out_Hightlight,
		eMouseCursor_PickAxe,						//ä������
		eMouseCursor_PickAxe_Hightlight,

		eMouseCursor_Max
	};
public:
	MouseCursor();
	~MouseCursor();

	VOID				Init( HINSTANCE hInst );
	VOID				Release();

	VOID				Update();

	VOID				SetMouseCursor( eMouseCursor mp );
	MouseCursor::eMouseCursor 
						GetMouseCursor();

	BOOL				IsShowCursor( void ) { return m_bIsShow; }
	VOID				ShowCursor( BOOL bFlag );

private:

	HCURSOR				m_hCursor[eMouseCursor_Max];
	eMouseCursor		m_eCurCursor;

	BOOL				m_bIsShow;
};

#endif // __MOUSE_CURSOR_H__