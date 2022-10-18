
#include "stdafx.h"
#include "WriteFileLog.h"
#include <map>

WriteFileLog::WriteFileLog()
{
}

WriteFileLog::~WriteFileLog()
{
}

bool WriteFileLog::WriteLogFile(TCHAR * const logfilepath, TCHAR * const txtfilepath)
{
    CWriteLogMng LogMgr;

    if(!LogMgr.Open(logfilepath))
    {
        return false;
    }

    HANDLE txtfile = ::CreateFile(txtfilepath, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);

    if(txtfile == NULL)
    {
        LogMgr.Close();
        return false;
    }

    BaseLogData* pObj = NULL;
    std::map<DWORD, LOG_TYPE> log_info;

    LOG_TYPE log_type;
    log_type.log_count_ = 0;

    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "���ӷα�");
    log_info[LOG_SESSION] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "�����۷α�");
    log_info[LOG_ITEM] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "����ġ");
    log_info[LOG_EXP] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "�׼�");
    log_info[LOG_ACTION] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "��ų");
    log_info[LOG_SKILL] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "������");
    log_info[LOG_SNAPSHOT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "������ (ĳ����)");
    log_info[SNAPSHOT_REGULAR] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "������ (�κ�)");
    log_info[SNAPSHOT_LOGININVEN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "������ (â��)");
    log_info[SNAPSHOT_WAREHOUSE_OEPN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "ä�� �α�");
    log_info[LOG_CHAT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "�̺�Ʈ");
    log_info[LOG_EVENT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "GM(���) �α�");
    log_info[LOG_ADMIN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "���α�");
    log_info[LOG_MONEY] = log_type;
#ifdef _GUILD_LOG
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "���α�");
    log_info[LOG_GUILD] = log_type;
#endif
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "��������Ʈ�α�");
    log_info[LOG_ACCUMULATEPOINT] = log_type;


    while(true)
    {
        pObj = LogMgr.Read();

        if(pObj == NULL)
            break;

        if( pObj->GetCategory() == LOG_SNAPSHOT) 
        {
                switch(pObj->GetLogType())
                {
                case SNAPSHOT_REGULAR:
                    {
                        log_info[SNAPSHOT_REGULAR].log_count_++;      
                    }
                    break;
                case SNAPSHOT_LOGININVEN:
                case SNAPSHOT_LOGOUTINVEN:					//�κ��丮�� ����
                case SNAPSHOT_WAREHOUSE_OPEN_INVEN:			// â�� ���� �κ��� ������ ����
                case SNAPSHOT_WAREHOUSE_CLOSE_INVEN:		// â�� ������ �κ��� ������ ����
                    {
                        log_info[SNAPSHOT_LOGININVEN].log_count_++;      
                    }
                    break;
                case SNAPSHOT_WAREHOUSE_OEPN:
                case SNAPSHOT_WAREHOUSE_CLOSE:		//â�� ����
                    {
                        log_info[SNAPSHOT_WAREHOUSE_OEPN].log_count_++;      
                    }
                }
        }
        
        
        log_info[pObj->GetCategory()].log_count_++;      
    }

    char display[MAX_PATH];
    DWORD writed = 0;

    std::map<DWORD, LOG_TYPE>::iterator iter = log_info.begin();

    for(; iter != log_info.end() ; iter++)
    {
        ZeroMemory(display,MAX_PATH);
        StringCchPrintf(display, sizeof(display), _T("%s: %d\r\n"), 
                        iter->second.log_decription, iter->second.log_count_);
        ::WriteFile(txtfile,display,strlen(display),&writed,NULL);
    }

    ::CloseHandle(txtfile);
    txtfile = NULL;

    return true;
}

//DWORD log_session           = 0;//���ӷα�
//DWORD log_item              = 0;//�����۷α�
//DWORD log_exp               = 0;//����ġ
//DWORD log_action            = 0;//�׼�
//DWORD log_skill             = 0;//��ų
//DWORD log_snapshot          = 0;//������
//DWORD log_chat              = 0;//ä�� �α�
//DWORD log_event             = 0;//�̺�Ʈ
//DWORD log_admin             = 0;//GM(���) �α�
//DWORD log_money             = 0;//���α�
//DWORD log_guild             = 0;//���α�
//DWORD log_accumulatepoint   = 0;//��������Ʈ�α�


//switch(pObj->GetCategory())
//{
//case LOG_SESSION:
//    log_session++;
//    break;
//case LOG_ITEM:
//    log_item++;
//    break;
//case LOG_EXP:
//    log_exp++;
//    break;
//case LOG_ACTION:
//    log_action++;
//    break;
//case LOG_SKILL:
//    log_skill++;
//    break;
//case LOG_SNAPSHOT:
//    log_snapshot++;
//    break;
//case LOG_CHAT:
//    log_chat++;
//    break;
//case LOG_EVENT:
//    log_event++;
//    break;
//case LOG_ADMIN:
//    log_admin++;
//    break;
//case LOG_MONEY:
//    log_money++;
//    break;
//case LOG_GUILD:
//    log_guild++;
//    break;
//case LOG_ACCUMULATEPOINT:
//    log_accumulatepoint++;
//    break;
//}
