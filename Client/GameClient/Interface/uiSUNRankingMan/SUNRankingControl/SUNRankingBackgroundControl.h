#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingControl.h"

class SUNRankingBackgroundControl : public SUNRankingControl
{
public:
	SUNRankingBackgroundControl(void);
	virtual ~SUNRankingBackgroundControl(void);

public:
	virtual void Init(SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr);
	virtual void Update();
	virtual void Render();
	virtual void Release();
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
