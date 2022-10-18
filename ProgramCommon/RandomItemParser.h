////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   RandomItemParser.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/11/22 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PROGRAMCOMMON_RANDOMITEMPARSER_H_
#define PROGRAMCOMMON_RANDOMITEMPARSER_H_

#ifdef _SERVER
#include <boost/pool/object_pool.hpp>
#endif // _SERVER
#include "ScriptCode.IParser.h"
#include "Singleton.h"
#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
#include "RatioSelector.h"

struct MSG_CG_ITEM_RANDOMIZE_SYN;
class CCSunOnline;
class SCItemSlot;
class SCItemSlotContainer;


// 랜덤 아이템 파서
class RandomItemParser : public util::Singleton<RandomItemParser>, public IParser
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    struct OptionParam
    {
        enum { kMaxKeySize = 5 };
        typedef uint16_t Key;
        OptionParam() : is_valid(false), size(0)
        {
            ::ZeroMemory(keys, sizeof(keys));
            for (int i = 0; i < _countof(grades); ++i)
            {
                grades[i] = RandomValueGrade::kNone;
            }
        }
        bool is_valid;
        uint8_t size;
        Key keys[kMaxKeySize];
        RandomValueGrade::Value grades[kMaxKeySize];
    }; // OptionParam


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    RandomItemParser();
    ~RandomItemParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release() {}
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    // 랜덤화 가능한지 검사한다.
    bool EnableRandomize(const BASE_ITEMINFO& item_info) const;

    // 옵션을 랜덤화 한다.
    OptionParam RandomizeOption(
        const RandomValueTypeGrade::Value value_type_grade, const BASE_ITEMINFO& item_info) const;

    // 아이템 정보에 옵션을 만든다.
    bool CreateOption(OptionParam& INOUT param, BASE_ITEMINFO& OUT item_info) const;

    // 랜덤화 아이템이 사용 가능한지 검사한다.
    bool EnableItemRandomize(
        const SCItemSlotContainer& inventory, 
        const POSTYPE request_item_pos, 
        const POSTYPE target_item_pos, 
        SCItemSlot*& OUT request_item, 
        SCItemSlot*& OUT target_item) const;

    // 랜덤화 아이템 사용 요청을 만든다.
    bool GetItemRandomizeRequest(
        const SCItemSlotContainer& inventory, 
        const POSTYPE request_item_pos, 
        const POSTYPE target_item_pos, 
        MSG_CG_ITEM_RANDOMIZE_SYN& OUT request) const;

    // 메모리풀에서 아이템 정보를 생성한다.
    BASE_ITEMINFO* ConstructItemInfo()
    {
#ifdef _SERVER
#ifdef _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
        BASE_ITEMINFO* const item_info = TAllocNew(BASE_ITEMINFO);
#else
        BASE_ITEMINFO* const item_info = item_info_pool_.construct();
#endif //_NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
#else
        BASE_ITEMINFO* const item_info = new BASE_ITEMINFO;
#endif // _SERVER
        if (item_info != NULL)
        {
            if (created_item_infos_.find(item_info) != created_item_infos_.end())
            {
                // 새로 생성 했는데 같은 주소를 사용하는 객체가 이미 존재한다.
                return NULL;
            }
            created_item_infos_.insert(item_info);
        }
        return item_info;
    }

    // 메모리풀에서 아이템 정보를 삭제한다.
    void DestroyItemInfo(BASE_ITEMINFO* const item_info)
    {
        if (item_info == NULL)
        {
            return;
        }
        CreatedItemInfos::iterator it = created_item_infos_.find(item_info);
        if (it == created_item_infos_.end())
        {
            // 여기서 생성한 객체가 아닌데 삭제하려고 시도했다.
            return;
        }
        created_item_infos_.erase(it);
#ifdef _SERVER
#ifdef _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
        TAllocDelete(BASE_ITEMINFO, item_info);
#else
        item_info_pool_.destroy(item_info);
#endif //_NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
#else
        delete item_info;
#endif // _SERVER
    }
    

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    template <typename PartT>
    struct IHasParts
    {
        typedef typename PartT::Order PartOrder;
        typedef typename PartT::Ratio PartRatio;
        typedef STLX_VECTOR<PartT> Parts;
        typedef RatioSelector<PartOrder, PartRatio, PartT::kMaxRatio, RatioAccessMode::kConst> PartSelector;

        const PartT* GetPart(const PartOrder order) const
        {
            if (order < PartT::kMinOrder || order > PartT::kMaxOrder || order > parts.size())
            {
                return NULL;
            }
            return &(parts[order-1]);
        }

        const PartT* SelectPart() const
        {
            const PartRatio ratio = rand() % part_selector.GetTotalRatio();
            PartOrder order = 0;
            if (part_selector.Select(ratio, order) == false)
            {
                return NULL;
            }
            return GetPart(order);
        }

        Parts parts;
        PartSelector part_selector;
    }; // IHasParts


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤값
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct RandomValuePart
    {
        enum OrderBoundary
        {
            kMinOrder = 1, 
            kMaxOrder = 10
        };
        enum ValueBoundary
        {
#ifdef _NA_008540_20151027_ADD_ITEMOPTION_ELITE4
            kMinValue = -10000, 
            kMaxValue = 100000
#else
            kMinValue = -100, 
            kMaxValue = 9999
#endif //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
        };
        enum RatioBoundary
        {
            kMinRatio = 1, 
            kMaxRatio = 1000
        };
        typedef uint8_t Order;
        typedef int Value;
        typedef uint16_t Ratio;

        Order order; // 순서
        Value my_value; // 값
        Ratio ratio; // 발생확률
        RandomValueGrade::Value grade; // 등급

        RandomValuePart() : order(0), my_value(0), ratio(0), grade(RandomValueGrade::kNone)
        {
        }
    }; // RandomValuePart

    struct RandomValue : public IHasParts<RandomValuePart>
    {
        enum IndexBoundary
        { 
            kMinIndex = 1, 
            kMaxIndex = 9999 
        };
        typedef uint16_t Index;
        
        RandomValue() : index(0)
        {
        }

        Index index;
    }; // RandomValue

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤값 관리자
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class RandomValueManager
    {
    public:
        RandomValueManager() {}
        ~RandomValueManager() {}
    
    public:
        bool LoadScript(const RandomItemParser& parser, const bool is_reload);
        
        const RandomValue* GetValue(const RandomValue::Index index) const
        {
            RandomValues::const_iterator it = random_values_.find(index);
            if (it != random_values_.end())
            {
                return &(it->second);
            }
            return NULL;
        }
    
    private:
        typedef STLX_HASH_MAP<RandomValue::Index, RandomValue> RandomValues;
        RandomValues random_values_;
        __DISABLE_COPY(RandomValueManager);
    }; // RandomValueManager

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤옵션
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct RandomOptionPart
    {
        enum OrderBoundary
        {
            kMinOrder = 1, 
            kMaxOrder = 100
        };
        enum ValueGrade
        {
            kValueGrade0 = 0, 
            kValueGrade1 = 1, 
            kValueGradeSize
        };
        enum RatioBoundary
        {
            kMinRatio = 1, 
            kMaxRatio = 1000
        };
        typedef uint8_t Order;
        typedef uint8_t ActiveType;
        typedef uint16_t Kind;
        typedef uint8_t ValueType;
        typedef uint16_t Ratio;

        Order order; // 순서
        bool is_empty; // 빈옵션 여부
        ActiveType active_type; // 옵션발동조건
        Kind kind; // 옵션종류
        ValueType value_type; //옵션값종류
        const RandomValue* random_values[kValueGradeSize]; // 캐시수준별랜덤값
        Ratio ratio; // 발생확률
        
        RandomOptionPart() : order(0), is_empty(false), active_type(0), kind(0), value_type(0), ratio(0)
        {
            ::ZeroMemory(random_values, sizeof(random_values));
        }

        const RandomValue* GetValue(const RandomValueTypeGrade::Value grade) const
        {
            if ((grade-1) < kValueGrade0 || (grade-1) >= kValueGradeSize)
            {
                return NULL;
            }
            return random_values[grade-1];
        }
    }; // RandomOptionPart

    struct RandomKey;

    struct RandomOption : public IHasParts<RandomOptionPart>
    {
        enum IndexBoundary
        { 
            kMinIndex = 1, 
            kMaxIndex = 9999 
        };
        typedef uint16_t Index;
        typedef STLX_MAP<uint16_t, const RandomKey*> Keys;
        
        RandomOption() : index(0)
        {
        }

        Index index;
        Keys keys;
    }; // RandomOption

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤옵션 관리자
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class RandomOptionManager
    {
    public:
        RandomOptionManager() {}
        ~RandomOptionManager() {}

    public:
        bool LoadScript(const RandomItemParser& parser, const bool is_reload);

        const RandomOption* GetOption(const RandomOption::Index index) const
        {
            RandomOptions::const_iterator it = random_options_.find(index);
            if (it != random_options_.end())
            {
                return &(it->second);
            }
            return NULL;
        }

    private:
        typedef STLX_HASH_MAP<RandomOption::Index, RandomOption> RandomOptions;
        RandomOptions random_options_;
        __DISABLE_COPY(RandomOptionManager);
    }; // RandomOptionManager

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤아이템
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct RandomItem
    {
        enum { kMaxOptionSize = 5 };
        typedef uint32_t ItemCode;
        typedef STLX_VECTOR<const RandomOption*> Options;

        RandomItem() : item_code(0)
        {
        }

        const RandomOption* GetOption(const size_t index) const
        {
            if (index < 0 || index >= kMaxOptionSize || index >= options.size())
            {
                return NULL;
            }
            return options[index];
        }

        ItemCode item_code;
        Options options;
    }; // RandomItem


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤아이템 관리자
    ////////////////////////////////////////////////////////////////////////////////////////////////

    class RandomKeyManager;

    class RandomItemManager
    {
    public:
        RandomItemManager() {}
        ~RandomItemManager() {}

    public:
        bool LoadScript(const RandomItemParser& parser, const bool is_reload);

        const RandomItem* GetItem(const RandomItem::ItemCode item_code) const
        {
            RandomItems::const_iterator it = random_items_.find(item_code);
            if (it != random_items_.end())
            {
                return &(it->second);
            }
            return NULL;
        }
    
    private:
        typedef STLX_MAP<RandomItem::ItemCode, RandomItem> RandomItems;
        RandomItems random_items_;
        __DISABLE_COPY(RandomItemManager);
        friend class RandomKeyManager;
        friend class CCSunOnline;
    }; // RandomItemManager


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 랜덤키 관리자
    ////////////////////////////////////////////////////////////////////////////////////////////////

    struct RandomKey
    {
        enum KeyBoundary
        { 
            kMinKey = 1, 
            kMaxKey = 0xFFFF // 주의!! 2^16 초과하면 스트림 확장 필요하다.
        };
        typedef uint16_t Key;

        RandomKey() : key(0), option_part(NULL), value_part(NULL)
        {
        }

        Key key;
        const RandomOptionPart* option_part;
        const RandomValuePart* value_part;
    }; // RandomKey

    class RandomKeyManager
    {
    public:
        RandomKeyManager() {}
        ~RandomKeyManager() {}

    public:
        bool LoadScript(RandomItemParser& parser, const bool is_reload);

        const RandomKey* GetKey(const RandomKey::Key key) const
        {
            RandomKeys::const_iterator it = random_keys_.find(key);
            if (it != random_keys_.end())
            {
                return &(it->second);
            }
            return NULL;
        }

        const RandomKey* GetKey(
            const RandomOption::Index option_index, 
            const RandomOptionPart::Order option_order, 
            const RandomValue::Index value_index, 
            const RandomValuePart::Order value_order) const
        {
            const KeyData key_data = 
                CreateKeyData(option_index, option_order, value_index, value_order);
            KeyDatas::const_iterator it = key_datas_.find(key_data);
            if (it != key_datas_.end())
            {
                return it->second;
            }
            return NULL;
        }

    private:
        typedef uint64_t KeyData;
        typedef STLX_MAP<RandomKey::Key, RandomKey> RandomKeys;
        typedef STLX_HASH_MAP<KeyData, const RandomKey*> KeyDatas;

        KeyData CreateKeyData(
            const RandomOption::Index option_index, 
            const RandomOptionPart::Order option_order, 
            const RandomValue::Index value_index, 
            const RandomValuePart::Order value_order) const
        {
            BOOST_STATIC_ASSERT(sizeof(RandomOption::Index) <= 2);
            BOOST_STATIC_ASSERT(sizeof(RandomOptionPart::Order) <= 2);
            BOOST_STATIC_ASSERT(sizeof(RandomValue::Index) <= 2);
            BOOST_STATIC_ASSERT(sizeof(RandomValuePart::Order) <= 2);
            BOOST_STATIC_ASSERT(sizeof(KeyData) == sizeof(uint64_t));
            const uint32_t option_part = MAKELONG(option_index, option_order);
            const uint32_t value_part = MAKELONG(value_index, value_order);
            return static_cast<uint64_t>(option_part | (static_cast<uint64_t>(value_part) << 32));
        }

        bool InsertKeysToOption(RandomItemParser& parser) const;

        RandomKeys random_keys_;
        KeyDatas key_datas_;
        __DISABLE_COPY(RandomKeyManager);
        friend class CCSunOnline;
    }; // RandomKeyManager

private:
    RandomValueManager value_manager_;
    RandomOptionManager option_manager_;
    RandomItemManager item_manager_;
    RandomKeyManager key_manager_;
    typedef STLX_HASH_SET<BASE_ITEMINFO*> CreatedItemInfos;
    CreatedItemInfos created_item_infos_;
#ifdef _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
#else
#ifdef _SERVER
    enum { kItemInfoPoolSize = 1024 };
    typedef boost::object_pool<BASE_ITEMINFO> ItemInfoPool;
    ItemInfoPool item_info_pool_; 
#endif // _SERVER
#endif //_NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM
    friend class CCSunOnline;
}; // RandomItemParser

#endif // PROGRAMCOMMON_RANDOMITEMPARSER_H_