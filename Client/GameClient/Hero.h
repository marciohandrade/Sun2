//------------------------------------------------------------------------------
/**

    월드 상의 자기 유저 캐릭터 특성 정의 및 슬롯 처리  
    @author

    @since

    @remarks
        -2005.06.07 소스 정리(지완)
        - 

*/

#pragma once

#include "player.h"
#include "HeroDef.h"
#include <boost/shared_ptr.hpp>
#include "WGPL/WGPLShuffle.h"

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "ToggleSkill.h"
#else
#include "AuraSkill.h"
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

class SkillAttrCalculator;
class QuickContainer;

class PlayerAttributes;
class SCSlotContainer;
class QuickContainer;
class SummonSkillContainer;
class HeroEquipmentContainer;
class StyleContainer;
class SummonSKillContainer;
class uiHelmetShow;


class Hero 
    : public Player
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    , public ToggleSkill
#else
    , public AuraSkill
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
{
public:
	enum ITEM_SLOT_IDX
	{
		Inventory = 0,
		Quick,
		Skill
	};
	Hero();
	virtual ~Hero();

private:
	virtual void				InitContainer();
	virtual void				ReleaseContainer();

public:
	virtual void                RenderInfo();
	virtual void				RefreshAppearRender(BOOL refresh_bone = FALSE);
	virtual void				OnDead();

	virtual BOOL				Process( DWORD dwTick );
	virtual void                ShowConflictMessage();

	virtual BOOL				Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void				Destroy();
	virtual int					DoAction(PLAYER_ACTION *pAction);
	virtual BOOL				CheckQueueActionCondition();
	virtual BOOL				ProcessNextAction();	
	virtual void				TransForm();
	void                        HeroTransForm();

	virtual void				UnTransForm();
	virtual SCSlotContainer *	GetSlotContainer( SLOTIDX slotIdx );
	QuickContainer *			GetQuickContainer();
	HeroEquipmentContainer *	GetHeroEquipContainer();
    BOOL						CanDoQueueAction(PLAYER_ACTION *pAction);
	
	void						SetHeroInfo( const HERO_INFO * pHeroInfo );
	void                        SetCannotMoveFlag(BOOL bFlag);
	BOOL                        GetCannotMoveFlag();

	BOOL                        IsLockInput();

	VOID						GetInventoryTotalInfo(  BYTE * OUT pItemSteam, WORD & OUT wSize );
	VOID						GetQuickTotalInfo( QUICK_TOTAL_INFO & OUT rQuickInfo ); 
	
	VOID						ResizeInventoryTotalInfo(BYTE byExtraInventoryTabCount);

	VOID						SetInventoryTotalInfo( const INVENTORY_TOTAL_INFO & rItemInfo );
	VOID						SetInventoryTotalInfo( const BYTE * IN pItemStream, WORD wSize );
	VOID						SetSkillTotalInfo( const SKILL_TOTAL_INFO & rSkillInfo );
	VOID						SetQuickTotalInfo( const QUICK_TOTAL_INFO & rQuickInfo );
	VOID						SetStyleTotalInfo( const STYLE_TOTAL_INFO & rStyleInfo );


	void						SetFreeInitState(BYTE count);
	BYTE						GetFreeInitState();


	VOID						UpdateTotalItemDate();

	virtual  BOOL               Render();
	
protected:
	SCSlotContainer *			m_pBankContainer;
	SCSlotContainer *			m_pInventoryContainer;
	QuickContainer *			m_pQuickContainer;
	StyleContainer *            m_pStyleContainer;
	int                         m_iCanNotMoveTime;
	SummonSkillContainer *      m_pSummonContainer;
	HeroEquipmentContainer *	m_pHeroEquipContainer;



	int							m_iComboCount;
	DWORD						m_dwKeyboardMovePacketSendDuration; // 키보드 이동 패킷 전송 주기
	DWORD						m_dwKeyboardMovePacketSendTime;		// 키보드 이동 패킷 전송시간.
	BOOL						m_bCompleteTransForm;

    PlayerAttributes*		    m_pPlayerAttriburtes;

public:
    PlayerAttributes*	        GetPlayerAttribute() { return m_pPlayerAttriburtes; }

	// Style 1~3타 카운트
	int							GetComboCount();
	void						SetComboCount(int iCount);
	
	// 임시
	virtual BOOL				SetPosition( const WzVector& wvPos,int index = -1);

	BOOL                        IsCompleteTransForm();//변신이 완료 됐나 
	BOOL                        IsCancelTransForm(DWORD dwSkillClassCode);

    BYTE                        GetAttackPropensity();
    BOOL                        SendUseSkillPacket( DWORD SkillCode );

	BOOL						SendPacket( void *pBuf, int iLength );
    BOOL						SendStopMovePacket();
    bool                        SendStandUp();
	void						SetKeyboardMovePacketSendDuration(DWORD dwDuration);
	DWORD						GetKeyboardMovePacketSendDuration();
	void						SetKeyboardMovePacketSendTime(DWORD dwTime);
	void						AddKeyboardMovePacketSendTime(DWORD dwTime);
	DWORD						GetKeyboardMovePacketSendTime();
	virtual DWORD				GetWeaponKind();


	// 스킬 사용 유무 관련
	virtual BOOL                CanUseSkill(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);


    int                         GetBonusSkillCooltime();
    int                         GetRatioBonusSkillCooltime();

	BOOL						CanUseSkillWithoutCooltime(SLOTCODE skillID, BOOL bShowFailMessage = FALSE, bool is_check_monster = false);
	virtual	BOOL				CanUseSkillLimitMP(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
	
	BOOL                        CanUseAction(SLOTCODE skillID);
	BOOL                        CanUseEmotion(SLOTCODE skillID);

	BOOL						CanUseSkillLimitLevel(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
	BOOL						CanUseSkillLimitWeapon(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
	BOOL						CanUseStyleLimitWeapon(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BOOL                        CanUseSkillLimitFP(SLOTCODE skillID,BOOL bShowFailMessage = FALSE);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	BOOL                        CanUseSkillType(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
	BOOL						CanUseSkillSlotState(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);


	BOOL                        CanUseItem(const BASE_ITEMINFO *pItemInfo,BOOL bShowFailMessage = FALSE);
    BOOL                        CanUseChangeGenderItem();

    //------------------------------------------------------------------------------
    // Qualifier : 해당 ac 아이템을 사용할수 있는지 여부
    // Parameter : WORD ac_code
    // Parameter : BYTE ac_reference_id
    //------------------------------------------------------------------------------
    bool                        CanUseACItem(const BASE_ITEMINFO *pItemInfo);

	MONEY						GetMoney( void );
	void						SetMoney(MONEY Money);
	virtual DWORD				GetMaxHP();
	virtual DWORD				GetMaxMP();
	virtual void                SetHP(DWORD dwHP);
	virtual void                SetMP(DWORD dwHP);
	virtual void				SetMaxSD(DWORD dwMaxSD);
	virtual void				SetSD(DWORD dwSD);
	virtual DWORD				GetSD();
	virtual DWORD				GetMaxSD();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual void                SetMaxFP(const PLAYERFP& max_fp);
    virtual void                SetFP(const PLAYERFP& fp);
    virtual PLAYERFP            GetFP();
    virtual PLAYERFP            GetMaxFP();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual void                SetHPInfo(HPINFO *pInfo);

	virtual	int					AddSkillEffect(SKILL_EFFECT *pEffect);
    virtual BOOL		        DeleteSkillEffect(int iEffectID);

	void						UpdateDominationRewardAttr(SkillAttrCalculator &SkillCalc);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void                        UpdateGuildFacilityAttr(SkillAttrCalculator& skill_calc);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	float						GetProcessMultiplier();
	virtual float               GetAttackSpeedModifier();
	virtual float               GetMoveSpeedModifier();

	virtual void				SetMaxHP( DWORD iHP );
	virtual void				SetMaxMP( DWORD iMP );
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
	virtual void				SetAttackSpeedRatio(int iSpeed);
	virtual void				SetMoveSpeedRatio(int iSpeed);
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER

	void                        SetCustomAttackSpeed(float fSpeed);
	void                        SetCustomMoveSpeed(BOOL bUse,float fSpeed = 1.0f);

	BOOL                        CanEquipItem();

	BOOL                        SendUnTransFormPacket();

	// 이벤트
	virtual void                OnFootStep(BOOL b2D = FALSE);
	void						OnFootStepPlaySound();

	virtual void				OnWaterWave();
	virtual void				OnWaterEllipse();
	
	virtual  void				OnDamage(const DAMAGE_PRAM &rDamage);
	virtual void				OnLevelUp();
    void                        SetRemainStatPoint( int Point )  { GetCharInfo()->m_iRemainStat = Point; }
    void                        SetRemainSkillPoint( int Point ) { GetCharInfo()->m_iRemainSkill = Point; }

	void						OnUseItemWithCoolTime(CODETYPE ItemCode);

	virtual BOOL				OnUseItem(DWORD dwItemCode);	

    void                        RefreshEquipData();

    void                        UpdateItemAttrCalculator();
    void                        UpdateSkillAttr();
    void                        UpdateActiveSkillEffectAttr(SkillAttrCalculator &calc);
    void                        ItemAttrCalculatorByItemslot(SCSlot& item_slot, bool is_equip, POSTYPE position);

    int                         GetItemScore(); // override


	void						RefreshStyleQuick();

	BOOL						IsShowFullMap( void ) { return m_bShowFullMap; }
	BOOL						ToggleFullMapWindow( void ) { m_bShowFullMap ^= TRUE; return m_bShowFullMap; }
	void						ShowFullMapWindow( BOOL bShow ) { m_bShowFullMap = bShow; } 
	int							GetEmptyUseSlot();


	void						SetHeroPetInfo( BASE_PLAYERINFO* pPlayerInfo);
	virtual void				SetPetAttributes( POSTYPE pos,BYTE bIsFullness , BYTE byValue);
	virtual void				DeletePetSummon();												//펫제거 
	void						UpdatePetInfo( POSTYPE pos );

	void						UsePetFeed();													//펫사료사용
	POSTYPE						GetPetItemPos(){ return m_PetInfo.HERO_.m_PetItemPos;}	
	void						UpdatePetSkillAttr(SkillAttrCalculator &SkillCalc);
#ifdef _NA_004035_20120227_COSTUME_ITEM
    void                        UpdateCostumeInfo(SkillAttrCalculator& SkillCalc);
#endif// _NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	void                        UpdateDecorateCostumeInfo(SkillAttrCalculator& SkillCalc);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
    void                        UpdateBadgeItemInfo(SkillAttrCalculator& SkillCalc);

	
	//펫 자동소환해제 관련
	void						PetChangeON( POSTYPE pos ){m_PetInfo.HERO_.m_bChangePet = TRUE; m_PetInfo.HERO_.m_PetChangePos=pos;}
	void						PetChangeOFF(){m_PetInfo.HERO_.m_bChangePet = FALSE;}
	POSTYPE						PetChangePos(){ return m_PetInfo.HERO_.m_PetChangePos;}
	BOOL						IsPetChange(){ return m_PetInfo.HERO_.m_bChangePet;}
		
	RIDER_INFO					m_RiderInfo;
    virtual void				RefreshRider();
	virtual void				CreateRider(DWORD dwID,BOOL bEffect=FALSE);
	virtual void				DestroyRider();
	void						ShowRiderInven(BOOL bOepn);
    void						SendRiderReleasePacket(bool is_battle=true);

    bool    SendWingsControlByAccessory(bool is_spread, bool is_force_send);
    bool    SendWingsControlByRider(bool is_spread, bool is_force_send);
    bool    SendZONE_EX_QUEST_PORTAL_ENTER_SYN();

private:
    bool    SendWingsControl(eSlotContainerIndex container_index, POSTYPE slot_at, bool is_spread);

public:
	void						SetEtherItemLock(BOOL bZoneMove, BOOL bLock);
	void						SetEtherChargePos(POSTYPE pos) { m_EtherInfo.byEtherChargePos = pos; }
	void						SetEtherOptionValue(int iValue) { m_EtherInfo.iValue = iValue; }
	void						SetEtherWaitPacket(BOOL bWait) { m_EtherInfo.bWait = bWait; }
	void						ClearEthreInfo();
	virtual void				SetEtherActive(BYTE byIndex);
	
	//에테르 자동소환해제 관련
	void						EtherChangeON(POSTYPE pos) { m_EtherInfo.bChange = TRUE; m_EtherInfo.ChangePos = pos; }
	void						EtherChangeOFF() { m_EtherInfo.bChange = FALSE; }
	const ETHER_INFO&			GetEthreInfo() { return m_EtherInfo; }
	ETHER_INFO					m_EtherInfo;


#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
	void							SetPCRoomStatus( CHANGE_PCROOM_STATUS::eStatus ePCBangStatus )	{ m_ePCBangStatus = ePCBangStatus; }
	CHANGE_PCROOM_STATUS::eStatus	GetPCRoomStatus()												{ return m_ePCBangStatus; }
#endif

    
	eTRIGGER_STATE				m_eTriggerState;

	typedef stdext::hash_map<DWORD, eTRIGGER_STATE>			TRIGGERSTATE_MAP;	
	typedef std::pair<DWORD, eTRIGGER_STATE>					TRIGGERSTATE_PAIR;

	typedef stdext::hash_map<DWORD, eSHORTCUT_AREA_STATE>	SHORTCUT_AREA_STATE_MAP;
	typedef std::pair<DWORD, eSHORTCUT_AREA_STATE>			SHORTCUT_AREA_STATE_PAIR;

	TRIGGERSTATE_MAP		    m_MapTriggerState;
	SHORTCUT_AREA_STATE_MAP		m_MapShorcutAreaState;

	BOOL                        m_bCustomAttackSpeed;
	float                       m_fCustomAttackSpeed;
	BOOL                        m_bCannotMove;


	BOOL                        m_bCustomMoveSpeed;
	float                       m_fCustomMoveSpeed;

	int							m_iHeroTurnState;

public:
	eTRIGGER_STATE				GetTriggerState( DWORD iAreaID  ) ;
	void						SetTriggerState( DWORD iAreaID,eTRIGGER_STATE TriggerState );

	eSHORTCUT_AREA_STATE		GetShortcutState( DWORD dwAreaID  ) ;
	void						SetShortcutState( DWORD dwAreaID, eSHORTCUT_AREA_STATE ShortcutState );
	
	void                        InitTriggerState();
	void						InitShortcutState();
	BOOL                        IsKeboardMoving();

	BOOL                        IsOldKeboardMoving();

public:
	DWORD						m_dwPacketStatus;

	BOOL						m_bShowFullMap;

	TAB_TARGET_MONSTER_vector	m_vectorTargetMonster;
	TAB_TARGET_MONSTER			m_CurTargetInfo;

	//	주인공 IDLE 상태일 때, 회전 정보( 회전 안함, 왼쪽 회전, 오른쪽 회전 )
	//	위에 enum 값 있음 eHERO_TURN_STATE
	void						SetHeroTurnState( int iTurnState ) { m_iHeroTurnState = iTurnState; }
	int							GetHeroTurnState() { return m_iHeroTurnState; }

protected:
	BOOL						m_bAccountState;
	BOOL						m_bIsScreenShotMode;
	BOOL						m_bForceAttack;		//강제 공격 
 
	
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#else
    void						OutputMsg_ForceWithDrawCampSoldier(DWORD dwLevel);
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER


public:


    virtual bool    IsHero() { return true; }
	BOOL                IsForceAttack();//강제 공격 관련
	void                SetForceAttack(BOOL bForce);
	BOOL                CanUseStyle();

	void						SetAccountState( BOOL bState );
	BOOL						GetAccountState( void );

	virtual void                HideHelmet(BOOL bFlag,BOOL bRefresh = TRUE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	virtual void                HideWing(BOOL bFlag,BOOL bRefresh = TRUE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    virtual void                HideCostume(BOOL bFlag,BOOL bRefresh = TRUE);
#endif


	virtual void                SetChaoState(BYTE bState);


	void						SetScreenShotMode( BOOL bScreenShotMode ) { m_bIsScreenShotMode = bScreenShotMode; }
	BOOL						IsScreenShotMode( void ) { return m_bIsScreenShotMode; }	

public :
	BOOL						IsParty( void );
	BOOL						IsReachMaxLevel();


	//인벤에 있을때 효과 발동(옵션,이펙트)하는류의 아이템 이펙트 처리 (함수명 변경 필요)

	void	ActionCrystalWarp();

protected:

	BOOL						m_bLockSkill;
public:
	void						SetLockSkill(BOOL bYes){m_bLockSkill = bYes;}
	BOOL						IsLockSkill(){return m_bLockSkill;}
    DWORD m_dwCrystalWarpObjKeys[CRYSTAL_WARP_OBJ_MAX_CNT];

private:
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
	CHANGE_PCROOM_STATUS::eStatus m_ePCBangStatus;
#endif

public:
	void						SetCrystalWarpKey(DWORD dwKey, bool bAdd = true);
    bool                        IsCrystalWarpKey(DWORD dwKey);

    struct
    {
        //CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD
        KEYTYPE portal_key;
        uint8_t open_state;
        ulong portal_lifetime;
        bool has_portal_info()
        {
            return ((portal_lifetime > 0) && (portal_key > 0) &&
                    ((open_state == MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD::eOpenState_Created) 
                        || (open_state == MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD::eOpenState_Countdown)));

        }
    }FixedPortalState;

    struct
    {
        //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
        float ang;
        WzVector pos;
        DWORD sent_tick;
        BYTE last_sent_move_flag;
        bool is_first_kbmove;

        //! debug
        int debug_kbmove_skip_count;
        float debug_kbmove_gab;
        float debug_move_speed;
    }KBMove;

    struct
    {
        //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
        DWORD delay_tick;
        bool is_recv_nak;
    }PacketNak;

private:
    bool is_waiting_invite_group_;

public:
    bool IsWaitingInviateGroup() const { return is_waiting_invite_group_; }
    void SetWaitingInviateGroup(bool value) { is_waiting_invite_group_ = value; }

#ifdef _NA_000000_20120410_SPEEDHACK
    virtual void	SetMoveSpeed(float fMoveSpeed);
    virtual float	GetMoveSpeed(void); 	

    WGPL::Shuffle<float, true, 2> move_speed_;
#endif //_NA_000000_20120410_SPEEDHACK


#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
private:
    struct ActivedComboSkillData
    {
        int actived_skill_class_code_;
        DWORD end_time_;
    };

    std::vector<ActivedComboSkillData> actived_combo_skill_array_;
public:

    void AddActivedComboSkill( int skill_class_code, DWORD duration );
    void ClearActivedComboSkill();
    void UpdateActivedComboSkill();

    BOOL IsActivedComboSkill( SLOTCODE SkillClassCode  );
#endif

#ifdef _NA_004035_20120227_COSTUME_ITEM
public:
    POSTYPE GetActivedCostumeItemSlotIndex();
    void SetActivedCostumeItemSlotIndex(POSTYPE inventory_position);
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
public:
	POSTYPE GetActivedDecorateCostumeItemSlotIndex();
	void    SetActivedDecorateCostumeItemSlotIndex(POSTYPE inventory_position);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    void RefreshInvenItemEffect();
    void RefreshBadgeItemEffect();
    void RefreshPCBangItemEffect();

    BYTE GetPCBangMode();   // override Player
};

//------------------------------------------------------------------------------
/**
*/
inline
BOOL Hero::IsForceAttack()
{
	return m_bForceAttack;
}

//------------------------------------------------------------------------------
/**
*/
inline
void Hero::SetForceAttack(BOOL bForce)
{
	m_bForceAttack=bForce;
}

