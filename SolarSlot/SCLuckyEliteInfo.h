#pragma once

#ifdef __CN_1159__LUCKY_ELITE_ITEM

#include "SCSlotStruct.h"

class BitStream;

class SCLuckyEliteInfo
{	
	__PROPERTY			( WORD, wAtkDefRatio );		//���ݷ�(���� ����) ����
public:
	void		Encode( BitStream & IN bitstream ) const;
	void		Decode( BitStream & OUT bitstream );

	void		Encode( OPTIONPART * OUT pEncodedData ) const;
	void		Decode( const OPTIONPART * IN pEncodedData );

	void		Clear();

	BOOL		CalValue( SLOTCODE ItemCode );

protected:
};

#endif //< __CN_1159__LUCKY_ELITE_ITEM