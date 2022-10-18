#pragma once
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

class ResourceVersionParser : public util::Singleton<ResourceVersionParser>, public IParser
{
public:
    ResourceVersionParser() {}
    ~ResourceVersionParser() {}

    virtual void Release();

public:
    BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

    STLX_STRING& nation_code() { return nation_code_; }
    DWORD resource_version() const { return resource_version_; }

private:
    BOOL _Load( BOOL bReload );

    void set_resource_version(DWORD value) { resource_version_ = value; }

private:
    STLX_STRING nation_code_;
    DWORD resource_version_;
};

#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT