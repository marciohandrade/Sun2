#pragma once
#ifndef GAMESERVER_GMCMDMANAGER_H
#define GAMESERVER_GMCMDMANAGER_H
//==================================================================================================
// @history
//  - ~10.12.08
//  - 10.12.08, waverix, code arrangment
//  - 10.12.09~10, waverix, refactoring to easy implement structure for test unit module and
//                                      to exact grant gm command qualifing condition by GM grades

#include <GMCmdParser.h>
//#include "GMCmdString.h"

//==================================================================================================

class GMCmdManager : public IParser
{
public:
    static const int kMaxGmCommandStringLength = 128;
    //
    struct HandlerNode {
        typedef bool (*func_handler_t)(DWORD player_key, const GmCmdTokens& cmds);
        bool activated; // whether this gm command is activated
        uint8_t authorization; // gm_grade { eGM_GRADE_FIRST = 101, }
        ulong hash_korean;
        ulong hash_english;
        func_handler_t handler;
        //
        const char* __str_korean;
        const char* __str_english;
    };
    //
    GMCmdManager();
    ~GMCmdManager();
    // interfaces for util::Singleton<>
    static GMCmdManager* Instance();
    static void DestroyInstance();
    // IParser interfaces
    virtual void Init();
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    //
    bool ApplyTransparent(Player* player, BOOL turn_on);           //투명
    bool ApplyInvincibilty(Player* player, BOOL turn_on);          //무적
    bool ApplySpeed(Player* player, int nLevel);                   //속도
    bool ApplyEntrance(Player* player, BOOL turn_on);              //입장
    bool ApplyMaxDamage(Player* player, BOOL turn_on);
    bool ApplyMaxExp(Player* player, BOOL turn_on);
    bool ApplyDropListAll(Player* player, BOOL turn_on);

    bool ParseCommand(DWORD player_key, const char* szCmd);
    //bool CheckRight(eGM_STRING type, Player* player);
    bool CheckRight(const HandlerNode* gm_cmd_info, Player* player);

    bool ProcessCommandEx(DWORD player_key, const GmCmdTokens& cmds);

    Player* GetGMPlayer(DWORD player_key);
    // CHANGES: f101209.1L, added the interface for total gm command information
    const HandlerNode* GetGMCmdInfo(const char* gm_string_token) const;

    eGM_STRING GetGMOptionType(const char* gm_option_token) const;
    eATTR_TYPE strStatToEnum(const char* strStat);

private:
    typedef STLX_HASH_MAP<ulong, const HandlerNode*> CmdRouter;
    //
    static GMCmdManager* CreateInstance();
    // load 'GMRights.txt' script
    bool LoadRights();
    //
    bool RegisterAll();
    bool Register_NodeList(const HandlerNode* node_array, size_t number_of_nodes);
    bool SetupAuthorization();
    //
    bool RegisterBatchCmd(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterComplex(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterDomination(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterEtc(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterGuild(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterInfo(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterMob(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterPlayerControl(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterRecord(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterSlot(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterStat(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterZone(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterFomulas(const HandlerNode** node_array, size_t* result_array_count);
    //
    bool RegisterTestUnit_iamsample(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_arycoat(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_gamelulu(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_hukim(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_jaykang(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_namka199(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_waverix(const HandlerNode** node_array, size_t* result_array_count);
    bool RegisterTestUnit_youngmoon(const HandlerNode** node_array, size_t* result_array_count);
    //
    GMCmdParser     gm_cmd_tokenizer_;
    CmdRouter       gm_cmd_router_;
    //GMCmdString     gm_cmd_string_table_;
    static GMCmdManager* instance_static_;
};

//==================================================================================================

// interfaces for util::Singleton<>
inline GMCmdManager* GMCmdManager::Instance()
{
    if (instance_static_) {
        return instance_static_;
    }
    return CreateInstance();
}

// load 'GMRights.txt' script
inline bool GMCmdManager::LoadRights()
{
    return SetupAuthorization();
}

#endif //GAMESERVER_GMCMDMANAGER_H