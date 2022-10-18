#pragma once
//------------------------------------------------------------------------------
/**
    
    ��Ʈ�� ��Ŷ ó�� �� ���� ���� �� ó�� 

    @author
        - ���翵 
    
    @since

    @remarks
        - 2005.06.07 �ҽ� ����(����)
        - 2005.07.13 ��ų ���� ���� ��Ŷ �߰�(����) 
        CG_SKILL_ACTION_INSTANT_RESULT_BRD,
        CG_SKILL_ACTION_DELAY_START_BRD,
        CG_SKILL_ACTION_DELAY_RESULT_BRD,
        CG_SKILL_ACTION_NAK
		- ����� �⺻ ���� ���� (_HERO_INFO ����) (�翵)
*/

#include "SceneBase.h"
#include "TriggerParser.h"
#include "MapArea.h"
#include "WeaponSoundInfoParser.h"
#include "BattleRoomEntry.h"
#include "Interface/TextDisplayManager.h"
#include "PacketControl/WarMessages_Sync.h"
#include "HeroDef.h"

#include "SkillFacade.h"

using namespace std;
enum eTRIGGER_STATE;
enum eSHORTCUT_AREA_STATE;

extern util::Timer * g_pDelayLeaveFieldTimer;

class SceneBase;
class Character;
class Hero;
class ChatDialog;
class TiredSystemDialog;
class TriggerManEx;




const int MONSTER_FINISH_FLY_CHANCE = 2; // ���Ͱ� ���󰡼� ���� Ȯ��

enum eDebugTextType
{
	 eDebugTextType_None = 0,
	 eDebugTextType_RenderState,	 
	 eDebugTextType_HeroStatus,
	 eDebugTextType_Resource,
	 eDebugTextType_KeyQueue,
	 eDebugTextType_SkillQueue,
	 eDebugTextType_Camera,
	 eDebugTextType_Monster,
     eDebugTextType_Sound, //!<_YMS_SOUND_CACHE
     eDebugTextType_Autobot,

     eDebugTextType_Max,	
};

enum eExpType
{
	eNormalExp = 0,
	eGroupExp,
};

typedef struct _MOVE_INFO
{
	DWORD            m_dwKey;
	DWORD            m_dwDelay;
	BOOL             bSuccess;	
	WzVector         vFrom;
	WzVector         vTo;
	WzVector         vActualTo;

} MOVE_INFO;

typedef STLX_LIST<MOVE_INFO> MOVE_INFO_LIST; 

typedef struct _EXP_POPUP_INFO
{
	EXPTYPE					m_dwExpOffset;	
	int						m_nBonusExpRatio;
	DWORD                   m_dwCharacterKey;
} EXP_POPUP_INFO;

typedef STLX_LIST<EXP_POPUP_INFO> EXP_INFO_LIST; 

enum
{
	eSCREEN_SHOT_FULL,
	eSCREEN_SHOT_NOT_UI,
	eSCREEN_SHOT_NOT_UI_AND_HERO,
};

#include "BattleSceneEventText.h"

class BattleRoomEntry;

class BattleScene : public SceneBase
{
    cSceneEventText m_EventText;

public:
	enum PACKET_STATUS_BATTLESCENE                        
	{ 
		PACKET_STATUS_BATTLESCENE_NONE					= 0, 
		PACKET_STATUS_BATTLESCENE_PARTY_MEMBER_WARP		= 1,        //����� ��Ƽ�� ��ó�� ����
		PACKET_STATUS_BATTLESCENE_PARTY_MEMBER_SUMMON	= (2 << 1), //��Ƽ���� ��� ��ȯ        
	};

	BattleScene( void );
	~BattleScene( void );

protected:
	static DWORD			ms_RoomKey;	// ���� �� Ű
	static BattleRoomEntry	ms_BattleRoomEntry;

	static int   m_SceneCount;

	int						m_iDbgTextType;
	BOOL					m_bDeadFlag;

	DWORD                   m_dwFieldID; // ���� �ʵ� ���̵�.

	static BOOL				m_bWaitEnterSync;
	static BOOL             m_bDrawWait;

	BOOL				    m_bPlayerAck;
	HANDLE				    m_hTexture;
	// �������� 
	DWORD               m_dwBattleTimeSerial;
	HANDLE				m_hMissionClearTexture;
	//	ī�޶� ����
	BOOL				m_bUseAutoCamera;
	BOOL				m_bIsMouseLBClick;
	eTRIGGER_STATE		m_eTrigerState;
	eSHORTCUT_AREA_STATE m_eShorcutAreaState;
	int					m_iMapObjectID;
	St_PE_BLUR          m_oldBlur;
	St_FogParameter     m_oldFogParameter;
	int                 m_iBrightDelay;
	int                 m_iGrayScaleDelay;
	WzColor             m_GrayScaleColor;
	DWORD				m_dwMissionClearTick;
    BOOL                m_bCameraDetech;
	ChatDialog *		m_pChatDlg;

	TiredSystemDialog*	m_pTiredSystemDlg;

    // ���ݰ�� �׷�ȭ 
    DWORD               m_GoupId;

	BOOL				m_bComebackFromLobby;	// �κ񿡼� ������ �°��� üũ�ϴ� ����

	static BOOL			m_bLobbyFlag;			// ���� �κ����� check ���ִ� ���� �÷���
	static BOOL			m_bLobbyPendingFlag;	// �κ�� ������ �� �÷���
	static BOOL			m_bLobbyOutPendingFlag;	// �κ� ���������� �� �÷���

	// ����ġ ���߿� ����ֱ����� ����
	EXP_INFO_LIST       m_ExpList;

	// ����׿� �̵�����
	MOVE_INFO_LIST      m_MoveInfoList;
	CPathExplorer       *m_pTestMoveInfoExplorer;

	BOOL                m_bRenderLightDirection;
	WzVector            m_vTestDir;

	enum eBlindType
	{
		BLIND_TYPE_NONE,
		BLIND_TYPE_BLACK,
		BLIND_TYPE_WHITE,
	};
	eBlindType			m_BlindType;
	int                 m_iCurBlindRate;
	BOOL                m_bBlind;


	std::vector<DWORD>	m_VecQuestArea;	// üũ�ؾ��� ����Ʈ ����.


	float	m_fFog_weight;
	float	m_fBlur_weight;

	static util::Timer	m_StayTimer;
	static int			m_iGameConnectedTime;

	TriggerManEx* m_pTriggerMan;
	BOOL		  m_bTriggerInit;


	DWORD				m_dwGambleTime;	
	BOOL				m_bGambleStart;

public:
	virtual void		Activate( void );				//	Ȱ��ȭ �Ǿ��� ��
	virtual BOOL		Init( void );					//	�ʱ�ȭ, �ε�
	virtual BOOL		Process( DWORD dwTick );		//	���� ���
	virtual BOOL		Render( DWORD dwTick );			//	���� ������
	virtual BOOL        PreInterfaceRender(DWORD dwTick); // �������̽� ����������
	virtual BOOL        PostInterfaceRender(DWORD dwTick); // �������̽� ���ķ�����
	virtual void		Release( void );				//	����
	virtual void		ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��
	virtual void		ParsePacketInChat( MSG_BASE * pMsg ); //	ü�� ���� ��Ŷ ó��
	virtual BOOL        SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam ,SI_MESSAGE *stMessage);
	virtual void        KeyManagerProc(DWORD wParam,DWORD lParam);
	virtual void        AddMonsterWithPacket(MONSTER_RENDER_INFO *pRenderInfo,float fHPMultiplier,BOOL bEnter = FALSE,DWORD dwMapObjectID = 0);
    virtual void        AddNPCWithPacket(MERCHANT_RENDER_INFO* render_info);
    

	virtual void        AddTotemMonsterWithPacket(TOTEMNPC_RENDER_INFO *pRenderInfo,float fHPMultiplier);
	virtual void		ProcessFade(DWORD dwTick);
	virtual void        OnDeviceReset();

    virtual void        OnRecvVkrAttack(DWORD	dwAttackerKey,DWORD	dwClientSerial,DWORD dwTargetKey,DAMAGETYPE	wDamage,DWORD dwTargetHP,BYTE byEffect);
	
    virtual void        DoSomethingPacket(BYTE byProtocol){};
	virtual int			OnEndRendering( DWORD dwTick );

public:
    static BOOL         IsWaitEnterSync() { return m_bWaitEnterSync; }
	static void			SetHeroInfo(BYTE byClassCode);	// ĳ���� ������ ó������ �����Ҷ������.�̱ۿ� 
	static void			SaveHero(Hero * pHero);         // ���� ����� ��������� �̿�.
	static const HERO_INFO *	GetHeroInfo();
	static void			SetRoomKey(DWORD dwKey);
	static DWORD		GetCurRoomKey();
	static void			SetPendingBattleRoomEntry(BattleRoomEntry * IN pBattleRoomEntry);   //// ����� / ����, ��Ƽ�� �ʴ� ���� �� ���
	static BattleRoomEntry* GetPendingBattleRoomEntry();
	//////////////////////////////////////////////////////////////////////////
	// �κ����
	BOOL				IsComeBackFromLobby() const;

	static BOOL			IsLobby();
	static BOOL			IsLobbyInPending();
	static BOOL			IsLobbyOutPending();

	static VOID			SetLobbyFlag(BOOL bFlag);
	static VOID			SetLobbyInPending(BOOL bFlag);
	static VOID			SetLobbyOutPending(BOOL bFlag);
    static void         CallbackWGPL(void);
	//////////////////////////////////////////////////////////////////////////


    DWORD               MakeGroupID();

	void				SetFieldID(DWORD dwFieldID);
	void				BackToTheVillage(CODETYPE VillageCode = 0);
	BOOL                IsProjectileDamage(DWORD dwSkillCode);
	//BOOL                CreateProjectileSkillResult(DAMAGETYPE wDamage,DWORD dwTargetKey,DWORD dwTargetHP,DWORD timeSerial,DWORD dwSkillCode,DWORD dwAttackerKey);

	static void			BackToTheVillageSyn();
	static void			BackToTheVillageTransact();
	static void			LeaveDominationTransact();

	void                SetMonsterHPMultiplier(float fHPMultiplier);
	float		        GetMonsterHPIncreaseRatio();

	void				SettingNPC(MAPID MapCode ,FIELDID FieldID);

    //single test
	void                QueueSkillAction(PLAYER_ACTION *pAction,Character *pChar);

    void AddSkillResult( Character* pChr, SkillFacade::ResultData* result_data );
    void AddSkillFieldEffect( Character* pChr, SkillFacade::ResultData* result_data );
    void AddAbilityResult( Character* pChr, Character* pTarget, DWORD dwSkillCode, SKILL_RESULT_CODE* pResult, cAttackResultData& ResultData );
    void AddSplitDamageResult( Character* character, ATTACK_RESULT* attack_result, int hit_count );
    bool GetSplitHitCount( SLOTCODE skill_class_code, int& hit_count );

	void                AddReflectDamageResult(char *pPacket,Character *pChr);	



    virtual DWORD				AddBattleTimeSerial();
	virtual DWORD				GetBattleTimeSerial();

	void                SetFogParam( BYTE bR, BYTE bG, BYTE bB, float fFogStart, float fFogEnd, float fFogRate );

	void                SetGrayScaleDelay(int iGrayDelay,WzColor color);
	void				SetGrayScale();	
	void				SetBrightDelay(int iBrightDelay);	
	void				SetBrightBlur();	

	static 	void		SendPlayerEnterSyn(BOOL bWaitDraw = TRUE);

    void SetEventScript( int iScript ) { m_EventText.SetEventScript(iScript); }

protected:
	virtual void		OnRecvCG_CONNECTION( MSG_BASE * pMsg );
	virtual void		OnRecvCG_CHARINFO( MSG_BASE * pMsg );
	virtual void		OnRecvCG_MAP( MSG_BASE * pMsg );
	virtual void		OnRecvCG_ZONE( MSG_BASE * pMsg );
	virtual void		OnRecvCG_ZONE_EX(MSG_BASE* pMsg);
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	virtual void        OnRecvCG_EVENT( MSG_BASE* message );
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT

	virtual void		OnRecvCG_Vendor(MSG_BASE* pMsg);

	virtual void		OnRecvCG_WareHouse(MSG_BASE* pMsg);

	virtual void		OnRecvCG_SYNC( MSG_BASE * pMsg );
	virtual void		OnRecvCG_BATTLE( MSG_BASE * pMsg );
	virtual void		OnRecvCG_STYLE( MSG_BASE * pMsg );
	virtual void		OnRecvCG_SKILL( MSG_BASE * pMsg );
	virtual void		OnRecvCG_CONVERSATION( MSG_BASE * pMsg );
	virtual void		OnRecvCG_STATUS( MSG_BASE * pMsg );
	virtual void		OnRecvCG_TRIGER( MSG_BASE * pMsg );
	virtual void        OnRecvCG_GM( MSG_BASE * pMsg );
    virtual void        OnRecvCG_Party( MSG_BASE * pMsg );
	virtual void        OnRecvCG_Summon( MSG_BASE * pMsg );
	virtual void		OnRecvCG_EVENT_INVENTORY( MSG_BASE * pMsg );
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	virtual void        OnRecvCG_EVENT_PEAKTIME_NOTIFY( MSG_BASE* message );
	virtual void        OnRecvCG_GMEVENT_PEAKTIME_EVENTLIST_CMD( MSG_BASE* message );
	virtual void        OnRecvCG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD( MSG_BASE* message );
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	virtual void        OnRecvCG_AREACONQUEST(MSG_BASE *pMsg);
	virtual void        OnRecvCG_MISSION(MSG_BASE *pMsg);
	virtual void        OnRecvCW_CHAT(MSG_BASE *pMsg);	
	virtual void		OnRecvCG_SYNC_In_Lobby( MSG_BASE * pMsg );
	virtual void		OnRecvCG_SYNC_In_Lobby_Pending( MSG_BASE * pMsg );
	virtual void		OnRecvCG_SYNC_In_Lobby_Out_Pending( MSG_BASE * pMsg );
	virtual void		OnRecvCG_SSQ_CTRL_CMD(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header);
	virtual void		OnRecvCG_SYNC_COMPOSE(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header);
    virtual void        OnRecv_SpreadWings(const MSG_CG_SYNC_WINGS_CONTROL_CMD* pk);
    virtual void        OnRecv_FurlWings(const MSG_CG_SYNC_WINGS_CONTROL_CMD* pk);

    //! 2011.1.10 / i4u4me / �Ӽ� ������ �Ǹ鼭 �߰�/����� ��Ŷ 
    virtual void		OnRecv_MSGSUB_BATTLE_MONSTER_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_MONSTER_ATTACK_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_MONSTER_JUMP_ATTACK_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_BATTLE_PLAYER_ATTACK_BRD(nsSync::MSGSUB_BATTLE_PLAYER_ATTACK_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD(const nsSync::MSGSUB_BATTLE_PLAYER_AREA_ATTACK_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_BRD(const nsSync::MSGSUB_STYLE_PLAYER_ATTACK_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD(nsSync::MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_ACTION_DELAY_START_BRD(nsSync::MSGSUB_SKILL_ACTION_DELAY_START_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD(nsSync::MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD* recv_packet, BYTE* addition_data);
    virtual void		OnRecv_MSGSUB_SKILL_CASTING_CANCEL_BRD(const nsSync::MSGSUB_SKILL_CASTING_CANCEL_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_PERIODIC_DAMAGE_BRD(const nsSync::MSGSUB_SKILL_PERIODIC_DAMAGE_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_REFLECT_DAMAGE_BRD(const nsSync::MSGSUB_SKILL_REFLECT_DAMAGE_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_EFFECT_BOMB_BRD(const nsSync::MSGSUB_SKILL_EFFECT_BOMB_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_SELF_DESTRUCTION_BRD(const nsSync::MSGSUB_SKILL_SELF_DESTRUCTION_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_TRANSPARENT_BRD(const nsSync::MSGSUB_SKILL_TRANSPARENT_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_SUCTION_BRD(const nsSync::MSGSUB_SKILL_SUCTION_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_VITAL_SUCTION_BRD(const nsSync::MSGSUB_SKILL_VITAL_SUCTION_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD(nsSync::MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD* recv_packet, BYTE* addition_data);

    //! 2011.4.25 / i4u4me / ä�� 
    virtual void		OnRecv_MSGSUB_COLLECT_ACTION_BRD(const nsSync::MSGSUB_COLLECT_ACTION_BRD* recv_packet);
    virtual void		OnRecv_MSGSUB_COLLECT_CANCEL_BRD(const nsSync::MSGSUB_COLLECT_CANCEL_BRD* recv_packet);

	virtual void		OnRecvCG_COLLECT(MSG_BASE* pMsg );
	void				OutputCollectionResultMsg(DWORD ResultCode);

	void				OutputSummonResultMsg(DWORD ResultCode);

public:

	void				AddAttackResultQueueAction( cAttackResultData& ResultData, DWORD dwTimeSerial, DWORD dwAttackId );
    //void				AddAttackResultQueueAction( ATTACK_RESULT_TARGET_INFO *pResult,DWORD dwTimeSerial,DWORD size,DWORD dwAttackId);
	void				InitQuestAreaInfo();

	void				SetShowInterfaceForObserverMode( BOOL bShow );

	// �ؽ�Ʈ ���
protected:
	void				RenderText( DWORD dwTick );
	void				RenderDamage(DWORD dwTick);
	void                RenderMissionClearMessage();
	void                RenderBlindEffect();

protected:
	virtual void		ProcessKeyInput_GM();
	virtual void		ProcessKeyInput_HotKey();
	virtual void		ProcessKeyInput_Hero(DWORD dwTick);
    virtual void		ProcessKeyInput_Quick();
	virtual void		ProcessKeyInput();
	virtual void		ProcessUnits(DWORD dwTick);
	void				ProcessCamera(LPDIMOUSESTATE2 pMouseState,DWORD dwTick);
	void				ProcessEffect(DWORD dwTick);	
	void				ProcessTrigger();
	void                ProcessExpList();
	void                ProcessEventScript(DWORD dwTick);
	void				ProcessQuestArea();
	void				ProcessQuestTime();

public:
	TriggerManEx*		GetTriggerMan(){return m_pTriggerMan;}


protected:
	BOOL                CheckHeroInArea(DWORD dwAreaID);
    BOOL				CheckHeroInShortcutBattleArea(TCHAR* sAreaID, DWORD dwAreaID);

	void				RenderClone();
	int					GetCharacterHP(DWORD dwID);	
	void				ProcessSpecialEffect(DWORD dwTick);	
    void                ProcessBlind(DWORD dwTick);
	void				ProcessGrayScaleDelay(DWORD dwTick);	
	void                RestoreRenderState();
	void                ReleaseInterfaceResource();
	void                ReleaseObjects();
	
	void				PrintYoungManProtectMessage();
	void				PrintPcBangBanMessage();
	void				UpdateGameStayTime();

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	virtual	void		RenderGameRatingMark();
#endif // _JBH_ADD_AGE_LIMIT_MARK

    void                UpdateItemOwner();

	void				UpdateItemShareSystem();
	void				NoticeBattleZoneItemShareSystem();

	BOOL				ChangeStatus(DWORD dwObjKey,BYTE byConditionType);

	void				PopupExp(EXPTYPE expoffset , int bonusExpRatio = 0, int ExpType = eNormalExp);

	void				InitQuestInfo();

	void				RenderGMResultCode(DWORD dwCode);
	void				SetSSQRewardInfo(const MSG_CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD* pRecvPacket);

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
public:
    void			SetCurrentRadarLayer(int current_layer);
protected:
    void			InitRadarLayer(MAPCODE map_code);
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

public:
	// ����׿�
	void                ProcessMoveInfo(DWORD dwTick);
	void                RenderMoveInfo();
	void                RegisterMoveInfo(HONGPOS vCur,HONGPOS vDest,DWORD dwKey,BOOL bThrust);
	void                RenderLightDirection();
	void                ShowLightDirection();
	void                ProcessRenderOption(LPDIMOUSESTATE2 pMouseState);
	static void         ShowErrorCodeSkill(DWORD code);
	
	//	������ �Կ��� ���� ī�޶�
	WzVector			m_FreeCameraVector;

	
private:
	// ����׿�
	void                _renderDbgText(DWORD dwTick);
	void                _renderDbgInfoRenderStates(int& x, int& y);
	void                _renderDbgInfoHeroStatus(int& x, int& y);
	void                _renderDbgInfoResourceStatus(int& x, int& y);
	void				_renderDbgInfoKeyQueueNum(int& x, int& y);
	void				_renderDbgInfoSkillQueueNum(int& x, int& y);
	void                _renderDbgInfoCameraSpring(int& x, int& y);
	void                _renderDbgInfoMonsterInfo(int& x, int& y);
    void                _renderDbgInfoSound(int& x, int& y);
    void                _renderDbgInfoAutobot(int& x, int& y);

    void                RenderDbgMonsterRange();

	//���� ���� ��ų ����� 
private:
	static int			m_iScreenShotState;
	float               m_fMonsterHPMultiplier;
	DWORD               m_dwOldSelectId;
    DWORD               pre_object_key;
    //int                 aggro_player_count;
    //AggroValueInfo      aggro_value_info[MAX_TARGET_NUM];

#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
public:
    static WzID last_loaded_submapid_;
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP

public:
	static void			SetScreenShotState( int iState );
	static int			GetScreenShotState( void );

	void				InserCenterTextMessage( char * szText, DWORD eType );

	// ���� �ε���
protected:
	int				  m_iPrevSectorIndex;
public:
    void      SendHeroSectorIndex();

	static KEYTYPE	  m_CurLobbyKey;
	static DWORD	  m_dwVillCheckSum;
    static DWORD      m_dwVillCheckSum_WMO;
public:
	static KEYTYPE    GetCurLobbyKey(){return m_CurLobbyKey;}
    static void ProcessKeyInput_PartyMember(int party_member_number);
    static void ShowRadirDialog();


#ifdef _INFINITE_MAP_LOADING_TEST
	BOOL	m_bReadyMapLoading;
	DWORD	m_dwStartTick;
	std::vector<int> m_vecLoadingIndex;
#endif
public:
	static BOOL      m_bMissionReward;
    static bool     m_bPendingBackToSomeWhere;


	TextDisplayManager	m_TextDisplayManager;
};


//------------------------------------------------------------------------------
/**

*/
inline DWORD BattleScene::MakeGroupID()
{
    return ++m_GoupId;
}

WORD GetPlayerSize(BYTE *pPacket,int nRenderInfoSize);

