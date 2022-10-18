#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

struct BASE_GuildMarkInfo;

class uiGuildMarkCombination : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,  // 배경탭1(원형)
        kControl_CheckBT_C002,      // 배경탭2(방패)
        kControl_CheckBT_C004,      // 배경탭3(기타)

        kControl_VScroll_VS02,      // 배경 스크롤

        kControl_Button_B001,       // 배경아이콘1
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Button_B010,
        kControl_Button_B011,
        kControl_Button_B012,
        kControl_Button_B013,       // 배경아이콘10

        kControl_Button_B014,       // 배경아이콘11
        kControl_Button_B015,
        kControl_Button_B016,
        kControl_Button_B017,
        kControl_Button_B018,
        kControl_Button_B019,
        kControl_Button_B020,
        kControl_Button_B021,
        kControl_Button_B022,
        kControl_Button_B023,       // 배경아이콘20

        kControl_Button_B024,       // 배경아이콘21
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,
        kControl_Button_B028,
        kControl_Button_B029,
        kControl_Button_B030,
        kControl_Button_B031,
        kControl_Button_B032,
        kControl_Button_B033,       // 배경아이콘30


        kControl_CheckBT_C000,      // 문양탭1(동물)
        kControl_CheckBT_C003,      // 문양탭2(기호)
        kControl_CheckBT_C005,      // 문양탭3(기타)

        kControl_VScroll_V000,      // 문양스크롤

        kControl_Button_B004,       // 문양아이콘1
        kControl_Button_B034,
        kControl_Button_B035,
        kControl_Button_B036,
        kControl_Button_B037,
        kControl_Button_B038,
        kControl_Button_B039,
        kControl_Button_B040,
        kControl_Button_B041,
        kControl_Button_B042,       // 문양아이콘10

        kControl_Button_B043,       // 문양아이콘11
        kControl_Button_B044,
        kControl_Button_B045,
        kControl_Button_B046,
        kControl_Button_B047,
        kControl_Button_B048,
        kControl_Button_B049,
        kControl_Button_B050,
        kControl_Button_B051,
        kControl_Button_B052,       // 문양아이콘20

        kControl_Button_B053,       // 문양아이콘21
        kControl_Button_B054,
        kControl_Button_B055,
        kControl_Button_B056,
        kControl_Button_B057,
        kControl_Button_B058,
        kControl_Button_B059,
        kControl_Button_B060,
        kControl_Button_B061,
        kControl_Button_B062,       // 문양아이콘30

        kControl_Button_B000,       // 조합된아이콘

        kControl_Picture_P002,      // 배경 휠 영역
        kControl_Picture_P012,      // 문양 휠 영역
        
        kControl_Button_B005,       // 확인 버튼
        kControl_Button_B006,       // 취소 버튼
        kControl_Button_B999,       // 닫기 버튼

        kControl_Size,
    };

    enum
    {
        kMarkNumberByLine = 10,
        kLineNumber = 3,
        kTabNumber = 3,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMarkCombination(InterfaceManager* ui_manager);
    virtual ~uiGuildMarkCombination();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);

    void OnClickRegisterButton();
    void OnClickBackGroundTab(DWORD tab_index);
    void OnClickPatternTab(DWORD tab_index);
    void OnClickBackGroundButton(DWORD button_index);
    void OnClickPatternButton(DWORD button_index);

    void UpdateCheckTab();

    void ParseGuildMarkInfo();
    void InitMarkCombination();
    void RenderBackGroundPalette();
    void RenderPatternPalette();

    STLX_VECTOR<BASE_GuildMarkInfo*>* GetcurrentBackGroundContainer();
    STLX_VECTOR<BASE_GuildMarkInfo*>* GetcurrentPatternContainer();

    DWORD background_tab() const { return background_tab_; }
    void set_background_tab(DWORD value) { background_tab_ = value; }

    DWORD pattern_tab() const { return pattern_tab_; }
    void set_pattern_tab(DWORD value) { pattern_tab_ = value; }

    DWORD selected_background() const { return selected_background_; }
    void set_selected_background(DWORD value) { selected_background_ = value; }
    DWORD selected_pattern() const { return selected_pattern_; }
    void set_selected_pattern(DWORD value) { selected_pattern_ = value; }

    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_circle() { return mark_type_circle_; }
    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_shield() { return mark_type_shield_; }
    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_bg_etc() { return mark_type_bg_etc_; }
    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_figure() { return mark_type_figure_; }
    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_symbol() { return mark_type_symbol_; }
    STLX_VECTOR<BASE_GuildMarkInfo*>& mark_type_pt_etc() { return mark_type_pt_etc_; }

private:
    DWORD background_tab_;
    DWORD pattern_tab_;
    DWORD selected_background_;
    DWORD selected_pattern_;

    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_circle_;
    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_shield_;
    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_bg_etc_;
    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_figure_;
    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_symbol_;
    STLX_VECTOR<BASE_GuildMarkInfo*> mark_type_pt_etc_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
