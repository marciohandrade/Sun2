#ifndef BATCHUNIT_REGISTERING_MARK
    #error "Can't direct include, use in BatchProcessor_List.cpp"
#endif
#ifndef __DBPROXY_DBUNIT_RANK_REPORTER_UPDATE_UNIT_HXX
#define __DBPROXY_DBUNIT_RANK_REPORTER_UPDATE_UNIT_HXX

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#if defined(_JAPAN)
#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT

#include <ObjKeyGenerator.h>

#include "RankConvertQuery.h"

//==================================================================================================

namespace batch_unit {

class RankReportUpdateUnit : public batch::IBatchUnit
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
        Query_SetRankReport* query_;

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
            QUERY_SELECT_ITEMLIST,
            QUERY_UPDATE_RANKREPORT,
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
    //
public:
    RankReportUpdateUnit();
    virtual ~RankReportUpdateUnit();

    virtual void SetUnitType(int unit_type);
    virtual eSTEP   CurrentStep() const;
    virtual BOOLEAN BatchInit();
    virtual BOOLEAN BatchProcess();
    virtual BOOLEAN BatchRelease();
    virtual void DBResult(DWORD index, QueryResult* query_result);

private:
    BOOLEAN SelectItemList();
    BOOLEAN SelectedItemList(const KeyNode& index, QueryResult* query_result);
    bool MakeUpdateQuery(const batch_unit::RankReportNode& update_info);
    BOOLEAN UpdateRankReport();
    BOOLEAN UpdatedRankReport(const KeyNode& index, QueryResult* query_result);

    int unit_type_;
    const char* select_query_string_;
    const char* update_query_string_;
    eSTEP step_;
    BOOLEAN total_completed_;
    BOOLEAN suspend_select_query_;
    BOOLEAN completed_update_query_;
    ITimerBase timer_;
    ITimerBase statistics_timer_;
    PendingKeyGenerator select_key_generator_;
    PendingKeyGenerator update_key_generator_;
    DWORD max_transactions_;
    CharListArg arguments_;
    Statistics statistics_;
};

}; //end of namespace 'batch_unit'

#endif //_NA000000_110218_RANK_REPORTER_IN_DBUNIT
#endif //defined(_JAPAN)
#endif// DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
//==================================================================================================
#endif //__DBPROXY_DBUNIT_RANK_REPORTER_UPDATE_UNIT_HXX
