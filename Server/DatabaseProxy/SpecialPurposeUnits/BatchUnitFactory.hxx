
#ifndef __DBPROXY_SPECIALPURPOSEUNITS_BATCHUNITFACTORY_HXX
#define __DBPROXY_SPECIALPURPOSEUNITS_BATCHUNITFACTORY_HXX
#pragma once

#include "QueryObjects/QueryPoolFactory.h"
#include "ACConvertUnit.h"
#define BATCHUNIT_REGISTERING_MARK
#include "./ACAchievementUpdateUnit.hxx"
#undef BATCHUNIT_REGISTERING_MARK

#define BATCHUNIT_REGISTERING_MARK
#if !defined(_JAPAN)
  #include "./RankReportUpdateUnit.hxx"
#else
  #include "./RankReportUpdateUnitForJapan.hxx"
#endif
#undef BATCHUNIT_REGISTERING_MARK

#include "BatchQueryPool.hxx"

class BatchunitFactory
{
public:
    enum UNIT_TYPE {
        UNIT_TYPE_NONE = 0,
        AC_ACHIEVEMENT_UPDATE = 1,
        AC_CONVERT = 2,
        RANK_REPORT_INVENTORY = 3,
        RANK_REPORT_WAREHOUSE = 4,
        RANK_REPORT_GUILD_WAREHOUSE = 5,
    };

public:
    static UNIT_TYPE GetUnitType()
    {
        return static_cast<UNIT_TYPE>(GetPrivateProfileInt("DB_UNIT", "type", 1, "./DBPUnit.ini"));
    };

    static batch::IBatchUnit* CreateBatchUnit(UNIT_TYPE custom_select)
    {        
        const UNIT_TYPE unit_type = (custom_select == UNIT_TYPE_NONE) ?
            GetUnitType() : custom_select; __TOUCHED(unit_type);

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        switch (unit_type)
        {
        case AC_ACHIEVEMENT_UPDATE:
            return new batch_unit::ACAchievementUpdateUnit;
    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        case AC_CONVERT:            
            return new batch_unit::ACConvertUnit;
    #endif
    #ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
        case RANK_REPORT_INVENTORY:
        case RANK_REPORT_WAREHOUSE:
        case RANK_REPORT_GUILD_WAREHOUSE:
            return new batch_unit::RankReportUpdateUnit;
    #endif
        }
#endif

        ASSERT(!"Not Define Unit Type");
        __debugbreak();
        return NULL;        
    };

    static batch::IBatchQueryPool* CreateBatchQueryPool(UNIT_TYPE custom_select)
    {        
        const UNIT_TYPE unit_type = (custom_select == UNIT_TYPE_NONE) ?
            GetUnitType() : custom_select; __TOUCHED(unit_type);

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        switch (unit_type)
        {
        case AC_ACHIEVEMENT_UPDATE:
            return new batch::ACAchievementUpdateBatchQueryPool;
    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        case AC_CONVERT:            
            return new batch::ACConvertBatchQueryPool;
    #endif
    #ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
        case RANK_REPORT_INVENTORY:
        case RANK_REPORT_WAREHOUSE:
        case RANK_REPORT_GUILD_WAREHOUSE:
            return new batch::RankReporterQueryPool;
    #endif
        }
#endif

        ASSERT(!"Not Define Unit Type");
        __debugbreak();
        return NULL;        
    };
};


#endif //__DBPROXY_SPECIALPURPOSEUNITS_BATCHUNITFACTORY_HXX
