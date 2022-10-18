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
		{	// �� ����Ʈ
			if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			{
				return ( true);
			}
		}
		else
		{	// �� ����Ʈ
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0xB0 <= *lpszCheck && *lpszCheck <= 0xF7)
			{	// ����
				if ( 0xA1 <= *lpszTrail && *lpszTrail <= 0xFE)
				{	// ������
					if ( *lpszCheck == 0xD7 && 0xFA <= *lpszTrail)
					{	// �Ϻ� �ȵǴ� Ư������
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
		{	// �� ����Ʈ
			if ( *lpszCheck < 0x21 || *lpszCheck > 0x7E)
			{
				return ( true);
			}
		}
		else
		{	// �� ����Ʈ
			unsigned char *lpszTrail = lpszCheck + 1;
			if ( 0xA4 <= *lpszCheck && *lpszCheck <= 0xF9)
			{	// ����
				if ( ( 0x40 <= *lpszTrail && *lpszTrail <= 0x7E) ||
					( 0xA1 <= *lpszTrail && *lpszTrail <= 0xFE))
				{	// ������
					if ( *lpszCheck == 0xF9 && 0xDD <= *lpszTrail)
					{	// �Ϻ� �ȵǴ� Ư������
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
        case ' ' ://���� 
        case '\t' ://���� �ȵȴ�.
            return true;
        }

        if(current_char & 0x80)
        {
            if ( ((n+1) < length) &&
                (current_char == 0x81) && 
                (next_char == 0x40))
            {
                // �ش繮�ڴ� ���������� �����̴�. ���ȵ�
                return true;
            }
            if (current_char >= 0xA0 && current_char <= 0xDF)
            {
                //�̿����� 2����Ʈ ������ �ƴ� �ݰ����ڿ����̴�. ����ϸ� 1����Ʈ�� �ѱ��.
                ;
            }
            else
            {
                // �׿� �Ͼ� �� ���ڴ� 2����Ʈ
                n++;
            }
        }
        else if(isdigit(current_char))//���ڸ� 
        {
            ;
        }
        else if(isalpha(current_char))//�����̸�
        {
            ;
        }
        else //��Ÿ 
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
        // �ѹ���Ʈ�� ��ȸ�ϸ� ����üũ
        switch (Text[index])
        {
        case ' ' ://���� 
        case '\t' ://���� �ȵȴ�.
            return true;
        }

        if (Text[index] & 0x80) // ���þƹ��� ǥ��
        {
            // ���þ� �����ڵ� �뿪�� üũ
            unsigned char temp_char = Text[index];
            if (((temp_char >= 0xC0) && (temp_char <= 0xFF)) || 
                (temp_char == 0xA8) || 
                (temp_char == 0xB8))
            {
                type_check_set.insert(kRussianType);
            }
            else
            {
                // ������� �ʴ� ���� �� ��ȣ
                return true;
            }
        }
        else if(isdigit(Text[index]))
        {
            //���ڸ� ���
        }
        else if(isalpha(Text[index]))
        {
            //�����̸� ���
            type_check_set.insert(kEnglishType);
        }
        else
        {
            // ��Ÿ ������ ��� ������� ����
            return true;
        }

        if (type_check_set.size() > kMaxCheckSet)
        {
            // �ش� ������ ��� ����� ���þƹ����� ȥ��� ���ڿ��̴�.
            // ���þ� ���� �䫊�� ���ؼ� �ش� ������ ������� ����
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
		case ' ' ://���� 
		case '\t' ://���� �ȵȴ�.
			return true;
		}

		if(szSrc[n]& 0x80)
		{

			unsigned char	cBuf1=		szSrc[n];
			unsigned char	cBuf2=		szSrc[n+1];
			unsigned short	wBuf=		(unsigned short)cBuf1<<8 | cBuf2;

            // 2����Ʈ���ڴ� �ѱ۸� ��� [11/2/2010 iforall]
			if(wBuf >= 0xB0A1  &&  wBuf <= 0xC8FE)	//�ѱ� 	
			{
				n++;
			}
            else
            {
                return true;
            }
			//else if(wBuf >= 0xCAA1  &&  wBuf <= 0xFDFE) //���� 
			//{
			//	return true;
			//}
			//else if(wBuf >= 0xA1A1  &&  wBuf <= 0xACFE) //2����Ʈ Ư�� ����
			//{
			//	return true;
			//}
			//n++;
		}
		else if(isdigit(szSrc[n]))//���ڸ� 
		{
			;
		}
		else if(isalpha(szSrc[n]))//�����̸�
		{
			;
		}
		else //��Ÿ 
		{
			return true;
		}
	}

#endif//_APPLY_NHN_FILTERFING
	return false;
}
//*****************************************************************************
//	���ڿ����� white space�� tab �� ����� ���ڸ� �����.
//
//	Paremeters
//		char *szDest	: trim ��Ų ������ ���� ������
//		char *szSrc		: trim ��ų ���ڿ�
//	Return Values
//		void
//	Remark
//		���� ��� : ' ' '\t' '\n' '\"'
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
//	���ڿ����� Ư�� ���̸�ŭ�� �߶� ��Ʈ���� �����.
//
//	Paremeters
//		char *szDest	: �߶� ��Ʈ���� ���� ������
//		char *szSrc		: �߶� ���ڿ�
//		int nLength		: �߶� ����
//	Return Values
//		void
//	Remark
//		strncpy + �ι��� �ֱ� �� ���.
//		���� szSrc�� ���̰� nLength���� ������ szSrc�� ���̸�ŭ �����Ѵ�.
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
//	�� ���ڿ����� Ư�� ���ڿ��� ��ġ�� ã�Ƴ���.
//
//	Paremeters
//		char *szStr		: �˻��� ���ڿ� (ex : ABCDEFG)
//		char *szCharSet	: ã�� ���		(ex : EF)
//	Return Values
//		char*			: szChatSet�� ��ġ ������. ������ NULL.
//	Remark
//		strstr�� ��ɰ� ������ ������ �ѱ� find�� �������� �ذ�
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
            m_NationType == FILTER_TAIWAN) //�߱��� ����
        {
            // lammy �ӽ� ���� : �Ϻ��� OS���� 0x8152 �� 0x815b�� ���ٰ� ó�� �ϴ� ��찡 �ִ�.
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

    // pre_wild_card �� ���� ���� �ε��� ���� �� ���� ����
    // pst wild_card �� ���� �� ��ȸ�� ���� �ִ��� üũ
    size_t loop_count = (pre_wild_card == true) ? (string_length - charset_length + 1) : 1;
    for (size_t check_index = 0; check_index < loop_count; )
    {
        memset(l_szStr, 0, charset_length + 1);
        GetStringPart(l_szStr, szStr + check_index, int(charset_length));

        if (m_NationType == FILTER_JAPAN ||
            m_NationType == FILTER_CHINA ||
            m_NationType == FILTER_TAIWAN) //�߱��� ����
        {
            // lammy �ӽ� ���� : �Ϻ��� OS���� 0x8152 �� 0x815b�� ���ٰ� ó�� �ϴ� ��찡 �ִ�.
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
/** ���͸�
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
                //Ű�������͸�(�ѱ�orƯ������)�� ����ִ�..
                return true;
            }
        }
    }
#endif//_GS_ADD_KEY_FILTERING(Ű�������͸�)

#ifdef _APPLY_NHN_FILTERFING
	//============================
	// NHN ���͸�(IAF)���
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
	case -1: assert(!"szSrc�� szDest���� ũ��"); break;
	case -2: assert(!"!�˼����¿���");			 break;
	}
#endif//_DEBUG
	return (nRet==1) ? (TRUE):(FALSE);
	
#else//_APPLY_NHN_FILTERFING
	//============================
	// NHN ���͸�(IAF)�̻��
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
        // ��ġ����
        int source_length = int(strlen(szSrc));
        int replaced_count = 0;
        int dest_position = 0;
        char temp_charater = 0;
        for (int index = 0; index < source_length; ++index)
        {
            if ((replaced_count < length_replace) &&
                (&(szSrc[index]) == &(replace_position[replaced_count])))
            {
                // dest ���� �ʰ��� ����
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
#endif//_APPLY_NHN_FILTERFING(NHN ���͸�)

}

//*****************************************************************************
//	�����͸� �ʱ�ȭ�Ѵ�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
//	Return Values
//		void
//	Remark
//*****************************************************************************
void	CNSlangTextFilter::Clear()
{
    slang_text_info_list_.clear();
}

//*****************************************************************************
//	�� DATA ������ �ҷ��´�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
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
//	�� DATA ������ �����Ͽ��� �ҷ��´�.
//
//	Paremeters
//		const char *szFName	: �ҷ��� ���� �̸�
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
//	LOAD���� �θ� data���� �ı���Ų��.
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
//	�ҽ����� ġȯ�� ����� ã�Ƴ���.
//
//	Paremeters
//		int *nLength		: ġȯ ����� ���̸� ���� ������
//		const char *szSrc	: ġȯ ����� ã�� ���ڿ�
//	Return Values
//		char*				: ġȯ ����� ��ġ
//	Remark
//		ġȯ ����� ������ nLength���� 0�� ���� NULL�� ��ȯ�ȴ�.
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