#ifndef _STATEINFOPARSER_H_
#define _STATEINFOPARSER_H_
#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

struct BASE_STATEINFO;

//==================================================================================================

class StateInfoParser : public util::Singleton<StateInfoParser>, public IParser
{
public:
    StateInfoParser();
    virtual ~StateInfoParser();

    void Init(DWORD dwPoolSize);
    void Init(DWORD dwPoolSize, char* pszPackFileName);
    void Release();

    void Reload();
    void Unload();

    BASE_STATEINFO* GetStateInfo(CODETYPE dwStateCode);

public:
    BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

private:
    BOOL _Load(BOOL bReload);

private:
    util::SolarHashTable<BASE_STATEINFO *>* m_pStateInfoHashTable;
    char m_pszFileName[MAX_PATH];
};


inline BASE_STATEINFO*
StateInfoParser::GetStateInfo(CODETYPE dwStateCode) {
    return m_pStateInfoHashTable->GetData(dwStateCode);
}

#endif // _STATEINFOPARSER_H_