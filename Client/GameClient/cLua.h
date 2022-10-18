#ifndef __CLUA__
#define __CLUA__

#ifdef _USE_LUA_SCRIPT
struct lua_State;

#define LuaGlue extern "C" int
extern "C" {
#include <Lua_library/lua-5.2.3/src/lua.h>
#include <Lua_library/lua-5.2.3/src/lualib.h>
#include <Lua_library/lua-5.2.3/src/lauxlib.h>

    typedef int (*LuaFunctionType)(struct lua_State *pLuaState);
};

extern "C" {
typedef struct 
{
	const char *name;
	int (*func)(lua_State *);
}luaDef;
}

class cLua
{
public:
	cLua();
	virtual ~cLua();

	bool		RunScript(const char *pFilename);
	bool		RunString(const char *pCommand);
	const char *GetErrorString(void);
	bool		AddFunction(const char *pFunctionName, LuaFunctionType pFunction);
	const char *GetStringArgument(int num, const char *pDefault=NULL);
	double		GetNumberArgument(int num, double dDefault=0.0);
	void		PushString(const char *pString);
	void		PushNumber(double value);

	void		SetErrorHandler(void(*pErrHandler)(const char *pError)) {m_pErrorHandler = pErrHandler;}

	lua_State	*GetScriptContext(void)		{return m_pScriptContext;}

private:
	lua_State	*m_pScriptContext;
	void(*m_pErrorHandler)(const char *pError);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class cLuaMain
{
private:
    static cLuaMain* ms_instance;

private:
    cLua* m_pLua;
    std::vector<util::_tstring> lua_glue_func_vec_;

public:
    cLuaMain();
    ~cLuaMain();

    static cLuaMain* instance() { return ms_instance; }

    cLua* GetLua() { return m_pLua; }

    void InitLua();
    void OpenLuaConsole();
    void ToggleLuaConsole();
    void PrintLuaFuncList();
    void ReleaseLua();
};

#define GET_LUA() cLuaMain::instance()->GetLua()



#endif
#endif