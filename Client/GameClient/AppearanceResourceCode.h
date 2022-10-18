#pragma once
#include "Const.h"
#include "CharacterDef.h"

namespace ResourceCode
{
    // 클래스갯수
    int GetNumberOfClass();

    // 파츠갯수
    int GetNumberOfParts();

    // 각 클래스의 기본 성별
    GenderType::Value GetBaseGenderOfClass(eCHAR_TYPE class_type);

    // 각 클래스/성별에 대한 외형클래스(*)
    eCHAR_TYPE GetImageClassType(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 
    eCHAR_TYPE GetSecondSkinClassType(eCHAR_TYPE base_class);

    // 기본 베이스 리소스코드
    DWORD GetBaseSkinOfClass(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 각 클래스/성별 기본파츠 리소스코드
    DWORD GetBasePartCodeOfClass(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 타락한사원 방어구 리소스코드
    DWORD GetPartCodeForTemple(eCHAR_TYPE class_type, GenderType::Value class_gender, BodyPart body_part);
    
    // 타락한사원 무기 리소스코드
    DWORD GetWeaponCodeForTemple(eWEAPONTYPE weapon_type);

    // 성별 유효성 체크 및 유효하지 않은 성별은 기본 성별을 리턴
    GenderType::Value CheckValidGender(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 아이템스킨코드얻기
    DWORD GetItemSkinImageCodeByGender(BASE_ITEMINFO* item_info, GenderType::Value class_gender);
    
    // 캐릭터생성 - 시네마틱카메라 리소스 코드
    DWORD GetCinematicCameraInCharacterScene();
}