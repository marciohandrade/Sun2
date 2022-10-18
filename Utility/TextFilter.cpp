#include "UtilityCommon.h"
#include "../ProgramCommon/Define.h"
#include "SolarFileReader.h"
#include ".\TextFilter.h"
#ifdef _APPLY_NHN_FILTERFING
	#include "IAF_Export.h"
#endif//_APPLY_NHN_FILTERFING

namespace util {
;

struct TextTempBuffer;
struct TextTempBufferStack
{
    static const int kMaxDepths = 4;
    struct Buffer
    {
        Buffer()
            : text_length_(0),
              text_buffer_(NULL)
        {
        }
        ~Buffer()
        {
            if (text_buffer_)
            {
                delete [] text_buffer_;
                text_buffer_ = NULL;
            }
        }
        void Resize(size_t text_length)
        {
            if (text_length_ < text_length) {
                if (text_buffer_) {
                    delete [] text_buffer_;
                }
                text_length_ = text_length;
                text_buffer_ = new char[text_length_];
            }
        }

        //
        size_t text_length_;
        char* text_buffer_;
    private:
        __DISABLE_COPY(Buffer);
    };

    TextTempBufferStack()
        : depth_(0)
    {
    }
    ~TextTempBufferStack()
    {
    }

    Buffer* Push(size_t text_length)
    {
        if (depth_ >= _countof(buffers_)) {
            __debugbreak();
            return NULL;
        }
        Buffer* buffer = &buffers_[depth_++];
        buffer->Resize(text_length);
        return buffer;
    }

    void Pop()
    {
        if (depth_ <= 0) {
            __debugbreak();
            return;
        }
        --depth_;
    }

    int depth_;
    Buffer buffers_[kMaxDepths];
};

struct TextBufferTransaction
{
    TextBufferTransaction(TextTempBufferStack* buffer_stack, size_t text_length)
        : buffer_stack_(buffer_stack)
    {
        buffer_ = buffer_stack_->Push(text_length);
    }
    ~TextBufferTransaction()
    {
        buffer_stack_->Pop();
    }
    TextTempBufferStack::Buffer* buffer_;
    TextTempBufferStack* const buffer_stack_;
private:
    __DISABLE_COPY(TextBufferTransaction);
};

static TextTempBufferStack text_temp_buffer_stack_static_;

}; //end of namespace

#define	ISHANGUL(ch)	((ch) & 0x80)

ITextFilter::ITextFilter()
{
	Initialize();
	SetNationType();

    length_replace_string_ = 0;
    replace_string_ = NULL;
    replace_type_ = KReplace_By_Character;
    check_type_ = kCheckSubString;
}

ITextFilter::~ITextFilter()
{
	Release();

    length_replace_string_ = 0;
    SAFE_DELETE_ARRAY(replace_string_);
}

bool ITextFilter::CheckIncludeStringChina(const char * Text)
{
#ifdef _APPLY_NHN_FILTERFING
	//no operation
#else
	for ( unsigned char *lpszCheck = ( unsigned char*)Text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{	// 한 바이트
			if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			{
				return ( true);
			}
		}
		else
		{	// 두 바이트
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0xB0 <= *lpszCheck && *lpszCheck <= 0xF7)
			{	// 한자
				if ( 0xA1 <= *lpszTrail && *lpszTrail <= 0xFE)
				{	// 한자임
					if ( *lpszCheck == 0xD7 && 0xFA <= *lpszTrail)
					{	// 일부 안되는 특수문자
						return ( true);
					}
				}
				else
				{
					return ( true);
				}
			}
			else
			{
				return ( true);
			}

			++lpszCheck;
		}
	}
#endif//_APPLY_NHN_FILTERFING
	return false;
}
bool ITextFilter::CheckIncludeStringTaiwan(const char * Text)
{
#ifdef _APPLY_NHN_FILTERFING
	//no operation
#else
	for ( unsigned char *lpszCheck = ( unsigned char*)Text; *lpszCheck; ++lpszCheck)
	{
		if ( 1 == _mbclen( lpszCheck))
		{	// 한 바이트
			if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			{
				return ( true);
			}
		}
		else
		{	// 두 바이트
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0xA4 <= *lpszCheck && *lpszCheck <= 0xF9)
			{	// 한자
				if ( ( 0x40 <= *lpszTrail && *lpszTrail <= 0x7E) ||
					( 0xA1 <= *lpszTrail && *lpszTrail <= 0xFE))
				{	// 한자임
					if ( *lpszCheck == 0xF9 && 0xDD <= *lpszTrail)
					{	// 일부 안되는 특수문자
						return ( true);
					}
				}
				else
				{
					return ( true);
				}
			}
			else
			{
				return ( true);
			}

			++lpszCheck;
		}
	}
#endif//_APPLY_NHN_FILTERFING
	return false;
}

bool	ITextFilter::CheckIncludeStringJapan(const char * Text)
{
#ifdef _APPLY_NHN_FILTERFING
	//no operation
#else
    size_t length = strlen(Text);
    for (size_t n = 0; n < length; ++n)
    {
        unsigned char current_char = Text[n];
        unsigned char next_char = Text[n+1];
        switch (current_char)
        {
        case ' ' ://공백 
        case '\t' ://탭은 안된다.
            return true;
        }

        if(current_char & 0x80)
        {
            if ( ((n+1) < length) &&
                (current_char == 0x81) && 
                (next_char == 0x40))
            {
                // 해당문자는 전각문자의 공백이다. 허용안됨
                return true;
            }
            if (current_char >= 0xA0 && current_char <= 0xDF)
            {
                //이영역은 2바이트 전각이 아닌 반각문자영역이다. 허용하며 1바이트씩 넘긴다.
                ;
            }
            else
            {
                // 그외 일어 및 한자는 2바이트
                n++;
            }
        }
        else if(isdigit(current_char))//숫자면 
        {
            ;
        }
        else if(isalpha(current_char))//영문이면
        {
            ;
        }
        else //기타 
        {
            return true;
        }
    }
#endif//_APPLY_NHN_FILTERFING
	return false;
}

bool ITextFilter::CheckIncludeStringRussia(const char * Text)
{
    const int kEnglishType = 0;
    const int kRussianType = 1;
    const int kMaxCheckSet = 1;

    STLX_SET<int> type_check_set;

    size_t string_lenth = strlen(Text);
    for (size_t index = 0 ; index < string_lenth; ++index)
    {
        // 한바이트씩 순회하며 문자체크
        switch (Text[index])
        {
        case ' ' ://공백 
        case '\t' ://탭은 안된다.
            return true;
        }

        if (Text[index] & 0x80) // 러시아문자 표시
        {
            // 러시아 문자코드 대역폭 체크
            unsigned char temp_char = Text[index];
            if (((temp_char >= 0xC0) && (temp_char <= 0xFF)) || 
                (temp_char == 0xA8) || 
                (temp_char == 0xB8))
            {
                type_check_set.insert(kRussianType);
            }
            else
            {
                // 허용하지 않는 문자 및 기호
                return true;
            }
        }
        else if(isdigit(Text[index]))
        {
            //숫자면 통과
        }
        else if(isalpha(Text[index]))
        {
            //영문이면 통과
            type_check_set.insert(kEnglishType);
        }
        else
        {
            // 기타 문자의 경우 허용하지 않음
            return true;
        }

        if (type_check_set.size() > kMaxCheckSet)
        {
            // 해당 사항의 경우 영어와 러시아문자의 혼용된 문자열이다.
            // 러시아 현지 요쳥에 의해서 해당 사항은 허용하지 않음
            return true;
        }
    }
    return false;
}

bool  ITextFilter::CheckIncludeString(const char *szSrc)
{

#ifdef _APPLY_NHN_FILTERFING
		//no operation
#else
	for (int n = 0; n < (signed)strlen(szSrc); n++)
	{
		switch (szSrc[n])
		{
		case ' ' ://공백 
		case '\t' ://탭은 안된다.
			return true;
		}

		if(szSrc[n]& 0x80)
		{

			unsigned char	cBuf1=		szSrc[n];
			unsigned char	cBuf2=		szSrc[n+1];
			unsigned short	wBuf=		(unsigned short)cBuf1<<8 | cBuf2;

            // 2바이트문자는 한글만 허용 [11/2/2010 iforall]
			if(wBuf >= 0xB0A1  &&  wBuf <= 0xC8FE)	//한글 	
			{
				n++;
			}
            else
            {
                return true;
            }
			//else if(wBuf >= 0xCAA1  &&  wBuf <= 0xFDFE) //한자 
			//{
			//	return true;
			//}
			//else if(wBuf >= 0xA1A1  &&  wBuf <= 0xACFE) //2바이트 특수 문자
			//{
			//	return true;
			//}
			//n++;
		}
		else if(isdigit(szSrc[n]))//숫자면 
		{
			;
		}
		else if(isalpha(szSrc[n]))//영문이면
		{
			;
		}
		else //기타 
		{
			return true;
		}
	}

#endif//_APPLY_NHN_FILTERFING
	return false;
}
//*****************************************************************************
//	문자열에서 white space와 tab 을 지우고 문자만 남긴다.
//
//	Paremeters
//		char *szDest	: trim 시킨 내용을 담을 포인터
//		char *szSrc		: trim 시킬 문자열
//	Return Values
//		void
//	Remark
//		삭제 대상 : ' ' '\t' '\n' '\"'
//*****************************************************************************
void	ITextFilter::Trim(char *szDest, const char *szSrc)
{
    const size_t length = strlen(szSrc);
    size_t i = 0, n = 0;
    for (; n < length; ++n)
	{
		switch (szSrc[n])
		{
		case ' ' :
		case '\t' :
		case '\n' :
		//case '\"' :
			break;

		default :
			szDest[i] = szSrc[n];
			i++;
			break;
		}
	}
	szDest[i] = '\0';
}


//*****************************************************************************
//	문자열에서 특정 길이만큼만 잘라낸 스트링을 만든다.
//
//	Paremeters
//		char *szDest	: 잘라낸 스트링을 담을 포인터
//		char *szSrc		: 잘라낼 문자열
//		int nLength		: 잘라낼 길이
//	Return Values
//		void
//	Remark
//		strncpy + 널문자 넣기 의 기능.
//		만일 szSrc의 길이가 nLength보다 작으면 szSrc의 길이만큼 복사한다.
//*****************************************************************************
void	ITextFilter::GetStringPart(char *szDest, const char *szSrc, int nLength)
{
	if (strlen(szSrc) <= (size_t)nLength)
		strcpy(szDest, szSrc);
	else
	{
		memcpy(szDest, szSrc, nLength);
		szDest[nLength] = '\0';
	}
}

//*****************************************************************************
//	한 문자열에서 특정 문자열의 위치를 찾아낸다.
//
//	Paremeters
//		char *szStr		: 검색할 문자열 (ex : ABCDEFG)
//		char *szCharSet	: 찾는 대상		(ex : EF)
//	Return Values
//		char*			: szChatSet의 위치 포인터. 없으면 NULL.
//	Remark
//		strstr과 기능과 사용법이 같으나 한글 find의 문제점을 해결
//*****************************************************************************
char* ITextFilter::FindString(char* szStr, const char* szCharSet )
{
    const size_t string_length = _tcslen(szStr);
    const size_t charset_length = _tcslen(szCharSet);
    if (string_length < charset_length) {
        return NULL;
    }
    util::TextBufferTransaction transaction(&util::text_temp_buffer_stack_static_,
                                            charset_length + 1);
    char* const l_szStr = transaction.buffer_->text_buffer_;

    for (size_t n = 0; n < string_length - charset_length + 1;)
    {
        memset(l_szStr, 0, charset_length + 1);
        GetStringPart(l_szStr, szStr + n, int(charset_length));

        if (m_NationType == FILTER_JAPAN ||
            m_NationType == FILTER_CHINA ||
            m_NationType == FILTER_TAIWAN) //중국도 적용
        {
            // lammy 임시 수정 : 일본어 OS에서 0x8152 과 0x815b을 같다고 처리 하는 경우가 있다.
            if (_tcscmp(l_szStr, szCharSet) == 0) {
                return szStr + n;
            }
        }
        else
        {
            if (CSTR_EQUAL == CompareString(m_lcid, NORM_IGNORECASE,
                                            l_szStr, int(_tcslen(l_szStr)),
                                            szCharSet, int(_tcslen(szCharSet))))
            {
                return szStr + n;
            }
        }

        if (ISHANGUL(szStr[n])) {
            n += 2;
        }
        else {
            ++n;
        }
    }

    return NULL;
}

char* ITextFilter::FindStringUseWildCard(char *szStr, const char *szCharSet, size_t iCharSetIndex)
{
    SlangTextInfo* wild_card_info = &(slang_text_info_list_[iCharSetIndex]);
    const size_t string_length = _tcslen(szStr);
    const size_t charset_length = _tcslen(szCharSet);
    if (string_length < charset_length)
    {
        return NULL;
    }
    util::TextBufferTransaction transaction(&util::text_temp_buffer_stack_static_,
        charset_length + 1);
    char* const l_szStr = transaction.buffer_->text_buffer_;

    const bool pre_wild_card = wild_card_info->pre_wild_card_;
    const bool post_wild_card = wild_card_info->post_wild_card_;

    // pre_wild_card 에 따라 시작 인덱스 고정 및 루프 설정
    // pst wild_card 에 따라 더 순회가 남아 있는지 체크
    size_t loop_count = (pre_wild_card == true) ? (string_length - charset_length + 1) : 1;
    for (size_t check_index = 0; check_index < loop_count; )
    {
        memset(l_szStr, 0, charset_length + 1);
        GetStringPart(l_szStr, szStr + check_index, int(charset_length));

        if (m_NationType == FILTER_JAPAN ||
            m_NationType == FILTER_CHINA ||
            m_NationType == FILTER_TAIWAN) //중국도 적용
        {
            // lammy 임시 수정 : 일본어 OS에서 0x8152 과 0x815b을 같다고 처리 하는 경우가 있다.
            if (_tcscmp(l_szStr, szCharSet) == 0)
            {
                if ((post_wild_card == true) || 
                    ((post_wild_card == false) && (szStr[check_index + charset_length] == '\0')))
                {
                    return szStr + check_index;
                }
            }
        }
        else
        {
            if (CSTR_EQUAL == CompareString(m_lcid, NORM_IGNORECASE,
                l_szStr, int(_tcslen(l_szStr)),
                szCharSet, int(_tcslen(szCharSet))))
            {
                if ((post_wild_card == true) || 
                    ((post_wild_card == false) && (szStr[check_index + charset_length] == '\0')))
                {
                    return szStr + check_index;
                }
            }
        }

        if (ISHANGUL(szStr[check_index])) {
            check_index += 2;
        }
        else {
            ++check_index;
        }
    }

    return NULL;
}

void ITextFilter::SetNationType( FILTER_NATION_TYPE Type )
{
	m_NationType = Type;

	switch( Type )
	{
	case FILTER_DEFAULT:
        m_lcid = MAKELCID( MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ), SORT_DEFAULT );
        break;
	case FILTER_KOREA:
        m_lcid = MAKELCID( MAKELANGID( LANG_KOREAN, SUBLANG_DEFAULT ), SORT_DEFAULT );
        break;
	case FILTER_TAIWAN:
		m_lcid = MAKELCID( MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL ), SORT_DEFAULT );
		break;

	case FILTER_CHINA:
		m_lcid = MAKELCID( MAKELANGID( LANG_CHINESE, SUBLANG_DEFAULT ), SORT_DEFAULT );
		break;
	case FILTER_JAPAN:
		m_lcid = MAKELCID( MAKELANGID( LANG_JAPANESE, SUBLANG_DEFAULT ), SORT_DEFAULT );
		break;
    case FILTER_RUSSIA:
        m_lcid = MAKELCID( MAKELANGID( LANG_RUSSIAN, SUBLANG_DEFAULT ), SORT_DEFAULT );
        break;
	}

}

void ITextFilter::SetReplaceString(const char* replace_string, FILTER_REPLACE_TYPE replace_type)
{
    if (replace_string == NULL)
    {
        length_replace_string_ = 0;
        SAFE_DELETE_ARRAY(replace_string_);
        replace_type_ = KReplace_By_Character;
    }

    length_replace_string_ = int(strlen(replace_string));
    SAFE_DELETE_ARRAY(replace_string_);
    replace_string_ = new char[length_replace_string_ + 1];
    strncpy(replace_string_, replace_string, length_replace_string_);
    replace_string_[length_replace_string_] = '\0';
    
    replace_type_ = replace_type;
}

//------------------------------------------------------------------------------
/** 필터링
*/
bool ITextFilter::IsHaveReplace(char* szSrc, char* szDest, int nDestSize) 
{
    __UNUSED(szDest); __UNUSED(nDestSize); // (WAVERIX) (090722) (ADD-IN)

#ifdef _GS_ADD_KEY_FILTERING
    if(!m_bNameFilter)
    {
        char* char_name = szSrc;
        for(const char* it = char_name; *it; ++it)
        {
            char chr = *it;
            if((chr<0 || chr>127))
            {
                //키보드필터링(한글or특수문자)가 들어있다..
                return true;
            }
        }
    }
#endif//_GS_ADD_KEY_FILTERING(키보드필터링)

#ifdef _APPLY_NHN_FILTERFING
	//============================
	// NHN 필터링(IAF)사용
	//============================
	int nRet = 0;
	if(!szDest)
	{
		char szTemp[1024];
		nRet = IAF::Filtering(szSrc,szTemp,sizeof(szTemp),(IAF::_enOption)m_bNameFilter);
	}
	else
	{	
		nRet = IAF::Filtering(szSrc,szDest,nDestSize,(IAF::_enOption)m_bNameFilter);
	}
#ifdef _DEBUG
	switch(nRet)
	{
	case -1: assert(!"szSrc이 szDest보다 크다"); break;
	case -2: assert(!"!알수없는에러");			 break;
	}
#endif//_DEBUG
	return (nRet==1) ? (TRUE):(FALSE);
	
#else//_APPLY_NHN_FILTERFING
	//============================
	// NHN 필터링(IAF)미사용
	//============================
    int length_replace = 0;
    char* replace_position = NULL;
    replace_position = GetReplaceTarget(&length_replace, szSrc);
    if (szDest == NULL || nDestSize < 1)
    {
        return (replace_position != NULL) ? true : false;
    }
    if (replace_position != NULL && length_replace != 0)
    {
        // 대치하자
        int source_length = int(strlen(szSrc));
        int replaced_count = 0;
        int dest_position = 0;
        char temp_charater = 0;
        for (int index = 0; index < source_length; ++index)
        {
            if ((replaced_count < length_replace) &&
                (&(szSrc[index]) == &(replace_position[replaced_count])))
            {
                // dest 길이 초과시 리턴
                if (dest_position >= nDestSize)
                {
                    szDest[dest_position-1] = '\0';
                    return true;
                }

                temp_charater = replace_position[replaced_count];
                if (temp_charater != ' ' &&
                    temp_charater != '\t' &&
                    temp_charater != '\n')
                {
                    for (int index_replace_string = 0;
                         index_replace_string < length_replace_string_;
                         ++index_replace_string)
                    {
                        szDest[dest_position] = replace_string_[index_replace_string];
                        ++dest_position;
                    }
                }
                else
                {
                    szDest[dest_position] = szSrc[index];
                    ++dest_position;
                }
                switch (replace_type_)
                {
                case KReplace_By_Character:
                    {
                        ++replaced_count;
                    }                    
                    break;
                case KReplace_By_Word:
                    {
                        replaced_count = length_replace;
                        index += (length_replace - 1);
                    }
                    break;
                }                
            }
            else
            {
                szDest[dest_position] = szSrc[index];
                ++dest_position;
            }            
        }
        return true;
    }
    return false;
#endif//_APPLY_NHN_FILTERFING(NHN 필터링)

}

//*****************************************************************************
//	데이터를 초기화한다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************
void	CNSlangTextFilter::Clear()
{
    slang_text_info_list_.clear();
}

//*****************************************************************************
//	욕 DATA 파일을 불러온다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************
HRESULT	CNSlangTextFilter::Load(const char *szFName,bool bNameFilter)
{
	m_bNameFilter = bNameFilter;

	FILE *pFile = fopen(szFName, "r");
	if (pFile == NULL)
		return false;

	char l_szStr[1024];

    slang_text_info_list_.clear();

    STLX_STRING slang_full_text;
    STLX_STRING slang_text;

    int slang_text_index;
    int first_wild_index;
    int last_wild_index;
    bool pre_wild_card;
    bool post_wild_card;
    int slang_text_length;

	fseek(pFile, 0, 0);
	while (fgets(l_szStr, 1023, pFile) != NULL)
	{
		Trim(l_szStr, l_szStr);
		if (l_szStr[0] != '/' && l_szStr[1] != '/' &&strlen(l_szStr) != 0)
		{
			{
                slang_full_text = l_szStr;

                if (check_type_ == kCheckSubString)
                {
                    pre_wild_card = true;
                    post_wild_card = true;
                    slang_text = slang_full_text;
                }
                else
                {
                    slang_text_index = (int)slang_full_text.find_first_not_of('*');
                    first_wild_index = (int)slang_full_text.find_first_of('*');
                    last_wild_index = (int)slang_full_text.find_last_of('*');
                    pre_wild_card = (first_wild_index < slang_text_index) && (first_wild_index != -1);
                    post_wild_card = last_wild_index > slang_text_index;
                    slang_text_length = (int)(post_wild_card ? last_wild_index - slang_text_index : slang_full_text.length() - slang_text_index);
                    slang_text = slang_full_text.substr(slang_text_index, slang_text_length);
                }

                slang_text_info_list_.push_back(SlangTextInfo(slang_text, pre_wild_card, post_wild_card));
			}
		}
	}
	fclose(pFile);
	return true;
}

//*****************************************************************************
//	욕 DATA 파일을 팩파일에서 불러온다.
//
//	Paremeters
//		const char *szFName	: 불러올 파일 이름
//	Return Values
//		void
//	Remark
//*****************************************************************************

HRESULT	CNSlangTextFilter::LoadFromPack(const char *szFName,bool bNameFilter)
{
	m_bNameFilter = bNameFilter;

	util::SolarFileReader sr;

	BOOL bResult = sr.OpenFileFromPack("Data\\Resource.wpk", szFName, SEPERATOR_ONLY_NEWLINE);
	if(!bResult)
		return FALSE;

	char pToken[SLANG_LENGTH_MAX];

    slang_text_info_list_.clear();

    STLX_STRING slang_full_text;
    STLX_STRING slang_text;

    int slang_text_index;
    int first_wild_index;
    int last_wild_index;
    bool pre_wild_card;
    bool post_wild_card;
    int slang_text_length;

	eTOKEN_TYPE ettType;
	while (true)
	{
		ettType = sr.GetNextTokenType( TOKEN_STRING );
        if (ettType == TOKEN_END)
        {
            break;
        }
		strncpy( pToken, sr.GetTokenString(), SLANG_LENGTH_MAX );

        slang_full_text = pToken;

        if (check_type_ == kCheckSubString)
        {
            pre_wild_card = true;
            post_wild_card = true;
            slang_text = slang_full_text;
        }
        else
        {
            slang_text_index = (int)slang_full_text.find_first_not_of('*');
            first_wild_index = (int)slang_full_text.find_first_of('*');
            last_wild_index = (int)slang_full_text.find_last_of('*');
            pre_wild_card = (first_wild_index < slang_text_index) && (first_wild_index != -1);
            post_wild_card = last_wild_index > slang_text_index;
            slang_text_length = (int)(post_wild_card ? last_wild_index - slang_text_index : slang_full_text.length() - slang_text_index);
            slang_text = slang_full_text.substr(slang_text_index, slang_text_length);
        }

        slang_text_info_list_.push_back(SlangTextInfo(slang_text, pre_wild_card, post_wild_card));
	}

	sr.CloseFile();

	return TRUE;
}

//*****************************************************************************
//	LOAD에서 부른 data들을 파괴시킨다.
//
//	Paremeters
//		void
//	Return Values
//		void
//	Remark
//*****************************************************************************
HRESULT	CNSlangTextFilter::Destroy()
{
	if (!IsInit())
		return false;

	m_xInit = false;
    Clear();
	return true;
}

//*****************************************************************************
//	소스에서 치환할 대상을 찾아낸다.
//
//	Paremeters
//		int *nLength		: 치환 대상의 길이를 받을 포인터
//		const char *szSrc	: 치환 대상을 찾을 문자열
//	Return Values
//		char*				: 치환 대상의 위치
//	Remark
//		치환 대상이 없으면 nLength에는 0이 들어가고 NULL이 반환된다.
//*****************************************************************************
char*	CNSlangTextFilter::GetReplaceTarget(int *nLength, char *szSrc)
{
	if (!IsInit())
		return NULL;
    util::TextBufferTransaction transaction(&util::text_temp_buffer_stack_static_,
                                            _tcslen(szSrc) + 1);
    char* const l_szStr = transaction.buffer_->text_buffer_;

    const char* slang_text = NULL;
    char* target_text = NULL;
    size_t slang_info_num = slang_text_info_list_.size();

	Trim(l_szStr, szSrc);
	for (size_t n = 0; n < slang_info_num; n++)
	{
        slang_text = slang_text_info_list_[n].slang_text_.c_str();
        target_text = FindStringUseWildCard(l_szStr, slang_text, n);

		if (target_text != NULL)
		{
			int l_nTargetPos = static_cast<int>(target_text - l_szStr);
			int l_nOffsetSpaceNumber = 0, l_nTargetSpaceNumber = 0;
            int l_nOffset = 0;
            int i = 0;
			for (; ; i++)
			{
				if (szSrc[i] == ' ' || szSrc[i] == '\t' || szSrc[i] == '\n')
					l_nOffsetSpaceNumber++;
				else
				{
					if (l_nOffset >= l_nTargetPos)
						break;
					l_nOffset++;
				}
			}
            const int slang_target_length = static_cast<int>(strlen(slang_text));
			for (l_nOffset = 0; l_nOffset < slang_target_length; ++i)
			{
				if (szSrc[i] == ' ' || szSrc[i] == '\t' || szSrc[i] == '\n')
					l_nTargetSpaceNumber++;
				else
					l_nOffset++;
			}

			if (nLength != NULL)
				*nLength = slang_target_length + l_nTargetSpaceNumber;

			return (szSrc + l_nTargetPos + l_nOffsetSpaceNumber);
		}
	}
	return NULL;
}