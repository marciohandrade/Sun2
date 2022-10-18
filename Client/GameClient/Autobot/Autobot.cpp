#include "SunClientPrecompiledHeader.h"

#include "Autobot.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Condition/AutobotCondition.h"
#include "Condition/AutobotConditionManager.h"
#include "Area/Field/AutobotAreaField.h"
#include "Area/Dungeon/AutobotAreaDungeon.h"
#include "Area/Village/AutobotAreaVillage.h"
#include "Autobot/Action/AutobotActionAttack.h"
#include "Interface/uiAutobot/uiAutobotResult.h"
#include "AutomaticInfoParser.h"

#include "GlobalData.h" 
#include "Hero.h"
#include "NPC.h"
#include "HeroActionInput.h"
#include "Mouse.h"
#include "RadarFullDialog.h"
#include "Interface/uiRadarMan/uiRadarMan.h"
#include "Interface/uiPartyMan/uiPartyMan.h"
#include "Item.h"
#include "ItemManager.h"
#include "ItemInfoParser.h"
#include "PacketRequestManager.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "ShopInfoParser.h"
#include "ObjectManager.h"
#include "InventoryDialog.h"
#include "ShopDialog.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "BattleScene.h"
#include "ChatMsg.h"
#include "ChatDialog.h"
#include "Interface/uiChatMan/uiChatMan.h"
#include "GlobalFunc.h"
#include "CooltimeManager.h"


//------------------------------------------------------------------------------
Autobot::Autobot() : started_(0), backup_hero_money_(0), backup_hero_exp_(0)
{
    state_machine_ = new AutobotStateMachine<Autobot>(this);

    state_machine()->RegisterState("field", AutobotAreaField::Instance());
    state_machine()->RegisterState("village", AutobotAreaVillage::Instance());
    state_machine()->RegisterState("dungeon", AutobotAreaDungeon::Instance());
}
//------------------------------------------------------------------------------
Autobot::~Autobot()
{
    SAFE_DELETE(state_machine_);
}
//------------------------------------------------------------------------------
void Autobot::Start()
{
    if (g_pHero == NULL)
    {
        return;
    }

    AutobotPasspointManager::Instance()->Clear();

    //------------------------------------------------------------------------------ 
    //! 채널 참가
    if (AutobotConditionManager::Instance()->community().IsChannel())
    {
        ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
        if ((chat_dialog != NULL) && (chat_dialog->GetExistChannel() == FALSE))
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {NULL, };
            Sprintf(message, _T("%s_%s"), 
                            AutobotConditionManager::Instance()->community().channel_level(),
                            AutobotConditionManager::Instance()->community().channel_name());

            if (Stricmp(chat_dialog->GetChannelName(), message) != 0)
            {
                chat_dialog->SendChatChannelCreate(message);
            }
        }
    }

    //! 주위 반경 사냥이면 자기 위치 셋팅
    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area)
    {
        AutobotConditionManager::Instance()->hunting().set_area_position(g_pHero->GetPosition());
    }

    //! 스크립트 유효성 체크
    //! 우선 부활만 체크(지역 점령전 악용)
    if ((AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Resurrection) && 
        (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH) == NULL))
    {
        AutobotConditionManager::Instance()->revival().set_revival_type(AutobotCondition_Revival::kRevival_Stay);
    }
    else if ((AutobotConditionManager::Instance()->revival().revival_type() == AutobotCondition_Revival::kRevival_Village) && 
             (AutomaticInfoParser::Instance()->GetAutomaticInfo(eAUTO_REBIRTH_VILIAGE) == NULL))
    {
        AutobotConditionManager::Instance()->revival().set_revival_type(AutobotCondition_Revival::kRevival_Stay);
    }

    //! 백업
    backup_hero_money_ = g_pHero->GetMoney();
    backup_hero_exp_ = g_pHero->GetExp();

#ifdef _YMS_AUTOBOT_GAME
    g_InterfaceManager.ShowDialog(uiAutobotResult::kDialog_ID, FALSE);
#endif //_YMS_AUTOBOT_GAME

    started_ = true;
}
//------------------------------------------------------------------------------
void Autobot::Stop(bool is_return_village)
{
    if (IsStarted() == false)
    {
        return;
    }

    if (g_pHero)
    {
#ifdef _YMS_AUTOBOT_GAME
        uiAutobotResult* resulte_dialog = GET_CAST_DIALOG(uiAutobotResult, uiAutobotResult::kDialog_ID);
        if (resulte_dialog != NULL)
        {
            resulte_dialog->ShowInterface(TRUE);

            resulte_dialog->SetResult(g_pHero->GetMoney() - backup_hero_money_, 
                                        g_pHero->GetExp() - backup_hero_exp_);
        }
#endif //_YMS_AUTOBOT_GAME

        g_pHero->SetSeekNodeCount(20000);
    }

    g_SkillQueue.ClearAll();
    g_HeroInput.SetAreaSkill(FALSE);


    //------------------------------------------------------------------------------
    //! 마을로 돌아가기
    if (is_return_village)
    {
        if ((state_machine()->CurrentState() != NULL) && 
            (_stricmp(state_machine()->CurrentState()->Name(), "AutobotAreaField") == 0))
        {
            SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, eITEMTYPE_SPECIAL, eITEMWASTE_RETURNSCROLL);
            if ((item_slot != NULL) && item_slot->GetNum() > 0)
            {
                Autobot::Instance()->UseItem(item_slot);
            }
        }
    }
    

    state_machine()->ChangeState(NULL);

    started_ = false;
}
//------------------------------------------------------------------------------
bool Autobot::Update(DWORD dwTick)
{
    if (IsStarted() == false)
    {
        return true;
    }
    if ((dwTick <= 0) || 
        (g_pNetwork == NULL) || 
        (g_pNetwork->IsConnected(CI_GAMESERVERIDX) == FALSE))
    {
        Stop(false);
        return true;
    }
    if (g_pHero == NULL)
    {
        state_machine()->ChangeState(NULL);
        return true;
    }
    if ((BattleScene::IsWaitEnterSync() == TRUE) || 
        (g_pHero->IsSteel() == true)) //!< 처음 스폰된 상태(무적 상태)
    {
        return true;
    }
    if (SyncState() == false)
    {
        return true;
    }

    return state_machine()->Update();
}
//------------------------------------------------------------------------------
bool Autobot::NetworkProc(MSG_BASE* packet)
{
    if (IsStarted() == false)
    {
        return true;
    }

    if (packet->m_byCategory == CG_PARTY)
    {
        switch (packet->m_byProtocol)
        {
        case CG_PARTY_JOIN_RESPONSE_SYN:
            {
                if (AutobotConditionManager::Instance()->community().party_type() == AutobotCondition_Community::kParty_Agree)
                {
                    MSG_CG_PARTY_JOIN_RESPONSE_ACK sync;
                    GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

                    //! 이후 app에서도 처리 안하게 한다.
                    return false;
                }
                else if (AutobotConditionManager::Instance()->community().party_type() == AutobotCondition_Community::kParty_Ignore)
                {
                    MSG_CG_PARTY_JOIN_RESPONSE_NAK sync;
                    GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

                    //! 이후 app에서도 처리 안하게 한다.
                    return false;
                }
            }
            break;
        case CG_PARTY_LEAVE_BRD:
            {
                if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
                {
                    MSG_CG_PARTY_LEAVE_BRD* msg = static_cast<MSG_CG_PARTY_LEAVE_BRD*>(packet);

                    if ((msg->m_dwObjKey > 0) && 
                        (msg->m_dwObjKey == AutobotConditionManager::Instance()->hunting().GetFollowPlayer()))
                    {
                        //! 다음 찾기
                        if (AutobotConditionManager::Instance()->hunting().NextFollowPlayer(msg->m_dwObjKey) == false)
                        {
                            //! 중지
                            Stop(false);
                        }
                    }
                }
            }
            break;
        case CG_PARTY_DESTROY_BRD:
            {
                if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
                {
                    //! 파티 깨졌으면 중지
                    Stop(false);
                }
            }
            break;
        }
    }

    return state_machine()->NetworkProc(packet);
}
//------------------------------------------------------------------------------
bool Autobot::MessageProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    if (IsStarted() == false)
    {
        return true;
    }
    return true;
}
//------------------------------------------------------------------------------
bool Autobot::SyncState()
{
    if (GameFramework::GetCurrentScene() != NULL)
    {
        if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE)
        {
            if (state_machine()->CurrentState() == AutobotAreaVillage::Instance())
            {
                return true;
            }
            g_pHero->SetSeekNodeCount(100000000);
            state_machine()->ChangeState("village");
            return false;
        }
        else if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD)
        {
            if (state_machine()->CurrentState() == AutobotAreaField::Instance())
            {
                return true;
            }
            g_pHero->SetSeekNodeCount(100000000);
            state_machine()->ChangeState("field");
            return false;
        }
        else if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING) || 
                 (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
        {
            if (state_machine()->CurrentState() == AutobotAreaDungeon::Instance())
            {
                return true;
            }
            g_pHero->SetSeekNodeCount(100000000);
            state_machine()->ChangeState("dungeon");
            return false;
        }
    }

    state_machine()->ChangeState(NULL);
    return false;
}
//------------------------------------------------------------------------------
bool Autobot::UseItem(SCItemSlot* item_slot)
{
    if (item_slot == NULL)
    {
        return false;
    }
    if (PacketRequestManager::Instance()->is_sent(CG_ITEM, CG_ITEM_USE_SYN))
    {
        return false;
    }
  
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_slot->GetCode());
    if (item_info == NULL)
    {
        return false;
    }

    util::Timer* cool_timer = xGetCooltimeManager()->GetItemCooltime(item_info->m_wCoolTimeType);
    if ((cool_timer != NULL) && (cool_timer->IsExpired(FALSE) == FALSE)) 
    {
        return false;
    }

    if (g_pHero->CanUseItem(item_info) == FALSE)
    {
        return false;
    }

    ItemManager::Instance()->SendItemUseMsg(item_slot->GetSlotIdx(), item_slot->GetPos());

    //ItemManager::Instance()->SendItemUseMsg 함수 내부에서 하는것이 좋은데.
    PacketRequestManager::Instance()->PushPacket((new MSG_CG_ITEM_USE_SYN),
                                                 (new MSG_CG_ITEM_USE_ACK), 
                                                 (new MSG_CG_ITEM_USE_NAK),
                                                 sizeof(MSG_CG_ITEM_USE_SYN), 
                                                 PACKET_WAITING_MILLISECOND, 
                                                 false);
    return true;
}
//------------------------------------------------------------------------------
bool Autobot::CanUseSkill(SLOTCODE skill_code)
{
    if (skill_code <= 0)
    {
        return false;
    }

	if( g_pHero->CannotAction() == TRUE )
	{
		return false;
	}

    if (g_pHero->IsImpossible() )
    {
        return false;
    }
    
    STATE::TYPE current_state = g_pHero->GetCurState();
    STATE::TYPE next_state = g_pHero->GetNextState();

    PLAYER_ACTION& action = g_HeroInput.GetQueueActionRef();

    if (skill_code == eACTION_START_SKILL_CODE)
    {
        if ((current_state != STATE::SKILL) && (current_state != STATE::ATTACK) && 
            (next_state != STATE::SKILL) && (next_state != STATE::ATTACK) && 
            (action.ActionID != ACTION_SKILL) && (action.ActionID != ACTION_ATTACK))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if ((current_state != STATE::SKILL) && 
             (next_state != STATE::SKILL) && 
             (action.ActionID != ACTION_SKILL))
    {
        if (g_pHero->CanUseSkill(skill_code) == TRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
//------------------------------------------------------------------------------
bool Autobot::HasSellItem(OUT POSTYPE* inventory_slot_pos)
{
    InventoryDialog* inventory_dialog = GET_CAST_DIALOG(InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG);
    if (inventory_dialog == NULL)
    {
        return true;
    }

    POSTYPE max_slot = inventory_dialog->GetMaxSlotNum();

    for (POSTYPE i = 0; i < max_slot; ++i)
    {
        if (inventory_dialog->IsEmpty(i) || 
            (inventory_dialog->GetSlot(i).GetSlotType() != ST_ITEM))
        {
            continue;
        }
        
        SCItemSlot& item_slot = (SCItemSlot&)(inventory_dialog->GetSlot(i));
        if (item_slot.IsBlocked() || item_slot.IsUserBlocked() || item_slot.IsLocked())
        {
            continue;;
        }

        if ((AutobotConditionManager::Instance()->shop().GetBuyItemCount(item_slot.GetCode()) <= 0) && //!< 구입 목록에 없고
            (AutobotConditionManager::Instance()->shop().IsSellItem(&item_slot) == true))   //!< 만매로 셋팅되어 있는것만
        {
            if (inventory_slot_pos != NULL)
            {
                *inventory_slot_pos = i;
            }
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------
bool Autobot::HasBuyItem(OUT SLOTCODE* item_code, OUT NUMTYPE* buy_count)
{
    AutobotCondition_Shop::ConditionBuyItemMap::iterator begin = AutobotConditionManager::Instance()->shop().buyitem_map().begin();
    AutobotCondition_Shop::ConditionBuyItemMap::iterator end = AutobotConditionManager::Instance()->shop().buyitem_map().end();
    for (; begin != end; ++begin)
    {
        SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, begin->first);

        if (item_slot == NULL)
        {
            if (item_code != NULL)
            {
                *item_code = begin->first;
            }
            if (buy_count != NULL)
            {
                *buy_count = begin->second;
            }
            return true;
        }
        else
        {
            if (item_slot->GetNum() < begin->second)
            {
                if (item_code != NULL)
                {
                    *item_code = begin->first;
                }
                if (buy_count != NULL)
                {
                    *buy_count = begin->second - item_slot->GetNum();
                }
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool Autobot::SellItem(POSTYPE inventory_slot_pos)
{
    ItemManager::Instance()->SendSellMsg(SI_INVENTORY, inventory_slot_pos, 0, ENCHANT_MAX);

    PacketRequestManager::Instance()->PushPacket((new MSG_CG_ITEM_SELL_SYN),
                                                (new MSG_CG_ITEM_SELL_ACK), 
                                                (new MSG_CG_ITEM_SELL_NAK),
                                                sizeof(MSG_CG_ITEM_SELL_SYN), 
                                                PACKET_WAITING_MILLISECOND, 
                                                false);

    return true;
}
//------------------------------------------------------------------------------
bool Autobot::BuyItem(SLOTCODE item_code, NUMTYPE buy_count)
{
    ShopDialog* shop_dialog = (ShopDialog*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP);
    if (shop_dialog == NULL)
    {
        return false;
    }

    SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_NPCSTORE, item_code, buy_count);
    if (item_slot == NULL)
    {
        return false;
    }
    if (item_slot->GetPriceForBuy() > g_pHero->GetMoney())
    {
        return false;
    }

    ItemManager::Instance()->SendBuyMsg(shop_dialog->GetShopID(), 
                                        item_slot->GetCode(),
                                        item_slot->GetPos() / ShopDialog::SHOP_MAX_SLOT,    //shop_dialog->GetCurrentTab(), 
                                        shop_dialog->GetRelativeSlotPos(item_slot->GetPos()), 
                                        buy_count);

    //ItemManager::Instance()->SendItemUseMsg 함수 내부에서 하는것이 좋은데.
    PacketRequestManager::Instance()->PushPacket((new MSG_CG_ITEM_BUY_SYN),
                                                (new MSG_CG_ITEM_BUY_ACK), 
                                                (new MSG_CG_ITEM_BUY_NAK),
                                                sizeof(MSG_CG_ITEM_BUY_SYN), 
                                                PACKET_WAITING_MILLISECOND, 
                                                false);

    return true;
}
//------------------------------------------------------------------------------
bool Autobot::RepairItemWearAll()
{
    if (ItemManager::Instance()->GetPriceRepairItemAll(SI_EQUIPMENT) > g_pHero->GetMoney())
    {
        return false;
    }

    ItemManager::Instance()->SendItemRepairMsg(SI_EQUIPMENT, 0, true);

    //ItemManager::Instance()->SendItemRepairMsg 함수 내부에서 하는것이 좋은데.
    PacketRequestManager::Instance()->PushPacket((new MSG_CG_ITEM_REPAIR_DURA_SYN),
                                                 (new MSG_CG_ITEM_REPAIR_DURA_ACK), 
                                                 (new MSG_CG_ITEM_REPAIR_DURA_NAK),
                                                 sizeof(MSG_CG_ITEM_REPAIR_DURA_SYN), 
                                                 PACKET_WAITING_MILLISECOND, 
                                                 false);

    return true;
}
//------------------------------------------------------------------------------
void Autobot::Lua_Action(const char* parameter1, const char* parameter2)
{
    if (_stricmp(parameter1, "start") == 0)
    {
        Start();
    }
    else if (_stricmp(parameter1, "stop") == 0)
    {
        Stop(false);
    }
    else if (_stricmp(parameter1, "clear") == 0)
    {
        AutobotPasspointManager::Instance()->Clear();
    }
    else if (_stricmp(parameter1, "huntertype") == 0)
    {
    }
    else if (_stricmp(parameter1, "targetrange") == 0)
    {
        AutobotConditionManager::Instance()->hunting().set_target_range(static_cast<float>(atof(parameter2)));
    }
}
//------------------------------------------------------------------------------
int Autobot::GetRandomDestinationTile(int skip_tile_count)
{
    static int vary_symbol = 1;

    int max_tile = max(g_pSunTerrain->x_pPathFinder->GetNumPathTile(), 1);
    if (vary_symbol > 0)
    {
        if (g_pHero->GetPathExplorer()->GetTile() + skip_tile_count > int(max_tile * 0.7f))
        {
            vary_symbol = -1;
        }
    }
    else
    {
        if (g_pHero->GetPathExplorer()->GetTile() - skip_tile_count < int(max_tile * 0.3f))
        {
            vary_symbol = 1;
        }
    }

    for (int i = skip_tile_count; i < max_tile; ++i)
    {
        int next_tile_no = abs((g_pHero->GetPathExplorer()->GetTile() + (i * vary_symbol))) % max_tile;
        WzVector v = g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(next_tile_no);
        
        if (AutobotConditionManager::Instance()->hunting().IsValidArea(v) && 
            (GameFunc::IsMove(g_pHero->GetPosition(), v, TRUE, g_pHero->GetSeekNodeCount()) == TRUE))
        {
            return next_tile_no;
        }
    }
    return -1;
}
//------------------------------------------------------------------------------
WzVector Autobot::GetRandomDestinationPosition(float radius)
{
    float random_value = GlobalFunc::GetRandom(314) * 0.01f;

    WzVector pos;

    pos.x = g_pHero->GetPosition().x + (cosf(random_value) * radius);
    pos.y = g_pHero->GetPosition().y + (sinf(random_value) * radius);
    pos.z = g_pHero->GetPosition().z;

    int tile_no = GameFunc::GetNearestTile(pos, PTA_ONLY_JUMP | PTA_NO_WALK);
    if (tile_no >= 0)
    {
        return g_pSunTerrain->x_pPathFinder->GetRandomPosInTile(tile_no);
    }
    
    return g_pHero->GetPosition();
}

//------------------------------------------------------------------------------
bool Autobot::IsPossibleTarget(DWORD object_key)
{
    if (IsStarted() == false)
    {
        return true;
    }

    return ((AutobotConditionManager::Instance()->hunting().IsAvoidMonster(object_key) == false) && 
            (AutobotConditionManager::Instance()->hunting().IsMonsterInValidArea(object_key) == true));
}
//------------------------------------------------------------------------------
bool Autobot::IsPossiblePickup(Item& item)
{
    if ((g_pHero == NULL) || (IsStarted() == false))
    {
        return true;
    }

    return AutobotConditionManager::Instance()->pickup().IsPossiblePickup(item);
}
//------------------------------------------------------------------------------
bool Autobot::GetAreaSkillClickPosition(SLOTCODE skill_code, WzVector& target_pos)
{
    if ((g_pHero == NULL) || (IsStarted() == false))
    {
        return false;
    }

    return AutobotActionAttack::Instance()->GetAreaSkillTargetPosition(skill_code, target_pos);
}
//------------------------------------------------------------------------------ 
void Autobot::WriteChat(ChatMsg* chat)
{
    if (IsStarted() == false)
    {
        return;
    }

    AutobotConditionManager::Instance()->WriteChat(chat);
}
//------------------------------------------------------------------------------
void Autobot::Render_Checkpoint()
{
    if (IsStarted() == false)
    {
        return;
    }

#ifdef _DEV_VER
    std::list<AutobotPasspoint>::iterator begin = AutobotPasspointManager::Instance()->passpoint_list().begin();
    std::list<AutobotPasspoint>::iterator end = AutobotPasspointManager::Instance()->passpoint_list().end();

    for (; begin != end; ++begin)
    {
        WzColor colorback = g_pSunRenderer->GetDiffuseColor();
        {
            BvSphere bv;
            bv.m_fRadius = 2.0f;
            bv.m_wvCenter = begin->position();

            g_pSunRenderer->SetColor(WzColor_RGBA(255,0,0,0));
            g_pSunRenderer->RenderBvSphere(&bv);
        }
        g_pSunRenderer->SetColor(colorback);
    }

    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area)
    {
        WzColor colorback = g_pSunRenderer->GetDiffuseColor();
        {
            BvSphere bv;
            bv.m_fRadius = AutobotConditionManager::Instance()->hunting().area_range();
            bv.m_wvCenter = AutobotConditionManager::Instance()->hunting().area_position();

            g_pSunRenderer->SetColor(WzColor_RGBA(255,255,0,0));
            g_pSunRenderer->RenderBvSphere(&bv);

            bv.m_fRadius = 1.0f;
            g_pSunRenderer->SetColor(WzColor_RGBA(255,255,255,0));
            g_pSunRenderer->RenderBvSphere(&bv);
        }
        g_pSunRenderer->SetColor(colorback);
    }

#endif //_DEV_VER

}
//------------------------------------------------------------------------------
void Autobot::Render_DebugInfo()
{
    if (IsStarted() == false)
    {
        return;
    }

#ifdef _DEV_VER

    if(NULL == g_pMap)
    {
        return;
    }

    if (IsStarted() == false)
    {
        return;
    }

    AutobotArea* area_state = static_cast<AutobotArea*>(state_machine()->CurrentState());
    if (area_state == NULL)
    {
        return;
    }
    AutobotState<AutobotArea>* action_state = area_state->state_machine()->CurrentState();
    if (action_state == NULL)
    {
        return;
    }

    TCHAR lpszText[512] = {0,};

    int x = 400;
    int y = 5;
    {
        Snprintf( lpszText, 256-1, _T("Area State = %s"), area_state->Name() );
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Free)
        {
            Snprintf( lpszText, 256-1, _T("Hunter Type = %s"), "kHuntingType_Free");
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
            y += 20;
        }
        else if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
        {
            Snprintf( lpszText, 256-1, _T("Hunter Type = %s"), "kHuntingType_Party");
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
            y += 20;
        }
        else if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Area)
        {
            Snprintf( lpszText, 256-1, _T("Hunter Type = %s"), "kHuntingType_Area");
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
            y += 20;

            x += 20; 
            Snprintf( lpszText, 256-1, _T("area range = %f"), AutobotConditionManager::Instance()->hunting().area_range());
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
            x -= 20; 
            y += 20;

        }

        Snprintf( lpszText, 256-1, _T("Action State = %s"), action_state->Name() );
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        AutobotPasspoint* passport = AutobotPasspointManager::Instance()->Current();
        if (passport)
        {
            x += 20; 

            if (passport->passpoint_type() == AutobotPasspoint::kPasspoint_Temporarily) 
            {
                Snprintf(lpszText, 256-1, _T("passpoint type = %s"), "kPasspoint_Temporarily");
            }
            if (passport->passpoint_type() == AutobotPasspoint::kPasspoint_Runaway) 
            {
                Snprintf(lpszText, 256-1, _T("passpoint type = %s"), "kPasspoint_Runaway");
            }
            if (passport->passpoint_type() == AutobotPasspoint::kPasspoint_Return) 
            {
                Snprintf(lpszText, 256-1, _T("passpoint type = %s"), "kPasspoint_Return");
            }
            if (passport->passpoint_type() == AutobotPasspoint::kPasspoint_FindFollowPlayer) 
            {
                Snprintf(lpszText, 256-1, _T("passpoint type = %s"), "kPasspoint_FindFollowPlayer");
            }
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
            x -= 20; 
            y += 20;
        }

        Snprintf( lpszText, 256-1, _T("Hero State(Cur/Next) = %d / %d"), g_pHero->GetCurState(), g_pHero->GetNextState());
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

    }
#endif //_DEV_VER

}
