#pragma once
#include "uibase.h"
#include "uibattlescoreman\uibattlescoreman.h"

class uiBattleScoreMan;
class uiBattleScoreBlood : public uiBase
{	
	enum eBATTLE_SCOREBLOOD_CONTROL
	{
	    BATTLE_SCORE_BUTTON_EXIT=0,
	    BATTLE_SCORE_LIST,
	    BATTLE_SCORE_TXT_TITLE,
	    BATTLE_SCORE_TXT_DISCRIPT,
	    BATTLE_SCORE_BUTTON_ID,
	    BATTLE_SCORE_BUTTON_CLASS,
	    BATTLE_SCORE_BUTTON_RANK,
	    BATTLE_SCORE_BUTTON_LV,
	    BATTLE_SCORE_BUTTON_MAXDAMAGE,
	    BATTLE_SCORE_BUTTON_GIVEDAMAGE,
	    BATTLE_SCORE_BUTTON_GETDAMAGE,
	    BATTLE_SCORE_BUTTON_KILL,
	    BATTLE_SCORE_BUTTON_HEAL,
	    BATTLE_SCORE_BUTTON_DIE,
        BATTLE_SCORE_CHECK_BUTTON_HOME,
        BATTLE_SCORE_CHECK_BUTTON_AWAY,
		BATTLE_SCORE_MAX, 
	};
    enum eBATTLE_BLOOD_TAB_BUTTONS
    {
        TAB_BUTTON_HOME_TEAM = 0,
        TAB_BUTTON_AWAY_TEAM,
        TAB_BUTTON_MAX
    };
public:	
	uiBattleScoreBlood(InterfaceManager* manager_ptr);
	virtual ~uiBattleScoreBlood(void);
	virtual void		Init(CDrawBase* pDrawBase);
	virtual void		Release();
	virtual void		MessageProc(SI_MESSAGE* pMessage);
	virtual void	    OnShowWindow(BOOL val);
	uiBattleScoreMan*	battle_score_manager();
	void				refresh_list_control_data(nArray<BATTLESCORE_DATA> &ArrayType);
    void                set_home_team_selected(bool value){ home_team_selected_ = value; }
    bool                home_team_selected(){ return home_team_selected_; };
    virtual void		OnRenderSolarDialog();

private:
	CControlWZ*		    get_control(POSTYPE AtPos);
	POSTYPE				get_control_id_to_position(WzID controls_id);
	void				InitControls();
	void				clear_selected();
	void				set_list_controls_data(int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline);
private:
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    HANDLE handle_texture_;
    St_TextureContainer* texture_information_;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

	CCtrlListWZ*		battle_players_list_control_;
	CCtrlButtonWZ*		list_title_button[uiBattleScoreMan::BTSCROE_LIST_MAX];
    CCtrlButtonCheckWZ* team_check_button[TAB_BUTTON_MAX];
	static WzID			controls_id_[BATTLE_SCORE_MAX];
    bool                home_team_selected_;
	uiBattleScoreMan*	battle_score_manager_;
   	BOOL				m_bInitDlg;							//dlg는 최초 OnShowWindow(0)한번호출된다 
	bool				m_bIsInited;
};
//------------------------------------------------------------------------------
/**
*/

inline POSTYPE uiBattleScoreBlood::get_control_id_to_position(WzID controls_id)
{
	for (POSTYPE i = 0; i < BATTLE_SCORE_MAX; ++i )
	{
		if (controls_id_[i] == controls_id)
		{
			return i;
		}
	}
	return BATTLE_SCORE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiBattleScoreBlood::get_control(POSTYPE AtPos)
{
	if (AtPos >= BATTLE_SCORE_MAX)
    {
		ASSERT(false);
        return NULL;
    }
	return GetControlWZ(controls_id_[AtPos]);
}
