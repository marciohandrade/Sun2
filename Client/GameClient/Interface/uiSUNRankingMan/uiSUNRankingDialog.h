#pragma once
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM

#include "uiBase.h"
#include "uiSUNRankingMan.h"
#include "VScrollDialog.h"
#include "DropdownControl.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

#define ANIMATION_FRAME 12
#define ANIMATION_FRAME_WIDTH 40.0f
#define ANIMATION_FRAME_HEIGHT 40.0f

const int my_ranking_background_image_index = 6573;
const int ranking_background_image_index = 6574;
const int class_texture_index[eCHAR_TYPE_MAX] = 
{
	    0,
		6565,  // 버서커
		6566,  // 드래곤나이트
		6567,  // 섀도우
		6568,  // 발키리
		6569,  // 엘리멘탈리스트
		6570,  // 미스틱
		6571,  // 헬로이드
		6572,  // 위치블레이드
};

class SUNRankingMouseWheel
{
public:
	struct AMOVEVAL
	{
		float speed_;
		float accel_;
		int   y_;
		bool  is_continue_;

		AMOVEVAL()
		{
			speed_ = 0.0f;
			accel_ = 0.0f;
			y_ = 0;
			is_continue_ = false;
		}
	};

	enum MOUSEWHEEL_STATE
	{
		MOUSEWHEEL_NONE,
		MOUSEWHEEL_DOWN,
		MOUSEWHEEL_UP,
		MOUSEWHEEL_MAX,
	};

public:
	SUNRankingMouseWheel();
	~SUNRankingMouseWheel();

	// getter
public:
	float            get_init_time_for_accel_() const { return init_time_for_accel_; }
	bool             get_is_accel_() const { return is_accel_; }
	MOUSEWHEEL_STATE get_mouse_wheel_state_() const { return mouse_wheel_state_; }

	// setter
public:
	void             set_init_time_for_accel_(const float& init_time_for_accel) { init_time_for_accel_ = init_time_for_accel; }
	void             set_is_accel_(const bool& is_accel) { is_accel_ = is_accel; }
	void             set_mouse_wheel_state_(const MOUSEWHEEL_STATE& mouse_wheel_state) { mouse_wheel_state_ = mouse_wheel_state; }

public:
	bool AccelMove(MOUSEWHEEL_STATE state, AMOVEVAL& tagVal, const float& fTimeDelta);
	void ResetMouseWheel();

private:
	float            init_time_for_accel_;
	bool             is_accel_;
	MOUSEWHEEL_STATE mouse_wheel_state_;
	int              sun_ranking_mode_;
};

struct SUN_RANKING_SCORE_INFO
{
	SCORE   score_;
	float   extra_score_info_;
	bool    is_level_value_;

	SUN_RANKING_SCORE_INFO()
	{
		score_ = 0;
		extra_score_info_ = 0.0f; // 레벨을 위한 정보
		is_level_value_ = false;
	}
};

struct SETTLE_DATE
{
	BYTE type;
	BYTE day;
	WORD time;
	TCHAR result_string[INTERFACE_STRING_LENGTH];
	bool is_initialize;
	bool is_convert;

	void ConvertTimeString()
	{
		if ( is_initialize == false )
		{
			return;
		}

		TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR day_string[INTERFACE_STRING_LENGTH] = {0, };
		TCHAR time_string[INTERFACE_STRING_LENGTH] = {0, };

		if ( type == eSUN_RANKING_SETTLE_MONTH )
		{
			// 3626 : 매월 %s일 %s
			g_InterfaceManager.GetInterfaceString(3626, format_string);
			_itoa(day, day_string, 10);
		}
		else if ( type == eSUN_RANKING_SETTLE_WEEK )
		{
			// 3627 : 매주 %s요일 %s
			g_InterfaceManager.GetInterfaceString(3627, format_string);

			// 3628 ~ : 일 월 화 수 목 금 토
			g_InterfaceManager.GetInterfaceString(3628 + day, day_string);
		}
		

		int hour = time / 100;
		int min  = time % 100;

		Snprintf(time_string, 
			INTERFACE_STRING_LENGTH-1, 
			_T("%02d:%02d"), 
			hour, 
			min);

		Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, day_string, time_string);
		
		is_convert = true;
	}

	SETTLE_DATE()
	{
		type = 0;
		day  = 0;
		time = 0;
		is_initialize = false;
		is_convert = false;
		ZeroMemory(result_string, INTERFACE_STRING_LENGTH);
	}
};

const LONG kranking_screen_view_max_slot_count = 9; // 화면에 보이는 슬롯 갯수
const BYTE kdivide_slot = 10; // 슬롯하나를 몇 등분으로 나눔

class uiSUNRankingMan;
class SUNRankingSlot;
typedef std::map<int, SUNRankingSlot*>::iterator SUNRankingSlotMapIter;
typedef std::list<SUNRankingSlot*>::iterator SUNRankingSlotListIter;
class uiSUNRankingDialog : public uiBase, public VScrollDialog
{
	enum UIControlPos
	{		
		kControl_CheckBT_C000 = 0,    // Best of Best버튼
		kControl_CheckBT_C001,        // 엘리트 전사버튼
		kControl_CheckBT_C002,        // 레벨왕 버튼
		kControl_CheckBT_C003,        // 전설적인 장비 버튼
		kControl_CheckBT_C004,        // 강력한 한방 버튼
		kControl_CheckBT_C005,        // 최고의 공격수 버튼
		kControl_CheckBT_C006,        // 치유의 달인 버튼

		kControl_Button_B000,         // 순위 버튼
		kControl_Button_B005,
		kControl_Button_B002,         // 캐릭터 명 버튼
		kControl_Button_B001,         // 점수 버튼

		kControl_Button_B012,         // x버튼

		kControl_Button_B004,         // 드롭다운 버튼
		kControl_Button_B003,         // 드롭다운 화살표 버튼
		kControl_List_L001,           // 드롭다운 리스트
		kControl_Picture_P003,        // 드롭다운 그림

		kControl_List_L000,           // 슬롯 리스트
		kControl_Picture_P008,        // 슬롯 Picture

		kControl_VScroll_V000,

		kControl_Picture_P006,        // 내 순위 슬롯
		kControl_Picture_P005,        // 1
		kControl_Picture_P010,        // 2
		kControl_Picture_P012,        // 3
		kControl_Picture_P013,        // 4
		kControl_Picture_P014,        // 5
		kControl_Picture_P015,        // 6
		kControl_Picture_P016,        // 7
		kControl_Picture_P017,        // 8

		kControl_List_L002,           // 모드 설명

		kControl_Button_B007,         // 열람실 버튼

		kControl_Size,
	};

public:
	uiSUNRankingDialog(InterfaceManager* ui_manager);
	virtual ~uiSUNRankingDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);
	virtual void NetworkProc(MSG_BASE* recv_packet);
	virtual void Process(DWORD tick);
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

private:
	bool         OnLButtonClick(SI_MESSAGE* message);
	bool         OnVScrollUp(SI_MESSAGE* message);
	bool         OnVScrollDown(SI_MESSAGE* message);
	bool         OnVScrollThumb(SI_MESSAGE* message);
	bool         OnMouseOver(SI_MESSAGE* message);

protected:
	POSTYPE      getCtrlIDToPos( WzID wzId );

protected:
	// 임시로 이렇게 정의
	virtual CCtrlVScrollWZ* vscroll_control() { return NULL; }
	virtual bool GetScrollArea(RECT& area) { return true; }

protected:
	void         RankingArticleInit();        // 랭킹 항목 탭 버튼 초기화
	void         RankingArticleButtonClick(const DWORD& position); // 랭킹 항목 탭을 클릭 했을 때
	void         DropdownClassFilterInit();
	void         set_sun_ranking_mode_(const int& sun_ranking_mode);
	//int          get_sun_ranking_mode_() const { return sun_ranking_mode_; }
	int          get_sun_ranking_mode_();
	bool         isSunRankingModeValidCheck(const int& mode);
	void         WaitPlayAnimation();
	void         InitTimer();
	void         RenderTimer(DWORD time);
	void         RenderSettleTime();
	float        CalculateExpToPercent(const LEVELTYPE& level, const EXPTYPE& accum_exp);

private:
	void         ResizeVscroll(const LONG& scroll_count); // 내 랭킹 포함.
	void         UpdateMouseWheel();
	void         OpenTutorialGuid();

public:
	void         UpdateSlotList(SunRankingInfoForClient* infos, const int& my_ranking, const SCORE& my_score);
	bool         get_is_init_() const { return is_init_; }
	void         set_is_init_(const bool& is_init) { is_init_ = is_init; }
	void         set_settle_date_(const SETTLE_DATE& settle_date);
	void         ResetSettleDate();
	void         ResetSUNRankingDialog();
	void         ResultMessageProc(const RC::eSUN_RANKING_RESULT& result_code);

private:
	static WzID wz_id_[kControl_Size];

	// getter
private:
	uiSUNRankingMan* get_ui_sun_ranking_manager_() const { return ui_sun_ranking_manager_; }
	DropdownControl& get_dropdown_class_filter_() { return dropdown_class_filter_; }

private:
	uiSUNRankingMan*                                 ui_sun_ranking_manager_;
	DropdownControl                                  dropdown_class_filter_;
	std::list<SUNRankingSlot*>                       sun_ranking_slot_view_list_[SUN_RANKING_MAX];
	CCtrlVScrollWZ*                                  vscroll_control_;
	LONG                                             move_pivot_;
	SUNRankingMouseWheel::AMOVEVAL                   moveval_;
	SUNRankingMouseWheel                             sun_ranking_mouse_wheel_;
	FTextParser                                      description_ftext_parser_;
	FTextSentence                                    description_ftext_sentence_;
	FTextParam                                       description_ftext_param_;
	FTextRender                                      description_ftext_render_;
	int                                              sun_ranking_mode_;
	HANDLE                                           wait_texture_;
	CTimerSimple                                     wait_animation_timer_;
	FRECT                                            texture_rectangles_[ANIMATION_FRAME];
	bool                                             is_init_;

	// 타이머 관련
	DWORD                                            current_time_[SUN_RANKING_MAX];

	// 정산 시간 관련
	SETTLE_DATE                                      settle_date_;
};

inline
POSTYPE uiSUNRankingDialog::getCtrlIDToPos( WzID wzId )
{
	for ( POSTYPE i = 0; i < kControl_Size; ++i )
	{
		if ( wz_id_[i] == wzId )
		{
			return i;
		}
	}
	return kControl_Size;
}

inline
int uiSUNRankingDialog::get_sun_ranking_mode_()
{
	if ( false == isSunRankingModeValidCheck(sun_ranking_mode_) )
	{
		sun_ranking_mode_ = 0;
	}

	return sun_ranking_mode_; 
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
