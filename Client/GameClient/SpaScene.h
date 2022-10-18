#pragma once

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "scenebase.h"
#include "battlescene.h"


class SpaScene : public BattleScene
{
public:
    SpaScene(void); // 생성자
    virtual ~SpaScene(void); // 소멸자

    virtual BOOL Init(); // 초기화, 로딩
    virtual BOOL Process(DWORD tick); // 메인 계산
    virtual BOOL Render(DWORD tick); //	메인 랜더링
    virtual void Release(); // 종료

    // 네트워크 패킷 처리
    virtual void ParsePacket(MSG_BASE* message);

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);
};

extern SpaScene g_SpaScene;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM