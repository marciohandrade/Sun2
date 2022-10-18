#include "SunClientPrecompiledHeader.h"
#include "createcharselect.h"
#include "CharacterScene.h"
#include "Application.h"
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass2/uiSelectClass2.h"
#else
#include "uiSelectClass/uiSelectClass.h"
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
#include "InterfaceManager.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
CreateCharSelect::CreateCharSelect(InterfaceManager * pUIman)
	:uiBase(pUIman)
{
}

CreateCharSelect::~CreateCharSelect(void)
{
}

WzID CreateCharSelect::m_wzId[CCS_MAX] = 
{
    StrToWzID("I000"),
    StrToWzID("I001"),
};


VOID CreateCharSelect::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
}

//------------------------------------------------------------------------------ 
VOID CreateCharSelect::Release()
{

}
//------------------------------------------------------------------------------ 
void CreateCharSelect::OnShowWindow(BOOL val)
{
    uiBase::OnShowWindow(val);
}
//------------------------------------------------------------------------------ 
VOID CreateCharSelect::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
			case CCS_RETURN:	
                {
				    g_CharSelectScene.SetCreateCharacterMode(FALSE);
                }
				break;
            case CCS_OK:
                {
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                    SelectClass2* select_class = NULL;
                    uiCreateCharacterMan* create_character_manager_ptr = GET_DIALOG_MANAGER(uiCreateCharacterMan , UIMAN_CREATECHARACTER);
                    if (create_character_manager_ptr)
                    {
                        select_class = create_character_manager_ptr->get_select_class_ptr();
                    }
#else
                    SelectClass* select_class = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
                    if (select_class)
                    {
                        select_class->OnExcute();
                    }
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

CControlWZ * CreateCharSelect::getControl( POSTYPE AtPos )
{
	assert( AtPos < CCS_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE	CreateCharSelect::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CCS_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CCS_MAX;

}