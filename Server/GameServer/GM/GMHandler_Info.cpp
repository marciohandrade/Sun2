#include "stdafx.h"
#include "GMCmdManager.h"
//
//

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

//어카운트
static bool ProcessAccount(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //인자 조건이 맞지 않습니다.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    BOOL bAccountOn = false;
    eGM_STRING type = gm_manager->GetGMOptionType(vecCmd[1].c_str());
    if (type == GMSTR_TURNON)            bAccountOn = true;
    else if (type == GMSTR_TURNOFF)  bAccountOn = false;
    else 
        return false;

    // ack
    MSG_CG_GM_ACCOUNT_MODE_ACK AckMsg;
    AckMsg.m_bTurnOn = bAccountOn;
    pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));

    GAMELOG->WriteAccount(pPlayer);

    return true;
}

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterInfo(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "어카운트", "account", &ProcessAccount), // 어카운트
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}

