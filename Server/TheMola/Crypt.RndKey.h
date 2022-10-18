#pragma once

//=============================================================================================================================
/// Crypt.RndKey structure
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 05. 15
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)를 위한 기본 암호화 키 정의
	  이것은 이후 여러 버전별, 타입별로 각기 다른 키를 가지도록 변경될 것이다.
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

