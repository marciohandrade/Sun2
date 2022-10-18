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

/// @brief 아이템 합성 또는 분해 관련 기능을 처리하는 클래스
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
    // 합성 또는 분해 내역 삽입 요청을 처리한다.
    void HandleInsertHistory(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    void HandleInsertHistoryDBR(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // 합성 또는 분해 내역들 얻어오기 요청을 처리한다.
    void HandleSelectHistories(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    void HandleSelectHistoriesDBR(
        ChildServerSession* const server_session, 
        const MSG_BASE* const message, 
        const MessageSize message_size) const;

    // 합성 또는 분해 내역들 수정 요청을 처리한다.
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