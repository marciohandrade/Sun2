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
class StaticArray : public boost::array<ELEM_T, N> // ���� �迭�� ������ ���ø�
{
public:
    StaticArray() : size_(0)
    {
    }

public:
    // �ݺ���
    iterator end() { return elems+size_; }
    const_iterator end() const { return elems+size_; }

    // ������
    reference back() { return elems[size_-1]; }
    const_reference back() const { return elems[size_-1]; }

    // ũ��
    size_type size() const { return size_; } // �迭�� ���� ũ�⸦ ��´�.
    void set_size(size_type new_size) // �迭�� ���� ũ�⸦ �����Ѵ�.
    { 
        FASSERT(new_size <= N);
        size_ = new_size;
    }
    size_type increse_size(const size_type add_size) // �迭 ũ�⸦ ������Ų��.
    {
        FASSERT((add_size + size_) <= N);
        size_ = size_ + add_size;
        return size_;
    }
    bool empty() const { return size_ == 0; } // �迭�� ������� �˻��Ѵ�.
    bool full() const { return size_ == N; } // �迭�� ��� á���� �˻��Ѵ�.
    void clear() { size_ = 0; } // �迭�� ����.    
    
    // ����/����
    void push_back(const_reference elem) // ���� �����Ѵ�.
    {
        FASSERT(!full());
        elems[size_] = elem;
        ++size_;
    }

    // Ž��
    const_iterator find(const_reference elem) const // �ش� ��Ҹ� ã�´�.
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

// �ռ� ����
struct CompositionInfo
{
    enum 
    { 
        kUnused = 0, // ��� ����
        kUsed = 1 // ���
    }; 
    typedef CODETYPE CompositionCode;
    typedef BYTE ColumnNumber;
    struct MaterialInfo  // ��� ����
    {
        enum { kMaxSize = 9 }; // ��� �ִ� ����
        typedef CODETYPE MaterialCode;
        typedef CODETYPE MaterialTypeCode;
        typedef LEVELTYPE MaterialLevel;
        typedef DURATYPE MaterialNumber;
        typedef KindOfItemType::value_type MaterialIGType;

        // �ʱ�ȭ
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

        bool is_used_; // ��� ��� ����
        ColumnNumber column_number_; // Į�� ��ȣ
        MaterialCode code_; // ��� �ڵ�
        MaterialTypeCode type_code_; // ��� ���� �ڵ�
        MaterialLevel level_; // ��� ����
        MaterialNumber number_; // ��� ����
        MaterialIGType ig_type_; // ��� ������ Ÿ�� (�Ϲ�, �����, �Ϲ� ���׸���, ����� ���׸���, ����)
        POSTYPE inventory_position_; // ����� �κ��丮 ��ġ        
    }; // MaterialInfo
    typedef util::internal::StaticArray<MaterialInfo, MaterialInfo::kMaxSize> MaterialList; // ��� ���
    struct RewardInfo // ���� ����
    {
        enum { kMaxSize = 9 }; // ���� �ִ� ����
        enum // ���� ����
        {
            kItemCode = 1, // ������ �ڵ�
            kRewardCode = 2 // ���� �ڵ�
        };
        enum { kMaxRewardRatioSize = 1000 }; // �ִ� ���� Ȯ��
        typedef int RewardType;
        typedef CODETYPE RewardCode;
        typedef KindOfItemType::value_type RewardIGType;
        typedef WORD RewardRatio;
        typedef DURATYPE RewardNumber;

        // �ʱ�ȭ
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

        bool is_used_; // ���� ��� ����
        ColumnNumber column_number_; // Į�� ��ȣ
        RewardType type_; // ���� ����
        RewardCode code_; // ���� �ڵ�
        RewardIGType ig_type_; // ���� ������ Ÿ�� (�Ϲ�, �����, �Ϲ� ���׸���, ����� ���׸���, ����)
        RewardRatio ratio_; // ���� Ȯ��
        RewardNumber number_; // ���� ����
    }; // RewardInfo
    typedef util::internal::StaticArray<RewardInfo, RewardInfo::kMaxSize> RewardList; // ���� ���
    typedef int Weight;

    // �ʱ�ȭ
    explicit CompositionInfo(const CompositionCode code = kUnused) : code_(code), weight_(kUnused)
    {
    }

    CompositionCode code_; // �ռ� �ڵ�
    MaterialList material_list_; // ��� ���
    RewardList reward_list_; // ���� ���
    Weight weight_; // ����ġ (�Է��� ��ᰡ ���� �ռ��� ������ �� ����ġ�� ���� ������ ����)
}; // CompositionInfo
typedef STLX_MAP<CompositionInfo::CompositionCode, CompositionInfo*> CompositionMap; // �ռ����� ��

// ��ġ�ϴ� �ռ� ����
struct MatchedCompositionInfo
{
    typedef CompositionInfo::CompositionCode CompositionCode;
    enum { kUnused = 0 }; // ��� ����
    enum { kMaxSize = 20 }; // ��ġ�ϴ� �ռ� ���� �ִ� ����
    struct MatchedMaterialInfo // ��ġ�ϴ� ��� ����
    {
        // �ʱ�ȭ
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

        bool is_find_; // ��ġ�ϴ� ��� �߰� ����
        const CompositionInfo::MaterialInfo* composition_material_; // ��ġ�� �ռ� ���
        bool is_mixed_input_material_; // �ڵ尡 ���� �Է� ��� ��ħ ����
        CompositionInfo::MaterialList::const_iterator input_material_start_pos_; // ��ġ�� �Է� ����� ���� ��ġ
        CompositionInfo::MaterialList::const_iterator input_material_end_pos_; // ��ġ�� �ԏ� ����� ������ ��ġ
    };
    typedef util::internal::StaticArray<MatchedMaterialInfo, CompositionInfo::MaterialInfo::kMaxSize> MatchedMaterialList;

    // �ʱ�ȭ
    explicit MatchedCompositionInfo() : composition_(NULL)
    {
    }

    const CompositionInfo* composition_; // ��ġ�� �ռ�
    MatchedMaterialList matched_material_list_; // ��ġ�ϴ� ��� ���
}; // MatchedCompositionInfo
typedef util::internal::StaticArray<MatchedCompositionInfo, MatchedCompositionInfo::kMaxSize> MatchedCompositionList; // ��ġ�ϴ� �ռ� ���

// ���� ����
struct DecompositionInfo
{
    enum { kUnused = 0 }; // ��� ����
    typedef WORD DecompositionPoint; // ���� ����Ʈ
    struct ZardInfo // ��� ����
    {
        enum { kMaxSize = 4 }; // ��� �ִ� ����
        enum // ��� ���
        {
            kGradeLv1 = 1, // �ϱ�
            kGradeLv2 = 2, // �߱�
            kGradeLv3 = 3, // ���
            kGradeLv4 = 4 // �ֻ��
        };
        typedef int ZardGrade;
        typedef BYTE ZardNumber;

        ZardGrade grade_; // ��� ���
        ZardNumber number_; // ��� ����

        // �ʱ�ȭ
        explicit ZardInfo() : grade_(kUnused), number_(kUnused)
        {
        }
    }; // ZardInfo
    typedef util::internal::StaticArray<ZardInfo, ZardInfo::kMaxSize> ZardList; // ��� ���
    struct MaterialInfo // ��� ����
    {
        enum { kMaxSize = 9 }; // ��� �ִ� ����
        typedef BYTE Enchant;
        typedef BYTE SocketNumber;
        typedef LEVELTYPE ItemLevel;
        typedef DURATYPE MaterialNumber;

        Enchant enchant_; // ��þƮ
        SocketNumber socket_number_; // ���� ����
        ItemLevel item_level_; // ������ ����
        ZardList zard_list_; // ��� ���
        MaterialNumber number_; // ��� ����
        POSTYPE inventory_position_; // ����� �κ��丮 ��ġ

        // �ʱ�ȭ
        explicit MaterialInfo() : 
            enchant_(kUnused), 
            socket_number_(kUnused), 
            item_level_(kUnused), 
            number_(kUnused), 
            inventory_position_(kUnused)
        {
        }
    }; // MaterialInfo
    typedef util::internal::StaticArray<MaterialInfo, MaterialInfo::kMaxSize> MaterialList; // ��� ���
    struct RewardInfo // ���� ����
    {    
        enum { kMaxSize = 9 }; // ���� �ִ� ����
        enum // ���� ����
        {
            kItemCode = 1, // ������ �ڵ�
            kZard = 2 // ���
        };
        enum { kMaxRewardRatioSize = 100000 }; // �ִ� ���� Ȯ��
        typedef int RewardType;
        typedef CODETYPE RewardCode;
        typedef int RewardRatio;

        RewardType type_; // ���� ����
        RewardCode code_; // ���� �ڵ� (���� ������ ����϶��� ��� ������ ��� ����)
        RewardCode min_code_; // ������ ��ũ��Ʈ���� ����� �ּ� ������ �ڵ�
        RewardCode max_code_; // ������ ��ũ��Ʈ���� ����� �ִ� ������ �ڵ�
        RewardRatio ratio_; // ���� ����Ʈ 1�� ���� Ȯ��
        STLX_VECTOR<RewardCode> zard_list; // ������ ��ũ��Ʈ���� �ּ�~�ִ� �����ȿ� �ִ� ����� ������ ��� 

        // �ʱ�ȭ
        explicit RewardInfo() : 
            type_(kUnused), code_(kUnused), min_code_(kUnused), max_code_(kUnused), ratio_(kUnused)
        {
        }

        // ���� ���� ����� ������ �ڵ带 �����Ѵ�.
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
    typedef util::internal::StaticArray<RewardInfo, RewardInfo::kMaxSize> RewardList; // ���� ���
}; // DecompositionInfo

// ����� �κ��丮 ��ġ ���
typedef util::internal::StaticArray<POSTYPE, CompositionInfo::MaterialInfo::kMaxSize> MaterialInventoryPositionList;

#endif //}} PROGRAMCOMMON_CUBESYSTEM_CUBESTRUCT_H_