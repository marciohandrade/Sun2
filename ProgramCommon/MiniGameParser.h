#pragma once

#ifndef PRAGRAMCOMMON_MINIGAME_PARSER_H
#define PRAGRAMCOMMON_MINIGAME_PARSER_H
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include "Struct.h"

class MiniGameBingoSetting
{
public:
    MiniGameBingoSetting();
    ~MiniGameBingoSetting(){};

    int GetMaxBingoNumber() { return m_max_bingo_number; };
    int GetFreeCellMaxNumber() { return m_free_cell_max_number; };
    int GetFreeCellMinNumber() { return m_free_cell_min_number; };
    CODETYPE GetRewardCode(int bingo_count);;

private:
    int m_max_bingo_number;
    int m_free_cell_max_number;
    int m_free_cell_min_number;
    std::map<int, CODETYPE> m_reward_code;

    friend class MiniGameParser;
};

class MiniGameParser : public util::Singleton<MiniGameParser>, public IParser
{
public:
    MiniGameParser(void){};
    ~MiniGameParser(void){};

public:
    /** Parser **/
    virtual void Release(){};
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    /** MiniGame **/
    MiniGameBingoSetting& GetMiniGameBingoSetting(){ return m_minigame_bingo_setting; }

private:
    bool LoadMiniGameBingoScript();

    MiniGameBingoSetting m_minigame_bingo_setting;
};

#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#endif // PRAGRAMCOMMON_MINIGAME_PARSER_H