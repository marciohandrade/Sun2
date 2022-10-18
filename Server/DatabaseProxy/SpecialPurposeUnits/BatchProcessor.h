#ifndef __DBPROXY_BATCH_PROCESSOR_H
#define __DBPROXY_BATCH_PROCESSOR_H
#pragma once

class QueryResult;

namespace batch {
   
//==================================================================================================
// (NOTE) (100103) (WAVERIX) ���� ������ �ƴ� ����� (�� ����) ������ �и��Ѵ�.
// ���� �Ϲ�ȭ�Ǵ� �����̶��, WorkLoadManager�� ��Ͻ�Ű���� �Ѵ�.

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
    //  �۾��� batch_unit:IBatchUnit* �� ����Ѵ�. �ϴ� ����ϸ�,
    //  BatchProcessor�� ���� BatchRelease�� ȣ�� �� ������ ��ü lifetime�� ���� �����ؼ��� �ȵȴ�.
    //  batch_unit�� ��Ͻ��� CurrentStep()�� ����� STEP_NONE�̾�� �Ѵ�.
    static BOOLEAN RegisterBatchUnit(IBatchUnit* batch_unit);
    static BOOLEAN UnregisterBatchUnits();
    BOOLEAN _RegisterBatchUnits(BOOLEAN is_register);

    static BatchProcessor* batch_processor_static_;
    // (NOTE) (100103) (WAVERIX) �� ����, ���� ���� BatchProcess�� ���� ���� ó���� �������� �ʴ´�.
    // �ʿ��ϴٸ� ���� ���� �����ؾ� ������, �ϴ��� �ϳ��� �����ϱ�� �Ѵ�.
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
