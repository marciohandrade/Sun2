#pragma once
#ifndef DBPROXY_SCRIPTPARSERMANAGER_H
#define DBPROXY_SCRIPTPARSERMANAGER_H

#include <ScriptParserManager.h>

//==================================================================================================
//
class DBProxyScriptParserManager : public ScriptParserManager
{
public:
    DBProxyScriptParserManager();
    ~DBProxyScriptParserManager();

public:
    virtual void InitParserPool(void* pool_option);
    virtual BOOL RegisterParser();
    virtual void DestroyInstanceEx();
};

#endif //DBPROXY_SCRIPTPARSERMANAGER_H