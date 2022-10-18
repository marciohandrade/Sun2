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

    // UI기본(슬롯포함)
    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);

    virtual ItemUnitRenderer* GetItemUnitRender() const { return item_renderer_; }
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);								
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);

    // 데이터 로드 & 갱신
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

    // UI 갱신
    void RefreshSkillIcon();
    void RefreshBackGround();
    void RefreshTab();
    void RefreshControlText();
    void RefreshPlusButton();

public:
    void RefreshTree(bool data_clear = true);
    void RefreshSkillSlot();

private:
    // 마우스처리
    void MouseClickButton(POSTYPE pos);
    void MouseClickSlot(POSTYPE pos, bool pickup);
    void OnClickPlusButton(POSTYPE pos);
    void MouseClickTab(POSTYPE pos);

    // 갱신
    void UpdateMouse();
    void UpdateUnitRender();
    void RenderSlotImage();
    void RenderToolTip();
    
    // 레벨업 이펙트 연출
    void RenderTreeEffect();

    // 아이템렌더러
    void InitItemRender();
    void FlushUnitDraw();
    
    // 컨트롤러
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

    // 특성 관련 (임시)
    DropdownControl dropdwon_attaib_;   
    int actived_attrib_index_;  // 활성화된 특성의 인덱스

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
