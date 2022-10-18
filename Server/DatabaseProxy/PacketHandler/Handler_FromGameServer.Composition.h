////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CompositionOrDecompositionHandler.h
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2010/11/29 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef DBPROXY_HANDLER_FROM_GAMESERVER_COMPOSITION_H
#define DBPROXY_HANDLER_FROM_GAMESERVER_COMPOSITION_H

/// @brief ������ �ռ� �Ǵ� ���� ���� ����� ó���ϴ� Ŭ����
class CompositionOrDecompositionHandler
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Define
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    typedef WORD MessageSize;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    explicit CompositionOrDecompositionHandler();
    ~CompositionOrDecompositionHandler();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    // �ռ� �Ǵ� ���� ���� ���� ��û�� ó���Ѵ�.
    void HandleInsertHistory(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    void HandleInsertHistoryDBR(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // �ռ� �Ǵ� ���� ������ ������ ��û�� ó���Ѵ�.
    void HandleSelectHistories(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    void HandleSelectHistoriesDBR(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // �ռ� �Ǵ� ���� ������ ���� ��û�� ó���Ѵ�.
    void HandleUpdateHistories(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    void HandleUpdateHistoriesDBR(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    __DISABLE_COPY(CompositionOrDecompositionHandler);
}; // CompositionOrDecompositionHandler

#endif // DBPROXY_HANDLER_FROM_GAMESERVER_COMPOSITION_H