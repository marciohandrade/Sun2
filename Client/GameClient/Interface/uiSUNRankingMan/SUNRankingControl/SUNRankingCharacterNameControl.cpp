#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingCharacterNameControl.h"
#include "../uiSUNRankingDialog.h"

SUNRankingCharacterNameControl::SUNRankingCharacterNameControl(void)
{
}

SUNRankingCharacterNameControl::~SUNRankingCharacterNameControl(void)
{
	Release();
}

void SUNRankingCharacterNameControl::Release()
{

}

void SUNRankingCharacterNameControl::Init( SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr )
{
	parent_slot_ = parent_slot;
	total_control_ptr_ = total_control_ptr;

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);
	DWORD font_ID = StrToWzID("mn12");
	COLORREF font_color = WzColor_RGBA(255, 255, 0, 255);

	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	if ( parent_slot_->get_index_() == 0 )
	{
		Snprintf(result_string, 
			     INTERFACE_STRING_LENGTH-1, 
				 _T("<SHADOW=2><SDWCOLOR=000000FF><FONT=tt13><COLOR=FFFFFFFF>%s</COLOR></FONT></SDWCOLOR></SHADOW>"), 
				 parent_slot_->get_character_name_().c_str());
	}
	else
	{
		Snprintf(result_string, 
			     INTERFACE_STRING_LENGTH-1, 
				 _T("<SHADOW=2><SDWCOLOR=000000FF><FONT=tt13><COLOR=FFFFFF00>%s</COLOR></FONT></SDWCOLOR></SHADOW>"), 
				 parent_slot_->get_character_name_().c_str());
	}
	
	if( m_FTextParser.Parser(result_string, m_FTextSentence) )
	{
		m_FTextRender.ClearSentence();
		m_FTextRender.Append(m_FTextSentence);
		m_FTextSentence.Release();
	}
}

void SUNRankingCharacterNameControl::Update()
{

}

void SUNRankingCharacterNameControl::Render()
{
	if ( is_show_() == false )
		return;

	BASE_TextureListInfo* tex_info = NULL;
	if ( parent_slot_->get_character_type_() >= eCHAR_TYPE_MAX )
	{
		return;
	}

	if ( class_texture_index[parent_slot_->get_character_type_()] == 0 )
	{
		return;
	}

	RECT parent_rect = parent_slot_->get_control_rect_();
	RECT origin_parent_rect = parent_rect;

	static int left = 70;
	static int top = 7;
	static int right = 35;
	static int bottom = 15;

	parent_rect.left += left;
	parent_rect.top += top;
	parent_rect.right = right;
	parent_rect.bottom -= bottom;

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

	HANDLE tex_handle = TextureListInfoHandleManager::Instance()->GetTexture(class_texture_index[parent_slot_->get_character_type_()], tex_info);
	if ( (tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL) )
	{
		if ( is_bottom == true )
		{
			float percent_result = static_cast<float>(parent_rect.bottom - result_rect.bottom);
			percent_result = percent_result / (float)(parent_rect.bottom);
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

	SIZE textsize;
	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(parent_slot_->get_character_name_().c_str(),lstrlen(parent_slot_->get_character_name_().c_str()),&textsize);
	static LONG text_size_rect_width = 200;
	static LONG text_top_gap = 7;
	static LONG texture_text_gap = 28;
	RECT character_name_rect = parent_rect;
	character_name_rect.left = parent_rect.left + parent_rect.right + texture_text_gap;
	character_name_rect.top  = parent_rect.top + text_top_gap;
	character_name_rect.right = character_name_rect.left + text_size_rect_width;
	character_name_rect.bottom = character_name_rect.top + 20;

	is_bottom = false;
	result_rect = character_name_rect;
	if (parent_slot_->get_index_() != 0 && character_name_rect.bottom > total_rect.bottom)
	{
		is_bottom = true;
		LONG differ = character_name_rect.bottom - total_rect.bottom;
		character_name_rect.bottom = (character_name_rect.bottom - character_name_rect.top) - differ;
	}

	RECT ftext_rect;
	ftext_rect = character_name_rect;
	ftext_rect.right = character_name_rect.left + character_name_rect.right;
	ftext_rect.bottom = character_name_rect.top + character_name_rect.bottom;
	// 중앙 정렬
	//ftext_rect.left = (ftext_rect.right / 2) - (textsize.cx / 2); // 중앙 정렬하면 어색하다
	bool is_differ_rect = false;
	if ( ftext_rect.left != m_FTextRender.GetRect().left )
	{
		is_differ_rect = true;
	}
	m_FTextRender.SetRect( ftext_rect );
	m_FTextRender.SetBeginLineIndex(0);
	// 이전 영역의 차이가 있으면 Render를 두번 해준다.
	if ( is_differ_rect == true )
	{
		m_FTextRender.Render();
	}
	m_FTextRender.Render();
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
