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
            ui_guide_tutorial_dialog_->SetData(guide_index, 1);//�׻� 1������
            ui_guide_tutorial_dialog_->ShowInterface(TRUE);
        }
        break;
    case kDialogType_Information:
        if (ui_guide_infomation_dialog_)
        {
            ui_guide_infomation_dialog_->SetData(guide_index, 1);//�׻� 1������
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
            ui_guide_infomation_dialog_->SetData(1, 1);//�׻� 1������
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
            (notice->min_level > notice->max_level || g_pHero->GetLevel() <= notice->max_level))// �ִ� ������ �ּ� �������� ������ �ִ뷹�� üũ ����
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
                        (param2 <= notice->GetParam(1))) // �ش� ��ġ or %���� ������
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
            // TODO: �ߺ����൵ üũ�ؾ���
            // guide ��� �ڵ�� ���⿡
            if (notice->repeat_type != TotalGuideNotice::kContinue)
            {
                //�ߺ�üũ
                is_repeat =  CheckRepeatData(notice->code, notice->repeat_type);
            }

            if (is_repeat == true)
            {
                continue;
            }
            else //�ݺ� ���� ������ + �����̸� 
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

    // ��Ʋ �κ񿡼��� üũ ���Ѵ�(���Ŀ� ��Ʋ�� �������� �Ǹ� üũ ���� �ʵ���)
    if (( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOBBY )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHARSELECT )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOGIN )
        || ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_NONE ) )
        return;

    switch (character_status)
    {
    case TotalGuideNotice::kCreate:	//1. ĳ���� ����
        {
            if (GlobalFunc::IsBattleZone())
            {
                return;
            }
        }
        break;
    case TotalGuideNotice::kNpcClick:	//2. NPCŬ��
    case TotalGuideNotice::kEnterToMap:	//3. Ư�� �� ����
    case TotalGuideNotice::kGetExp:	//4. ����ġ ����
    case TotalGuideNotice::kGetItem://5. ������ ����
    case TotalGuideNotice::kLevelUp://6. ������
    case TotalGuideNotice::kFreeChaos://7.����ī��
    case TotalGuideNotice::kChaos://8.ī��
    case TotalGuideNotice::kDead://9.ĳ���� ���
    case TotalGuideNotice::kCreateGuild://10.��� â��
    case TotalGuideNotice::kUnderHP://11.HP Ư�� �ۼ�Ʈ ����
    case TotalGuideNotice::kQuest://12.����Ʈ ����/�Ϸ�
    case TotalGuideNotice::kAccumulatePoint: // 14.������ ����Ʈ ȹ���
    case TotalGuideNotice::kRemainGuide://16.���� ���� ���̵� ������ �ִ� ���
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
    //�ʱ�ȭ
    repeat_list_.InitData();
    repeat_save_list_.InitData();

    //�ݺ� ����Ʈ�� ����
    const size_t repeat_size = sizeof(repeat_list_.repeat_list[0]);

    if (game_option->binary_data_size_ > 0)
    {
        //����� ���� ����
        memcpy(&repeat_list_.repeat_data_header, game_option->binary_data_, sizeof(REPEAT_DATA_HEADER));

        //���� �˻�
        if (repeat_list_.repeat_data_header.packet_ver == REPEAT_DATA_PACKET_VER)
        {
            //Repeat ������ ����
            int currunt_index = 0;
            void* buffer_pointer = static_cast<void*>(game_option->binary_data_ + sizeof(REPEAT_DATA_HEADER));
            WORD* repeat_pointer = static_cast<WORD*>(buffer_pointer);

            for(int index = 0; index < repeat_list_.repeat_data_header.repeat_list_count; ++index)
            {
                WORD guide_index = *(repeat_pointer + index);
                
                const TotalGuideNotice* notice = TotalGuideParser::Instance()->GetNotice(guide_index);

                if ((notice == NULL) || (notice->repeat_type != TotalGuideNotice::kOnce))
                {
                    //�ݺ����� �����Ͱ� �ƴ�
                }
                else
                {
                    repeat_list_.repeat_list[currunt_index] = guide_index;
                    ++currunt_index;
                }
            }
            repeat_list_.repeat_data_header.repeat_list_count = currunt_index;
        }

        //���� �����Ϳ� �ݺ��ؾ� �Ǵ� �����Ͱ� �߸� �����ִ����� ����
        //VerifyRepeatData(&repeat_list_);

        memcpy(&repeat_save_list_, &repeat_list_, sizeof(REPEAT_LIST_DATA));
    }
}

void uiGuideSystemMan::SaveRepeatData()
{
    //�ݺ� ����Ʈ�� ����
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN sync;
    ZeroMemory(&(sync.game_option_), sizeof(sync.game_option_));

    sync.game_option_.option_type_ = GameOption::kNoticePerChar;
    sync.game_option_.binary_data_size_ = repeat_save_list_.GetSize();
    memcpy(sync.game_option_.binary_data_, &repeat_save_list_, repeat_save_list_.GetSize());

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
}

bool uiGuideSystemMan::CheckRepeatData(WORD guide_notice_code,TotalGuideNotice::RepeatType repeat_type)
{
    //�ش� �ڵ��� Notice�� ���� ����� ���� �ݺ� ���� üũ
    //���� �߾����� true ����
    for (uint index = 0; index < repeat_list_.repeat_data_header.repeat_list_count; ++index)
    {
        if (repeat_list_.repeat_list[index] == guide_notice_code)
        {
            return true;
        }
    }

    if (repeat_type != TotalGuideNotice::kContinue)
    {
        //���̺�
		int index;
        if (index > TotalGuideNotice::kMaxSize)
        {
            //���� : �ε��� �ʰ�
            assert(!"TotalGuideNoticeManager::CheckRepeatData()���� index�� ���� ���� ���� ������ ������ϴ�.");
            return false;
        }
        repeat_list_.repeat_list[repeat_list_.repeat_data_header.repeat_list_count] = guide_notice_code;
        repeat_list_.repeat_data_header.repeat_list_count += 1;

        //�ٷ� �����Ҳ��� ���⼭ �ٷ� ����
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
        //�ð� üũ
        if (waiting_save_recv_timer_->IsExpired() == true)
        {
            set_waiting_repeat_save_recv(false);
            set_waiting_library_save_recv(false);
            set_waiting_first_packet_recv(false);

            //�ð� �ى����� ����
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
                    //���� ����Ʈ�� ���ư� ���� Ŭ�󿡼� ���� �ȵ�
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
    //������ �����͸� ����
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN sync;
    ZeroMemory(&(sync.game_option_), sizeof(sync.game_option_));

    sync.game_option_.option_type_ = GameOption::kGuidePerChar;
    sync.game_option_.binary_data_size_ = library_list_data_.GetSize();
    memcpy(sync.game_option_.binary_data_, &library_list_data_, library_list_data_.GetSize());

    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
}




