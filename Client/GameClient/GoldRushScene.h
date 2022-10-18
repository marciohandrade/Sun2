#pragma once

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "SceneBase.h"
#include "BattleScene.h"
#include "KillMessageDisplayManager.h"

class GoldRushScene : public BattleScene
{
public:
    GoldRushScene(void);
    virtual ~GoldRushScene(void);

    virtual BOOL Init();
    virtual BOOL Process(DWORD tick);
    virtual BOOL Render(DWORD tick);
    virtual BOOL PostInterfaceRender(DWORD tick);
    virtual void Release();    

    // Network
    virtual void ParsePacket(MSG_BASE* message);
    virtual void OnRecvCG_SYNC(MSG_BASE* message);
    virtual void OnRecvCG_STATUS(MSG_BASE* message);

    virtual void ProcessKeyInput_Hero(DWORD tick); /// 히어로 관련 인풋 처리

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object_ptr, BOOL force_enemy);
    void InsertKillMessage(TCHAR* first_user_id, DWORD first_user_color, TCHAR* second_user_id, DWORD second_user_color, TCHAR* format_string);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    void InsertMonsterKillMessage(TCHAR* first_user_id, DWORD first_user_color, TCHAR* monster_id, DWORD second_user_color, TCHAR* format_string);  // 골드러시 킬 메세지
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH    

private:
    KillMessageDisplayManager message_display_manager_;
};

extern GoldRushScene g_GoldRushScene;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH