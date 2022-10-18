#ifndef GAMESERVER_ITEMMANAGER_MOVE_H
#define GAMESERVER_ITEMMANAGER_MOVE_H

class SCItemSlotContainer;
class ItemManager;

//==================================================================================================
namespace nsSlot {
;
//==================================================================================================
// NOTE: f110418.1L, Sub-Manager of the ItemManager about item durability
class ItemMoveImpl
{
public:
    ItemMoveImpl();
    // called by ItemManager::Init
    ItemMoveImpl(ItemManager* item_manager);
    // Slot��(Slot��) �̵�
    RC::eITEM_RESULT Move(const SLOTIDX from_slot_index, const POSTYPE from_pos, 
                          const SLOTIDX to_slot_index, const POSTYPE to_pos, 
                          const DURATYPE move_quantity);
    //
private:
    enum eSwapContinualResult { eSwap_Stop, eSwap_Process };
    //-----------------------------------------------------------------------------------------------
    struct CacheArg
    {
        // �̱���ü�� Ư�� �Ѱ��� ������ ��ǥ�Ѵ�
        // slot_index/container , pos_in_slots�� ���� �⺻�����̴�.
        // �׷��Ƿ� �� ������ ���� �� ���µ��� �ʵ��� �Ѵ�.
        SLOTIDX slot_index;
        POSTYPE pos_in_slots;
        bool slot_exist;
        SCItemSlotContainer* container;
        const SCItemSlot* item_slot;
        const nsSlot::ItemRuleInfo* item_rule_info;
        const BASE_ITEMINFO* item_info;

#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        VOID SetData(ItemManager* item_mgr, SLOTIDX index, POSTYPE slot_pos);
        BOOL DeleteItem(SCSlot* slot_clone_before_delete);
        RC::eSLOT_INSERT_RESULT InsertItem(SCSlot& slot);
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    };
    //-----------------------------------------------------------------------------------------------
    //
    bool CheckMovingSlotIndexRule(RC::eITEM_RESULT* item_result,
        const SLOTIDX from_slot_index, const SLOTIDX to_slot_index) const;
    // Ư�� ���ǿ� ���� �ش� ���� �� ��ġ�� �ش� �������� ���� �� ���� ���� �˻�
    // ItemContainer �Ǵ� Slot����� ������ ���� ������ �ƴ϶�,
    // Zone�� ���� ���� ���ǵ��� �Ǵ��ϱ� ���� �Լ�, Move���� ���
    bool CheckInsertableCondition(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to) const;
    // CHANGES: f110701.2L, added the rule is to prevent irregula stat calcuation
    // like the dragon transformed state.
    bool CheckDeletableCondition(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to) const;
    // NOTE: f110701.2L
    bool CheckMovingAcceptableState(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to) const;
    //
    bool CheckMovingFilterRule(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to) const;
    // NOTE: f110418.2L, added rule to prevent swapping fails
    // when an equipment switch to other an equipment
    eSwapContinualResult CheckEnableSwapWhenSwapEquipments(
        RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to);
    bool MoveDefault(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to);
    bool MoveDivision(RC::eITEM_RESULT* item_result,
        CacheArg* const from, CacheArg* const to,
        const DURATYPE move_quantity);
    //
    // CHANGES: f110418.1L, moved logging parts from 'CG_ITEM_MOVE_SYN' handler.
    bool LogOnSuccess(CacheArg* const from, CacheArg* const to,
                      const DURATYPE move_quantity);
    //-----------------------------------------------
    // fields...
    ItemManager* const root_;
    //
    friend class ItemManager;
    //
    __DISABLE_COPY(ItemMoveImpl);
};

//==================================================================================================
inline ItemMoveImpl::ItemMoveImpl()
    : root_(NULL)
{
}

inline ItemMoveImpl::ItemMoveImpl(ItemManager* item_manager)
    : root_(item_manager)
{
}

//==================================================================================================
}; //end of namespace
//==================================================================================================

#endif //GAMESERVER_ITEMMANAGER_MOVE_H