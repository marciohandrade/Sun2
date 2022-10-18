#include "stdafx.h"
#include ".\queryexecuteprocessor.h"
#include ".\SolarDatabase.h"
#include ".\IDBCInterface.h"
#include ".\QueryResult.h"


QueryExecuteProcessor::QueryExecuteProcessor(SolarDatabase& solar_db,
                                             IDBCInterface* dbc_instance)
    : solar_db_(solar_db),
      dbc_instance(dbc_instance)
{
    end_thread_event_ = CreateEvent(NULL, false, false, NULL);
}

QueryExecuteProcessor::~QueryExecuteProcessor()
{
    CloseHandle(end_thread_event_);
}

//==================================================================================================

void QueryExecuteProcessor::EndThread()
{
    SetEvent(end_thread_event_);
}

DWORD QueryExecuteProcessor::run()
{
    Query* query = NULL;
    HANDLE hEvent[2];
    hEvent[0] = solar_db_.inter_data_.qep_wakeup_event_;
    hEvent[1] = end_thread_event_;

    const bool kInfinite_loop = true;
    while (kInfinite_loop)
    {
        if (WaitForMultipleObjects(2, hEvent, false, 150 /*INFINITE*/) ==
            WAIT_OBJECT_0 + 1)
        {
            break;
        }
        //
        
        while (kInfinite_loop)
        {
            if (solar_db_.inter_data_.query_list_->IsEmpty()) {
                break;
            }
            util::RingBufferInfo ring_buffer_info;
            solar_db_.inter_data_.query_result_list_->WritableInfo(&ring_buffer_info);
            if ((ring_buffer_info.first.size + ring_buffer_info.second.size) < sizeof(query)) {
                break;
            }
            ;{  //critical section
                solar_db_.inter_data_.query_list_->Dequeue(\
                    reinterpret_cast<uint8_t*>(&query), sizeof(query));
            };
            if (query == NULL) {
                break;
            }
            //
            const eQUERYTYPE query_type = query->GetQueryType();
            if (query_type == eGeneralQuery)
            {
                DWORD dwStartQueryTime = GetTickCount();
                query->SetStartQueryTime(dwStartQueryTime);
                /// Execute Query and push in QueryResultList
                if (dbc_instance->Execute(query) == false)
                {
                    // Äõ¸® ½ÇÆÐ!
                    // nothing to do
                }
                query->SetQueryTime(GetTickCount() - dwStartQueryTime);
            }
            else if (query_type == eLargeQuery)
            {
                DWORD dwStartQueryTime = GetTickCount();
                query->SetStartQueryTime(dwStartQueryTime);
                /// Execute Query and push in QueryResultList
                if (dbc_instance->ExecuteLargeData(query) == false)
                {
                    // Äõ¸® ½ÇÆÐ!
                    // nothing to do
                }
                query->SetQueryTime(GetTickCount() - dwStartQueryTime);
            };
            ;{  // critical section
                solar_db_.inter_data_.query_result_list_->Enqueue(\
                    reinterpret_cast<uint8_t*>(&query), sizeof(query));
            };
        } //end of while(inter)
    } //end of while
    //
    return 0;
}
