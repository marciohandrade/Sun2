#pragma once
#include "Const.h"
#include "CharacterDef.h"

namespace ResourceCode
{
    // Ŭ��������
    int GetNumberOfClass();

    // ��������
    int GetNumberOfParts();

    // �� Ŭ������ �⺻ ����
    GenderType::Value GetBaseGenderOfClass(eCHAR_TYPE class_type);

    // �� Ŭ����/������ ���� ����Ŭ����(*)
    eCHAR_TYPE GetImageClassType(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // 
    eCHAR_TYPE GetSecondSkinClassType(eCHAR_TYPE base_class);

    // �⺻ ���̽� ���ҽ��ڵ�
    DWORD GetBaseSkinOfClass(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // �� Ŭ����/���� �⺻���� ���ҽ��ڵ�
    DWORD GetBasePartCodeOfClass(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // Ÿ���ѻ�� �� ���ҽ��ڵ�
    DWORD GetPartCodeForTemple(eCHAR_TYPE class_type, GenderType::Value class_gender, BodyPart body_part);
    
    // Ÿ���ѻ�� ���� ���ҽ��ڵ�
    DWORD GetWeaponCodeForTemple(eWEAPONTYPE weapon_type);

    // ���� ��ȿ�� üũ �� ��ȿ���� ���� ������ �⺻ ������ ����
    GenderType::Value CheckValidGender(eCHAR_TYPE class_type, GenderType::Value class_gender);

    // �����۽�Ų�ڵ���
    DWORD GetItemSkinImageCodeByGender(BASE_ITEMINFO* item_info, GenderType::Value class_gender);
    
    // ĳ���ͻ��� - �ó׸�ƽī�޶� ���ҽ� �ڵ�
    DWORD GetCinematicCameraInCharacterScene();
}