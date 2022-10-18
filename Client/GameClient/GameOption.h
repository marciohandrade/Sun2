#pragma once

#include "ChatDialog_def.h"

#include "Narray.h"

#define GAMEOPTION		CGameOption::Instance()
class CGameOption : public CGameParameter, public util::Singleton<CGameOption>
{
private:
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


    // 사용가능한 해상도 목록
	nArray<SCREEN_SETTING>		m_ScreenSettingArray;
	int							m_ScreenSettingCount;

    // 
    OptionParam					m_OptionParam;

private:
    void UpdateScreenSettingArray();

private:
	BOOL						m_bExceptionSize;
	St_ScreenFormat				m_ScreenFormat;
	
public:

    //GS_BUG_FIX:해상도맥스일때..정상적으로 윈도우크기를변경하지못해 좌표가 어긋나는버그
    inline			void	GetExceptionSize(St_ScreenFormat& rScreenFormat){	memcpy(&rScreenFormat,&m_ScreenFormat,sizeof(St_ScreenFormat));}
    inline			void	SetExceptionSize( const St_ScreenFormat& rScreenFormat){	memcpy(&m_ScreenFormat,&rScreenFormat,sizeof(St_ScreenFormat));}
    inline			BOOL	IsExceptionSize() { return m_bExceptionSize;}
    inline			void	SetExceptionFlag(BOOL bFlag) { m_bExceptionSize = bFlag;}


	//////////////////////////////////////////////////////////////////////////

public:
	CGameOption();
	~CGameOption();

	BOOL			Load();
	BOOL			Save();

    void ApplyScreenOption();
    void ApplyOptionValue( nOptionType::eOptionTypeIndex TypeIndex, int Value );
    void ApplyOption();

    void ToggleFullScreenMode();

	const OptionParam& GetGameOption() const { return m_OptionParam; }
    OptionParam& GetGameOption() { return m_OptionParam; }
	void SetGameOption( const OptionParam& OptionParameter ) { m_OptionParam = OptionParameter; }

    void SetScreenIndex( int index ) { m_OptionParam.SetScreenIndex(index); }
    void SetScreenMode( OptionParam::OptionScreenMode ScreenMode ) { m_OptionParam.SetScreenMode( ScreenMode ); }


	//	화질 관련
	//inline 			BOOL		GetTextureQuality() 		{ return m_OptionParam.iTextureQuality; }
    inline			int		GetLOD()					{ return m_OptionParam.GetOptionValue( nOptionType::LOD_LEVEL ); }
    inline 			BOOL	GetNormalTextureQuality() 	{ return m_OptionParam.GetOptionValue( nOptionType::NORMAL_MAP_QUALITY ); }
    inline 			BOOL	GetShadowQuality()			{ return m_OptionParam.GetOptionValue( nOptionType::SHADOW_QUALITY ); }
    inline 			BOOL	IsTileBlending()			{ return m_OptionParam.GetOptionValue( nOptionType::USE_TILE_BLENDING ); }
    inline			void	SetTileBlending(BOOL bVal)	{ m_OptionParam.SetOptionValue( nOptionType::USE_TILE_BLENDING, bVal ); } 

	//	이펙트 관련
    inline 			BOOL	GetBlurState() 				{ return m_OptionParam.GetOptionValue( nOptionType::USE_BLUR ); }
    inline			BOOL	GetFogState()				{ return m_OptionParam.GetOptionValue( nOptionType::USE_FOG ); }
    inline			BOOL	GetMapDistantView()			{ return m_OptionParam.GetOptionValue( nOptionType::USE_MAP_DISTANCE_VIEW ); }
    inline			BOOL	GetSystemVoice()			{ return m_OptionParam.GetOptionValue( nOptionType::USE_SYSTEM_VOICE ); }
    inline			BOOL	GetNPCVoice()				{ return m_OptionParam.GetOptionValue( nOptionType::USE_NPC_VOICE ); }

	
    inline			float	GetSystemVolume()			{ return m_OptionParam.GetOptionValueFloat( nOptionType::SYSTEM_VOLUME ); }
    inline			float	GetSpeakerVolume()			{ return m_OptionParam.GetOptionValueFloat( nOptionType::SPEAKER_VOLUME ); }
	
	//	사운드 음량
	inline 			float	GeBGMVolume() 				{ return m_OptionParam.GetOptionValueFloat( nOptionType::BGM_VOLUME ); }
	inline			float	GetAmbientEffectVolume()	{ return m_OptionParam.GetOptionValueFloat( nOptionType::AMBIENT_VOLUME ); }
	inline			float	GetEffectVolume()			{ return m_OptionParam.GetOptionValueFloat( nOptionType::EFFECT_VOLUME ); }

    inline          int     GetCameraViewPos() const    { return m_OptionParam.GetOptionValue( nOptionType::CAMERA_VIEW_HEIGHT_OFFSET ); }

    inline          int     GetDamageOutput()           { return m_OptionParam.GetOptionValue( nOptionType::DAMAGE_TEXT_OUTPUT ); }

#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
    // nOptionType::USE_AUTO_MOVE 용도를 스킬후자동이동으로 대체 사용
    inline          BOOL    GetAutoMove()               { return TRUE; }
    inline          BOOL    GetSkillAfterAutoMove()     { return m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_MOVE ); }
#else
    inline          BOOL    GetAutoMove()               { return m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_MOVE ); }
#endif //_NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION

    inline          BOOL    GetShowPlayerName()         { return m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_NAME ); }
    inline          BOOL    GetShowNPCName()            { return m_OptionParam.GetOptionValue( nOptionType::SHOW_NPC_NAME ); }
    inline			BOOL	GetShowGuildName()          { return m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_GUILD ); }
    inline          BOOL    GetShowHelmet()             { return m_OptionParam.GetOptionValue( nOptionType::SHOW_HELMET ); }
    inline          void    SetShowHelmet( BOOL bShow ) { m_OptionParam.SetOptionValue( nOptionType::SHOW_HELMET, bShow ); }

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    inline          BOOL    GetShowCostume()             { return m_OptionParam.GetOptionValue( nOptionType::HIDE_COSTUME ); }
    inline          void    SetShowCostume( BOOL bShow ) { m_OptionParam.SetOptionValue( nOptionType::HIDE_COSTUME, bShow ); }
#endif

#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
	MONEY                   GetHeimTradingvolumeNotification();
#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    inline          void    SetShowWing( BOOL bShow )   { m_OptionParam.SetOptionValue( nOptionType::SHOW_WING, bShow ); }
    inline          BOOL    GetShowWing()               { return m_OptionParam.GetOptionValue( nOptionType::SHOW_WING ); }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

    inline          void    SetShowHelp( BOOL bShow )   { m_OptionParam.SetOptionValue( nOptionType::USE_HELP, bShow ); }
    inline          BOOL    GetShowHelp()               { return m_OptionParam.GetOptionValue( nOptionType::USE_HELP );  }
    inline          BOOL    GetAutoTarget()             { return m_OptionParam.GetOptionValue( nOptionType::USE_AUTO_TARGET );}
    inline          BOOL    GetForwardTarget()          { return m_OptionParam.GetOptionValue( nOptionType::USE_FORWARD_TARGET ); }
    inline          BOOL    GetFixTarget()              { return m_OptionParam.GetOptionValue( nOptionType::USE_FIXED_TARGET ); }
    inline          BOOL    GetChangeEyeMouse()         { return m_OptionParam.GetOptionValue( nOptionType::USE_CHANGE_EYE_MOUSE ); }
    inline          BOOL    GetMoveToCamera()           { return m_OptionParam.GetOptionValue( nOptionType::USE_MOVE_TO_CAMERA ); }
    inline          INT     GetMouseSens()              { return m_OptionParam.GetOptionValue( nOptionType::MOUSE_SENS_VALUE ); }
    inline          INT     GetMouseViewSens()          { return m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SENS_VALUE ); }

	// 스킬 옵션 관련 
    inline          BOOL    GetSkillQueueing()                  { return m_OptionParam.GetOptionValue( nOptionType::USE_SKILLQUEUEING ); }
    inline          void    SetSkillQueueing(BOOL bQueueing)    { m_OptionParam.SetOptionValue( nOptionType::USE_SKILLQUEUEING, bQueueing ); }
    inline          INT     GetMouseViewSpeed()                 { return m_OptionParam.GetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE ); }
    inline			void	SetMouseViewSpeed(int iSpeed)       { m_OptionParam.SetOptionValue( nOptionType::MOUSE_VIEW_SPEED_VALUE, iSpeed ); }

    inline			BOOL	GetShadowOn()				        { return m_OptionParam.GetOptionValue( nOptionType::SHOW_SHADOW ); }
    inline			void	SetShadowOn(BOOL bShadow)	        { m_OptionParam.SetOptionValue( nOptionType::SHOW_SHADOW, bShadow ); }

    inline			float	GetGamma()					        { return m_OptionParam.GetOptionValueFloat( nOptionType::GAMMA_VALUE ); }
    inline			void	SetGamma(float fGamma)		        { m_OptionParam.SetOptionValue( nOptionType::GAMMA_VALUE, (int)(fGamma*100.0f) ); }

	const SCREEN_SETTING& GetAvailableScreenSetting(int index);
    int                 GetAvailableScreenSettingIndex(int width, int height, int bpp);
	int                 GetAvailableScreenSettingCount();

    inline			BOOL	GetOperateMultiThread()             { return m_OptionParam.GetOptionValue( nOptionType::USE_MULTITHREAD ); }
    inline			BOOL	GetShowMoviePlay()                  { return m_OptionParam.GetOptionValue( nOptionType::USE_PLAY_MOVIE );  }
    inline			BOOL	GetDrawMarketTitle()                { return m_OptionParam.GetOptionValue( nOptionType::SHOW_VENDOR_TITLE ); }
    inline			float	GetCullLength()                     { return m_OptionParam.GetOptionValueFloat( nOptionType::CULLING_DISTANCE_VALUE ); }
#ifdef _YJ_PK_OPTION_001120
    inline			BOOL	GetPKWithoutCtrlKey()               {return m_OptionParam.GetOptionValue( nOptionType::USE_PK_WITH_CONTROL_KEY ); }
#endif //_YJ_PK_OPTION_001120

    inline			BOOL	GetPickUpItemNotify()       { return m_OptionParam.GetOptionValue( nOptionType::NOTIFY_PICKUP_ITEM ); } 
    inline			BOOL	GetNotifyDropItem()         { return m_OptionParam.GetOptionValue( nOptionType::NOTIFY_DROP_ITEM ); }

    inline			BOOL	GetShowPlayers()            { return m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYERS ); }
    inline			BOOL	GetShowVendors()            { return m_OptionParam.GetOptionValue( nOptionType::SHOW_VENDERS ); }

    inline			BOOL	GetShowPetName()            { return m_OptionParam.GetOptionValue( nOptionType::SHOW_PLAYER_PETNAME ); }

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
    inline			BOOL	GetShowHeadEffect()         { return m_OptionParam.GetOptionValue( nOptionType::SHOW_HEAD_EFFECT ); }
#endif//__CN_ADD_HEAD_EFFECT_FLAG(INVEN악세사리이펙트버튼)

    inline			BOOL	GetDisplayMonterInfo()              { return m_OptionParam.GetOptionValue( nOptionType::SHOW_MONSTERINFO ); }
    inline			BOOL	GetShowMyRenderInfo()               { return m_OptionParam.GetOptionValue( nOptionType::SHOW_MY_RENDER_INFO ); }
    inline			void	SetOperateMultiThread( BOOL val)    { m_OptionParam.SetOptionValue( nOptionType::USE_MULTITHREAD, val ); }
    inline			void	SetShowMoviePlay( BOOL val )        { m_OptionParam.SetOptionValue( nOptionType::USE_PLAY_MOVIE, val ); }
    inline			void    SetDrawMarketTitle( BOOL val )      { m_OptionParam.SetOptionValue( nOptionType::SHOW_VENDOR_TITLE, val ); }
    inline			void	SetCullLength( float fval)          { m_OptionParam.SetOptionValue( nOptionType::CULLING_DISTANCE_VALUE, (int)(fval*100.0f) ); }
	
    void                    SetChatFilterNormal( int value )    { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_NORMAL, value ); }
    void                    SetChatFilterParty( int value )     { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_PARTY, value ); }
    void                    SetChatFilterGuild( int value )     { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_GUILD, value ); }
    void                    SetChatFilterChannel( int value )   { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_CHANNEL, value ); }
    void                    SetChatFilterSystem( int value )    { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_SYSTEM, value ); }
#ifdef _K2P_ADD_WISPER_WINDOW
    void                    SetChatFilterWshiper( int value )   { m_OptionParam.SetOptionValue( nOptionType::CHAT_FILTER_WHISPER, value ); }
#endif 

    int                     GetChatFilterNormal()               { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_NORMAL ); }
    int                     GetChatFilterParty()                { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_PARTY ); }
    int                     GetChatFilterGuild()                { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_GUILD ); }
    int                     GetChatFilterChannel()              { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_CHANNEL ); }
    int                     GetChatFilterSystem()               { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_SYSTEM ); }
#ifdef _K2P_ADD_WISPER_WINDOW
    int                     GetChatFilterWshiper()              { return m_OptionParam.GetOptionValue( nOptionType::CHAT_FILTER_WHISPER ); }
#endif


    int                     GetLanguageType() const             { return m_OptionParam.GetOptionValue( nOptionType::LANGUAGE_TYPE ); }
    void                    SetLanguageType(LanguageType val)   { m_OptionParam.SetOptionValue( nOptionType::LANGUAGE_TYPE, val ); }
#ifdef _DH_OS_LANGUAGETYPE
	int                     GetOSUserDefaultLanguage();
#endif//_DH_OS_LANGUAGETYPE
    int                     GetPetGaugeShowType() const                 { return m_OptionParam.GetOptionValue( nOptionType::PET_GAUGE_TYPE ); }
    void                    SetPetGaugeShowType(PetGauge_ShowType val)  { m_OptionParam.SetOptionValue( nOptionType::PET_GAUGE_TYPE, val ); }
    BYTE GetQuickSlotSize() const                                       { return m_OptionParam.GetOptionValue( nOptionType::QUICKSLOT_COUNT ); }
    void SetQuickSlotSize(BYTE val)                                     { m_OptionParam.SetOptionValue( nOptionType::QUICKSLOT_COUNT, val ); }
    BYTE GetQuickSlotIndex() const                                      { return m_OptionParam.GetOptionValue( nOptionType::QUICKSLOT_INDEX ); }
    void SetQuickSlotIndex(BYTE val)                                    { m_OptionParam.SetOptionValue( nOptionType::QUICKSLOT_INDEX, val ); }
    BOOL GetChannelAccept()                                             { return m_OptionParam.GetOptionValue( nOptionType::USE_BLOCK_CHANNEL_INVITE ); }
    void SetChannelAccept(BOOL val)                                     { m_OptionParam.SetOptionValue( nOptionType::USE_BLOCK_CHANNEL_INVITE, val ); }
    BOOL GetPickingFriend()                                             { return m_OptionParam.GetOptionValue( nOptionType::PICKING_FRIEND ); }
    void SetPickingFriend(BOOL val)                                     { m_OptionParam.SetOptionValue( nOptionType::PICKING_FRIEND, val ); }

#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
    // nOptionType::USE_AUTO_MOVE 용도를 스킬후자동이동으로 대체 사용
    //BOOL GetSkillUseAfterAutoAttack()                                   { return m_OptionParam.GetOptionValue( nOptionType::SKILL_USE_AFTER_AUTOATTACK ); }
    //void SetSkillUseAfterAutoAttack(BOOL val)                           { m_OptionParam.SetOptionValue( nOptionType::SKILL_USE_AFTER_AUTOATTACK, val ); }
#endif //_NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
};


