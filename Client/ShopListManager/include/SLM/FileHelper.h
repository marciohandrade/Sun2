#pragma once
#include <SLM/CommonSLM.h>


class CFileHelper
{
public:
	CFileHelper(void);
	~CFileHelper(void);
private:
	CFileHelper(CFileHelper& cls);
	CFileHelper& operator=(CFileHelper& cls);

public:
	void init(void);

	bool openFile(std::string& filepath);
	bool openFile(std::wstring& filepath);

	bool CheckUTF8BOM(std::wstring& filepath);
	bool IsUTF8BOM(void);
	bool IsEOF(void);

	std::wstring	Trim(std::wstring& str, const std::wstring& drop = std::wstring(L"\r\n"));
	std::wstring	GetLine(void);

	tstring			TrimTString(tstring& str, const tstring& drop = tstring(_T("\r\n")));
	tstring			GetLineTString();

	std::string		DetectDelimiter(std::string& delimiter);
	std::wstring	DetectDelimiter(std::wstring& delimiter);



private:
	FILE*	FileHandle;
	bool	IsUTF8BOMFile;
	int		CurrentLineNum;

	static const	int MaxLength;
};
