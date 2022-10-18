#pragma once
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiBase.h"

class uiGuildRelationAsk: public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B999 = 0,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Text_B000,
        kControl_Text_T001,
        kControl_Text_S001,
        kControl_Edit_E000,
        kControl_Size,
    };

public:
    enum GuildAskType
    {
        kGuild_Alliance_Relation = 0,       // 동맹 신청
        kGuild_Hostility_Relation,          // 적대 선포
        kGuild_Answer_Hostility_Relation,   // 적대 수락

        kGuild_Release_Alliance_Relation,   // 동맹 해제
        kGuild_Release_Hostility_Relation,  // 적대 해제
        kGuild_Cancel_Hostility_Relation,   // 선포 취소

        kGuild_Invite,                      // 길드 초대
        kGuild_Rename,                      // 길드명 변경

        kMaxType,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildRelationAsk(InterfaceManager* ui_manager);
    virtual ~uiGuildRelationAsk();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void SetEditText(TCHAR* edit_text);

    void set_guild_ask_type(uiGuildRelationAsk::GuildAskType value) { guild_ask_type_ = value; }
    uiGuildRelationAsk::GuildAskType guild_ask_type() const { return guild_ask_type_; }    
    void set_item_slot_position(POSTYPE value) { item_slot_position_ = value; }
    POSTYPE item_slot_position() const { return item_slot_position_; }

    STLX_STRING& temp_string() { return temp_string_; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickConfirmButton();

    void InitAskInfo();
    void UpdateInputNotice();
    DWORD ProcessGuildRelation(const TCHAR* guild_name);

    

private:
    GuildAskType guild_ask_type_;
    POSTYPE item_slot_position_;
    STLX_STRING temp_string_;
};

#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
