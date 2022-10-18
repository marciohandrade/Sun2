#pragma once
#include "uiBase.h"
#include "SCItemSlotContainer.h"
#include "TextDisplayManager.h"

#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
#define IMAGE_SLOT_COUNTER 30
#endif

class uiCharaterAccumulatePointDialog :   public uiBase,  public SCItemSlotContainer
{
    enum eDIALOG_POS
    {
        ACCUMULATEPOINT_REWARD_BUTTON,
        ACCUMULATEPOINT_COIN_TEXT,
        ACCUMULATEPOINT_CLOSE_BUTTON,

        ACCUMULATEPOINT_INCREASE_BUTTON,    //<- 반복횟수 증가버튼
        ACCUMULATEPOINT_DECREASE_BUTTON,    //<- 반복횟수 감소버튼
        ACCUMULATEPOINT_REPEAT_TEXT,        //<- 반복횟수
        
        // ItemSlot Render관련
        ACCUMULATEPOINT_REWARD_ITEM_SLOT0,  //<- 선택된 아이템 슬롯

        ACCUMULATEPOINT_REWARD_ITEM_SLOT1,  //<- 18개의 세팅된 아이템슬롯
        ACCUMULATEPOINT_REWARD_ITEM_SLOT2,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT3,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT4,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT5,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT6,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT7,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT8,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT9,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT10,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT11,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT12,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT13,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT14,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT15,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT16,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT17,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT18,
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
        ACCUMULATEPOINT_REWARD_ITEM_SLOT19,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT20,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT21,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT22,  //<- 18개의 세팅된 아이템슬롯
        ACCUMULATEPOINT_REWARD_ITEM_SLOT23,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT24,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT25,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT26,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT27,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT28,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT29,
        ACCUMULATEPOINT_REWARD_ITEM_SLOT30,
#endif

        // 보상연출 관련
        ACCUMULATEPOINT_SELECT1_ITEM_PIC1,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC2,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC3,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC4,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC5,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC6,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC7,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC8,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC9,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC10,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC11,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC12,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC13,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC14,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC15,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC16,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC17,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC18,
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
        ACCUMULATEPOINT_SELECT1_ITEM_PIC19,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC20,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC21,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC22,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC23,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC24,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC25,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC26,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC27,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC28,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC29,
        ACCUMULATEPOINT_SELECT1_ITEM_PIC30,
#endif

        ACCUMULATEPOINT_SELECT2_ITEM_PIC1,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC2,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC3,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC4,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC5,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC6,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC7,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC8,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC9,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC10,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC11,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC12,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC13,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC14,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC15,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC16,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC17,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC18,
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
        ACCUMULATEPOINT_SELECT2_ITEM_PIC19,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC20,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC21,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC22,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC23,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC24,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC25,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC26,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC27,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC28,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC29,
        ACCUMULATEPOINT_SELECT2_ITEM_PIC30,
#endif
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD
#else
         ACCUMULATEPOINT_QUESTION_ITEM_PIC1,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC2,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC3,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC4,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC5,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC6,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC7,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC8,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC9,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC10,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC11,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC12,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC13,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC14,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC15,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC16,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC17,
         ACCUMULATEPOINT_QUESTION_ITEM_PIC18,
#endif
        DIALOG_POS_MAX,
    };

    enum
    {
        REWARD_ITEM_COUNT = 1,
        REWARD_ITEM_GROUP_COUNT = MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_ACK::ACCUMULATE_POINT_ITEM_COUNT,

        SLOT_TOTAL_COUNT = REWARD_ITEM_COUNT + REWARD_ITEM_GROUP_COUNT, 
    };
    enum
    {
        BGM_INDEX = 18602,              // 보상받을 때의 배경음악
        START_SOUND_INDEX = 18604,      // 보상받기 연출이 시작될 때의 사운드
        STOP_SOUND_INDEX = 18605,       // 보상이 결정되었을 때의 사운드
        EFFECT_SOUND1_INDEX = 18606,    // 보상연출이 돌아갈때의 사운드 1,2,3
        EFFECT_SOUND2_INDEX = 18607,
        EFFECT_SOUND3_INDEX = 18608,

        EFFECT_SOUND_NEW1 = 562,
        EFFECT_SOUND_NEW2 = 563,
    };

public:
    enum eRewardStep
    {
        NONE_STEP = 0,
        PREPARE_STEP,
        WAIT_FOR_ITEMGROUP,
        DISPLAY_REWARD_STEP,
        ORDER_REWARD_STEP,
        REPEAT_PREPARE_STEP,
        WAIT_STEP,

        STEP_MAX,
    };

public:
    uiCharaterAccumulatePointDialog(InterfaceManager* pUIMan);
    virtual ~uiCharaterAccumulatePointDialog();

    void Init(CDrawBase * pDrawBase);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * pMessage);

    void set_require_point(DWORD require_point)    {   require_point_ = require_point; }

    void ClearItemContainer();
    void PrepareRewardItemGroup(SLOTCODE* itemcode_array);
    void ClickRewardButton();
    void DisplayRewardItem(MSG_CG_ITEM_USE_ACCUMULATE_POINT_ACK* recv_msg);
    void ProcessReward();
    eRewardStep GetRewardStep() {   return reward_step_; }

    void InserRewardMessage(TCHAR* reward_message);

    void ProcessErrorCode(DWORD error_code);

public:
    void set_is_close_reserve_(const bool& is_close_reserve);        
    void set_repeat_count_(const int& repeat_count);
    bool get_is_close_reserve_();
public:
    static void ESCProcess(DWORD wParam, DWORD lParam);

protected:
    virtual void Process(DWORD dwTick);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL val);    

private:
    void HideAllBlinkPictureControls();

private:
    static WzID controls_id_[DIALOG_POS_MAX];

    bool is_initialize;

    ItemUnitRenderer* item_unit_renderer_;
    CCtrlButtonWZ* reward_button_;
    CCtrlStaticWZ* coin_text_;
    CCtrlButtonWZ* reward_slot_button_;   
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD 
    CCtrlPictureWZ* blink_pictures_[IMAGE_SLOT_COUNTER*2];
#else
 	CCtrlPictureWZ* blink_pictures_[REWARD_ITEM_GROUP_COUNT*3];
#endif

    CCtrlButtonWZ* repeat_button_[2];
    CCtrlStaticWZ* repeat_text_;

    INVENTORY_TOTAL_INFO* reward_item_info_;    

    TextDisplayManager reward_message_manager_;
    Timer display_timer_;
    
    eRewardStep reward_step_;
    POSTYPE order_item_position_;
    DWORD display_item_position_;    
    DWORD previous_step_time_;
    DWORD time_for_move_;
    int remain_step_;
    int last_step_;

    SOUND_INDEX reward_sound_handle_;
    DWORD confirm_dialog_key_;
    DWORD tooltip_position_;

    DWORD require_point_;
    int repeat_count_;
#ifdef _NA_000000_20130726_RENEWAL_POINT_REWARD 
    int save_repect_count_;

    //////////////////////////////////////////////////////////////////////////
    SLOTCODE* itemcode_array_;
    std::vector<IconImage*> Image_list_; 
    bool is_close_reserve_;

    std::vector<SCItemSlot> render_slot_icon_vector_;
    std::vector<POSTYPE> Render_slot_Ctrl_;
    CTimerSimple list_refresh_timer_;
    CTimerSimple list_refresh_timer_2;
#endif
};

