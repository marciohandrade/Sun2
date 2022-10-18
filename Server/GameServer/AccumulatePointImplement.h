#ifndef SUN_GAMESERVER_ACCUMULATEPOINTIMPLEMENT_H_
#define SUN_GAMESERVER_ACCUMULATEPOINTIMPLEMENT_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

#include ".\BaseImplement.h"


/* 클래스 다이어그램 : 2010.03.03 by 남기봉

 GameScriptParserManager <*>--------------> AccumulatePointParser
                                                    A
                                                    |
                                                    |
 Player <*>----------> AccumulatePoint <>-----------|
                               A
                               |
                               |
 PlayerManager <>--------------|
                               |
                               |
 AccumulatePointImplement <m>--|
            A
            |
            |
           <*>        
 CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN or CG_ITEM_USE_ACCUMULATE_POINT_SYN

*/ 

/* class AccumulatePointImplement : 2010.03.03 by 남기봉

- Handler_CG_ITEM 에서 [생성/호출] 되며 적립포인트 사용에 대한 처리 담당.

- DropRatioInfoList.txt 에서 보상그룹코드에 해당하는 아이템 리스트를 
  가져와 클라이언트에 전송한다.

- 적립 포인트 사용시 랜덤한 아이템을 보상받고, 적립포인트를 감소 시킨다.

*/

class AccumulatePoint;

class AccumulatePointImplement : public BaseImplement
{
public:
    AccumulatePointImplement();
	virtual ~AccumulatePointImplement();

protected:
    virtual BOOL OnCheck( MSG_BASE* pRecvMessage, DWORD& OUT dwResult );
    virtual VOID OnImplement( MSG_BASE* pRecvMessage );
    virtual VOID OnFailed( DWORD dwResult );

private:
    void OnImplement( MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN* syn_message );
    void OnImplement( MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN* syn_message );

    RC::eITEM_RESULT GenerateRewardItem(uint reward_group_code, 
                                        SCItemSlot& OUT output_item, 
                                        uchar& OUT drop_item_array_order);


private:
    AccumulatePoint*    accumulate_point_;
};

#endif //_NA_1419_20100121_ACCUMULATE_POINT

#endif //SUN_GAMESERVER_ACCUMULATEPOINTIMPLEMENT_H_