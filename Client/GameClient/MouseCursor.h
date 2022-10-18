#ifndef __MOUSE_CURSOR_H__
#define __MOUSE_CURSOR_H__

//=============================================================================================================================
///  시스템 마우스 커서 클래스
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 14
	@note
		- 마우스 커서 추가시 enum eMouseCursor 타입을 추가하고 Init()에 커서를 Load해야 한다.
		- 스테이터스 추가(eMouseCursor 참조)(재영)
*/
//=============================================================================================================================

#pragma once

#include <Singleton.h>

class MouseCursor : public util::Singleton<MouseCursor>
{
public:
	enum eMouseCursor
	{
		eMouseCursor_Default				  = 0, 		// 일반 커서
		eMouseCursor_Cursor_Cancle				 , 		// 일반 커서(일반커서로 돌아갈때)
		eMouseCursor_Default_Highlight			 ,		// 일반 커서 하일라이트
		eMouseCursor_Default_Click				 ,		// 일반 커서(클릭)
		eMouseCursor_Open_Box					 ,		// 상자열기 커서
		eMouseCursor_Open_Box_Highlight			 ,		// 상자열기 하일라이트 커서
		eMouseCursor_Operating_switch			 ,		// 스위치 조작 커서
		eMouseCursor_Operating_switch_Highlight  ,		// 스위치 조작 하일라이트 커서
		eMouseCursor_Attack_Target				 ,		// 칼 모양 커서
		eMouseCursor_Attack_Target_Highlight	 ,		// 칼 모양 하일라이트 커서
		eMouseCursor_Use						 ,		// 커서 사용모드
		eMouseCursor_Use_Highlight				 ,		// 커서 사용모드 하일라이트
		eMouseCursor_Enter_Zone					 ,		// 문 모양 커서
		eMouseCursor_Enter_Zone_Highlight		 ,		// 문 모양 커서 하일라이트
		eMouseCursor_Waiting					 ,		// 대기모드 커서
		eMouseCursor_Conversation				 ,		// 대화 커서
		eMouseCursor_Repair						 ,		// 망치모양 커서
		eMouseCursor_Repair_Highlight			 ,		// 망치하일라이트
		eMouseCursor_Buy						 ,		// 구입 커서
		eMouseCursor_Buy_Highlight				 ,		// 구입 하일라이트
		eMouseCursor_Sell						 ,		// 판매 커서
		eMouseCursor_Sell_Highlight				 ,		// 판매 하일라이트
		eMouseCursor_Composite					 ,		// 조합커서 (반짝이 애니)
		eMouseCursor_Composite_Highlight		 ,		// 조합하일라이트
		eMouseCursor_Magic_Scope				 ,		// 마법 사용 커서
		eMouseCursor_Magic_Scope_Highlight		 ,		// 마법 사용 하일라이트
		eMouseCursor_DO_NOT_GO_TILE				 ,		// 마법 사용 하일라이트
		eMouseCursor_Waste_Basket				 ,		// 휴지통
		eMouseCursor_Waste_Basket_Hightlight	 ,		// 휴지통 하일라이트
		eMouseCursor_Item_Rank_Down				 ,		// 아이템 랭크 다운
		eMouseCursor_Repair_Limited_Item		,		// 리미티드 아이템 수리
		eMouseCursor_Repair_Limited_Item_Hightlight		,		// 리미티드 아이템 수리 하일라이트
		eMouseCursor_Donation,							//기부 관련 커서(X_Mas)
		eMouseCursor_Donation_Highlight,					//
		eMouseCursor_WireHouse_In,		
		eMouseCursor_WireHouse_In_Highlight,
		eMouseCursor_WireHouse_Out,
		eMouseCursor_WireHouse_Out_Hightlight,
		eMouseCursor_PickAxe,						//채집관련
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