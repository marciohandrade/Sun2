#pragma once
//------------------------------------------------------------------------------
/**

    월드 상의 몬스터 특성을 정의 
    @author

    @since

    @remarks
        -2005.06.07 소스 정리(지완)
        - 
*/
#include "Character.h"
#include "SCNPC.h"

enum MonsterPart
{
	MP_BONE		= 0,
	MP_COUNT,
};

enum
{
	MS_NONE,
	MS_DAMAGE,
	MS_TUMBLE,
};

enum eMonsterType
{
	MONSTER_TYPE_GENERAL = 0,
	MONSTER_TYPE_SETTING_MAPOBJECT,		// 설치형 몬스터
	MONSTER_TYPE_TOTEM,
};

enum MONSTER_STAND_STATE
{
    kStand_State = 0,   // 지면 위에 서있는 상태
    kStand_Down_State,  // 지면 속으로 파고드는 상태
    kStand_Up_State,    // 지면 위로 올라서는 상태
    kBurrow_State,      // 지면 속에 파고든 상태
};

class MonsterGauge;
class NPCAttributes;
class SkillAttrCalculator;

class Monster : public Character
{
public:
	Monster( void );
	virtual ~Monster( void );

protected:
	BASE_NPCINFO        m_NPCInfo;
	NPCAttributes *		m_pNPCAttriburtes;

public:
	virtual BOOL		Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void		Destroy( void );

	virtual void		SetMonsterInfo(BASE_NPCINFO *pInfo);
	BASE_NPCINFO *		GetMonsterInfo();
	
	virtual BOOL		Render( void );
	virtual float		GetMeshHeight();
	virtual void		RenderInfo( void );
	virtual void		TransForm();
	virtual int			DoAction(PLAYER_ACTION *pAction);
	
	virtual WzVector    GetTrailBonePos(int index, int start);
	virtual WzID        GetTrailEffectCode();
	virtual void		SetScale(float fScale);

    virtual BOOL        SetPosition( const WzVector& wvPos, int iTileIndex = -1);

	// 전투처리
	virtual WzID		GetCurrentAttackAnimation();
	virtual WzID		GetCurrentIdleAnimation();
	virtual WzID		GetCurrentMoveAnimation();
    virtual WzID		GetCurrentRunAwayAnimation();
	virtual WzID		GetCurrentDeathAnimation();
	virtual WzID		GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical);
	virtual WzID        GetCurrentBlockAnimation();
	virtual WzID        GetCurrentCamAnimation();
    virtual WzID        GetCurrentAttackJumpAnimation();

	virtual void		PlayDamageAnimation(BOOL bRight,BOOL bCritical);
	virtual void		PlayEvadeAnimation();
	virtual void		PlayBlockAnimation();
	virtual WzVector	GetArrowBonePos(int index);
	virtual WzVector	GetWeaponBonePos(int index);

	virtual void		SetHP( DWORD iHP );
	void				PlayDamageSound(eWEAPONSOUNDKIND eSoundKind);
	void				CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId);
	void				SetMonsterGaugeShowDelay(int iDelay);
	void				RenderMonsterDetailInfo();
	virtual BOOL		Process(DWORD dwTick);
	virtual void        ProcessVoice(DWORD dwTick);
	virtual float		ProcessAnimation( DWORD dwTick ,WzEvent *pEvent = NULL,BOOL bIgnoreEvent = FALSE);
	void                RenderMonsterStatus();

	void                ChangeHPRatio(float fRatio);
	NPCAttributes *		GetNpcAttribute();

	// 이벤트
	virtual  void       OnDamage(const DAMAGE_PRAM &rDamage);

	virtual void        OnUseSkill();
	virtual void        OnDead();
	virtual void        OnStartRun();

    // 이름 설정 
    virtual void		SetName( const tstring& Name );

    virtual void		RestoreObjectName();

	// 스텟 관련
	virtual void		SetExp(EXPTYPE dwExp);
	virtual EXPTYPE		GetExp();
	virtual EXPTYPE		GetNextExp();

	virtual void		SetLevel(LEVELTYPE LV);
	virtual void		SetMaxHP( DWORD iHP );
	virtual void		SetMaxMP( DWORD iMP );
	virtual void        SetAttackSpeedRatio(int iSpeed);
	virtual void        SetMoveSpeedRatio(int iSpeed);
	
	virtual LEVELTYPE	GetLevel();

	virtual DWORD		GetMaxHP();
	virtual DWORD		GetMaxMP();
	//이모티콘 상태
	virtual void        SetCondition(BYTE byCondition);

	virtual float       GetAttackSpeedModifier();
	virtual float       GetMoveSpeedModifier();

	virtual BOOL        CannotAir();      // 공중에뜰수 없는 상태 

	VOID				SetAttackSpeedModifier(float fAttSpeed);	
	VOID				SetMoveSpeedModifier(float fMoveSpeed)	{ m_fMoveSpeedModifier = fMoveSpeed; }

	void				OnShadowSummon();
	virtual BOOL		IsShadowSummon() { return m_bShadowSummon; }
	virtual void		StartExtraEffect();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void                OnWitchBladeImmoveSummon();
    virtual BOOL        IsWitchBladeSummon() { return m_bWitchBladeSummon; }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//GS_BUG_FIX:서버쪽뷰포트에 의해 패킷순서가 바뀌어서 오기때문에 보정을위한 함수
	void				SetMissedSummoner();
	void				FindMissedSumoner();

	void				UpdateSummonTargeting();
	void				InitSummonTarget();

	virtual BOOL        IsRangedNormalAttack();

	virtual void		SetIdleAnimation();
	void				ProcessStandAnimationDelay(DWORD dwDelay);

    virtual void SetAnimation(WzID animation_id, BOOL is_loop = FALSE, BOOL bRestart = TRUE);
    void SetAnimationForCentipedeMonster(WzID animation_id, BOOL is_loop = FALSE, BOOL bRestart = TRUE);
    void ProcessAnimationForCentipedeMonster();
    
    bool IsCentipedeMonster();
    void set_stand_state(MONSTER_STAND_STATE stand_state);
    MONSTER_STAND_STATE stand_state();

    virtual WzVector    GetSkillEffectScale(SKILL_EFFECT* skill_effect);

	virtual DWORD       CreateProjectile(Character *pTarget,int iArrowBone = 0);

#ifdef __NA_0_20080415_PROCESS_END_PROECSS
	virtual int			OnEndProcess( DWORD dwTick );
#endif

	virtual float		GetBodyRadius();

	virtual void        CreatePopupMesh(
		CODETYPE SkillCode,
		INT64 iNumber,
		eDAMAGEPOPUP_KIND eKind,
		WzColor color,
		BOOL bCritical,
		BOOL bDoubleDamage,
		float fMultiplier = 1.0f,
		int iSecondary = -1,
		WzColor secondarycol = WzColor_RGBA(0,0,0,0),
		float fHeight = 0.0f,
		BOOL bHero = FALSE
		);
	

    BOOL m_IsDeathMove;
    WzVector m_DeathMoveTargetPosition;
    DWORD m_DeathMoveTime;
    DWORD m_DeathSkillCode;

    virtual void StartDeathMove();
    virtual BOOL ProcessDeathMove(DWORD dwTick);
    virtual void EndDeathMove();
    virtual void SetDeathSkillCode(DWORD SkillCode) { m_DeathSkillCode = SkillCode; }

	void                AddDelaySkillSerial(DWORD dwDelaySkill);
    DWORD               GetDelaySkillSerialCount();
	DWORD               GetDelaySkillSerial();
	void                SetParentId(const DWORD & dwId,const BYTE & byCommand,BOOL bHideDialog = FALSE);
	DWORD				GetParentId();
	
	// 사운드
	void                PlayIdleVoice();
	void                PlayBattleVoice();
	void                PlaySkillVoice();
	void                PlayDieVoice();
	void                PlayStateVoice();	

	virtual BOOL        CanUseSkill(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
    virtual BOOL        CanUseSkillWithoutCooltime(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
	BOOL                CanUseSkillLimitClassDefine(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);

	void                CreateEffect();
	void                DestoryEffect();
	void                SetTransFormNpcCode(DWORD dwCode);
	void				SetMonsterType(eMonsterType MonsterType) { m_eMonsterType = MonsterType; }
	eMonsterType		GetMonsterType() const { return m_eMonsterType; }		
	void                SetRangeEffectCode(HANDLE hEffect);
	
	void				ProcessSingleMove();
	void				SetSingMoveState(BOOL bState){m_bSingleMove =bState;}
	BOOL				GetSingMoveState(){return m_bSingleMove;}

	void				SetShowName( BOOL b = FALSE );
	DWORD				GetColorText( int monsterLv, int HeroLv );
	LEVELTYPE			GetDisplayLevel();
	void				RenderHPGuage( void );
	void				SetIdleAniID( DWORD dwStand = StrToWzID( "N002" ), DWORD dwIdle = StrToWzID( "N001" ), DWORD dwAttackStand = StrToWzID( "N003" ) );

    virtual BYTE GetMyResist(){ return m_NPCInfo.element_weakness.value; }

protected:
	DWORD				m_dwParentId;
	DWORD               m_dwTransFormCode;
	int					m_iMonsterGaugeShowDelay;

	float				m_fAttackSpeedModifier;
	float				m_fMoveSpeedModifier;

	int                 m_iAlarmVoice;
	int                 m_iWaitSoundDelay;
	
	SCNPC               m_scNPCAtt;
	list<DWORD>			m_DelaySkillSerial;
   	float				m_fScale;
	HANDLE              m_hRangeEffect;
	eMonsterType		m_eMonsterType;
	BOOL				m_bSingleMove;
	BOOL				m_IsShowName;
	DWORD				m_dwShowNameTick;
	BOOL				m_bShadowSummon;			//섀도우에 의해 컨트롤당하는 몬스터인지		
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BOOL                m_bWitchBladeSummon;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	BOOL				m_bMissedSummoner;			//주인과 연결이 되지않았다.
	HANDLE              m_hSummonTarget;
	WzVector            m_vTargetScale;
	DWORD				m_dwStandAniID;
	DWORD				m_dwIdleAniID;
	DWORD				m_dwAttackStandAniID;
	DWORD				m_dwMasterGuildID; //주인의 길드 ID
	HANDLE				m_hCrystalWarpEffect;
	DWORD				m_dwMapObjectKey;
	bool				m_bCreateDeadBody;

    //! 소환수 아드레날린
    HANDLE  m_hSummonIntensifyEffect;
public:
	void				CreateCrystalWarpEffect(DWORD dwEffectID);
	void				DestoryCrystalWarpEffect();
    void                DestorySummonIntensifyEffect();
    HANDLE              CreateSummonIntensifyEffect(DWORD effect_id);

	void				SetMasterGuildID(DWORD dwID){m_dwMasterGuildID = dwID;}
	DWORD				GetMasterGuildID(){return m_dwMasterGuildID;}

	void				SetMapObjectKey(DWORD dwMapObjectKey){m_dwMapObjectKey = dwMapObjectKey;}
	DWORD				GetMapObjectKey(){return m_dwMapObjectKey;}

	void				SetTriggerMapObjectAnimation();

	void				SetbCreateDeadBody(bool bVal){m_bCreateDeadBody = bVal;}

	WzVector			m_wvOriginPos;	//설치형 몬스터 전용
	bool				m_bGetOriginPos;//설치형 몬스터 전용

    MONSTER_STAND_STATE stand_state_;


    virtual void SetPolymorph( BOOL Active );

};

inline 	DWORD Monster::GetParentId()
{
	return m_dwParentId;
}

inline void Monster::SetRangeEffectCode(HANDLE hEffect)
{
	m_hRangeEffect=hEffect;
}

inline bool Monster::IsCentipedeMonster()
{
    if (m_NPCInfo.m_wClass == (eNPC_CLASS_CENTIPEDE + 1))
    {
        return true;
    }
    return false;
}

inline void Monster::set_stand_state(MONSTER_STAND_STATE stand_state)
{
    stand_state_ = stand_state;
}

inline MONSTER_STAND_STATE Monster::stand_state()
{
    return stand_state_;
}
