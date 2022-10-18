//  (C) 2010 Webzen Sun Studio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
#include "ItemUnitRenderer.h"
#include "uiSkillTree.h"
#include "SkillInventoryStoreParser.h"
#include "SkillSlot.h"
#include "SlotKeyGenerator.h"
#include "SkillInfoParser.h"
#include "QuickContainer.h"
#include "ItemManager.h"
#include "Hero.h"
#include "SkillMain.h"
#include "InterfaceManager.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Command.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "StringTableInfo.h"
#include "SoundEffect.h"
#include "MouseHandler.h"
#include "Mouse.h"


enum eDefine
{
    kMaxTab = 3,
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    kMaxClass = 8,
#else
  #ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    kMaxClass = 7,
  #else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    kMaxClass = 6,
  #endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    kMaxColumn = 6,
    kMaxRow = 7,
    kMaxSlot = kMaxColumn*kMaxRow,  // 42
};

// 인덱스 중요함
enum eDIALOG_POS
{
    // 슬롯

    // 배경
    kSlot_Background_Begin = 0,
    kSlot_Background_End = kSlot_Background_Begin + (kMaxSlot-1),

    kSlot_Slot_Begin,
    kSlot_Slot_End = kSlot_Slot_Begin + (kMaxSlot-1),

    kSlot_Button_Begin,
    kSlot_Button_End = kSlot_Button_Begin + (kMaxSlot-1),

    kSlot_Outline_Begin,
    kSlot_Outline_End = kSlot_Outline_Begin + (kMaxSlot-1),


    // 숙련1.2
    kTextExt1,       
    kTextExt2,

    // 닫기버튼
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

    kLeftGauge, kRightGauge,

    // 콤보박스용 컨트롤
    kCombo_TextButton,
    kCombo_Button,
    kCombo_List,
    kCombo_Background,

    // 특성 적용
    kButton_Apply,
    kButton_Apply_Image,

    kDlgMax,
};

//------------------------------------------------------------------------------
static WzID wzId_[kDlgMax] = 
{ 
    // BACKGROUND
    StrToWzID("P014"),StrToWzID("P182"),StrToWzID("P183"), StrToWzID("P035"),StrToWzID("P222"),StrToWzID("P223"),
    StrToWzID("P184"),StrToWzID("P185"),StrToWzID("P186"), StrToWzID("P224"),StrToWzID("P225"),StrToWzID("P226"),
    StrToWzID("P187"),StrToWzID("P188"),StrToWzID("P189"), StrToWzID("P227"),StrToWzID("P228"),StrToWzID("P229"),
    StrToWzID("P190"),StrToWzID("P191"),StrToWzID("P192"), StrToWzID("P230"),StrToWzID("P231"),StrToWzID("P232"),
    StrToWzID("P193"),StrToWzID("P194"),StrToWzID("P195"), StrToWzID("P233"),StrToWzID("P234"),StrToWzID("P235"),
    StrToWzID("P196"),StrToWzID("P197"),StrToWzID("P198"), StrToWzID("P236"),StrToWzID("P237"),StrToWzID("P238"),
    StrToWzID("P199"),StrToWzID("P200"),StrToWzID("P201"), StrToWzID("P239"),StrToWzID("P240"),StrToWzID("P241"),


    // SLOT
    StrToWzID("P056"),StrToWzID("P242"),StrToWzID("P243"), StrToWzID("P077"),StrToWzID("P262"),StrToWzID("P263"),
    StrToWzID("P244"),StrToWzID("P245"),StrToWzID("P246"), StrToWzID("P264"),StrToWzID("P265"),StrToWzID("P266"),
    StrToWzID("P247"),StrToWzID("P248"),StrToWzID("P249"), StrToWzID("P267"),StrToWzID("P268"),StrToWzID("P269"),
    StrToWzID("P250"),StrToWzID("P251"),StrToWzID("P252"), StrToWzID("P270"),StrToWzID("P271"),StrToWzID("P272"),
    StrToWzID("P253"),StrToWzID("P254"),StrToWzID("P255"), StrToWzID("P273"),StrToWzID("P274"),StrToWzID("P275"),
    StrToWzID("P256"),StrToWzID("P257"),StrToWzID("P258"), StrToWzID("P276"),StrToWzID("P277"),StrToWzID("P278"),
    StrToWzID("P259"),StrToWzID("P260"),StrToWzID("P261"), StrToWzID("P279"),StrToWzID("P280"),StrToWzID("P281"),


    // PLUS BUTTON
    StrToWzID("B004"),StrToWzID("B046"),StrToWzID("B047"), StrToWzID("B025"),StrToWzID("B066"),StrToWzID("B067"),
    StrToWzID("B048"),StrToWzID("B049"),StrToWzID("B050"), StrToWzID("B068"),StrToWzID("B069"),StrToWzID("B070"),
    StrToWzID("B051"),StrToWzID("B052"),StrToWzID("B053"), StrToWzID("B071"),StrToWzID("B072"),StrToWzID("B073"),
    StrToWzID("B054"),StrToWzID("B055"),StrToWzID("B056"), StrToWzID("B074"),StrToWzID("B075"),StrToWzID("B076"),
    StrToWzID("B057"),StrToWzID("B058"),StrToWzID("B059"), StrToWzID("B077"),StrToWzID("B078"),StrToWzID("B079"),
    StrToWzID("B060"),StrToWzID("B061"),StrToWzID("B062"), StrToWzID("B080"),StrToWzID("B081"),StrToWzID("B082"),
    StrToWzID("B063"),StrToWzID("B064"),StrToWzID("B065"), StrToWzID("B083"),StrToWzID("B084"),StrToWzID("B085"),


    // OUTLINE IMAGE
    StrToWzID("P140"),StrToWzID("P322"),StrToWzID("P323"), StrToWzID("P161"),StrToWzID("P342"),StrToWzID("P343"),
    StrToWzID("P324"),StrToWzID("P325"),StrToWzID("P326"), StrToWzID("P344"),StrToWzID("P345"),StrToWzID("P346"),
    StrToWzID("P327"),StrToWzID("P328"),StrToWzID("P329"), StrToWzID("P347"),StrToWzID("P348"),StrToWzID("P349"),
    StrToWzID("P330"),StrToWzID("P331"),StrToWzID("P332"), StrToWzID("P350"),StrToWzID("P351"),StrToWzID("P352"),
    StrToWzID("P333"),StrToWzID("P334"),StrToWzID("P335"), StrToWzID("P353"),StrToWzID("P354"),StrToWzID("P355"),
    StrToWzID("P336"),StrToWzID("P337"),StrToWzID("P338"), StrToWzID("P356"),StrToWzID("P357"),StrToWzID("P358"),
    StrToWzID("P339"),StrToWzID("P340"),StrToWzID("P341"), StrToWzID("P359"),StrToWzID("P360"),StrToWzID("P361"),

    //숙련1.2 텍스트
    StrToWzID("S001"), StrToWzID("S002"),

    // X버튼
    StrToWzID("B086"), 
    
    // 스킬.퍽.액션.이모티콘 탭
    StrToWzID("C000"), StrToWzID("C001"), StrToWzID("C002"),

    // 버서커.드래곤.섀도우.발키리.앨리, 미스틱
    StrToWzID("P001"), StrToWzID("P010"), StrToWzID("P012"), StrToWzID("P013"), StrToWzID("P011"), StrToWzID("P005"),

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    StrToWzID("P016"), StrToWzID("P019"), 
#else
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    StrToWzID("P016"),  // 임의로 미스틱 배경
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // 적용, 리셋 버튼
    StrToWzID("B001"), StrToWzID("B002"),

    // 적용 버튼 위의 깜빡임 이미지
    StrToWzID("P003"),

    // 잔여 스킬포인트 영역의 투명 이미지 (툴팁에 사용)
    StrToWzID("P006"),

    StrToWzID("P007"),  // 왼쪽 게이지
    StrToWzID("P009"),  // 오른쪽 게이지


    StrToWzID("B005"),   // 콤보용 텍스트버튼
    StrToWzID("B000"),   // 콤보용 버튼
    StrToWzID("L000"),   // 콤보용 리스트
    StrToWzID("P004"),   // 콤보용 배경

    StrToWzID("B003"),   // 활성화 버튼
    StrToWzID("P000"),   // 활성화 이펙트 이미지
};

int uiSkillTree::GetMaxRowCount()
{
    return kMaxRow;
}

//------------------------------------------------------------------------------
POSTYPE uiSkillTree::GetControlPosition(WzID wzId)
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
CControlWZ* uiSkillTree::GetControlByPosition(POSTYPE pos)
{
    assert( pos < kDlgMax );
    return GetControlWZ( wzId_[pos] );
}


////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
uiSkillTree::uiSkillTree(InterfaceManager* ui_man) :
uiBase(ui_man),
item_renderer_(NULL)
{
    skill_slot_container_.Init(kMaxSlot);

    this->InitItemRender();

    is_mouse_overed_on_button_ = false;
    mouse_overed_control_ = 0;

    is_animate_ = FALSE;

    sound_handle_ = 0;
}
//------------------------------------------------------------------------------
uiSkillTree::~uiSkillTree()
{
    this->Release();
}



SkillSlot* uiSkillTree::GetSkillSlot( int skill_slot_position )
{
    if( skill_slot_container_.IsEmpty( skill_slot_position ) == FALSE )
    {
        return static_cast<SkillSlot*>(&skill_slot_container_.GetSlot(skill_slot_position));
    }
    return NULL;
}

//------------------------------------------------------------------------------
/** 
*/
static int GetHeroIndex()
{
    int hero_index = g_pHero->GetClass() - 1;
    hero_index = max(hero_index, 0);
    hero_index = min(hero_index, kMaxClass - 1);
    return hero_index;
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    if( CControlWZ* control = GetControlByPosition( kButtonBlendImage ) )
        control->SetDiscardMessage(true);
    if( CControlWZ* control = GetControlByPosition( kButton_Apply_Image ) )
        control->SetDiscardMessage(true);

    for( int i=0;i<kMaxSlot;++i)
    {
        if( CControlWZ* control = GetControlByPosition( kSlot_Background_Begin + i ) )
            control->SetDiscardMessage(true);
        if( CControlWZ* control = GetControlByPosition( kSlot_Outline_Begin + i ) )
            control->SetDiscardMessage(true);
        if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kSlot_Slot_Begin + i ) )
            picture_control->EnableLButtonEvent(true);
    }

    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kLeftGauge ) )
    {
        picture_control->EnableInnerClipDraw(true);
        picture_control->SetClipRateHeight(0.3f);
    }
    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kRightGauge  ) )
    {
        picture_control->EnableInnerClipDraw(true);
        picture_control->SetClipRateHeight(0.8f);
    }

    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kButtonBlendImage ) )
    {
        picture_control->EnableBlend(true);
    }

    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kButton_Apply_Image ) )
    {
        picture_control->EnableBlend(true);
    }

    dropdwon_attaib_.Init( GetControlByPosition(kCombo_TextButton),
                            (CCtrlButtonWZ*)GetControlByPosition(kCombo_Button),
                            (CCtrlListWZ*)GetControlByPosition(kCombo_List), 
                            NULL, 
                            (CCtrlPictureWZ*)GetControlByPosition(kCombo_Background) );


    dropdwon_attaib_.set_simple_menu(true);
    dropdwon_attaib_.list_control()->SetStartLineGapTop(10);
    dropdwon_attaib_.list_control()->SetSellHeight(16);

    actived_attrib_index_ = 0;  // 일단 0번을 활성화로 설정
    SetComboIndex(0);           // 콤보박스의 인덱스를 0번으로 설정해둔다


    // 이번 버전에는 특성 관련 UI는 숨긴다
    dropdwon_attaib_.SetVisible(false);
    if( CControlWZ* control = GetControlByPosition( kButton_Apply ) )
    {
        control->ShowWindowWZ( WZ_HIDE );
    }
    if( CControlWZ* control = GetControlByPosition( kButton_Apply_Image ) )
    {
        control->ShowWindowWZ( WZ_HIDE );
    }


    _InitializeDrawTexture();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Release()
{
    _ReleaseDrawTexture();
    dropdwon_attaib_.Release();

    FlushUnitDraw();
    SAFE_RELEASENDELETE(item_renderer_);
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::Clear()
{
    FlushUnitDraw();

    xGetSkillData()->ClearSkillData();
}

void uiSkillTree::_InitializeDrawTexture()
{       
    TCHAR skill_ui[] = "Data\\Interface\\Common_bg02.tga";

    TCHAR skill_effect[] = "Data\\Interface\\skill_eff.tga";
    TCHAR skill_effect2[] = "Data\\Interface\\skill_eff_02.tga";

    m_DrawTextureManager.Initialize();        
    m_DrawTextureManager.AddImage( "chain_left", skill_ui,       0, 850, 170, 29 );
    m_DrawTextureManager.AddImage( "chain_right", skill_ui,       170, 850, 170, 29 );

    char temp[256];
    for( size_t i=0;i!=4;++i)
    {
        for( size_t j=0;j!=4;++j)
        {
            wsprintf(temp, "move_effect_right_%01d", i*4+j);
            m_DrawTextureManager.AddImage( temp, skill_effect,   j*128, i*128, 128, 128 );
        }
    }
    for( size_t i=0;i!=4;++i)
    {
        for( size_t j=0;j!=4;++j)
        {
            wsprintf(temp, "move_effect_left_%01d", i*4+j);
            m_DrawTextureManager.AddImage( temp, skill_effect2,   j*128, i*128, 128, 128 );
        }
    }
}

void uiSkillTree::_ReleaseDrawTexture()
{
    m_DrawTextureManager.Release();
}

void uiSkillTree::SetComboIndex( int index )
{
    RefreshComboText();
    dropdwon_attaib_.Select( index );

    OnChangedCombo();
}

void uiSkillTree::OnChangedCombo()
{
    // 콤보박스의 선택이 변경된 경우 상황에 맞춰 '활성화' 버튼의 상태를 변경한다.

    int current_combo_index = dropdwon_attaib_.list_control()->GetCurSel();

    bool is_enable_button = actived_attrib_index_ != current_combo_index;

    if( CControlWZ* control = GetControlByPosition( kButton_Apply ) )
    {
        control->EnableWindowWZ( is_enable_button );
    }

    if( CControlWZ* control = GetControlByPosition( kButton_Apply_Image ) )
    {
        control->ShowWindowWZ( is_enable_button == true ? WZ_SHOW : WZ_HIDE );
    }
}


void uiSkillTree::RefreshComboText()
{
    dropdwon_attaib_.DeleteItemAll();

    WzColor color1 = WzColor_RGBA( 240, 201, 121, 255 );
    WzColor color2 = WzColor_RGBA( 117, 117, 117, 255 );

    DWORD dropdown_index = 0;
    dropdwon_attaib_.InsertItem( actived_attrib_index_ == dropdown_index ? 2467 : 2469, NULL, StrToWzID("st10"), DT_VCENTER | DT_CENTER, 0, actived_attrib_index_ == dropdown_index ? color1 : color2 );
    ++dropdown_index;
    dropdwon_attaib_.InsertItem( actived_attrib_index_ == dropdown_index ? 2468 : 2470, NULL, StrToWzID("st10"), DT_VCENTER | DT_CENTER, 0, actived_attrib_index_ == dropdown_index ? color1 : color2 );

    dropdwon_attaib_.ResetVScroll();
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer* uiSkillTree::GetSlotContainer()
{
    return &skill_slot_container_;
}

//------------------------------------------------------------------------------
/** 슬롯 삽입
*/
RC::eSLOT_INSERT_RESULT uiSkillTree::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    if (skill_slot_container_.ValidPos(at_pos) == false)
    {
        return RC::RC_INSERT_SLOT_FAILED;
    }

    // 슬롯에 삽입
    return skill_slot_container_.SCSkillSlotContainer::InsertSlot(at_pos, slot);
}
//------------------------------------------------------------------------------
/** 슬롯 삭제
*/
BOOL uiSkillTree::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    if (skill_slot_container_.IsEmpty(at_pos))
    {
        return FALSE;
    }

    if (GetItemUnitRender())
    {
        GetItemUnitRender()->RemoveItemUnit(skill_slot_container_.GetSlot(at_pos).GetSerial());
    }

    return skill_slot_container_.DeleteSlot(at_pos, slot_clone_before_delete); 
}
//------------------------------------------------------------------------------
/**  SkillInventory.txt 정보내용을 채운다
*/
void uiSkillTree::LoadSkillTreeSlot(const SkillInven_s* data)
{
    for (int idx = 0; idx < kMaxColumn; ++idx)
	{
		int skill_Code = data->iSkillID[idx];
		if (skill_Code <= 0)
		{
			continue;
		}


		//슬롯 내용채움
		SkillSlot skill_slot;
		skill_slot.SetCode(skill_Code);
		skill_slot.SetSerial(g_SlotKeyGenerator.GetKey());
        int real_pos = (data->bSkillTab2 - 1)*kMaxColumn + idx;
        InsertSlot(real_pos, skill_slot);



		SkillData skill_data;
		skill_data.SetSlotPosition( real_pos, data->bSkillTab2 - 1 );
		skill_data.SetCurSkill( skill_Code );

        xGetSkillData()->AddSkillData( skill_data );

		//curSkillInfo->SetFirst();
		//BASE_ABILITYINFO* ablity_info = NULL;
		//while (ablity_info = curSkillInfo->GetNext())
		//{
		//	info.currStateCodes.push_back(ablity_info->m_wAbilityID);
		//}
	}
}

//------------------------------------------------------------------------------
/** 게임 접속시 캐릭터정보의 스킬 슬롯스트림을 통해 스킬 정보를 초기화
*/
void uiSkillTree::SetSkillTotalInfo(const SKILL_TOTAL_INFO& skill_total_info)
{
    const SKILLSLOT* total_slot = skill_total_info.m_Slot;

    SkillInfoParser* parser = SkillInfoParser::Instance();
    for (POSTYPE idx = 0; idx < skill_total_info.m_Count; ++idx)
    {
        SkillSlot post_slot(total_slot[idx].m_Stream);
        if( SkillScriptInfo* skill_info = parser->GetSkillInfo(post_slot.GetCode()) )
        {
            // 스킬 활성화
            xGetSkillData()->AddNewSkillCode( post_slot.GetCode() );
            //SetSkillActive(post_slot.GetCode());
        }
    }

    xGetSkillData()->RefreshActiveSkill( xGetSkillData()->GetSkillDataSet() );

    //RefreshArrowImage();
    RefreshSkillSlot();
}

void uiSkillTree::RefreshSkillSlot()
{
    // 스킬정보가 변경된것이 있다면 여기에서 갱신됨
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        int slot_position = skill_data.GetSlotPosition();
        if( skill_slot_container_.IsEmpty( slot_position ) == FALSE )
        {
            SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(skill_slot_container_.GetSlot( skill_data.GetSlotPosition() ) );
            skill_slot.SetSlotType( ST_SKILL );
            skill_slot.SetCode( skill_data.GetSkillCode() );
        }
    }


    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kLeftGauge ) )
    {
        size_t use_point = skill_set.GetTotalUsedPoint(true);
        float gauge_height = (float)use_point / 140.0f;
        picture_control->SetClipRateHeight(gauge_height);
    }
    if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kRightGauge  ) )
    {
        size_t use_point = skill_set.GetTotalUsedPoint(false);
        float gauge_height = (float)use_point / 140.0f;
        picture_control->SetClipRateHeight(gauge_height);
    }


}

//------------------------------------------------------------------------------
/** 탭정보 갱신(이펙트 애니를 위해 저장해둔다)
*/
void uiSkillTree::RefreshTree(bool data_clear)
{
    // 적용, 취소 버튼 표시여부
    if( xGetSkillData()->IsSkillSimulated() )
    {

        if (GetControlWZ( wzId_[kButtonBlendImage]) != NULL)
        {
            GetControlWZ( wzId_[kButtonBlendImage] )->ShowWindowWZ(WZ_SHOW);
        }
        if (GetControlWZ(wzId_[kButton1])  != NULL)
        {
            GetControlWZ( wzId_[kButton1] )->EnableWindowWZ(WZS_ENABLE);
        }
        if (GetControlWZ(wzId_[kButton2])  != NULL)
        {
            GetControlWZ( wzId_[kButton2] )->EnableWindowWZ(WZS_ENABLE);
        }
    }
    else
    {
        if (GetControlWZ( wzId_[kButtonBlendImage]) != NULL)
        {
            GetControlWZ( wzId_[kButtonBlendImage] )->ShowWindowWZ(WZ_HIDE);
        }
        if (GetControlWZ( wzId_[kButton1]) != NULL)
        {
            GetControlWZ( wzId_[kButton1] )->EnableWindowWZ(WZS_DISABLE);
        }
        if (GetControlWZ( wzId_[kButton2]) != NULL)
        {
            GetControlWZ( wzId_[kButton2] )->EnableWindowWZ(WZS_DISABLE);
        }
    }

    // 일단 다 숨긴다
    for (int i = 0; i < kMaxSlot; ++i)
    {
        if( CControlWZ* control = GetControlByPosition(kSlot_Background_Begin + i) )
        {
            control->ShowWindowWZ(WZ_HIDE);
        }

        if( CControlWZ* control = GetControlByPosition(kSlot_Button_Begin + i) )
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
        if( CControlWZ* control = GetControlByPosition(kSlot_Outline_Begin + i) )
        {
            control->ShowWindowWZ(WZ_HIDE);
        }
    }

    // 레벨업 상태를 갱신한 후
    xGetSkillData()->RefreshLevelupCondition();

    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        // 스킬 있는경우에는 배경을 표시
        if( CControlWZ* control = GetControlByPosition(skill_set[i].GetSlotPosition() + kSlot_Background_Begin) )
        {
            control->ShowWindowWZ(WZ_SHOW);
        }

        // 레벨업이 가능하면 버튼 표시
        if (skill_set[i].IsEnableLevelup())
        {
            if( CControlWZ* control = GetControlByPosition(skill_set[i].GetSlotPosition() + kSlot_Button_Begin) )
            {
                control->ShowWindowWZ(WZ_SHOW);
            }
        }

        // 최대레벨인경우 테두리 표시
        if (skill_set[i].IsMaxLevel() )
        {
            if( CControlWZ* control = GetControlByPosition(skill_set[i].GetSlotPosition() + kSlot_Outline_Begin) )
            {
                control->ShowWindowWZ(WZ_SHOW);
            }
        }
    }

    //// 잠김 이미지처리
    //UpdateLockState();
}

//------------------------------------------------------------------------------
/** 이 슬롯이 스킬 슬롯의 아이템인가 (아니면 퀵슬롯의 스킬슬롯인가 -_-)
*/
BOOL uiSkillTree::IsSkillSlot(SCSkillSlot* slot)
{
    SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for( size_t i=0;i!=skill_set.size();++i)
    {
        SkillData& skill_data = skill_set[i];

        SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(skill_slot_container_.GetSlot( skill_data.GetSlotPosition() ) );
        if( &skill_slot == slot )
        {
            return TRUE;
        }
    }
    
    return FALSE;
}


//------------------------------------------------------------------------------
/** 
*/
void uiSkillTree::InitItemRender()
{
    if (GetItemUnitRender() == NULL)
    {
        item_renderer_ = new ItemUnitRenderer;
        item_renderer_->Init( uiBase::GetDrawBase(), skill_slot_container_.GetMaxSlotNum() );
        item_renderer_->ShowCooldown(TRUE);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::FlushUnitDraw()
{
    for (int idx = 0; idx < skill_slot_container_.GetMaxSlotNum(); ++idx)
    {
        if (skill_slot_container_.IsEmpty(idx) == FALSE)
        {
            g_SlotKeyGenerator.Restore(DWORD(skill_slot_container_.GetSlot(idx).GetSerial()));
            DeleteSlot(idx, NULL);
        }
    }
    skill_slot_container_.ClearAll();
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RefreshUI()
{
    RefreshTree();
    RefreshSkillIcon();
    RefreshBackGround();
    RefreshTab();
    RefreshControlText();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnUpdateSolarDialog()
{
    dropdwon_attaib_.Update();

    UpdateUnitRender();
    UpdateMouse();
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnRenderSolarDialog()
{
    // UI의 Draw 이후에 별도로 해주어야하는것들이 꽤 있다

    if ( GetItemUnitRender() == NULL )
    {
        return;
    }

    // 아이콘
    GetItemUnitRender()->Render();

    // 아이콘의 레벨
    RenderSlotImage();

    // 레벨업 버튼은 다시한번 직접 그려준다 (아이콘 이미지에 묻혀버려서...)
    for( int i=0;i!=kMaxSlot;++i)
    {
        if( CControlWZ* button_control = GetControlByPosition(kSlot_Button_Begin + i) )
        {
            button_control->OnDraw(true);
        }
    }


    // 잔여포인트 표시
    {
        DWORD remain_point = xGetSkillData()->GetRemainUseablePoint();

        RECT control_rect;
        GetDialogWZ()->GetClientRect(&control_rect);

        static int draw_x = 173;
        static int draw_y = 99;
        static int draw_w = 32;
        static int draw_h = 20;

        RECT draw_rect;
        draw_rect.left = draw_x + control_rect.left;
        draw_rect.top = draw_y + control_rect.top;
        draw_rect.right = draw_rect.left + draw_w;
        draw_rect.bottom = draw_rect.top + draw_h;

        g_InterfaceManager.RenderDrawNumber( draw_rect, remain_point, DT_RIGHT | DT_TOP, eIMAGE_NUMBER_LARGE_YELLOW );
    }

    RenderTreeEffect();


    // 특성 리스트를 맨마지막에.. 
    if( CControlWZ* control = GetControlByPosition(kCombo_List) )
    {
        control->OnDraw(true);
    }

    RenderToolTip();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiSkillTree::ShowDialog( BOOL val )
{
    if( val == TRUE )
    {
        xGetSkillData()->ResetTreeData();
        //RefreshArrowImage();
        RefreshSkillSlot();
    }
    else if( val == FALSE )
    {
        xGetSkillData()->ResetTreeData();
    }


    uiBase::ShowDialog( val );
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::OnShowWindow(BOOL val)
{
    if( uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL) )
    {
        ui_skill_man->OpenDialog(uiSkillMan::kSkillTree, val);

        if (val)
        {
            RefreshUI();
        }
        else
        {
            // 효과음이 재생중이면 끈다
            if( sound_handle_ != 0 )
            {
                g_SoundEffect.StopUI( sound_handle_ );
                sound_handle_ = 0;
            }

            // 애니메이션이 진행중이면 끈다
            is_animate_ = FALSE;
        }

    }

}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::MessageProc(SI_MESSAGE* message)
{
    {   // 특성 콤보의 메세지를 먼저 처리
        bool is_change = false;
        if (dropdwon_attaib_.MessageProc(message, OUT is_change))
        {
            if (is_change)
            {
                OnChangedCombo();
            }
            return;
        }
    }


    POSTYPE pos = GetControlPosition(message->dwCtrlID);

    switch (message->eResultMsg) 
    {
    case RT_MSG_RBUTTONCLICK:
        {
            MouseClickSlot(pos, false);
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
            is_mouse_overed_on_button_ = false;

            if( pos == kButton1 )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            else if( pos == kButton2 )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            else if( pos == kRemainSkillPoint )
            {
                mouse_overed_control_ = pos;
                is_mouse_overed_on_button_ = true;
            }
            break;
        }
    case RT_MSG_LBUTTONCLICK:
        {
            MouseClickButton(pos);
            MouseClickSlot(pos, true);
            OnClickPlusButton(pos);
            MouseClickTab(pos);

            if (pos == kEscButton)
            {
                ShowInterface(FALSE);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::MouseClickTab(POSTYPE pos)
{
    if ((pos < kTabBein) ||
        (pos > kTabEnd))
    {
        return;
    }
    
    RefreshTab();
    
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    ui_skill_man->OpenTab((pos - kTabBein) + 1);
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::UpdateUnitRender()
{
    if (GetItemUnitRender() == NULL)
    {
        return;
    }

    GetItemUnitRender()->Update();

    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    ItemUnitRenderer::RenderUnit* render_unit = NULL;
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        const SkillData& skill_data = skill_set.at(i);
        SCSlot& slot = skill_slot_container_.GetSlot( skill_data.GetSlotPosition() );

        render_unit = GetItemUnitRender()->GetItemUnit( slot.GetSerial() );
        if (render_unit)
        {
			render_unit->SetState( skill_data.GetBlockState() ? skill_data.GetBlockState() : skill_data.GetState() );
            render_unit->SetShow(TRUE);
        }
    }
}

void uiSkillTree::MouseClickButton(POSTYPE pos)
{
    if ( pos == kButton1 )
    {
        std::vector<int> new_code_array;
        if( xGetSkillData()->MakeNewSkillCodeArray( new_code_array ) == true )
        {
            MSG_CG_SKILL_SELECT_SKILLPOINT_SYN send_packet;
            send_packet.skill_count = new_code_array.size();
            for( size_t i=0;i!=new_code_array.size();++i)
            {
                send_packet.skill_code_array[i] = new_code_array[i];
            }


            // 2803	스킬을 배우시곘습니까?
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString( eST_SKILL_TREE_LEARN_MESSAGEBOX, szMessage, INTERFACE_STRING_LENGTH );
            if( uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM) )
            {
                pSystemMan->Sys_ConfirmLock(szMessage, new SendPacketCommand( CI_GAMESERVERIDX, &send_packet, sizeof(send_packet) ) );
            }
        }

        // 적용
        pos = pos;
        return;
    }
    else if( pos == kButton2 )
    {
        xGetSkillData()->ResetTreeData();
        //RefreshArrowImage();
        RefreshSkillSlot();
        RefreshUI();

        // 리셋
        return;
    }
}

//------------------------------------------------------------------------------
/** 마우스클릭처리(우클릭:집기, 좌클릭:사용)
*/
void uiSkillTree::MouseClickSlot(POSTYPE control_position, bool pickup)
{
    if( control_position >= kSlot_Slot_Begin && control_position <= kSlot_Slot_End )
    {
        int slot_position = control_position - kSlot_Slot_Begin;

        if( skill_slot_container_.IsEmpty(slot_position) == TRUE )
        {
            return;
        }

        if ( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySlotPosition(slot_position) )
	    {

            if( xGetSkillData()->IsSkillSimulated() )
            {
                // 2804, 스킬 시뮬레이션 중에는 스킬 아이콘을 이동, 사용할 수 없습니다. 
                TCHAR buffer[INTERFACE_STRING_LENGTH];
                g_InterfaceManager.GetInterfaceString(eST_SKILL_TREE_BEGIN_NOTICE, buffer, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_SysMsg(buffer);

                return;
            }

		    if ( skill_data->IsActiveState() && skill_data->IsActive() && skill_data->IsPassiveSkill() == false )
		    {
			    if (pickup)
			    {
                    if( CControlWZ* control = GetControlByPosition(control_position) )
                    {
                        RECT rect = control->GetSizeRect();
				        MouseHandler::Instance()->ItemTransaction(SI_SKILL, slot_position, &rect);
                    }
			    }
			    else
			    {
				    MouseHandler::Instance()->ItemUseTransaction(SI_SKILL, slot_position);
			    }
		    }
	    }
    }
}


//------------------------------------------------------------------------------
/** 레벨업 버튼
*/
void uiSkillTree::OnClickPlusButton(POSTYPE pos)
{
    // 버튼 영역체크
    if ((pos < kSlot_Button_Begin) ||
        (pos > kSlot_Button_End))
    {
        return;
    }
  
    if( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySlotPosition(pos - kSlot_Button_Begin) )
    {
        g_SoundEffect.PlayUI( UI_SOUND_SKILLTREE_BUTTON );

        if( xGetSkillData()->LevelupSkill(skill_data) == true )
        {
            //// 이 스킬을 습득했을때 활성화 되는 화살표가 있으면 표시하도록 설정
            //skill_tree_effect_.SetAnimationArrow( skill_data->GetSkillClassCode() , skill_data->GetSkillLevel() );

            RefreshTree(true);
            RefreshSkillSlot();
        }
    }
}
//------------------------------------------------------------------------------
/** 슬롯돌며 마우스 오버 체크
*/
void uiSkillTree::UpdateMouse()
{
    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        POSTYPE slot_pos = skill_set[i].GetSlotPosition();
        
        if( CControlWZ* control = GetControlByPosition(slot_pos) )
        {
            RECT rect = control->GetSizeRect();
            if (rect.left <= mouse_x && rect.right >= mouse_x)
            {
                if (rect.top <= mouse_y && rect.bottom >= mouse_y)
                {
                    m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SKILL;
                    m_MouseOverObject.iSkillIdx = i;
                    break;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RenderToolTip()
{
    // 버튼의 툴팁
    if (is_mouse_overed_on_button_ && 
        (mouse_overed_control_ != INVALID_POSTYPE_VALUE))
    {
        if( CControlWZ* control = GetControlByPosition(mouse_overed_control_) )
        {
            if( uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP) )
            {
                int string_index;
                if( mouse_overed_control_ == kButton1 )
                {
                    string_index = eST_SKILL_TREE_LEARN_APPLY;
                }
                else if( mouse_overed_control_ == kButton2 )
                {
                    string_index = eST_SKILL_TREE_LEARN_RESET;
                }
                else if( mouse_overed_control_ == kRemainSkillPoint )
                {
                    string_index = eST_SKILL_TREE_TOOLTIP_REMAIN_POINT;
                }

                static TCHAR buffer[INTERFACE_STRING_LENGTH];

                g_InterfaceManager.GetInterfaceString( string_index, buffer, INTERFACE_STRING_LENGTH );
                tooltip_manager->RegisterTooltip( buffer );
				tooltip_manager->SetTooltipDialogID( GetDialogKey() );
            }
        } 
    }

    if (m_MouseOverObject.eMouseOverType != eMOUSE_OVER_SKILL)
    {
        return;
    }

    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();

    const SkillData& skill_data = skill_set.at((size_t)m_MouseOverObject.iSkillIdx);
    SCSkillSlot& skill_slot = static_cast<SCSkillSlot &>(skill_slot_container_.GetSlot(skill_data.GetSlotPosition()));
      
    if (uiToolTipMan* tooltip = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP) )
    {
       tooltip->RegisterTooltipSkill(skill_data.GetCurSkillInfo(),
                                     skill_data.GetNextSkillInfo(), 
                                     skill_data.IsActiveState());
	   tooltip->SetTooltipDialogID(GetDialogKey());

    }
}

void uiSkillTree::RenderTreeEffect()
{
    static int left_offect_x = 24;
    static int left_offset_y = 163;
    static int right_offset_x = 187;
    static int right_offset_y = 163;

    static float block_left_offset_x = 24.f;
    static float block_right_offset_x = 190.f;
    static float block_offset_y = 181.f;
    static float block_width = 167.f;
    static float block_height = 56.f;
    static DWORD block_color = 0x60000000;

    bool is_simulated = xGetSkillData()->IsSkillSimulated();

    if( is_animate_ == FALSE )
    {
        // 0: 기본 상태

        // 레벨별 간격을 구한다
        int gauge_height_per_level = 0;
        if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kLeftGauge ) )
        {
            RECT gauge_rect = picture_control->GetSizeRect();
            gauge_height_per_level = gauge_rect.bottom - gauge_rect.top;
            gauge_height_per_level /= 7;
        }

        // 좌우 현재 표시 레벨을 구한다
        const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSet();

        int left_position = skill_set.GetTotalUsedPoint(true) / 20;
        int right_position = skill_set.GetTotalUsedPoint(false) / 20;


        // 다이얼로그의 현재 위치를 구한다
        RECT dialog_rect;
        GetDialogWZ()->GetClientRect( &dialog_rect );

        
        // 현재 표시레벨 이하를 어두워 지도록 ColorFill  (좌,우)
        {
            int block_position = left_position;
            while( block_position < kMaxColumn )
            {
                float draw_x = (float)dialog_rect.left + block_left_offset_x;
                float draw_y = (float)dialog_rect.top + block_offset_y + (float)(gauge_height_per_level*block_position);
                g_pSunRenderer->ColorFill( block_color, draw_x, draw_y, block_width, block_height );
                ++block_position;
            }
        }

        {
            int block_position = right_position;
            while( block_position < kMaxColumn )
            {
                float draw_x = (float)dialog_rect.left + block_right_offset_x;
                float draw_y = (float)dialog_rect.top + block_offset_y + (float)(gauge_height_per_level*block_position);
                g_pSunRenderer->ColorFill( block_color, draw_x, draw_y, block_width, block_height );
                ++block_position;
            }
        }

        // 현재 표시레벨 위치에 체인 이미지를 그린다 (좌,우)
        if( left_position < kMaxRow - 1 )
        {
            RECT draw_rect;
            draw_rect.left = dialog_rect.left + left_offect_x;
            draw_rect.top = dialog_rect.top + left_offset_y;
            draw_rect.right = draw_rect.left + 170;
            draw_rect.bottom = draw_rect.top + 29;
            m_DrawTextureManager.Draw( "chain_left", draw_rect, 0, gauge_height_per_level*left_position, is_simulated ? 0.5f : 1.0f );
        }

        if( right_position < kMaxRow - 1 )
        {
            RECT draw_rect;
            draw_rect.left = dialog_rect.left + right_offset_x;
            draw_rect.top = dialog_rect.top + right_offset_y;
            draw_rect.right = draw_rect.left + 170;
            draw_rect.bottom = draw_rect.top + 29;
            m_DrawTextureManager.Draw( "chain_right", draw_rect, 0, gauge_height_per_level*right_position, is_simulated ? 0.5f : 1.0f );
        }
    }
    else if( is_animate_ == TRUE )
    {
        // 1: 애니메이션 상태

        // 전체 구간은 아래와 같이 구성되어있다
        // 0 ~ 500 정지 (토네이도 생성)
        // 500 ~ 2500 이동
        // 2500 ~ 3000 정지 (토네이도 페이드아웃)

        // delta_time 애니메이션 전체 진행 시간
        // max_time 애니메이션 전체 시간
        // move_delta 이동 진행시간
        // max_move_time 이동 전체 시간

        int delta_time = GetCurrentTime() - animate_start_time_;
        int max_time = 2000;    // 아래3개의 시간으로 구성된 총 시간
        int max_move_time = 1500;
        int max_fadein_time = 150;
        int max_fadeout_time = 350;

        int move_delta = 0;
        if( delta_time >= max_fadein_time )
            move_delta = min(delta_time - max_fadein_time, max_move_time);

        // 토네이도의 알파를 구해둔다 (페이드 아웃일때)
        float tornado_rate = 1.0f;
        if( delta_time + max_fadeout_time > max_time )
        {
            // 종료 0.5초직전 부터 감소
            tornado_rate = (float)((max_time - delta_time)/(float)max_fadeout_time);
            if( tornado_rate < 0.0f )
                tornado_rate = 0.0f;
        }


        // 레벨별 간격을 구한다
        int gauge_height_per_level = 0;
        if( CCtrlPictureWZ* picture_control = (CCtrlPictureWZ*)GetControlByPosition( kLeftGauge ) )
        {
            RECT gauge_rect = picture_control->GetSizeRect();
            gauge_height_per_level = gauge_rect.bottom - gauge_rect.top;
            gauge_height_per_level /= 7;
        }

        // 좌우 표시 레벨을 구한다
        const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSet();

        int left_position = skill_set.GetTotalUsedPoint(true) / 20;
        int right_position = skill_set.GetTotalUsedPoint(false) / 20;

        if( left_position > kMaxRow -1 )
            left_position = kMaxRow -1;
        if( right_position > kMaxRow -1 )
            right_position = kMaxRow -1;


        // 다이얼로그의 현재 위치를 구한다
        RECT dialog_rect;
        GetDialogWZ()->GetClientRect( &dialog_rect );


        // 회오리 이펙트의 프레임 인덱스를 구한 후 텍스쳐 키워드를 구해둔다
        // 총 16장의 스프라이트를 이용하며, 시작할 때 4개가 애니메이션 되고, 이후 나머지 12개가 루프를 돌며 애니메이션이 된다.
        int frame_index = 0;

        int time_per_frame = 60;   // 1프레임에 사용할 시간 0.06초 즉 초당 1.6666 프레임 (1000/60)
        if( delta_time <= time_per_frame * 4 )
        {
            // 최초의 4프레임 (0,1,2,3 프레임)
            frame_index = delta_time / time_per_frame;
        }
        else
        {
            // 이후 12프레임의 루프(4~15)
            frame_index = (delta_time - time_per_frame*4 ) / time_per_frame;
            frame_index = frame_index % 12;
            frame_index += 4;
        }

        char left_tornado_name[256];
        char right_tornado_name[256];
        wsprintf(left_tornado_name,"move_effect_left_%01d", frame_index );
        wsprintf(right_tornado_name,"move_effect_right_%01d", frame_index );




        // 현재 표시레벨 이하를 어두워 지도록 ColorFill  (좌,우)
        {
            int block_position = old_left_level_;
            while( block_position < kMaxRow - 1 )
            {
                float draw_x = (float)dialog_rect.left + block_left_offset_x;
                float draw_y = (float)dialog_rect.top + block_offset_y + (float)(gauge_height_per_level*block_position);
                g_pSunRenderer->ColorFill( block_color, draw_x, draw_y, block_width, block_height );
                ++block_position;
            }
        }

        {
            int block_position = old_right_level_;
            while( block_position < kMaxRow - 1 )
            {
                float draw_x = (float)dialog_rect.left + block_right_offset_x;
                float draw_y = (float)dialog_rect.top + block_offset_y + (float)(gauge_height_per_level*block_position);
                g_pSunRenderer->ColorFill( block_color, draw_x, draw_y, block_width, block_height );
                ++block_position;
            }
        }


        // 체인과, 토네이도를 그린다
        static int tornado_offset_x = 5;
        static int tornado_offset_y = 145;
        static int tornado_offset_x2 = 312;
        static int tornado_offset_y2 = 145;
        static int tornado_size = 64;

        if( old_left_level_ < kMaxRow -1 )
        {
            if( old_left_level_ == left_position )
            {
                RECT draw_rect;
                draw_rect.left = dialog_rect.left + left_offect_x;
                draw_rect.top = dialog_rect.top + left_offset_y;
                draw_rect.right = draw_rect.left + 170;
                draw_rect.bottom = draw_rect.top + 29;
                m_DrawTextureManager.Draw( "chain_left", draw_rect, 0, gauge_height_per_level*left_position, 1.0f );
            }
            else
            {
                // move_delta 를 이용해 이전 레벨과 현재 레벨 사이를 이동시킨 후 토네이도와 함꼐 그린다.
                int gauge_height_by_frame = old_left_level_ * gauge_height_per_level + 
                    (left_position - old_left_level_) * gauge_height_per_level * move_delta / max_move_time;

                RECT draw_rect;
                draw_rect.left = dialog_rect.left + left_offect_x;
                draw_rect.top = dialog_rect.top + left_offset_y;
                draw_rect.right = draw_rect.left + 170;
                draw_rect.bottom = draw_rect.top + 29;
                m_DrawTextureManager.Draw( "chain_left", draw_rect, 0, gauge_height_by_frame, left_position == kMaxRow-1 ? tornado_rate : 1.0f );

                draw_rect.left = dialog_rect.left + tornado_offset_x;
                draw_rect.top = dialog_rect.top + tornado_offset_y;
                draw_rect.right = draw_rect.left + tornado_size;
                draw_rect.bottom = draw_rect.top + tornado_size;
                m_DrawTextureManager.Draw( left_tornado_name, draw_rect, 0, gauge_height_by_frame, tornado_rate );
            }
        }

        if( old_right_level_ < kMaxRow - 1 )
        {
            if( old_right_level_ == right_position )
            {
                RECT draw_rect;
                draw_rect.left = dialog_rect.left + right_offset_x;
                draw_rect.top = dialog_rect.top + right_offset_y;
                draw_rect.right = draw_rect.left + 170;
                draw_rect.bottom = draw_rect.top + 29;
                m_DrawTextureManager.Draw( "chain_right", draw_rect, 0, gauge_height_per_level*right_position, 1.0f );
            }
            else
            {
                int gauge_height_by_frame = old_right_level_ * gauge_height_per_level + 
                    (right_position - old_right_level_) * gauge_height_per_level * move_delta / max_move_time;

                RECT draw_rect;
                draw_rect.left = dialog_rect.left + right_offset_x;
                draw_rect.top = dialog_rect.top + right_offset_y;
                draw_rect.right = draw_rect.left + 170;
                draw_rect.bottom = draw_rect.top + 29;
                m_DrawTextureManager.Draw( "chain_right", draw_rect, 0, gauge_height_by_frame, right_position == kMaxRow-1 ? tornado_rate : 1.0f );

                draw_rect.left = dialog_rect.left + tornado_offset_x2;
                draw_rect.top = dialog_rect.top + tornado_offset_y2;
                draw_rect.right = draw_rect.left + tornado_size;
                draw_rect.bottom = draw_rect.top + tornado_size;
                m_DrawTextureManager.Draw( right_tornado_name, draw_rect, 0, gauge_height_by_frame, tornado_rate );
            }
        }

        // 시간이 만료되면 종료
        if( delta_time > max_time )
            is_animate_ = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiSkillTree::RenderSlotImage()
{
    const SkillDataSet& skill_set = xGetSkillData()->GetSkillDataSetForTree();
    for (size_t i = 0; i < skill_set.size(); ++i)
    {
        const SkillData& skill_data = skill_set[i];
        if (skill_data.GetCurSkillInfo() == NULL)
        {
            continue;
        }
        
        // Stat Level
        if( CControlWZ* control = GetControlByPosition(skill_data.GetSlotPosition() + kSlot_Slot_Begin) )
        {
            RECT rect = control->GetSizeRect();
            int level = skill_data.GetSkillLevel();

            if (skill_data.IsActiveState() == false )
            {
                // 표시안함
                // 0레벨이지만 1레벨로 표시
                //level = 1;
                //g_InterfaceManager.RenderDrawNumber(rect, level, DT_LEFT | DT_TOP);
            }
            else
            {
                if( level > 0 )
                {
                    // 10 미만일떄 녹색, 10일떄 흰색
                    g_InterfaceManager.RenderDrawNumber(rect, level, DT_RIGHT | DT_BOTTOM, level < 10 ? eIMAGE_NUMBER_GREEN : eIMAGE_NUMBER_WHITE );    
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/** 텍스트(숙련1, 숙련2)
*/
void uiSkillTree::RefreshControlText()
{
    static const int string_list[kMaxClass][2] = 
    {
        {eST_BUTTON_TEXT_MASTERY1_BERSERKER, eST_BUTTON_TEXT_MASTERY2_BERSERKER},
        {eST_BUTTON_TEXT_MASTERY1_DRAGON, eST_BUTTON_TEXT_MASTERY2_DRAGON},
        {eST_BUTTON_TEXT_MASTERY1_SHADOW, eST_BUTTON_TEXT_MASTERY2_SHADOW},
        {eST_BUTTON_TEXT_MASTERY1_VALKIRYE, eST_BUTTON_TEXT_MASTERY2_VALKIRYE},
		{eST_BUTTON_TEXT_MASTERY1_ELEMENTAL, eST_BUTTON_TEXT_MASTERY2_ELEMENTAL},
		{eST_BUTTON_TEXT_MASTERY1_MYSTIC, eST_BUTTON_TEXT_MASTERY2_MYSTIC},
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        {eST_BUTTON_TEXT_MASTERY1_HELROID, eST_BUTTON_TEXT_MASTERY2_HELROID},
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {eST_BUTTON_TEXT_MASTERY1_WITCHBLADE, eST_BUTTON_TEXT_MASTERY2_WITCHBLADE},
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    // 숙련1
    int hero_index = GetHeroIndex();
    TCHAR text[INTERFACE_STRING_LENGTH];

    if( CControlWZ* control = GetControlByPosition(kTextExt1) )
    {
        int string_code = string_list[hero_index][0];
        g_InterfaceManager.GetInterfaceString(string_code, text, INTERFACE_STRING_LENGTH);

        control->SetTextWZ(text);
    }

    // 숙련2
    if( CControlWZ* control = GetControlByPosition(kTextExt2) )
    {
        int string_code = string_list[hero_index][1];
        g_InterfaceManager.GetInterfaceString(string_code, text, INTERFACE_STRING_LENGTH);

        control->SetTextWZ(text);
    }
}

//------------------------------------------------------------------------------
/** 탭버튼
*/
void uiSkillTree::RefreshTab()
{
    // Tab
    CCtrlButtonCheckWZ* button = NULL;
    for (int idx = 0; idx < kMaxTab; ++idx)
    {
        button = static_cast<CCtrlButtonCheckWZ*>(GetControlByPosition(idx + kTabBein));
        button->SetCheckState((idx == 0) ? (true) : (false));
    }
}
//------------------------------------------------------------------------------
/** 뒷배경
*/
void uiSkillTree::RefreshBackGround()
{
    // Back
    for (int idx = 0; idx < kMaxClass; ++idx)
    {
        if (GetControlByPosition(idx + kImgBegin) != NULL)
        {
            GetControlByPosition(idx + kImgBegin)->ShowWindowWZ(WZ_HIDE);
        }
    }

    if (GetControlByPosition(GetHeroIndex() + kImgBegin) != NULL)
    {
        GetControlByPosition(GetHeroIndex() + kImgBegin)->ShowWindowWZ(WZ_SHOW);
    }
}
//------------------------------------------------------------------------------
/**  아이콘 그리기
*/
void uiSkillTree::RefreshSkillIcon()
{
    GetItemUnitRender()->FlushUnitDraw();

    int max_slot = skill_slot_container_.GetMaxSlotNum();
    for( int idx = 0; idx < max_slot; ++idx)
    {
        if (skill_slot_container_.IsEmpty(idx) == FALSE)
        {
            SCSlot& slot = skill_slot_container_.GetSlot(idx);
            if (GetItemUnitRender()->GetItemUnit(slot.GetSerial()) == false)
            {
                if (GetControlByPosition(kSlot_Slot_Begin+idx) != NULL)
                {
                    GetItemUnitRender()->AddItemUnit(slot.GetSerial(), slot.GetCode(),
                        &GetControlByPosition(kSlot_Slot_Begin+idx)->GetSizeRect(), &slot, 
                        GetControlByPosition(kSlot_Slot_Begin+idx)); 
                }
            }
        }
    }
}


void uiSkillTree::StartAnimation( int old_left_level, int left_level, int old_right_level, int right_level )
{
    bool is_changed_left = old_left_level != left_level && old_left_level < kMaxRow -1;
    bool is_changed_right = old_right_level != right_level && old_right_level < kMaxRow -1;

    if( is_changed_left == false && 
        is_changed_right == false )
        return;

    old_left_level_ = old_left_level;
    old_right_level_ = old_right_level;

    is_animate_ = 1;
    animate_start_time_ = GetCurrentTime();

    // 효과음 재생으 시작한다
    if( sound_handle_ != 0 )
    {
        g_SoundEffect.StopUI( sound_handle_ );
    }
    sound_handle_ = g_SoundEffect.PlayUI( UI_SOUND_SKILLTREE_BLUEEFFECT );
}

#endif//_NA_0_20120321_SKILLTREE_SECOND_RENEWAL