#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//=============================================================================================================================
///  ResourceManager
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 28
	@remarks
		- ������ ���ְ� �������� ������ �ִ� ���ҽ� �޴���
  
    @note
		- CWorldBase ��ü�� ����Ϸ��� GetMapInfo(mapid)�� MapInfo ��ü�� ������ �´�.
		- �߻�ü�� ���� ���� ���� �ð��� ��� ���� ������ �Ǵ� ��ü�� ���� ���ҽ��� ���� ����ÿ��� unit�� ���� �Ѵ�.
*/
//=============================================================================================================================


#pragma once

#include <Singleton.h>
#include "CharacterDef.h"
#include "Narray.h"


#define RESOURCE_TEMP          0x80000000

#define RESOURCE_MASK          0x00FFFFFF

extern const DWORD ENGINE_RESOURCE_ID_OFFSET;
extern const DWORD SCRIPT_RESOURCE_ID_OFFSET;


class CDrawBase;
class ResourceCacher;
class BattleRoomEntry;

class ResourceManager : public util::Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();
	VOID				Init();
	VOID				Release();

	void                Process(DWORD dwTick);
	
	// ������������ ��Ų �̹��� �ڵ带 ���ɴϴ�.
	RESOURCECODE        GetItemSkinResourceCode( CODETYPE itemcode );
	
	// �������� ���Կ� ������ �̹��� �ڵ带 ���ɴϴ�.	
	RESOURCECODE        GetItemSlotImageResourceCode(CODETYPE itemcode, BOOL bIsLimited = FALSE); 

	// ��ų ������ �ڵ带 �����Ѵ�.
	RESOURCECODE		GetItemSkillSlotImageResourceCode(CODETYPE skillcode); 

	// ��ų�� ���� ���� ���ҽ� �ڵ带 �ε� �Ѵ�.
	RESOURCECODE		GetSkillTransFormImageResourceCode(CODETYPE skillcode); 

	RESOURCECODE        GetNPCSkinResourceCode(CODETYPE itemcode);  // NPC�� ��Ų �ڵ带
	RESOURCECODE        GetTempResourceCode(CODETYPE itemcode);

    // _NA_002050_20110216_ADD_GENDER(������ ���� ó�� �߰�)
    // ������������ ��Ų �̹��� �ڵ带 ���ɴϴ�.
    RESOURCECODE GetItemSkinResourceCode(CODETYPE item_code, 
        eCHAR_TYPE class_type, 
        int body_part, 
        GenderType::Value class_gender);
    RESOURCECODE GetItemSkinResourceCode(CODETYPE item_code, GenderType::Value class_gender);

    // ĳ���� ���� ���ҽ� �ڵ带 ���ɴϴ�.
    RESOURCECODE GetCharacterBoneResourceCode(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // ĳ���� �⺻ ��Ų �ڵ带 ���ɴϴ�.
    RESOURCECODE GetDefaultSkinCode(eCHAR_TYPE class_type, BodyPart body_part, GenderType::Value class_gender);
    // ��� �� ���̽� ��Ų�ڵ带 ���ɴϴ�.
    RESOURCECODE GetCharacterPartSkinResourceCode(eCHAR_TYPE class_type, 
        BodyPart body_part, 
        int variation_code, 
        int variation_set, 
        GenderType::Value class_gender);
    float GetWeaponScale(eCHAR_TYPE class_type, GenderType::Value class_gender);

	void                IncreaseResourceRefCount(RESOURCECODE code);

	VOID				PreLoadResource( RESOURCECODE code, float fRequestRateResizeTexture = -1.0f);

	CWzUnitDraw *	    AllocResourceUnitDraw( RESOURCECODE code, float fRequestRateResizeTexture = -1.0f, bool bFreeAll = true, bool is_npc = false);
	CWzUnit *			AllocResourceUnit( RESOURCECODE code );
	VOID				FreeResourceUnit( RESOURCECODE code);
	VOID				FreeResourceUnitDraw( CWzUnitDraw * pUnitDraw );

    VOID                FreeResourceUnitDrawVer2( CWzUnitDraw * pUnitDraw, bool bForceRemove=false );

	void                FreeAllCachedData();
	void				FreeMostData();
	void                LastFree();

	int                 GetLoadedResourceCount();
	int                 GetWaitForFreeResourceCount();
	

private:	
	ResourceCacher *		m_pResourceCacher;

};

#endif // __RESOURCE_MANAGER_H__