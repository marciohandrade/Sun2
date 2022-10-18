#pragma once
//=======================================================================================================================
/// SocketOptionParser class
/**
    @author
            Kim Min Wook < taiyo@webzen.com >	
    @since
            2006. 1. 9
    @remarks
            - 소켓 옵션에 대한 파서
            - 소켓구멍에 옵션을 붙일때 sSOCKETITEM_OPTION의 정보를 이용한다.
            - 소켓옵션에 대한 Encode, Decode를 할 때, sSOCKET_OPTION 정보를 이용한다.
    @note
            -
    @history
            - 2006.11.29 : 소켓 옵션 개수 증가 32->64
*/
//=======================================================================================================================

#include <ItemOptionHeader.h>
#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include "RatioSelector.h"

#ifdef _NA_0_20100727_SOCKET_SYSTEM_PARSER

//==================================================================================================

// 쟈드 옵션 정보
struct ZardOptionInfo
{
    typedef BYTE ItemOptionCode; // 아이템 기준 옵션 코드
    typedef int OptionPoint; // 옵션포인트
    struct OptionValueInfo // 옵션값 정보
    {
        typedef int ValueType;

        OptionPoint point; // 옵션포인트
        eVALUE_TYPE	type; // 옵션타입
        ValueType value; // 옵션값

        OptionValueInfo() : point(0), type(VALUE_TYPE_VALUE), value(0)
        {
        }
    private:
        __DISABLE_COPY(OptionValueInfo);
    };
    typedef STLX_MAP<OptionPoint, OptionValueInfo*> OptionValueMap;
    
public:
    ZardOptionInfo() : 
        item_option_code(eATTR_TYPE_INVALID), attr_option_code(eATTR_TYPE_INVALID)
    {
    }
    ~ZardOptionInfo();

public:
    // 옵션값을 삽입한다. 삽입에 실패하면 새 옵션값은 자동 해제된다.
    bool Insert(OptionValueInfo*& INOUT new_option_value);

    // 포인트에 따른 옵션값을 찾는다.
    const OptionValueInfo* Find(const OptionPoint option_point) const;

    // 포인트에 따른 옵션값이 존재하는지 검사한다.
    bool IsExist(const OptionPoint option_point) const
    {
        return (option_value_map_.find(option_point) != option_value_map_.end());
    }

    // 옵션값 목록을 얻는다.
    const OptionValueMap& GetOptionValues() const
    {
        return option_value_map_;
    }

    // 옵션 타입을 얻는다.
    eVALUE_TYPE GetOptionType() const;

public:
    ItemOptionCode item_option_code; // 아이템 기준 옵션코드
    eATTR_TYPE attr_option_code; // 속성 기준 옵션코드
    
private:
    OptionValueMap option_value_map_;
    __DISABLE_COPY(ZardOptionInfo);
}; // ZardOptionInfo

typedef WORD SOCKETINDEX; 

// 소켓 옵션
struct sSOCKET_OPTION
{
    typedef BYTE ItemOptionKind;
    typedef int OptionValue;
    struct OptionPointInfo // 옵션 포인트 정보
    {
        enum { kMaxListSize = 5 };
        typedef BYTE ListSizeType;

        ZardOptionInfo::ItemOptionCode item_option_code; // 아이템 기준 옵션 코드
        ZardOptionInfo::OptionPoint option_point; // 옵션 포인트
        const ZardOptionInfo* zard_option; // 쟈드 옵션 정보

        OptionPointInfo() : item_option_code(eATTR_TYPE_INVALID), option_point(0), zard_option(NULL)
        {
        }
    private:
        __DISABLE_COPY(OptionPointInfo);
    };

public:
    sSOCKET_OPTION() : 
        index(0), 
        zard_grade(kZardGradeUnused), 
        name_code(0), 
        item_option_kind(eATTR_TYPE_INVALID), 
        attr_option_kind(eATTR_TYPE_INVALID), 
        item_code(0), 
        option_image(0), 
        option_type(VALUE_TYPE_VALUE), 
        option_value(0), 
        option_point_list_size(0)
    {
    }

public:
    // NOTE: f110330.3L, added interface to access easy iterating
    int GetPointArrayInfo(const OptionPointInfo** begin_it) const;
    // 옵션 포인트 정보를 얻는다.
    OptionPointInfo* GetPointInfoByMutable(const OptionPointInfo::ListSizeType index);
    // 옵션 포인트 정보를 얻는다.
    const OptionPointInfo* GetPointInfo(const OptionPointInfo::ListSizeType index) const;
    // 포인트 방식 이전 버전을 위해 주 옵션값을 설정한다.
    bool SetMainOption();
    
public:
    SOCKETINDEX index;
    eSOCKET_LEVEL zard_grade; // 쟈드등급
    CODETYPE name_code; // 옵션 이름 코드 (포인트 방식 이전 지원/이후 버전에선 삭제 예정)
    ItemOptionKind item_option_kind; // 아이템 기준 옵션 코드 (포인트 방식 이전 지원/이후 버전에선 주 옵션값)
    eATTR_TYPE attr_option_kind; // 속성 기준 옵션 코드 (포인트 방식 이전 버전 지원/이후 버전에선 주 옵션값)
    CODETYPE item_code; // 쟈드 아이템 코드
    CODETYPE option_image; // 쟈드 아이콘 이미지
    eVALUE_TYPE	option_type; // 옵션값 입력 형식 (포인트 방식 이전 버전 지원/이후 버전에선 주 옵션값)
    OptionValue option_value; // 옵션값 (포인트 방식 이전 버전 지원/이후 버전에선 주 옵션값)
    OptionPointInfo::ListSizeType option_point_list_size; // 옵션 포인트 목록 크기

private:
    OptionPointInfo option_point_list_[OptionPointInfo::kMaxListSize]; // 옵션 포인트 목록
    __DISABLE_COPY(sSOCKET_OPTION);
}; // sSOCKET_OPTION


// NOTE: f110330.3L, added interface to access easy iterating
inline int sSOCKET_OPTION::GetPointArrayInfo(const OptionPointInfo** begin_it) const
{
    *begin_it = option_point_list_;
    return option_point_list_size;
}

// 옵션 포인트 정보를 얻는다.
inline sSOCKET_OPTION::OptionPointInfo*
sSOCKET_OPTION::GetPointInfoByMutable(const OptionPointInfo::ListSizeType index)
{
    if (index < 0 || index >= _countof(option_point_list_))
    {
        return NULL;
    }
    return &(option_point_list_[index]);
}

// 옵션 포인트 정보를 얻는다.
inline const sSOCKET_OPTION::OptionPointInfo*
sSOCKET_OPTION::GetPointInfo(const OptionPointInfo::ListSizeType index) const
{
    if (index < 0 || index >= option_point_list_size)
    {
        // 읽기 전용으로 접근하기 때문에 유효한 데이터가 들어있는 범위까지만 허용한다.
        return NULL;
    }
    return &(option_point_list_[index]);
}

//==================================================================================================
// 소켓 옵션 파서
class SocketOptionParser : public util::Singleton<SocketOptionParser>, public IParser
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    enum { kInvalidSocketOptionIndex = 0 };
    enum { kEmptySocketOptionIndex = 0 };
    typedef WORD ZardOptionRatioKey; // 등급 + 옵션코드
    typedef int ZardGrade;
    typedef int ZardOptionKind;
    typedef STLX_HASH_MAP<SOCKETINDEX, sSOCKET_OPTION*> SocketOptionMap;
private:
    // ZardOptionRatioMap
    typedef RatioSelector<SOCKETINDEX> ZardOptionRatio;
    typedef STLX_HASH_MAP<ZardOptionRatioKey, ZardOptionRatio*> ZardOptionRatioMap;
    typedef STLX_HASH_MAP<CODETYPE, ZardOptionRatio*> ZardItemRatioMap;
    // ZardOptionMap
    typedef STLX_HASH_MAP<ZardOptionInfo::ItemOptionCode, ZardOptionInfo*> ZardOptionMap;
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    SocketOptionParser();
    ~SocketOptionParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);
    
public:
    // 소켓 옵션을 찾는다.
    const sSOCKET_OPTION* const FindSocketOption(const SOCKETINDEX socket_option_index) const;

    // 소켓 옵션맵을 얻는다.
    const SocketOptionMap& GetSocketOptionMap() const
    {
        return socket_option_map_;
    }
    
    // 등급에 해당하는 쟈드가 가질 수 있는 옵션 중 하나를 얻는다.
    SOCKETINDEX GetRandomZardOptionByGrade(
        const ZardGrade zard_grade, 
        const ZardOptionKind zard_item_option_kind) const;

    // 쟈드가 가질 수 있는 옵션 중 하나를 얻는다.
    SOCKETINDEX GetRandomZardOption(const CODETYPE item_code) const;

    // 쟈드가 가질 수 있는 옵션이 오직 하나일 경우 얻을 수 있는 옵션을 얻는다.
    SOCKETINDEX GetFixedZardOption(const CODETYPE item_code) const;

    // 쟈드 옵션을 찾는다.
    ZardOptionInfo* FindZardOption(const ZardOptionInfo::ItemOptionCode item_option_code) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    bool LoadSocketOption(const bool is_reload);
    void ReleaseSocketOption();
    ZardOptionRatio* FindZardOptionRatio(
        const ZardGrade zard_grade, 
        const ZardOptionKind zard_item_option_kind) const;
    bool LoadZardOption(const bool is_reload);
    void ReleaseZardOption();
    bool LoadZardPoint(const int row, sSOCKET_OPTION* OUT socket_option);

private:
    SocketOptionMap socket_option_map_;
    ZardOptionRatioMap zard_option_ratio_map_;
    ZardItemRatioMap zard_item_ratio_map_;
    ZardOptionMap zard_option_map_;
    __DISABLE_COPY(SocketOptionParser);
};

//==================================================================================================
#else // _NA_0_20100727_SOCKET_SYSTEM_PARSER
//==================================================================================================
#define MAX_ATTRIBUTENAME_LENGTH (16)

struct sSOCKET_OPTION
{
    INT				m_AttrIndex;				// 키
    CODETYPE		m_SocketItemCode;
    char			m_pszAttrName[MAX_ATTRIBUTENAME_LENGTH];
    CODETYPE		m_NCode;
    eVALUE_TYPE		m_NumericType;
    INT				m_iValue[SOCKETLV_MAX];
};

struct sSOCKETITEM_OPTION
{
    CODETYPE		m_SocketItemCode;			//< 키
    INT				m_AttrIndex;
    char			m_pszAttrName[MAX_ATTRIBUTENAME_LENGTH];
    CODETYPE		m_NCode;
    eSOCKET_LEVEL	m_SocketLevel;		
    eVALUE_TYPE		m_NumericType;
    BYTE			m_byValue;
};


class SocketOptionParser : public util::Singleton<SocketOptionParser>, public IParser
{
    typedef STLX_HASH_MAP<WORD, sSOCKET_OPTION> SOCKET_OPTION_INFO;   

public:
    SocketOptionParser(void);
    virtual ~SocketOptionParser(void);

    VOID				Init( char * pszPackFileName );
    VOID				Release(){}
    VOID				Unload();
    VOID				Reload();

    inline const sSOCKETITEM_OPTION * GetSocketItemOption( CODETYPE SocketItemCode ) { return m_SocketOptionParamHash.GetData(SocketItemCode); }
    inline const sSOCKET_OPTION * GetSocketOption( DWORD OptionIndex );

            BOOL		LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
            BOOL		LoadSocketOption(BOOL bReload);

private:

    WORD				ValidCheck(sSOCKETITEM_OPTION *pInfo);

private:
    util::SolarHashTable<sSOCKETITEM_OPTION *>	m_SocketOptionParamHash;
    sSOCKET_OPTION								m_SocketOption[MAX_ITEM_OPTION_KIND_NUM];
    char                                        m_pszFileName[MAX_PATH];
};


inline const sSOCKET_OPTION * SocketOptionParser::GetSocketOption( DWORD OptionIndex )
{
    if( OptionIndex > MAX_ITEM_OPTION_KIND_NUM ) return NULL;

    return &m_SocketOption[OptionIndex];
}
#endif //_NA_0_20100727_SOCKET_SYSTEM_PARSER