#ifndef __DBPROXY_AC_CONVERT_QUERY_H
#define __DBPROXY_AC_CONVERT_QUERY_H

#pragma once

#include "QueryObjects/StructInQuery.h"

class Query_Update_Character_ACInfo : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Update_Character_ACInfo)
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,
    };

    struct sPARAM
    {
        int result_;
    } pParam[PARAM_ROW_NUM];    

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM      (0, result_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()   

    _INIT_BINDING_DATA()

    inline INT ResultCode() { return pParam[0].result_; }
    inline BOOL ResultSuccess() { return (pParam[0].result_==0); }

    void* node_pointer_;
};

class Query_Select_Character_ACInfo : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_Select_Character_ACInfo)
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = 500,
        RESULT_COL_NUM	= 6,
    };

    struct sPARAM
    {
        int end_page_;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {        
        CHARGUID char_guid_;
        short ac_version_;
        BYTE ac_stream_[MAX_ACSTREAM_SIZE];
        BYTE mission_stream_[MAX_MISSIONSTREAM_SIZE];
        short quest_version_;
        BYTE quest_complete_stream_[MAX_COMPL_QUESTSTREAM_SIZE];
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];


    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM      (0, end_page_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)              
    _BINDING_COLUMN     (0, char_guid_)
    _BINDING_COLUMN     (1, ac_version_)
    _BINDING_COLUMN_PTR (2, ac_stream_)
    _BINDING_COLUMN_PTR (3, mission_stream_)
    _BINDING_COLUMN     (4, quest_version_)
    _BINDING_COLUMN_PTR (5, quest_complete_stream_)
    _END_BINDING_DATA   ()

    // 결과 처리는 Record반환 개수에 의존한다.

    void SetQueriedPage(int queried_page) {
        queried_page_ = queried_page;
    }
    int GetQueriedPage() const {
        return queried_page_;
    }

    bool IsLastedPage() const {
        return queried_page_ >= pParam->end_page_;
    }
private:
    int queried_page_;
};

#endif //__DBPROXY_AC_CONVERT_QUERY_H
