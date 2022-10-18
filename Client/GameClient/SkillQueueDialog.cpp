//------------------------------------------------------------------------------
//  SkillQueueDialog.cpp
// 
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SkillQueueDialog.h"
#include "DlgPosInfoParser.h"
#include "QuickContainer.h"
#include "hero.h"
#include "uiSkillQueueMan\uiSkillQueueMan.h"

WzID SkillQueueDialog::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("P000"),  // DIALOG_SLOT00
        StrToWzID("P001"),  // DIALOG_SLOT01
        StrToWzID("P002"),  // DIALOG_SLOT02
};

//------------------------------------------------------------------------------
/**
*/
SkillQueueDialog::SkillQueueDialog(InterfaceManager * pUIman):
		uiBase(pUIman)
{
}

//------------------------------------------------------------------------------
/**
*/
SkillQueueDialog::~SkillQueueDialog()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void
SkillQueueDialog::Init(CDrawBase * pDrawBase)
{
	uiBase::Init(pDrawBase);

    is_once_position_init_ = false;

	POINT pt;
	WzID DlgID = GetDialogWZ()->GetDlgID();
	if ( DlgPosInfoParser::Instance()->GetDlgPoint(DlgID, &pt) )
	{
	
		GetDialogWZ()->MoveWindowWZ( (float)pt.x, (float)pt.y );
	}
	else
	{
	
		GetDialogWZ()->MoveWindowWZ( 414.0f,665.0f);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void
SkillQueueDialog::Release()
{
}

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
SkillQueueDialog::OnShowWindow( BOOL val )
{
  
}

//------------------------------------------------------------------------------
/**
*/
void
SkillQueueDialog::MessageProc( SI_MESSAGE * pMessage )
{
    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            
            case DIALOG_PICTURE0:
                {
                    
                }
                break;

			case DIALOG_PICTURE1:
				{

				}
				break;

			case DIALOG_PICTURE2:
				{

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
RECT 
SkillQueueDialog::GetSkillRect(int index)
{
    if (is_once_position_init_ == false)
    {
        is_once_position_init_ = true;
        this->MoveDefaultWindowPos();
    }

	assert(index<DIALOG_MAX);

	RECT rcControl = getControl(index)->GetSize();

	rcControl.right+=rcControl.left;
	rcControl.bottom+=rcControl.top;

	return rcControl;
}

void SkillQueueDialog::MoveDefaultWindowPos()
{
    if (g_pHero != NULL)
    {
        QuickContainer* quick_container = g_pHero->GetQuickContainer();

        int quick_size = 1;
        if (quick_container != NULL)
        {
            if (quick_container->double_size() == true)
            {
                quick_size = 2;
            }
            //else // int quick_size = 1
            uiSkillQueueMan* ui_skill_queue_manager_ptr = 
                GET_DIALOG_MANAGER(uiSkillQueueMan, UIMAN_SKILLQUEUE);
            if (ui_skill_queue_manager_ptr != NULL)
            {
                ui_skill_queue_manager_ptr->MoveSkillQueueDialog(quick_size);
            }                        
        }
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

