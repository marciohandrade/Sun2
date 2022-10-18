#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingBackgroundControl.h"
#include "../uiSUNRankingDialog.h"

SUNRankingBackgroundControl::SUNRankingBackgroundControl(void)
{
}

SUNRankingBackgroundControl::~SUNRankingBackgroundControl(void)
{
	Release();
}

void SUNRankingBackgroundControl::Release()
{
	parent_slot_ = NULL;
}

void SUNRankingBackgroundControl::Init(SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr)
{
	parent_slot_ = parent_slot;
	total_control_ptr_ = total_control_ptr;
}

void SUNRankingBackgroundControl::Update()
{

}

void SUNRankingBackgroundControl::Render()
{
	if ( is_show_() == false )
		return;

	RECT parent_rect = parent_slot_->get_control_rect_();
	RECT origin_parent_rect = parent_rect;

	RECT total_rect = total_control_ptr_->GetSizeRect();
	RECT result_rect = parent_rect;
	bool is_bottom = false;	

	if (parent_slot_->get_index_() != 0 && parent_rect.top + parent_rect.bottom > total_rect.bottom)
	{
		is_bottom = true;
		LONG differ = parent_rect.top + parent_rect.bottom - total_rect.bottom;
		differ = parent_rect.bottom - differ;
		result_rect.bottom = differ;
	}

	if ( parent_slot_->get_index_() == 0 )
	{
		BASE_TextureListInfo* my_tex_info = NULL;
		HANDLE my_tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(my_ranking_background_image_index, my_tex_info);
		if (  (my_tex_handle != INVALID_HANDLE_VALUE) && (my_tex_info != NULL) )
		{
			g_pSunRenderer->RenderTexture(my_tex_handle,
				(float)parent_rect.left,
				(float)parent_rect.top,
				(float)parent_rect.right,
				(float)parent_rect.bottom,
				(float)my_tex_info->tex_size.left,
				(float)my_tex_info->tex_size.top,
				(float)my_tex_info->tex_size.right,
				(float)my_tex_info->tex_size.bottom);
		}
	}
	else
	{
		BASE_TextureListInfo* tex_info = NULL;
		HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(ranking_background_image_index, tex_info);
		if ( (tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL) )
		{
			if ( is_bottom == true )
			{
				float percent_result = static_cast<float>(parent_rect.bottom - result_rect.bottom);
				percent_result = percent_result / static_cast<float>(parent_rect.bottom);
				float top_gap = 0.0f;
				float bottom_gap = 0.0f;
				top_gap = tex_info->tex_size.top * percent_result;
				bottom_gap = tex_info->tex_size.bottom * percent_result;
				g_pSunRenderer->RenderTexture(tex_handle,
					(float)result_rect.left,
					(float)result_rect.top,
					(float)result_rect.right,
					(float)result_rect.bottom,
					(float)tex_info->tex_size.left,
					(float)tex_info->tex_size.top,
					(float)tex_info->tex_size.right,
					(float)tex_info->tex_size.bottom - bottom_gap);
			}
			else
			{
				g_pSunRenderer->RenderTexture(tex_handle,
					(float)parent_rect.left,
					(float)parent_rect.top,
					(float)parent_rect.right,
					(float)parent_rect.bottom,
					(float)tex_info->tex_size.left,
					(float)tex_info->tex_size.top,
					(float)tex_info->tex_size.right,
					(float)tex_info->tex_size.bottom);
			}
		}
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
