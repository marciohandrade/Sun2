#include "SunClientPrecompiledHeader.h"
#include "MouseCursor.h"
#include "GraphicResourceList.h"

MouseCursor::MouseCursor()
	:	m_eCurCursor ( eMouseCursor_Max ),
		m_bIsShow( TRUE )
{
}

MouseCursor::~MouseCursor()
{
	Release();
}

VOID MouseCursor::Init( HINSTANCE hInst )
{
	// 기본
	RESOURCE_INFO * pInfo = GraphicResourceList::Instance()->GetItemInfo(65001);
	assert(pInfo);
	m_hCursor[eMouseCursor_Default]			= LoadCursorFromFile( pInfo->szFilename );
	m_hCursor[eMouseCursor_Cursor_Cancle]	= LoadCursorFromFile( pInfo->szFilename );

	// 기본 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65002);
	assert(pInfo);
	
	// 기본 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65002);
	assert(pInfo);
	m_hCursor[eMouseCursor_Default_Highlight]	= LoadCursorFromFile( pInfo->szFilename );
	// 기본 클릭
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65003);
	assert(pInfo);
	m_hCursor[eMouseCursor_Default_Click]		= LoadCursorFromFile( pInfo->szFilename );				
	// 상자 열기 
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65004);
	assert(pInfo);
	m_hCursor[eMouseCursor_Open_Box]			= LoadCursorFromFile( pInfo->szFilename );		
	// 상자 열기 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65005);
	assert(pInfo);
	m_hCursor[eMouseCursor_Open_Box_Highlight]	= LoadCursorFromFile( pInfo->szFilename );		
	// 스위치 
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65006);
	assert(pInfo);
	m_hCursor[eMouseCursor_Operating_switch]	= LoadCursorFromFile( pInfo->szFilename );	
	// 스위치 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65007);
	assert(pInfo);
	m_hCursor[eMouseCursor_Operating_switch_Highlight] = LoadCursorFromFile( pInfo->szFilename );
	// 공격 
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65008);
	assert(pInfo);
	m_hCursor[eMouseCursor_Attack_Target]		= LoadCursorFromFile( pInfo->szFilename );		
	// 공격 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65009);
	assert(pInfo);
	m_hCursor[eMouseCursor_Attack_Target_Highlight]	= LoadCursorFromFile( pInfo->szFilename );
	// 사용
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65010);
	assert(pInfo);
	m_hCursor[eMouseCursor_Use]					= LoadCursorFromFile( pInfo->szFilename );	
	// 사용 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65011);
	assert(pInfo);
	m_hCursor[eMouseCursor_Use_Highlight]		= LoadCursorFromFile( pInfo->szFilename );
	// Enter Zone
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65012);
	assert(pInfo);
	m_hCursor[eMouseCursor_Enter_Zone]			= LoadCursorFromFile( pInfo->szFilename );		
	// Enter Zone 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65013);
	assert(pInfo);
	m_hCursor[eMouseCursor_Enter_Zone_Highlight] = LoadCursorFromFile( pInfo->szFilename );
	// 대기모드
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65014);
	assert(pInfo);
	m_hCursor[eMouseCursor_Waiting]				= LoadCursorFromFile( pInfo->szFilename );	
	// 대기모드 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65015);
	assert(pInfo);
	m_hCursor[eMouseCursor_Conversation]		= LoadCursorFromFile( pInfo->szFilename );		
	// 수리
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65016);
	assert(pInfo);
	m_hCursor[eMouseCursor_Repair]				= LoadCursorFromFile( pInfo->szFilename );		
	// 수리 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65017);
	assert(pInfo);
	m_hCursor[eMouseCursor_Repair_Highlight]	= LoadCursorFromFile( pInfo->szFilename );
	// 구입
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65018);
	assert(pInfo);
	m_hCursor[eMouseCursor_Buy]					= LoadCursorFromFile( pInfo->szFilename );	
	// 구입 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65019);
	assert(pInfo);
	m_hCursor[eMouseCursor_Buy_Highlight]		= LoadCursorFromFile( pInfo->szFilename );		
	// 판매
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65020);
	assert(pInfo);
	m_hCursor[eMouseCursor_Sell]				= LoadCursorFromFile( pInfo->szFilename );		
	// 판매 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65021);
	assert(pInfo);
	m_hCursor[eMouseCursor_Sell_Highlight]		= LoadCursorFromFile( pInfo->szFilename );
	// 조합
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65022);
	assert(pInfo);
	m_hCursor[eMouseCursor_Composite]			= LoadCursorFromFile( pInfo->szFilename );		
	// 조합 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65023);
	assert(pInfo);
	m_hCursor[eMouseCursor_Composite_Highlight] = LoadCursorFromFile( pInfo->szFilename );
	// 마법 범위 공격
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65024);
	assert(pInfo);
	m_hCursor[eMouseCursor_Magic_Scope]			= LoadCursorFromFile( pInfo->szFilename );	
	// 마법 범위 공격 하일라이트
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65025);
	assert(pInfo);
	m_hCursor[eMouseCursor_Magic_Scope_Highlight] = LoadCursorFromFile( pInfo->szFilename );
	// 가지못하는 타일
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65026);
	assert(pInfo);
	m_hCursor[eMouseCursor_DO_NOT_GO_TILE] = LoadCursorFromFile( pInfo->szFilename );

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65027);
	assert(pInfo);
	if (pInfo)
	{
		m_hCursor[eMouseCursor_Waste_Basket] = LoadCursorFromFile( pInfo->szFilename );
	}	

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65028);
	assert(pInfo);
	if (pInfo)
	{
		m_hCursor[eMouseCursor_Waste_Basket_Hightlight] = LoadCursorFromFile( pInfo->szFilename );
	}



	pInfo = GraphicResourceList::Instance()->GetItemInfo(65029);
	assert(pInfo);
	if( pInfo )
	{
		m_hCursor[eMouseCursor_Item_Rank_Down] = LoadCursorFromFile(pInfo->szFilename);
	}


	pInfo = GraphicResourceList::Instance()->GetItemInfo(65031);
	assert(pInfo);
	if( pInfo )
		m_hCursor[eMouseCursor_Repair_Limited_Item] = LoadCursorFromFile(pInfo->szFilename);

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65032);
	assert(pInfo);
	if( pInfo )
		m_hCursor[eMouseCursor_Repair_Limited_Item_Hightlight] = LoadCursorFromFile(pInfo->szFilename);

	//기부 관련
	pInfo = GraphicResourceList::Instance()->GetItemInfo(65033);
	assert(pInfo);
	if(pInfo)
		m_hCursor[eMouseCursor_Donation] = LoadCursorFromFile(pInfo->szFilename);

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65034);
	assert(pInfo);
	if(pInfo)
		m_hCursor[eMouseCursor_Donation_Highlight] = LoadCursorFromFile(pInfo->szFilename);

	int num = 65035;
	for( int i = 0; i < 4; i++ )
	{
		pInfo = GraphicResourceList::Instance()->GetItemInfo( num + i );
		if( pInfo )
			m_hCursor[eMouseCursor_WireHouse_In + i] = LoadCursorFromFile( pInfo->szFilename );
	}

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65039);
	assert(pInfo);
	if(pInfo)
		m_hCursor[eMouseCursor_PickAxe] = LoadCursorFromFile(pInfo->szFilename);

	pInfo = GraphicResourceList::Instance()->GetItemInfo(65040);
	assert(pInfo);
	if(pInfo)
		m_hCursor[eMouseCursor_PickAxe_Hightlight] = LoadCursorFromFile(pInfo->szFilename);

	m_eCurCursor = eMouseCursor_Default;

	m_bIsShow = TRUE;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
MouseCursor::Release()
{
	for( int i = 0 ; i < eMouseCursor_Max ; ++i )
	{
		if (m_hCursor[i])
		{
			DestroyCursor( m_hCursor[i] );
			m_hCursor[i] = NULL;
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
MouseCursor::Update()
{
	if( m_eCurCursor != eMouseCursor_Max )
	{
		if( m_bIsShow )
		{
			::SetCursor( m_hCursor[m_eCurCursor] );
		}
		else
		{
			::SetCursor( NULL );
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
MouseCursor::SetMouseCursor( eMouseCursor mp )
{
	if( m_eCurCursor == mp ) return ;

	m_eCurCursor = mp;

	Update();
}


//----------------------------------------------------------------------------
/**
*/
MouseCursor::eMouseCursor	
MouseCursor::GetMouseCursor()
{
	return m_eCurCursor;
}


//----------------------------------------------------------------------------
/**
*/
VOID MouseCursor::ShowCursor( BOOL bFlag )
{
	m_bIsShow = bFlag;
	Update();
}


