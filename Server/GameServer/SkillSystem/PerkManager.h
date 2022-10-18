#pragma once
#ifndef GAMESERVER_PERK_MANAGER_H
#define GAMESERVER_PERK_MANAGER_H

#include <SkillSystem/PerkDataContainer.h>

class Player;

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

//==================================================================================================

namespace nsSkill {
;

class PerkDataContainer;

//==================================================================================================

class PerkManager
{
public:
    enum eRequestResult {
        eReqResult_None, //non-action
        eReqResult_ReplyResult, // send a message to client
        eReqResult_NeedForward, // wait an another result
        //
        eReqResult_Counts
    };
    static const ulong kUpdateExpirationDateInterval = 1 * 1000;
    static const ulong kUpdateStatusInterval = 1 * 1130; // to avoid cross product with a data timer
    //
    PerkManager();
    virtual ~PerkManager();
    //
    // this method is same to release method if an owner is null.
    void Reuse(Player* owner);
    void Update(const util::ServerTimeInfoPerFrame* time_info_per_frame);
    const nsSkill::PerkDataContainer* perk_data_container() const;
    // NOTE: the perk system experience rule, add only battle experience value.
    bool AddExp(int acquired_exp);
    // CHANGES: f101108.3L, to support GM commmand.
    bool ChangePerkLevel(int changed_level);
    //
    void StorePerk(const PerkDataList& perk_data_list, DWORD char_key);
    void LoadPerk(PerkDataList* perk_data_list);
    // CHANGES: f110727.2L, fixed a missing stopped timer resume event
    void OnEnterField(GameField* game_field);
    eRequestResult ProcessRequestedMessage(const PerkControl& req_info,
                                           PerkControl* header, PerkControlData* data);
	bool CanProcessState();
private:
    bool CanProcessState(PerkControl* header, PerkControlData* data);
    eRequestResult ProcessRegister(const PerkControl& req_info,
                                   PerkControl* header, PerkControlData* data);
    eRequestResult ProcessUnregister(const PerkControl& req_info,
                                     PerkControl* header, PerkControlData* data);
    eRequestResult ProcessPlants(const PerkControl& req_info,
                                 PerkControl* header, PerkControlData* data);
    eRequestResult ProcessUproot(const PerkControl& req_info,
                                 PerkControl* header, PerkControlData* data);
    bool UpdateExpirationDate(const util::ServerTimeInfoPerFrame* time_info_per_frame);
    bool UpdateStatus(const util::ServerTimeInfoPerFrame* time_info_per_frame);
    bool ProcessStatus(const PerkDataContainer::TouchList& changed_list);
    //
    ITimerBase update_expiration_date_timer_;
    ITimerBase update_status_timer_;
    //
    PerkDataContainer* const perk_data_container_;
    //
    struct InterData {
        Player* player;
        int acquired_experience;
    } inter_;
    //
    __DISABLE_COPY(PerkManager);
}; //end of class

//==================================================================================================

inline const nsSkill::PerkDataContainer* PerkManager::perk_data_container() const
{
    return perk_data_container_;
}

}; //end of namespace

//==================================================================================================

#endif //_NA002217_100728_EP2_PERK_SYSTEM_

#endif //GAMESERVER_PERK_MANAGER_H