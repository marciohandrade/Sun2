#ifndef __APPEARANCE_MANAGER_H__
#define __APPEARANCE_MANAGER_H__
//------------------------------------------------------------------------------
/**
������Ʈ Ÿ�Կ� ���� �������� ���� 
@author

@since

@remarks
-2005.06.07 �ҽ� ����(����)
- 
*/
#pragma once
#include "Singleton.h"
#include "CharacterDef.h"
class Object;


struct BASE_NPCINFO;

#define APPEARANCEMGR	AppearanceManager::Instance()

typedef struct _EQUIPAPPEARINFO
{
    DWORD dwItemID;
    BOOL  bDivine;
    int   iEnchant;

} EQUIPAPPEARINFO;

struct APPEARANCEINFO
{
    enum eAPPEARKIND
    {
        PLAYER,
        MONSTER,
        PET,
        SKYBOX,
        ITEM,
        MONEY,
        MAPOBJECT,
        CAMERA,
        PLAYERTRANSFORM,
        POLOMORPH,
    };
    eAPPEARKIND ObjectType;

    union
    {
        struct
        {				//PLAYER
            DWORD Job;	// equip�̶� ��Ÿ ������ ��ƾ� �Ұ��̴�.
        };

        struct
        {				//PLAYERTRANSFORM
            DWORD resourceCode;	
        };

        struct 
        {				//MONSTER
            BASE_NPCINFO* pMonsterInfo;
        };

        // TODO // ���߿� Ưȭ�� ������ �ִٸ� �־����.

        struct
        {				//SKY
            DWORD SkyBoxNum;
        };

        struct 
        {				// ITEM
            DWORD ItemCode;			
            DWORD dwMoney;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            BYTE is_dice_item;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
        };

        struct 
        {				// MAPOBJECT
            DWORD		MapObjectID;			// Unit ID
            DWORD		dwAttr;					// �Ӽ��� �޾Ƴִ´�.
            TCHAR		szUnitFile[MAX_PATH];
            WzVector	wzPos;
            WzVector	wzRot;
            WzVector	wzScale;
            WzColor		wzColor;				// �����÷�
            WzBoundingVolume wzBV;
            WzID		wiUserID;	//ä������ ID (�̸����� �ʿ�)
        };
    };

    // ���
    EQUIPAPPEARINFO	Equipment[BP_COUNT];

    // �ؽ��� ����
    float texture_rate;

};

class AppearanceManager : public util::Singleton<AppearanceManager>
{
public:
    const static int kAppearanceAttachIndex_Count = 5;
    const static int kAppearanceAttachIndex_RightHand = 0;
    const static int kAppearanceAttachIndex_LeftHand = 1;
    const static int kAppearanceAttachIndex_Accessory1 = 2;
    const static int kAppearanceAttachIndex_Accessory2 = 3;
    const static int kAppearanceAttachIndex_Accessory3 = 4;
    //const static int kAppearanceAttachIndex_Fullset = 5;

	AppearanceManager(void);
	~AppearanceManager(void);

    void LoadPlayerItemAppearInfo();

	void SetAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject, BOOL bRefreshBone, BOOL bMakeBoneOnly);
	void SetPlayerAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject, BOOL bRefreshBone, BOOL bMakeBoneOnly);
	void SetMonsterAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject, BOOL bRefreshBone = FALSE );
    void SetPetAppearance(APPEARANCEINFO* appear_ptr, Object* object_ptr);

	//void SetSkyBoxAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject, BOOL bRefreshBone = FALSE );
	void SetItemAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject);
	//void SetItemMoneyAppearance( APPEARANCEINFO * pAppearInfo, Object *pObject, BOOL bRefreshBone = FALSE );
	void SetMapObjectAppearance( APPEARANCEINFO * pAppearInfo, Object * pObject, BOOL bRefreshBone = FALSE );
	void SetPlayerTransFormAppearance(APPEARANCEINFO * pAppearInfo,Object * pObject, BOOL bRefreshBone = FALSE);
	void SetCameraObjectAppearance(APPEARANCEINFO * pAppearInfo,Object *pObject, BOOL bRefreshBone = FALSE );
    void SetPolymorphApperance( Object *pObject, BOOL bRefreshBone = FALSE );


    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier:
    //! Parameter: OUT WzMatrix & mx_pos
    //! Parameter: DWORD item_code : �ٿ��� ���� ������ �ڵ�
    //------------------------------------------------------------------------------
    bool GetAccessoryPosition(OUT WzMatrix& mx_pos, DWORD item_code);
    bool GetAccessoryPosition(OUT WzVector& pos, DWORD item_code);
    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier:
    //! Parameter: OUT WzMatrix & mx_rot
    //! Parameter: DWORD item_code : ���� �ƾ��� �ڵ�
    //------------------------------------------------------------------------------
    bool GetAccessoryRotation(OUT WzMatrix& mx_rot, DWORD item_code);

private:
    // about player
    void _SetPlayerBodyAppearance(APPEARANCEINFO* pAppearInfo, Object * pObject);

    void _SetPlayerWeaponAppearance(APPEARANCEINFO* pAppearInfo, Object * pObject);

    void _SetPlayerSpecialAccessoryAppearance(APPEARANCEINFO* pAppearInfo, Object * pObject);

    bool InvalidAccessory(DWORD ItemCode);

    void ApplySpecialAccessoryAppearance(APPEARANCEINFO* appear_info, Character* character_ptr);


private:
    void _SetEquipItemPart( Object* pObject, const EQUIPAPPEARINFO& equip_info, BodyPart parts_index, float texture_rate, GenderType::Value Gender );
    void _SetCustumBodyPart( Object* pObject, DWORD CustomCode, float texture_rate  );
    void _SetDefaultPart( Object* pObject, eCHAR_TYPE Job, BodyPart PartIndex, float texture_rate, GenderType::Value Gender );
};
#endif

