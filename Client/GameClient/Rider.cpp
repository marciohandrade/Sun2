#include "SunClientPrecompiledHeader.h"

#include "Player.h"
#include "Hero.h"
#include "Rider.h"
#include "NpcInfoParser.h"
#include "GameFunc.h"
#include "ResourceManager.h"
#include "RiderContainer.h"
#include "RidingInfoParser.h"
#include "ItemInfoParser.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "ObjectAniEventController.h"
#include "GlobalData.h"
#include "Object.h"

const WzVector c_wvUp = { 0.0f, 0.0f, 1.0f };
//------------------------------------------------------------------------------
/** 
*/
Rider::Rider( void ):
m_bSetInterpolate(FALSE),
m_iLinkNode(-1),
body_skin_(0),
effect_code_(0),
is_interpoate_(false),
interpolate_axis_(c_UpVector),
prev_axis_(c_UpVector),
current_animation_(0),
prev_angle_(0.f),
is_spread_(0)
{
	MatrixIdentity(&rotate_matrix());
	memset(&m_vLinkPos,0,sizeof(m_vLinkPos));
	memset(&m_ppRenderPart,0,sizeof(m_ppRenderPart));

    InitializeEffect();

    //! �ʱ�ȭ
    m_Color = 0xffffffff;
}
//------------------------------------------------------------------------------
/** 
*/
Rider::~Rider( void )
{
    Destroy();
}
//------------------------------------------------------------------------------
/** 
*/
BOOL Rider::Create(DWORD dwRiderIndex, Player* master_ptr, BOOL summon_effect)
{
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(dwRiderIndex);     
    
    if(rider_info == NULL)
    {
        assert(!"���������ʴ� ���̴��ε���");
        return FALSE;
    }

    BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo( rider_info->monster_code_);

    if(pInfo == NULL)
    {
        assert(!"���̴������ڵ� �������");
        return FALSE;
    }

   body_skin_ = ResourceManager::Instance()->GetNPCSkinResourceCode(pInfo->m_MonsterCode);
   m_Animaping = rider_info->anicode_[0];
   m_dwRiderIndex = dwRiderIndex;
     
   if (summon_effect)
   {
       set_effect_code(rider_info->spawn_effect_code_);
   }
   
   //������Ʈ���� �����ϸ� ��ȯ�Ѱ��̴�.
    RefreshApperRider(master_ptr);
   
    return TRUE;
}
//------------------------------------------------------------------------------
/**
*/
bool Rider::RefreshApperRider(Player* master_ptr)
{
    if ( master_ptr != NULL && master_ptr->IsUseDelayLoading() )
    {
        return false;
    }

    if (GetRenderPart())
    {
        return false;
    }

    ResourceManager* resource_instance = ResourceManager::Instance();
    
    m_ppRenderPart[RP_BASE] = resource_instance->AllocResourceUnitDraw(body_skin_, -1.f, false);

    if (m_ppRenderPart[RP_BASE] && GetRenderPart()->GetWzUnit())
    {
        WzBoneInfo *pBaseInfo = GetRenderPart()->GetWzUnit()->GetBoneInfo(StrToWzID("LINK"));		

        if (pBaseInfo)
        {
            m_iLinkNode = pBaseInfo->m_iNode;
        }

        //����Ʈ �ڵ� ����� ���(Player�� ������¿��� ȣ��������쿡�� ����Ʈ�����ϰ�)
        //�ٸ������� Ż�ͼ�ȯ���¿��� ������ �������� ������ ����ÿ��� �ȶ���
        if (effect_code())
        {
            GlobalFunc::CreateEffectBeUpToOption(effect_code(), master_ptr->GetVisiblePos());
        }

        m_bSetInterpolate = TRUE;
    }

    if (master_ptr)
    {
        if (master_ptr->GetCurState() == STATE::MOVE)
        {
            // �̵� ���̾��ٸ� ���̴� ���� �ִϸ��̼�����
            WzID master_anicode = GlobalFunc::GetPlayerAnimationByRiding(eANI_RUN, GetAniMapping());
            master_ptr->SetPlayerAnimation(master_anicode, eANI_RUN, TRUE);
        }
        else
        {
            //����:Ż�� ���ҽ� ��ȯ�Ŀ� �ٷ� IDLE �ִϸ��̼ǽ����ش�
            ePLAYER_ANI rider_anicode = ((rand() % 2) == 1) ? (eANI_IDLE):(eANI_IDLE2);
            WzID master_anicode = GlobalFunc::GetPlayerAnimationByRiding(rider_anicode, GetAniMapping());
            master_ptr->SetPlayerAnimation(master_anicode, rider_anicode, FALSE);
        }
    }

    return true;
}
//------------------------------------------------------------------------------
/**
*/
void Rider::SetSubtype(int nPartIdx,int nSubType)
{
    //0�� ���̽�������
    nPartIdx += RP_BODY;

	if(nPartIdx<RP_COUNT && nSubType<LIMIT_SUBTYPE)
	{
        if(m_ppRenderPart[nPartIdx])
        {
            int nNum = m_ppRenderPart[nPartIdx]->GetSubTypeNum();
            if(nSubType>nNum)
            {
                assert(!"����Ÿ���� ����");
                return;
            }
            m_ppRenderPart[nPartIdx]->SetSubType(nSubType);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void Rider::SetScale(WzVector vScale)
{
	for(int i=0;i<RP_COUNT;++i)
	{
		if(m_ppRenderPart[i])
		{
			m_ppRenderPart[i]->SetScale(vScale.x,vScale.y,vScale.z);
		}
	}
}
//------------------------------------------------------------------------------
/** 
*/
void Rider::EnableFunction(BOOL bFlag)
{
    for(int i=0;i<RP_COUNT;++i)
    {
        if(m_ppRenderPart[i])
        {
            m_ppRenderPart[i]->EnableCustomLight(bFlag);
            m_ppRenderPart[i]->EnableShadowCast(bFlag);
            m_ppRenderPart[i]->EnableShadowReceive(bFlag);               
        }
    }

}
//------------------------------------------------------------------------------
/** ���ڷ� ������ ���̴� ��ü�κ��� ���̴��� �����س���.(����Ÿ�Թ׽�Ų)
*/
void Rider::CopyRider(Rider *pRider, bool is_with_parts/* = true*/)
{
    CWzUnitDraw* pSrc   = pRider->GetRenderPart();  //����
    CWzUnitDraw* pDest  = GetRenderPart();          //����Ǿ���(this)

    //���� �ٵ� �Ҵ��̾ȵȻ��¶���Ҵ��Ѵ�.
    if(pSrc && pSrc->GetWzUnit())
    {
       if(!pDest)
       {
          m_ppRenderPart[RP_BASE] = ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
       }
       else
       {
           //�Ѵ��ִµ� Ÿ���� �ٸ��ٸ�
           if((pDest->GetWzUnit() == NULL) ||
              (pSrc->GetWzUnit()->GetType()!=pDest->GetWzUnit()->GetType()))
           {
               //�����ϰ�
               ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[RP_BASE]);
               //�ٵ� �Ҵ��Ѵ�
               m_ppRenderPart[RP_BASE]= ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
               m_dwRiderIndex=pRider->GetRiderIndex();
           }
       }
       
    }

    if (is_with_parts)
    {
	    //�������
	    for(int i=RP_BODY;i<RP_COUNT;++i)
	    {
            pSrc    = pRider->GetRenderPart(i);  //����
            pDest   = GetRenderPart(i);          //����Ǿ���(this)

		    //���Ҵ��ϰ��
		    if(!pDest)
		    {
			    //���������Ѵٸ� �Ҵ��� �ִ����������ش�(�����̱⶧��)
			    if(pSrc && pSrc->GetWzUnit())
			    {
				    m_ppRenderPart[i]=ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
                    m_ppRenderPart[i]->SetSubType(pSrc->GetSubType());
				    m_ppRenderPart[i]->SetAniRefUnitDraw( GetRenderPart());
			    }
		    }
		    else
		    {
			    //������ ���� && ��������
			    if(pSrc)
			    {
                    //�Ѵ������ ���ҽ��ڵ尡 �´���üũ�Ѵ�.
                    if ((pDest->GetWzUnit() == NULL) ||
                        (pSrc->GetWzUnit()->GetType()!=pDest->GetWzUnit()->GetType()))
                    {
                        //�����ϰ�
                        ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[i]);
                        //�Ҵ�����
                        m_ppRenderPart[i] = ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
                        //�ִϹ� ����Ÿ�Ժ������ش�.
                        m_ppRenderPart[i]->SetAniRefUnitDraw( GetRenderPart());
                        m_ppRenderPart[i]->SetSubType(pSrc->GetSubType());
                    }
                   
			    }
			    else
			    {
                    if(pDest)
                    {
                        ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[i]);
                        m_ppRenderPart[i] = NULL;
                    }
    		    }
		    }
	    }
    } //if (is_with_parts)
}
//------------------------------------------------------------------------------
/** ��ȯ������ Player������������ ���ý����ش�.
*/
void Rider::Transform(CWzUnitDraw* pMaster)
{
    if ((pMaster == NULL) ||
        (GetRenderPart() == NULL))
    {
       return;
    }
   
    const WzMatrix *pLinkMatrix = GetRenderPart()->GetTransformBuffer();

    if (pLinkMatrix == NULL)
    {
        return;
    }

    static WzMatrix temp_matrix;
    memcpy(&temp_matrix, &pLinkMatrix[m_iLinkNode], sizeof(temp_matrix));
    //1.Position: ��ġ�κ��� �����Ѵ�
    m_vLinkPos.x = temp_matrix._41;
    m_vLinkPos.y = temp_matrix._42;
    m_vLinkPos.z = temp_matrix._43;
    pMaster->SetPosition(m_vLinkPos, FALSE);
    //2.Rotation: ȸ���κ��� �����Ѵ�
    temp_matrix._41 = 0.f;
    temp_matrix._42 = 0.f;
    temp_matrix._43 = 0.f;   
    pMaster->SetRotationByMatrix(&temp_matrix);
    //3.Flag������ �������� �ٷ� �ٰԲ��Ѵ�
    if(m_bSetInterpolate)
    {
        m_bSetInterpolate = FALSE;
        pMaster->SetInterpolationFrame(0);
    }
}
//------------------------------------------------------------------------------
/**  ���̴��� ������Ʈ������ ������ �°� �ٿ�����Ѵ�.
	 ĳ���Ͷ��� Ż�Ϳ� ź���¿����� ���������Ʈ������ �����Ѵ�.
*/
void Rider::SetRotation(const WzVector* normal_axis, float master_angle)
{
    if ((is_interpoate() == false) &&
       (prev_angle() == master_angle) &&
       (prev_axis() == *normal_axis))
    {
        return;
    }

    WzVector axis = *normal_axis;

    // check degree limit
    float axis_angle = n_rad2deg(acosf(VectorDotProduct(normal_axis, &c_UpVector)));
    if ((axis_angle < 20.f) ||
        (axis_angle > 70.f))
    {
        axis = c_UpVector;
    }
    // Interpolation
    float angle_diff = n_rad2deg(acosf(VectorDotProduct(&axis, &interpolate_axis())));
    if ((angle_diff != 0.f) && 
        (angle_diff  > 5.f))
    {
        set_is_interpoate(true);
        InterpolateVector(&axis, &axis, &interpolate_axis(), (5.f / angle_diff));
    }
    else
    {
        set_is_interpoate(false);
    }

    // Generate Matrix
    WzVector dir = {sin(master_angle), -cos(master_angle), 0.f};
    WzVector right;
    VectorCrossProduct(&right, &axis, &dir);
    VectorNormalize(&right, &right);
    VectorCrossProduct(&dir, &axis, &right);
    VectorNormalize(&dir, &dir);
    memset(&rotate_matrix(), 0, sizeof(rotate_matrix()));
    rotate_matrix()._11 = right.x; rotate_matrix()._12 = right.y; rotate_matrix()._13 = right.z; //X��
    rotate_matrix()._21 = dir.x;   rotate_matrix()._22 = dir.y;   rotate_matrix()._23 = dir.z;   //Y��
    rotate_matrix()._31 = axis.x;  rotate_matrix()._32 = axis.y;  rotate_matrix()._33 = axis.z;; //Z��
    rotate_matrix()._44 = 1.f;

    // Apply Matrix
    if (GetRenderPart())
    {
        GetRenderPart()->SetRotationByMatrix(&rotate_matrix());
    }

    set_prev_axis(*normal_axis);
    set_prev_angle(master_angle);
    set_interpolate_axis(axis);
    
}
//------------------------------------------------------------------------------
/**
*/
void Rider::SetForceLod(int iLod)
{
    for(int i=0;i<RP_COUNT;++i)
    {
        if(m_ppRenderPart[i])
        {
            m_ppRenderPart[i]->SetForcedLOD(iLod);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void Rider::SetAnimation(WzID wzID,BOOL bLoop, BOOL bRestart)
{
    DestroyAllLoopEffect();

    if (current_animation_ == wzID)
    {
        return;
    }

    if (bLoop == TRUE)
    {
        bRestart = FALSE;
    }
    //BOOL restart_animation = (bLoop) ? (FALSE):(TRUE);

	if (GetRenderPart())
	{
		GetRenderPart()->SetCurrentAnimation(wzID, bLoop, bRestart);
        current_animation_ = wzID;
	}
}
//------------------------------------------------------------------------------
/**
*/
BOOL Rider::Process(DWORD dwTick, Player* master_ptr)
{
	if (GetRenderPart() != NULL)
	{
        static WzEvent ani_event;
        ZeroMemory(&ani_event, sizeof(ani_event));
		g_pSunRenderer->ProgressUnitFrame(GetRenderPart(), (float)dwTick, &ani_event, &current_ani_progress_rate_);

        if (master_ptr != NULL)
        {
            EventInfo event_info(ani_event);
            if ((dwTick != 0) && 
                ((master_ptr != g_pHero) || (g_pHero->GetShow() == TRUE)))
            {
                for (size_t i = 0; i != event_info.GetSoundEventCount(); ++i)
                {
                    ProcessSoundEvent(event_info.GetSoundEvent(i), GetRenderPart()->GetPosition());
                }
            }

            // ����Ʈ �̺�Ʈ
            for (size_t i = 0; i < event_info.GetEffectEventCount(); ++i)
            {
                ProcessEffectEvent(event_info.GetEffectEvent(i), 
                                    GetRenderPart(), 
                                    GetRenderPart()->GetPosition(), 
                                    master_ptr->GetDirection(), 
                                    master_ptr->GetObjectKey());
            }

            // Ŀ���� �̺�Ʈ
            for (size_t i = 0; i != event_info.GetCustomEventCount(); ++i)
            {
                ProcessCustomEvent(event_info.GetCustomEvent(i), master_ptr->GetPosition());
            }
        }
	}


	return TRUE;
}
//------------------------------------------------------------------------------
/**
*/
BOOL Rider::Render()
{
    WzColor oldColor  = g_pSunRenderer->GetDiffuseColor();
    
    g_pSunRenderer->SetColor(m_Color);
    
    for(int i=0;i<RP_COUNT;++i)
	{
		if(m_ppRenderPart[i])
		{
            if (g_pSunRenderer->CheckUnitLoaded(m_ppRenderPart[i]))
            {
			    g_pSunRenderer->RenderUnit(m_ppRenderPart[i]);
            }
		}
	}
    g_pSunRenderer->SetColor(oldColor);
	return TRUE;
}
//------------------------------------------------------------------------------
/**

*/
BOOL Rider::Render(St_RenderCommand *pCommand)
{
	//�׿�����
	for(int i=0;i<RP_COUNT;++i)
	{
		if(m_ppRenderPart[i])
		{
            if (g_pSunRenderer->CheckUnitLoaded(m_ppRenderPart[i]))
            {
			    g_pSunRenderer->RenderUnit(m_ppRenderPart[i],pCommand);
            }
		}
	}
	return TRUE;
}
//------------------------------------------------------------------------------
/**
	
*/
void Rider::Destroy( void )
{
	for(int i=0;i<RP_COUNT;++i)
	{
		if(m_ppRenderPart[i])
		{
			ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[i]);
			m_ppRenderPart[i] = NULL;
		}
	}
    m_iLinkNode		= -1;

    DestroyAllEffect();
}
//------------------------------------------------------------------------------
/**
*/
void Rider::AnimateUnit(Player* master_ptr)
{
    if (GetRenderPart())
    {
	    g_pSunRenderer->AnimateUnit(GetRenderPart());	

        if (master_ptr != NULL)
        {
            UpdateTrail();
            UpdateEffectPos(GetRenderPart(), GetRenderPart()->GetPosition(), master_ptr->GetDirection());
        }
    }
}
//------------------------------------------------------------------------------
/** ���̴���������
*/
void Rider::DetachRiderPart(int nIdx)
{
    nIdx+=RP_BODY;
    if(nIdx>RP_BASE && nIdx<RP_COUNT)
    {
        //�������� �����Ѵٸ� ����
        if(m_ppRenderPart[nIdx])
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[nIdx]);
            m_ppRenderPart[nIdx] = NULL;
        }

        //������ ��ũ��Ʈ ���� ���̽���Ų�� �޾��ش�.
        const  sRiderInfo* pInfo = RidingInfoParser::Instance()->GetInfo(m_dwRiderIndex);

        if(pInfo && (nIdx-1)>=0)
        {
           RESOURCECODE SkinCode  = pInfo->base_skin[nIdx-1];

           //0�ϰ�쿡�� ���̽� ��Ų�� ���°���̴�.
           if(SkinCode!=0)
           {
		      m_ppRenderPart[nIdx] = ResourceManager::Instance()->AllocResourceUnitDraw(ENGINE_RESOURCE_ID_OFFSET + SkinCode);
              m_ppRenderPart[nIdx]->SetAniRefUnitDraw( GetRenderPart());
           }
        }
    }
}

//------------------------------------------------------------------------------ 
void Rider::DetachPart(const RiderParts& rider_parts)
{
    if (rider_parts < RP_BODY && rider_parts >= RP_COUNT)
    {
        return;
    }

    if(m_ppRenderPart[rider_parts])
    {
        ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[rider_parts]);
        m_ppRenderPart[rider_parts] = NULL;
    }

    //������ ��ũ��Ʈ ���� ���̽���Ų�� �޾��ش�.
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(m_dwRiderIndex);

    if (rider_info != NULL)
    {
        RESOURCECODE skin_code  = rider_info->base_skin[rider_parts - 1];

        //0�ϰ�쿡�� ���̽� ��Ų�� ���°���̴�.
        if (skin_code != 0)
        {
            m_ppRenderPart[rider_parts] = ResourceManager::Instance()->AllocResourceUnitDraw(ENGINE_RESOURCE_ID_OFFSET + skin_code);
            m_ppRenderPart[rider_parts]->SetAniRefUnitDraw(GetRenderPart());
        }
    }
}
//------------------------------------------------------------------------------
/** ���̴���������
*/
void Rider::AttachRiderPart(int nIdx, DWORD dwItemCode)
{
    nIdx+=RP_BODY;
	if(nIdx>RP_BASE && nIdx<RP_COUNT)
	{
        BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( dwItemCode );
        if(!pInfo)
        {
            assert(!"���̴� ��������������");
            return;
        }
        
        RESOURCECODE SkinCode = 0;

		//������ ���Ⱑ �����Ǿ��־��ٸ� ����
		if(m_ppRenderPart[nIdx] && m_ppRenderPart[nIdx]->GetWzUnit())
		{
            SkinCode = m_ppRenderPart[nIdx]->GetWzUnit()->GetType();
            
            //�����Ѿ������̸� �����Ǿ������� ���������ʴ´�.
            if(SkinCode!=ENGINE_RESOURCE_ID_OFFSET + pInfo->m_wWICode)
            {
                ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[nIdx]);
                m_ppRenderPart[nIdx] = NULL;
            }
            else
            {
                //no operation
                return;
            }
		}

		//���ҽ��Ҵ�
        SkinCode = ENGINE_RESOURCE_ID_OFFSET + pInfo->m_wWICode;
		m_ppRenderPart[nIdx] = ResourceManager::Instance()->AllocResourceUnitDraw(SkinCode);

		//�ִ�����������
		if(m_ppRenderPart[nIdx])
		{
			m_ppRenderPart[nIdx]->SetAniRefUnitDraw( GetRenderPart());
		}
	}
}
//------------------------------------------------------------------------------
/** ���̴���������
*/
void Rider::CopyRiderBody(DWORD dwRiderIndex,  Player* master_ptr)
{
    const sRiderInfo* pInfo = RidingInfoParser::Instance()->GetInfo(dwRiderIndex);

    if(pInfo)
    {
        BASE_NPCINFO *pNpcInfo=NPCInfoParser::Instance()->GetNPCInfo( pInfo->monster_code_);

        if(!pNpcInfo)
        {
            return;
        }

        RESOURCECODE SkinCode= ResourceManager::Instance()->GetNPCSkinResourceCode(pNpcInfo->m_MonsterCode );

        //�ٵ��ֱ��ϴ�
        if(m_ppRenderPart[RP_BASE] && m_ppRenderPart[RP_BASE]->GetWzUnit())
        {
            //Body�̹����� �ٸ��� �����ϰ� �ٽ� �Ҵ��ؾ��Ѵ�.
            if(m_ppRenderPart[RP_BASE]->GetWzUnit()->GetType()!=SkinCode)
            {
                ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[RP_BASE]);
                m_ppRenderPart[RP_BASE] = ResourceManager::Instance()->AllocResourceUnitDraw(SkinCode);
                m_dwRiderIndex=dwRiderIndex;
            }
        }
        else
        {
            //�ٵ� ������ �����
            Create(dwRiderIndex, master_ptr, FALSE);
        }

    }
}
//------------------------------------------------------------------------------ 
bool Rider::is_possible_fly()
{
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(m_dwRiderIndex);

    if (rider_info)
    {
        return rider_info->flying_;
    }
    return false;
}