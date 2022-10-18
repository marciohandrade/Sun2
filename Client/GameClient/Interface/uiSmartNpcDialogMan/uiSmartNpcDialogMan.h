#pragma once

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM

//
// 보통 ui들은 다이얼로그들이 매니저에 등록되고 매니저가 다이얼로그를 파괴시킴.
// 레지스터 시켜놓고 쓰일 때 로드 할 수 있게 되어있음. (미리 키와 파일명을 세팅해놓음, 등록만 해놓음) (미리 모양이 정의된 형태)
// 지능형ui 는 각각 모양 타입별로 생성되니까  한번 생성해놓고 쓰기엔 개수가 많아지므로 비효율적임
// 필요할때 생성시키고, 쓰고난뒤 바로 해제하는게 좋음


struct IM_SMARTNPCDIALOG_MANAGER
{
    static const DWORD SMARTNPCDIALOG_MENU;
    static const DWORD SMARTNPCDIALOG_REPLY;
};

enum SmartDialogType
{
    SmartDialogType_Menu,
    SmartDialogType_Reply,

    SmartDialogType_Max
};

enum SmartDialogSubType
{
    SmartDialogSubType_Ok = 1,
    SmartDialogSubType_OkNo,
    SmartDialogSubType_OkNoCancle,
    SmartDialogSubType_Edit,
    SmartDialogSubType_Slot,
    SmartDialogSubType_SlotInput,
    SmartDialogSubType_ForceOk,

    SmartDialogSubType_Max

};

class uiSmartNpcDialog;

class uiSmartNpcDialogMan : public Singleton<uiSmartNpcDialogMan> 
{
public:
    enum 
    {
        MaxMenuButton = 6,
        MaxReplyButton = 3
    };
    uiSmartNpcDialogMan();
    ~uiSmartNpcDialogMan();

    uiSmartNpcDialog* ParserSmartDialogPacket(SmartDialogType type, CHAR* data_);

    // 작업되면 밑으루
    uiSmartNpcDialog* CreateSmartNpcDialog(SmartDialogType type, BYTE sub_type);

private:
    BOOL CreateSmartDialog(DWORD dialog_key);
    uiSmartNpcDialog* CreateSmartDialogTypeMenu(BYTE sub_type, BYTE button_count);
    uiSmartNpcDialog* CreateSmartDialogTypeReply(BYTE sub_type);

};


#endif //_NA_000000_20140120_SMART_NPC_SYSTEM