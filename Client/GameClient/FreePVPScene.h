#pragma once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "scenebase.h"
#include "battlescene.h"

class FreePVPScene : public BattleScene
{
public:
    FreePVPScene(void); // 생성자
    virtual ~FreePVPScene(void); // 소멸자

    virtual BOOL Init(); // 초기화, 로딩
    virtual BOOL Process(DWORD tick); // 메인 계산
    virtual void Release(); // 종료

    // 네트워크 패킷 처리
    virtual void ParsePacket(MSG_BASE* message);
    virtual void OnRecvCG_SYNC(MSG_BASE* message);
    virtual void OnRecvCG_STATUS(MSG_BASE* message);

    virtual void ProcessKeyInput_Hero(DWORD tick);  ///	히어로 관련 인풋 처리 

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);

};

extern FreePVPScene g_FreePVPScene;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
