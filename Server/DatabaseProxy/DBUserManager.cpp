#include "stdafx.h"
#include "./DBUserManager.h"

#include <Macro.h>
#include "DBUser.h"
#include "ProxyNodes/DBQueryProxyManager.h"

//==================================================================================================

DBUserManager g_DBUserManager;

//==================================================================================================

void DBUserManager::Init(DWORD pool_size)
{
    user_pool_ = new util::CMemoryPoolFactory<DBUser>;
    character_pool_ = new util::CMemoryPoolFactory<DBCharacter>;
    user_pool_->Initialize(pool_size);
    character_pool_->Initialize(pool_size * 6);
}

void DBUserManager::Release()
{
    //풀 반환
    for (UserList::const_iterator it = users_.begin(); it != users_.end(); ++it)
    {
        DBUser* const db_user = it->second;
        if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        db_user->Release();
        user_pool_->Free(db_user);
    }
    users_.clear();
    
    //풀 삭제
    SAFE_DELETE(user_pool_);
    SAFE_DELETE(character_pool_);
}


void DBUserManager::Update()
{
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    
    UserList::iterator it = users_.begin();
    while (it != users_.end())
    {
        DBUser* const db_user = it->second;
        if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
        {
            users_.erase(it++);
            continue;
        }
        if (db_user->Update() == false)
        {
            UserList::iterator it_erase = it++;
            if(db_query_proxy_manager->DisconnectUser(db_user, false) == false) { // false 일때 삭제한다. arycoat - 2012.07.13
                users_.erase(it_erase);
            }
            continue;
        }
        // last
        ++it;
    }
}

DBUser* DBUserManager::CreateUser(DWORD user_key)
{
    if (users_.find(user_key) != users_.end()) 
    {
        return NULL;
    }

    DBUser* db_user = (DBUser*)user_pool_->Alloc();
    db_user->Init();    
    users_[user_key] = db_user;

    return db_user;
}

DBUser* DBUserManager::FindUser(DWORD user_key) const
{
    UserList::const_iterator it = users_.find(user_key);
    if (it == users_.end()) 
    {
        return NULL;
    }
    DBUser* const db_user = it->second;
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return NULL;
    }
    return db_user;
}

//////////////////////////////////////////////////////////////////////////
// arycoat - 2012.07.13
// memory leak detected!!!
void DBUserManager::DestroyUser(DBUser* db_user)
{
    if(RemoveFromTable(db_user->GetUserKey()) == NULL) {
        // it's ok. keep going~
    }
    db_user->Release();
    user_pool_->Free(db_user);
}

bool DBUserManager::InsertFromPendings(DBUser* db_user)
{
    if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
    {
        return false;
    }
    const DWORD user_key = db_user->GetUserGuid();
    users_[user_key] = db_user;
    return true;
}

DBUser* DBUserManager::RemoveFromTable(DWORD user_key)
{
    UserList::iterator it = users_.find(user_key);
    if (it == users_.end()) 
    {
        return NULL;
    }
    DBUser* const db_user = it->second;
    users_.erase(it);
    return db_user;
}

// (CHANGES) (f100422.7L) the 'removeRelateUser' function is changed to \
// the composed function of the DBUserManager
void DBUserManager::RemoveUsersByServerIndex(const int session_index, eSERVER_TYPE server_type)
{
    FOREACH_CONTAINER(const UserList::value_type& node, users_, UserList)
    {
        DBUser* const db_user = node.second;
        if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        const USERGUID user_guid = db_user->GetUserGuid();
        ChildServerSession* const server_session = db_user->GetServerSession();
        if (server_session == NULL)
        {
            // TODO(WAVERIX) release control
            SUNLOG(
                eCRITICAL_LOG,
                _T("["__FUNCTION__"]|Msg=can't find session in db_user|UserGuid=%d|"),
                user_guid
            );
            continue;
        }
        int user_session_index = server_session->GetSessionIndex();
        if (session_index == user_session_index)
        {
            const BYTE channel_id = db_user->GetChannelID();
            const bool is_release = (server_type == GAME_SERVER);
            SUNLOG(
                eCRITICAL_LOG,
                _T("|BatchProcess remove users on disconnect server|")
                _T("|ServerType=%d|UserGuid=%d|%s|"),
                server_type, 
                user_guid, 
                (is_release ? "Destroying" : "Pending")
            );
            if (is_release) 
            {
                db_user->RequestDisconnect(channel_id);
            }
            else 
            {
                db_user->MakeToExpiredLatestUpdateTick();
            }
        }
    }
}

void DBUserManager::DisplayPoolInfo() const
{
    SUNLOG(eCRITICAL_LOG, _T("users : %d"), users_.size());

    ;{
        SUNLOG(eCRITICAL_LOG, _T("user_pool_ : [band:%u node:%u]"), 
    	    user_pool_->GetNumberOfBands(), 
            user_pool_->GetAvailableNumberOfTypes() );
    };
    ;{
        SUNLOG(eCRITICAL_LOG, _T("character_pool_ : [band:%u node:%u]"), 
            character_pool_->GetNumberOfBands(), 
            character_pool_->GetAvailableNumberOfTypes() );
    };
}

void DBUserManager::Display() const
{
    DWORD statistics_number_of_users[eDB_STATE_MAX] = {0, };
    for (UserList::const_iterator it = users_.begin(); it != users_.end(); ++it)
    {
        const DBUser* const db_user = it->second;
        if (impl::IsValidUser(db_user, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        // 음... DBState가 잘못될 가능성은 없는가?
        ++statistics_number_of_users[db_user->GetDBState()];
    }
    
    int written = 0;
    STRING_SAFER_QUERY1024 output_buffer;
    written += output_buffer.CopyFromCompose(
        written, 
        _T("---------------------------------\n"), 
        statistics_number_of_users[eDB_STATE_NONE]);

    if (statistics_number_of_users[eDB_STATE_NONE]) 
    {
        written += output_buffer.CopyFromCompose(
            written,
            _T("DBUser Connections[ETC]\t\t: %u \n"),
            statistics_number_of_users[eDB_STATE_NONE]);
    }
    if (statistics_number_of_users[eDB_STATE_CREATING]) 
    {
        written += output_buffer.CopyFromCompose(
            written,
            _T("DBUser Connections[CREATING]\t: %u \n"),
            statistics_number_of_users[eDB_STATE_CREATING]);
    }
    if (statistics_number_of_users[eDB_STATE_CREATED]) 
    {
        written += output_buffer.CopyFromCompose(
            written,
            _T("DBUser Connections[CREATED]\t: %u \n"),
            statistics_number_of_users[eDB_STATE_CREATED]);
    }
    if (statistics_number_of_users[eDB_STATE_DESTROY_WAIT]) 
    {
        written += output_buffer.CopyFromCompose(
            written,
            _T("DBUser Connections[DESTROYWAIT]\t: %u \n"),
            statistics_number_of_users[eDB_STATE_DESTROY_WAIT]);
    }
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    if (int pendings = nsQuery::DBQueryProxyManager::Instance()->GetPendingUsers()) 
    {
        written += output_buffer.CopyFromCompose(
            written, _T("DBUser Pendings\t: %u \n"), pendings);
    }
    SUNLOG(eCRITICAL_LOG, _T("%s"), (TCHAR*)output_buffer);
}

DBCharacter* DBUserManager::CreateCharacter()
{
    return (DBCharacter*)character_pool_->Alloc();
}

void DBUserManager::DestroyCharacter( DBCharacter* const db_character )
{
    character_pool_->Free(db_character);
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
const DBCharacter* const DBUserManager::FindConnectedCharacter( CHARGUID char_guid )
{
    for (UserList::const_iterator it = users_.begin(); it != users_.end(); ++it)
    {
        DBUser* const db_user = it->second;
        if (db_user == NULL)
        {
            continue;
        }
        if (db_user->IsReleased())
        {
            continue;
        }
        
        DBCharacter* db_character = db_user->GetConnectedChar();
        if (db_character == NULL)
        {
            continue;
        }
        if (db_character->IsReleased())
        {
            continue;
        }
        if (db_character->GetCharGuid() == char_guid)
        {
            return db_character;
        }
    }
    return NULL;
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

namespace impl {

bool IsValidUser(const DBUser* const user, const char* func_str, const int line_number)
{
    if (user == NULL)
    {
        return false;
    }
    if (user->IsReleased())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|DBUser is already released.|UserGuid = %d|"), 
            func_str, 
            line_number, 
            user->GetUserGuid()
        );
        return false;
    }
    return true;
}

} // impl