#include "SunClientPrecompiledHeader.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "uiCharaterAccumulatePointDialog/uiCharaterAccumulatePointDialog.h"
#include "uiSystemMan/uiSystemMan.h"
#include "InterfaceManager.h"
#include "ItemUnitRenderer.h"
#include "SCItemSlot.h"
#include "SlotKeyGenerator.h"
#include "Mouse.h"
#include "Hero.h"
#include "ItemManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Application.h"
#include "SoundEffect.h"
#include "GlobalFunc.h"

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD 
#include "GameFunc.h"
#include "ResourceManager.h"
#endif

const DWORD kLastStep = 5;
const DWORD kTimeForBlink = 166;
const DWORD KInitTimeForMove = 80;
const DWORD KMaxTimeForMove = 800;
const DWORD KGapTimeForMove = KMaxTimeForMove - KInitTimeForMove;
const DWORD kVelocityStepTime1 = 3000;
const DWORD KCloseTimeAfterOrder = 2500;
const DWORD KDisplayQuestionTime = 800;
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD 
const int KMaxRepeatCount = 30;
#else
const int KMaxRepeatCount = 18;
#endif
const DWORD KRewardMessageHeight = 142;

WzID uiCharaterAccumulatePointDialog::controls_id_[DIALOG_POS_MAX] = 
{
    StrToWzID("BT18"),//ACCUMULATEPOINT_REWARD_BUTTON,
    StrToWzID("ST00"),//ACCUMULATEPOINT_COIN_TEXT,
    StrToWzID("B999"),//ACCUMULATEPOINT_CLOSE_BUTTON,

    StrToWzID("BT01"),//ACCUMULATEPOINT_INCREASE_BUTTON,    //<- 반복횟수 증가버튼
    StrToWzID("BT02"),//ACCUMULATEPOINT_DECREASE_BUTTON,    //<- 반복횟수 감소버튼
    StrToWzID("ST01"),//ACCUMULATEPOINT_REPEAT_TEXT,        //<- 반복횟수

    // ItemSlot Render관련
    StrToWzID("BT00"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT0,  //<- 선택된 아이템 슬롯
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD 
    StrToWzID("B001"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT1,  //<- 30개의 세팅된 아이템슬롯i
    StrToWzID("B002"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT2,
    StrToWzID("B003"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT3,
    StrToWzID("B004"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT4,
    StrToWzID("B005"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT5,
    StrToWzID("B018"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT6,
    StrToWzID("B000"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT7,
    StrToWzID("B019"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT8,
    StrToWzID("B020"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT9,
    StrToWzID("B006"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT10,
    StrToWzID("B017"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT11,
    StrToWzID("B021"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT12,
    StrToWzID("B022"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT13,
    StrToWzID("B023"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT14,
    StrToWzID("B007"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT15,
    StrToWzID("B016"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT16,
    StrToWzID("B027"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT17,
    StrToWzID("B028"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT18
    StrToWzID("B029"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT1,
    StrToWzID("B008"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT2,
    StrToWzID("B015"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT3,
    StrToWzID("B024"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT4,
    StrToWzID("B025"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT5,
    StrToWzID("B026"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT6,
    StrToWzID("B009"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT7,
    StrToWzID("B014"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT8,
    StrToWzID("B013"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT9,
    StrToWzID("B012"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT10,
    StrToWzID("B011"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT11,
    StrToWzID("B010"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT12,

    // 보상연출 관련
    StrToWzID("PI18"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC1,
    StrToWzID("PI19"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
    StrToWzID("PI38"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
    StrToWzID("PI39"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
    StrToWzID("PI58"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
    StrToWzID("PI71"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
    StrToWzID("P019"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
    StrToWzID("P020"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
    StrToWzID("P022"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
    StrToWzID("PI59"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
    StrToWzID("PI70"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
    StrToWzID("P023"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC12,
    StrToWzID("P024"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC13,
    StrToWzID("P025"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC14,
    StrToWzID("PI60"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC15,
    StrToWzID("PI69"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC16,
    StrToWzID("P026"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC17,
    StrToWzID("P027"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC18,
    StrToWzID("P028"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC1,
    StrToWzID("PI61"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
    StrToWzID("PI68"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
    StrToWzID("P029"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
    StrToWzID("P030"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
    StrToWzID("P031"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
    StrToWzID("PI62"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
    StrToWzID("PI67"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
    StrToWzID("PI66"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
    StrToWzID("PI65"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
    StrToWzID("PI64"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
    StrToWzID("PI63"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC12,

    // 보상연출 관련
    StrToWzID("P032"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC1,
    StrToWzID("P096"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
    StrToWzID("P097"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
    StrToWzID("P098"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
    StrToWzID("P099"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
    StrToWzID("P100"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
    StrToWzID("P101"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
    StrToWzID("P102"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
    StrToWzID("P103"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
    StrToWzID("P104"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
    StrToWzID("P105"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
    StrToWzID("P106"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC12,
    StrToWzID("P107"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC13,
    StrToWzID("P108"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC14,
    StrToWzID("P109"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC15,
    StrToWzID("P110"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC16,
    StrToWzID("P111"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC17,
    StrToWzID("P112"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC18,
    StrToWzID("P113"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC1,
    StrToWzID("P114"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
    StrToWzID("P115"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
    StrToWzID("P116"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
    StrToWzID("P117"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
    StrToWzID("P118"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
    StrToWzID("P119"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
    StrToWzID("P120"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
    StrToWzID("P121"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
    StrToWzID("P122"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
    StrToWzID("P123"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
    StrToWzID("P124"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC12,
#else
	StrToWzID("B001"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT1,  //<- 18개의 세팅된 아이템슬롯i
    StrToWzID("B002"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT2,
    StrToWzID("B003"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT3,
    StrToWzID("B004"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT4,
    StrToWzID("B005"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT5,
    StrToWzID("B006"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT6,
    StrToWzID("B007"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT7,
    StrToWzID("B008"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT8,
    StrToWzID("B009"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT9,
    StrToWzID("B010"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT10,
    StrToWzID("B011"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT11,
    StrToWzID("B012"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT12,
    StrToWzID("B013"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT13,
    StrToWzID("B014"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT14,
    StrToWzID("B015"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT15,
    StrToWzID("B016"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT16,
    StrToWzID("B017"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT17,
    StrToWzID("B018"),//ACCUMULATEPOINT_REWARD_ITEM_SLOT18,

	// 보상연출 관련
    StrToWzID("PI20"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC1,
    StrToWzID("PI21"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
    StrToWzID("PI22"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
    StrToWzID("PI23"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
    StrToWzID("PI24"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
    StrToWzID("PI25"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
    StrToWzID("PI26"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
    StrToWzID("PI27"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
    StrToWzID("PI28"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
    StrToWzID("PI29"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
    StrToWzID("PI30"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
    StrToWzID("PI31"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC12,
    StrToWzID("PI32"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC13,
    StrToWzID("PI33"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC14,
    StrToWzID("PI34"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC15,
    StrToWzID("PI35"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC16,
    StrToWzID("PI36"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC17,
    StrToWzID("PI37"),//ACCUMULATEPOINT_SELECT1_ITEM_PIC18,

    StrToWzID("PI40"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC1,
    StrToWzID("PI41"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC2,
    StrToWzID("PI42"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC3,
    StrToWzID("PI43"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC4,
    StrToWzID("PI44"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC5,
    StrToWzID("PI45"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC6,
    StrToWzID("PI46"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC7,
    StrToWzID("PI47"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC8,
    StrToWzID("PI48"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC9,
    StrToWzID("PI49"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC10,
    StrToWzID("PI50"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC11,
    StrToWzID("PI51"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC12,
    StrToWzID("PI52"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC13,
    StrToWzID("PI53"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC14,
    StrToWzID("PI54"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC15,
    StrToWzID("PI55"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC16,
    StrToWzID("PI56"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC17,
    StrToWzID("PI57"),//ACCUMULATEPOINT_SELECT2_ITEM_PIC18,

    StrToWzID("PI18"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC1,
    StrToWzID("PI19"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC2,
    StrToWzID("PI38"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC3,
    StrToWzID("PI39"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC4,
    StrToWzID("PI58"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC5,
    StrToWzID("PI59"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC6,
    StrToWzID("PI60"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC7,
    StrToWzID("PI61"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC8,
    StrToWzID("PI62"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC9,
    StrToWzID("PI63"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC10,
    StrToWzID("PI64"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC11,
    StrToWzID("PI65"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC12,
    StrToWzID("PI66"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC13,
    StrToWzID("PI67"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC14,
    StrToWzID("PI68"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC15,
    StrToWzID("PI69"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC16,
    StrToWzID("PI70"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC17,
    StrToWzID("PI71"),//ACCUMULATEPOINT_QUESTION_ITEM_PIC18,
#endif 
};


uiCharaterAccumulatePointDialog::uiCharaterAccumulatePointDialog(InterfaceManager* pUIMan)
:uiBase(pUIMan),
reward_item_info_(NULL),
reward_message_manager_(KRewardMessageHeight, StrToWzID("mi17")),
item_unit_renderer_(NULL),
reward_button_(NULL),
coin_text_(NULL),
reward_slot_button_(NULL),
reward_step_(NONE_STEP),
confirm_dialog_key_(0),
tooltip_position_(0),
is_initialize(false)
{    
    SCItemSlotContainer::Init(SLOT_TOTAL_COUNT, 0);
}

uiCharaterAccumulatePointDialog::~uiCharaterAccumulatePointDialog()
{
    Release();
    SAFE_DELETE(reward_item_info_);
}

void uiCharaterAccumulatePointDialog::Init( CDrawBase * pDrawBase )
{
    uiBase::Init(pDrawBase);

    reward_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_REWARD_BUTTON]));
    SUN_ASSERT(reward_button_);
    coin_text_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_COIN_TEXT]));
    SUN_ASSERT(coin_text_);
    reward_slot_button_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_REWARD_ITEM_SLOT0]));
    SUN_ASSERT(reward_slot_button_);
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD     
    for (int index = 0; index < IMAGE_SLOT_COUNTER; ++index)
    {
        blink_pictures_[index] = static_cast<CCtrlPictureWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_SELECT1_ITEM_PIC1+index]));
        SUN_ASSERT(blink_pictures_[index]);
        blink_pictures_[index+IMAGE_SLOT_COUNTER] = 
            static_cast<CCtrlPictureWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_SELECT2_ITEM_PIC1+index]));
        SUN_ASSERT(blink_pictures_[index+REWARD_ITEM_GROUP_COUNT]);
    }
#else
    for (int index = 0; index < REWARD_ITEM_GROUP_COUNT; ++index)
    {
        blink_pictures_[index] = static_cast<CCtrlPictureWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_SELECT1_ITEM_PIC1+index]));
        SUN_ASSERT(blink_pictures_[index]);
        blink_pictures_[index+REWARD_ITEM_GROUP_COUNT] = 
            static_cast<CCtrlPictureWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_SELECT2_ITEM_PIC1+index]));
        SUN_ASSERT(blink_pictures_[index+REWARD_ITEM_GROUP_COUNT]);
        blink_pictures_[index+(REWARD_ITEM_GROUP_COUNT * 2)] = 
            static_cast<CCtrlPictureWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_QUESTION_ITEM_PIC1+index]));
        SUN_ASSERT(blink_pictures_[index+(REWARD_ITEM_GROUP_COUNT * 2)]);
    }
#endif
    repeat_button_[0] = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_INCREASE_BUTTON]));
    SUN_ASSERT(repeat_button_[0]);
    repeat_button_[1] = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_DECREASE_BUTTON]));
    SUN_ASSERT(repeat_button_[1]);
    repeat_text_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_REPEAT_TEXT]));
    SUN_ASSERT(repeat_text_);

    item_unit_renderer_ = new ItemUnitRenderer;
    SUN_ASSERT(item_unit_renderer_);
    if (item_unit_renderer_)
    {
        item_unit_renderer_->Init(GetDrawBase(), SLOT_TOTAL_COUNT);
        item_unit_renderer_->SetOverLap(FALSE);
    }
    display_timer_.Clear();
    reward_item_info_ = NULL;
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD    
    render_slot_icon_vector_.clear();
    Render_slot_Ctrl_.clear();

    list_refresh_timer_.Off();
    list_refresh_timer_.On(1);

    list_refresh_timer_2.Off();
    list_refresh_timer_2.On(1);
#endif
    is_initialize = true;

    is_close_reserve_ = false;
}

void uiCharaterAccumulatePointDialog::Release()
{
    render_slot_icon_vector_.clear();
    SAFE_RELEASENDELETE(item_unit_renderer_);
}

void uiCharaterAccumulatePointDialog::OnShowWindow(BOOL val)
{
    if (val)
    {
        //MoveDefaultWindowPos();

        reward_sound_handle_ = 0;
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD    
#else
        HideAllBlinkPictureControls();
#endif
        g_InterfaceManager.LockDlgs(IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG);

        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_Accumulate_Dialog;
        msg.wParam = IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG;
        msg.DoSomething = uiCharaterAccumulatePointDialog::ESCProcess;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        ClearItemContainer();
        g_InterfaceManager.UnLockDlgs();

        if (reward_item_info_)
        {
            ItemManager::Instance()->InsertInventoryTotalInfo(POINT_REWARD, (*reward_item_info_));
            SAFE_DELETE(reward_item_info_);
        }
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD    
        Render_slot_Ctrl_.clear();
        render_slot_icon_vector_.clear();
		save_repect_count_ = 0;
#endif
        repeat_count_ = 1;
        
        reward_step_ = NONE_STEP;

        is_close_reserve_ = false;
        g_KeyQueueManager.DeleteMsg(eDoType_Accumulate_Dialog);
    }
}

void uiCharaterAccumulatePointDialog::Process(DWORD dwTick)
{
    reward_message_manager_.Process(dwTick);
}

void uiCharaterAccumulatePointDialog::OnUpdateSolarDialog()
{
    if ( ( reward_step_ == NONE_STEP || reward_step_ == WAIT_STEP
        || reward_step_ == PREPARE_STEP )
        && is_close_reserve_ == true )
    {
        ShowInterface(FALSE);
    }
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD   
    for (int index = 0; index < IMAGE_SLOT_COUNTER; ++index)
    {
         blink_pictures_[index+IMAGE_SLOT_COUNTER]->ShowWindowWZ(WZ_HIDE);
    }
#endif

   uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
    if (ui_manager)
    {
        DWORD accumulate_point = ui_manager->accumulate_point();
        TCHAR point_text[INTERFACE_STRING_LENGTH] = {0, };
        Snprintf(point_text, INTERFACE_STRING_LENGTH, _T("%d"), accumulate_point);
        coin_text_->SetTextWZ(point_text);

        Snprintf(point_text, INTERFACE_STRING_LENGTH, _T("%d"), repeat_count_);
        repeat_text_->SetTextWZ(point_text);
    }
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD   
    for (int i = 0 ; i < repeat_count_ ; ++i)
    {
        blink_pictures_[i+IMAGE_SLOT_COUNTER]->ShowWindowWZ(WZ_SHOW);
    }
#endif

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD   
#else
 
    if (reward_step_ == DISPLAY_REWARD_STEP)
    {
        DWORD progress_time = display_timer_.GetProcessTime();
        DWORD step_move = 0;
        int sound_index = 0;

        time_for_move_ = KInitTimeForMove;

        if (progress_time > kVelocityStepTime1)
        {
            if (remain_step_ == -1)
            {
                int remain_step = static_cast<int>(order_item_position_) - static_cast<int>(display_item_position_ % REWARD_ITEM_GROUP_COUNT);
                remain_step += REWARD_ITEM_GROUP_COUNT;
                remain_step %= REWARD_ITEM_GROUP_COUNT;                
                if (remain_step == kLastStep + last_step_)
                {
                    remain_step_ = kLastStep + last_step_;
                }
            }
            if (remain_step_ > -1)
            {
                float progress_ratio = static_cast<float>(remain_step_) / (kLastStep + last_step_);
                progress_ratio = 1.0f - progress_ratio;
                time_for_move_ += static_cast<DWORD>(progress_ratio * progress_ratio * (KGapTimeForMove));
            }            
        }

        step_move = (progress_time - previous_step_time_) / time_for_move_;

        if (remain_step_ == 0)
        {
            reward_step_ = ORDER_REWARD_STEP;
            display_timer_.Clear();
            display_timer_.SetTimer(KCloseTimeAfterOrder);
            g_SoundEffect.Play(STOP_SOUND_INDEX, Sound::Instance()->GetEffectVolume());
            wzsnd_stop(reward_sound_handle_);
            reward_sound_handle_ = 0;

            if (reward_item_info_)
            {
                ItemManager::Instance()->InsertInventoryTotalInfo(POINT_REWARD, *reward_item_info_);
                SAFE_DELETE(reward_item_info_);
            }                        
        }
        if (remain_step_ >= 0)
        {
            float volume = static_cast<float>(remain_step_) / (kLastStep + last_step_);
            if (volume > 1.0f)
            {
                volume = 1.0f;
            }
            wzsnd_set_volume(reward_sound_handle_, volume * Sound::Instance()->GetEffectVolume());
        }
        
        DWORD blink_position = display_item_position_ % 18;
        blink_pictures_[blink_position]->ShowWindowWZ(WZ_HIDE);
        blink_pictures_[blink_position+REWARD_ITEM_GROUP_COUNT]->ShowWindowWZ(WZ_HIDE);
        if (step_move > 0)
        {
            previous_step_time_ += (step_move * time_for_move_);
            display_item_position_ += step_move;
            if (time_for_move_ <= KInitTimeForMove)
            {
                sound_index = EFFECT_SOUND1_INDEX;
            }
            else if (time_for_move_ <= KInitTimeForMove + (KGapTimeForMove / 2))
            {
                sound_index = EFFECT_SOUND2_INDEX;
            }
            else if (time_for_move_ > KInitTimeForMove + (KGapTimeForMove / 2))
            {
                sound_index = EFFECT_SOUND3_INDEX;
            }
            g_SoundEffect.Play(sound_index, Sound::Instance()->GetEffectVolume());
            
            if (remain_step_ > 0)
            {                
                remain_step_ -= step_move;
                if (remain_step_ < 0)
                {
                    remain_step_ = 0;
                }
            }            
        }
        blink_position = display_item_position_ % 18;            
        if ((progress_time / kTimeForBlink) % 2)
        {
            blink_pictures_[blink_position]->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            blink_pictures_[blink_position+REWARD_ITEM_GROUP_COUNT]->ShowWindowWZ(WZ_SHOW);
        }
    }

    if (reward_step_ == ORDER_REWARD_STEP)
    {
        // 아이템 결정 연출
        DWORD progress_time = display_timer_.GetProcessTime();
        DWORD blink_position = display_item_position_ % 18;            
        if ((progress_time / kTimeForBlink) % 2)
        {
            blink_pictures_[blink_position]->ShowWindowWZ(WZ_SHOW);
            blink_pictures_[blink_position+REWARD_ITEM_GROUP_COUNT]->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            blink_pictures_[blink_position]->ShowWindowWZ(WZ_HIDE);
            blink_pictures_[blink_position+REWARD_ITEM_GROUP_COUNT]->ShowWindowWZ(WZ_SHOW);
        }

        if (display_timer_.IsExpired() == TRUE)
        {
            if (repeat_count_ > 0)
            {
                repeat_count_--;
                reward_step_ = REPEAT_PREPARE_STEP;
                display_timer_.Clear();
                display_timer_.SetTimer(KDisplayQuestionTime);
                HideAllBlinkPictureControls();

                // Question 표시            
                for (int index = 0; index < REWARD_ITEM_GROUP_COUNT; ++index)
                {                
                    blink_pictures_[index+(REWARD_ITEM_GROUP_COUNT * 2)]->ShowWindowWZ(WZ_SHOW);
                }
            }
#ifdef _DEV_VER
            GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "AccumulatePoint OrderItemPosition(%d)", display_item_position_%18);
#endif
        }        
    }

    // 반복 연출
    if (reward_step_ == REPEAT_PREPARE_STEP)
    {
        if (display_timer_.IsExpired() == TRUE)
        {
            display_timer_.Clear();
            uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
            if (ui_manager)
            {
                ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(false);

                if (repeat_count_ == 0)
                {
                    DWORD repeat_count = min(1, ui_manager->accumulate_point());
                    repeat_count_ = max(0, repeat_count);

                    reward_step_ = WAIT_STEP;
                    reward_button_->EnableWindowWZ(true);
                    repeat_button_[0]->EnableWindowWZ(true);
                    repeat_button_[1]->EnableWindowWZ(true);
                }
            }

        }
    }

    if (confirm_dialog_key_)
    {
        uiBase* confirm_dialog = GET_DIALOG(confirm_dialog_key_);
        if (confirm_dialog == NULL)
        {
            confirm_dialog_key_ = 0;
            g_InterfaceManager.LockDlgs(IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG);            
        }
    }

    if (g_Input.GetState(DIK_ESCAPE, KS_UP, TRUE) || g_Input.GetState(DIK_ESCAPE, IKS_UP, TRUE))
    {
        if (IsVisible() == TRUE)    
        {
            if ((reward_step_ == NONE_STEP)
                || (reward_step_ == WAIT_STEP)
                || (reward_step_ == PREPARE_STEP))
            {
                ShowInterface(FALSE);
            }
        }
    }    
#endif
}

void uiCharaterAccumulatePointDialog::OnRenderSolarDialog()
{
	RECT rc;
    POINT pt = g_pApplication->Get2DModeMapping();
    SetRect(&rc, 0, 0, pt.x, pt.y);
    g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0, 0, static_cast<float>(rc.right), static_cast<float>(rc.bottom));
    GetDialogWZ()->OnDraw(true);

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD   
    if(reward_step_ != REPEAT_PREPARE_STEP)
    {
        if ((reward_step_ == DISPLAY_REWARD_STEP) || (reward_step_ == ORDER_REWARD_STEP))
        {
            static int Next_Item = 0;

            CCtrlButtonWZ* control_button = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_REWARD_ITEM_SLOT1 +  save_repect_count_]));
            uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());

            DWORD time = list_refresh_timer_.SpentTime();
            if(time > 40)
            {
                Next_Item++;

                if(Next_Item >= MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK::ACCUMULATE_POINT_ITEM_COUNT)
                    Next_Item = 0;

                 Image_list_[Next_Item]->Render(&control_button->GetSizeRect());
                list_refresh_timer_.Off();
                list_refresh_timer_.On(1);
            }

            time = list_refresh_timer_2.SpentTime();
            if(time > 500)
            {
                reward_step_ = REPEAT_PREPARE_STEP;
                
                Next_Item = 0;

                if (reward_item_info_)
                {
                    ItemManager::Instance()->InsertInventoryTotalInfo(POINT_REWARD, *reward_item_info_);
                    SAFE_DELETE(reward_item_info_);
                }  

                Render_slot_Ctrl_.push_back(ACCUMULATEPOINT_REWARD_ITEM_SLOT1 +  save_repect_count_);
                SCItemSlot& item_slot = (SCItemSlot&)GetSlot(order_item_position_);
                SCItemSlot temp_slot;
                memcpy(&temp_slot, &item_slot, sizeof(item_slot));                
                render_slot_icon_vector_.push_back(temp_slot);

                if ( is_close_reserve_ == true )
                {
                    is_close_reserve_ = false;                    
                    repeat_count_ = 1;                    
                }

                save_repect_count_++;

                if(repeat_count_ > save_repect_count_)
                    ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(false);

                list_refresh_timer_2.Off();
                list_refresh_timer_2.On(1);
            }

            if(save_repect_count_ >= repeat_count_)
            {
                g_SoundEffect.StopUI(reward_sound_handle_);
                reward_sound_handle_ = g_SoundEffect.PlayUI(EFFECT_SOUND_NEW2);

                ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(false);

                DWORD repeat_count = min(1, ui_manager->accumulate_point());
                repeat_count_ = max(0, repeat_count);

                reward_step_ = WAIT_STEP;
                reward_button_->EnableWindowWZ(true);
                repeat_button_[0]->EnableWindowWZ(true);
                repeat_button_[1]->EnableWindowWZ(true);
            }
        }   
    }

    for (int i = 0 ; i < (int)Render_slot_Ctrl_.size() ; ++i)
    {
        SCItemSlot& item_slot = render_slot_icon_vector_[i];

        CODETYPE image_code = GameFunc::GetImagecode(&item_slot, item_slot.GetCode());
        BOOL is_limited = GameFunc::IsLimitedItem(&item_slot, item_slot.GetCode());
        RESOURCECODE resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
        IconImage* SlotIconImage = ICONMGR()->Add(resourceCode);

        CCtrlButtonWZ* control_button = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[Render_slot_Ctrl_[i]]));

        SlotIconImage->Render(&control_button->GetSizeRect());

        uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
        if (tooltip_manager)
        {
            int iMouseX = Mouse::Instance()->GetMouseX();
            int iMouseY = Mouse::Instance()->GetMouseY();

            RECT rc = control_button->GetSizeRect();

            if (rc.left <= iMouseX && rc.right >= iMouseX)
            {
                if (rc.top <= iMouseY && rc.bottom >= iMouseY)
                {
                    
                    tooltip_manager->RegisterTooltipItem(&item_slot, FALSE);
                    tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
        }
    }

   reward_message_manager_.Render();
#else
    
    POSTYPE item_position = static_cast<POSTYPE>(display_item_position_ % 18);
    if (item_unit_renderer_
        && reward_step_ != REPEAT_PREPARE_STEP)
    {
        item_unit_renderer_->Render();

        if ((reward_step_ == DISPLAY_REWARD_STEP) || (reward_step_ == ORDER_REWARD_STEP))
        {
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(item_position));
            ItemUnitRenderer::RenderUnit* render_unit = item_unit_renderer_->GetItemUnit(item_slot.GetSerial());
            RECT control_rect = reward_slot_button_->GetSizeRect();
            render_unit->m_pIconImage->Render(&control_rect);            
        }
    }

	reward_message_manager_.Render();

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager)
    {
        int iMouseX = Mouse::Instance()->GetMouseX();
        int iMouseY = Mouse::Instance()->GetMouseY();

        POSTYPE slot_pos = ACCUMULATEPOINT_REWARD_ITEM_SLOT0;
        for( ; slot_pos <= ACCUMULATEPOINT_REWARD_ITEM_SLOT18; ++slot_pos )
        {
            CControlWZ* ctrl_ptr = GetControlWZ(controls_id_[slot_pos]);
            RECT rc = ctrl_ptr->GetSizeRect();

            if (rc.left <= iMouseX && rc.right >= iMouseX)
            {
                if (rc.top <= iMouseY && rc.bottom >= iMouseY)
                {
                    if (slot_pos == ACCUMULATEPOINT_REWARD_ITEM_SLOT0)
                    {
                        if (reward_step_ == ORDER_REWARD_STEP)
                        {
                            SCSlot& item_slot = GetSlot(item_position);
                            tooltip_manager->RegisterTooltipItem(&item_slot, FALSE);
							tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
                    }
                    else 
                    {
                        if ((reward_step_ == PREPARE_STEP) ||
                            (reward_step_ == DISPLAY_REWARD_STEP) ||
                            (reward_step_ == ORDER_REWARD_STEP))
                        {
                            POSTYPE item_pos = static_cast<POSTYPE>((slot_pos - ACCUMULATEPOINT_REWARD_ITEM_SLOT1) % 18);
                            SCSlot& item_slot = GetSlot(item_pos);
                            tooltip_manager->RegisterTooltipItem(&item_slot, FALSE);
							tooltip_manager->SetTooltipDialogID(GetDialogKey());
                        }
                    }
                }
            }
        }
    }
    
#endif
}

void uiCharaterAccumulatePointDialog::MessageProc( SI_MESSAGE * pMessage )
{
    POSTYPE control_position = 0xFF;
    tooltip_position_ = 0;
    for( POSTYPE index = 0 ; index < DIALOG_POS_MAX; ++index )
    {
        if( controls_id_[index] == pMessage->dwCtrlID )
        {
            control_position =  index;
            break;
        }
    }
    if (control_position == 0xFF)
    {
        return;
    }

    switch (pMessage->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch(control_position)
            {
            case ACCUMULATEPOINT_REWARD_BUTTON:
                {
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
                    Render_slot_Ctrl_.clear();
                    render_slot_icon_vector_.clear();
                    save_repect_count_ = 0;
                    
#endif

                    ClickRewardButton();
                }                
                break;
            case ACCUMULATEPOINT_CLOSE_BUTTON:
                {
                    if ((reward_step_ == NONE_STEP)
                        || (reward_step_ == WAIT_STEP)
                        || (reward_step_ == PREPARE_STEP))
                    {
                        ShowInterface(FALSE);
                    }                    
                }
                break;
            case ACCUMULATEPOINT_INCREASE_BUTTON:
                {
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
                    Render_slot_Ctrl_.clear();
                    render_slot_icon_vector_.clear();
#endif
                    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
                    if (ui_manager)
                    {
                        int accumulate_point = static_cast<int>(ui_manager->accumulate_point());
                        repeat_count_++;
                        if ((repeat_count_ > KMaxRepeatCount)
                            || (repeat_count_ > accumulate_point))
                        {
                            repeat_count_ = min(KMaxRepeatCount, accumulate_point);
                        }  
                    }
                }
                break;
            case ACCUMULATEPOINT_DECREASE_BUTTON:
                {
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
                    Render_slot_Ctrl_.clear();
                    render_slot_icon_vector_.clear();
#endif
                    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
                    if (ui_manager)
                    {
                        int accumulate_point = static_cast<int>(ui_manager->accumulate_point());
                        repeat_count_--;
                        if (repeat_count_ < 1)
                        {
                            repeat_count_ = min(KMaxRepeatCount, accumulate_point);
                        }                        
                    }
                }
                break;
            }
        }
        break;

    case RT_MSG_MOUSEOVER:
        {
        }
        break;
    default:
        {
            SolarDialog::MessageProc(pMessage);
        }
        break;
    }
}

void uiCharaterAccumulatePointDialog::ClearItemContainer()
{
    DWORD item_serial = 0;
    for (POSTYPE index = 0; index < GetMaxSlotNum(); ++index)
    {
        SCItemSlot& item_slot = (SCItemSlot&)GetSlot(index);
        item_serial = (DWORD)item_slot.GetSerial();
        if (item_serial != 0)
        {
            g_SlotKeyGenerator.Restore(item_serial);
            if (item_unit_renderer_)
            {
                item_unit_renderer_->RemoveItemUnit(item_serial);
            }            
        }
    }
    ClearAll();
}

void uiCharaterAccumulatePointDialog::PrepareRewardItemGroup(SLOTCODE* itemcode_array)
{
    ClearItemContainer();

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
    itemcode_array_ = itemcode_array;
	Image_list_.clear();
#endif


    SCItemSlot temp_slot;
    DWORD item_serial;
    CCtrlButtonWZ* control_button;
    RECT control_rect;
    for (POSTYPE index = 0; index < MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK::ACCUMULATE_POINT_ITEM_COUNT; ++index)
    {
        control_button = static_cast<CCtrlButtonWZ*>(GetControlWZ(controls_id_[ACCUMULATEPOINT_REWARD_ITEM_SLOT1+index]));
        SUN_ASSERT(control_button);
        if (control_button)
        {
            temp_slot.SetCode(itemcode_array[index]);
            item_serial = g_SlotKeyGenerator.GetKey();
            temp_slot.SetSerial(item_serial);

            control_rect = control_button->GetSizeRect();

            SCSlotContainer::InsertSlot(index, temp_slot);
            SCItemSlot& item_slot = (SCItemSlot&)GetSlot(index);
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
            CODETYPE image_code = GameFunc::GetImagecode(&item_slot, item_slot.GetCode());
            BOOL is_limited = GameFunc::IsLimitedItem(&item_slot, item_slot.GetCode());
            RESOURCECODE resourceCode = ResourceManager::Instance()->GetItemSlotImageResourceCode(image_code, is_limited);
            IconImage* SlotIconImage = ICONMGR()->Add(resourceCode);
            Image_list_.push_back(SlotIconImage);
#else
			if (item_unit_renderer_)
             {
                 item_unit_renderer_->AddItemUnit(item_slot.GetSerial(), item_slot.GetCode(), &control_rect, &item_slot, control_button);
             }     
#endif
                    
        }
    }

    if (reward_step_ == NONE_STEP
        || reward_step_ == WAIT_STEP)
    {        
        reward_button_->EnableWindowWZ(true);
        repeat_button_[0]->EnableWindowWZ(true);
        repeat_button_[1]->EnableWindowWZ(true);
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
#else  
        for (int index = 0; index < REWARD_ITEM_GROUP_COUNT; ++index)
        {                
            blink_pictures_[index+(REWARD_ITEM_GROUP_COUNT * 2)]->ShowWindowWZ(WZ_HIDE);
        }
#endif
        reward_step_ = PREPARE_STEP;
    }
    else if (reward_step_ == REPEAT_PREPARE_STEP)
    {
        reward_button_->EnableWindowWZ(false);
        repeat_button_[0]->EnableWindowWZ(false);
        repeat_button_[1]->EnableWindowWZ(false);

        
        ClickRewardButton();
        reward_step_ = WAIT_FOR_ITEMGROUP;
    }   
}

void uiCharaterAccumulatePointDialog::ClickRewardButton()
{
    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
    if (ui_manager)
    {
        if ((reward_step_ == NONE_STEP)
            || (reward_step_ == WAIT_STEP))
        {
            ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(false);
        }
        else if ((reward_step_ == PREPARE_STEP)
                 || (reward_step_ == REPEAT_PREPARE_STEP))
        {
            repeat_button_[0]->EnableWindowWZ(false);
            repeat_button_[1]->EnableWindowWZ(false);

            ui_manager->SEND_MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN();
            reward_step_ = WAIT_FOR_ITEMGROUP;
        }  
    } 
}

void uiCharaterAccumulatePointDialog::DisplayRewardItem(MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK* recv_msg)
{
    if (NULL == recv_msg)
    {
        SUN_ASSERT(recv_msg);
        return;
    }
    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
    if (ui_manager)
    {
        ui_manager->set_accumulate_point(recv_msg->m_wAccumulatePoint);
    }
    SAFE_DELETE(reward_item_info_);
    reward_item_info_ = new INVENTORY_TOTAL_INFO;
    memcpy(reward_item_info_, &(recv_msg->m_ItemInfo), recv_msg->m_ItemInfo.GetSize());

    reward_step_ = DISPLAY_REWARD_STEP;
    order_item_position_ = recv_msg->m_byDropItemOrder;
    display_item_position_ = 0;
    time_for_move_ = KInitTimeForMove;
    remain_step_ = -1;
    last_step_ = kLastStep + GlobalFunc::GetRandom(kLastStep);

    g_SoundEffect.StopUI(reward_sound_handle_);
    reward_sound_handle_ = g_SoundEffect.PlayUI(EFFECT_SOUND_NEW1);

//     reward_sound_handle_ = g_SoundEffect.Play(BGM_INDEX, Sound::Instance()->GetEffectVolume(), TRUE);
//     g_SoundEffect.Play(START_SOUND_INDEX, Sound::Instance()->GetEffectVolume());

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
	list_refresh_timer_2.Off();
    list_refresh_timer_2.On(1);
    list_refresh_timer_.Off();
    list_refresh_timer_.On(1);
#else
	display_timer_.Clear();
    display_timer_.SetTimer(0);
#endif
    previous_step_time_ = 0;

    reward_button_->EnableWindowWZ(false);
#ifdef _DEV_VER
    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK::m_byDropItemOrder(%d)", order_item_position_);
#endif //_DEV_VER
}

void uiCharaterAccumulatePointDialog::HideAllBlinkPictureControls()
{
    for (int index = 0; index < REWARD_ITEM_GROUP_COUNT; ++index)
    {
        blink_pictures_[index]->ShowWindowWZ(WZ_HIDE);
        blink_pictures_[index+REWARD_ITEM_GROUP_COUNT]->ShowWindowWZ(WZ_HIDE);
    }  
}

void uiCharaterAccumulatePointDialog::InserRewardMessage(TCHAR* reward_message)
{
    TEXT_MEMBER_SHARED_PTR_TYPE message_data(new Text_Member(reward_message, WzColor_RGB(0,255,0)));
    reward_message_manager_.Insert(message_data);
}

void uiCharaterAccumulatePointDialog::ProcessErrorCode(DWORD error_code)
{
    switch (error_code)
    {    
    case RC::RC_ACCUMULATE_POINT_FAILED:
    case RC::RC_ACCUMULATE_POINT_NOSPACEININVENTORY:
    case RC::RC_ACCUMULATE_POINT_HAVENOTPOINT:
        {
            if (is_initialize)
            {
                reward_step_ = NONE_STEP;
                reward_button_->EnableWindowWZ(true);
                repeat_button_[0]->EnableWindowWZ(true);
                repeat_button_[1]->EnableWindowWZ(true);
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD  
#else
                HideAllBlinkPictureControls();
                display_timer_.Clear();
                ClearItemContainer();
#endif
                SAFE_DELETE(reward_item_info_);
            }
        }
        break;
    }
}

void uiCharaterAccumulatePointDialog::ESCProcess( DWORD wParam, DWORD lParam )
{
    uiCharaterAccumulatePointDialog* dialog_ptr = 
        GET_CAST_DIALOG(uiCharaterAccumulatePointDialog, IM_CHARACTERSTATUS_MANAGER::CHARACTER_ACCUMULATEPOINT_DLG);
    if (dialog_ptr == NULL)
    {
        return;
    }

    dialog_ptr->set_is_close_reserve_(true);    
}

void uiCharaterAccumulatePointDialog::set_is_close_reserve_( const bool& is_close_reserve )
{
    is_close_reserve_ = is_close_reserve;
}

bool uiCharaterAccumulatePointDialog::get_is_close_reserve_()
{
    return is_close_reserve_;
}

void uiCharaterAccumulatePointDialog::set_repeat_count_( const int& repeat_count )
{
    repeat_count_ = repeat_count;
}