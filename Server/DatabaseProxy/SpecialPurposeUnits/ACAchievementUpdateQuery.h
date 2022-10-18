#ifndef __DBPROXY_AC_ACHIEVEMENT_UPDATE_QUERY_H
#define __DBPROXY_AC_ACHIEVEMENT_UPDATE_QUERY_H
#pragma once

#include "QueryObjects/StructInQuery.h"

namespace batch_unit {
;

#define MAX_GET_ACSTREAM_INFO_ROWS_PER_PAGE     (500)

};

class Query_GetACStreamInfo : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_GetACStreamInfo)
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 1,

        RESULT_ROW_NUM  = MAX_GET_ACSTREAM_INFO_ROWS_PER_PAGE,
        RESULT_COL_NUM	= 7,
    };

    struct sPARAM
    {
        int end_page_;
    } pParam[PARAM_ROW_NUM];

    struct sQUERY
    {
        int server_guid_;
        CHARGUID char_guid_;
        short ac_version_;
        BYTE ac_stream_[MAX_ACSTREAM_SIZE];
        BYTE mission_stream_[MAX_MISSIONSTREAM_SIZE];
        short quest_version_;//QUESTPART_PROGR::QUESTPART_PROGR_VERSION
        BYTE quest_complete_stream_[MAX_COMPL_QUESTSTREAM_SIZE];
    } pResult[RESULT_ROW_NUM];

    ULONG uLength[RESULT_COL_NUM];


    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM      (0, end_page_, SQL_PARAM_OUTPUT)
    _END_PARAM_DATA     ()

    _BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)      
    _BINDING_COLUMN     (0, server_guid_)
    _BINDING_COLUMN     (1, char_guid_)
    _BINDING_COLUMN     (2, ac_version_)
    _BINDING_COLUMN_PTR (3, ac_stream_)
    _BINDING_COLUMN_PTR (4, mission_stream_)
    _BINDING_COLUMN     (5, quest_version_)
    _BINDING_COLUMN_PTR (6, quest_complete_stream_)
    _END_BINDING_DATA   ()

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


class Query_SetACStreamInfo : public QueryResult
{
    __DECL_SAFECUSTOMPOOL_PTR(Query_SetACStreamInfo);
public:
    enum
    {
        PARAM_ROW_NUM	= 1,
        PARAM_COL_NUM	= 8,

        //RESULT_ROW_NUM  = 1,
        //RESULT_COL_NUM	= 8,
    };

    struct sPARAM
    {
        int server_guid_;
        CHARGUID char_guid_;
        short area1_;
        short area2_;
        short area3_;
        short area4_;
        short area5_;
        short area6_;
    } pParam[PARAM_ROW_NUM];

    //struct sQUERY
    //{
    //    
    //} pResult[RESULT_ROW_NUM];
    //ULONG uLength[RESULT_COL_NUM];

    _BEGIN_PARAM_DATA   (sPARAM, pParam, PARAM_COL_NUM)
    _BINDING_PARAM      (0, server_guid_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (1, char_guid_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (2, area1_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (3, area2_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (4, area3_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (5, area4_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (6, area5_, SQL_PARAM_INPUT)
    _BINDING_PARAM      (7, area6_, SQL_PARAM_INPUT)
    _END_PARAM_DATA     ()

    _INIT_BINDING_DATA()
    //_BEGIN_BINDING_DATA (sQUERY, pResult, uLength, RESULT_ROW_NUM, RESULT_COL_NUM)      
    //_BINDING_COLUMN     (0, server_guid_)
    //_BINDING_COLUMN     (1, char_guid_)
    //_BINDING_COLUMN     (2, area1_)
    //_BINDING_COLUMN     (3, area2_)
    //_BINDING_COLUMN     (4, area3_)
    //_BINDING_COLUMN     (5, area4_)
    //_BINDING_COLUMN     (6, area5_)
    //_BINDING_COLUMN     (7, area6_)
    //_END_BINDING_DATA   ()

public:
    void* node_pointer_;
};


#endif //__DBPROXY_AC_ACHIEVEMENT_UPDATE_QUERY_H
