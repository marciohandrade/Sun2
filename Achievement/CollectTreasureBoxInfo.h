#ifndef __ACHIEVEMENT_COLLECTTREASUREBOXINFO_H
#define __ACHIEVEMENT_COLLECTTREASUREBOXINFO_H
#pragma once

#include <Singleton.h>
#include <chunk_stl.h>

struct Collect_Condition_Info
{
    enum {MAX_COLLECT_CONDITION = 2}; 

    WORD    condition_[MAX_COLLECT_CONDITION];
};

class CollectTreasureBoxInfo : public util::Singleton<CollectTreasureBoxInfo>
{
	typedef STLX_HASH_MAP<WORD, Collect_Condition_Info>	 COLLECT_CONDITION_MAP;    
	typedef COLLECT_CONDITION_MAP::iterator				 COLLECT_CONDITION_ITER;
	typedef std::pair<COLLECT_CONDITION_ITER, bool>		 COLLECT_CONDITION_RESULT;

public:
    CollectTreasureBoxInfo(void);
	~CollectTreasureBoxInfo(void);

    void AddCollectCondition(WORD location_code, WORD condition_first, WORD condition_second);
    Collect_Condition_Info* GetCollectCondition(WORD location_code);

private:
    COLLECT_CONDITION_MAP collect_condition_;
};

#endif //__ACHIEVEMENT_COLLECTTREASUREBOXINFO_H
