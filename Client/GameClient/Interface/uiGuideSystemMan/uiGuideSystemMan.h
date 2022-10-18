#pragma once

#include "uiBaseMan.h"
#include "TotalGuideParser.h"

struct IM_GUIDE_SYSTEM_MANAGER
{
    static const DWORD HELP_ALERT;
    static const DWORD TUTORIAL;
    static const DWORD TUTORIAL_OPEN;
    static const DWORD INFOMATION;
};

#define WAITING_SAVE_RECV_TIME 1000
#define REPEAT_DATA_PACKET_VER 100
#define LIBRARY_DATA_PACKET_VER 101 // 101 ����,
/* LIBRARY_DATA_PACKET_VER ���� �����丮 (��������� +100, ó������� +1)
 100: �� ������ ���� �������� ���� ���� �ʱ�� ����
 101: 100 ������ ����� ���� ���� 1 ����
*/

struct REPEAT_DATA_HEADER
{
    typedef unsigned short Size_Repeat;

    Size_Repeat packet_ver;
    Size_Repeat repeat_list_count;
    
    //������ ���� ������ ���̳ʸ� �����͸� �����ϰ� �б� ������ ������ ������ �ٲ� �� �־ �Դϴ�.
};

struct REPEAT_LIST_DATA
{
    REPEAT_LIST_DATA();
    ~REPEAT_LIST_DATA();

    void InitData();

    uint16_t GetSize() const 
    {
        const size_t kRepeatHeaderSize = sizeof(REPEAT_DATA_HEADER);
        return (kRepeatHeaderSize + sizeof(repeat_list[0]) * repeat_data_header.repeat_list_count);
    }

    REPEAT_DATA_HEADER repeat_data_header;
    WORD repeat_list[TotalGuideNotice::kMaxSize];
};

struct LIBARAY_DATA_HEADER
{
    typedef unsigned short Size_Library;

    Size_Library packet_ver;
    Size_Library library_list_count;

    //������ ���� ������ ���̳ʸ� �����͸� �����ϰ� �б� ������ ������ ������ �ٲ� �� �־ �Դϴ�.
};

struct LIBARAY_DATA
{
    typedef unsigned short Size_Library_Data;

    Size_Library_Data library_list_data;
    Size_Library_Data is_show;
};

struct LIBRARY_LIST_DATA
{
    LIBRARY_LIST_DATA();
    ~LIBRARY_LIST_DATA();

    void InitData();

    uint16_t GetSize() const 
    {
        const size_t kLibraryHeaderSize = sizeof(LIBARAY_DATA_HEADER);
        return (kLibraryHeaderSize + sizeof(library_list[0]) * library_data_header.library_list_count);
    }

    LIBARAY_DATA_HEADER library_data_header;
    LIBARAY_DATA library_list[TotalGuideNotice::kMaxSize];
};

class uiGuideMessageDialog;
class uiGuideTutorialDialog;
class uiGuideTutorialOpenDialog;
class uiGuideInfomationlDialog;
class uiGuideSystemMan: public uiBaseMan
{
private:
    virtual void OnInitialize();
    virtual void OnRelease();

public:
    enum DialogType
    {
        kDialogType_Tutorial = 1,
        kDialogType_Information = 2,
        kDialogType_HelpAlert = 3,
        kDialogType_Max,
    };
    uiGuideSystemMan(InterfaceManager* ui_manager);

    enum LogOutType
    {
        kGotoCharacterSelect = 0,
        kGotoServerSelect = 1,
        kGotoExit = 2,
        kLogOutTypeEnd = 3,
    };
public:

    void Update();
    void Clear();
    void OpenGuideDialog(BYTE guide_dialog_type, TotalGuideNotice::Index guide_index);
    void AddGuide(DWORD character_status, DWORD param1, DWORD param2);
    void CheckHelpMessage(DWORD character_status, DWORD param1 = 0, DWORD param2 = 0);
    void ResetInfomationDialog();
    void set_is_have_notice_once(bool is_have_notice_once) {is_have_notice_once_ = is_have_notice_once;}

    //�ݺ� ������ ����
    void SEND_GUIDE_DATA_GAMEOPTION_LOAD_SYN();
    void LoadRepeatData(GameOption* game_option);
    void SaveRepeatData();
    bool CheckRepeatData(WORD guide_notice_code, TotalGuideNotice::RepeatType repeat_type);
    void OpenGuideInfoDialog();

    void AddInfoNotice(TotalGuideNotice::Index guide_index);//ui_guide_infomation_dialog_ �� ������ �߰�
    void DeleteInfoNotice(TotalGuideNotice::Index guide_index);
    LIBRARY_LIST_DATA* library_list_data(){ return &library_list_data_; }

#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
    void SaveData();
#else
    void SaveAndOut(LogOutType logout_type);
    void UpdateSaveTimer(); //���� ���� ��Ŷ�� ��ٸ�
    void SetWaitingSaveTimer(DWORD time_){waiting_save_recv_timer_->SetTimer(time_);}

    void set_waiting_repeat_save_recv(bool is_waiting) {waiting_repeat_save_recv_ = is_waiting;}
    bool waiting_repeat_save_recv() {return waiting_repeat_save_recv_;}
    void set_waiting_library_save_recv(bool is_waiting) {waiting_library_save_recv_ = is_waiting;}
    bool waiting_library_save_recv() {return waiting_library_save_recv_;}

    void set_waiting_first_packet_recv(bool is_waiting) {waiting_first_packet_recv_ = is_waiting;}
    bool waiting_first_packet_recv() {return waiting_first_packet_recv_;}
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT

    //������ ������ ����
    void SEND_LIBRARY_DATA_GAMEOPTION_LOAD_SYN();
    void LoadLibraryData(GameOption* game_option);
    void SaveLibraryData();
    
private:
    uiGuideMessageDialog* ui_guide_message_dialog_;
    uiGuideTutorialDialog* ui_guide_tutorial_dialog_;
    uiGuideTutorialOpenDialog* ui_guide_tutorial_open_dialog_;
    uiGuideInfomationlDialog* ui_guide_infomation_dialog_;

#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
#else
    bool waiting_repeat_save_recv_;
    bool waiting_library_save_recv_;
    bool waiting_first_packet_recv_; //repeat �� library ���� ��Ŷ �� ���� �� ��Ŷ�� �ִ���
    util::Timer* waiting_save_recv_timer_;
    LogOutType logout_type_;
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT

    REPEAT_LIST_DATA repeat_list_;
    REPEAT_LIST_DATA repeat_save_list_;
    LIBRARY_LIST_DATA library_list_data_;
    bool is_have_notice_once_;//������ ������ �ִ��� �ѹ� �ٿ�� ���� ����
};
