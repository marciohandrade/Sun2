#pragma once
#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterSelectList : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  //ĳ���ͽ���1
        kControl_CheckBT_C001,      //ĳ���ͽ���2
        kControl_CheckBT_C002,      //ĳ���ͽ���3
        kControl_CheckBT_C003,      //ĳ���ͽ���4
        kControl_CheckBT_C004,      //ĳ���ͽ���5
        kControl_CheckBT_C005,      //ĳ���ͽ���6
        kControl_CheckBT_C006,      //ĳ���ͽ���7
        kControl_CheckBT_C007,      //ĳ���ͽ���8
        kControl_CheckBT_C008,      //ĳ���ͽ���9
        kControl_CheckBT_C009,      //ĳ���ͽ���10
        kControl_Size,
    };

    enum
    {
        kMaxListSlot = 10,
    };

public:
    uiCharacterSelectList(InterfaceManager* ui_manager);
    virtual ~uiCharacterSelectList();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);

    void SetServerName();
    void SetSelectedCharacterSlotIndex(int slot_index);
    void SetSelectedCharacterListIndex(int slot_index);
    void CharacterListUp();
    int GetSelectedCharacterSlotIndex();

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    void RefreshSelectList();
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);

    void ShowListSlotByCount(int slot_count);
    void SetCharacterSlotInfo(int list_index, 
                              TCHAR* character_name, 
                              BYTE delete_check,
                              int level, 
                              BYTE class_type, 
                              MAPCODE current_mapcode,
                              TCHAR* day_for_delete);
    void SetCharacterSlot(int list_index);
    void OnSelectPrevCharacter();
    void OnSelectNextCharacter();
    int GetListIndexFromSlotIndex(int slot_index);

private:
    std::vector<int> character_slot_list_;
    FTextRender character_info_text_[kMaxListSlot];
    int selected_list_index_;
};


