#include "StdAfx.h"
#include ".\vendorslot.h"
#include <BitStream.hpp>

VendorSlot::VendorSlot(void)
:	m_pItemSlot(NULL)
,	m_Money(0)
,	m_bLocked(FALSE)
{
}

VendorSlot::~VendorSlot(void)
{
}

VOID VendorSlot::Copy( SCSlot & IN slot )
{
	SCSlot::Copy(slot);

	VendorSlot & rVendorSlot = (VendorSlot & )slot;
	m_pItemSlot = rVendorSlot.GetLinkItem();
	m_Money = rVendorSlot.GetMoney();
	m_bLocked = rVendorSlot.m_bLocked;
}

VOID VendorSlot::Create( SCItemSlot & IN slot, const MONEY & money )
{
	// 인벤토리의 아이템에 락을 건다!!!!
	SetLinkItem( &slot );
	m_Money = money;
	this->SetNum( slot.GetNum() );
	m_bLocked = FALSE;
}
VOID VendorSlot::CopyOut(VENDOR_ITEMSLOTEX & OUT Info)
{
	ASSERT(GetLinkItem());
	Info.m_Money = GetMoney();
	Info.m_bLocked = (BYTE)IsLocked();
	Info.m_Stream.m_Pos = GetPos();
	GetLinkItem()->CopyOut(Info.m_Stream.m_Stream);
	if( !GetLinkItem()->WithDura() )	//< 실제 판매 개수를 셋팅함
		Info.m_Stream.m_Stream.Part.byDura = GetNum();
}

void VendorSlot::Copy( BitStream & IN bitstream )
{
	bitstream.Read(&m_Money, 8*8 );
	bitstream.Read(&m_bLocked, 1 );
	m_pItemSlot->Copy(bitstream);	
}

void VendorSlot::CopyOut( BitStream & OUT bitstream )
{
    bitstream.Write(&m_Money, 8*8);
    bitstream.Write(&m_bLocked, 1);
	GetLinkItem()->CopyOut(bitstream);	
}

void VendorSlot::Copy( BitStream & IN stream, eSLOT_SERIALIZE eType )
{
	switch( eType )
	{
	case eSLOT_SERIALIZE_CLIENT_SAVE: Copy( stream ); break;
	//case eSLOT_SERIALIZE_RENDER_SAVE: Copy_Render( stream ); break;
	}
}

void VendorSlot::CopyOut( BitStream & OUT stream, eSLOT_SERIALIZE eType )
{
	switch( eType )
	{
	case eSLOT_SERIALIZE_CLIENT_LOAD: CopyOut( stream ); break;
	//case eSLOT_SERIALIZE_RENDER_LOAD: CopyOut_Render( stream ); break;
	}
}