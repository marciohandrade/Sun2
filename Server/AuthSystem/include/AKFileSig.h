// ***************************************************************
//  AKFileSig   version:  1.0   ¡¤  date: 08/01/2006
//  -------------------------------------------------------------
//  
//  -------------------------------------------------------------
//  
//  Copyright (C) 2006, Webzen INC.
//	All Rights Reserved
//
//  Author : Ji Man Ham, WEBZEN Research & Development Dept.
//           faethm@hotmail.com , jmham@webzen.com
// ***************************************************************
//  
// ***************************************************************

#pragma once

#define ret_true 1
#define ret_false 0

#define SIG_LENGTH 40

#define SIG_AUTH_DISABLE 0
#define SIG_AUTH_TYPE_MD5 1
#define SIG_AUTH_TYPE_SHA1 2

typedef struct _SIGNATURE_BODY
{
	DWORD		sId;						// Signature ID
	CHAR	FileSignature[SIG_LENGTH];
} SIGNATURE_BODY, *PSIGNATURE_BODY;

DWORD GetAKFileSigSize( DWORD dwSigCount);
INT CreateAKFileSig( PVOID pOutSig, 
					DWORD dwSigCount, 
					DWORD dwSigAuthType, 
					PSIGNATURE_BODY pInSig );