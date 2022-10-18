#include "stdafx.h"
#include "csvlib.h"

Csvlib::Csvlib() 
: m_nCnt(0)
{
}

Csvlib::~Csvlib()
{
	Clear();
}

void Csvlib::Clear()
{
	m_nCnt = 0;
	m_MList.clear();
	m_VList.clear();
}

bool Csvlib::LoadFromFile(char* lpszPath)
{
	if ( !lpszPath ) return false;

	char szData[LINE_BUF_1K];

	std::ifstream from(lpszPath);
	while(!from.eof())
	{
		from.getline(szData, LINE_BUF_1K);
		if ( !strlen(szData) ) break;

		if ( m_nCnt++ == 0 ) DecodeCsv(Csvlib::FIELD, szData);
		else DecodeCsv(Csvlib::RECORD, szData);
	}

	return true;
}

string Csvlib::Select(int nIdx, char* lpszField)
{
	if ( !lpszField || !strlen(lpszField) ) return "";

	// find field ke
	std::map < string, byte > ::iterator iter;
	iter = m_MList.find(lpszField);
	if ( iter == m_MList.end() ) return "";

	// find record key
	int nKey = (int)m_MList.size() * nIdx + iter->second;
	if ( nKey < 0 || nKey >= (int)m_VList.size() ) return "";

	return m_VList[nKey];
}

void Csvlib::DecodeCsv(Csvlib::CSV_TYPE eType, char* lpszData)
{
	char szData[RECORD_BUF_64B];
	bool bLockActive = false;
	int nCnt = 0, nFieldCnt = 0;

	for ( int i = 0; i <= (int)strlen(lpszData); i++ )
	{
		if ( lpszData[i] == 34 )
		{
			if ( lpszData[i+1] == 34 ) i += 1;
			else
			{
				bLockActive = !bLockActive;
				continue;
			}			
		}

		if ( !bLockActive && ( lpszData[i] == 44 || i == strlen(lpszData) ) )
		{
			szData[nCnt] = '\0';
			nCnt = 0;

			switch(eType)
			{
				case Csvlib::FIELD	: m_MList.insert(pair<string, byte> (szData, nFieldCnt++));	break;
				case Csvlib::RECORD	: m_VList.push_back(szData);								break;
			}
		}
		else
		{
			szData[nCnt++] = lpszData[i];
		}
	}
}
