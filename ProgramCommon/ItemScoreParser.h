////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ItemScoreParser.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/07/04 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef PROGRAMCOMMON_ITEMSCOREPARSER_H_
#define PROGRAMCOMMON_ITEMSCOREPARSER_H_

#ifdef _NA_002935_20110704_ITEM_SCORE

#include "ScriptCode.IParser.h"
#include "Singleton.h"
#include "SCSlotStruct.h"

class SCItemSlot;
class PlayerAttributes;
struct IEquipmentRestriction;

// 장비 평가 점수 스크립트 파서
class ItemScoreParser : public util::Singleton<ItemScoreParser>, public IParser
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    ItemScoreParser();
    ~ItemScoreParser();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    virtual void Release() {}
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:
    typedef float Weight;
    
    // 개별 아이템의 평가 점수를 계산한다.
    Weight CalculateSingleScore(const SCItemSlot& item_slot, const bool is_pc_room) const;
    
    // 착용한 장비 전체로 판단해야할 평가 점수를 계산한다.
    void CalculateTotalEquipScore(
        const PlayerAttributes& player_attrs, 
        Weight& OUT socket_score, 
        Weight& OUT set_option_score) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:    
    // 아이템 레벨 가중치
    struct ItemLevelWeight
    {
        typedef POSTYPE EquipPosition;
        typedef STLX_HASH_MAP<EquipPosition, Weight> EquipPositionMap;
        
        LEVELTYPE item_level;
        EquipPositionMap equip_position_map;

        explicit ItemLevelWeight(const LEVELTYPE _item_level) : item_level(_item_level)
        {
        }
    private:
        __DISABLE_COPY(ItemLevelWeight);
    };
    typedef STLX_HASH_MAP<LEVELTYPE, const ItemLevelWeight*> ItemLevelWeightMap;
    
    // 인챈트 가중치
    struct EnchantWeight
    {
        typedef BYTE EnchantGrade;
        enum ItemType
        {
            kWeapon = 0, // 무기
            kArmor, // 방어구
            kMaxSize
        };
        
        EnchantGrade enchant_grade;
        Weight item_type_list[kMaxSize];
        
        explicit EnchantWeight(
            const EnchantGrade _enchant_grade, const Weight _item_type_list[kMaxSize]) : 
            enchant_grade(_enchant_grade)
        {
            BOOST_STATIC_ASSERT(sizeof(item_type_list[0]) == sizeof(_item_type_list[0]));
            ::CopyMemory(item_type_list, _item_type_list, sizeof(item_type_list));
        }
    private:
        __DISABLE_COPY(EnchantWeight);
    };
    typedef STLX_HASH_MAP<EnchantWeight::EnchantGrade, const EnchantWeight*> EnchantWeightMap;
    
    // 엑스트라 스톤 가중치
    struct ExtraStoneWeight
    {
        typedef int ExtraStoneSize;
        enum ItemType
        {
            kWeapon = 0, // 무기
            kArmor, // 방어구
            kMaxSize
        };

        ExtraStoneSize extra_stone_size;
        Weight item_type_list[kMaxSize];
        
        explicit ExtraStoneWeight(
            const ExtraStoneSize _extra_stone_size, const Weight _item_type_list[kMaxSize]) : 
            extra_stone_size(_extra_stone_size)
        {
            BOOST_STATIC_ASSERT(sizeof(item_type_list[0]) == sizeof(_item_type_list[0]));
            ::CopyMemory(item_type_list, _item_type_list, sizeof(item_type_list));
        }
    private:
        __DISABLE_COPY(ExtraStoneWeight);
    };
    typedef STLX_HASH_MAP<ExtraStoneWeight::ExtraStoneSize, const ExtraStoneWeight*> ExtraStoneWeightMap;
    
    // 쟈드 옵션 포인트 가중치
    typedef int ZardOptionPoint;
    typedef STLX_MAP<ZardOptionPoint, Weight> ZardOptionPointWeightMap;
    
    // 옵션 가중치
    struct OptionWeight
    {
        enum ValueType
        {
            kFix = 0, // 고정형
            kRatio, // 비율형
            kMaxSize
        };

        eATTR_TYPE attr_option_code;
        bool is_reverse_benefit;
        Weight value_type_list[kMaxSize];
        
        explicit OptionWeight(
            const eATTR_TYPE _attr_option_code, 
            const bool _is_reverse_benefit, 
            const Weight _value_type_list[kMaxSize]) : 
            is_reverse_benefit(_is_reverse_benefit), 
            attr_option_code(_attr_option_code)
        {
            BOOST_STATIC_ASSERT(sizeof(value_type_list[0]) == sizeof(_value_type_list[0]));
            ::CopyMemory(value_type_list, _value_type_list, sizeof(value_type_list));
        }
    private:
        __DISABLE_COPY(OptionWeight);
    };
    typedef STLX_HASH_MAP<eATTR_TYPE, const OptionWeight*> OptionWeightMap;

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    // 각성 가중치
    class AwakeningWeight
    {
    public:
        enum { kAwakeningMaxWeight = 9999 };
        enum ItemType
        {
            kWeapon = 0, // 무기
            kArmor  = 1, // 방어구
            kMaxSize,
        };

        AwakeningWeight()
        {
            memset(value_type_list, 0, sizeof(Weight) * kMaxSize);
        }

        bool SetWeight(Weight weight, ItemType type)
        {
            if (type < 0 || kMaxSize <= type || weight < 0 || weight > kAwakeningMaxWeight)
            {
                return false;
            }
            value_type_list[type] = weight;
            return true;
        }

        Weight GetWeight(ItemType type) const
        {
            if (type < 0 || kMaxSize <= type)
            {
                return 0;
            }
            return value_type_list[type];
        }
    private:
        Weight value_type_list[kMaxSize];
    };
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

    // 가중치 관리자
    class WeightManager
    {
    public:
        WeightManager();
        ~WeightManager();
        
        // 초기화한다.
        void Clear();
        
        // 아이템 레벨 가중치를 삽입한다. (이미 같은 가중치가 있으면 삭제후 삽입)
        void InsertItemLevelWeight(const ItemLevelWeight* weight, const bool is_divine);
        
        // 아이템 레벨 가중치를 찾는다.
        Weight FindItemLevelWeight(
            const LEVELTYPE item_level, 
            const ItemLevelWeight::EquipPosition equip_position, 
            const bool is_divine) const;
        
        // 인챈트 가중치를 삽입한다. (이미 같은 가중치가 있으면 삭제후 삽입)
        void InsertEnchantWeight(const EnchantWeight* weight);
        
        // 인챈트 가중치를 찾는다.
        Weight FindEnchantWeight(
            const EnchantWeight::EnchantGrade enchant_grade, 
            const EnchantWeight::ItemType item_type) const;
        
        // 엑스트라 스톤 가중치를 삽입한다. (이미 같은 가중치가 있으면 삭제후 삽입)
        void InsertExtraStoneWeight(const ExtraStoneWeight* weight);
        
        // 엑스트라 스톤 가중치를 찾는다.
        Weight FindExtraStoneWeight(
            const ExtraStoneWeight::ExtraStoneSize extra_stone_size, 
            const ExtraStoneWeight::ItemType item_type) const;
        
        // 쟈드 옵션 포인트 가중치를 삽입한다. (이미 같은 가중치가 있으면 삭제후 삽입)
        void InsertZardOptionPointWeight(
            const ZardOptionPoint zard_option_point, const Weight weight);
        
        // 쟈드 옵션 포인트 가중치를 찾는다.
        Weight FindZardOptionPointWeight(const ZardOptionPoint zard_option_point) const;
        
        // 옵션 가중치를 삽입한다. (이미 같은 가중치가 있으면 삭제후 삽입)
        void InsertOptionWeight(const OptionWeight* weight);
        
        // 옵션 가중치를 찾는다.
        Weight FindOptionWeight(
            const eATTR_TYPE attr_option_code, 
            const OptionWeight::ValueType value_type) const;

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        bool SetAwakeningWeight(Weight weight, AwakeningWeight::ItemType type)
        {
            return awakening_weight_.SetWeight(weight, type);
        }
        Weight GetAwakeningWeight(AwakeningWeight::ItemType type) const
        {
            return awakening_weight_.GetWeight(type);
        }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

    private:
        ItemLevelWeightMap item_level_weight_map_;
        ItemLevelWeightMap divine_item_level_weight_map_;
        EnchantWeightMap enchant_weight_map_;
        ExtraStoneWeightMap extra_stone_weight_map_;
        ZardOptionPointWeightMap zard_option_point_weight_map_;
        OptionWeightMap option_weight_map_;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
        AwakeningWeight awakening_weight_;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
        __DISABLE_COPY(WeightManager);
    };
    
private:
    // 가중치 스크립트를 읽는다.
    bool LoadWeight(const bool is_reload);
    
    // 옵션 가중치를 계산한다.
    Weight CalculateOptionWeight(
        const BASE_ITEMINFO::OptionIndexes& option_indexes, 
        const BASE_ITEMINFO::OptionTypes& option_types, 
        const BASE_ITEMINFO::OptionValues& option_values) const;

private:
    WeightManager weight_manager_;
    __DISABLE_COPY(ItemScoreParser);

}; // ItemScoreParser

#endif //_NA_002935_20110704_ITEM_SCORE
#endif // PROGRAMCOMMON_ITEMSCOREPARSER_H_