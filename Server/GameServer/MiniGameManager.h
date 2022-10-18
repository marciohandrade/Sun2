#ifndef SERVER_GAMESERVER_MINIGAMEMANAGER_H
#define SERVER_GAMESERVER_MINIGAMEMANAGER_H

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM

#include <map>

struct EVENT_INFO;
class MiniGame;
struct MiniGameParam;

void GenerateBingo(MiniGameBingoInfos* minigame_bingo_infos);
EVENT_INFO* GetCurEventInfo(eEVENT_MODE event_mode);

class MiniGameManager
{
public:
    MiniGameManager();

    // Common
    bool IsEventProgress(const eMINIGAME_TYPE minigame_type);
    RC::eMINIGAME_RESULT DoAction(MiniGameParam& minigame_param);
    RC::eMINIGAME_RESULT GetReward(MiniGameParam& minigame_param);

    // Bingo
    const MiniGameBingoInfos& GetMiniGameBingoInfos() {return m_minigame_bingo_infos; }
    void SetMiniGameBingoInfos(MiniGameBingoInfos& minigame_bingo_infos);

private:
    // Common
    void _SetMiniGameParamFromInfos(MiniGameParam& minigame_param);
    MiniGame* _GetMiniGame(eMINIGAME_TYPE minigame_type);
    bool _SetMiniGameInfos(MiniGameParam& minigame_param);

    MiniGameBingoInfos m_minigame_bingo_infos;
};


#endif // _NA_008404_20150712_MINIGAME_SYSTEM

#endif // SERVER_GAMESERVER_MINIGAMEMANAGER_H