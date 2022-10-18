#pragma once
#ifndef __QUERY_EXECUTE_PROCESSOR_H__
#define __QUERY_EXECUTE_PROCESSOR_H__

#include <IThreadInterface.h>

class IDBCInterface;
class SolarDatabase;

//==================================================================================================
/// 큐에 쌓인 쿼리들을 뽑아서 처리하는 스레드 클래스
/**
  @author
    Kim Min Wook < taiyo@webzen.co.kr > 
  @since
    2004. 10. 20
  @remarks
    - WakeUp 이벤트가 발생하면 스레드가 깨어난다.
    - QueryExecuteProcessor 인스턴스가 여러개면 큐에서 쿼리를 경쟁적으로 뽑는다.
    - 뽑은 쿼리는 IDBCInterface->Execute(query)를 실행한다.
  @note
    - QueryExecuteProcessor 인스턴스가 여러개면 큐에 쌓인 쿼리들은 순서를 보장할 수 없다.
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