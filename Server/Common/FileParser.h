#ifndef __FILE_PARSER_H__
#define __FILE_PARSER_H__

#pragma once

#include <ServerStruct.h>

class CFileParser
{
private:
	SERVER_ENV				m_ServerEnv;

public:
	CFileParser(void);
	virtual ~CFileParser(void);

	void Init(void);
	virtual BOOL ParseInitFile(char* szFileName);
	inline SERVER_ENV* GetServerEnv() { return &m_ServerEnv;}

};

#endif // __FILE_PARSER_H__
