#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#pragma once
#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "../SUNRankingSlot.h"

class SUNRankingSlot;
class SUNRankingControl
{
public:
	SUNRankingControl(void);
	virtual ~SUNRankingControl(void);

public:
	virtual void Init(SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr) PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

protected:
	bool         is_show_() const;

protected:
	CControlWZ*  get_total_control_() const { return total_control_ptr_; }
	void         set_total_control(CControlWZ* total_control_ptr) { total_control_ptr_ = total_control_ptr; }

protected:
	SUNRankingSlot* parent_slot_;
	CControlWZ* total_control_ptr_;
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
