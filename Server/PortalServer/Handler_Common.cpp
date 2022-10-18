#include "StdAfx.h"
#include ".\Handler_Common.h"



VOID Handler_Common::OnSERVERCOMMON_HEARTBEAT( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}