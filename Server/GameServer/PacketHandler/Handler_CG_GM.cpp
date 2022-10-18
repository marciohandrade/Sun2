#include "StdAfx.h"
#include ".\handler_cg_gm.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include "GameField.h"
#include "Player.h"
#include "PlayerManager.h"
#include <ServerSession.h>
#include "DropManager.h"
#include "GameServerEx.h"
#include "Map.h"
#include "Item.h"
#include "Define.h"
#include "ObjectFactory.h"
#include <ItemInfoParser.h>
#include "GameZone.h"
#include "GameZoneManager.h"
#include "GameRoom.h"
#include "StatusManager.h"
#include "GMCmdManager.h"

Handler_CG_GM::Handler_CG_GM()
{

}

Handler_CG_GM::~Handler_CG_GM()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_GM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_GM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_GM, CG_GM_STRING_CMD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


// 스트링 명령어 처리
Handler_CG_GM_IMPL(CG_GM_STRING_CMD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    // f101210.2L
    const MSG_CG_GM_STRING_CMD_SYN* recv_msg = static_cast<const MSG_CG_GM_STRING_CMD_SYN*>(pMsg);
    // with null-terminated string
    TCHAR copied_buffer[_countof(recv_msg->m_szStringCmd)]; // MAX_STRING_CMD_LENGTH + 1
    _tcsncpy(copied_buffer, recv_msg->m_szStringCmd, _countof(copied_buffer));
    copied_buffer[_countof(copied_buffer) - 1] = _T('\0');
    //
    GMCmdManager::Instance()->ParseCommand(recv_msg->m_dwKey, copied_buffer);
}

