#ifndef		_CHAR_SKILL_DIALOG_H
#define		_CHAR_SKILL_DIALOG_H

#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#else

#include "uiBase.h"
#include "PlayerAttributes.h"
#include "ItemAttrCalculator.h"

typedef map<int,int> USE_TOOLTIP_CTRL_STATUS_WINDOW;

//------------------------------------------------------------------------------
/**

    캐릭터 스테이터스
    @author
        유재영
    @since

    @remarks
    - 2005.04.30 다이얼로그 초기 작업 -지완   
    - 2005.06.22 메세지 buttonUp 처리로 변경- 지완  
	- 2005.08.23 추가 / 수정 - 재영
*/

class InterfaceManager;
class PlayerAttributes;
class uiToolTipMan;

class Char_SkillDialog	:	public uiBase
{
public:

	enum Char_Skill_Pos
	{
		CHAR_SKILL_VALUE_LEVEL = 0,	//LEVEL
		CHAR_SKILL_VALUE_NAME,		//CHARACTER NAME 
		CHAR_SKILL_MARK_P,			//이것은 뭔지모르겠지만 아마도 마크같다.
		CHAR_SKILL_VALUE_CLASS,		//CLASS
		CHAR_SKILL_VALUE_GUILD_NAME,	//GUILD NAME 
		CHAR_SKILL_STR_B,		//힘 
		CHAR_SKILL_DEX_B,		//민첩
		CHAR_SKILL_VIT_B,		//체력
		CHAR_SKILL_INT_B,		//지능
		CHAR_SKILL_SPR_B,		//지혜

		CHAR_SKILL_VALUE_STR,		//힘
		CHAR_SKILL_VALUE_PHYSIC_ATTACK_MIN,//물리공격 min
		CHAR_SKILL_VALUE_DEX,		//민첩
		CHAR_SKILL_VALUE_AVOID,		//물리공격회피율
		CHAR_SKILL_VALUE_PHYSIC_ATTACK_RATE,		//물리공격성공률
		CHAR_SKILL_VALUE_STAMINA,		//체력
		CHAR_SKILL_VALUE_PHYSIC_DEFENSE_RATE,		//물리방어력
		CHAR_SKILL_VALUE_ATTACK_SPEED,		//공격속도

		CHAR_SKILL_VALUE_MOVE_SPEED,        //이동속도
		CHAR_SKILL_VALUE_WISDOM,		//지력

        CHAR_SKILL_VALUE_SPIRIT,		//정신력
        CHAR_SKILL_VALUE_CRITICAL_RATE,	//크리티컬확률 
        CHAR_SKILL_CLOSE,
        CHAR_SKILL_STAT,		//BONUS POINT
        CHAR_SKILL_VALUE_PHYSIC_ATTACK_MAX,	//물리데미지 max

        CHAR_SKILL_VALUE_MAGIC_ATTACK_MIN,	// 추가 스킬 공격력
        CHAR_SKILL_VALUE_PHYSIC_DEFENSE_MAX,		//물리방어력 max

		CHAR_SKILL_VALUE_ATTR_WATER,	// 수
		CHAR_SKILL_VALUE_ATTR_WIND,		// 풍
		CHAR_SKILL_VALUE_ATTR_GROUND,	// 지
		CHAR_SKILL_VALUE_ATTR_FIRE,		// 화
		CHAR_SKILL_VALUE_ATTR_DARK,		// 암
        CHAR_SKILL_VALUE_ATTR_WATER2,	// 수
        CHAR_SKILL_VALUE_ATTR_WIND2,		// 풍
        CHAR_SKILL_VALUE_ATTR_GROUND2,	// 지

		CHAR_SKILL_VALUE_ATTR_FIRE2,		// 화
        CHAR_SKILL_VALUE_ATTR_DARK2,		// 암

		CHAR_SKILL_ICON_ATTR_WATER,	
		CHAR_SKILL_ICON_ATTR_WIND,
		CHAR_SKILL_ICON_ATTR_GROUND,
		CHAR_SKILL_ICON_ATTR_FIRE,
		CHAR_SKILL_ICON_ATTR_DARK,
		CHAR_SKILL_ICON_ATTR_WATER2,

		CHAR_SKILL_ICON_ATTR_WIND2,
		CHAR_SKILL_ICON_ATTR_GROUND2,
		CHAR_SKILL_ICON_ATTR_FIRE2,
		CHAR_SKILL_ICON_ATTR_DARK2,

		CHAR_SKILL_STATE_INIT_BTN,
		CHAR_SKILL_STATE_INIT_COUNT_EDIT,
		CHAR_SKILL_STATE_INIT_TEXT,

        CHAR_SKILL_ACCUMULATE_TIME_TEXT,
        CHAR_SKILL_ACCUMULATE_POINT_TEXT,
        CHAR_SKILL_ACCUMULATEPOINT_REWARD_BUTTON,


        kControl_Picture_P000,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P015,
        kControl_Picture_P016,
        kControl_Picture_P013,
        kControl_Picture_P014,
        kControl_Picture_P017,

        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S010,
        kControl_Text_S011,
        kControl_Text_S012,
        kControl_Text_S013,

        kControl_Text_S001, // 스킬 발동 효과
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S019,
        kControl_Text_S020,
        
        kControl_Text_S021, //!< 첫번째 줄
        kControl_Text_S031,
        kControl_Text_S041,
        kControl_Text_S051,
        kControl_Text_S061,
        kControl_Text_S080,
        kControl_Text_S081,
        kControl_Text_S100,
        kControl_Text_S022, //!< 2
        kControl_Text_S032,
        kControl_Text_S042,
        kControl_Text_S052,
        kControl_Text_S062,
        kControl_Text_S079,
        kControl_Text_S082,
        kControl_Text_S099,
        kControl_Text_S023, //!< 3
        kControl_Text_S033,
        kControl_Text_S043,
        kControl_Text_S053,
        kControl_Text_S063,
        kControl_Text_S078,
        kControl_Text_S083,
        kControl_Text_S098,
        kControl_Text_S024, //!< 5
        kControl_Text_S034,
        kControl_Text_S044,
        kControl_Text_S054,
        kControl_Text_S064,
        kControl_Text_S077,
        kControl_Text_S084, 
        kControl_Text_S097,
        kControl_Text_S025, //!< 5
        kControl_Text_S035,
        kControl_Text_S045,
        kControl_Text_S055,
        kControl_Text_S065,
        kControl_Text_S076,
        kControl_Text_S085,
        kControl_Text_S096,
        kControl_Text_S026, //!< 6
        kControl_Text_S036,
        kControl_Text_S046,
        kControl_Text_S056,
        kControl_Text_S066,
        kControl_Text_S075,
        kControl_Text_S086,
        kControl_Text_S095,
        kControl_Text_S027, //!< 7
        kControl_Text_S037,
        kControl_Text_S047,
        kControl_Text_S057,
        kControl_Text_S067,
        kControl_Text_S074,
        kControl_Text_S087,
        kControl_Text_S094,
        kControl_Text_S028, //!< 8
        kControl_Text_S038,
        kControl_Text_S048,
        kControl_Text_S058,
        kControl_Text_S068,
        kControl_Text_S073,
        kControl_Text_S088,
        kControl_Text_S093,
        kControl_Text_S029, //!< 9
        kControl_Text_S039,
        kControl_Text_S049,
        kControl_Text_S059,
        kControl_Text_S069,
        kControl_Text_S072,
        kControl_Text_S089,
        kControl_Text_S092,
        kControl_Text_S030, //!< 9
        kControl_Text_S040,
        kControl_Text_S050,
        kControl_Text_S060,
        kControl_Text_S070,
        kControl_Text_S071,
        kControl_Text_S090,
        kControl_Text_S091,
        kControl_VScroll_V000,

        kControl_Button_B005,
        kControl_Button_B011,
        kControl_Button_B012,
        kControl_Button_B013,
        kControl_Button_B014,
        kControl_Button_B015,
        kControl_Picture_P003,

		CHAR_SKIL_MAX,
	};

	enum eUp_Button
	{
		UP_BUTTON_BEGIN = CHAR_SKILL_STR_B,

        UP_BUTTON_END = CHAR_SKILL_SPR_B,

		UP_BUTTON_SIZE = UP_BUTTON_END - UP_BUTTON_BEGIN + 1,
	};

public:
					Char_SkillDialog(InterfaceManager* pUIMan);
	virtual			~Char_SkillDialog(void);
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
    virtual void	OnRenderSolarDialog();
    VOID            RemoveBit(eATTR_TYPE attrType);

	void			FreeStateControlShowWindow( ENUM_STATEWZ eState );
	
    void            InitAccumulateTimer()   {   accumulate_timer_.SetTimer(0);  }

protected:
	virtual VOID	OnShowWindow(BOOL val);
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	virtual VOID	OnUpdateSolarDialog();
    virtual void    UpdateMouseWheel();
	BOOL			DoStat(eATTR_TYPE attrType);	//스탯관련 변경처리 
	BOOL            CheckStat(eATTR_TYPE attrType);	//패킷 중복 날리기 검사,남아있는 스탯 검사 
    

    // val > 0 WzColor_RGBA(20,255,20,255));
    // val < 0 WzColor_RGBA(20,20,255,255));
    // val = 0 WzColor_RGBA(255,255,255,255));
    void            SetControlTextColor(CCtrlImageStaticWZ* control, int val);
    void            SetParameterCaption(Char_Skill_Pos pos, int val);
    void            SetParameterCaptionWithPercent(Char_Skill_Pos pos, int val);
    void            SetParameterCaption(Char_Skill_Pos pos, int iCalcValue, int iBaseValue);
	void            SetParameterCaption(CCtrlImageStaticWZ *pControl,int iCalcValue,int iBaseValue);
	void            SetParameterCaption2(CCtrlImageStaticWZ *pControl,int iCalcValue,int iBaseValue);
	void            SetParameterDamage(CCtrlImageStaticWZ *pControl,int iCalcValue,int iBaseValue);
    void            SetParameterCaptionWithPercent(Char_Skill_Pos pos, int calc_value, int base_value);
    void            SetParameterCaptionWithPercent(CCtrlImageStaticWZ* control_ptr, int calc_value, int base_value);
	DWORD           GetBit(eATTR_TYPE attrType);
	void			_renderGuildMark();

private:		
	static WzID		m_wzId[CHAR_SKIL_MAX];

    const static int kSocketOptionShowRowCount = 8;
    const static int kSocketOptionShowColCount = 9;

	int				AddValueForSpeed(PlayerAttributes *pCharInfo,eATTR_TYPE attrType);

    
public:
        typedef std::vector<ItemAttrCalculator::SocketOptionCalculator::CalculationResult> SocketOptionVector;

private:		
    bool    IsOverControl(Char_Skill_Pos pos);
    void    RenderTooltip();
	void	RenderTooltip_StringCode(uiToolTipMan* tooltip_manager, DWORD string_code );
    void    RenderTooltip_MagicalAttackPower(uiToolTipMan* tooltip_manager, 
                                            DWORD string_code,
                                            PlayerAttributes* player_attributes, 
                                            eATTACK_TYPE attack_type);
    void    RenderTooltip_MagicalDefense(uiToolTipMan* tooltip_manager, 
                                        DWORD string_code,
                                        PlayerAttributes* player_attributes, 
                                        eATTACK_TYPE attack_type);
    void    RenderTooltip_BaseAndCalc(uiToolTipMan* tooltip_manager, 
                                        int base_value, 
                                        int calc_value, 
                                        bool is_fource = true,
                                        DWORD string_code = 0);

    void    UpdateParameter();
    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: 해당 아이템의 소켓정보를 화면에 출력
    //! Parameter: SCItemSlot * item_slot : 아이템 정보
    //! Parameter: int col : 각각의 소켓정보를 보여줄 ui 배열 칼럼(eEQUIPCONTAINER_POS 와 같음)
    //! Parameter: socketcount_vector : 소켓정보 합계(포인트)
    //------------------------------------------------------------------------------
    void    UpdateSocketOptionValue(SCItemSlot* item_slot, int col, 
                                    IN Char_SkillDialog::SocketOptionVector& socketcount_vector);
    void    UpdateSocketOptionName(IN Char_SkillDialog::SocketOptionVector& socketcount_vector);

    void    SetSocketOptionValue(int col, int row, int val, WzColor text_color);
    void    SetSocketOptionValue(Char_Skill_Pos pos, TCHAR* text_string, WzColor text_color);
    void    SetSocketOptionValue(Char_Skill_Pos pos, int val, WzColor text_color);
    void    CalcTotalSocketOption(OUT Char_SkillDialog::SocketOptionVector& socketcount_vector);
    void    RenderTooltip_SocketOption(IN Char_SkillDialog::SocketOptionVector& socketcount_vector);

    bool    OnVScrollUp(SI_MESSAGE* message);
    bool    OnVScrollDown(SI_MESSAGE* message);
    bool    OnVScrollThumb(SI_MESSAGE* message);
    void    OnVScrollUp(CCtrlVScrollWZ* scroll_control);
    void    OnVScrollDown(CCtrlVScrollWZ* scroll_control);

    Char_SkillDialog::SocketOptionVector& socketoption_vector() { return socketoption_vector_; }
    int socket_option_show_begin_index() const { return socket_option_show_begin_index_; }
    void set_socket_option_show_begin_index(int val) { socket_option_show_begin_index_ = val; }

    SocketOptionVector socketoption_vector_;
    int socket_option_show_begin_index_;


	CControlWZ*		m_pPictureGuildMark;
    CCtrlImageStaticWZ* accumulate_time_text_;
    CCtrlImageStaticWZ* accumulate_point_text_;
    CCtrlButtonWZ* accumulate_reward_button_;
    Timer           accumulate_timer_;

public:
    static void CallbackSendPacketStatusStatApply(bool is_yes);

    PlayerAttributes& simulation_player_attributes() { return simulation_player_attributes_; }
    void RefreshSimulationPlayerAttributes();
    void reset_simulation_stat_count() { ZeroMemory(simulation_stat_count_, sizeof(simulation_stat_count_)); }

private:
    eATTR_TYPE GetAttrTypeFromButtonPos(Char_Skill_Pos pos);
    void UpdateSimulationPlayerAttributes(Char_Skill_Pos pos, int add_value);
    bool IsSimulationStatChanged();
    void RefreshStatButton();
    void SEND_STATUS_STAT_APPLY_SYN();

    SCPlayer& simulation_scplayer() { return simulation_scplayer_; }
    bool is_pressed_button() const { return is_pressed_button_; }
    void set_is_pressed_button(bool val) { is_pressed_button_ = val; }
    DWORD pressed_button_time() const { return pressed_button_time_; }
    void set_pressed_button_time(DWORD val) { pressed_button_time_ = val; }
    Char_Skill_Pos pressed_button_index() const { return pressed_button_index_; }
    void set_pressed_button_index(Char_Skill_Pos val) { pressed_button_index_ = val; }
    int simulation_stat_count(eATTR_TYPE attr_type) { return simulation_stat_count_[attr_type]; }
    void add_simulation_stat_count(eATTR_TYPE attr_type, int val) { simulation_stat_count_[attr_type] += val; }

    SCPlayer         simulation_scplayer_;
    PlayerAttributes simulation_player_attributes_;

    //! 스텟 증가시 계속 누르고 있으면 계속 증가 하게 수정.
    Char_Skill_Pos  pressed_button_index_;
    bool            is_pressed_button_;
    DWORD           pressed_button_time_;   //!< 버튼 처음 눌렀을때의 시간.

    //! 버퍼등으로 상태가 갱신되었을경우 지금까지 한것을 적용하기 위해서 백업 받아 놓는다.
    int             simulation_stat_count_[eATTR_SPR + 1];

public:
	DWORD           m_dwPacketStatus;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif //_CHAR_SKILL_DIALOG_H