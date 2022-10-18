#ifndef _AutobotActionPotion_H_
#define _AutobotActionPotion_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class InventoryDialog;

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionPotion : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionPotion* Instance();
    ~AutobotActionPotion();

    virtual char* Name(){ return "AutobotActionPotion"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionPotion();
  
  //copy ctor and assignment should be private
  AutobotActionPotion(const AutobotActionPotion&);
  AutobotActionPotion& operator=(const AutobotActionPotion&);

  bool UsePotion(eITEMTYPE item_type, eWASTETYPE waste_type);

  //bool ProcessPotion(InventoryDialog* inventory_dialog, std::vector<SLOTCODE>& code_vector);
  //bool ProcessACItem(InventoryDialog* inventory_dialog);
};


#endif