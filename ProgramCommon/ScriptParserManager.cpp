#include "stdafx.h"
#include "./ScriptParserManager.h"

ScriptParserManager::ScriptParserManager(void)
{
#if defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    ScriptParserManager::StartServerDataLoad();
#endif
}

ScriptParserManager::~ScriptParserManager(void)
{
#if defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    ScriptParserManager::FinishServerDataLoad();
#endif
}

#if defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
void
ScriptParserManager::StartServerDataLoad()
{
    TableData::CreateEnvironment();
}

void
ScriptParserManager::FinishServerDataLoad()
{
    TableData::DestroyEnvironment();
}
#endif

//SolarAuth를 적용시켰을때, FileName과 pParser가 NULL인 경우도 있다.
BOOL ScriptParserManager::RegisterScriptData(
    const eSCRIPT_CODE script_code, 
    const char* file_name,
    IParser* const parser, 
    const eSEPERATOR_MODE seperator_mode,
    const char* pack_file_name, 
    const BOOL is_big_data, 
    SCRIPT_DATA::DestroyParserFunction const destroy_parser_function
) {
    if (FindScript(script_code) != NULL)
    {
        return FALSE;
    }
        
    SCRIPT_DATA* const script = new SCRIPT_DATA;
    script->m_ScriptCode = script_code;
    if (file_name == NULL)
    {
        script->m_szFileName[0] = '\0';
    }        
    else
    {
        strncpy(script->m_szFileName, file_name, _countof(script->m_szFileName));
        script->m_szFileName[_countof(script->m_szFileName) - 1] = '\0';
    }

    ZeroMemory(script->m_szVersion, sizeof(script->m_szVersion));
    script->m_pIParser = parser;
    script->m_SeperatorMode = seperator_mode;

    if (pack_file_name == NULL)
    {
        script->m_szPackFileName[0] = '\0';
    }
    else
    {
        strncpy(script->m_szPackFileName, pack_file_name, _countof(script->m_szPackFileName));
        script->m_szPackFileName[_countof(script->m_szPackFileName) - 1] = '\0';
    }
    script->m_bBigData = is_big_data;

    if (parser != NULL)
    {
        if (script->m_pIParser->InsertFileName(script_code, script->m_szFileName) == FALSE)
        {
            return FALSE;
        }
        if (destroy_parser_function != NULL)
        {
            script->DestroyParser = destroy_parser_function;
        }
    }

    m_listScriptData.push_back(script);

    return TRUE;
}

BOOL
ScriptParserManager::Load()
{
#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    TableData::CreateEnvironment();
#endif

    BOOLEAN is_error = 0;;
    SCRIPT_DATA_LIST::const_iterator it = m_listScriptData.begin(),
                                     end = m_listScriptData.end();
    for( ; it != end; ++it)
    {
        SCRIPT_DATA* pScript = *it;
        if(pScript == 0)
        {
            is_error = TRUE;
            break;
        }
        IParser* pParser = pScript->m_pIParser;
        if(pParser == 0)
            continue;

        // 솔라오스에서 ILoad호출은 여기서 하지 않는다.
        // 각 서버의 마스터 서버 패킷 핸들러에서 한다.
    #ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
        if(!pParser->Load(pScript->m_ScriptCode, pScript->m_szFileName,
                          pScript->m_bBigData, pScript->m_SeperatorMode,
                          pScript->m_szPackFileName, FALSE))
            continue;
    #endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

        strncpy(pScript->m_szVersion, pParser->GetVersion(), _countof(pScript->m_szVersion));
        pScript->m_szVersion[_countof(pScript->m_szVersion) - 1] = '\0';
        //strncpy(pScript->m_szVersion, pParser->GetVersion(), MAX_PATH);
        //pScript->m_szVersion[MAX_PATH] = '\0';
        SUNLOG(eDEV_LOG, "Version = %s", pScript->m_szVersion);
    }

#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    TableData::DestroyEnvironment();
#endif

    if(is_error)
        return FALSE;

    return TRUE;
}

void ScriptParserManager::Release()
{
    for (SCRIPT_DATA_LIST::const_iterator it = m_listScriptData.begin(); 
        it != m_listScriptData.end(); ++it)
    {
        SCRIPT_DATA* const script = *it;
        if (script == NULL)
        {
            continue;
        }
        IParser* const parser = script->m_pIParser;
        if (parser != NULL)
        {
            parser->Release();
        }
        

        if (script->DestroyParser != NULL)
        {
            script->DestroyParser();
        }
        delete script;
    }
    m_listScriptData.clear();

    SUNLOG(eFULL_LOG, "Parser Release Complete");
}

const char*
ScriptParserManager::GetVersion(eSCRIPT_CODE scriptCode)
{
    SCRIPT_DATA* pData = FindScript(scriptCode);
    if(pData == 0)
        return NULL;

    return pData->m_szVersion;
}

const char*
ScriptParserManager::GetFileName(eSCRIPT_CODE scriptCode)
{
    SCRIPT_DATA* pData = FindScript(scriptCode);
    if(pData == 0)
        return NULL;

    return pData->m_szFileName;
}

SCRIPT_DATA*
ScriptParserManager::FindScript(eSCRIPT_CODE scriptCode)
{
    SCRIPT_DATA_LIST::const_iterator it = m_listScriptData.begin(),
                                     end = m_listScriptData.end();
    for( ; it != end; ++it)
    {
        SCRIPT_DATA* pScript = *it;
        if(pScript == 0)
            continue;

        if(pScript->m_ScriptCode == scriptCode)
            return pScript;
    }

    return NULL;
}

SCRIPT_DATA*
ScriptParserManager::FindScript(const char* pszFileName)
{
    SCRIPT_DATA_LIST::const_iterator it = m_listScriptData.begin(),
                                     end = m_listScriptData.end();
    for( ; it != end; ++it)
    {
        SCRIPT_DATA* pScript = *it;
        if(pScript == 0)
            continue;

        if(strcmp(pScript->m_szFileName, pszFileName) == 0)
            return pScript;
    }

    return NULL;
}

//SolarAuth용 데이터 가져오기 함수... 
BOOL
ScriptParserManager::GetScriptDataNodeList(sSCRIPT_DATA_INFO_NODE* pNode, WORD wMaxCount)
{
    WORD wCount = 0;
    SCRIPT_DATA_LIST::const_iterator it = m_listScriptData.begin(),
                                     end = m_listScriptData.end();
    for( ; it != end; ++it)
    {
        sSCRIPT_DATA_INFO_NODE* pData = (sSCRIPT_DATA_INFO_NODE*)*it; // hummmm...
        if(pData == 0)
            continue;

        *(pNode++) = *pData;
        wCount++;

        if(wCount >= wMaxCount)
            return FALSE;
    }

    // 다 되었으면, SIGNAL과 EMPTY의 값도 넣어줘야 한다.
    sSCRIPT_DATA_INFO_NODE nodeSignal, nodeEmpty;
    nodeSignal.m_ScriptCode = SCRIPT_NOTIFY_SIGNAL;
    nodeSignal.m_pIParser = NULL;
    *(pNode++) = nodeSignal;
    nodeEmpty.m_ScriptCode = SCRIPT_EMPTY;
    nodeEmpty.m_pIParser = NULL;
    *(pNode++) = nodeEmpty;

    return TRUE;
}


