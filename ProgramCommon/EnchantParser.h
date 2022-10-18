#pragma once
//=======================================================================================================================
/// EnchantParser class
/**
    @author
            Kim Min Wook < taiyo@webzen.com >	
    @since
            2006. 1. 4
    @remarks
            - 인첸트를 하는데 필요한 재료들을 나타내는 스크립트
            - 인첸트 할 아이템의 레벨을 기준으로 인첸트(1~12)를 한데 묶었음
    @note
            - 
    @history
        
*/
//=======================================================================================================================

#include <SolarHashTable.h>
#include <Singleton.h>
#include <Define.h>
#include <SolarFileReader.h>
#include <ItemOptionHeader.h>
#include <ScriptCode.IParser.h>


// 10 -> 11 로 인챈 하는 상황 일경우 현재 아이템(eENCHANT_10LV) => eENCHANT_HIGH
// (LST1024)(080818) 기존 eENCHANTLEVEL과 기준이 달라서 새로 만듬.
enum eENCHANT_GRADE
{
    kEnchantErrLV = 0,
    kEnchantLV0 = (1 << 0),
    kEnchantLV1 = (1 << 1),
    kEnchantLV2 = (1 << 2),
    kEnchantLV3 = (1 << 3),
    kEnchantLV4 = (1 << 4),
    kEnchantLV5 = (1 << 5),
    kEnchantLV6 = (1 << 6),
    kEnchantLV7 = (1 << 7),
    kEnchantLV8 = (1 << 8),
    kEnchantLV9 = (1 << 9),
    kEnchantLV10 = (1 << 10),
    kEnchantLV11 = (1 << 11),
    kEnchantLV12 = (1 << 12),
    kEnchantLV13 = (1 << 13),
    kEnchantLV14 = (1 << 14),
    kEnchantLV15 = (1 << 15),
    kEnchantLow = kEnchantLV0|kEnchantLV1|kEnchantLV2|kEnchantLV3|kEnchantLV4|kEnchantLV5,
    kEnchantMiddle = kEnchantLV6|kEnchantLV7|kEnchantLV8|kEnchantLV9,
    kEnchantHigh = kEnchantLV10|kEnchantLV11,
    kEnchantHighest = kEnchantLV12|kEnchantLV13|kEnchantLV14, 
#ifdef _NA_003966_20111227_ADD_ENCHANT
    kEnchantImposible = kEnchantLV15,	
#else
    kEnchantImposible = kEnchantLV12,	
#endif
};

struct sMATERIALS
{
    SLOTCODE m_MaterialItemCode;
    WORD m_MaterialItemNum;

    sMATERIALS() : m_MaterialItemCode(0), m_MaterialItemNum(0)
    {
    }
};

struct sFARE_RATIO
{
    float m_HaimFare;
    float m_MaterialFare;

    sFARE_RATIO() : m_HaimFare(0.0f), m_MaterialFare(0.0f)
    {
    }
};

#ifdef _NA_20100307_BETAKEY_SYSTEM
struct sBETAKEY
{
    int m_betakey;
    float m_pay_ratio;

    sBETAKEY() : m_betakey(0), m_pay_ratio(0.0f)
    {
    }
};
#endif

class MATERIALS_FOR_LEVEL
{
    friend class EnchantParser;
public:
    enum { kMaxMaterialNumber = 5 };

    MATERIALS_FOR_LEVEL() : enchant_(0)
    {
    }

    bool IsDownGrade() const 
    { 
        return down_grade_.grade && down_grade_.percent; 
    }
    bool IsDownGrade100Per() const 
    { 
        return down_grade_.percent == 100; 
    }
    uint8_t GetDownGradeType() const
    { 
        return down_grade_.grade; 
    }
    uint8_t GetDownPer() const 
    { 
        return down_grade_.percent; 
    }
    uint8_t GetDownLV() const 
    { 
        return down_grade_.level; 
    }
    bool IsLimitRatio()	const 
    { 
        return limit_ratio_.is_limit_ratio;
    }
    float GetLimitRatio() const 
    { 
        return limit_ratio_.ratio; 
    }
    const sMATERIALS* GetMaterials(const int index) const
    {
        if (kMaxMaterialNumber <= index)
        {
            return NULL;
        }            
        if (materials_[index].m_MaterialItemCode == 0 || materials_[index].m_MaterialItemNum == 0)
        {
            return NULL;
        }
        return &materials_[index];
    }
    const sFARE_RATIO* GetFareRatio(const eENCHANT_OPTION option) const
    {
        if (option & eENCHANT_100PER)
        {
            return &fare_ratios_[0];
        }
        if (option & eENCHANT_75PER)
        {
            return &fare_ratios_[1];
        }
        if (option & eENCHANT_50PER)
        {
            return &fare_ratios_[2];
        }            
        if (option & eENCHANT_25PER)
        {
            return &fare_ratios_[3];
        }
        return NULL;
    }
#ifdef _NA_20100307_BETAKEY_SYSTEM
    const sBETAKEY* GetBetaKeyInfo() const
    {
        return &betakey_;
    }
#endif
private:
    struct DownGrade
    {
        uint8_t grade;
        uint8_t percent;
        uint8_t level;

        DownGrade() : grade(0), percent(0), level(0)
        {
        }
    };
    struct LimitRatio
    {
        bool is_limit_ratio;
        float ratio;

        LimitRatio() : is_limit_ratio(false), ratio(0.0f)
        {
        }
    };
private:
    enum { kMaxFareNumber = 4 };
    uint8_t enchant_;
    DownGrade down_grade_;
    LimitRatio limit_ratio_;
#ifdef _NA_20100307_BETAKEY_SYSTEM
    sBETAKEY betakey_;
#endif
    sMATERIALS materials_[kMaxMaterialNumber];
    sFARE_RATIO fare_ratios_[kMaxFareNumber];    
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
public:
        enum { kMaxAddOptionNumber = 3 };

        typedef WORD AddOptionIndexes[kMaxAddOptionNumber];    // OptionKind   [옵션 코드 입력]
        typedef BYTE AddOptionTypes[kMaxAddOptionNumber];      // OptionType   [옵션 종류 1:수치, 2:비율]
        typedef int  AddOptionValues[kMaxAddOptionNumber];     // OptionValue  [옵션 값]

        struct {
            // Normal options
            AddOptionIndexes add_option_kind_;
            AddOptionTypes   add_option_type_;
            AddOptionValues  add_option_value_;
        };
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION
};

class EnchantParser : public util::Singleton<EnchantParser>, public IParser
{
public:
    EnchantParser();
    ~EnchantParser();

public:
    virtual void Release() {}
    virtual BOOL LoadScript(eSCRIPT_CODE code, BOOL reload);

public:
    eENCHANT_GRADE GetEnchantGrade(const uint8_t enchant) const;

    POSTYPE	CalcMaterialNum(const uint16_t material_number, const double material_fare) const;

    int	PercentOptionToInt(const eENCHANT_OPTION option) const;

    const MATERIALS_FOR_LEVEL* GetEnchantMatInfo(
        const LEVELTYPE level, 
        const uint8_t enchant, 
        const bool is_weapon, 
        const bool is_limited, 
        const eENCHANT_OPTION option = eENCHANT_NOT_OPT
    ) const;

#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    const MATERIALS_FOR_LEVEL* GetEnchantInfo(const bool is_weapon, 
                                                 const LEVELTYPE item_level,
                                                 const uint8_t enchant) const;
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION

    float GetLimitRatio( 
        const LEVELTYPE level, 
        const uint8_t enchant, 
        const bool is_weapon, 
        const bool is_limited
    ) const;    
    
    bool IsOption100Per(const eENCHANT_OPTION option) const
    { 
        return option & eENCHANT_100PER;
    }
    
private:
    enum
    {
        kMinNormalCode	= 1, 
        kMinLimitedCode = 10001, 
        kMinCashNormalCode = 20001, 
        kMinCashLimitedCode = 30001, 
        kMaxCode = 40000
    };
    struct Material
    {	
        enum ItemType
        { 
            kWeapon = 0, 
            kArmor = 1, 
            kMaxItemType 
        };
        Material() : item_type(kMaxItemType), item_level(0)
        {
        }
        uint8_t	item_type;
        LEVELTYPE item_level;
        MATERIALS_FOR_LEVEL normals[MAX_ENCHANT_GRADE];			
        MATERIALS_FOR_LEVEL limiteds[MAX_ENCHANT_GRADE];			
        MATERIALS_FOR_LEVEL cash_normals[MAX_ENCHANT_GRADE];		
        MATERIALS_FOR_LEVEL cash_limiteds[MAX_ENCHANT_GRADE];		
    };
    typedef STLX_HASH_MAP<LEVELTYPE, Material> Materials;

private:
    bool LoadScript(const bool reload);
    
    bool IsValid(
        const int row, 
        const int code, 
        const uint8_t item_type, 
        const MATERIALS_FOR_LEVEL& materal_info
    ) const;

private:    
    Materials weapon_materials_;
    Materials armor_materials_;
};