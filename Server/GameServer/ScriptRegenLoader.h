#pragma once

#include <ObjKeyGenerator.h>

class FieldInfo;
class UnitRegenInfoManager;
class UnitGroupInfoManager;

class ScriptRegenLoader
{
public:
	ScriptRegenLoader();
	~ScriptRegenLoader() {}

	VOID			Load( FieldInfo *pFieldInfo, MAPCODE MapCode, FIELDCODE FieldCode, 
						UnitRegenInfoManager * pUnitRegenInfoMGR, UnitGroupInfoManager * pGroupInfoMGR );

private:
	VOID			ExecuteGroupRegen( REGEN_INFO *pRegenInfo, UnitRegenInfoManager * pUnitRegenInfoMGR, UnitGroupInfoManager * pGroupInfoMGR );
	UnitRegenInfo * CreateSingleRegen( FieldInfo *pFieldInfo, REGEN_INFO *pRegenInfo, WORD NPCCode );
	BOOL			SetStartPos( FieldInfo *pFieldInfo, WzSpecialArea *pArea, UnitRegenInfo *pUnitRegenInfo );

private:
	util::CObjKeyGenerator<>	m_RegenKeyGenerater;
};


extern ScriptRegenLoader g_ScriptRegenLoader;

















































































