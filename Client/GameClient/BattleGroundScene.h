#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "scenebase.h"
#include "battlescene.h"
#include "KillMessageDisplayManager.h"

class KillMessageDisplayManager;

class BattleGroundScene : public BattleScene
{
public:
    BattleGroundScene(void); // 생성자
    virtual ~BattleGroundScene(void); // 소멸자

    virtual BOOL Init(); // 초기화, 로딩
    virtual BOOL Process(DWORD tick); // 메인 계산
    virtual BOOL Render(DWORD tick); //	메인 랜더링
    virtual BOOL PostInterfaceRender(DWORD tick); //  인터페이스 이후 그릴것(2D)
    virtual void Release(); // 종료

    // 네트워크 패킷 처리
    virtual void ParsePacket(MSG_BASE* message);
    virtual void OnRecvCG_SYNC(MSG_BASE* message);
    virtual void OnRecvCG_STATUS(MSG_BASE* message);

    virtual void ProcessKeyInput_Hero(DWORD tick);  ///	히어로 관련 인풋 처리 

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);
    void InsertKillMessage(TCHAR* first_user_id, DWORD first_user_color, TCHAR* second_user_id, DWORD second_user_color, TCHAR* format_string);
    
private:
    KillMessageDisplayManager message_display_manager_;
};

extern BattleGroundScene g_BattleGroundScene;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND