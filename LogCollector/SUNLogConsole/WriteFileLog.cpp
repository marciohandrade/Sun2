
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

    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "접속로그");
    log_info[LOG_SESSION] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "아이템로그");
    log_info[LOG_ITEM] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "경험치");
    log_info[LOG_EXP] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "액션");
    log_info[LOG_ACTION] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "스킬");
    log_info[LOG_SKILL] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "스냅샷");
    log_info[LOG_SNAPSHOT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "스냅샷 (캐릭터)");
    log_info[SNAPSHOT_REGULAR] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "스냅샷 (인벤)");
    log_info[SNAPSHOT_LOGININVEN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "스냅샷 (창고)");
    log_info[SNAPSHOT_WAREHOUSE_OEPN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "채팅 로그");
    log_info[LOG_CHAT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "이벤트");
    log_info[LOG_EVENT] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "GM(운영자) 로그");
    log_info[LOG_ADMIN] = log_type;
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "돈로그");
    log_info[LOG_MONEY] = log_type;
#ifdef _GUILD_LOG
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "길드로그");
    log_info[LOG_GUILD] = log_type;
#endif
    StringCchCopy(log_type.log_decription, sizeof(log_type.log_decription), "적립포인트로그");
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
                case SNAPSHOT_LOGOUTINVEN:					//인벤토리만 저장
                case SNAPSHOT_WAREHOUSE_OPEN_INVEN:			// 창고를 열때 인벤의 아이템 정보
                case SNAPSHOT_WAREHOUSE_CLOSE_INVEN:		// 창고를 닫을때 인벤의 아이템 정보
                    {
                        log_info[SNAPSHOT_LOGININVEN].log_count_++;      
                    }
                    break;
                case SNAPSHOT_WAREHOUSE_OEPN:
                case SNAPSHOT_WAREHOUSE_CLOSE:		//창고만 저장
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

//DWORD log_session           = 0;//접속로그
//DWORD log_item              = 0;//아이템로그
//DWORD log_exp               = 0;//경험치
//DWORD log_action            = 0;//액션
//DWORD log_skill             = 0;//스킬
//DWORD log_snapshot          = 0;//스냅샷
//DWORD log_chat              = 0;//채팅 로그
//DWORD log_event             = 0;//이벤트
//DWORD log_admin             = 0;//GM(운영자) 로그
//DWORD log_money             = 0;//돈로그
//DWORD log_guild             = 0;//길드로그
//DWORD log_accumulatepoint   = 0;//적립포인트로그


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
