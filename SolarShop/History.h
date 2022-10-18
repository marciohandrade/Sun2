#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL

//=============================================================================================================================
/// History class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2007. 1. 8(��)
	@remark
		- �̷� ���� Ŭ����
	@note
		- ���� ������ / ���� ���� �̷� / ���� �̷�
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
	char m_pszCharName[MAX_CHARACTER_NAME_LENGTH+1];		//< �̷��� ��ü
	char m_pszItemName[MAX_ITEM_NAME_LENGTH+1];				//< ������ �̸�
};

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
