#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#ifndef __SSQ_MSG_HDR__
#define __SSQ_MSG_HDR__
// <Prototype of WAR_MSG_HDR>
//  전체 메시지의 길이가 담겨진다. (variable stream)
struct WAR_MSG_HDR
{
    typedef ulong MSG_SIZE_T; // expanded by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    MSG_SIZE_T size;
};

//	<Raw Header of Each Message>
//	-	각 메시지의 타입들이 기재된다.
//	-	1Byte, 하지만 raw stream인 WAR_TAG_V의 이것을 직접 사용하는 경우는 없어야 한다.
//		(이것 common routine에서의 offset연산 편의성이 목적이다.)
//		모든 메시지들은 실질적으로 WAR_TAG1_<id, type>을 상속해서 구현되게 되어 있다.
//	<목적>
//	-	Server/Client 각각 pointer연산을 수행하면서 버그를 유발할 수 있다.
//		하나의 로직으로 serializing input/output이 가능해지도록 하는 것이 목표다.
typedef struct WAR_TAG_V
{
	BYTE				_TAG;
} *WAR_TAG_P;
#endif


const WORD			MAX_WAR_MSG_SEQUENCIAL_STREAM_SIZE = 8000;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#define WAR_MSG_DECL( protocol )	\
	struct MSGSUB_##protocol : WAR_TAG1_< protocol, MSGSUB_##protocol >

//────────────────────────────────────────────────────────────────
//	<Sample>
//	case SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE:
//	{
//		RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION );
//		// enable keyword 'rMsg'
//		rMsg.Count ~~
#define RECV_PATTERNCODE_FOR_WAR( subprotocol )										\
	const MSGSUB_##subprotocol& rMsg = static_cast<MSGSUB_##subprotocol&>(*pIt);	\
	size = rMsg.GetSize()




//==================================================================================================
//
#pragma pack(push, 1)
//--------------------------------------------------------------------------------------------------
//  <Base Header of Each Message>
template<uint8_t _PROTOCOL, typename _MSGTYPE >
struct WAR_TAG1_ : public WAR_TAG_V
{
    static const uint8_t value = _PROTOCOL;
    typedef _MSGTYPE msg_type;
    typedef WAR_TAG1_<value, msg_type> this_type;

    //  이것은 기본 연산자... 해당 타입을 상속한 별도의 타입은 가변 길이를 가진다면
    //  GetSize를 별도로 구현해야 한다. 그외의 고정형은 이것으로 모두 해결될 것이다.
    // default, can overriding
    uint16_t GetSize() const {
        return static_cast<uint16_t>(sizeof(*static_cast<const msg_type*>(this)));
    }
    bool SerializeTo(WAR_MSG_HDR* const IN header) const
    {
        uint16_t size = static_cast<const msg_type*>(this)->GetSize();
        WAR_MSG_HDR::MSG_SIZE_T offset = size + header->size;
        if (MAX_WAR_MSG_SEQUENCIAL_STREAM_SIZE < offset) {
            return false;
        }

        msg_type* const dest  = reinterpret_cast<msg_type*>(\
            reinterpret_cast<uint8_t*>(header + 1) + header->size);
        *dest = *static_cast<const msg_type*>(this);
        //
        dest->_TAG = msg_type::value;
        header->size = offset;

        return true;
    }
};
#pragma pack(pop)



