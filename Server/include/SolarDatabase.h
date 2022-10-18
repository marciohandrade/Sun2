#pragma once
#ifndef __SOLAR_DATABAES_H__
#define __SOLAR_DATABAES_H__

/*=======================================================================================================================
// 사용법 예제)


    !!!!! Caution !!!!!
    // 셋중의 하나의 Define해야 함
    // Database 모듈을 사용하고자 하는 프로젝트의 stdafx.h과 같은 공통 include파일에 넣어주는 것을 권장한다.
    // 혹은 프로젝트 속성의 precompile define에 넣어 두는 것이 좋다.
    #define _USING_ODBC      // ODBC사용
    //#define _USING_OLEDB   // OLEDB사용
    //#define _USING_SQLLIB  // SQLLIB사용


    // 초기화 코드
    SolarDatabaseDesc db_desc;
    db_desc.database_module_type = DBCInterfaceType_ODBC;
    //db_desc.database_module_type = DBCInterfaceType_OLEDB;
    //db_desc.database_module_type = DBCInterfaceType_SQLLIB;
    db_desc.query_pool_size = 50000;
    db_desc.query_processor_num = 1;
    db_desc.database_ip = "IP주소";
    db_desc.database_name = "데이터베이스";
    db_desc.user_name = "계정아디";
    db_desc.user_password = "비밀번호";
    db_desc.fnptr_error_message = CallBackMessage1;
    db_desc.fnptr_query_result = CallBackQueryResult1;

    m_pDB = new SolarDatabase;
    m_pDB->Initialize(db_desc);


    // 계속 호출되어야 하는 부분
    m_pDB->Update();


    // DB모듈 종료
    m_pDB->Release();
    delete m_pDB;


    // 쿼리 코드
    // 메모리 풀에서 쿼리object 할당
    AccountTestQuery* query = QUERYALLOC(AccountTestQuery);
    query->SetIndex(1234); // 쿼리 구분을 위해
    query->SetVoidObject(this); // 커스텀 포인터 전달
    query->SetQuery("select* from AccountTest where UserGuid =123"); // 쿼리 구문
    m_pDB->DBQuery(query);


    // 쿼리 결과 콜백 코드
    void CallBackQueryResult1(DWORD dwIndex, QueryResult* pData)
    {
        switch (dwIndex)
        {
        case 1234:
            {
            // 쿼리 처리
            }
            break;
        }

        // 쿼리사용후 메모리 반환
        QUERYFREE(AccountTestQuery, (AccountTestQuery *)pData);
    }

    // 에러 메시지 콜백 코드
    void CallBackMessage1(char* szMessage)
    {
        printf(szMessage);
        printf("\n");
    }
=======================================================================================================================*/

//=======================================================================================================================
/// Database에 연결하여 Query+Result하는 데이터베이스 클래스
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr >	
    @since
        2004. 10. 20
    @remarks
        - SolarDatabase는 ODBC, OLEDB, MSSQLLIB 모듈을 제공한다.	
        - SolarDatabase는 데이터 복사 memcpy를 줄였다.바인딩시 데이터 복사 일어남
    < Usage >
        - DB에 연결하고자하는 프로젝트(ex:DBProxy)에서 SolarDatabase 인스턴스를 가진다.
        - SolarDatabaseDesc의 관련정보를 넣고 Initialize()를 호출한다.
        - 매 프레임 적당한 시간 간격으로 Update()를 호출한다.
        - 쿼리할 데이터를 Query class의 구조에 맞게 정의(Binding)한 후 DBQuery()를 호출한다. (Non-blocking function)
        - 쿼리한 결과는 SolarDatabaseDesc에 정의한 콜백함수로 통보된다.
        - DB연결을 종료하려면 SolarDatabase의 Relese()를 호출한다.
    @note
        - MSSQLLIB는 MSSQLDatabase이외에는 사용할 수 없다.
        - SolarDatabase의 Relese()함수 호출후 DBQuery()는 FALSE를 리턴한다.
        - SolarDatabase의 DBQuery()함수가 TRUE를 리턴한 Query는 종료되더라도 안전하게 쿼리된다.
        - SolarDatabaseDesc의 query_processor_num의 값이 2이상일때는 DBQuery()한 쿼리결과의 순서를 보장할 수 없다.
        (단, 5배 이상의 속도향상을 볼 수 있다.)
        - DBQuery()에 인자로 넘기는 Query 인스턴스는 User Side에서 메모리 풀로 관리하는 것을 권장한다.
        - Module사용에 필요한 관련 파일들 : 
        SolarDatabase.dll SolarDatabase.lib SolarDatabase.h SolarDatabaseDefine.h QueryResult.h

    @todo
        - 2005.3.6 : ODBC의 에러 메시지 출력!
    @history
        - ~10/08/06|custom module|
        - 10/08/06|waverix|code arrangement related to change types and making const member
*/
//=======================================================================================================================

#include "SolarDatabaseDefine.h"
#include <ThreadPool.h>
#include <WxRingBuffer.h>

class QueryResult;
class IDBCInterface;

//==================================================================================================

class SOLARDATABASE_API SolarDatabase
{
    friend class QueryExecuteProcessor;
public:
    SolarDatabase();
    ~SolarDatabase();

    bool  Initialize(const SolarDatabaseDesc& desc);
    void  Update();
    void  Release();

    bool  DBQuery(Query* query);

    bool IsRemainsQueries() const;
    int GetBufferedQueryNum() const;

    bool  ReConnect(WORD db_index);
private:
    //
    inline void SwitchQueryResultList();
    //
    struct SolarDBData {
        IDBCInterface**   dbc_instance_array_;
        util::ThreadPool* thread_pool_;
        fnCallBackMessage fnptr_error_message_;
        fnCallBackQueryResult fnptr_query_result_;
        WORD   query_processor_num_;
        bool   is_end_process_;
        HANDLE qep_wakeup_event_;

        util::WxRingBuffer* query_list_;
        util::WxRingBuffer* query_result_list_;
    };
    SolarDBData inter_data_;
};

//==================================================================================================
// implements


//==================================================================================================


#endif // __SOLAR_DATABAES_H__