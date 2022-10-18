#pragma once
//------------------------------------------------------------------------------
/**

    ���� ���� �ٸ� ���� ĳ���� Ư�� ���� 
    @author

    @since

    @remarks
        -2005.06.07 �ҽ� ����,SCPlayer ���� ����� ����(����)
        - 
*/

#include "Character.h"
#include "SetItemOptionInfoParser.h"
#include "PlayerDef.h"
#include "PlayerData.h"


class CWzUnitDraw;
class EquipmentContainer;
class SCSlotContainer;
class SkillAttrCalculator;
class ItemAttrCalculator;
class Rider;
class RiderContainer;

class cMoveFlag
{
private:
	int m_MoveFlag;

public:
	cMoveFlag() { m_MoveFlag = 0; }

	void Reset() { m_MoveFlag = 0; }

	int GetMoveFlag() { return m_MoveFlag; }
	void SetMoveFlag( int MoveFlag ) { m_MoveFlag = MoveFlag; }

	BOOL IsMove() 
	{
		// ���� �̵��� �ƴ�
		return (m_MoveFlag & KEYBOARDMOVE_FOWARD) || (m_MoveFlag & KEYBOARDMOVE_BACKWARD) || (m_MoveFlag & KEYBOARDMOVE_LEFTSTEP) || (m_MoveFlag & KEYBOARDMOVE_RIGHTSTEP); 
	}

	void AddForward() { m_MoveFlag |= KEYBOARDMOVE_FOWARD; }
	void AddBackward() { m_MoveFlag |= KEYBOARDMOVE_BACKWARD; }
	void AddLeftTurn() { m_MoveFlag |= KEYBOARDMOVE_LEFTTURN; }
	void AddRightTurn() { m_MoveFlag |= KEYBOARDMOVE_RIGHTTURN; }
	void AddLeftStep() { m_MoveFlag |= KEYBOARDMOVE_LEFTSTEP; }
	void AddRightStep() { m_MoveFlag |= KEYBOARDMOVE_RIGHTSTEP; }
};


class Player : public Character//, public SCPlayer
{
public:
	Player( void );
	virtual ~Player( void );

protected:
    cPlayerData                 m_PlayerData;
    //���� Ŭ���̾�Ʈ ���� �Ӽ� 
    RiderContainer *            m_pRiderContainer;
	int							m_iCurTile;         //  ���� ��� �ִ� Ÿ�� �Ӽ�, ���� ���� �Ӽ� �˻簡 ���� ������� ���� �˻��� ������ ��� �̵� �Ҷ��� ���� �ؼ� �˻� ���� ��� �Ҽ� �ְ� 
	int                         m_iOldTile;         //  ���� ��� �ִ� Ÿ�� �Ӽ� 
	DWORD                       m_dwOldTileAtt;

	virtual void				InitContainer();
	virtual void				ReleaseContainer();

	virtual void    AnimateUnit();
    virtual float   GetHeightOffset();
//
//#ifdef __NA_0_20080415_PROCESS_END_PROECSS
//	virtual int					OnEndProcess( DWORD dwTick );
//#endif

private:
    int m_ItemScore;    // �������, �÷��̾ ���

public:
    void                        SetItemScore(DWORD TotalItemScore ) { m_ItemScore = TotalItemScore; }
    virtual int                 GetItemScore() { return m_ItemScore; }

	virtual void				ApplyTerrainColor(BOOL bApply);

	virtual void				BeforeRefreshTileAtt();
	virtual void				AfterRefreshTileAtt();
	virtual void                ShowConflictMessage(){}

	virtual SCSlotContainer *	GetSlotContainer( SLOTIDX slotIdx );
	virtual BOOL				Create( DWORD dwObjectKey, DWORD class_code = 0 );
	virtual void				Destroy( void );

	//@ �÷��̾� ���� ���� ����
	void						SetPlayerInfo(PLAYER_RENDER_INFO_EX* pPlayerInfo);

	virtual BOOL				IsSimpleRenderMode();	// ����ȭ�� ������ ����� ���ΰ�. (�ε��� �� �� �÷��̾�)

    virtual void SetClass(eCHAR_TYPE character_class);
    virtual void SetGender(GenderType::Value gender);
	
	// ��Ţ ���� 
    virtual void				RefreshAppearPet();
    virtual void				RefreshAppearRider();

	//�̸�Ƽ�� ����
	virtual void                SetCondition(BYTE byCondition);
	
	virtual BOOL				ProcessSkillEffect(DWORD dwTick);
	

	virtual float				ProcessAnimation( DWORD dwTick ,WzEvent *pEvent = NULL,BOOL bIgnoreEvent = FALSE);
	virtual int					DoAction(PLAYER_ACTION *pAction);

    virtual void				TransForm();
    virtual void				UnTransForm();
			void                ForceTransFrom();
			void				ForceUnTransForm();

	virtual WzVector			GetHeadBonePos();

	void                        SetTransSkillCode(DWORD dwSkillCode);
	DWORD                       GetTransSkillCode();

	void						ResetMoveFlag();
	void						SetMoveFlag(int MoveFlag);
	void						AddMoveFlag(int MoveFlag);
	void						RemoveMoveFlag(int MoveFlag);
	BOOL						CheckMoveFlag(int MoveFlag);


	int							GetMoveFlag();
	void						SetOldMoveFlag(int iOldMoveFlag);
	int							GetOldMoveFlag();
	void						SetOldKeyboardDirection(WzVector *pVector);
	WzVector					GetOldKeyboardDirection();

	virtual void                SetSpecialMoveFlag(BYTE type);
	BYTE                        GetSpecialMoveFlag();

    void GetEquipItemTotalInfoByCheckValidItem(EQUIP_ITEM_INFO& OUT equip_item_info);
	VOID GetEquipItemTotalInfo( EQUIP_ITEM_INFO & OUT rItemInfo );
	VOID SetEquipItemTotalInfo( const EQUIP_ITEM_INFO & IN rItemInfo );
    VOID SetEquipItemBaseInfo( POSTYPE total, EQUIP_ITEM_BASEINFO::data_type* IN pSlot );
    void RefreshEquipData();
    
	void						SetClientChar(const CLIENT_CHARACTER_PART& ClientPart);


	virtual void				PlayDamageAnimation(BOOL bRight,BOOL bCritical);

	virtual WzID				GetCurrentReviveAnimation();
	virtual WzID				GetCurrentKnockBack2Animation();

#ifdef _DEV_TAIYO_OPTON_ADD
	virtual BOOL				CanUseSkillLimitCoolTime(SLOTCODE skillID, BOOL bShowFailMessage = FALSE);
#endif

	virtual WzID				GetCurrentAttackAnimation();	
	virtual WzID				GetCurrentIdleAnimation();
	virtual WzID				GetCurrentMoveAnimation();
	virtual WzID				GetCurrentDeathAnimation();
	virtual WzID				GetCurrentSitAnimation();
	virtual WzID				GetCurrentSittingAnimation();
	virtual WzID				GetCurrentStandUpAnimation();

	virtual WzID				GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical);
	virtual WzID				GetCurrentLeftTurnAnimation();
	virtual WzID				GetCurrentRightTurnAnimation();
	virtual WzID				GetCurrentLeftStepAnimation();
	virtual WzID				GetCurrentRightStepAnimation();
	virtual WzID				GetCurrentBackStepAnimation();
	virtual WzID                GetCurrentBlockAnimation();

    //! ���� �ִϸ��̼�
    virtual WzID				GetCurrentRiderSpreadAnimation();
    virtual WzID				GetCurrentRiderFureAnimation();


	//Ư�� �̵�
	virtual WzID				GetCurrentSpecialLeftStepAnimation();
	virtual WzID				GetCurrentSpecialRightStepAnimation();
	virtual WzID				GetCurrentSpecialFrontStepAnimation();
	virtual WzID				GetCurrentSpecialBackStepAnimation();

	//���� �̵� 
	virtual WzID				GetCurrentJumpStartAnimation();
	virtual WzID				GetCurrentJumpKeepAnimation();
	virtual WzID				GetCurrentJumpEndAnimation();
	virtual WzID				GetCurrentJumpLandAnimation();

    //------------------------------------------------------------------------------
    //! Returns:   WzID
    //! Qualifier: ���� �ִ� ���¿��� space key�� ������ �ٸ� ������� �ٲ�.
    //! Parameter: WzID default_ani_id : �⺻ �ִϸ��̼�
    //------------------------------------------------------------------------------
    virtual WzID                GetCurrentSpreadOtherAnimation(WzID default_ani_id, INOUT ePLAYER_ANI& player_ani);

	

	virtual WzID				GetCurrentJumpEndRunAnimation();

	virtual WzID				GetCurrentJumpStunAnimation();

	virtual WzID				GetCurrentJumpStunKeepAnimation();

	virtual WzID				GetCurrentJumpStunEndAnimation();

	virtual WzID		        GetCurrentCamAnimation();

	virtual float				GetAngleForMoveDirection();


	//==================================================
	// 0905��Ŷ���� ��
	//==================================================
	void						SetPlayerPetInfo(PLAYER_RENDER_INFO_EX* pPlayerInfo);
	BOOL						CreatePetSummon(WORD wPetIndex);							//�����
	virtual void				DeletePetSummon();								//������ 
	BOOL						IsPetReturn(void) { return m_PetInfo.m_bPetSummoned; }		//���ȯ����
	DWORD						GetPetObjectKey(void) { return m_PetInfo.m_dwPetObjKey; }	//��Ű��
	WORD						GetPetIndex() { return m_PetInfo.m_wPetIndex; }
	virtual void				SetPetAttributes(POSTYPE pos, BYTE bIsFullness, BYTE byValue);
	void						SetPetEmotion(ePET_EFFECT eState);
	void						SetPetNameUse(BYTE bUseName) { m_PetInfo.PLAYER_.m_byPetName = bUseName; }		
	BYTE						IsSetPetName() { return m_PetInfo.PLAYER_.m_byPetName; }
	TCHAR*						GetPetName() { return m_PetInfo.PLAYER_.m_sPetName; }
	void						SetPetName(TCHAR* pszName);

private:
    char				        GetRiderKindCode();
    void						AnimateRider();
    BOOL						ProcessRider(DWORD dwTick);
    void						RenderRider();
    void						SetRindingState(BOOL bFlag){ m_bRiding = bFlag; }		
public:
	virtual BOOL				ProcessInterpolation( DWORD dwTick ,float fInterpolateRatio = 0.93f);
	BOOL						IsRiding();
    virtual void		        SetAnimation(WzID idAnimation, BOOL bLoop = FALSE);
    virtual void		        SetAnimation(const char* pAnimation, BOOL bLoop = FALSE);
	void						SetPlayerAnimation(WzID wzID, ePLAYER_ANI eState, BOOL bLoop);
	virtual void				CreateRider(DWORD dwID,BOOL bEffect=FALSE);
	virtual void				DestroyRider();
	virtual void				AttachRiderPart(int nIdx, DWORD dwItemCode);
    virtual void				DetachRiderPart(int nIdx);
	void						RiderSubtype(int nPartIdx,int nSubType);
	WzVector					GetNormal(){return m_vNormal;}
    int				            GetRiderIdx(){return m_nRiderIdx;}
    virtual void				RefreshRider();
    void				        SetKeyBoardJumpFlag(BOOL bFlag){m_bRiderJumpFlag=bFlag;}

    Rider* rider() { return m_pRider; }
    bool is_riding_jump() const { return is_riding_jump_; }
    void set_is_riding_jump(bool flag) { is_riding_jump_ = flag; }
    float GetJumpHeight();
    float GetJumpDistance();
#ifdef _NA_20120220_JUMP_RENEWAL
#else
    bool CalcJumpTime(float move_length);
#endif

    //! ���� �������� �����Ǿ� �ִ��� üũ
    bool is_wing_equip(OUT POSTYPE* at_pos);

    //! ������ ���� �ִ��� ĳ���� ���� ���ϸ��̼� üũ�� ���
    bool is_spread_wings() { return is_spread_wings_; }
    void set_spread_wings(bool val);

    //! ������ ���ҽ��� �޷� �ִ��� AppearanceManager ���� �˻�
    bool is_attached_wing_accessory(OUT int* attach_index);


    BOOL				        IsEventWeapon();

	virtual void				ProcessRushClone();
	virtual DWORD				GetCurrentAttackStyle();
	virtual void				PlayEvadeAnimation();
	virtual void				PlayBlockAnimation();
	virtual void                SetIdleAnimation();
	void						SetBeHaveState(ePLAYER_BEHAVE_STATE eBeHaveState);
	ePLAYER_BEHAVE_STATE		GetBeHaveState();
	

	void                        ProcessStandAnimationDelay(DWORD dwTick);

    BOOL                        CheckChangeAttackStyle( DWORD dwStyle );
    BOOL						SetCurrentAttackStyle(DWORD dwStyle);
	void                        SetCurrentNomalAttackStyle(DWORD dwWeaponType);	//����Ÿ������ ��Ÿ�� ���� ���� 
    BOOL                        CheckAttackStyle(DWORD dwStyle);				//���� â������ ����� ��Ÿ�� ���� ���� �˻� 

	BOOL                        CheckStyleForWeapon(DWORD dwWeaponType);		//���� �����ÿ� �׻� üũ �ؾ� �Ѵ�.
	void                        RefreshPunchStyle();//ĳ���� Ÿ������ ��ġ �ڵ带 ��´�.

	void						PlayDamageSound(eWEAPONSOUNDKIND eSoundKind);
	virtual void				CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId);

	virtual BOOL				Process( DWORD dwTick );

	// ����




	// ��Ű�� ����
	virtual void				OpenCrossbow();
	virtual void                CloseCrossbow();
	void                        ShotCrossbow();

	void                        LaunchEtherWeapon();

	int							GetValkyrieShotDelay();
	BOOL						CanValkyrieAction();
	virtual void				ValkyrieShot(DWORD dwAttackSerial); // ����~
	WzID						GetValkyrieShotAnimation(); // ����~
	void						ProcessValkyrieShot(DWORD dwTick);
	virtual DWORD				GetValkyrieShotSerial();

	WzID						GetValkyrieReloadAnimation(); 
	int							GetBulletCount();
	void                        SetBulletCount(int iCount);
	int                         DecreaseBulletCount();
	void                        ValkyrieReload( BOOL bAnimation = TRUE );


	virtual BOOL				OnUseItem(DWORD dwItemCode);	
	virtual BOOL				OnUnUseItem(DWORD dwItemCode);	

    virtual void                OnLevelUp();
	
	virtual  void			    OnDamage(const DAMAGE_PRAM &rDamage);

	virtual  void				OnAttack(DWORD dwSkillCode);
	
	virtual void				OnFootStep(BOOL b2D = FALSE);
	virtual void				OnShotStart();
	virtual void				OnShotFire(int iShotCount);
	virtual void				OnShotEnd();
	virtual void				OnParalyzed();
	virtual int					GetShotCount();

	BOOL				        IsSummonIntensify();
	void				        ProcessSummonIntensifyEffect();
	BOOL                        IsFlyFinishWeapon();

	void                        RenderTextOnHeadGuild(TCHAR *szString, WzColor color, int yoffset);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    void                        RenderGoldMarkOnHead(RECT rc, float fDepth, eGUILD_MARK_RELATION eRelation);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	void                        RenderRankingGradeOnHead(RECT rc, float fDepth, eGUILD_MARK_RELATION relation);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    void                        RenderDominationMarkOnHead(RECT rect, float depth, eGUILD_MARK_RELATION eRelation);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	bool		                IsExistGuildMark() {return m_GuildRenderInfo.m_BackGroundIndex && m_GuildRenderInfo.m_PatternIndex;}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    const TCHAR* GetNameWithTitle() { return name_with_title_.c_str(); }
    void SetTitleName(WORD title_code);
    WORD GetTitleCode();
#endif //_NA_003027_20111013_HONOR_SYSTEM
	
public:
	virtual BOOL				Render( void );

	virtual float				GetMeshHeight();
	
	
	virtual void                RenderInfo();

	virtual WzVector			GetWeaponBonePos(int index);//���� virtual�� ����
	virtual WzVector            GetArrowBonePos(int index);
	virtual void				SetWeaponKind(DWORD dwWeaponKind);

    virtual BOOL				SetPosition( const WzVector& wvPos, int iTileIndex = -1);
	virtual BOOL				SetPosition( const HONGPOS& Pos );

	virtual BASE_ITEMINFO *		GetWeaponInfo();
	int                         GetShotBlendBone();

	BASE_PLAYERINFO*			GetCharInfo(){ return m_PlayerData.GetCharacterInfo();}
    void                        SetCharInfo( const BASE_PLAYERINFO* pPlayerInfo ) { m_PlayerData.SetCharInfo( pPlayerInfo ); }

	void						SetPartVariation(int iPart, int iVariation);

	WzColor						GetEstablisherColor();
	
	virtual void SetHeightVariation(int iIndex);

	// �̸� ���� 
	virtual void		SetName( const tstring& Name );


	PLAYER_MAKE_TYPE	GetPlayerType() const;
	void				SetPlayerType(PLAYER_MAKE_TYPE Type);

	eCHAREX_TYPE		GetClassExType();

    void TransformGenderType();


	// ���� ����
	virtual void		SetExp(EXPTYPE dwExp);
	virtual EXPTYPE		GetExp();
	virtual EXPTYPE		GetNextExp();

	virtual void		SetLevel(LEVELTYPE LV);
	virtual void		SetHP(DWORD dwHP );
	virtual void		SetMP(DWORD dwHP );
	virtual void		SetMaxHP( DWORD iHP );
	virtual void		SetMaxMP( DWORD iMP );
	virtual void        SetAttackSpeedRatio(int iSpeed);
	virtual void        SetMoveSpeedRatio(int iSpeed);

	virtual LEVELTYPE	GetLevel();

	virtual DWORD		GetHP();
	virtual DWORD		GetMP();
	virtual DWORD		GetMaxHP();
	virtual DWORD		GetMaxMP();
    virtual float		GetHP_Percent();
    virtual float		GetMP_Percent();
	virtual DWORD		GetSD();
	virtual DWORD		GetMaxSD();
	virtual void		SetSD(DWORD dwSD);
	virtual void		SetMaxSD(DWORD dwMaxSD);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual PLAYERFP    GetFP();
    virtual PLAYERFP    GetMaxFP();
    virtual void        SetFP(const PLAYERFP& fp);
    virtual void        SetMaxFP(const PLAYERFP& max_fp);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//���̵彺ų���� �Լ�
	virtual void		SetHideSkill(BOOL bFlag);	
	virtual BYTE		GetHideSkill(){return m_sHideInfo.byHideState;} 
    virtual void		SetForceHideSkill(BYTE byState){m_sHideInfo.byHideState=byState;}	
	virtual void		SetHideReserve(BOOL bFlag){  m_sHideInfo.byHideReserve = bFlag;}
	virtual BYTE		GetHideReserve(){return m_sHideInfo.byHideReserve;} 
    void		        RefreshHideState(BOOL bClear=FALSE);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // ���� ������
    void                SetBattleGroundHideMode(bool is_hide);
    BattleGroundHideMode CheckBattleGroundHideMode();
    void                RefreshBattleGroundHideMode();
    void                set_battle_ground_hide_mode(BattleGroundHideMode hide_mode) { battle_ground_hide_mode_ = hide_mode; }
    BattleGroundHideMode battle_ground_hide_mode() { return battle_ground_hide_mode_; }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    //�ٸ��÷������ ȿ������(�������̵��ؼ����)
    virtual void		StartExtraEffect();
    virtual void		EndExtraEffect();
private:    
    BYTE                GetHideCondition();
    HIDE_SKILL_INFO     m_sHideInfo;
    BattleGroundHideMode battle_ground_hide_mode_; // ���� ������ (�Ʊ����� ������)

public:

	virtual float       GetMoveSpeedModifier();
	virtual float       GetAttackSpeedModifier();
	virtual WzID        GetCurrentTransFormAnimation();

	virtual float       GetAttackRange( void );
	virtual BOOL        IsRangedNormalAttack();

	//// ��ųȿ�� ����
	//virtual	int			AddSkillEffect(SKILL_EFFECT *pEffect);
	//virtual BOOL		DeleteSkillEffect(int iEffectID);



	virtual DWORD       CreateProjectile(Character *pTarget,int iArrowBone = 0);
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


	VOID				SetTeam(int iTeam);
	int					GetTeam() const;

	void				ApplyStyleBuff( DWORD dwStyleCode ,SkillAttrCalculator &SkillCalc);

	BOOL			    CanBeAttackTarget();

	//summon	
	void                AddSummonId(const DWORD &dwSummonId);
	BOOL                RemoveSummonId(const DWORD &dwSummonId);
	DWORD               GetSummonId()const;
	int					GetSummonSize()const;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BOOL                FindSummon(const DWORD& summon_id)const;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    PLAYER_GUILD_RENDER_INFO& GetGuildRenderInfo() { return m_GuildRenderInfo; }
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	CLIENT_PLAYER_RANKING_GRADE_INFO& GetClientPlayerRankingGradeInfo() { return client_player_ranking_grade_info_; }
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
    void SetDominationStateInfo(const DOMINATION_STATE_TOTAL_INFO* domination_state_info);
    STLX_VECTOR<MAPCODE>& GetDominationStateInfo() { return domination_state_info_; }    
    // ��� �������� �� ���ɻ��� ���� �ʱ�ȭ
    void ClearDominationStateInfo();
    void CreateDominationGuildRewardEffect(DWORD guild_duty);
    void RemoveDominationStateEffect();
	GUILDGUID			GetPlayerGuildID(){return m_GuildRenderInfo.m_GuildGuid;}

    void                SetGuildRenderInfo(const GUILD_RENDER_INFO* pGuildInfo);
	void				SetGuildName(char * pszGuildName, int iLen );
    void                ClearGuildRenderInfo();

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	void                SetClientPlayerRankingGradeInfo(const CLIENT_PLAYER_RANKING_GRADE_INFO* client_player_ranking_grade_info);
	void                ClearClientPlayerRankingGradeInfo();
	void                RemovePlayerRankingEffect();
	void                CreatePlayerRankingEffect();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

    void CheckDominationRelation(); // ������ �ȿ����� ���Ͱ��� ����

	TCHAR *             GetGuildName();
	
	
	BOOL				IsExistGuild() { return m_bExistGuild; }
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	BOOL                get_is_client_player_ranking_grade_() const { return is_client_player_ranking_grade_; }
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	BOOL                IsCasting();
	void                SetCast(BOOL bCast);

	EXPTYPE				GetAccumExp(LEVELTYPE LV);

	void                SetStandUp(BOOL bFlag);
	BOOL                IsStandUp();
	
    virtual void SetPolymorph( BOOL Active );



public:

	//		GM ���
	void				SetGMGrade( BYTE byGMGrade )		{ m_byGMGrade = byGMGrade; if( m_byGMGrade ) SetChatColor( WzColor_RGBA( 170, 124, 255, 255 ) ); }
	BYTE				GetGMGrade( void )					{ return m_byGMGrade; }
	BOOL				IsGM();
	//		PC �� ���
	void				SetPCBangMode( BYTE byPCBang )		{ m_byPCBangUser = byPCBang ;}
	virtual BYTE		GetPCBangMode()						{ return ( m_byPCBangUser &&  !m_byBlockPCBang);}
	void				SetBlockPCBang( BYTE byBlock )      { m_byBlockPCBang = byBlock; }
	BYTE				GetBlockPCBang()					{ return m_byBlockPCBang; }

	//		���� ���
	void				SetInvisible( BOOL bInvisible )		{ m_bInvisible = bInvisible; }
	BOOL				IsInvisible( void )					{ return m_bInvisible; }

	//		����

	void				SetInvincible( BOOL bInvincible ,int iTime=0) { m_bInvincible = bInvincible; m_iInvicibleTime=iTime;}
	BOOL				IsInvincible( void )				{ return m_bInvincible; }

	BOOL                HasHPInfo();
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    BOOL                HasSDInfo();
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	void                SetHPInfo(HPINFO *pInfo);
	TCHAR *				GetMarketTitle() const;
	void				SetMarketTitle(const char * pszName, int iLen);


	BOOL                IsAttackBeginPlayer();
	BOOL                IsChao();
	BYTE		        GetChaoState();

	void                SetChaoState(BYTE bState);

	void                SetAttackBeginPlayer(BOOL bAttack);//���� ���� 

	void                SetKeyboardMoveTime(DWORD dwTime);
	void                AddKeyboardMoveTime(DWORD dwDelta);
	DWORD               GetKeyboardMoveTime();


	void				_renderinfo_GM();
	void				_fillRenderinfoState();
	void				selectRenderinfo_Color();
	BOOL				IsShowRenderinfo();
	BOOL				IsPlayerHaveRenderinfoState() { return m_bRenderinfoState; }
	BOOL				IsShowVedorTitle();
	void				_renderinfo_VendorTitle();
	void				renderInfo_InCharacterSenen();
	void				renderInfo_InBattleScene();
	void				renderInfo_Guild();
	//BOOL                IsHero() { return m_bHero; }
    virtual bool    IsHero() { return false; }
	void				GetClassExName(OUT TCHAR * pszOutString);
	virtual void		CreateEtherWeaponEffect(bool bShotEffect = false, bool bFillupEffect = false);
	virtual void		DestoryEtherWeaponEffect();
	void				UpdateEtherWeaponEffectPos();
	virtual bool		IsEtherDeviecBulletChargeing();
	int					GetEtherBulletEffectIndex();
	virtual void		SetEtherActive(BYTE byIndex) { m_EtherIndex = byIndex; }
	BYTE				GetEthreActive() { return m_EtherIndex;}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	void                UpdatePlayerRankingEffect();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

	virtual float		GetBodyRadius();

	DWORD				GetCollectObjKey(){return m_dwCollectObjKey;}
	void				SetCollectObjKey(DWORD dwObjKey){m_dwCollectObjKey = dwObjKey;}

	DWORD				GetCollectID(){return m_dwCollectID;}
	void				SetCollectID(DWORD dwCollectID){m_dwCollectID = dwCollectID;}

	DWORD				GetCollectStartTime(){return m_dwCollectStartTime;}
	void				SetCollectStartTime(DWORD dwTime){m_dwCollectStartTime = dwTime;}

	DWORD				GetCollectMaxTime(){return m_dwCollectMaxTime;}
	void				SetCollectMaxTime(DWORD dwTime){m_dwCollectMaxTime = dwTime;}

	BYTE				GetCollectState(){return m_dwCollectState;}
	void				SetCollectState(BYTE btState){m_dwCollectState = btState;}

	void				DoCollect(DWORD dwTargetKey, DWORD dwCollectionID, DWORD dwTime);
	
    bool receive_equip_info() const { return receive_equip_info_; }
    void set_receive_equip_info(bool val) { receive_equip_info_ = val; }

    BYTE etheria_extract_state() const { return etheria_extract_state_; }
    void set_etheria_extract_state(BYTE val) { etheria_extract_state_ = val; }
    DWORD etheria_extract_time() const { return etheria_extract_time_; }
    void set_etheria_extract_time(DWORD val) { etheria_extract_time_ = val; }

	bool				IsShowByGameOption();

public:

#ifdef _NA_20120220_JUMP_RENEWAL
	struct sJumpData
	{
		cMoveFlag m_JumpFlag;
		WzVector vFoward;

		WzVector m_vJumpDirection;
		WzVector m_vJumpTargetPosition;
		WzVector m_vJumpStartPosition;
		float m_fJumpMoveForce;
		float m_fJumpUpperForce;
		float m_fJumpDistanceToLand;
		float m_fJumpStartHeight;
		float m_fJumpTargetHeight;
		std::vector<WzVector> m_JumpPositionArray;
		float m_fJumpTick;

		BOOL m_bJumpStun;
		BOOL m_bJumpStart;
		BOOL m_bJumpEndEffect;
		HANDLE m_hStunEffect;
		float m_fJumpEndFrame;
		BOOL m_bCreateJumpEndEffect;
		DWORD m_JumpTime;
		int m_JumpStunTime;

		WzVector m_CharacterDirection;

	};
	sJumpData m_JumpData;

#else //_NA_20120220_JUMP_RENEWAL

	struct sJumpData
	{
		cMoveFlag m_JumpFlag;

		float m_fGravity;
		float m_fUpperForce;

		WzVector m_CharacterDirection;
		WzVector m_JumpStartPosition;
		WzVector m_JumpTargetPosition;

		BOOL m_bCreateJumpEndEffect;
		BOOL m_bJumpStun;
		BOOL m_bJumpStart;
		BOOL m_bJumpEndEffect;
		HANDLE m_hStunEffect;
		float m_fJumpEndFrame;
		DWORD m_JumpTime;

		int m_JumpStunTime;

		float m_SrcZ;
		DWORD m_dwVerticalMoveTime;
		DWORD m_dwHorizonMoveTime;

	};
	sJumpData m_JumpData;

#endif //_NA_20120220_JUMP_RENEWAL


protected:

	BOOL                    m_bAttackBeginPlayer;

	int						m_iMoveFlag;	
	BYTE					m_bSpecialMoveFlag;	
	bool receive_equip_info_;
    
    int						m_iOldMoveFlag;
	WzVector				m_vOldDirection;	

	int						m_iValkyrieShotDelayTime;
	int						m_iValkyrieShotDelay;
	int						m_iValkyrieShotCount;
	int						m_iValkyrieShotRemain;
	float					m_fValkyrieShotSpeed;
	int						m_iValkyrieBulletCount;
	int						m_iValkyrieShotAnimVariation;
	DWORD					m_dwValkyrieShotSerial;
	BOOL					m_bValkyrieReload;

	int						m_iValkyrieShotAnimCount;
	int						m_iValkyrieShotAnimSet;

	WORD					m_wAttSpeedRatio;
	WORD					m_wMoveSpeedRatio;
	//BOOL                    m_bHero;
	//DWORD					m_dwAttackStyle;

	DWORD                   m_dwKeyboardMoveTime;

	ePLAYER_BEHAVE_STATE	m_BehaveState;

	PLAYER_MAKE_TYPE		m_PlayerMakeType;
    
	DWORD					m_dwDustTime; // �������� �ֱ�	
	BOOL                    m_bHideHelmet; // ����� ������ΰ�.
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    BOOL                    m_bHideWing;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	BOOL                    m_bInvincible; // ����
	int                     m_iInvicibleTime;
	BOOL                    m_bCasting;//ĳ���� ���ΰ�
	DWORD                   m_dwShieldTime;
	DWORD                   m_dwTransSkillCode;

	// PVP ����
	int						m_iTeam;		// PVP �� �� ��ȣ (���忡���� ���)
	// ��ȯ ���� 

	nArray<DWORD>			m_SummonIdArray;// ��ȯü ���̵� 

	// ����� ���� ����
	// ��� ������ BASE_PLAYERINFO
	BOOL					m_bExistGuild;
	//HANDLE					m_hSummonIntensifyEffect;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	// ��ŷ ������ ���� ����
	BOOL                    is_client_player_ranking_grade_;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    STLX_TSTRING name_with_title_;
#endif //_NA_003027_20111013_HONOR_SYSTEM

protected:
	BYTE					m_byGMGrade;		//	GM ���
	BOOL					m_bInvisible;		//	����

	BYTE					m_byPCBangUser;		// 0 �Ϲ�, 1 PC ��
	BYTE					m_byBlockPCBang;

	BOOL                    m_bHPInfo;          // �������� HP������ �޾Ҵ�.
	BOOL                    m_bStandUp;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    BOOL                    m_bSDInfo;          // �������� SD������ �޾Ҵ�.
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	util::_tstring			m_strMarketTitle;


	TCHAR					m_szCharState[INTERFACE_STRING_LENGTH];
	BOOL					m_bRenderinfoState;
	WzColor					m_RenderinfoColor;

    PLAYER_GUILD_RENDER_INFO m_GuildRenderInfo;
    STLX_VECTOR<MAPCODE> domination_state_info_;
    std::list<HANDLE>	domination_effect_list_;

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	CLIENT_PLAYER_RANKING_GRADE_INFO client_player_ranking_grade_info_;
	std::list<HANDLE>                player_ranking_grade_effect_list_;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE


	HANDLE					m_hEtherWeaponEffect[2][3];
	BYTE					m_EtherIndex;
	PET_INFO				m_PetInfo;
    Rider*					m_pRider;
    BOOL					m_bRiding;
    BOOL					m_bIdleFlag;
    int 					m_nRiderIdx;
    BOOL 					m_bRiderJumpFlag;
    bool                    is_riding_jump_;
	WzID					m_wiPreAniID;
    bool                    is_spread_wings_;

	DWORD					m_dwCollectObjKey;
	DWORD					m_dwCollectID;
	DWORD					m_dwCollectStartTime;
	DWORD					m_dwCollectMaxTime;
	BYTE					m_dwCollectState;
    
    BYTE etheria_extract_state_;
    DWORD etheria_extract_time_;

	HANDLE					m_hCollectStartEffect;
    HANDLE					m_hDominationStampEffect;
public:
	void					SetCollectStartEftHndl(HANDLE hEffect){m_hCollectStartEffect = hEffect;}
	HANDLE					GetCollectStartEftHndl(){return m_hCollectStartEffect;}
    void                    CreateDominationStampEffect();
    void                    DestroyDominationStampEffect();


public:
	BYTE			m_DelChk;	//< 0: �����ȵ� ĳ��, 1,2:������(DB�������) 3: ������ ĳ��
	TCHAR			m_tszDelDate[MAX_TIMEDATA_SIZE+1];
	int						GetCharDelRemainTime();
    TCHAR* GetCharDelRemainTimeString(TCHAR* string);
private:
	void					 _renderinfo_Remain_Char_Del();

	BYTE			m_btIdleAni;
	BYTE			m_btCriticalAttackAni; //(0 == Nomal), (1 == Cri,Double), (2 == Miss, Block)

public:
	void			SetbtCriAttkAni(BYTE btVal){m_btCriticalAttackAni = btVal;}
	BOOL			GetbtCriAttkAni(){return m_btCriticalAttackAni;}

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
private:
    void            RenderDiceOnHead();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _NA_004035_20120227_COSTUME_ITEM
public:
    virtual void SetCostumeItemCode( SLOTCODE code );
    virtual SLOTCODE GetCostumeItemCode();
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	virtual void SetDecorateCostumeItemCode( SLOTCODE code );
	virtual SLOTCODE GetDecorateCostumeItemCode();
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	BOOL m_IsCostumIdleAnimation;	// ���� �ִϸ��̼��� �ڽ�Ƭ �������� ���̵� �ִϸ��̼����� ��ä���� �̸� �ĺ��ϱ� ���� �÷���
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
public:
    virtual void        SetSpaItemCode(SLOTCODE item_code);
    virtual SLOTCODE    GetSpaItemCode();
    void                SetSpaSittingAnimation(DWORD dwTick);
    void                SetSpaSittingAnimationDelay();
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    BOOL                IsInSpaTile_Change_SitIdle(); // �ɱ�IDLE �ִϸ��̼� �ٲٴ� Ÿ��
    BOOL                IsInSpaTile_Change_Sit(); // �ɱ� �ִϸ��̼� �ٲٴ� Ÿ��
#else
    BOOL                IsInSpaTile();
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
private:
    SLOTCODE spa_item_code_;   // ��õ �� ������ �ڵ�
    
    DWORD spa_sit_idle_ani_delay_; // ��õ ���� Ư���ִ� �ֱ�
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

};

inline 	
void                        
Player::SetSpecialMoveFlag(BYTE type)
{
	m_bSpecialMoveFlag=type;
}

inline 	
BYTE
Player::GetSpecialMoveFlag()
{
	return m_bSpecialMoveFlag;
}

inline
VOID				
Player::SetTeam(int iTeam)
{
	this->m_iTeam = iTeam;
}

inline
int					
Player::GetTeam() const
{
	return this->m_iTeam;
}


//----------------------------------------------------------------------------
/**
*/
inline
void                
Player::AddSummonId(const DWORD &dwSummonId)
{
	m_SummonIdArray.Append(dwSummonId);
}

//----------------------------------------------------------------------------
/**
*/
inline 
BOOL                
Player::RemoveSummonId(const DWORD &dwSummonId)
{
	for(int i=0;i<m_SummonIdArray.Size();i++)
	{
		if(m_SummonIdArray[i]==dwSummonId)
		{
			m_SummonIdArray.EraseQuick(i);
			return TRUE;
		}
	}

	return FALSE;
}
//----------------------------------------------------------------------------
/** ��ȯü����
*/
inline 
int
Player::GetSummonSize()const
{
	return m_SummonIdArray.Size();
}
//----------------------------------------------------------------------------
/**
*/
inline 
DWORD
Player::GetSummonId()const
{
	if(m_SummonIdArray.Size())
		return m_SummonIdArray.Front();

	return NONE_ID;
}


//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
inline
BOOL
Player::FindSummon(const DWORD& summon_id)const
{
    for(int i=0;i<m_SummonIdArray.Size();i++)
    {
        if(m_SummonIdArray[i] == summon_id)
        {
            return TRUE;
        }
    }
    return FALSE;
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//------------------------------------------------------------------------------
/**
*/

inline void Player::ClearGuildRenderInfo()
{
    m_bExistGuild = FALSE;
    ZeroMemory(&m_GuildRenderInfo, sizeof(m_GuildRenderInfo));
    ClearDominationStateInfo();
}

//------------------------------------------------------------------------------
inline
void
Player::SetGuildName(char* ptszGuildName, int iLen)
{
	m_bExistGuild = TRUE;

	BASE_PLAYERINFO * pInfo = GetCharInfo();
	if(pInfo)
	{
		ZeroMemory(pInfo->m_tszGuildName, sizeof(pInfo->m_tszGuildName));
		StrnCopy(pInfo->m_tszGuildName, ptszGuildName, MAX_GUILDNAME_LENGTH);
		StrnCopy(m_GuildRenderInfo.m_tszGuildName, ptszGuildName, MAX_GUILDNAME_LENGTH);
	}
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
inline
void Player::ClearClientPlayerRankingGradeInfo()
{
	is_client_player_ranking_grade_ = FALSE;
	client_player_ranking_grade_info_.ranking_type = eSUN_RANKING_TYPE_END;
	client_player_ranking_grade_info_.grade = eSUN_RANKING_GRADE_END;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

//------------------------------------------------------------------------------
/**
	���� ĳ��Ʈ �����ΰ�
*/
inline
BOOL Player::IsCasting()
{
	return m_bCasting;
}

//------------------------------------------------------------------------------
/**
*/
inline
void Player::SetCast(BOOL bCast)
{
	m_bCasting=bCast;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void Player::SetStandUp(BOOL bFlag)
{
	m_bStandUp=bFlag;
}


//------------------------------------------------------------------------------
/**
*/
inline 
BOOL Player::IsStandUp()
{
	return m_bStandUp;
}



//------------------------------------------------------------------------------
/**
*/
inline 
BOOL Player::IsAttackBeginPlayer()
{
	return m_bAttackBeginPlayer;
}

//------------------------------------------------------------------------------
/**
*/
inline
void Player::SetAttackBeginPlayer(BOOL bAttack)
{
	m_bAttackBeginPlayer=bAttack;
}

//------------------------------------------------------------------------------
/**
*/
inline
BOOL Player::IsChao()
{
	BASE_PLAYERINFO* pPlayerInfo=GetCharInfo();

	if(pPlayerInfo)
	{
		if(pPlayerInfo->m_ChaoState==eCHAO_STATE_PREV_CHAO||pPlayerInfo->m_ChaoState==eCHAO_STATE_CHAO)
		{
			return TRUE;
		}

	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE Player::GetChaoState()
{
	BASE_PLAYERINFO* pPlayerInfo=GetCharInfo();

	if(pPlayerInfo)
	{
		return pPlayerInfo->m_ChaoState;
	}

	return eCHAO_STATE_NORMAL;
}

//------------------------------------------------------------------------------
/**
*/
inline
void Player::SetChaoState(BYTE bState)
{
	BASE_PLAYERINFO* pPlayerInfo=GetCharInfo();

	if(pPlayerInfo)
	{
		pPlayerInfo->m_ChaoState=bState;
	}
}




