#include "stdafx.h"

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM

#include "MiniGame.h"
#include "MiniGameParser.h"
#include "RewardManager.h"

bool MiniGame::ObtainReward( Player* player, CODETYPE reward_code )
{
    if (reward_code == 0)
    {
        return true;
    }

    MSG_CG_ITEM_OBTAIN_ITEM_CMD cmd_msg;
    cmd_msg.m_tagInventory.m_InvenCount = 0;
    cmd_msg.m_tagInventory.m_TmpInvenCount = 0;
    RC::eREWARD_RESULT result = g_RewardManager.RewardItem(RewardManager::eREWARD_MINIGAME_BINGO, 
        reward_code, 0, player, cmd_msg.m_tagInventory);
    if( result != RC::RC_REWARD_SUCCESS )
    {
        MSG_CG_ITEM_OBTAIN_ITEM_NAK nak_msg;
        nak_msg.m_byErrorCode = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return false;
    }

    player->SendPacket(&cmd_msg, cmd_msg.GetSize());
    return true;
}

RC::eMINIGAME_RESULT MiniGameBingo::DoAction( MiniGameParam& minigame_param )
{
    MiniGameBingoActionParam& bingo_param = static_cast<MiniGameBingoActionParam&>(minigame_param);
    if (bingo_param.minigame_bingo_infos == NULL)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    if (CompareIndex(bingo_param.minigame_bingo_infos) == false)
    {
        GenerateBingo(bingo_param.minigame_bingo_infos);
    }

    if (bingo_param.bingo_number <= 0 
        || bingo_param.bingo_number > MiniGameParser::Instance()->GetMiniGameBingoSetting().GetMaxBingoNumber())
    {
        return RC::RC_MINIGAME_BINGO_INVALID_ITEM_INFO;
    }

    for (int i = 0; i < MINIGAME_BINGO_LENGTH; ++i)
    {
        if (bingo_param.minigame_bingo_infos->m_bingo_infos[i].bingo_number
            == bingo_param.bingo_number)
        {
            if (bingo_param.minigame_bingo_infos->m_bingo_infos[i].bingo_state
                == eMINIGAME_BINGO_STATE_EMPTY)
            {
                bingo_param.minigame_bingo_infos->m_bingo_infos[i].bingo_state = eMINIGAME_BINGO_STATE_FILLED;
                return RC::RC_MINIGAME_ACTION_SUCCESS;
            }
            else
            {
                return RC::RC_MINIGAME_BINGO_ALREADY_FILLED_NUMBER;
            }
        }
    }

    return RC::RC_MINIGAME_BINGO_NOT_EXIST_NUMBER;
}

int MiniGameBingo::CalculateBingo( MiniGameBingoInfos* minigame_bingo_infos )
{
    if (minigame_bingo_infos == NULL)
    {
        return 0;
    }

    int complete_count = 0;

    int horizen_check[MINIGAME_BINGO_HEIGHT] = {0,};
    for (int w = 0; w < MINIGAME_BINGO_WIDTH; ++w)
    {
        int vertical_check = 0;
        for (int h = 0; h < MINIGAME_BINGO_HEIGHT; ++h)
        {
            int bingo_pos = w + h * MINIGAME_BINGO_WIDTH;
            if (bingo_pos < 0 || MINIGAME_BINGO_LENGTH <= bingo_pos)
            {
                SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Bingo Pos (%d, %d, %d)", 
                    __FUNCTION__, w, h, bingo_pos);
                continue;
            }

            if (minigame_bingo_infos->m_bingo_infos[bingo_pos].bingo_state
                != eMINIGAME_BINGO_STATE_EMPTY)
            {
                ++vertical_check;
                ++horizen_check[h];
            }
        }
        if (vertical_check == MINIGAME_BINGO_HEIGHT)
        {
            ++complete_count;
        }
    }

    for (int i = 0; i < MINIGAME_BINGO_HEIGHT; ++i)
    {
        if (horizen_check[i] == MINIGAME_BINGO_WIDTH)
        {
            ++complete_count;
        }
    }

    int diagonal_check[2] = {0, };
    if (MINIGAME_BINGO_HEIGHT == MINIGAME_BINGO_WIDTH)
    {
        int w = 0;
        int h = 0;
        for (; w < MINIGAME_BINGO_WIDTH; ++w, ++h)
        {
            int bingo_pos = w + h * MINIGAME_BINGO_WIDTH;
            if (bingo_pos < 0 || MINIGAME_BINGO_LENGTH <= bingo_pos)
            {
                SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Bingo Pos (%d, %d, %d)", 
                    __FUNCTION__, w, h, bingo_pos);
                continue;
            }
            if (minigame_bingo_infos->m_bingo_infos[bingo_pos].bingo_state
                != eMINIGAME_BINGO_STATE_EMPTY)
            {
                ++diagonal_check[0];
            }

            bingo_pos = MINIGAME_BINGO_WIDTH - 1 - w + h * MINIGAME_BINGO_WIDTH;
            if (bingo_pos < 0 || MINIGAME_BINGO_LENGTH <= bingo_pos)
            {
                SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Bingo Pos (%d, %d, %d)", 
                    __FUNCTION__, w, h, bingo_pos);
                continue;
            }
            if (minigame_bingo_infos->m_bingo_infos[bingo_pos].bingo_state
                != eMINIGAME_BINGO_STATE_EMPTY)
            {
                ++diagonal_check[1];
            }
        }
        if (diagonal_check[0] == MINIGAME_BINGO_WIDTH)
        {
            ++complete_count;
        }
        if (diagonal_check[1] == MINIGAME_BINGO_WIDTH)
        {
            ++complete_count;
        }
    }

    return complete_count;
}

RC::eMINIGAME_RESULT MiniGameBingo::GetReward( MiniGameParam& minigame_param )
{
    MiniGameBingoRewardParam& bingo_param = static_cast<MiniGameBingoRewardParam&>(minigame_param);
    if (bingo_param.minigame_bingo_infos == NULL)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    if (CompareIndex(bingo_param.minigame_bingo_infos) == false)
    {
        GenerateBingo(bingo_param.minigame_bingo_infos);
    }

    int bingo_count = CalculateBingo(bingo_param.minigame_bingo_infos);

    CODETYPE reward_code = MiniGameParser::Instance()->GetMiniGameBingoSetting().GetRewardCode(bingo_count);

    if (ObtainReward(bingo_param.player, reward_code) == true)
    {
        GenerateBingo(bingo_param.minigame_bingo_infos);
        return  RC::RC_MINIGAME_REWARD_SUCCESS;
    }

    return RC::RC_MINIGAME_BINGO_REWARD_FAILED;
}

bool CompareIndex( MiniGameBingoInfos* minigame_bingo_infos )
{
    if (minigame_bingo_infos == NULL)
    {
        return false;
    }

    EVENT_INFO* event_info = GetCurEventInfo(eEVENT_MINIGAME_BINGO);
    if (event_info == NULL)
    {
        return false;
    }

    if (minigame_bingo_infos->bingo_event_index != event_info->m_NpcCode)
    {
        return false;
    }
    return true;
}


#endif // _NA_008404_20150712_MINIGAME_SYSTEM