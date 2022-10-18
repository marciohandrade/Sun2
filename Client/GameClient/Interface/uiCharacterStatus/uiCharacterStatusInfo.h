#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "PlayerAttributes.h"
#include "uiToolTipMan/uiToolTipMan.h"

class uiCharacterStatusInfo : public uiBase
{
    enum UIControlPos
    {
        // 탭 컨트롤
        kControl_CheckBT_C000 = 0,
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_CheckBT_C003, 
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        kControl_CheckBT_C001,
        kControl_CheckBT_C002,
        kControl_CheckBT_C032,

        // 기본 능력치 - 스탯, 보너스포인트, 초기화
        kControl_TextImage_TI14,        // 힘
        kControl_Button_B000,           // 힘 +
        kControl_Button_B005,           // 힘 -
        kControl_TextImage_TI16,        // 민첩
        kControl_Button_B002,           // 민첩 +
        kControl_Button_B011,           // 민첩 -
        kControl_TextImage_TI18,        // 체력
        kControl_Button_B004,           // 체력 +
        kControl_Button_B012,           // 체력 -
        kControl_TextImage_TI20,        // 지력
        kControl_Button_B006,           // 지력 +
        kControl_Button_B013,           // 지력 -
        kControl_TextImage_TI15,        // 정신력
        kControl_Button_B001,           // 정신력 +
        kControl_Button_B014,           // 정신력 -
        kControl_TextImage_TI21,        // 보너스포인트
        kControl_Button_B015,           // 포인트 적용버튼
        kControl_Picture_P003,          // 포인트 적용 깜박이
        kControl_TextImage_TI33,        // 초기화 횟수
        kControl_Button_B003,           // 초기화 버튼
        kControl_Text_ST00,             // 초기화 텍스트
        
        // 종합 능력치 - 기본 공격력/방어력, 기타 능력지
        kControl_TextImage_TI04,        // 기본 공격력
        kControl_TextImage_TI05,        // 추가 공격력
        kControl_TextImage_TI08,        // 기본 방어력
        kControl_TextImage_I001,        // 추가 방어력
        kControl_TextImage_TI12,        // 방어 성공률
        kControl_TextImage_TI09,        // 공격 성공률
        kControl_TextImage_TI32,        // 이동속도
        kControl_TextImage_TI13,        // 공격속도
        kControl_TextImage_TI11,        // 크리티컬 확률
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_TextImage_I003,        // 크리티컬 피격확률
		kControl_TextImage_I004,        // 크리티컬 데미지증가
		kControl_TextImage_I005,        // 크리티컬 데미지증가(%)
#else
        kControl_TextImage_I000,        // 스킬 강화력
#endif // _NA_008472_20150903_IMPROVED_BALANCE

        // 원소 공격력/저항률
        kControl_TextImage_TI25,        // 화 공
        kControl_TextImage_TI22,        // 수 공
        kControl_TextImage_TI24,        // 지 공
        kControl_TextImage_TI23,        // 풍 공
        kControl_TextImage_TI26,        // 암 공

        kControl_TextImage_TI28,        // 화 저
        kControl_TextImage_TI31,        // 수 저
        kControl_TextImage_TI29,        // 지 저
        kControl_TextImage_TI30,        // 풍 저
        kControl_TextImage_TI27,        // 암 저

#ifdef _NA_002935_20110704_ITEM_SCORE
        kControl_TextImage_I002,             // 장비평점
#endif //_NA_002935_20110704_ITEM_SCORE

        kControl_Size,
    };

    struct SimulationInfo 
    {
        SimulationInfo() { simulation_player.Initialize(); }
        ~SimulationInfo() { simulation_player.Release(); }

        cPlayerData simulation_player;
        PlayerAttributes simulation_player_attributes;
        UIControlPos pressed_button_index;
        bool is_pressed_button;
        DWORD pressed_button_time;   //!< 버튼 처음 눌렀을때의 시간.

        //! 버퍼등으로 상태가 갱신되었을경우 지금까지 한것을 적용하기 위해서 백업 받아 놓는다.
        int simulation_stat_count[eATTR_SPR + 1];

        void ResetStatCount()
        {
            ZeroMemory(simulation_stat_count, sizeof(simulation_stat_count));
        }
    };

public:
    static const DWORD kDialog_ID;

public:
    uiCharacterStatusInfo(InterfaceManager* interface_manager);
    ~uiCharacterStatusInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void RefreshSimulationPlayerAttributes();
    void ResetSimulationStatCount();
    PlayerAttributes* GetSimulationPlayerAttributes()
    {
        return &(simulation_info().simulation_player_attributes); 
    }

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	const BASE_PLAYERINFO* GetSimulation_BASE_PLAYERINFO()
	{
		return (simulation_info().simulation_player.GetCharacterInfo());
	}
	void ShowWindowRefreshSimulation();
#endif // _NA_008472_20150903_IMPROVED_BALANCE

    static void CallbackSendPacketStatusStatApply(bool is_yes);

private:
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void RefreshTab();
    void ProcessStatbutton();
    void UpdateParameter();
    void RenderTooltip();

	void RenderTooltip_StringCode(uiToolTipMan* tooltip_manager, int StringCode );
    void RenderTooltip_BaseAndCalc(uiToolTipMan* tooltip_manager, 
        int base_value, 
        int calc_value, 
        bool is_force = true,
        DWORD string_code = 0);
    
    // val > 0 WzColor_RGBA(20,255,20,255));
    // val < 0 WzColor_RGBA(20,20,255,255));
    // val = 0 WzColor_RGBA(255,255,255,255));

    void SetControlTextColor(CCtrlImageStaticWZ* control_ptr, int val, bool is_max = false);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	void SetControlReverseTextColor(CCtrlImageStaticWZ* control_ptr, int val, bool is_max = false);
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    void SetParameterCaption(UIControlPos control_position, int val);
    void SetParameterCaption(UIControlPos control_position, int calc_value, int base_value);
    void SetParameterDamage(CCtrlImageStaticWZ* control_ptr,int iMinValue,int iMaxValue);
    void SetParameterCaptionWithPercent(UIControlPos control_position, int calc_value, int base_value);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	void SetParameterCaptionWithPercentReverseTextColor(UIControlPos control_position, int calc_value, int base_value);
#endif // _NA_008472_20150903_IMPROVED_BALANCE

    SimulationInfo& simulation_info() { return simulation_info_; }

    eATTR_TYPE GetAttrTypeFromButtonPos(UIControlPos control_position);
    void UpdateSimulationPlayerAttributes(UIControlPos control_position, int add_value);
    bool IsSimulationStatChanged();
    void RefreshStatButton();
    void SEND_STATUS_STAT_APPLY_SYN();

    void ShowFreeStatInitControls(ENUM_STATEWZ show_state);

    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDown(SI_MESSAGE* message);
    bool OnLButtonUp(SI_MESSAGE* message);

    bool IsMaxParameterCaption(UIControlPos control_position, IN OUT int& calc_value);

private:
    SimulationInfo simulation_info_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM
