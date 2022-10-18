/*
	2009-07-24, ������
	������Ʈ ������ ���� ���Ǵ� ��ü
*/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/ListManager.h>


class CShopList;

class CShopListManager : public CListManager
{
public:
	CShopListManager();	
	virtual ~CShopListManager();

	CShopList*		GetListPtr() {return m_ShopList.get();};

private:
	std::auto_ptr<CShopList>	m_ShopList;

	void			RegistScriptFiles();
	void			RegistScriptFilesT();
	void			RegistScriptFilesW();

	WZResult		LoadScript(bool bDownLoad);
	WZResult		LoadScriptT(bool bDownLoad);
	WZResult		LoadScriptW(bool bDownLoad);
};