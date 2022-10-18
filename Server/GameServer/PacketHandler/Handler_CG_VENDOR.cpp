#include "StdAfx.h"
#include ".\handler_cg_vendor.h"
#include ".\Player.h"
#include ".\PlayerManager.h"
#include ".\VendorManager.h"
#include ".\VendorPageManager.h"
#include "TradeSlotContainer.h"
#include "GameServerEx.h"
#include "VendorSlot.h"
#include <SCItemSlot.h>
#include <BitStream.hpp>

Handler_CG_VENDOR::Handler_CG_VENDOR(void)
{
}

Handler_CG_VENDOR::~Handler_CG_VENDOR(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_VENDOR::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_VENDOR::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_START_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_END_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_MODIFY_REQ_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_MODIFY_APPLY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_DELETE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_BUY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_MODIFY_TITLE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_VIEW_START_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_VIEW_END_SYN, PHASE_SERVICE),

        //상점 리스트 NPC
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_LIST_START_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_REQUIRE_PAGE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_SEARCH_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_LIST_END_SYN, PHASE_SERVICE),

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
        HANDLER_NODE_INFO(CG_VENDOR, CG_VENDOR_ENHANCED_SEARCH_SYN, PHASE_SERVICE),
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

BOOST_STATIC_ASSERT(RC::RC_VENDOR_RESULT_COUNTS <= UCHAR_MAX &&
                    RC::RC_VENDOR_LIST_RESULT_COUNTS <= UCHAR_MAX);
//------------------------------------------------------------------------------------------------
// 개설자
//------------------------------------------------------------------------------------------------

// (ADD)(20100324)(hukim)(STYLE) 플레이어가 유효한지 검사한다.
static bool ScopedCheckPlayer(const Player* const player)
{
    ASSERT(player != NULL);
    return (player != NULL) && (player->GetField() != NULL);
}

#if SUN_CODE_DELETE_RESERVED
// NOTE: f110531.3L, this check routine has many template warning problems.
// it will be removed to prevent blind effect.
// (ADD)(20100324)(hukim)(STYLE) 결과값을 에러코드에 설정한다.
template <typename RESULT_TYPE, typename ERROR_TYPE>
static bool ScopedSetResultToErrorCode(const RESULT_TYPE result, ERROR_TYPE& OUT error_code)
{
    // (NOTE)(20100324)(hukim)(STYLE) 잠재적 캐스팅 문제를 여기로 집중한다.
    // 컴파일러 경고는 유지한다. 오버 플로우시 에러 처리를 해주면 더 좋겠지만 기존 플로우를
    // 유지하기 위해 일부러 그대로 두었다.

    if (SAFE_NUMERIC_TYPECAST(RESULT_TYPE, result, ERROR_TYPE) == false)
    {
        ASSERT(false);
        // 나중에 오버 플로우 처리를 여기에 구현 하던지 아니면 명시적으로 캐스팅하자.
        //return false;
    }
    error_code = result;
    return true;
}
// (NOTE)(20100324)(hukim)(STYLE)
// 메시지 구조체는 크기를 일괄 int로 넘기는데 메시지 구조체를 수정하는것이 바람직하긴 하지만
// 여파가 커서 차선책으로 중간층을 둔다.
template <typename MSG_SIZE_TYPE, typename REAL_MSG_SIZE_TYPE>
static REAL_MSG_SIZE_TYPE ScopedGetMessageSize(const MSG_SIZE_TYPE msg_size, const REAL_MSG_SIZE_TYPE type_hint)
{
    // (NOTE)(20100324)(hukim)(STYLE) 잠재적 캐스팅 문제를 여기로 집중한다.
    // 컴파일러 경고는 유지한다. 오버 플로우시 에러 처리를 해주면 더 좋겠지만 기존 플로우를
    // 유지하기 위해 일부러 그대로 두었다.

    __UNUSED(type_hint);

    if (SAFE_NUMERIC_TYPECAST(MSG_SIZE_TYPE, msg_size, REAL_MSG_SIZE_TYPE) == false)
    {
        // 나중에 오버 플로우 처리를 여기에 구현 하던지 아니면 명시적으로 캐스팅하자.
        ASSERT(false);
    }
    return msg_size;
}
#endif

Handler_CG_VENDOR_IMPL(CG_VENDOR_START_SYN)
{
    __UNUSED(pServerSession);

    const MSG_CG_VENDOR_START_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_START_SYN*>(pMsg);
    if (receive_msg->GetSize() != wSize)
    {
        MessageOut(eDEV_LOG, _T("[CHKSZ] CG_VENDOR_START_SYN"));
        return;
    }

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }

    const ProtectorInventory& inventory_protector = 
        player->GetWarehouseInventoryProtector().INVENTORY();
    if (inventory_protector.IsUsable() == FALSE)
    {
        MSG_CG_VENDOR_START_NAK nak_msg;
        nak_msg.m_byErrorCode = RC::RC_VENDOR_INVENTORY_NEED_PASSWORD;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }
    util::StrArrayForVendorTitle title;
    _tcsncpy(title, receive_msg->m_pszTitle, _countof(title));
    title[_countof(title) - 1] = _T('\0');
    //util::cSTRING_REF title(receive_msg->m_pszTitle, sizeof(TCHAR)*MAX_VENDOR_TITLE_LENGTH);
    const RC::eVENDOR_RESULT start_vendor_result = 
        g_VendorManager.StartVendor(player, title, receive_msg->m_ItemInfo);
    if (start_vendor_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_START_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));

        MSG_CG_VENDOR_START_BRD brd_msg;
        brd_msg.m_dwPlayerKey = player->GetObjectKey();
        strncpy(brd_msg.m_pszTitle, receive_msg->m_pszTitle, _countof(brd_msg.m_pszTitle));
        brd_msg.m_pszTitle[_countof(brd_msg.m_pszTitle) - 1] = '\0';
        player->SendPacketAround(&brd_msg, brd_msg.GetSize(), FALSE);

#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
        // 개인상점 시작 로그
        GAMELOG->LogItem(ITEM_VENDOR_START, player, NULL);
#endif //_NA_000000_20131031_GAMELOG_REDUCE

        // 개인상점 아이템 정보
        VendorSlotContainer* vendor_container = player->GetVendorContainer();
        for (POSTYPE pos = 0; pos < vendor_container->GetMaxSlotNum(); ++pos)
        {
            if (vendor_container->IsEmpty(pos) == FALSE)
            {
                // (CHANGE)(20100324)(hukim)(STYLE) item_money 타입 수정
                // MONEY 타입을 DWORD로 캐스팅했다가 다시 MONEY 타입으로 캐스팅해서 로그에 기록한다.
                // DWORD 타입 최대값을 초과하는 경우 로그에 정상적으로 기록되지 않을 것이다.

                const VendorSlot& vendor_slot = 
                    static_cast<const VendorSlot&>(vendor_container->GetSlot(pos));
                const MONEY item_money = vendor_slot.GetMoney();
                const DURATYPE item_number = vendor_slot.GetNum();
                const SCItemSlot* const item = vendor_slot.GetLinkItem();
                if (item == NULL)
                {
                    continue;
                }                    
#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
                GAMELOG->LogItem(ITEM_VENDOR_PRICE, player, item, item_money, item_number);
#endif //_NA_000000_20131031_GAMELOG_REDUCE
            }
        }
        //SCSlotContainer* vendor_container = static_cast<SCSlotContainer*>(player->GetVendorContainer());
        //for (POSTYPE pos = 0; pos < vendor_container->GetMaxSlotNum(); ++pos)
        //{
        //    if (vendor_container->IsEmpty(pos) == FALSE)
        //    {
        //        const VendorSlot& vendor_slot = 
        //            static_cast<const VendorSlot&>(vendor_container->GetSlot(pos));
        //        const DWORD item_money = static_cast<DWORD>(vendor_slot.GetMoney());
        //        const DURATYPE item_number = vendor_slot.GetNum();
        //        const SCItemSlot* const item = vendor_slot.GetLinkItem();
        //        if (item == NULL)
        //        {
        //            continue;
        //        }                    
        //        GAMELOG->LogItem(ITEM_VENDOR_PRICE, player, item, item_money, item_number);
        //    }
        //}
        // 개인상점 아이템 정보 끝
    }
    else
    {
        MSG_CG_VENDOR_START_NAK nak_msg;
        nak_msg.m_byErrorCode = start_vendor_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_END_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_END_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_END_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_RESULT end_vendor_result = g_VendorManager.EndVendor(player);
    if (end_vendor_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_END_ACK ack_msg;
        ack_msg.m_byEndKind = receive_msg->m_byEndKind;             //< 0이면 종료, 1이면 PAUSE
        player->SendPacket(&ack_msg, sizeof(ack_msg));

        MSG_CG_VENDOR_END_BRD brd_msg;
        brd_msg.m_dwPlayerKey = player->GetObjectKey();
        player->SendPacketAround(&brd_msg, sizeof(brd_msg), FALSE);

#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
        //개인상점 끝 로그
        GAMELOG->LogItem(ITEM_VENDOR_END, player, NULL);
#endif //_NA_000000_20131031_GAMELOG_REDUCE
    }
    else
    {
        MSG_CG_VENDOR_END_NAK nak_msg;
        nak_msg.m_byErrorCode = end_vendor_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_MODIFY_REQ_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_MODIFY_REQ_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_MODIFY_REQ_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_RESULT modify_request_result = 
        g_VendorManager.ModifyRequest(player, receive_msg->m_VendorPos);
    if (modify_request_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_MODIFY_REQ_ACK ack_msg;
        ack_msg.m_VendorPos = receive_msg->m_VendorPos;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    #if SUN_CODE_BACKUP
        // NOTE: f110531.3L, client does not control.
        MSG_CG_VENDOR_MODIFY_REQ_BRD msg_brd;
        msg_brd.m_VendorPos = receive_msg->m_VendorPos;
        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, sizeof(msg_brd));
        };
    #endif
    }
    else
    {
        MSG_CG_VENDOR_MODIFY_REQ_NAK nak_msg;
        nak_msg.m_byErrorCode = modify_request_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_MODIFY_CANCEL_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_MODIFY_CANCEL_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_MODIFY_CANCEL_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_RESULT modify_cancel_result = 
        g_VendorManager.ModifyCancel(player, receive_msg->m_VendorPos);
    if (modify_cancel_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_MODIFY_CANCEL_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    #if SUN_CODE_BACKUP
        // NOTE: f110531.3L, client does not control.
        MSG_CG_VENDOR_MODIFY_CANCEL_BRD msg_brd;
        msg_brd.m_VendorPos = receive_msg->m_VendorPos;
        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, sizeof(msg_brd));
        };
    #endif
    }
    else
    {
        MSG_CG_VENDOR_MODIFY_CANCEL_NAK nak_msg;
        nak_msg.m_byErrorCode = modify_cancel_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_MODIFY_APPLY_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_MODIFY_APPLY_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_MODIFY_APPLY_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    MSG_CG_VENDOR_MODIFY_APPLY_BRD msg_brd;
    const RC::eVENDOR_RESULT modify_apply_result = 
        g_VendorManager.ModifyApply(player, receive_msg->m_ItemSlot, msg_brd.m_ItemSlot);
    if (modify_apply_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_MODIFY_APPLY_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    #if SUN_CODE_BACKUP
        // NOTE: f110531.3L, client does not control.
        msg_brd.m_VendorPos = receive_msg->m_ItemSlot.m_VendorPos;
        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, sizeof(msg_brd));
        };
    #endif
    }
    else
    {
        MSG_CG_VENDOR_MODIFY_APPLY_NAK nak_msg;
        nak_msg.m_byErrorCode = modify_apply_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_INSERT_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_INSERT_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_INSERT_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }

    MSG_CG_VENDOR_INSERT_BRD msg_brd;
    const RC::eVENDOR_RESULT insert_result = 
        g_VendorManager.Insert(player, receive_msg->m_ItemSlot, msg_brd.m_ItemSlot);
    if (insert_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_INSERT_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));

        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, sizeof(msg_brd));
        };
    }
    else
    {
        MSG_CG_VENDOR_INSERT_NAK nak_msg;
        nak_msg.m_byErrorCode = insert_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_DELETE_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_DELETE_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_DELETE_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_RESULT delete_result = g_VendorManager.Delete(player, receive_msg->m_VendorPos);
    if (delete_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_DELETE_ACK ack_msg;
        ack_msg.m_VendorPos = receive_msg->m_VendorPos;
        player->SendPacket(&ack_msg, sizeof(ack_msg));

        MSG_CG_VENDOR_DELETE_BRD msg_brd;
        msg_brd.m_VendorPos = receive_msg->m_VendorPos;
        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, sizeof(msg_brd));
        };
    }
    else
    {
        MSG_CG_VENDOR_DELETE_NAK nak_msg;
        nak_msg.m_byErrorCode = delete_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_MODIFY_TITLE_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_MODIFY_TITLE_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_MODIFY_TITLE_SYN*>(pMsg);
    
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    util::StrArrayForVendorTitle title;
    _tcsncpy(title, receive_msg->m_pszTitle, _countof(title));
    title[_countof(title) - 1] = _T('\0');

    const RC::eVENDOR_RESULT modify_title_result = g_VendorManager.ModifyTitle(player, title);
    if (modify_title_result == RC::RC_VENDOR_SUCCESS)
    {
        MSG_CG_VENDOR_MODIFY_TITLE_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    #if SUN_CODE_BACKUP
        // NOTE: f110531.3L, client does not control.
        MSG_CG_VENDOR_MODIFY_TITLE_BRD msg_brd;
        msg_brd.m_dwPlayerKey = player->GetObjectKey();
        strncpy(msg_brd.m_pszTitle, receive_msg->m_pszTitle, _countof(msg_brd.m_pszTitle));
        msg_brd.m_pszTitle[_countof(msg_brd.m_pszTitle) - 1] = '\0';
        if (VendorSlotContainer* vendor_container = player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(player, &msg_brd, msg_brd.GetSize());
        };
    #endif
    }
    else
    {
        MSG_CG_VENDOR_MODIFY_TITLE_NAK nak_msg;
        nak_msg.m_byErrorCode = modify_title_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

//------------------------------------------------------------------------------------------------
// 관람자
//------------------------------------------------------------------------------------------------
Handler_CG_VENDOR_IMPL(CG_VENDOR_VIEW_START_SYN)
{
    __UNUSED((pServerSession, pMsg, wSize));
    
    const MSG_CG_VENDOR_VIEW_START_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_VIEW_START_SYN*>(pMsg);

    Player* const observer_player = 
        PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(observer_player) == false)
    {
        return;
    }
    
    Player* const establisher_player = 
        PlayerManager::Instance()->FindPlayerByObjKey(receive_msg->m_dwPlayerKey);
    if (ScopedCheckPlayer(establisher_player) == false)
    {
        MSG_CG_VENDOR_VIEW_START_NAK nak_msg;
        nak_msg.m_byErrorCode = 111;
        observer_player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }    

    const RC::eVENDOR_RESULT start_view_vendor_result = 
        g_VendorManager.StartViewVendor(establisher_player, observer_player);
    if (start_view_vendor_result == RC::RC_VENDOR_SUCCESS)
    {
        VendorSlotContainer* const vendor_container = observer_player->GetVendorContainer();
        
        MSG_CG_VENDOR_VIEW_START_ACK ack_msg;
        memcpy(ack_msg.m_pszTitle, vendor_container->GetTitle(), MAX_VENDOR_TITLE_LENGTH*sizeof(TCHAR));

        // (BUGFIX) (091116) (WAVERIX) 1st modified - size
        ack_msg.m_Size = sizeof(ack_msg.m_BitStreamBuffer);// sizeof(RENDER_ITEMSTREAMEX)*(MAX_PERSONAL_VENDOR_SLOT_NUM);
        BitStream stream(ack_msg.m_BitStreamBuffer, ack_msg.m_Size, TRUE);
        vendor_container->Serialize(stream, eSLOT_SERIALIZE_CLIENT_LOAD);
        ack_msg.m_Size = static_cast<uint16_t>(stream.GetNumberOfBytesUsed());

        observer_player->SendPacket(&ack_msg, ack_msg.GetSize());

#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
        //개인상점 관람 로그
        GAMELOG->LogItem(ITEM_VENDOR_VIEW, observer_player, NULL, 0, 0, 
            establisher_player->GetUserID(), establisher_player->GetCharName());
        //개인상점 관람 로그 끝
#endif //_NA_000000_20131031_GAMELOG_REDUCE
    }
    else
    {
        MSG_CG_VENDOR_VIEW_START_NAK nak_msg;
        nak_msg.m_byErrorCode = start_view_vendor_result;
        observer_player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_VIEW_END_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_VIEW_END_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_VIEW_END_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_RESULT end_view_vendor_result = g_VendorManager.EndViewVendor(player);
    if (end_view_vendor_result == RC::RC_VENDOR_SUCCESS)
    {       
#if !defined(_NA_000000_20131031_GAMELOG_REDUCE)
        GAMELOG->LogItem(ITEM_VENDOR_VIEW_END, player, NULL);
#endif //_NA_000000_20131031_GAMELOG_REDUCE

        MSG_CG_VENDOR_VIEW_END_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    }
    else
    {
        MSG_CG_VENDOR_VIEW_END_NAK nak_msg;
        nak_msg.m_byErrorCode = end_view_vendor_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_BUY_SYN)
{
    __UNUSED((pServerSession, wSize));
    
    const MSG_CG_VENDOR_BUY_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_BUY_SYN*>(pMsg);

    Player* observer_player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(observer_player) == false)
    {
        return;
    }

    const ProtectorInventory& inventory_protector = 
        observer_player->GetWarehouseInventoryProtector().INVENTORY();
    if (inventory_protector.IsUsable() == FALSE)
    {
        MSG_CG_VENDOR_START_NAK nak_msg;
        nak_msg.m_byErrorCode = RC::RC_VENDOR_INVENTORY_NEED_PASSWORD;
        observer_player->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    MSG_CG_VENDOR_BUY_ACK ack_msg;
    const RC::eVENDOR_RESULT buy_result = g_VendorManager.Buy(observer_player, 
        receive_msg->m_VendorPos, receive_msg->m_BuyNum, ack_msg.m_InventoryTotalInfo);
    if (buy_result == RC::RC_VENDOR_SUCCESS)
    {
        // 유저
        ack_msg.m_Money = observer_player->GetMoney();
        observer_player->SendPacket(&ack_msg, ack_msg.GetSize());

        // 상인
        Player* const establisher_player = observer_player->GetVendorContainer()->GetEstablisher();
        if (establisher_player != NULL)
        {
            MSG_CG_VENDOR_BUY_CMD cmd_msg;
            cmd_msg.m_VendorPos = receive_msg->m_VendorPos;
            cmd_msg.m_Money = establisher_player->GetMoney();
            cmd_msg.m_SellNum = receive_msg->m_BuyNum;
            establisher_player->SendPacket(&cmd_msg, sizeof(cmd_msg));
            establisher_player->SerializeInfoToDBProxy();
        }
        
        MSG_CG_VENDOR_BUY_BRD brd_msg;
        brd_msg.m_VendorPos = receive_msg->m_VendorPos;
        brd_msg.m_SellNum = receive_msg->m_BuyNum;
        if (VendorSlotContainer* vendor_container = observer_player->GetVendorContainer()) {
            vendor_container->SendPacketToObservers(observer_player, &brd_msg, sizeof(brd_msg));
        };
        // 정보저장
        observer_player->SerializeInfoToDBProxy();
    }
    else
    {
        MSG_CG_VENDOR_BUY_NAK nak_msg;
        nak_msg.m_byErrorCode = buy_result;
        observer_player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

//------------------------------------------------------------------------------------------------
// 상점 리스트
//------------------------------------------------------------------------------------------------
Handler_CG_VENDOR_IMPL(CG_VENDOR_LIST_START_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_LIST_START_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_LIST_START_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_LIST_RESULT start_vendor_list_result = 
        g_VendorPageManager.StartVendorList(player);
    if (start_vendor_list_result == RC::RC_VENDOR_LIST_SUCCESS)
    {
        MSG_CG_VENDOR_LIST_START_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));      
    }
    else
    {
        MSG_CG_VENDOR_LIST_START_NAK nak_msg;
        nak_msg.m_byErrorCode = start_vendor_list_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_REQUIRE_PAGE_SYN)
{
    __UNUSED((pServerSession, wSize));
    
    const MSG_CG_VENDOR_REQUIRE_PAGE_SYN* const receive_msg = 
        static_cast<MSG_CG_VENDOR_REQUIRE_PAGE_SYN*>(pMsg);
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
    const RC::eVENDOR_LIST_RESULT require_page_result = 
        g_VendorPageManager.RequirePage(player, 
                                        static_cast<eVENDOR_LIST_PAGE_KIND>(receive_msg->m_byKind), 
                                        receive_msg->m_byPage);
    if (require_page_result == RC::RC_VENDOR_LIST_SUCCESS)
    {
        VENDOR_LIST_TOTAL_INFO vendor_list_info;

        const VendorPage* const vendor_page = player->GetVendorPage();
        vendor_page->GetVendorListTotalInfo(vendor_list_info);
        
        MSG_CG_VENDOR_REQUIRE_PAGE_ACK ack_msg;
        player->SendPacket(&ack_msg, ack_msg.ConvertforSend(vendor_list_info));
    }
    else
    {
        MSG_CG_VENDOR_REQUIRE_PAGE_NAK nak_msg;
        nak_msg.m_byErrorCode = require_page_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_SEARCH_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_SEARCH_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_SEARCH_SYN*>(pMsg);
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (ScopedCheckPlayer(player) == false)
    {
        return;
    }
    
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    const RC::eVENDOR_LIST_RESULT search_result = 
        g_VendorPageManager.Search(player, 
                                   static_cast<eVENDOR_LIST_PAGE_KIND>(receive_msg->m_ebyKind), 
                                   receive_msg->m_sPattern, 
                                   receive_msg->m_ItemCode);
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    const RC::eVENDOR_LIST_RESULT search_result = 
        g_VendorPageManager.Search(player, 
                                   static_cast<eVENDOR_LIST_PAGE_KIND>(receive_msg->m_ebyKind), 
                                   receive_msg->m_sPattern);
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
    if (search_result == RC::RC_VENDOR_LIST_SUCCESS)
    {
        MSG_CG_VENDOR_SEARCH_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    }
    else
    {
        MSG_CG_VENDOR_SEARCH_NAK nak_msg;
        nak_msg.m_byErrorCode = search_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

Handler_CG_VENDOR_IMPL(CG_VENDOR_LIST_END_SYN)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_CG_VENDOR_LIST_END_SYN* const receive_msg = static_cast<MSG_CG_VENDOR_LIST_END_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    ASSERT(player != NULL);
    if (player == NULL)
    {
        return;
    }
    // 클라이언트의 캐릭터가 필드에서 나간 후 상점 종료 패킷을 보내므로 체크 하지 않음.
    //if (!player->GetField()) return;  

    const RC::eVENDOR_LIST_RESULT end_vendor_list_result = g_VendorPageManager.EndVendorList(player);
    if (end_vendor_list_result == RC::RC_VENDOR_LIST_SUCCESS)
    {
        MSG_CG_VENDOR_LIST_END_ACK ack_msg;
        player->SendPacket(&ack_msg, sizeof(ack_msg));
    }
    else
    {
        MSG_CG_VENDOR_LIST_END_NAK nak_msg;
        nak_msg.m_byErrorCode = end_vendor_list_result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
Handler_CG_VENDOR_IMPL(CG_VENDOR_ENHANCED_SEARCH_SYN)
{
    const MSG_CG_VENDOR_ENHANCED_SEARCH_SYN* const receive_msg = \
        static_cast<MSG_CG_VENDOR_ENHANCED_SEARCH_SYN*>(pMsg);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(receive_msg->m_dwKey);
    if (player == NULL) {
        return;
    }

    g_VendorManager.vendor_search_find(player, receive_msg->search_items, receive_msg->page);
}
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
 