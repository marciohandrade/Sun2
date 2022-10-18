#ifndef SUN_GAMESERVER_GAMEBATTLERESULT_H_
#define SUN_GAMESERVER_GAMEBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT


/* 클래스 다이어그램 : 2010.03.18 by 남기봉

                            BattlePlayer <*>---> BattleRecorder
                                         <m>---> Player
                                  A
                                  |
                                <*:n>
        GameZone <*>-----> GameBattleResult <--- CHuntingBattleResult
            A                                 |
            |                                 |- DominationBattleResult
            |                                 |
            |                                 |- InstanceDungeonBattleResult
            |                                 |
            |                                 |- MissionBattleResult
            |                                 |
            |                                 |- PVPBattleResult
           < > 
CG_ZONE_PLAYER_BATTLE_RESULT_SYN

*/

/* class GameBattleResult : 2010.03.18 by 남기봉

- GameZone 에 [추가/삭제]된 Player들을 해시맵으로 관리하며, 클라이언트의 전투 상황 요청시
  Player의 전투 상황이 기록되는 class BattleRecord 값을 참조하여 전투결과값을 보낸다.

- class BattlePlayer는 클라이언트에서 Player들의 전투결과 요청시 변경된[추가/삭제/정보] 정보만 
  보내기 위해 [추가/삭제/정보]상태를 관리하는 Hash Map을 사용하며 이를 'CheckKeyHash'이라 한다.

                  < Check Key Hash Map >
                    key          value
  BattlePlayer --- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- ...            ...

- Player 의 Zone에 추가시 처음 한번 해시맵에 생성후 Zone의 파괴시 제거한다.
  이는 접속 유저의 종료 후 새로운 접속에 따른 Player가 가지는 전투 상황값의 초기화 시점의 문제 해결을
  위한 방안으로 좀더 나은 대책이 나오면 변경하도록 한다.

*/

#include "BattleRecorder.h"

class Player;
class Character;

class UpdateSelector
{
public:
    struct UpdateKey
    {
        uint match_number;
        uint dirty_number;
    };

    UpdateSelector() : update_key_(0) {}
    ~UpdateSelector() {}

    void ResetUpdateSelector() 
    { 
        update_key_ = 0; 
    }
    
    void ResetUpdateKey(UpdateKey& updatekey)
    {
        updatekey.match_number = 0;
        updatekey.dirty_number = 0;
    }

    bool IsUpdated(UpdateKey& source, UpdateKey& target)
    {
        return (source.match_number >= target.dirty_number) ? true : false;
    }
    
    void DoUpdate(UpdateKey& updatekey)
    {
        updatekey.match_number = update_key_;
    }

    void DoDirty(UpdateKey& updatekey)
    {
        ++update_key_;
        updatekey.dirty_number = update_key_;
    }

private:
    uint update_key_;
};




class GameBattleResult
{
public:
    enum
    {
        HOME_TEAM = 0,
        AWAY_TEAM = 1,
    };

    GameBattleResult(void);
    virtual ~GameBattleResult(void);

    bool Init(bool enable_pvp);
    void Release();
    void Clear();

    virtual bool Enter(Player* player);
    virtual void Leave(Player* player, bool abnormal_disconnect);
    virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);
    
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    void DoRecord(Character* character, eBATTLE_RECORD_TYPE record_type, int impact_value);
#else
    void DoRecord(Character* character, eBATTLE_RECORD_TYPE record_type, uint impact_value);
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_001990_ACSYSTEM_ADD
    const BattleRecorder::Record* GetPlayerBattleResult(Player* player);
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    void AddToTalDropHeim(MONEY add_heim);
    MONEY GetTotalDropHeim() const;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG 
protected:
    struct BattlePlayer
    {
        //Player*         player;
        CHARGUID         char_guid; //player->GetCharGuid();
        int                team;
        BattleRecorder  battle_recorder;
        UpdateSelector::UpdateKey  update_key;    

        BattlePlayer()
        {
            char_guid = 0; 
            team = 0;
            ZeroMemory(&battle_recorder, sizeof(battle_recorder));
            ZeroMemory(&update_key, sizeof(update_key));
        }
    };

    GameBattleResult::BattlePlayer* FindBattlePlayer(CHARGUID character_guid);

    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    virtual void NotifyEnterMessage(GameBattleResult::BattlePlayer* battle_player);
    virtual void NotifyLeaveMessage(GameBattleResult::BattlePlayer* battle_player, bool abnormal_disconnect);
#else 
    void NotifyEnterMessage(GameBattleResult::BattlePlayer* battle_player);
    void NotifyLeaveMessage(GameBattleResult::BattlePlayer* battle_player, bool abnormal_disconnect);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    void ReleaseBattlePlayer();

    typedef STLX_HASH_MAP<CHARGUID, GameBattleResult::BattlePlayer*> BattlePlayerHash;
    typedef util::CMemoryPoolFactory<GameBattleResult::BattlePlayer> BattlePlayerPool;

    BattlePlayerHash battle_player_hash_;
    BattlePlayerPool battle_player_pool_;

    UpdateSelector  update_selector_;
    bool            enable_pvp_;
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    MONEY           total_droped_heim_;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG 
};


//---------------------------------------------------------------------------------
inline GameBattleResult::BattlePlayer* GameBattleResult::FindBattlePlayer(CHARGUID character_guid)
{
    if (battle_player_hash_.empty())
        return 0;

    BattlePlayerHash::iterator iter = battle_player_hash_.find(character_guid);
    if (iter != battle_player_hash_.end())
        return iter->second;

    return 0;
}

#endif //SUN_GAMESERVER_GAMEBATTLERESULT_H_
