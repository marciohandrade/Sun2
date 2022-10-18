#include "SunClientPrecompiledHeader.h"


#include "DiceCastManager.h"

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
#include "ItemManager.h"
#include "globalfunc.h"
#include "SoundEffect.h"
#include "Hero.h"

int DiceCastManager::GetDiceNumberToValue(WORD number)
{
    if (number <= DICE_NUMBER_ONE) {return 1;}
    else if (number <= DICE_NUMBER_TWO) {return 2;}
    else if (number <= DICE_NUMBER_THREE) {return 3;}
    else if (number <= DICE_NUMBER_FOUR) {return 4;}
    else if (number <= DICE_NUMBER_FIVE) {return 5;}
    else if (number <= DICE_NUMBER_SIX) {return 6;}
    else {assert(!"dice value �� �ִ밪�� �ʰ��Ͽ����ϴ�.");}
    return 0;
}

BYTE DiceCastManager::GetDiceNumber(DWORD user_key)
{
    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (dice_draw_info_[i].member_key == 0)
        {
            return 0;
        }
        if (dice_draw_info_[i].member_key == user_key)
        {
            return GetDiceNumberToValue(dice_draw_info_[i].random_num);
        }
    }
    return 0;
}

void DiceCastManager::SetDiceDrawInfo(_DICECAST_RESULT* dice_draw_info, int player_count)
{
    is_start_animation_ = true;
    draw_start_animation_tick_ = GetTickCount();

    //�������� ���� ����
    g_SoundEffect.PlayUI(UI_SOUND_DICE_ROLL);

    ZeroMemory(dice_draw_info_, sizeof(dice_draw_info_));
    for (int i=0; i < player_count; ++i)
    {
        if (dice_draw_info[i].member_key == 0)
        {
            return;
        }
        dice_draw_info_[i] = dice_draw_info[i];
    }
}

DWORD DiceCastManager::GetAnimationTick()
{
    if (is_start_animation_ == true)
    {
        DWORD process_tick = GetTickCount() - draw_start_animation_tick_;
        if (process_tick > DICE_DRAWING_TIME)
        {
            is_start_animation_ = false;
        }
        else if (process_tick > DICE_ANIMATION_TIME)
        {
            //���� �޼��� ���
            if (strlen(output_message_) != 0)
            {
                GlobalFunc::default_OutputMsg(eOUTPUT_MSG_TOP_SHOW, output_message_, 5000);
                ZeroMemory(output_message_, sizeof(output_message_));

                //�������� �� ����
                if (g_pHero && (g_pHero->GetObjectKey() == high_number_player_key_))
                {
                    g_SoundEffect.PlayUI(UI_SOUND_DICE_STOP_WIN);
                }
                else
                {
                    g_SoundEffect.PlayUI(UI_SOUND_DICE_STOP);
                }
                
             }
        }
        return process_tick;
    }
    return 0;
}

bool DiceCastManager::IsDiceDraw()
{
    if (is_start_animation_ == true)
    {
        return true;
    }

    return false;
}

bool DiceCastManager::IsDiceDraw(DWORD user_key)
{
    if (is_start_animation_ == false)
    {
        return false;
    }

    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (dice_draw_info_[i].member_key == 0)
        {
            break;
        }
        if (dice_draw_info_[i].member_key == user_key)
        {
            return true;
        }
    }

    return false;
}

void DiceCastManager::SetDiceDrawOff()
{
    is_start_animation_ = false;
}

void DiceCastManager::SetCenterMessage(TCHAR *center_message)
{
    //������ ����� �޼����� �־����� �Ѹ���  ����
    if (strlen(output_message_) != 0)
    {
        GlobalFunc::default_OutputMsg(eOUTPUT_MSG_TOP_SHOW, output_message_, 5000);
        ZeroMemory(output_message_, sizeof(output_message_));
    }

    if (center_message == NULL)
    {
        return;
    }

    StrCopy(output_message_, center_message);
}

#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

