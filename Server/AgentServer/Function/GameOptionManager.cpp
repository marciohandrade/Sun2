#include "stdafx.h"
#include "./GameOptionManager.h"

#include <PacketStruct_AD.h>

#include "GameOptionAccessor.h"

//==================================================================================================

#ifdef _NA_0_20110329_GAMEOPTION_SETTING

GameOptionManager::GameOptionManager() : is_running_update_(false)
{
    GameOptionAccessor::Initialize();
    // CHANGES: f110705.2L, prevent a problem that occured an uninitilaized buffer
    // when the GameOption type enumeration extended
    ZeroMemory(accessor_hash_, sizeof(accessor_hash_));
    accessor_hash_[GameOption::kAutoHunting] = new AutoHuntingAccessor();
    accessor_hash_[GameOption::kKeyCustomizing] = new KeyCustomizingAccessor();
#ifdef _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    accessor_hash_[GameOption::kGuidePerUser] = new GuidePerUserAccessor();
    accessor_hash_[GameOption::kGuidePerChar] = new GuidePerCharAccessor();
    accessor_hash_[GameOption::kNoticePerChar] = new NoticePerCharAccessor();
#endif
    accessor_hash_[GameOption::kKeyCustomizingPerChar] = new CharacterKeyCustomizing();
    accessor_hash_[GameOption::kLastSelectedChar] = new LastSelectedCharAccessor();
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    accessor_hash_[GameOption::kPerkListChar] = new PerkListAccessor();
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
}

GameOptionManager::~GameOptionManager()
{
    GameOptionAccessor** it = accessor_hash_;
    GameOptionAccessor** end = &accessor_hash_[_countof(accessor_hash_)];
    for (; it != end; ++it)
    {
        SAFE_DELETE(*it);
    };

    ZeroMemory(accessor_hash_, sizeof(accessor_hash_));

    GameOptionAccessor::Release();
    update_msg_queue_.clear();
}

void GameOptionManager::Load_GameOption(User* user, MSG_CG_ETC_GAMEOPTION_LOAD_SYN* syn_message)
{
    GameOptionAccessor* accessor = Find(syn_message->option_type_);
    if (accessor == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK nak_message;
        nak_message.error_code_ = RC::RC_GAME_OPTION_TYPE_FAIL;
        nak_message.option_type_ = syn_message->option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    GameOptionEx* gameoption_ex = accessor->Find(user);
    if (gameoption_ex == NULL)
    {
        gameoption_ex = accessor->Create(user);
        if (gameoption_ex == NULL)
        {
            MSG_CG_ETC_GAMEOPTION_LOAD_NAK nak_message;
            nak_message.error_code_ = RC::RC_GAME_OPTION_FAIL;
            nak_message.option_type_ = syn_message->option_type_;
            user->SendPacket(&nak_message, sizeof(nak_message));
            return;
        }
    }

    RC::eGAME_OPTION_RESULT result = RC::RC_GAME_OPTION_SUCCESS;
    switch (gameoption_ex->state)
    {
    case GameOptionEx::data_none:
        {
            AgentServer* agent = AgentServer::GetInstance();
            MSG_AD_ETC_GAMEOPTION_SELECT_SYN db_syn_message;
            db_syn_message.m_dwKey = user->GetUserKey();
            db_syn_message.user_guid_ = gameoption_ex->user_guid;
            db_syn_message.character_guid_ = gameoption_ex->character_guid;
            db_syn_message.option_type_ = syn_message->option_type_;

            if (agent->SendToDBPServer(&db_syn_message, sizeof(db_syn_message)))
                gameoption_ex->state = GameOptionEx::data_wait;
            else
                result = RC::RC_GAME_OPTION_FAIL;
        }
        break;

    case GameOptionEx::data_wait:
        {
            result = RC::RC_GAME_OPTION_TRANSACTION;
        }
        break;

    case GameOptionEx::data_loaded:
        {
            MSG_CG_ETC_GAMEOPTION_LOAD_ACK ack_message;
            Copy(gameoption_ex, &(ack_message.game_option_));
            user->SendPacket(&ack_message, ack_message.GetSize());
        }
        break;

    default:
        {
            result = RC::RC_GAME_OPTION_FAIL;
        }
        break;
    }

    if (result != RC::RC_GAME_OPTION_SUCCESS)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK nak_message;
        nak_message.error_code_ = result;
        nak_message.option_type_ = syn_message->option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
    }
}

void GameOptionManager::Save_GameOption(User* user, MSG_CG_ETC_GAMEOPTION_SAVE_SYN* syn_message)
{
    const GameOption& game_option = syn_message->game_option_;
    GameOptionAccessor* accessor = Find(game_option.option_type_);
    if (accessor == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_SAVE_NAK nak_message;
        nak_message.error_code_ = RC::RC_GAME_OPTION_TYPE_FAIL;
        nak_message.option_type_ = game_option.option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    GameOptionEx* gameoption_ex = accessor->Find(user);
    if (gameoption_ex == NULL)
    {
        gameoption_ex = accessor->Create(user);
        if (gameoption_ex == NULL)
        {
            MSG_CG_ETC_GAMEOPTION_SAVE_NAK nak_message;
            nak_message.error_code_ = RC::RC_GAME_OPTION_FIND_OPTION_FAIL;
            nak_message.option_type_ = game_option.option_type_;
            user->SendPacket(&nak_message, sizeof(nak_message));
            return;
        }
    }

    if (gameoption_ex->state & GameOptionEx::data_loaded)
    {
        Copy(&(syn_message->game_option_), gameoption_ex);

        MSG_AD_ETC_GAMEOPTION_UPDATE_CMD cmd_message;
        cmd_message.m_dwKey = user->GetUserKey();
        cmd_message.user_guid_ = gameoption_ex->user_guid;
        cmd_message.character_guid_ = gameoption_ex->character_guid;
        Copy(gameoption_ex, &(cmd_message.game_option_));

#ifdef _NA_004517_20120312_GAME_OPTION_TRANSACTION
        update_msg_queue_.push_back(cmd_message);
#else
        AgentServer* agent = AgentServer::GetInstance();
        agent->SendToDBPServer(&cmd_message, cmd_message.GetSize());

        MSG_CG_ETC_GAMEOPTION_SAVE_ACK ack_message;
        ack_message.option_type_ = game_option.option_type_;
        user->SendPacket(&ack_message, sizeof(ack_message));
#endif // _NA_004517_20120312_GAME_OPTION_TRANSACTION
    }
    else
    {
        RC::eGAME_OPTION_RESULT result = RC::RC_GAME_OPTION_SUCCESS;
        if (gameoption_ex->state == GameOptionEx::data_wait)
            result = RC::RC_GAME_OPTION_TRANSACTION;
        else
            result = RC::RC_GAME_OPTION_UNLOADED;

        MSG_CG_ETC_GAMEOPTION_SAVE_NAK nak_message;
        nak_message.error_code_ = result;
        nak_message.option_type_ = game_option.option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
    }
}

void GameOptionManager::Select_Ack(User* user, MSG_AD_ETC_GAMEOPTION_SELECT_ACK* ack_message)
{
    const GameOption& game_option = ack_message->game_option_;
    GameOptionAccessor* accessor = Find(game_option.option_type_);
    if (accessor == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK nak_message;
        nak_message.error_code_ = RC::RC_GAME_OPTION_TYPE_FAIL;
        nak_message.option_type_ = game_option.option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    GameOptionEx* gameoption_ex = accessor->Find(user);
    if (gameoption_ex == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK nak_message;
        nak_message.error_code_ = RC::RC_GAME_OPTION_FIND_OPTION_FAIL;
        nak_message.option_type_ = game_option.option_type_;
        user->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    Copy(&(ack_message->game_option_), gameoption_ex);
    gameoption_ex->state = gameoption_ex->data_loaded;


    MSG_CG_ETC_GAMEOPTION_LOAD_ACK cg_ack_message;
    Copy(gameoption_ex, &(cg_ack_message.game_option_));
    user->SendPacket(&cg_ack_message, cg_ack_message.GetSize());
}

void GameOptionManager::Select_Nak(User* user, MSG_AD_ETC_GAMEOPTION_SELECT_NAK* nak_message)
{
    if (nak_message->option_type_ < GameOption::lower_boundary ||
        nak_message->option_type_ >= GameOption::max_type_count)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK cg_nak_message;
        cg_nak_message.error_code_ = RC::RC_GAME_OPTION_TYPE_FAIL;
        cg_nak_message.option_type_ = nak_message->option_type_;
        user->SendPacket(&cg_nak_message, sizeof(cg_nak_message));
        return;
    }

    const GameOption::Type option_type = static_cast<GameOption::Type>(nak_message->option_type_);
    GameOptionAccessor* accessor = Find(option_type);
    if (accessor == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK cg_nak_message;
        cg_nak_message.error_code_ = RC::RC_GAME_OPTION_TYPE_FAIL;
        cg_nak_message.option_type_ = nak_message->option_type_;
        user->SendPacket(&cg_nak_message, sizeof(cg_nak_message));
        return;
    }

    GameOptionEx* gameoption_ex = accessor->Find(user);
    if (gameoption_ex == NULL)
    {
        MSG_CG_ETC_GAMEOPTION_LOAD_NAK cg_nak_message;
        cg_nak_message.error_code_ = RC::RC_GAME_OPTION_FIND_OPTION_FAIL;
        cg_nak_message.option_type_ = nak_message->option_type_;
        user->SendPacket(&cg_nak_message, sizeof(cg_nak_message));
        return;
    }

    gameoption_ex->state = GameOptionEx::data_none;

    MSG_CG_ETC_GAMEOPTION_LOAD_NAK cg_nak_message;
    cg_nak_message.error_code_ = nak_message->error_code_;
    cg_nak_message.option_type_ = nak_message->option_type_;
    user->SendPacket(&cg_nak_message, sizeof(cg_nak_message));
}

void GameOptionManager::Disconnect_User(User* user)
{
    //AgentServer* agent = AgentServer::GetInstance();

    GameOptionAccessor** it = accessor_hash_;
    GameOptionAccessor** end = &accessor_hash_[_countof(accessor_hash_)];
    for (; it != end; ++it)
    {
        GameOptionAccessor* accessor = *it;
        // CHANGES: f110705.2L
        if (accessor == NULL) {
            continue;
        };
        GameOptionEx* gameoption_ex = accessor->Find(user);
        // CHANGES: f110708.4L, the 'Find' method has not the implicit allocation since sun:r53618
        if (gameoption_ex == NULL) {
            continue;
        };
        accessor->Delete(user);
    }
}

void GameOptionManager::HandleUpdateAck(const MSG_AD_ETC_GAMEOPTION_UPDATE_ACK* const update_ack)
{
    is_running_update_ = false;
    User* const user = UserManager::Instance()->GetUser(update_ack->m_dwKey);
    if (user != NULL)
    {
        if (update_ack->is_success)
        {
            MSG_CG_ETC_GAMEOPTION_SAVE_ACK save_ack;
            save_ack.option_type_ = static_cast<GameOption::Type>(update_ack->option_type);
            user->SendPacket(&save_ack, sizeof(save_ack));
        }
        else
        {
            MSG_CG_ETC_GAMEOPTION_SAVE_NAK save_nak;
            save_nak.error_code_ = RC::RC_GAME_OPTION_LOAD_FAIL_FROM_DB;
            save_nak.option_type_ = static_cast<GameOption::Type>(update_ack->option_type);
            user->SendPacket(&save_nak, sizeof(save_nak));
        }
    }
}

void GameOptionManager::Update()
{
    if (is_running_update_ || update_msg_queue_.empty())
    {
        return;
    }
    MSG_AD_ETC_GAMEOPTION_UPDATE_CMD& update_cmd = update_msg_queue_.front();
    AgentServer* agent = AgentServer::GetInstance();
    agent->SendToDBPServer(&update_cmd, update_cmd.GetSize());
    update_msg_queue_.pop_front();
    is_running_update_ = true;
}

#endif // _NA_0_20110329_GAMEOPTION_SETTING