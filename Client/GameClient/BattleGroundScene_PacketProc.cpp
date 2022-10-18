#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "BattleGroundScene.h"

//----------------------------------------------------------------------------
void BattleGroundScene::ParsePacket(MSG_BASE* message)
{
    switch (message->m_byCategory)
    {
    case CG_SYNC:
        {
            OnRecvCG_SYNC(message);
        }
        break;

    case CG_STATUS:
        {
            OnRecvCG_STATUS(message);
        }
        break;
    default:
        {
            BattleScene::ParsePacket(message);
        }
        break;
    }
}
//----------------------------------------------------------------------------
/**
*/
void BattleGroundScene::OnRecvCG_SYNC(MSG_BASE* message)
{
    BattleScene::OnRecvCG_SYNC(message);
}
//----------------------------------------------------------------------------
/**
*/
void BattleGroundScene::OnRecvCG_STATUS(MSG_BASE* message)
{
    BattleScene::OnRecvCG_STATUS(message);
    /*
    switch ( pMsg->m_byProtocol )
        {
        case CG_STATUS_RESURRECTION_BRD:
            {
                MSG_CG_STATUS_RESURRECTION_BRD * pRecvMsg = (MSG_CG_STATUS_RESURRECTION_BRD *)pMsg;
            }
            break;
    
        case CG_STATUS_DEAD_BRD :
            {
                MSG_CG_STATUS_DEAD_BRD *pRecvPacket = (MSG_CG_STATUS_DEAD_BRD *)pMsg;
            }
            break;
    
        default:
            break;
        }*/
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND