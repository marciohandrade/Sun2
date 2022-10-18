//------------------------------------------------------------------------------
/**

	인풋 입력에 따른 캐릭터 상태 처리 
	@author

	@since

	@remarks
		-2005.06.07 소스 정리(지완)
- 

*/

#pragma once

class Character;
class TerrainObject;
class Item;

#include "GameFunc.h"

#define PICKING_IGNORE_ITEM               0x00000001  
#define PICKING_IGNORE_FRIEND             0x00000002
#define PICKING_IGNORE_CORPSE             0x00000004

#define	FOLLOW_DELAY_TIME				  500

enum UI_VOICE_KIND
{
	UI_VOICE_NONE = 0,
	UI_VOICE_LIMIT_USE,
	UI_VOICE_TARGET_TOO_FAR,
	UI_VOICE_NOT_ENOUGH_MONEY,
	UI_VOICE_NOT_ENOUGH_MANA,
	UI_VOICE_NOT_ENOUGH_POINT,
	UI_VOICE_NO_REPAIRABLE,
	UI_VOICE_NO_TARGET,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    UI_VOICE_NOT_ENOUGH_FP,
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

const int UI_VOICE_DELAY = 3000;

enum SPECIAL_MOVE
{
	SPECIAL_MOVE_FORWORD =0,
	SPECIAL_MOVE_BACKWORD,
	SPECIAL_MOVE_LEFT,
	SPECIAL_MOVE_RIGHT,
	SPECIAL_MOVE_MAX,
};

enum SEARCH_TARGET_TYPE
{
    SEARCH_TARGET_NEAREST = 0,
    SEARCH_TARGET_NEXT,
    SEARCH_TARGET_PREVIOUS,

    SEARCH_TARGET_MAX,
};

class PickingData
{
private:
    DWORD m_dwPickedTarget;
    WzVector m_vPickedPos;	
    int m_iPickedTile;

    BOOL m_bCanMoveToPickedPos;
    BOOL m_bIgnorePlayer;

public:
    PickingData();

    void Init();
    void ProcessPicking();

public:
    BOOL IsCanMoveToPickedPosition() { return m_bCanMoveToPickedPos; }

    Object*     GetPickedObject();
    Character*  GetPickedCharacter();
    Item*       GetPickedItem();
    MapObject*  GetPickedMapObject();
    DWORD       GetPickedObjectKey();

    int GetPickedTileIndex() { return m_iPickedTile; }
    const WzVector& GetPickedPosition() const { return m_vPickedPos; }
};

class KeyboardInputData
{
    // Foward, Backward, LeftStep, RightStep
    BOOL m_KeyboardPressed[4];
    BOOL m_bLeftTurnPressed;
    BOOL m_bRightTurnPressed;


    //스페셜 이동
    BOOL m_SpecialKeyboardPressed[4];
    DWORD m_LastInputTime[4];
    BOOL m_bSpecialMoveFirstDown[4];



public:
    KeyboardInputData()
    {
        Reset();
    }

    void Reset()
    {
        m_KeyboardPressed[0] = FALSE;
        m_KeyboardPressed[1] = FALSE;
        m_KeyboardPressed[2] = FALSE;
        m_KeyboardPressed[3] = FALSE;
        m_bLeftTurnPressed = FALSE;
        m_bRightTurnPressed = FALSE;


        //스페셜 이동
        m_SpecialKeyboardPressed[0] = FALSE;
        m_SpecialKeyboardPressed[1] = FALSE;
        m_SpecialKeyboardPressed[2] = FALSE;
        m_SpecialKeyboardPressed[3] = FALSE;

        m_LastInputTime[0] = 0;
        m_LastInputTime[1] = 0;
        m_LastInputTime[2] = 0;
        m_LastInputTime[3] = 0;
        m_bSpecialMoveFirstDown[0] = FALSE;
        m_bSpecialMoveFirstDown[1] = FALSE;
        m_bSpecialMoveFirstDown[2] = FALSE;
        m_bSpecialMoveFirstDown[3] = FALSE;
    }




    void UpdateSpecialMove3( DWORD CurrentTickTime )
    {
        // 누르고 있으면 자동적으로 스탭을 밟는 방식
        m_SpecialKeyboardPressed[0] = m_KeyboardPressed[0];
        m_SpecialKeyboardPressed[1] = m_KeyboardPressed[1];
        m_SpecialKeyboardPressed[2] = m_KeyboardPressed[2];
        m_SpecialKeyboardPressed[3] = m_KeyboardPressed[3];
    }

    void UpdateSpecialMove4( DWORD CurrentTickTime )
    {
        // 키를 누른후 어느정도 시간동안을 스탭을 밟는 방식
        for( size_t i=0;i!=4;++i)
        {
            m_SpecialKeyboardPressed[i] = FALSE;

            if( m_bSpecialMoveFirstDown[i] == FALSE && m_KeyboardPressed[i] == TRUE )
            {
                m_LastInputTime[i] = CurrentTickTime;
                m_bSpecialMoveFirstDown[i] = TRUE;
            }

            if( m_bSpecialMoveFirstDown[i] == TRUE && m_KeyboardPressed[i] == FALSE )
            {
                m_bSpecialMoveFirstDown[i] = FALSE;
            }

            static DWORD s_time_delay = 300;

            if( m_LastInputTime[i] + s_time_delay >= CurrentTickTime )
            {
                m_SpecialKeyboardPressed[i] = TRUE;
            }

        }
    }

    void UpdateSpecialMove2( DWORD CurrentTickTime )
    {
        // 키를 뗀 후 다시 눌러주어야 하는 방식
        for( size_t i=0;i!=4;++i)
        {
            m_SpecialKeyboardPressed[i] = FALSE;

            if( m_bSpecialMoveFirstDown[i] == FALSE && m_KeyboardPressed[i] == TRUE )
            {
                m_SpecialKeyboardPressed[i] = TRUE;
                m_bSpecialMoveFirstDown[i] = TRUE;
            }

            if( m_bSpecialMoveFirstDown[i] == TRUE && m_KeyboardPressed[i] == FALSE )
            {
                m_bSpecialMoveFirstDown[i] = FALSE;
            }
        }
    }


    void UpdateSpecialMove5( DWORD CurrentTickTime )
    {
        for( size_t i=0;i!=4;++i)
        {
            m_SpecialKeyboardPressed[i] = FALSE;

            if( m_bSpecialMoveFirstDown[i] == FALSE && m_KeyboardPressed[i] == TRUE )
            {
                m_SpecialKeyboardPressed[i] = TRUE;
                m_bSpecialMoveFirstDown[i] = TRUE;
            }

        }
    }

    void UpdateSpecialMove52()
    {
        for( size_t i=0;i!=4;++i)
        {
            if( m_bSpecialMoveFirstDown[i] == TRUE && m_KeyboardPressed[i] == FALSE )
            {
                m_bSpecialMoveFirstDown[i] = FALSE;
            }
        }

    }

    void UpdateSpecialMove( DWORD CurrentTickTime )
    {
        m_SpecialKeyboardPressed[0] = FALSE;
        m_SpecialKeyboardPressed[1] = FALSE;
        m_SpecialKeyboardPressed[2] = FALSE;
        m_SpecialKeyboardPressed[3] = FALSE;

        static DWORD s_double_down_time = 500;

        int pressed_key_count = 0;
        if( m_KeyboardPressed[0] )
            ++pressed_key_count;
        if( m_KeyboardPressed[1] )
            ++pressed_key_count;
        if( m_KeyboardPressed[2] )
            ++pressed_key_count;
        if( m_KeyboardPressed[3])
            ++pressed_key_count;

        
        if( pressed_key_count == 1 )
        {
            for( size_t i=0;i!=4;++i)
            {
                if( m_KeyboardPressed[i] == TRUE && m_bSpecialMoveFirstDown[i] == FALSE )
                {
                    m_bSpecialMoveFirstDown[i] = TRUE;
                    if( m_LastInputTime[i] + s_double_down_time >= CurrentTickTime )
                    {
                        m_SpecialKeyboardPressed[i] = TRUE;
                        m_LastInputTime[i] = 0;
                    }
                    else
                    {
                        m_LastInputTime[i] = CurrentTickTime;
                    }
                }
            }
        }

        for( size_t i=0;i!=4;++i)
        {
            if( m_KeyboardPressed[i] == FALSE )
                m_bSpecialMoveFirstDown[i] = FALSE;

            if( m_SpecialKeyboardPressed[i] == TRUE )
                m_LastInputTime[i] = 0;
        }
    }

public:

    void SetFowardPressed( BOOL Pressed ) { m_KeyboardPressed[0] = Pressed; }
    void SetBackwardPressed( BOOL Pressed ) { m_KeyboardPressed[1] = Pressed; }
    void SetLeftStepPressed( BOOL Pressed ) { m_KeyboardPressed[2] = Pressed; }
    void SetRightStepPressed( BOOL Pressed ) { m_KeyboardPressed[3] = Pressed; }
    void SetLeftTurnPressed( BOOL Pressed ) { m_bLeftTurnPressed = Pressed; }
    void SetRightTurnPressed( BOOL Pressed ) { m_bRightTurnPressed = Pressed; }

    BOOL GetFowardPressed() { return m_KeyboardPressed[0]; }
    BOOL GetBackwardPressed() { return m_KeyboardPressed[1]; }
    BOOL GetLeftStepPressed() { return m_KeyboardPressed[2]; }
    BOOL GetRightStepPressed() { return m_KeyboardPressed[3]; }
    BOOL GetLeftTurnPressed() { return m_bLeftTurnPressed; }
    BOOL GetRightTurnPressed() { return m_bRightTurnPressed; }

    BOOL IsKeyboardMovePressed() { return GetFowardPressed() || GetBackwardPressed() || GetLeftStepPressed() || GetRightStepPressed();  }
    BOOL IsKeyboardPressed() { return GetFowardPressed() || GetBackwardPressed() || GetLeftStepPressed() || GetRightStepPressed() || GetLeftTurnPressed() || GetRightTurnPressed(); }

    BOOL GetSpecialMoveFoward() { return m_SpecialKeyboardPressed[0]; }
    BOOL GetSpecialMoveBackword() { return m_SpecialKeyboardPressed[1]; }
    BOOL GetSpecialMoveLeftStep() { return m_SpecialKeyboardPressed[2]; }
    BOOL GetSpecialMoveRightStep() { return m_SpecialKeyboardPressed[3]; }
};


class HeroActionInput
    : public PickingData
{
public:
	HeroActionInput(void);
	~HeroActionInput(void);

	void    Init();
	void	Process(DWORD dwTick);
	void    SetTransFormAction(PLAYER_ACTION *pAction);

	void	SetComboDelay(int iDelay);
	int     GetComboDelay();
	void	SetCurrentTarget(DWORD dwID,BOOL bCheck=TRUE);
	DWORD	GetCurrentTarget();
	BOOL	ProcessQueueAction();
	

	BOOL    ProcessQueueTransForm();


	BOOL    CanDoNextAttack();
	void    SetCanDoNextAttack(BOOL bFlag);
	float	GetActionLength(PLAYER_ACTION action);
	PLAYER_ACTION GetQueueAction();
    PLAYER_ACTION& GetQueueActionRef() { return m_QueueAction; }

	BOOL    CanBeSkillTarget(DWORD dwSkillID,Character *pTarget,BOOL bForceAttack,int &iResult);
	BOOL	CheckSoulControl(Character* pTarget,DWORD dwSkillID,int &iResult);

    BOOL	IsAttackSkillArea() { return m_ShowTargetAreaPointer; }
	
	void    SetComboReset();//히트수 초기화 

	float   GetAttackActionLength();


	void	SetEnableInput(BOOL bFlag);
	BOOL	IsEnableInput();

	void    ValkyrieShot();
	void    ValkyrieReload();

	void    SetAutoAttack(BOOL bFlag,BOOL bCheck=TRUE);
	void    SetAutoRun(BOOL bFlag,BOOL bCheck=TRUE);
    BOOL    GetAutoRun();
	void    SetAreaSkill(BOOL bShow);

	BOOL	IsAutoAttackMode( void );
	DWORD   IncreaseComboCount();
	DWORD   GetComboCount();
	void	ClearPickingTarget();
    BOOL	CanSitState();

	void    PlayLimitUseSound();
	void    PlayNoTargetSound();
	void    PlayTargetTooFarSound();
	void    PlayNotEnoughMoneySound();
	void    PlayNotEnoughManaSound();
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    void    PlayNotEnoughFPSound();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	void    PlayNotEnoughPointSound();
	void    PlayNoRepairableSound();

	BOOL    CannotAttack(Object* pTargetObject);
    void    SetCannotAttack(BOOL bFlag) { m_bCannotAttack = bFlag; }
	bool	CanAttackInDominationWar();
	int     GetSkillQueueSize(){ return g_SkillQueue.Size();}
	BOOL    InsertSkill(SLOTCODE dwSkillid);
	BOOL    OnSkill(SLOTCODE dwSkillid);
	BOOL    UseSummonSkill(SLOTCODE dwSkillid);

	BOOL	IsIdleState( void );
	BOOL    CanGetItem(DWORD dwOwnerKey);
    BOOL    CannotMoveState() const;
	void	SetAckBeginVendor(BOOL	val) { m_bAckBeginVendor = val;}

    BOOL	OnLClickedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind, bool nove_nearest);

    BOOL    MoveToPosition( const WzVector& DestPosition, bool move_nearest );
	BOOL	MoveToAction(WzVector vDest, bool move_nearest );

    float   GetSkillRange( BASE_SKILLINFO* skill_info );

	DWORD	GetClickedNpcCode() { return m_dwClickedNpcCode;}

	void	PickUpAllItem();


#ifdef MAP_EDIT_MODE
	BOOL    OnRClickedMapObject(MapObject *pObj);
	BOOL    OnRClickedTile();
	TerrainObject *GetTargetTerrainObject();
	void    SetEditMapObjKey(DWORD dwKey);
	DWORD   GetEditMapObjKey();

#endif 
	BOOL	OnSpacePushItem( BOOL bAllPickup = FALSE);

#ifdef _NA_20100307_BETAKEY_SYSTEM
    BOOL IsUseableNPCBetakey(BASE_NPCINFO* pNPCInfo);
#endif//_NA_20100307_BETAKEY_SYSTEM

private:
	BOOL    UseSkill(DWORD skillid,BOOL bForceAttack);
	BOOL    UseAreaSkill();

	BOOL    OnLDragedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind);
	BOOL	OnShiftLClickedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind);

public:
    void    OnSearchTarget(SEARCH_TARGET_TYPE search_type);
	BOOL	UpdateTargetList( float fDistance, BOOL bCheckCameraAngle = TRUE ,BOOL bChekChaoState=FALSE);
	BOOL	OnTabPushTarget( void );
	BOOL	OnNextTarget( void );
	BOOL	OnPrevTarget( void );
    BOOL	OnLClickedPosition(WzVector* pClickedPosition);
    BOOL    OnLClickedMapObject(MapObject *pObj, bool bKeyUp = true);


    // 대상 맵오브젝트로 이동후 타입별 액션을 취한다
    BOOL    MoveToMapObjectWithAction(MapObject* pObj, bool bKeyUp = true );  

public:
    void    ProcessTargeting();
    void	PressQuickSlotKey();
    void    ToggleChangeSlotSize();

private:
	BOOL	OnLClickedItem(Item *pItem);
    BOOL	OnValkyrieLClickedCharacter(Character* pObj);
	BOOL	OnValkyrieLDragedCharacter(Character* pObj);
	BOOL	OnLClickedPositionWithItem(WzVector* pClickedPosition);
	BOOL	OnRClickedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind);
	BOOL	OnRClickedPosition(WzVector* pClickedPosition);	
	BOOL	FindNearesetPathTile(WzVector *pClickedPos,WzVector *pRet);

	VOID	InitTerm();
    VOID    ProcessVoiceDelay(DWORD dwTick);

    BOOL	ProcessKeyboardMove();
	BOOL	ProcessValkyrieKeyboardMove();
	BOOL	ProcessElementalistKeyboardMove();
    BOOL    ProcessDefaultKeyboardMove();

	BOOL    ProcessKeyboardJump();
    BOOL    ProcessSpreadWing();

	BOOL	ProcessMouse();
	BOOL	ProcessValkyrieMouse();
	BOOL	ProcessMouseWhenKeyboardMove();	
	VOID	ProcessAutoAttack();
	BOOL	ProcessQueuePlayerMeet();
	BOOL	ProcessQueueNPCMeet();
    BOOL    ProcessQueueVendorMeet();
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	BOOL    ProcessQueueStatuePlayerMeet();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	BOOL    ProcessQueueUseObject();
	BOOL	ProcessQueueCollectObject();
	BOOL	ProcessQueueAttack();
	BOOL	ProcessQueueSkill();
	BOOL	ProcessQueuePlayerFollow();
	BOOL	ProcessSpecialMove();
	BOOL    ProcessQueueWayPointMeet();
    bool    ProcessQueueAirShipMeet();
	BOOL	ProcessQueueDominationObject();

	BOOL	ProcessQueueGetItem();
	BOOL	ProcessQueueShot();
	void    ProcessOptionKey();
	void	ProcessBaseOptionKey();
	void    ProcessComboDelay(DWORD dwTick);

	void	ProcessCursor();
	void	ProcessObserverModeKey( DWORD dwTick );
    void    ProcessAssistTarget();
	BOOL	ProcessTargetClick( Character *pTarget, BOOL bAutoAttack = TRUE );
    void    ProcessKeyboardAttack();	
	BOOL    AttackNearestTarget();
	DWORD   FindNearestTarget();
	void	SetHeroTurnState();
	BOOL	IsActionState();
	BOOL	SetNearestTarget();

public:
    BOOL        IsPicking() const { return m_bPicking; }
    WzVector	GetPickingPosition() { return m_PickingPosition; }

    BOOL		CanMoveToPickedPos() { return PickingData::IsCanMoveToPickedPosition(); }
    BOOL		GetKeyboardQueueAttack() { return m_bKeyboardQueueAttack; }
	void		SetMoveDelay(DWORD dwDelay);
	void		ResetSpecialMove();	

	BOOL		SetFollowState( BOOL bFlag, DWORD dwPlayerKey = 0 );
	BOOL		GetFollowState( void );
	DWORD		GetFollowPlayer( void );
	void		ProcessFollow( void );

	BOOL		ProcessUserAction( SLOTCODE dwSkillid );
	BOOL		ProcessUserEmoticon( SLOTCODE dwSkillid );
	BOOL        IsSpecialMove(); 

    void        ProcessTeleportMoveSkill(WzVector teleport_direction);
	BOOL        UseSpecialMoveSkill( DWORD SkillCode, const WzVector& TargetPosition );

	BOOL		IsShootingMode(); // 발키리 샷 모드인가.
	BOOL        lsForceAttack();

	BOOL        ChoaCanUse();
	BOOL        IsShowPartyMemberOnly();
    BOOL        SetShowPartyMemberOnly(BOOL only_show_party);

	void		SetFixTarget(BOOL bFixTarget);
	BOOL		GetFixTarget(void);

	void		SetAutoTarget(BOOL	bTarget) { m_bAutoTarget = bTarget;}
	BOOL		GetAutoTarget() { return m_bAutoTarget;}



	void		SetForwardTargeting(BOOL val);
	BOOL		GetForwardTargeting();

	void		ClickPartyMember(DWORD dwObjKey);

	void		SetMousePointHide(BOOL	val) { m_bIsMousePointHide =val;}
	BOOL		GetMousePointHide()	{return m_bIsMousePointHide;}
	POINT		GetMousePoint() { return m_MousePoint;}


#ifdef PERF_AUTO_MOVE_TEST
	void        SetAutoMove(BOOL bMove){m_bAutoMove=bMove;}
	BOOL        GetAutoMove(){ return m_bAutoMove;}
	void        SwitchMove(){ m_bSwitch=!m_bSwitch;}
#endif 

	void		PickingForShowMonsterName( void );

	void		SetCameraBaseMove(BOOL val);
	BOOL		GetCameraBaseMove();
	BOOL		IsShowPetInfo( void )	  { return m_bShowPetInfo;}
    BOOL		ToggleShowPetInfo( void );

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    void ForceAreaSkillPosition(DWORD skill_code, WzVector target);
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

private:
	DWORD       GetSkillCode(DWORD dwClassId,BOOL & bActive,float &dwDistance);

    WzVector    GetCanTeleportPosition(WzVector vDest, int iSplineNum = 10);

	void        ShowCanNotMoveReason(DWORD dwReason);
	BOOL		IsAckBeginVendor() { return m_bAckBeginVendor;}

private:
	BOOL				m_bShowPetInfo;
	WzVector            m_PickingPosition; //마우스로 클릭한 피킹 지점 
	BOOL                m_bPicking;

    SLOTCODE			m_TargetAreaSkillID;
    BOOL                m_ShowTargetAreaPointer;	// 텔레포트 상태에선 마우스 피킹시 사용하게 하기 위해
    BOOL                m_IsClickedTargetArea;      // 지역지정 스킬을 사용할때 지역을 클릭하였는가
    WzVector            m_TargetAreaPosition;       // 지역지정 스킬을 사용하기 위한 위치


	DWORD				m_dwSelectedTarget;
	BOOL				m_bAutoAttack;
	BOOL				m_bKeyBoardMoveCheck;
	int					m_iComboDelay;
	int					m_iComboCount;
	PLAYER_ACTION		m_QueueAction;
	DWORD		        m_dwStartPickPosTime;	

	nArray<PLAYER_ACTION> m_TransFormActionQueue;

	BOOL                m_bCanDoNextAttack;


	BOOL                m_bAutoRun;
	BOOL				m_bEnableInput;	// 입력 가능 / 불가능 
	BOOL				m_bIsHeroKeyboardMove;
	DWORD               m_OldReason;
	DWORD               m_dwOldUseSkillId;
	BOOL				m_bAckBeginVendor;

    bool                spread_wing_key_pressed_;

#ifdef PERF_AUTO_MOVE_TEST
	BOOL                m_bAutoMove;
	WzVector            m_vSrcPos;
	WzVector            m_vDestPos;
	BOOL                m_bSwitch;
#endif

#ifdef MAP_EDIT_MODE
	DWORD               m_dwEditMapObjKey;
#endif 

	BOOL				m_bFixTarget;
	BOOL				m_bAutoTarget;
	int                 m_iMoveToActionTime;
	BOOL                m_bKeyboardQueueAttack;
	int                 m_iKeyboardQueueAttackDelay;
    BOOL                m_bCanInputMode;

	POINT				m_MousePoint;
	BOOL				m_bIsMousePointHide;
	int                 m_iUIVoiceDelay;
	DWORD               m_dwUIVoice;

	//특수 이동 관련 다닥
	DWORD               m_dwSpecialMovePreTime[SPECIAL_MOVE_MAX];
	BOOL                m_bSpecialMoveFirstDown[SPECIAL_MOVE_MAX];
	BOOL                m_bSpecialMove;
	int                 m_iMoveDelay;

	// 공격불가.
	BOOL                m_bCannotAttack;

	BOOL				m_bFollowPlayer;
	DWORD				m_dwFollowPlayerKey;
	DWORD				m_dwFollowPlayerTime;
	RELATIONKIND::TYPE	m_TargetType;
	BOOL                m_bShowPartyMemberOnly;
	BOOL				m_bForwardTargeting;
	DWORD				m_dwClickedNpcCode;
	BOOL				m_bCameraBaseMove;
	// 키갱신 

    //! process 시 계속 갱신
    //! 날고 있는 상태에서 점프키를 누르면 다른 모션(애니메이션)을 하기 위함.
    bool jump_key_pressed_;

    void CancelSkillQueue();

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    static   BOOL  bIsRotateL;
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX

private:
    KeyboardInputData m_KeyboardState;

public:
    void set_forward_key_pressed(bool val) { m_KeyboardState.SetFowardPressed(val); }
    void set_backward_key_pressed(bool val) { m_KeyboardState.SetBackwardPressed(val); }
    void set_left_turn_key_pressed(bool val) { m_KeyboardState.SetLeftTurnPressed(val); }
    void set_right_turn_key_pressed(bool val) { m_KeyboardState.SetRightTurnPressed(val); }
    void set_left_step_key_pressed(bool val) { m_KeyboardState.SetLeftStepPressed(val); } 
    void set_right_step_key_pressed(bool val) { m_KeyboardState.SetRightStepPressed(val); }

    BOOL IsRightKeyPressed() { return m_KeyboardState.GetRightTurnPressed(); }
    BOOL IsLeftKeyPressed() { return m_KeyboardState.GetLeftTurnPressed(); }

    bool is_jump_key_pressed() const { return jump_key_pressed_; }
    void set_jump_key_pressed(bool val) { jump_key_pressed_ = val; }

    bool is_spread_wing_key_pressed() const { return spread_wing_key_pressed_; }
    void set_spread_wing_key_pressed(bool val) { spread_wing_key_pressed_ = val; }

    void ClearTransFromQueue() { m_TransFormActionQueue.Clear(); }
};

//------------------------------------------------------------------------------
/**
	키 큐 메니져와 연동 됨으로 항상 셋함수 사용 하세요!
*/
inline void HeroActionInput::SetAutoAttack(BOOL bFlag,BOOL /*bCheck*/)
{
//#ifdef _MILKY_DEBUG
//	bFlag = FALSE;
//#endif //_MILKY_DEBUG
	m_bAutoAttack=bFlag;
	if( m_bAutoAttack )
	{
		//	오토 어택이 켜지면 따라다니기를 꺼준다
		SetFollowState( FALSE );
	}
}
//------------------------------------------------------------------------------
/**
	키 큐 메니져와 연동 됨으로 항상 셋함수 사용 하세요!
*/
inline void HeroActionInput::SetAutoRun(BOOL bFlag,BOOL /*bCheck*/)
{
	m_bAutoRun=bFlag;
}

inline BOOL HeroActionInput::GetAutoRun()
{
    return m_bAutoRun;
}

inline void HeroActionInput::SetMoveDelay(DWORD dwDelay)
{
	m_iMoveDelay=dwDelay;
}

inline void HeroActionInput::ResetSpecialMove()
{
	memset(m_dwSpecialMovePreTime,0,sizeof(DWORD)*SPECIAL_MOVE_MAX);
	memset(m_bSpecialMoveFirstDown,0,sizeof(BOOL)*SPECIAL_MOVE_MAX);
	m_bSpecialMove=FALSE;
}

inline 	BOOL HeroActionInput::GetFollowState( void )
{
	return m_bFollowPlayer;
}

inline 	DWORD HeroActionInput::GetFollowPlayer( void )
{
	return m_dwFollowPlayerKey;
}

inline BOOL HeroActionInput::IsSpecialMove()
{
	return m_bSpecialMove;
}

inline BOOL HeroActionInput::IsShowPartyMemberOnly()
{
	return m_bShowPartyMemberOnly;
}

inline BOOL HeroActionInput::SetShowPartyMemberOnly(BOOL only_show_party)
{
    m_bShowPartyMemberOnly = only_show_party;
}

extern HeroActionInput g_HeroInput;
