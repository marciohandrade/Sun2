#pragma once

#ifdef _NA_008472_20150903_IMPROVED_BALANCE

#include "uiBase.h"
#include "PlayerAttributes.h"
#include "uiToolTipMan/uiToolTipMan.h"

class uiCharacterStatusInfo2 : public uiBase
{
	enum UIControlPos
	{
		// 탭 컨트롤
		kControl_CheckBT_C000 = 0, 
		kControl_CheckBT_C003, 
		kControl_CheckBT_C001, 
		kControl_CheckBT_C002, 
		kControl_CheckBT_C032, 

		// 종합 능력치
		kControl_TextImage_TI09,     // 스킬공격력 증가(%)
		kControl_TextImage_TI12,     // 스킬 강화력
		kControl_TextImage_TI32,     // 버프시간 증가
		kControl_TextImage_TI13,     // 피해 디버프시간 감소
		kControl_TextImage_TI11,     // 가해 디버프시간 강화
		kControl_TextImage_I003,     // 스킬상태무시
		kControl_TextImage_I004,     // 최대데미지 발생확률
		kControl_TextImage_I005,     // 조합 성공률
		kControl_TextImage_I001,     // 하임 습득
		kControl_TextImage_I002,     // 경험치 습득

		kControl_Size, 
	};

public:
	static const DWORD kDialog_ID;
public:
	uiCharacterStatusInfo2(InterfaceManager* interface_manager);
	~uiCharacterStatusInfo2(void);

	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);

private:
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

	void         RefreshTab();

	void         RenderTooltip();
	void         RenderTooltip_BaseAndCalc(uiToolTipMan* tooltip_manager, 
		                                   int base_value, 
		                                   int calc_value, 
		                                   bool is_force = true, 
		                                   DWORD string_code = 0);
	void         RenderTooltip_BaseAndCalcMilisecond(uiToolTipMan* tooltip_manager, 
		                                             int base_value, 
													 int calc_value, 
													 bool is_force = true, 
													 DWORD string_code = 0);

	void         RenderTooltip_StringCode(uiToolTipMan* tooltip_manager, int StringCode);

	void         SetControlTextColor(CCtrlImageStaticWZ* control_ptr, int val, bool is_max = false);
	void         SetControlReverseTextColor(CCtrlImageStaticWZ* control_ptr, int val, bool is_max = false);
	void         SetParameterCaption(UIControlPos control_position, int val);
	void         SetParameterCaption(UIControlPos control_position, int calc_value, int base_value);
	void         SetParameterDamage(CCtrlImageStaticWZ* control_ptr, int iMinValue, int iMaxValue);
	void         SetParameterCaptionWithPercent(UIControlPos control_position, int calc_value, int base_value);
	void         SetParameterCaptionWithSecond(UIControlPos control_position, int calc_value, int base_value);
	void         SetParameterCaptionWithSecondReverse(UIControlPos control_position, int calc_value, int base_value);
	void         SetParameterCaptionWithPercentReverseTextColor(UIControlPos control_position, int calc_value, int base_value);
	void         SetParameterCaptionWithMilisecond(UIControlPos control_position, int calc_value, int base_value);

	bool         IsMaxParameterCaption(UIControlPos control_position, IN OUT int& calc_value);

	void         UpdateParameter();

	bool         OnLButtonClick(SI_MESSAGE* message);
	bool         OnLButtonDown(SI_MESSAGE* message);
	bool         OnLButtonUp(SI_MESSAGE* message);
};

#endif // _NA_008472_20150903_IMPROVED_BALANCE
