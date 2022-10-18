//------------------------------------------------------------------------------
//  DelayLoadManager.cpp
//  (C) 2010 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "DelayLoadManager.h"
#include "Player.h"
#include "ObjectManager.h"
//------------------------------------------------------------------------------
/** 컨테이너 크기 및 로드,콜타입 설정
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
    이큅정보를 받았을때만 처리한다 현재 섹터정보를 받을때 패킷이
    나뉘어서 오기때문에 Equip정보를 받았는지 체크해야된다!
    1번 패킷---> ALLPLAYERS_CMD 
    2번 패킷---> ALLPLAYERS_EQUIPINFO_CMD
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

        // 로딩시간체크
        if ((clock_function::GetTickCount() - check_tick) > delay_time_[kLoadDelay])
        {
            break;
        }

        Player* player_ptr =static_cast<Player*>(g_ObjectManager.GetObject(*current_itr));

        if (player_ptr)
        {
            // 이큅장비가 셋팅되지않았을경우(장비및 펫.라이더로드안함)
            if (player_ptr->receive_equip_info() == false)
            {
                continue;
            }

            // STEP1:주인공장비로드
            if (player_ptr->IsUseDelayLoading() == true)
            {
                // Player 파츠로드
                player_ptr->SetUseDelayLoading(false);
                player_ptr->RefreshAppearRender();

                // 로딩시간체크
                if ((clock_function::GetTickCount() - check_tick) > delay_time_[kLoadDelay])
                {
                    break;
                }
            }
            
            // STEP2:라이더.펫로드
            player_ptr->RefreshAppearRider();
            player_ptr->RefreshAppearPet();    
        }
        
        // STEP3:리스트에서 지운다
        load_player_list_.erase(current_itr);
    }

    process_check_time = clock_function::GetTickCount();
}
//------------------------------------------------------------------------------
/** 지연로딩 리스트에 플레이어 키값을 삽입한다(이미삽입된것은 삽입안함)
*/
void DelayLoadManager::InsertPlayer(DWORD player_key)
{
    //이미 삽입되어있는지 체크한다
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