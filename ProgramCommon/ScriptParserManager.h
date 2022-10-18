#ifndef __SCRIPT_PARSER_MANAGER_H
#define __SCRIPT_PARSER_MANAGER_H
#pragma once

//==================================================================================================
//  (History)
//  - 07.02.15, JsCho, Create for for New Script Loading System
//                   , __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
//  - 09.07.12, waverix, code rearrangement
//                     , delete working macro '__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM'

//#include <list>
#include "Struct.h"
#include "ScriptCode.IParser.h"

#define ERROR_PARSER(a)          \
    if(!(a)) {                   \
        printf("Parser Failed"); \
        return FALSE;            \
    }

//나중에 요놈은 Singleton으로 처리한다.
class ScriptParserManager
{
public:
    ScriptParserManager();
    virtual ~ScriptParserManager(); // NOTE: f110519.7L, added virtual keyword

    // GAMESERVER에 특화됨.. 각 서버에 특화된 파서 로드 함수를 여기서 선언.
    virtual void InitParserPool(void* pPoolOption) = 0;
    virtual BOOL RegisterParser() = 0;
    virtual void DestroyInstanceEx() = 0;

    BOOL    Load(); // 파서 로딩.
    void    Release();
    SCRIPT_DATA* FindScript(eSCRIPT_CODE scriptCode);
    SCRIPT_DATA* FindScript(const char* pszFileName);
    const char*  GetVersion(eSCRIPT_CODE scriptCode);
    const char*  GetFileName(eSCRIPT_CODE scriptCode);

#if defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    static void StartServerDataLoad();
    static void FinishServerDataLoad();
#endif

protected:
    BOOL RegisterScriptData(
        const eSCRIPT_CODE scriptCode, 
        const char* pszFileName,
        IParser* const pParser, 
        const eSEPERATOR_MODE seperatorMode = SEPERATOR_WHITESPACE,
        const char* pszPackFileName = NULL, 
        const BOOL bBigData = FALSE, 
        SCRIPT_DATA::DestroyParserFunction const destroy_parser_function = NULL
    );

    template <typename PARSER_T>
    bool RegisterScript(const eSCRIPT_CODE script_code, const char* file_name)
    {
        if (RegisterScriptData(
            script_code, 
            file_name, 
            PARSER_T::Instance(), 
            SEPERATOR_WHITESPACE, 
            NULL, 
            FALSE, 
            &PARSER_T::DestroyInstance) == FALSE)
        {
            printf("|Failed to register script.|ScriptCode = %d|FileName = %s|", script_code, file_name);
            return false;
        }
        return true;
    }

    BOOL    GetScriptDataNodeList(sSCRIPT_DATA_INFO_NODE* pNode, WORD wMaxCount);

private:
    typedef STLX_LIST<SCRIPT_DATA*>  SCRIPT_DATA_LIST;
    SCRIPT_DATA_LIST m_listScriptData;
};

#endif //__SCRIPT_PARSER_MANAGER_H
