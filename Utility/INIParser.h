#ifndef __UTILITY_INI_PARSER_H
#define __UTILITY_INI_PARSER_H
#pragma once

//==================================================================================================
//  (History)
//  - 08.04.28, JsCho, Create
//  - 09.07.12, waverix, code rearrangement
//              note : ����� ������ ������� ���α׷����� �Ǿ� �ִ�.
//                     ��... ������ �ذ��ϰ� �Ѿ��.
//  - 09.07.19, waverix, changes = { change interface 'GetData' }
#include <map>
#include <string>

namespace util {

// (WAVERIX) (090712) (REMOVED) �̷� �������� ����... namespace �ȿ� include�� ...
//#include <map>
//#include <string>

// (WAVERIX) (090712) (CHANGES) �ڷ��� ���� + nested type, (�Ϻ� �۾� ��ũ������ �� ����)

class INIParser
{
public:
    typedef std::map<std::string, std::string>  KEY_MAP;
    typedef std::map<std::string, KEY_MAP*>     APP_MAP;
    static const DWORD INI_DATA_SIZE = 64;
    static const DWORD OverrunCheckMark = 0xDEADBEEF;
    enum INIToken 
    {
        INI_TOKEN_END,
        INI_APP,
        INI_KEY,
        INI_ERROR,
        INI_SUCCESS
    };

    INIParser(void);
    ~INIParser(void);

    bool     OpenFile(const char* pszFileName, const char* pszPackFileName = NULL);
    bool     SetBuffer(const char* pszBuffer, DWORD dwSize);
    bool     INILoad();
    void     Release();
    void     Display();
    // (WAVERIX) (090719) (CHANGES)
    const char* GetData(const char* pszApp, const char* pszKey) const;
    //bool     GetData(const char* pszApp, const char* pszKey, std::string& OUT strData);

private:
    INIToken GetToken();

    char     _getchar();
    INIToken _skipComment();
    INIToken _skipSpace();
    void     _undo_getchar();
    bool     _IsSpace(char ch);
    bool     _IsCRLF(char ch);

    // (Fields)
    APP_MAP m_mapApp;

    // (Instant field) ...
    INIToken        m_CurrentToken;
    char*           m_pszBuffer;
    unsigned int    m_dwBufferPos;
    unsigned int    m_dwBufferSize;
    char    m_szFileName[MAX_PATH];
    //���� ��ū�� �����͸� ����. �̶�� �Ѵܴ�...
#pragma pack(push, 1)
    char  m_szApp[INI_DATA_SIZE];
    const DWORD overrun_checker_00_;
    char  m_szKey[INI_DATA_SIZE];
    const DWORD overrun_checker_01_;
    char  m_szValue[INI_DATA_SIZE];
    const DWORD overrun_checker_02_;
#pragma pack(pop)
    __DISABLE_COPY(INIParser);
};


}; //end of namespace 'util'

#endif //__UTILITY_INI_PARSER_H
