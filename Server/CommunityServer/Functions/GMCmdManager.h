#pragma once
#ifndef WORLDSERVER_GMCMDMANAGER_H
#define WORLDSERVER_GMCMDMANAGER_H
//==================================================================================================
// @history
//  - ~10.12.08
//  - 10.12.08, waverix, code arrangment
//  - 10.12.09~10, waverix, refactoring to easy implement structure for test unit module and
//                                      to exact grant gm command qualifying condition by GM grades

#include <ScriptCode.IParser.h>
#include <GMCmdParser.h>

//==================================================================================================

namespace ns_world {
;

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
    // interfaces for util::Singleton<>
    static GMCmdManager* Instance();
    // IParser interfaces
    virtual void Init();
    virtual void Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    //
    bool ParseCommand(DWORD user_key, const char* szCmd);
    //bool CheckRight(eGM_STRING type, Player* player);
    bool CheckRight(const HandlerNode* gm_cmd_info, ns_world::WorldUser* player);

    bool ProcessCommandEx(DWORD user_key, const GmCmdTokens& cmds);

    ns_world::WorldUser* GetGMUser(DWORD user_key);
    // CHANGES: f101209.1L, added the interface for total gm command information
    const HandlerNode* GetGMCmdInfo(const char* gm_string_token) const;

    eGM_STRING GetGMOptionType(const char* gm_option_token) const;
    //
    bool NoticeBroadcast(ns_world::WorldUser* user,
        eNOTICE_TYPE notice_type, const char* gm_notice_msg);
private:
    typedef STLX_HASH_MAP<ulong, const HandlerNode*> CmdRouter;
    //
    GMCmdManager();
    virtual ~GMCmdManager();

    static GMCmdManager* CreateInstance();
    // load 'GMRights.txt' script
    bool LoadRights();
    //
    bool RegisterAll();
    bool Register_NodeList(const HandlerNode* node_array, size_t number_of_nodes);
    bool SetupAuthorization();
    //
    bool RegisterCommands(const HandlerNode** node_array, size_t* result_array_count);
    //
    // data fields...
    GMCmdParser     gm_cmd_tokenizer_;
    CmdRouter       gm_cmd_router_;
    static GMCmdManager* instance_static_;
};

}; //end of namespace
//==================================================================================================

// interfaces for util::Singleton<>
inline ns_world::GMCmdManager* ns_world::GMCmdManager::Instance()
{
    if (instance_static_) {
        return instance_static_;
    }
    return CreateInstance();
}

// load 'GMRights.txt' script
inline bool ns_world::GMCmdManager::LoadRights()
{
    return SetupAuthorization();
}

#endif //GAMESERVER_GMCMDMANAGER_H