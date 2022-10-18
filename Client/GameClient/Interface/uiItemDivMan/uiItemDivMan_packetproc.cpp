//------------------------------------------------------------------------------
//  uiItemDivMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiItemDivMan.h"

//------------------------------------------------------------------------------
/**
*/
void
uiItemDivMan::NetworkProc( MSG_BASE * pMsg )
{
    switch( pMsg->m_byCategory )
    {
    case CG_ITEM:
        {
            switch (pMsg->m_byProtocol)
            {
            case CG_ITEM_DIVIDE_ACK:
                {
                    MSG_CG_ITEM_DIVIDE_ACK* recv =
                        (MSG_CG_ITEM_DIVIDE_ACK *)pMsg;
                }
                break;

            case CG_ITEM_DIVIDE_NAK:
                {
                    MSG_CG_ITEM_DIVIDE_NAK* recv =
                        (MSG_CG_ITEM_DIVIDE_NAK *)pMsg;
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
