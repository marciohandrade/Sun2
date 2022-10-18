#pragma once

//��������������������������������������������������������������������������������������������������������������������������������
//
#ifndef __SSQ_MSG_HDR__
#define __SSQ_MSG_HDR__
// <Prototype of WAR_MSG_HDR>
//  ��ü �޽����� ���̰� �������. (variable stream)
struct WAR_MSG_HDR
{
    typedef ulong MSG_SIZE_T; // expanded by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    MSG_SIZE_T size;
};

//	<Raw Header of Each Message>
//	-	�� �޽����� Ÿ�Ե��� ����ȴ�.
//	-	1Byte, ������ raw stream�� WAR_TAG_V�� �̰��� ���� ����ϴ� ���� ����� �Ѵ�.
//		(�̰� common routine������ offset���� ���Ǽ��� �����̴�.)
//		��� �޽������� ���������� WAR_TAG1_<id, type>�� ����ؼ� �����ǰ� �Ǿ� �ִ�.
//	<����>
//	-	Server/Client ���� pointer������ �����ϸ鼭 ���׸� ������ �� �ִ�.
//		�ϳ��� �������� serializing input/output�� ������������ �ϴ� ���� ��ǥ��.
typedef struct WAR_TAG_V
{
	BYTE				_TAG;
} *WAR_TAG_P;
#endif


const WORD			MAX_WAR_MSG_SEQUENCIAL_STREAM_SIZE = 8000;


//��������������������������������������������������������������������������������������������������������������������������������
//
#define WAR_MSG_DECL( protocol )	\
	struct MSGSUB_##protocol : WAR_TAG1_< protocol, MSGSUB_##protocol >

//��������������������������������������������������������������������������������������������������������������������������������
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

    //  �̰��� �⺻ ������... �ش� Ÿ���� ����� ������ Ÿ���� ���� ���̸� �����ٸ�
    //  GetSize�� ������ �����ؾ� �Ѵ�. �׿��� �������� �̰����� ��� �ذ�� ���̴�.
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



