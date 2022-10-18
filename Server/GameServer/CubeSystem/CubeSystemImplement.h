////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   CubeSystemImplement.h
/// @author hukim
/// @brief  [2010/08/25 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SERVER_GAMESERVER_CUBESYSTEM_CUBESYSTEMIMPLEMENT_H_
#define SERVER_GAMESERVER_CUBESYSTEM_CUBESYSTEMIMPLEMENT_H_
#ifdef _NA_002253_20100811_CUBE_SYSTEM

namespace util {
namespace internal {
class ItemCompositionOrDecompositionReward;
} // internal
} // util

// ť�� �ý����� ������ Ŭ����
class CubeSystemImplement
{
    typedef WORD MessageSize;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Constructor & Destructor
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    explicit CubeSystemImplement();
    ~CubeSystemImplement();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Public
    ////////////////////////////////////////////////////////////////////////////////////////////////

public:
    // ť���� �������� �ռ� �Ǵ� �����Ѵ�.
    RC::eITEM_RESULT CubeComposeOrDecompose(
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
        Player& INOUT requester, 
        util::internal::ItemCompositionOrDecompositionReward& OUT reward) const;


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // REGION : Private
    ////////////////////////////////////////////////////////////////////////////////////////////////

private:
    __DISABLE_COPY(CubeSystemImplement);
}; // CubeSystemImplement

#endif // _NA_002253_20100811_CUBE_SYSTEM
#endif //}} SERVER_GAMESERVER_CUBESYSTEM_CUBESYSTEMIMPLEMENT_H_