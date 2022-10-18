#ifndef _AUTOBOT_H_
#define _AUTOBOT_H_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "Singleton.h"

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class NPC;
class Item;
struct MSG_BASE;
struct ChatMsg;
class AutobotPasspoint; 

//------------------------------------------------------------------------------
class Autobot : public Singleton<Autobot>
{ 
public:
    Autobot();
    ~Autobot();

    void Start();
    bool IsStarted() const { return started_; }
    void Stop(bool is_return_village);

    //------------------------------------------------------------------------------
    //! Returns:   bool :   false : 이후 프로세스에서 처리 안함
    //!                     true : 이후 프로세스에서 처리
    //------------------------------------------------------------------------------
    bool Update(DWORD dwTick);
    bool NetworkProc(MSG_BASE * packet);
    bool MessageProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

    bool SellItem(POSTYPE inventory_slot_pos);
    bool BuyItem(SLOTCODE item_code, NUMTYPE buy_count);
    bool RepairItemWearAll();
    bool UseItem(SCItemSlot* item_slot);
    bool CanUseSkill(SLOTCODE skill_code);
    bool HasSellItem(OUT POSTYPE* inventory_slot_pos);
    bool HasBuyItem(OUT SLOTCODE* item_code, OUT NUMTYPE* buy_count);
    bool IsPossibleTarget(DWORD object_key);
    bool IsPossiblePickup(Item& item);

    bool GetAreaSkillClickPosition(SLOTCODE skill_code, WzVector& target_pos);
    int GetRandomDestinationTile(int skip_tile_count);
    WzVector GetRandomDestinationPosition(float radius);

    void WriteChat(ChatMsg* chat);

    void Render_Checkpoint();
    void Render_DebugInfo();

    //------------------------------------------------------------------------------
    AutobotStateMachine<Autobot>* state_machine() const { return state_machine_; }

    //------------------------------------------------------------------------------
    //! 루아에서 실행할수 있는 테스트 용으로
    void Lua_Action(const char* parameter1, const char* parameter2);

private:
    bool SyncState();

    AutobotStateMachine<Autobot>* state_machine_;

    bool started_;

    //! 결과를 보여주기 위해서 
    MONEY backup_hero_money_;
    EXPTYPE backup_hero_exp_;
};



#endif
