#include "StdAfx.h"
#include ".\CollectTreasureBoxInfo.h"

#include <CommonDef.h>

CollectTreasureBoxInfo::CollectTreasureBoxInfo(void)
{
	collect_condition_.clear();
}

CollectTreasureBoxInfo::~CollectTreasureBoxInfo(void)
{
	collect_condition_.clear();
}

void CollectTreasureBoxInfo::AddCollectCondition(WORD location_code, WORD condition_first, WORD condition_second)
{
    if((location_code == 0) || (condition_first == 0 && condition_second == 0))
        return;

    Collect_Condition_Info condition_info;
    condition_info.condition_[0] = condition_first;
    condition_info.condition_[1] = condition_second;

    COLLECT_CONDITION_RESULT result;
    
    result = collect_condition_.insert(COLLECT_CONDITION_MAP::value_type(location_code, condition_info));

    if(!result.second)
    {
        SUNLOG(eCRITICAL_LOG, "CollectInfo.txt �� �ߺ��� AC Location ���� �����մϴ�. (location code: %d)", location_code);
        assert(!"CollectInfo.txt �����Ϳ� ������ �ֽ��ϴ�. �α׸� Ȯ���ϼ���");
    }
}

Collect_Condition_Info* CollectTreasureBoxInfo::GetCollectCondition(WORD location_code)
{
    COLLECT_CONDITION_ITER iter = collect_condition_.find(location_code);
    if(iter == collect_condition_.end())
        return NULL;

    return (Collect_Condition_Info*)&iter->second;
}