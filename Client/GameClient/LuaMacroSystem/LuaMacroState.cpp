#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

#include "LuaMacroState.h"
#include "cLua.h"
#include "Hero.h"
#include "HeroActionInput.h"
#include "MapInfoParser.h"
#include "Map.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "uiQuestMan/uiQuestMan.h"
#include "Monster.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Tree/uiBattlezone2Tree.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#include "BattleScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "LuaMacroSystem.h"
#include "HeroData.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "NPC.h"
#include "MouseHandler.h"
#include "SkillInfoParser.h"
#include "quickcontainer.h"
#include "itemmanager.h"
#include "quickslot.h"
#include "SkillMain.h"
#include "MapObject.h"
#include ".\CollectionParser.h"
#include "RewardDialog.h"
#include "uiPersonalWayPointWindow/uiPersonalWayPointWindow.h"
#include "CastLeaveFieldDialog.h"
#include "Item.h"
#include "SCItemSlot.h"
void WriteMacroLog(TCHAR* message, BOOL force_write /* = FALSE*/)
{
    if ((force_write == FALSE) &&
        (xGetHeroData()->macro_system()->write_log() == FALSE))
    {
        return;
    }

    if (message == NULL)
    {
        return;
    }

    if (FILE* fp = fopen("MacroLog.txt","a+"))
    {
        struct tm * today;
        time_t		ltime;
        time( &ltime );
        today = localtime( &ltime );

        TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
        sprintf(buffer,"[%02d:%02d:%02d] ", today->tm_hour, today->tm_min, today->tm_sec);

        BYTE stack_depth = xGetHeroData()->macro_system()->stack_count();

        for (int i = 0; i < stack_depth; ++i)
        {
            strcat(buffer, ">");
        }
        strcat(buffer, " ");
        strcat(buffer, message);

        fprintf(fp, buffer);

        fclose(fp);
    }
}

// ��ũ��Ʈ ���� ����
void MacroScript::SetFileName(const TCHAR* script_name)
{
    strncpy(script_name_, script_name, LUA_FILE_NAME_LENGTH-1);
}
void MacroScript::CallScript()
{
    // ��� ��ũ��Ʈ ȣ��
    lua_State* script_context = GET_LUA()->GetScriptContext();

    std::string strTestFile = (std::string) "Data\\Scripts\\" + script_name_ + ".LUA";

    luaL_dofile(script_context, strTestFile.c_str());
    
    lua_getglobal(script_context, "Main");

    lua_call(script_context, 0 /*�Էº���0��*/, 0);
}
void MacroScript::InsertMacroCommand(MacroCommand command)
{
    macro_queue_.push_back(command);
}
MacroCommand MacroScript::PopMacroCommand()
{
    if (macro_queue_.empty() == false)
    {
        MacroCommand command = macro_queue_.front();
        macro_queue_.pop_front();

        return command;
    }
    else
    {
        // ��� ��� ����. ȣ�� ����
        MacroCommand command;
        command.command_type_ = Macro_EndMacro;

        return command;
    }
}

// ��ũ�� ����
void MacroState::Init()
{
    tick_count_ = 0;
    wait_time_ = 0;
    is_satisfied_ = FALSE;
}
BOOL MacroState::is_satisfied_wait_time(DWORD tick_)
{
    if (wait_time_ > 0)
    {
        wait_time_ -= tick_;
        return FALSE;
    }
    return TRUE;
}
void MacroState::UsePotion()
{
    const int loof_count = 10; // 10������ üũ

    bool need_hp = ((float)g_pHero->GetHP() / (float)g_pHero->GetMaxHP() * 100.0f) < 80.0f;
    bool need_mp = ((float)g_pHero->GetMP() / (float)g_pHero->GetMaxMP() * 100.0f) < 80.0f;

    // ȸ���� �ʿ��ϴٸ�
    if (need_hp || need_mp)
    {
        QuickContainer* quick_container =
            static_cast<QuickContainer*>(ItemManager::Instance()->GetItemContainerDialog(SI_QUICK));
        if (quick_container == NULL)
        {
            return;
        }

        for (int i = 0; i < loof_count; ++i)
        {
            POSTYPE slot_pos = quick_container->GetRealSlotPos(i);

            // ���� ���� ����
            if (slot_pos > quick_container->GetMaxSlotNum())
            {
                continue;
            }
            if (quick_container->IsEmpty(slot_pos)) 
            {
                continue;
            }
            QuickSlot& quick_slot = static_cast<QuickSlot&>(quick_container->GetSlot(slot_pos));
            if (quick_slot.GetStatus() != SLOT_RENDER_STATE_ACTIVATED)
            {
                continue;
            }
            if (quick_slot.GetOrgSlotType() != ST_ITEM)
            {
                continue;
            }
            if (quick_slot.GetOrgSlot() == NULL)
            {
                continue;
            }
            const BASE_ITEMINFO* item_info = 
                ((SCItemSlot*)quick_slot.GetOrgSlot())->GetItemInfo();
            if (item_info == NULL)
            {
                continue;
            }

            POSTYPE OrgPos = quick_slot.GetOrgPos();
            BOOL bRet = TRUE;

            if (item_info->m_wType != eITEMTYPE_WASTE)
            {
                continue;
            }

            if (((need_hp == true) && (item_info->m_byWasteType == eITEMWASTE_HPPOTION)) ||
                ((need_mp == true) && (item_info->m_byWasteType == eITEMWASTE_MPPOTION)) ||
                (item_info->m_byWasteType == eITEMWASTE_HPMP_POTION))
            {
                if (g_pHero->CanUseItem(item_info))
                {
                    MouseHandler::Instance()->ItemUseTransactionFromInventory(SI_INVENTORY, OrgPos);
                    return;
                }
            }
        }
    }
}
void MacroState::PickupItem()
{
    //xGetHeroData()->macro_system();

    Item            *pObj;
    Object          *pObject;
    SCObject        SCObj;
    // ��ü ������Ʈ ��, �����۸� �̾Ƴ��� �Ÿ� ���
    Object_Map_Itr itr = g_ObjectManager.GetBegin();
    for (itr = g_ObjectManager.GetBegin() ; itr != g_ObjectManager.GetEnd(); ++itr)
    {
        pObject = static_cast<Object *>(itr->second);
        if (pObject == 0)
        {
            return;
        }

        // �������ΰ�?
        SCObj = (itr->second)->GetSCObject();
        if (SCObj.GetObjectType() != ITEM_OBJECT)
        {
            continue;
        }
        pObj = (Item*)pObject;

        // �������� ���� ���ڸ� Ŭ�� �����ϴ�~
        if( pObj->IsOpened() == TRUE )
        {
            continue;
        }

        // �ʹ� �ָ� �˻� ��󿡼� ����
        float fDistance;
        WzVector vPlayerPos = g_pHero->GetPosition();
        WzVector vTargetPos = pObj->GetPosition();
        WzVector vOffset = vTargetPos - vPlayerPos;
        fDistance = VectorLength( &vOffset );
        if (fDistance > DISTANCE_SPACE_GET_ITEM)
        {
            continue;
        }


        if (pObj->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
        {
            if (GlobalFunc::CanGetHaim(pObj))
            {
                if (pObj->GetSendPickPacket() == FALSE)
                {
                    // �԰ڴٰ� ��û�� �� ���� �������� �ݴ´�.
                    ItemManager::Instance()->SendPickUpItemObject(pObj);
                    pObj->SetSendPickPacket(TRUE);
                    return;
                }
            }
        }
        else
        {
            // ���� ����Ʈ�� �ִ���.
            SCItemSlot& item_slot = pObj->GetItemSlot();
            STLX_VECTOR<DWORD> pickup_list = xGetHeroData()->macro_system()->pickup_list();
            STLX_VECTOR<DWORD>::iterator itr = pickup_list.begin();
            for( ; itr != pickup_list.end() ; ++itr)
            {
                DWORD item_code = (*itr);
                if (item_code == item_slot.GetItemCode())
                {
                    if (pObj->GetSendPickPacket() == FALSE)
                    {
                        // �԰ڴٰ� ��û�� �� ���� �������� �ݴ´�.
                        ItemManager::Instance()->SendPickUpItemObject(pObj);
                        pObj->SetSendPickPacket(TRUE);
                        return;
                    }
                }
            }
        }

    }
}
//MacroStateMove
void MacroStateMove::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "Move : %.1f, %.1f, %.1f\n", target_position_.x, target_position_.y, target_position_.z);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    last_map_code_ = g_pMap->GetMapID();

    MacroState::Init();
}
void MacroStateMove::Action()
{
    g_HeroInput.OnLClickedPosition(&target_position_);
}
void MacroStateMove::Process(DWORD tick_)
{
    tick_count_ += tick_;

    if (tick_count_ > 5000)
    {
        Action();
        tick_count_ = 0;

        UsePotion();
        PickupItem();
    }

    return;
}
BOOL MacroStateMove::is_satisfied(DWORD tick_)
{
    if (last_map_code_ != g_pMap->GetMapID())
    {
        if (wait_time() == 0)
        {
            set_wait_time(500);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }

        return FALSE;
    }

    // ��ǥ�� �����ߴ���
    WzVector distance = g_pHero->GetPosition() - target_position_;

    if (VectorLength2(&distance) < 0.5f)
    {
        if (wait_time() == 0)
        {
            set_wait_time(500);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }

    return FALSE;
}
void MacroStateMove::SetTargetPosition(float x, float y, float z)
{
    target_position_.x = x;
    target_position_.y = y;
    target_position_.z = z;
}

//MacroStateMoveMap
void MacroStateMoveMap::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "MoveMap : %d\n", map_code_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG
}
void MacroStateMoveMap::Action()
{
    WORD index = MapInfoParser::Instance()->GetWayPointIndex(g_pMap->GetCurrentFieldID(), map_code_);

    if (index != 0)
    {
        MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
        sync.m_wIndex = index;
        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));
    }
    else
    {
        // ���� ���� ��������Ʈ ���̵�
    }
}
BOOL MacroStateMoveMap::is_satisfied(DWORD tick_)
{
    if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOAD)
    {
        return FALSE;
    }

    if (g_pMap->GetMapID() == map_code_)
    {
        // �ε� ���� ����ؼ� Scene �Ѿ�� �� 5�ʰ� ���;
        if (wait_time() == 0)
        {
            set_wait_time(5000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }
        
    return FALSE;
}
void MacroStateMoveMap::SetMapCode(DWORD map_code)
{
    map_code_ = map_code;
}

//MacroStateNpcMeet
void MacroStateNpcMeet::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "NpcMeet : %d\n", npc_code_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    last_map_code_ = g_pMap->GetMapID();

    MacroState::Init();
}
void MacroStateNpcMeet::Action()
{
    Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
    for ( ; found_itr != end_itr; ++found_itr)
    {			
        Object* object = found_itr->second;
        if (object == NULL)
        {
            continue;
        }

        eOBJECT_TYPE eObjectType = object->GetSCObject().GetObjectType();
        if (eObjectType != NPC_OBJECT)
        {
            continue;
        }

        NPC* npc = static_cast<NPC*>(object);

        if (npc->GetNpcInfo()->m_MonsterCode == npc_code_)
        {
            Character* character = static_cast<Character*>(object);
            RELATIONKIND::TYPE relation_type =
                GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(character, false);
            g_HeroInput.OnLClickedCharacter(character, relation_type, false);

            return;
        }
    }
}
void MacroStateNpcMeet::Process(DWORD tick_)
{
    tick_count_ += tick_;

    if (tick_count_ > 5000)
    {
        Action();
        tick_count_ = 0;
    }

    return;
}
BOOL MacroStateNpcMeet::is_satisfied(DWORD tick_)
{
    if (last_map_code_ != g_pMap->GetMapID())
    {
        if (wait_time() == 0)
        {
            set_wait_time(1000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }

        return FALSE;
    }

    // �ش� NPC�� Ÿ�Կ� ���� ������ â�� �ʹ� ����.
    // �ϴ� �ش� npc�� �� Ŭ���ߴ��� ������ ����.
    uiQuestMan* quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (quest_manager)
    {
        if (quest_manager->GetNPCCode() == npc_code_)
        {
            if (wait_time() == 0)
            {
                set_wait_time(1000);
            }
            else
            {
                return is_satisfied_wait_time(tick_);
            }
        }
    }
    return FALSE;

}
void MacroStateNpcMeet::SetNpcCode(DWORD npc_code)
{
    npc_code_ = npc_code;

    uiQuestMan* quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (quest_manager)
    {
        // �� ������ ���������� ������
        quest_manager->SetNPCCode(0);
    }

}

//MacroStateCreateRoom
void MacroStateCreateRoom::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "CreateRoom : %d,%d,%d\n", depth1_,depth2_,depth3_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager)
    {
        party_manager->LeaveParty();
    }

    MacroState::Init();
}
void MacroStateCreateRoom::Action()
{
    BATTLEZONE2_TREE tree_data;
    tree_data.menu_depth[1] = depth3_;
    tree_data.menu_depth[2] = depth2_;
    tree_data.menu_depth[3] = depth1_;
    uiBattlezone2Tree* battlezone2_tree = GET_CAST_DIALOG(uiBattlezone2Tree, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_TREE);
    if (battlezone2_tree)
    {
        battlezone2_tree->set_created_room_menu_index(tree_data.menu_index);
        battlezone2_tree->set_is_creating(true);
        battlezone2_tree->SetDefaultRoomName();
        battlezone2_tree->SetRoomPassword("1234");
        battlezone2_tree->CreateRoom();
    }
}
BOOL MacroStateCreateRoom::is_satisfied(DWORD tick_)
{
    if (g_pHero->IsParty() == TRUE)
    {
        if (wait_time() == 0)
        {
            set_wait_time(1000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }

    return FALSE;
}
void MacroStateCreateRoom::SetMenuIndex(BYTE depth1, BYTE depth2, BYTE depth3)
{
    depth1_ = depth1;
    depth2_ = depth2;
    depth3_ = depth3;
}

//MacroStateJoinRoom
void MacroStateJoinRoom::Init()
{
#ifdef WRITE_MACRO_LOG
    WriteMacroLog("JoinRoom \n");
#endif //WRITE_MACRO_LOG

    last_map_code_ = g_pMap->GetMapID();

    MacroState::Init();
}
void MacroStateJoinRoom::Action()
{
    uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
    if (battlezone2_lobby)
    {
        battlezone2_lobby->SendGameStart();
    }
}
BOOL MacroStateJoinRoom::is_satisfied(DWORD tick_)
{
    if (last_map_code_ != g_pMap->GetMapID())
    {
        if (wait_time() == 0)
        {
            set_wait_time(5000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }

    return FALSE;
}

//MacroStateExitRoom
void MacroStateExitRoom::Init()
{
#ifdef WRITE_MACRO_LOG
    WriteMacroLog("ExitRoom \n");
#endif //WRITE_MACRO_LOG

    last_map_code_ = g_pMap->GetMapID();

    MacroState::Init();
}
void MacroStateExitRoom::Action()
{
    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager)
    {
        party_manager->LeaveParty();
    }    
}
BOOL MacroStateExitRoom::is_satisfied(DWORD tick_)
{
    if (last_map_code_ != g_pMap->GetMapID())
    {
        if (wait_time() == 0)
        {
            set_wait_time(5000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }

    return FALSE;
}

//MacroStateQuestAccept
void MacroStateQuestAccept::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "QuestAccept : %d\n", quest_code_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateQuestAccept::Action()
{
    uiQuestMan* quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (quest_manager)
    {
        quest_manager->NET_SEND_CG_QUEST_ACCEPT_SYN(static_cast<QCODE>(quest_code_));
    }
}
BOOL MacroStateQuestAccept::is_satisfied(DWORD tick_)
{
    /*
    uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
        if (quest_list_dialog == NULL)
        {
            return FALSE;
        }
    
        bool have_quest = false;
        for (UINT i = 0; i < quest_list_dialog->GetMainQuestCodes().size(); ++i)
        {
            if (quest_list_dialog->GetMainQuestCodes()[i] == quest_code_)
            {
                have_quest = true;
                break;
            }
        }
        for (UINT i = 0; i < quest_list_dialog->GetSubQuestCodes().size(); ++i)
        {
            if (quest_list_dialog->GetSubQuestCodes()[i] == quest_code_)
            {
                have_quest = true;
                break;
            }
        }*/
    
    bool have_quest = true; // ��� Ȯ���� �ʿ� ���� �� ����. ��Ŷ�� ������ ����.
    if (have_quest == true)
    {
        if (wait_time() == 0)
        {
            set_wait_time(500);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }
    
    return FALSE;
}

//MacroStateQuestReward
void MacroStateQuestReward::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "QuestReward : %d, %d\n", quest_code_, reward_index_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateQuestReward::Action()
{
    uiQuestMan* quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (quest_manager)
    {
        quest_manager->NET_SEND_CG_QUEST_REWARD_SYN(static_cast<QCODE>(quest_code_),static_cast<BYTE>(reward_index_));
    }
}
BOOL MacroStateQuestReward::is_satisfied(DWORD tick_)
{
    /*
    Quest* quest = g_pQMGR_Real->FindQuest(static_cast<QCODE>(quest_code_));
        if (quest == NULL)
        {
            quest = g_pQMGR_Temp->FindQuest(static_cast<QCODE>(quest_code_));
    
            if (quest == NULL)
            {
                // ����Ʈ �ڵ尡 ������ �ƴ�. �׳� �������� �ѱ���
                return TRUE;
            }
        }
    
        if (quest && quest->GetQState() == QS_COMPLETED)
        {
            if (wait_time() == 0)
            {
                set_wait_time(500);
            }
            else
            {
                return is_satisfied_wait_time(tick_);
            }
        }*/
    
    // ��� Ȯ���� �ʿ� ���� �� ����. ��Ŷ�� ������ ����.
    if (wait_time() == 0)
    {
        set_wait_time(500);
    }
    else
    {
        return is_satisfied_wait_time(tick_);
    }


    return FALSE;
}
//MacroStateAttack
void MacroStateAttack::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "Attack : %d, %d\n", target_monster_, fight_time_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    last_map_code_ = g_pMap->GetMapID();
    old_position_ = g_pHero->GetPosition();
    total_tick_count_ = 0;
    current_slot_number_ = 0;

    MacroState::Init();
}
void MacroStateAttack::Action()
{
    if ((fight_time_ != 0) &&
        (fight_time_ < total_tick_count_))
    {
        // �ð��� ������ ���� ��ġ�� �̵�
        g_HeroInput.SetAutoAttack(FALSE);
        g_HeroInput.OnLClickedPosition(&old_position_);
        return;
    }

    // ��ó Ÿ�� ����
    Monster* target = GetNearestTarget(target_monster_);
    if (target)
    {
        // Ÿ���� ������ ����
        g_HeroInput.SetCurrentTarget(target->GetObjectKey());
        g_HeroInput.ProcessTargeting();
        
        Attack(target);
    }
    else
    {
        // Ÿ���� ������ ���� ��ġ�� �̵�
        g_HeroInput.SetAutoAttack(FALSE);
        g_HeroInput.OnLClickedPosition(&old_position_);
    }
}
void MacroStateAttack::Process(DWORD tick_)
{
    tick_count_ += tick_;
    total_tick_count_ += tick_;

    if (tick_count_ > 1000)
    {
        Action();
        tick_count_ = 0;

        UsePotion();
        PickupItem();
    }
}
BOOL MacroStateAttack::is_satisfied(DWORD tick_)
{
    if (last_map_code_ != g_pMap->GetMapID())
    {
        return TRUE;
    }

    // ��ǥ�� �����ߴ���
    WzVector distance = g_pHero->GetPosition() - old_position_;
    if (VectorLength2(&distance) < 0.5f)
    {
        // �ð��� �� ������ ��
        if ((fight_time_ != 0) &&
            (fight_time_ < total_tick_count_))
        {
            return TRUE;
        }

        // �ֺ��� Ÿ���� ������ ��
        if (GetNearestTarget(target_monster_) == NULL)
        {
            return TRUE;
        }
    }

    return FALSE;
}
Monster* MacroStateAttack::GetNearestTarget(DWORD monster_code)
{
    // ��ó�� �ִ� ���� Ž��
    Monster* find_character = NULL;
    float find_character_distance = 999.9f;


    Object* object = NULL;
    Monster* character = NULL;
    Object_Map_Itr itr = g_ObjectManager.GetBegin();
    for (itr = g_ObjectManager.GetBegin() ; itr != g_ObjectManager.GetEnd() ; ++itr)
    {
        object = static_cast<Object*>(itr->second);
        if (object == NULL)
        {
            return NULL;
        }

        if (object->IsKindOfObject(MONSTER_OBJECT) == FALSE)
        {
            continue;
        }

        character = static_cast<Monster*>(object);

        if (monster_code != 0)
        {
            if (character->GetMonsterInfo()->m_MonsterCode != monster_code)
            {
                continue;
            }
        }
        
        // ���ΰ�?
        RELATIONKIND::TYPE relation_type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(character, FALSE);
        if (RELATIONKIND::Enemy == relation_type)
        {
            // ��� �ִ� ��
            if (character->IsDead() == TRUE)
            {
                continue;
            }

            if(character->IsObserverMode())
            {
                continue;
            }

            // ���̵� ��ų������̶�� Ÿ���úҰ�
            if (character->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
            {
                continue;
            }

            WzVector offset = g_pHero->GetPosition() - character->GetPosition();
            float distance = VectorLength(&offset);
            if (distance <= 30.0f &&fabs(offset.z) < 10.0f) //30.0f����?
            {
                // ���� ����� �� Ž��
                if (distance < find_character_distance)
                {
                    find_character_distance = distance;
                    find_character = character;
                }
            }
        }
    }

    return find_character;
}
void MacroStateAttack::Attack(Monster* target)
{
    // ��ų�� ���������
    if (g_HeroInput.GetSkillQueueSize() == 0)
    {
        // �̹��� ����� ��ų ������
        int skill_code = GetSkillCode();
        if (skill_code < 0)
        {
            // �� �Ǵ� ��ų�� ������ �������������
            g_HeroInput.OnSkill(35000);
            g_HeroInput.ProcessQueueAction();
            return;
        }

        // ��ų ���
        g_HeroInput.SetAutoAttack(FALSE);

        BASE_SKILLINFO* skill_info = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)skill_code);
        if (skill_info)
        {
            if (skill_info->m_byTarget == SKILL_TARGET_AREA)
            {
                g_HeroInput.OnSkill(skill_code);
                g_HeroInput.ForceAreaSkillPosition(skill_code, target->GetPosition());
                g_HeroInput.ProcessQueueAction();
            }
            else if ((skill_info->m_byTarget == SKILL_TARGET_FRIEND) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_FRIEND) ||
                (skill_info->m_byTarget == SKILL_TARGET_ME) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ME) ||
                (skill_info->m_byTarget == SKILL_TARGET_SUMMON))
            {
                g_HeroInput.SetCurrentTarget(g_pHero->GetObjectKey());
                g_HeroInput.ProcessTargeting();
                g_HeroInput.OnSkill(skill_code);
                g_HeroInput.ProcessQueueAction();
            }
            else if ((skill_info->m_byTarget == SKILL_TARGET_ENEMY) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY) ||
                (skill_info->m_byTarget == SKILL_TARGET_ENEMY_AND_ME))
            {
                g_HeroInput.OnSkill(skill_code);
                g_HeroInput.ProcessQueueAction();
            }
            else
            {
                // �ٸ� Ÿ�Ե��� �ϴ� ����.. ��ü��ų������..
            }
        }
    }
}

int MacroStateAttack::GetSkillCode()
{
    const int loof_count = 10;

    QuickContainer* quick_container =
        static_cast<QuickContainer*>(ItemManager::Instance()->GetItemContainerDialog(SI_QUICK));
    if (quick_container == NULL)
    {
        return -1;
    }

    for (int i = 0; i < loof_count; ++i)
    {
        // ������ ������ŭ ���ư��� ����� ��ų�� �˻�
        current_slot_number_ += 1;

        if (current_slot_number_ >= loof_count)
        {
            current_slot_number_ -= loof_count;
        }

        POSTYPE slot_pos = quick_container->GetRealSlotPos(current_slot_number_);

        // ���� ���� ����
        if (slot_pos > quick_container->GetMaxSlotNum())
        {
            continue;
        }
        if (quick_container->IsEmpty(slot_pos)) 
        {
            continue;
        }
        QuickSlot& quick_slot = static_cast<QuickSlot&>(quick_container->GetSlot(slot_pos));
        if (quick_slot.GetStatus() != SLOT_RENDER_STATE_ACTIVATED)
        {
            continue;
        }
        if (quick_slot.GetOrgSlotType() != ST_SKILL)
        {
            // �ϴ� ��ų�� ���
            continue;
        }

        int skill_code = xGetSkillData()->GetRealSkillCode(quick_slot.GetOrgCode());
        if (skill_code == INVALID_SKILL_CODE)
        {
            //�׼ǽ�ų ����
            continue;
        }

        BASE_SKILLINFO* skill_info = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)skill_code);
        if (skill_info)
        {
            if ((skill_info->m_byTarget == SKILL_TARGET_ENEMY) ||
                (skill_info->m_byTarget == SKILL_TARGET_FRIEND) ||
                (skill_info->m_byTarget == SKILL_TARGET_ME) ||
                (skill_info->m_byTarget == SKILL_TARGET_AREA) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_FRIEND) ||
                (skill_info->m_byTarget == SKILL_TARGET_REACHABLE_ME) ||
                (skill_info->m_byTarget == SKILL_TARGET_SUMMON) ||
                (skill_info->m_byTarget == SKILL_TARGET_ENEMY_AND_ME))
            {
                // ok
            }
            else
            {
                //SKILL_TARGET_FRIEND_CORPSE
                //SKILL_TARGET_ENEMY_PLAYER
                //SKILL_TARGET_ENEMY_CORPSE
                //SKILL_TARGET_AREA_ENEMY_CORPSE
                continue;
            }
        }

        if(g_pHero->CanUseSkill(skill_code) == FALSE)
        {
            continue;
        }

        return skill_code;
    }

    return -1;
}

//MacroStateWait
void MacroStateWait::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "Wait : %d\n", wait_time_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateWait::Action()
{
    return;
}
void MacroStateWait::Process(DWORD tick_)
{
    tick_count_ += tick_;

    if (tick_count_ > 1000)
    {
        tick_count_ = 0;

        UsePotion();
        PickupItem();
    }
}
BOOL MacroStateWait::is_satisfied(DWORD tick_)
{
    if (wait_time() == 0)
    {
        set_wait_time(wait_time_);
    }
    else
    {
        return is_satisfied_wait_time(tick_);
    }

    return FALSE;
}
//MacroStateJump
void MacroStateJump::Init()
{
#ifdef WRITE_MACRO_LOG
    WriteMacroLog("Jump\n");
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateJump::Action()
{
    g_pHero->m_JumpData.m_JumpFlag.AddForward();
    g_pHero->SetNextState(STATE::KEYBOARDJUMP,g_CurTime);
}
void MacroStateJump::Process(DWORD tick_)
{
    tick_count_ += tick_;
}
BOOL MacroStateJump::is_satisfied(DWORD tick_)
{
    // 1�ʰ� ������ ���� ���� �ƴϸ� TRUE
    if (tick_count_ >= 1000)
    {
        if (g_pHero->GetCurState() != STATE::KEYBOARDJUMP)
        {
            if (wait_time() == 0)
            {
                set_wait_time(500);
            }
            else
            {
                return is_satisfied_wait_time(tick_);
            }
        }
    }
    return FALSE;
}
//MacroStateMoveAttack
void MacroStateMoveAttack::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "MoveAttack : %.1f,%.1f,%.1f\n",
        target_position_.x, target_position_.y, target_position_.z);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroStateAttack::Init();

    // MacroStateAttack::Init() �Ŀ� old_position �� �ٲ��ָ� ��

    old_position_ = target_position_;
    fight_time_ = 0; 
}
void MacroStateMoveAttack::Action()
{
    MacroStateAttack::Action();
}
void MacroStateMoveAttack::Process(DWORD tick_)
{
    MacroStateAttack::Process(tick_);
}
BOOL MacroStateMoveAttack::is_satisfied(DWORD tick_)
{
    return MacroStateAttack::is_satisfied(tick_);
}
void MacroStateMoveAttack::SetTargetPosition(float x, float y, float z)
{
    target_position_.x = x;
    target_position_.y = y;
    target_position_.z = z;
}
//MacroStateTriggerClick
void MacroStateTriggerClick::Init()
{
#ifdef WRITE_MACRO_LOG
    WriteMacroLog("TriggerClick\n");
#endif //WRITE_MACRO_LOG

    MacroState::Init();
    last_map_code_ = g_pMap->GetMapID();

    // ���� ����� Ʈ���� ������Ʈ�� Ž��
    float distance = 30.0f; // 30�Ÿ� �ȿ���..
    MapObject* near_object_ = NULL;

    target_object_;
    Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
    for ( ; found_itr != end_itr; ++found_itr)
    {			
        Object* object = found_itr->second;
        if (object == NULL)
        {
            continue;
        }

        eOBJECT_TYPE eObjectType = object->GetSCObject().GetObjectType();
        if (eObjectType != MAP_OBJECT)
        {
            continue;
        }

        MapObject* map_object = static_cast<MapObject*>(object);
        
        if (map_object->IsTriggerObj() == FALSE)
        {
            continue;
        }
           
        if (map_object->GetShow() == FALSE)
        {
            continue;
        }

        if (map_object->CheckTriggerActive() == FALSE)
        {
            continue;
        }
        
        //if (pMapObject->IsTriggerOperating())

        float calc_distance = VectorLength(&(map_object->GetPosition() - g_pHero->GetVisiblePos()));
        if (calc_distance < distance)
        {
            distance = calc_distance;
            near_object_ = map_object;
        }
    }

    target_object_ = near_object_;
}
void MacroStateTriggerClick::Action()
{
    // �ش� ������Ʈ Ŭ��
    g_HeroInput.MoveToMapObjectWithAction(target_object_);
}
void MacroStateTriggerClick::Process(DWORD tick_)
{
    tick_count_ += tick_;

    if (tick_count_ > 5000)
    {
        Action();
        tick_count_ = 0;
    }

    return;
}
BOOL MacroStateTriggerClick::is_satisfied(DWORD tick_)
{
    if ((target_object_ == NULL) ||
        (last_map_code_ != g_pMap->GetMapID()))
    {
        return TRUE;
    }

    // ��ǥ�� �����ߴ���
    //
    WzBoundingVolume *bv = target_object_->GetBoundingVolume();
    float fMargin = 0.0f;
    if (bv)
    {
        switch(bv->m_eBvType)
        {
        case BVT_AABB:
            {
                WzVector diff = bv->m_aabb.m_wvMax - bv->m_aabb.m_wvMin;
                fMargin = max(diff.x,diff.y) / 2.0f;
            }
            break;
        case BVT_SPHERE:
            {
                fMargin = bv->m_BvSphere.m_fRadius;
            }
            break;                 
        }
    }

    // ���� MapObject GetPosition �½��ϴ� MapObject�� VisiblePos �Ⱦ��ϴ� �ٲ��� ������!!
    float fDistance = VectorLength(&(target_object_->GetPosition() - g_pHero->GetVisiblePos()));
    fDistance -= fMargin;

    if (fDistance <= DISTANCE_NPC_MEET)
    {
        if (wait_time() == 0)
        {
            set_wait_time(500);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }
  
    return FALSE;
}

//MacroStateCollectClick
void MacroStateCollectClick::Init()
{
#ifdef WRITE_MACRO_LOG
    WriteMacroLog("CollectClick\n");
#endif //WRITE_MACRO_LOG

    MacroState::Init();
    last_map_code_ = g_pMap->GetMapID();
    totol_tick_count_ = 0;

    // ���� ����� ä�� ������Ʈ�� Ž��
    float distance = 30.0f; // 30�Ÿ� �ȿ���..
    MapObject* near_object_ = NULL;

    target_object_;
    Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
    for ( ; found_itr != end_itr; ++found_itr)
    {			
        Object* object = found_itr->second;
        if (object == NULL)
        {
            continue;
        }

        eOBJECT_TYPE eObjectType = object->GetSCObject().GetObjectType();
        if (eObjectType != MAP_OBJECT)
        {
            continue;
        }

        MapObject* map_object = static_cast<MapObject*>(object);

        if (map_object->GetCollectionID() == INVALID_WZID)
        {
            continue;
        }

        if (map_object->GetShow() == FALSE)
        {
            continue;
        }

        const CollectionInfo* collect_info = CollectionParser::Instance()->FindCollectionInfo(map_object->GetCollectionID());
        if (collect_info == NULL)
        {
            continue;
        }

        float calc_distance = VectorLength(&(map_object->GetPosition() - g_pHero->GetVisiblePos()));
        if (calc_distance < distance)
        {
            distance = calc_distance;
            near_object_ = map_object;
        }
    }

    target_object_ = near_object_;
}
void MacroStateCollectClick::Action()
{
    // �ش� ������Ʈ Ŭ��
    if (g_pHero->GetCurState() != STATE::COLLECT)
    {
        g_HeroInput.MoveToMapObjectWithAction(target_object_);
    }
}
void MacroStateCollectClick::Process(DWORD tick_)
{
    tick_count_ += tick_;
    totol_tick_count_ += tick_;

    if (tick_count_ > 5000)
    {
        Action();
        tick_count_ = 0;
    }

    return;
}
BOOL MacroStateCollectClick::is_satisfied(DWORD tick_)
{
    if ((target_object_ == NULL) ||
        (last_map_code_ != g_pMap->GetMapID()))
    {
        return TRUE;
    }

    if (target_object_->GetCollectionID() == INVALID_WZID)
    {
        return TRUE;
    }

    return FALSE;
}
//MacroStateMissionReward
void MacroStateMissionReward::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "MissoinReward : %d\n", select_item_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateMissionReward::Action()
{
    RewardDialog *reward_dialog = (RewardDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_REWARD );
    if (reward_dialog)
    {
        MSG_CG_TRIGGER_REWARD_SYN SendPacket;
        SendPacket.m_Num = static_cast<WORD>(reward_dialog->GetRewardIndex());
        SendPacket.m_SelectedItem = select_item_;
        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
    }
}
BOOL MacroStateMissionReward::is_satisfied(DWORD tick_)
{
    if (wait_time() == 0)
    {
        set_wait_time(500);
    }
    else
    {
        return is_satisfied_wait_time(tick_);
    }

    return FALSE;
}
//MacroStateRuneStone
void MacroStateRuneStone::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "RuneStone : %d\n", index_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateRuneStone::Action()
{
    uiPersonalWayPointWindow* ui_waypoint = 
        GET_CAST_DIALOG(uiPersonalWayPointWindow, IM_PERSONAL_WAYPOINT_MANAGER::WAYPOINT_DIALOG);
    if (ui_waypoint)
    {
        ui_waypoint->SetSeleteIndex(index_);
        ui_waypoint->SEND_CG_CHAR_WAYPOINT_INFO_EXE_CMD();
    }
}
void MacroStateRuneStone::Process(DWORD tick_)
{
    tick_count_ += tick_;
}
BOOL MacroStateRuneStone::is_satisfied(DWORD tick_)
{
    if (tick_count_ < 1000)
    {
        // 1�� �� ���� üũ
        return FALSE;
    }

    CastLeaveFieldDialog* castleave_dialog = GET_CAST_DIALOG(CastLeaveFieldDialog, StrToWzID("0281"));    
    if (castleave_dialog == NULL)
    {
        return TRUE;
    }
 
    if (eDoType_PersonalWarp != castleave_dialog->GetDoType())
    {
        if (wait_time() == 0)
        {
            set_wait_time(5000);
        }
        else
        {
            return is_satisfied_wait_time(tick_);
        }
    }

    return FALSE;
}
//MacroStateDummyMemory
MacroStateDummyMemory::~MacroStateDummyMemory()
{
    if (memory_pointer_ != NULL)
    {
        delete memory_pointer_;
        memory_pointer_ = NULL;
    }
}
void MacroStateDummyMemory::Init()
{
#ifdef WRITE_MACRO_LOG
    TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(buffer, "DummyMemory : %d\n", alloc_size_);
    WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

    MacroState::Init();
}
void MacroStateDummyMemory::Action()
{
    if (memory_pointer_ != NULL)
    {
        for (DWORD i = 0; i < last_alloc_size_; ++i)
        {
            delete [] memory_pointer_[i];
            memory_pointer_[i] = NULL;
        }

        delete []memory_pointer_;
        memory_pointer_ = NULL;
    }

    if (alloc_size_ == 0)
    {
        return;
    }

    last_alloc_size_ = alloc_size_;
    memory_pointer_ = new BYTE*[alloc_size_];
    for (DWORD i = 0; i < alloc_size_; ++i)
    {
        memory_pointer_[i] = new BYTE[1024*1024];
    }
}
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM