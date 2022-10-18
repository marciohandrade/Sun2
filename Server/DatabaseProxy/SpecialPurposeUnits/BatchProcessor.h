#ifndef __DBPROXY_BATCH_PROCESSOR_H
#define __DBPROXY_BATCH_PROCESSOR_H
#pragma once

class QueryResult;

namespace batch {
   
//==================================================================================================
// (NOTE) (100103) (WAVERIX) 정식 로직이 아닌 관계로 (툴 목적) 별도로 분리한다.
// 만약 일반화되는 로직이라면, WorkLoadManager에 등록시키도록 한다.

struct IBatchUnit
{
    enum eSTEP {
        STEP_NONE           = 0,
        STEP_PROCESSING     ,
        STEP_COMPLETED      ,
        STEP_RELEASED       ,
    };
    virtual void SetUnitType(int tag) = 0;
    virtual eSTEP   CurrentStep() const = 0;
    virtual BOOLEAN BatchInit() = 0;
    virtual BOOLEAN BatchProcess() = 0; // tokenized task
    virtual BOOLEAN BatchRelease() = 0;
    virtual void DBResult(DWORD index, QueryResult* query_result) = 0;
};

//==================================================================================================

class IBatchQueryPool;

class BatchProcessor
{
private:
    typedef STLX_DEQUE<IBatchUnit*> PendingBatchUnits;
    BatchProcessor();
    ~BatchProcessor();
public:
    static void CreateInstance();
    static void DestroyInstance();

    static BOOLEAN Update();

    static BOOLEAN IsAutoMode();
    static BOOLEAN Started();
    static void ChangeRunState(BOOLEAN start);
    static BOOLEAN RegisterBatchUnits();

    static void QueryResultCallBack(DWORD index, QueryResult* query_result);
private:
    // @desc
    //  작업할 batch_unit:IBatchUnit* 을 등록한다. 일단 등록하면,
    //  BatchProcessor에 의해 BatchRelease가 호출 될 때까지 객체 lifetime을 임의 수정해서는 안된다.
    //  batch_unit은 등록시점 CurrentStep()의 결과가 STEP_NONE이어야 한다.
    static BOOLEAN RegisterBatchUnit(IBatchUnit* batch_unit);
    static BOOLEAN UnregisterBatchUnits();
    BOOLEAN _RegisterBatchUnits(BOOLEAN is_register);

    static BatchProcessor* batch_processor_static_;
    // (NOTE) (100103) (WAVERIX) 현 시점, 복수 개의 BatchProcess를 돌릴 만한 처리는 존재하지 않는다.
    // 필요하다면 복수 개를 수행해야 겠지만, 일단은 하나만 유지하기로 한다.
    BOOLEAN auto_mode_;
    BOOLEAN started_;
    IBatchUnit* batch_unit_;
    IBatchQueryPool* batch_query_pool_;
    PendingBatchUnits batch_units_;
};


inline void
BatchProcessor::DestroyInstance() {
    SAFE_DELETE(batch_processor_static_);
}

inline BOOLEAN
BatchProcessor::IsAutoMode() {
    if (batch_processor_static_ == NULL)
        return false;
    return !!batch_processor_static_->auto_mode_;
}

inline BOOLEAN
BatchProcessor::Started() {
    if (batch_processor_static_ == NULL)
        return false;
    return !!batch_processor_static_->started_;
}

}; //end of namespace 'batch'

#endif //__DBPROXY_BATCH_PROCESSOR_H
