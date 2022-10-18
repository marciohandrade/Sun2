////////////////////////////////////////////////////////////////////////////////
///
/// @file   CubeStruct.h
/// @author hukim
/// @brief  [2010/08/24 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PROGRAMCOMMON_CUBESYSTEM_CUBESTRUCT_H_
#define PROGRAMCOMMON_CUBESYSTEM_CUBESTRUCT_H_

#include <SCSlotHeader.h>
#include <boost/array.hpp>

namespace util {
namespace internal {
template <typename ELEM_T, std::size_t N>
class StaticArray : public boost::array<ELEM_T, N> // 정적 배열을 구현한 템플릿
{
public:
    StaticArray() : size_(0)
    {
    }

public:
    // 반복자
    iterator end() { return elems+size_; }
    const_iterator end() const { return elems+size_; }

    // 접근자
    reference back() { return elems[size_-1]; }
    const_reference back() const { return elems[size_-1]; }

    // 크기
    size_type size() const { return size_; } // 배열의 현재 크기를 얻는다.
    void set_size(size_type new_size) // 배열의 현재 크기를 설정한다.
    { 
        FASSERT(new_size <= N);
        size_ = new_size;
    }
    size_type increse_size(const size_type add_size) // 배열 크기를 증가시킨다.
    {
        FASSERT((add_size + size_) <= N);
        size_ = size_ + add_size;
        return size_;
    }
    bool empty() const { return size_ == 0; } // 배열이 비었는지 검사한다.
    bool full() const { return size_ == N; } // 배열이 모두 찼는지 검사한다.
    void clear() { size_ = 0; } // 배열을 비운다.    
    
    // 삽입/삭제
    void push_back(const_reference elem) // 끝에 삽입한다.
    {
        FASSERT(!full());
        elems[size_] = elem;
        ++size_;
    }

    // 탐색
    const_iterator find(const_reference elem) const // 해당 요소를 찾는다.
    {
        for (size_type i = 0; i < size_; ++i)
        {
            if (elems[i] == elem)
            {
                return (elems+i);
            }
        }
        return end();
    }
    
private:
    size_type size_;
}; // StaticArray
} // internal
} // util

// 합성 정보
struct CompositionInfo
{
    enum 
    { 
        kUnused = 0, // 사용 안함
        kUsed = 1 // 사용
    }; 
    typedef CODETYPE CompositionCode;
    typedef BYTE ColumnNumber;
    struct MaterialInfo  // 재료 정보
    {
        enum { kMaxSize = 9 }; // 재료 최대 개수
        typedef CODETYPE MaterialCode;
        typedef CODETYPE MaterialTypeCode;
        typedef LEVELTYPE MaterialLevel;
        typedef DURATYPE MaterialNumber;
        typedef KindOfItemType::value_type MaterialIGType;

        // 초기화
        explicit MaterialInfo() : 
            is_used_(false), 
            column_number_(kUnused), 
            code_(kUnused), 
            type_code_(kUnused), 
            level_(kUnused), 
            number_(kUnused), 
            ig_type_(KindOfItemType::Default), 
            inventory_position_(kUnused)
        {
        }

        bool is_used_; // 재료 사용 여부
        ColumnNumber column_number_; // 칼럼 번호
        MaterialCode code_; // 재료 코드
        MaterialTypeCode type_code_; // 재료 종류 코드
        MaterialLevel level_; // 재료 레벨
        MaterialNumber number_; // 재료 개수
        MaterialIGType ig_type_; // 재료 아이템 타입 (일반, 디바인, 일반 에테리아, 디바인 에테리아, 외형)
        POSTYPE inventory_position_; // 재료의 인벤토리 위치        
    }; // MaterialInfo
    typedef util::internal::StaticArray<MaterialInfo, MaterialInfo::kMaxSize> MaterialList; // 재료 목록
    struct RewardInfo // 보상 정보
    {
        enum { kMaxSize = 9 }; // 보상 최대 개수
        enum // 보상 종류
        {
            kItemCode = 1, // 아이템 코드
            kRewardCode = 2 // 보상 코드
        };
        enum { kMaxRewardRatioSize = 1000 }; // 최대 보상 확률
        typedef int RewardType;
        typedef CODETYPE RewardCode;
        typedef KindOfItemType::value_type RewardIGType;
        typedef WORD RewardRatio;
        typedef DURATYPE RewardNumber;

        // 초기화
        explicit RewardInfo() : 
            is_used_(false), 
            column_number_(kUnused), 
            type_(kUnused), 
            code_(kUnused), 
            ig_type_(KindOfItemType::Default), 
            ratio_(kUnused), 
            number_(kUnused)
        {
        }

        bool is_used_; // 보상 사용 여부
        ColumnNumber column_number_; // 칼럼 번호
        RewardType type_; // 보상 종류
        RewardCode code_; // 보상 코드
        RewardIGType ig_type_; // 보상 아이템 타입 (일반, 디바인, 일반 에테리아, 디바인 에테리아, 외형)
        RewardRatio ratio_; // 보상 확률
        RewardNumber number_; // 보상 개수
    }; // RewardInfo
    typedef util::internal::StaticArray<RewardInfo, RewardInfo::kMaxSize> RewardList; // 보상 목록
    typedef int Weight;

    // 초기화
    explicit CompositionInfo(const CompositionCode code = kUnused) : code_(code), weight_(kUnused)
    {
    }

    CompositionCode code_; // 합성 코드
    MaterialList material_list_; // 재료 목록
    RewardList reward_list_; // 보상 목록
    Weight weight_; // 가중치 (입력한 재료가 복수 합성이 가능할 때 가중치가 높은 순으로 선택)
}; // CompositionInfo
typedef STLX_MAP<CompositionInfo::CompositionCode, CompositionInfo*> CompositionMap; // 합성정보 맵

// 일치하는 합성 정보
struct MatchedCompositionInfo
{
    typedef CompositionInfo::CompositionCode CompositionCode;
    enum { kUnused = 0 }; // 사용 안함
    enum { kMaxSize = 20 }; // 일치하는 합성 정보 최대 개수
    struct MatchedMaterialInfo // 일치하는 재료 정보
    {
        // 초기화
        explicit MatchedMaterialInfo()
        {
            Clear();
        }
        void Clear()
        {
            is_find_ = false;
            composition_material_ = NULL;
            is_mixed_input_material_ = false;
            input_material_start_pos_ = NULL;
            input_material_end_pos_ = NULL;
        }

        bool is_find_; // 일치하는 재료 발견 여부
        const CompositionInfo::MaterialInfo* composition_material_; // 일치한 합성 재료
        bool is_mixed_input_material_; // 코드가 같은 입력 재료 합침 여부
        CompositionInfo::MaterialList::const_iterator input_material_start_pos_; // 일치한 입력 재료의 시작 위치
        CompositionInfo::MaterialList::const_iterator input_material_end_pos_; // 일치한 입룍 재료의 마지막 위치
    };
    typedef util::internal::StaticArray<MatchedMaterialInfo, CompositionInfo::MaterialInfo::kMaxSize> MatchedMaterialList;

    // 초기화
    explicit MatchedCompositionInfo() : composition_(NULL)
    {
    }

    const CompositionInfo* composition_; // 일치한 합성
    MatchedMaterialList matched_material_list_; // 일치하는 재료 목록
}; // MatchedCompositionInfo
typedef util::internal::StaticArray<MatchedCompositionInfo, MatchedCompositionInfo::kMaxSize> MatchedCompositionList; // 일치하는 합성 목록

// 분해 정보
struct DecompositionInfo
{
    enum { kUnused = 0 }; // 사용 안함
    typedef WORD DecompositionPoint; // 분해 포인트
    struct ZardInfo // 쟈드 정보
    {
        enum { kMaxSize = 4 }; // 쟈드 최대 개수
        enum // 쟈드 등급
        {
            kGradeLv1 = 1, // 하급
            kGradeLv2 = 2, // 중급
            kGradeLv3 = 3, // 상급
            kGradeLv4 = 4 // 최상급
        };
        typedef int ZardGrade;
        typedef BYTE ZardNumber;

        ZardGrade grade_; // 쟈드 등급
        ZardNumber number_; // 쟈드 개수

        // 초기화
        explicit ZardInfo() : grade_(kUnused), number_(kUnused)
        {
        }
    }; // ZardInfo
    typedef util::internal::StaticArray<ZardInfo, ZardInfo::kMaxSize> ZardList; // 쟈드 목록
    struct MaterialInfo // 재료 정보
    {
        enum { kMaxSize = 9 }; // 쟈드 최대 개수
        typedef BYTE Enchant;
        typedef BYTE SocketNumber;
        typedef LEVELTYPE ItemLevel;
        typedef DURATYPE MaterialNumber;

        Enchant enchant_; // 인첸트
        SocketNumber socket_number_; // 소켓 개수
        ItemLevel item_level_; // 아이템 레벨
        ZardList zard_list_; // 쟈드 목록
        MaterialNumber number_; // 재료 개수
        POSTYPE inventory_position_; // 재료의 인벤토리 위치

        // 초기화
        explicit MaterialInfo() : 
            enchant_(kUnused), 
            socket_number_(kUnused), 
            item_level_(kUnused), 
            number_(kUnused), 
            inventory_position_(kUnused)
        {
        }
    }; // MaterialInfo
    typedef util::internal::StaticArray<MaterialInfo, MaterialInfo::kMaxSize> MaterialList; // 재료 목록
    struct RewardInfo // 보상 정보
    {    
        enum { kMaxSize = 9 }; // 보상 최대 개수
        enum // 보상 종류
        {
            kItemCode = 1, // 아이템 코드
            kZard = 2 // 쟈드
        };
        enum { kMaxRewardRatioSize = 100000 }; // 최대 보상 확률
        typedef int RewardType;
        typedef CODETYPE RewardCode;
        typedef int RewardRatio;

        RewardType type_; // 보상 종류
        RewardCode code_; // 보상 코드 (보상 종류가 쟈드일때는 쟈드 정보의 등급 참조)
        RewardCode min_code_; // 아이템 스크립트에서 쟈드의 최소 아이템 코드
        RewardCode max_code_; // 아이템 스크립트에서 쟈드의 최대 아이템 코드
        RewardRatio ratio_; // 분해 포인트 1당 보상 확률
        STLX_VECTOR<RewardCode> zard_list; // 아이템 스크립트에서 최소~최대 범위안에 있는 쟈드인 아이템 목록 

        // 초기화
        explicit RewardInfo() : 
            type_(kUnused), code_(kUnused), min_code_(kUnused), max_code_(kUnused), ratio_(kUnused)
        {
        }

        // 보상 받을 쟈드의 아이템 코드를 선택한다.
        RewardCode SelectZard() const
        {
            if (type_ != kZard || zard_list.empty())
            {
                return 0;
            }
            const int zard_index = random(0, zard_list.size()-1);
            return zard_list[zard_index];
        }
    }; // RewardInfo
    typedef util::internal::StaticArray<RewardInfo, RewardInfo::kMaxSize> RewardList; // 보상 목록
}; // DecompositionInfo

// 재료의 인벤토리 위치 목록
typedef util::internal::StaticArray<POSTYPE, CompositionInfo::MaterialInfo::kMaxSize> MaterialInventoryPositionList;

#endif //}} PROGRAMCOMMON_CUBESYSTEM_CUBESTRUCT_H_