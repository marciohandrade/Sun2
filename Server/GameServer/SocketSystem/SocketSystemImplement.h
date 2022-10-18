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

// ���� �ý��� ���� ����� ������ Ŭ����
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
    // �������� ���ϼ��� Ȯ�� ��û�� ó���Ѵ�.
    void HandleIdentifySocketMessage(
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // ��带 �ռ��Ѵ�.
    RC::eITEM_RESULT ZardCompose(
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
        Player& OUT requester, 
        util::internal::ItemCompositionOrDecompositionReward& OUT reward);

    // ���� �ٽ� ���� ��û�� ó���Ѵ�.
    void HandleRecreateSocket(
        const MSG_BASE* const message, 
        const MessageSize message_size) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
    RC::eITEM_RESULT _GetZardComposeResultIndex(Player& requester, SCItemSlotContainer* const inventory, 
                                                const POSTYPE* zard_position_list, SOCKETINDEX* OUT socket_index);   //����ռ��� �����(��� ����Ʈ�ɼ� �ε���)�� �޴´�.
    RC::eITEM_RESULT _GetZardItemCodeByIndex(Player& requester, SOCKETINDEX socket_index, CODETYPE* OUT zard_code);  //�ε��������� ���(�������ڵ�)�� ã�´�
#endif //_NA_007973_20150120_ADD_MULTIOPTION_ZARD

    __DISABLE_COPY(SocketSystemImplement);
}; // SocketSystemImplement

#endif // SERVER_GAMESERVER_SOCKETSYSTEM_SOCKETSYSTEMIMPLEMENT_H_