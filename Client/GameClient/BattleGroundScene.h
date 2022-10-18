#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "scenebase.h"
#include "battlescene.h"
#include "KillMessageDisplayManager.h"

class KillMessageDisplayManager;

class BattleGroundScene : public BattleScene
{
public:
    BattleGroundScene(void); // ������
    virtual ~BattleGroundScene(void); // �Ҹ���

    virtual BOOL Init(); // �ʱ�ȭ, �ε�
    virtual BOOL Process(DWORD tick); // ���� ���
    virtual BOOL Render(DWORD tick); //	���� ������
    virtual BOOL PostInterfaceRender(DWORD tick); //  �������̽� ���� �׸���(2D)
    virtual void Release(); // ����

    // ��Ʈ��ũ ��Ŷ ó��
    virtual void ParsePacket(MSG_BASE* message);
    virtual void OnRecvCG_SYNC(MSG_BASE* message);
    virtual void OnRecvCG_STATUS(MSG_BASE* message);

    virtual void ProcessKeyInput_Hero(DWORD tick);  ///	����� ���� ��ǲ ó�� 

    virtual RELATIONKIND::TYPE GetRelationKindOfCharacter(Character* object, BOOL force_enemy);
    void InsertKillMessage(TCHAR* first_user_id, DWORD first_user_color, TCHAR* second_user_id, DWORD second_user_color, TCHAR* format_string);
    
private:
    KillMessageDisplayManager message_display_manager_;
};

extern BattleGroundScene g_BattleGroundScene;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND