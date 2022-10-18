#ifndef __PROHIBITION_WORD_PARSER_H
#define __PROHIBITION_WORD_PARSER_H
#pragma once
//==================================================================================================
//  (History)
//  - 09.07.10, waverix, code rearrangement
//                     , move from global namespace to class nested type
//

//--------------------------------------------------------------------------------------------------
//  (Utility)
#include <Singleton.h>
#include <SolarFileReader.h>
#include <TextFilter.h>
#include <SafeStringBufferControl.h>
//--------------------------------------------------------------------------------------------------
//  (ProgramCommon)
#include <Struct.h>
#include <ScriptCode.IParser.h>

//#define MAX_PATTERN_LENGTH 20 // (WAVERIX) (090710) (CHANGES)
//static int m_nBanWordCnt = 0; // (WAVERIX) (090710) (REMOVED)

//==================================================================================================
//
class ProhibitionWordParser : public util::Singleton<ProhibitionWordParser>,
                              public IParser,
                              public ITextFilter
{
    static const int MAX_PATTERN_LENGTH = 20; // (CHANGES) define -> const
    typedef STLX_VECTOR<STLX_STRING>  BAN_WORD_VEC;
    typedef STLX_VECTOR<STLX_STRING>  FILTER_VEC;
    typedef STLX_VECTOR<STLX_STRING>  FILTER_NAME_VEC;
public:
    ProhibitionWordParser(void);
    virtual ~ProhibitionWordParser(void);

    void    Init(char* pszPackFileName);
    void    Release() {}

    void    Unload();

    //----------------------------------------------------------------------------------------------
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    BOOL  VerifyCharName(util::cSTRING_REF& IN rCharName, BYTE byLanguageOfFilter);
#endif
    BOOL  VerifyCharName(util::cSTRING_REF& IN rCharName);
    void  CalFailFuction();
    void  InitPattern();

    //BOOL CheckIncludeString(TCHAR* ptszCharName);  //특수문자 검사
    BOOL  CheckIncludeBanWord(TCHAR* ptszCharName);  //금지문자 검사
    void  CheckIncludeSymbol(TCHAR* ptszMemo);

    virtual HRESULT Create();
    virtual HRESULT Destroy();
    virtual HRESULT Save(const char* szFName);
    virtual HRESULT Load(const char* szFName, bool bNameFilter = true);
    virtual HRESULT LoadFromPack(const char* szFName, bool bNameFilter = true);

    // IParser interface
    virtual BOOL  LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
    // CHANGES: f110826.1L, changes the interface specification
    template<size_t _Size>
    bool CheckFilterName(const TCHAR (&null_terminated_string)[_Size]) const;
    // CHANGES: f110826.1L, changes the interface specification
    template<size_t _Size>
    bool CheckFilter(const TCHAR (&null_terminated_string)[_Size]) const;
#if !defined(_NA002676_WORLD_SERVER_RENEWAL) || 1// to compatible world server (old version)
    BOOL  CheckFilter(util::cSTRING_REF& IN rCHRCTL);
    BOOL  CheckFilterName(util::cSTRING_REF& IN rCHRCTL);
#endif

protected:
    virtual char* GetReplaceTarget(int* nLength, char* szSrc);

private:
    BOOL  _LoadFilter(BOOL bReload);
    BOOL  _LoadFilterName(BOOL bReload);

    // CHANGES: f110826.1L, changes the interface specification
    bool CheckFilterName(const TCHAR* null_terminated_string, size_t string_length) const;
    // CHANGES: f110826.1L, changes the interface specification
    bool CheckFilter(const TCHAR* null_terminated_string, size_t string_length) const;

    //----------------------------------------------------------------------------------------------
    // (Fields)
    ITextFilter*    m_pITextFilter;
    BAN_WORD_VEC    m_vecBanWord;
    FILTER_VEC      m_vecFilter;
    FILTER_NAME_VEC m_vecFilterName;
    util::cSTRING_REF_RW_WITH_BUFFER<MAX_MEMO_LENGTH + 1> m_Buffer;

    // UNUSED SECTION?
    BYTE   m_PatternLength;
    TCHAR  m_Pattern[MAX_PATTERN_LENGTH + 1];
    TCHAR  m_PatternFail[MAX_PATTERN_LENGTH + 1];
    //

    // 한번 사용하면 쓸모 없는 것 저~ 멀리 치워두자.
    TCHAR  m_pszFileName[MAX_PATH];
};

//==================================================================================================
// CHANGES: f110826.1L, changes the interface specification
template<size_t _Size>
bool ProhibitionWordParser::CheckFilterName(const TCHAR (&null_terminated_string)[_Size]) const
{
    BOOST_STATIC_ASSERT(_Size != 0);
    if (null_terminated_string[_Size - 1] != _T('\0')) {
        return false;
    };
    return CheckFilterName(null_terminated_string, ::_tcslen(null_terminated_string));
};

// CHANGES: f110826.1L, changes the interface specification
template<size_t _Size>
bool ProhibitionWordParser::CheckFilter(const TCHAR (&null_terminated_string)[_Size]) const
{
    BOOST_STATIC_ASSERT(_Size != 0);
    if (null_terminated_string[_Size - 1] != _T('\0')) {
        return false;
    };
    return CheckFilter(null_terminated_string, ::_tcslen(null_terminated_string));
}


#endif //__PROHIBITION_WORD_PARSER_H
