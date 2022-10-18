//------------------------------------------------------------------------------
//  DelayLoadManager.cpp
//  (C) 2010 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "DelayLoadManager.h"
#include "Player.h"
#include "ObjectManager.h"
//------------------------------------------------------------------------------
/** �����̳� ũ�� �� �ε�,��Ÿ�� ����
*/
DelayLoadManager::DelayLoadManager()
{
    delay_time_[kProcessDelay] = 200;
    delay_time_[kLoadDelay] = 30;
}

//------------------------------------------------------------------------------
/** 
*/
DelayLoadManager::~DelayLoadManager()
{
    clear();
}

//------------------------------------------------------------------------------
/** 
    ��Ţ������ �޾������� ó���Ѵ� ���� ���������� ������ ��Ŷ��
    ����� ���⶧���� Equip������ �޾Ҵ��� üũ�ؾߵȴ�!
    1�� ��Ŷ---> ALLPLAYERS_CMD 
    2�� ��Ŷ---> ALLPLAYERS_EQUIPINFO_CMD
*/
void DelayLoadManager::Process()
{
    static DWORD process_check_time = 0;

    if ((clock_function::GetTickCount() - process_check_time) < delay_time_[kProcessDelay])
    {
        return;
    }
  
    DWORD check_tick = clock_function::GetTickCount();

    LOAD_PALYER_LIST_ITR list_itr = load_player_list_.begin();
    LOAD_PALYER_LIST_ITR current_itr;
    
    while ( list_itr != load_player_list_.end())
    {
        current_itr = list_itr;
        ++list_itr;

        // �ε��ð�üũ
        if ((clock_function::GetTickCount() - check_tick) > delay_time_[kLoadDelay])
        {
            break;
        }

        Player* player_ptr =static_cast<Player*>(g_ObjectManager.GetObject(*current_itr));

        if (player_ptr)
        {
            // ��Ţ��� ���õ����ʾ������(���� ��.���̴��ε����)
            if (player_ptr->receive_equip_info() == false)
            {
                continue;
            }

            // STEP1:���ΰ����ε�
            if (player_ptr->IsUseDelayLoading() == true)
            {
                // Player �����ε�
                player_ptr->SetUseDelayLoading(false);
                player_ptr->RefreshAppearRender();

                // �ε��ð�üũ
                if ((clock_function::GetTickCount() - check_tick) > delay_time_[kLoadDelay])
                {
                    break;
                }
            }
            
            // STEP2:���̴�.��ε�
            player_ptr->RefreshAppearRider();
            player_ptr->RefreshAppearPet();    
        }
        
        // STEP3:����Ʈ���� �����
        load_player_list_.erase(current_itr);
    }

    process_check_time = clock_function::GetTickCount();
}
//------------------------------------------------------------------------------
/** �����ε� ����Ʈ�� �÷��̾� Ű���� �����Ѵ�(�̹̻��ԵȰ��� ���Ծ���)
*/
void DelayLoadManager::InsertPlayer(DWORD player_key)
{
    //�̹� ���ԵǾ��ִ��� üũ�Ѵ�
    Object* object_ptr = g_ObjectManager.GetObject(player_key);

    if (object_ptr == NULL) 
    {
        return;
    }

    if (object_ptr->IsUseDelayLoading()) 
    {
        return;
    }

    object_ptr->SetUseDelayLoading(true);
    load_player_list_.push_back(player_key);
}