#include "StdAfx.h"
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
#include "ResourceVersionParser.h"

void ResourceVersionParser::Release()
{

}

BOOL ResourceVersionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
    switch( ScriptCode )
    {
    case SCRIPT_RESOURCE_VERSION:
        return _Load( bReload );
    }

    return FALSE;
}

BOOL ResourceVersionParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
    int row_size = GetRowSize();
    if (row_size == 0)
    {
        return FALSE;
    }

    nation_code() = GetDataString("NationCode", 0);
    set_resource_version(GetDataDWORD("ResourceVersion", 0));

    return TRUE;
}

#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT