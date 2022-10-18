#pragma once


class SCItemSlotContainer;
class ItemUnitRenderer;

class ItemSlotContainer
{
private:
    eSlotContainerIndex m_SlotContainerIndex;

    SCItemSlotContainer* m_pSlotContainer;
    ItemUnitRenderer* m_pItemUnitRenderer;


public:
    ItemSlotContainer();
    
    eSlotContainerIndex GetSlotContainerIndex() const { return m_SlotContainerIndex; }

    SCItemSlotContainer* GetSlotContainer() { return m_pSlotContainer; }

    void Clear();
    void Update();
    void Render();

    void SetSlotRenderState_DeActive( int slot_index );
    void SetSlotRenderState_Active( int slot_index );

    void Initialize( int slot_count, eSlotContainerIndex slot_container_index );
    void Release();


    void SetSlotItem( int slot_index, SCItemSlot& item, CControlWZ* slot_control );
    void RemoveSlot( int slot_index );
    SCItemSlot* GetSlotItem( int slot_index );

private:

};
