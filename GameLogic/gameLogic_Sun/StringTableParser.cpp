#include "StdAfx.h"
#include ".\stringtableparser.h"
#include <string>
#include "util/util.h"

CStringTableParser::CStringTableParser(void)
{
	m_iter = m_mapStringTable.end();
}

CStringTableParser::~CStringTableParser(void)
{
	m_mapStringTable.clear();
}

bool CStringTableParser::Reload()
{
	return Init((TCHAR*)m_sTablePath.c_str());
}

bool CStringTableParser::Init(TCHAR* pszFullPath)
{
	//file open
	m_sTablePath = pszFullPath;
	FILE* fp = fopen(pszFullPath, "r");
	if(fp)
	{
		TCHAR szBuff[1024];
		ZeroMemory(szBuff, sizeof(TCHAR)*1024);
		while(_fgetts(szBuff, 1024, fp) != NULL)
		{
			trimLeft(szBuff);
			trimRight(szBuff);

			if( (strncmp(szBuff, "//", 2) == 0) )
			{				
				continue;
			}

			//시작과 끝이면 넘어간다.
			if( (strncmp(szBuff, "start", 5) == 0) )
			{
				_LOG("STRIGNTABLE - START(%s)",pszFullPath);
				continue;
			}

			if(strncmp(szBuff, "end", 3) == 0)
			{
				_LOG("STRIGNTABLE - END");
				break;
			}

			std::string str = szBuff;
			int nPos1 = str.find("\t");
			if(std::string::npos ==  nPos1)
			{
				_LOG("StringTable parse Error.(%s)", str.c_str());
				continue;
			}

			std::string code = str.substr(0, nPos1);
						

			int nPos2 = str.find("\n");
			std::string desc = str.substr(nPos1+1, nPos2-nPos1-1);

			TCHAR szTmp[1024] = {0,};
			strcpy(szTmp, desc.c_str());
			trimLeft(szTmp);
			trimRight(szTmp);
			desc = szTmp;
			int iCode = atoi(code.c_str());
			m_mapStringTable[iCode] = desc;
			m_mapIdxTable[desc] = iCode;
	//		_LOG("[%d] %s", iCode, desc.c_str());


/*			TCHAR szSeps[]   = "\t";

			//탭을 기준으로 코드와 스트링을 분리해서 파싱한다.
			TCHAR* pszCode = _tcstok(szBuff, szSeps);
			TCHAR* pszString = _tcstok(NULL, szSeps);


			//둘다 널이 아니면.... 맵에 집어넣는다.
			if(pszCode && pszString)
			{
				pszString[_tcslen(pszString)-1] = '\0';
				int iCode = _ttoi(pszCode);

				if(203009 == iCode)
				{
					_LOG("");
				}

				m_mapStringTable[iCode] = pszString;
				m_mapIdxTable[pszString] = iCode;
				_LOG("[%d] %s", iCode, pszString);
			}
			else
			{
				//실패다.
				m_mapStringTable.clear();
				m_mapIdxTable.clear();
				fclose(fp);
				return false;
			}
			*/
		}
		fclose(fp);
		return true;
	}
	return false;
}

TCHAR* CStringTableParser::GetStringByCode(int iCode)
{
	std::map<int, std::string>::iterator iter;

	//찾았다면..
	if ((iter = m_mapStringTable.find(iCode)) != m_mapStringTable.end())
	{
		return (TCHAR*)(*iter).second.c_str(); 
	}
	return NULL;
	
}

int CStringTableParser::GetCodeByString(std::string str)
{
	std::map<std::string, int>::iterator iter;

	//찾았다면..
	if ((iter = m_mapIdxTable.find(str)) != m_mapIdxTable.end())
	{
		return (int)(*iter).second; 
	}
	return -1;
}

void CStringTableParser::SetFirst()
{
	m_iter = m_mapStringTable.begin();
}

char* CStringTableParser::GetNext()
{
	char* psz = NULL;

	if(m_iter != m_mapStringTable.end())
	{
		psz = (char*)m_iter->second.c_str();
		m_iter++;

		return psz;
	}
	else
		return psz;
}
