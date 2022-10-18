#pragma once
#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterSelectList : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  //캐릭터슬롯1
        kControl_CheckBT_C001,      //캐릭터슬롯2
        kControl_CheckBT_C002,      //캐릭터슬롯3
        kControl_CheckBT_C003,      //캐릭터슬롯4
        kControl_CheckBT_C004,      //캐릭터슬롯5
        kControl_CheckBT_C005,      //캐릭터슬롯6
        kControl_CheckBT_C006,      //캐릭터슬롯7
        kControl_CheckBT_C007,      //캐릭터슬롯8
        kControl_CheckBT_C008,      //캐릭터슬롯9
        kControl_CheckBT_C009,      //캐릭터슬롯10
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


