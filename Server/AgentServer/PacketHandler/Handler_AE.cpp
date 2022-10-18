#include "stdafx.h"
#include "Handler_AE.h"
//#include "PacketHandler.Shared.h"

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AE::On##p }


sPACKET_HANDLER_INFO*	Handler_AE::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERTYPE, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AE::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<TEMP_SERVER>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<AUTH_AGENT_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//
HANDLER_AE_IMPL(SERVERCOMMON_SERVERTYPE)
{
    MSG_SERVER_TYPE* recv_msg = static_cast<MSG_SERVER_TYPE*>(pMsg);
    if (FlowControl::FCAssert(sizeof(*recv_msg) == wSize) == false) {
        return;
    };
    pServerSession->Redirect2NewSession(recv_msg);
}
