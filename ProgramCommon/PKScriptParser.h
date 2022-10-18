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
		ENUM_POSSESSION_DROP_NO			= 0,	//귀속 아이템 드랍 않함
		ENUM_POSSESSION_DROP_ORIGINAL	= 1,	//귀속 아이템 원형 그대로 드랍
		ENUM_POSSESSION_DROP_DESTROY	= 2,	//귀속 아이템 파괴해서 드랍
		
		ENUM_DROP_MAX,
	};

	LEVELTYPE		m_PKAllowableLV;		// PK 가능 레벨
	LEVELTYPE		m_PKProtectLV;			// PK 보호 레벨

	int				m_BadFriendDropRatio;	// 나쁜친구(리벤지) 드랍률
	int				m_PreChaoDropRatio;		// 프리카오 드랍률
	int				m_ChaoDropRatio;		// 카오 드랍률
	int				m_EquipDropRatio;		// 장비 드랍률(장비/인벤 선택)

	DWORD			m_PreChaoTime;			// 프리카오 유지시간(초단위)
	DWORD			m_ChaoTime;				// 카오 유지시간
	DWORD			m_AddChaoTime;			// 카오 추가시간

	DWORD			m_BoradoriAttackTime;	// 부정공격자 보라돌이 적용시간
	DWORD			m_BoradoriNormalTime;	// 정당공격자 보라돌이 적용시간

    BYTE            m_IgnorePKTile;
	WORD			m_wEND_ROW_KEY;			// 끝을 표시하는 파라미터
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