#ifndef _uiAutobotDialog_h_
#define _uiAutobotDialog_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uiBase.h"

struct BASE_AutomaticComboInfo;

//------------------------------------------------------------------------------ 
class uiAutobotDialog : public uiBase
{
    friend class uiAutobotMan;

public:
    uiAutobotDialog(InterfaceManager* ui_man);
    virtual ~uiAutobotDialog(void);

protected:
    virtual void DefaultUI() = 0;
    virtual bool UpdateCondition() = 0;
    virtual bool UpdateUI() = 0;
    virtual eAUTO_HUNTING_TYPE GetHuntingTypeFromControlIndex(DWORD control_index){ return eAUTO_TYPE_INVALID; }

    virtual bool InsertItemToDropdown(DWORD list_index, char* item_text, void* user_data);
    virtual bool InsertItemToDropdown(DWORD list_index, std::vector<BASE_AutomaticComboInfo*>& value_vector);
    virtual void ChangeDropdownItem(DWORD list_index, DWORD staticbox_index);

    //------------------------------------------------------------------------------
    //! Qualifier: �޺� �ڽ� �������� ���� ��
    //! Parameter: DWORD list_index : 
    //! Parameter: DWORD staticbox_index
    //! Parameter: int select_index : listbox �ε���
    //! Parameter: const char* item_text : listbox Ÿ�̺� ��
    //! Parameter: int combo_option_value : ItemData(BASE_AutomaticComboInfo) �� option_value ��
    //------------------------------------------------------------------------------
    virtual void SelectDropdownItem(DWORD list_index, DWORD staticbox_index, int select_index);
    virtual void SelectDropdownItem(DWORD list_index, DWORD staticbox_index, const char* item_text);
    virtual void SelectDropdownItemByComboValue(DWORD list_index, DWORD staticbox_index, int combo_option_value);
    virtual void SelectDropdownItemByComboItemCode(DWORD list_index, DWORD staticbox_index, int combo_item_code);

    virtual void* GetCurDropdownItemData(DWORD list_index);
    virtual const char* GetCurDropdownItemText(DWORD list_index);

    virtual bool IsEnableMenu(eAUTO_HUNTING_TYPE hunting_type, SLOTCODE item_code);
    //------------------------------------------------------------------------------
    //! Returns:   void
    //! Qualifier: �����ϰ� �ִ� �������� �ش� ����� ������ is_enable ������ ������� disable ��Ŵ
    //! Parameter: eAUTO_HUNTING_TYPE hunting_type : �޴���ȣ
    //! Parameter: SLOTCODE item_code : �ڵ���� �ƾ��� �ڵ�
    //! Parameter: DWORD control_index : ��Ʈ�� ��ȣ
    //! Parameter: bool is_enable : �ش� ��Ʈ�� enable/disable
    //! Parameter: DWORD text_control_index : static text control�� ������� disable �ϸ� �ؽ�Ʈ ���� ���� �����Ѵ�.
    //------------------------------------------------------------------------------
    virtual void SetEnableByItem(DWORD control_index, bool is_enable, SLOTCODE item_code, DWORD text_control_index);
    virtual void SetCheckedByItem(DWORD control_index, bool is_checked, SLOTCODE item_code);

};

#endif //_YMS_AUTOBOT_GAME

#endif //_uiAutobotDialog_h_