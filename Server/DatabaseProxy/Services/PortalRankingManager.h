#pragma once
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifndef DBPROXY_PORTAL_RANKKING_MANAGER_H
#define DBPROXY_PORTAL_RANKKING_MANAGER_H

#if defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP)
//==================================================================================================
//  @history
//  - 
#include <RankingManager.h>

//==================================================================================================
//
class PortalRankingManager : public RankingManager
{
    typedef RankingManager base_type;
public:
    PortalRankingManager();
    virtual ~PortalRankingManager();
    // RankingManager interfaces
    virtual void Initialize();
    virtual void Update();

    // control interfaces
    bool enabled_service() const;
    void OnScriptsLoaded();
    bool RequestRankingInformationToDB();
    void ChangeToRankingLoaded();

protected:
    // RankingManager interfaces
    // NOTE: 상속 받은 클래스에서 구현해야 하는것
    virtual void OnChangedStatus();
    virtual void OnSettled();

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
};

//==================================================================================================
// inlining implements
extern PortalRankingManager g_RankingMgr; // access interface
//
// control interfaces
inline bool PortalRankingManager::enabled_service() const {
    return (ranking_mode_ == this->eRankingMode_Loaded);
}

//==================================================================================================
#endif //_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
//==================================================================================================

#endif //DBPROXY_PORTAL_RANKKING_MANAGER_H
#endif // _NA_008012_20150130_RANKING_SYSTEM