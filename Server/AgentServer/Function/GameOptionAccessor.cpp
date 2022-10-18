#include "stdafx.h"
#include "GameOptionAccessor.h"


#ifdef _NA_0_20110329_GAMEOPTION_SETTING

GameOptionAccessor::GameOptionExPool GameOptionAccessor::gameoptionex_pool_;

void GameOptionAccessor::Initialize()
{
    GameOptionAccessor::gameoptionex_pool_.Initialize(0, 20, "GameOptionEx", NULL);
}

void GameOptionAccessor::Release()
{
    GameOptionAccessor::gameoptionex_pool_.Release();
}



GameOptionAccessor::GameOptionAccessor()
{
}

GameOptionAccessor::~GameOptionAccessor()
{
}

GameOptionEx* GameOptionAccessor::Find(User* user)
{
    uint key = get_accessor_key(user);
    GameOptionEx* gameoptionex = NULL;
    STLX_HASH_MAP<uint, GameOptionEx*>::iterator iter = hash_map_.find(key);
    if (iter != hash_map_.end())
        gameoptionex = iter->second;
    return gameoptionex;
}

GameOptionEx* GameOptionAccessor::Create(User* user)
{
    uint key = get_accessor_key(user);
    GameOptionEx* gameoptionex = (GameOptionEx*)gameoptionex_pool_.Alloc();
    ASSERT(gameoptionex);
    if (gameoptionex == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "GameOptionAccessor::Create() Memory Alloc Fail");
        return NULL;
    }

    init_gameoptionex(user, gameoptionex);
    hash_map_[key] = gameoptionex;
    return gameoptionex;
}

void GameOptionAccessor::Delete(User* user)
{
    uint key = get_accessor_key(user);
    STLX_HASH_MAP<uint, GameOptionEx*>::iterator iter = hash_map_.find(key);
    if (iter != hash_map_.end())
    {
        gameoptionex_pool_.Free(iter->second);
        hash_map_.erase(iter);
    }
}

//=====================================================================================

AutoHuntingAccessor::AutoHuntingAccessor()
{
}

AutoHuntingAccessor::~AutoHuntingAccessor()
{
}

uint AutoHuntingAccessor::get_accessor_key(User* user) const
{
    return user->GetSelectedCharGuid();
}

void AutoHuntingAccessor::init_gameoptionex(User* user, GameOptionEx* OUT gameoptionex) const
{
    gameoptionex->user_guid = user->GetUserGUID();
    gameoptionex->character_guid = user->GetSelectedCharGuid();
    gameoptionex->accessor_guid = user->GetSelectedCharGuid();
    gameoptionex->state = GameOptionEx::data_none;
    gameoptionex->binary_data_size_ = 0;
    gameoptionex->option_type_ = GameOption::kAutoHunting;
}


//=====================================================================================

KeyCustomizingAccessor::KeyCustomizingAccessor()
{
}

KeyCustomizingAccessor::~KeyCustomizingAccessor()
{
}

uint KeyCustomizingAccessor::get_accessor_key(User* user) const
{
    return user->GetUserGUID();
}

void KeyCustomizingAccessor::init_gameoptionex(User* user, GameOptionEx* OUT gameoptionex) const
{
    gameoptionex->user_guid = user->GetUserGUID();
    gameoptionex->character_guid = 0;
    gameoptionex->accessor_guid = user->GetUserGUID();
    gameoptionex->state = GameOptionEx::data_none;
    gameoptionex->binary_data_size_ = 0;
    gameoptionex->option_type_ = GameOption::kKeyCustomizing;
}

//{ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
uint GuidePerUserAccessor::get_accessor_key(User* user) const
{
    return user->GetUserGUID();
}
void GuidePerUserAccessor::init_gameoptionex(User* user, GameOptionEx* OUT option) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = 0;
    option->accessor_guid = user->GetUserGUID();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kGuidePerUser;
}
//
uint GuidePerCharAccessor::get_accessor_key(User* user) const
{
    return user->GetSelectedCharGuid();
}
void GuidePerCharAccessor::init_gameoptionex(User* user, GameOptionEx* OUT option) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = user->GetSelectedCharGuid();
    option->accessor_guid = user->GetSelectedCharGuid();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kGuidePerChar;    
}

uint NoticePerCharAccessor::get_accessor_key(User* user) const
{
    return user->GetSelectedCharGuid();
}
void NoticePerCharAccessor::init_gameoptionex(User* user, GameOptionEx* OUT option) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = user->GetSelectedCharGuid();
    option->accessor_guid = user->GetSelectedCharGuid();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kNoticePerChar;    
}

uint CharacterKeyCustomizing::get_accessor_key(User* user) const
{
    return user->GetSelectedCharGuid();
}
void CharacterKeyCustomizing::init_gameoptionex(User* user, GameOptionEx* OUT option) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = user->GetSelectedCharGuid();
    option->accessor_guid = user->GetSelectedCharGuid();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kKeyCustomizingPerChar;
}

uint LastSelectedCharAccessor::get_accessor_key(User* user) const
{
    return user->GetUserGUID();
}
void LastSelectedCharAccessor::init_gameoptionex(User* user, GameOptionEx* OUT option) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = 0;
    option->accessor_guid = user->GetUserGUID();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kLastSelectedChar;
}

#endif // _NA_0_20110329_GAMEOPTION_SETTING

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
uint PerkListAccessor::get_accessor_key( User* user ) const
{
    return user->GetSelectedCharGuid();
}

void PerkListAccessor::init_gameoptionex( User* user, GameOptionEx* OUT option ) const
{
    option->user_guid = user->GetUserGUID();
    option->character_guid = user->GetSelectedCharGuid();
    option->accessor_guid = user->GetSelectedCharGuid();
    option->state = GameOptionEx::data_none;
    option->binary_data_size_ = 0;
    option->option_type_ = GameOption::kPerkListChar;
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM