#ifndef _AutobotConditionManager_H_
#define _AutobotConditionManager_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <ProgramCommon/wzvector.h>
#include "Singleton.h"

#include "AutobotCondition.h"

struct ChatMsg;
class Autobot;

//------------------------------------------------------------------------------
class AutobotConditionManager : public Singleton<AutobotConditionManager>
{
    friend class Autobot;

public:
    AutobotConditionManager();
    ~AutobotConditionManager();

    bool Init(char* character_name);
    void Clear();
    bool Save();
    bool Load();
    bool SaveToBuffer(ArchiveOfStream& ar);
    bool LoadFromBuffer(ArchiveOfStream& ar);
    bool IsLoaded() const { return loaded_; }
    void CheckValid(SLOTCODE item_code);

    AutobotCondition_Recovery& recovery() { return recovery_; }
    AutobotCondition_Skill& skill() { return skill_; }
    AutobotCondition_Revival& revival() { return revival_; }
    AutobotCondition_Shop& shop() { return shop_; }
    AutobotCondition_Hunting& hunting() { return hunting_; }
    AutobotCondition_Pickup& pickup() { return pickup_; }
    AutobotCondition_Recall& recall() { return recall_; }
    AutobotCondition_Community& community() { return community_; }

    void set_is_loaded(bool is_loaded){ loaded_ = is_loaded; }
    bool SendCheck();
    AutobotCondition_Recovery&  dummy_recovery() { return dummy_recovery_; }
    AutobotCondition_Skill&     dummy_skill() { return dummy_skill_; }
    AutobotCondition_Revival&   dummy_revival() { return dummy_revival_; }
    AutobotCondition_Shop&      dummy_shop() { return dummy_shop_; }
    AutobotCondition_Hunting&   dummy_hunting() { return dummy_hunting_; }
    AutobotCondition_Pickup&    dummy_pickup() { return dummy_pickup_; }
    AutobotCondition_Recall&    dummy_recall() { return dummy_recall_; }
    AutobotCondition_Community& dummy_community() { return dummy_community_; }

private:
    void WriteChat(ChatMsg* chat);

    AutobotCondition_Recovery recovery_;
    AutobotCondition_Skill skill_;
    AutobotCondition_Revival revival_;
    AutobotCondition_Shop shop_;
    AutobotCondition_Hunting hunting_;
    AutobotCondition_Pickup pickup_;
    AutobotCondition_Recall recall_;
    AutobotCondition_Community community_;

    AutobotCondition_Recovery   dummy_recovery_;
    AutobotCondition_Skill      dummy_skill_;
    AutobotCondition_Revival    dummy_revival_;
    AutobotCondition_Shop       dummy_shop_;
    AutobotCondition_Hunting    dummy_hunting_;
    AutobotCondition_Pickup     dummy_pickup_;
    AutobotCondition_Recall     dummy_recall_;
    AutobotCondition_Community  dummy_community_;

    bool loaded_;
    std::string condition_filename_;
    std::string chat_log_filename_;
};

#endif
