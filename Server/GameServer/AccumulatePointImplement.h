#ifndef SUN_GAMESERVER_ACCUMULATEPOINTIMPLEMENT_H_
#define SUN_GAMESERVER_ACCUMULATEPOINTIMPLEMENT_H_


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

#include ".\BaseImplement.h"


/* Ŭ���� ���̾�׷� : 2010.03.03 by �����

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

/* class AccumulatePointImplement : 2010.03.03 by �����

- Handler_CG_ITEM ���� [����/ȣ��] �Ǹ� ��������Ʈ ��뿡 ���� ó�� ���.

- DropRatioInfoList.txt ���� ����׷��ڵ忡 �ش��ϴ� ������ ����Ʈ�� 
  ������ Ŭ���̾�Ʈ�� �����Ѵ�.

- ���� ����Ʈ ���� ������ �������� ����ް�, ��������Ʈ�� ���� ��Ų��.

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