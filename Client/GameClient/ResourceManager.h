#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//=============================================================================================================================
///  ResourceManager
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 28
	@remarks
		- 아이템 유닛과 맵정보를 가지고 있는 리소스 메니져
  
    @note
		- CWorldBase 객체를 사용하려면 GetMapInfo(mapid)로 MapInfo 객체를 가지고 온다.
		- 발사체와 같이 게임 실행 시간에 계속 생성 삭제가 되는 객체에 대한 리소스는 게임 종료시에만 unit을 해제 한다.
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
	
	// 아이템장착시 스킨 이미지 코드를 얻어옵니다.
	RESOURCECODE        GetItemSkinResourceCode( CODETYPE itemcode );
	
	// 아이템이 슬롯에 들어갔을때 이미지 코드를 얻어옵니다.	
	RESOURCECODE        GetItemSlotImageResourceCode(CODETYPE itemcode, BOOL bIsLimited = FALSE); 

	// 스킬 아이콘 코드를 리턴한다.
	RESOURCECODE		GetItemSkillSlotImageResourceCode(CODETYPE skillcode); 

	// 스킬로 부터 변신 리소스 코드를 로드 한다.
	RESOURCECODE		GetSkillTransFormImageResourceCode(CODETYPE skillcode); 

	RESOURCECODE        GetNPCSkinResourceCode(CODETYPE itemcode);  // NPC의 스킨 코드를
	RESOURCECODE        GetTempResourceCode(CODETYPE itemcode);

    // _NA_002050_20110216_ADD_GENDER(성별에 따른 처리 추가)
    // 아이템장착시 스킨 이미지 코드를 얻어옵니다.
    RESOURCECODE GetItemSkinResourceCode(CODETYPE item_code, 
        eCHAR_TYPE class_type, 
        int body_part, 
        GenderType::Value class_gender);
    RESOURCECODE GetItemSkinResourceCode(CODETYPE item_code, GenderType::Value class_gender);

    // 캐릭터 뼈대 리소스 코드를 얻어옵니다.
    RESOURCECODE GetCharacterBoneResourceCode(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 캐릭터 기본 스킨 코드를 얻어옵니다.
    RESOURCECODE GetDefaultSkinCode(eCHAR_TYPE class_type, BodyPart body_part, GenderType::Value class_gender);
    // 헤어 및 페이스 스킨코드를 얻어옵니다.
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