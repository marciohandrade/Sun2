#include "stdafx.h"
#include "lua_manager.h"

#ifdef _DEBUG
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/lua_d.lib")
#else
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/lua.lib")
#endif

CLuaStreamPack::CLuaStreamPack()
    : buffer(0)
{

}

CLuaStreamPack::~CLuaStreamPack()
{
    clear_buffer();
}

bool CLuaStreamPack::open( const char* _filename, Decrypt_CFunction _df /*= NULL*/ )
{
    filename = _filename;

    file.open(filename.c_str(), ios::in | ios::binary);
    if (!file)
        return false;

    if (buffer) {
        clear_buffer();
    }

    file.seekg(0, ios::end);
    size = (size_t)file.tellg();
    buffer = new char[size];

    file.seekg(0, ios::beg);
    file.read(buffer, size);
    file.close();

    if (_df) {
        _df(buffer, (unsigned int)size);
    }

    return true;
}

void CLuaStreamPack::clear_buffer()
{
    if (buffer)
        delete[] buffer;
}

bool CLuaStreamPack::do_buffer( lua_State* L )
{
    if(!buffer /*&& !open(filename.c_str(), df)*/)
        return false;

    int base = lua_gettop(L);
    lua_pushcfunction(L, LuaHandler::traceback);
    lua_insert(L, base);

    const char* szName = filename.c_str();
    int status = ( luaL_loadbuffer(L, buffer, size, szName) || lua_pcall(L, 0, LUA_MULTRET, base) );

    //string strmsg = lua_tostring(L, -1);
    lua_remove(L, base);

    if (status != 0)
        lua_gc(L, LUA_GCCOLLECT, 0);

    return ( status == 0 ) ? true : false;
}

//////////////////////////////////////////////////////////////////////////
//
LuaHandler::LuaHandler()
{
    register_function_ = NULL;
    state_ = NULL;
}

LuaHandler::~LuaHandler()
{

}

bool LuaHandler::initialize( OnRegister_CFunction function_name )
{
    register_function_ = function_name;

    lua_State* pLuaState = luaL_newstate();
    luaL_openlibs(pLuaState);

    if (register_function_)
        register_function_(pLuaState);

    state_ = pLuaState;

    return true;
}


void LuaHandler::close()
{
    lua_close(state_);
}

int LuaHandler::traceback( lua_State* L )
{
    lua_getglobal(L, "debug");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return 1;
    }
    lua_getfield(L, -1, "traceback");
    if (!lua_isfunction(L, -1)) {
        lua_pop(L, 2);
        return 1; 
    }
    lua_pushvalue(L, 1);  /* pass error message */
    lua_pushinteger(L, 2);  /* skip this function and traceback */
    lua_call(L, 2, 1);  /* call debug.traceback */
    return 1;
}

