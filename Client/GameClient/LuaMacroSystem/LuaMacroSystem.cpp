#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

#include "LuaMacroSystem.h"


#include "GameFramework.h"
#include "SceneBase.h"
#include "GlobalFunc.h"

#ifdef WRITE_MACRO_LOG
  #include <Psapi.h>
  #include <TlHelp32.h>
  #pragma comment(lib,"psapi.lib")
#endif //WRITE_MACRO_LOG

MacroSystem::MacroSystem()
{
    script_stack_.clear();
    macro_script_memorypool.Initialize(20);

    current_macro_ = NULL;

    for (int i = 0; i < Macro_Max; i++)
    {
        macro_states_[i] = NULL;    
    }

    write_log_ = FALSE;

    macro_states_[Macro_RunMacro]= new MacroStateRunMacro();
    macro_states_[Macro_EndMacro]= new MacroStateEndMacro();
    macro_states_[Macro_Move]    = new MacroStateMove();
    macro_states_[Macro_MoveMap] = new MacroStateMoveMap();
    macro_states_[Macro_NpcMeet] = new MacroStateNpcMeet();
    macro_states_[Macro_CreateRoom] = new MacroStateCreateRoom();
    macro_states_[Macro_JoinRoom] = new MacroStateJoinRoom();
    macro_states_[Macro_ExitRoom] = new MacroStateExitRoom();

    macro_states_[Macro_QuestAccept] = new MacroStateQuestAccept();
    macro_states_[Macro_QuestReward] = new MacroStateQuestReward();
    macro_states_[Macro_Attack] = new MacroStateAttack();
    macro_states_[Macro_Wait]     = new MacroStateWait();
    macro_states_[Macro_Jump]     = new MacroStateJump();
    macro_states_[Macro_MoveAttack] = new MacroStateMoveAttack();

    macro_states_[Macro_TriggerClick] = new MacroStateTriggerClick();
    macro_states_[Macro_CollectClick] = new MacroStateCollectClick();
    macro_states_[Macro_MissionReward] = new MacroStateMissionReward();

    macro_states_[Macro_RuneStone] = new MacroStateRuneStone();
    macro_states_[Macro_DummyMemory] = new MacroStateDummyMemory();

    macro_states_[Macro_Max]     = NULL;

    clear_pickup_item();

#ifdef WRITE_MACRO_LOG
    WriteMacroLog("-------------  GameStart -------------\n", TRUE);
#endif //WRITE_MACRO_LOG
}

MacroSystem::~MacroSystem()
{
    macro_script_memorypool.Release();

    for (int i = 0; i < Macro_Max; i++)
    {
        if (macro_states_[i])
        {
            delete macro_states_[i];
            macro_states_[i] = NULL;
        }
    }

#ifdef WRITE_MACRO_LOG
    WriteMacroLog("-------------   GameEnd  -------------\n", TRUE);
#endif //WRITE_MACRO_LOG
}

void MacroSystem::Process(DWORD tick_)
{
    tick_count_ += tick_;
    if (tick_count_ > 30000)
    {
        WriteMemoryInfo();
        tick_count_ = 0;
    }

    if (GameFramework::GetCurrentScene() &&
        GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_LOAD)
    {
        return;
    }

    if (current_macro_)
    {
        current_macro_->Process(tick_);

        if (current_macro_->is_satisfied(tick_))
        {
            current_macro_ = NULL;
            NextMacro();
        }
    }
}
void MacroSystem::NextMacro()
{
    if (script_stack_.empty())
    {
        // 모든 스크립트 완료
        return;
    }

    MacroScript* current_script = script_stack_.front();

    if (current_script)
    {
        MacroCommand command = current_script->PopMacroCommand();
        ExecuteCommand(command);
    }
}
void MacroSystem::ResetMacro()
{
    while (!script_stack_.empty())
    {
        MacroScript* current_script = script_stack_.front();

        if (current_script)
        {
            while (1)
            {
                MacroCommand command = current_script->PopMacroCommand();
                if (command.command_type_ == Macro_EndMacro)
                {
                    break;
                }
            }
        }
        script_stack_.pop_front();
    }

    current_macro_ = NULL;

    clear_pickup_item();

    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "매크로를 리셋합니다."); 

#ifdef WRITE_MACRO_LOG
    WriteMacroLog("-- ResetMacro -- \n");
#endif //WRITE_MACRO_LOG
}
void MacroSystem::InsertCommand(MacroCommand command)
{
    if (script_stack_.empty())
    {
#ifdef WRITE_MACRO_LOG
        TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
        sprintf(buffer, "-- FirstMacroStart : %s -- \n", command.value.file_name_);
        WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

       ExecuteCommand(command);
       return;
    }

    MacroScript* current_script = script_stack_.front();
    if (current_script)
    {
        current_script->InsertMacroCommand(command);
    }
}
void MacroSystem::ExecuteCommand(MacroCommand command)
{
    // 실제 명령을 실행하는 곳
    switch (command.command_type_)
    {
    case Macro_RunMacro:
        {
            MacroScript* macro_script = static_cast<MacroScript*>(macro_script_memorypool.Alloc());
            macro_script->SetFileName(command.value.file_name_);

            script_stack_.push_front(macro_script);

            macro_script->CallScript();

            current_macro_ = macro_states_[Macro_RunMacro];

#ifdef WRITE_MACRO_LOG
            TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
            sprintf(buffer, "RunMacro : %s\n", command.value.file_name_);
            WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG
        }
        break;
    case Macro_EndMacro:
        {
            // 스택에서 제거
            if (script_stack_.empty())
            {
#ifdef WRITE_MACRO_LOG
                WriteMacroLog("-- EndAllMacro -- \n");
#endif //WRITE_MACRO_LOG

                return;
            }

            MacroScript* macro_script = script_stack_.front();

#ifdef WRITE_MACRO_LOG
            TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
            sprintf(buffer, "EndMacro : %s\n", macro_script->GetFileName());
            WriteMacroLog(buffer);
#endif //WRITE_MACRO_LOG

            script_stack_.pop_front();

            macro_script_memorypool.Free(macro_script);

            current_macro_ = macro_states_[Macro_EndMacro];
        }
        break;
    case Macro_Move:
        {
            MacroStateMove* macro_state = static_cast<MacroStateMove*>(macro_states_[Macro_Move]);
            if (macro_state)
            {
                macro_state->SetTargetPosition(
                    command.value.MovePosition.x_,
                    command.value.MovePosition.y_,
                    command.value.MovePosition.z_
                    );

                current_macro_ = macro_states_[Macro_Move];
            }
        }
        break;
    case Macro_MoveMap:
        {
            MacroStateMoveMap* macro_state = static_cast<MacroStateMoveMap*>(macro_states_[Macro_MoveMap]);
            if (macro_state)
            {
                macro_state->SetMapCode(command.value.map_code_);

                current_macro_ = macro_states_[Macro_MoveMap];
            }
        }
        break;
    case Macro_NpcMeet:
        {
            MacroStateNpcMeet* macro_state = static_cast<MacroStateNpcMeet*>(macro_states_[Macro_NpcMeet]);
            if (macro_state)
            {
                macro_state->SetNpcCode(command.value.npc_code_);

                current_macro_ = macro_states_[Macro_NpcMeet];
            }
        }
        break;
    case Macro_CreateRoom:
        {
            MacroStateCreateRoom* macro_state = static_cast<MacroStateCreateRoom*>(macro_states_[Macro_CreateRoom]);
            if (macro_state)
            {
                macro_state->SetMenuIndex(
                    command.value.CreateRoomCommand.depth1_,
                    command.value.CreateRoomCommand.depth2_,
                    command.value.CreateRoomCommand.depth3_);

                current_macro_ = macro_states_[Macro_CreateRoom];
            }
        }
        break;
    case Macro_JoinRoom:
        {
            MacroStateJoinRoom* macro_state = static_cast<MacroStateJoinRoom*>(macro_states_[Macro_JoinRoom]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_JoinRoom];
            }
        }
        break;
    case Macro_ExitRoom:
        {
            MacroStateExitRoom* macro_state = static_cast<MacroStateExitRoom*>(macro_states_[Macro_ExitRoom]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_ExitRoom];
            }
        }
        break;
        //
    case Macro_QuestAccept:
        {
            MacroStateQuestAccept* macro_state = static_cast<MacroStateQuestAccept*>(macro_states_[Macro_QuestAccept]);
            if (macro_state)
            {
                macro_state->SetQuestCode(command.value.quest_code_);

                current_macro_ = macro_states_[Macro_QuestAccept];
            }
        }
        break;
    case Macro_QuestReward:
        {
            MacroStateQuestReward* macro_state = static_cast<MacroStateQuestReward*>(macro_states_[Macro_QuestReward]);
            if (macro_state)
            {
                macro_state->SetQuestCode(command.value.QuestReward.quest_code_);
                macro_state->SetRewardIndex(command.value.QuestReward.reward_index_);

                current_macro_ = macro_states_[Macro_QuestReward];
            }
        }
        break;
    case Macro_Attack:
        {
            MacroStateAttack* macro_state = static_cast<MacroStateAttack*>(macro_states_[Macro_Attack]);
            if (macro_state)
            {
                macro_state->SetMonsterCode(command.value.Attack.monster_code_);
                macro_state->SetTime(command.value.Attack.fight_time_);

                current_macro_ = macro_states_[Macro_Attack];
            }
        }
        break;
    case Macro_Wait:
        {
            MacroStateWait* macro_state = static_cast<MacroStateWait*>(macro_states_[Macro_Wait]);
            if (macro_state)
            {
                macro_state->SetWaitTime(command.value.wait_time_);

                current_macro_ = macro_states_[Macro_Wait];
            }
        }
        break;
    case Macro_Jump:
        {
            MacroStateJump* macro_state = static_cast<MacroStateJump*>(macro_states_[Macro_Jump]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_Jump];
            }
        }
        break;
    case Macro_MoveAttack:
        {
            MacroStateMoveAttack* macro_state = static_cast<MacroStateMoveAttack*>(macro_states_[Macro_MoveAttack]);
            if (macro_state)
            {
                macro_state->SetMonsterCode(command.value.MoveAttack.monster_code_);
                macro_state->SetTime(0);
                macro_state->SetTargetPosition(
                    command.value.MoveAttack.x_,
                    command.value.MoveAttack.y_,
                    command.value.MoveAttack.z_
                    );

                current_macro_ = macro_states_[Macro_MoveAttack];
            }
        }
        break;
    case Macro_TriggerClick:
        {
            MacroStateTriggerClick* macro_state = static_cast<MacroStateTriggerClick*>(macro_states_[Macro_TriggerClick]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_TriggerClick];
            }
        }
        break;
    case Macro_CollectClick:
        {
            MacroStateCollectClick* macro_state = static_cast<MacroStateCollectClick*>(macro_states_[Macro_CollectClick]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_CollectClick];
            }
        }
        break;
    case Macro_MissionReward:
        {
            MacroStateMissionReward* macro_state = static_cast<MacroStateMissionReward*>(macro_states_[Macro_MissionReward]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_MissionReward];
                macro_state->set_select_item(command.value.select_item_);
            }
        }
        break;
    case Macro_RuneStone:
        {
            MacroStateRuneStone* macro_state = static_cast<MacroStateRuneStone*>(macro_states_[Macro_RuneStone]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_RuneStone];
                macro_state->set_index(command.value.index_);
            }
        }
        break;
    case Macro_DummyMemory:
        {
            MacroStateDummyMemory* macro_state = static_cast<MacroStateDummyMemory*>(macro_states_[Macro_DummyMemory]);
            if (macro_state)
            {
                current_macro_ = macro_states_[Macro_DummyMemory];
                macro_state->set_alloc_size(command.value.size_);
            }
        }
        break;
    }

    current_macro_->Init();
    current_macro_->Action();

}

void MacroSystem::WriteMemoryInfo()
{
#ifdef WRITE_MACRO_LOG
    PROCESS_MEMORY_COUNTERS memomy_counters;
    memset(&memomy_counters,0,sizeof(memomy_counters));

    //snapshot정보
    HANDLE	snap_shot;
    snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snap_shot == NULL)
    {
        return;
    }

    TCHAR GameName[256];
    memset(GameName,0,sizeof(TCHAR)*256);
#if defined(_DEBUG)
    sprintf(GameName,"%s","GameClient_Debug.exe");
#elif defined(_GAMECLIENT_VER)
    sprintf(GameName,"%s","GameClient.exe");
#else
    sprintf(GameName,"%s","Sungame.exe");
#endif 

    PROCESSENTRY32 process_entry;
    process_entry.dwSize = sizeof(process_entry);
    BOOL process_found = FALSE;
    process_found = Process32First(snap_shot,&process_entry);
    while (process_found)
    {
        if (strcmp(process_entry.szExeFile, GameName) == 0)
        {
            //이건 내 프로세스
            HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, false, process_entry.th32ProcessID);
            if (hProcess)
            {
                GetProcessMemoryInfo(hProcess,&memomy_counters,sizeof(memomy_counters));
                break;
            }
            break;
        }
        process_found = Process32Next(snap_shot,&process_entry);
    }

    if (memomy_counters.WorkingSetSize)
    {
        TCHAR buffer[INTERFACE_STRING_LENGTH] = {0,};
        sprintf(buffer, " [ UseMemory : %5d MB ]\n", (memomy_counters.WorkingSetSize/1024/1024));
        WriteMacroLog(buffer);
    }
#endif //WRITE_MACRO_LOG
}
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM