//------------------------------------------------------------------------------
/**

    ��ųUI ������

    @author		����

    @since		10.09.03

    @remarks	
- 
*/

#pragma once
struct IM_SKILL_MANAGER
{
    static const DWORD SKILL_TREE_DLG;
    static const DWORD SKILL_PERK_DLG;
    static const DWORD SKILL_PERK_SLOT_DLG;
    static const DWORD SKILL_ACTION_DLG;
};

#include "uibaseman.h"

struct SkillDialogPos
{
    bool is_set;
    float x,y;
};

class uiSkillTree;
class uiSkillAction;
class SCSkillSlot;
class SkillData;
class uiPerkDialog;

class uiSkillMan : public uiBaseMan
{
public:
    enum 
    {
        kSkillTree = 1,
        kSkillPerk = 2,
        kSkillAction = 3,
        kSkillMax = 4,
    };

    uiSkillMan(InterfaceManager *pUIMan);

    // �ʱ�ȭ
    virtual void OnInitialize();
    virtual void NetworkProc(MSG_BASE* msg);
    
    // ��ų�����ε�(skillinventory.txt)
    void PreloadSkillInvtoryStoreDatas();

    // ��������
    void ResetOnceInitSkillInfos();
	void RefreshValidSkillFromQuick();

    // ������ ��ų������ ����
    void SetSkillInfos(const SKILL_TOTAL_INFO& skillInfos);

    // �ܺ�����
    int GetSkillState(SLOTCODE skill_code);
    int getRenderState(SLOTCODE skill_code);
    SCSkillSlot* GetSkillSlotByCode(CODETYPE code);

    // ��ųâ ��ġ�� Űť
    void OpenTab(int type);
    void OpenDialog(int type, BOOL is_open);
    
    DWORD GetCurrentDialogKey()	{ return current_dialogkey_; }
    void SetCurrentDialogKey(DWORD key)	{ current_dialogkey_ = key; }

    uiSkillTree* skill_tree();

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    void SEND_Perk_List_SYN();
    void RECV_Perk_List_ACK(GameOption& game_option);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

private:
    
    bool init_data() const { return init_data_; }
    void set_init_data(bool val) { init_data_ = val; }

    uiBase* get_skill_dlg(int type){ return skill_dlg_[type];}
    void set_skill_dlg(int type, uiBase* dlg){ skill_dlg_[type] = dlg;}

    uiSkillAction* skill_action();
    SkillDialogPos& ui_position(){return ui_position_;}

    DWORD current_dialogkey_;
    SkillDialogPos ui_position_;
    bool init_data_;
    
    uiBase* skill_dlg_[kSkillMax];


    static void OnAnimationChanged();

};

//------------------------------------------------------------------------------
/**
*/
inline void uiSkillMan::ResetOnceInitSkillInfos()
{
    set_init_data(false);
    ui_position_.is_set = false;
}
