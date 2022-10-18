#pragma once

#include ".\BaseImplement.h"

class CrystallizeImplement : public BaseImplement
{
private:
    struct sOutput
    {
        sOutput() 
        : TotalCount(0)
        {
        }
        BYTE             TotalCount;
        SLOTCODE            ObtainCodeArray[MAX_INVENTORY_SLOT_NUM];
        BYTE             ObtainCountArray[MAX_INVENTORY_SLOT_NUM];
    };

public:
    CrystallizeImplement( void );
	virtual ~CrystallizeImplement( void );

protected:
    virtual BOOL OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult );
    virtual VOID OnImplement( MSG_BASE* pRecvMessage );
    virtual VOID OnFailed( DWORD dwResult );

private:
    RC::eITEM_RESULT Crystallize( SCItemSlot& rTargetItem, CrystallizeImplement::sOutput& OUT Output );

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
    // 결정화시킨 아이템 삭제
    VOID DeleteTargetItem(SCItemSlot& rTargetItem);
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
};

