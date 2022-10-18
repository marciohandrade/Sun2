#ifndef	__TEXT_FILTER__
#define	__TEXT_FILTER__

//------------------------------------------------------------------------------
/**
@brief

//	텍스트 필터링을 위한 클래스.
//	필터링 대상이 있는지를 검사하는 함수 (IsHaveReplace)와
//	실제로 필터링을 하는 함수 (Replace) 두개로 이루어져 있다.
//	새 필터를 만들 때는 ITextFilter를 상속한 뒤
//	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc);
//	virtual	void	ReplaceWord(char *szDest, const char *szSrc, int nMaxLength);
//	두개 함수를 override함으로써 제작하게 된다.

@author
	- 지완 

@since
	- 2005.8.12
@remarks

*/

#define	SLANG_LENGTH_MAX	32

enum FILTER_NATION_TYPE
{
	FILTER_DEFAULT = 0,
	FILTER_KOREA,
	FILTER_CHINA,
	FILTER_TAIWAN,
	FILTER_JAPAN,
    FILTER_RUSSIA,
};

enum FILTER_REPLACE_TYPE
{
    KReplace_By_Character = 0,
    KReplace_By_Word,
};

enum FILTER_CHECK_TYPE
{
    kCheckSubString = 0,
    kCheckFullString = 1,
};

class IAbstractInterface
{
public:
	bool		IsInit() {return m_xInit;	}
	
protected:
	bool		m_xInit;
};

class	ITextFilter : public IAbstractInterface
{
protected:
    struct SlangTextInfo
    {
        STLX_STRING slang_text_;
        bool pre_wild_card_;
        bool post_wild_card_;

        SlangTextInfo()
            :pre_wild_card_(true),post_wild_card_(true)
        {
            slang_text_.clear();
        }

        SlangTextInfo(STLX_STRING& slang_text, bool pre_wild_card, bool post_wild_card)
            :slang_text_(slang_text),
            pre_wild_card_(pre_wild_card),
            post_wild_card_(post_wild_card)
        {}
    };

public :
	ITextFilter();
	virtual	~ITextFilter();

	virtual	HRESULT	Create() = 0;
	virtual	HRESULT	Destroy() = 0;
	virtual	HRESULT	Save(const char *szFName) = 0;
	virtual	HRESULT	Load(const char *szFName,bool bNameFilter=true) = 0;
	virtual	HRESULT	LoadFromPack(const char *szFName,bool bNameFilter=true) = 0;

	bool            CheckIncludeString(const char *szSrc);//
    
	bool			CheckIncludeStringChina(const char * Text);
	bool			CheckIncludeStringTaiwan(const char * Text);
	bool			CheckIncludeStringJapan(const char * Text);
    bool			CheckIncludeStringRussia(const char * Text);

	bool			IsHaveReplace(char *szSrc,char *szDest=NULL,int nDestSize=0);

	void			SetNationType( FILTER_NATION_TYPE Type = FILTER_DEFAULT );
    
    void SetReplaceString(const char* replace_string, FILTER_REPLACE_TYPE replace_type);
    void SetCheckType(FILTER_CHECK_TYPE filter_check_type) { check_type_ = filter_check_type; }

protected :
	void			Clear() {}
	void			Initialize() {m_xInit = false;	}
	void			Release() {}

	char *			FindString(char *szStr, const char *szCharSet);
    char *			FindStringUseWildCard(char *szStr, const char *szCharSet, size_t iCharSetIndex);

	void			GetStringPart(char *szDest, const char *szSrc, int nLength);
	void			Trim(char *szDest, const char *szSrc);

	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc) = 0;

protected :
	bool			m_bNameFilter;	
    
    FILTER_CHECK_TYPE check_type_;
    STLX_VECTOR<SlangTextInfo> slang_text_info_list_;
	
private :

	DWORD				m_lcid;
	FILTER_NATION_TYPE	m_NationType;	
	
    int length_replace_string_;
    char* replace_string_;
    FILTER_REPLACE_TYPE replace_type_;
};

class	CNSlangTextFilter : public ITextFilter
{
public :
	CNSlangTextFilter() {Clear();	Create();}
	virtual			~CNSlangTextFilter() {Destroy();	}

	virtual	HRESULT	Create() {m_xInit = true;	return true;	}
	virtual	HRESULT	Destroy();
	virtual	HRESULT	Save(const char *szFName) {szFName;return false;	}
	virtual	HRESULT	Load(const char *szFName,bool bNameFilter=true);
	virtual	HRESULT	LoadFromPack(const char *szFName,bool bNameFilter=true);

protected :
	void			Clear();
	virtual	char*	GetReplaceTarget(int *nLength, char *szSrc);
public:

};


#endif