#pragma once

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
class Player;

class PlayerSpaInfo
{
public:
    PlayerSpaInfo(Player* owner): owner_(owner), spa_ticket_code_(0) {}
    ~PlayerSpaInfo() {}
public:
    RC::eITEM_RESULT SetSpaInfo(SLOTIDX slotcontainer_type, POSTYPE item_pos);

    const DWORD GetSpaTicketCode() { return spa_ticket_code_; }

    VOID CheckAndResetSpaTicket(POSTYPE item_pos); //�����Ǵ� �������� üũ�Ѵ�, ����Ƽ���̸� ���½�Ű��
    VOID ResetSpaInfo();          // ����Ƽ������ ����
private:
    BOOL _SetSpaValueInfo(CODETYPE item_code);

    VOID _SetSpaTicketCode(DWORD code) { spa_ticket_code_ = code; }
private:
    Player* owner_;

    DWORD spa_ticket_code_; // ��õ �����(����) �������ڵ�
};
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM