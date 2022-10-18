//=======================================================================================================================
/// VendorPage class
/**
@author
Kim Chang Hyun< shogen@webzen.com >	
@since
2006. 11. 20~
@note 
- 
@history
- Manager인데 해쉬를 직접 가지지 않고 파서를 참조한다. 파서가 해쉬를 가지고 있는데 Common이라서 옮기기가 애매하다.

*/
//=======================================================================================================================

#pragma once

#include <lua/lua_manager.h>

class SyncMerchant;

class ExtraNPCManager
{
public:
	ExtraNPCManager(void);
	//~ExtraNPCManager(void);	

	BOOL IsLengthCanUseNPC( CODETYPE NPCCODE, Player* pPlayer, eNPC_TYPE eCheckNPCType=eNPC_TYPE_NONE );
    // NOTE: f110704.5L
    bool CheckProtectedNpc(SyncMerchant* check_npc) const;

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
public:
    bool initialize();
    void release();

    void ParseCGPacket(MSG_BASE* recv_msg);
    void ParseDGPacket(MSG_BASE* recv_msg);
    
    CLuaStreamPack* find_lua_pack(CODETYPE npc_code);
    bool initialize_lua_handler();
    static int register_functions(lua_State* lua_state);
    static bool Decrypt( char* buffer, unsigned bufferSize );

    int execute_script_main(int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack);
    int execute_script_menu(int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack);
    int execute_script_messagebox(int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack,
                                    CHAR* _data, WORD _data_size);
    int execute_script_attendance(int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack,
                                    int result);

    static int OnShowMessageBox(lua_State* lua_state);
    static int OnShowMenu(lua_State* lua_state);

    static int GetCharacterClass(lua_State* lua_state);
    static int GetCharacterLevel(lua_State* lua_state);
    /*static int GetCharacterExp(lua_State* lua_state);*/

    static int GetInventoryEmptySlotCount(lua_State* lua_state);
    static int CheckItemCount(lua_State* lua_state);
    static int RobItem(lua_State* lua_state);
    static int GiveItem(lua_State* lua_state);

    static int ZoneChange(lua_State* lua_state);
    static int CheckPotalToDominationField(lua_State* lua_state);
    static int SendDominationMinimapList(lua_State* lua_state);
    static int IsDominationGoing(lua_State* lua_state);
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    static int GetDominationGuildGuid(lua_State* lua_state);
    static int GetCharacterGuildGuid(lua_State* lua_state);
#endif // _NA_000000_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    static int IsDominationChannel(lua_State* lua_state);

    static int SetAttendance(lua_State* lua_state);
    static int GetAttendance(lua_State* lua_state);

private:
    LuaHandler lua_handler_;
    typedef std::map<CODETYPE, CLuaStreamPack*> LuaStreamPackMap;
    LuaStreamPackMap npc_script_map_;
    std::list<CLuaStreamPack*> lua_stream_pack_array_;
    typedef std::map<int, std::string> StringMap;
    StringMap menu_string_;
    std::map<int, std::map<int, std::string> > messagebox_string_;
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

private:
};

extern ExtraNPCManager g_ExtraNPCManager;