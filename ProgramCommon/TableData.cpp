#include "stdafx.h"
#include "./TableData.h"


//==================================================================================================
//==================================================================================================

#define WRAP_CODE_FOR_DEVPARTNER_SKIP(block)    block

#define _SDL_IS_WHITESPACE(chr) ((chr) == ' ' || (chr) == '\t')
#define _SDL_IS_COMMENT(pchr)   (*(pchr) == '/' || *((pchr)+1) == '/')
#define _SDL_IS_SECTOR(chr)     ((chr) == '[')
#define _SDL_IS_EOL(pchr)       (*((pchr)) == '\0') //(*((pchr)+1) == '\0')

#define token_check_by_filter(token_val, filter_ptr)  \
    (filter_ptr[(token_val) >> 3] & (1 << ((token_val) & 7)))

__forceinline char*
TableData::_RawTokenizer(char* pToken, DelimiterBuffer* delimeters)
{   // single thread support
    // reference c-runtime library 'strtok'
    const BYTE* filter = (BYTE*)delimeters;
    static char* next = 0;
    BYTE* str = pToken ? (BYTE*)pToken : (BYTE*)next;

    while(token_check_by_filter(*str, filter) && *str)
        ++str;

    pToken = (char*)str;

    for( ; *str; ++str) {
        if(token_check_by_filter(*str, filter)) {
            *str++ = '\0';
            break;
        }
    }

    next = (char*)str;

    return pToken == (char*)str ? 0 : pToken;
}


TableData::TableData()
    : m_pBuffer(0)
    , m_ColumnCount(0)
    , m_pColumnHash(0)
    , m_LineCount(0)
{
    m_LineRef.pRef = 0;
}

char*
TableData::file_data_buffer_static_ = 0;

BYTE*
TableData::token_stream_static_ = 0;

//DWORD*
//TableData::crc_table_static_ = 0;

TableData::DelimiterBuffer*
TableData::delimiters_static_[2] = { 0, };

TableData::TABLE_NAME_HASH*
TableData::column_hash_static_ = 0;
/*
// changes, f100813.3L, replace an interface to acquire an unique id
//////////////////////////////////////////////////////////////////////////
// reference Unreal3 GameEngine, Epic Games, co.ltd.
// - changes : 처음 사용한 SHA alogrithm hash는 유니크 할 수는 없었다.
//            희박한 확률이라도 Hash중복 가능성이 있었고, 또한 기타 여러
//            가지 면에서도 당근 아래와 같은 코드가 더 효율적이다.
#define _SDL_CRC32_POLYNOMIAL  0x04C11DB7

BOOLEAN
TableData::_MakeHashInit()
{
    for(DWORD crc = 0 ; crc < 256 ; ++crc) {
        for(DWORD chr = (crc << 24), loop = 8 ; loop ; --loop) {
            chr = (chr & 0x80000000)
                ?  (chr << 1) ^ _SDL_CRC32_POLYNOMIAL
                :  (chr << 1)
                ;
            crc_table_static_[crc] = chr;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

DWORD
TableData::MakeHash(const char* const pszString)
{
    const char* it = pszString;
    DWORD crc = DWORD(-1);

    for ( ; *it != '\0'; ++it)
    {
        char  c   = *it;
        DWORD cL  = c & 255;
              crc = (crc << 8) ^ crc_table_static_[ (crc >> 24) ^ cL ];
        DWORD cH  = (c >> 8) & 255;
              crc = (crc << 8) ^ crc_table_static_[ (crc >> 24) ^ cH ];
    }

    return ~crc;
}
*/
BOOLEAN
TableData::CreateEnvironment()
{
    // 최대치 산정해 두자.
    file_data_buffer_static_ = (char*)malloc(MaxScriptDataSize);

    // relation file_size & token_list_buffer_size
    // MaxTokenBlockSize == (file_size / 2) * sizeof(token_info)
    token_stream_static_ = (BYTE*)malloc(MaxTokenStreamSize);
    //crc_table_static_ = (DWORD*)malloc(256 * sizeof(DWORD));

    delimiters_static_[0] = (DelimiterBuffer*)malloc(sizeof(DelimiterBuffer));
    delimiters_static_[1] = (DelimiterBuffer*)malloc(sizeof(DelimiterBuffer));
    ZeroMemory(delimiters_static_[0], sizeof(DelimiterBuffer));
    ZeroMemory(delimiters_static_[1], sizeof(DelimiterBuffer));

#undef SetDelimBit
#define SetDelimBit(chr, slot)  \
    slot[(chr) >> 3] |= 1 << ((chr) & 7)

    DelimiterBuffer& rRowDelim = *delimiters_static_[0];
    SetDelimBit('\r', rRowDelim);
    SetDelimBit('\n', rRowDelim);

    DelimiterBuffer& rColDelim = *delimiters_static_[1];
    SetDelimBit('\t', rColDelim);
    //#undef SetDelimBit

    //_MakeHashInit();
    column_hash_static_ = new TABLE_NAME_HASH;
    column_hash_static_->Initialize(300);
    return true;
}


BOOLEAN
TableData::DestroyEnvironment()
{
    // (BUGFIX) (091119) (WAVERIX) SolarAuth로부터 스크립트를 제공 받는 경우에는
    // 네트워크 연결/재연결 등, 연결 이벤트마다 스크립트 확인 절차를 거치게 되면서
    // 리소스 해제 루틴이 호출되게 된다.
    if(file_data_buffer_static_ == 0)
        return true;

    free(file_data_buffer_static_);
    file_data_buffer_static_ = 0;

    free(token_stream_static_);
    token_stream_static_ = 0;

    //free(crc_table_static_);
    //crc_table_static_ = 0;

    free(delimiters_static_[0]);
    free(delimiters_static_[1]);

    delete column_hash_static_;

    return true;
}


BOOLEAN
TableData::_OpenFileAndReadBuffer(const char* fileName,
                                  BOOLEAN use_packfile, const char* packFileName)
{
    if(fileName == NULL)
        return FALSE;

    if(use_packfile == 0)
    {
        HANDLE fileHandle = ::CreateFile(fileName,
                                        GENERIC_READ,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        DWORD fileSize = ::GetFileSize(fileHandle, NULL);
        DWORD paddingFileSize = fileSize + 10;

        if(!FlowControl::FCAssert(paddingFileSize <= MaxScriptDataSize))
            return FALSE;

        m_pBuffer = file_data_buffer_static_;
        DWORD OUT readedSize = 0;
        ::ReadFile(fileHandle, m_pBuffer, fileSize, &readedSize, NULL);
        ZeroMemory(m_pBuffer + readedSize, paddingFileSize - readedSize);
        ::CloseHandle((HANDLE)fileHandle);
    }
    else
    {
        util::SolarFileReader sr;
        BOOLEAN result = !!sr.OpenFileFromPack(const_cast<char*>(packFileName),
                                               const_cast<char*>(fileName),
                                               SEPERATOR_WHITESPACE_FOR_COMMENTS);
        if(result == 0)
        {
            SUNLOG(eCRITICAL_LOG, "File[%s] Open Error", fileName);
            //char szError[MAX_PATH + 1];
            //_snprintf(szError, MAX_PATH, "File[%s] Open Error \n", pszFileName);
            //SUNLOG(eCRITICAL_LOG, szError);
            ASSERT(!"File Open Error");
            return FALSE;
        }
        
        const char* buffer = sr.GetBuffer();
        DWORD fileSize = sr.GetBufferSize();
        DWORD paddingFileSize = fileSize + 10;

        if(!FlowControl::FCAssert(paddingFileSize <= MaxScriptDataSize))
            return FALSE;

		m_pBuffer = file_data_buffer_static_;
        CopyMemory(m_pBuffer, buffer, fileSize);
        ZeroMemory(m_pBuffer + fileSize, paddingFileSize - fileSize);

		sr.CloseFile();
    }
    return TRUE;
}


BOOLEAN
TableData::_UnLoad()
{
    m_ColumnCount = 0;
    m_pBuffer = 0;

    util::LList::Init(&m_Rows);
    if(m_pColumnHash)
        m_pColumnHash = NULL;

    m_LineRef.count = 0;
    if(m_LineRef.pRef)
        m_LineRef.pRef = 0;
    m_LineCount = 0;

    return TRUE;
}

BOOLEAN
TableData::Load(eSCRIPT_CODE script_code, const TCHAR* pszFileName,
                BOOLEAN use_packfile, const char* packFileName)
{
    if(_OpenFileAndReadBuffer(pszFileName, use_packfile, packFileName) == FALSE)
    {
        //printf("error --
        return FALSE;
    }

    BOOLEAN bRet = _BuildDictionary(script_code);
    return bRet;
}

BOOLEAN
TableData::LoadBuffer(eSCRIPT_CODE script_code, const BYTE* pBuffer, const DWORD buffer_size)
{
    if(!(pBuffer && buffer_size))
        return FALSE;

    const DWORD alloced_size = buffer_size + !!(pBuffer[buffer_size-1] != '\0');
    if(!FlowControl::FCAssert(alloced_size <= MaxScriptDataSize))
        return FALSE;

    m_pBuffer = file_data_buffer_static_;
    CopyMemory(m_pBuffer, pBuffer, buffer_size);
    m_pBuffer[alloced_size - 1] = '\0';

    BOOLEAN bRet = _BuildDictionary(script_code);
    return bRet;
}


void
TableData::RegisterColumnDelimiters(const char* delimiters)
{
    ZeroMemory(delimiters_static_[1], sizeof(DelimiterBuffer));
    DelimiterBuffer& rColDelim = *delimiters_static_[1];
    for(const char* it = delimiters; *it; ++it) {
        char delim = *it;
        SetDelimBit(delim, rColDelim);
    }
}

BOOLEAN
TableData::_BuildDictionary(eSCRIPT_CODE script_code)
{
    BYTE* pTokenOffset = token_stream_static_;

    DWORD64* const pTokenTailMark =
        (DWORD64*)&token_stream_static_[MaxTokenStreamSize - sizeof(DWORD64)];
    *pTokenTailMark = 0xDEADBEEFDEADBEEFull;

    {   // 스크립트 특유의 구분자 설정
#ifdef _NA_0_20100312_MODIFY_DELIMITERS_FOR_TEXTSCRIPT
        const char* delimiters_text = "\t";
#else
        const char* delimiters_text = "\"\t";
#endif        
        const char* delimiters = " \t"; // { space, tab }
        switch(script_code)
        {
        case SCRIPT_QUEST_TEXT_INFO:
        case SCRIPT_HELP_MESSAGE:
#ifdef _DH_DIVIDE_MESSAGE_TEXT
        case SCRIPT_MESSAGE_TEXT:
#else
        case SCRIPT_MESSAGE:
#endif//_DH_DIVIDE_MESSAGE_TEXT
        case SCRIPT_FILTER:
        case SCRIPT_FILTERNAME:
#ifdef _DH_DISPLAY_FOR_MISSION_OBJECT
		case SCRIPT_MISSION_TEXT_INFO:
#endif//_DH_DISPLAY_FOR_MISSION_OBJECT
#ifdef _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
        case SCRIPT_TOTAL_GUIDE_MESSAGE:
#endif //_NA_002521_20110613_TOTAL_GUIDE_SYSTEM
        case SCRIPT_HOT_KEY_INFO:
		case SCRIPT_MISSIONHELP_INFO:
        case SCRIPT_OPTION_CATEGORY_INFO:
            delimiters = delimiters_text;
            break;
        }
        RegisterColumnDelimiters(delimiters);
    };
//////////////////////////////////////////////////////////////////////////
//WRAP_CODE_FOR_DEVPARTNER_SKIP(
    {   // Build Row Tokenizing
        util::LList::Init(&m_Rows);
        char* pToken = _RawTokenizer(m_pBuffer, delimiters_static_[0]);
        while(pToken != 0)
        {
            LineListNode* const pLine = reinterpret_cast<LineListNode*>(pTokenOffset);
            pTokenOffset += sizeof(LineListNode);

            util::LList::Init(pLine);
            LinePair& rPair = pLine->linePair;
            rPair.pLinePtr = pToken;
            rPair.pColumns = 0;
            util::LList::AddPrev(pLine, &m_Rows);
            pToken = _RawTokenizer(0, delimiters_static_[0]);
            ++m_LineCount;
        }
    };
//); //WRAP_CODE_FOR_DEVPARTNER_SKIP
//////////////////////////////////////////////////////////////////////////
//WRAP_CODE_FOR_DEVPARTNER_SKIP(
    {   // Build Col Tokenizing
        // CHANGES: f110415.5L, we need line comparision based on column schemes.
        bool check_first_line = true;
        LineListNode* it = static_cast<LineListNode*>(m_Rows.next);
        while (it != &m_Rows)
        {
            LinePair& rPair = it->linePair;
            ColInfo* const pCols = reinterpret_cast<ColInfo*>(pTokenOffset);

            rPair.pColumns = pCols;
            pCols->count = 0;

            char* pOffset = rPair.pLinePtr;
            BOOLEAN bEOL = _SDL_IS_EOL(pOffset);
            char* pToken = bEOL ? 0 : _RawTokenizer(pOffset, delimiters_static_[1]);
            while(pToken != 0)
            {
                if(_SDL_IS_COMMENT(pToken))
                    break;
                pCols->pRef[pCols->count++] = pToken;
                pToken = _RawTokenizer(0, delimiters_static_[1]);
            }

            if(bEOL)
                break;

            if(pCols->count == 0)
            {
                it = util::LList::Delete(it);
                --m_LineCount;
                continue;
            }
            // CHANGES: f110415.5L, we need line comparision based on column schemes.
            if (check_first_line) {
                m_ColumnCount = pCols->count;
                check_first_line = false;
            };

            while(_SDL_IS_WHITESPACE(*pOffset))
                ++pOffset;

            if(_SDL_IS_COMMENT(pOffset))
            {
                it = util::LList::Delete(it);
                --m_LineCount;
                continue;
            }

            //pTokenOffset += sizeof(pCols->count) + (sizeof(char*) * pCols->count);
            pTokenOffset += sizeof(pCols->count) + (sizeof(char*) * m_ColumnCount);
            it = it->next;
        }
    };
//); //WRAP_CODE_FOR_DEVPARTNER_SKIP
//////////////////////////////////////////////////////////////////////////
    // Buffer Overrun이 발생하지 않아야 하며, (발생한다면, 실제 데이터가 대단히 커졌을 가능성 있음)
    // 데이터가 또한 없으면 안된다.
    if(!FlowControl::FCAssert(pTokenOffset < (BYTE*)pTokenTailMark &&
                              util::LList::IsEmpty(&m_Rows) == FALSE))
        return FALSE;
//////////////////////////////////////////////////////////////////////////
    {   // parse & make hash column name
        m_pColumnHash = column_hash_static_;// new SolarHashTable<INT>;
        m_pColumnHash->RemoveAll();
    #if SOLAR_HASH_TABLE_VERSION < 0x0200
        m_pColumnHash->Initialize(m_ColumnCount);
    #endif
        LineListNode* columnLine = static_cast<LineListNode*>(m_Rows.next);
        LinePair& rPair = columnLine->linePair;

        char* pOffset = rPair.pLinePtr; //for debug
        ColInfo* pCols = rPair.pColumns;

        int counter = 0;
        char **it = &pCols->pRef[0], **itend = &pCols->pRef[pCols->count];
        for( ; it != itend; ++it)
        {
            char* pToken = *it;
            if(pToken[0] == 'F' && (strncmp("FieldEnd", pToken, 10) == 0))
                break;
//WRAP_CODE_FOR_DEVPARTNER_SKIP(
            DWORD hash = util::StreamHashKey::GetStringHashKey(pToken);
            INT value = m_pColumnHash->GetData(hash);
            ASSERT(!value);
            m_pColumnHash->Add(++counter, hash);	// 0이 아닌 값으로 설정
//); //WRAP_CODE_FOR_DEVPARTNER_SKIP
        }

        util::LList::Delete(columnLine);
        --m_LineCount;

        if(m_ColumnCount != counter + 1) {
            // critical error
        }
        else if(m_LineCount == 0) {
            --m_ColumnCount;
        }
        else {
            m_ColumnCount = counter;
        }
    };
//////////////////////////////////////////////////////////////////////////
//WRAP_CODE_FOR_DEVPARTNER_SKIP(
    static ColInfo* pCols_debug;
    pCols_debug = 0;
    {   // make serialized array index table
        m_LineRef.count = m_LineCount;
        m_LineRef.pRef = reinterpret_cast<ColInfo**>(pTokenOffset);
        pTokenOffset += sizeof(ColInfo*) * m_LineRef.count;

        LineListNode* it = static_cast<LineListNode*>(m_Rows.next);
        for(int lineNo = 0; it != &m_Rows; it = it->next)
        {
            LinePair& rPair = it->linePair;
            ColInfo* pCols = rPair.pColumns;
            m_LineRef.pRef[lineNo++] = pCols;
            if(m_ColumnCount != pCols->count) {
                pCols_debug = pCols;
            #ifdef _SERVER
                SUNLOG(eCRITICAL_LOG,
                       "Script=%d|Column0=%s|Column1=%s|Msg=Column mismatched, Check script data|",
                       script_code, pCols->pRef[0], pCols->pRef[1]);
            #endif
                // (WARNING) 여기서 문제가 발생하면, 'pCols_debug'에 기재된 레코드에 column에 문제가
                // 있는 겁니다. (누락된 column또는 더 많은 입력이 일어난 상태)
                // - parity check methodology
                __debugbreak();
            }
        }
    }
//); //WRAP_CODE_FOR_DEVPARTNER_SKIP
//////////////////////////////////////////////////////////////////////////
    // Buffer overrun이 발생해서는 안된다. 발생한다면, 실제 데이터가 대단히 커졌을 가능성 있음.
    return FlowControl::FCAssert(pTokenOffset < (BYTE*)pTokenTailMark &&
                                 *pTokenTailMark == 0xDEADBEEFDEADBEEFull);
}

//==================================================================================================
//==================================================================================================

