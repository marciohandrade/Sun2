#include "stdafx.h"

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM

#include "MiniGameManager.h"
#include <SunEventInfoParser.h>
#include "MiniGameParser.h"
#include "MiniGame.h"

MiniGameManager::MiniGameManager()
{
    m_minigame_bingo_infos.Clear();
}

void MiniGameManager::SetMiniGameBingoInfos( MiniGameBingoInfos& minigame_bingo_infos )
{
    memcpy(&m_minigame_bingo_infos, &minigame_bingo_infos, sizeof(minigame_bingo_infos));

    if (IsEventProgress(eMINIGAME_TYPE_BINGO) == true
        && (m_minigame_bingo_infos.IsGenerated() == false || CompareIndex(&minigame_bingo_infos) == false))
    {
        GenerateBingo(&m_minigame_bingo_infos);
    }
}

bool MiniGameManager::IsEventProgress(const eMINIGAME_TYPE minigame_type)
{
    eEVENT_MODE event_mode = eEVENT_MAX;
    switch(minigame_type)
    {
    case eMINIGAME_TYPE_BINGO:
        event_mode = eEVENT_MINIGAME_BINGO;
        break;
    default:
        return false;
    }

    EVENT_INFO* event_info = GetCurEventInfo(eEVENT_MINIGAME_BINGO);
    if (event_info == NULL)
    {
        return false;
    }

    return true;
}

MiniGame* MiniGameManager::_GetMiniGame( eMINIGAME_TYPE minigame_type )
{
    MiniGame* minigame = NULL;

    switch (minigame_type)
    {
    case eMINIGAME_TYPE_BINGO:
        {
            static MiniGameBingo minigame_bingo;
            minigame = &minigame_bingo;
        } break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Type (%d)", 
                __FUNCTION__, minigame_type);
        } break;
    }
    return minigame;
}

bool MiniGameManager::_SetMiniGameInfos( MiniGameParam& minigame_param )
{
    switch (minigame_param.minigame_type)
    {
    case eMINIGAME_TYPE_BINGO:
        {
            MiniGameBingoParam& bingo_param = static_cast<MiniGameBingoParam&>(minigame_param);
            bingo_param.minigame_bingo_infos = &m_minigame_bingo_infos;
            return true;
        } break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Type (%d)", 
                __FUNCTION__, minigame_param.minigame_type);
        } break;
    }
    
    return false;
}

void MiniGameManager::_SetMiniGameParamFromInfos( MiniGameParam& minigame_param )
{
    switch (minigame_param.minigame_type)
    {
    case eMINIGAME_TYPE_BINGO:
        {
            MiniGameBingoActionParam& minigame_bingo_param = static_cast<MiniGameBingoActionParam&>(minigame_param);
            minigame_bingo_param.minigame_bingo_infos = &m_minigame_bingo_infos;
        } break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[%s] Invalid MiniGame Type (%d)", 
                __FUNCTION__, minigame_param.minigame_type);
        } break;
    }
}

RC::eMINIGAME_RESULT MiniGameManager::DoAction( MiniGameParam& minigame_param )
{
    if (IsEventProgress(minigame_param.minigame_type) == false)
    {
        return RC::RC_MINIGAME_NOT_OPEN_TIME;
    }

    MiniGame* minigame = _GetMiniGame(minigame_param.minigame_type);
    if (minigame == NULL)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    if (_SetMiniGameInfos(minigame_param) == false)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    return minigame->DoAction(minigame_param);
}

RC::eMINIGAME_RESULT MiniGameManager::GetReward( MiniGameParam& minigame_param )
{
    if (IsEventProgress(minigame_param.minigame_type) == false)
    {
        return RC::RC_MINIGAME_NOT_OPEN_TIME;
    }

    MiniGame* minigame = _GetMiniGame(minigame_param.minigame_type);
    if (minigame == NULL)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    if (_SetMiniGameInfos(minigame_param) == false)
    {
        return RC::RC_MINIGAME_INVAILD_TYPE;
    }

    return minigame->GetReward(minigame_param);
}

void GenerateBingo( MiniGameBingoInfos* minigame_bingo_infos )
{
    int max_number = MiniGameParser::Instance()->GetMiniGameBingoSetting().GetMaxBingoNumber();
    if (max_number < MINIGAME_BINGO_LENGTH)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] Max bingo number is too small"), __FUNCTION__);
        return;
    }

    if (minigame_bingo_infos == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] Minigame Bingo invaild state"), __FUNCTION__);
        return;
    }

    EVENT_INFO* event_info = GetCurEventInfo(eEVENT_MINIGAME_BINGO);
    if (event_info == NULL)
    {
        return;
    }

    minigame_bingo_infos->bingo_event_index = event_info->m_NpcCode;

    std::vector<int> temp_cell(max_number);
    for (int i = 0; i < max_number; ++i)
    {
        temp_cell[i] = i + 1;
    }

    std::random_shuffle(temp_cell.begin(), temp_cell.end());

    for (int i = 0; i < MINIGAME_BINGO_LENGTH; ++i)
    {
        minigame_bingo_infos->m_bingo_infos[i].bingo_number = temp_cell[i];
        minigame_bingo_infos->m_bingo_infos[i].bingo_state = eMINIGAME_BINGO_STATE_EMPTY;
    }

    int free_cell_number = 
        random(MiniGameParser::Instance()->GetMiniGameBingoSetting().GetFreeCellMinNumber(),
        MiniGameParser::Instance()->GetMiniGameBingoSetting().GetFreeCellMaxNumber());

    for (int i = 0; i < free_cell_number; ++i)
    {
        int free_cell = random(0, MINIGAME_BINGO_LENGTH - 1);
        minigame_bingo_infos->m_bingo_infos[free_cell].bingo_state = eMINIGAME_BINGO_STATE_FREE;
    }
}

EVENT_INFO* GetCurEventInfo( eEVENT_MODE event_mode )
{

    EVENTINFO_MAP* rMap = SunEventInfoParser::Instance()->GetEventInfoMap();
    EVENTINFO_MAP_ITER end_iter = rMap->end();
    EVENTINFO_MAP_ITER cur_iter = rMap->begin();
    for (; cur_iter != end_iter; ++cur_iter)
    {
        EVENT_INFO& eventInfo = cur_iter->second;
        if (eventInfo.m_byEventMode != event_mode)
        {
            continue;
        }

        if (eventInfo.CheckEvent() == TRUE)
        {
            return &cur_iter->second;
        }
    }

    return NULL;
}

#endif // _NA_008404_20150712_MINIGAME_SYSTEM