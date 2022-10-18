#pragma once

//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCharPopupMan class
//------------------------------------------------------------------------------

#include "uiCharPopupMan_def.h"
#include "uibaseman.h"
class uiCharPopupMenu;

class uiCharPopupMan : public uiBaseMan
{
public:
    uiCharPopupMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();

private:
    BOOL                    _LoadUI();
    //--------------------------------------------------------------------------
    // functions
    //
public:
	void					SetPopupInfo(DWORD dwObjKey);

	void					SetPopupType(int iType);
	int						GetPopupType() {return m_iPopupType;}

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
	void CheckPopupType(DWORD target_key, const TCHAR* character_name = "", eOBJECT_TYPE object_type = MAX_OBJECT);
#else
    void CheckPopupType();
#endif//_DH_SHOW_VENDOR_LIST_POPUP

	bool                    ProcessMenuFunc(int iIndex);
	bool					IsTargetGroupMember();
	bool					GetGroupType() {return m_bGroupType;}
    bool                    IsTargetBlockMember();
    bool					block_type() {return block_type_;}
#ifdef _NA_002935_20110704_ITEM_SCORE
    #ifdef _DH_SHOW_VENDOR_LIST_POPUP
        bool                    IsShowEquip(Object* object);
    #else
        bool                    IsShowEquip();
    #endif//_DH_SHOW_VENDOR_LIST_POPUP
    bool                    show_equip() {return show_equip_;}
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    void set_is_vendor_popup(bool val);
    void set_vendor_name(const TCHAR* name);
    void set_object_type(eOBJECT_TYPE object_type);
    void set_character_name(const TCHAR* name);
#endif//_DH_SHOW_VENDOR_LIST_POPUP
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiCharPopupMenu*				m_pUICharPopupMenu;
	bool							m_bShow;
	
	DWORD							m_dwTargetObjKey;						// 선택한 캐릭의 오브젝트 키
	
	int								m_iPopupType;
	bool							m_bGroupType;
    bool					        block_type_;
#ifdef _NA_002935_20110704_ITEM_SCORE
    bool                            show_equip_; // 장비 보기
#endif //_NA_002935_20110704_ITEM_SCORE
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    bool is_vendor_popup_; // 개인상점에서 보기인가?
    TCHAR character_name_[MAX_CHARNAME_LENGTH+1];
    eOBJECT_TYPE object_type_;
#endif//_DH_SHOW_VENDOR_LIST_POPUP
};

//--------------------------------------------------------------------------
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
inline
void uiCharPopupMan::set_is_vendor_popup(bool val)
{
    is_vendor_popup_ = val;
}

inline
void uiCharPopupMan::set_character_name(const TCHAR* name)
{
    strncpy(character_name_, name, MAX_CHARNAME_LENGTH+1);
}

inline
void uiCharPopupMan::set_object_type(eOBJECT_TYPE object_type)
{
    object_type_ = object_type;
}
#endif//_DH_SHOW_VENDOR_LIST_POPUP
//--------------------------------------------------------------------------
inline
void uiCharPopupMan::SetPopupInfo(DWORD dwObjKey)
{
	m_dwTargetObjKey = dwObjKey;
}

//--------------------------------------------------------------------------
inline
void uiCharPopupMan::SetPopupType(int iType)
{
	m_iPopupType = iType;
}