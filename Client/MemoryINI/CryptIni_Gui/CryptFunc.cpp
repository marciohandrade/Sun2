#include "stdafx.h"
#include "CryptFunc.h"

//
//	암호화 알고리즘을 기술함, 필요에 따라서 Encrypt/Decrypt를 따로 만들 수 있음
//
//		주의:	내부적으로 malloc류의 함수로 버퍼를 생성한 후,
//				out_data로 넘겨주어야 함, 동적할당된 버퍼는 사용이 끝나면
//				MemoryIni에서 해제 시켜주므로 해제 걱정은 필요 없음
//


int 
__stdcall 
SimpleXorCryption(
	PBYTE in_data, 
	DWORD in_size, 
	PBYTE *out_data,	
	DWORD *out_size
	)
{
	PBYTE tmp = (PBYTE) calloc(1, in_size);

	if ( NULL == tmp )
	{
		_ASSERTE( !"calloc fail" );
		return -1;
	}

	for (DWORD i = 0; i < in_size; i++)
	{
		tmp[i] = in_data[i] ^ 0x32;
	}

	*out_data = tmp;
	*out_size = in_size;

	return 0;
}

