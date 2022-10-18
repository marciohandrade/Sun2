/*------------------------------------------------------------------------------
 * SUN_DBHandler.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _SUN_DB_HANDLER_H_
#define _SUN_DB_HANDLER_H_

#include "DBHandler.h"

class SUN_DBHandler : public DBHandler
{
public:
	SUN_DBHandler(int max, TCHAR * dsn, TCHAR * id, TCHAR * passwd);
	virtual ~SUN_DBHandler();

	// �α��� ������ ������ ���� write�� ���� �����ؾ��Ѵ�..
	// test function�� �̿��ؼ� ���� �ؾ���..
	//BOOL WriteBlob(TCHAR* szSQL); //  test function
	BOOL WriteInvenSnapShotBlob(TCHAR* szSQL, InventoryDataEx& stData);
	BOOL WriteItemBlob(TCHAR* szSQL, ItemDataEx& stData);
	BOOL WriteSnapShotBlob(TCHAR* szSQL, SnapShotDataEx& stData);
	BOOL WriteWarehouseBlob(TCHAR* szSQL, WareHouseDataEx&stData);
};

#endif //_SUN_DB_HANDLER_H_
