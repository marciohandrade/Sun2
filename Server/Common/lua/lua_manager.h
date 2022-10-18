#ifndef _SERVER_COMMON_LUA_LUA_MANAGER_H
#define _SERVER_COMMON_LUA_LUA_MANAGER_H

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

extern "C" {
#include <Lua_library/lua-5.2.3/src/lua.h>
#include <Lua_library/lua-5.2.3/src/lualib.h>
#include <Lua_library/lua-5.2.3/src/lauxlib.h>
};

//////////////////////////////////////////////////////////////////////////
// CLuaStreamPack - encryped script supporter
// arycoat - 2008.4.
// 
typedef int (*Decrypt_CFunction)(char*, unsigned);

class CLuaStreamPack
{
public:
    std::string filename;
    std::fstream file;
    char* buffer;
    size_t size;

    CLuaStreamPack();
    ~CLuaStreamPack();

    bool open(const char* _filename, Decrypt_CFunction _df = NULL);

    void clear_buffer();
    bool do_buffer(lua_State* L);
};

//////////////////////////////////////////////////////////////////////////
//
class LuaHandler
{
public:
    typedef int (*OnRegister_CFunction)(lua_State* );

    LuaHandler();
    ~LuaHandler();

    bool initialize(OnRegister_CFunction function_name);
    void close();
    static int traceback(lua_State* L);

    OnRegister_CFunction register_function_;
    lua_State* state_;
};


#endif //_SERVER_COMMON_LUA_LUA_MANAGER_H