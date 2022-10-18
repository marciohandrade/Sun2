#pragma once

namespace nOptionType
{

    enum eOptionTypeIndex
    {
        // 1
        TEXTURE_QUALITY = 0,
        LOD_LEVEL,
        NORMAL_MAP_QUALITY,
        SHADOW_QUALITY,

        //IS_CRASH_REPORT,

        // 2
        USE_BLUR,
        USE_FOG,
        USE_MAP_DISTANCE_VIEW,
        USE_SYSTEM_VOICE,
        USE_NPC_VOICE,
        //USE_EFFECT_BATCH,
        USE_TILE_BLENDING,
        USE_VOLUME_LIGHT,

        // 3
        SYSTEM_VOLUME,
        SPEAKER_VOLUME,
        BGM_VOLUME,
        AMBIENT_VOLUME,
        EFFECT_VOLUME,

        // 4
        EFFECT_QUALITY,
        DAMAGE_TEXT_OUTPUT,

        // 5
        SHOW_PLAYER_NAME,
        SHOW_PLAYER_GRADE,
        SHOW_PLAYER_GUILD,
        SHOW_PLAYER_PETNAME,

        // 6
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
        SHOW_HEAD_EFFECT,        // 헤드이펙트
#endif //__CN_ADD_HEAD_EFFECT_FLAG
        SHOW_NPC_NAME,           // 이름표시
        SHOW_HELMET,               // 투구표시
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        SHOW_WING,               // 날개표시
#endif //_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

        // 7
        USE_AUTO_MOVE,              // 자동이동
        USE_HELP,                   // 도움말
        USE_SKILLQUEUEING,          // 스킬 큐잉
        USE_FIXED_UI_POSITION,      // 인터페이스 고정
        USE_AUTO_TARGET,            // 자동 타겟
        USE_FORWARD_TARGET,         // 전방위 타겟
        USE_FIXED_TARGET,           // 타겟 고정
        USE_CHANGE_EYE_MOUSE,       // 시야 마우스 변경
        USE_MOVE_TO_CAMERA,         // 카메라 방향으로 이동
        USE_SPRING_CAMERA,          // 스프링 카메라 	
        USE_BLOCK_WHISPER,          // 귓속말 차단
        USE_BLOCK_TRADE,            // 거래 차단
        USE_BLOCK_CHANNEL_INVITE,   // 

        // 8
#ifdef _YJ_PK_OPTION_001120
        USE_PK_WITH_CONTROL_KEY,
#endif //_YJ_PK_OPTION_001120
        NOTIFY_PICKUP_ITEM,
        NOTIFY_DROP_ITEM,

        // 9
        SHOW_PLAYERS,
        SHOW_VENDERS,
        SHOW_MONSTERINFO,
        SHOW_MY_RENDER_INFO,

        // 10
        MOUSE_SENS_VALUE,
        MOUSE_VIEW_SENS_VALUE,
        MOUSE_VIEW_SPEED_VALUE,

        // 11
        SHOW_SHADOW,
        GAMMA_VALUE,
        USE_EXTENSION_CAMERA_DISTANCE,

        // 12
        USE_PLAY_MOVIE,
        USE_MULTITHREAD,

        SHOW_VENDOR_TITLE,
        CULLING_DISTANCE_VALUE,

        // 13
        CHAT_FILTER_NORMAL,
        CHAT_FILTER_PARTY,
        CHAT_FILTER_GUILD,
        CHAT_FILTER_CHANNEL,
        CHAT_FILTER_SYSTEM,
#ifdef _K2P_ADD_WISPER_WINDOW
        CHAT_FILTER_WHISPER,
#endif //_K2P_ADD_WISPER_WINDOW

        // 14
        LANGUAGE_TYPE,

        PET_GAUGE_TYPE,

        QUICKSLOT_COUNT,
        QUICKSLOT_INDEX,

        //IS_CHANNEL_ACCEPT,

        // 15
#ifdef _DH_BATTLEZONE2_
        PICKING_FRIEND,
#endif //_DH_BATTLEZONE2_

        CAMERA_VIEW_HEIGHT_OFFSET,

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        HIDE_COSTUME,
#endif

#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
        //SKILL_USE_AFTER_AUTOATTACK,     // 스킬 사용 후 자동 공격 여부
#endif //_NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION

#ifdef _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
        SHADOW_DISTANCE,
#endif //_NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
		HEIM_TRADINGVOLUME, 
#endif // _NA_008678_20160203_ADD_HEIM_NOTIFICATION_OPTION

        MAX,
    };
};

class CGameParameter;

class OptionParam
{
public:
    enum OptionScreenMode
    {
        kScreenMode_Full = 0,
        kScreenMode_Window,
        kScreenMode_MaxWindow,  //<! 창모드 최대화
        kScreenMode_Count
    };

private:
    // 아래의 5개는 엔진을통해서 세이브/로드됨
    int m_ScreenX;
    int m_ScreenY;
    int m_iBPP;
    int m_ScreenIndex;
    int m_ScreenMode;

public:
    void SetScreenBitPerPixel( int bpp ) { m_iBPP = bpp; }
    void SetScreenSize( int Width, int Height ) { m_ScreenX = Width; m_ScreenY = Height; }
    void SetScreenMode( OptionScreenMode Mode ) { m_ScreenMode = Mode; }
    void SetScreenIndex( int Index ) { m_ScreenIndex = Index; }

    int GetScreenWidth() const { return m_ScreenX; }
    int GetScreenHeight() const { return m_ScreenY; }
    int GetScreenMode() const { return m_ScreenMode; }
    int GetScreenIndex() const { return m_ScreenIndex; }

private:
    int m_OptionValue[nOptionType::MAX];

public:
    void Save(CGameParameter* pParameter );
    void Load(CGameParameter* pParameter );

    int GetTextureQuality() const               { return m_OptionValue[nOptionType::TEXTURE_QUALITY]; }
    int GetLODLevel() const                     { return m_OptionValue[nOptionType::LOD_LEVEL]; }
    int GetEffectQuality() const                { return m_OptionValue[nOptionType::EFFECT_QUALITY]; }
    int GetShadowQuality() const                { return m_OptionValue[nOptionType::SHADOW_QUALITY]; }

    int IsRenderShadow() const                  { return m_OptionValue[nOptionType::SHOW_SHADOW]; }
    int IsRenderBlur() const                    { return m_OptionValue[nOptionType::USE_BLUR]; }
    int IsRenderFog() const                     { return m_OptionValue[nOptionType::USE_FOG]; }
    int IsUseMapDistanceView() const            { return m_OptionValue[nOptionType::USE_MAP_DISTANCE_VIEW]; }

    int GetOptionValue( nOptionType::eOptionTypeIndex TypeIndex ) const { return m_OptionValue[TypeIndex]; }
    float GetOptionValueFloat( nOptionType::eOptionTypeIndex TypeIndex ) const { return (float)m_OptionValue[TypeIndex] * 0.01f; }

    void SetOptionValue( nOptionType::eOptionTypeIndex TypeIndex, int Value ) { m_OptionValue[TypeIndex] = Value; }

};
