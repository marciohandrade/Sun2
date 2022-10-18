#pragma once
#include <SLM/CommonSLM.h>

class CStringToken  
{
public:
	CStringToken();
	virtual ~CStringToken();
	CStringToken(const tstring& dataLine, const tstring& delim);
	
	size_t countTokens();							//��ū�� ���� 
	bool hasMoreTokens();							//��ū�� �����ϴ��� Ȯ�� 
	tstring nextToken();							//���� ��ū

private: 
	tstring data;
	tstring delimiter;								//������, ������ 
	std::vector<tstring> tokens;					//��ū�� ���Ϳ� ���� 
	std::vector<tstring>::iterator index;			//���Ϳ� ���� �ݺ��� 

	void split();									//��Ʈ���� �����ڷ� ������ ���Ϳ� ���� 
};


class CStringTokenW  
{
public:
	CStringTokenW();
	virtual ~CStringTokenW();
	CStringTokenW(const std::wstring& dataLine, const std::wstring& delim);
	
	size_t			countTokens();						//��ū�� ���� 
	bool			hasMoreTokens();					//��ū�� �����ϴ��� Ȯ�� 
	std::wstring	nextToken();						//���� ��ū

private: 
	std::wstring data;
	std::wstring delimiter;								//������, ������ 
	std::vector<std::wstring> tokens;					//��ū�� ���Ϳ� ���� 
	std::vector<std::wstring>::iterator index;			//���Ϳ� ���� �ݺ��� 

	void split();										//��Ʈ���� �����ڷ� ������ ���Ϳ� ���� 
};

