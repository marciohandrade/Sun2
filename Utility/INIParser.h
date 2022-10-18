#ifndef __UTILITY_INI_PARSER_H
#define __UTILITY_INI_PARSER_H
#pragma once

//==================================================================================================
//  (History)
//  - 08.04.28, JsCho, Create
//  - 09.07.12, waverix, code rearrangement
//              note : 상당히 위험한 방식으로 프로그래밍이 되어 있다.
//                     뭐... 적당히 해결하고 넘어가자.
//  - 09.07.19, waverix, changes = { change interface 'GetData' }
#include <map>
#include <string>

namespace util {

// (WAVERIX) (090712) (REMOVED) 이런 비상식적인 일을... namespace 안에 include는 ...
//#include <map>
//#include <string>

// (WAVERIX) (090712) (CHANGES) 자료형 변경 + nested type, (일부 작업 매크로제거 차 변경)

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
    //현재 토큰의 데이터를 저장. 이라고 한단다...
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
