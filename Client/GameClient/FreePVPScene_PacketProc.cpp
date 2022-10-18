#include "SunClientPrecompiledHeader.h"
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "FreePVPScene.h"

//----------------------------------------------------------------------------
void FreePVPScene::ParsePacket(MSG_BASE* message)
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
void FreePVPScene::OnRecvCG_SYNC(MSG_BASE* message)
{
    BattleScene::OnRecvCG_SYNC( message );

}

//----------------------------------------------------------------------------
/**
*/
void FreePVPScene::OnRecvCG_STATUS(MSG_BASE* message)
{
    BattleScene::OnRecvCG_STATUS(message);
}
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
