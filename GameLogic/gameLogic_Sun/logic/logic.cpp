// logic.cpp

#include "stdafx.h"
#include <Struct.h>
#include <SolarFileReader.h>
#include "CSunOnline.h"

#include "logic.h"


void GetStringByCode(DWORD dwCode, char* dest, int destLen)
{
	char* psz = NULL;

	do 
	{
		//������ 2009-10-30 ���ڿ� ã�� ���� ���� 
		//���� stringtable.txt => stringtableforname.txt
		//���� stringtableforname.txt => stringtable.txt
		psz = STRTBL_PARSER2->GetStringByCode(dwCode);
		if(psz)
			break;

		psz = STRTBL_PARSER1->GetStringByCode(dwCode);
		if(psz)
			break;

		psz = STRTBL_PARSER3->GetStringByCode(dwCode);
		if(psz)
			break;

		psz = "NULL";

	} while(FALSE);

	strncpy(dest, psz, destLen);
}



