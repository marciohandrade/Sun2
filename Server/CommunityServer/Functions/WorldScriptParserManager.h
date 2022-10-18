#pragma once
#ifndef WORLDSERVER_WORLD_SCRIPT_PARSER_MANAGER_H
#define WORLDSERVER_WORLD_SCRIPT_PARSER_MANAGER_H

#include <ScriptParserManager.h>

namespace ns_world {
;

class WorldScriptParserManager : public ScriptParserManager
{
public:
    WorldScriptParserManager();
    virtual ~WorldScriptParserManager();

    virtual void InitParserPool(void* pool_option);
    virtual BOOL RegisterParser();
    virtual void DestroyInstanceEx();
};

}; //end of namespace

#endif //WORLDSERVER_WORLD_SCRIPT_PARSER_MANAGER_H