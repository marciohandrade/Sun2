#include "SunClientPrecompiledHeader.h"
#include "uicreatecharacterman.h"
#include "interfacemanager.h"

#include "CreateCharBottomDialog.h"
#include "CreateCharLeftDialog.h"
#include "CreateCharSelect.h"
#include "CreateCharRotateButton.h"
#include "CreateCharCustomDialog.h"
#include "uiCreateClassInfo/uiCreateClassInfo.h"
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass2/uiSelectClass2.h"
#else
#include "uiSelectClass/uiSelectClass.h"
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#include "uiCreateCharacterMan/CreateCharRaceDialog.h"
#include "uiCreateCharacterMan/CreateCharClassDialog.h"
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_DIALOG				= StrToWzID("0030");
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG		= StrToWzID("0033");
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_LEFT_DIALOG			= StrToWzID("0032");
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT				= StrToWzID("0031");
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
const DWORD IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG			        = StrToWzID("0311");
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON			= StrToWzID("0313");
#else
const DWORD IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG					= StrToWzID("0036");
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON			= StrToWzID("0034");
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
const DWORD IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO           = StrToWzID("0039");
const DWORD IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG         = StrToWzID("0310");
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
const DWORD IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG            = StrToWzID("0312");
const DWORD IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG           = g_IDGenerator.GenerateID();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

uiCreateCharacterMan::uiCreateCharacterMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pBottomDialog		= NULL;
    m_pCustomDialog		= NULL;
    m_pSelectDialog		= NULL;
    m_pRotateButton		= NULL;
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    select_class_ptr_ = NULL;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
}

//------------------------------------------------------------------------------
/**
*/
void	uiCreateCharacterMan::OnInitialize()
{
	m_pBottomDialog = CreateUIDialog<CreateCharBottomDialog>(
						IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG, 
						"Data\\Interface\\3_3_Character_Create_Bottom.iwz", 
						this, TRUE);
	assert(m_pBottomDialog);

#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    select_class_ptr_ = new SelectClass2;
    if (select_class_ptr_)
    {
        select_class_ptr_->Init();
    }    
#else
    uiBase* create_class_info = CreateUIDialog<uiCreateClassInfo>( 
        IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO,
        "Data\\Interface\\3_9_Character_Info.iwz", 
        this, TRUE);
    assert(create_class_info);
    uiBase* select_class_creation = CreateUIDialog<SelectClass>( 
        IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG,
        "Data\\Interface\\3_10_Character_Creation.iwz", 
        this, TRUE);
    assert(select_class_creation);
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES    

	m_pCustomDialog = CreateUIDialog<CreateCharCustomDialog>( 
					IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG,		
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                    "Data\\Interface\\3_11_Character_Create.iwz",
#else
					"Data\\Interface\\3_6_Character_Create_Left.iwz", 	
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
					this, TRUE );
	assert(m_pCustomDialog);

    if ( m_pCustomDialog )
    {
	    m_pCustomDialog->ShowInterface( FALSE );        
    }

	m_pSelectDialog = CreateUIDialog<CreateCharSelect>(
						IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT,
						"Data\\Interface\\3_1_Character_Create_Select.iwz", 
						this, TRUE);
	assert(m_pSelectDialog);

	m_pRotateButton = CreateUIDialog<CreateCharRotateButton>(
						IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON,
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                        "Data\\Interface\\3_13_Character_Rotate.iwz", 
#else
						"Data\\Interface\\3_4_Character_Rotate_Bottom.iwz", 
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
						this, TRUE);
	assert(m_pRotateButton);

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    m_pExplainRaceDialog = CreateUIDialog<CreateCharRaceDialog>(
        IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG,
        "Data\\Interface\\3_12_Character_Comment.iwz", 
        this, TRUE);
    assert(m_pExplainRaceDialog);

    m_pExplainClassDialog = CreateUIDialog<CreateCharClassDialog>(
        IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG,
        "Data\\Interface\\3_12_Character_Comment.iwz", 
        this, TRUE, TRUE);
    assert(m_pExplainClassDialog);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

}

bool uiCreateCharacterMan::OnOverWindowInCreate(int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	
	int CountValue = 0;
	if(m_pBottomDialog && m_pBottomDialog->GetDialogWZ())
	{
		RECT rect_size;
		m_pBottomDialog->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
	if(m_pCustomDialog && m_pCustomDialog->GetDialogWZ())
	{
		RECT rect_size;
		m_pCustomDialog->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
	if(m_pSelectDialog && m_pSelectDialog->GetDialogWZ())
	{
		RECT rect_size;
		m_pSelectDialog->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
	if(m_pRotateButton && m_pRotateButton->GetDialogWZ())
	{
		RECT rect_size;
		m_pRotateButton->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	if(m_pExplainRaceDialog && m_pExplainRaceDialog->GetDialogWZ())
	{
		RECT rect_size;
		m_pExplainRaceDialog->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
	if(m_pExplainClassDialog && m_pExplainClassDialog->GetDialogWZ())
	{
		RECT rect_size;
		m_pExplainClassDialog->GetDialogWZ()->GetClientRect(&rect_size);

		if(isInClientRectPos(rect_size,x,y))
		{
			CountValue++;
		}
	}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

	return CountValue > 0 ? true : false;

}

bool uiCreateCharacterMan::isInClientRectPos( RECT pos,int x, int y )
{
	if(pos.left < x && pos.right > x &&
	   pos.top < y && pos.bottom > y)
	{
		return true;
	}

	return false;
}

#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
void uiCreateCharacterMan::OnRelease()
{   
    SAFE_DELETE(select_class_ptr_);
}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES