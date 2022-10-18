#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "SUNRankingPointControl.h"

SUNRankingPointControl::SUNRankingPointControl(void)
{
}

SUNRankingPointControl::~SUNRankingPointControl(void)
{
	Release();
}

void SUNRankingPointControl::Release()
{

}

void SUNRankingPointControl::Init( SUNRankingSlot* parent_slot, CControlWZ* total_control_ptr )
{
	parent_slot_ = parent_slot;
	total_control_ptr_ = total_control_ptr;

	RECT rc = {0, };
	m_FTextRender.Init(g_pSunRenderer, rc);

	DWORD font_ID = StrToWzID("mn12");
	COLORREF font_color = WzColor_RGBA(255, 255, 255, 255);
	TCHAR point_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	NUMBERFMT fmt = { 0, 0, 3, _T("."), _T(","), 1 };
	SUN_RANKING_SCORE_INFO score_info = parent_slot_->get_point_();
	if ( score_info.is_level_value_ == true && parent_slot_->get_rank_() > 0 )
	{
		Snprintf(result_string, INTERFACE_STRING_LENGTH-1, _T("%I64u(%.2f%%)"), score_info.score_, score_info.extra_score_info_);
	}
	else
	{
		if ( parent_slot_->get_rank_() == 0 )
		{
			score_info.score_ = 0;
		}

		Snprintf(point_string, INTERFACE_STRING_LENGTH-1, _T("%I64u"), score_info.score_);
		::GetNumberFormat( LOCALE_SYSTEM_DEFAULT, NULL, point_string, &fmt, result_string, sizeof(result_string));
	}

	if ( parent_slot_->get_index_() == 0 )
	{
		Snprintf(point_string, 
			     INTERFACE_STRING_LENGTH-1, 
			     _T("<SHADOW=2><SDWCOLOR=000000FF><FONT=tt13><COLOR=FFFFFFFF>%s</COLOR></FONT></SDWCOLOR></SHADOW>"), 
			     result_string);
	}
	else
	{
		Snprintf(point_string, 
			     INTERFACE_STRING_LENGTH-1, 
			     _T("<SHADOW=2><SDWCOLOR=000000FF><FONT=tt13><COLOR=FFFFFF00>%s</COLOR></FONT></SDWCOLOR></SHADOW>"), 
			     result_string);
	}

	if( m_FTextParser.Parser(point_string, m_FTextSentence) )
	{
		m_FTextRender.ClearSentence();
		m_FTextRender.Append(m_FTextSentence);
		m_FTextSentence.Release();
	}
}

void SUNRankingPointControl::Update()
{

}

void SUNRankingPointControl::Render()
{
	if ( is_show_() == false )
		return;

	RECT parent_rect = parent_slot_->get_control_rect_();
	RECT origin_parent_rect = parent_rect;

	static int left = 280;
	static int top = 11;
	static int right = 40;
	static int bottom = 12;

	parent_rect.left += left;
	parent_rect.top += top;
	parent_rect.right = right;
	parent_rect.bottom -= bottom;

	static LONG text_size_rect_width = 200;
	static LONG text_top_gap = 3;
	RECT point_text_rect = parent_rect;
	point_text_rect.left = parent_rect.left + parent_rect.right + 3;
	point_text_rect.top  = parent_rect.top + text_top_gap;
	point_text_rect.right = point_text_rect.left + text_size_rect_width;
	point_text_rect.bottom = point_text_rect.top + 20;

	RECT total_rect = total_control_ptr_->GetSizeRect();
	RECT result_rect = point_text_rect;
	bool is_bottom = false;

	if (parent_slot_->get_index_() != 0 && point_text_rect.bottom > total_rect.bottom)
	{
		is_bottom = true;
		LONG differ = point_text_rect.bottom - total_rect.bottom;
		point_text_rect.bottom = (point_text_rect.bottom - point_text_rect.top) - differ;
	}

	RECT ftext_rect;
	ftext_rect = point_text_rect;
	ftext_rect.right = point_text_rect.left + point_text_rect.right;
	ftext_rect.bottom = point_text_rect.top + point_text_rect.bottom;
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
