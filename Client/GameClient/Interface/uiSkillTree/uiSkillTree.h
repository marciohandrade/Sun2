#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "uiSkillTree_second.h"
#else

//------------------------------------------------------------------------------
/**

    스킬UI 리뉴얼

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

    // 인덱스 중요함
    enum eDIALOG_POS
    {
        // 슬롯
        kSlotBegin = 0,              
        kSlotEnd = kMaxSlot - 1,    
        // +버튼
        kPlusBegin,    
        kPlusEnd = kPlusBegin + (kMaxSlot - 1),
        // 숙련1.2
        kTextExt1,       
        kTextExt2,       
        kEscButton,                    
        // 탭버튼
        kTabBein,
        kTabEnd = kTabBein + (kMaxTab - 1),
        // 배경이미지
        kImgBegin, 
        kImgEnd = kImgBegin + (kMaxClass - 1),

        // 적용, 리셋 버튼
        kButton1,kButton2,

        // 적용 버튼 위의 깜빡임 이미지
        kButtonBlendImage,

        // 잔여 스킬포인트 영역의 투명 이미지 (툴팁에 사용)
        kRemainSkillPoint,
        kDlgMax,
    };

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

    // UI 갱신
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
    void RenderOpenEffect();

    // 아이템렌더러
    void InitItemRender();
    void FlushUnitDraw();
    
    // 컨트롤러
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