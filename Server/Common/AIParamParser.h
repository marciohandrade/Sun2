#ifndef __COMMON_AIPARAMPARSER_H__
#define __COMMON_AIPARAMPARSER_H__
#pragma once


#include <SolarHashTable.h>
#include <Singleton.h>
#include <ServerStruct.h>

// @history
// ~09/12/23
// 09/12/23|waverix|code-arrangement, change STLX_MAP->STLX_HASHMAP|

class AIParamParser : public util::Singleton<AIParamParser>, public IParser
{
public:
    typedef WORD AICODE_T;

    AIParamParser();
    ~AIParamParser();

    void Init(DWORD dwPoolSize);

    BOOL LoadCommon(char* pszFileName, BOOL bReload = false);
    BOOL LoadType(char* pszFileName, BOOL bReload = false);
    void Reload();
    void Release();

    // (NOTE) (091223) (WAVERIX) AICommonInfo: const 의 형태로 일괄 변환 후 제거하도록 유도하자.
    inline AI_COMMON_INFO&  GetCommonInfo() { return m_AICommonInfo; }

    AI_TYPE_INFO* GetTypeInfo(AICODE_T aiCode);
    const AI_TYPE_INFO* GetTypeInfo(AICODE_T aiCode) const;

    BOOL CheckNPCInfoParser();

    BOOL _LoadCommon(BOOL bReload);
    BOOL _LoadType(BOOL bReload);
    virtual BOOL LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload);

    // (CHANGES) (WAVERIX) add const reference for property i/o
    static const AI_COMMON_INFO&  AICommonInfo;
private:
    typedef STLX_HASH_MAP<AICODE_T, AI_TYPE_INFO> AI_CODE_TABLE;
    AI_CODE_TABLE  m_AITypeTable;
    AI_COMMON_INFO m_AICommonInfo; // compatible interface
    char m_pszCommonFileName[MAX_PATH];
    char m_pszTypeFileName[MAX_PATH];
};

//==================================================================================================

inline AI_TYPE_INFO*
AIParamParser::GetTypeInfo(AICODE_T aiCode) {
    AI_CODE_TABLE::iterator it = m_AITypeTable.find(aiCode);
    if(it != m_AITypeTable.end()) {
        AI_TYPE_INFO& rInfo = it->second;
        return &rInfo;
    }
    return NULL;
}

inline const AI_TYPE_INFO*
AIParamParser::GetTypeInfo(AICODE_T aiCode) const {
    AI_CODE_TABLE::const_iterator it = m_AITypeTable.find(aiCode);
    if(it != m_AITypeTable.end()) {
        const AI_TYPE_INFO& rInfo = it->second;
        return &rInfo;
    }
    return NULL;
}

#endif //__COMMON_AIPARAMPARSER_H__

