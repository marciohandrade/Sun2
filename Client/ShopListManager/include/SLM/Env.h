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
// tstring ��
	static const tstring		BANNER_SCRIPT_FILENAME;
	static const tstring		SHOPLIST_SCRIPT_CATEGORY;
	static const tstring		SHOPLIST_SCRIPT_PACKAGE;
	static const tstring		SHOPLIST_SCRIPT_PRODUCT;
	static const tstring		SHOPLIST_SCRIPT_PROPERTY;
	static const tstring		SHOPLIST_SCRIPT_PACKAGE_MAP;
	static const tstring		SHOPLIST_SCRIPT_PRODUCT_MAP;

	static const tstring		SCRIPT_DELIMITER;				// ��ũ��Ʈ ������ ������ (�⺻)
	static const tstring		SCRIPT_DELIMITER_OLD;			// ��ũ��Ʈ ������ ���� ������

// std::wstring ��
	static const std::wstring	BANNER_SCRIPT_FILENAME_W;
	static const std::wstring	SHOPLIST_SCRIPT_CATEGORY_W;
	static const std::wstring	SHOPLIST_SCRIPT_PACKAGE_W;
	static const std::wstring	SHOPLIST_SCRIPT_PRODUCT_W;
	static const std::wstring	SHOPLIST_SCRIPT_PROPERTY_W;
	static const std::wstring	SHOPLIST_SCRIPT_PACKAGE_MAP_W;
	static const std::wstring	SHOPLIST_SCRIPT_PRODUCT_MAP_W;

	static const std::wstring	SCRIPT_DELIMITER_W;				// ��ũ��Ʈ ������ ������ (�⺻)
	static const std::wstring	SCRIPT_DELIMITER_OLD_W;			// ��ũ��Ʈ ������ ���� ������

// Version
	static const int			SCRIPT_VERSION3;			// �� ��ũ��Ʈ 3�� ���� ���
	static const int			SCRIPT_VERSION6;			// �� ��ũ��Ʈ 6�� ���� ���
};

