#include "stdafx.h"
#include "CryptFunc.h"

//
//	��ȣȭ �˰����� �����, �ʿ信 ���� Encrypt/Decrypt�� ���� ���� �� ����
//
//		����:	���������� malloc���� �Լ��� ���۸� ������ ��,
//				out_data�� �Ѱ��־�� ��, �����Ҵ�� ���۴� ����� ������
//				MemoryIni���� ���� �����ֹǷ� ���� ������ �ʿ� ����
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

