/*------------------------------------------------------------------------------
 * SUN_LogReader.cpp
 *------------------------------------------------------------------------------
 * LogReader Ŭ������ ��ӹ޾� ���� �ĸ��� ���� �α� ���� Ŭ�����̴�.
 *
 * �ĸ��� �α��� ������ ���� ���� ���� �� �߰���
 * �� Ŭ������ �����Ͽ� ����Ѵ�.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "LogCriticalSection.h"
#include "LogMain.h"
#include "SUN_LogReader.h"
#include "LogCollectorLog.h"
#include "SUN_LogWriter.h"

#ifdef _DEBUG
ULONGLONG ulPreOffet = 0;
BOOL bFirst = TRUE;
#endif //_DEBUG
extern LogCriticalSection * g_pGlobalCS;

extern LogMain g_SUNLogMain;

/**	----------------------------------------------------------------------------
	\brief	��¥ ����, IP ������ ��� �ɹ� ���� ����

	\param	
	\return	
------------------------------------------------------------------------------*/
SUN_LogReader::SUN_LogReader(INT nWorld, TCHAR * szStartDate, TCHAR * szEndDate, TCHAR * szServerIP, TCHAR * szPrefix, BOOL is_restore)
{
	if ( NULL != szStartDate )
	{
		if (FAILED(StringCchCopy(m_szStartDate, sizeof(m_szStartDate), szStartDate)) )
		{
			_ASSERTE(!"StringCchCopy");
		}
		
	}

	if ( NULL != szEndDate )
	{
		if (FAILED(StringCchCopy(m_szEndDate, sizeof(m_szEndDate), szEndDate)) )
		{
			_ASSERTE(!"StringCchCopy");
		}
		
	}

	if ( NULL != szServerIP )
	{
		if (FAILED(StringCchCopy(m_szServerIP, sizeof(m_szServerIP), szServerIP)) )
		{
			_ASSERTE(!"StringCchCopy");
		}
	}

	if(NULL != szPrefix)
	{
		if (FAILED(StringCchCopy(m_szPrefix, sizeof(m_szPrefix), szPrefix)) )
		{
			_ASSERTE(!"StringCchCopy");
		}

	}

	m_nWorld = nWorld;
    is_restore_ = is_restore;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
SUN_LogReader::~SUN_LogReader()
{
}
#include <winerror.h>
/**	----------------------------------------------------------------------------
	\brief	�α� ���� ���� �б� ����

	\param	
	\return	
------------------------------------------------------------------------------*/
int SUN_LogReader::DoReadLogFile(SERVERINFO *pstInfo)
{
	if (NULL == pstInfo)
	{
		_ASSERTE(!"DoReadLogFile");
		return -1;
	}

	CWriteLogMng LogMgr;
	if ( !(LogMgr.Open(pstInfo->szFullPath)) )
	{
		DWORD dwErr = ::GetLastError();

		if(ERROR_FILE_NOT_FOUND == dwErr) // ���� ����.
		{
			pstInfo->eLogState = LOGSTATE_NOT_EXIST_FILE;
		}
		else if(ERROR_LOGON_FAILURE == dwErr ||
				ERROR_ACCESS_DENIED == dwErr || 
				ERROR_BAD_NET_NAME == dwErr ||
				ERROR_BAD_NETPATH == dwErr ||
				ERROR_NETNAME_DELETED == dwErr ||
				ERROR_CONNECTION_INVALID == dwErr ) // �α׿� ����
		{
			pstInfo->eLogState = LOGSTATE_NETWORK_LOGON_FAIL;
		}
		else
		{
			TCHAR szMSG[100] = {0,};

			StringCchPrintf(szMSG, sizeof(szMSG), _T("NOT DEFINE ERROR STRING : %s , CODE : %d"), pstInfo->szFullPath, dwErr);
			LogCollectorLog Log(_T("OPEN_ERROR"), _T("OPEN_ERROR"));
			Log.AddLog(szMSG);

			pstInfo->eLogState = LOGSTATE_UNKNOWN_OPEN_FAIL;
		}

#ifdef _KOREA
		Sleep(3000);
#else
		Sleep(1000);
#endif
		
		return -1;
	}

    LogMgr.SetLogFileOffset((fpos_t)pstInfo->ulOffset);
    ReadLog(LogMgr, pstInfo);
    LogMgr.Close();
	return 0;
}


#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
int SUN_LogReader::ReadLog(CWriteLogMng & LogMgr, SERVERINFO * pServerInfo)
{
    int   nReadCount = 0;	// 100�� ������ ���� read ini ���
    int exp_log_count = 0;
    BaseLogData* pObj = NULL;
    LogWriter* pLogWriter = (LogWriter*)g_SUNLogMain.GetLogWriter();
    BOOL exit_flag = FALSE;
    TCHAR message[MAX_PATH] = {0,};

#ifdef YOUNGMOON_TEST
    DWORD dwtick = GetTickCount();
    DWORD db_write_tick = 0;
    DWORD file_read_tick = 0;
#endif

#ifdef NAMKA_TEST
    ULONGLONG pre_offset = 0;
#endif

    while (!exit_flag)
    {
        if (m_bReadThreadExitFlag)
            break;

#ifdef NAMKA_TEST
        pre_offset = pServerInfo->ulOffset;
#endif

        ++nReadCount;
        if (nReadCount > 100)
        {
            // 100�� ������ ���� read ini ���
            nReadCount = 0;
            SaveLastReadInfo(pServerInfo);
        }
#ifdef YOUNGMOON_TEST
        DWORD file_read = GetTickCount();
#endif
        pObj = LogMgr.Read();
#ifdef YOUNGMOON_TEST
        file_read_tick += GetTickCount() - file_read;
#endif
        if (pObj)
        {

            pServerInfo->ulBeforeBreakOffset = 0;
            pServerInfo->nRepeatCount = 0;
            pServerInfo->eLogState = LOGSTATE_LOADING;
            pServerInfo->ulOffset = LogMgr.GetLogFileOffset();
#ifdef YOUNGMOON_TEST
            DWORD write_tick = GetTickCount();
#endif

#ifdef NAMKA_TEST
            if (stricmp(pObj->GetAccountID(), "xiaopanguan") == 0)
            {
                TCHAR message[MAX_PATH];
                StringCchPrintf(message, sizeof(message), _T("path : %s, offset : %I64d, C :%d, T : %d") ,
                    pServerInfo->szFullPath, pre_offset, pObj->GetCategory(), pObj->GetLogType());
                LogCollectorLog Log(_T("READ_ERROR"), _T("READ_ERROR"));
                Log.AddLog(message);
                WriteLogToDataBase(pServerInfo, pLogWriter, pServerInfo->ulOffset, pObj);
            }
#else
            WriteLogToDataBase(pServerInfo, pLogWriter, pServerInfo->ulOffset, pObj);
#endif

#ifdef YOUNGMOON_TEST
            db_write_tick += GetTickCount() - write_tick;
#endif
            if (pObj->GetCategory() == LOG_EXP)
            {
                if (++exp_log_count > 1000)
                {
                    exp_log_count = 0;

                    TCHAR message[MAX_PATH] = { 0, };
                    StringCchPrintf(message, sizeof(message), \
                        _T("path : %s, offset : %I64d, C :%d, T : %d, TIME : %s") , \
                        pServerInfo->szFullPath, pServerInfo->ulOffset, 
                        pObj->GetCategory(), pObj->GetLogType(), pObj->GetWriteTime());
                    
                    LogCollectorLog Log(_T("EXPLOG"), _T("EXPLOG"));
                    Log.AddLog(message);
                }
            }
        }
        else
        {
            exit_flag = TRUE;
            DWORD dwErr = ::GetLastError();
            switch (dwErr)
            {
                // �α׿� ����
            case ERROR_LOGON_FAILURE:
            case ERROR_ACCESS_DENIED:
            case ERROR_BAD_NET_NAME:
            case ERROR_BAD_NETPATH:
            case ERROR_NETNAME_DELETED:
            case ERROR_CONNECTION_INVALID:
                {
                    pServerInfo->ulOffset = LogMgr.GetLogFileOffset();
                    pServerInfo->eLogState = LOGSTATE_NETWORK_LOGON_FAIL;

                    StringCchPrintf(message, sizeof(message), _T("Read Fail : %s, OFFSET : %I64d , CODE : %d"), pServerInfo->szFullPath, LogMgr.GetLogFileOffset(), dwErr);
                    LogCollectorLog Log(_T("READ_ERROR"), _T("READ_ERROR"));
                    Log.AddLog(message);
                }
                break;

                // IO �۾��� ��ĥ �� ���� �����̱� ������ ���޽ð��� ������ �Ѵ�.
            case ERROR_IO_PENDING:
                {
                    pServerInfo->ulOffset = LogMgr.GetLogFileOffset();
                    pServerInfo->eLogState = LOGSTATE_IO_PENDING;
                }
                break;

            case ERROR_SUCCESS:
                {
                    pServerInfo->ulOffset = LogMgr.GetLogFileOffset();
                    pServerInfo->eLogState = LOGSTATE_REALTIME_SAME_OFFSET;
                    SERVERINFO temp;
                    int find_next = FindNextFilePath(pServerInfo, &temp);
                    if (find_next != 0)
                    {
                        ++(pServerInfo->nRepeatCount);
                    }
                }
                break;

            default:
                {
                    pServerInfo->ulOffset = LogMgr.GetLogFileOffset();
                    pServerInfo->eLogState = LOGSTATE_INVALIED_OFFSET;

                    StringCchPrintf(message, sizeof(message), _T("Read Fail : %s, OFFSET : %I64d , CODE : %d"), pServerInfo->szFullPath, LogMgr.GetLogFileOffset(), dwErr);
                    LogCollectorLog Log(_T("READ_ERROR"), _T("READ_ERROR"));
                    Log.AddLog(message);
                }
                break;
            }

            ::SetLastError(dwErr);
            ::Sleep(100);
        }

        ////////////////////////////////////////////////////////////////////////
        //	CPU ���ϰ� 100%�� ġ�ݱ⿡ ���޽ð��� ��. 2007-08-07 pavonine66
        ::Sleep(1);
        ////////////////////////////////////////////////////////////////////////
    }

#ifdef YOUNGMOON_TEST
    if(GetTickCount() - dwtick > 10000)
    {
        printf("\nSpanTime: %u\n",GetTickCount() - dwtick);
    }    
#endif

    SaveLastReadInfo(pServerInfo);

    return 0;
}

#else
/**	----------------------------------------------------------------------------
	\brief	�о�� �����͸� m_LogDataList�� �߰�

	\param	
	\return	
------------------------------------------------------------------------------*/
int SUN_LogReader::ReadLog(CWriteLogMng & LogMgr, SERVERINFO * pServerInfo)
{	
	int   iServerCode  = pServerInfo->nServerCode;
//	int   iChannelCode = pServerInfo->nChannelID;
	int   nReadCount = 0;	// 100�� ������ ���� read ini ���
	DWORD dwCnt = 0;
	BaseLogData* pObj = NULL;
	BOOL bIncreaseOffset = FALSE;

	LogWriter* pLogWriter = (LogWriter*)g_SUNLogMain.GetLogWriter();

	while (TRUE)
	{
		/*
		while (TRUE)
		{
			g_pGlobalCS->Enter();
			size_t nLog = m_LogDataList.size();
			g_pGlobalCS->Leave();

			if (nLog <= 6000)
			{
				break;
			}
			else
			{
				OutputDebugString(_T("LOGDATA COUNT BETTER THAN 6000"));
			}

			if (m_bReadThreadExitFlag)
			{
				break;
			}
		}
		*/

		if (m_bReadThreadExitFlag)
		{
			break;
		}

		if (nReadCount > 300)
		{
			// 100�� ������ ���� read ini ���
			nReadCount = 0;
			SaveLastReadInfo(pServerInfo);
			/*
#ifdef _KOREA
			Sleep(1000);
#endif
			*/
		}
		nReadCount++;

#ifdef _DEBUG
		ULONGLONG TmpOffset1 = LogMgr.GetLogFileOffset();
#endif //_DEBUG

		pObj = LogMgr.Read();

		if (pObj == NULL)
		{
			/*
#ifdef _DEBUG
			if(bFirst)
			{
				HANDLE Handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
				TCHAR szMSG[100] = {0,};
				StringCchPrintf(szMSG,sizeof(szMSG), _T("\nLAST OFFSET : %I64d => READ NULL POS\n"), ulPreOffet);
				PrintUserColorString(Handle, USER_RED, szMSG);
				bFirst = FALSE;
			}
			
#endif //_DEBUG
			*/
			DWORD dwErr = ::GetLastError();

			if( ERROR_LOGON_FAILURE == dwErr ||
				ERROR_ACCESS_DENIED == dwErr ||
				ERROR_BAD_NET_NAME == dwErr  ||
				ERROR_BAD_NETPATH == dwErr ||
				ERROR_NETNAME_DELETED == dwErr ||
				ERROR_CONNECTION_INVALID == dwErr ) // �α׿� ����
			{
				pServerInfo->eLogState = LOGSTATE_NETWORK_LOGON_FAIL;
				TCHAR szMsg[30] = {0,};
				StringCchPrintf(szMsg, sizeof(szMsg), "File Read ERR : LOGSTATE_NETWORK_LOGON_FAIL");
				OutputDebugString(szMsg);
			}	
			else if(0 == dwErr) //������ ���µ� ������ ��� 1. ���ϳ��� ��, 2. ���θ� ����?
			{
				pServerInfo->eLogState = LOGSTATE_REALTIME_SAME_OFFSET;
				SaveLastReadInfo(pServerInfo);

				if(40 > pServerInfo->nRepeatCount)
				{
					if(pServerInfo->nRepeatCount == 0)
					{
						IsLogComplete(pServerInfo, TRUE);
						ULONGLONG ulTmpOffset = (ULONGLONG)LogMgr.GetLogFileOffset();
			
						pServerInfo->ulBeforeBreakOffset = pServerInfo->ulFileSize < ulTmpOffset ? pServerInfo->ulOffset : ulTmpOffset;
					}
					
					pServerInfo->nRepeatCount++;

#ifdef _KOREA
					Sleep(3000);
#else
					Sleep(2000);
#endif
				}
				else
				{
					pServerInfo->nRepeatCount = 0;

					if(!IsLogComplete(pServerInfo, TRUE))
					{
						//////////////////////////////////////////////////////////////////////////
						// �߱����� ������ �߻�. Ÿ�̹� ������ Ȯ���� ���� �켱 ���� 2009/05/22
#ifndef _CHINA
						ULONGLONG ulTmpOffset = (ULONGLONG)LogMgr.GetLogFileOffset();
						bIncreaseOffset = TRUE;
						pServerInfo->ulOffset = pServerInfo->ulFileSize < ulTmpOffset ? pServerInfo->ulOffset : ulTmpOffset;
						pServerInfo->ulOffset++;
						LogMgr.SetLogFileOffset((fpos_t)pServerInfo->ulOffset);
						pServerInfo->eLogState = LOGSTATE_FIND_OFFET;
#endif			
						//////////////////////////////////////////////////////////////////////////
						Sleep(2000);

						//SaveLastReadInfo(pServerInfo);
					}
					else
					{
						bIncreaseOffset = FALSE;
					}

#ifdef _KOREA
					Sleep(3000);
#endif
				}

				break;
			}
			else if(dwErr == 997) // IO �۾��� ��ĥ �� ���� �����̱� ������ ���޽ð��� ������ �Ѵ�.
			{
				::Sleep(100);
				break;
			}
			else // ����ó������ ���� Err Code�� ������ ���.
			{
				TCHAR szMSG[MAX_PATH] = {0,};
				StringCchPrintf(szMSG, sizeof(szMSG), _T("NOT DEFINE ERROR STRING : %s, OFFSET : %I64d , CODE : %d"), pServerInfo->szFullPath, LogMgr.GetLogFileOffset(), dwErr);

				LogCollectorLog Log(_T("READ_ERROR"), _T("READ_ERROR"));

				Log.AddLog(szMSG);

#ifdef _KOREA
				Sleep(3000);	
#endif
			}
			
			break;
		}
		else
		{
#ifdef _DEBUG
			ulPreOffet = TmpOffset1;
#endif //_DEBUG
/*            // �α� �뷮�� �ʹ� �������� ������. 2008-12-22
			if(bIncreaseOffset || pServerInfo->ulBeforeBreakOffset != 0)
			{
				TCHAR szMSG[MAX_PATH] = {0,};
				StringCchPrintf(szMSG, sizeof(szMSG), _T("OFFSET_INCREASE : %s, BEFORE OFFSET : %I64d , NOW OFFSET : %I64d"), pServerInfo->szFullPath, pServerInfo->ulBeforeBreakOffset, pServerInfo->ulOffset);

				LogCollectorLog Log(_T("OFFSET_INCREASE"), _T("READ_INCREASE"));
				Log.AddLog(szMSG);
			}
*/
			bIncreaseOffset = FALSE;
			pServerInfo->ulBeforeBreakOffset = 0;
			pServerInfo->nRepeatCount = 0;
			pServerInfo->eLogState = LOGSTATE_LOADING;
		}


		pServerInfo->ulOffset = LogMgr.GetLogFileOffset();

        WriteLogToDataBase(pServerInfo, pLogWriter, pServerInfo->ulOffset, pObj);

		////////////////////////////////////////////////////////////////////////
		//	CPU ���ϰ� 100%�� ġ�ݱ⿡ ���޽ð��� ��. 2007-08-07 pavonine66
		::Sleep(10);
		////////////////////////////////////////////////////////////////////////
	}

	return 0;
}

#endif //_NA_0_20110803_DIV_GAMELOG_FILE

template<class T>
void LogDataBuffer(SERVERINFO* server_info, LogWriter* log_writer, BaseLogData* log_data)
{
    T log_temp;
    T* log = static_cast<T*>(&log_temp);
    *log = *(static_cast<T*>(log_data));
    log->m_nChannelCode = server_info->nChannelID;
    log->m_nServerCode  = server_info->nServerCode;
    log_writer->DoWriteLogDB(log);
}

void SUN_LogReader::WriteLogToDataBase(SERVERINFO* server_info, 
                                       LogWriter* log_writer, 
                                       fpos_t offset, 
                                       BaseLogData* log_data)
{
    switch(log_data->GetCategory())
    {
    case LOG_SESSION:
        {
            LogDataBuffer<SessionDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_ITEM:
        {
            LogDataBuffer<ItemDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_EXP:
        {
            LogDataBuffer<ExpDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_ACTION:
        {
            LogDataBuffer<ActionDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_SKILL:
        {
            LogDataBuffer<SkillDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_SNAPSHOT:
        switch(log_data->GetLogType())
        {
        case SNAPSHOT_REGULAR:
            {
                LogDataBuffer<SnapShotDataEx>(server_info, log_writer, log_data);
            }
            break;
        case SNAPSHOT_LOGININVEN:					// �α��� �Ҷ� �κ��丮, â�� �ִ� ������ ����
        case SNAPSHOT_LOGOUTINVEN:					// �α׾ƿ� �Ҷ� �κ��丮, â�� �ִ� ������ ����
        case SNAPSHOT_WAREHOUSE_OPEN_INVEN:			// â�� ���� �κ��� ������ ����
        case SNAPSHOT_WAREHOUSE_CLOSE_INVEN:		// â�� ������ �κ��� ������ ����
            {
                LogDataBuffer<InventoryDataEx>(server_info, log_writer, log_data);
            }
            break;
        case SNAPSHOT_WAREHOUSE_CLOSE:		//â�� ����(â�� �� ��, ���� ���� ������ ����)
        case SNAPSHOT_WAREHOUSE_OEPN:       // 2007-08-07 �߰�. = pavonine66 =
            {
                LogDataBuffer<WareHouseDataEx>(server_info, log_writer, log_data);
            }
            break;
        } // switch
        break;
    case LOG_CHAT:
        {
            LogDataBuffer<ChatDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_EVENT:		
        break;
    case LOG_ADMIN:
        {
            LogDataBuffer<AdminDataEx>(server_info, log_writer, log_data);
        }
        break;
    case LOG_MONEY:
        {
            LogDataBuffer<MoneyDataEx>(server_info, log_writer, log_data);
        }
        break;
#ifdef _GUILD_LOG
    case LOG_GUILD:
        {
            LogDataBuffer<GuildLogDataEx>(server_info, log_writer, log_data);
        }
        break;
#endif //_GUILD

#ifdef _POINT_LOG
    case LOG_ACCUMULATEPOINT:
        {
            LogDataBuffer<AccumulatePointDataEx>(server_info, log_writer, log_data);
        }
        break;
#endif //_POINT_LOG

    default:	
        {
            LogCollectorLog cLog(_T("UNKNOWN_OBJ"), _T("UNKNOWN_OBJ"));
            cLog.AddLog(_T("==================================================\n"));
            cLog.AddLog(_T("Not Define Log\n"));
            cLog.AddLog(_T("Name : %s, OFFSET : %I64d\n, TYPE : %x"), server_info->szFullPath, offset, log_data->GetCategory());
            cLog.AddLog(_T("==================================================\n"));
        }		
    }
}

