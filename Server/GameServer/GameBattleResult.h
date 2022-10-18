#ifndef SUN_GAMESERVER_GAMEBATTLERESULT_H_
#define SUN_GAMESERVER_GAMEBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT


/* Ŭ���� ���̾�׷� : 2010.03.18 by �����

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

/* class GameBattleResult : 2010.03.18 by �����

- GameZone �� [�߰�/����]�� Player���� �ؽø����� �����ϸ�, Ŭ���̾�Ʈ�� ���� ��Ȳ ��û��
  Player�� ���� ��Ȳ�� ��ϵǴ� class BattleRecord ���� �����Ͽ� ����������� ������.

- class BattlePlayer�� Ŭ���̾�Ʈ���� Player���� ������� ��û�� �����[�߰�/����/����] ������ 
  ������ ���� [�߰�/����/����]���¸� �����ϴ� Hash Map�� ����ϸ� �̸� 'CheckKeyHash'�̶� �Ѵ�.

                  < Check Key Hash Map >
                    key          value
  BattlePlayer --- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- CHARGUID  insert/remove/update
                |- ...            ...

- Player �� Zone�� �߰��� ó�� �ѹ� �ؽøʿ� ������ Zone�� �ı��� �����Ѵ�.
  �̴� ���� ������ ���� �� ���ο� ���ӿ� ���� Player�� ������ ���� ��Ȳ���� �ʱ�ȭ ������ ���� �ذ���
  ���� ������� ���� ���� ��å�� ������ �����ϵ��� �Ѵ�.

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
