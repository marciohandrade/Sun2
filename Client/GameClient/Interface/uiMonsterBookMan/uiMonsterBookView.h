#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookMan/uiMonsterBookBase.h"
#include "uiBase.h"


class uiMonsterBookView : public uiMonsterBookBase
{
public:
    enum UIControlPos
    {
        kControl_Text_S007 = 100,   // lv %s
        kControl_Text_S018,         // �̸�
        kControl_Text_S013,         // hp
        kControl_Text_S014,         // mp
        kControl_Text_S015,         // ���ݷ�
        kControl_Text_S016,         // ����
        kControl_Text_S017,         // ���ݼӵ�
        kControl_Text_S006,         // ��ų���ݷ�

        kControl_Picture_P010,      // ��Ȯ�� �̹���
        kControl_Picture_P016,      // ��� �̹���
        kControl_Picture_P013,      // < > ��ư �ڹ��
        kControl_Button_B000,       // <��ư
        kControl_Button_B001,       // >��ư

        kControl_Size,
    };

public:
    uiMonsterBookView(InterfaceManager* ui_manager);
    virtual ~uiMonsterBookView();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void Process(DWORD tick);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

    void SetMonsterInfo(DWORD monster_code);    // �ǿ� �´� �������� ����
    void CreateMonster(DWORD monster_code);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RenderTooltip();

    BOOL is_active_monster_;
    //------------------------------------------------------------------------------ 
    // ������Ʈ ȸ��
    void TurnLeft(float radin = 0.2f);
    void TurnRight(float radin = 0.2f);
    
    struct PreviewScreenInfo
    {
        float rot;  //! ȸ�� radin_value �ִϸ��̼ǰ�.
        float zoom;
        FPOINT org;

        POINT mouse_pos;
        void reset()
        {
            mouse_pos.x = 0;
            mouse_pos.y = 0;
            rot = PI;
            zoom = 1.0f;
            org.x = 0;
            org.y = 0;
        }
        void check_valid_value()
        {
            if (rot > (PI * 2.0f))  rot -= (PI * 2.0f);
            if (rot < 0.0f)         rot += (PI * 2.0f);
            if (zoom > 2.0f)        zoom = 2.0f;
            if (zoom < 0.15f)       zoom = 0.15f;

            if (org.x >  1.5f)      org.x =  1.5f;
            if (org.x < -1.5f)      org.x = -1.5f;
            if (org.y >  1.5f)      org.y =  1.5f;
            if (org.y < -1.5f)      org.y = -1.5f;
        }
    };

    PreviewScreenInfo& screen_info() { return screen_info_; }

private:
    PreviewScreenInfo screen_info_;
    //------------------------------------------------------------------------------ 
};

#endif //_NA_007086_20140318_MONSTERBOOK
