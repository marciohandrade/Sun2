#pragma once

#include ".\BaseImplement.h"

#ifndef _NA_000251_20100727_SOCKET_SYSTEM
class RankUpImplement : public BaseImplement
{
private:
    struct sOutput
    {
        POSTYPE RankUpItemPos;
        POSTYPE DeleteItemPos;
        MONEY   RankUpFare;
    };

public:
	RankUpImplement( void );
	virtual ~RankUpImplement( void );

    BOOLEAN DoRankUp( SCItemSlot& INOUT Equipment );

protected:
    virtual BOOL OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult );
    virtual VOID OnImplement( MSG_BASE* pRecvMessage );
    virtual VOID OnFailed( DWORD dwResult );


private:
    RC::eITEM_RESULT RankUpEquipmentAndEquipment( SCItemSlot& IN Equipment1, 
                                                SCItemSlot& IN Equipment2, 
                                                RankUpImplement::sOutput& OUT Output );
    
    RC::eITEM_RESULT RankUpEquipmentAndRequirement( SCItemSlot& IN Equipment, 
                                                    SCItemSlot& IN Requirement,
                                                    RankUpImplement::sOutput& OUT Output );


    BOOLEAN DoEquipmentRankOptionRandomChange( SCItemSlot& IN Equipment );
    BOOLEAN DoEquipmentRankUp( SCItemSlot& IN Equipment );

    RC::eITEM_RESULT CheckEnableForRankOptionChange( SCItemSlot& IN Equipment );
    RC::eITEM_RESULT CheckEnableForRankUp( SCItemSlot& IN Equipment );

private:
    sRANK_OPTION* CreateRankOption( WORD wItemType, eRANK Rank, DWORD dwExceptAttribute = 0 );
    MONEY GetRankUpPrice( SCItemSlot& IN Equipment );
    BOOLEAN IsSuccessToRankUpRatio( SCItemSlot& IN Equipment );
    BOOLEAN CreateSocketHole( SCItemSlot& ItemSlot, BYTE byCurRank );
};
#endif // _NA_000251_20100727_SOCKET_SYSTEM