#include "StdAfx.h"

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
#include "MiniGameParser.h"

MiniGameBingoSetting::MiniGameBingoSetting()
{
    m_max_bingo_number = 0;
    m_free_cell_max_number = 0;
    m_free_cell_min_number = 0;
    m_reward_code.clear();
}

CODETYPE MiniGameBingoSetting::GetRewardCode( int bingo_count )
{
    std::map<int, CODETYPE>::iterator find_iter = m_reward_code.find(bingo_count);
    if (find_iter != m_reward_code.end())
    {
        return find_iter->second;
    }
    return 0;
}

BOOL MiniGameParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));

    LoadMiniGameBingoScript();

    return TRUE;
}

bool MiniGameParser::LoadMiniGameBingoScript()
{
    int max_bingo_number = 0;
    max_bingo_number = GetINIDataInt("MINIGAME_BINGO", "MAX_BINGO_NUMBER");
    m_minigame_bingo_setting.m_max_bingo_number = max_bingo_number;

    int free_cell_max_number = 0;
    free_cell_max_number = GetINIDataInt("MINIGAME_BINGO", "FREE_CELL_MAX_NUMBER");
    m_minigame_bingo_setting.m_free_cell_max_number = free_cell_max_number;

    int free_cell_min_number = 0;
    free_cell_min_number = GetINIDataInt("MINIGAME_BINGO", "FREE_CELL_MIN_NUMBER");
    m_minigame_bingo_setting.m_free_cell_min_number = free_cell_min_number;

    char temp_buffer[128];
    ZeroMemory(&temp_buffer, sizeof(temp_buffer));
    for (int reward_index = 1; reward_index <= MINIGAME_BINGO_MAX_REWARD_COUNT; ++reward_index)
    {
        _snprintf(temp_buffer, _countof(temp_buffer), "REWARD_CODE_%d", reward_index);
        int reward_code = 0;
        reward_code = GetINIDataInt("MINIGAME_BINGO", temp_buffer);
        m_minigame_bingo_setting.m_reward_code.insert(std::make_pair(reward_index, reward_code));
    }

    if (max_bingo_number < MINIGAME_BINGO_LENGTH)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] Max bingo number is too small"), __FUNCTION__);
        ASSERT(false);
        return false;
    }
    
    return true;
}
#endif // _NA_008404_20150712_MINIGAME_SYSTEM