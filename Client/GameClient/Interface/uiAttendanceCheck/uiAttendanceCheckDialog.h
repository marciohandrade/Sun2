#pragma once
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiBase.h"
#include "CalendarSlot.h"
#include "FTextRender.h"

#define MAX_CALENDAR_SLOT 35    // 7 * 5

class uiAttendanceCheckDialog : public uiBase
{
    enum eFP_CONTROL_POS
    {
        eControl_Static_S003,   // 제목
        eControl_VScroll_V002,  // 스크롤
        eControl_Static_S002,  // 설명란 제목
        eControl_Static_S045,  // 설명란 텍스트?
        eControl_Picture_P009, // 달력 배경

        eControl_Static_S000,  // 일요일
        eControl_Static_S001,  // 월요일
        eControl_Static_S004,  // 화요일
        eControl_Static_S005,  // 수요일
        eControl_Static_S006,  // 목요일
        eControl_Static_S007,  // 금요일
        eControl_Static_S008,  // 토요일

        eControl_Button_B000,  // 다음달보기/현재달보기
        eControl_Button_B001,  // 보상받기
        eControl_Button_B002,  // 닫기
        eControl_Button_B086,  // x버튼

        eControl_Picture_P043,eControl_Picture_P044,eControl_Picture_P046, // 강조1열
        eControl_Picture_P047,eControl_Picture_P048,eControl_Picture_P049,
        eControl_Picture_P050,
        eControl_Picture_P051,eControl_Picture_P052,eControl_Picture_P053, // 강조2열
        eControl_Picture_P054,eControl_Picture_P055,eControl_Picture_P056,
        eControl_Picture_P057,
        eControl_Picture_P058,eControl_Picture_P059,eControl_Picture_P060, // 강조3열
        eControl_Picture_P061,eControl_Picture_P062,eControl_Picture_P063,
        eControl_Picture_P064,
        eControl_Picture_P065,eControl_Picture_P066,eControl_Picture_P067, // 강조4열
        eControl_Picture_P068,eControl_Picture_P069,eControl_Picture_P070,
        eControl_Picture_P071,
        eControl_Picture_P072,eControl_Picture_P073,eControl_Picture_P074, // 강조5열
        eControl_Picture_P075,eControl_Picture_P076,eControl_Picture_P077,
        eControl_Picture_P078,

        eControl_Picture_P045,eControl_Picture_P079,eControl_Picture_P080, //슬롯배경1열
        eControl_Picture_P081,eControl_Picture_P082,eControl_Picture_P083,
        eControl_Picture_P084,
        eControl_Picture_P085,eControl_Picture_P086,eControl_Picture_P087, //슬롯배경2열
        eControl_Picture_P088,eControl_Picture_P089,eControl_Picture_P090,
        eControl_Picture_P091,
        eControl_Picture_P092,eControl_Picture_P093,eControl_Picture_P094, //슬롯배경3열
        eControl_Picture_P095,eControl_Picture_P096,eControl_Picture_P097,
        eControl_Picture_P098,
        eControl_Picture_P099,eControl_Picture_P100,eControl_Picture_P101, //슬롯배경4열
        eControl_Picture_P102,eControl_Picture_P103,eControl_Picture_P104,
        eControl_Picture_P105,
        eControl_Picture_P106,eControl_Picture_P107,eControl_Picture_P108, //슬롯배경5열
        eControl_Picture_P109,eControl_Picture_P110,eControl_Picture_P111,
        eControl_Picture_P112,

        eControl_Static_S009,eControl_Static_S010,eControl_Static_S011, //날짜1열
        eControl_Static_S012,eControl_Static_S013,eControl_Static_S014,
        eControl_Static_S015,
        eControl_Static_S016,eControl_Static_S017,eControl_Static_S018, //날짜2열
        eControl_Static_S019,eControl_Static_S020,eControl_Static_S021,
        eControl_Static_S022,
        eControl_Static_S023,eControl_Static_S024,eControl_Static_S025, //날짜3열
        eControl_Static_S026,eControl_Static_S027,eControl_Static_S028,
        eControl_Static_S029,
        eControl_Static_S030,eControl_Static_S031,eControl_Static_S032, //날짜4열
        eControl_Static_S033,eControl_Static_S034,eControl_Static_S035,
        eControl_Static_S036,
        eControl_Static_S037,eControl_Static_S038,eControl_Static_S039, //날짜5열
        eControl_Static_S040,eControl_Static_S041,eControl_Static_S042,
        eControl_Static_S043,
 
        eControl_Picture_P113,eControl_Picture_P114,eControl_Picture_P115, //도장마크1열
        eControl_Picture_P116,eControl_Picture_P117,eControl_Picture_P118,
        eControl_Picture_P119,
        eControl_Picture_P120,eControl_Picture_P121,eControl_Picture_P122, //도장마크2열
        eControl_Picture_P123,eControl_Picture_P124,eControl_Picture_P125,
        eControl_Picture_P126,
        eControl_Picture_P127,eControl_Picture_P128,eControl_Picture_P129, //도장마크3열
        eControl_Picture_P130,eControl_Picture_P131,eControl_Picture_P132,
        eControl_Picture_P133,
        eControl_Picture_P134,eControl_Picture_P135,eControl_Picture_P136, //도장마크4열
        eControl_Picture_P137,eControl_Picture_P138,eControl_Picture_P139,
        eControl_Picture_P140,
        eControl_Picture_P143,eControl_Picture_P144,eControl_Picture_P145, //도장마크5열
        eControl_Picture_P146,eControl_Picture_P147,eControl_Picture_P148,
        eControl_Picture_P149,

        eControl_Max,
    };
public:
    uiAttendanceCheckDialog(InterfaceManager *ui_manager);
    virtual ~uiAttendanceCheckDialog(void);

    VOID            Init( CDrawBase* draw_base_ptr );
    virtual VOID    OnUpdateSolarDialog();
    virtual VOID    OnRenderSolarDialog();
    virtual VOID    Release();

    virtual VOID    MessageProc( SI_MESSAGE* message );
    virtual VOID    OnShowWindow(BOOL is_show);

    ///
    void UpdateCalendar(int year, int month); // 달력 갱신
    void UpdateCalendar();

    void set_default_month(int month) { default_month_ = month; }

private:
    void UpdateMouseWheel();
    bool OnLButtonClick(SI_MESSAGE* message);
    
    void RegisterControl();        // 컨트롤 Init()
    void InitCalendarSlot();       // 각 컨트롤과 Slot 연결
    void ReleaseCalendarSlot();    // 달력 비움

    void UpdateCalendar_Scroll(int year, int month, eATTENDANCE_TYPE type);
    void UpdateCalendar_Title(int string_id);
    void UpdateCalendar_Explanation_Title(eATTENDANCE_TYPE type);
    void UpdateCalendar_Explanation(int string_id);
    void UpdateCalendar_DayoftheWeek(eATTENDANCE_TYPE type);
    void UpdateCalendar_Button();

    // 달력의 날짜 슬롯    
    CalendarSlot calendar_slot[MAX_CALENDAR_SLOT];

    // 현재 년원일
    int current_year_;
    int current_month_;
    int default_month_;

    CCtrlVScrollWZ* scroll_control_;
    CControlWZ* calendar_control_;
    FTextRender	ftext_render_;
};
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
