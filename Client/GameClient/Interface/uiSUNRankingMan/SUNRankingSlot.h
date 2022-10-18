#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM

#include "uiSUNRankingDialog.h"

class SUNRankingControl;
typedef std::list<SUNRankingControl*>::iterator SUNRankingControlIter;

class SUNRankingSlot
{
public:
	SUNRankingSlot(void);
	~SUNRankingSlot(void);

public:
	BOOL Init(CControlWZ* parent_control, CCtrlVScrollWZ* vscroll_control, const BYTE& index, const eCHAR_TYPE& character_type, const int& rank, const std::string& character_name, const SUN_RANKING_SCORE_INFO& point);
	void Update();
	void Render();
	void Release();

	// getter
public:
	RECT                   get_control_rect_() const { return control_rect_; }
	BYTE                   get_index_() const        { return index_; }
	eCHAR_TYPE             get_character_type_() const { return character_type_; }
	int                    get_rank_() const { return rank_; }
	std::string            get_character_name_() const { return character_name_; }
	SUN_RANKING_SCORE_INFO get_point_() const { return point_; }
	bool                   get_is_show_() const { return is_show_; }

	// setter
public:
	void                   set_control_rect_(const RECT& control_rect) { control_rect_ = control_rect; }
	void                   set_index_(const BYTE& index) { index_ = index; }
	void                   set_character_type_(const eCHAR_TYPE& character_type) { character_type_ = character_type; }
	void                   set_character_name_(const std::string& character_name) { character_name_ = character_name; }
	void                   set_point_(const SUN_RANKING_SCORE_INFO& point) { point_ = point; }
	void                   set_is_show_(const bool& is_show) { is_show_ = is_show; }

protected:
	CControlWZ*                   parent_control_;
	CCtrlVScrollWZ*               vscroll_control_;
	SUNRankingControl*            back_ground_;
	RECT                          control_rect_;
	BYTE                          index_;
	std::list<SUNRankingControl*> slot_controls_list_;
	eCHAR_TYPE                    character_type_;
	int                           rank_;
	std::string                   character_name_;
	SUN_RANKING_SCORE_INFO        point_;
	bool                          is_show_;

protected:
	static LONG        move_pivot_;
	static bool        is_message_;

public:
	static void        set_move_pivot_(const LONG& move_pivot) { move_pivot_ = move_pivot; }
	static LONG        get_move_pivot_() { return move_pivot_; }
	static void        set_is_message_(const bool& is_message) { is_message_ = is_message; }
	static bool        get_is_message() { return is_message_; }
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
