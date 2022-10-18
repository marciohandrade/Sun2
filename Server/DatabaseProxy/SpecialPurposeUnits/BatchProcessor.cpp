#include "stdafx.h"
#include "BatchProcessor.h"
#include "DBProxyServerEx.h"

namespace batch {

//==================================================================================================
BatchProcessor*
BatchProcessor::batch_processor_static_ = NULL;


void
BatchProcessor::CreateInstance() {
    // thread unsafe singleton
    if (batch_processor_static_ == NULL) {
        batch_processor_static_ = new BatchProcessor;
        batch_processor_static_->RegisterBatchUnits();
    }
}

BatchProcessor::BatchProcessor()
    : auto_mode_(false)
    , started_(false)
    , batch_unit_(NULL)
    , batch_query_pool_(NULL)
{
    auto_mode_ = !!GetPrivateProfileInt("DBPUNIT_DATA", "UNIT_AUTO_MODE", 0, "./DBPUnit.ini");
}

BatchProcessor::~BatchProcessor()
{
    UnregisterBatchUnits();
}

void
BatchProcessor::ChangeRunState(BOOLEAN start)
{
    if (batch_processor_static_ == NULL)
        return;

    BatchProcessor* const this_ = batch_processor_static_;
    BOOLEAN prev_state = this_->started_;
    BOOLEAN post_state = !!start;
    this_->started_ = post_state;

    size_t pendings = this_->batch_units_.size();
    SUNLOG(eCRITICAL_LOG, "|Msg=Run batch unit processor|Prev=%s|Next=%s|Pendings=%d|",
           prev_state ? "Started" : "Stopped",
           post_state ? "Started" : "Stopped",
           pendings);
}

BOOLEAN
BatchProcessor::Update()
{
    if (batch_processor_static_ == NULL)
        return false;

    BatchProcessor* const this_ = batch_processor_static_;

    if (this_->started_ == false)
        return false;

    IBatchUnit* batch_unit = this_->batch_unit_;
    if (batch_unit == NULL)
    {
        PendingBatchUnits& pendings = this_->batch_units_;
        if (pendings.empty())
        {
            ChangeRunState(false);
        #if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
            if (this_->auto_mode_)
                g_DBProxyServer.RequestShutdown();
        #endif

            return false;
        }
        this_->batch_unit_ = batch_unit = pendings.front();
        pendings.pop_front();
    }

    const IBatchUnit::eSTEP step = batch_unit->CurrentStep();
    switch (step)
    {
    case batch_unit->STEP_NONE:
        batch_unit->BatchInit();
        break;
    case batch_unit->STEP_PROCESSING:
        batch_unit->BatchProcess();
        break;
    case batch_unit->STEP_COMPLETED:
        batch_unit->BatchRelease();
        this_->batch_unit_ = NULL;
        break;
    case batch_unit->STEP_RELEASED:
        //ASSERT(!"Unexpected batch unit step");
        return false;
    }

    return true;
}

BOOLEAN
BatchProcessor::RegisterBatchUnit(IBatchUnit* batch_unit)
{
    if (batch_unit == NULL ||
        batch_unit->CurrentStep() != batch_unit->STEP_NONE)
    {
        return false;
    }

    // no check duplicated batch unit
    BatchProcessor* const this_ = batch_processor_static_;
    PendingBatchUnits& pendings = this_->batch_units_;
    pendings.push_back(batch_unit);

    return true;
}

void
BatchProcessor::QueryResultCallBack(DWORD index, QueryResult* query_result)
{
    if (batch_processor_static_ == NULL)
        return;
    BatchProcessor* const this_ = batch_processor_static_;

    IBatchUnit* const batch_unit = \
        reinterpret_cast<IBatchUnit*>(query_result->GetVoidObject());

    if(batch_unit != this_->batch_unit_) {
        SUNLOG(eCRITICAL_LOG, "Warning! skipped result occurred!");
        return;
    }

    batch_unit->DBResult(index, query_result);
}

}; //end of namespace 'batch'

