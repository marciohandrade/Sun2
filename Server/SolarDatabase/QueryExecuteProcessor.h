#pragma once
#ifndef __QUERY_EXECUTE_PROCESSOR_H__
#define __QUERY_EXECUTE_PROCESSOR_H__

#include <IThreadInterface.h>

class IDBCInterface;
class SolarDatabase;

//==================================================================================================
/// ť�� ���� �������� �̾Ƽ� ó���ϴ� ������ Ŭ����
/**
  @author
    Kim Min Wook < taiyo@webzen.co.kr > 
  @since
    2004. 10. 20
  @remarks
    - WakeUp �̺�Ʈ�� �߻��ϸ� �����尡 �����.
    - QueryExecuteProcessor �ν��Ͻ��� �������� ť���� ������ ���������� �̴´�.
    - ���� ������ IDBCInterface->Execute(query)�� �����Ѵ�.
  @note
    - QueryExecuteProcessor �ν��Ͻ��� �������� ť�� ���� �������� ������ ������ �� ����.
*/
//==================================================================================================

class QueryExecuteProcessor : public util::IThreadInterface
{
public:
    QueryExecuteProcessor(SolarDatabase& solar_db, IDBCInterface* dbc_instance);
    virtual ~QueryExecuteProcessor();

    virtual void  EndThread();
    virtual DWORD run();

private:
    SolarDatabase& solar_db_;
    HANDLE end_thread_event_;
    IDBCInterface* dbc_instance;

    __DISABLE_COPY(QueryExecuteProcessor);
};


#endif // __QUERY_EXECUTE_PROCESSOR_H__