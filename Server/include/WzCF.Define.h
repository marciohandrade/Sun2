#pragma once

//=============================================================================================================================
/// WzCF Definitions
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 04
	@remark
	- Webzen Crypt File
	@note
	- 파일에 대한 암호화/복호화 라이브러리(TheMOLA)를 위한 기본 정의
	@history 
	-  
*/
//=============================================================================================================================

//#include <Global.Define.h>
#define __WAVERIX_THE_MOLA_UPDATE_VERSION2__
#define __WAVERIX_DATA_STRUCTURE_RELOCATION__

#include <windows.h>

typedef CHAR*					QueryKey;

#define MIN_FILE_DATA_SIZE		(4096)
#define MAX_ID_SIZE				(23)
#define MAX_PWD_SIZE			(MAX_ID_SIZE)

