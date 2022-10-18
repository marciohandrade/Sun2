/*------------------------------------------------------------------------------
 * SUN_LogReader.cpp
 *------------------------------------------------------------------------------
 * LogWriter 클래스를 상속받아 만든 파르페 전용 로그 리더 클래스이다.
 *
 * 파르페 로그의 데이터 변경 등의 수정 및 추가는
 * 이 클래스를 수정하여 사용한다.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "LogCollectorLog.h"
#include "LogMain.h"
#include "SUN_LogWriter.h"
#include "SUN_DBHandler.h"

extern DBHandler			* g_pDBHandler;

/**	----------------------------------------------------------------------------
\brief	

\param	
\return	
------------------------------------------------------------------------------*/
SUN_LogWriter::SUN_LogWriter()
{
}

/**	----------------------------------------------------------------------------
\brief	

\param	
\return	
------------------------------------------------------------------------------*/
SUN_LogWriter::~SUN_LogWriter()
{
}

/**	----------------------------------------------------------------------------
\brief	

\param	
\return	
------------------------------------------------------------------------------*/
void SUN_LogWriter::DoWriteLogDB(LPVOID pData)
{
    if (NULL == pData)
    {
        _ASSERTE(!"DoWriteLogDB");
    }

    BOOL bSuccess = TRUE;
    BaseLogData * pLogData = (BaseLogData *)pData;

    switch(pLogData->GetCategory())
    {
    case LOG_SESSION:		//접속로그
        {
            SessionDataEx* pSession = static_cast<SessionDataEx*>(pData);
            bSuccess = ExecSP_Login(*pSession);
            break;
        }
    case LOG_ITEM:		//아이템로그
        {
            ItemDataEx* pItem = static_cast<ItemDataEx*>(pData);
            bSuccess = ExecSP_Item(*pItem);
            break;
        }
    case LOG_EXP:		//경험치 로그
        {
            ExpDataEx* pExp = static_cast<ExpDataEx*>(pData);
            bSuccess = ExecSP_Exp(*pExp);
            break;
        }
    case LOG_ACTION:			//액션로그
        {
            ActionDataEx* pAction = static_cast<ActionDataEx*>(pData);
            bSuccess = ExecSP_Action(*pAction);
            break;
        }
    case LOG_SNAPSHOT:		//스냅샷로그
        {
            // 많이 바뀌어야 겠구나...
            // stLogHeader.m_nLogType 에 따라서

            switch(pLogData->GetLogType())
            {
            case SNAPSHOT_REGULAR:
                {
                    SnapShotDataEx* pCharSnap = static_cast<SnapShotDataEx*>(pData);
                    bSuccess = ExecSP_CharSnap(*pCharSnap);
                }
                break;
            case SNAPSHOT_LOGININVEN:
            case SNAPSHOT_LOGOUTINVEN:					//인벤토리만 저장
            case SNAPSHOT_WAREHOUSE_OPEN_INVEN:			// 창고를 열때 인벤의 아이템 정보
            case SNAPSHOT_WAREHOUSE_CLOSE_INVEN:		// 창고를 닫을때 인벤의 아이템 정보
                {
                    InventoryDataEx* pInven = static_cast<InventoryDataEx*>(pData);
                    bSuccess= ExecSP_InvenSnap(*pInven);	
                }
                break;
            case SNAPSHOT_WAREHOUSE_OEPN:
            case SNAPSHOT_WAREHOUSE_CLOSE:		//창고만 저장
                {
                    WareHouseDataEx* pInven = static_cast<WareHouseDataEx*>(pData);
                    bSuccess = ExecSP_WarehouseSnap(*pInven);
                }
            }

        }
        break;

    case LOG_SKILL:		//스킬로그
        {
            SkillDataEx* pSkill = static_cast<SkillDataEx*>(pData);
            bSuccess = ExecSP_Skill(*pSkill);
        }
        break;

    case LOG_CHAT:		//채팅 로그
        {
            ChatDataEx* pChat = static_cast<ChatDataEx*>(pData);
            bSuccess = ExecSP_Chat(*pChat);
        }
        break;

    case LOG_ADMIN:		// GM 로그
        {

            AdminDataEx* pAdmin= static_cast<AdminDataEx*>(pData);
            bSuccess = ExecSP_Admin(*pAdmin);
        }
        break;

    case LOG_MONEY:			// Money 로그
        {
            MoneyDataEx* pMoney = static_cast<MoneyDataEx*>(pData);
            bSuccess = ExecSP_Money(*pMoney);
        }
        break;
#ifdef _GUILD_LOG
    case LOG_GUILD:
        {
            GuildLogDataEx* pGuild = static_cast<GuildLogDataEx*>(pData);
            //////////////////////////////////////////////////////////////////////////
            // 이곳에서 DB에 길드로그 INSERT할 수 있는 함수 호출!
            bSuccess = ExecSP_Guild(*pGuild);
            //////////////////////////////////////////////////////////////////////////
        }
        break;
#endif //_GUILD

#ifdef _POINT_LOG
    case LOG_ACCUMULATEPOINT:
        {
            AccumulatePointDataEx* pPoint = static_cast<AccumulatePointDataEx*>(pData);
            //////////////////////////////////////////////////////////////////////////
            // 이곳에서 DB에 길드로그 INSERT할 수 있는 함수 호출!
            bSuccess = ExecSP_Point(*pPoint);
            //////////////////////////////////////////////////////////////////////////
        }
        break;
#endif //_POINT_LOG

    default:
        {
            LogCollectorLog cLog(_T("UNKNOWN_LOG"), _T("UNKNOWN_LOG"));
            cLog.AddLog(_T("Category Code : 0x%x"), pLogData->GetCategory());
        }	

    }

    if(!bSuccess)
    {
        LogCollectorLog cLog(_T("DATE_CONVERT_ERROR"), _T("DATE_CONVERT_ERROR"));
        TCHAR szCategory[MAX_PATH];
        ZeroMemory(szCategory, sizeof(szCategory));

        ConvertCategory(szCategory, pLogData->GetCategory());

        cLog.AddLog(_T("Category Code : %s"), szCategory);

    } 
}

/**	----------------------------------------------------------------------------
\brief	날짜를 형식에 맞게 CONVERT(ex> 2008-02-11 12:35:15).

\param	
\return	
------------------------------------------------------------------------------*/
BOOL SUN_LogWriter::ConvertActionTimeFormat(TCHAR *pszSourceStr, TCHAR* pszTargetStr)
{
    //이 함수는 060404100003111 이렇게 들어오는 스트링을
    // 2006-04-04 10:00:03.111 이런식으로 바꿔주는 역할을 한다.

    if(_tcslen(pszSourceStr) == 0)
        return FALSE;

    TCHAR szYear[3];
    TCHAR szMonth[3];
    TCHAR szDay[3];
    TCHAR szHour[3];
    TCHAR szMin[3];
    TCHAR szSec[3];
    TCHAR szMilliSec[4];
    memcpy(szYear , &pszSourceStr[0],2 );
    memcpy(szMonth , &pszSourceStr[2],2 );
    memcpy(szDay , &pszSourceStr[4],2 );
    memcpy(szHour , &pszSourceStr[6],2 );
    memcpy(szMin , &pszSourceStr[8],2 );
    memcpy(szSec , &pszSourceStr[10],2 );
    memcpy(szMilliSec , &pszSourceStr[12],3 );
    szYear[2] = _T('\0');
    szMonth[2] = _T('\0');
    szDay[2] = _T('\0');
    szHour[2] = _T('\0');
    szMin[2] = _T('\0');
    szSec[2] = _T('\0');
    szMilliSec[3] = _T('\0');


    StringCchPrintf(pszTargetStr, 100, _T("20%s-%s-%s %s:%s:%s.%03s"), szYear, szMonth, szDay, szHour, szMin, szSec, szMilliSec);

    return TRUE;
}


/**	----------------------------------------------------------------------------
\brief	각 data type에 따라 저장 프로시저가 호출 된다.

\param	
\return	
------------------------------------------------------------------------------*/
BOOL SUN_LogWriter::ExecSP_Login(SessionDataEx& stSessionData)
{
    BOOL ret = FALSE;
    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szLogTime[100];
    ZeroMemory(szLogTime, sizeof(szLogTime));

    TCHAR szLoginTimeAction[100];
    ZeroMemory(szLoginTimeAction, sizeof(szLoginTimeAction));

    TCHAR szLogoutTimeAction[100];
    ZeroMemory(szLogoutTimeAction, sizeof(szLogoutTimeAction));


    if(!ConvertActionTimeFormat(stSessionData.GetWriteTime(), szLogTime))
        return FALSE;

    // 로그 아웃 시점시 대부분의 값들이 NULL 값 일 경우가 생긴다.
    ConvertActionTimeFormat(stSessionData.GetLoginTime(), szLoginTimeAction);

    if(!ConvertActionTimeFormat(stSessionData.GetWriteTime(), szLogoutTimeAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stSessionData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_ConnectInsert N'20%s', N'%s',N'%s', N'%s', %d,N'%s', %u, N'%s',N'%s', %d, %d, %d, N'%s'"),
        szTableName,
        stSessionData.GetAccountID(), 
        stSessionData.GetCharName(), 
        szLogTime, 
        stSessionData.GetLogType(), 
        stSessionData.GetUserIP(), 
        stSessionData.GetUsedTime(), 
        szLoginTimeAction, 
        szLogoutTimeAction,
        stSessionData.GetLogoutType(), 
        stSessionData.m_nServerCode,
        stSessionData.m_nChannelCode,
        stSessionData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP); 

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Item(ItemDataEx& stItemData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stItemData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stItemData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');


    StringCchPrintf(szSP, sizeof(szSP),_T("exec S_ItemInsert N'20%s', N'%s', N'%s', N'%s',%d, %I64d, %u, %d, %I64d, %u, %d, %d, %d, N'%s', N'%s', ?, ?, %d, %d, N'%s', N'%s', %d"),
        szTableName,
        stItemData.GetAccountID(), 
        stItemData.GetCharName(), 
        szAction, 
        stItemData.GetLogType(), 
        stItemData.GetItemSerial(),
        stItemData.GetItemCode(),
        stItemData.GetItemNum(),
        stItemData.GetMoney(), 
        stItemData.GetMapCode(), 
        stItemData.GetMapX(), 
        stItemData.GetMapY(), 
        stItemData.GetMapZ(), 
        stItemData.GetToAccountID(), 
        stItemData.GetToCharName(), 
        stItemData.m_nServerCode,
        stItemData.m_nChannelCode,
        stItemData.GetUserIP(), 
        stItemData.GetAddInfo(),
        stItemData.GetItemStreamVersion()
        );

    SUN_DBHandler* pSUN_DBHandler = (SUN_DBHandler*)m_pDBHandler;

    ret = pSUN_DBHandler->WriteItemBlob(szSP, stItemData); 

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Exp(ExpDataEx& stExpData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stExpData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stExpData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_ExpInsert N'20%s', N'%s', N'%s', N'%s',%d, %ld, %ld, %I64d, %I64d, %d, %d, N'%s', N'%s'"),
        szTableName,
        stExpData.GetAccountID(), 
        stExpData.GetCharName(), 
        szAction, 
        stExpData.GetLogType(), 
        stExpData.GetNewLevel(), 
        stExpData.GetOldLevel(), 
        stExpData.GetNewExp(), 
        stExpData.GetOldExp(),
        stExpData.m_nServerCode, 
        stExpData.m_nChannelCode,
        stExpData.GetUserIP(), 
        stExpData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP); 

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Action(ActionDataEx& stActionData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));

    if(!ConvertActionTimeFormat(stActionData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stActionData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_ActionInsert N'20%s', N'%s', N'%s', N'%s',%ld, %ld, %ld, %ld, %ld, %d, %d, N'%s', N'%s'"),
        szTableName,
        stActionData.GetAccountID(),
        stActionData.GetCharName(), //stActionData.GetCharName()
        szAction, 
        stActionData.GetLogType(), 
        stActionData.GetMapCode(), 
        stActionData.GetMapX(),
        stActionData.GetMapY(), 
        stActionData.GetMapZ(),
        stActionData.m_nServerCode, 
        stActionData.m_nChannelCode,
        stActionData.GetUserIP(), 
        stActionData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_CharSnap(SnapShotDataEx& stCharData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stCharData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stCharData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

#if NATION_BUILD_VERSION >= 901 && NATION_BUILD_VERSION < 903
    //#pragma message("compile is NOT JPN\n")
    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_CharInsert N'20%s', N'%s', N'%s', N'%s', N'%s', '%d', ?, ?, ?, ?, ?, ?, ?, ?, ?, %d, %d, N'%s', %d, %d, %d"),
#elif NATION_BUILD_VERSION >= 903
    //#pragma message("compile is 0903\n")
    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_CharInsert N'20%s', N'%s', N'%s', N'%s', N'%s', '%d', ?, ?, ?, ?, ?, ?, ?, ?, ?, %d, %d, N'%s', %d, %d, %d, %d"),
#else
    //#pragma message("compile is JPN\n")
    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_CharInsert N'20%s', N'%s', N'%s', N'%s', N'%s', '%d', ?, ?, ?, ?, ?, ?, ?, ?, ?, %d, %d, N'%s', %d, %d"),
#endif
        szTableName,
        stCharData.GetAccountID(), 
        stCharData.GetCharName(), 
        stCharData.GetUserIP(), 
        szAction,
        stCharData.GetLogType(),
        stCharData.m_nServerCode, 
        stCharData.m_nChannelCode,
        stCharData.GetAddInfo(), 
        stCharData.GetItemBinaryVersion(),
        //#if defined(_0816) || defined(_0817)  || defined(_0901) || defined(_0902) || defined(_0903)
        0
        //#else
        //		stCharData.GetBasePlayerInfoBinaryVersion()
        //#endif //_0816		
#if NATION_BUILD_VERSION >= 901
        , stCharData.GetACStreamVersion()
#if NATION_BUILD_VERSION >= 903
#pragma message("compile is Batter than 0903\n") 
        , stCharData.GetQuestStreamVersion()
#endif
#endif
        );
    /*
    OutputDebugString(szSP);
    TCHAR szTmp[100];
    StringCchPrintf(szTmp, sizeof(szTmp), _T("GetACStreamVersion => %d"), stCharData.GetACStreamVersion());
    OutputDebugString(szTmp);
    */

    SUN_DBHandler* pSUN_DBHandler = (SUN_DBHandler*)m_pDBHandler;

    ret = pSUN_DBHandler->WriteSnapShotBlob(szSP, stCharData);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;


}

BOOL SUN_LogWriter::ExecSP_InvenSnap(InventoryDataEx& stInvenData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stInvenData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stInvenData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_InvenInsert N'20%s', N'%s', N'%s', N'%s', N'%s', %d, %I64d, ?, ?, %d, %d, N'%s', %d"), 
        szTableName,
        stInvenData.GetAccountID(), 
        stInvenData.GetCharName(), 
        stInvenData.GetUserIP(), 
        szAction, 
        stInvenData.GetLogType(),
        stInvenData.GetInventoryMoney(),
        stInvenData.m_nServerCode, 
        stInvenData.m_nChannelCode,
        stInvenData.GetAddInfo(),
        stInvenData.GetItemBinaryVersion()
        );

    //OutputDebugString(szSP);

    SUN_DBHandler* pSUN_DBHandler = (SUN_DBHandler*)m_pDBHandler;

    ret = pSUN_DBHandler->WriteInvenSnapShotBlob(szSP, stInvenData);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_WarehouseSnap(WareHouseDataEx& stWareData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stWareData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stWareData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_WarehouseInsert N'20%s', N'%s', N'%s', N'%s', N'%s', %d, %I64d, ?, %d, %d, N'%s', %d"), 
        szTableName,
        stWareData.GetAccountID(), 
        stWareData.GetCharName(), 
        stWareData.GetUserIP(), 
        szAction, 
        stWareData.GetLogType(),
        stWareData.GetMoney(),
        stWareData.m_nServerCode, 
        stWareData.m_nChannelCode,
        stWareData.GetAddInfo(),
        stWareData.GetItemBinaryVersion()
        );

    //OutputDebugString(szSP);

    SUN_DBHandler* pSUN_DBHandler = (SUN_DBHandler*)m_pDBHandler;

    ret = pSUN_DBHandler->WriteWarehouseBlob(szSP, stWareData);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;

}

BOOL SUN_LogWriter::ExecSP_Skill(SkillDataEx& stSkillData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stSkillData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stSkillData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');


    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_SkillInsert N'20%s', N'%s', N'%s', N'%s', N'%s', %d, %d, %d, %d, %d, N'%s'"), 
        szTableName,
        stSkillData.GetAccountID(), 
        stSkillData.GetCharName(), 
        stSkillData.GetUserIP(), 
        szAction,
        stSkillData.GetLogType(), 
        stSkillData.GetSkillLevel(),
        stSkillData.GetSkillUseCnt(), 
        stSkillData.m_nServerCode, 
        stSkillData.m_nChannelCode,
        stSkillData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Chat(ChatDataEx& stChatData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szContext[256]; // 바꾸기
    ZeroMemory(szContext, sizeof(szContext));

    StringCchCopy(szContext, sizeof(szContext), stChatData.GetContext() );

    CheckContext(szContext);

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stChatData.GetWriteTime(), szAction) )
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stChatData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    //CheckContext(szMsg);
#ifdef _CHAT_USERID
    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_ChatInsert N'20%s', %d, %d, N'%s', %d, N'%s', N'%s', %d, N'%s ', %d, %d, %d, N'%s', N'%s', N'%s'"),
        szTableName,
        stChatData.m_nChannelCode, 
        stChatData.GetUserGuid(),
        stChatData.GetCharName(),
        stChatData.GetListenUserGuid(),
        stChatData.GetListenCharName(), 
        szAction,
        stChatData.GetLogType(), 
        szContext, 
        stChatData.GetMapCode(), 
        stChatData.GetRoomNo(),
        stChatData.m_nServerCode, 
        stChatData.GetUserIP(), 
        stChatData.GetAddInfo(),
        stChatData.GetAccountID() //0817부터 추가. 2008-12-22
        );
#else
    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_ChatInsert N'20%s', %d, %d, N'%s', %d, N'%s', N'%s', %d, N'%s ', %d, %d, %d, N'%s', N'%s'"),
        szTableName,
        stChatData.m_nChannelCode, 
        stChatData.GetUserGuid(),
        stChatData.GetCharName(),
        stChatData.GetListenUserGuid(),
        stChatData.GetListenCharName(), 
        szAction,
        stChatData.GetLogType(), 
        szContext, 
        stChatData.GetMapCode(), 
        stChatData.GetRoomNo(),
        stChatData.m_nServerCode, 
        stChatData.GetUserIP(), 
        stChatData.GetAddInfo()
        );
#endif //_CHAT_USERID


    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Admin(AdminDataEx& stAdminData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stAdminData.GetWriteTime(), szAction) )
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stAdminData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_AdminInsert N'20%s', N'%s', N'%s', N'%s',%ld, %ld, %ld, %ld, %ld, %ld, N'%s', %d, %d, N'%s', N'%s'"),
        szTableName,
        stAdminData.GetAccountID(),
        stAdminData.GetCharName(), 
        szAction, 
        stAdminData.GetLogType(), 
        stAdminData.GetMapCode(), 
        stAdminData.GetMapX(), 
        stAdminData.GetMapY(), 
        stAdminData.GetMapZ(),
        0,
        stAdminData.GetToCharName(),
        stAdminData.m_nServerCode, 
        stAdminData.m_nChannelCode,
        stAdminData.GetUserIP(), 
        stAdminData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}

BOOL SUN_LogWriter::ExecSP_Money(MoneyDataEx& stMoneyData)
{
    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stMoneyData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stMoneyData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');


    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_MoneyInsert N'20%s', N'%s', N'%s ', N'%s',%ld, %ld, %ld, %ld, %ld, N'%s', N'%s ', %I64d, %I64d, %d, %d, %d, N'%s', N'%s'"),
        szTableName,
        stMoneyData.GetAccountID(),
        stMoneyData.GetCharName(), 
        szAction, 
        stMoneyData.GetLogType(), 
        stMoneyData.GetMapCode(), 
        stMoneyData.GetMapX(), 
        stMoneyData.GetMapY(), 
        stMoneyData.GetMapZ(),
        stMoneyData.GetToAccoundID(),
        stMoneyData.GetToCharName(),
        stMoneyData.GetMoney(),
        stMoneyData.GetToMoney(),
        stMoneyData.GetRoomKey(),
        stMoneyData.m_nServerCode, 
        stMoneyData.m_nChannelCode, 
        stMoneyData.GetUserIP(), 
        stMoneyData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;

}

#ifdef _GUILD_LOG
BOOL SUN_LogWriter::ExecSP_Guild(GuildLogDataEx& stGuildData)
{
    BOOL ret = FALSE;
    BOOL bDefineCode = TRUE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stGuildData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stGuildData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');

    switch(stGuildData.GetLogType())
    {
    case GLD_CRTE :
    case GLD_DTRY_WAIT :
    case GLD_DTRY_CACL :
    case GLD_DTRY_CPLT :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMasterCharName(),
            0,
            0
            );
        break;
    case GLD_CORP_CRTE :
    case GLD_CORP_DTRY :
    case GLD_MEM_JOIN :
    case GLD_MEM_KICK :
    case GLD_MEM_OUT :
    case GLD_CAMP_CRT :
    case GLD_CAMP_DTRY :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMemberCharName(),
            stGuildData.GetCorpsNum(),
            0
            );
        break;
    case GLD_REL_SET :
    case GLD_REL_CACL :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'%s', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMasterCharName(),
            stGuildData.GetRelation(),
            stGuildData.GetTargetGuildGuid(),
            stGuildData.GetTargetGuildName()
            );
        break;
    case GULD_LEV_UP :
        TCHAR szTotalUP[50];
        TCHAR szTotalGP[50];
        ZeroMemory(szTotalUP, sizeof(szTotalUP));
        ZeroMemory(szTotalGP, sizeof(szTotalGP));

        StringCchPrintf(szTotalUP, sizeof(szTotalUP), _T("%d"), stGuildData.GetTotalUP());
        StringCchPrintf(szTotalGP, sizeof(szTotalGP), _T("%d"), stGuildData.GetTotalGP());

        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'%s', N'%s'"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMasterCharName(),
            stGuildData.GetGuildGrade(),
            stGuildData.GetPrevGuildGrade(),
            szTotalUP,
            szTotalGP
            );
        break;
    case GLD_DUTY_CHG :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMemberCharName(),
            stGuildData.GetMemberDuty(),
            0
            );
        break;
    case GLD_RIGT_DLGT :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMemberCharName(),
            stGuildData.GetMemberRight(),
            0
            );
        break;
    case GLD_UP_GIVE :
        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'', N''"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMemberCharName(),
            stGuildData.GetDeltaUP(),
            stGuildData.GetTotalUP()
            );
        break;
    default :
        {
#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
        TCHAR column3[20];
        TCHAR column4[20];
        ZeroMemory(column3, sizeof(column3));
        ZeroMemory(column4, sizeof(column4));

        StringCchPrintf(column3, sizeof(column3), _T("%s"), stGuildData.GetDefaultColumn3());
        StringCchPrintf(column4, sizeof(column4), _T("%s"), stGuildData.GetDefaultColumn4());

        StringCchPrintf(szSP, sizeof(szSP),
            _T("exec S_GuildInsert N'20%s', N'%s', %d, N'%s', %d, N'%s', %d, %d, N'%s', N'%s'"),
            szTableName,
            szAction,
            stGuildData.GetLogType(),
            stGuildData.GetGuildName(),
            stGuildData.GetGuildGuid(),
            stGuildData.GetMasterCharName(),
            stGuildData.GetDefaultColumn1(),
            stGuildData.GetDefaultColumn2(),
            column3,
            column4
            );
#else
			bDefineCode = FALSE;
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
        break;
        }
    }

    if(bDefineCode)
    {
        ret = m_pDBHandler->DoSQL(szSP);
    }
    else
    {
        StringCchPrintf(szSP, sizeof(szSP), _T("GUILD CODE : %s\n"));
        LogCollectorLog cLog(_T("UNDEFINE_CODE"), _T("UNDEFINE_CODE"));
        cLog.AddLog(szSP);
    }


    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;
}
#endif //_GUILD

#ifdef _POINT_LOG
BOOL SUN_LogWriter::ExecSP_Point(AccumulatePointDataEx& stPointData)
{

    BOOL ret = FALSE;

    TCHAR szSP[1024];
    ZeroMemory(szSP, sizeof(szSP));

    TCHAR szAction[100];
    ZeroMemory(szAction, sizeof(szAction));
    if(!ConvertActionTimeFormat(stPointData.GetWriteTime(), szAction))
        return FALSE;

    TCHAR szTableName[100];
    ZeroMemory(szTableName, sizeof(szTableName));

    memcpy(szTableName, stPointData.GetWriteTime(), 6);
    szTableName[6]=_T('\0');


    StringCchPrintf(szSP, sizeof(szSP), _T("exec S_AccumulatePointInsert N'20%s', N'%s', %d, N'%s', N'%s', %d, %d, %d, %d, %d, %d, %d, N'%s'"),
        szTableName,
        szAction,
        stPointData.GetLogType(), 
        stPointData.GetAccountID(),
        stPointData.GetCharName(),  
        stPointData.m_nServerCode, 
        stPointData.m_nChannelCode, 
        stPointData.GetAccumulateYear(),
        stPointData.GetAccumulateTime(),
        stPointData.GetOldAccumulatePoint(),
        stPointData.GetNewAccumulatePoint(),
        stPointData.GetRewardItemCode(),
        stPointData.GetAddInfo()
        );

    ret = m_pDBHandler->DoSQL(szSP);

    if ( !ret )
    {
        LogCollectorLog cLog(_T("FAIL_LOG"), _T("FAIL_LOG"));
        cLog.AddSQL(szSP);
        //cLog.AddWzl((void*)&stCharLoginData, sizeof(BINLOG__CHARACTER_LOGIN));
    }

    return ret;

}
#endif //_POINT_LOG

BOOL SUN_LogWriter::CheckContext(TCHAR* szContext)
{
    TCHAR szBuffer[1024*10];
    ZeroMemory(szBuffer, sizeof(szBuffer));
    //strcpy(szBuffer, szContext);


    size_t len = 0;
    StringCchLength(szContext, 256, &len); //바꾸기

    for(int i = 0; i< (int)len; i++)
    {
        TCHAR cTmp = szContext[i];
        if(cTmp == _T('\''))
        {
            //szContext[i] = '\"';
            cTmp = _T('\"');
        }
        szBuffer[i] = cTmp;
    }

    //szBuffer[len] = NULL;

    //StringCbCopy(szContext, len, szBuffer);
    StringCchCopy(szContext, 256, szBuffer); //바꾸기

    return TRUE;
}

BOOL SUN_LogWriter::ConvertCategory(TCHAR* szCategory, eLOG_CATEGORY eCategory)
{
    if(szCategory == NULL)
    {
        return FALSE;
    }

    BOOL bRet = TRUE;
    TCHAR szTmpCategory[MAX_PATH];

    switch(eCategory)
    {
    case LOG_SESSION :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_SESSION"));
        break;
    case LOG_ITEM :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_ITEM"));
        break;
    case LOG_EXP :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_EXP"));
        break;
    case LOG_ACTION :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_ACTION"));
        break;
    case LOG_SKILL :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_SKILL"));
        break;
    case LOG_SNAPSHOT :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_SNAPSHOT"));
        break;
    case LOG_CHAT :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_CHAT"));
        break;
    case LOG_EVENT :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_EVENT"));
        break;
    case LOG_ADMIN :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_ADMIN"));
        break;
    case LOG_MONEY : 
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_MONEY"));
        break;
    case LOG_GUILD : 
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_GUILD"));
        break;
    default :
        StringCchCopy(szTmpCategory, sizeof(szTmpCategory), _T("LOG_NOT_DEFINE"));
        break;
    }


    StringCchCopy(szCategory, MAX_PATH, szTmpCategory);

    return bRet;

}

