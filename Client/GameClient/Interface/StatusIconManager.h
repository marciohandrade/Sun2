#pragma once

#include "StateInfoParser.h"

class InterfaceManager;
class CDrawBase;
struct BASE_STATEINFO;
class CElementRender;
/*
// 이미지코드/텍스처핸들 소유/관리

// 키를 통한 상태아이콘 관리

// 상태아이콘 등록

// 등록된 상태아이콘 정보를 이용한 메쉬 정보 갱신

// 렌더

// ETC.
    - 행/열로 텍스처 UV 좌표 얻기
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
        MAX_INDEX_STATE_INFO = 140,         // 최대 140개(히어로(30)+파티원(10*9)+타겟(10)+소환수(10))
        IMAGACODE_STATE_TEXTURE = 38335,    // 상태버프 텍스쳐 이미지코드
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
    STATUS_ICON_CONTAINER status_icon_container_;     // 일반 아이콘-한번에 그린다.
    STATUS_ICON_CONTAINER cash_status_icon_container_;// 캐시 아이콘-블렌딩을 해야하므로 따로 그린다.
    HANDLE state_texture_;
    HANDLE cash_state_texture_;

    SI_FRECT frame_texture_rect_;
    SI_FRECT frame_texture_rect2_;

    void _RenderIconWithBlend( const STATUS_ICON_CONTAINER& IconContainer, HANDLE Texture );

};

