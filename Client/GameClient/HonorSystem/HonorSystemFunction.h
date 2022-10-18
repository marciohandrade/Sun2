#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

namespace HonorSystem{

namespace HonorSystemFunction
{
    /// 타이틀 사용
    void UseTitleNameForHero(WORD title_code);

    // 플레이어 타이틀 적용
    void SetTitleName(DWORD player_key, WORD title_code);

    // 사용가능한 칭호인지 체크
    bool CheckValidTitleCodeForHero();

    // 타이틀이 변경됨 - 이벤트 처리
    void OnChangeTitle(WORD title_code);

    // 칭호 보상 - 이벤트처리
    void OnRewardTitle(WORD titile_code);
    // 칭호 추가/삭제 
    void OnAddTitle();
    void OnRemoveTitle();

    // 칭호 리스트 메뉴 갱신
    void RefreshTitleListMenu();

    // 명성/평판 메뉴 갱신
    void RefreshHonorPointListMenu();

    // 명성/평판 수치 변경 - 이벤트 처리
    void OnChangeFamePoint(WORD fame_code, DWORD changed_point);
    void OnChangeReputePoint(WORD repute_code, DWORD changed_point);

};//HonorSystemFunction

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM