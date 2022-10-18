#pragma once

#ifdef _NA_008472_20150903_IMPROVED_BALANCE

#include "uiBase.h"
#include "PlayerAttributes.h"
#include "uiToolTipMan/uiToolTipMan.h"

class uiCharacterStatusInfo2 : public uiBase
{
	enum UIControlPos
	{
		// �� ��Ʈ��
		kControl_CheckBT_C000 = 0, 
		kControl_CheckBT_C003, 
		kControl_CheckBT_C001, 
		kControl_CheckBT_C002, 
		kControl_CheckBT_C032, 

		// ���� �ɷ�ġ
		kControl_TextImage_TI09,     // ��ų���ݷ� ����(%)
		kControl_TextImage_TI12,     // ��ų ��ȭ��
		kControl_TextImage_TI32,     // �����ð� ����
		kControl_TextImage_TI13,     // ���� ������ð� ����
		kControl_TextImage_TI11,     // ���� ������ð� ��ȭ
		kControl_TextImage_I003,     // ��ų���¹���
		kControl_TextImage_I004,     // �ִ뵥���� �߻�Ȯ��
		kControl_TextImage_I005,     // ���� ������
		kControl_TextImage_I001,     // ���� ����
		kControl_TextImage_I002,     // ����ġ ����

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
