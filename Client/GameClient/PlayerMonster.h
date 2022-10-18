#pragma once
//------------------------------------------------------------------------------
/**
    @author

    @since

    @remarks
*/

#include "Monster.h"
#include "SetItemOptionInfoParser.h"

class MonsterGauge;
class NPCAttributes;
class PlayerAttributes;
class CWzUnitDraw;
class EquipmentContainer;
class SCSlotContainer;
class SkillAttrCalculator;
class ItemAttrCalculator;

class PlayerMonster : public Monster
{
public:
	PlayerMonster( void );
	virtual ~PlayerMonster( void );

protected:
 
	//cPlayerData                 m_PlayerData;
	int							m_iCurTile;
	int                         m_iOldTile;
	DWORD                       m_dwOldTileAtt;
	int							m_iComboCount;
	int							m_iComboDelay;

	virtual void				InitContainer();
	virtual void				ReleaseContainer();

public:
	virtual void				SetMonsterInfo(BASE_NPCINFO *pInfo);
	virtual void				ApplyTerrainColor(BOOL bApply);
	virtual void				BeforeRefreshTileAtt();
	virtual void				AfterRefreshTileAtt();
	virtual BOOL				Create( DWORD dwObjectKey, DWORD dwID = 0 );
	virtual void				Destroy( void );
	//void						SetPlayerInfo(PLAYER_RENDER_INFO_EX* pPlayerInfo);
	virtual float				ProcessAnimation( DWORD dwTick ,WzEvent *pEvent = NULL,BOOL bIgnoreEvent = FALSE);
	virtual int					DoAction(PLAYER_ACTION *pAction);
	virtual WzVector			GetHeadBonePos();

	void                        SetTransSkillCode(DWORD dwSkillCode);
	DWORD                       GetTransSkillCode();
	void						ResetMoveFlag();
	void						SetMoveFlag(int MoveFlag);
	void						AddMoveFlag(int MoveFlag);
	void						RemoveMoveFlag(int MoveFlag);
	BOOL						CheckMoveFlag(int MoveFlag);
	void						SetComboDelay(int iDelay);
	int							GetComboDelay();
	DWORD						IncreaseComboCount();
	void						ResetJumpFlag();
	void						SetJumpFlag(int JumpFlag);
	void						AddJumpFlag(int JumpFlag);
	void						RemoveJumpFlag(int JumpFlag);
	BOOL						CheckJumpFlag(int JumpFlag);
	int							GetJumpFlag();
	void						SetOldJumpFlag(int iOldJumpFlag);
	int							GetOldJumpFlag();
	float                       GetLandFrame();
	void                        SetLandFrame(float fFrame);
	int							GetMoveFlag();
	void						SetOldMoveFlag(int iOldMoveFlag);
	int							GetOldMoveFlag();
	void						SetOldKeyboardDirection(WzVector *pVector);
	WzVector					GetOldKeyboardDirection();
	virtual void                SetSpecialMoveFlag(BYTE type);
	BYTE                        GetSpecialMoveFlag();
    void RefreshEquipData();
	virtual void				PlayDamageAnimation(BOOL bRight,BOOL bCritical);
	virtual WzID				GetCurrentReviveAnimation();
	virtual void				AnimateUnit();

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

	virtual WzID				GetCurrentSpecialLeftStepAnimation();
	virtual WzID				GetCurrentSpecialRightStepAnimation();
	virtual WzID				GetCurrentSpecialFrontStepAnimation();
	virtual WzID				GetCurrentSpecialBackStepAnimation();
	virtual WzID				GetCurrentJumpStartAnimation();
	virtual WzID				GetCurrentJumpKeepAnimation();
	virtual WzID				GetCurrentJumpEndAnimation();
	virtual WzID				GetCurrentJumpLandAnimation();

	virtual WzID				GetCurrentJumpEndRunAnimation();
	virtual WzID				GetCurrentJumpStunAnimation();
	virtual WzID				GetCurrentJumpStunKeepAnimation();
	virtual WzID				GetCurrentJumpStunEndAnimation();
	virtual WzID		        GetCurrentCamAnimation();
	virtual WzVector			GetTrailBonePos(int index, int start);
	virtual WzID				GetTrailEffectCode();
	
	virtual DWORD				GetCurrentAttackStyle();
	virtual void				PlayEvadeAnimation();
	virtual void				PlayBlockAnimation();
	virtual void                SetIdleAnimation();
	void                        ProcessStandAnimationDelay(DWORD dwTick);
    BOOL						SetCurrentAttackStyle(DWORD dwStyle,BOOL bCheck=TRUE);
	void                        SetCurrentNomalAttackStyle(DWORD dwWeaponType);	 

    BOOL                        CheckAttackStyle(DWORD dwStyle);				
	BOOL                        CheckStyleForWeapon(DWORD dwWeaponType);		
	void                        RefreshPunchStyle();
	void						PlayDamageSound(eWEAPONSOUNDKIND eSoundKind);
	virtual void				CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId);
	virtual BOOL				Process( DWORD dwTick );
	virtual BYTE				GetPCBangMode()	{ return FALSE;}

	virtual void				OpenCrossbow();
	virtual void                CloseCrossbow();
	void                        ShotCrossbow();
	void                        LaunchEtherWeapon();
	int							GetValkyrieShotDelay();
	BOOL						CanValkyrieAction();
	virtual void				ValkyrieShot(DWORD dwAttackSerial);
	WzID						GetValkyrieShotAnimation(); 
	void						ProcessValkyrieShot(DWORD dwTick);
	virtual DWORD				GetValkyrieShotSerial();
	WzID						GetValkyrieReloadAnimation(); 
	int							GetBulletCount();
	void                        SetBulletCount(int iCount);
	int                         DecreaseBulletCount();
	void                        ValkyrieReload( BOOL bAnimation = TRUE );
	
	virtual  void				OnDamage(const DAMAGE_PRAM &rDamage);

	virtual  void				OnAttack(DWORD dwSkillCode);
	virtual void				OnFootStep(BOOL b2D = FALSE);
	virtual void				OnShotStart();
	virtual void				OnShotFire(int iShotCount);
	virtual void				OnShotEnd();
	virtual void				OnParalyzed();
	virtual int					GetShotCount();


	BOOL						IsFlyFinishWeapon();
public:
	virtual BOOL				Render( void );
	virtual float				GetMeshHeight();
	virtual void                RenderInfo();

	virtual WzVector			GetWeaponBonePos(int index);//지완 virtual로 변경
	virtual WzVector            GetArrowBonePos(int index);
	virtual BOOL				SetPosition( const WzVector& wvPos, int iTileIndex = -1);
	virtual BOOL				SetPosition( const HONGPOS& Pos );
	
	virtual BASE_ITEMINFO *		GetWeaponInfo();
	int                         GetShotBlendBone();
	//BASE_PLAYERINFO*			GetCharInfo(){ return m_PlayerData.GetCharacterInfo();}
	void						SetPartVariation(int iPart, int iVariation);
	WzColor						GetEstablisherColor();
	int							GetComboCount();
	void						SetComboCount(int iCount);
	void						ProcessComboDelay(DWORD dwTick);

	//eCHAR_TYPE			GetClass();
	void                SetClass(eCHAR_TYPE eType);

	virtual void		SetExp(EXPTYPE dwExp);
	virtual EXPTYPE		GetExp();
	virtual EXPTYPE		GetNextExp();

	virtual void		SetLevel(LEVELTYPE LV);
	virtual void        SetAttackSpeedRatio(int iSpeed);
	virtual void        SetMoveSpeedRatio(int iSpeed);
	virtual LEVELTYPE	GetLevel();
	virtual float       GetMoveSpeedModifier();
	virtual float       GetAttackSpeedModifier();
	virtual float       GetAttackRange( void );
	virtual BOOL        IsRangedNormalAttack();
	virtual void        UpdateActiveSkillEffectAttr(SkillAttrCalculator &calc);
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
	void				ApplyStyleBuff( DWORD dwStyleCode ,SkillAttrCalculator &SkillCalc);
	BOOL			    CanBeAttackTarget();
	BOOL                IsCasting();
	void                SetCast(BOOL bCast);

	EXPTYPE				GetAccumExp(LEVELTYPE LV);

	void                SetStandUp(BOOL bFlag);
	BOOL                IsStandUp();
public:
	BOOL				IsShowRenderinfo();
	void				SetInvisible( BOOL bInvisible )		{ m_bInvisible = bInvisible; }
	BOOL				IsInvisible( void )					{ return m_bInvisible; }
	void				SetInvincible( BOOL bInvincible ,int iTime=0) { m_bInvincible = bInvincible; m_iInvicibleTime=iTime;}
	BOOL				IsInvincible( void )				{ return m_bInvincible; }
	void                SetKeyboardMoveTime(DWORD dwTime);
	void                AddKeyboardMoveTime(DWORD dwDelta);
	DWORD               GetKeyboardMoveTime();

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    virtual void SetCostumeItemCode( SLOTCODE code );
    virtual SLOTCODE GetCostumeItemCode();
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	virtual void SetDecorateCostumeItemCode( SLOTCODE code );
	virtual SLOTCODE GetDecorateCostumeItemCode();
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

private:
	virtual void SetPolymorph( BOOL Active );



public:
	int						m_iJumpFlag;
	int						m_iOldJumpFlag;
	WzVector				vFoward;    
	WzVector				vVisibleDest;
	WzVector				vSrc;
	//WzVector				vVisibleSrc;
	float					fSrcZ;
	float					fDestZ;
	float                   m_fLandFrame;
	DWORD					dwVerticalMoveTime;
	DWORD					dwHorizonMoveTime;
	BOOL                    bCreateJumpEndEffect;
	BOOL                    bJumpStun;
	DWORD					dwCurTime;
	BOOL					bLinerMove;
	float					fRate;
	BOOL					bJumpEndEff;
	BOOL					bJumpStart;
	float					fJumpEndFrame;
	int                     iJumpStunTime;
	HANDLE                  hStunEff;
protected:
	int						m_iMoveFlag;	
	BYTE					m_bSpecialMoveFlag;	
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
	DWORD					m_dwAttackStyle;
	DWORD					m_dwOldAttackStyle;
	DWORD                   m_dwKeyboardMoveTime;

	DWORD					m_dwDustTime; 
	BOOL                    m_bHideHelmet; 
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    BOOL                    m_bHideWing; 
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	BOOL                    m_bInvincible;
	int                     m_iInvicibleTime;
	BOOL                    m_bCasting;
	DWORD                   m_dwShieldTime;
	DWORD                   m_dwTransSkillCode;
	BOOL					m_bInvisible;		
	BOOL                    m_bStandUp;
	BOOL					m_bRenderinfoState;
	WzColor					m_RenderinfoColor;

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    SLOTCODE            playermonster_costume_item_;
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	SLOTCODE            playermonster_decorate_costume_item_;
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME
};


inline 	
void                        
PlayerMonster::SetSpecialMoveFlag(BYTE type)
{
	m_bSpecialMoveFlag=type;
}

inline 	
BYTE
PlayerMonster::GetSpecialMoveFlag()
{
	return m_bSpecialMoveFlag;
}


//------------------------------------------------------------------------------
/**
*/
inline
BOOL PlayerMonster::IsCasting()
{
	return m_bCasting;
}

//------------------------------------------------------------------------------
/**
*/
inline
void PlayerMonster::SetCast(BOOL bCast)
{
	m_bCasting=bCast;
}

//------------------------------------------------------------------------------
/**
*/
inline 
void PlayerMonster::SetStandUp(BOOL bFlag)
{
	m_bStandUp=bFlag;
}


//------------------------------------------------------------------------------
/**
*/
inline 
BOOL PlayerMonster::IsStandUp()
{
	return m_bStandUp;
}





