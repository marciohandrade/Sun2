#include "Stdafx.h"
#include ".\SCLuckyEliteInfo.h"

#ifdef __CN_1159__LUCKY_ELITE_ITEM
#include <BitStream.hpp>
#include <ItemInfoParser.h>
#include <FateItemInfoParser.h>

void SCLuckyEliteInfo::Clear()
{	
	m_wAtkDefRatio		= 0;
}

void SCLuckyEliteInfo::Encode( BitStream & IN bitstream ) const
{	
	bitstream.Write( m_wAtkDefRatio,	9 );	
}

void SCLuckyEliteInfo::Decode( BitStream & OUT bitstream )
{
	bitstream.Read( m_wAtkDefRatio,		9 );
}

void SCLuckyEliteInfo::Encode( OPTIONPART * OUT pEncodedData ) const
{
	pEncodedData->DateTime	= m_wAtkDefRatio;
}

void SCLuckyEliteInfo::Decode( const OPTIONPART * IN pEncodedData )
{
	Clear();

	m_wAtkDefRatio	= pEncodedData->DateTime;
}

BOOL SCLuckyEliteInfo::CalValue( SLOTCODE ItemCode )
{
	BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo( ItemCode );
	if( pItemInfo == NULL )
	{
		DISPMSG( "Not Exist Item Info : %d \n", ItemCode );
		ASSERT(FALSE);
		return FALSE;
	}	

	sFATE_ITEM_INFO* pLuckyItemInfo = FateItemInfoParser::Instance()->GetFateItemInfoByRefItemCode( ItemCode );
	
	if( !pLuckyItemInfo )
	{
		SetwAtkDefRatio( 100 );
		return FALSE;
	}

	//엘리트_럭키 아이템은 랜덤이 아니라 첫번째 하나만 사용
	SetwAtkDefRatio( (WORD)dRandom(pLuckyItemInfo->m_wAtkDef_Min[0], pLuckyItemInfo->m_wAtkDef_Max[0]) );

	return TRUE;
}
#endif //< __CN_1159__LUCKY_ELITE_ITEM