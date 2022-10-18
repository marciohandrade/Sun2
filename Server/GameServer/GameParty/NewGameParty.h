#pragma once

#include "BaseParty.h"

//////////////////////////////////////////////////////////////////////////
//  @history
//  - check-in  : lovesusun,
//  - 090514    : waverix,      move type definitions for has-a relation
//                              delete PARTY_MEMBERKEY_DEQUEUE_ITER
//                              change variable name : camel case style
class Player;

class NewGameParty : public BaseParty
{
    static const DWORD UpdateTimerInterval = 1 * 1000;
    struct MemberNode
    {
        DWORD player_key_;
        //!(_NA_20110630_BATTLE_ZONE_RENEWAL)
        DWORD current_hp_, max_hp_, reserve_hp_, current_mp_, max_mp_;
        DWORD current_sd_, max_sd_;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        PLAYERFP current_fp_, max_fp_;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
    typedef STLX_LIST<MemberNode> PartyMemberQueue;
    //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    typedef _DICECAST_RESULT DicecastResultArray[MAX_PARTYMEMBER_NUM];

public:
	NewGameParty();
	~NewGameParty();

    void Update();
	void Init(WORD party_key, DWORD master_key, ePARTY_TYPE party_type);
	void Release();

	RC::ePARTY_RESULT Join(const _PARTY_MEMBER_INFO& member_info);
	RC::ePARTY_RESULT Leave(DWORD member_key);

	DWORD GetFIFOMemberKey();
	DWORD GetRandomMemberKey(eCHAR_TYPE classType = eCHAR_TYPE_MAX);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    DWORD GetDicecastResultKey(Player* player, DicecastResultArray& result);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#if WAVERIX_CODE_DELETE_RESERVED
    inline LEVELTYPE GetTotalLevel() const { return total_level_; }
    void MemberLevelUp(DWORD member_key);
#endif

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    void SendPlayerRenderInfo(Player* request_player, Player* target_player)const;
    void NotifyMembersInfoToPlayer(Player* player)const;
    void NotifyPlayerInfoToPartyMember(Player* player);

private:

private:
    ITimerBase update_timer_;
	PartyMemberQueue member_queue_;

#if WAVERIX_CODE_DELETE_RESERVED
    LEVELTYPE total_level_;
#endif
};

