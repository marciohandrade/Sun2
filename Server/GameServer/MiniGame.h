#ifndef SERVER_GAMESERVER_MINIGAME_H
#define SERVER_GAMESERVER_MINIGAME_H

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM

class Player;

bool CompareIndex(MiniGameBingoInfos* minigame_bingo_infos);

struct MiniGameParam
{
public:
    MiniGameParam(eMINIGAME_TYPE type){ minigame_type = type; };
    eMINIGAME_TYPE minigame_type;

private:
    MiniGameParam(){};
};

struct MiniGameBingoParam : public MiniGameParam
{
    MiniGameBingoParam(eMINIGAME_TYPE type)
        :MiniGameParam(type)
    {
        minigame_bingo_infos = NULL;
    }
    MiniGameBingoInfos* minigame_bingo_infos;
};

struct MiniGameBingoActionParam : public MiniGameBingoParam
{
    MiniGameBingoActionParam(eMINIGAME_TYPE type, int number)
        :MiniGameBingoParam(type)
    {
        bingo_number = number;
    }

    int bingo_number;
};

struct MiniGameBingoRewardParam : public MiniGameBingoParam
{
    MiniGameBingoRewardParam(eMINIGAME_TYPE type, Player* player_)
        :MiniGameBingoParam(type)
    {
        player = player_;
    }
    Player* player;
};

class MiniGame
{
public:
    MiniGame()
    {
        m_minigame_type = eMINIGAME_TYPE_NONE;
    }

    virtual RC::eMINIGAME_RESULT DoAction(MiniGameParam& minigame_param) = 0;
    virtual RC::eMINIGAME_RESULT GetReward(MiniGameParam& minigame_param) = 0;

protected:
    bool ObtainReward( Player* player, CODETYPE reward_code );
    eMINIGAME_TYPE m_minigame_type;
};

class MiniGameBingo : public MiniGame
{
public:
    MiniGameBingo()
    {
        m_minigame_type = eMINIGAME_TYPE_BINGO;
    }

    RC::eMINIGAME_RESULT DoAction(MiniGameParam& minigame_param);
    RC::eMINIGAME_RESULT GetReward(MiniGameParam& minigame_param);

private:
    int CalculateBingo(MiniGameBingoInfos* minigame_bingo_infos);
};

#endif // _NA_008404_20150712_MINIGAME_SYSTEM

#endif // SERVER_GAMESERVER_MINIGAME_H