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
        // -- 서버 목록 요청
    case CL_AUTH_S2C_ANS_SVRLIST:
        {
            this->OffPending();
            //
            // <-- 서버 상태 목록
            //
            this->NET_RECV_S2C_ANS_SVRLIST(pMsg);
        }
        break;

    case CL_AUTH_S2C_SERVER_NAME_LIST:
        {
            this->OffPending();
            //
            // <-- 서버 이름 목록
            //
            this->NET_RECV_S2C_SERVER_NAME_LIST(pMsg);
        }
        break;
    
        // -- 서버 선택
    case CL_AUTH_S2C_ANS_SVRSELECT:
        {
            this->OffPending();
            //
            // <-- 서버 선택 결과
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
