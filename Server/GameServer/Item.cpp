// Item.cpp: implementation of the Item class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Item.h"

Item::Item()
{
	SetObjectType( ITEM_OBJECT );
	SetFromMonsterKey( 0 );
}

Item::~Item()
{

}

BOOL Item::Init()
{
	m_ItemSlot.Clear();
	m_Money = 0;
	m_OwnerPlayerKey = 0;
	m_FromMonsterKey = 0;

	return NonCharacter::Init();
}

VOID Item::Release()
{
	m_ItemSlot.Clear();
	m_Money = 0;
	m_OwnerPlayerKey = 0;
	m_FromMonsterKey = 0;

	NonCharacter::Release();
}

BOOL Item::Update( DWORD dwDeltaTick )
{
    if(NonCharacter::Update(dwDeltaTick) == FALSE)
        return FALSE;

	// 필드 아이템 삭제
	if( GetField() && m_DestroyTimer.IsExpired() )
	{
		return FALSE;
	}

	// 소유권 Free
	if( m_OwnershipTimer.IsExpired() )
	{
		SetOwnerPlayerKey( 0 );

		// 클라에 알려준다.
/*		MSG_CG_ITEM_FREE_OWNERSHIP_BRD brdMsg;
		brdMsg.m_dwObjectKey = GetObjectKey();
		SendPacketAround( &brdMsg,sizeof(brdMsg), FALSE );*/
	}
	return TRUE;
}

VOID Item::GetItemRenderInfo( ITEM_RENDER_INFO & OUT RenderInfo )
{
	RenderInfo.m_dwObjectKey		= GetObjectKey();
	RenderInfo.m_dwOwnerPlayerKey	= GetOwnerPlayerKey();
	if(IsEqualObjectKind(MONEY_OBJECT))
	{
		RenderInfo.m_byFieldItemType = ITEM_RENDER_INFO::eFIELDITEM_MONEY;
		RenderInfo.m_Money = GetMoney();
	}
	else
	{
		RenderInfo.m_byFieldItemType = ITEM_RENDER_INFO::eFIELDITEM_ITEM;
		GetItem( RenderInfo.m_ItemStream );
	}
	WzVector vec;
	GetPos(&vec);
	RenderInfo.m_fPos[0]		= vec.x;
	RenderInfo.m_fPos[1]		= vec.y;
	RenderInfo.m_fPos[2]		= vec.z;

#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    RenderInfo.m_dwRemainedTime = m_OwnershipTimer.GetRemainedTime();
#endif
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    RenderInfo.dicecast_item_type_ = m_ItemSlot.GetDiceCastType();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
}




