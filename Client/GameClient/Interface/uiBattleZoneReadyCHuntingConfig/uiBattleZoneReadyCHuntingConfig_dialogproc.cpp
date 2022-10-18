//------------------------------------------------------------------------------
//  uiBattleZoneReadyCHuntingConfig_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneReadyCHuntingConfig.h"
#include "BattleLobby.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "MapInfoParser.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneReadyCHuntingConfig::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("B001"),	//DLGPOS_BTN_FIELDLIST,
    StrToWzID("B002"),	//DLGPOS_BTN_RULES,
    StrToWzID("B011"),	//DLGPOS_BTN_RULES_LEFT,
    StrToWzID("B012"),	//DLGPOS_BTN_RULES_RIGHT,
    StrToWzID("B003"),	//DLGPOS_BTN_PVPMODES,
    StrToWzID("B013"),	//DLGPOS_BTN_PVPMODES_LEFT,
    StrToWzID("B014"),	//DLGPOS_BTN_PVPMODES_RIGHT,
    StrToWzID("T001"),	//DLGPOS_TXT_FIELD,
    StrToWzID("L001"),	//DLGPOS_LST_FIELDLIST,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiBattleZoneReadyCHuntingConfig::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->CheckControls())
            return;

        //UpdatePosition();
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    if (!this->GetManager())
        return;

    if (!this->GetManager()->GetBattleLobby())
        return;

	if (!this->GetManager()->IsMaster())
        return;

    if (this->GetManager()->GetCHuntingJoinedFlag())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_PVPMODES_LEFT: { doPVPMoveLeft(); } break;
            case DLGPOS_BTN_PVPMODES_RIGHT: { doPVPMoveRight(); } break;

            case DLGPOS_BTN_FIELDLIST:
                {
                    m_bShowFieldList = !m_bShowFieldList;
                }
                break;
            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneReadyCHuntingConfig::OnUpdateSolarDialog()
{


    if (!this->CheckControls())
        return;

    if (!this->GetManager())
        return;

	if (!this->GetManager()->GetBattleLobby())
        return;

    this->updateUIField();
    this->updateFieldName();
    this->updateCompModeType();
    this->updateCompRuleType();

	if (!this->GetManager()->IsMaster() || this->GetManager()->GetCHuntingJoinedFlag())
    {
        m_pBtnControls[0]->ShowWindowWZ(WZ_HIDE);

        m_pBtnControls[2]->ShowWindowWZ(WZ_HIDE);
        m_pBtnControls[3]->ShowWindowWZ(WZ_HIDE);

        m_pBtnControls[5]->ShowWindowWZ(WZ_HIDE);
        m_pBtnControls[6]->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        m_pBtnControls[0]->ShowWindowWZ(WZ_SHOW);

        m_pBtnControls[2]->ShowWindowWZ(WZ_SHOW);
        m_pBtnControls[3]->ShowWindowWZ(WZ_SHOW);

        m_pBtnControls[5]->ShowWindowWZ(WZ_SHOW);
        m_pBtnControls[6]->ShowWindowWZ(WZ_SHOW);
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
