#pragma once
#include <Singleton.h>
#include "HotKeyDefine.h"

class KeyCommand
{
public:
    KeyCommand() {}
    virtual ~KeyCommand() {}
    
    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair) = 0;
};

//////////////////////////////////////////////////////////////////////////

class DialogToggleCommand : public KeyCommand, public util::Singleton<DialogToggleCommand>
{
    typedef void (*ActionFunction)();
    typedef STLX_MAP<HOT_KEY_FUNCTION_INDEX, ActionFunction> ActionFunctionMap;
public:
    DialogToggleCommand() {}
    virtual ~DialogToggleCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 

    void RegisterActionFunction(HOT_KEY_FUNCTION_INDEX hot_key_function, ActionFunction action_function_ptr);

private:
    ActionFunctionMap& action_function_container() { return action_function_container_; }

private:
    ActionFunctionMap action_function_container_;
};

class SystemMenuNCancelCommand : public KeyCommand, public util::Singleton<SystemMenuNCancelCommand>
{
public:
    SystemMenuNCancelCommand() {}
    virtual ~SystemMenuNCancelCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class QuickSlotModeCommand : public KeyCommand, public util::Singleton<QuickSlotModeCommand>
{
public:
    QuickSlotModeCommand() {}
    virtual ~QuickSlotModeCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class QuickSlotUseCommand : public KeyCommand, public util::Singleton<QuickSlotUseCommand>
{
public:
    QuickSlotUseCommand() {}
    virtual ~QuickSlotUseCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class SummonControlCommand : public KeyCommand, public util::Singleton<SummonControlCommand>
{
public:
    SummonControlCommand() {}
    virtual ~SummonControlCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class TargetEnemyCommand : public KeyCommand, public util::Singleton<TargetEnemyCommand>
{
public:
    TargetEnemyCommand() {}
    virtual ~TargetEnemyCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class TargetPartyMemberCommand : public KeyCommand, public util::Singleton<TargetPartyMemberCommand>
{
public:
    TargetPartyMemberCommand() {}
    virtual ~TargetPartyMemberCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class ScreenShotCommand : public KeyCommand, public util::Singleton<ScreenShotCommand>
{
public:
    ScreenShotCommand() {}
    virtual ~ScreenShotCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class ChatOpenNSendCommand : public KeyCommand, public util::Singleton<ChatOpenNSendCommand>
{
public:
    ChatOpenNSendCommand() {}
    virtual ~ChatOpenNSendCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class CameraControlCommand : public KeyCommand, public util::Singleton<CameraControlCommand>
{
public:
    CameraControlCommand() {}
    virtual ~CameraControlCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class HeroAutoRunCommand : public KeyCommand, public util::Singleton<HeroAutoRunCommand>
{
public:
    HeroAutoRunCommand() {}
    virtual ~HeroAutoRunCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class HeroMoveCommand : public KeyCommand, public util::Singleton<HeroMoveCommand>
{
public:
    HeroMoveCommand() {}
    virtual ~HeroMoveCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class HeroJumpCommand : public KeyCommand, public util::Singleton<HeroJumpCommand>
{
public:
    HeroJumpCommand() {}
    virtual ~HeroJumpCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class HeroActionSkillCommand : public KeyCommand, public util::Singleton<HeroActionSkillCommand>
{
public:
    HeroActionSkillCommand() {}
    virtual ~HeroActionSkillCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

class HeroSpreadWingCommand : public KeyCommand, public util::Singleton<HeroSpreadWingCommand>
{
public:
    HeroSpreadWingCommand() {}
    virtual ~HeroSpreadWingCommand() {}

    virtual void OnAction(HOT_KEY_FUNCTION_INDEX hot_key_function, KEY_BIND_PAIR key_bind_pair); 
};

