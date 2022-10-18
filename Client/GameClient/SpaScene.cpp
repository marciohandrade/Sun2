#include "SunClientPrecompiledHeader.h"

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "SpaScene.h"

//
#include "itemManager.h"
#include "ObjectManager.h"
#include "ResultCode.h"
#include "HeroActionInput.h"
#include "Hero.h"
#include "MapObject.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneInfo.h"
#include "ChaosSystem/uiChaosZoneResult.h"
#include "ChaosSystem/uiChaosZoneTimer.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "MapResMgr.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "GlobalFunc.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "BattleGroundFlagInfoParser.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"
//
///////////////////////////////////////////////////////////////

SpaScene g_SpaScene;

//-------------------------------------------------------------------------------------------
SpaScene::SpaScene() 
{
    m_SceneType = SCENE_TYPE_SPA;
}

//-------------------------------------------------------------------------------------------
SpaScene::~SpaScene()
{
}

//-------------------------------------------------------------------------------------------
BOOL SpaScene::Init()
{
    BattleScene::Init();

    if (g_pMap)
    {
        GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_SPA_FIELD);
    }

    return TRUE;
}

//-------------------------------------------------------------------------------------------
BOOL SpaScene::Process(DWORD tick)
{
    BOOL bRet = BattleScene::Process(tick);

    SendHeroSectorIndex();

    return bRet;
}

//-------------------------------------------------------------------------------------------
BOOL SpaScene::Render(DWORD tick)
{
    BOOL bRet = BattleScene::Render(tick);

    return bRet;
}

//-------------------------------------------------------------------------------------------
void SpaScene::Release( void )
{
    BattleScene::Release();
}

//-------------------------------------------------------------------------------------------
RELATIONKIND::TYPE SpaScene::GetRelationKindOfCharacter(Character* object, BOOL force_enemy)
{
    return GameFunc::GetRelationKindOfPlayerInSpa(object, force_enemy);
}

//----------------------------------------------------------------------------
void SpaScene::ParsePacket(MSG_BASE* message)
{
    switch (message->m_byCategory)
    {
    case CG_ITEM:
        {
            switch (message->m_byProtocol)
            {
            case CG_ITEM_SPA_COSTUME_CHANGE_BRD:
                {
                    MSG_CG_ITEM_SPA_COSTUME_CHANGE_BRD* recv_message =
                        static_cast<MSG_CG_ITEM_SPA_COSTUME_CHANGE_BRD*>(message);
        
                    Object* object = g_ObjectManager.GetObject(recv_message->player_object_key);
                    if (object && object->IsKindOfObject(CHARACTER_OBJECT))
                    {
                        Character* player = static_cast<Character*>(object);
                        player->SetSpaItemCode(recv_message->spa_ticket_code);
                        player->RefreshAppearRender();
                    }
                }
                break;
            case CG_ITEM_SPA_COSTUME_CHANGE_NAK:
                {
                    MSG_CG_ITEM_SPA_COSTUME_CHANGE_NAK* recv_message =
                        static_cast<MSG_CG_ITEM_SPA_COSTUME_CHANGE_NAK*>(message);

                    if (recv_message->error_code == RC::RC_ITEM_SPA_COSTUME_INVALID_ITEM)
                    {
                        // 70533 온천 입장권이 아닙니다.
                        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(70533));
                    }
                    else
                    {
                        ItemManager::Instance()->NoticeReasonItemErrorCode(recv_message->error_code);
                    }
                }
                break;
            }
        }
        break;
    }

    BattleScene::ParsePacket(message);

}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM