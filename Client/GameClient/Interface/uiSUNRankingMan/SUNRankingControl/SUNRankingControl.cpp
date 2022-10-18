#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingControl.h"

SUNRankingControl::SUNRankingControl(void) : 
parent_slot_(NULL),
total_control_ptr_(NULL)
{
}

SUNRankingControl::~SUNRankingControl(void)
{
}

bool SUNRankingControl::is_show_() const
{
	return parent_slot_->get_is_show_();
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
