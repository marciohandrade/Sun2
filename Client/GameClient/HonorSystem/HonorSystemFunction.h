#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

namespace HonorSystem{

namespace HonorSystemFunction
{
    /// Ÿ��Ʋ ���
    void UseTitleNameForHero(WORD title_code);

    // �÷��̾� Ÿ��Ʋ ����
    void SetTitleName(DWORD player_key, WORD title_code);

    // ��밡���� Īȣ���� üũ
    bool CheckValidTitleCodeForHero();

    // Ÿ��Ʋ�� ����� - �̺�Ʈ ó��
    void OnChangeTitle(WORD title_code);

    // Īȣ ���� - �̺�Ʈó��
    void OnRewardTitle(WORD titile_code);
    // Īȣ �߰�/���� 
    void OnAddTitle();
    void OnRemoveTitle();

    // Īȣ ����Ʈ �޴� ����
    void RefreshTitleListMenu();

    // ��/���� �޴� ����
    void RefreshHonorPointListMenu();

    // ��/���� ��ġ ���� - �̺�Ʈ ó��
    void OnChangeFamePoint(WORD fame_code, DWORD changed_point);
    void OnChangeReputePoint(WORD repute_code, DWORD changed_point);

};//HonorSystemFunction

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM