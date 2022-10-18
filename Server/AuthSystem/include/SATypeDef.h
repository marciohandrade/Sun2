/*-----------------------------------------------------------------------------
 * SATypeDef.h
 *-----------------------------------------------------------------------------
 * define data structures
 *-----------------------------------------------------------------------------
 * All rights reserved by Rho Yong Hwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * Revision History:
 * Date					Who					What
 * ----------------		----------------	----------------
 * 13.09.2007			Rho Yong Hwan		birth
**---------------------------------------------------------------------------*/
#ifndef _SA_TYPE_DEF_H_
#define _SA_TYPE_DEF_H_

//
// Shape Authenticate data
//
#define SA_VALUE_SIZE			256
typedef struct _SA_AUTH_DATA
{
	BYTE	*ImageData;
	DWORD	ImageSize;
	TCHAR	Value[SA_VALUE_SIZE];		// 사용자 입력과 비교할 값 (문자열)
} SA_AUTH_DATA, *PSA_AUTH_DATA;

#endif//_SA_TYPE_DEF_H_