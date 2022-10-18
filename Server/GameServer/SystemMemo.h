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

    //-- 메모 보내기
    void SendSystemMemo(const BasicSystemMemoInfo* system_memo_info, DWORD memo_info_list_num,
                        eSYSTEM_MEMO_TYPE system_memo_type);
    //-- 시스템 메모 보상 받기
    RC::eITEM_RESULT RewardSysMemoItem(Player* player, const MSG_BASE* recv_msg);
    //-- 시스템 메모 정보 복사
    void CopySystemMemoInfo(BasicSystemMemoInfo* dest_memo_info, 
                            const BasicSystemMemoInfo* source_memo_info);
    int GetMemoDataToINT(const TCHAR* memo_contents, WORD index);
    
private:
    //-- Default 시스템 메모 보내기
    void SendSysMemoForDefault(const BasicSystemMemoInfo* memo_info, DWORD memo_info_list_num);
    //-- 지역 점령전 시스템 메모 보내기
    void SendSysMemoForDomination(const SystemMemoInfoOfDomination* memo_info, 
        DWORD memo_info_list_num);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //-- 월드랭킹 순위권 알림 메모 보내기
    void SendSystemMemoOfWorldRanking(const SystemMemoInfoOfWorldRanking* memo_info, DWORD memo_info_list_num);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    //-- 쪽지 내 아이템 관련 정보 업데이트
    void UpdateSysMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,  bool read_stat,
        SLOTCODE reward_item_code, ULONGLONG reward_item_num);
    //-- 보상 받지 못하는 부분 롤백
    void RollBackSystemMemoRewardItemInfo(MEMOGUID memo_guid, CHARGUID char_guid,
        SLOTCODE reward_item_code, ULONGLONG reward_item_num);

};

#endif //GAMESERVER_SYSTEM_MEMO_H