#include "stdafx.h"
#include ".\handler_common.h"



Handler_Common::Handler_Common(void)
{
}

Handler_Common::~Handler_Common(void)
{
}

VOID Handler_Common::OnSERVERCOMMON_HEARTBEAT( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}
