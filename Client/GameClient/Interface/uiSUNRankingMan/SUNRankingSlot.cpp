#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingSlot.h"
#include "SUNRankingControl/SUNRankingBackgroundControl.h"
#include "SUNRankingControl/SUNRankingRankControl.h"
#include "SUNRankingControl/SUNRankingCharacterNameControl.h"
#include "SUNRankingControl/SUNRankingPointControl.h"
#include "uiSUNRankingDialog.h"

LONG SUNRankingSlot::move_pivot_ = 0;
bool SUNRankingSlot::is_message_ = true;

SUNRankingSlot::SUNRankingSlot(void) : 
parent_control_(NULL),
back_ground_(NULL),
vscroll_control_(NULL),
is_show_(true)
{
}

SUNRankingSlot::~SUNRankingSlot(void)
{
	Release();
}

void SUNRankingSlot::Release()
{
	SAFE_DELETE(back_ground_);

	for ( SUNRankingControlIter iter = slot_controls_list_.begin(); 
		iter != slot_controls_list_.end(); 
		)
	{
		delete (*iter);
		(*iter) = NULL;
		iter = slot_controls_list_.erase(iter);
	}
	slot_controls_list_.clear();
}

BOOL SUNRankingSlot::Init(CControlWZ* parent_control, 
						  CCtrlVScrollWZ* vscroll_control,
						  const BYTE& index, 
						  const eCHAR_TYPE& character_type, 
						  const int& rank, 
						  const std::string& character_name, 
						  const SUN_RANKING_SCORE_INFO& point )
{
	parent_control_  = parent_control;
	vscroll_control_ = vscroll_control;
	if ( parent_control_ == NULL )
	{
		return FALSE;
	}

	index_ = index;
	character_type_ = character_type;
	rank_           = rank;
	character_name_ = character_name;
	point_          = point;

	// parent의 전체 사이즈를 얻어온다.
	RECT rect = parent_control_->GetSizeRect();
	control_rect_.top    = rect.top;
	control_rect_.left   = rect.left;
	control_rect_.right  = (LONG)parent_control_->GetSizeWidth();
	control_rect_.bottom = (LONG)(parent_control_->GetSizeHeight() / 11);
	control_rect_.right -= 14; // VScrollControll정도를 빼줌

	back_ground_ = new SUNRankingBackgroundControl;
	back_ground_->Init(this, parent_control_);

	SUNRankingControl* sun_ranking_control = new SUNRankingRankControl;
	sun_ranking_control->Init(this, parent_control_);
	slot_controls_list_.push_back(sun_ranking_control);
	sun_ranking_control = new SUNRankingCharacterNameControl;
	sun_ranking_control->Init(this, parent_control_);
	slot_controls_list_.push_back(sun_ranking_control);
	sun_ranking_control = new SUNRankingPointControl;
	sun_ranking_control->Init(this, parent_control_);
	slot_controls_list_.push_back(sun_ranking_control);

	return TRUE;
}

void SUNRankingSlot::Update()
{
	// parent의 전체 사이즈를 얻어온다.
	RECT rect = parent_control_->GetSizeRect();
	control_rect_.top    = rect.top;
	control_rect_.left   = rect.left;
	control_rect_.right  = (LONG)parent_control_->GetSizeWidth();
	control_rect_.bottom = (LONG)(parent_control_->GetSizeHeight() / kranking_screen_view_max_slot_count);
	if ( get_index_() != 0 )
	{
		control_rect_.right -= 15;
	}

	if ( index_ != 0)
	{
		control_rect_.top = (control_rect_.top + (index_ * control_rect_.bottom)) - (vscroll_control_->GetScrollPosWZ() * move_pivot_);
		if ( get_is_message() == true )
		{
			if ( control_rect_.top + control_rect_.bottom < rect.top + control_rect_.bottom )
			{
				is_show_ = false;
			}
			else if ( control_rect_.top > rect.bottom )
			{
				is_show_ = false;
			}
			else
			{
				is_show_ = true;
			}
		}
	}

	if ( back_ground_ )
	{
		back_ground_->Update();
	}

	for ( SUNRankingControlIter iter = slot_controls_list_.begin(); 
		  iter != slot_controls_list_.end(); 
		  ++iter )
	{
		(*iter)->Update();
	}
}

void SUNRankingSlot::Render()
{
	if ( back_ground_ )
	{
		back_ground_->Render();
	}

	for ( SUNRankingControlIter iter = slot_controls_list_.begin(); 
		iter != slot_controls_list_.end(); 
		++iter )
	{
		(*iter)->Render();
	}
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
