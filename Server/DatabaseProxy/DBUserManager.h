#pragma once

//==================================================================================================
/// DBUserManager class
/**
    @author	Kim Min Wook < taiyo@webzen.com >
    @since	
    @remark

    @note
    - 
    @history 
    - 2006.11.10 : 리펙토링, DBUser를 바로 캐쉬없이 삭제하는 것으로 변경
    - 2010.04.22|waverix|fit in coding style about the indent rule
                        |(f100422.7L) the 'removeRelateUser' function is changed to \
                         the composed function of the DBUserManager
    - 2010.04.23|waverix|fit in coding style
*/
//==================================================================================================

class DBUser;
class DBCharacter;

class DBUserManager
{
public:
    DBUserManager() 
    {
    }
    ~DBUserManager() 
    {
    }

public:
    void Init(DWORD pool_size);
    void Release();
    void Update();

    DBUser* CreateUser(DWORD user_key);
    DBUser* FindUser(DWORD user_key) const;
    void DestroyUser(DBUser* db_user);
    bool InsertFromPendings(DBUser* db_user);
    DBUser* RemoveFromTable(DWORD user_key);
    // (CHANGES) (f100422.7L) the 'removeRelateUser' function is changed to \
    // the composed function of the DBUserManager
    void RemoveUsersByServerIndex(const int session_index, eSERVER_TYPE debug_type);
    void DisplayPoolInfo() const;
    void Display() const;
    DBCharacter* CreateCharacter();
    void DestroyCharacter(DBCharacter* const db_character);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    const DBCharacter* const FindConnectedCharacter(CHARGUID char_guid);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

private:
    typedef STLX_HASH_MAP<DWORD, DBUser*> UserList;
    typedef util::CMemoryPoolFactory<DBUser> UserPool;
    typedef util::CMemoryPoolFactory<DBCharacter> CharacterPool;
    
private:
    UserPool* user_pool_;
    CharacterPool* character_pool_;
    UserList users_;
};

namespace impl {

bool IsValidUser(const DBUser* const user, const char* func_str, const int line_number);

} // impl

extern DBUserManager g_DBUserManager;