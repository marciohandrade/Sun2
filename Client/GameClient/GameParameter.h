#pragma once

#include <singleton.h>

typedef util::_tstring ATTRNAMETYPE;

#define		MAX_SCREEN_SETTING			18

typedef struct _SCREEN_SETTING
{
	St_ScreenSetting ScreenSetting;
	int iOriginalScreenSetting;
    std::string SettingText;
} SCREEN_SETTING;

enum eOptionDamageOutput
{
	eODO_SHOWALL				= 0,	//모두
	eODO_SHOWMINE		    = 1,	//나만
	eODO_NONE			    = 2,	//끔

	eODO_MAX,
};


template<class T>
class AttrMap
{
	map<ATTRNAMETYPE, T> m_Map;
public:
	T& GetParam(ATTRNAMETYPE& attrName)
	{
		map<ATTRNAMETYPE,T>::iterator iter = m_Map.find(attrName);		
		if(iter == m_Map.end())
		{
			//Read를 먼저 해주어야 합니다.
			assert(0);			
		}

		return iter->second;
	}

	friend class CGameParameter;
};

class CGameParameter
{
protected:

	char					m_szFullPath[MAX_PATH];
	AttrMap<int>			m_IntParam;
	AttrMap<float>			m_FloatParam;
	AttrMap<util::_tstring>	m_StringParam;

	char					m_szPackFileName[MAX_PATH];
	char					m_szSettingFileNameForPack[MAX_PATH];
	TCHAR					m_szCetegoryName[MAX_PATH];

	util::_tstring			m_engineRoot;

public:

	CGameParameter(void);
	CGameParameter(LPCSTR filename);
	virtual ~CGameParameter(void);

	int				ReadInt( LPCTSTR szKeyName, int nDefaultValue, LPCTSTR szCetegoryName = NULL );
	float			ReadFloat( LPCTSTR szKeyName, float fDefaultValue, LPCTSTR szCetegoryName = NULL );
	util::_tstring&	ReadString( LPCTSTR szKeyName, LPCTSTR szDefaultValue, LPCTSTR szCetegoryName = NULL );
    
	int				GetIntParam(ATTRNAMETYPE attrName)		{	return m_IntParam.GetParam(attrName);	}
	float			GetFloatParam(ATTRNAMETYPE attrName)	{	return m_FloatParam.GetParam(attrName);	}
	util::_tstring&	GetStringParam(ATTRNAMETYPE attrName)	{	return m_StringParam.GetParam(attrName);}

	BOOL			WriteInt( LPCTSTR szKeyName, int nValue, LPCTSTR szCetegoryName = NULL );
	BOOL			WriteFloat( LPCTSTR szKeyName, float fValue, LPCTSTR szCetegoryName = NULL );

	void			SetSettingFileName( LPCSTR szSettingFileName );
	void			SetPackFileName( LPCSTR szPackFileName );

	BOOL			IsSystemPackFileName( void )			{ return m_szPackFileName[0]; }
	LPCTSTR         GetEngineRoot()							{ return m_engineRoot.c_str();}

	void			SetCetegoryName( LPCSTR szCategoryName );

protected :

	LPCTSTR			SelectCetegoryName( LPCTSTR szCategoryName );
	void			Read( util::_tstring & szOutput, LPCTSTR szKeyName, LPCTSTR szDefaultValue, LPCTSTR szCetegoryName );
	BOOL			Write( LPCTSTR szKeyName, LPCTSTR szValue, LPCTSTR szCetegoryName );
	
};


//////////////////////////////////////////////////////////////////////////
// GameParameter 들...


#define GENERALPARAM CGeneralGameParam::Instance()
class CGeneralGameParam : public CGameParameter, public util::Singleton<CGeneralGameParam>
{
private:

	// 자주 이용하는 것은 변수로 등록하여 사용한다.
	BOOL		m_bNet;		
	MAPID		m_Start_MapID;
	FIELDID		m_Start_FieldID;
	MAPID		m_CharSelectMapID;
	DWORD		m_dwVillageMapID;
    int         m_iStartTileIndex;

	int         m_iShadowType;
	int			m_iCameraRotation;
	int         m_iScreenShotMode;
	
	WzVector    m_vCameraTo;

	BOOL        m_bInterpolatePosition;	
	BOOL        m_bServerDebug;

	//	동영상 촬영용 프리 카메라 모드
	int				m_iIsFreeCameraMode;
	BOOL			m_bFreeCameraState;
	float			m_fFreeCameraSpeed;
	int				m_iFreeCameraTurnSpeed;
	
	util::_tstring	m_szCinematicFileName;
    util::_tstring	m_szCharacterSceneCinematicFileName;

    BOOL            m_bShowConsole;
    util::_tstring  m_szConsoleSetting;
    util::_tstring  m_szFileLogSetting;

	//BOOL			m_iSpecialMode;
	eSPECIAL_MODE	m_eSpecialMode;
	BOOL			m_bUsePackFile;

	
	int         m_iMultiThreadLoading;	//  멀티스레드 로드
	BOOL        m_bMoveNakProcess;      // 이동처리에 따른 Nak 처리

	BOOL		m_bStartLuaConsole;

    // TEST RESOURCE 
    BOOL        m_bResourceTestScene;
    BOOL        m_bTestGraphicResource;
    BOOL        m_bTestMap;
    BOOL        m_bTestEffect;
    BOOL        m_bTestInterface;
	BOOL		m_bTestRscFailedOnly;

    // TEST_UI_SCENE
    BOOL        m_bTestUIScene;
    int         m_iTestUIScene_ExtID;

#ifdef _DEV_VER
    bool        show_ui_name_;//Keyword:_DH_SHOW_UI_NAME 
    bool        show_string_id_;//Keyword:_DEBUG_STRINGIDEX_SHOW
    bool        show_trigger_;//define: _DH_TRIGGER_LOG_MESSAGE
    bool        show_item_id_;//define:_SCJ_SHOW_ITEM_ID
    bool        show_npc_id_;//define:_SCJ_SHOW_NPC_ID
#endif
    bool        show_ssq_player_name_;//define:_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
	//////////////////////////////////////////////////////////////////////////
	DWORD			m_dwUserID;
	

	//============================================================================================== program.ini로 옮겨야함!!!
	// 게임 공용 파라미터  -----------------------------------------------
	int         m_iIntervalToWorldServerReconnect;
	BOOL        m_bSpringCamera;
	/// spiring constant
	float       m_springConst;
	/// damping constant
	float       m_dampConst;
	/// spring length constatnt
	float       m_springLen;

	// 길드 랭크 리스트 카운드 
	int			m_iGuildRankUpDisplayCount;


	BOOL        m_bSoundLog;

	float       m_fCameraDistance;
	float       m_fMaxCameraDistance;

    //! 날개를 편상태에서 아래로 점프 가능한 높이
    //! 일정 거리 이상이면 낭떨어지로 떨어지지 않게 한다.
    //! 기본값은 50.0f
    float       wings_down_jump_height_;

    //크래쉬리포트 전송 타입
    int send_dump_type_;
    //전장 다중 예약 허용
    int battle_ground_multi_reservation_;

#ifdef _DEV_VER
    //==============================================================================================
    // 개발모드 옵션을 위한 클라이언트 변수
    bool m_bEnableAutoKill;             /// 몬스터를 자동으로 죽이는 옵션
    bool m_bEnableAutoCooltimeReset;    /// 스킬 사용후 쿨타임을 리셋

public:
    void SetEnableAutoKill( bool IsEnable ) { m_bEnableAutoKill = IsEnable; }
    void SetEnableAutoCooltimeReset( bool IsEnable ) { m_bEnableAutoCooltimeReset = IsEnable; }
    bool IsEnableAutoKill() { return m_bEnableAutoKill; }
    bool IsEnableAutoCooltimeReset() { return m_bEnableAutoCooltimeReset; }
#endif

public:
    //------------------------------------------------------------------------------ 
    //! 통합샵
    //------------------------------------------------------------------------------ 
    //std::wstring    conveniencestore_connect_ip;
    //std::wstring    conveniencestore_remote_path;
    util::_tstring  conveniencestore_connect_ip;
    util::_tstring  conveniencestore_remote_path;
    unsigned short  conveniencestore_port_num;
    bool            conveniencestore_always_download;
    //------------------------------------------------------------------------------ 


	//==============================================================================================

public:
	CGeneralGameParam();

	inline			BOOL	IsNet()					{ return m_bNet;	}
	
	inline			MAPID	GetStartMapID()			{ return m_Start_MapID; }
	inline			FIELDID	GetStartFieldID()		{ return m_Start_FieldID; }
	inline			MAPID	GetCharSelectMapID()	{ return m_CharSelectMapID; }
	inline			DWORD	GetVillageMapID()		{ return m_dwVillageMapID; }

    inline          int     GetStartTileIndex()     { return m_iStartTileIndex; }
	
	inline			int  	GetShadowType()			{ return m_iShadowType; }
	inline			int  	GetGetCameraRotaion()	{ return m_iCameraRotation; }
	
	inline			int		GetScreenShotMode()		{ return m_iScreenShotMode;	}
	inline			void	SetScreenShotMode(int iScreenShotMode)		{ m_iScreenShotMode = iScreenShotMode;	}

	inline			WzVector    GetvCameraTo()		{ return m_vCameraTo;	}
	inline			BOOL	IsServerDebug()			{ return m_bServerDebug; }
	inline			BOOL	IsInterpolatePosition()	{ return m_bInterpolatePosition;	}

	inline			TCHAR  *GetCinematicFileName()	{ return (TCHAR *)m_szCinematicFileName.c_str(); }
                    TCHAR* characterscene_cinematic_filename() { return (TCHAR*)m_szCharacterSceneCinematicFileName.c_str(); }


    inline          BOOL    IsShowConsole() { return m_bShowConsole; }
    inline          TCHAR  *GetConsoleSetting()	{ return (TCHAR *)m_szConsoleSetting.c_str(); }

	inline          BOOL    GetMoveNakProcess()     { return m_bMoveNakProcess; }

	inline			void	SetStartLuaConsole(BOOL bStartLuaConsole)	{ m_bStartLuaConsole = bStartLuaConsole;	}
	inline			BOOL  	IsStartLuaConsole()		{ return m_bStartLuaConsole; }

	inline			eSPECIAL_MODE 	GetSpecialMode()		{ return m_eSpecialMode; }
	inline			void	SetSpecialMode(eSPECIAL_MODE eMode)	{ m_eSpecialMode = eMode;	}
	
	inline          void    SetUsePackFile( BOOL bIsUse )	{ m_bUsePackFile = bIsUse; }
	inline          BOOL    IsUsePackFile( void )			{ return m_bUsePackFile; }

    inline          void    UnsetMultiThreadLoading()               { m_iMultiThreadLoading = 0; }
	inline			void	SetMultiThreadLoading()					{ m_iMultiThreadLoading = 1; }
	inline          int     IsMultiThreadLoading( void )			{ return m_iMultiThreadLoading; }

	
	void			Load( void );


	//	동영상 촬영용 프리 카메라 모드
	inline			int		IsFreeCameraMode() { return m_iIsFreeCameraMode; }
	inline			void	SetFreeCameraState( BOOL bFreeCamera ) { m_bFreeCameraState = bFreeCamera; }
	inline			BOOL	GetFreeCameraState() { return m_bFreeCameraState; }
	inline			float	GetFreeCameraSpeed() { return m_fFreeCameraSpeed; }
	inline			int		GetFreeCameraTurnSpeed() { return m_iFreeCameraTurnSpeed; }
	
    // TEST RESOURCE SCENE
    inline          BOOL    IsTestResourceScene() { return this->m_bResourceTestScene; }
    inline          BOOL    IsTestResourceGraphic() { return this->m_bTestGraphicResource; }
    inline          BOOL    IsTestResourceMap() { return this->m_bTestMap; }
    inline          BOOL    IsTestResourceEffect() { return this->m_bTestEffect; }
    inline          BOOL    IsTestResourceInterface() { return this->m_bTestInterface; }
	inline			BOOL	IsTestResourceFailedOnly() {return this->m_bTestRscFailedOnly; }

	inline          void    SetTestResourceScene(BOOL bFlag) { m_bResourceTestScene = bFlag; }
    inline          void    SetTestResourceGraphic(BOOL bFlag) { m_bTestGraphicResource = bFlag; }
    inline          void    SetTestResourceMap(BOOL bFlag) { m_bTestMap = bFlag; }
    inline          void    SetTestResourceEffect(BOOL bFlag) { m_bTestEffect = bFlag; }
    inline          void    SetTestResourceInterface(BOOL bFlag) { m_bTestInterface = bFlag; }
	inline			void	SetTestResourceFailedOnly(BOOL bFlag) { m_bTestRscFailedOnly = bFlag; }

    // TEST_UI_SCENE
    inline          BOOL    IsTestUIScene() { return this->m_bTestUIScene; }
    inline          int     GetTestUISceneID() { return this->m_iTestUIScene_ExtID; }

	////////////////////////////////////////////////////////////////
	inline			DWORD	GetUserID()				{ return m_dwUserID; }
	inline          void    SetUserID(DWORD dwUserID) { this->m_dwUserID = dwUserID; }

#ifdef _DEV_VER
    inline bool IsShowTrigger(){ return show_trigger_; }//define:_DH_TRIGGER_LOG_MESSAGE
    inline void SetShowTrigger(bool show_trigger){ show_trigger_ = show_trigger; }//define:_DH_TRIGGER_LOG_MESSAGE
    inline bool IsShowUIName(){ return show_ui_name_; }//KeyWord:_DH_SHOW_UI_NAME
    inline void SetShowUIName(bool show_ui_name){ show_ui_name_ = show_ui_name; }//KeyWord:_DH_SHOW_UI_NAME
    inline bool IsShowStringId(){ return show_string_id_; }//KeyWord:_DEBUG_STRINGIDEX_SHOW
    inline void SetShowStringId(bool show_string_id){ show_string_id_ = show_string_id; }//KeyWord:_DEBUG_STRINGIDEX_SHOW
    inline bool IsShowItemId(){ return show_item_id_; }//define:_SCJ_SHOW_ITEM_ID
    inline void SetShowItemId(bool show_item_id){ show_item_id_ = show_item_id; }//define:_SCJ_SHOW_ITEM_ID
    inline bool IsShowNpcId(){ return show_npc_id_; }//define:_SCJ_SHOW_NPC_ID
    inline void SetShowNpcId(bool show_npc_id){ show_npc_id_ = show_npc_id; }//define:_SCJ_SHOW_NPC_ID
#endif
    inline bool IsShowSSQPlayerName(){ return show_ssq_player_name_; }//define:_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
    inline void SetShowSSQPlayerName(bool is_show){ show_ssq_player_name_ = is_show; }//define:_NA_004756_120504_SHOW_SSQ_PLAYER_NAME

	//============================================================================================== program.ini로 옮겨야함!!!
	// 게임 공용 파라미터  -----------------------------------------------
    inline          int     GetIntervalToWorldServerReconnect() { return this->m_iIntervalToWorldServerReconnect; }
	
	inline          BOOL    IsSpringCamera(){ return m_bSpringCamera; }
	inline          void    SetSpringCamera(BOOL bSpring){ m_bSpringCamera=bSpring;}
	
	// getting
	inline float    GetSpringConst(){ return m_springConst;}
	inline float    GetDampConst(){ return m_dampConst;}
	inline float    GetSpringLen(){ return m_springLen;}

	// setting
	inline  void	SetSpringConst(float springconst){ m_springConst=springconst;}
	inline 	void    SetDampConst(float dampconst){m_dampConst=dampconst;}
	inline  void    SetSpringLen(float springlen){m_springLen=springlen;}

	inline	int		GetGuildRankUpDisplayCount() { return m_iGuildRankUpDisplayCount; }
	inline	BOOL  	GetSoundLogFlag()	    { return m_bSoundLog; }
    
	inline float	GetCameraDistance()		{ return m_fCameraDistance; }
	inline float	GetMaxCameraDistance()	{ return m_fMaxCameraDistance; }
	inline void		SetMaxCameraDistance(float fMaxCameraDistance)	{ m_fMaxCameraDistance = fMaxCameraDistance; }
	
    float wings_down_jump_height() const { return wings_down_jump_height_; }

    // 크래쉬 수집 관련
    int GetSendDumpType() {return send_dump_type_;}
    //전장 다중 예약 허용
    int GetBattleGRoundMultiReservation() {return battle_ground_multi_reservation_;}
};

//=============================================================================================

#define CLIENTINFO CClientInfoParam::Instance()
class CClientInfoParam : public CGameParameter, public util::Singleton<CClientInfoParam>
{
	util::_tstring	m_ClientVersion;

public:

	CClientInfoParam();
	~CClientInfoParam();

	void			Load( void );

	inline 			TCHAR *	GetClientVersion() 		{ return (TCHAR *)m_ClientVersion.c_str(); }
};

//=============================================================================================

#define TESTPARAM CTestGameParam::Instance()
class CTestGameParam : public CGameParameter, public util::Singleton<CTestGameParam>
{
public:
	CTestGameParam();

};

#define GRAPHICPARAM CGraphicParam::Instance()
class CGraphicParam : public CGameParameter , public util::Singleton<CGraphicParam>
{
public:
	CGraphicParam();
	~CGraphicParam();

	void	Load(void);
	
	inline	void	SetBlurValue( int val) { m_iBlurValue = val;}
	inline	int		GetBlurValue() {return m_iBlurValue; }
	inline	void	SetFogValue( int val) { m_iFogValue = val;}
	inline	int		GetFogValue() {return m_iFogValue;};
	inline	void	SetLoadingTextStartPointX( int val) { m_iLoadingText_StartPointX = val;}
	inline	int		GetLoadingTextStartPointX() { return m_iLoadingText_StartPointX;}
	inline  void	SetLoadingTextStartPointY( int val) { m_iLoadingText_StartPointY = val;}
	inline	int		GetLoadingTextStartPointY() {return m_iLoadingText_StartPointY;}
	inline	void	SetLoadingTextCardOut(int val) {m_iLoadingText_CardOut = val;}
	inline	int		GetLoadingTextCardOut() {return m_iLoadingText_CardOut;}
	inline	void	SetTerrainColorRate(float val) { m_fTerrain_Color_Rate = val;}
	inline	float	GetTerrainColorRate() {return m_fTerrain_Color_Rate;}
	inline  float   GetPkFogBegin(){ return m_fPkFogBegin;}
	inline  float   GetPkFogEnd(){ return m_fPkFogEnd;}
	inline  float   GetPkFogRate(){ return m_fPkFogRate;}

    float hero_gauge_velocity(DWORD max_hp, bool sit);
    float camera_eye_height_rider_spread() const { return camera_eye_height_rider_spread_; }
    float camera_eye_height_wings_spread() const { return camera_eye_height_wings_spread_; }

public:
	int		m_iLoadingText_StartPointX;
	int		m_iLoadingText_StartPointY;
	int		m_iLoadingText_CardOut;
	int		m_iFogValue;
	int		m_iBlurValue;
	float	m_fTerrain_Color_Rate;

	float   m_fPkFogBegin;	
	float   m_fPkFogEnd;	
	float   m_fPkFogRate;	

    WzVector m_wvCharScenePos[MAX_CHARACTER_LIST_NUM];
    WzColor  m_wcCharCreateSceneFogColor;


    //------------------------------------------------------------------------------ 
    //! _YMS_SMOOTH_GAUGE_
    //! 속도 밀리세컨드 단위
    //! 첫번재 인자
    //! 0 : STAND, 1 : SIT
    //! 두번째 인자 피통 
    //! 0 : 1000 이하, 1 : 1000~2000, 2 : 2000~3000, 3 : 3000~4000, 4 : 4000 이상
    const static int kGaugeStateKindCount = 2;
    const static int kGaugeHpKindCount = 5;
    float hero_gauge_velocity_[kGaugeStateKindCount][kGaugeHpKindCount];

    float camera_eye_height_wings_spread_; //!< 날개로 날고 있을때 카메라 카메라 눈높이
    float camera_eye_height_rider_spread_; //!< 칼버스 탈것을 타고 날고 있을때 카메라 눈높이
};

#define PROGRAMPARAM CProgramParam::Instance()

class CProgramParam : public CGameParameter , public util::Singleton<CProgramParam>
{
public:
	CProgramParam();
	~CProgramParam();

	void Load(void);

	inline	BOOL	CheckBlockChatRepeat() {return m_bCheckBlockChatRepeat;}
	inline	int		GetBlockChatRepeatTime() { return m_iBlockChatRepeatTime;}
	inline  BOOL    GetEffectBatch()    { return m_bEffectBatch; }
    inline  int     GetAutoEffectRate() { return m_iAutoEffectRate; }

#ifdef _GS_ADD_BIGEVENT_MSG_1122
	inline	BOOL	IsBigEvent() {return m_bBigEventTimer;}
	inline	int		GetQuestCode() { return m_wBigEventQuestCode;}
	inline  BOOL    GetStringCode(){ return m_iBigEventStringCode; }
	inline  BOOL    GetDisplayTime(){ return m_iBigEventDisplayTime; }
#endif//_GS_ADD_BIGEVENT_MSG_1122

	inline  set<int>&	GetNewServerSet()	{ return m_NewServerSet; }
    inline int          GetNewIconWidth()   { return m_IconWidth; }
    inline int          GetNewIconHeight()  { return m_IconHeight; }
	inline int		GetDominationChannel()	{	return m_DominationChannel;	}
    inline int		SetDominationChannel(int DominationChannel) { return m_DominationChannel = DominationChannel; }
#ifdef _DEV_VER
    unsigned int    search_frame_per_update_time() const { return search_frame_per_update_time_; }
    unsigned int    number_of_searching_per_a_frame() const { return number_of_searching_per_a_frame_; }
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    const char* GetReplaceWordForSlangWord() const { return replace_string_.c_str(); }
    DWORD GetReplaceType() const { return replace_type_; }
    inline bool IsEnablePerkSetting() const { return is_enable_perk_setting_; }

    bool is_sound_caching() const { return is_sound_caching_; }

    // 캐릭터 삭제대기 레벨
    DWORD delete_level_for_wait() const { return delete_level_for_wait_; }

    //! OptionParam::m_LanguageType 값과 매칭되는 값이다
    LanguageType nation_type() const { return nation_type_; }

    bool is_block_cash_shop() const { return is_block_cash_shop_; }

private:

	BOOL	m_bCheckBlockChatRepeat;
	int		m_iBlockChatRepeatTime;
	BOOL    m_bEffectBatch;

    int     m_iAutoEffectRate;  // 자동 이펙트 생성 비율 조절

#ifdef _GS_ADD_BIGEVENT_MSG_1122
	BOOL	m_bBigEventTimer;
	WORD	m_wBigEventQuestCode;
	int     m_iBigEventStringCode;
	int     m_iBigEventDisplayTime;
#endif//_GS_ADD_BIGEVENT_MSG_1122


	set<int>		m_NewServerSet;
    int             m_IconWidth;
    int             m_IconHeight;

	int m_DominationChannel;
#ifdef _DEV_VER
    unsigned int search_frame_per_update_time_;
    unsigned int number_of_searching_per_a_frame_;    
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    std::string replace_string_;
    int replace_type_;    
    bool is_enable_perk_setting_;
    bool is_sound_caching_;

    DWORD delete_level_for_wait_;       // 캐릭터 삭제대기 레벨

    //! OptionParam::m_LanguageType 값과 매칭되는 값이다
    LanguageType nation_type_;

    bool is_block_cash_shop_;
};

#define TIMERPARAM CTimerParam::Instance()

class CTimerParam : public CGameParameter, public util::Singleton<CTimerParam>
{
public:
    CTimerParam();
    ~CTimerParam();

    void Load(void);

    inline int GetShowTiredSystem() {return show_tired_system;}
    inline BOOL GetShowEventTimer() {return is_show_event_timer;}
    inline int GetMaxTiredTime() { return max_time_tired_system;}
    inline int GetMaxEventTime() {return max_event_time;}

private:
    int show_tired_system;
    BOOL is_show_event_timer;
    int max_time_tired_system;
    int max_event_time;
};

