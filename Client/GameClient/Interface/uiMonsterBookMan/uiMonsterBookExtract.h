#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_007086_20140318_MONSTERBOOK_EXTRACT
#include "uiMonsterBookMan/uiMonsterBookBase.h"
#include "uiBase.h"


class uiMonsterBookExtract : public uiMonsterBookBase
{
public:
    enum UIControlPos
    {
        kControl_Text_S006,    // OFF.ON?
        kControl_Text_S007,    // 255?
        kControl_Picture_P010, // 미확인 이미지

        kControl_Size,
    };

public:
    uiMonsterBookExtract(InterfaceManager* ui_manager);
    virtual ~uiMonsterBookExtract();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

    void SetMonsterInfo(DWORD monster_code);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RenderTooltip();
};

#endif //_NA_007086_20140318_MONSTERBOOK
