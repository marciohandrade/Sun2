#pragma once
#ifndef __DBPROXY_DBUNIT_RANK_REPORTER_QUERY_H
#define __DBPROXY_DBUNIT_RANK_REPORTER_QUERY_H

#include "QueryObjects/StructInQuery.h"

// implemented by _NA000000_110218_RANK_REPORTER_IN_DBUNIT

namespace batch_unit {
;

#define MAX_GET_ITEM_ROWS_PER_PAGE     (2000)

};

//USP_GetItemListForGuildWare
//USP_GetItemListForInven
//USP_GetItemListForWare
// 1: Inventory + Equipment, 2: Warehouse, 3: Guild warehouse
class Query_GetItemList : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GetItemList)
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_GET_ITEM_ROWS_PER_PAGE,
        RESULT_COL_NUM	= 5,
    };

    struct sQUERY_PARAM
    {
        int query_page;
        int end_page;
    } parameter_[PARAM_ROW_NUM];

    struct sQUERY_RESULT
    {
        int guid;
        short version;
        int item_code;
        short dura_or_num;
        uint8_t option_stream[sizeof(OPTIONPART)];
    } query_result_[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];


    _BEGIN_PARAM_DATA(sQUERY_PARAM, parameter_, PARAM_COL_NUM)
        //_BINDING_PARAM(0, query_page, SQL_PARAM_INPUT)
        //_BINDING_PARAM(1, end_page, SQL_PARAM_OUTPUT)
        _BINDING_PARAM(0, end_page, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA();

    _BEGIN_BINDING_DATA(sQUERY_RESULT, query_result_, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)      
        _BINDING_COLUMN(0, guid)
        _BINDING_COLUMN(1, item_code)
        _BINDING_COLUMN(2, dura_or_num)
        _BINDING_COLUMN(3, version)
        _BINDING_COLUMN_PTR(4, option_stream)
    _END_BINDING_DATA();

    // 결과 처리는 Record반환 개수에 의존한다.

    void SetQueriedPage(int queried_page) {
        queried_page_ = queried_page;
    }
    int QueriedPage() const {
        return queried_page_;
    }
public:
    int queried_page_;
};

//==================================================================================================
#if !defined(_JAPAN)

//USP_SetRankReportForGuildWare
//USP_SetRankReportForInven
//USP_SetRankReportForWare
class Query_SetRankReport : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SetRankReport);
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 11,
    };

    struct sQUERY_PARAM
    {
        int guid;
        // RANK_E = 0, <- NONE
        // RANK_D, RANK_C, RANK_B, RANK_MA, RANK_A, RANK_PA, RANK_MS, RANK_S, RANK_PS
        short rank_array[10]; // [0]:unused, [1]~[9]
    } parameter_[PARAM_ROW_NUM];

    //struct sQUERY
    //{
    //    
    //} result_[RESULT_ROW_NUM];
    //ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA(sQUERY_PARAM, parameter_, PARAM_COL_NUM)
        _BINDING_PARAM(0, guid, SQL_PARAM_INPUT)
        _BINDING_PARAM(1, rank_array[1], SQL_PARAM_INPUT) // RANK_D
        _BINDING_PARAM(2, rank_array[2], SQL_PARAM_INPUT) // RANK_C
        _BINDING_PARAM(3, rank_array[3], SQL_PARAM_INPUT) // RANK_B
        _BINDING_PARAM(4, rank_array[4], SQL_PARAM_INPUT) // RANK_MA
        _BINDING_PARAM(5, rank_array[5], SQL_PARAM_INPUT) // RANK_A
        _BINDING_PARAM(6, rank_array[6], SQL_PARAM_INPUT) // RANK_PA
        _BINDING_PARAM(7, rank_array[7], SQL_PARAM_INPUT) // RANK_MS
        _BINDING_PARAM(8, rank_array[8], SQL_PARAM_INPUT) // RANK_S
        _BINDING_PARAM(9, rank_array[9], SQL_PARAM_INPUT) // RANK_PS
        _BINDING_PARAM(10, rank_array[0], SQL_PARAM_INPUT) // is_accessory
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

public:
    void* node_pointer_;
};

//==================================================================================================
#elif defined(_JAPAN)
//==================================================================================================

namespace batch_unit {
;
struct RankReportNode
{
    int guid;
    int item_code;
    // RANK_E = 0, <- NONE, not reporting
    // RANK_D, RANK_C, RANK_B, RANK_MA, RANK_A, RANK_PA, RANK_MS, RANK_S, RANK_PS
    short rank_grade;
};

}; //end of namespace
//USP_SetRankReportForGuildWare
//USP_SetRankReportForInven
//USP_SetRankReportForWare
class Query_SetRankReport : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SetRankReport);
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 3,
    };

    struct sQUERY_PARAM : batch_unit::RankReportNode
    {
    } parameter_[PARAM_ROW_NUM];

    _BEGIN_PARAM_DATA(sQUERY_PARAM, parameter_, PARAM_COL_NUM)
        _BINDING_PARAM(0, guid, SQL_PARAM_INPUT)
        _BINDING_PARAM(1, item_code, SQL_PARAM_INPUT)
        _BINDING_PARAM(2, rank_grade, SQL_PARAM_INPUT)
    _END_PARAM_DATA();

    _INIT_BINDING_DATA();

public:
    void* node_pointer_;
};

//==================================================================================================
#else
    #error "Unknown nation code"
#endif
//==================================================================================================

#endif //__DBPROXY_DBUNIT_RANK_REPORTER_QUERY_H