#include "stdafx.h"
#include "GMCmdManager.h"
//
//
#include "PacketHandler/PacketHandler.h"

#include "GameZone.h"

#include "ItemManager.h"

// Internal utilities
#include "./GMHandler_InterUtils.hxx"

namespace nsGM {
;

//��������
static bool ProcessDisConnect(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)      //���� ������ ���� �ʽ��ϴ�.
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    MSG_AG_GM_FORCE_DISCONNECT_CMD cmd;
    _tcsncpy(cmd.m_szCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);
    cmd.m_szCharName[MAX_CHARNAME_LENGTH]='\0';
    pPlayer->SendPacket(&cmd, sizeof(cmd));

    return true;
}


//ĳ���� ����
static bool ProcessCharInfo(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    if (vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode    =  RC::RC_GM_INVALID_ARGUMENT; // ���ڰ� ���� �ʽ��ϴ�.
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }   

    MSG_AG_GM_CHAR_INFO_CMD CmdMsg;
    _tcsncpy(CmdMsg.m_szCharName, vecCmd[1].c_str(), _countof(CmdMsg.m_szCharName));
    CmdMsg.m_szCharName[_countof(CmdMsg.m_szCharName) - 1] = _T('\0');
    pPlayer->SendPacket(&CmdMsg, sizeof(CmdMsg));
    return true;
}


static bool ProcessUserSummon(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    if (vecCmd.size() != 2)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_INVALID_ARGUMENT;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false; 
    }

    GameZone *pZone = pPlayer->GetGameZonePtr();
    if (!pZone)      
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_PLAYER_NOTEXIST_TO_ZONE;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

    GameField *pField = pPlayer->GetField();
    if (!pField)
    {
        MSG_CG_GM_STRING_CMD_NAK NakMsg;
        NakMsg.m_byErrorCode = RC::RC_GM_PLAYER_NOTEXIST_TO_FIELD;
        pPlayer->SendPacket(&NakMsg, sizeof(NakMsg));
        return false;
    }

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if(pPlayer->ReCallPlayer(vecCmd[1].c_str(), pZone->GetKey(), pField->GetFieldCode(), 
                             pPlayer->GetRandomPosInAround(3), ePORTAL_TYPE_GM, 
                             INVALID_POSTYPE_VALUE, 0))
    {
        SendGMCmdAck(pPlayer, vecCmd[0].c_str());        
    }    
#else
    // GM�� ��ġ������ ��Ƽ� ��ȯ����� �̵���Ű���� ����Ѵ�.
    MSG_AG_GM_GO_CMD AGMsg;
    _tcsncpy(AGMsg.m_szCharName, vecCmd[1].c_str(), MAX_CHARNAME_LENGTH);
    AGMsg.m_szCharName[MAX_CHARNAME_LENGTH] = '\0';
    AGMsg.m_RoomKey = pZone->GetKey();
    AGMsg.m_FieldCode = pField->GetFieldCode();
    AGMsg.m_vDestPos = pPlayer->GetRandomPosInAround(3);
    pPlayer->GetPos(&AGMsg.m_vCurPos);
    AGMsg.m_PortalType = (BYTE)ePORTAL_TYPE_GM;
    AGMsg.m_atItemPos = INVALID_POSTYPE_VALUE;
    AGMsg.m_atItemCode = 0;
    pPlayer->SendPacket(&AGMsg, sizeof(AGMsg));
    // GM ��� ����
    SendGMCmdAck(pPlayer, vecCmd[0].c_str());
#endif

    

    return true;
}

// ���̻�������
static bool ProcessClearWayDelTimeCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size(); 
    CHKSendErr(1 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    ItemManager* pItemManager = pPlayer->GetItemManager(); 
    CHKSendErr(!pItemManager, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

    SCSlotContainer* pContainer = (SCSlotContainer *)pItemManager->GetItemSlotContainer(SI_INVENTORY);
    CHKSendErr(!pContainer, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

    int pos = 0;
    for (; pos < MAX_INVENTORY_SLOT_NUM; ++pos)
    {
        if (pContainer->IsEmpty(pos))
            continue;

        SCItemSlot &rItemSlot = (SCItemSlot &)pContainer->GetSlot(pos);

        if (rItemSlot.GetSerial() != 0 && 
            rItemSlot.IsWayPointItem())
        {
            rItemSlot.ClearWayPointItem();
            pPlayer->GetItemManager()->SendItemChange(rItemSlot);
            break;
        }
    }

    CHKSendErr(pos == MAX_INVENTORY_SLOT_NUM, MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_CMD_FAILED, pPlayer);

    return true;
}

// ���ο��� [�ε�,����,����,����] [DB_idx]
static bool ProcessWayPointCmd(DWORD player_key, const GmCmdTokens& vecCmd)
{
    GMCmdManager* const gm_manager = GMCmdManager::Instance();
    Player* pPlayer = gm_manager->GetGMPlayer(player_key);
    if (!pPlayer)
        return false;

    //���ڰ��� üũ
    INT nArgSize = (INT)vecCmd.size();
    CHKSendErr(2 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

    //.//

    MSG_BASE* pMsg  = NULL;
    WORD      wSize = 0;

    if (strcmp(vecCmd[1].c_str(), "�ε�") == 0)
    {
        MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN* pTempMsg = new MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN;

        pMsg  = pTempMsg;
        wSize = sizeof(MSG_CG_CHAR_WAYPOINT_INFO_LOAD_SYN);
    }
    else if (strcmp(vecCmd[1].c_str(), "����") == 0)
    {
        MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN* pTempMsg =  new MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN;

        pMsg  = pTempMsg;
        wSize = sizeof(MSG_CG_CHAR_WAYPOINT_INFO_SAVE_SYN);
    }
    else if (strcmp(vecCmd[1].c_str(), "����") == 0)
    {
        CHKSendErr(3 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

        MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN* pTempMsg =  new MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN;
        pTempMsg->m_IdxWayPoint = atoi(vecCmd[2].c_str());
        pMsg  = pTempMsg;
        wSize = sizeof(MSG_CG_CHAR_WAYPOINT_INFO_DEL_SYN);
    }
    else if (strcmp(vecCmd[1].c_str(), "����") == 0)
    {
        CHKSendErr(3 > nArgSize , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);

        MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD* pTempMsg = new MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD;
        pTempMsg->m_IdxWayPoint = atoi(vecCmd[2].c_str());
        pMsg  = pTempMsg;
        wSize = sizeof(MSG_CG_CHAR_WAYPOINT_INFO_EXE_CMD);
    }
    else
    {
        CHKSendErr(true , MSG_CG_GM_STRING_CMD_NAK, RC::RC_GM_INVALID_ARGUMENT, pPlayer);
    }

    //.//

    pMsg->m_dwKey = player_key;
    PacketHandler::Instance()->ParsePacket(PACKET_HANDLER_AGENT, pPlayer->GetServerSession(), (MSG_BASE *)pMsg, wSize);
    delete pMsg;

    return true;
}

}; //end of namespace 'nsGM'


bool GMCmdManager::RegisterPlayerControl(const HandlerNode** node_array, size_t* result_array_count)
{
    using namespace nsGM;
    bool success = false;

#define GMHANDLER_MAPPING(activated, korean, english, handler) \
    { activated, eGM_GRADE_MAX, 0xFFFFFFFF, 0xFFFFFFFF, handler, korean, english }

    static HandlerNode const_node_array[] =
    {
        GMHANDLER_MAPPING(true, "��������", "kick",  &ProcessDisConnect), // ��������
        GMHANDLER_MAPPING(true, "����", "info",  &ProcessCharInfo), // ĳ���� ���� ��û
        GMHANDLER_MAPPING(true, "��ȯ", "call",  &ProcessUserSummon),
        // unused
        GMHANDLER_MAPPING(false, "TODOOO", "TODOOO",  &ProcessClearWayDelTimeCmd),
        // unused
        GMHANDLER_MAPPING(false, "TODOOO", "TODOOO",  &ProcessWayPointCmd),
    }; //

    ModifyGmHandlerNode(const_node_array, _countof(const_node_array));
    *node_array = const_node_array;
    *result_array_count = _countof(const_node_array);
    success = true;
    //
    return success;
}
