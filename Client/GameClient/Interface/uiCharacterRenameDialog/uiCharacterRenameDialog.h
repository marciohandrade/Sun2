#pragma once
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterRenameDialog : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_BT62 = 0,   // 변경하기1
        kControl_Button_B000,       // 변경하기2
        kControl_Button_B001,       // 변경하기3
        kControl_Button_B002,       // 변경하기4
        kControl_Button_B003,       // 변경하기5
        kControl_Button_B004,       // 변경하기6
        kControl_Button_B006,       // 변경하기7
        kControl_Button_B007,       // 변경하기8
        kControl_Button_B008,       // 변경하기9
        kControl_Button_B009,       // 변경하기10
        kControl_Text_S000,         // 캐릭터이름1
        kControl_Text_S001,         // 캐릭터이름2
        kControl_Text_S002,         // 캐릭터이름3
        kControl_Text_S003,         // 캐릭터이름4
        kControl_Text_S004,         // 캐릭터이름5
        kControl_Text_S005,         // 캐릭터이름6
        kControl_Text_S006,         // 캐릭터이름7
        kControl_Text_S007,         // 캐릭터이름8
        kControl_Text_S008,         // 캐릭터이름9
        kControl_Text_S009,         // 캐릭터이름10
        kControl_TextImage_TI00,    // 나중에변경
        kControl_Button_Exit,       // X버튼
        kControl_Text_ST00,         // 안내메시지
        kControl_Size,
    };
public:
    uiCharacterRenameDialog(InterfaceManager* ui_manager);
    virtual ~uiCharacterRenameDialog();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);

    void InitNoticeText();
    void RefreshNeedRenameList();

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();

private:
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);

    bool SelectRename(int list_index);

private:
    typedef std::pair<int, std::string> RenameData;
    typedef std::list<RenameData> RenameList;

    RenameList need_rename_list_;
    RenameData selected_rename_;

    FTextRender notice_text_;

};
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE