#pragma once
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterRenameDialog : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_BT62 = 0,   // �����ϱ�1
        kControl_Button_B000,       // �����ϱ�2
        kControl_Button_B001,       // �����ϱ�3
        kControl_Button_B002,       // �����ϱ�4
        kControl_Button_B003,       // �����ϱ�5
        kControl_Button_B004,       // �����ϱ�6
        kControl_Button_B006,       // �����ϱ�7
        kControl_Button_B007,       // �����ϱ�8
        kControl_Button_B008,       // �����ϱ�9
        kControl_Button_B009,       // �����ϱ�10
        kControl_Text_S000,         // ĳ�����̸�1
        kControl_Text_S001,         // ĳ�����̸�2
        kControl_Text_S002,         // ĳ�����̸�3
        kControl_Text_S003,         // ĳ�����̸�4
        kControl_Text_S004,         // ĳ�����̸�5
        kControl_Text_S005,         // ĳ�����̸�6
        kControl_Text_S006,         // ĳ�����̸�7
        kControl_Text_S007,         // ĳ�����̸�8
        kControl_Text_S008,         // ĳ�����̸�9
        kControl_Text_S009,         // ĳ�����̸�10
        kControl_TextImage_TI00,    // ���߿�����
        kControl_Button_Exit,       // X��ư
        kControl_Text_ST00,         // �ȳ��޽���
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