//---------------------------------------------------------------------------
// CursorChangeTransact
//
//	(C) 2005 y2jinc
//---------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "cursorchangetransact.h"
#include "MouseCursor.h"


//----------------------------------------------------------------------------
/**
*/
CursorChangeTransact::CursorChangeTransact(void)
{
}


//----------------------------------------------------------------------------
/**
*/
CursorChangeTransact::~CursorChangeTransact(void)
{
	Release();
}


//----------------------------------------------------------------------------
/**
*/
void 
CursorChangeTransact::Init()
{
	this->m_PriorityMap.clear();
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Default, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Cursor_Cancle, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Default_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Default_Click, eCURSOR_PRIORITY_TYPE_MEDIUM));
    this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Open_Box, eCURSOR_PRIORITY_TYPE_MEDIUM));
    this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Open_Box_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Operating_switch, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Operating_switch_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Attack_Target, eCURSOR_PRIORITY_TYPE_MEDIUM));			
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Attack_Target_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));	
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Use, eCURSOR_PRIORITY_TYPE_MEDIUM));						
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Use_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));				
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Enter_Zone, eCURSOR_PRIORITY_TYPE_SOME_HIGH));					
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Enter_Zone_Highlight, eCURSOR_PRIORITY_TYPE_SOME_HIGH));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Waiting, eCURSOR_PRIORITY_TYPE_HIGH));					
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Conversation, eCURSOR_PRIORITY_TYPE_MEDIUM));				
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Repair, eCURSOR_PRIORITY_TYPE_HIGH));						
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Repair_Highlight, eCURSOR_PRIORITY_TYPE_HIGH));			
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Buy, eCURSOR_PRIORITY_TYPE_HIGH));						
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Buy_Highlight, eCURSOR_PRIORITY_TYPE_HIGH));				
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Sell, eCURSOR_PRIORITY_TYPE_HIGH));						
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Sell_Highlight, eCURSOR_PRIORITY_TYPE_HIGH));				
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Composite, eCURSOR_PRIORITY_TYPE_HIGH));					
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Composite_Highlight, eCURSOR_PRIORITY_TYPE_HIGH));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Magic_Scope, eCURSOR_PRIORITY_TYPE_MEDIUM));				
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Magic_Scope_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_DO_NOT_GO_TILE, eCURSOR_PRIORITY_TYPE_MEDIUM));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Waste_Basket, eCURSOR_PRIORITY_TYPE_HIGH));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Waste_Basket_Hightlight, eCURSOR_PRIORITY_TYPE_HIGH));		
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Item_Rank_Down, eCURSOR_PRIORITY_TYPE_HIGH) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Repair_Limited_Item, eCURSOR_PRIORITY_TYPE_HIGH) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Repair_Limited_Item_Hightlight, eCURSOR_PRIORITY_TYPE_HIGH) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Donation, eCURSOR_PRIORITY_TYPE_MEDIUM) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_Donation_Highlight, eCURSOR_PRIORITY_TYPE_MEDIUM) );

	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_WireHouse_In, eCURSOR_PRIORITY_TYPE_SOME_HIGH) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_WireHouse_In_Highlight, eCURSOR_PRIORITY_TYPE_SOME_HIGH) );

	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_WireHouse_Out, eCURSOR_PRIORITY_TYPE_SOME_HIGH) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_WireHouse_Out_Hightlight, eCURSOR_PRIORITY_TYPE_SOME_HIGH) );

	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_PickAxe, eCURSOR_PRIORITY_TYPE_MEDIUM) );
	this->m_PriorityMap.insert(
		CURSOR_PRIORITY_MAP_PAIR(MouseCursor::eMouseCursor_PickAxe_Hightlight, eCURSOR_PRIORITY_TYPE_MEDIUM) );

	this->m_eCursorVec.push_back(MouseCursor::eMouseCursor_Default);
}


//----------------------------------------------------------------------------
/**
*/
void 
CursorChangeTransact::Release()
{
	this->m_eCursorVec.clear();
	this->m_PriorityMap.clear();
}


//----------------------------------------------------------------------------
/**
*/
void 
CursorChangeTransact::ForceChange( MouseCursor::eMouseCursor eCursor )
{
	this->m_eCursorVec[0] = eCursor;
	MouseCursor::Instance()->SetMouseCursor( eCursor );
}


//----------------------------------------------------------------------------
/**
*/
void 
CursorChangeTransact::ChangeTranaction( MouseCursor::eMouseCursor eCursor )
{
	if ( this->m_eCursorVec[0] == eCursor )
		return;

	CURSOR_PRIORITY_MAP_ITR NewItr = this->m_PriorityMap.find( (DWORD)eCursor );
	CURSOR_PRIORITY_MAP_ITR OldItr = this->m_PriorityMap.find( this->m_eCursorVec[0] );

	if ( NewItr != this->m_PriorityMap.end() && 
		 OldItr != this->m_PriorityMap.end() )
	{
		DWORD dwOldPriority = (DWORD)OldItr->second;
		DWORD dwNewPriority = (DWORD)NewItr->second;

#if CURSOR_CHANGE_DEBUG
		if (eCursor == MouseCursor::eMouseCursor_Cursor_Cancle)
		{
			_asm nop;
		};
#endif

#if CURSOR_CHANGE_LOG
		TCHAR szText[256];
		_stprintf(szText, _T("Cursor : ( %d ), ( %d )\nPriority : ( %d ), ( %d )\n"), this->m_eCursorVec[0], eCursor, dwOldPriority, dwNewPriority);
		OutputDebugString(szText);
#endif

		if (dwNewPriority <= dwOldPriority)
		{ 
			// 우선순위가 높거나 같다.
			this->m_eCursorVec[0] = eCursor;

			MouseCursor::Instance()->SetMouseCursor( eCursor );
		}
		else
		{
			// 우선순위가 낮다.
			// 현재는 씹는다.
		}
	}
}



