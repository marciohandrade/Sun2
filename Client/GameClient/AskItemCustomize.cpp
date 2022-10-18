#include "SunClientPrecompiledHeader.h"
#include "AskItemCustomize.h"
#include "ItemManager.h"
#include "GameConst_Color.h"
#include "InterfaceManager.h"
#include "hero.h"
#include "HeroActionInput.h"

AskItemCustomize::AskItemCustomize(InterfaceManager *pUIMan)
:AskDialog(pUIMan)
{

}

AskItemCustomize::~AskItemCustomize()
{

}

void AskItemCustomize::OnUpdateSolarDialog()
{
	if (this->GetStatus() == ASK_STATUS_OK)
	{
		this->DoIt();
		SetStatus(ASK_STATUS_NORMAL);
	}
	else if (this->GetStatus() == ASK_STATUS_CANCEL)
	{
		this->Cancel();
		SetStatus(ASK_STATUS_NORMAL);
	}
}
void AskItemCustomize::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case ASK_OK:
				{
					this->m_status = ASK_STATUS_OK;
				}
				break;

			case ASK_CANCEL:
				{
					this->m_status = ASK_STATUS_CANCEL;
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

VOID AskItemCustomize::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		SetTexts();

        g_pHero->StopMove();    // 합성하면서 포탈타는 것 등을 못하게 하기위해 멈춘다
        g_pHero->ResetMoveFlag();
        g_HeroInput.SetAutoRun(FALSE);
        g_HeroInput.SetFollowState(FALSE);
	}
}

void AskItemCustomize::SetTexts()
{
    TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
    
    int string_code = 6295;

    switch (customize_type())
    {
    case eITEMWASTE_ETHEREALIZER_SHELL:
        {
            string_code = 6293;
        }
        break;
    case eITEMWASTE_ETHEREALIZER_ETHERIA:
        {
            string_code = 6294;
        }
        break;
    }
	g_InterfaceManager.GetInterfaceString(string_code, message);
    InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}

void AskItemCustomize::DoIt()
{
    g_pHero->StopMove();    // 합성하면서 포탈타는 것 등을 못하게 하기위해 멈춘다
    g_pHero->ResetMoveFlag();
    g_HeroInput.SetAutoRun(FALSE);
    g_HeroInput.SetFollowState(FALSE);

    //캐스팅시작        
    ItemManager::Instance()->SendEtheriaCastingStart(slot_pos(), use_pos());

    ShowInterface(FALSE);
}
void AskItemCustomize::Cancel()
{
	ShowInterface(FALSE);
}
