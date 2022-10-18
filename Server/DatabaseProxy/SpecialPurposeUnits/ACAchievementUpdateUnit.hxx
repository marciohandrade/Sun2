#ifndef BATCHUNIT_REGISTERING_MARK
    #error "Can't direct include, use in BatchProcessor_List.cpp"
#endif
#ifndef __DBPROXY_AC_ACHIEVEMENT_UPDATE_UNIT_HXX
#define __DBPROXY_AC_ACHIEVEMENT_UPDATE_UNIT_HXX

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

#include <ObjKeyGenerator.h>

#include <AreaConquest.h>
#include <AreaConquestInfoParser.h>

#include <MissionManager.h>

#include "DBCharacter.h"
#include "ACAchievementUpdateQuery.h"

//==================================================================================================

namespace batch_unit {

class ACAchievementUpdateUnit : public batch::IBatchUnit
{
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
        Query_SetACStreamInfo* query_;

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
public:
    ACAchievementUpdateUnit();
    virtual ~ACAchievementUpdateUnit();

    virtual void SetUnitType(int unit_type) {}
    virtual eSTEP   CurrentStep() const;
    virtual BOOLEAN BatchInit();
    virtual BOOLEAN BatchProcess();
    virtual BOOLEAN BatchRelease();
    virtual void DBResult(DWORD index, QueryResult* query_result);

private:
    BOOLEAN SelectCharList();
    BOOLEAN SelectedCharList(const KeyNode& index, QueryResult* query_result);
    BOOLEAN UpdateACInfo();
    BOOLEAN UpdatedACInfo(const KeyNode& index, QueryResult* query_result);

    eSTEP step_;
    BOOLEAN total_completed_;
    BOOLEAN suspend_query_character_;
    BOOLEAN completed_query_character_;
    ITimerBase timer_;
    ITimerBase statistics_timer_;
    PendingKeyGenerator select_key_generator_;
    PendingKeyGenerator key_generator_;
    DWORD max_transactions_;
    CharListArg arguments_;
    Statistics statistics_;
};

class AreaConquestForACUnit : public AreaConquest
{
public:
    AreaConquestForACUnit();
    virtual ~AreaConquestForACUnit();

    virtual void Init();

    void AlignStream(BYTE ac_stream[MAX_ACSTREAM_SIZE],
                     BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
                     BYTE quest_complete_stream[MAX_COMPL_QUESTSTREAM_SIZE]);
    int GetCurrentAchievePointForUpdate(int area_index);

private:
    void _AlignMissionInfo(int area_index, AreaConquestInfo* ac_info);
    void _AlignQuestInfo(int area_index, AreaConquestInfo* ac_info);
    MissionManager mission_manager_;
};

}; //end of namespace 'batch_unit'

#endif// DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
//==================================================================================================
#endif //__DBPROXY_AC_ACHIEVEMENT_UPDATE_UNIT_HXX
