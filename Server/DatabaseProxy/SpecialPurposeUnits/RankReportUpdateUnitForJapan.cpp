#include "stdafx.h"
#include "BatchProcessor.h"

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#if defined(_JAPAN)
#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT

#include "VersionControl.h"
#include "SCItemSlot.h"

#define BATCHUNIT_REGISTERING_MARK
#include "./RankReportUpdateUnitForJapan.hxx"
#undef BATCHUNIT_REGISTERING_MARK

#include "./BatchUnitFactory.hxx"

#define REF_LOGIC_FOR_RANK_REPORT   (0)
#define TEMP_DISABLE_SECTION (0)

//==================================================================================================
// implements

namespace batch_unit {

using batch::IBatchUnit;

void
RankReportUpdateUnit::UpdateNode::Init()
{
    ZeroMemory(this, sizeof(*this));
    util::LList::Init(this);
}

RankReportUpdateUnit::RankReportUpdateUnit()
    : unit_type_(0)
    , select_query_string_(NULL)
    , update_query_string_(NULL)
    , step_(STEP_NONE)
    , total_completed_(false)
    , suspend_select_query_(false)
    , completed_update_query_(false)
{
    const DWORD UserSlotLastKey = 100;
    BOOST_STATIC_ASSERT(kPending_StartKey < UserSlotLastKey);
    select_key_generator_.Create(kPending_StartKey, UserSlotLastKey);

    max_transactions_ = GetPrivateProfileInt("DBPUNIT_DATA",
                                             "MAX_UPDATES_PER_UNITTIME",
                                             300,
                                             "./DBPUnit.ini");
    if (max_transactions_ < 300)
        max_transactions_ = 300; // for Japan query pool... recommendations:3000
    update_key_generator_.Create(kPending_StartKey, max_transactions_);

    ZeroMemory(&arguments_, sizeof(arguments_));
    util::LList::Init(&arguments_.pending_nodes_);
    util::LList::Init(&arguments_.updating_nodes_);
    ZeroMemory(&statistics_, sizeof(statistics_));
}

RankReportUpdateUnit::~RankReportUpdateUnit()
{
}

void RankReportUpdateUnit::SetUnitType(int unit_type)
{
    if (false) {
    }
    else if (unit_type == BatchunitFactory::RANK_REPORT_INVENTORY)
    {
        select_query_string_ = "{call USP_GetItemListForInven(%d, ?)}";
        update_query_string_ =
            "{call USP_SetRankReportForInven(?, ?, ?)}";
    }
    else if (unit_type == BatchunitFactory::RANK_REPORT_WAREHOUSE)
    {
        select_query_string_ = "{call USP_GetItemListForWare(%d, ?)}";
        update_query_string_ =
            "{call USP_SetRankReportForWare(?, ?, ?)}";
    }
    else if (unit_type == BatchunitFactory::RANK_REPORT_GUILD_WAREHOUSE)
    {
        select_query_string_ = "{call USP_GetItemListForGuildWare(%d, ?)}";
        update_query_string_ =
            "{call USP_SetRankReportForGuildWare(?, ?, ?)}";
    }
}

inline IBatchUnit::eSTEP
RankReportUpdateUnit::CurrentStep() const
{
    return step_;
}

BOOLEAN
RankReportUpdateUnit::BatchInit()
{
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]\n");

    ZeroMemory(&arguments_, sizeof(arguments_));
    util::LList::Init(&arguments_.pending_nodes_);
    util::LList::Init(&arguments_.updating_nodes_);

    timer_.SetTimer(300);
    statistics_timer_.SetTimer(5000);
    step_ = this->STEP_PROCESSING;

    SUNLOG(eCRITICAL_LOG, "|ChangeUnitType|GetQuery=%s|SetQuery=%s|",
           select_query_string_, update_query_string_);

    return true;
}

BOOLEAN
RankReportUpdateUnit::BatchProcess()
{
    if (timer_.IsExpired() == false)
        return true;

    //printf("["__FUNCTION__"]\n");

    SelectItemList();
    UpdateRankReport();

    bool forced_statistics_logging = false;
    // completion check
    if (completed_update_query_)
    {
        if (util::LList::IsEmpty(&arguments_.pending_nodes_) &&
            util::LList::IsEmpty(&arguments_.updating_nodes_) &&
            this->max_transactions_ == update_key_generator_.GetSize()) // to fit dual buffering db queue
        {
            total_completed_ = true;
            forced_statistics_logging = true;
        }
    };
    if (forced_statistics_logging || statistics_timer_.IsExpired())
    {
        float interval = static_cast<float>(statistics_timer_.GetInterval());
        ;     interval /= 1000.0f;

        float selecting_ratio = statistics_.solved_selectings_ / interval;
        float updating_ratio = statistics_.solving_updatings_ / interval;

        SUNLOG(eCRITICAL_LOG,
               "|Number of get queried=%u|Number of set queried=%u|",
               statistics_.number_of_get_queried_, statistics_.number_of_set_queried_);

        SUNLOG(eCRITICAL_LOG,
               "|Pendings=%d|Updatings=%d|gets/sec=%2.2f|sets/sec=%2.2f|",
               arguments_.number_of_pending_nodes_, arguments_.number_of_updating_nodes_,
               selecting_ratio, updating_ratio);

        statistics_.solved_selectings_ = 0;
        statistics_.solving_updatings_ = 0;
    }

    BOOLEAN continued = true;
    if (total_completed_)
    {
        continued = false;
        step_ = this->STEP_COMPLETED;
    }

    return continued;
}

BOOLEAN
RankReportUpdateUnit::BatchRelease()
{
    printf("["__FUNCTION__"]\n");
    //for (PendingNode* it = pending_nodes_;
    //     it != &pending_nodes_[_countof(pending_nodes_)];
    //     ++it)
    //{
    //    SAFE_DELETE(it->character_);
    //    ZeroMemory(it, sizeof(*it));
    //}
    step_ = this->STEP_RELEASED;
    //
    delete this; // don't implement after at this point
    //
    return true;
}

void
RankReportUpdateUnit::DBResult(DWORD index, QueryResult* query_result)
{
    BOOST_STATIC_ASSERT(sizeof(KeyNode) == sizeof(index));
    KeyNode key_node = { 0, };
    key_node.composed_ = index;

    switch (key_node.query_type_)
    {
    case key_node.QUERY_SELECT_ITEMLIST:
        {
            SelectedItemList(key_node, query_result);
        }
        break;
    case key_node.QUERY_UPDATE_RANKREPORT:
        {
            UpdatedRankReport(key_node, query_result);
        }
        break;
    }
}

}; //end of namespace 'batch_unit'
//
//
//==================================================================================================
//
//
namespace batch_unit {
;

BOOLEAN
RankReportUpdateUnit::SelectItemList()
{
    if (completed_update_query_ != false) {
        return false;
    }

    if (arguments_.total_pages_ != 0 &&
        arguments_.selected_page_ >= arguments_.total_pages_)
    {
        completed_update_query_ = true;
        return false;
    }

    if (suspend_select_query_ != false) {
        return false;
    }

    if (select_key_generator_.IsEmpty() != false) {
        return false; // wait next time
    }

    const DWORD user_slot_index = select_key_generator_.GetKey();

    ++arguments_.selected_page_;

    STRING_SAFER_QUERY128 query_buffer;
    query_buffer.MakeString(select_query_string_, arguments_.selected_page_);

    Query_GetItemList* query = Query_GetItemList::ALLOC();
    KeyNode key_node = { 0, };
    key_node.query_type_ = key_node.QUERY_SELECT_ITEMLIST;
    key_node.node_key_ = static_cast<WORD>(user_slot_index);
    query->SetVoidObject(this);
    query->SetIndex(key_node.composed_);
    query->SetQuery(query_buffer);
    query->SetQueriedPage(arguments_.selected_page_);
    if (Query_GetItemList::sQUERY_PARAM* parameter = &query->parameter_[0]) {
        parameter->query_page = arguments_.selected_page_;
    };

    SolarDatabaseInDBP* first_db = g_DBProxyServer.Get1stDatabase();
    first_db->DBQuery(query);

    return false;
}

BOOLEAN
RankReportUpdateUnit::SelectedItemList(const KeyNode& index, QueryResult* query_result_base)
{
    select_key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_get_queried_;
    ++statistics_.solved_selectings_;

    Query_GetItemList* const query_result = static_cast<Query_GetItemList*>(query_result_base);
    const DWORD number_of_rows = query_result->GetResultRowNum();

    if (number_of_rows == 0)
    {
        Query_GetItemList::FREE(query_result);
        // force stop
        completed_update_query_ = true;
        return false;
    }

    // latest update
    arguments_.total_pages_ = query_result->parameter_[0].end_page;

    const ItemInfoParser* item_parser = ItemInfoParser::Instance();
    // working variables
    ITEMPART item_part;
    ITEMSTREAMEX item_stream, result_item_stream;
    eRANK convert_result_rank = RANK_E;
    int converted_version;
    //
    DWORD row = 0;
    for (; row < number_of_rows; ++row)
    {
        const Query_GetItemList::sQUERY_RESULT& result = query_result->query_result_[row];
        const SLOTCODE item_code = static_cast<SLOTCODE>(result.item_code);
        if (result.item_code == 0) {
            continue;
        };
    #if SUN_CODE_BACKUP
        // NOTE: f110304.1L, added 2nd filter rule to reward a missing items
        if (result.item_code < 32768) {
            continue;
        }
    #endif
        const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(item_code);
        if (item_info == NULL) {
            continue;
        }
        // NOTE: in Japan, default equip items enable sufficient conversions on connect user.
        // reference: _NA000000_110218_RANK_REPORTER_IN_DBUNIT_IN_JAPAN
        if ((item_info->IsAccessory() || item_info->IsSpecialAccessory()) == false) {
            continue;
        };

        ;{  // make item_part without serial
            ZeroMemory(&item_part, sizeof(item_part));
            item_part.wCode = item_code;
            item_part.byDura = static_cast<DURATYPE>(result.dura_or_num);
        };
        ;{  // make item stream
            item_stream.Part = item_part;
            CopyMemory(&item_stream.OptionPart, result.option_stream,
                       sizeof(item_stream.OptionPart));
        };

        // interfaces for _NA000000_110218_RANK_REPORTER_IN_DBUNIT
        // = { SCItemSlot::ClearConvertResult, SCItemSlot::GetConvertResult }
        SCItemSlot::ClearConvertResult();
        bool converted = VersionControl::ConvertItem(&converted_version, &result_item_stream,
                                                     result.version, &item_stream);
        if (converted == false) {
            continue;
        }
        if (SCItemSlot::GetConvertResult(&convert_result_rank) == false) {
            continue;
        };
        if (convert_result_rank == 0) {
            continue; // no rank item
        };

        batch_unit::RankReportNode rank_report = { 0, };
        rank_report.guid = result.guid;
        rank_report.item_code = item_code;
        rank_report.rank_grade = convert_result_rank;
        MakeUpdateQuery(rank_report);
        //
    }

    Query_GetItemList::FREE(query_result);

    // update remained last record in chunked page 
    if (arguments_.total_pages_ != 0 &&
        arguments_.selected_page_ >= arguments_.total_pages_)
    {
#if TEMP_DISABLE_SECTION
#endif
    }

    return true;
}

bool RankReportUpdateUnit::MakeUpdateQuery(const batch_unit::RankReportNode& update_info)
{
    // ACTION: insert query & refresh

    UpdateNode* update_node = TAllocNew(UpdateNode);
    update_node->Init();
    //update_node->key_ = update_key_generator_.GetKey(); //assert key_ != 0
    update_node->query_ = Query_SetRankReport::ALLOC();
    // alignment processed values

    Query_SetRankReport* query = update_node->query_;
    if (Query_SetRankReport::sQUERY_PARAM* parameter = &query->parameter_[0]) {
        *static_cast<RankReportNode*>(parameter) = update_info;
    }
    //
    util::LList::AddPrev(update_node, &arguments_.pending_nodes_);
    ++arguments_.number_of_pending_nodes_;

    return true;
}

BOOLEAN
RankReportUpdateUnit::UpdateRankReport()
{
    UpdateNode* it = static_cast<UpdateNode*>(arguments_.pending_nodes_.next);
    for (; it != &arguments_.pending_nodes_;)
    {
        if (update_key_generator_.IsEmpty()) {
            return false;
        }

        UpdateNode* extracted = it;
        it = util::LList::Delete(it);
        if (arguments_.number_of_pending_nodes_)
            --arguments_.number_of_pending_nodes_;

        extracted->key_ = update_key_generator_.GetKey();

        Query_SetRankReport* const query = extracted->query_;
        query->node_pointer_ = extracted; // passing object;

        util::LList::AddPrev(extracted, &arguments_.updating_nodes_);
        ++arguments_.number_of_updating_nodes_;

        // query { update AC info };
        const Query_SetRankReport::sQUERY_PARAM* parameter = &query->parameter_[0];

        STRING_SAFER_QUERY128 query_buffer;
        query_buffer.MakeString(update_query_string_);

        KeyNode key_node = { 0, };
        key_node.query_type_ = key_node.QUERY_UPDATE_RANKREPORT;
        key_node.node_key_ = static_cast<WORD>(extracted->key_);
        query->SetVoidObject(this);
        query->SetIndex(key_node.composed_);
        query->SetQuery(query_buffer);

        SolarDatabaseInDBP* second_db = g_DBProxyServer.Get2ndDatabase();
        second_db->DBQuery(query);
    }

    return true;
}

BOOLEAN
RankReportUpdateUnit::UpdatedRankReport(const KeyNode& index, QueryResult* query_result_base)
{
    update_key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_set_queried_;
    //static int queried = 0;
    //printf("Number of queried=%u\n", ++queried);

    Query_SetRankReport* const query_result = static_cast<Query_SetRankReport*>(query_result_base);

    UpdateNode* update_node = reinterpret_cast<UpdateNode*>(query_result->node_pointer_);
    util::LList::Delete(update_node);

    if (arguments_.number_of_updating_nodes_)
        --arguments_.number_of_updating_nodes_;

    ++statistics_.solving_updatings_;
    //printf("Updated %s\n", query_result->GetQuery());

    TAllocDelete(UpdateNode, update_node);
    Query_SetRankReport::FREE(query_result);

    // balance formula
    size_t remains = update_key_generator_.GetSize();
    suspend_select_query_ = remains < 10 ||
        (MAX_GET_ITEM_ROWS_PER_PAGE * 7) < arguments_.number_of_pending_nodes_;

    return true;
}

}; //end of namespace 'batch_unit'
//
//
//==================================================================================================
//
//
namespace batch_unit {
;


}; //end of namespace 'batch_unit'
//
//==================================================================================================
#endif //_NA000000_110218_RANK_REPORTER_IN_DBUNIT
#endif //defined(_JAPAN)
#endif //DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

