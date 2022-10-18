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

    //! 초기화
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
        assert(!"존재하지않는 라이더인덱스");
        return FALSE;
    }

    BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo( rider_info->monster_code_);

    if(pInfo == NULL)
    {
        assert(!"라이더몬스터코드 존재안함");
        return FALSE;
    }

   body_skin_ = ResourceManager::Instance()->GetNPCSkinResourceCode(pInfo->m_MonsterCode);
   m_Animaping = rider_info->anicode_[0];
   m_dwRiderIndex = dwRiderIndex;
     
   if (summon_effect)
   {
       set_effect_code(rider_info->spawn_effect_code_);
   }
   
   //오브젝트값이 존재하면 소환한것이다.
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

        //이펙트 코드 존재시 출력(Player가 입장상태에서 호출했을경우에는 이펙트존재하고)
        //다른유저가 탈것소환상태에서 정보를 받을때나 마을에 입장시에는 안띄운다
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
            // 이동 중이었다면 라이더 무브 애니메이션으로
            WzID master_anicode = GlobalFunc::GetPlayerAnimationByRiding(eANI_RUN, GetAniMapping());
            master_ptr->SetPlayerAnimation(master_anicode, eANI_RUN, TRUE);
        }
        else
        {
            //연출:탈것 리소스 소환후에 바로 IDLE 애니메이션시켜준다
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
    //0은 베이스파츠임
    nPartIdx += RP_BODY;

	if(nPartIdx<RP_COUNT && nSubType<LIMIT_SUBTYPE)
	{
        if(m_ppRenderPart[nPartIdx])
        {
            int nNum = m_ppRenderPart[nPartIdx]->GetSubTypeNum();
            if(nSubType>nNum)
            {
                assert(!"서브타입이 없음");
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
/** 인자로 들어오는 라이더 객체로부터 라이더를 복사해낸다.(서브타입및스킨)
*/
void Rider::CopyRider(Rider *pRider, bool is_with_parts/* = true*/)
{
    CWzUnitDraw* pSrc   = pRider->GetRenderPart();  //원본
    CWzUnitDraw* pDest  = GetRenderPart();          //복사되어질(this)

    //현재 바디가 할당이안된상태라면할당한다.
    if(pSrc && pSrc->GetWzUnit())
    {
       if(!pDest)
       {
          m_ppRenderPart[RP_BASE] = ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
       }
       else
       {
           //둘다있는데 타입이 다르다면
           if((pDest->GetWzUnit() == NULL) ||
              (pSrc->GetWzUnit()->GetType()!=pDest->GetWzUnit()->GetType()))
           {
               //해제하고
               ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[RP_BASE]);
               //바디 할당한다
               m_ppRenderPart[RP_BASE]= ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
               m_dwRiderIndex=pRider->GetRiderIndex();
           }
       }
       
    }

    if (is_with_parts)
    {
	    //장비파츠
	    for(int i=RP_BODY;i<RP_COUNT;++i)
	    {
            pSrc    = pRider->GetRenderPart(i);  //원본
            pDest   = GetRenderPart(i);          //복사되어질(this)

		    //비할당일경우
		    if(!pDest)
		    {
			    //원본존재한다면 할당후 애니참조시켜준다(파츠이기때문)
			    if(pSrc && pSrc->GetWzUnit())
			    {
				    m_ppRenderPart[i]=ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
                    m_ppRenderPart[i]->SetSubType(pSrc->GetSubType());
				    m_ppRenderPart[i]->SetAniRefUnitDraw( GetRenderPart());
			    }
		    }
		    else
		    {
			    //원본이 존재 && 나도존재
			    if(pSrc)
			    {
                    //둘다존재시 리소스코드가 맞는지체크한다.
                    if ((pDest->GetWzUnit() == NULL) ||
                        (pSrc->GetWzUnit()->GetType()!=pDest->GetWzUnit()->GetType()))
                    {
                        //해제하고
                        ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[i]);
                        //할당한후
                        m_ppRenderPart[i] = ResourceManager::Instance()->AllocResourceUnitDraw(pSrc->GetWzUnit()->GetType());
                        //애니및 서브타입복사해준다.
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
/** 변환정보를 Player메인파츠에게 셋팅시켜준다.
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
    //1.Position: 위치부분을 복사한다
    m_vLinkPos.x = temp_matrix._41;
    m_vLinkPos.y = temp_matrix._42;
    m_vLinkPos.z = temp_matrix._43;
    pMaster->SetPosition(m_vLinkPos, FALSE);
    //2.Rotation: 회전부분을 복사한다
    temp_matrix._41 = 0.f;
    temp_matrix._42 = 0.f;
    temp_matrix._43 = 0.f;   
    pMaster->SetRotationByMatrix(&temp_matrix);
    //3.Flag있을땐 보간없이 바로 붙게끔한다
    if(m_bSetInterpolate)
    {
        m_bSetInterpolate = FALSE;
        pMaster->SetInterpolationFrame(0);
    }
}
//------------------------------------------------------------------------------
/**  라이더는 직접메트릭스를 지형에 맞게 붙여줘야한다.
	 캐릭터또한 탈것에 탄상태에서는 만들어진메트릭스를 공유한다.
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
    rotate_matrix()._11 = right.x; rotate_matrix()._12 = right.y; rotate_matrix()._13 = right.z; //X축
    rotate_matrix()._21 = dir.x;   rotate_matrix()._22 = dir.y;   rotate_matrix()._23 = dir.z;   //Y축
    rotate_matrix()._31 = axis.x;  rotate_matrix()._32 = axis.y;  rotate_matrix()._33 = axis.z;; //Z축
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

            // 이펙트 이벤트
            for (size_t i = 0; i < event_info.GetEffectEventCount(); ++i)
            {
                ProcessEffectEvent(event_info.GetEffectEvent(i), 
                                    GetRenderPart(), 
                                    GetRenderPart()->GetPosition(), 
                                    master_ptr->GetDirection(), 
                                    master_ptr->GetObjectKey());
            }

            // 커스텀 이벤트
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
	//그외파츠
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
/** 라이더파츠제거
*/
void Rider::DetachRiderPart(int nIdx)
{
    nIdx+=RP_BODY;
    if(nIdx>RP_BASE && nIdx<RP_COUNT)
    {
        //아이템이 존재한다면 제거
        if(m_ppRenderPart[nIdx])
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[nIdx]);
            m_ppRenderPart[nIdx] = NULL;
        }

        //제거후 스크립트 참조 베이스스킨을 달아준다.
        const  sRiderInfo* pInfo = RidingInfoParser::Instance()->GetInfo(m_dwRiderIndex);

        if(pInfo && (nIdx-1)>=0)
        {
           RESOURCECODE SkinCode  = pInfo->base_skin[nIdx-1];

           //0일경우에는 베이스 스킨이 없는경우이다.
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

    //제거후 스크립트 참조 베이스스킨을 달아준다.
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(m_dwRiderIndex);

    if (rider_info != NULL)
    {
        RESOURCECODE skin_code  = rider_info->base_skin[rider_parts - 1];

        //0일경우에는 베이스 스킨이 없는경우이다.
        if (skin_code != 0)
        {
            m_ppRenderPart[rider_parts] = ResourceManager::Instance()->AllocResourceUnitDraw(ENGINE_RESOURCE_ID_OFFSET + skin_code);
            m_ppRenderPart[rider_parts]->SetAniRefUnitDraw(GetRenderPart());
        }
    }
}
//------------------------------------------------------------------------------
/** 라이더파츠장착
*/
void Rider::AttachRiderPart(int nIdx, DWORD dwItemCode)
{
    nIdx+=RP_BODY;
	if(nIdx>RP_BASE && nIdx<RP_COUNT)
	{
        BASE_ITEMINFO * pInfo = ItemInfoParser::Instance()->GetItemInfo( dwItemCode );
        if(!pInfo)
        {
            assert(!"라이더 장비아템인포없넹");
            return;
        }
        
        RESOURCECODE SkinCode = 0;

		//기존에 무기가 장착되어있었다며 해제
		if(m_ppRenderPart[nIdx] && m_ppRenderPart[nIdx]->GetWzUnit())
		{
            SkinCode = m_ppRenderPart[nIdx]->GetWzUnit()->GetType();
            
            //동일한아이템이면 장착되어있으면 복사하지않는다.
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

		//리소스할당
        SkinCode = ENGINE_RESOURCE_ID_OFFSET + pInfo->m_wWICode;
		m_ppRenderPart[nIdx] = ResourceManager::Instance()->AllocResourceUnitDraw(SkinCode);

		//애니참조시켜줌
		if(m_ppRenderPart[nIdx])
		{
			m_ppRenderPart[nIdx]->SetAniRefUnitDraw( GetRenderPart());
		}
	}
}
//------------------------------------------------------------------------------
/** 라이더파츠장착
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

        //바디가있긴하다
        if(m_ppRenderPart[RP_BASE] && m_ppRenderPart[RP_BASE]->GetWzUnit())
        {
            //Body이미지가 다르다 제거하고 다시 할당해야한다.
            if(m_ppRenderPart[RP_BASE]->GetWzUnit()->GetType()!=SkinCode)
            {
                ResourceManager::Instance()->FreeResourceUnitDraw(m_ppRenderPart[RP_BASE]);
                m_ppRenderPart[RP_BASE] = ResourceManager::Instance()->AllocResourceUnitDraw(SkinCode);
                m_dwRiderIndex=dwRiderIndex;
            }
        }
        else
        {
            //바디가 없으면 만든다
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