#pragma once
#ifndef __SOLAR_DATABAES_H__
#define __SOLAR_DATABAES_H__

/*=======================================================================================================================
// ���� ����)


    !!!!! Caution !!!!!
    // ������ �ϳ��� Define�ؾ� ��
    // Database ����� ����ϰ��� �ϴ� ������Ʈ�� stdafx.h�� ���� ���� include���Ͽ� �־��ִ� ���� �����Ѵ�.
    // Ȥ�� ������Ʈ �Ӽ��� precompile define�� �־� �δ� ���� ����.
    #define _USING_ODBC      // ODBC���
    //#define _USING_OLEDB   // OLEDB���
    //#define _USING_SQLLIB  // SQLLIB���


    // �ʱ�ȭ �ڵ�
    SolarDatabaseDesc db_desc;
    db_desc.database_module_type = DBCInterfaceType_ODBC;
    //db_desc.database_module_type = DBCInterfaceType_OLEDB;
    //db_desc.database_module_type = DBCInterfaceType_SQLLIB;
    db_desc.query_pool_size = 50000;
    db_desc.query_processor_num = 1;
    db_desc.database_ip = "IP�ּ�";
    db_desc.database_name = "�����ͺ��̽�";
    db_desc.user_name = "�����Ƶ�";
    db_desc.user_password = "��й�ȣ";
    db_desc.fnptr_error_message = CallBackMessage1;
    db_desc.fnptr_query_result = CallBackQueryResult1;

    m_pDB = new SolarDatabase;
    m_pDB->Initialize(db_desc);


    // ��� ȣ��Ǿ�� �ϴ� �κ�
    m_pDB->Update();


    // DB��� ����
    m_pDB->Release();
    delete m_pDB;


    // ���� �ڵ�
    // �޸� Ǯ���� ����object �Ҵ�
    AccountTestQuery* query = QUERYALLOC(AccountTestQuery);
    query->SetIndex(1234); // ���� ������ ����
    query->SetVoidObject(this); // Ŀ���� ������ ����
    query->SetQuery("select* from AccountTest where UserGuid =123"); // ���� ����
    m_pDB->DBQuery(query);


    // ���� ��� �ݹ� �ڵ�
    void CallBackQueryResult1(DWORD dwIndex, QueryResult* pData)
    {
        switch (dwIndex)
        {
        case 1234:
            {
            // ���� ó��
            }
            break;
        }

        // ��������� �޸� ��ȯ
        QUERYFREE(AccountTestQuery, (AccountTestQuery *)pData);
    }

    // ���� �޽��� �ݹ� �ڵ�
    void CallBackMessage1(char* szMessage)
    {
        printf(szMessage);
        printf("\n");
    }
=======================================================================================================================*/

//=======================================================================================================================
/// Database�� �����Ͽ� Query+Result�ϴ� �����ͺ��̽� Ŭ����
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr >	
    @since
        2004. 10. 20
    @remarks
        - SolarDatabase�� ODBC, OLEDB, MSSQLLIB ����� �����Ѵ�.	
        - SolarDatabase�� ������ ���� memcpy�� �ٿ���.���ε��� ������ ���� �Ͼ
    < Usage >
        - DB�� �����ϰ����ϴ� ������Ʈ(ex:DBProxy)���� SolarDatabase �ν��Ͻ��� ������.
        - SolarDatabaseDesc�� ���������� �ְ� Initialize()�� ȣ���Ѵ�.
        - �� ������ ������ �ð� �������� Update()�� ȣ���Ѵ�.
        - ������ �����͸� Query class�� ������ �°� ����(Binding)�� �� DBQuery()�� ȣ���Ѵ�. (Non-blocking function)
        - ������ ����� SolarDatabaseDesc�� ������ �ݹ��Լ��� �뺸�ȴ�.
        - DB������ �����Ϸ��� SolarDatabase�� Relese()�� ȣ���Ѵ�.
    @note
        - MSSQLLIB�� MSSQLDatabase�̿ܿ��� ����� �� ����.
        - SolarDatabase�� Relese()�Լ� ȣ���� DBQuery()�� FALSE�� �����Ѵ�.
        - SolarDatabase�� DBQuery()�Լ��� TRUE�� ������ Query�� ����Ǵ��� �����ϰ� �����ȴ�.
        - SolarDatabaseDesc�� query_processor_num�� ���� 2�̻��϶��� DBQuery()�� ��������� ������ ������ �� ����.
        (��, 5�� �̻��� �ӵ������ �� �� �ִ�.)
        - DBQuery()�� ���ڷ� �ѱ�� Query �ν��Ͻ��� User Side���� �޸� Ǯ�� �����ϴ� ���� �����Ѵ�.
        - Module��뿡 �ʿ��� ���� ���ϵ� : 
        SolarDatabase.dll SolarDatabase.lib SolarDatabase.h SolarDatabaseDefine.h QueryResult.h

    @todo
        - 2005.3.6 : ODBC�� ���� �޽��� ���!
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