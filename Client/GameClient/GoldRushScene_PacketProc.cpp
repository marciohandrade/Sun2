#include "SunClientPreCompiledHeader.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "GoldRushScene.h"

//----------------------------------------------------------------------------
void GoldRushScene::ParsePacket(MSG_BASE* message)
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
void GoldRushScene::OnRecvCG_SYNC(MSG_BASE* message)
{
    BattleScene::OnRecvCG_SYNC(message);
}
//----------------------------------------------------------------------------
void GoldRushScene::OnRecvCG_STATUS(MSG_BASE* message)
{
    BattleScene::OnRecvCG_STATUS(message);
}
//----------------------------------------------------------------------------
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH