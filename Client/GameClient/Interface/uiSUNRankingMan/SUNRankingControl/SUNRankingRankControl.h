#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingControl.h"

class SUNRankingRankControl : public SUNRankingControl
{
public:
	SUNRankingRankControl(void);
	virtual ~SUNRankingRankControl(void);

public:
	virtual void Init(SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr);
	virtual void Update();
	virtual void Render();
	virtual void Release();

private:
	int MakeDigitArray(int value, BYTE* digitarray);
private:
	static HANDLE number_texture_handle_;
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
