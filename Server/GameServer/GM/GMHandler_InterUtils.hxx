#pragma once
#ifndef GAMSERVER_GMHANDLER_INTERUTILS_HXX
#define GAMSERVER_GMHANDLER_INTERUTILS_HXX

#include "GMCmdTokenizer.hxx"

#define _SendErr(ErrType,ErrNum,pConnect) { ##ErrType NakMsg; NakMsg.m_byErrorCode = ErrNum; pConnect->SendPacket(&NakMsg, sizeof(NakMsg)); }
#define CHKSendErr(bErr,ErrType,ErrNum,pConnect) if (bErr) { _SendErr(ErrType,ErrNum,pConnect); return false; }

//==================================================================================================

// 구현 전용 지역 함수
namespace util { namespace internal {
;

// 입력값을 검사한다.
extern bool scoped_check_input_in_gm(Player* const gm, const GmCmdTokens& gm_cmd_arguments,
    const size_t num_of_min_gm_cmd_argument, const size_t num_of_max_gm_cmd_argument,
    const bool is_check_field);

// 문자열을 숫자 형식으로 변환한다.
template <typename NUM_TYPE>
void scoped_string_to_number(const GmCmdTokens::Token& number_string, NUM_TYPE& OUT number)
{
    // NOTE: prevent to accept various type to unify type control except for numerical 3 types
    BOOST_STATIC_ASSERT((boost::is_same<NUM_TYPE, uint8_t>::value ||
                         boost::is_same<NUM_TYPE, int>::value ||
                         boost::is_same<NUM_TYPE, ulong>::value));
    number = static_cast<NUM_TYPE>(_ttoi(number_string.c_str()));
}

}}; // namespace util

//==================================================================================================

namespace nsGM {
;

extern void SendGMCmdAck(Player* pPlayer, const char* source);
extern bool ModifyGmHandlerNode(GMCmdManager::HandlerNode* node_array, size_t number_of_nodes);
extern void SendGMCmdNak(Player* player, const BYTE error_code);

};

#endif //GAMSERVER_GMHANDLER_INTERUTILS_HXX