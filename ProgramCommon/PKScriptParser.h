#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

struct PKOptionInfo 
{
    enum {  
        ignore_BoundaryPart = 0x01,
        ignore_NomalPart    = 0x02,
        ignore_FreePart     = 0x04, 
        ignore_Domination   = 0x08, //_NA001385_20090924_DOMINATION_BASE 
        ignore_SafetyPart   = 0x10, //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
    };
    //__NA_0_POSSESSION_ITEM_DROP
	enum ePoessessionDropOption
	{
		ENUM_POSSESSION_DROP_NO			= 0,	//�ͼ� ������ ��� ����
		ENUM_POSSESSION_DROP_ORIGINAL	= 1,	//�ͼ� ������ ���� �״�� ���
		ENUM_POSSESSION_DROP_DESTROY	= 2,	//�ͼ� ������ �ı��ؼ� ���
		
		ENUM_DROP_MAX,
	};

	LEVELTYPE		m_PKAllowableLV;		// PK ���� ����
	LEVELTYPE		m_PKProtectLV;			// PK ��ȣ ����

	int				m_BadFriendDropRatio;	// ����ģ��(������) �����
	int				m_PreChaoDropRatio;		// ����ī�� �����
	int				m_ChaoDropRatio;		// ī�� �����
	int				m_EquipDropRatio;		// ��� �����(���/�κ� ����)

	DWORD			m_PreChaoTime;			// ����ī�� �����ð�(�ʴ���)
	DWORD			m_ChaoTime;				// ī�� �����ð�
	DWORD			m_AddChaoTime;			// ī�� �߰��ð�

	DWORD			m_BoradoriAttackTime;	// ���������� ������ ����ð�
	DWORD			m_BoradoriNormalTime;	// ��������� ������ ����ð�

    BYTE            m_IgnorePKTile;
	WORD			m_wEND_ROW_KEY;			// ���� ǥ���ϴ� �Ķ����
	ePoessessionDropOption		m_ePossessionDropOption;
};

class PKScriptParser : public util::Singleton<PKScriptParser>, public IParser
{
public:
	PKScriptParser();
	~PKScriptParser();

	VOID					Reload();
	virtual VOID			Release();
	inline PKOptionInfo&	GetPKOptionInfo()	{ return m_PKOptionInfo; }

public:
	BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL _Load( BOOL bReload );
private:
	BOOL					CheckValidity();
	VOID					Unload();

	PKOptionInfo			m_PKOptionInfo;
	TCHAR					m_pszFileName[MAX_PATH];
};