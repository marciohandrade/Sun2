#include "stdafx.h"
#include "BatchUnitFactory.hxx"

#if DBPROXY_TYPE_SELECTOR != DBPROXY_TYPE_UNIT

namespace batch {

BOOLEAN
BatchProcessor::RegisterBatchUnits()
{
    ASSERT(!"Unexepected call");
    return false;
}

BOOLEAN
BatchProcessor::UnregisterBatchUnits()
{
    ASSERT(!"Unexepected call");
    return false;
}

}; //end of namespace 'batch'

#endif

//==================================================================================================

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

__IMPL_SAFECUSTOMPOOL_PTR(Query_GetACStreamInfo);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SetACStreamInfo);

__IMPL_SAFECUSTOMPOOL_PTR(Query_Select_Character_ACInfo);
__IMPL_SAFECUSTOMPOOL_PTR(Query_Update_Character_ACInfo);

#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
__IMPL_SAFECUSTOMPOOL_PTR(Query_GetItemList);
__IMPL_SAFECUSTOMPOOL_PTR(Query_SetRankReport);
#endif

namespace batch {
;

static IBatchUnit* batch_db_unit_static_ = NULL;

BOOLEAN
BatchProcessor::_RegisterBatchUnits(BOOLEAN is_register)
{
    if (is_register)
    {
        batch_query_pool_ = BatchunitFactory::CreateBatchQueryPool(BatchunitFactory::UNIT_TYPE_NONE);
        batch_query_pool_->Init();
    }

    // (NOTE) is_register일 경우, 이곳에서 IBatchUnit들을 일괄 등록한다. 해제시 일괄 해제
    BOOLEAN registered = false;

    if (is_register)
    {
        batch_db_unit_static_ = BatchunitFactory::CreateBatchUnit(BatchunitFactory::UNIT_TYPE_NONE);

        registered = RegisterBatchUnit(batch_db_unit_static_);
        if (registered == false)
        {
            SAFE_DELETE(batch_db_unit_static_)
            return false;
        }
    }
    else
    {
        SAFE_DELETE(batch_db_unit_static_);
    }

    if (is_register == false) {
        SAFE_DELETE(batch_query_pool_);
    }

    return true;
}

BOOLEAN
BatchProcessor::RegisterBatchUnits()
{
    if (batch_processor_static_ == NULL) {
        return false;
    }

    BatchProcessor* const this_ = batch_processor_static_;
#if !defined(_NA000000_110218_RANK_REPORTER_IN_DBUNIT)
    this_->_RegisterBatchUnits(true);
#endif
#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
    ;{
        // RANK_REPORT_INVENTORY, RANK_REPORT_WAREHOUSE, RANK_REPORT_GUILD_WAREHOUSE
        // sharing pool
        this_->batch_query_pool_ =
            BatchunitFactory::CreateBatchQueryPool(BatchunitFactory::RANK_REPORT_INVENTORY);
        this_->batch_query_pool_->Init();

        if (IBatchUnit* batch_unit =
                BatchunitFactory::CreateBatchUnit(BatchunitFactory::RANK_REPORT_INVENTORY))
        {
            batch_unit->SetUnitType(BatchunitFactory::RANK_REPORT_INVENTORY);
            bool registered = RegisterBatchUnit(batch_unit) != false;
            if (registered == false)
            {
                SAFE_DELETE(batch_unit);
                SUNLOG(eCRITICAL_LOG, _T("fail to create batch unit"));
                return false;
            }
        }
        if (IBatchUnit* batch_unit =
                BatchunitFactory::CreateBatchUnit(BatchunitFactory::RANK_REPORT_WAREHOUSE))
        {
            batch_unit->SetUnitType(BatchunitFactory::RANK_REPORT_WAREHOUSE);
            bool registered = RegisterBatchUnit(batch_unit) != false;
            if (registered == false)
            {
                SAFE_DELETE(batch_unit);
                SUNLOG(eCRITICAL_LOG, _T("fail to create batch unit"));
                return false;
            }
        }
        if (IBatchUnit* batch_unit =
                BatchunitFactory::CreateBatchUnit(BatchunitFactory::RANK_REPORT_GUILD_WAREHOUSE))
        {
            batch_unit->SetUnitType(BatchunitFactory::RANK_REPORT_GUILD_WAREHOUSE);
            bool registered = RegisterBatchUnit(batch_unit) != false;
            if (registered == false)
            {
                SAFE_DELETE(batch_unit);
                SUNLOG(eCRITICAL_LOG, _T("fail to create batch unit"));
                return false;
            }
        }
    };
#endif
    return true;
}

BOOLEAN
BatchProcessor::UnregisterBatchUnits()
{
    if (batch_processor_static_ == NULL) {
        return false;
    }

    BatchProcessor* const this_ = batch_processor_static_;
#if !defined(_NA000000_110218_RANK_REPORTER_IN_DBUNIT)
    this_->_RegisterBatchUnits(false);
#endif
#ifdef _NA000000_110218_RANK_REPORTER_IN_DBUNIT
    ;{
        SAFE_DELETE(this_->batch_query_pool_);
        // NOTE: other units are released in BatchRelease
    };
#endif
    return true;
}

}; //end of namespace 'batch'

#endif //DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

