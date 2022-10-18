#pragma once
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

#include "LuaMacroState.h"


class MacroSystem
{
public:
    MacroSystem();
    ~MacroSystem();


    void Process(DWORD tick_);
    void NextMacro();
    void ResetMacro();

    void InsertCommand(MacroCommand command);
    void ExecuteCommand(MacroCommand command);

    BYTE stack_count() { return script_stack_.size(); }

    void set_write_log(BOOL is_bool) { write_log_ = is_bool; }
    BOOL write_log() { return write_log_; }

    void WriteMemoryInfo();

    void add_pickup_item(DWORD item_code) { pickup_list_.push_back(item_code); }
    void clear_pickup_item() { pickup_list_.clear(); }
    STLX_VECTOR<DWORD>& pickup_list() { return pickup_list_; }

private:
    typedef STLX_DEQUE<MacroScript*> MacroScriptStack;
    util::CMemoryPoolFactory<MacroScript> macro_script_memorypool;

    MacroState* current_macro_;
    MacroState* macro_states_[Macro_Max];
    MacroScriptStack script_stack_;

    BOOL write_log_;
    DWORD tick_count_;
    
    STLX_VECTOR<DWORD> pickup_list_;
};




#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM