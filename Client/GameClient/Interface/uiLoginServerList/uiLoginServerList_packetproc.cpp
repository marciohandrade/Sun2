//------------------------------------------------------------------------------
//  uiLoginServerList_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginServerList.h"

//------------------------------------------------------------------------------
/**
*/
void
uiLoginServerList::NetworkProc( MSG_BASE * pMsg )
{
#ifdef _CK_MOD_UI_LOGIN
#else
    switch (pMsg->m_byProtocol)
    {
        // -- ���� ��� ��û
    case CL_AUTH_S2C_ANS_SVRLIST:
        {
            this->OffPending();
            //
            // <-- ���� ���� ���
            //
            this->NET_RECV_S2C_ANS_SVRLIST(pMsg);
        }
        break;

    case CL_AUTH_S2C_SERVER_NAME_LIST:
        {
            this->OffPending();
            //
            // <-- ���� �̸� ���
            //
            this->NET_RECV_S2C_SERVER_NAME_LIST(pMsg);
        }
        break;
    
        // -- ���� ����
    case CL_AUTH_S2C_ANS_SVRSELECT:
        {
            this->OffPending();
            //
            // <-- ���� ���� ���
            //
            this->NET_RECV_S2C_ANS_SVRSELECT(pMsg);
        }
        break;
    }
#endif
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
