#include "SunClientPrecompiledHeader.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiGuideMessageDialog/uiGuideMessageDialog.h"
#include "uiGuideTutorialDialog/uiGuideTutorialDialog.h"
#include "uiGuideTutorialOpenDialog/uiGuideTutorialOpenDialog.h"
#include "uiGuideInfomationDialog/uiGuideInfomationDialog.h"
#include "Hero.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "GlobalFunc.h"

const DWORD IM_GUIDE_SYSTEM_MANAGER::HELP_ALERT = StrToWzID("0434");
const DWORD IM_GUIDE_SYSTEM_MANAGER::TUTORIAL = StrToWzID("0435");
const DWORD IM_GUIDE_SYSTEM_MANAGER::TUTORIAL_OPEN = StrToWzID("0437");
const DWORD IM_GUIDE_SYSTEM_MANAGER::INFOMATION = StrToWzID("0436");
const char c_guide_system_help_alert_ui_name[] = ("Data\\Interface\\43_4_Help.iwz");
const char c_guide_system_tutorial_ui_name[] = ("Data\\Interface\\43_5_Tutorial.iwz");
const char c_guide_system_tutorial_opeb_ui_name[] = ("Data\\Interface\\43_7_Tutorial.iwz");
const char c_guide_system_infomation_ui_name[] = ("Data\\Interface\\43_6_Tutorial.iwz");


REPEAT_LIST_DATA::REPEAT_LIST_DATA()
{
    InitData();
}
REPEAT_LIST_DATA::~REPEAT_LIST_DATA()
{

}
void REPEAT_LIST_DATA::InitData()
{
    repeat_data_header.packet_ver = REPEAT_DATA_PACKET_VER;
    repeat_data_header.repeat_list_count = 0;
    ZeroMemory(repeat_list, sizeof(repeat_list));
}

LIBRARY_LIST_DATA::LIBRARY_LIST_DATA()
{
    InitData();
}
LIBRARY_LIST_DATA::~LIBRARY_LIST_DATA()
{

}
void LIBRARY_LIST_DATA::InitData()
{
    library_data_header.packet_ver = 0;
    library_data_header.library_list_count = 0;
    ZeroMemory(library_list, sizeof(library_list));
}

uiGuideSystemMan::uiGuideSystemMan(InterfaceManager* ui_manager) : uiBaseMan(ui_manager)
{
    
}

void uiGuideSystemMan::OnRelease()
{
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
    SAFE_DELETE(waiting_save_recv_timer_);
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
}

void uiGuideSystemMan::OnInitialize()
{
    // load ui
    ui_guide_message_dialog_ = CreateUIDialog<uiGuideMessageDialog>(IM_GUIDE_SYSTEM_MANAGER::HELP_ALERT, 
        c_guide_system_help_alert_ui_name, this, TRUE);
    assert(ui_guide_message_dialog_);

    ui_guide_tutorial_dialog_ = CreateUIDialog<uiGuideTutorialDialog>(IM_GUIDE_SYSTEM_MANAGER::TUTORIAL, 
        c_guide_system_tutorial_ui_name, this, TRUE);
    assert(ui_guide_tutorial_dialog_);

    ui_guide_tutorial_open_dialog_ = CreateUIDialog<uiGuideTutorialOpenDialog>(IM_GUIDE_SYSTEM_MANAGER::TUTORIAL_OPEN, 
        c_guide_system_tutorial_opeb_ui_name, this, TRUE);
    assert(ui_guide_tutorial_open_dialog_);

    ui_guide_infomation_dialog_ = CreateUIDialog<uiGuideInfomationlDialog>(IM_GUIDE_SYSTEM_MANAGER::INFOMATION, 
        c_guide_system_infomation_ui_name, this, TRUE);
    assert(ui_guide_infomation_dialog_);


#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
    // initialize
    logout_type_ = kLogOutTypeEnd;
    waiting_repeat_save_recv_ = false;
    waiting_library_save_recv_ = false;
    waiting_first_packet_recv_ = false;
    waiting_save_recv_timer_ = new util::Timer;
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT

    is_have_notice_once_ = false;
}

void uiGuideSystemMan::Update()
{
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
    UpdateSaveTimer();
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
}

void uiGuideSystemMan::OpenGuideDialog(BYTE guide_dialog_type, TotalGuideNotice::Index guide_index)
{
    switch (guide_dialog_type)
    {
    case kDialogType_HelpAlert:
        if (ui_guide_message_dialog_)
        {
            ui_guide_message_dialog_->AddData(guide_index);
            //ui_guide_message_dialog_->ShowInterface(TRUE);
        }
        break;
    case kDialogType_Tutorial:
        if (ui_guide_tutorial_dialog_)
        {
            ui_guide_tutorial_dialog_->ShowInterface(FALSE);
            ui_guide_tutorial_dialog_->SetData(guide_index, 1);//항상 1페이지
            ui_guide_tutorial_dialog_->ShowInterface(TRUE);
        }
        break;
    case kDialogType_Information:
        if (ui_guide_infomation_dialog_)
        {
            ui_guide_infomation_dialog_->SetData(guide_index, 1);//항상 1페이지
            ui_guide_infomation_dialog_->ShowInterface(TRUE);
        }
        break;
    }
}

void uiGuideSystemMan::OpenGuideInfoDialog()
{
    if (ui_guide_infomation_dialog_)
    {
        if (ui_guide_infomation_dialog_->IsVisible())
        {
            ui_guide_infomation_dialog_->ShowInterface(FALSE);
        }
        else
        {
            ui_guide_infomation_dialog_->SetData(1, 1);//항상 1페이지
            ui_guide_infomation_dialog_->ShowInterface(TRUE);
        }        
    }
}

void uiGuideSystemMan::ResetInfomationDialog()
{
    if (ui_guide_infomation_dialog_)
    {
        ui_guide_infomation_dialog_->ResetData();
    }
}

void uiGuideSystemMan::AddGuide(DWORD character_status, DWORD param1 /*= 0*/, DWORD param2 /*= 0*/)
{
    bool is_addition = false;
    const TotalGuideNotice::CharStatus status = static_cast<TotalGuideNotice::CharStatus>(character_status);

    const TotalGuideNoticeManager::CharStatusMap& status_map = 
        TotalGuideParser::Instance()->GetNoticeManager().char_status_map;
    TotalGuideNoticeManager::CharStatusMap::const_iterator pos = status_map.lower_bound(status);
    TotalGuideNoticeManager::CharStatusMap::const_iterator end_pos = status_map.upper_bound(status);
    for (;pos != end_pos; ++pos)
    {
        const TotalGuideNotice* const notice = pos->second;

        is_addition = false;
        if (notice->min_level <= g_pHero->GetLevel() && 
            (notice->min_level > notice->max_level || g_pHero->GetLevel() <= notice->max_level))// 최대 레벨이 최소 레벨보다 낮으면 최대레벨 체크 안함
        {
            switch (character_status)
            {
            case TotalGuideNotice::kGetItem:
            case TotalGuideNotice::kQuest:
                {
                    if (param1 == notice->GetParam(0) &&
                        param2 == notice->GetParam(1))
                    {
                        is_addition = true;
                    }
                }
                break;
            case TotalGuideNotice::kUnderHP:
                {
                    if ((notice->GetParam(0) == param1) &&
                        (param2 <= notice->GetParam(1))) // 해당 수치 or %보다 작을때
                    {
                        is_addition = true;
                    }
                }
                break;
            case TotalGuideNotice::kRemainGuide:
                {
                    if (is_have_notice_once_ == false && ui_guide_infomation_dialog_->IsHaveNoticeGuide())
                    {
                        is_have_notice_once_ = true;
                        is_addition = true;
                    }
                }
                break;
            default:
            {
                if (notice->GetParam(0) == param1)
                {
                    is_addition = true;
                }
            }
            break;
            }

            if (is_addition == false)
            {
                continue;
            }

            bool is_repeat = false;
            // TODO: 중복실행도 체크해야함
            // guide 사용 코드는 여기에
            if (notice->repeat_type != TotalGuideNotice::kContinue)
            {
                //중복체크
                is_repeat =  CheckRepeatData(notice->code, notice->repeat_type);
            }

            if (is_repeat == true)
            {
                continue;
            }
            else //반복 금지 데이터 + 생성이면 
            {
                if (character_status == TotalGuideNotice::kCreate)
                {
                    is_have_notice_once_ = true;
                }
            }

            if (ui_guide_message_dialog_)
            {
                ui_guide_message_dialog_->AddData(notice->code);
            }
        }
    }
}

void uiGuideSystemMan::CheckHelpMessage(DWORD character_status, DWORD param1/* = 0*/, DWORD param2/* = 0*/)
{
    if(!g_pHero) return;

    /*if(!m_bLoadInfo)
    {
        LoadReadInfo();
    }*/

    // 배틀 로비에서는 체크 안한다(추후에 배틀존 리뉴얼이 되면 체크 하지 않도록)
    if (( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOBBY )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOGIN )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ) )
        return;

    switch (character_status)
    {
    case TotalGuideNotice::kCreate:	//1. 캐릭터 생성
        {
            if (GlobalFunc::IsBattleZone())
            {
                return;
            }
        }
        break;
    case TotalGuideNotice::kNpcClick:	//2. NPC클릭
    case TotalGuideNotice::kEnterToMap:	//3. 특정 맵 진입
    case TotalGuideNotice::kGetExp:	//4. 경험치 습득
    case TotalGuideNotice::kGetItem://5. 아이템 습득
    case TotalGuideNotice::kLevelUp://6. 레벨업
    case TotalGuideNotice::kFreeChaos://7.프리카오
    case TotalGuideNotice::kChaos://8.카오
    case TotalGuideNotice::kDead://9.캐릭터 사망
    case TotalGuideNotice::kCreateGuild://10.길드 창설
    case TotalGuideNotice::kUnderHP://11.HP 특정 퍼센트 이하
    case TotalGuideNotice::kQuest://12.퀘스트 수락/완료
    case TotalGuideNotice::kAccumulatePoint: // 14.접속자 포인트 획득시
    case TotalGuideNotice::kRemainGuide://16.보지 않은 가이드 정보가 있는 경우
        break;
    default:
        return;
    }

    AddGuide(character_status, param1, param2);
}

void uiGuideSystemMan::AddInfoNotice(TotalGuideNotice::Index guide_index)
{
    if (ui_guide_infomation_dialog_)
    {
        ui_guide_infomation_dialog_->AddData(guide_index);
        ui_guide_infomation_dialog_->SelectList(guide_index);        
    }
}

void uiGuideSystemMan::DeleteInfoNotice(TotalGuideNotice::Index guide_index)
{
    if (ui_guide_infomation_dialog_)
    {
        TotalGuide* guide = TotalGuideParser::Instance()->GetGuide(guide_index);
        if (guide)
        {
            ui_guide_infomation_dialog_->DeleteData(guide->category_code, guide_index);
        }
    }
}

void uiGuideSystemMan::SEND_GUIDE_DATA_GAMEOPTION_LOAD_SYN()
{
    repeat_list_.InitData();
    repeat_save_list_.InitData();

    MSG_CG_ETC_GAMEOPTION_LOAD_SYN sync;
    sync.option_type_ = GameOption::kNoticePerChar;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
}
void uiGuideSystemMan::LoadRepeatData(GameOption* game_option)
{
    //초기화
    repeat_list_.InitData();
    repeat_save_list_.InitData();

    //반복 리스트를 받음
    const size_t repeat_size = sizeof(repeat_list_.repeat_list[0]);

    if (game_option->binary_data_size_ > 0)
    {
        //헤더를 먼저 읽음
        memcpy(&repeat_list_.repeat_data_header, game_option->binary_data_, sizeof(REPEAT_DATA_HEADER));

        //버전 검사
        if (repeat_list_.repeat_data_header.packet_ver == REPEAT_DATA_PACKET_VER)
        {
            //Repeat 데이터 검증
            int currunt_index = 0;
            void* buffer_pointer = static_cast<void*>(game_option->binary_data_ + sizeof(REPEAT_DATA_HEADER));
            WORD* repeat_pointer = static_cast<WORD*>(buffer_pointer);

            for(int index = 0; index < repeat_list_.repeat_data_header.repeat_list_count; ++index)
            {
                WORD guide_index = *(repeat_pointer + index);
                
                const TotalGuideNotice* notice = TotalGuideParser::Instance()->GetNotice(guide_index);

                if ((notice == NULL) || (notice->repeat_type != TotalGuideNotice::kOnce))
                {
                    //반복안함 데이터가 아님
                }
                else
                {
                    repeat_list_.repeat_list[currunt_index] = guide_index;
                    ++currunt_index;
                }
            }
            repeat_list_.repeat_data_header.repeat_list_count = currunt_index;
        }

        //받은 데이터에 반복해야 되는 데이터가 잘못 들어와있는지의 검증
        //VerifyRepeatData(&repeat_list_);

        memcpy(&repeat_save_list_, &repeat_list_, sizeof(REPEAT_LIST_DATA));
    }
}

void uiGuideSystemMan::SaveRepeatData()
{
    //반복 리스트를 저장
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN sync;
    ZeroMemory(&(sync.game_option_), sizeof(sync.game_option_));

    sync.game_option_.option_type_ = GameOption::kNoticePerChar;
    sync.game_option_.binary_data_size_ = repeat_save_list_.GetSize();
    memcpy(sync.game_option_.binary_data_, &repeat_save_list_, repeat_save_list_.GetSize());

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
}

bool uiGuideSystemMan::CheckRepeatData(WORD guide_notice_code,TotalGuideNotice::RepeatType repeat_type)
{
    //해당 코드의 Notice가 실행 됬었던 건지 반복 여부 체크
    //실행 했었으면 true 리턴
    for (uint index = 0; index < repeat_list_.repeat_data_header.repeat_list_count; ++index)
    {
        if (repeat_list_.repeat_list[index] == guide_notice_code)
        {
            return true;
        }
    }

    if (repeat_type != TotalGuideNotice::kContinue)
    {
        //세이브
		int index;
        if (index > TotalGuideNotice::kMaxSize)
        {
            //에러 : 인덱스 초과
            assert(!"TotalGuideNoticeManager::CheckRepeatData()에서 index가 예측 하지 못한 범위를 벗어났습니다.");
            return false;
        }
        repeat_list_.repeat_list[repeat_list_.repeat_data_header.repeat_list_count] = guide_notice_code;
        repeat_list_.repeat_data_header.repeat_list_count += 1;

        //바로 저장할꺼면 여기서 바로 저장
        if (repeat_type == TotalGuideNotice::kOnce)
        {
            repeat_save_list_.repeat_list[repeat_save_list_.repeat_data_header.repeat_list_count] = guide_notice_code;
            repeat_save_list_.repeat_data_header.repeat_list_count += 1;
        }
    }

    return false;
}

#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
void uiGuideSystemMan::SaveData()
{
    SaveRepeatData();
    SaveLibraryData();
}
#else
void uiGuideSystemMan::SaveAndOut(LogOutType logout_type)
{
    SetWaitingSaveTimer(WAITING_SAVE_RECV_TIME);
    logout_type_ = logout_type;
    set_waiting_first_packet_recv(false);
    SaveRepeatData();
    SaveLibraryData();
}
void uiGuideSystemMan::UpdateSaveTimer()
{
    if ((waiting_repeat_save_recv() == true) &&
        (waiting_library_save_recv() == true))
    {
        //시간 체크
        if (waiting_save_recv_timer_->IsExpired() == true)
        {
            set_waiting_repeat_save_recv(false);
            set_waiting_library_save_recv(false);
            set_waiting_first_packet_recv(false);

            //시간 다됬으면 종료
            switch(logout_type_)
            {
            case kGotoCharacterSelect:
                {
                    logout_type_ = kLogOutTypeEnd;
                    MSG_CG_CONNECTION_BACKTOCHARSELECT_SYN SendPacket;
                    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
                }
                break;
            case kGotoServerSelect:
                {
                    logout_type_ = kLogOutTypeEnd;
                    //서버 리스트로 돌아갈 때는 클라에서 제어 안됨
                }
                break;
            case kGotoExit:
                {
                    logout_type_ = kLogOutTypeEnd;
                    GlobalFunc::LogOutGame(false);
                }
                break;
            case kLogOutTypeEnd:
                {
                    //no operation
                }
                break;
            default:
                assert(!"Unknown value = logout_type_");
                break;
            }
        }
    }
}
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT

void uiGuideSystemMan::SEND_LIBRARY_DATA_GAMEOPTION_LOAD_SYN()
{
    library_list_data_.InitData();

    MSG_CG_ETC_GAMEOPTION_LOAD_SYN sync;
    sync.option_type_ = GameOption::kGuidePerChar;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
}

void uiGuideSystemMan::LoadLibraryData(GameOption* game_option)
{
    library_list_data_.InitData();

    if (game_option->binary_data_size_ > 0)
    {
        memcpy(&library_list_data_, game_option->binary_data_, game_option->binary_data_size_);
    }
}

void uiGuideSystemMan::SaveLibraryData()
{
    //열람실 데이터를 저장
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN sync;
    ZeroMemory(&(sync.game_option_), sizeof(sync.game_option_));

    sync.game_option_.option_type_ = GameOption::kGuidePerChar;
    sync.game_option_.binary_data_size_ = library_list_data_.GetSize();
    memcpy(sync.game_option_.binary_data_, &library_list_data_, library_list_data_.GetSize());

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
}




