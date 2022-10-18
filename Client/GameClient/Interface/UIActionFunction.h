#pragma once

namespace UIActionFunction
{
    /// 캐릭터정보창 토글
    void ToggleCharacterStatusUI();

    /// 인벤토리창 토글
    void ToggleInventoryUI();

    /// 인벤토리확장 토글
    void ToggleExtendInventoryUI();

    /// 스킬창 토글
    void ToggleSkillUI();

    /// 퀘스트창 토글
    void ToggleQuestUI();

    /// 길드창 토글
    void ToggleGuildUI();

    /// 커뮤니티창 토글
    void ToggleCommunityUI();

    /// AC창 토글
    void ToggleACUI();

    /// 펫정보창 토글
    void TogglePetUI();

    /// 시스템메뉴 및 취소
    void ShowSystemMenuNCancel();

    /// 환경설정창 토글
    void ToggleSystemMenuUI();

    /// 대륙맵 토글
    void ToggleContinentMap();

    /// 전투정보창 토글
    void ToggleBattleScoreUI();

    /// 전체맵 모드 변경
    void ToggleFullRadarMode();

    /// 캐시샵창 토글
    void ToggleCashShopUI();

    /// 정보열람실 토글
    void ToggleGuideInfomationUI();

    /// 미션과제창 토글
    void ToggleMissionObjectUI();

    /// 퀵슬롯 확장 토글
    void ToggleQuickSlotExtend();

    /// 퀵슬롯 라인 체인지
    void ExchangeQuickSlot();

    /// 소환수 명령
    void CommandSummon(int command_index);

    /// 소환수 스킬사용
    void UseSummonSkill(int skill_index);
}