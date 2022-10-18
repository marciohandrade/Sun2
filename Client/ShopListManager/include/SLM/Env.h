#pragma once
#include <SLM/CommonSLM.h>

class CEnv
{
public:
	CEnv(void);
	~CEnv(void);

private:
	CEnv(CEnv& cls);
	CEnv& operator=(CEnv& cls);

public:
// tstring 용
	static const tstring		BANNER_SCRIPT_FILENAME;
	static const tstring		SHOPLIST_SCRIPT_CATEGORY;
	static const tstring		SHOPLIST_SCRIPT_PACKAGE;
	static const tstring		SHOPLIST_SCRIPT_PRODUCT;
	static const tstring		SHOPLIST_SCRIPT_PROPERTY;
	static const tstring		SHOPLIST_SCRIPT_PACKAGE_MAP;
	static const tstring		SHOPLIST_SCRIPT_PRODUCT_MAP;

	static const tstring		SCRIPT_DELIMITER;				// 스크립트 파일의 구분자 (기본)
	static const tstring		SCRIPT_DELIMITER_OLD;			// 스크립트 파일의 예전 구분자

// std::wstring 용
	static const std::wstring	BANNER_SCRIPT_FILENAME_W;
	static const std::wstring	SHOPLIST_SCRIPT_CATEGORY_W;
	static const std::wstring	SHOPLIST_SCRIPT_PACKAGE_W;
	static const std::wstring	SHOPLIST_SCRIPT_PRODUCT_W;
	static const std::wstring	SHOPLIST_SCRIPT_PROPERTY_W;
	static const std::wstring	SHOPLIST_SCRIPT_PACKAGE_MAP_W;
	static const std::wstring	SHOPLIST_SCRIPT_PRODUCT_MAP_W;

	static const std::wstring	SCRIPT_DELIMITER_W;				// 스크립트 파일의 구분자 (기본)
	static const std::wstring	SCRIPT_DELIMITER_OLD_W;			// 스크립트 파일의 예전 구분자

// Version
	static const int			SCRIPT_VERSION3;			// 샵 스크립트 3개 파일 사용
	static const int			SCRIPT_VERSION6;			// 샵 스크립트 6개 파일 사용
};

