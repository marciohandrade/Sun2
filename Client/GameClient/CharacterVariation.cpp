#include "SunClientPrecompiledHeader.h"

#include "CharacterVariation.h"
#include "Camera.h"


////////////////////////////////////////////////////////////////////////////////////////////////
cCharacterBaseData::cCharacterBaseData()
{
    m_Gender = GenderType::kAll;
    m_Class = eCHAR_TYPE_MAX;

    SetHeightVariation(2);

    SetHideHelmet(false);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    SetHideWing(false);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    SetHideCostume(false);
#endif

    for( int i=0;i!=PLAYER_VARIATION_MAX;++i)
    {
        m_PlayerVariation[i] = 1;
    }
}

void cCharacterBaseData::SetHeightVariation(int variation)
{
    if (variation >= MAX_HEIGHT_VARIATION)
    {
        variation = (MAX_HEIGHT_VARIATION - 1);
    }
    else if (variation < 0)
    {
        variation = 0;
    }
    m_HeightVariation = variation;
}

int cCharacterBaseData::GetPartVariation(int part_index)
{
    if (part_index < 0 || part_index >= PLAYER_VARIATION_MAX)
    {
        return 1;
    }
    return m_PlayerVariation[part_index];
}

void cCharacterBaseData::SetPartVariation(int part_index, int variation)
{
    if (part_index < 0 || part_index >= PLAYER_VARIATION_MAX)
    {
        return;
    }
    m_PlayerVariation[part_index] = variation;
}


#include "SCItemSlot.h"
#include "SkillInfoParser.h"
#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "ItemInfoParser.h"

////////////////////////////////////////////////////////////////////////////////////////////////
cPlayerItemEffect::cPlayerItemEffect()
{
    for( int i=eITEM_EFFECT_TYPE_MIN;i<eITEM_EFFECT_TYPE_MAX;++i)
    {
        ClearItemEffectParam(i);
    }
}

void cPlayerItemEffect::Initialize()
{

}

void cPlayerItemEffect::Destroy()
{
    for (int a = 0; a < eITEM_EFFECT_TYPE_MAX; a++)
    {
        ITEM_EFFECT_PARAM effect_param;
        GetItemEffectParam(a, effect_param);
        if (effect_param.hEffect != INVALID_HANDLE_VALUE)
        {
            g_pEffectManager->RemoveEffectPtr(effect_param.hEffect);
        }
    }
    ClearItemEffectParam();
}

void cPlayerItemEffect::GetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param)
{
    if (index < 0 || index >= eITEM_EFFECT_TYPE_MAX)
    {
        return;
    }
    effect_param = m_ItemEffectParameter[index];
}

void cPlayerItemEffect::SetItemEffectParam(int index, ITEM_EFFECT_PARAM& effect_param)
{
    if (index < 0 || index >= eITEM_EFFECT_TYPE_MAX)
    {
        return;
    }
    m_ItemEffectParameter[index] = effect_param;
}
void cPlayerItemEffect::ClearItemEffectParam()
{
    for (int index = 0; index < eITEM_EFFECT_TYPE_MAX; ++index)
    {
        ClearItemEffectParam(index);
    }
}

void cPlayerItemEffect::ClearItemEffectParam(int index)
{
    if (index < 0 || index >= eITEM_EFFECT_TYPE_MAX)
    {
        return;
    }
    memset(&(m_ItemEffectParameter[index].vScale), 0, sizeof(WzVector));
    m_ItemEffectParameter[index].hEffect = INVALID_HANDLE_VALUE;
    m_ItemEffectParameter[index].bPositionType = eSKILLEFFECT_POS_NONE;
    m_ItemEffectParameter[index].bRotateWithBone = FALSE;
    m_ItemEffectParameter[index].dwItemCode = 0;
}

void cPlayerItemEffect::AddItemEffect( Character* pCharacter, const BASE_ITEMINFO* pItemInfo )
{
    // 1. ������ ������ ������ (���������� ����Ʈ�� �߻��ϴ� ���̽�)
    if( SCItemSlot::CanEquip(pItemInfo) )
    {
        if( pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LRHAND ) //����� �μ����� �ٲ� �ش�.
        {
            AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_RHAND_EQUIP );
            AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_LHAND_EQUIP );
        }
        else if(pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LHAND)
        {
            AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_LHAND_EQUIP );
        }	
        else 
        {
            AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_RHAND_EQUIP );
        }	
    }

    // 2. ������ �������� (���۰� ȶ���� ����)
    if( pItemInfo->m_wType != eITEMTYPE_TOTEM && pItemInfo->m_wType != eITEMTYPE_TORCH )
    {
        // ��ų �ڵ尡 �ִ°����� ������������ �ĺ��Ѵ�
        // ���������� ���� ����Ʈ�̸�, ������ ��� �÷��̾ �� ����Ʈ�ڵ��� �������־�� �Ѵ�.
        if( SkillInfoParser::Instance()->GetSkillInfo(pItemInfo->m_wSkillCode) != NULL )
        {
            if (pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LRHAND)//����� �μ����� �ٲ� �ش�.
            {
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_RHAND_WASTE );
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_LHAND_WASTE );
            }
            else if(pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LHAND)
            {
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_LHAND_WASTE );
            }	
            else //�������� ������ �������� ����.
            {
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_RHAND_WASTE );
            }

        }
        else
        {
            // ������ ��� ����Ʈ (����Ʈ�� ����ϰ� �ڵ������� ���ŵǹǷ� Ư���� �ڵ��� �������� �ʿ䰡 ���� ���.. ���� ���� ����Ʈ�� �ȵȴ�)
            CreateWasteItemEffect( pCharacter, pItemInfo->m_Code );
        }
    }
}

void cPlayerItemEffect::AddItemEffect( Character* pCharacter, const BASE_ITEMINFO* pItemInfo, eITEM_EFFECT_TYPE EffectType )
{
    if( EffectType < eITEM_EFFECT_TYPE_MIN || EffectType >= eITEM_EFFECT_TYPE_MAX )
    {
        return;
    }

    if( pItemInfo == NULL )
    {
        return;
    }

    if( pItemInfo->m_dwEffectCode == StrToWzID("Null") )
    {
        return;
    }

    RemoveItemEffect( EffectType );

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (GlobalFunc::IsSpaScene())
    {
        return;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

    ITEM_EFFECT_PARAM EffectParam;
    ZeroMemory(&EffectParam,sizeof(EffectParam));

    EffectParam.dwItemCode = pItemInfo->m_Code;
    EffectParam.bRotateWithBone = pItemInfo->m_bEfffectBoneRot;

    switch( EffectType )
    {
    case eITEM_EFFECT_TYPE_INVENTORY:
    case eITEM_EFFECT_TYPE_BADGE:
        EffectParam.bPositionType = eSKILLEFFECT_POS_HEAD;
        EffectParam.iBoneIndex = -1;
        break;

    case eITEM_EFFECT_TYPE_LHAND_EQUIP:
    case eITEM_EFFECT_TYPE_LHAND_WASTE:
        EffectParam.bPositionType = eSKILLEFFECT_POS_LHAND;
        EffectParam.iBoneIndex = pCharacter->GetBoneIndex( eSKILLEFFECT_POS_LHAND );
        break;

    case eITEM_EFFECT_TYPE_RHAND_EQUIP:
    case eITEM_EFFECT_TYPE_RHAND_WASTE:
        EffectParam.bPositionType = eSKILLEFFECT_POS_RHAND;
        EffectParam.iBoneIndex = pCharacter->GetBoneIndex( eSKILLEFFECT_POS_RHAND );
        break;
    }


    float EffectScale = 1.0f;

    if( pItemInfo->m_Code == 9116)
    {
        //!< �߱� vvip : Ŭ���� ���� ����� Ʋ���� �� �޶�� ��û
        eCHAR_TYPE character_class = g_pHero->GetImageClass();
        switch (character_class)
        {
        case eCHAR_BERSERKER:
            EffectScale = 1.3f;
            break;
        case eCHAR_DRAGON:
            EffectScale = 1.1f;
            break;
        default:
            //eCHAR_ELEMENTALIST, eCHAR_VALKYRIE, eCHAR_SHADOW, eCHAR_MYSTIC
            EffectScale = 1.05f;
            break;
        }
    }

    EffectParam.vScale.x = EffectScale;
    EffectParam.vScale.y = EffectScale;
    EffectParam.vScale.z = EffectScale;


    // �����ɶ� ����Ʈ�� �ʱ���ġ�� ����������, UpdateItemEffectPos���� ��ġ�� �������
    WzVector wvPosition = pCharacter->GetSkillEffectPos(EffectParam.bPositionType);

    EffectParam.hEffect = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, wvPosition,  EffectParam.vScale, c_ZeroVector, pCharacter->GetObjectKey());

    SetItemEffectParam(EffectType, EffectParam );
}

void cPlayerItemEffect::RefreshRenderEffectItem( Character* pCharacter, SLOTCODE* ItemCodes, int ItemCount)
{
    if ((ItemCount == 0)
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        || GlobalFunc::IsSpaScene()
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        RemoveItemEffect( eITEM_EFFECT_TYPE_INVENTORY );
        RemoveItemEffect( eITEM_EFFECT_TYPE_BADGE );

        return;
    }

    for(int i = 0; i < ItemCount; i++)
    {
        if( BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCodes[i]) )
        {
            if (pItemInfo->m_wType != eITEMTYPE_RENDER_EFFECT_ITEM)
            {
                continue;
            }

            if( pItemInfo->IsExistInvenOptions() )
            {
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_INVENTORY );
            }
            else //if( pItemInfo->IsExistEquipOptions() ) // 2013.06.11 / ������ / �� �ּ��� ������ �ؿ�...
            {
                // �ɼǹߵ��������� �κ��丮����Ʈ���� ��������Ʈ���� ����������
                // �ɼ��� ���� ���� �����ۿ��� �ɼǹߵ����� ��ü�� ��� ����Ʈ�� ��µ��� �ʴ� ������ �־���.
                // ���� �������� ������ �÷��� �߰����� �ʴ� �̻�, ����δ� �κ��丮����Ʈ �������� �ƴϸ� ���� ����Ʈ�� ������.
                // ��� �Ϲ� �����۰� �����ϱ� ���� if (pItemInfo->m_wType != eITEMTYPE_RENDER_EFFECT_ITEM) �߰�
                AddItemEffect( pCharacter, pItemInfo, eITEM_EFFECT_TYPE_BADGE );
            }
        }
    }
}

BOOL cPlayerItemEffect::CreateWasteItemEffect( Character* pCharacter, DWORD ItemCode)
{
    // ������ ��뿡 ���� ��ȸ�� ����Ʈ�� �����ϴ� �Լ�
    // ��������Ʈ�� �� �ڵ��� ������ ��ü�� �����Ƿ� ���⿡�� �ٷ������ �ʴ´�

    BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(ItemCode);

    if( pItemInfo == NULL )
    {
        return FALSE;
    }

    if (pItemInfo->m_dwEffectCode == Null_Value)
    {
        return FALSE;
    }

    if( pItemInfo->m_wType == eITEMTYPE_FIRECRACKER ||
        pItemInfo->m_wType == eITEMTYPE_POLLEN ||
        pItemInfo->m_wType == eITEMTYPE_TORCH ||
        pItemInfo->m_wType == eITEMTYPE_MINE )
    {
        return TRUE;
    }


    CEffectBody* pBody = NULL;

    // ������ ������ Ÿ���� �ִ� �������� ��� �ڵ� ó�� 
    if( pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LRHAND )
    {
        // ��տ��� 2�� �����Ѵ�

        WzVector vPos = pCharacter->GetSkillEffectPos(eSKILLEFFECT_POS_RHAND );
        HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, pCharacter->GetObjectKey());

        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
        pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
        if (pBody && pBody->IsLoop())
        {
            g_pEffectManager->RemoveEffectPtr(hHandle);
        }

        vPos = pCharacter->GetSkillEffectPos(eSKILLEFFECT_POS_LHAND );
        hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, pCharacter->GetObjectKey());

        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
        pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
        if (pBody && pBody->IsLoop())
        {
            g_pEffectManager->RemoveEffectPtr(hHandle);
        }
    }
    else
    {
        WzVector vPos = pCharacter->GetSkillEffectPos( (eSKILLEFFECT_POS)pItemInfo->m_byEffectPosition );
        HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, pCharacter->GetObjectKey());

        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
        pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
        if (pBody && pBody->IsLoop())
        {
            g_pEffectManager->RemoveEffectPtr(hHandle);
        }
    }	

    return TRUE;
}

void cPlayerItemEffect::UpdateItemEffectPos( Character* pCharacter )
{
    if (GlobalFunc::IsSSQScene()
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        || GlobalFunc::IsSpaScene()
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        return;
    }

    WzColor color = pCharacter->GetColor();

    for (int i = 0; i < eITEM_EFFECT_TYPE_MAX; i++)
    {
        ITEM_EFFECT_PARAM effect_param;
        GetItemEffectParam(i, effect_param);

        if( effect_param.hEffect == INVALID_HANDLE_VALUE)
            continue;

        effect_param.iBoneIndex = pCharacter->GetBoneIndex(effect_param.bPositionType);

        CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(effect_param.hEffect);

        WzVector wvPos, wvWorldPos;
        WzMatrix wmPosMat, wmRotMat, wmScale, mat;

        if (pBody)
        {
            if (pCharacter->GetShow() && (Alpha_WzColor(color) > 0.0f))
            {
                pBody->SetShow(TRUE);
            }
            else
            {
                pBody->SetShow(FALSE);
            }

            wvPos = c_ZeroVector;

            if (effect_param.iBoneIndex == -1)
            {
                wvPos = pCharacter->GetSkillEffectPos(effect_param.bPositionType);
                MatrixIdentity(&mat);
            }
            else
            {
                mat = pCharacter->GetBoneMatrix(effect_param.iBoneIndex);
            }

            VectorTransformCoord(&wvWorldPos, &wvPos, &mat);

            // �Ӹ� ���� �÷��ش�
            if( i == eITEM_EFFECT_TYPE_INVENTORY || i == eITEM_EFFECT_TYPE_BADGE )
            {

#ifdef _CHINA
                if (effect_param.bPositionType == eSKILLEFFECT_POS_HEAD)
                {
                    wvWorldPos = wvWorldPos + g_Camera.GetCameraDirection();
                }
#endif //_CHINA 

#ifdef __CN_ADD_HEAD_EFFECT_FLAG
                //float fOffsetZ = 0.0f;

                if (effect_param.bPositionType == eSKILLEFFECT_POS_HEAD)
                {
                    //�ɼ��÷��װ� �����ִٸ� ����Ʈ �Ⱥ����ش�
                    if (GAMEOPTION->GetShowHeadEffect())
                    {
                        pBody->SetShow(TRUE);
                    }
                    else
                    {
                        pBody->SetShow(FALSE);
                        continue;
                    }
                }
#endif//__CN_ADD_HEAD_EFFECT_FLAG(�߱�:INVEN�Ǽ��縮����Ʈ FLAG�߰�)
                if (effect_param.bPositionType == eSKILLEFFECT_POS_HEAD)
                {
                    switch(pCharacter->GetImageClass())
                    {
                    case eCHAR_BERSERKER:
                    case eCHAR_DRAGON:
                    case eCHAR_SHADOW:
                        {
                            wvWorldPos.z += 0.25f;
                        }
                        break;
                    case eCHAR_VALKYRIE:
                    case eCHAR_ELEMENTALIST:
                        {
                            wvWorldPos.z += 0.20f;
                        }
                        break;
                    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                        {
                            wvWorldPos.z += 0.35f;
                        }
                        break;
                    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                        {
                            wvWorldPos.z += 0.40f;
                        }
                        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    case eCHAR_WITCHBLADE:
                        {
                            wvWorldPos.z += 0.30f;
                        }
                        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    }
                }
#ifdef __CN_ADD_HEAD_EFFECT_FLAG
                //wvWorldPos.z += fOffsetZ;	
#endif//__CN_ADD_HEAD_EFFECT_FLAG	

            }

            MatrixTranslation( &wmPosMat, wvWorldPos.x, wvWorldPos.y, wvWorldPos.z );

            if(effect_param.iBoneIndex!=-1&&effect_param.bRotateWithBone)
            {
                wmRotMat=mat;				
                wmRotMat._41 = 0.0f;
                wmRotMat._42 = 0.0f;
                wmRotMat._43 = 0.0f;
            }
            else 
            {
                MatrixIdentity( &wmRotMat );
                WzVector vPos;
                vPos=wvWorldPos;
                WzVector vDest,vTemp;
                vDest=vPos + pCharacter->GetDirection()*10.0f;
                vDest =  vDest - vPos;
                vTemp = vDest;
                vTemp.z = 0.0f;

                float fLength;
                if (vTemp.x == 0.0f && vTemp.y == 0.0f)
                {
                    fLength = 0.0f;
                }
                else
                {
                    fLength = VectorLength(&vTemp);
                }

                WzVector vRot;
                vRot.x = -atan2(vDest.z,fLength);
                vRot.z = Math_GetAngleFromVector(&vDest);
                vRot.y = 0.0f;
                MatrixRotationZYX(&wmRotMat,&vRot);

            }
            WzVector vScale=c_ZeroVector;
            pCharacter->GetScale(&vScale);
            vScale.x-=1.0f;
            vScale.y-=1.0f;
            vScale.z-=1.0f;

            MatrixScaling( &wmScale, effect_param.vScale.x+vScale.x, effect_param.vScale.y+vScale.y, effect_param.vScale.z +vScale.z);

            pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );
        }

        SetItemEffectParam(i, effect_param);
    }
}

BOOL cPlayerItemEffect::IsExistItemEffect(DWORD ItemCode)
{
    ITEM_EFFECT_PARAM effect_param;
    GetItemEffectParam(eITEM_EFFECT_TYPE_RHAND_WASTE, effect_param);
    if (effect_param.hEffect != INVALID_HANDLE_VALUE)
    {
        return TRUE;
    }

    GetItemEffectParam(eITEM_EFFECT_TYPE_LHAND_WASTE, effect_param);
    if (effect_param.hEffect != INVALID_HANDLE_VALUE)
    {
        return TRUE;
    }
    for (int a = 0; a < eITEM_EFFECT_TYPE_MAX; a++)
    {
        GetItemEffectParam(a, effect_param);
        if(effect_param.dwItemCode == ItemCode)
        {
            return TRUE;
        }
    }
    return FALSE;
}

void cPlayerItemEffect::RemoveItemEffect(eITEM_EFFECT_TYPE EffectType )
{
    ITEM_EFFECT_PARAM CurrentItemEffect;
    int EffectTypeIndex = static_cast<int>(EffectType);
    GetItemEffectParam(EffectTypeIndex, CurrentItemEffect);

    if (CurrentItemEffect.hEffect != INVALID_HANDLE_VALUE)
    {
        g_pEffectManager->RemoveEffectPtr(CurrentItemEffect.hEffect);
        ClearItemEffectParam( EffectTypeIndex);
    }
}

BOOL cPlayerItemEffect::RemoveItemEffectByItemCode( DWORD ItemCode )
{
    for (int i = 0; i < eITEM_EFFECT_TYPE_MAX; i++)
    {
        ITEM_EFFECT_PARAM effect_param;
        GetItemEffectParam(i, effect_param);
        if( effect_param.dwItemCode == ItemCode )
        {
            if( effect_param.hEffect != INVALID_HANDLE_VALUE )
            {
                g_pEffectManager->RemoveEffectPtr(effect_param.hEffect);
                ClearItemEffectParam(i);
                return TRUE;
            }
        }
    }

    return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
cPlayerSetItemEffect::cPlayerSetItemEffect()
{
    m_ShowFullsetEffect = TRUE;

    //m_FullsetEffectAttachData.clear();
    memset(&m_FullsetChangeItemInfo, 0, sizeof(m_FullsetChangeItemInfo));

    for (int index = 0; index < MAX_SETITEM_EFFECT_NUM; ++index)
    {
        m_FullsetEffectInfo[index].m_dwEffectID = INVALID_WZID;
        m_FullsetEffectInfo[index].m_btBoneIdx = 0;
    }
}


void cPlayerSetItemEffect::GetFullsetEffectInfo(int index, sFullSetEffectInfo& effect_info)
{
    if (index < 0 || index >= MAX_SETITEM_EFFECT_NUM)
    {
        return;
    }
    effect_info = m_FullsetEffectInfo[index];
}

void cPlayerSetItemEffect::SetFullsetEffectInfo(int index, const sFullSetEffectInfo& effect_info)
{
    if (index < 0 || index >= MAX_SETITEM_EFFECT_NUM)
    {
        return;
    }
    m_FullsetEffectInfo[index] = effect_info;
}

void cPlayerSetItemEffect::ClearFullsetEffectInfo()
{
    for (int index = 0; index < MAX_SETITEM_EFFECT_NUM; ++index)
    {
        m_FullsetEffectInfo[index].m_dwEffectID = INVALID_WZID;
        m_FullsetEffectInfo[index].m_btBoneIdx = 0;
    }
}


#include "ItemManager.h"
#include "Character.h"
#include "GlobalFunc.h"
#include "EquipmentContainer.h"
#include "ResourceManager.h"
#include "SCItemSlot.h"
#include "GameFunc.h"


void cPlayerSetItemEffect::RefreshFullsetItemInfo( Character* pCharacter )
{
    WORD setitem_optioninfo_setgroupcode = 0;
    eKIND_OF_EQUIPED_SETITEM setitem_kind = CheckKindOfSetItem(pCharacter, setitem_optioninfo_setgroupcode);
    RefreshFullsetEffect( pCharacter, setitem_kind, setitem_optioninfo_setgroupcode);
}

bool cPlayerSetItemEffect::IsSetitemEquipmentSlot( Character* pCharacter, int equipment_slot_index)
{
    switch (equipment_slot_index)
    {
    case eEQUIPCONTAINER_ARMOR:
    case eEQUIPCONTAINER_HELMET:
    case eEQUIPCONTAINER_PANTS:
    case eEQUIPCONTAINER_BOOTS:
    case eEQUIPCONTAINER_GLOVE:
        {
            return true;
        }        
        break;
    case eEQUIPCONTAINER_BELT:
    case eEQUIPCONTAINER_PROTECTOR:
    case eEQUIPCONTAINER_SHIRTS:
        {
            if (pCharacter->IsKindOfObject(PLAYER_OBJECT) != FALSE)
            {
                return true;
            }

            if (pCharacter->IsKindOfObject(NPC_OBJECT) != FALSE)
            {
                return false;
            }
        }
        break;
    }

    return false;
}

//-------------------------------------------------------------------------------------------
/** ��Ʈ �������� ��� ���� �Ͽ�����? ( Ǯ���� �����Ͽ���? )
GS_DESC:���� ���������� �������̿��� �Ƹ������� �ٸ������� ������� �ٸ��ٸ� Ǯ���� �ƴ϶�
��üũ�ϰ��ִµ� �´����� �𸣰���
*/
eKIND_OF_EQUIPED_SETITEM cPlayerSetItemEffect::CheckKindOfSetItem(Character* pCharacter, OUT WORD& setitem_optioninfo_setgroupcode)
{
    EquipmentContainer* pContainer = pCharacter->GetEquipmentContainer();
    if (pContainer == NULL)
    {
        return eKIND_OF_EQUIPED_SETITEM_NONE;
    }

    eITEMSUBTYPE eSubType;
    WORD		 SetCode;
    BOOL bDivine = FALSE;

    SCItemSlot& rItemSlot = (SCItemSlot&)pContainer->GetSlot(eEQUIPCONTAINER_ARMOR);			

    const BASE_ITEMINFO* pItemInfo = rItemSlot.GetImageItemInfo();

    //������ȿ�˻�
    if (rItemSlot.IsContainValidItem() && NULL != pItemInfo )
    {
        eSubType = (eITEMSUBTYPE)pItemInfo->m_wSubType;
        SetCode  = pItemInfo->set_group_code;
    }
    else
    {
        eSubType = eITEMSUBTYPE_GENERAL;
        SetCode  = 0;
    }

    ClearFullsetEffectInfo();

    eKIND_OF_EQUIPED_SETITEM eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_NONE;	

    for ( int i = 0; i < pContainer->GetMaxSlotNum(); ++i )
    {	
        // �̳�鸸 üũ�Ѵ�.
        // �÷��̾�� Monster/NPC �� ������ Ʋ����.
        if (IsSetitemEquipmentSlot(pCharacter, i) == false)
        {
            continue;
        }

        if ( pContainer->IsEmpty(i) )
        {
            return eKIND_OF_EQUIPED_SETITEM_NONE;
        }

        SCItemSlot& rItemSlot = (SCItemSlot&)pContainer->GetSlot(i);

        if( pCharacter->IsMyHero() == TRUE )
        {
            if (pContainer->GetEquipmentManageInfo()->slots[i].activated == false)
            {
                // �������� ��Ȱ��ȭ�� ��� ������
                return eKIND_OF_EQUIPED_SETITEM_NONE;
            }
        }

        const BASE_ITEMINFO* pItemInfo = rItemSlot.GetImageItemInfo();
        if ( !pItemInfo )
        {
            return eKIND_OF_EQUIPED_SETITEM_NONE;
        }

        // ������ ���� �ٸ� ������ üũ�Ѵ�.
        // eEQUIPCONTAINER_ARMOR�� ������ ������ ���Ѵ�.
        if( eSubType != pItemInfo->m_wSubType || SetCode != pItemInfo->set_group_code )
        {
            return eKIND_OF_EQUIPED_SETITEM_NONE;
        }
    }

    //Ǯ������ �����ϰ� �ִٸ�..
    if( 0 != pItemInfo->GetApplySetItemOptionType())
    {
        if( SETITEM_OPTION_INFO* pSetItemOpt = SetItemOptionInfoParser::Instance()->Find(pItemInfo->set_group_code) )
        {
            for(int i = 0; i < MAX_SETITEM_EFFECT_NUM; ++i)
            {
                if( pSetItemOpt->m_dwEffectCode[i] != INVALID_WZID )
                {
                    sFullSetEffectInfo effect_info;
                    effect_info.m_dwEffectID = pSetItemOpt->m_dwEffectCode[i][pCharacter->GetGender()];
                    effect_info.m_btBoneIdx = pSetItemOpt->m_btBoneIdx[i][pCharacter->GetGender()];
                    // ��Ű������ó�� ó���Ѵ�(bysubType)
                    if( pSetItemOpt->m_byItemType == 5 )
                    {
                        eSubType = eITEMSUBTYPE_FATE;
                    }
                    SetFullsetEffectInfo(i, effect_info);
                }
            }
            setitem_optioninfo_setgroupcode = pSetItemOpt->m_wSetGroupCode;
        }
    }

    switch ( eSubType )
    {
    case eITEMSUBTYPE_GENERAL:
        {
            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_GENERAL;
        }
        break;

    case eITEMSUBTYPE_ELITE_SSQ:
    case eITEMSUBTYPE_ELITE:
        {
            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_ELITE;
        }
        break;


    case eITEMSUBTYPE_UNIQUE:
        {
            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_UNIQUE;
        }
        break;

    case eITEMSUBTYPE_FATE:
        {
            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_LUCKY;
        }
        break;

    default:
        eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_NONE;
        break;
    }			

    return eKindofEquipedSet;
}

BOOL cPlayerSetItemEffect::GetAttachEffectForSetItemPos( Character* pCharacter, OUT WzVector& Attach_Position)
{
    int iBoneIndex = -1;

    // ���� ���� ���ؿ´�.	
    switch( pCharacter->GetImageClass() )
    {
    case eCHAR_BERSERKER:       iBoneIndex = 5;
        break;
    case eCHAR_DRAGON:          iBoneIndex = 5;
        break;
    case eCHAR_VALKYRIE:        iBoneIndex = 6;
        break;
    case eCHAR_ELEMENTALIST:    iBoneIndex = 6;	
        break;
    case eCHAR_SHADOW:          iBoneIndex = 6;
        break;
    case eCHAR_MYSTIC:          iBoneIndex = 5;
        break;
    case eCHAR_HELLROID:        iBoneIndex = 5; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        break;
    case eCHAR_WITCHBLADE:      iBoneIndex = 5; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        break;
    default:
        break;
    }

    Attach_Position = pCharacter->GetVisiblePos();
    Attach_Position.z += pCharacter->GetHeight() / 2.0f;

    return GameFunc::GetBonePos( pCharacter->GetBasePart(), iBoneIndex, &Attach_Position);
}

void cPlayerSetItemEffect::ProcessFullsetEffect(Character* pCharacter )
{
    for( size_t i=0;i!=m_FullsetEffectAttachData.size();++i)
    {
        if( m_FullsetEffectAttachData[i].m_hEffectHandle == INVALID_HANDLE_VALUE)
        {
            continue;
        }

        switch (m_FullsetEffectAttachData[i].eSetType)
        {	
        case eKIND_OF_EQUIPED_SETITEM_NONE:
            break;

        case eKIND_OF_EQUIPED_SETITEM_ELITE:
        case eKIND_OF_EQUIPED_SETITEM_UNIQUE:
        case eKIND_OF_EQUIPED_SETITEM_GENERAL:
        case eKIND_OF_EQUIPED_SETITEM_DIVINE:
        case eKIND_OF_EQUIPED_SETITEM_LUCKY:
            {
                if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_FullsetEffectAttachData[i].m_hEffectHandle) )
                {
                    if( m_FullsetEffectAttachData[i].eSetType == eKIND_OF_EQUIPED_SETITEM_LUCKY ||
                        m_FullsetEffectAttachData[i].eSetType == eKIND_OF_EQUIPED_SETITEM_ELITE )
                    {
                        WzMatrix wmPosMat, wmRotMat, wmScale, mat;
                        WzMatrix wmRotTemp, wmRotTemp2, wmRotTemp3, wzRotTemp4;

                        int iBoneIndex = pCharacter->GetBoneIndex(m_FullsetEffectAttachData[i].m_btBoneIdx);
                        mat = pCharacter->GetBoneMatrix(iBoneIndex);

                        //Pos
                        //////////////////////////////////////////////////////////////////////////
                        MatrixTranslation(&wmPosMat, mat._41, mat._42, mat._43);
                        //////////////////////////////////////////////////////////////////////////

                        //Rot
                        //////////////////////////////////////////////////////////////////////////
                        switch (m_FullsetEffectAttachData[i].m_btBoneIdx)
                        {
                        case eSKILLEFFECT_POS_HEAD:
                            {
                                wmRotTemp2     = mat;	
                                wmRotTemp2._41 = 0.0f;
                                wmRotTemp2._42 = 0.0f;
                                wmRotTemp2._43 = 0.0f;

                                
                                MatrixRotationY(&wmRotTemp, WZ_PI/2.f); //Y������ 90�� ������ ���δ�(���� Ȯ��)

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
                                // ����̵��� ��쿡 �Ӹ����� �ٸ� ĳ���Ϳ� ���̰� �־ ȸ������ �ٸ�
                                if( pCharacter->GetImageClass() == eCHAR_HELLROID )
                                {
                                    MatrixRotationZ(&wmRotTemp3, WZ_PI*2.0f); //X������ 90�� ������ ���δ�(���� Ȯ��)
                                }
                                else
                                {
                                    MatrixRotationZ(&wmRotTemp3, WZ_PI*1.5f); //X������ 90�� ������ ���δ�(���� Ȯ��)
                                }
#else //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                                MatrixRotationZ(&wmRotTemp3, WZ_PI*1.5f); //X������ 90�� ������ ���δ�(���� Ȯ��)
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                                MatrixMultiply(&wzRotTemp4, &wmRotTemp, &wmRotTemp3);
                                MatrixMultiply(&wmRotMat, &wzRotTemp4, &wmRotTemp2);
                            }
                            break;

                        case eSKILLEFFECT_POS_BODY:
                            {
                                wmRotTemp2     = mat;	
                                wmRotTemp2._41 = 0.0f;
                                wmRotTemp2._42 = 0.0f;
                                wmRotTemp2._43 = 0.0f;

                                MatrixRotationY(&wmRotTemp, WZ_PI/2.f); //Y������ 90�� ������ ���δ�(���� Ȯ��)
                                MatrixMultiply(&wmRotMat, &wmRotTemp, &wmRotTemp2);
                            }
                            break;

                        case eSKILLEFFECT_POS_RHAND:
                        case eSKILLEFFECT_POS_LHAND:
                            {
                                wmRotTemp2     = mat;	
                                wmRotTemp2._41 = 0.0f;
                                wmRotTemp2._42 = 0.0f;
                                wmRotTemp2._43 = 0.0f;

                                MatrixRotationY(&wmRotTemp, WZ_PI*1.5f); //Y������ 270�� ������ ���δ�(���� Ȯ��)
                                MatrixMultiply(&wmRotMat, &wmRotTemp, &wmRotTemp2);
                            }
                            break;

                        default:
                            {
                                wmRotMat     = mat;	
                                wmRotMat._41 = 0.0f;
                                wmRotMat._42 = 0.0f;
                                wmRotMat._43 = 0.0f;
                            }
                            break;
                        }
                        //////////////////////////////////////////////////////////////////////////

                        //Scale
                        //////////////////////////////////////////////////////////////////////////
                        WzVector vScale = c_ZeroVector;
                        pCharacter->GetScale(&vScale);
                        MatrixScaling( &wmScale, vScale.x, vScale.y, vScale.z);
                        //////////////////////////////////////////////////////////////////////////

                        pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );
                    }
                    else
                    {
                        WzVector vRot;
                        if ( pCharacter->GetClass() == eCHAR_ELEMENTALIST)
                        {
                            vRot.z = pCharacter->GetAngle(); 
                            vRot.y = 0.f;
                            vRot.x = pCharacter->GetAngleX();
                        }
                        else
                        {
                            vRot.x = 0.0f;
                            vRot.y = 0.0f;
                            vRot.z = 0.0f;
                        }

                        WzVector vScale;
                        vScale.x = 1.0f;
                        vScale.y = 1.0f;
                        vScale.z = 1.0f;

                        WzVector wvPos;
                        GetAttachEffectForSetItemPos(pCharacter, wvPos);

                        pBody->SetResultMatrix(wvPos,vRot,vScale);
                    }
                }
            }
            break;

        case eKIND_OF_EQUIPED_SETITEM_SET:
        default:
            break;
        }
    }
}

void cPlayerSetItemEffect::ChangeFullsetResource( Character* pCharacter, WORD setitem_optioninfo_setgroupcode )
{
    EquipmentContainer* equipment_container = pCharacter->GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(EquipmentContainer::EQUIP_ARMOR));
    SETITEM_OPTION_INFO* setitem_optioninfo = SetItemOptionInfoParser::Instance()->Find(setitem_optioninfo_setgroupcode);
    if ((setitem_optioninfo == NULL) || (setitem_optioninfo->change_armor_resource[pCharacter->GetGender()] == 0))
    {
        return;
    }
    RESOURCECODE new_res_code = setitem_optioninfo->change_armor_resource[pCharacter->GetGender()] + ENGINE_RESOURCE_ID_OFFSET;

    BodyPart change_parts_index = BP_ARMOR;
    CWzUnitDraw** parts = pCharacter->GetParts();
    if (parts == NULL)
    {
        return;
    }

    if (static_cast<int>(pCharacter->GetPartCount()) <= change_parts_index)
    {
        return;
    }

    if ((parts[change_parts_index] == NULL) || (parts[change_parts_index]->GetWzUnit() == NULL))
    {
        return;
    }

    RESOURCECODE cur_res_code = parts[change_parts_index]->GetWzUnit()->GetType();
    if (new_res_code == cur_res_code)
    {
        return;
    }

    CWzUnitDraw* new_unit = ResourceManager::Instance()->AllocResourceUnitDraw(new_res_code);
    if (new_unit != NULL)
    {
        new_unit->SetSubType(parts[change_parts_index]->GetSubType());

        ResourceManager::Instance()->FreeResourceUnitDraw(parts[change_parts_index]);

        parts[change_parts_index] = new_unit;
        parts[change_parts_index]->SetAniRefUnitDraw(pCharacter->GetBasePart());

    }
}

void cPlayerSetItemEffect::RestoreFullsetResource( Character* pCharacter )
{
    EquipmentContainer* equipment_container = pCharacter->GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    BodyPart change_parts_index = BP_ARMOR;
    if (static_cast<size_t>(change_parts_index) >= pCharacter->GetPartCount())
    {
        return;
    }

    CWzUnitDraw** parts = pCharacter->GetParts();
    if ((parts[change_parts_index] == NULL) || (parts[change_parts_index]->GetWzUnit() == NULL))
    {
        return;
    }

    RESOURCECODE cur_res_code = parts[change_parts_index]->GetWzUnit()->GetType();
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(EquipmentContainer::EQUIP_ARMOR));
    if (item_slot.GetCode() <= 0)
    {
        return;
    }
    int equip_pos = equipment_container->GetEquipPosByParts(BP_ARMOR);
    if (equipment_container->GetEquipmentManageInfo()->slots[equip_pos].activated == false)
    {
        // �������� ��Ȱ��ȭ�� ��� �������
        return;
    }

    RESOURCECODE item_res_code = ResourceManager::Instance()->GetItemSkinResourceCode(item_slot.GetImageCode(), pCharacter->GetGender());

    if (item_res_code == 0)
    {
        return;
    }
    if (item_res_code == cur_res_code)
    {
        return;
    }
    CWzUnitDraw* new_unit = ResourceManager::Instance()->AllocResourceUnitDraw(item_res_code);
    if (new_unit != NULL)
    {
        new_unit->SetSubType(parts[change_parts_index]->GetSubType());

        ResourceManager::Instance()->FreeResourceUnitDraw(parts[change_parts_index]);

        parts[change_parts_index] = new_unit;
        parts[change_parts_index]->SetAniRefUnitDraw(pCharacter->GetBasePart());
    }

}

void cPlayerSetItemEffect::RefreshFullsetEffect( Character* pCharacter, eKIND_OF_EQUIPED_SETITEM kind_of_equiped_setitem, WORD setitem_optioninfo_setgroupcode)
{
    // Ǯ�� �������� ����ڵ� ������ ����
    if( kind_of_equiped_setitem == eKIND_OF_EQUIPED_SETITEM_NONE || pCharacter->is_attached_wing_accessory(NULL) == true )
    {
        RestoreFullsetResource(pCharacter);
    }
    else
    {
        ChangeFullsetResource(pCharacter, setitem_optioninfo_setgroupcode);
    }


    // ��� ����Ʈ�� ��Ʈ������ ����Ʈ�����͸� �����Ѵ�
    ReleaseFullsetEffect();

    // ��Ʈ������ ����Ʈ�����͸� �籸���Ѵ�
    if (eKIND_OF_EQUIPED_SETITEM_NONE != kind_of_equiped_setitem)
    {
        WzVector wvPos;
        GetAttachEffectForSetItemPos(pCharacter, wvPos);

        for (int i = 0; i < MAX_SETITEM_EFFECT_NUM; ++i)
        {
            sFullSetEffectInfo effect_info;
            GetFullsetEffectInfo(i, effect_info);

            if( effect_info.m_dwEffectID != INVALID_WZID )
            {
                // �������̱� �ɼ��� ���� ��� �ȱ�����Ʈ ������ �����Ѵ�.(�ȱ�����Ʈ�� �ٴ� ���ε���:1)
                if( pCharacter->IsHideHelmet() == false || effect_info.m_btBoneIdx != 1 )
                {
                    sAttachEffectForSetItem AttachEffectInfo;
                    AttachEffectInfo.eSetType = kind_of_equiped_setitem; 
                    AttachEffectInfo.m_btBoneIdx = effect_info.m_btBoneIdx;
                    AttachEffectInfo.m_EffectID = effect_info.m_dwEffectID;
                    m_FullsetEffectAttachData.push_back(AttachEffectInfo);

                    //AttachEffectInfo.m_hEffectHandle = GlobalFunc::CreateEffectBeUpToOption( effect_info.m_dwEffectID, wvPos, pCharacter->GetObjectKey());
                }
            }
        }
    }

    // ����Ʈ�����͸� �̿��� ���� ����Ʈ�� �����Ѵ�
    RefreshFullsetEffect(pCharacter);
}

void cPlayerSetItemEffect::RefreshFullsetEffect(Character* pCharacter)
{
    // �ϴ� ����Ʈ�� ��� �����Ѵ�
    for( size_t i=0;i!=m_FullsetEffectAttachData.size();++i)
    {
        if( m_FullsetEffectAttachData[i].m_hEffectHandle != INVALID_HANDLE_VALUE )
        {
            g_pEffectManager->RemoveEffectPtr(m_FullsetEffectAttachData[i].m_hEffectHandle);
            m_FullsetEffectAttachData[i].m_hEffectHandle = INVALID_HANDLE_VALUE;
        }
    }

    // ����Ʈ�� ǥ���ؾ��Ѵٸ� �����Ѵ�
    if( m_ShowFullsetEffect == TRUE )
    {
        WzVector wvPos;
        GetAttachEffectForSetItemPos(pCharacter, wvPos);

        for( size_t i=0;i!=m_FullsetEffectAttachData.size();++i)
        {
            if( pCharacter->IsHideHelmet() == TRUE && m_FullsetEffectAttachData[i].m_btBoneIdx == 1)
            {
                continue;
            }

#ifdef _NA_008536_20151023_ELITE4TH_ADD
			// �뺯�ſ����� ���� FullsetEffect�� ���� �ʴ´�.
			if ( pCharacter->GetClass() == eCHAR_DRAGON && pCharacter->IsTransForm() == TRUE && m_FullsetEffectAttachData[i].m_btBoneIdx == 1 )
			{
				continue;
			}
#endif // _NA_008536_20151023_ELITE4TH_ADD

            if( m_FullsetEffectAttachData[i].m_hEffectHandle == INVALID_HANDLE_VALUE )
            {
                HANDLE hEffect = GlobalFunc::CreateEffectBeUpToOption( m_FullsetEffectAttachData[i].m_EffectID, wvPos, pCharacter->GetObjectKey());
                m_FullsetEffectAttachData[i].m_hEffectHandle = hEffect;
            }
        }
    }
}

void cPlayerSetItemEffect::ReleaseFullsetEffect()
{
    for( size_t i=0;i!=m_FullsetEffectAttachData.size();++i)
    {
        if (m_FullsetEffectAttachData[i].m_hEffectHandle != INVALID_HANDLE_VALUE)
        {
            g_pEffectManager->RemoveEffectPtr(m_FullsetEffectAttachData[i].m_hEffectHandle);			
        }
    }
    m_FullsetEffectAttachData.clear();
}

void cPlayerSetItemEffect::ShowFullsetEffect(BOOL IsShow)
{
    m_ShowFullsetEffect = IsShow;
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
cPlayerFPFullEffect::cPlayerFPFullEffect()
{
    for (int i = 0; i < MAX_FP_FULL_EFFECT_H; ++i)
    {
        fp_full_effect_handle_[i] = INVALID_HANDLE_VALUE;
    }
}

cPlayerFPFullEffect::~cPlayerFPFullEffect()
{
    DestroyFPFullEffect();
}

void cPlayerFPFullEffect::CreateFPFullEffect(Character* character_ptr)
{
    if (character_ptr == NULL)
        return;

    for (int i = 0; i < MAX_FP_FULL_EFFECT_H; ++i)
    {
        if (fp_full_effect_handle_[i] != INVALID_HANDLE_VALUE)
            return;
    }    

    DestroyFPFullEffect();

    // �氭ö ������� ��û���� �� ��ȣ�� ���� ���.
    WzVector Attach_Position;
    GameFunc::GetBonePos( character_ptr->GetBasePart(), 32, &Attach_Position);
    fp_full_effect_handle_[LEFT_FP_FULL_EFFECT_H] = 
        GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KF47"), 
                                              Attach_Position, 
                                              character_ptr->GetObjectKey());

    GameFunc::GetBonePos( character_ptr->GetBasePart(), 56, &Attach_Position);
    fp_full_effect_handle_[RIGHT_FP_FULL_EFFECT_H] = 
        GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KF47"), 
                                              Attach_Position, 
                                              character_ptr->GetObjectKey());
}

void cPlayerFPFullEffect::DestroyFPFullEffect()
{
    for( size_t i=0;i!=MAX_FP_FULL_EFFECT_H;++i)
    {
        if (fp_full_effect_handle_[i] != INVALID_HANDLE_VALUE)
        {
            g_pEffectManager->RemoveEffectPtr(fp_full_effect_handle_[i]);
            fp_full_effect_handle_[i] = INVALID_HANDLE_VALUE;
        }
    }
}

void cPlayerFPFullEffect::ProcessFPFullEffect(Character* character_ptr)
{
    if (character_ptr == NULL)
        return;

    WzMatrix wmPosMat, wmRotMat, wmScale, mat;
    for (int i = 0; i < MAX_FP_FULL_EFFECT_H; ++i)
    {
        if (fp_full_effect_handle_[i] == INVALID_HANDLE_VALUE)
            continue;

        int input_bone_index = 0;
        if (i == LEFT_FP_FULL_EFFECT_H)
            input_bone_index = 32;
        if (i == RIGHT_FP_FULL_EFFECT_H)
            input_bone_index = 56;

        mat = character_ptr->GetBoneMatrix(input_bone_index);

        if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(fp_full_effect_handle_[i]) )
        {
            //Pos
            //////////////////////////////////////////////////////////////////////////
            MatrixTranslation(&wmPosMat, mat._41, mat._42, mat._43);
            //////////////////////////////////////////////////////////////////////////

            wmRotMat     = mat;
            wmRotMat._41 = 0.0f;
            wmRotMat._42 = 0.0f;
            wmRotMat._43 = 0.0f;

            MatrixScaling(&wmScale, 1.0f ,1.0f, 1.0f);

            pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );
        }
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE