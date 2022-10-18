#pragma once

struct MSG_BASE;

#include <ServerConst.h>
#include <PacketControl/PacketControl.h>
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
namespace nsPacket { namespace Server {

class PacketControl
{
public:
inline static	PacketInfo	Pack( const eSERVER_TYPE fromXtype, const eSERVER_TYPE sendToXtype, MSG_BASE* const pMsg, const WORD wSize )
{
#define MsgFrom_( from )	(from)
#define SendTo_( to )		| ((to)<<16)
    switch( MsgFrom_(fromXtype) SendTo_(sendToXtype) )
    {
    case MsgFrom_(GAME_SERVER) SendTo_(AGENT_SERVER):
        return SubControl<GAME_SERVER, AGENT_SERVER>::MakeStream( pMsg, wSize );
    }
#undef SendTo_
#undef MsgFrom_
	PacketInfo ret = { 0, 0 };
	return ret;
}
	//static	PacketInfo	UnPack( const MsgType::AG&, MSG_BASE* const pMsg, const WORD wSize )	{ return MakeStreamG2A( FALSE, pMsg, wSize ); }
private:
    template< eSERVER_TYPE _FROM, eSERVER_TYPE _TO >
    struct SubControl { static	PacketInfo	MakeStream( MSG_BASE* const pMsg, const WORD wSize ); };

    template<>
    struct SubControl<GAME_SERVER, AGENT_SERVER> { static	PacketInfo	MakeStream( MSG_BASE* const pMsg, const WORD wSize ); };
    //static	PacketInfo	MakeStreamG2A( const BOOLEAN isPack, MSG_BASE* const pMsg, const WORD wSize );
private:
    static const DWORD	SIZEofSTREAM		= 32768;
    static BYTE			ms_PacketStream[SIZEofSTREAM];
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
class Environment
{
public:
	static	BOOLEAN		Init();
};

}}; //end of namespace


