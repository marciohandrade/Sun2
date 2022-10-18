#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <srv.h>
#include <time.h>

#include "XP_MD5_FUNC.h"

void printError (SRV_PROC *pSrvProc, CHAR* szErrorMsg);
void printUsage (SRV_PROC *pSrvProc);
void BytesToStr(DWORD cb, void* pv, LPSTR sz);



ULONG __declspec(dllexport) __GetXpVersion()
{
    return ODS_VERSION;
}


void printError (SRV_PROC *pSrvProc, CHAR* szErrorMsg)
{
    srv_sendmsg(pSrvProc, SRV_MSG_ERROR, XP_ERROR, SRV_INFO, 1,
            NULL, 0, (DBUSMALLINT) __LINE__, 
            szErrorMsg,
            SRV_NULLTERM);

    srv_senddone(pSrvProc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
}

void printUsage (SRV_PROC *pSrvProc)
{
    // usage: exec xp_crypt @string, @hash output

    srv_sendmsg(pSrvProc, SRV_MSG_ERROR, XP_PARAM_ERROR, SRV_INFO, 1,
            NULL, 0, (DBUSMALLINT) __LINE__, 
            "usage: exec XP_MD5_CryptKeyVal @string, @index, @keyval OUTPUT\nWhere @keyval is a char of at least 16 bytes.",
            SRV_NULLTERM);
    srv_senddone(pSrvProc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);

}

void BytesToStr(DWORD cb, void* pv, LPSTR sz)
{

	BYTE* pb = (BYTE*) pv;
	int b;
	DWORD i;

	for (i = 0; i<cb; i++) {
		b = (*pb & 0xF0) >> 4;
		*sz++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
		b = *pb & 0x0F;
		*sz++ = (b <= 9) ? b + '0' : (b - 10) + 'A';

		pb++;
	}

	*sz++ = 0;
}
