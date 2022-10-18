#pragma once

//------------------------------------------------------------------------------
/**

    월드 상의 캐릭터 특성을 정의 
    @author

    @since

    @remarks
        -2005.06.07 소스 정리,SCCharacter 포함 관계로 변경(지완)
        - 
*/

#include "Object.h"
#include <SCCharacter.h>
#include "CharacterDef.h"
#include "SkillDef.h"
#include "CharacterData.h"

typedef struct _EQUIPAPPEARINFO EQUIPAPPEARINFO;
class SCSlotContainer;

class Character : public Object, public cCharacterData
{
// State클래스만 friend로 지정하세요!
friend class StateHeroAttack;
friend class StateAttack;
friend class StateAir;
friend class StateJump;
friend class StateEventJump;
friend class StateDamage;
friend class StateDown;
friend class StateSpecialMove;
friend class StateSit;
friend class StateEmoticon;
friend class StateKeyboardJump;
friend class StateAttackJump;
friend class StateTransForm;
friend class StateFallDown;
friend class StateKeyboardMove;
friend class StateAirFallDown;
friend class StateKnockBack2;
friend class StateKnockBack3;
friend class StateFireCracker;

friend class StateCollect;

protected:
	Character();
	virtual ~Character();

private:
	WzVector m_wvOldVisiblePos;			// 옥트리 처리 최적화용 백업
	WzVector m_wvInterpolateOffset;		// 캐릭터 좌표 보간용

	float m_fVisibleAngleModifier;		// 캐릭터 각도 보간치
	float m_fVisibleAngleZ;				// 보간이 적용된 각도


	float m_fHeight;	// 뜬높이
	BYTE m_byFloor;		// 층 정보
	BOOL m_FreeMove;	// 지형에 상관없이 움직이는 캐릭터
	BOOL m_bShowName;	// 이름 보이기 

	eForceTurn m_ForceTurn;   

	void SetForceTurn(eForceTurn value) { m_ForceTurn = value; }

	float GetVisibleAngleModifer(){ return m_fVisibleAngleModifier; }


public:
    void                    SetShow(BOOL bFlag);    // override Object

	void					SetHeight(float fHeight) { m_fHeight = fHeight; } 
	float                   GetHeight() { return m_fHeight; }
	void					SetFloor(BYTE byFloor) { m_byFloor = byFloor;}
	BYTE					GetFloor() {return m_byFloor;}
	BOOL					IsFreeMove() const { return m_FreeMove; }
	void					SetFreeMove(BOOL value) { m_FreeMove = value; }
	BOOL					IsShowName()			{ return m_bShowName; }
	void					SetShowName(BOOL show)	{ m_bShowName = show; }

	void					SetVisibleAngleModifer(float fAngle);

    BOOL                    IsBodyOnlyRenderMode();


	void SetAngle( float fAngleZ, BOOL bSetVisibleAngle = FALSE );
	void SetAngle(float fAngleX, float fAngleY, float fAngleZ);
	void Rotate(float fAngle, BOOL bSetVisibleAngle = FALSE );
#ifdef _DH_NPC_GROUP_NAME
    void RenderTextOnHeadGroup(TCHAR *szString, WzColor color, int yoffset);
#endif//_DH_NPC_GROUP_NAME
protected:
	float GetVisibleAngleZ() const { return m_fVisibleAngleZ; }
	void SetVisibleAngleZ(float value) { m_fVisibleAngleZ = value; }

protected:
    //서버 클라이언트 공용 속성 
    SCCharacter         m_scCharAtt;

    //
	BOOL				m_bObserverMode;	//	옵저버
	DWORD               m_dwHideTime;
	BOOL                m_bHide;
	WzVector            m_vNearPos;         //  갈수 없는 타일 위에 서있는 것들 

	DWORD				m_dwTargetObjectKey;
	float				m_fAttackRange;
	float				m_fReachNPCRange;
	float				m_fMoveSpeed;	
	eATTACK_SEQUENCE	m_eAttackSequence;	
	DWORD				m_dwHP;
	DWORD				m_dwMP;
	DWORD				m_dwSD;
	DWORD				m_dwMaxSD;
	BOOL				m_bShield;


	DWORD				m_reserveHP;
	WzVector            m_vNormal;

	// 쳐다보기 관련.	
	DWORD				m_dwLockedTargetID;
	BOOL				m_bLockedPosition;
	WzVector			m_vLockedPosition;	
	BOOL				m_bLocked;
	WzVector			m_vVisibleDirection;
	// 점프관련
	BOOL				m_bJump;
	WzVector			m_vJumpFrom;
	WzVector			m_vJumpTo;
	float				m_fJumpDistance;
	float				m_fEventJumpHeight;
	float				m_fEventBaseHeight;
	float				m_fEventJumpStartX;
	//데미지 처리 관련
	HONGPOS				m_vAttackStartPos;
	HONGPOS				m_vAttackEndPos;
	BOOL				m_bAttackMove;        
	DWORD				m_dwLatestAttack;       // 최근에 얻어맞은(!) 공격의 씨리얼 
	DWORD               m_dwLatestQueueAttack;  // 자신의 큐행동에 영향을 미치는 공격의 씨리얼
	DWORD				m_dwAttackSerial;       // 자신이 현재 처리하고있는 공격의 씨리얼!
	DWORD				m_dwNextAttackSerial;   // 다음 공격에 할당될 공격의 씨리얼
	DWORD               m_dwNextActionAttackSerial; // 주인공이 아닌 다른 캐릭터가 다음 Action에 사용하게될 AttackSerial
	int					m_iHitCount;            // 처리한 AttackResult Count! (Combo 카운트하고는 다름!)
	int					m_iHitEvent;
	BOOL				m_bRightHit;

    //! 반사계열 데미지 처리를 위해서
    ATTACK_RESULT_LIST	m_ReflectDamageList;   

	ATTACK_RESULT_LIST	m_AttackResultList;   
	int					m_iAttackStandTime;    // 


	// 경직
	DWORD				m_dwHitPause;
	// 떨기
	WzVector			m_vVibrateDirection;
	float				m_fVibrateForce;	
	BOOL                m_bVibrateMove;
	float               m_fVibrateCurrent;
	// Idle
	DWORD				m_dwStandAnimDelay; // 두리번 주기
	// 돌리기
	float				m_fSpinForce;
	// 이동용 
	BYTE				m_bMoveState;	
	WzVector			m_vTargetPosition;
	WzVector			m_vNextMoveCurPos;
	WzVector			m_vNextMoveDestPos;
	BYTE				m_bNextMoveState;
	BOOL				m_bNextMoveThrust;
	BOOL				m_bNextMove;
	int					m_iNextMoveCurTile;
	int					m_iNextMoveDestTile;
	// 포탈용
	WzVector            m_vDestWarp;     

	// 점프 공격용
    ATTACK_JUMP_INFO    m_AttackJumpInfo;

	// 상태처리 관련
	PLAYER_ACTION_QUEUE	m_ActionQueue;
	PLAYER_ACTION		m_CurAction;
	// 색상
	WzColor				m_Color;
	// 스킬
    std::vector<SKILL_EFFECT*> m_SkillEffectArray;


	// 투기
	int                 m_iFightingEnergy;
	
	// 렌더링
	float               m_fMeshHeight;

    // 페이드
	BOOL				m_bFade;
	int					m_iFadeTotDelay;
	int					m_iFadeCurDelay;	
	int					m_iTextFadeCurDelay;

	// 이모션 아이콘
	int                 m_iEmoteIcon;
	int                 m_iEmoteIconDelay;
	CWzUnitDraw *		m_pEmoteDraw;
	// 사운드
	int					m_iVoiceHandle;
	int					m_iVoiceKind;

	WzID				m_CurrentEmotionAnimationID;
#ifdef _YJ_FISHING_SYSTEM_000933
	WzID				m_wiCurFishingAniID;
#endif //_YJ_FISHING_SYSTEM_000933
	// 사망
	BOOL				m_bDeathTimer;
	int					m_iDeathTimer;


	list<DWORD>			m_vNotProcessedList;
    
    DWORD               m_dwMoveTarget;
    list<DWORD>         m_listTargeter;

    //	이동을 시작할 때의 시간( 0.5초 간격으로 이동중에 타겟 위치 다시 갱신을 위해 )
    DWORD		        m_dwStartMoveTime;	
    
    DWORD		        m_dwSpecialMoveCurTime;
    BOOL		        m_bSpecialMoveMove;
    BYTE		        m_byCondition;
    BOOL		        m_bSitting;
#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
    BOOL		        sitting_packet_delay_;
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
    DWORD               m_dwAttackEffectTime;
    BOOL                m_bNoCull;
    BOOL                m_bCameraAlpha;
	float               m_fAlpha;
	DWORD               m_dwCurColorTime;
	WzColor				m_OldTerrainColor;

    DWORD               unitid_for_trigger_;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    HANDLE              vice_sword_effect_handle_[2]; // 바이스소드 이펙트
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

public:
	WzVector            m_vLastSend;
	WzVector            m_vTemp;
	WzVector            m_vTemp2;
	WzVector            m_vTemp3;
	WzVector            m_vTemp4;
	WzVector            m_vTemp5;
	float               m_fTurnAccelerate;

    DWORD               m_dwTerrainAttribute;

	DWORD               m_dwCurTileAtt;

#ifdef _NA_004035_20120227_COSTUME_ITEM
    virtual void SetCostumeItemCode( SLOTCODE code ) {};
    virtual SLOTCODE GetCostumeItemCode() { return 0; };
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	virtual void SetDecorateCostumeItemCode( SLOTCODE code ) {};
	virtual SLOTCODE GetDecorateCostumeItemCode() { return 0; };
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    virtual void SetSpaItemCode(SLOTCODE item_code) {};
    virtual SLOTCODE GetSpaItemCode() { return 0; };
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

public:
	// 생성 파괴
	virtual BOOL		Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void		Destroy( void );
	// 렌더링
	virtual BOOL		Render( void );
	virtual BOOL		OnUseItem(DWORD dwItemCode);	
	virtual BOOL		OnUnUseItem(DWORD dwItemCode);	

    virtual void        InitMeshHeight();

	virtual DWORD       GetWeaponKind();
    virtual char        GetWeaponAnimationCode();

	virtual BOOL		RenderOutline( const WzColor& color , float fThickness );
	virtual void		RenderTextOnHead(TCHAR *szString,WzColor color,int yoffset,BOOL bForceRender=FALSE, bool bIsCharacterScene=false );

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
	virtual void        CreateEffectAtHead(WzID effectID,float fHeight);
	virtual float		ProcessAnimation( DWORD dwTick ,WzEvent *pEvent = NULL,BOOL bIgnoreEvent = FALSE);	
	virtual float       GetMeshHeight();
	virtual void        SetMeshHeight(float fMeshHeight);


    virtual void StartDeathMove() {}
    virtual BOOL ProcessDeathMove(DWORD dwTick) { return FALSE; }
    virtual void EndDeathMove() {}

    virtual void SetDeathSkillCode(DWORD SkillCode) {}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual void                CreateViceSwordWeaponEffect();
    virtual void                DestroyViceSwordWeaponEffect();
    void                        UpdateViceSwordWeaponEffect();
    BOOL                        is_vice_sword_effect_();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE


	// 변신 관련
	struct sTransformData
	{
		RESOURCECODE						m_TransFormResourceCode;
		eTRANSFORM_TYPE						m_TransFormType;
		eTRANSFORM_TYPE                     m_OldTransFormType;
		eTRANSFORM_PROCESS					m_TransFormProcess;

		BYTE                                m_bAppearKind;

		void Init() 
		{
			m_TransFormResourceCode = 0;
			m_bAppearKind = 0;
			m_TransFormProcess = TRANSFORM_PROCESS_NONE;

			m_TransFormType = eTRANSFORM_TYPE_NONE;
			m_OldTransFormType = m_TransFormType;
		}

		//변신 시작이냐 끝이냐를 알린다.
		eTRANSFORM_PROCESS  GetTransFormProcessType() { return m_TransFormProcess;}
		void                SetTransFormProcessType(eTRANSFORM_PROCESS eProcess) { m_TransFormProcess = eProcess; }

		// 변신할 리소스 타입 플레이어는 파트구조 그 이외는 통 메쉬 
		RESOURCECODE        GetTransFormResourceCode();
		void                SetTransFormResourceCode(RESOURCECODE code) { m_TransFormResourceCode = code;}
		void                SetTransFormType(eTRANSFORM_TYPE bType) { m_OldTransFormType = m_TransFormType; m_TransFormType=bType;}
		eTRANSFORM_TYPE     GetTransFormType() { return m_TransFormType;}
		eTRANSFORM_TYPE     GetOldTransFormType() { return m_OldTransFormType;}

		void                SetAppearanceKind(BYTE bType) {m_bAppearKind=bType;}
		BYTE				GetAppearanceKind() {return m_bAppearKind;}
	};
	sTransformData m_TransformData;

public:
	sTransformData&     GetTransformData() { return m_TransformData; }

	BOOL                IsTransForm() { return GetTransformData().GetTransFormType() != eTRANSFORM_TYPE_NONE; }


public:
    BOOL IsMyHero();
	BOOL IsPartyMember();
	//BOOL IsEnemy();




	virtual int                 GetSpecialAccessoryBone(WzID id);
	virtual int				    GetWeaponBone(int index);

    virtual void                UpdateSkillAttr() {}
	virtual void				UpdateItemAttrCalculator(){}
	virtual void				SetWeaponKind(DWORD dwWeaponKind){}
	virtual void				OpenCrossbow(){}
	virtual void                CloseCrossbow(){}

	//virtual PlayerAttributes *	GetPlayerAttribute(){ return NULL;}
	virtual BYTE				GetPCBangMode()	{ return FALSE;}
	virtual bool				IsEtherDeviecBulletChargeing()	{ return FALSE;}	
	virtual void		        CreateEtherWeaponEffect(bool bShotEffect = false, bool bFillupEffect = false){}
	virtual void				DestoryEtherWeaponEffect(){}

	virtual SCSlotContainer *	GetSlotContainer( SLOTIDX slotIdx );
	virtual void        ApplyTerrainColor(BOOL bApply);
    
	void                SetFadeIn(int iDelay);
	void                UpdateOctree( BOOL bForce );
	void				UpdateOctree();

	// 이동 관련	
	virtual BOOL		MakePathPacket( MSG_CG_SYNC_MOVE_SYN *pMsg );
	virtual BOOL		MakeTargetPathPacket( MSG_CG_SYNC_TARGET_MOVE_SYN *pMsg );
	virtual void		StopMove( void );
	virtual BOOL		IsMoving( void );

	virtual WzVector    GetArrowBonePos(int iAttachIndex);
	virtual WzVector	GetWeaponBonePos(int index);

	virtual WzVector	GetHandBonePos(int index);
	virtual WzVector    GetSkillEffectPos(eSKILLEFFECT_POS position_type, int bone_index = 0);
    virtual WzVector    GetSkillEffectPos(SKILL_EFFECT* skill_effect);
    virtual WzVector    GetSkillEffectRotate(SKILL_EFFECT* skill_effect);
    virtual WzVector    GetSkillEffectScale(SKILL_EFFECT* skill_effect);
	virtual WzVector	GetHeadBonePos();
	virtual WzVector	GetBodyPos();

	virtual float		GetBodyRadius() { return 0.0f; }
    virtual bool        GetBodyBonePos(OUT WzVector* pos);

	virtual int         GetBoneIndex(BYTE byType);
    virtual int         GetBoneIndexByWzID(WzID bone_id);

	virtual DWORD       CreateProjectile(Character *pTarget,int iArrowBone = 0);
	
	virtual DWORD		GetCurrentAttackStyle(){ return 9999;}
	virtual void		ValkyrieShot(DWORD dwAttackSerial){}
	virtual DWORD		GetValkyrieShotSerial(){return 0;}

	void				SetInterpolatePosition();	
	virtual BOOL		Move(
                            WzVector vCurPos,
                            WzVector vDestPos,
                            BYTE byState,
                            BOOL bThrust,
                            int iCurTileNum = -1,
                            int iDestTileNum = -1);
	virtual void		SetNextMove(
                            WzVector vCurPos,
                            WzVector vDestPos,
                            BYTE byState,
                            BOOL bThrust,
                            int iCurTileNum = -1,
                            int iDestTileNum = -1);
	void				StartNextMove();
	virtual void		SetMoveFlag(int /*flag*/) {;};
	virtual void        SetSpecialMoveFlag(BYTE /*type*/){;};
    
    // state 처리 관련
    void                SetStateTotalInfo( STATE_TOTAL_INFO & rStateTotalInfo );
	
	//이모티콘 상태
	virtual void        SetCondition(BYTE byCondition);
    virtual void        SetEmotion( WzID wzEmotionID ) { m_CurrentEmotionAnimationID = wzEmotionID; }
    virtual WzID        GetCurrentEmotion() { return m_CurrentEmotionAnimationID; } 

	virtual void		OnWaterWave();
	virtual float		GetAngleForMoveDirection();
	virtual void		OnWaterEllipse();
	WzVector			GetWaterTerrainPos();
	HANDLE				m_hWaterEllipse;
	

#ifdef _YJ_FISHING_SYSTEM_000933
    void				SetCurFishingAniID(WzID wzAniID) { m_wiCurFishingAniID = wzAniID; }
    WzID				GetCurFishingAniID() { return m_wiCurFishingAniID; }
#endif //_YJ_FISHING_SYSTEM_000933

    //서버 클라 공용 데이터
    SCCharacter &       GetSCCharacter() { return m_scCharAtt; }

	// 고개돌리기 관련
	void				LockOn(DWORD dwObjectKey);
	void				LockOnPosition(BOOL bFlag,WzVector wzPos);
	DWORD				GetLockedTarget();

	// 게임 처리부분
	virtual BOOL		Process( DWORD dwTick );
	virtual BOOL        ProcessSkillEffect(DWORD dwTick);
	virtual BOOL		ProcessInterpolation( DWORD dwTick ,float fInterpolateRatio = 0.93f);
	void				ProcessAllRemainActionResult(); // 로그아웃하기전에 모든 액션의 결과를 처리한다.
	void 		        ProcessTextFade( DWORD dwTick );
	// Action queue
	int					EmptyActionQueue();
	int					GetActionQueueCount();
	int					PutAction(PLAYER_ACTION *pAction);
	virtual BOOL		CheckQueueActionCondition();
	virtual int			DoNextAction();
	virtual int			DoAction(PLAYER_ACTION *pAction);


	void				SetCurrentAction(PLAYER_ACTION *pAction);
	PLAYER_ACTION*		GetCurrentAction();

	PLAYER_ACTION*		GetNextAction();
	// 점프
	BOOL				IsJumping();
	void				SetJumpFlag(BOOL bFlag);
	void				SetJumpInfo(WzVector *pvJumpFrom,WzVector *pvJumpTo);
	void				GetJumpInfo(WzVector *pvJumpFrom,WzVector *pvJumpTo,float *pDistance);
	// 애니메이션
	virtual WzID		GetCurrentIdleAnimation() = 0;
	virtual void		SetIdleAnimation();
	virtual WzID		GetCurrentMoveAnimation() = 0;
    virtual WzID		GetCurrentRunAwayAnimation();
	virtual void        SetMoveAnimation();
    virtual void        SetRunAwayAnimation();
    virtual WzID		GetCurrentAttackAnimation() = 0;
	virtual WzID		GetCurrentDeathAnimation() = 0;
	virtual WzID		GetCurrentKnockBack2Animation();
	virtual WzID		GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical) = 0;
	virtual WzID		GetCurrentBlockAnimation();
	virtual WzID		GetCurrentCamAnimation();
    virtual WzID        GetCurrentAttackJumpAnimation();
	virtual WzID        GetCurrentReviveAnimation();
	virtual WzID        GetCurrentTransFormAnimation();


	virtual void		PlayDamageAnimation(BOOL /*bRight*/,BOOL /*bCritical = FALSE*/) {;};
	virtual void		PlayEvadeAnimation() {;};
	virtual void		PlayBlockAnimation() {;};
	virtual void        SetCurrentMoveSpeed();
    void                SetStandAnimationDelay();

    DWORD GetStandAnimationDelay() { return m_dwStandAnimDelay; }
    int GetAttackStandTime() { return m_iAttackStandTime; }

	// 전투처리
	DWORD				GetLatestAttack();
	void				SetLatestAttack(DWORD dwTimeSerial);
	DWORD				GetLatestQueueAttack();
	void				SetLatestQueueAttack(DWORD dwTimeSerial);
	WzVector            GetNearTargetPos(WzVector vSrc);
	WzVector            GetTarGetPosition(){ return m_vTargetPosition; }
	virtual BOOL		CanBeAttackTarget();
	void				SetCurrentAttackSerial(DWORD dwAttackSerial); // Hero는 절대 호출하면안됨!
	DWORD				GetCurrentAttackSerial(); 
	void				SetNextAttackSerial(DWORD dwAttackSerial);
	DWORD				GetNextAttackSerial();
	void				AddNextAttackSerial();
	void                SetNextActionAttackSerial(DWORD dwAttackSerial);
	DWORD               GetNextActionAttackSerial();
	void				SetHitCount(int count);
	int					GetHitCount();
	virtual int         GetShotCount();
	int					AddAttackResult(ATTACK_RESULT *pAttResult);
	int					GetAttackResultListSize();
    BOOL                GetAttackTargetList(DWORD dwAttackSerial,int *pNum,DWORD *pTarget, int target_buffer_size);
	void				EmptyAttackResultList();

    BOOL				ProcessAttackResult_Group(DWORD dwAttackSerial, BOOL bRight = FALSE, BOOL bAddtionalEffect = TRUE );
    BOOL				ProcessAttackResult_Target(DWORD dwAttackSerial, DWORD target_object_key, BOOL bRight = FALSE, BOOL bAddtionalEffect = TRUE );
    BOOL				ProcessAttackResult(DWORD dwAttackSerial, BOOL bAll, BOOL bRight = FALSE, BOOL bAddtionalEffect = TRUE );

    void                ProcessReflectDamage();
	
	void				SetAttackMovePath(HONGPOS vCurPos,HONGPOS vDestPos);
	void				SetAttackMove(BOOL bMove);
	void				SetHitPause(DWORD dwPause);
	DWORD				GetHitPause();
	//		옵저버 모드
	void				SetObserverMode( BYTE byObserverMode, BOOL IsHero = FALSE );
	BOOL				IsObserverMode( void );


	void				Vibrate(WzVector vDirection, float fForce);
	void				HitSpin(float fForce, BOOL bRight);

	void				SetDelayTime(DWORD dwTime);
	DWORD				GetDelayTime();

	void				SetCurrentDelayCount(DWORD dwTime);
	void				AddCurrentDelayCount(DWORD dwTime);
	DWORD				GetCurrentDelayCount();
	
	BOOL                IsMove();      //

	BOOL				IsDying(); // 죽는 애니메이션중
	BOOL				IsDead();  // 죽었음~
	void				Hide(BOOL bFlag);

	void                SetNoCull(BOOL bFlag);

	virtual void		PlayDamageSound(eWEAPONSOUNDKIND eSoundKind);
	virtual void		CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId);

	// 유닛 파라메터
	virtual void		SetHP( DWORD iHP );
	virtual void		SetMP( DWORD iMP );
	//virtual void		SetExp(EXPTYPE dwExp) = 0;
	//virtual EXPTYPE		GetExp()	= 0;
	//virtual EXPTYPE		GetNextExp()= 0;

	//virtual void		SetLevel(LEVELTYPE LV)	= 0;

    virtual void		SetMaxHP( DWORD iHP ) {}
    virtual void		SetMaxMP( DWORD iMP ) {}
    virtual void        SetAttackSpeedRatio(int iSpeed) {}
    virtual void        SetMoveSpeedRatio(int iSpeed) {}

	virtual void        SetHPInfo(HPINFO *pInfo);	
	//virtual LEVELTYPE	GetLevel()	= 0;
	virtual DWORD		GetMP();
	virtual DWORD		GetHP();
    
	virtual DWORD		GetMaxHP()	= 0;
	virtual DWORD		GetMaxMP()	= 0;

	virtual DWORD		GetSD(){return m_dwSD;} 
	virtual DWORD		GetMaxSD(){return m_dwMaxSD;} 
	virtual void		SetSD(DWORD dwSD){m_dwSD=dwSD;} 
	virtual void		SetMaxSD(DWORD dwMaxSD){m_dwMaxSD=dwMaxSD;}
	
	virtual void		SetReserveHP( DWORD val );
	virtual DWORD		GetReserveHP( void );

	virtual float       GetAttackSpeedModifier();
	virtual float       GetMoveSpeedModifier();

	void				SetTargetID(DWORD dwObjectKey);	
    DWORD				GetTargetID() { return m_dwTargetObjectKey; }
	Object *			GetTargetObject( void );
	
	void				SetAttackSequence( eATTACK_SEQUENCE sequence );
	eATTACK_SEQUENCE	GetAttackSequence( void );

	void				SetAttackRange( float fRange );
	float				GetAttackRange( void ) { return m_fAttackRange; }
	float				GetReachNPCRange( void ) { return m_fReachNPCRange; }
	void				SetAttackStandTime(int time);
	virtual BOOL        IsRangedNormalAttack();

	BYTE				GetMoveState();
	void				SetMoveState(BYTE state);
	virtual void		SetMoveSpeed( float fMoveSpeed );
	virtual float		GetMoveSpeed( void ); 	
	BOOL				UseSkill( int SkillID );

    // 상태이상	
    BOOL                IsShield(){return m_bShield;}
    BOOL                IsStun();
	BOOL                IsSleep();
	BOOL                IsFrozen();
	BOOL                IsBlind();
    BOOL                IsBlind2();
    BOOL                IsPolymorph();
    BOOL                IsDetectingHide();
	BOOL				IsDarkofLightZoneState();
	BOOL				IsGravityZoneState();
	BOOL                IsFainting(); // 걸렸을때 흐느적거리는 애니를 플레이해야한다
	BOOL                IsTransFormation();//변신 상태인지 조사 
	BOOL                IsBlock();
    bool                IsSteel();

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	BOOL                IsAbnormalityProtection(); // 이상상태 보호 상태인지 체크
#endif // _NA_008472_20150903_IMPROVED_BALANCE


    BOOL m_IsPolymorphApperance;
    virtual void SetPolymorph( BOOL Active );

	BOOL				CannotAction();     // 액션이 가능한 상태인지 (공격, 감정표현, 스킬사용)
	BOOL                CannotMove();		// 이동 불가 상태가 됐다.
	virtual BOOL        CannotAir();		// 공중에뜰수 없는 상태 

    //! Player 상속 받는다.
    virtual bool        is_spread_wings() { return false; }

	BOOL				IsImpossible();		// 접속 직후의 무적상태 (공격불가)

	virtual void		CloneEffect(DWORD LiveTime, DWORD DisappearTime,BYTE byStyle);
	virtual void		ProcessRushClone(){}

	// 거의 모든 스테이트의 공통적인 처리를 담당한다
	// TRUE리턴시 state를 change해야한다.
	virtual BOOL		CommonStateProcess();
	virtual BOOL		ProcessNextAction();
	virtual float		GetProcessMultiplier();

	//void				GetPos( WzVector * /*pPos*/ ) { assert(!"GetPosition()쓰세요!!");};
	//void				SetPos( WzVector * /*pPos*/ ) { assert(!"SetPosition()쓰세요!!");};

    //virtual  HANDLE     AddStateEffect(WzID state_effect_id, 
    //                                    DWORD target_objectkey, 
    //                                    SKILL_EFFECT* skill_effect);
    std::vector<SKILL_EFFECT*>& GetSkillEffectArray() { return m_SkillEffectArray; }
	virtual  int        AddSkillEffect(SKILL_EFFECT *pEffect);
	SKILL_EFFECT*       GetSkillEffect(int iEffectID);
    SKILL_EFFECT*       FindSkillEffect( eCHAR_STATE_TYPE StateType );
	virtual  BOOL       DeleteSkillEffect(int iEffectID);
	virtual  void       DeleteAllSkillEffect();
	virtual  BOOL       RemoveSkillEffectWithStatus(eCHAR_STATE_TYPE StateType);
	//virtual  BOOL       RemoveItemEffectByItemCode( DWORD dwItemCode ) { return TRUE; }

    virtual void        OnAddState( eCHAR_STATE_TYPE StateType );
    virtual void        OnRemoveState( eCHAR_STATE_TYPE StateType, BOOL IsDestroy = FALSE );

	void                UpdateSkillEffectPos();
    int                 GetSkillEffectCount(DWORD status_code); // status_code 와 같은 상태를 몇개 가지고 있느냐 (분노갯수확인용)

	void				DoPeriodicDamage(DAMAGETYPE wDamage,DWORD dwTargetKey,DWORD dwTargetHP,DWORD timeSerial,DWORD dwSkillCode);

    void				AddReflectDamage(DAMAGETYPE wDamage, DWORD dwTargetKey, DWORD dwTargetHP, DWORD timeSerial, DWORD dwSkillCode);

	void				SetFightingEnergy(int count);
	int					GetFightingEnergy();

	virtual  void       OnStartMove();
	virtual  void       OnStartRun();
	virtual  void       OnParalyzed();

	virtual  void       OnDamage(const DAMAGE_PRAM &rDamage);
	
	

	virtual  void       OnAttack(DWORD dwSkillCode);
	
	virtual  void       OnMiss(BOOL bCreateDamageMesh);
	virtual  void       OnFootStep(BOOL b2D = FALSE);
	virtual  void       OnShotStart(); // 원거리 공격 *시작*시! (쏘는 순간이 아님)
	virtual  void       OnShotFire(int iShotCount);  // 원거리 공격 발사 순간!!!
	virtual  void       OnShotEnd(); // 원거리 공격 끝났을시! (쏘는 순간이 아님)
	virtual  void       OnDead();
	virtual  void       OnUseSkill();
	virtual  void		RefreshAppearRender(BOOL refresh_bone = FALSE);

	virtual  DWORD      GetCurrentTileAtt( BOOL bForce );

	float               GetRangeWithHero();

    void				SetDestWarp(const WzVector& vDest) { m_vDestWarp = vDest; } 
    const WzVector&     GetDestWarp() const { return m_vDestWarp; }

	virtual void        RenderInfo();

	virtual float       GetArrowTargetHeight();
	virtual WzVector    GetArrowTargetPos();

	// 바닥속성
	DWORD               GetTerrainAttribute();
	// 타게팅 카운트
	void                AddTargetingCount(DWORD dwTargeter);
	void                RemoveTargetingCount(DWORD dwTargeter);
	int                 GetTargetingCount();

	void                SetMoveTarget(DWORD dwMoveTarget);
	DWORD               GetMoveTarget();
	void                ReleaseMoveTarget();
    void                ReleaseHeroTarget();

	// 아이콘 출력
	int		            GetEmoticonIcon() const;
	void                ShowEmoticonIcon(int iEmote,int iDelay);
	void                ProcessEmoteDelay(DWORD dwTick);
	void                FreeEmoteIcon();
	// 사운드
	void                StopVoice();	
	// 스킬 사용 조건 체크 
	virtual BOOL        CanUseSkill(SLOTCODE /*skillID*/,BOOL /*bShowFailMessage = FALSE*/){ return TRUE; }
	virtual BOOL	    CanUseSkillLimitMP(SLOTCODE skillID,BOOL bShowFailMessage = FALSE);
	virtual BOOL	    CanUseSkillLimitHP(SLOTCODE skillID,BOOL bShowFailMessage = FALSE);
	virtual BOOL	    CanUseSkillLimitCoolTime(SLOTCODE skillID,BOOL bShowFailMessage = FALSE);
	
	//하이드관련 함수
	virtual void		SetHideReserve(BOOL bFlag){}
	virtual BYTE		GetHideReserve(){return 0;} 
	virtual void		SetForceHideSkill(BYTE byState){}	
	virtual void		SetHideSkill(BOOL bFlag){}
	virtual BYTE		GetHideSkill(){return 0;} 

    //하이드효과
	virtual void		StartExtraEffect(){}
	virtual void		EndExtraEffect(){}

	BOOL                IsInNotprocessedList(DWORD dwAttackSerial);
	void                RemoveNotprocessedList(DWORD dwAttackSerial);
	void                AddNotprocessedList(DWORD dwAttackSerial);


    void		        SetCameraAlpha(BOOL bFlag) { m_bCameraAlpha = bFlag;}
    BOOL		        GetCondition() { return m_byCondition; }
    void		        SetSitState( BOOL bState ) { m_bSitting = bState; }
    BOOL		        GetSitState( void ) { return m_bSitting; }

#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
    void		        set_sitting_packet_delay( BOOL state ) { sitting_packet_delay_ = state; }
    BOOL		        sitting_packet_delay(void) { return sitting_packet_delay_; }
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG

    void		        SetStartMoveTime( DWORD dwTime );
    DWORD		        GetStartMoveTime( void );

    int                 GetSeekNodeCount() const { return m_nSeekNodeCount; }
    void                SetSeekNodeCount(int seek_count) { m_nSeekNodeCount = seek_count; }

    void                set_unitid_for_trigger(DWORD unitid) { unitid_for_trigger_ = unitid; }
    DWORD               unitid_for_trigger() { return unitid_for_trigger_; }

    //! effect 스케일 조절을 위함
    float GetWaterEffectSizeFromBoundingVolume();

private:
    void                _ProcessRemainAttackResult( DWORD attack_serail, BOOL is_right, BOOL is_additional_effect );

	BOOL                _ProcessAttackResult(ATTACK_RESULT* pResult, BOOL bRight = FALSE, BOOL bAddtionalEffect = TRUE);

	std::list<HANDLE>   m_listDamageEffectHndl;

private:
    int m_nSeekNodeCount;  //!< default = 20000

public:
    // [3/10/2011 iforall]
    // Player, PlayerMonster 클래스에서 Appearance/Equip 관련 공통코드 가져옴
    // 애니매이션/전투/액션 등에 관한 공통코드도 가져온 이후 분리작업 필요
    virtual void AnimateUnit();

//private:
    //cCharacterData m_CharacterData;

    void CreateCharacterVariation(APPEARANCEINFO* appearance_info, BOOL only_bone);
    void DestroyCharacterVariation();

    //CharacterVariation* GetCharacterVariation();
    //eCHAR_TYPE GetClass();
    //virtual void SetClass(eCHAR_TYPE character_class);
    //eCHAR_TYPE GetImageClass();
    //GenderType::Value GetGender();
    //DWORD GetClassCode();
    //virtual void SetGender(GenderType::Value gender);
    //bool GetRealEquipFlag(int index) const;
    //void SetRealEquipFlag(int index, bool equip_flag);
    //void ClearRealEquipFlag();

    //EquipmentContainer* GetEquipmentContainer();
    //void SetEquipmentContainer(EquipmentContainer* equip_container);
    //int GetPartVariation(int part_index);
    //void SetPartVariation(int part_index, int variation, bool is_refresh);
    //float GetHeightVariationValue();
    //int GetHeightVariation();
    //virtual void SetHeightVariation(int variation);
    //void GetFullsetEffectInfo(int index, sFullSetEffectInfo& effect_info);
    //void SetFullsetEffectInfo(int index, sFullSetEffectInfo& effect_info);
    //void ClearFullsetEffectInfo();
    //void ChangeFullsetResource(WORD setitem_optioninfo_setgroupcode);
    //void RestoreFullsetResource();
    //void GetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param);
    //void SetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param);
    //void ClearItemEffectParam();
    //void ClearItemEffectParam(int index);
    //std::vector<sAttachEffectForSetItem>* GetFullsetEffectAttachInfo();
    //void InitAttachEffectForSetItem();
    //void ProcessAttachEffectForSetItem(DWORD tick);
    //void RefreshAttachEffectForSetItem(eKIND_OF_EQUIPED_SETITEM kind_of_equiped_setitem,
    //                                   WORD setitem_optioninfo_setgroupcode);
    //void ReleaseAttachEffectForSetItem();

    //bool IsSetitemEquipmentSlot(int equipment_slot_index);
    //eKIND_OF_EQUIPED_SETITEM GetEquipedKindOfSetItem(OUT WORD& setitem_optioninfo_setgroupcode);
    //BOOL GetAttachEffectForSetItemPos(WzVector& OUT attach_position);
    //int GetAttachEffectForEliteItemBone();

    //! 날개가 펴져 있는지체크 한다.
    //! 기본적으로 PC만 날개가 달리기때문에 Player class에서 상속 받아서 구현한다.
    virtual bool is_attached_wing_accessory(OUT int* attach_index) { return false; }

    //BOOL IsHideHelmet();
    void HideHelmet(BOOL is_hide, BOOL is_refresh = TRUE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    //BOOL IsHideWing();
    void HideWing(BOOL is_hide, BOOL is_refresh = TRUE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    void HideCostume(BOOL is_Hide,BOOL is_refresh = TRUE);
#endif

    //sFullSetChangeItemInfo* GetFullsetChangeInfo();
    //void SetFullsetChangeInfo(sFullSetChangeItemInfo& change_item_info);
    //HANDLE GetDestroyShieldEffect();
    void CreateShieldEffect();
    void UpdateShieldEffect();
    void ReleaseShieldEffect();

    virtual BYTE GetMyResist() { return NpcElementWeakness::eElementWeakness_Default; }

    //! 2011.7.28 / i4u4me / 스타일이 제 정의 되면 사라질 코드
    //! 임시로 하나의 스타일에 2종류의 애니메이션이 나오게 하기 위함
    WzID    GetNormalAttackAniCode(BASE_STYLEINFO* info, eATTACK_SEQUENCE attack_sequence, int ani_type);


protected:
    //void SetDestroyShieldEffect(HANDLE effect_handle);
    static void CheckEmptyHelmet(int equip_pos, 
                                 APPEARANCEINFO& parts_info, 
                                 int hair_variation, 
                                 int face_variation);
    static void SetEquipAppearHelmet(APPEARANCEINFO& parts_info, 
                                     SCItemSlot& item_slot, 
                                     int hair_variation, 
                                     int face_variation,
                                     BOOL hide_helmet,
                                     GenderType::Value gender );
    static void SetEquipAppearInfo(EQUIPAPPEARINFO* parts_info, SCItemSlot& item_slot);
    
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    void RefreshSpaRender(BOOL refresh_bone = FALSE);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

public:
    //!_NA003109_STYLE_PRESENTATION_IMPROVEMENT
    // 버서커, 드래곤은 2타 , 3타 혼용
    struct  
    {
        int combo_max_count;

        //BASE_STYLEINFO, eATTACK_SEQUENCE, ani_type 조합해서 애니코드를 구한다.
        //0 2타 애니, 1:3타애니1, 2:3타애니2
        int ani_type; 
    }NormalAttack;

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 상태이상 관련
	enum eDownAnimationState
	{
		DOWN_ANIMATION_STATE_START,	// 쓰러지는 모션			F001
		DOWN_ANIMATION_STATE_DOWN,	// 쓰러져 누워있는 모션		F002
		DOWN_ANIMATION_STATE_GETUP,	// 일어서는 모션			F003
		DOWN_ANIMATION_STATE_NONE,
	};
	eDownAnimationState m_DownAnimationState;
	DWORD				m_DownTime;		// 다운 상태가 끝나는 시간

	enum eAirAnimationState
	{
#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
        // 더블어택 BLOW > SLASH > 종료
        // 에어블로우 BLOW > DOWN > DOWN_STUN > GETUP > 종료
        AIR_ANIMATION_STATE_BLOW,
        AIR_ANIMATION_STATE_SLASH,
        AIR_ANIMATION_STATE_DOWN,
        AIR_ANIMATION_STATE_DOWN_STUN,
        AIR_ANIMATION_STATE_GETUP,
#else // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
        AIR_ANIMATION_STATE_BLOW,
        AIR_ANIMATION_STATE_DOWN,
        AIR_ANIMATION_STATE_GETUP,
#endif // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL

		AIR_ANIMATION_STATE_NONE,
	};
	eAirAnimationState m_AirAnimationState;
	
	struct sAirData
	{
        enum eMoveType
        {
            MOVE_TYPE_NORMAL = 0,
            MOVE_TYPE_AIRBLOW,
        };

        eMoveType m_MoveType;

        float m_MaxHeight;
        DWORD m_StartTime;
        DWORD m_DownTime;


		float m_fGravity;
		float m_fUpperForce;
		float m_fDownForceLimit;
		float m_fHighLimit;

        BOOL m_NoDown;

		DWORD m_AirTime;
	};
	sAirData m_AirData;

	struct sFallDownData
	{
		float m_fGravity;
		float m_fUpperForce;
	};
	sFallDownData m_FallDownData;

	void SetIdleState();
	void SetStun( const WzVector& CurrentPosition );
	void SetKnockBack( const WzVector& CurrentPosition, const WzVector& TargetPosition );
	void SetKnockBack2( const WzVector& CurrentPosition, const WzVector& TargetPosition );
	void SetKnockBack3( const WzVector& CurrentPosition, const WzVector& TargetPosition );
	void SetDown( DWORD Duration );
	void SetKnockBackDown( const WzVector& CurrentPosition, const WzVector& TargetPosition, DWORD DownDuration );
	void SetAir( DWORD Duration );
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    void SetAirBlowAir( DWORD Duration ); // 에어블로우 Air를 따로 정의
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

	void UpdateAir( DWORD TickTime );
	void AddAirForce( float fAddUpperForce, float fHeightLimit );

	void SetFallDown( float fGravity, float fUpperForce );
	void UpdateFallDown( DWORD TickTime );
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    BOOL CheckDominationCollectState(); // 스턴이나 다른 상태를 바꾸기 전에 점령전에서 채집 중인지 판단한다.
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
};

DWORD ConvertServerToClientEffect(DWORD dwEffect);
