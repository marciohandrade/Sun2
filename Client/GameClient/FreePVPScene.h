#pragma once

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "scenebase.h"
#include "battlescene.h"

class FreePVPScene : public BattleScene
{
public:
    FreePVPScene(void); // ������
    virtual ~FreePVPScene(void); // �Ҹ���

    virtual BOOL Init(); // �ʱ�ȭ, �ε�
    virtual BOOL Process(DWORD tick); // ���� ���
    virtual void Release(); // ����

    // ��Ʈ��ũ ��Ŷ ó��
    virtual void ParsePacket(MSG_BASE* message);
    virtual void OnRecvCG_SYNC(MSG_BASE* message);
    virtual void OnRecvCG_STATUS(MSG_BASE* message);

    virtual void ProcessKeyInput_Hero(DWORD tick);  ///	����� ���� ��ǲ ó�� 

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);

};

extern FreePVPScene g_FreePVPScene;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
