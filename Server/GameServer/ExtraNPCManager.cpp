#include "Stdafx.h"
#include "ExtraNPCManager.h"

#include "NPCInfoParser.h"
#include "PublicMath.h"

#include "Player.h"
#include "ItemManager.h"
#include "InventorySlotContainer.h"
#include "SyncMerchant.h"
#include "GameZone.h"
#include "GameDominationManager.h"
#include <PacketStruct_CG_SmartNPC.h>
#include <PacketStruct_SmartNPCSystem.h>

#include "AuMD5Encrypt.h"
#include <lua/lua_manager.h>
#include <TinyXml/tinyxml.h>
#include <json/json.h>

#include "ServerOptionParserEx.h"

//==================================================================================================

ExtraNPCManager g_ExtraNPCManager;

ExtraNPCManager::ExtraNPCManager()
{
	
}

BOOL
ExtraNPCManager::IsLengthCanUseNPC( CODETYPE NPCCODE, Player* pPlayer, eNPC_TYPE eCheckNPCType )
{
    GameField* const game_field = pPlayer->GetField();
    if (game_field == NULL) {
        return false;
    };
	EXTRA_NPCINFO *	pExtraNPCInfo = NPCInfoParser::Instance()->GetExtraInfo(NPCCODE);
	if( pExtraNPCInfo == NULL )
	{
		SUNLOG( eCRITICAL_LOG,  "없는 ExtraNPC 참조 : IsCanUseNPC()" );
		return FALSE;
	}

    bool violated_field_check = (pExtraNPCInfo->m_FieldID != game_field->GetFieldCode());
#ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
    GameZone* const game_zone = game_field->GetGameZone();
    if (violated_field_check && (game_zone->GetMapCode() == HardCode::MAPCODE_for_TUTORIAL_FIELD)) {
        violated_field_check = false;
    }
#endif
    if (violated_field_check)
	{
		SUNLOG(eCRITICAL_LOG, "다른 Zone 에서 NPC 이용 시도(hacking) - shogen" );
		return FALSE;
	}

	if( eCheckNPCType != eNPC_TYPE_NONE && pExtraNPCInfo->m_eNPCTYPE != eCheckNPCType )
	{
		SUNLOG(eCRITICAL_LOG, "잘못된 NPC 이용 시도(hacking)" );
		return FALSE;
	}

    // (WAVERIX) (090825) (CHANGES) 퀘스트 수락시 NPC와의 거리 소수점 단위 오차로 인해 정상 동작 안하는 문제 수정
    // 그 외에도 사용하는 함수인 관계로 좀 여유를 두었다. 높이도 고려되는 값이라 클라이언트와 미묘한
    // 오차가 발생한다.

    const float DistanceThisWithNpcSquared = (DISTANCE_NPC_MEET + 0.3f) * (DISTANCE_NPC_MEET + 0.3f);

    // using the unit id that is changed by _NA_0_20100222_UNIT_TRIGGERS_REGEN
    float distance_current = 0.f;

    if (pExtraNPCInfo->m_unitID != 0)
    {
        GameField* const field = pPlayer->GetField();
        NPC* const npc = field->FindNPCForUnitID(pExtraNPCInfo->m_unitID);

        if (!npc)
            return false;

        distance_current = Math_VectorLength2(*npc->GetPosPtr(), *pPlayer->GetPosPtr());
    }
    else
    {
        BOOST_STATIC_ASSERT(_countof(pExtraNPCInfo->m_fNPCPos) == 3 &&
                            sizeof(pExtraNPCInfo->m_fNPCPos) == sizeof(WzVector));

        distance_current = Math_VectorLength2((WzVector&)(pExtraNPCInfo->m_fNPCPos),
                                             *pPlayer->GetPosPtr());
    }

    if(!(distance_current < DistanceThisWithNpcSquared))
        return false;

	return TRUE;
}

//==================================================================================================
// NOTE: f110704.5L
bool ExtraNPCManager::CheckProtectedNpc(SyncMerchant* check_npc) const
{
    const DWORD unit_id = check_npc->GetRegenID();
    if (unit_id == 0) {
        return false;
    };
    const BASE_NPCINFO* npc_info = check_npc->GetBaseInfo();
    const EXTRA_NPCINFO* extra_npc_info = \
        NPCInfoParser::Instance()->GetExtraInfo(npc_info->m_MonsterCode);
    if (extra_npc_info == NULL) {
        return false;
    };
    if (unit_id != extra_npc_info->m_unitID) {
        return false;
    };
    // Check ExtraNpcInfo.NpcType
    if (extra_npc_info->m_eNPCTYPE != eNPC_TYPE_PROTECTS) {
        return false;
    };
    return true;
}

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
bool ExtraNPCManager::initialize()
{
    TiXmlDocument manager_document;
    if(!manager_document.LoadFile("data\\NPCManager\\NpcManager.xml"))
        return false;
    
    TiXmlNode* link_files_node = manager_document.FirstChild("LinkFile");
    if(!link_files_node)
        return false;

    for (TiXmlElement* lua_file = link_files_node->FirstChildElement("LUAFile"); \
        lua_file; lua_file = lua_file->NextSiblingElement())
    {
        TiXmlElement* name_element = lua_file->FirstChildElement("NAME");
        const CHAR* lua_file_name = (name_element) ? name_element->GetText() : NULL;

        CLuaStreamPack* lua_pack = new CLuaStreamPack;
        if (lua_pack == NULL) {
            SUNLOG(eCRITICAL_LOG, "lua stream pack new operator fail");
            continue;
        }

        CHAR file_name_path[MAX_PATH] = { 0, };
        _snprintf(file_name_path, sizeof(file_name_path), \
            "data\\NPCManager\\%s", lua_file_name);

        if (lua_pack->open(file_name_path, (Decrypt_CFunction)ExtraNPCManager::Decrypt) == false) {
            SUNLOG(eCRITICAL_LOG, "%s open fail", lua_file_name);
            delete lua_pack;
            continue;
        }

        lua_stream_pack_array_.push_back(lua_pack);

        for (TiXmlElement* npc_id_element = lua_file->FirstChildElement("NPCID"); \
            npc_id_element; npc_id_element = npc_id_element->NextSiblingElement())
        {
            int npc_id = atoi(npc_id_element->GetText());

            npc_script_map_.insert(LuaStreamPackMap::value_type(npc_id, lua_pack));
        }
    }

    if (initialize_lua_handler() == false)
        return false;

    //////////////////////////////////////////////////////////////////////////
    //
    TiXmlDocument dialog_document_;
    if(!dialog_document_.LoadFile("data\\NPCManager\\NpcDialog.xml"))
        return false;

    TiXmlNode* dialog_node = dialog_document_.FirstChild("NpcDialog");
    if(!dialog_node)
        return false;

    TiXmlNode* menu_list_node = dialog_node->FirstChild("MenuList");
    if(!menu_list_node)
        return false;

    for (TiXmlElement* menu_element = menu_list_node->FirstChildElement("Menu"); \
        menu_element; menu_element = menu_element->NextSiblingElement())
    {
        Json::Value menu;

        int menu_index;
        menu_element->Attribute("ID", &menu_index);
        menu["ID"] = menu_index;

        TiXmlElement* type_element = menu_element->FirstChildElement("TYPE");
        const CHAR* TYPE = (type_element) ? type_element->GetText() : NULL;
        menu["TYPE"] = (TYPE) ? atoi(TYPE) : 1;

        TiXmlElement* title_element = menu_element->FirstChildElement("TITLE");
        const CHAR* title_string = (title_element) ? title_element->GetText() : NULL;
        menu["TITLE"] = title_string;

        TiXmlElement* dialog_element = menu_element->FirstChildElement("Dialog");
        const CHAR* dialog_string = (dialog_element) ? dialog_element->GetText() : NULL;
        menu["DIALOG"] = dialog_string;

        Json::Value button_array;
        for (TiXmlElement* button_element = menu_element->FirstChildElement("Button");
            button_element; button_element = button_element->NextSiblingElement())
        {
            Json::Value button;

            TiXmlElement* EID_elem = button_element->FirstChildElement("EID");
            const CHAR* EID = (EID_elem) ? EID_elem->GetText() : NULL;
            if (EID) {
                button["EID"] = atoi(EID);
            }

            TiXmlElement* STEP_elem = button_element->FirstChildElement("STEP");
            const CHAR* STEP = (STEP_elem) ? STEP_elem->GetText() : NULL;
            if (STEP) {
                button["STEP"] = atoi(STEP);
            }

            TiXmlElement* TEXT_elem = button_element->FirstChildElement("TEXT");
            const CHAR* TEXT = (TEXT_elem) ? TEXT_elem->GetText() : NULL;
            if (TEXT) {
                button["TEXT"] = TEXT;
            }

            button_array.append(button);
        }

        menu["BUTTONS"] = button_array;

        //Json::StyledWriter writer;
        Json::FastWriter writer;
        std::string menu_dialog_string = writer.write(menu);
        //std::cout << menu_dialog_string << std::endl << menu_dialog_string.length() << std::endl;

        if (false == menu_string_.insert( \
                        StringMap::value_type(menu_index, menu_dialog_string)).second)
        {
            SUNLOG(eCRITICAL_LOG, "[%s:%d] duplicated menu id - %d", \
                __FUNCTION__, __LINE__, menu_index);
        }
    }

    TiXmlNode* messagebox_list_node = dialog_node->FirstChild("MessageBoxList");
    if(!messagebox_list_node)
        return false;

    for (TiXmlElement* group_element = messagebox_list_node->FirstChildElement("Group"); \
        group_element; group_element = group_element->NextSiblingElement())
    {
        int group_index;
        group_element->Attribute("ID", &group_index);
        
        for (TiXmlElement* message_element = group_element->FirstChildElement("Message"); \
            message_element; message_element = message_element->NextSiblingElement())
        {
            int message_index;
            message_element->Attribute("ID", &message_index);

            const CHAR* message_string = message_element->GetText();
            std::string s(message_string, strlen(message_string));
            messagebox_string_[group_index][message_index] = s;
        }
    }

    return true;
}


void ExtraNPCManager::release()
{
    for(std::list<CLuaStreamPack*>::iterator it = lua_stream_pack_array_.begin(); \
        it != lua_stream_pack_array_.end(); ++it)
    {
        CLuaStreamPack* pack = (*it);
        if (pack) {
            delete (pack);
            pack = NULL;
        }
    }

    lua_stream_pack_array_.clear();
    npc_script_map_.clear();
    menu_string_.clear();
	lua_handler_.close();
}

CLuaStreamPack* ExtraNPCManager::find_lua_pack(CODETYPE npc_code)
{
    LuaStreamPackMap::iterator it = npc_script_map_.find(npc_code);
    if (it == npc_script_map_.end())
        return NULL;

    return (it->second);
}

bool ExtraNPCManager::initialize_lua_handler()
{
    if (lua_handler_.initialize((LuaHandler::OnRegister_CFunction)register_functions) != true)
        return false;

    return true;
}

int ExtraNPCManager::register_functions( lua_State* lua_state )
{
    if(lua_state == NULL)
        return -1;

    lua_register(lua_state, "ShowMessageBox", ExtraNPCManager::OnShowMessageBox);
    lua_register(lua_state, "ShowMenu", ExtraNPCManager::OnShowMenu);

    lua_register(lua_state, "GetClass", ExtraNPCManager::GetCharacterClass);
    lua_register(lua_state, "GetLevel", ExtraNPCManager::GetCharacterLevel);
    /*lua_register(lua_state, "GetExp", ExtraNPCManager::GetCharacterExp);*/ // 현재 루아 라이브러리가 int64를 지원 안하므로 사용불가

    lua_register(lua_state, "GetInventoryEmptyCount", ExtraNPCManager::GetInventoryEmptySlotCount);
    lua_register(lua_state, "CheckItemCount", ExtraNPCManager::CheckItemCount);
    lua_register(lua_state, "RobItem", ExtraNPCManager::RobItem);
    lua_register(lua_state, "GiveItem", ExtraNPCManager::GiveItem);

    lua_register(lua_state, "ZoneChange", ExtraNPCManager::ZoneChange);
    lua_register(lua_state, "CheckPotalToDominationField", ExtraNPCManager::CheckPotalToDominationField);
    lua_register(lua_state, "SendDominationMinimapList", ExtraNPCManager::SendDominationMinimapList);

    lua_register(lua_state, "IsDominationGoing", ExtraNPCManager::IsDominationGoing);
    lua_register(lua_state, "IsDominationChannel", ExtraNPCManager::IsDominationChannel);
    lua_register(lua_state, "SetAttendance", ExtraNPCManager::SetAttendance);
    lua_register(lua_state, "GetAttendance", ExtraNPCManager::GetAttendance);

#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    lua_register(lua_state, "GetDominationGuildGuid", ExtraNPCManager::GetDominationGuildGuid);
    lua_register(lua_state, "GetCharacterGuildGuid", ExtraNPCManager::GetCharacterGuildGuid);
#endif // _NA_000000_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

    /*lua_register(lua_state, "CastSkill", AgsmNpcManager::CastSkill);
    lua_register(lua_state, "Move", AgsmNpcManager::Move);
    lua_register(lua_state, "ReturnTown", AgsmNpcManager::ReturnTown);
    lua_register(lua_state, "SetDefaultInvincible", AgsmNpcManager::SetDefaultInvincible);*/

    return 0;
}

int ExtraNPCManager::execute_script_main( int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack )
{
    if( /*CID == 0 || EID == 0 ||*/ !lua_pack )
        return -1;

    lua_State* lua_state = lua_handler_.state_;
    if(lua_state == NULL)
        return -1;

    if (lua_pack->do_buffer(lua_state) == false)
        return -1;

    lua_getglobal(lua_state, "Main");
    lua_pushnumber(lua_state, CID);
    lua_pushnumber(lua_state, NID);
    lua_pushnumber(lua_state, EID);
    lua_pushnumber(lua_state, STEP);

    int status = lua_pcall(lua_state, 4, 1, 0);

    if (status != 0)
    {
        char strCharBuff[256] = { 0, };
        _snprintf(strCharBuff, sizeof(strCharBuff), "%s", lua_tostring(lua_state, -1));
        //AuLogFile_s("Log\\LuaError.log", strCharBuff);

        fprintf(stderr, "\t%s\n", strCharBuff);
        lua_pop(lua_state, 1);

        return -1;
    }

    int ret = 0;
    while (lua_gettop(lua_state))
    {
        switch (lua_type(lua_state, lua_gettop(lua_state)))
        {
        case LUA_TNUMBER: 
            ret = (int)lua_tonumber(lua_state, lua_gettop(lua_state));
            break;
        default: 
            ret = -1;
            break;
        }
        lua_pop(lua_state, 1 );
    }

    return ret;
}

int ExtraNPCManager::execute_script_menu( int CID, int NID, int EID, int STEP, CLuaStreamPack* lua_pack )
{
    if( /*CID == 0 || EID == 0 ||*/ !lua_pack )
        return -1;

    lua_State* lua_state = lua_handler_.state_;
    if(lua_state == NULL)
        return -1;

    if (lua_pack->do_buffer(lua_state) == false)
        return -1;

    lua_getglobal(lua_state, "OnMenu");
    lua_pushnumber(lua_state, CID);
    lua_pushnumber(lua_state, NID);
    lua_pushnumber(lua_state, EID);
    lua_pushnumber(lua_state, STEP);

    int status = lua_pcall(lua_state, 4, 1, 0);

    if (status != 0)
    {
        char strCharBuff[256] = { 0, };
        _snprintf(strCharBuff, sizeof(strCharBuff), "%s", lua_tostring(lua_state, -1));
        //AuLogFile_s("Log\\LuaError.log", strCharBuff);

        fprintf(stderr, "\t%s\n", strCharBuff);
        lua_pop(lua_state, 1);

        return -1;
    }

    int ret = 0;
    while (lua_gettop(lua_state))
    {
        switch (lua_type(lua_state, lua_gettop(lua_state)))
        {
        case LUA_TNUMBER: 
            ret = (int)lua_tonumber(lua_state, lua_gettop(lua_state));
            break;
        default: 
            ret = -1;
            break;
        }
        lua_pop(lua_state, 1 );
    }

    return ret;
}

int ExtraNPCManager::execute_script_messagebox( int CID, int NID, int EID, int STEP, \
                                            CLuaStreamPack* lua_pack, CHAR* _data, WORD _data_size)
{
    if( /*CID == 0 || EID == 0 ||*/ !lua_pack )
        return -1;

    std::string data_string(_data, _data_size);
    Json::Value messagebox_data;
    Json::Reader reader;
    reader.parse(data_string, messagebox_data);
    
    int button_index = messagebox_data.get("BUTTON", 0).asInt();
    Json::Value temp_value = messagebox_data.get("STRING", "");
    const char* input_string = temp_value.asCString();

    DWORD itemcode = 0;

    lua_State* lua_state = lua_handler_.state_;
    if(lua_state == NULL)
        return -1;

    if (lua_pack->do_buffer(lua_state) == false)
        return -1;

    lua_getglobal(lua_state, "OnMessageBox");
    lua_pushnumber(lua_state, CID);
    lua_pushnumber(lua_state, NID);
    lua_pushnumber(lua_state, EID);
    lua_pushnumber(lua_state, STEP);

    lua_pushnumber(lua_state, button_index);
    lua_pushstring(lua_state, input_string);
    lua_pushnumber(lua_state, itemcode);

    int status = lua_pcall(lua_state, 7, 1, 0);

    if (status != 0)
    {
        char strCharBuff[256] = { 0, };
        _snprintf(strCharBuff, sizeof(strCharBuff), "%s", lua_tostring(lua_state, -1));
        //AuLogFile_s("Log\\LuaError.log", strCharBuff);

        fprintf(stderr, "\t%s\n", strCharBuff);
        lua_pop(lua_state, 1);

        return -1;
    }

    int ret = 0;
    while (lua_gettop(lua_state))
    {
        switch (lua_type(lua_state, lua_gettop(lua_state)))
        {
        case LUA_TNUMBER: 
            ret = (int)lua_tonumber(lua_state, lua_gettop(lua_state));
            break;
        default: 
            ret = -1;
            break;
        }
        lua_pop(lua_state, 1 );
    }

    return ret;
}

int ExtraNPCManager::OnShowMessageBox( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -7);
    INT32 NID = (INT32)lua_tonumber(lua_state, -6);
    INT32 EID = (INT32)lua_tonumber(lua_state, -5);
    INT32 STEP = (INT32)lua_tonumber(lua_state, -4);

    INT32 message_type = (INT32)lua_tonumber(lua_state, -3);
    INT32 group_index = (INT32)lua_tonumber(lua_state, -2);
    INT32 message_index = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 7);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 1;

    Json::Value message_box;
    message_box["TYPE"] = message_type;
    message_box["string"] = g_ExtraNPCManager.messagebox_string_[group_index][message_index];

    Json::FastWriter writer;
    std::string message_box_dialog = writer.write(message_box);

    MSG_CG_NPCDIALOG_SHOWMESSAGEBOX_CMD packet(CID, NID, EID, STEP);
    packet.data_length_ = message_box_dialog.length();
    memcpy(packet.data_, message_box_dialog.c_str(), message_box_dialog.length());
    player->SendPacket(&packet, sizeof(packet));

    return 0;
}

int ExtraNPCManager::OnShowMenu( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -3);
    INT32 NID = (INT32)lua_tonumber(lua_state, -2);
    INT32 MenuID = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 3);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 1;

    StringMap::iterator it = g_ExtraNPCManager.menu_string_.find(MenuID);
    if (it == g_ExtraNPCManager.menu_string_.end()) {
        SUNLOG(eCRITICAL_LOG, "[%s:%d] invalid menu id - %d", __FUNCTION__, __LINE__, MenuID);
        return 1;
    }

    std::string& menu_dialog = it->second;
    if (menu_dialog == "") {
        SUNLOG(eCRITICAL_LOG, "[%s:%d] invalid menu id - %d", __FUNCTION__, __LINE__, MenuID);
        return 1;
    }

    MSG_CG_NPCDIALOG_SHOWMENU_CMD packet(CID, NID, /*EID*/0, /*STEP*/0);
    packet.data_length_ = menu_dialog.length();
    memcpy(packet.data_, menu_dialog.c_str(), menu_dialog.length());
    player->SendPacket(&packet, sizeof(packet));

    return 0;
}

int ExtraNPCManager::execute_script_attendance(int CID, int NID, int EID, int STEP, \
                                               CLuaStreamPack* lua_pack, int result)
{
    if( /*CID == 0 || EID == 0 ||*/ !lua_pack )
        return -1;

    lua_State* lua_state = lua_handler_.state_;
    if(lua_state == NULL)
        return -1;

    if (lua_pack->do_buffer(lua_state) == false)
        return -1;

    lua_getglobal(lua_state, "OnAttendance");
    lua_pushnumber(lua_state, CID);
    lua_pushnumber(lua_state, NID);
    lua_pushnumber(lua_state, EID);
    lua_pushnumber(lua_state, STEP);

    lua_pushnumber(lua_state, result);

    int status = lua_pcall(lua_state, 5, 1, 0);

    if (status != 0)
    {
        char strCharBuff[256] = { 0, };
        _snprintf(strCharBuff, sizeof(strCharBuff), "%s", lua_tostring(lua_state, -1));
        //AuLogFile_s("Log\\LuaError.log", strCharBuff);

        fprintf(stderr, "\t%s\n", strCharBuff);
        lua_pop(lua_state, 1);

        return -1;
    }

    int ret = 0;
    while (lua_gettop(lua_state))
    {
        switch (lua_type(lua_state, lua_gettop(lua_state)))
        {
        case LUA_TNUMBER: 
            ret = (int)lua_tonumber(lua_state, lua_gettop(lua_state));
            break;
        default: 
            ret = -1;
            break;
        }
        lua_pop(lua_state, 1 );
    }

    return ret;
}

void ExtraNPCManager::ParseCGPacket(MSG_BASE* recv_msg)
{
    MSG_CG_CHARINFO_SMARTNPC_PACKET* const packet = \
        static_cast<MSG_CG_CHARINFO_SMARTNPC_PACKET*>(recv_msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player || !player->GetField()) return;

    if (packet->CID != player->GetObjectKey()) {
        SUNLOG(eCRITICAL_LOG, "[%s:%d] invalid character object key - %d", \
            __FUNCTION__, __LINE__, player->GetCharGuid());
        return;
    }

    if (IsLengthCanUseNPC(packet->NID, player, eNPC_TYPE_SMARTNPC) == false) {
        return;
    }

    DWORD CID = packet->CID;
    DWORD NID = packet->NID;
    DWORD EID = packet->EID;
    DWORD STEP = packet->STEP;

    NPCInfoParser* const npc_parser = NPCInfoParser::Instance();
    EXTRA_NPCINFO* const extra_info = npc_parser->GetExtraInfo(NID);
    if (NULL == extra_info)
        return;
    CLuaStreamPack* lua_pack = g_ExtraNPCManager.find_lua_pack(extra_info->m_NPCCODE);
    if (NULL == lua_pack)
        return;

    switch(packet->packet_type)
    {
        case NPCDIALOG_OPERATION_CMD:
            {
                MSG_CG_NPCDIALOG_OPERATION_CMD* packet2 = \
                    static_cast<MSG_CG_NPCDIALOG_OPERATION_CMD*>(packet);

                execute_script_main(CID, NID, EID, STEP, lua_pack);
                //
            } break;
        case NPCDIALOG_ON_CLICK_MENU_CMD:
            {
                MSG_CG_NPCDIALOG_ON_CLICK_MENU_CMD* packet2 = \
                    static_cast<MSG_CG_NPCDIALOG_ON_CLICK_MENU_CMD*>(packet);

                execute_script_menu(CID, NID, EID, STEP, lua_pack);
                //
            } break;
        case NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD:
            {
                MSG_CG_NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD* packet2 = \
                    static_cast<MSG_CG_NPCDIALOG_ON_CLICK_SHOWMESSAGEBOX_CMD*>(packet);

                execute_script_messagebox(CID, NID, EID, STEP, lua_pack, \
                                          packet2->data_, packet2->data_length_);
                //
            } break;
    }
}

void ExtraNPCManager::ParseDGPacket( MSG_BASE* recv_msg )
{
    MSG_DG_EVENT_SMARTNPC_SYSTEM* packet = static_cast<MSG_DG_EVENT_SMARTNPC_SYSTEM*>(recv_msg);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey( packet->m_dwKey );
    if( !player || !player->GetField()) return;

    if (packet->CID != player->GetObjectKey())
    {
        SUNLOG(eCRITICAL_LOG, "[%s:%d] invalid character object key - %d", \
            __FUNCTION__, __LINE__, player->GetCharGuid());
        return;
    }

    DWORD CID = packet->CID;
    DWORD NID = packet->NID;
    DWORD EID = packet->EID;
    DWORD STEP = packet->STEP;

    NPCInfoParser* const npc_parser = NPCInfoParser::Instance();
    EXTRA_NPCINFO* const extra_info = npc_parser->GetExtraInfo(NID);
    if (NULL == extra_info) {
        return;
    }

    CLuaStreamPack* lua_pack = g_ExtraNPCManager.find_lua_pack(extra_info->m_NPCCODE);
    if (NULL == lua_pack) {
        return;
    }

    switch(packet->packet_type)
    {
        case DG_SMARTNPC_SET_ATTENDANCE_ACK:
        {
            MSG_SMARTNPC_SET_ATTENDANCE_ACK* packet2 = \
                static_cast<MSG_SMARTNPC_SET_ATTENDANCE_ACK*>(packet);

            execute_script_attendance(CID, NID, EID, STEP, lua_pack, packet2->result);
            //
        } break;
        case DG_SMARTNPC_GET_ATTENDANCE_ACK:
            {
                MSG_SMARTNPC_GET_ATTENDANCE_ACK* packet2 = \
                    static_cast<MSG_SMARTNPC_GET_ATTENDANCE_ACK*>(packet);

                execute_script_attendance(CID, NID, EID, STEP, lua_pack, packet2->result);
                //
            } break;
    }
}

bool ExtraNPCManager::Decrypt( char* buffer, unsigned bufferSize )
{
    const CHAR* NPCMANAGER_HASHKEY = "npcmanager_key";
    AuMD5Encrypt md5;

    return md5.DecryptString( NPCMANAGER_HASHKEY, buffer, bufferSize ) ? true : false;
}

int ExtraNPCManager::GetCharacterClass( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    const eCHAR_TYPE char_type = player->GetCharType();

    lua_pushinteger(lua_state, char_type);

    return 1;
}

/* 현재 루아 라이브러리가 int64를 지원하지 않으므로 사용 불가
int ExtraNPCManager::GetCharacterExp( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    const EXPTYPE char_exp = player->GetExp();

    lua_pushunsigned(lua_state, char_exp);

    return 1;
}
*/

int ExtraNPCManager::GetCharacterLevel( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    const LEVELTYPE char_level = player->GetLevel();

    lua_pushinteger(lua_state, char_level);

    return 1;
}

int ExtraNPCManager::GetInventoryEmptySlotCount( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    ItemManager* const item_manager = player->GetItemManager();
    SCSlotContainer* const inventory_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    const POSTYPE empty_slot_count = inventory_container->GetEmptySlotNum();

    lua_pushinteger(lua_state, empty_slot_count);

    return 1;
}

int ExtraNPCManager::CheckItemCount( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -2);
    SLOTCODE ItemTID = (SLOTCODE)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 2);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    ItemManager* const item_manager = player->GetItemManager();
    int item_count = item_manager->ExistItem(SI_INVENTORY, ItemTID, USHRT_MAX);

    lua_pushinteger(lua_state, item_count);

    return 1;
}

int ExtraNPCManager::RobItem( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -3);
    SLOTCODE ItemTID = (SLOTCODE)lua_tonumber(lua_state, -2);
    POSTYPE Count = (POSTYPE)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 3);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    MSG_CG_ITEM_LOSE_ACK msg_ack;
    ItemManager* const item_manager = player->GetItemManager();
    RC::eITEM_RESULT result = item_manager->DeleteAllKindOfItem( \
        1, &ItemTID, &Count, &msg_ack.m_ItemInfo);

    if (result == RC::RC_ITEM_SUCCESS) {
        player->SendPacket(&msg_ack, msg_ack.GetSize());
        GAMELOG->LogMultiItem( ITEM_SMARTNPC_ROB, player, 1, &ItemTID, &Count );
    }

    lua_pushinteger(lua_state, (result == RC::RC_ITEM_SUCCESS));

    return 1;
}

int ExtraNPCManager::GiveItem( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -3);
    SLOTCODE ItemTID = (SLOTCODE)lua_tonumber(lua_state, -2);
    POSTYPE Count = (POSTYPE)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 3);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL)
        return 0;

    MSG_CG_ITEM_PICK_ACK msg_ack;
    ItemManager* const item_manager = player->GetItemManager();
    RC::eITEM_RESULT result = item_manager->ObtainAllKindOfItem( \
        1, &ItemTID, &Count, &msg_ack.m_ItemInfo);
    
    if (result == RC::RC_ITEM_SUCCESS) {
        player->SendPacket(&msg_ack, msg_ack.GetSize());
        GAMELOG->LogMultiItem( ITEM_SMARTNPC_GIVE, player, 1, &ItemTID, &Count );
    }

    lua_pushinteger(lua_state, (result == RC::RC_ITEM_SUCCESS));

    return 1;
}

int ExtraNPCManager::ZoneChange(lua_State* lua_state)
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -2);
    ZONEKEY ZoneCode = (ZONEKEY)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 2);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    MSG_AG_ZONE_PORTAL_FIELD_MOVE_ACK ack;
    ack.m_wIndex = ZoneCode;
    player->SendPacket( &ack, sizeof(ack) );

    lua_pushinteger(lua_state, 1); // success

    return 1;
}

int ExtraNPCManager::CheckPotalToDominationField( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -2);
    ZONEKEY ZoneCode = (ZONEKEY)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 2);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    const RC::ePORTAL_RESULT retValue = player->CanPortal();
    if (RC::RC_PORTAL_CAN_SUCCESS != retValue)
    {
        lua_pushinteger(lua_state, -1); // fail
        return 1;
    }

    //점령전 맵으로 이동할 경우
    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    if (domination_manager->IsDominationArea(ZoneCode) == false ||
        domination_manager->CanEnterDominationField(player, ZoneCode) == false)
    {
        lua_pushinteger(lua_state, -2); // fail
        return 1;
    }

    lua_pushinteger(lua_state, 1); // success

    return 1;
}

int ExtraNPCManager::SendDominationMinimapList( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    MSG_CG_DOMINATION_MINIMAP_LIST_ACK sendMsg;
    sendMsg.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    sendMsg.m_NumDominationGuild = 
        domination_manager->GetMiniMapInfo(sendMsg.m_GuildInfo, _countof(sendMsg.m_GuildInfo));
    player->SendPacket(&sendMsg, sendMsg.GetSize());

    return 0;
}

int ExtraNPCManager::SetAttendance( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -6);
    INT32 NID = (INT32)lua_tonumber(lua_state, -5);
    INT32 EID = (INT32)lua_tonumber(lua_state, -4);
    INT32 STEP = (INT32)lua_tonumber(lua_state, -3);
    INT32 attendance_type = (INT32)lua_tonumber(lua_state, -2);
    INT32 attendance_data = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 6);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    switch(attendance_type)
    {
        case /*eAttendanceType::*/MONDSCHIEN_DOMINATION:
        {
            MSG_SMARTNPC_SET_ATTENDANCE_SYN msg(CID, NID, EID, STEP);
            msg.attendance_type = /*eAttendanceType::*/MONDSCHIEN_DOMINATION;
            msg.attendance_data = attendance_data;
            player->SendToGameDBPServer(&msg, sizeof(msg));
        } break;
    }

    return 0;
}

int ExtraNPCManager::IsDominationGoing( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -2);
    ZONEKEY MapCode = (ZONEKEY)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 2);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    GameDominationManager* const domination_manager = GameDominationManager::Instance();
    // IsDeleteMember 함수를 점령전 진행중인지 확인하는 용도로 활용
    bool is_going = !(domination_manager->IsDeleteMember(MapCode)); 

    lua_pushinteger(lua_state, is_going);

    return 1;
}

int ExtraNPCManager::GetAttendance( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -6);
    INT32 NID = (INT32)lua_tonumber(lua_state, -5);
    INT32 EID = (INT32)lua_tonumber(lua_state, -4);
    INT32 STEP = (INT32)lua_tonumber(lua_state, -3);
    INT32 attendance_type = (INT32)lua_tonumber(lua_state, -2);
    INT32 attendance_time = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 6);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    switch(attendance_type)
    {
        case /*eAttendanceType::*/MONDSCHIEN_DOMINATION:
        {
            MSG_SMARTNPC_GET_ATTENDANCE_SYN msg(CID, NID, EID, STEP);
            msg.attendance_type = /*eAttendanceType::*/MONDSCHIEN_DOMINATION;
            msg.attendance_time = attendance_time;
            player->SendToGameDBPServer(&msg, sizeof(msg));
        } break;
    }

    return 0;
}

#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
int ExtraNPCManager::GetDominationGuildGuid( lua_State* lua_state )
{
    ZONEKEY MapCode = (ZONEKEY)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    GameDominationManager* const domination_manager = GameDominationManager::Instance();

    const DOMINATION_INFO* domi_info = domination_manager->GetDominationInfo(MapCode);
    if (domi_info == NULL)
    {
        return 0;
    }

    lua_pushinteger(lua_state, domi_info->m_inGuildInfo.m_GuildGuid);

    return 1;
}

int ExtraNPCManager::GetCharacterGuildGuid( lua_State* lua_state )
{
    INT32 CID = (INT32)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

    lua_pushinteger(lua_state, player->GetGuildGuid());

    return 1;
}
#endif // _NA_000000_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

int ExtraNPCManager::IsDominationChannel( lua_State* lua_state )
{
    DWORD CID = (DWORD)lua_tonumber(lua_state, -1);

    lua_pop(lua_state, 1);

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(CID);
    if (player == NULL) {
        return 0;
    }

#ifdef _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO 
    const BYTE channel_id = GameOptionParser::Instance()->GetGameOption().m_byDominationChannel;
#else
    const BYTE channel_id = ServerOptionParserEx::Instance()->GetServerOption().m_byDominationChannel;
#endif
    if (channel_id != g_pGameServer->GetKey().GetChannelID()) 
    {
        lua_pushinteger(lua_state, 0); // fail
        return 1;
    }

    lua_pushinteger(lua_state, 1); // success
    return 1;
}

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
