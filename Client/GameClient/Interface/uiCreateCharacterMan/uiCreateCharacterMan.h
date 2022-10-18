#pragma once
#include "uiBaseMan.h"

struct IM_CREATE_CHAR_MANAGER
{
	static const DWORD CREATECHAR_DIALOG;
	static const DWORD CREATECHAR_BOTTOM_DIALOG;
	static const DWORD CREATECHAR_LEFT_DIALOG;
	static const DWORD CUSTOM_DIALOG;
	static const DWORD CREATECHAR_SELECT;
	static const DWORD CREATECHAR_ROTATEBUTTON;
    static const DWORD CREATECHAR_CLASS_INFO;
    static const DWORD SELECTCHAR_CLASS_DIALOG;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    static const DWORD EXPLAIN_RACE_DIALOG;
    static const DWORD EXPLAIN_CLASS_DIALOG;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
};

class CreateCharBottomDialog ;
class CreateCharCustomDialog;
class CreateCharSelect;
class CreateCharRotateButton;
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
class SelectClass2;
#else
class SelectClass;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

class CreateCharRaceDialog;
class CreateCharClassDialog;

class uiCreateCharacterMan :public uiBaseMan
{
public:
	uiCreateCharacterMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    virtual void            OnRelease();
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES


private:
	
	CreateCharBottomDialog*		    m_pBottomDialog;
	CreateCharCustomDialog*		    m_pCustomDialog;
	CreateCharSelect*				m_pSelectDialog ;
	CreateCharRotateButton*		    m_pRotateButton;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    CreateCharRaceDialog*			m_pExplainRaceDialog;
    CreateCharClassDialog*		    m_pExplainClassDialog;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    SelectClass2*                   select_class_ptr_;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

public:
	bool OnOverWindowInCreate(int x, int y);
	bool isInClientRectPos(RECT pos,int x, int y);
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    SelectClass2* get_select_class_ptr() { return select_class_ptr_; }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
};
