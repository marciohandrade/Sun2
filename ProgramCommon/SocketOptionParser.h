#pragma once
//=======================================================================================================================
/// SocketOptionParser class
/**
    @author
            Kim Min Wook < taiyo@webzen.com >	
    @since
            2006. 1. 9
    @remarks
            - ���� �ɼǿ� ���� �ļ�
            - ���ϱ��ۿ� �ɼ��� ���϶� sSOCKETITEM_OPTION�� ������ �̿��Ѵ�.
            - ���Ͽɼǿ� ���� Encode, Decode�� �� ��, sSOCKET_OPTION ������ �̿��Ѵ�.
    @note
            -
    @history
            - 2006.11.29 : ���� �ɼ� ���� ���� 32->64
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

// ��� �ɼ� ����
struct ZardOptionInfo
{
    typedef BYTE ItemOptionCode; // ������ ���� �ɼ� �ڵ�
    typedef int OptionPoint; // �ɼ�����Ʈ
    struct OptionValueInfo // �ɼǰ� ����
    {
        typedef int ValueType;

        OptionPoint point; // �ɼ�����Ʈ
        eVALUE_TYPE	type; // �ɼ�Ÿ��
        ValueType value; // �ɼǰ�

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
    // �ɼǰ��� �����Ѵ�. ���Կ� �����ϸ� �� �ɼǰ��� �ڵ� �����ȴ�.
    bool Insert(OptionValueInfo*& INOUT new_option_value);

    // ����Ʈ�� ���� �ɼǰ��� ã�´�.
    const OptionValueInfo* Find(const OptionPoint option_point) const;

    // ����Ʈ�� ���� �ɼǰ��� �����ϴ��� �˻��Ѵ�.
    bool IsExist(const OptionPoint option_point) const
    {
        return (option_value_map_.find(option_point) != option_value_map_.end());
    }

    // �ɼǰ� ����� ��´�.
    const OptionValueMap& GetOptionValues() const
    {
        return option_value_map_;
    }

    // �ɼ� Ÿ���� ��´�.
    eVALUE_TYPE GetOptionType() const;

public:
    ItemOptionCode item_option_code; // ������ ���� �ɼ��ڵ�
    eATTR_TYPE attr_option_code; // �Ӽ� ���� �ɼ��ڵ�
    
private:
    OptionValueMap option_value_map_;
    __DISABLE_COPY(ZardOptionInfo);
}; // ZardOptionInfo

typedef WORD SOCKETINDEX; 

// ���� �ɼ�
struct sSOCKET_OPTION
{
    typedef BYTE ItemOptionKind;
    typedef int OptionValue;
    struct OptionPointInfo // �ɼ� ����Ʈ ����
    {
        enum { kMaxListSize = 5 };
        typedef BYTE ListSizeType;

        ZardOptionInfo::ItemOptionCode item_option_code; // ������ ���� �ɼ� �ڵ�
        ZardOptionInfo::OptionPoint option_point; // �ɼ� ����Ʈ
        const ZardOptionInfo* zard_option; // ��� �ɼ� ����

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
    // �ɼ� ����Ʈ ������ ��´�.
    OptionPointInfo* GetPointInfoByMutable(const OptionPointInfo::ListSizeType index);
    // �ɼ� ����Ʈ ������ ��´�.
    const OptionPointInfo* GetPointInfo(const OptionPointInfo::ListSizeType index) const;
    // ����Ʈ ��� ���� ������ ���� �� �ɼǰ��� �����Ѵ�.
    bool SetMainOption();
    
public:
    SOCKETINDEX index;
    eSOCKET_LEVEL zard_grade; // �����
    CODETYPE name_code; // �ɼ� �̸� �ڵ� (����Ʈ ��� ���� ����/���� �������� ���� ����)
    ItemOptionKind item_option_kind; // ������ ���� �ɼ� �ڵ� (����Ʈ ��� ���� ����/���� �������� �� �ɼǰ�)
    eATTR_TYPE attr_option_kind; // �Ӽ� ���� �ɼ� �ڵ� (����Ʈ ��� ���� ���� ����/���� �������� �� �ɼǰ�)
    CODETYPE item_code; // ��� ������ �ڵ�
    CODETYPE option_image; // ��� ������ �̹���
    eVALUE_TYPE	option_type; // �ɼǰ� �Է� ���� (����Ʈ ��� ���� ���� ����/���� �������� �� �ɼǰ�)
    OptionValue option_value; // �ɼǰ� (����Ʈ ��� ���� ���� ����/���� �������� �� �ɼǰ�)
    OptionPointInfo::ListSizeType option_point_list_size; // �ɼ� ����Ʈ ��� ũ��

private:
    OptionPointInfo option_point_list_[OptionPointInfo::kMaxListSize]; // �ɼ� ����Ʈ ���
    __DISABLE_COPY(sSOCKET_OPTION);
}; // sSOCKET_OPTION


// NOTE: f110330.3L, added interface to access easy iterating
inline int sSOCKET_OPTION::GetPointArrayInfo(const OptionPointInfo** begin_it) const
{
    *begin_it = option_point_list_;
    return option_point_list_size;
}

// �ɼ� ����Ʈ ������ ��´�.
inline sSOCKET_OPTION::OptionPointInfo*
sSOCKET_OPTION::GetPointInfoByMutable(const OptionPointInfo::ListSizeType index)
{
    if (index < 0 || index >= _countof(option_point_list_))
    {
        return NULL;
    }
    return &(option_point_list_[index]);
}

// �ɼ� ����Ʈ ������ ��´�.
inline const sSOCKET_OPTION::OptionPointInfo*
sSOCKET_OPTION::GetPointInfo(const OptionPointInfo::ListSizeType index) const
{
    if (index < 0 || index >= option_point_list_size)
    {
        // �б� �������� �����ϱ� ������ ��ȿ�� �����Ͱ� ����ִ� ���������� ����Ѵ�.
        return NULL;
    }
    return &(option_point_list_[index]);
}

//==================================================================================================
// ���� �ɼ� �ļ�
class SocketOptionParser : public util::Singleton<SocketOptionParser>, public IParser
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    enum { kInvalidSocketOptionIndex = 0 };
    enum { kEmptySocketOptionIndex = 0 };
    typedef WORD ZardOptionRatioKey; // ��� + �ɼ��ڵ�
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
    // ���� �ɼ��� ã�´�.
    const sSOCKET_OPTION* const FindSocketOption(const SOCKETINDEX socket_option_index) const;

    // ���� �ɼǸ��� ��´�.
    const SocketOptionMap& GetSocketOptionMap() const
    {
        return socket_option_map_;
    }
    
    // ��޿� �ش��ϴ� ��尡 ���� �� �ִ� �ɼ� �� �ϳ��� ��´�.
    SOCKETINDEX GetRandomZardOptionByGrade(
        const ZardGrade zard_grade, 
        const ZardOptionKind zard_item_option_kind) const;

    // ��尡 ���� �� �ִ� �ɼ� �� �ϳ��� ��´�.
    SOCKETINDEX GetRandomZardOption(const CODETYPE item_code) const;

    // ��尡 ���� �� �ִ� �ɼ��� ���� �ϳ��� ��� ���� �� �ִ� �ɼ��� ��´�.
    SOCKETINDEX GetFixedZardOption(const CODETYPE item_code) const;

    // ��� �ɼ��� ã�´�.
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
    INT				m_AttrIndex;				// Ű
    CODETYPE		m_SocketItemCode;
    char			m_pszAttrName[MAX_ATTRIBUTENAME_LENGTH];
    CODETYPE		m_NCode;
    eVALUE_TYPE		m_NumericType;
    INT				m_iValue[SOCKETLV_MAX];
};

struct sSOCKETITEM_OPTION
{
    CODETYPE		m_SocketItemCode;			//< Ű
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