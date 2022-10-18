#pragma once
#ifndef GAMESERVER_SYSTEM_MEMO_H
#define GAMESERVER_SYSTEM_MEMO_H

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION 

#include "PostManager.h"

class SystemMemo : public PostManager
{
public:
    SystemMemo();
    ~SystemMemo();

    //-- �޸� ������
    void SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, DWORD memo_info_list_num,
                        eSYSTEM_MEMO_TYPE system_memo_type);
    //-- �ý��� �޸� ���� �ޱ�
    RC::eITEM_RESULT RewardSysMemoItem(Player* player, const MSG_BASE* recv_msg);
    //-- �ý��� �޸� ���� ����
    void CopySystemMemoInfo(BasicSystemMemoInfo* dest_memo_info, 
                            const BasicSystemMemoInfo* source_memo_info);
    int GetMemoDataToINT(const TCHAR* memo_contents, WORD index);
    
private:
    //-- Default �ý��� �޸� ������
    void SendSysMemoForDefault(const BasicSystemMemoInfo* memo_info, DWORD memo_info_list_num);
    //-- ���� ������ �ý��� �޸� ������
    void SendSysMemoForDomination(const SystemMemoInfoOfDomination* memo_info, 
        DWORD memo_info_list_num);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //-- ���巩ŷ ������ �˸� �޸� ������
    void SendSystemMemoOfWorldRanking(const SystemMemoInfoOfWorldRanking* memo_info, DWORD memo_info_list_num);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    //-- ���� �� ������ ���� ���� ������Ʈ
    void UpdateSysMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,  bool read_stat,
        SLOTCODE reward_item_code, ULONGLONG reward_item_num);
    //-- ���� ���� ���ϴ� �κ� �ѹ�
    void RollBackSystemMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,
        SLOTCODE reward_item_code, ULONGLONG reward_item_num);

};

#endif //GAMESERVER_SYSTEM_MEMO_H