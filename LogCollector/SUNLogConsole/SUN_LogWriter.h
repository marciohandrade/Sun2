/*------------------------------------------------------------------------------
* SUN_LogWriter.h
*------------------------------------------------------------------------------
* 
*------------------------------------------------------------------------------
* All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _SUN_LOG_WRITER_H_
#define _SUN_LOG_WRITER_H_

#include "LogWriter.h"

class SUN_LogWriter : public LogWriter
{
public:
    SUN_LogWriter();
    virtual ~SUN_LogWriter();
    virtual void DoWriteLogDB(PVOID pLogData);

private:

    BOOL ConvertActionTimeFormat(TCHAR *pszSourceStr, TCHAR* pszTargetStr);
    BOOL CheckContext(TCHAR* szContext);

    BOOL ExecSP_Login(SessionDataEx& stSessionData);
    BOOL ExecSP_Item(ItemDataEx& stItemData);
    BOOL ExecSP_Exp(ExpDataEx& stExpData);
    BOOL ExecSP_Action(ActionDataEx& stActionData);
    BOOL ExecSP_CharSnap(SnapShotDataEx& stCharData);
    BOOL ExecSP_InvenSnap(InventoryDataEx& stInvenData);
    BOOL ExecSP_WarehouseSnap(WareHouseDataEx& stWareData);
    BOOL ExecSP_Skill(SkillDataEx& stSkillData);
    BOOL ExecSP_Chat(ChatDataEx& stChatData);
    BOOL ExecSP_Admin(AdminDataEx& stAdminData);
    BOOL ExecSP_Money(MoneyDataEx& stMoneyData);
#ifdef _GUILD_LOG
    BOOL ExecSP_Guild(GuildLogDataEx& stGuildData);
#endif //_GUILD

#ifdef _POINT_LOG
    BOOL ExecSP_Point(AccumulatePointDataEx& stPointData);
#endif //_POINT_LOG

    BOOL ConvertCategory(TCHAR* szCategory, eLOG_CATEGORY eCategory);
};

#endif //_SUN_LOG_WRITER_H_
