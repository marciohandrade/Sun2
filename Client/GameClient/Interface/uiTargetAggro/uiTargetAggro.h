#pragma once

#include "uiBase.h"

class uiTargetAggro : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P000,      // background top
        kControl_Picture_P001,      // background middle
        kControl_Picture_P002,      // background bottom
        //¹ö¼­Ä¿
        kControl_Picture_P003,      // aggro 1st
        kControl_Picture_P004,      // aggro 2nd
        kControl_Picture_P005,      // aggro 3rd
        kControl_Picture_P006,      // aggro 4th
        kControl_Picture_P007,      // aggro 5th
        //µå·¡°ï
        kControl_Picture_P008,      // aggro 1st
        kControl_Picture_P009,      // aggro 2nd
        kControl_Picture_P010,      // aggro 3rd
        kControl_Picture_P011,      // aggro 4th
        kControl_Picture_P012,      // aggro 5th
        //¼¨µµ¿ì
        kControl_Picture_P017,      // aggro 5th
        kControl_Picture_P016,      // aggro 4th
        kControl_Picture_P015,      // aggro 3rd
        kControl_Picture_P014,      // aggro 2nd
        kControl_Picture_P013,      // aggro 1st
        //¹ßÅ°¸®
        kControl_Picture_P018,      // aggro 1st
        kControl_Picture_P019,      // aggro 2nd
        kControl_Picture_P020,      // aggro 3rd
        kControl_Picture_P021,      // aggro 4th
        kControl_Picture_P022,      // aggro 5th
        //¿¤¸®
        kControl_Picture_P023,      // aggro 1st
        kControl_Picture_P024,      // aggro 2nd
        kControl_Picture_P025,      // aggro 3rd
        kControl_Picture_P026,      // aggro 4th
        kControl_Picture_P027,      // aggro 5th
        //¹Ì½ºÆ½ //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        kControl_Picture_P028,      // aggro 1st
        kControl_Picture_P029,      // aggro 2nd
        kControl_Picture_P030,      // aggro 3rd
        kControl_Picture_P031,      // aggro 4th
        kControl_Picture_P032,      // aggro 5th

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        kControl_Picture_P033,      // aggro 1st
        kControl_Picture_P034,      // aggro 2nd
        kControl_Picture_P035,      // aggro 3rd
        kControl_Picture_P036,      // aggro 4th
        kControl_Picture_P037,      // aggro 5th
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        kControl_Picture_P038,      // aggro 1st
        kControl_Picture_P039,      // aggro 2nd
        kControl_Picture_P040,      // aggro 3rd
        kControl_Picture_P041,      // aggro 4th
        kControl_Picture_P042,      // aggro 5th
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        kControl_Text_S000,         // name 1st
        kControl_Text_S001,         // name 2nd
        kControl_Text_S002,         // name 3rd
        kControl_Text_S003,         // name 4th
        kControl_Text_S004,         // name 5th

        kControl_Size,
    };

    enum
    {
        kBackground_Max = (kControl_Picture_P002 - kControl_Picture_P000) + 1,
        kGague_Max = (kControl_Picture_P007 - kControl_Picture_P003) + 1,
        kText_Max = (kControl_Text_S004 - kControl_Text_S000) + 1,
    };

    enum
    {
        kDisplay_Normal = 0,
        kDisplay_Numeric,
        kDisplay_Percent,
    };
public:
    uiTargetAggro(InterfaceManager* ui_manager_ptr);
    virtual ~uiTargetAggro(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    virtual void NetworkProc(MSG_BASE* message);
    virtual void MessageProc(SI_MESSAGE* message);

    void set_display_type(BYTE display_type) { display_type_ = display_type; }

private:
    void RegisterControl();
    void SetAggroData(MSG_CG_ARRGO_DISPLAY_CMD* aggro_data);
    void ResizeTargetAggros(BYTE player_count);
    void SetDisplay(TCHAR* character_name, int index, float aggro_value, DWORD max_rate_aggro);
private:
    CCtrlPictureWZ* backgrounds_[kBackground_Max];
    CCtrlPictureWZ* aggro_gagues_[eCHAR_TYPE_MAX-1][kGague_Max];
    CCtrlStaticWZ* aggro_names_[kText_Max];
    DWORD gague_max_size_;
    BYTE display_type_;
    BYTE last_class_code_;
};
