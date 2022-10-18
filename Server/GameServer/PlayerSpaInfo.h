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

    VOID CheckAndResetSpaTicket(POSTYPE item_pos); //삭제되는 아이템을 체크한다, 스파티켓이면 리셋시키게
    VOID ResetSpaInfo();          // 스파티켓정보 리셋
private:
    BOOL _SetSpaValueInfo(CODETYPE item_code);

    VOID _SetSpaTicketCode(DWORD code) { spa_ticket_code_ = code; }
private:
    Player* owner_;

    DWORD spa_ticket_code_; // 온천 입장권(사용권) 아이템코드
};
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM