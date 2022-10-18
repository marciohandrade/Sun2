#include "SunClientPrecompiledHeader.h"
#include "uihelmetshow.h"
#include "Mouse.h"
#include "InterfaceManager.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "HeroData.h"
#include "Hero.h"

WzID uiHelmetShow::m_wzId[eControl_Max] = 
{ 
	StrToWzID("C000"),//eControl_Check_Show_C000,
	StrToWzID("P010"),//eControl_Picture_show_P010,
};

uiHelmetShow::uiHelmetShow() 
{
}

uiHelmetShow::uiHelmetShow( InterfaceManager* pUIMan ) : uiBase(pUIMan)
{

}

uiHelmetShow::~uiHelmetShow(void)
{
}

VOID uiHelmetShow::Init( CDrawBase * pDrawBase )
{
	CCtrlButtonCheckWZ* checkbutton = (CCtrlButtonCheckWZ*)getControl(eControl_Check_Show_C000);
	
	if(checkbutton)
	{
		bool isCheck = checkbutton ? !GAMEOPTION->GetShowHelmet() : false;
		checkbutton->SetCheckState(!isCheck);
	}
	

}

VOID uiHelmetShow::Release()
{

}

VOID uiHelmetShow::MessageProc( SI_MESSAGE * pMessage )
{
	
}

CControlWZ * uiHelmetShow::getControl( POSTYPE AtPos )
{
	assert( AtPos < eControl_Max );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE uiHelmetShow::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < eControl_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return eControl_Max;
}

void uiHelmetShow::MoveWindowToEquip( int x, int y )
{
	if(GetDialogWZ())
	{
		GetDialogWZ()->MoveWindowWZ((float)x,(float)y);
	}
	
}

void uiHelmetShow::Render()
{
	if(!this->IsVisible())
		return;
	
	CControlWZ* Controller = getControl(eControl_Check_Show_C000);

	if(Controller)
	{
		Controller->OnDraw(true);
	}

	Controller = getControl(eControl_Picture_show_P010);
	if(Controller)
	{
		Controller->OnDraw(true);
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	CCtrlButtonCheckWZ* checkbutton = (CCtrlButtonCheckWZ*)getControl(eControl_Check_Show_C000);
	CCtrlPictureWZ* helmetPicture = (CCtrlPictureWZ*)getControl(eControl_Picture_show_P010);
	if(checkbutton && helmetPicture)
	{
		bool isCheck =  checkbutton->GetCheckState();

		RECT rect = helmetPicture->GetSizeRect();

		if (rect.left <= iMouseX && rect.right >= iMouseX)
		{
			if (rect.top <= iMouseY && rect.bottom >= iMouseY)
			{
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				TCHAR notice_message[INTERFACE_STRING_LENGTH] = {'\0', };
				if(isCheck)
				{
					g_InterfaceManager.GetInterfaceString(2437, notice_message, INTERFACE_STRING_LENGTH);
				}
				else
				{
					g_InterfaceManager.GetInterfaceString(2436, notice_message, INTERFACE_STRING_LENGTH);
				}

				pUIMan->RegisterTooltip(notice_message);
			}
		}
	}

}

VOID uiHelmetShow::CheckMessageMouse( bool UpMessage )
{
	CCtrlButtonCheckWZ* Controller = (CCtrlButtonCheckWZ*)getControl(eControl_Check_Show_C000);

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();
	static bool isDownMosue = false;

	if(Controller)
	{
		RECT pos = Controller->GetSizeRect();

		if (pos.left <= iMouseX && pos.right >= iMouseX)
		{
			if (pos.top <= iMouseY && pos.bottom >= iMouseY)
			{
				if(UpMessage)
				{
					bool Check =  !Controller->GetCheckState();
					Controller->SetCheckState(Check);
					isDownMosue = false;


					CCtrlButtonCheckWZ* checkbutton = (CCtrlButtonCheckWZ*)getControl(eControl_Check_Show_C000);
					bool isCheck = checkbutton ? !checkbutton->GetCheckState() : false;
					if( g_pHero )
					{
						g_pHero->HideHelmet( isCheck );

						MSG_CG_STATUS_RENDER_OPTION_SYN msg;

						msg.m_byRenderOptionType = MSG_CG_STATUS_RENDER_OPTION_SYN::eRENDER_HELMET_OPTION;
						msg.m_byOptionLevel =isCheck ? 1 : 0;

						g_pHero->SendPacket(&msg,sizeof(msg));
					}
					else
					{
						xGetHeroData()->SetHideHelmet( isCheck );
					}
				}
			}
		}
	}
}

VOID uiHelmetShow::OnShowWindow( BOOL val )
{
	//창이 갱신될 경우 항상 세로 셋팅
	if(val)
	{

	}
}

void uiHelmetShow::set_check_show_helmet_state( const bool& is_check )
{
    CCtrlButtonCheckWZ* checkbutton = (CCtrlButtonCheckWZ*)getControl(eControl_Check_Show_C000);

    if(checkbutton)
    {        
        checkbutton->SetCheckState(is_check);
    }
}