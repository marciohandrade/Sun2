/*******************************************************************************
*
* FILE:         seedx.h
*
* DESCRIPTION:  header file for seedx.c
*
*******************************************************************************/

#ifndef SEED_H
#define SEED_H

/********************** Include files ************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace Crypt
{

	/******************* Constant Definitions *********************/

	#define NoRounds         16
	#define NoRoundKeys      (NoRounds*2)
	#define SeedBlockSize    16    /* in bytes */
	#define SeedBlockLen     128   /* in bits */


	/********************** Common Macros ************************/

	#if defined(_MSC_VER)
		#define ROTL(x, n)     (_lrotl((x), (n)))
		#define ROTR(x, n)     (_lrotr((x), (n)))
	#else
		#define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))
		#define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))
	#endif


	/**************** Function Prototype Declarations **************/

	#ifndef PROTOTYPES
	#define PROTOTYPES  1
	#endif

	#if PROTOTYPES
	#define PROTO_LIST(list)    list
	#else
	#define PROTO_LIST(list)    ()
	#endif

	void SeedEncrypt 
		PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
	void SeedDecrypt
		PROTO_LIST((BYTE *pbData, DWORD *pdwRoundKey));
	void SeedEncRoundKey
		PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));
	void SeedDecRoundKey
		PROTO_LIST((DWORD *pdwRoundKey, BYTE *pbUserKey));

	/******************************************************************/
}

#endif