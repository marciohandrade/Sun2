#include "SunClientPrecompiledHeader.h"
#include "StatusEffect.h"
#include "StateInfoParser.h"
#include "Character.h"
#include "GlobalFunc.h"



SKILL_EFFECT::SKILL_EFFECT()
{
    hEffect = INVALID_HANDLE_VALUE;

    effect_position_bone_index = 0;
    effect_position_offset = c_ZeroVector;
    bApplyParentRotate = false;
    bApplyParentScale = false;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    overlap_status_count_ = 1;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

SKILL_EFFECT::~SKILL_EFFECT()
{
    if (hEffect != INVALID_HANDLE_VALUE)
    {
        g_pEffectManager->RemoveEffectPtr(hEffect);
        hEffect = INVALID_HANDLE_VALUE;
    }
}

SKILL_EFFECT* SKILL_EFFECT::CreateStatusEffect( DWORD status_code, DWORD remain_time )
{
    BASE_STATEINFO* status_info = StateInfoParser::Instance()->GetStateInfo( status_code );
    if( status_info == NULL )
        return NULL;

    SKILL_EFFECT* instance = new SKILL_EFFECT();
    instance->dwSkillID = 0;
    instance->dwStatusID = status_code;
    instance->iRemainTime = remain_time;
    instance->status_info = status_info;

    //
    instance->effect_position_type = (eSKILLEFFECT_POS)status_info->m_byEffectPos;					
    instance->effect_position_bone_index = status_info->m_wBoneIndex;
    instance->effect_position_offset.x = status_info->m_fOffsetX;
    instance->effect_position_offset.y = status_info->m_fOffsetY;
    instance->effect_position_offset.z = status_info->m_fOffsetZ;
    instance->bApplyParentRotate = status_info->m_bApplyParentRotate;
    instance->bApplyParentScale = status_info->m_bApplyParentScale;
    //
    return instance;
}

SKILL_EFFECT* SKILL_EFFECT::CreateSkillStatusEffect( DWORD status_code, DWORD skill_code, DWORD remain_time, BASE_ABILITYINFO* ability_info )
{
    if( status_code == 0 || ability_info == NULL || skill_code == 0 )
        return NULL;

    BASE_STATEINFO* status_info = StateInfoParser::Instance()->GetStateInfo( status_code );
    if( status_info == NULL )
        return NULL;

    SKILL_EFFECT* instance = new SKILL_EFFECT();

    instance->dwSkillID = skill_code;
    instance->dwStatusID = status_code;
    instance->iRemainTime = remain_time;
    instance->status_info = status_info;

    instance->AbilityInfo = *ability_info;    // copy
    instance->dwAbilityID = (eABILITY)ability_info->m_wAbilityID;      // �̰� ���ϴ��� ��
    instance->fEffectHeight = 1.0f;

    //
    instance->effect_position_type = (eSKILLEFFECT_POS)status_info->m_byEffectPos;					
    instance->effect_position_bone_index = status_info->m_wBoneIndex;
    instance->effect_position_offset.x = status_info->m_fOffsetX;
    instance->effect_position_offset.y = status_info->m_fOffsetY;
    instance->effect_position_offset.z = status_info->m_fOffsetZ;
    instance->bApplyParentRotate = status_info->m_bApplyParentRotate;
    instance->bApplyParentScale = status_info->m_bApplyParentScale;

    //instance->wGroupId = status_info->m_wGroupCode;
    //instance->m_byIconKind = status_info->m_byIconKind;
    //instance->m_bDelType = status_info->m_bDelType;

    return instance;
}

void SKILL_EFFECT::UpdateEffect( Character* character )
{
    if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hEffect) )
    {
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
        if (bApplyParentRotate == false)
        {
            WzVector position = character->GetSkillEffectPos( effect_position_type, effect_position_bone_index ) + effect_position_offset;
            WzVector rotate = c_ZeroVector;
            WzVector scale = c_UnitVector;

            if( bApplyParentScale == true )
            {
                scale = character->GetScale();
            }

            pBody->SetResultMatrix( position, rotate, scale );

        }
        else //(bApplyParentRotate == true)
        {
            WzMatrix parent_matrix = character->GetBoneMatrix(5);   // �峪�� 5���� ���� ��
            WzMatrix move_scale_matrix;
            MatrixIdentity(&move_scale_matrix);

            move_scale_matrix._41 = effect_position_offset.x;
            move_scale_matrix._42 = effect_position_offset.y;
            move_scale_matrix._43 = effect_position_offset.z;

            if ((dwStatusID == 315) && (character->GetGender() != GenderType::kMale))
            {
                // ����Ʈ�� ������ ����ϰ� ��ų�� State�� �ϳ��� �����ϱ� ������, ���� ó���� �ȵǾ� ����ó��
                move_scale_matrix._11 *= 0.8f;
                move_scale_matrix._22 *= 0.8f;
                move_scale_matrix._33 *= 0.8f;
            }

            pBody->SetResultMatrix(&parent_matrix, NULL, &move_scale_matrix);
        }
#else
        WzVector position = character->GetSkillEffectPos( effect_position_type, effect_position_bone_index ) + effect_position_offset;
        WzVector rotate = c_ZeroVector;
        WzVector scale = c_UnitVector;

        if( bApplyParentRotate == true )
        {
            rotate.y = 0.0f;
            rotate.z = character->GetAngle(); 
            rotate.x = character->GetAngleX();
        }

        if( bApplyParentScale == true )
        {
            scale = character->GetScale();
        }

        pBody->SetResultMatrix( position, rotate, scale );
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    }
}


void SKILL_EFFECT::CreateEffect( Character* character, DWORD target_object_key )
{
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    if (bApplyParentRotate == false)
    {
        WzVector position = character->GetSkillEffectPos( effect_position_type, effect_position_bone_index ) + effect_position_offset;
        WzVector rotate = c_ZeroVector;
        WzVector scale = c_UnitVector;

        if( bApplyParentScale == true )
        {
            scale = character->GetScale();
        }

        hEffect = GlobalFunc::CreateEffectBeUpToOption( GetSkillEffectID(), position, scale, rotate, target_object_key, character->GetObjectKey() );
    }
    else //(bApplyParentRotate == true)
    {
        CWzUnitDraw* base_part = character->GetBasePart();

        WzVector scale_vector = base_part->GetScale();

        if ((dwStatusID == 315) && (character->GetGender() != GenderType::kMale))
        {
            scale_vector *= 0.8f;
        }

        hEffect = GlobalFunc::CreateEffectBeUpToOption(
            GetSkillEffectID(),
            base_part->GetPosition(),
            base_part->GetScale(),
            base_part->GetRotation(),
            target_object_key,
            character->GetObjectKey());
    }
#else
    WzVector position = character->GetSkillEffectPos( effect_position_type, effect_position_bone_index ) + effect_position_offset;
    WzVector rotate = c_ZeroVector;
    WzVector scale = c_UnitVector;

    if( bApplyParentRotate == true )
    {
        rotate.y = 0.0f;
        rotate.z = character->GetAngle(); 
        rotate.x = character->GetAngleX();
    }

    if( bApplyParentScale == true )
    {
        scale = character->GetScale();
    }

    //WzVector rotate = character->GetSkillEffectRotate(this);
    //DWORD object_key = character->GetObjectKey();

    hEffect = GlobalFunc::CreateEffectBeUpToOption( GetSkillEffectID(), position, scale, rotate, target_object_key, character->GetObjectKey() );
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
}


BOOL SKILL_EFFECT::Process(Object *pObject,DWORD dwTick)
{
	iRemainTime -= dwTick;

	if (iRemainTime < 0)
	{
		iRemainTime = 0;

		return TRUE;
	}
	return FALSE;
}
