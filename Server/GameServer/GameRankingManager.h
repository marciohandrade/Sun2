#pragma once
#ifdef _NA_008012_20150130_RANKING_SYSTEM 
#ifndef GAMESERVER_RANKING_MANAGER_H
#define GAMESERVER_RANKING_MANAGER_H

#include <RankingManager.h>

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
class SystemMemo;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
//==================================================================================================
//
class GameRankingManager : public RankingManager
{
    typedef RankingManager base_type;
public:
    GameRankingManager();
    virtual ~GameRankingManager();
    //
    // RankingManager interfaces
    virtual void Initialize();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    virtual void Release();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //virtual void Update();
    // control interfaces
    bool enabled_service() const;
    void OnScriptsLoaded();
    bool RequestRankingInformation();
    void ChangeToRankingLoaded();
    void OnReceivedStatusControl(eRANKING_STATUS ranking_status);
    //
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    void ExecuteReward(BOOL bSuccess,
        PLAYERKEY PlayerKey, RANKINGNO no, RANKINGKEY key,
        const TCHAR* charname, RANKINGTYPE ranking_type, BYTE ErrorCode = 0);
#else
    void ExecuteReward(BOOL bSuccess,
        PLAYERKEY PlayerKey, RANKINGNO no, RANKINGKEY key,
        const TCHAR* charname, BYTE ErrorCode = 0);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    void UpdateSettleTime_GameServer() { this->UpdateSettleTime(); }
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //결산 후, 지난주 랭킹 순위권자에게 알림쪽지를 보낸다.
    void SendSystemMemoOfWorldRanking();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

private:
    enum eRankingMode {
        eRankingMode_None = 0x00,
        eRankingMode_Instanced = 0x01,  // instanced,
        eRankingMode_ScriptLoaded = 0x02, // need the mode control if scripts receive from SolarAuth
        eRankingMode_RankingLoaded = 0x04,
        eRankingMode_RankingLoading = 0x10, // db transaction
        //
        eRankingMode_Loaded = eRankingMode_Instanced
                            | eRankingMode_ScriptLoaded
                            | eRankingMode_RankingLoaded,
    };
    //
    eRankingMode ranking_mode_;
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    SystemMemo* system_memo_;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
};

//==================================================================================================
// inlining implements
extern GameRankingManager g_GameRankingMgr;
//
// control interfaces
inline bool GameRankingManager::enabled_service() const {
    return (ranking_mode_ == this->eRankingMode_Loaded);
}
//==================================================================================================

#endif //GAMESERVER_RANKING_MANAGER_H
#endif // _NA_008012_20150130_RANKING_SYSTEM