//--------------------------------------------------------------------------------------------------
//  ItemCooltimeManager.cpp
//  (C) 2010 Webzen Sunstudio
//--------------------------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "ItemCooltimeManager.h"
#include "SCItemSlot.h"
#include <SCSlotContainer.h>
#include "ItemManager.h"
#include "uiItemCompositeMan/uiItemCompositeMan.h"
//--------------------------------------------------------------------------------------------------
/** �����̳� ũ�� �� �ε�,��Ÿ�� ����
*/
ItemCooltimeManager::ItemCooltimeManager()
{
 
}
//--------------------------------------------------------------------------------------------------
/** 
*/
ItemCooltimeManager::~ItemCooltimeManager()
{
    Release();
}
//--------------------------------------------------------------------------------------------------
/** 
*/
void ItemCooltimeManager::Release()
{
    if (cooltime_container().empty())
    {
        return;
    }

    CoolTimeItr itr = cooltime_container().begin();

    while (itr != cooltime_container().end())
    {
        CooltimeStruct* cooltime_data = static_cast<CooltimeStruct*>(itr->second);
        cooltime_container().erase(itr++);
        SetItemLock(cooltime_data, false);
        DeleteItem(cooltime_data);
    }
}
//--------------------------------------------------------------------------------------------------
/**  ������ Lock.UnLock
*/
bool ItemCooltimeManager::SetItemLock(CooltimeStruct* data, bool lock)
{
    if (data == NULL)
    {
        return false;
    }
    
    // �����̳� invalid
    SCSlotContainer* slot_container = ItemManager::Instance()->GetContainer(data->slot_index);
    if (slot_container == NULL )
    {
        return false;
    }
    
    // �����۽��� invalid
    SCItemSlot& item_slot = (SCItemSlot&)slot_container->GetSlot(data->item_pos);
    if (item_slot.IsContainValidItem() == FALSE)
    {
        return false;
    }

    // �ø��� invalid
    if (item_slot.GetSerial() != data->serial_key)
    {
        return false;
    }
    
    item_slot.SetLock(lock);

    // TODO:��Ÿ�� case���� ���Կ� ���� �� ó��
    switch(data->type)
    {
    case kEtheriaCombine:
        {
            SCItemSlot& item_slot = 
                (SCItemSlot&)slot_container->GetSlot(data->EtheriaCombine.from_pos);
            if (item_slot.IsContainValidItem() == TRUE)
            {
                item_slot.SetLock(lock);
            }
        }
        break;
    default:
        {

        }
        break;
    }

    return true;
}
//--------------------------------------------------------------------------------------------------
/**  ��Ÿ�� �����ۻ���
*/
void ItemCooltimeManager::InsertItem(const CooltimeStruct* data)
{
    if (data == NULL)
    {
        return;
    }
    // �ߺ� üũ
    if (FindCooltime(data->serial_key))
    {
        return;
    }
    
    CooltimeStruct* new_data = new CooltimeStruct;  

    memcpy(new_data, data, sizeof(CooltimeStruct));
    
    cooltime_container().insert(CoolTimePair(data->serial_key, new_data));
    
    if (SetItemLock(new_data, true) == false)
    {
        assert(!"Lock Fail");
    }

 }
//--------------------------------------------------------------------------------------------------
/** ��Ÿ�� ����������
*/
void ItemCooltimeManager::DeleteItem(CooltimeStruct* cooltime_data)
{
   SAFE_DELETE(cooltime_data)
}
//--------------------------------------------------------------------------------------------------
/**  �����̳ʿ� ������ ��������(get_time : ����ð����)
*/
bool ItemCooltimeManager::FindCooltime(DBSERIAL key, float* progress_time)
{
    if (cooltime_container().empty())
    {
        return false;
    }

    const CoolTimeItr itr = cooltime_container().find(key);
    
    if (itr == cooltime_container().end()) 
    {
        return false;
    }
    
    if (progress_time == NULL)
    {
        return true;    
    }

    const CooltimeStruct* cooltime_data = static_cast< CooltimeStruct* >(itr->second);
    
    *progress_time =  (cooltime_data) ? (cooltime_data->cooltime / cooltime_data->max_time) : (0.f);

    (*progress_time) *= 100.f;
    
    return true;    
}
//--------------------------------------------------------------------------------------------------
/** ��Ÿ�� �ð�üũ ���μ���
*/
void ItemCooltimeManager::Process(DWORD tick)
{
    if (cooltime_container().empty())
    {
        return;
    }

    CoolTimeItr itr = cooltime_container().begin();

    while (itr != cooltime_container().end())
    {
        CooltimeStruct* cooltime_data = static_cast< CooltimeStruct* >(itr->second);

        // safe_code: NULL�̸� �ȵ�!
        if (cooltime_data == NULL)
        {
            assert(!"cooltime_data is null");
            cooltime_container().erase(itr++);
            continue;
        }

        // �ð�üũ
        if ((cooltime_data->cooltime += tick) < cooltime_data->max_time)
        {
            ++itr;
            continue;
        }

        cooltime_data->cooltime = cooltime_data->max_time;

        // ����ó��
        if (cooltime_data->execute == false)
        {
            Execute(cooltime_data);
            cooltime_data->execute = true;
            ++itr;
            continue;
        }
        
        // ������Ÿ��
        if ((cooltime_data->dealy_time -= tick) > 0)
        {
            ++itr;
            continue;
        }

        DeleteItem(cooltime_data);
        cooltime_container().erase(itr++);
    }
}

//--------------------------------------------------------------------------------------------------
/** ��Ÿ�� ������ �����۾�
*/
void ItemCooltimeManager::Execute(CooltimeStruct* cooltime_data)
{
    if (cooltime_data == NULL)
    {
        return;
    }
    
    // ������ ������
    if (SetItemLock(cooltime_data, false) == false)
    {
        assert(!"UnLock Fail");
    }

    // TODO:��Ÿ�� ����� ó���ؾߵ� ��Ȳ�� �ٸ���� case�� �߰��ؼ� ������
    switch(cooltime_data->type)
    {
    case kEtheriaCombine:
        {
            ItemManager::Instance()->SendEtheriaCombinationPacket(
                                                    cooltime_data->EtheriaCombine.to_pos,
                                                    cooltime_data->EtheriaCombine.from_pos);


        }
        break;
    case kCrystalization:
        {            
            uiItemCompositeMan* ui_item_composite_man = GET_DIALOG_MANAGER(uiItemCompositeMan, UIMAN_ITEM_COMPOSITE);
            if (ui_item_composite_man)
            {
                ui_item_composite_man->NET_SEND_CG_ITEM_CRYSTALIZE_SYN(
                                                     cooltime_data->Crystalization.target_pos, 
                                                     cooltime_data->Crystalization.slot_code);
            }
        }
        break;
    default:
        {
            
        }
        break;
    }
}

bool ItemCooltimeManager::CancelCooltime( DBSERIAL key )
{
    if (cooltime_container().empty())
    {
        return false;
    }

    CoolTimeItr itr = cooltime_container().find(key);

    if (itr == cooltime_container().end()) 
    {
        return false;
    }

    CooltimeStruct* cooltime_data = static_cast< CooltimeStruct* >(itr->second);
    cooltime_container().erase(itr++);
    SetItemLock(cooltime_data, false);
    DeleteItem(cooltime_data);

    return true;
}











