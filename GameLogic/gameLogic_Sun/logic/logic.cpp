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
		//문상현 2009-10-30 문자열 찾는 순서 변경 
		//기존 stringtable.txt => stringtableforname.txt
		//변경 stringtableforname.txt => stringtable.txt
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



