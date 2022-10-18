#pragma once

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)

#include "Const.h"

class Player;

class PostManager
{
public:
    PostManager();
    ~PostManager();
    //-- 메모내 문자열별 데이터 얻기
    bool GetValueInMemo(const TCHAR* subject_memo, eSYSTEM_MEMO_TYPE& OUT memo_type, 
                        MAPCODE& OUT map_code);
protected:
    //-- 보상 가능 체크
    RC::eITEM_RESULT CheckRewardItem(Player* player, int amount_cost, SLOTCODE item_code,
                                     OUT int& number_of_insertable_items);
    //-- 아이템 보상
    RC::eITEM_RESULT RewardItem(Player* player, int amount_cost, SLOTCODE item_code);
    //-- 하임 보상
    bool RewardHeim(Player* player, ULONGLONG amount_cost, eSYSTEM_MEMO_TYPE memo_type);

private:
};

#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC