#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "PlayerAttributes.h"
#include "uiToolTipMan/uiToolTipMan.h"

class uiCharacterStatusInfo : public uiBase
{
    enum UIControlPos
    {
        // �� ��Ʈ��
        kControl_CheckBT_C000 = 0,
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_CheckBT_C003, 
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        kControl_CheckBT_C001,
        kControl_CheckBT_C002,
        kControl_CheckBT_C032,

        // �⺻ �ɷ�ġ - ����, ���ʽ�����Ʈ, �ʱ�ȭ
        kControl_TextImage_TI14,        // ��
        kControl_Button_B000,           // �� +
        kControl_Button_B005,           // �� -
        kControl_TextImage_TI16,        // ��ø
        kControl_Button_B002,           // ��ø +
        kControl_Button_B011,           // ��ø -
        kControl_TextImage_TI18,        // ü��
        kControl_Button_B004,           // ü�� +
        kControl_Button_B012,           // ü�� -
        kControl_TextImage_TI20,        // ����
        kControl_Button_B006,           // ���� +
        kControl_Button_B013,           // ���� -
        kControl_TextImage_TI15,        // ���ŷ�
        kControl_Button_B001,           // ���ŷ� +
        kControl_Button_B014,           // ���ŷ� -
        kControl_TextImage_TI21,        // ���ʽ�����Ʈ
        kControl_Button_B015,           // ����Ʈ �����ư
        kControl_Picture_P003,          // ����Ʈ ���� ������
        kControl_TextImage_TI33,        // �ʱ�ȭ Ƚ��
        kControl_Button_B003,           // �ʱ�ȭ ��ư
        kControl_Text_ST00,             // �ʱ�ȭ �ؽ�Ʈ
        
        // ���� �ɷ�ġ - �⺻ ���ݷ�/����, ��Ÿ �ɷ���
        kControl_TextImage_TI04,        // �⺻ ���ݷ�
        kControl_TextImage_TI05,        // �߰� ���ݷ�
        kControl_TextImage_TI08,        // �⺻ ����
        kControl_TextImage_I001,        // �߰� ����
        kControl_TextImage_TI12,        // ��� ������
        kControl_TextImage_TI09,        // ���� ������
        kControl_TextImage_TI32,        // �̵��ӵ�
        kControl_TextImage_TI13,        // ���ݼӵ�
        kControl_TextImage_TI11,        // ũ��Ƽ�� Ȯ��
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_TextImage_I003,        // ũ��Ƽ�� �ǰ�Ȯ��
		kControl_TextImage_I004,        // ũ��Ƽ�� ����������
		kControl_TextImage_I005,        // ũ��Ƽ�� ����������(%)
#else
        kControl_TextImage_I000,        // ��ų ��ȭ��
#endif // _NA_008472_20150903_IMPROVED_BALANCE

        // ���� ���ݷ�/���׷�
        kControl_TextImage_TI25,        // ȭ ��
        kControl_TextImage_TI22,        // �� ��
        kControl_TextImage_TI24,        // �� ��
        kControl_TextImage_TI23,        // ǳ ��
        kControl_TextImage_TI26,        // �� ��

        kControl_TextImage_TI28,        // ȭ ��
        kControl_TextImage_TI31,        // �� ��
        kControl_TextImage_TI29,        // �� ��
        kControl_TextImage_TI30,        // ǳ ��
        kControl_TextImage_TI27,        // �� ��

#ifdef _NA_002935_20110704_ITEM_SCORE
        kControl_TextImage_I002,             // �������
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
        DWORD pressed_button_time;   //!< ��ư ó�� ���������� �ð�.

        //! ���۵����� ���°� ���ŵǾ������ ���ݱ��� �Ѱ��� �����ϱ� ���ؼ� ��� �޾� ���´�.
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
