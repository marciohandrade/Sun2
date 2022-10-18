#pragma once

#include <fstream>
#include <WTypes.h>
#include <string>
#include <map>
#include <vector>

#define	LINE_BUF_1K			1024
#define RECORD_BUF_64B		64

using namespace std;

class Csvlib
{
	enum CSV_TYPE { FIELD, RECORD };

public:

	Csvlib();
	virtual ~Csvlib();

	void Clear();
	bool LoadFromFile(char* lpszPath);

	string Select(int nIdx, char* lpszField);
	int GetCount() { return m_nCnt - 1; };

private:
	void DecodeCsv(Csvlib::CSV_TYPE eType, char* lpszData);	

	int m_nCnt;									//* csv line cnt
	std::map <string, byte>		m_MList;		//* field data
	std::vector <string>		m_VList;		//* field record data	
};