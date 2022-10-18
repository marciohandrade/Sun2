#pragma once

#include <Singleton.h>
#include <string.h>
#include <map>

//using namespace util;


class CStringTableParser// : public Singleton<CStringTableParser>
{
public:
	CStringTableParser(void);
	~CStringTableParser(void);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : Init
	// param1 : StringTable.txt�� ���
	// ret : ��ũ��Ʈ���� �ε� ��������
	// desc : ��Ʈ�����̺��� �о� �޸𸮿� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	bool Init(TCHAR* pszFullPath);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : GetStringByCode
	// param1 : ��Ʈ�����̺��� �ڵ�
	// ret : �ش� �ڵ��� ��Ʈ��
	// desc : ��Ʈ�����̺��� �ش� �ڵ��� ��Ʈ���� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	TCHAR* GetStringByCode(int iCode);

	//////////////////////////////////////////////////////////////////////////////////////
	// func : GetCodeByString
	// param1 : ��Ʈ�����̺��� ��Ʈ��
	// ret : �ش� ��Ʈ���� �ڵ��ȣ
	// desc : ��Ʈ�����̺��� �ش� ��Ʈ���� �ڵ��ȣ�� �����Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	int GetCodeByString(std::string str);

	void SetFirst();
        char* GetNext();
	
	//////////////////////////////////////////////////////////////////////////////////////
	// func : Reload
	// param1 : n/a
	// ret : ��ε� ��������
	// desc : ��ε��Ѵ�.
	//////////////////////////////////////////////////////////////////////////////////////
	bool Reload();

	inline int GetCount() { m_mapStringTable.size(); }

private:
	//��Ʈ�����̺�
	std::map<int, std::string> m_mapStringTable;

	//��������
	std::map<std::string, int> m_mapIdxTable;
	
	std::string m_sTablePath;

	std::map<int, std::string>::iterator m_iter;
};
