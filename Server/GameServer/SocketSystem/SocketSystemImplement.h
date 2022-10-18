////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   SocketSystemImplement.h
/// @author hukim
/// @brief  [2010/10/06 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SERVER_GAMESERVER_SOCKETSYSTEM_SOCKETSYSTEMIMPLEMENT_H_
#define SERVER_GAMESERVER_SOCKETSYSTEM_SOCKETSYSTEMIMPLEMENT_H_

#include "ItemManager.h"

namespace util {
namespace internal {
class ItemCompositionOrDecompositionReward;
} // internal
} // util

// 소켓 시스템 관련 기능을 구현한 클래스
class SocketSystemImplement
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

    typedef WORD MessageSize;

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    explicit SocketSystemImplement();
    ~SocketSystemImplement();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    // 아이템의 소켓수를 확인 요청을 처리한다.
    void HandleIdentifySocketMessage(
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // 쟈드를 합성한다.
    RC::eITEM_RESULT ZardCompose(
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
        Player& OUT requester, 
        util::internal::ItemCompositionOrDecompositionReward& OUT reward);

    // 소켓 다시 생성 요청을 처리한다.
    void HandleRecreateSocket(
        const MSG_BASE* const message, 
        const MessageSize message_size) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
    RC::eITEM_RESULT _GetZardComposeResultIndex(Player& requester, SCItemSlotContainer* const inventory, 
                                                const POSTYPE* zard_position_list, SOCKETINDEX* OUT socket_index);   //쟈드합성의 결과물(쟈드 포인트옵션 인덱스)을 받는다.
    RC::eITEM_RESULT _GetZardItemCodeByIndex(Player& requester, SOCKETINDEX socket_index, CODETYPE* OUT zard_code);  //인덱스값으로 쟈드(아이템코드)를 찾는다
#endif //_NA_007973_20150120_ADD_MULTIOPTION_ZARD

    __DISABLE_COPY(SocketSystemImplement);
}; // SocketSystemImplement

#endif // SERVER_GAMESERVER_SOCKETSYSTEM_SOCKETSYSTEMIMPLEMENT_H_