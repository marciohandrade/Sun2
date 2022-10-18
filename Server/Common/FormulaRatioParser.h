#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#include <SolarFileReader.h>

enum RATIO_TYPE
{
	RATIO_TYPE_EXP_FIELD			= 1,		// ����ġ ����(�ʵ�)
	RATIO_TYPE_EXP_HUNTING			= 11,		// ����ġ ����(����)
	RATIO_TYPE_EXP_MISSION			= 21,		// ����ġ ����(�̼�)

	RATIO_TYPE_DROP_FIELD			= 101,		// ��� ����(�ʵ�)
	RATIO_TYPE_DROP_HUNTING			= 111,		// ��� ����(����)
	RATIO_TYPE_DROP_MISSION			= 121,		// ��� ����(�̼�)

	RATIO_TYPE_CHUNTING_EXP			= 1001,		// �������� ����ġ ����
	RATIO_TYPE_CHUNTING_MONEY		= 2001,		// �������� ���� ����
	RATIO_TYPE_CHUNTING_PARTYNUM	= 10000,	// �������� ��Ƽ������������ %��
	RATIO_TYPE_TIMEATTACK			= 10001,	// �������� ��� ����(Ÿ�Ӿ���)
	RATIO_TYPE_DEATHMATCH			= 10002,	// �������� ��� ����(������ġ)
	RATIO_TYPE_SURVIVAL				= 10003,	// �������� ��� ����(�����̹�)

	RATIO_TYPE_REGEN_TIME			= 20001,	// ���� �ӵ� ����
};


struct BASE_FORMULA_RATIOINFO
{
	WORD		m_wNCode;		// ���� �ڵ�(RATIO_TYPE)
	LEVELTYPE	m_Level;		// ����
	float		m_fRatio;		// ����
};

#define MAKE_FOMULAKEY( CODE, LEVEL )	(DWORD)( ((CODE&0xFFFF)<<16)|(LEVEL&0xFFFF) )

class FormulaRatioParser : public util::Singleton<FormulaRatioParser>, public IParser
{
public:
	FormulaRatioParser();
	~FormulaRatioParser();

	VOID					Init( DWORD dwPoolSize );
	VOID					Release();

	VOID					Reload();

	BASE_FORMULA_RATIOINFO*	GetFormulaRatio( WORD wCode, LEVELTYPE Level=0 );

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );

private:
	VOID					Unload();

private: // FILEDs
	util::SolarHashTable<BASE_FORMULA_RATIOINFO *>*		m_pRatioHashTable;
	char												m_pszFileName[MAX_PATH];
};





