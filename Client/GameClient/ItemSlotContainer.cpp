#include "SunClientPrecompiledHeader.h"
#include "ItemSlotContainer.h"

#include "SCItemSlotContainer.h"
#include "ItemUnitRenderer.h"
#include "SlotKeyGenerator.h"
#include "GlobalData.h"
#include "SCItemSlot.h"


ItemSlotContainer::ItemSlotContainer()
: m_pSlotContainer(NULL)
, m_pItemUnitRenderer(NULL)
, m_SlotContainerIndex(SI_MAX_TOTAL)
{
}

void ItemSlotContainer::Initialize( int slot_count, eSlotContainerIndex slot_container_index )
{
    m_SlotContainerIndex = slot_container_index;

    m_pSlotContainer = new SCItemSlotContainer();
    m_pSlotContainer->Init( slot_count, slot_container_index );


    m_pItemUnitRenderer = new ItemUnitRenderer();
    m_pItemUnitRenderer->Init( g_pSunRenderer, slot_count );
}

void ItemSlotContainer::Release()
{
    // 등록되어있는 SlotItem, Serial을 모두 제거해주어야 하지만 일단, 패스
    m_pItemUnitRenderer->Release();
    m_pSlotContainer->Release();

    delete m_pSlotContainer;
    delete m_pItemUnitRenderer;
}

void ItemSlotContainer::Update()
{
    m_pItemUnitRenderer->Update();
}

void ItemSlotContainer::Render()
{
    m_pItemUnitRenderer->Render();
}

void ItemSlotContainer::Clear()
{
    for( int i=0;i!=m_pSlotContainer->GetMaxSlotNum();++i)
    {
        RemoveSlot(i);
    }
}

void ItemSlotContainer::SetSlotRenderState_DeActive( int slot_index )
{
    if( SCItemSlot* item_slot = GetSlotItem( slot_index ) )
    {
        if( ItemUnitRenderer::RenderUnit* render_unit = m_pItemUnitRenderer->GetItemUnit(item_slot->GetSerial()) )
        {
            render_unit->SetState( SLOT_RENDER_STATE_DEACTIVATED );
        }
    }
}

void ItemSlotContainer::SetSlotRenderState_Active( int slot_index )
{
    if( SCItemSlot* item_slot = GetSlotItem( slot_index ) )
    {
        if( ItemUnitRenderer::RenderUnit* render_unit = m_pItemUnitRenderer->GetItemUnit(item_slot->GetSerial()) )
        {
            render_unit->SetState( SLOT_RENDER_STATE_ACTIVATED );
        }
    }
}

void ItemSlotContainer::SetSlotItem( int slot_index, SCItemSlot& item, CControlWZ* slot_control )
{
    RemoveSlot( slot_index );

    SCSlot* item_slot_ptr = (SCSlot*)&item;

    RECT rect = slot_control->GetSizeRect();

    if( m_pSlotContainer->InsertSlot( slot_index, (*item_slot_ptr)) == RC::RC_INSERT_SLOT_SUCCESS )
    {
        SCSlot& new_slot = m_pSlotContainer->GetSlot(slot_index);
        new_slot.SetSerial( g_SlotKeyGenerator.GetKey() );
        m_pItemUnitRenderer->AddItemUnit( new_slot.GetSerial(), item_slot_ptr->GetCode(), &rect, &new_slot, slot_control );
    }
}

void ItemSlotContainer::RemoveSlot( int slot_index )
{
    if( SCItemSlot* item_slot = GetSlotItem( slot_index ) )
    {
        m_pItemUnitRenderer->RemoveItemUnit( item_slot->GetSerial() );
        g_SlotKeyGenerator.Restore( (DWORD)item_slot->GetSerial() );

        m_pSlotContainer->DeleteSlot( slot_index, NULL );
    }
}

SCItemSlot* ItemSlotContainer::GetSlotItem( int slot_index )
{
    if( m_pSlotContainer->IsEmpty(slot_index) )
    {
        return NULL;
    }

    SCSlot& slot = m_pSlotContainer->GetSlot(slot_index);
    return (SCItemSlot*)&slot;
}
