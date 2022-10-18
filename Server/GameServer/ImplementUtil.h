#pragma once
#ifndef SERVER_GAMESERVER_IMPLEMENTUTIL_H_
#define SERVER_GAMESERVER_IMPLEMENTUTIL_H_

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ImplementUtil.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2010/10/13 by hukim] This is created.\n
/// @history
///     - ~10.12.20
///     - 10.12.20, waverix, CHANGES: f101220.1L, fixed invalid function declaration
///                          about c-style static function, reference='f101217.1L'
///     - 11.02.16, waverix, CHANGES: f110216.2L, same reason as 'f101220.1L'
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <boost/scoped_ptr.hpp>
//
#include "ItemManager.h"
#include "SCItemSlotContainer.h"
#include "RewardManager.h"

namespace util {
namespace internal {
;

// CHANGES: f110216.2L, fixed unexpected code duplicated state in address space
// ��û �޽����� ���� �⺻���� �˻縦 �Ѵ�.
template <typename REQUEST_MESSAGE_T>
bool CheckRequestMessage(
    const MSG_BASE* const base_message, 
    const WORD message_size, 
    const REQUEST_MESSAGE_T*& OUT message, 
    Player*& OUT requester)
{
    // ��Ŷ ũ�⸦ �˻��Ѵ�.
    message = static_cast<const REQUEST_MESSAGE_T*>(base_message);
    if (message->GetSize() != message_size) 
    {
        assert(false);
        return false;
    }
    
    // ��û�ڸ� �˻��Ѵ�.
    requester = util::internal::IsExistInField(message->m_dwKey);
    if (requester == NULL) {
        return false;
    }
    return true;
}

// pseudo: true = { exist player & exist game_field }
extern Player* IsExistInField(DWORD user_key);

// ��û�� ���¿� ���� �⺻���� �˻縦 �Ѵ�.
extern bool CheckRequesterState(
    Player* const requester, const TCHAR* called_function, int& OUT result_code);

// ������ �ռ� �Ǵ� ���� ����
class ItemCompositionOrDecompositionReward
{
public:
    enum { kUnused = 0 };
    enum { kDoNotUseLog = 0 };
    enum RewardType
    {
        kUndefinedReward = kUnused, 
        kItemCodeReward = 1,
        kRewardCodeReward = 2 
    };
    typedef int RequestType;
    typedef SLOTCODE RewardItemCode;
    typedef POSTYPE RewardItemNumber;
    typedef CODETYPE RewardCode;
    typedef WORD SOCKETINDEX;
    struct ItemReward // ������ ����
    {
        RewardItemCode item_code;
        RewardItemNumber number_of_item;
        KindOfItemType::value_type ig_type;
        SOCKETINDEX zard_option;

        ItemReward() : 
            item_code(kUnused), 
            number_of_item(0), 
            ig_type(KindOfItemType::Default), 
            zard_option(kUnused)
        {
        }
    }; // ItemReward
private:
    class ItemCodeReward;
    class RewardCodeReward;
    class RewardInterface // ���� �������̽�
    {
    protected:
        RewardInterface() : requester_(NULL)
        {
        }
    public:
        virtual ~RewardInterface() {}
        // �κ��丮�� ������ ��´�.
        virtual RC::eITEM_RESULT ObtainToInventory(
            const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items) = 0;
        // �ռ� �Ǵ� ���� ������ ���� ������ �߰��Ѵ�.
        virtual bool AddToHistory(ItemCompositionOrDecompositionHistory& OUT history) const = 0;
        // �κ��丮�� ���� ������ �ִ��� �˻��Ѵ�.
        virtual bool IsExistObtainedToInventory() const = 0;
        // �ʱ�ȭ �Ѵ�.
        void Init(Player* const requester)
        {
            requester_ = requester;
        }
        // ��ü �ν��Ͻ��� ��´�.
        template <typename CONCRETE_T> 
        CONCRETE_T* GetConcrete() 
        {
            return static_cast<CONCRETE_T*>(this);
        }
        // �ν��Ͻ��� �����Ѵ�.
        static RewardInterface* CreateInstance(const RewardType reward_type);
    protected:
        Player* requester_;
    }; // RewardInterface
    //
    class ItemCodeReward : public RewardInterface // ������ �ڵ� ����
    {
        friend class RewardInterface;
        enum { kMaxKindOfReward = 20 };
    private:
        explicit ItemCodeReward();
    public:
        bool Add(const ItemReward& item_reward);
        RC::eITEM_RESULT ObtainToInventory(
            const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items);
        bool AddToHistory(ItemCompositionOrDecompositionHistory& OUT history) const;
        bool IsExistObtainedToInventory() const
        {
            return (number_of_kinds_ > 0);
        }
    private:
        POSTYPE number_of_kinds_;
        RewardItemCode item_code_list_[kMaxKindOfReward];
        RewardItemNumber item_num_list_[kMaxKindOfReward];
        KindOfItemType::value_type ig_type_list_[kMaxKindOfReward];
        SOCKETINDEX zard_option_list_[kMaxKindOfReward];
        bool is_use_ig_type_;
        bool is_use_zard_option_;
        __DISABLE_COPY(ItemCodeReward);
    }; // ItemCodeReward
    class RewardCodeReward : public RewardInterface // ���� �ڵ� ���� ������
    {
        friend class RewardInterface;
    private:
        RewardCodeReward() : reward_code_(kUnused)
        {
        }
    public:
        bool Add(const RewardCode reward_code);
        RC::eITEM_RESULT ObtainToInventory(
            const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items);
        bool AddToHistory(ItemCompositionOrDecompositionHistory& OUT history) const;
        bool IsExistObtainedToInventory() const
        {
            return (obtained_result_.number_of_kinds > 0);
        }
    private:
        RewardCode reward_code_;
        RewardManager::ObtainedResult obtained_result_;
        __DISABLE_COPY(RewardCodeReward);
    }; // RewardCodeReward
public:
    ItemCompositionOrDecompositionReward() : reward_type_(kUndefinedReward)
    {
    }
    // �ʱ�ȭ �Ѵ�.
    bool Init(Player* const requester, const RewardType reward_type);
    // ������ �ڵ� ������ �߰��Ѵ�.
    bool AddItemReward(const ItemReward& item_reward);
    // ���� �ڵ� ������ �߰��Ѵ�.
    bool AddReward(const RewardCode reward_code);
    // �κ��丮�� ������ ��´�.
    RC::eITEM_RESULT ObtainRewardToInventory(
        const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items);
    // �ռ� �Ǵ� ���� ������ ���� ������ �߰��Ѵ�.
    bool AddToHistory(ItemCompositionOrDecompositionHistory& OUT history) const;
    // �κ��丮�� ���� ������ �ִ��� �˻��Ѵ�.
    bool IsExistObtainedToInventory() const;
private:
    RewardType reward_type_;
    boost::scoped_ptr<RewardInterface> reward_;
}; // ItemCompositionOrDecompositionReward
} // internal
} // util
#endif //}} SERVER_GAMESERVER_IMPLEMENTUTIL_H_