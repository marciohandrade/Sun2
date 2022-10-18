#pragma once
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uibase.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "VScrollDialog.h"

class uiBattleScoreMan;
class uiBattleScoreGoldRush : public uiBase
{
    enum eBATTLE_SCORESINGLE_POS
    {
        BTSCORE_BTN_EXIT=0,

        BTSCORE_BTN_RANK01,
        BTSCORE_BTN_ID01,
        BTSCORE_BTN_LV01,
        BTSCORE_BTN_MAXGOLD01,
        BTSCORE_BTN_KILL01,
        BTSCORE_BTN_GIVEDAMAGE01,        
        BTSCORE_BTN_HEAL01,        
        BTSCORE_TXT_TOTALGOLD01,

        BTSCORE_BTN_RANK02,
        BTSCORE_BTN_ID02,
        BTSCORE_BTN_LV02,
        BTSCORE_BTN_MAXGOLD02,
        BTSCORE_BTN_KILL02,
        BTSCORE_BTN_GIVEDAMAGE02,
        BTSCORE_BTN_HEAL02,
        BTSCORE_TXT_TOTALGOLD02,        

        BTSCORE_LIST01,
        BTSCORE_LIST02,

        BTSCORE_BTN_LEAVE_GOLDRUSH,

        BTSCORE_MAX, 
    };


    enum BTSCORE_LSTS
    {
        BTSCORE_LST_LIST01,
        BTSCORE_LST_LIST02,
        BTSCORE_LST_MAX,
    };

    enum
    {
        eNoneTeam,
        eRedTeam,
        eBlueTeam,
        eObserverTeam,
        eMAX_Team,
    };

public:
    enum GOLD_RUSH_REPORT_LIST
    {
        GOLD_REPORT_LIST_RANK = 0,
        GOLD_REPORT_LIST_CLASS,
        GOLD_REPORT_LIST_ID,
        GOLD_REPORT_LIST_LV,        
        GOLD_REPORT_LIST_GOLD_POINT,
        GOLD_REPORT_LIST_KILL_DIE_ASSIST,
        GOLD_REPORT_LIST_GIVEDAMAGE,
        GOLD_REPORT_LIST_GIVEHEAL,
        GOLD_REPORT_LIST_MAX,
    };

    
    
public:
    uiBattleScoreGoldRush(InterfaceManager* manager_ptr);
    virtual ~uiBattleScoreGoldRush(void);

public:
    virtual VOID Init( CDrawBase* draw_base_ptr);
    virtual VOID Release();

    virtual VOID MessageProc( SI_MESSAGE* message_ptr );
    virtual void OnShowWindow( BOOL val );
    uiBattleScoreMan* GetManager();

    //void refresh_list_control_data(nArray<BATTLESCORE_DATA>& ArrayType);
    void _RefreshListCtrlData(nArray<BATTLESCORE_DATA>& ArrayType, E_TEAM eTeamNum);
    void InitInfo();
    void RefreshInfo();
    void enable_exit_button(const bool is_enable);
    void exit_gold_rush(); // 골드러쉬 떠나기
    virtual void OnRenderSolarDialog();

private:
    CControlWZ* getControl( POSTYPE AtPos );
    POSTYPE     getCtrlIDToPos( WzID wzId );
    void        InitControls();
    void        _clearSelection();    
    //void        set_list_controls_data(CCtrlListWZ* list_ctrls_ptr, int index, int sub_index, TCHAR* text_string, bool is_online);
    void	    _SetListCtrlData(BTSCORE_LSTS eListNum, int nIndex,int nSubIndex,TCHAR* pszText, bool bIsOnline);    

private:
    HANDLE handle_texture_;
    St_TextureContainer* texture_infomation_;

    CCtrlListWZ* list_ctrls_ptr_[BTSCORE_LST_MAX];
    /*
    CCtrlButtonWZ* list_title_button01_ptr_[uiBattleScoreMan::BTSCROE_LIST_MAX];
    CCtrlButtonWZ* list_title_button02_ptr_[uiBattleScoreMan::BTSCROE_LIST_MAX];
    */

    static WzID wz_id_array_[BTSCORE_MAX];
    bool is_show_;
    bool is_init_dialog_;

    uiBattleScoreMan* battle_score_manager_ptr_;
    bool is_inited_;
    CCtrlStaticWZ* total_gold_point_[eTeamCNT];

    nArray<BATTLESCORE_DATA> score_member_array_; // 멤버 점수 리스트
};

inline
POSTYPE
uiBattleScoreGoldRush::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < BTSCORE_MAX ; ++i )
    {
        if( wz_id_array_[i] == wzId )
        {
            return i;
        }
    }
    return BTSCORE_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiBattleScoreGoldRush::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < BTSCORE_MAX );

    return GetControlWZ( wz_id_array_[AtPos] );
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
