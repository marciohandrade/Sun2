#ifndef _uiNewSkillAction_h_
#define _uiNewSkillAction_h_

//------------------------------------------------------------------------------
/**

    스킬UI 리뉴얼

    @author		

    @since		

    @remarks	
- 
*/
#include "uiBase.h"
#include "uiBaseMan_def.h"
#include "SkillSlotContainer.h"

typedef std::vector<uicSkillInfo> SKILL_ACTION;
class SkillSlot;
struct SkillInven_s;
class uiSkillAction : public uiBase, SkillSlotContainer    
{

public:
    enum
    {
        kMaxTab = 3,
        kMaxColumn = 6,
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
        kMaxRow = 7,
        kMaxSlot = kMaxColumn * kMaxRow,
#else
        kMaxRow = 6,
        kMaxSlot = 36,
#endif
    };

    // 인덱스 중요함
    enum eDIALOG_POS
    {
        // 액션 슬롯
        kActionBegin = 0,              
        kActionEnd = kMaxSlot - 1,    

        // 탭버튼
        kTabBein,
        kTabEnd = kTabBein + (kMaxTab - 1),

#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
        kTextLeft, kTextRight,
#endif

        // ESC
        kEscButton, 
        kDlgMax,
    };

    uiSkillAction(InterfaceManager* ui_man);
    virtual ~uiSkillAction();

    virtual void Init(CDrawBase* draw_base);

    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);

    void LoadSkillActionSlot(const SkillInven_s* data);
    void InitItemRender();
    void FlushUnitDraw();
    void Clear();

    ItemUnitRenderer* GetItemUnitRender() const {return item_renderer_;}

    virtual SCSlotContainer* GetSlotContainer();

    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);

    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);

    SkillSlot* FindSlotByCode(CODETYPE skill_code);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    
    // 컨트롤러
    CControlWZ* getControl(POSTYPE pos);
    POSTYPE GetControlPos(WzID wzid);
    bool invalid_control() const { return invalid_control_; }

    void UpdateMouse();
    void RenderToolTip();
    
    void RefreshActionIcon();
    void RefreshTab();

    void MouseClickSlot(POSTYPE pos, bool pickup);
    void MouseClickTab(POSTYPE pos);
    void UpdateUnitRender();

    // 액션 리스트
    SKILL_ACTION& skill_action_list(){ return skill_action_list_; }
    int skill_action_size(){ return skill_action_list_.size(); }
    uicSkillInfo& skill_action_list(int idx){ return skill_action_list_[idx]; }

    SKILL_ACTION skill_action_list_;
    bool invalid_control_;
    
    ItemUnitRenderer* item_renderer_;
    static WzID wzId_[kDlgMax];

};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiSkillAction::GetControlPos(WzID wzId)
{
    for (POSTYPE idx = 0; idx < kDlgMax; ++idx)
    {
        if (wzId_[idx] == wzId)
        {
            return idx;
        }
    }
    return kDlgMax;
}
//------------------------------------------------------------------------------
/**
*/
inline CControlWZ* uiSkillAction::getControl(POSTYPE pos)
{
    assert( pos < kDlgMax );
    return GetControlWZ( wzId_[pos] );
}

#endif // _uiNewSkillAction_h_
