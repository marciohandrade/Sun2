#pragma once

#include "StateInfoParser.h"

class InterfaceManager;
class CDrawBase;
struct BASE_STATEINFO;
class CElementRender;
/*
// �̹����ڵ�/�ؽ�ó�ڵ� ����/����

// Ű�� ���� ���¾����� ����

// ���¾����� ���

// ��ϵ� ���¾����� ������ �̿��� �޽� ���� ����

// ����

// ETC.
    - ��/���� �ؽ�ó UV ��ǥ ���
*/

class StatusIconManager
{
    typedef std::vector<SKILL_EFFECT_IMAGE*>  STATUS_ICON_CONTAINER;

public:
    enum
    {
#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
        MAX_HERO_ICON = 38,
#elif defined(_NA_000000_20130114_RENEWER_UI)
        MAX_HERO_ICON = 40,
#else
		MAX_HERO_ICON = 30,
#endif

#ifdef _NA_000000_20130114_RENEWER_UI
        MAX_PARTYMEMBER_ICON = 20,
#else
		 MAX_PARTYMEMBER_ICON = 10,
#endif

#ifdef _NA_008139_20150318_MODIFY_BUFF_ICON_UI
         MAX_TARGET_ICON = 36,
#elif defined(_NA_000000_20130114_RENEWER_UI)
		 MAX_TARGET_ICON = 40,
#else
		 MAX_TARGET_ICON = 10,
#endif
        MAX_SUMMON_ICON = 10,
        STATUS_ICON_SIZE = 14,
        ALPHA_VALUE_MOUSE_OVER_ICON = 100,
        MAX_INDEX_STATE_INFO = 140,         // �ִ� 140��(�����(30)+��Ƽ��(10*9)+Ÿ��(10)+��ȯ��(10))
        IMAGACODE_STATE_TEXTURE = 38335,    // ���¹��� �ؽ��� �̹����ڵ�
    };    

public:
    StatusIconManager();
    ~StatusIconManager();

public:
    bool Initialize(InterfaceManager* interfacemanager, CDrawBase* drawbase);
    void RegisterStatusIconInfo();
    void RegisterStatusIconInfo(std::vector<SKILL_EFFECT_IMAGE>& state_info_container);
    void RenderStatusIcon();
    
private:
    void Release();
    bool StatusIconRender();
    bool CashStatusIconRender();
    void LoadTexture();

private:
    bool is_initialize_;
    InterfaceManager* interfacemanager_;
    CDrawBase* drawbase_;
    CElementRender* element_render_;
    STATUS_ICON_CONTAINER status_icon_container_;     // �Ϲ� ������-�ѹ��� �׸���.
    STATUS_ICON_CONTAINER cash_status_icon_container_;// ĳ�� ������-������ �ؾ��ϹǷ� ���� �׸���.
    HANDLE state_texture_;
    HANDLE cash_state_texture_;

    SI_FRECT frame_texture_rect_;
    SI_FRECT frame_texture_rect2_;

    void _RenderIconWithBlend( const STATUS_ICON_CONTAINER& IconContainer, HANDLE Texture );

};

