//=============================================================================================================================
/// WUTLoader class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 28
	@note
		- WUT ���Ͽ��� ������ ����+�׷� ���� �д� static class
	@history
		
*/
//=============================================================================================================================

#pragma once


#include <string>

class CLoadHelper;
class UnitRegenInfoManager;
class UnitGroupInfoManager;

class WUTLoader
{
public:
	WUTLoader(void);
	~WUTLoader(void);

	static BOOL Load( FIELDCODE wutcode, 
		UnitRegenInfoManager * pRegenInfoMGR, UnitGroupInfoManager * pGroupInfoMGR );
	//static BOOL Load( std::string wmoFileName  );
};
