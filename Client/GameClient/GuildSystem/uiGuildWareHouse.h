#pragma once
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiBase.h"
#include <SCItemSlotContainer.h>

class BitStream;

class uiGuildWareHouse : public uiBase, public SCItemSlotContainer
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0,   // �����۽���1
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B007,       // �����۽���5

        kControl_Button_B008,       // �����۽���6
        kControl_Button_B009,
        kControl_Button_B010,
        kControl_Button_B011,
        kControl_Button_B012,       // �����۽���10

        kControl_Button_B013,       // �����۽���11
        kControl_Button_B014,
        kControl_Button_B015,
        kControl_Button_B016,
        kControl_Button_B017,       // �����۽���15

        kControl_Button_B018,       // �����۽���16
        kControl_Button_B019,
        kControl_Button_B020,
        kControl_Button_B021,
        kControl_Button_B022,       // �����۽���20

        kControl_Button_B023,       // �����۽���21
        kControl_Button_B024,
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,       // �����۽���25

        kControl_Button_B000,       // ��� ��ư
        kControl_Button_B006,       // ���� ��ư

        kControl_CheckBT_C001,      // �ǽ���1
        kControl_CheckBT_C002,      // �ǽ���2
        kControl_CheckBT_C003,      // �ǽ���3
        kControl_CheckBT_C000,      // �ǽ���4
        kControl_CheckBT_C004,      // �ǽ���5

        kControl_CheckBT_C006,      // ������ �԰�
        kControl_CheckBT_C005,      // ������ ���

        kControl_Button_B029,       // ���� �Ա�
        kControl_Button_B030,       // ���� ���

        kControl_Button_B999,
        kControl_Button_B005,

        kControl_Text_T009,         // ������ ����
        kControl_Text_S000,         // �Աݾȳ�����
        kControl_Edit_E000,         // �����Է¿���Ʈ

		DIALOG_LOCK_PICTURE_P033,
		DIALOG_LOCK_PICTURE_P032,
		DIALOG_LOCK_PICTURE_P034,

        kControl_Size,
    };

	enum 
	{
        kMaxItemSlot = 25,
        kMaxTab = 5,
        kMaxEditLength = 19,
	};

    enum WareHouseControlFlag
    {
        kUseStart = 1, 
        kUseEnd, 
        kSavingMoney,
        // ...
    };

public:
    static const DWORD kDialog_ID;

public:
	uiGuildWareHouse(InterfaceManager* ui_manager);
	virtual ~uiGuildWareHouse();

	virtual void Init(CDrawBase* draw_base);
	virtual void Release();

	virtual void MessageProc(SI_MESSAGE* message);
	virtual void NetworkProc(MSG_BASE* recv_packet);

	virtual void OnRenderSolarDialog();
	virtual void OnUpdateSolarDialog();

	virtual void OnShowWindow(BOOL is_show);

	virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE slot_position, SCSlot& IN item_slot);
	virtual BOOL DeleteSlot(POSTYPE slot_position, SCSlot* item_slot);

	virtual void OnInsert(SCSlot& IN item_slot);
	virtual void OnDelete(SCSlot& IN item_slot);

	// ���� �������������� ��ġ
	POSTYPE GetRealSlotPos(POSTYPE slot_position);
	// �������̽� ��Ʈ�� ��ġ.
	POSTYPE GetRelativeSlotPos(POSTYPE slot_position);

	void InitRenderer();
	void FlushUnitDraw();
	void ReFreshRenderSlot();
	
	void SendWareHouseStart();
	void SendWareHouseEnd();
    void SendWareHouseMoney(BYTE money_type);

	virtual void Serialize(POSTYPE slot_position, BitStream& bitstream, eSLOT_SERIALIZE serialize_type);

    virtual	ItemUnitRenderer* GetItemUnitRender() const { return item_unit_renderer_; }

    static void ResetMouseState(DWORD l_param, DWORD r_param);

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	void HeimTradingvolumeCancel();
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

private:
    void InitMoney();
    bool CheckInputMoney(DWORD money_type);
    bool CheckTab(int select_tab);
    bool OnLButtonClick(SI_MESSAGE* message);

    void UpdateMouseState();

    STLX_SET<WareHouseControlFlag>& control_flag() { return control_flag_; }

    MONEY money() const { return money_; }
    void set_money(MONEY val) { money_ = val; }
    MONEY balance() const { return balance_; }
    void set_balance(MONEY val) { balance_ = val; }
    int current_tab() const { return current_tab_; }
    void set_current_tab(int val) { current_tab_ = val; }
    int max_tab() const { return max_tab_; }
    void set_max_tab(int val) { max_tab_ = val; }

private:
    ItemUnitRenderer* item_unit_renderer_;
    STLX_SET<WareHouseControlFlag> control_flag_;
    MONEY money_;
    MONEY balance_;
    int current_tab_;
    int max_tab_;

	bool Render_ui_Draw_List_;
};

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
