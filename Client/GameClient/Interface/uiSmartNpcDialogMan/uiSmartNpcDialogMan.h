#pragma once

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM

//
// ���� ui���� ���̾�α׵��� �Ŵ����� ��ϵǰ� �Ŵ����� ���̾�α׸� �ı���Ŵ.
// �������� ���ѳ��� ���� �� �ε� �� �� �ְ� �Ǿ�����. (�̸� Ű�� ���ϸ��� �����س���, ��ϸ� �س���) (�̸� ����� ���ǵ� ����)
// ������ui �� ���� ��� Ÿ�Ժ��� �����Ǵϱ�  �ѹ� �����س��� ���⿣ ������ �������Ƿ� ��ȿ������
// �ʿ��Ҷ� ������Ű��, ������ �ٷ� �����ϴ°� ����


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

    // �۾��Ǹ� ������
    uiSmartNpcDialog* CreateSmartNpcDialog(SmartDialogType type, BYTE sub_type);

private:
    BOOL CreateSmartDialog(DWORD dialog_key);
    uiSmartNpcDialog* CreateSmartDialogTypeMenu(BYTE sub_type, BYTE button_count);
    uiSmartNpcDialog* CreateSmartDialogTypeReply(BYTE sub_type);

};


#endif //_NA_000000_20140120_SMART_NPC_SYSTEM