////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   SCRandomInfo.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/12/13 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SOLARSLOT_SCRANDOMINFO_H_
#define SOLARSLOT_SCRANDOMINFO_H_

#include "SCSlotStruct.h"

class CCSunOnline;

/// @brief This is SCRandomInfo.
class SCRandomInfo
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    SCRandomInfo();
    ~SCRandomInfo();

    // 혹시나 기본 복사가 일어날 경우를 대비한 안전 코드
private:
    SCRandomInfo(const SCRandomInfo& src)
    {
        Copy(src);
    }
    SCRandomInfo& operator=(const SCRandomInfo& src)
    {
        if (this != &src)
        {
            Copy(src);
        }
        return *this;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    void Clear();

    void Copy(const SCRandomInfo& src);

    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void SetInfo(const SERIALIZE_ITEM_OPTION_TYPE_T& src, const BASE_ITEMINFO* const item_info)
    {
        option_info_.Set(src);
        if (item_info != NULL)
        {
            CreateItemInfo(*item_info);
        }
    }

    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void GetInfo(SERIALIZE_ITEM_OPTION_TYPE_T* const dest) const
    {
        option_info_.Get(dest);
    }

    // 랜덤화된 아이템 정보를 얻는다.
    const BASE_ITEMINFO* GetItemInfo() const
    {
        return item_info_;
    }

    // 랜덤화 가능한지 검사한다.
    bool EnableRandomize(const BASE_ITEMINFO& org_item_info) const;

    // 랜덤화 한다.
    bool Randomize(
        const RandomValueTypeGrade::Value value_type_grade, const BASE_ITEMINFO& org_item_info);

    // 랜덤 정보를 기반으로 아이템 정보를 생성한다.
    bool CreateItemInfo(const BASE_ITEMINFO& org_item_info);

    // 랜덤화된 옵션의 등급을 얻는다.
    RandomValueGrade::Value GetRandomOptionGrade(const uint8_t index) const;
    
    // 옵션의 문자열 표현을 얻는다.
    template < size_t kConvStrBufSize >
    void OptionToString(
        const RandomValueTypeGrade::Value value_type_grade, 
        TCHAR OUT conv_str[]
    ) const {
        option_info_.ToString<kConvStrBufSize>(value_type_grade, conv_str);
    }

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    struct OptionInfo
    {
        enum KeyBoundary
        { 
            kMinKey = 1, 
            kMaxKey = 0xFFFF // 주의!! 2^16 초과하면 스트림 확장 필요하다.
        };
        enum { kMaxOptionSize = 5 };
        enum { kStringBufferSize = 128 };
        typedef uint16_t Key;
        
        OptionInfo()
        {
            Clear();
        }

        void Clear()
        {
            size = 0;
            ::ZeroMemory(keys, sizeof(keys));
            for (int i = 0; i < _countof(grades); ++i)
            {
                grades[i] = RandomValueGrade::kNone;
            }
        }

        void Copy(const OptionInfo& src)
        {
            *this = src;
        }

        template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
        void Set(const SERIALIZE_ITEM_OPTION_TYPE_T& src)
        {
            Clear();

            BOOST_STATIC_ASSERT(_countof(src.RandomOptions) == _countof(keys));
            for (int i = 0; i < _countof(src.RandomOptions); ++i)
            {
                const ulong temp_key = src.RandomOptions[i];
                if (temp_key < kMinKey || temp_key > kMaxKey)
                {
                    break;
                }
                const Key option_key = static_cast<Key>(src.RandomOptions[i]);
                keys[i] = option_key;
                ++size;
            }
        }

        template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
        void Get(SERIALIZE_ITEM_OPTION_TYPE_T* const dest) const
        {
            BOOST_STATIC_ASSERT(_countof(dest->RandomOptions) == _countof(keys));
            ::ZeroMemory(dest->RandomOptions, sizeof(dest->RandomOptions));
            if (size > _countof(keys))
            {
                return;
            }
            for (int i = 0; i < size; ++i)
            {
                (dest->RandomOptions)[i] = keys[i];
            }
        }

        template < size_t kConvStrBufSize >
        void ToString(const RandomValueTypeGrade::Value value_type_grade, TCHAR OUT conv_str[]) const
        {
            BOOST_STATIC_ASSERT(kConvStrBufSize >= kStringBufferSize);
            BOOST_STATIC_ASSERT(kMaxOptionSize == 5);
            ::_sntprintf(
                conv_str, 
                kConvStrBufSize, 
                _T("GRADE:%u,SIZE:%u,OPT:%u,%u,%u,%u,%u"), 
                value_type_grade, 
                size, 
                keys[0],
                keys[1], 
                keys[2], 
                keys[3], 
                keys[4]
            );
            conv_str[kConvStrBufSize-1] = _T('\0');
        }

        uint8_t size;
        Key keys[kMaxOptionSize];
        RandomValueGrade::Value grades[kMaxOptionSize];
    }; // OptionInfo
    
    BASE_ITEMINFO* item_info_; 
    OptionInfo option_info_;
    friend class SCItemSlot;
    friend class CCSunOnline;
}; // SCRandomInfo

#endif // SOLARSLOT_SCRANDOMINFO_H_  