#ifndef SERVER_GAMESERVER_CHARACTERINFOEXMANAGER_H
#define SERVER_GAMESERVER_CHARACTERINFOEXMANAGER_H

#include <CGuid.h> // error C2065: 'GUID_NULL'
#include <atltime.h>
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include <AttendanceManager.h>
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
#include <MiniGameManager.h>
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

class CharacterInfoExManager
{
public:
    CharacterInfoExManager();
    ~CharacterInfoExManager();

    void Init(Player* _player);
    void OnEnterField();
    void OnLeaveField();

    ToggleItemInfos& GetToggleItemInfos();
    nsSlot::TOGGLE_ITEMS* GetToggleItemInfo(const ToggleItemType toggle_item_type);
    void TurnOnOffToggleItems(bool turn_on);
#ifdef _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE
    void TurnOnOffToggleItems(const eITEMTYPE item_type, bool turn_on);

    const POSTYPE UnEquipBadge();    //장착중인 뱃지를 해제
    VOID EquipBadge(POSTYPE item_pos);
#endif //_NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE

    const BattleGroundRecordInfos& GetBattleGroundRecordInfos() const;
    void SetBattleGroundRecordInfos(const BattleGroundRecordInfos& battle_ground_record_infos);

    BattleGroundRecord* GetBattleGroundRecord(const int index);

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    inline const GoldRushRecordInfos& GetGoldRushRecordInfos() const {return goldrush_record_infos_; }
    void SetGoldRushRecordInfos(const GoldRushRecordInfos& goldrush_record_infos);
    GoldRushRecord* GetGoldRushRecord(const int index);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

    //!~ _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    CTime GetDailyQuestResetUseTime();
    void  SetDailyQuestResetUseTime(CTime daily_quest_reset_time);
    //~! _NA_000000_20130521_DAILY_QUEST_RESET_ITEM

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    const AttendanceInfo& GetAttendanceInfos() { return attendance_manager.GetAttendanceInfos(); }
    void SetAttendanceInfos(const AttendanceInfo& attendance_infos);
    AttendanceManager& GetAttendanceManager() { return attendance_manager; }
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    MiniGameManager& GetMiniGameManager() { return minigame_manager; }

    const MiniGameBingoInfos& GetMiniGameBingoInfos() { return minigame_manager.GetMiniGameBingoInfos(); }
    void SetMiniGameBingoInfos(MiniGameBingoInfos& minigame_bingo_infos);
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    
private:
    Player* player_;
    
    struct
    {
        ToggleItemInfos toggle_item_infos_;
        BattleGroundRecordInfos battle_ground_record_infos_; 
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        GoldRushRecordInfos goldrush_record_infos_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
        CTime previous_daily_quest_reset_time; //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        MiniGameManager minigame_manager;
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    };

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    struct  
    {
        // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        AttendanceManager attendance_manager;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    }; // user_extra_info_;
#endif // _NA_007932_20150106_USER_EXTRA_INFO

};
#endif //SERVER_GAMESERVER_CHARACTERINFOEXMANAGER_H