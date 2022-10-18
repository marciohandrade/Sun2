#ifndef _uiAutobotConditionSupport_h_
#define _uiAutobotConditionSupport_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotDialog.h"

//------------------------------------------------------------------------------ 
class uiAutobotConditionSupport : public uiAutobotDialog
{
    friend class uiAutobotMan;

    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,
        kControl_CheckBT_C005,
        kControl_CheckBT_C006,
        kControl_CheckBT_C007,
        kControl_CheckBT_C010,
        kControl_CheckBT_C024,
        kControl_CheckBT_C000,
        kControl_CheckBT_C002,
        kControl_CheckBT_C004,
        kControl_CheckBT_C008,
        kControl_CheckBT_C009,
        kControl_CheckBT_C011,
        kControl_CheckBT_C003,
        kControl_CheckBT_C012,
        kControl_CheckBT_C013,
        kControl_CheckBT_C014,
        kControl_CheckBT_C015,
        kControl_CheckBT_C016,
        kControl_CheckBT_C017,
        kControl_CheckBT_C018,
        kControl_CheckBT_C019,
        kControl_CheckBT_C020,
        kControl_CheckBT_C021,
        kControl_CheckBT_C022,
        kControl_CheckBT_C025,
        kControl_CheckBT_C023,
        kControl_Button_B999,
        kControl_Button_B034,
        kControl_Button_B035,
        kControl_Button_B036,
        kControl_Button_B037,
        kControl_Button_B000,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B006,
        kControl_Button_B010,
        kControl_Text_T101,
        kControl_Text_T999,
        kControl_Text_T006,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S012,
        kControl_Text_S019,
        kControl_Text_S000,
        kControl_Text_S003,
        kControl_Text_S026,
        kControl_Text_S031,
        kControl_Text_S032,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Text_S010,
        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Text_S033,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S014,
        kControl_Text_S020,
        kControl_Text_S021,
        kControl_Text_S022,
        kControl_Text_S023,
        kControl_Text_S024,
        kControl_Text_S025,
        kControl_Text_S027,
        kControl_Text_S028,
        kControl_Text_S029,
        kControl_Text_S030,
        kControl_Text_S034,
        kControl_Text_S035,
        kControl_Text_S036,
        kControl_Text_S037,
        kControl_Text_S038,
        kControl_Text_S041,
        kControl_Text_S042,
        kControl_Text_S044,
        kControl_Text_S011,
        kControl_Text_S013,
        kControl_Text_S017,
        kControl_Edit_E000,
        kControl_List_L000,
        kControl_List_L001,
        kControl_List_L002,
        kControl_List_L003,
        kControl_List_L004,
        kControl_List_L005,
        kControl_List_L006,
        kControl_List_L007,
        kControl_List_L008,
        kControl_List_L009,
        kControl_List_L010,
        kControl_List_L011,
        kControl_VScroll_VS00,
        kControl_VScroll_V000,
        kControl_Picture_P029,
        kControl_Picture_P014,
        kControl_Picture_P022,
        kControl_Picture_P030,
        kControl_Picture_P031,
        kControl_Picture_P055,
        kControl_Picture_P056,
        kControl_Picture_P057,
        kControl_Picture_P058,
        kControl_Picture_P059,
        kControl_Picture_P060,
        kControl_Picture_P061,
        kControl_Picture_P062,
        kControl_Picture_P063,  //!<귀환:인벤토리
        kControl_Picture_P064,  //!<귀환:hp
        kControl_Picture_P065,  //!<귀환:사료
        kControl_Picture_P067,  //!<귀환:장비류
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P013,
        kControl_Picture_P015,
        kControl_Picture_P016,  //!< 자동구매 : 귀환스크롤
        kControl_Picture_P017,  //!< 자동구매 : 사료
        kControl_Picture_P018,  //!< 자동구매 : 회복 hp 레벨
        kControl_Picture_P019,  //!< 자동구매 : 회복 hp 갯수
        kControl_Picture_P006,
        kControl_Picture_P009,
        kControl_Picture_P020,  //!< 커뮤니티기능 : 외치기( 타입)
        kControl_Picture_P021,  //!< 커뮤니티기능 : 외치기(시간:초)
        kControl_Picture_P023,  //!< 커뮤니티기능 : 자동채널 레벨
        kControl_Picture_PI00,  //!< 커뮤니티기능 : 자동체널 레벨 스크롤바
        kControl_Picture_P025,  //!< 커뮤니티기능 : 자동채널 이름
        kControl_Picture_P026,  //!< 커뮤니티기능 : 자동체널 이름 스크롤바

        kControl_Picture_P008,  //!< 그래이(disable) 판
        kControl_Picture_P024,
        kControl_Picture_P027,
        kControl_Picture_P028,
        kControl_Picture_P032,

        kControl_TextImage_TI07,
        kControl_TextImage_TI06,

        //! 2011.9.16 / i4u4me /  
        kControl_CheckBT_C026, //!< 필드로 복귀
        kControl_Text_S015,
        kControl_Picture_P033,

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiAutobotConditionSupport(InterfaceManager* ui_man);
    virtual ~uiAutobotConditionSupport(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void NetworkProc(MSG_BASE* msgbase);
    virtual void MessageProc(SI_MESSAGE* message);

protected:
    void DefaultUI();
    bool UpdateCondition();
    bool UpdateUI();
    eAUTO_HUNTING_TYPE GetHuntingTypeFromControlIndex(DWORD control_index);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnThumbPosition(SI_MESSAGE* message);

    void OnDropdownEnable();
    void OnDropdownShow(UIControlPos list_index, UIControlPos picture_index);
    void OnDropdownScrollDown(UIControlPos list_index, UIControlPos scroll_index);
    void OnDropdownScrollUp(UIControlPos list_index, UIControlPos scroll_index);
};

#endif //_YMS_AUTOBOT_GAME

#endif