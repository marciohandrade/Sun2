#pragma once

//=============================================================================================================================
/// Crypt.RndKey structure
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- ���Ͽ� ���� ��ȣȭ/��ȣȭ ���̺귯��(TheMOLA)�� ���� �⺻ ��ȣȭ Ű ����
	  �̰��� ���� ���� ������, Ÿ�Ժ��� ���� �ٸ� Ű�� �������� ����� ���̴�.
	@history 
	- 
*/
//=============================================================================================================================


#ifndef __RNDKEY_H
#define __RNDKEY_H

typedef DWORD	RNDKEY;

struct RANDOMKEY
{
	static RNDKEY	GetDBProxyKey( DWORD dwCurKey )
	{
		return ((DWORD)0x921F58D1*(dwCurKey) + 0x0144FF32);
	}
};

#endif //__RNDKEY_H

