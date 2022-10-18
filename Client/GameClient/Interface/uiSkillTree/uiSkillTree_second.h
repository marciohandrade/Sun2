#pragma once
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL

#include "uiBase.h"
#include "uiBaseMan_def.h"
#include "SkillSlotContainer.h"

#include "DropdownControl.h"


class SkillScriptInfo;
class SkillSlot;
class SkillDataSet;
struct SkillInven_s;

#include "DrawTexture.h"

class uiSkillTree : public uiBase
{
private:
    nDrawTexture::DrawTextureManager m_DrawTextureManager;

    SkillSlotContainer skill_slot_container_;

public:
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

    void RefreshUI();
    void Clear();

    BOOL IsSkillSlot(SCSkillSlot* slot);

private:
    VOID ShowDialog( BOOL val ); // override SolarDialog
    virtual void OnShowWindow( BOOL val );
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

    SCSlotContainer* GetSlotContainer(); // override uiBase

private:

    // UI ����
    void RefreshSkillIcon();
    void RefreshBackGround();
    void RefreshTab();
    void RefreshControlText();
    void RefreshPlusButton();

public:
    void RefreshTree(bool data_clear = true);
    void RefreshSkillSlot();

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
    void RenderTreeEffect();

    // �����۷�����
    void InitItemRender();
    void FlushUnitDraw();
    
    // ��Ʈ�ѷ�
    CControlWZ* GetControlByPosition(POSTYPE pos);
    POSTYPE GetControlPosition(WzID wzid);

public:
    SkillSlot* GetSkillSlot( int skill_slot_position );

    static int GetMaxRowCount();

    void StartAnimation( int old_left_level, int left_level, int old_right_level, int right_level );

private:
	ItemUnitRenderer* item_renderer_;

    int is_mouse_overed_on_button_;
    int mouse_overed_control_;

    // Ư�� ���� (�ӽ�)
    DropdownControl dropdwon_attaib_;   
    int actived_attrib_index_;  // Ȱ��ȭ�� Ư���� �ε���

    void SetComboIndex( int index );
    void RefreshComboText();
    void OnChangedCombo();

    void _InitializeDrawTexture();
    void _ReleaseDrawTexture();

    int sound_handle_;
    BOOL is_animate_;

    int old_left_level_;
    int old_right_level_;
    int animate_start_time_;
};

#endif //_NA_0_20120321_SKILLTREE_SECOND_RENEWAL
