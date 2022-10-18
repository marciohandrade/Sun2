#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneReadyPVPConfig.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"

#include "uiSystemMan/uiSystemMan.h"
#include "interfacemanager.h"

#include "BattleRoomEntry.h"
#include "BattleLobby.h"


//------------------------------------------------------------------------------
WzID uiBattleZoneReadyPVPConfig::m_wzId[DIALOG_PVP_CONFIG_MAX] = 
{
	StrToWzID("ST09"),// DIALOG_PVP_CONFIG_TXT_RULE,
	StrToWzID("ST08"),// DIALOG_PVP_CONFIG_TXT_MODE,
	StrToWzID("C001"),// DIALOG_PVP_CONFIG_CHK_FIELD_1,
	StrToWzID("C002"),// DIALOG_PVP_CONFIG_CHK_FIELD_2,
	StrToWzID("C003"),// DIALOG_PVP_CONFIG_CHK_FIELD_3,	
	StrToWzID("C004"),// DIALOG_PVP_CONFIG_CHK_RULE_1, // 10킬 
	StrToWzID("C005"),// DIALOG_PVP_CONFIG_CHK_RULE_2, // 30킬
	StrToWzID("C006"),// DIALOG_PVP_CONFIG_CHK_RULE_3, // 무한	
	StrToWzID("C007"),// DIALOG_PVP_CONFIG_CHK_MODE_1, // 개인전
	StrToWzID("C008"),// DIALOG_PVP_CONFIG_CHK_MODE_2, // 팀전
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyPVPConfig::uiBattleZoneReadyPVPConfig(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pBattleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (m_pBattleZoneMan);

    if (!this->m_pBattleZoneMan)
    {
#ifdef _DEBUG
        uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pBattleZoneMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_pCtrlStaticRule = NULL;
	m_pCtrlStaticMode = NULL;

	int iSize = 0;
	for(iSize = 0; iSize < CHK_FIELD_SIZE; ++iSize)
	{
		m_pCtrlCheckField[iSize] = NULL;
	}
	
	for(iSize = 0; iSize < CHK_RULE_SIZE; ++iSize)
	{
		m_pCtrlCheckRule[iSize] = NULL;
	}

	for(iSize = 0; iSize < CHK_MODE_SIZE; ++iSize)
	{
		m_pCtrlCheckMode[iSize] = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneReadyPVPConfig::~uiBattleZoneReadyPVPConfig()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::InitControls()
{
	m_pCtrlStaticRule = static_cast<CCtrlStaticWZ*>(this->getControl(DIALOG_PVP_CONFIG_TXT_RULE));
	if(m_pCtrlStaticRule)
	{
		m_pCtrlStaticRule->SetTextID(688);	// 룰 선택
	}

	m_pCtrlStaticMode = static_cast<CCtrlStaticWZ*>(this->getControl(DIALOG_PVP_CONFIG_TXT_MODE));
	if(m_pCtrlStaticMode)
	{
		m_pCtrlStaticMode->SetTextID(689); // PVP 모드
	}

	int iSize = 0;
	for(iSize = 0; iSize < CHK_FIELD_SIZE; ++iSize)
	{
		m_pCtrlCheckField[iSize] = static_cast<CCtrlButtonCheckWZ*>(this->getControl(DIALOG_PVP_CONFIG_CHK_FIELD_1 + iSize));
	}
	
	for(iSize = 0; iSize < CHK_RULE_SIZE; ++iSize)
	{
		m_pCtrlCheckRule[iSize] = static_cast<CCtrlButtonCheckWZ*>(this->getControl(DIALOG_PVP_CONFIG_CHK_RULE_1 + iSize));
		m_pCtrlCheckRule[iSize]->SetTextID(695 + iSize);
	}

	for(iSize = 0; iSize < CHK_MODE_SIZE; ++iSize)
	{
		m_pCtrlCheckMode[iSize] = static_cast<CCtrlButtonCheckWZ*>(this->getControl(DIALOG_PVP_CONFIG_CHK_MODE_1 + iSize));
		m_pCtrlCheckMode[iSize]->SetTextID(698 + iSize);
	}

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiBattleZoneReadyPVPConfig::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		initConfigUIText();
		//UpdatePosition();

	}
    else
    {
        
    }
}


//----------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::Clear()
{
	if(GetManager())
	{
		GetManager()->ClearPendings();

		BattleLobby * pBattleDlg = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);
		if (pBattleDlg)
		{
			this->GetManager()->SetBattleLobby(pBattleDlg);
		}
	
	}

}

//----------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::initConfigUIText()
{
	if (!_isEnable())
        return;

	//  룰 선택
	int i=0;
	for(i=0; i<CHK_RULE_SIZE; ++i)
	{
		if (this->m_pCtrlCheckRule[i])
		{
			this->m_pCtrlCheckRule[i]->SetTextID(695+i);	// 10 킬, 30킬, 무한 
		}
	}

	
	// PVP 모드
	
	for(i = 0; i < CHK_MODE_SIZE; ++i)
	{
		if (this->m_pCtrlCheckMode[i])
		{
			this->m_pCtrlCheckMode[i]->SetTextID(698+i); // 개인전, 팀전
		}
	}

	
}

//----------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::updateConfigPVPUI()
{
	if (!_isEnable())
        return;

	const int iFieldIndex = GetManager()->GetBattleRoomEntry().GetFieldIndex();

	int i =0;
	for (; i < CHK_FIELD_SIZE; ++i)
	{
		m_pCtrlCheckField[i]->SetCheckState(FALSE);
	}
	m_pCtrlCheckField[iFieldIndex]->SetCheckState(TRUE);


	const ePVP_RULE_TYPE eRule = GetManager()->GetBattleRoomEntry().GetPVPRule();
	for(i = 0; i < ePVP_RULE_MAX; ++i)
	{
		m_pCtrlCheckRule[i]->SetCheckState(FALSE);
	}
	m_pCtrlCheckRule[eRule]->SetCheckState(TRUE);

	const ePVP_MODE_TYPE ePVPMode = GetManager()->GetBattleRoomEntry().GetPVPMode();
	for (i = 0; i < ePVP_MODE_MAX; ++i)
	{
		m_pCtrlCheckMode[i]->SetCheckState(FALSE);
	}

	m_pCtrlCheckMode[ePVPMode%ePVP_MODE_MAX]->SetCheckState(TRUE);
}

////----------------------------------------------------------------------------
///**
//*/
//void uiBattleZoneReadyPVPConfig::UpdatePosition()
//{
//	//this->MoveWindowWZ(748, 298);
//}


//------------------------------------------------------------------------------
/**
*/
BOOL uiBattleZoneReadyPVPConfig::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!GetManager())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(!GetManager()->GetBattleLobby())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneReadyPVPConfig::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	if(!GetManager()->IsMaster())
		return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
			POSTYPE CurPos = getCtrlIDToPos( pMessage->dwCtrlID );
            switch( CurPos )
            {
            case DIALOG_PVP_CONFIG_CHK_FIELD_1:	
			case DIALOG_PVP_CONFIG_CHK_FIELD_2:
			case DIALOG_PVP_CONFIG_CHK_FIELD_3:	
                {
					int iFieldIndex = CurPos - DIALOG_PVP_CONFIG_CHK_FIELD_1;

					GetManager()->ModifyMap(iFieldIndex);
						
                }
                break;

			case DIALOG_PVP_CONFIG_CHK_RULE_1:
			case DIALOG_PVP_CONFIG_CHK_RULE_2:
			case DIALOG_PVP_CONFIG_CHK_RULE_3:
				{
					// 10킬/30킬/무한 선택
					ePVP_RULE_TYPE eRule = 
								ePVP_RULE_TYPE(CurPos - DIALOG_PVP_CONFIG_CHK_RULE_1);		

					GetManager()->ModifyPVPCfg(
							eRule,
							GetManager()->GetBattleRoomEntry().GetPVPMode()
							);
				}
				break;

			case DIALOG_PVP_CONFIG_CHK_MODE_1:
			case DIALOG_PVP_CONFIG_CHK_MODE_2:
				{
					// 개인전/팀전 선택
					ePVP_MODE_TYPE ePVPMode = 
								ePVP_MODE_TYPE( CurPos - DIALOG_PVP_CONFIG_CHK_MODE_1 );
							
					GetManager()->ModifyPVPCfg(
							GetManager()->GetBattleRoomEntry().GetPVPRule(),
							ePVPMode
							);
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
void uiBattleZoneReadyPVPConfig::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

	updateConfigPVPUI();
	
}