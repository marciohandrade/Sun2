#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "uiSkillTree_second.h"
#else

//------------------------------------------------------------------------------
/**

    ��ųUI ������

    @author		

    @since		10.09.03

    @remarks	
- 
*/
#pragma once
#include "uiBase.h"
#include "uiBaseMan_def.h"
#include "SkillSlotContainer.h"

#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
#include "uiSkillTreeEffect2.h"
#else
#include "uiSkillTreeEffect.h"
#endif


class SkillScriptInfo;
class SkillSlot;
class SkillDataSet;
struct SkillInven_s;


class uiSkillTree : public uiBase, public SkillSlotContainer
{
public:
    enum
    {
        kMaxTab = 3,
        kMaxClass = 5,
        kMaxColumn = 6,
        kMaxRow = 6,
        kMaxSlot = 36,
    };

    // �ε��� �߿���
    enum eDIALOG_POS
    {
        // ����
        kSlotBegin = 0,              
        kSlotEnd = kMaxSlot - 1,    
        // +��ư
        kPlusBegin,    
        kPlusEnd = kPlusBegin + (kMaxSlot - 1),
        // ����1.2
        kTextExt1,       
        kTextExt2,       
        kEscButton,                    
        // �ǹ�ư
        kTabBein,
        kTabEnd = kTabBein + (kMaxTab - 1),
        // ����̹���
        kImgBegin, 
        kImgEnd = kImgBegin + (kMaxClass - 1),

        // ����, ���� ��ư
        kButton1,kButton2,

        // ���� ��ư ���� ������ �̹���
        kButtonBlendImage,

        // �ܿ� ��ų����Ʈ ������ ���� �̹��� (������ ���)
        kRemainSkillPoint,
        kDlgMax,
    };

    uiSkillTree(InterfaceManager* ui_man);
    virtual ~uiSkillTree();

    // UI�⺻(��������)
    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual ItemUnitRenderer* GetItemUnitRender() const { return item_renderer_; }
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);								
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);

    // ������ �ε� & ����
    void SetSkillTotalInfo(const SKILL_TOTAL_INFO& skil_Info);
    void LoadSkillTreeSlot(const SkillInven_s* data);
    void LoadSkillTreeSlotArrow(const SkillInven_s* data);

    void UpdateLockState();
    void UpdateArrowState();

    void RefreshUI();
    void Clear();

    
    BOOL IsSkillSlot(SCSkillSlot* slot);

protected:
    VOID ShowDialog( BOOL val ); // override SolarDialog
    virtual void OnShowWindow( BOOL val );
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:

    SCSlotContainer* GetSlotContainer(); // override uiBase

    // UI ����
    void RefreshSkillIcon();
    void RefreshBackGround();
    void RefreshTab();
    void RefreshControlText();
    void RefreshPlusButton();
public:
    void RefreshTree(bool data_clear = true);
    void RefreshSkillSlot();
    void RefreshArrowImage();
private:

    // ���콺ó��
    void MouseClickButton(POSTYPE pos);
    void MouseClickSlot(POSTYPE pos, bool pickup);
    void OnClickPlusButton(POSTYPE pos);
    void MouseClickTab(POSTYPE pos);

    // ����
    void UpdateMouse();
    void UpdateUnitRender();
    void RenderSlotImage();
    void RenderToolTip();
    
    // ������ ����Ʈ ����
    void RenderOpenEffect();

    // �����۷�����
    void InitItemRender();
    void FlushUnitDraw();
    
    // ��Ʈ�ѷ�
    CControlWZ* getControl(POSTYPE pos);
    POSTYPE GetControlPos(WzID wzid);
    bool invalid_control() const { return invalid_control_; }


public:
    uiSkillTreeEffect& GetSkillTreeEffect() { return skill_tree_effect_; }

    static int GetMaxRowCount() { return kMaxRow; }
    SkillSlot* GetSkillSlot( int skill_slot_position );

private:
	uiSkillTreeEffect skill_tree_effect_;

	ItemUnitRenderer* item_renderer_;
    bool invalid_control_;

    int is_mouse_overed_on_button_;
    int mouse_overed_control_;


    static WzID wzId_[kDlgMax];

};
//------------------------------------------------------------------------------
/**
*/
inline POSTYPE uiSkillTree::GetControlPos(WzID wzId)
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
inline CControlWZ* uiSkillTree::getControl(POSTYPE pos)
{
    assert( pos < kDlgMax );
    return GetControlWZ( wzId_[pos] );
}
#endif //_NA_0_20120321_SKILLTREE_SECOND_RENEWAL