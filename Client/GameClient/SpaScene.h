#pragma once

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "scenebase.h"
#include "battlescene.h"


class SpaScene : public BattleScene
{
public:
    SpaScene(void); // ������
    virtual ~SpaScene(void); // �Ҹ���

    virtual BOOL Init(); // �ʱ�ȭ, �ε�
    virtual BOOL Process(DWORD tick); // ���� ���
    virtual BOOL Render(DWORD tick); //	���� ������
    virtual void Release(); // ����

    // ��Ʈ��ũ ��Ŷ ó��
    virtual void ParsePacket(MSG_BASE* message);

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);
};

extern SpaScene g_SpaScene;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM