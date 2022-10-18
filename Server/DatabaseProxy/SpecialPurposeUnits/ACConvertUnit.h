#ifndef __DBPROXY_AC_CONVERT_UNIT_HXX
#define __DBPROXY_AC_CONVERT_UNIT_HXX


#include "BatchProcessor.h"
#include "ACConvertQuery.h"

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED

#include <ObjKeyGenerator.h>

#include "AreaConquest.h"
#include "AreaConquestInfo.h"

#include "ItemInfoParser.h"
#include "NPCInfoParser.h"
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#include "AchievementParser.h"
#endif

#include <MissionManager.h>

#include "Achievement.h"
#include "AchievementSearch.h"


struct Character_Achievement_Unit
{
    Character_Achievement_Unit()
    {
        Character_Achievements achievement_info;
        achievement_info.achievement_count_ = 0;
        achievement_.Serialize(achievement_info, ACHIEVEMENT_SAVE);
    };

    void SetAchievementValue(WORD key, WORD value)
    {
        Achievement_SearchInfo* achieveement_searchinfo = 
            AchievementSearch::Instance()->GetAchievement_SearchInfo(key);

        if(!achieveement_searchinfo)
        {
            SUNLOG(eCRITICAL_LOG, "Not Found acievementinfo %u\n",key);
        }
        else
        {
            achievement_.SetAchievementValue(achieveement_searchinfo->parser_info_,
                achieveement_searchinfo->index_,
                value,
                ACHIEVEMENT_VALUE_SET);
        }
    }

    CHARGUID character_guid_;
    Achievement achievement_;
};

namespace batch_unit {
;

class ACConvertUnit : public batch::IBatchUnit
{
private:
    static const DWORD kPending_StartKey = 1;

    struct UpdateNode : public util::__LList<UpdateNode>
    {
        enum STATE {
            STATE_NONE      = 0,
            STATE_QUERYING  ,
            STATE_UPDATED   ,
            STATE_COUNTS
        };
        DWORD key_;
        BYTE state_;
        Query_Update_Character_ACInfo* query_;

        void Init();
    };

    typedef util::CObjKeyGenerator<DWORD> PendingKeyGenerator;
    struct CharListArg {
        int total_pages_;
        int selected_page_;
        int number_of_pending_nodes_;
        int number_of_updating_nodes_;
        util::__LList<UpdateNode> pending_nodes_;
        util::__LList<UpdateNode> updating_nodes_;
    };

    struct Statistics {
        int number_of_get_queried_;
        int number_of_set_queried_;
        int solved_selectings_;
        int solving_updatings_;
    };

    struct KeyNode {
        enum QUERY_TYPE {
            QUERY_NONE = 0,
            QUERY_SELECT_CHARACTERS,
            QUERY_UPDATE_AC,
            QUERY_COUNTS
        };
        union {
            DWORD composed_;
            struct {
                WORD query_type_;
                WORD node_key_;
            };
        };
    };

    class ACInfoConvertManager : public AreaConquest
    {
    public:
        void Init();

        void AlignStream(BYTE ac_stream[MAX_ACSTREAM_SIZE],
            BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
            BYTE quest_complete_stream[MAX_COMPL_QUESTSTREAM_SIZE],
            Character_Achievement_Unit& achievement_info);
        void _AlignMissionInfo(AreaConquestInfo* ac_info, Character_Achievement_Unit& achievement_info);
        void _AlignQuestInfo(AreaConquestInfo* ac_info, Character_Achievement_Unit& achievement_info);            

    private:
        AreaConquestInfo area_conquest_;
        MissionManager mission_manager_;
    };

private:
    void InitACScriptData();
    bool OnSelectCharacterACInfo(const KeyNode& index, QueryResult* query_result);
    bool OnUpdateCharacterACInfo(const KeyNode& index, QueryResult* query_result);
    void AddSelectedCharacters(QueryResult* query_result);
    void ConvertACInfo(Query_Select_Character_ACInfo::sQUERY& IN result, 
                        BYTE OUT ac_stream[MAX_ACSTREAM_SIZE],
                        BYTE OUT mission_stream[MAX_MISSIONSTREAM_SIZE],
                        BYTE OUT quest_stream[MAX_COMPL_QUESTSTREAM_SIZE],
                        BYTE OUT quest_prog[MAX_PROGR_QUESTSTREAM_SIZE]);
    void ConvertACToAchievement(CHARGUID char_guid,
                                BYTE ac_stream[MAX_ACSTREAM_SIZE],
                                BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
                                BYTE quest_stream[MAX_COMPL_QUESTSTREAM_SIZE],
                                BYTE quest_prog[MAX_PROGR_QUESTSTREAM_SIZE]);  
    void AddAchievementQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                            int contition_index, int condition_value);
    void AddAchieveItemQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                            int contition_index, int condition_value);
    void AddAchieveNPCQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                            int contition_index, int condition_value);
    void AddAchieveACNPCQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                            int contition_index, int condition_value);
    void AddAchieveMissionQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                            int contition_index, int condition_value);
    ACHIEVEMENT_OBJECT_TYPE GetObjectType(int contition_index);
    void Query_Achievement_Info(Character_Achievement_Unit& ac_info);
    bool QueryAchievementUpdate();

public:
    ACConvertUnit();
    ~ACConvertUnit();

    virtual void SetUnitType(int unit_type) {}
    virtual eSTEP   CurrentStep() const;
    virtual BOOLEAN BatchInit();
    virtual BOOLEAN BatchProcess(); // tokenized task
    virtual BOOLEAN BatchRelease();
    virtual void DBResult(DWORD index, QueryResult* query_result);

private:
    bool Get_Character_ACInfo();
    inline AchievementSearch* GetAchievementSearch() {return AchievementSearch::Instance();}
private:        
    eSTEP step_;
    BOOLEAN total_completed_;
    BOOLEAN suspend_query_character_;
    BOOLEAN completed_query_character_;
    ITimerBase statistics_timer_;
    PendingKeyGenerator select_key_generator_;
    PendingKeyGenerator key_generator_;
    DWORD max_transactions_;
    CharListArg arguments_;
    Statistics statistics_;

    //캐릭터 정보의 AC 정보를 변환하여 새로운 테이블에 write 하기전에 큐잉
    //UnitValueQueue<Character_Achievement_Unit> changed_ac_queue_;

private:
    typedef STLX_HASH_MAP<DWORD, BASE_ITEMINFO*>    ITEM_MAP;
    typedef STLX_HASH_MAP<DWORD, BASE_NPCINFO*>     NPC_MAP;

    ITEM_MAP    iteminfo_;
    NPC_MAP     npcInfo_;
};

}; // end of namespace

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED
#endif //#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

#endif //__DBPROXY_AC_CONVERT_UNIT_HXX

