#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL

//=============================================================================================================================
/// History class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2007. 1. 8(월)
	@remark
		- 이력 관련 클래스
	@note
		- 받은 선물함 / 보낸 선물 이력 / 구매 이력
	@history 
		- 
*/
//=============================================================================================================================
#pragma once

class History
{
public:
	History(void);
	virtual ~History(void);

public:
	const char * GetCharName() { return m_pszCharName; }
	const char * GetItemName() { return m_pszItemName; }

private:
	__PROPERTY( DWORD, ServerKey );
	__REF_PROPERTY( DWORD64, DateTime );					//< 20061209243500
	char m_pszCharName[MAX_CHARACTER_NAME_LENGTH+1];		//< 이력의 주체
	char m_pszItemName[MAX_ITEM_NAME_LENGTH+1];				//< 아이템 이름
};

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
