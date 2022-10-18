#include "stdafx.h"
#include "./ProhibitionWordParser.h"
#ifdef _APPLY_NHN_FILTERFING
    #include "IAF_Export.h"
#endif//_APPLY_NHN_FILTERFING

//==================================================================================================

ProhibitionWordParser::ProhibitionWordParser(void)
{
#ifdef __NA_000000_20080221_NATIONAL_FILTER
    #ifdef _JAPAN
        SetNationType(FILTER_JAPAN);
    #endif 
        /*
        #ifdef _CHINA
        SetNationType(FILTER_CHINA);
        #endif
        */

#endif
}

ProhibitionWordParser::~ProhibitionWordParser(void)
{
}

void
ProhibitionWordParser::Init(char* pszPackFileName)
{
    SetPackFileName(pszPackFileName);
}

//BOOL  ProhibitionWordParser::CheckIncludeString(TCHAR* ptszCharName)
//{
//    TCHAR* ptszStr = ptszCharName;
//
//  for (int n = 0; n < (signed)strlen(ptszStr); n++)
//  {
//      switch (ptszStr[n])
//      {
//      case ' ' ://공백 
//      case '\t'://탭은 안된다.
//          return FALSE;
//      }
//
//      if(ptszStr[n]& 0x80)
//      {
//          TCHAR   cBuf1 = ptszStr[n];
//          TCHAR   cBuf2 = ptszStr[n+1];
//
//          USHORT  wBuf = (unsigned short)cBuf1<<8 | cBuf2;
//
//          if(wBuf >= 0xB0A1  &&  wBuf <= 0xC8FE)  //한글    
//          {
//              ;
//          }
//          else if(wBuf >= 0xCAA1  &&  wBuf <= 0xFDFE) //한자 
//          {
//              return FALSE;
//          }
//          else if(wBuf >= 0xA1A1  &&  wBuf <= 0xACFE) //2바이트 특수 문자
//          {
//              return FALSE;
//          }
//          n++;
//      }
//      else if(isdigit(ptszStr[n]))//숫자면 
//      {
//          ;
//      }
//      else if(isalpha(ptszStr[n]))//영문이면
//      {
//          ;
//      }
//      else //기타 
//      {
//          return FALSE;
//      }
//  }
//
//  return TRUE;
//}

BOOL
ProhibitionWordParser::CheckIncludeBanWord(TCHAR* ptszCharName)
{
    TCHAR* ptszTmpCharName = ptszCharName;

    int length = _tcslen(ptszTmpCharName);
    if(length <= 0)
        return FALSE;

    // (WAVERIX_TEST)
    // 일단 수정하기까지 다음과 같이 하자.
    util::cSTRING_REF cStrRD(ptszCharName, length * sizeof(TCHAR));

    if(CheckFilter(cStrRD) == 0)
        return FALSE;
    if(CheckFilterName(cStrRD) == 0)
        return FALSE;

    return TRUE;
}

void
ProhibitionWordParser::CheckIncludeSymbol(TCHAR* ptszMemo)
{
    int nSize = _tcslen(ptszMemo);
    if(nSize <= 0)
        return;

    TCHAR tszNewMemo[MAX_MEMO_LENGTH] = { 0, };

    int nNewCnt = 0;

    for(int n = 0; n < nSize; n++)
    {
        if(ptszMemo[n] == ' ')
        {
            tszNewMemo[nNewCnt] = ' ';
            nNewCnt++;
        }

        //20070522_위 코드 처리시 wBuf에서 깨짐 현상 발생
        //0x80일 경우 한글로 판단하여 2바이트 처리하며 빨리 바꿔줘야한다.
        if((ptszMemo[n] & 0x80) == 0x80)
        {
            for(int i = 0; i < 2; i++)
            {
                tszNewMemo[nNewCnt] = ptszMemo[n];
                nNewCnt++;  
                if(i == 0)  n++;
            }
        }   
        else if(isdigit(ptszMemo[n]))//숫자면 
        {
            tszNewMemo[nNewCnt] = ptszMemo[n];
            nNewCnt++;
        }
        else if(isalpha(ptszMemo[n]))//영문이면
        {
            tszNewMemo[nNewCnt] = ptszMemo[n];
            nNewCnt++;
        }
        else //기타 
        {
            if('\n' == ptszMemo[n])
            {
                tszNewMemo[nNewCnt] = ptszMemo[n];
                nNewCnt++;
            }
        }
    }

    int nMemoLength = _tcslen(tszNewMemo);

    for(nMemoLength; nMemoLength > 0; nMemoLength--)
    {
        if(tszNewMemo[nMemoLength] == ' ')
        {
            tszNewMemo[nMemoLength] = '\0';
        }
        else
        {
            break;
        }
    }

    // TODO(WAVERIX) (090710) (NOTE) 왜 이렇게들 프로그래밍 하는 지 잘 이해가 가지 않는다.
    // 아님, 내가 이상한 건가? 일단 인터페이스 수정하기 전까지는 이대로 두기로 한다.
    // 기억에 의하면 일전에 광수씨 요청으로 엔진쪽 스트링 얻는 함수 관련해서 본 적 있는데,
    // 거기도 그렇던데...
    // 한 마디로, 'sizeof(char) * MAX_MEMO_LENGTH'이 가능한 근거는 뭐냥?
    _tcsncpy(ptszMemo, tszNewMemo, sizeof(char) * MAX_MEMO_LENGTH);
}

void
ProhibitionWordParser::InitPattern()
{
    m_PatternLength = 0;
    ZeroMemory(m_Pattern, sizeof(m_Pattern));
    ZeroMemory(m_PatternFail, sizeof(m_PatternFail));
}

void
ProhibitionWordParser::CalFailFuction()
{   
    m_PatternFail[0] = -1;
    for(int j = 1; j < m_PatternLength; j++)
    {
        int i = m_PatternFail[j-1];
        while((m_Pattern[j] != m_Pattern[i+1]) && (i >= 0))
            i=m_PatternFail[i];
        if(m_Pattern[j] == m_Pattern[i+1])
            m_PatternFail[j] = i+1;
        else
            m_PatternFail[j] = -1;
    }
}

BOOL
ProhibitionWordParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch(scriptCode)
    {
    case SCRIPT_FILTER:
        return _LoadFilter(bReload);
    case SCRIPT_FILTERNAME:
        return _LoadFilterName(bReload);
    }

    ASSERT(!"Invalid Type :Script Code");
    return FALSE;
}

BOOL
ProhibitionWordParser::_LoadFilter(BOOL reloaded)
{
    __UNUSED(reloaded);
    int nRowSize = GetRowSize();
    for(int nRow = 0; nRow < nRowSize; ++nRow)
    {
        STLX_STRING filter = GetDataString("Filter", nRow);
        m_vecFilter.push_back(filter);
    }

    return TRUE;
}

BOOL
ProhibitionWordParser::_LoadFilterName(BOOL reloaded)
{
    __UNUSED(reloaded);
    int nRowSize = GetRowSize();
    for(int nRow = 0; nRow < nRowSize; ++nRow)
    {
        STLX_STRING filter = GetDataString("FilterName", nRow);
        m_vecFilterName.push_back(filter);
    }

    return TRUE;
}

#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
BOOL
ProhibitionWordParser::VerifyCharName(util::cSTRING_REF& IN rCharName, BYTE byLanguageOfFilter)
{
    char* char_name = rCharName.GetBUFFER();
    switch(byLanguageOfFilter)
    {
    case 0: // 영어
        {
            #ifdef __NA_000000_20080221_NATIONAL_FILTER
                SetNationType(FILTER_DEFAULT);
            #endif
            // (WAVERIX) (090803) (WOPS:4463) 북미/글로벌은 'a'~'z', 'A'~'Z', '0'~'9'만 허용하기로 한다.
            // 1st check
            BOOST_STATIC_ASSERT('0' < '9' && 'A' < 'Z' && 'a' < 'z');
            for(const char* it = char_name; *it; ++it) 
            {
                char chr = *it;
                BOOLEAN is_acceptable = ('0' <= chr && chr <= '9') ||
                    ('A' <= chr && chr <= 'Z') ||
                    ('a' <= chr && chr <= 'z');
                if(is_acceptable == 0)
                    return false;
            }
            #ifdef _APPLY_NHN_FILTERFING
            char szTemp[1024];
            int result = IAF::Filtering(char_name, szTemp, _countof(szTemp), (IAF::FILTER_CHARACTER));
            return result == 0;
            #else
            if(m_pITextFilter->CheckIncludeString(char_name))
                return FALSE;
            #endif
        }
        break;

    case 1: // 한국어
        #ifdef __NA_000000_20080221_NATIONAL_FILTER
            SetNationType(FILTER_KOREA);
        #endif
        if(m_pITextFilter->CheckIncludeString(char_name))
            return FALSE;
        break;

    case 2: // 중국어
        #ifdef __NA_000000_20080221_NATIONAL_FILTER
            SetNationType(FILTER_CHINA);
        #endif
        #ifdef __NA_0_20090706_CHINA_VISTA_TEST
        if(m_pITextFilter->CheckIncludeStringJapan(char_name))
            return FALSE;
        #else
        if(m_pITextFilter->CheckIncludeStringChina(char_name))
            return FALSE;
        #endif
        break;

    case 3: // 대만어
        #ifdef __NA_000000_20080221_NATIONAL_FILTER
            SetNationType(FILTER_TAIWAN);
        #endif
        if(m_pITextFilter->CheckIncludeStringTaiwan(char_name))
            return FALSE;
        break;

    case 4: // 일본어
        #ifdef __NA_000000_20080221_NATIONAL_FILTER
            SetNationType(FILTER_JAPAN);
        #endif
        if(m_pITextFilter->CheckIncludeStringJapan(char_name))
            return FALSE;
        break;

    case 5: // 러시아어
        #ifdef __NA_000000_20080221_NATIONAL_FILTER
            SetNationType(FILTER_RUSSIA);
        #endif
        if(m_pITextFilter->CheckIncludeStringRussia(char_name))
            return FALSE;
        break;

    default:
        return FALSE;
    }

    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;
}
#endif // _NA_0_20100929_LANGUAGE_OF_FILTER
// 임시 방편... 일단 작업하려니까 많다...-.-;
BOOL
ProhibitionWordParser::VerifyCharName(util::cSTRING_REF& IN rCharName)
{
    char* char_name = rCharName.GetBUFFER();

#if defined(_KOREA)
    if(m_pITextFilter->CheckIncludeString(char_name))
        return FALSE;
    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;

#elif defined(_CHINA)
  #ifdef __NA_0_20090706_CHINA_VISTA_TEST
    if(m_pITextFilter->CheckIncludeStringJapan(char_name))
        return FALSE;
  #else
    if(m_pITextFilter->CheckIncludeStringChina(char_name))
        return FALSE;
  #endif
    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;

#elif defined(_JAPAN)
    if(m_pITextFilter->CheckIncludeStringJapan(char_name))
        return FALSE;
    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;

#elif defined(_RUSSIA)
    if(m_pITextFilter->CheckIncludeStringRussia(char_name))
        return FALSE;
    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;

#elif defined(_GLOBAL)
    // (WAVERIX) (090803) (WOPS:4463) 북미/글로벌은 'a'~'z', 'A'~'Z', '0'~'9'만 허용하기로 한다.
    // 1st check
    BOOST_STATIC_ASSERT('0' < '9' && 'A' < 'Z' && 'a' < 'z');
    for(const char* it = char_name; *it; ++it) {
        char chr = *it;
        BOOLEAN is_acceptable = ('0' <= chr && chr <= '9') ||
                                ('A' <= chr && chr <= 'Z') ||
                                ('a' <= chr && chr <= 'z');
        if(is_acceptable == 0)
            return false;
    }

  #ifdef _APPLY_NHN_FILTERFING
    char szTemp[1024];
    int result = IAF::Filtering(char_name, szTemp, _countof(szTemp),
                                (IAF::FILTER_CHARACTER));
    return result == 0;
  #else
    if(m_pITextFilter->CheckIncludeString(char_name))
        return FALSE;
    if(CheckIncludeBanWord(char_name) == 0)
        return FALSE;

    return TRUE;
  #endif

#endif
}

// CHANGES: f110826.1L, changes the interface specification
bool ProhibitionWordParser::CheckFilter(
    const TCHAR* null_terminated_string, size_t string_length) const
{
#ifdef _APPLY_NHN_FILTERFING
  #if defined(_GLOBAL) && defined(_SERVER)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
    char szTemp[1024];
    int result = IAF::Filtering(null_terminated_string, szTemp, _countof(szTemp),
                                (IAF::FILTER_CHAT));
    return result == 0;
#else //!_APPLY_NHN_FILTERFING
    TCHAR temp_buffer[1024]; //                max input size
    BOOST_STATIC_ASSERT(_countof(temp_buffer) > MAX_MEMO_LENGTH);
    if (FlowControl::FCAssert(_countof(temp_buffer) > string_length) == false) {
        return false;
    };

    _tcsncpy(temp_buffer, null_terminated_string, _countof(temp_buffer));
    temp_buffer[_countof(temp_buffer) - 1] = _T('\0');
    _tcsupr(temp_buffer);

    FILTER_VEC::const_iterator it = m_vecFilter.begin(),
                               end = m_vecFilter.end();
    for (; it != end; ++it)
    {
        const STLX_STRING& filter_string = (*it);
        const TCHAR* banned_word = filter_string.c_str();

        TCHAR* found = _tcsstr(temp_buffer, banned_word);
        if (found != NULL) {
            return false;
        };
    };

    return true;
#endif //!_APPLY_NHN_FILTERFING
}

// CHANGES: f110826.1L, changes the interface specification
bool ProhibitionWordParser::CheckFilterName(
    const TCHAR* null_terminated_string, size_t string_length) const
{
#ifdef _APPLY_NHN_FILTERFING
    char szTemp[1024];
    int result = IAF::Filtering(null_terminated_string, szTemp, _countof(szTemp),
                                (IAF::FILTER_CHARACTER));
    return result == 0;
#else //!_APPLY_NHN_FILTERFING
    TCHAR temp_buffer[1024]; //                max input size
    BOOST_STATIC_ASSERT(_countof(temp_buffer) > MAX_MEMO_LENGTH);
    if (FlowControl::FCAssert(_countof(temp_buffer) > string_length) == false) {
        return false;
    };

    _tcsncpy(temp_buffer, null_terminated_string, _countof(temp_buffer));
    temp_buffer[_countof(temp_buffer) - 1] = _T('\0');
    _tcsupr(temp_buffer);

    FILTER_NAME_VEC::const_iterator it = m_vecFilterName.begin(),
                                    end = m_vecFilterName.end();
    for (; it != end; ++it)
    {
        const STLX_STRING& filter_string = (*it);
        const TCHAR* banned_word = filter_string.c_str();

        TCHAR* found = _tcsstr(temp_buffer, banned_word);
        if (found != NULL) {
            return false;
        };
    };

    return true;
#endif //!_APPLY_NHN_FILTERFING
}

// to compatible world server (old version)
#if !defined(_NA002676_WORLD_SERVER_RENEWAL) || 1

BOOL
ProhibitionWordParser::CheckFilter(util::cSTRING_REF& rCHRCTL)
{
    if(!rCHRCTL.CopyTo(m_Buffer))
        return FALSE;

    m_Buffer.MakeSafeString();

    DWORD length = (DWORD)m_Buffer.GetSTRING_LENGTH();
    char* pOffset = m_Buffer.GetBUFFER();

#ifdef _APPLY_NHN_FILTERFING
  #if defined(_GLOBAL) && defined(_SERVER)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
    char szTemp[1024];
    int result = IAF::Filtering(pOffset, szTemp, _countof(szTemp),
                                (IAF::FILTER_CHAT));
    return result == 0;
#else //!_APPLY_NHN_FILTERFING
    for(DWORD dwIdx = 0; dwIdx < length; ++dwIdx, ++pOffset) {
        *pOffset = (TCHAR)_totupper(*pOffset);
//     #if defined(_GLOBAL)
//         // add filter : ANSI
//         if((BYTE(*pOffset) < 128) == 0)
//             return false;
//     #endif
    }
    pOffset = m_Buffer.GetBUFFER();

    FILTER_VEC::const_iterator it = m_vecFilter.begin(),
                               end = m_vecFilter.end();
    for( ; it != end; ++it)
    {
        const STLX_STRING& rStr = (*it);
        const TCHAR* ptszTmpBanWord = rStr.c_str();

        //pOffset = m_Buffer.GetBUFFER();
        TCHAR* pDest = _tcsstr(pOffset, ptszTmpBanWord);
        int nResult = (int)(pDest - pOffset + 1);

        if(pDest != NULL)
        {
            return FALSE;
        }
        else
        {
            continue;
        }
    }

    return TRUE;
#endif //!_APPLY_NHN_FILTERFING
}

BOOL
ProhibitionWordParser::CheckFilterName(util::cSTRING_REF& rCHRCTL)
{
    if(!rCHRCTL.CopyTo(m_Buffer))
        return FALSE;

    m_Buffer.MakeSafeString();

#ifdef _APPLY_NHN_FILTERFING
    char szTemp[1024];
    int result = IAF::Filtering(m_Buffer.GetBUFFER(), szTemp, _countof(szTemp),
                                (IAF::FILTER_CHARACTER));
    return result == 0;
#else //!_APPLY_NHN_FILTERFING
    DWORD dwLength = (DWORD)m_Buffer.GetSTRING_LENGTH();
    TCHAR* pOffset = (TCHAR*)m_Buffer.GetBUFFER();

    for(DWORD dwIdx=0 ; dwIdx<dwLength ; ++dwIdx, ++pOffset)
        *pOffset = (TCHAR)_totupper(*pOffset);
    pOffset = m_Buffer.GetBUFFER();

    FILTER_NAME_VEC::const_iterator it = m_vecFilterName.begin(),
                                    end = m_vecFilterName.end();
    for( ; it != end; ++it)
    {
        const STLX_STRING& rStr = (*it);
        const TCHAR* ptszTmpBanWord = rStr.c_str();

        //pOffset = m_Buffer.GetBUFFER();
        TCHAR* pDest = _tcsstr(pOffset, ptszTmpBanWord);
        int nResult = (int)(pDest - pOffset + 1);

        if(pDest != NULL)
        {
            return FALSE;
        }
        else
        {
            continue;
        }
    }

    return TRUE;
#endif //!_APPLY_NHN_FILTERFING
}

#endif //_NA002676_WORLD_SERVER_RENEWAL

HRESULT
ProhibitionWordParser::Create()
{
    return 0;
}

HRESULT
ProhibitionWordParser::Destroy()
{
    return 0;
}
HRESULT
ProhibitionWordParser::Save(const char* szFName)
{
    return 0;
}

HRESULT
ProhibitionWordParser::Load(const char* szFName,bool bNameFilter)
{
    return 0;
}

HRESULT
ProhibitionWordParser::LoadFromPack(const char* szFName,bool bNameFilter)
{
    return 0;
}

char*
ProhibitionWordParser::GetReplaceTarget(int* nLength, char* szSrc)
{
    return NULL;
}

