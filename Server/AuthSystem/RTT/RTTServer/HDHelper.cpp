/*-----------------------------------------------------------------------------
 * HDHelper.cpp
 *-----------------------------------------------------------------------------
 * HookDefender 관련 상수 / 코드 --> 문자열 변환 모듈
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2006, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2006, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#include "stdafx.h"

#include "EVENT_TYPE_SPEC.h"
#include "HDHelper.h"

#include "CStream.h"

#define ATR_STREAM_LEN		sizeof(int)

// do nothing 
//		ArmorKit 내부 라이브러러 함수라서.. 걍 무시하세요
//
#pragma warning(disable: 4100)
__inline LPCTSTR SetErrorString(BOOL IncludeGetLastError, TCHAR *fmt,...)
{
	return NULL;
}
#pragma warning(default: 4100)





LPCTSTR	CHDHelper::HDErrorCodeToString(INT iErrorCode)
{
	switch (iErrorCode)
	{
	case HDERR_NONE				:
		return S_HDERR_NONE;
	case HDERR_GENERIC			:
		return S_HDERR_GENERIC;
	case HDERR_EAT_RESTORE		:
		return S_HDERR_EAT_RESTORE;
	case HDERR_IAT_RESTORE		:
		return S_HDERR_IAT_RESTORE;
	case HDERR_INVALID_PE		:
		return S_HDERR_INVALID_PE;
	case HDERR_NO_API_FOUNDED	:
		return S_HDERR_NO_API_FOUNDED;
	case HDERR_INVALID_ADDRESS	:
		return S_HDERR_INVALID_ADDRESS;
	default:
		{
			return "Unknown error code";
		}
	}
}


/**	---------------------------------------------------------------------------
	\brief	hook type ---> humal readable string 

	\param	
	\return			
	\code
	\endcode		
-----------------------------------------------------------------------------*/
LPCTSTR	CHDHelper::HDHookTypeToString(ENUM_HOOK_TYPE HookType)
{
	switch (HookType)
	{
	case HT_NONE:
		return S_HT_NONE;
	case HT_ENTRY_HOOK_INVALID_ADDR_IAT:
		return S_HT_ENTRY_HOOK_INVALID_ADDR_IAT;
	case HT_ENTRY_HOOK_INVALID_ADDR_EAT:
		return S_HT_ENTRY_HOOK_INVALID_ADDR_EAT;
	case HT_ENTRY_HOOK_USE_CAVE_EAT:
		return S_HT_ENTRY_HOOK_USE_CAVE_EAT;
	case HT_INLINE_HOOK_EAT:
		return S_HT_INLINE_HOOK_EAT;
	default:
		return "Unknown Hook type";
	}
}

/**	-----------------------------------------------------------------------
	\brief	detour 패치가 된 경우 (jmp, call,...) EIP 를 계산

	\param	
			dwFuncEntryPoint	검사하고자 하는 API 의 엔트리 포인트 주소	
	\return		
			detour 패치가 된 경우 EIP 주소, 
			detour 패치가 안된 경우 0 리턴
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
DWORD_PTR CHDHelper::computeDetour(DWORD_PTR dwFuncEntryPoint)
{
	if (TRUE == IsBadReadPtr((LPVOID)dwFuncEntryPoint, 5))
	{
		_ASSERTE(!"computeDetour - bad read pointer");
		SetErrorString(FALSE, TEXT("%s >> 0x%08X is not valid address \n"), __FUNCTION__, dwFuncEntryPoint);
		return 0;
	}

	BYTE	opCode	= *((PBYTE)(dwFuncEntryPoint));
	DWORD	hook	= 0;
	DWORD	desc	= 0;

	//if (opCode == 0xEB) --> ignore for short jump (255 바이트 내의 점프임)
	if ( (opCode == 0xE8) || (opCode == 0xE9) )		// call , 32 비트 jmp
	{
		hook |= *( (PBYTE)(dwFuncEntryPoint + 1) ) << 0;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 2) ) << 8;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 3) ) << 16;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 4) ) << 24;

		// jmp, call 은 인스트럭션이 끝난 다음 바이트부터 offset 만큼 이동하므로..
		// 실제 불려지는 루틴의 주소는 dwFuncEntryPoint + 5 를 해줘야 한다. 
		//
		hook += (DWORD) (5 + dwFuncEntryPoint);
	}
	else if (opCode == 0xEA)
	{
		hook |= *( (PBYTE)(dwFuncEntryPoint + 1) ) << 0;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 2) ) << 8;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 3) ) << 16;
		hook |= *( (PBYTE)(dwFuncEntryPoint + 4) ) << 24;

		desc = *((WORD *)(dwFuncEntryPoint + 5));
	}

	return hook;
}


/**	-----------------------------------------------------------------------
	\brief	A130FF 형태의 헥사코드 문자열을 바이너리 코드로 변경해주는 함수
			binBuf 의 길이는 hexBuf 의 정확히 2/1 + sizeof(TCHAR) 이어야 한다. 
			헥사 코드는 한바이트를 두개의 문자로 표현하며 null-terminate 문자열이 필요하기 때문에..

	\param	
			hexBuf		변경할 헥사 코드 버퍼
			binBuf		바이너리 코드로 변경된 hexBuf 가 담길 버퍼
	\return			
			성공시 binBuf 의 길이를 리턴하고, binBuf 에 바이너리 값을 써 넣음
			실패시 -1 리턴 
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
typedef struct _HEX_BYTE
{
	CHAR	_16H;
	CHAR	_01H;
} HEX_BYTE, *PHEX_BYTE;


int	CHDHelper::convertHexBufToBinbuf(LPCTSTR hexBuf, PBYTE binBuf)
{
	_ASSERTE(NULL != hexBuf);
	_ASSERTE(NULL != binBuf);
	_ASSERTE(TRUE != IsBadReadPtr(hexBuf, _OPCODE_DUMP_BUF_LENGTH));
	_ASSERTE(TRUE != IsBadWritePtr(binBuf, _OPCODE_DUMP_LENGTH));
	_ASSERTE(0 == _tcslen(hexBuf) % 2);		// _tcslen 은 null-terminate 문자를 제외한 길이 리턴
													// hex 코드이므로 반드시 2의 배수여야 함

	if ( (NULL == hexBuf) || (NULL == binBuf) )
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - null param"), __FUNCTION__);
		return -1;
	}
	if (TRUE == IsBadReadPtr(hexBuf, _OPCODE_DUMP_BUF_LENGTH))
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - bad hexBuf"), __FUNCTION__);
		return -1;
	}
	if (TRUE == IsBadWritePtr(binBuf, _OPCODE_DUMP_LENGTH))
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - bad binBuf"), __FUNCTION__);
		return -1;
	}
	if (0 != _tcslen(hexBuf) % 2)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - corrupted hexBuf"), __FUNCTION__);
		return -1;
	}

	
	SIZE_T		i = 0;
	TCHAR		tmp[3] = {0x00};
	PHEX_BYTE	pHb = NULL;
	DWORD		OpByte = 0;
	INT			curPos = -1;
	SIZE_T		hexBufLen = 0;

	
	memset(binBuf, 0x00, _OPCODE_DUMP_LENGTH);
	hexBufLen = _tcslen(hexBuf);		// excluding null-terminate character
	while (i < hexBufLen)
	{
		curPos++;
		pHb = (PHEX_BYTE) &hexBuf[i];		
		tmp[0] = pHb->_16H;
		tmp[1] = pHb->_01H;
		tmp[2] = 0x00;

		OpByte = _tcstol( tmp, 0x00, 16);
		binBuf[curPos] = (BYTE)OpByte;

		i+= sizeof(HEX_BYTE);			
	}
	return curPos;
}


/**	-----------------------------------------------------------------------
	\brief	API_TO_RESTORE 구조체 벡터 hal 을 serialize 하는 메소드
			WARNING !!
				이 메소드가 리턴하는 pbuf 버퍼는 caller 쪽에서 사용후
				free_converted_stream() 메소드를 통해서 소멸시켜 줘야 한다.

	\param	
			IN	hal			API_TO_RESTORE 구조체 벡터
			IN	buf_limit	serialize 될 buf 의 길이 제한값
			OUT	pbuf_size	serialize 된 buf 의 길이 
			OUT	pbuf		serialize 된 buf
								WARNING!
							pbuf 는 초기화 되지 않은 포인터형 변수이어야 한다. (NULL ptr)
							초기화된 pointer 인 경우 memory leak 이 발생한다.(double malloc)

	\return			
			성공시 0
			실패시 -1
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int	CHDHelper::hal_to_stream(HOOKED_API_LIST& hal, int buf_limit, int *pbuf_size, char *&pbuf)
{
	{	// input parameter check
	_ASSERTE(TRUE != IsBadWritePtr(pbuf_size, sizeof(int)));
	if (TRUE == IsBadWritePtr(pbuf_size, sizeof(int)))
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - null param"), __FUNCTION__);
		return -1;
	}

	_ASSERTE(NULL == pbuf);
	if(NULL != pbuf)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - 4th param must be null"), __FUNCTION__);
		return -1;
	}

	_ASSERTE(sizeof(API_TO_RESTORE) <= buf_limit);
	if (sizeof(API_TO_RESTORE) > buf_limit)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid limit value"), __FUNCTION__);
		return -1;
	}

	}

	// let's do serialize the vector!
	//
	CMemoryStream ms;
	PAPI_TO_RESTORE patr = NULL;
	int stringLength = 0, atrLen = 0;

	HOOKED_API_LIST::iterator it = hal.begin();
	for (; it != hal.end(); it++)
	{
		patr = &(*it);
		if (NULL == patr)
		{
			_ASSERTE(!"invalid iterator");
			SetErrorString(FALSE, TEXT("%s >> invalid iterator"), __FUNCTION__);
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}
		
		CMemoryStream tmpMs;

		tmpMs.WriteToStream(&patr->HookType, sizeof(patr->HookType));
		// 문자열의 경우 
		// 			| 4byte length | 문자열 버퍼 | 
		// 로 만든다.
		stringLength = (int)_tcslen(patr->DLLName);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&patr->DLLName, stringLength);

		tmpMs.WriteToStream(&patr->Ordinal, sizeof(patr->Ordinal));

		stringLength = (int)_tcslen(patr->ApiName);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&patr->ApiName, stringLength);

		tmpMs.WriteToStream(&patr->OriginalAddr, sizeof(patr->OriginalAddr));
		tmpMs.WriteToStream(&patr->CurrentAddr, sizeof(patr->CurrentAddr));
		tmpMs.WriteToStream(&patr->DetourAddr, sizeof(patr->DetourAddr));

		/*stringLength = (int)_tcslen(patr->OriginalOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&patr->OriginalOpcodes, stringLength);

		stringLength = (int)_tcslen(patr->CurrentOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&patr->CurrentOpcodes, stringLength);

		stringLength = (int)_tcslen(patr->DetourAddrOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&patr->DetourAddrOpcodes, stringLength);*/
		
		tmpMs.WriteToStream(&patr->ValidStartAddr, sizeof(patr->ValidStartAddr));
		tmpMs.WriteToStream(&patr->ValidEndAddr, sizeof(patr->ValidEndAddr));


		// buf_limit 계산 (구조체 스트림 길이 값 포함)
		//
		if (buf_limit < (int) (ms.GetSize() + tmpMs.GetSize() + ATR_STREAM_LEN) )	
		{
			break;			
		}
		
		// tmp stream 을 복사하고, 초기화 한다.
		// 
		// API_TO_RESTORE 구조체 스트림의 전체 크기( integer ) | 스트림 데이터 
		// 형태로 다시 스트리밍 한다. 
		//
		//
		atrLen = tmpMs.GetSize();

		// API_TO_RESTORE 구조체 스트림의 길이를 쓴다. 
		if (-1 == ms.WriteToStream( &atrLen, sizeof(atrLen)) )
		{
			_ASSERTE(!"WriteToStream() failed");
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}

		// API_TO_RESTORE 구조체 스트림을 쓴다.
		if (-1 == ms.WriteToStream((char *)tmpMs.GetMemory(), atrLen) )
		{
			_ASSERTE(!"WriteToStream() failed");
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}
		//tmpMs.ClearStream();			tmpMS 에 대한 소멸자가 호출됨...필요없음


	}

	*pbuf_size = ms.GetSize();
	char *tmp_buf = (char *) malloc(ms.GetSize());
	memmove(tmp_buf, ms.GetMemory(), ms.GetSize());
	pbuf = tmp_buf;				// assign another pointer
	tmp_buf = NULL;				// never free this pointer
	return 0;
}


/**	-----------------------------------------------------------------------
	\brief	serialized 된 API_TO_RESTORE 구조체 스트림을 HOOKDED_API_LIST
			벡터로 만들어 내는 메소드
			WARNING!
				cleared_hal 에 append 하지 않고, 초기화 후 새로 추가한다.

	\param	
			IN	buf_size	pbuf 의 길이
			IN	pbuf		serialize 된 API_TO_RESTORE 구조체 스트림
			OUT	cleared_hal	deserialize 된 벡터
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int  CHDHelper::stream_to_hal(int buf_size, char *&pbuf, HOOKED_API_LIST& cleared_hal)
{
	// input parameter check block
	{ 
	_ASSERTE(0 < buf_size);	
	_ASSERTE(NULL != pbuf);	
	_ASSERTE(FALSE == IsBadReadPtr(pbuf, buf_size));
	if(	(0 > buf_size)								|| 
		(NULL == pbuf)								|| 		
		(FALSE != IsBadReadPtr(pbuf, buf_size)) )
	{
		SetErrorString(FALSE, TEXT("%s >> invalid parameter"), __FUNCTION__);
		return -1;
	}
	}
	
	// let's do deserialize the buffer
	// 
	cleared_hal.clear();
	API_TO_RESTORE atr;
	CMemoryStream ms;
	ms.WriteToStream(pbuf, buf_size);
	ms.ChangeCursor(0, 0);


	int stringLength = -1, atrLen = -1, ret = -1;
	for (;;)
	{
		ret = ms.ReadFromStream(&atrLen, ATR_STREAM_LEN);
		if ( 0 == ret)	// eof
			break;

		if ( -1 == ret) 
		{
			_ASSERTE(!"ReadFromStream");
			cleared_hal.clear();
			return -1;
		}


		// 길이 만큼 읽을수 있는지 검사
		//
		if (false == ms.CanReadFromStream(atrLen))
		{
			_ASSERTE(!"stream currupted");
			cleared_hal.clear();
			return -1;
		}

		memset(&atr, 0x00, sizeof(atr));

		// 스트림으로부터 읽어들이자~
		//		ReadFromStream() 에 대한 에러 처리 안함. -_-^ (귀찮아서)
		//		맘에 안들면 니가 해 -_-
		//
		ms.ReadFromStream(&atr.HookType, sizeof(atr.HookType));

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&atr.DLLName, stringLength);

		ms.ReadFromStream(&atr.Ordinal, sizeof(atr.Ordinal));

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&atr.ApiName, stringLength);

		ms.ReadFromStream(&atr.OriginalAddr, sizeof(atr.OriginalAddr));
		ms.ReadFromStream(&atr.CurrentAddr, sizeof(atr.CurrentAddr));
		ms.ReadFromStream(&atr.DetourAddr, sizeof(atr.DetourAddr));

		/*ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&atr.OriginalOpcodes, stringLength);

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&atr.CurrentOpcodes, stringLength);

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&atr.DetourAddrOpcodes, stringLength);*/

		ms.ReadFromStream(&atr.ValidStartAddr, sizeof(atr.ValidStartAddr));
		ms.ReadFromStream(&atr.ValidEndAddr, sizeof(atr.ValidEndAddr));

		cleared_hal.push_back(atr);
	}


	return 0;
}

/**	-----------------------------------------------------------------------
	\brief	

	\param	
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int  CHDHelper::hdfal_to_stream(HDFAULT_API_LIST &hdfal, int buf_limit, int *pbuf_size, char*&pbuf)
{
	{	// input parameter check blcok
	_ASSERTE(TRUE != IsBadWritePtr(pbuf_size, sizeof(int)));
	if (TRUE == IsBadWritePtr(pbuf_size, sizeof(int)))
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - null param"), __FUNCTION__);
		return -1;
	}

	_ASSERTE(NULL == pbuf);
	if(NULL != pbuf)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid param - 4th param must be null"), __FUNCTION__);
		return -1;
	}

	_ASSERTE(sizeof(RESTORE_FAULT_API) <= buf_limit);
	if (sizeof(RESTORE_FAULT_API) > buf_limit)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid limit value"), __FUNCTION__);
		return -1;
	}
	} // input parameter check blcok

	
	
	// let's do serialize the vector!
	//
	CMemoryStream ms;
	PRESTORE_FAULT_API prfa = NULL;
	int stringLength = 0, atrLen = 0;

	HDFAULT_API_LIST::iterator it = hdfal.begin();
	for (; it != hdfal.end(); it++)
	{
		prfa = &(*it);
		if (NULL == prfa)
		{
			_ASSERTE(!"invalid iterator");
			SetErrorString(FALSE, TEXT("%s >> invalid iterator"), __FUNCTION__);
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}
		
		CMemoryStream tmpMs;

		tmpMs.WriteToStream(&prfa->ErrorCode, sizeof(prfa->ErrorCode));		
		tmpMs.WriteToStream(&prfa->apiInfo.HookType, sizeof(prfa->apiInfo.HookType));
		// 문자열의 경우 
		// 			| 4byte length | 문자열 버퍼 | 
		// 로 만든다.
		stringLength = (int)_tcslen(prfa->apiInfo.DLLName);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&prfa->apiInfo.DLLName, stringLength);

		tmpMs.WriteToStream(&prfa->apiInfo.Ordinal, sizeof(prfa->apiInfo.Ordinal));

		stringLength = (int)_tcslen(prfa->apiInfo.ApiName);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&prfa->apiInfo.ApiName, stringLength);

		tmpMs.WriteToStream(&prfa->apiInfo.OriginalAddr, sizeof(prfa->apiInfo.OriginalAddr));
		tmpMs.WriteToStream(&prfa->apiInfo.CurrentAddr, sizeof(prfa->apiInfo.CurrentAddr));
		tmpMs.WriteToStream(&prfa->apiInfo.DetourAddr, sizeof(prfa->apiInfo.DetourAddr));

		/*stringLength = (int)_tcslen(prfa->apiInfo.OriginalOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&prfa->apiInfo.OriginalOpcodes, stringLength);

		stringLength = (int)_tcslen(prfa->apiInfo.CurrentOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&prfa->apiInfo.CurrentOpcodes, stringLength);

		stringLength = (int)_tcslen(prfa->apiInfo.DetourAddrOpcodes);
		tmpMs.WriteToStream(&stringLength, sizeof(int));
		tmpMs.WriteToStream(&prfa->apiInfo.DetourAddrOpcodes, stringLength);*/
		
		tmpMs.WriteToStream(&prfa->apiInfo.ValidStartAddr, sizeof(prfa->apiInfo.ValidStartAddr));
		tmpMs.WriteToStream(&prfa->apiInfo.ValidEndAddr, sizeof(prfa->apiInfo.ValidEndAddr));


		// buf_limit 계산 (구조체 스트림 길이 값 포함)
		//
		if (buf_limit < (int) (ms.GetSize() + tmpMs.GetSize() + ATR_STREAM_LEN) )	
		{
			break;			
		}
		
		// tmp stream 을 복사하고, 초기화 한다.
		// 
		// API_TO_RESTORE 구조체 스트림의 전체 크기( integer ) | 스트림 데이터 
		// 형태로 다시 스트리밍 한다. 
		//
		//
		atrLen = tmpMs.GetSize();

		// API_TO_RESTORE 구조체 스트림의 길이를 쓴다. 
		if (-1 == ms.WriteToStream( &atrLen, sizeof(atrLen)) )
		{
			_ASSERTE(!"WriteToStream() failed");
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}

		// API_TO_RESTORE 구조체 스트림을 쓴다.
		if (-1 == ms.WriteToStream((char *)tmpMs.GetMemory(), atrLen) )
		{
			_ASSERTE(!"WriteToStream() failed");
			
			// return fail
			//
			*pbuf_size = -1;
			pbuf = NULL;
			return -1;
		}
		//tmpMs.ClearStream();			tmpMS 에 대한 소멸자가 호출됨...필요없음


	}

	*pbuf_size = ms.GetSize();
	char *tmp_buf = (char *) malloc(ms.GetSize());
	memmove(tmp_buf, ms.GetMemory(), ms.GetSize());
	pbuf = tmp_buf;				// assign another pointer
	tmp_buf = NULL;				// never free this pointer
	return 0;
}





int  CHDHelper::stream_to_hdfal(int buf_size, char *&pbuf, HDFAULT_API_LIST &cleared_hdfal)
{
	_ASSERTE(0 < buf_size);
	_ASSERTE(NULL != pbuf);	
	_ASSERTE(FALSE == IsBadReadPtr(pbuf, buf_size));
	if(	( 0 > buf_size)								|| 		
		( NULL == pbuf)								|| 		
		( FALSE != IsBadReadPtr(pbuf, buf_size)) )
	{
		SetErrorString(FALSE, TEXT("%s >> invalid parameter"), __FUNCTION__);
		return -1;
	}


	// let's do deserialize the buffer
	// 
	cleared_hdfal.clear();
	RESTORE_FAULT_API rfa;
	CMemoryStream ms;
	ms.WriteToStream(pbuf, buf_size);
	ms.ChangeCursor(0, 0);

	int stringLength = -1, atrLen = -1, ret = -1;
	for (;;)
	{
		ret = ms.ReadFromStream(&atrLen, ATR_STREAM_LEN);
		if ( 0 == ret)	// eof
			break;

		if ( -1 == ret) 
		{
			_ASSERTE(!"ReadFromStream");
			cleared_hdfal.clear();
			return -1;
		}


		// 길이 만큼 읽을수 있는지 검사
		//
		if (false == ms.CanReadFromStream(atrLen))
		{
			_ASSERTE(!"stream currupted");
			cleared_hdfal.clear();
			return -1;
		}

		memset(&rfa, 0x00, sizeof(rfa));

		// 스트림으로부터 읽어들이자~
		//		ReadFromStream() 에 대한 에러 처리 안함. -_-^ (귀찮아서)
		//		맘에 안들면 니가 해 -_-
		//
		ms.ReadFromStream(&rfa.ErrorCode, sizeof(rfa.ErrorCode));
		ms.ReadFromStream(&rfa.apiInfo.HookType, sizeof(rfa.apiInfo.HookType));

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&rfa.apiInfo.DLLName, stringLength);

		ms.ReadFromStream(&rfa.apiInfo.Ordinal, sizeof(rfa.apiInfo.Ordinal));

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&rfa.apiInfo.ApiName, stringLength);

		ms.ReadFromStream(&rfa.apiInfo.OriginalAddr, sizeof(rfa.apiInfo.OriginalAddr));
		ms.ReadFromStream(&rfa.apiInfo.CurrentAddr, sizeof(rfa.apiInfo.CurrentAddr));
		ms.ReadFromStream(&rfa.apiInfo.DetourAddr, sizeof(rfa.apiInfo.DetourAddr));

		/*ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&rfa.apiInfo.OriginalOpcodes, stringLength);

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&rfa.apiInfo.CurrentOpcodes, stringLength);

		ms.ReadFromStream(&stringLength, sizeof(stringLength));
		ms.ReadFromStream(&rfa.apiInfo.DetourAddrOpcodes, stringLength);*/

		ms.ReadFromStream(&rfa.apiInfo.ValidStartAddr, sizeof(rfa.apiInfo.ValidStartAddr));
		ms.ReadFromStream(&rfa.apiInfo.ValidEndAddr, sizeof(rfa.apiInfo.ValidEndAddr));

		cleared_hdfal.push_back(rfa);
	}


	return 0;
}

/**	-----------------------------------------------------------------------
	\brief	hal_to_stream() 메소드를 통해 동적으로 할당한 버퍼를
			소멸시키는 메소드

	\param	
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
int  CHDHelper::free_converted_stream(char *buf_to_free)
{
	if(NULL == buf_to_free)
	{
		_ASSERTE(!"buf_to_free == NULL");
		SetErrorString(FALSE, TEXT("%s >> invalid param - null param"), __FUNCTION__);
		return-1;
	}

	//delete [] buf_to_free;
	free(buf_to_free);
	buf_to_free = NULL;

	return 0;
}


