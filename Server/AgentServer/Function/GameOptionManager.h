#ifndef GAME_OPTION_MANAGER_H_
#define GAME_OPTION_MANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifdef _NA_0_20110329_GAMEOPTION_SETTING
//===============================================================================================
//
// class GameOptionManager
//
//===============================================================================================
#include <Singleton.h>
#include "PacketStruct_AD.h"

class GameOptionAccessor;
// used protocols = {
struct MSG_AD_ETC_GAMEOPTION_SELECT_ACK;
struct MSG_AD_ETC_GAMEOPTION_SELECT_NAK;
//};

class GameOptionManager : public util::Singleton<GameOptionManager>
{
public:
    GameOptionManager();
    virtual ~GameOptionManager();

    void Load_GameOption(User* user, MSG_CG_ETC_GAMEOPTION_LOAD_SYN* syn_message);
    void Save_GameOption(User* user, MSG_CG_ETC_GAMEOPTION_SAVE_SYN* syn_message);
    void Select_Ack(User* user, MSG_AD_ETC_GAMEOPTION_SELECT_ACK* ack_message);
    void Select_Nak(User* user, MSG_AD_ETC_GAMEOPTION_SELECT_NAK* nak_message);
    void Disconnect_User(User* user);
    void HandleUpdateAck(const MSG_AD_ETC_GAMEOPTION_UPDATE_ACK* const update_ack);
    void Update();

private:
    void Copy(const GameOption* source, GameOption* dest)
    {
        // NOTE: f110609.9L, premises. the real size of 'GameOption' already checked.
        dest->option_type_ = source->option_type_;
        dest->binary_data_size_ = 0;
        // CHANGES: f110609.9L, prevent buffer overrun problem from the client cracked request message
        if (FlowControl::FCAssert(source->binary_data_size_ <= sizeof(source->binary_data_)))
        {
            dest->binary_data_size_ = source->binary_data_size_;
            CopyMemory(dest->binary_data_, source->binary_data_, source->binary_data_size_);
        };
    }
    GameOptionAccessor* Find(const GameOption::Type option_type);
    
private:
    bool is_running_update_;
    STLX_DEQUE<MSG_AD_ETC_GAMEOPTION_UPDATE_CMD> update_msg_queue_;
    GameOptionAccessor* accessor_hash_[GameOption::max_type_count];        
};

inline GameOptionAccessor* GameOptionManager::Find(const GameOption::Type option_type)
{
    if (FlowControl::FCAssert(option_type >= 0 && option_type < _countof(accessor_hash_))) {
        return accessor_hash_[option_type];
    };
    return NULL;
}

#endif // _NA_0_20110329_GAMEOPTION_SETTING

#endif // GAME_OPTION_MANAGER_H_
