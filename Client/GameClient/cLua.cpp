#include "SunClientPrecompiledHeader.h"
#include <cLua.h>

#ifdef _USE_LUA_SCRIPT

cLua::cLua()
{
	m_pErrorHandler = NULL;

	m_pScriptContext = luaL_newstate();
	luaopen_base(m_pScriptContext);
	luaopen_io(m_pScriptContext);
	luaopen_string(m_pScriptContext);
	luaopen_math(m_pScriptContext);
	luaopen_debug(m_pScriptContext);
	luaopen_table(m_pScriptContext);
}

cLua::~cLua()
{
	if(m_pScriptContext)
		lua_close(m_pScriptContext);
}

static std::string findScript(const char *pFname)
{
	FILE *fTest;

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( pFname, drive, dir, fname, ext );

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    std::string strTestFile = (std::string) "Data\\Scripts\\" + dir + fname + ".LUA";
    fTest = fopen(strTestFile.c_str(), "r");
#else
	std::string strTestFile = (std::string) drive + dir + "Scripts\\" + fname + ".LUB";
	fTest = fopen(strTestFile.c_str(), "r");
	if(fTest == NULL)
	{
		//not that one...
		strTestFile = (std::string) drive + dir + "Scripts\\" + fname + ".LUA";
		fTest = fopen(strTestFile.c_str(), "r");
	}

	if(fTest == NULL)
	{
		//not that one...
		strTestFile = (std::string) drive + dir + fname + ".LUB";
		fTest = fopen(strTestFile.c_str(), "r");
	}

	if(fTest == NULL)
	{
		//not that one...
		//not that one...
		strTestFile = (std::string) drive + dir + fname + ".LUA";
		fTest = fopen(strTestFile.c_str(), "r");
	}
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

	if(fTest != NULL)
	{
		fclose(fTest);
	}

	return strTestFile;
}



bool cLua::RunScript(const char *pFname)
{
	std::string strFilename = findScript(pFname);
	const char *pFilename = strFilename.c_str();

	if (0 != luaL_loadfile(m_pScriptContext, pFilename))
	{
		if(m_pErrorHandler)
		{
			char buf[256] = {0,};
			_snprintf(buf
				   , 256-1
				   , "Lua Error - Script Load\nScript Name:%s\nError Message:%s\n"
				   , pFilename
				   , luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		if(m_pErrorHandler)
		{
			char buf[256] = {0,};
			_snprintf(buf
				  , 256-1
				  , "Lua Error - Script Run\nScript Name:%s\nError Message:%s\n"
				  , pFilename
				  , luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	return true;

}

bool cLua::RunString(const char *pCommand)
{
	if (0 != luaL_loadbuffer(m_pScriptContext, pCommand, strlen(pCommand), NULL))
	{
		if(m_pErrorHandler)
		{
			char buf[256] = {0,};
			_snprintf(buf
				   , 256-1
				   , "Lua Error - String Load\nString:%s\nError Message:%s\n"
				   , pCommand
				   , luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	if (0 != lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0))
	{
		if(m_pErrorHandler)
		{
			char buf[256] = {0,};
			_snprintf(buf
				   , 256-1
				   , "Lua Error - String Run\nString:%s\nError Message:%s\n"
				   , pCommand
				   , luaL_checkstring(m_pScriptContext, -1));
			m_pErrorHandler(buf);
		}

		return false;
	}
	return true;
}

const char *cLua::GetErrorString(void)
{
	return luaL_checkstring(m_pScriptContext, -1);
}


bool cLua::AddFunction(const char *pFunctionName, LuaFunctionType pFunction)
{
	lua_register(m_pScriptContext, pFunctionName, pFunction);
	return true;
}

const char *cLua::GetStringArgument(int num, const char *pDefault)
{
	return luaL_optstring(m_pScriptContext, num, pDefault);

}

double cLua::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

void cLua::PushString(const char *pString)
{
	lua_pushstring(m_pScriptContext, pString);
}

void cLua::PushNumber(double value)
{
	lua_pushnumber(m_pScriptContext, value);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "WinConsole.h"

// 루아 라이브러리는 유저배포용에서는 링크하지않음 'ㅅ'
// _USE_LUA_SCRIPT 디파인을 참고
#ifdef _DEBUG
#pragma comment( lib, "../../lib/"VC_DIR_REDIRECT"/lua_d.lib" )
#else
#pragma comment( lib, "../../lib/"VC_DIR_REDIRECT"/lua.lib" )
#endif

extern luaDef SunGlue[];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cLuaMain* cLuaMain::ms_instance = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cLuaMain::cLuaMain()
: m_pLua(NULL)
{
    ms_instance = this;
}

cLuaMain::~cLuaMain()
{
    ReleaseLua();
    ms_instance = NULL;
}

bool LuaFuncNameSort(util::_tstring a, util::_tstring b)
{
    return a < b;
}

void cLuaMain::InitLua()
{
    m_pLua = new cLua;
    lua_glue_func_vec_.clear();

    //// 루아 글루 함수 등록 
    for(int i=0; SunGlue[i].name; i++)
    {
        TCHAR func_name[256] = {0,};
        StrnCopy(func_name, SunGlue[i].name, strlen(SunGlue[i].name));
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
#else
        _strlwr(func_name);
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
        m_pLua->AddFunction(func_name, SunGlue[i].func);
        lua_glue_func_vec_.push_back(func_name);
    }
    sort(lua_glue_func_vec_.begin(), lua_glue_func_vec_.end(), LuaFuncNameSort);
}

void cLuaMain::PrintLuaFuncList()
{
    size_t vector_size = lua_glue_func_vec_.size();
    for(size_t i = 0; i < vector_size; ++i)
    {
        CWinConsole::Write(lua_glue_func_vec_[i].c_str());
        CWinConsole::Write("\n");   
    }
}

void cLuaMain::OpenLuaConsole()
{
    CWinConsole::StartConsole(g_hInstance, this->m_pLua);
}

void cLuaMain::ToggleLuaConsole()
{
    if (CWinConsole::ConsoleReady())
    {
        CWinConsole::CloseConsole();
    }
    else
    {
        CWinConsole::StartConsole(g_hInstance, this->m_pLua);
    }
}

void cLuaMain::ReleaseLua()
{
    lua_glue_func_vec_.clear();
    SAFE_DELETE( m_pLua );
    CWinConsole::StopConsole();
}

#endif