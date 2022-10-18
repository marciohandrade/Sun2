//------------------------------------------------------------------------------
//  uiBattleZoneReadyMissionConfig_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiBattleZoneReadyMissionConfig.h"
#include "MapInfoParser.h"
#include "interfacemanager.h"
#include "BattleScene.h"

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::Clear()
{
	if (this->GetManager())
    {
		this->GetManager()->ClearPendings();

        BattleLobby * pBattleDlg = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
        if (pBattleDlg)
        {
            this->GetManager()->SetBattleLobby(pBattleDlg);
        }
    

        if (this->GetManager()->IsReadyToModifyMissionCfg())
        {
		    const sMAPINFO * pMapInfo = 
			    MapInfoParser::Instance()->FindMapInfo( static_cast<MAPCODE>(BattleScene::GetPendingBattleRoomEntry()->m_MapCode) );
		    if (pMapInfo)
		    {
			    m_byCurSubMission = pMapInfo->byMType;
		    }		
		    else
		    {
			    assert(0);
			    m_byCurSubMission = GetManager()->GetMinSubMission();
			    this->GetManager()->ModifyMap(m_byCurSubMission);
		    }

		    if(m_byCurSubMission)
            {
			    this->SelectSubMission();
            }
		    else
            {
			    this->SelectMainMission();
            }
        }		
    }
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZoneReadyMissionConfig::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::SelectMainMission()
{
    m_pChkControls[0]->SetCheckState(true);
    m_pChkControls[1]->SetCheckState(false);
    m_dwTextColorMainMission = RGBA(240, 201, 121, 255);
    m_dwTextColorSubMission = RGBA(119,119,119,119);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::SelectSubMission()
{
    m_pChkControls[0]->SetCheckState(false);
    m_pChkControls[1]->SetCheckState(true);
    m_dwTextColorMainMission = RGBA(119,119,119,119);
    m_dwTextColorSubMission = RGBA(240, 201, 121, 255);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::DoSubMissionUP()
{
    if (!this->GetManager())
        return;

    if (this->GetManager()->IsReadyToModifyMissionCfg())
    {
		m_byCurSubMission = GetManager()->GetSubMissionUp(m_byCurSubMission);
		if(m_byCurSubMission == 0)
			return;

        if (!GENERALPARAM->IsTestUIScene())
            this->GetManager()->ModifyMap(m_byCurSubMission);

        this->SelectSubMission();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::DoSubMissionDOWN()
{
    if (!this->GetManager())
        return;

    if (this->GetManager()->IsReadyToModifyMissionCfg())
    {
		m_byCurSubMission = GetManager()->GetSubMissionDown(m_byCurSubMission);
		if(m_byCurSubMission == 0)
			return;

        if (!GENERALPARAM->IsTestUIScene())
            this->GetManager()->ModifyMap(m_byCurSubMission);

        this->SelectSubMission();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyMissionConfig::SetCfg_FieldByMapCode(MAPCODE mapCode)
{
    const sMAPINFO* mapInfo;

    if (GENERALPARAM->IsTestUIScene())
        mapInfo = MapInfoParser::Instance()->FindMapInfo(1);
    else
        mapInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);

    if (!mapInfo)
        return;

	m_byCurSubMission = mapInfo->byMType;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
