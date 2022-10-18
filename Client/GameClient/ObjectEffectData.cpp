#include "SunClientPrecompiledHeader.h"
#include "ObjectEffectData.h"
#include "GlobalData.h"
#include <client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>
#include "EventInfo.h"
#include "GameFunc.h"
#include "GlobalFunc.h"


ObjectEffectData::ObjectEffectData()
{

}

void ObjectEffectData::Initialize()
{
	for( int i=0;i!=MAX_TRAIL; ++i )
	{
		m_bTrailActivate[i] = FALSE;
		m_hTrailEffect[i] = INVALID_HANDLE_VALUE;
		ZeroMemory(&m_arTrailParam[i], sizeof(TRAIL_PARAM) );
	}

	for ( int i = 0; i < MAX_EFFECT_BUFFER; ++i)
	{
		for (int j = 0; j < MAX_OBJECT_EFFECT; ++j)
		{
			m_EffectParam[i][j].hEffect = INVALID_HANDLE_VALUE;
			m_EffectParam[i][j].iBoneIndex = 0;
			m_EffectParam[i][j].bRotateWithBone = FALSE;
            m_EffectParam[i][j].bLoopEffect = FALSE;
            m_EffectParam[i][j].bOnlyPosition = FALSE;
            m_EffectParam[i][j].bRotateWithBone = FALSE;
            m_EffectParam[i][j].EffectID = 0;
			ZeroMemory(&m_EffectParam[i][j].quatRot, sizeof(WzQuaternion));		
			ZeroMemory(&m_EffectParam[i][j].vOffset, sizeof(WzVector));
			ZeroMemory(&m_EffectParam[i][j].vScale, sizeof(WzVector));
		}
	}
}

void ObjectEffectData::Release()
{
	EndTrail(0);
	EndTrail(1);
	ReleaseAllEffect();
}

void ObjectEffectData::StartTrail( int TrailIndex, const TRAIL_PARAM& TrailParam, HANDLE hEffect )
{
	//assert(TrailIndex >= 0 && TrailIndex < MAX_TRAIL);

	m_arTrailParam[TrailIndex] = TrailParam;
	m_hTrailEffect[TrailIndex] = hEffect;

	CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle( hEffect );


	if( pBody == NULL )
	{
        //! frame에 따라서 effect를 생성 안할수도 있음.(주석제거)
		//assert(!"검기 이펙트를 생성하지 못했습니다");
		return;
	}

	pBody->SetRenderType(AR_REFRACTION_GLASS);
	pBody->SetRefractionRate(1.0f);		// 굴절률.(0에 가까워 질수록 투명)
	pBody->SetRefractionAlpha(1.0f);	// 1.0 에 가까워질수록 본 이미지에 가까워짐

	m_bTrailActivate[TrailIndex] = TRUE;			

	if( CEElementFlatChain* pFChain = pBody->GetFlatChainPtr() )
	{
		pFChain->SetLifeInfo(0, 30000);	
		pFChain->SetTailNum(TrailParam.iTailNum);							
		pFChain->SetSplineLength(0.1f);
	}
	else
	{
		//assert(!"검기 이펙트를 생성하지 못했습니다");
		return;
	}
}

void ObjectEffectData::EndTrail( int TrailIndex )
{
	assert(TrailIndex >= 0 && TrailIndex < MAX_TRAIL);

	if ( m_hTrailEffect[TrailIndex] != INVALID_HANDLE_VALUE)
	{
		if (m_bTrailActivate[TrailIndex] == TRUE ) 
		{
			g_pEffectManager->RemoveEffectPtr(m_hTrailEffect[TrailIndex]);

			m_hTrailEffect[TrailIndex] = INVALID_HANDLE_VALUE;
			m_bTrailActivate[TrailIndex] = FALSE;
		}
	}
}

int ObjectEffectData::GetTrailAttachmentIndex( int TrailIndex )
{
	TrailIndex = min( TrailIndex, MAX_TRAIL-1 );
	TrailIndex = max( 0, TrailIndex);

	return m_arTrailParam[TrailIndex].iAttachmentIndex;
}

void ObjectEffectData::UpdateTrail( int TrailIndex, const WzVector& Position1, const WzVector& Position2 )
{
	HANDLE hEffect = m_hTrailEffect[TrailIndex];
	if( hEffect == INVALID_HANDLE_VALUE )
		return;

	if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hEffect) )
	{
		if( CEElementFlatChain* pFChain = pBody->GetFlatChainPtr() )
		{
			pFChain->SetPosition( Position1, Position2 );
		}
	}
	else
	{
		m_hTrailEffect[TrailIndex] = INVALID_HANDLE_VALUE;
		m_bTrailActivate[TrailIndex] = FALSE;
	}
}

void ObjectEffectData::ReleaseAllEffect()
{
	for (int i = 0; i < MAX_EFFECT_BUFFER; ++i)
	{
		for (int j = 0; j < MAX_OBJECT_EFFECT; ++j)
		{
			ReleaseEffect( j, i );
		}
	}
}

void ObjectEffectData::ReleaseLoopEffect()
{
	for (int i = 0; i < MAX_EFFECT_BUFFER; ++i)
	{
		for (int j = 0; j < MAX_OBJECT_EFFECT; ++j)
		{			
			if( m_EffectParam[i][j].hEffect != INVALID_HANDLE_VALUE )
			{	
				if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_EffectParam[i][j].hEffect) )
				{
					if( pBody->IsLoop() == true )
					{
						ReleaseEffect(j,i);
					}
				}
			}
		}
	}
}

void ObjectEffectData::ReleaseEffect( int Index, int BufferIndex )
{
	//assert(Index >= 0 && Index < MAX_OBJECT_EFFECT);

	if (m_EffectParam[BufferIndex][Index].hEffect != INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_EffectParam[BufferIndex][Index].hEffect);

		m_EffectParam[BufferIndex][Index].hEffect = INVALID_HANDLE_VALUE;
		m_EffectParam[BufferIndex][Index].iBoneIndex = 0;
		m_EffectParam[BufferIndex][Index].bRotateWithBone = FALSE;
		m_EffectParam[BufferIndex][Index].EffectID = 0;
		m_EffectParam[BufferIndex][Index].bLoopEffect = FALSE;
	}
}

void ObjectEffectData::ReleaseCurEffect(int Index)
{
	//라이프타임이 살아있는 이펙트일때는 바로 제거하지말구
	//소멸할때까지 PRE_EFFECT에 저장한뒤 소멸시킨다.

	if(m_EffectParam[CUR_EFFECT][Index].hEffect!=INVALID_HANDLE_VALUE)
	{
		if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_EffectParam[CUR_EFFECT][Index].hEffect) )
		{

			//중복해서 또 이펙트 들어온다면 PRE_EFFECT는 해제한다
			ReleaseEffect( Index, PRE_EFFECT );

			//중복이펙트는 제거한다.
			if(pBody->IsLoop()) 
			{
				return;
			}

			m_EffectParam[PRE_EFFECT][Index].iBoneIndex		=	m_EffectParam[CUR_EFFECT][Index].iBoneIndex;
			m_EffectParam[PRE_EFFECT][Index].vOffset		=	m_EffectParam[CUR_EFFECT][Index].vOffset;
			m_EffectParam[PRE_EFFECT][Index].quatRot		=	m_EffectParam[CUR_EFFECT][Index].quatRot;
			m_EffectParam[PRE_EFFECT][Index].vScale			=	m_EffectParam[CUR_EFFECT][Index].vScale;
			m_EffectParam[PRE_EFFECT][Index].bRotateWithBone=	m_EffectParam[CUR_EFFECT][Index].bRotateWithBone;
			m_EffectParam[PRE_EFFECT][Index].bOnlyPosition	=	m_EffectParam[CUR_EFFECT][Index].bOnlyPosition;
			m_EffectParam[PRE_EFFECT][Index].hEffect		=	m_EffectParam[CUR_EFFECT][Index].hEffect;
		}
	}
}

BOOL ObjectEffectData::BeginEffect( CWzUnitDraw* pUnitDraw, const WzVector& Position, const WzVector& Direction, const EffectEventInfo& EffectEventInfo, DWORD ObjectKey )
{
	int EffectIndex = EffectEventInfo.GetEventIndex();

	EFFECT_PARAM& CurEffectParam = m_EffectParam[CUR_EFFECT][EffectIndex];

	// 1. 이펙트 핸들에 대한 처리
	HANDLE hEffect = INVALID_HANDLE_VALUE;
	if	(CurEffectParam.bLoopEffect == TRUE &&
		(CurEffectParam.EffectID == EffectEventInfo.GetEffectID()))
	{
		// 루프 이펙트이며, 현재 이펙트와 같으면 그대로 사용
		hEffect = CurEffectParam.hEffect;
	}
	else
	{
		// 현재 진행중인 이펙트를 제거한다. 
		ReleaseEffect( EffectIndex, CUR_EFFECT );

        WzVector EffectPosition = Position;

        // 본 위치를 사용 하는 경우 본의 위치를 구한다.
        if( EffectEventInfo.GetBoneIndex() > 0 )
        {
		    if( GameFunc::GetBonePos( pUnitDraw, EffectEventInfo.GetBoneIndex(), &EffectPosition) == FALSE )
		    {
			    return FALSE;
		    }
        }

		// 이펙트 생성
		hEffect = GlobalFunc::CreateEffectBeUpToOption( EffectEventInfo.GetEffectID(), EffectPosition, ObjectKey );
	}

	// 2. 핸들과 이벤트의 성정된 정보를 설정
	CurEffectParam.hEffect = hEffect;
	CurEffectParam.iBoneIndex = EffectEventInfo.GetBoneIndex();;
	CurEffectParam.vOffset = EffectEventInfo.GetEffectPosition();
	CurEffectParam.vScale = EffectEventInfo.GetEffectScale();
	CurEffectParam.quatRot = EffectEventInfo.GetEffectQuaternion();


	// 3. 옵션에 따른 적용
	// 200일때 회전적용무 오직 위치로만
	if ( EffectEventInfo.IsLocationOnly() == true )
	{
		CurEffectParam.bOnlyPosition = TRUE;
		CurEffectParam.bRotateWithBone = FALSE;
	}
	else if ( EffectEventInfo.IsRotate() == true )
	{
		CurEffectParam.bOnlyPosition = FALSE;
		CurEffectParam.bRotateWithBone = TRUE;
	}
	else
	{
		CurEffectParam.bRotateWithBone =  CurEffectParam.bOnlyPosition = FALSE;
	}


	// 4. 실제 이펙트에 적용
	if( CEffectBody* effect_body = g_pEffectManager->GetEffectPtrHandle( hEffect) )
	{
		CurEffectParam.EffectID = EffectEventInfo.GetEffectID(); // 이펙트가 생성 된 후에 이펙트 아이디를 설정
		CurEffectParam.bLoopEffect = effect_body->IsLoop();	// 루프 여부는 이펙트의 설정에서 가져옴



		WzVector world_pos;
        WzMatrix mat_rot;
        MatrixIdentity(&mat_rot);

		if( CurEffectParam.iBoneIndex == -1 )   // 기본 위치에 생성하는 경우
		{
			world_pos = Position;

            if (CurEffectParam.bOnlyPosition)
            {
                //본 회전적용안함
                MatrixIdentity(&mat_rot);
            }
            else
            {
                WzVector rotation = {0.f, 0.f, Math_GetAngleFromVector(&Direction)};
                MatrixRotationZYX(&mat_rot, &rotation);

                // 로테이션이 적용된 Offset만큼 이동
                VectorTransformCoord(&rotation, &CurEffectParam.vOffset, &mat_rot);
                world_pos += rotation;
            }
        }
        else if( CurEffectParam.iBoneIndex == -2 )  // 고정된 위치에 생성하는 경우
        {
			world_pos = Position + CurEffectParam.vOffset;
            CurEffectParam.bOnlyPosition = FALSE;
            CurEffectParam.bRotateWithBone = FALSE;

            WzVector rotation = {0.f, 0.f, Math_GetAngleFromVector(&Direction)};
            MatrixRotationZYX(&mat_rot, &rotation);

            // 로테이션이 적용된 Offset만큼 이동
            VectorTransformCoord(&rotation, &CurEffectParam.vOffset, &mat_rot);
            world_pos += rotation;
		}
		else                                        // 본위치를 기준으로 생성하는 경우
		{
            WzMatrix mat;
            if( GameFunc::GetBoneMatrix( pUnitDraw, EffectEventInfo.GetBoneIndex(), &mat ) == FALSE )
            {
                return FALSE;
            }

			VectorTransformCoord(&world_pos, &CurEffectParam.vOffset, &mat );

            if (CurEffectParam.bOnlyPosition)
            {
                //본 회전적용안함
                MatrixIdentity(&mat_rot);
            }
            else if(CurEffectParam.bRotateWithBone)
            {
                //본 회전적용
                WzMatrix tmep_mat;
                QuaternionToMatrix(&tmep_mat, &CurEffectParam.quatRot);
                MatrixMultiply( &mat_rot, &tmep_mat, &mat);
                mat_rot._42 =mat_rot._41 = mat_rot._43 = 0.0f;
            }
            else
            {
                // 회전적용
                WzVector rotation = {0.f, 0.f, Math_GetAngleFromVector(&Direction)};
                MatrixRotationZYX(&mat_rot, &rotation);
            }
		}

		// 위치행렬
		WzMatrix mat_pos;
		MatrixTranslation( &mat_pos, world_pos.x, world_pos.y, world_pos.z );


        // 스케일
		WzMatrix mat_scale;
		MatrixScaling(&mat_scale, CurEffectParam.vScale.x, CurEffectParam.vScale.y, CurEffectParam.vScale.z);
		effect_body->SetResultMatrix( &mat_pos, &mat_rot, &mat_scale);


		return TRUE;
	}

	return FALSE;
}

void ObjectEffectData::UpdateEffect( CWzUnitDraw* pUnitDraw, const WzVector& Position, const WzVector& Direction )
{
	static const int FORCE_TRANSFROM = -2;

	for ( int i=0; i < GetEffectBufferCount(); ++i )
	{
		for ( int j = 0; j < GetEffectCount(); ++j )
		{
			EFFECT_PARAM& EffectParam = m_EffectParam[i][j];

			if (EffectParam.hEffect != INVALID_HANDLE_VALUE)
			{
				if ( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(EffectParam.hEffect) )
				{
					// 본인스값이 강제 변환일때는 갱신하지않는다
					if (EffectParam.iBoneIndex == FORCE_TRANSFROM)
					{
						continue;
					}

					WzVector wvPos, wvWorldPos;
					WzMatrix wmPosMat, wmRotMat, wmRot, wmScale, mat;

					wvPos = EffectParam.vOffset;


					if( EffectParam.iBoneIndex == -1 )
					{
						wvWorldPos = Position;

                        WzVector rotation = {0.f, 0.f, Math_GetAngleFromVector(&Direction)};
                        MatrixRotationZYX(&wmRotMat, &rotation);

                        // 로테이션이 적용된 Offset만큼 이동
                        VectorTransformCoord(&rotation, &EffectParam.vOffset, &wmRotMat);
                        wvWorldPos += rotation;
					}
					else
					{
                        if( GameFunc::GetBoneMatrix( pUnitDraw, EffectParam.iBoneIndex, &mat ) == FALSE )
                        {
                            continue;
                        }

						VectorTransformCoord( &wvWorldPos, &wvPos, &mat );
					}

					MatrixTranslation( &wmPosMat, wvWorldPos.x, wvWorldPos.y, wvWorldPos.z );


					if(EffectParam.bOnlyPosition)
					{
						MatrixIdentity( &wmRotMat );
					}
					else if( EffectParam.bRotateWithBone )
					{
						QuaternionToMatrix( &wmRot, &EffectParam.quatRot );
						MatrixMultiply( &wmRotMat, &wmRot, &mat );
						wmRotMat._41 = 0.0f;
						wmRotMat._42 = 0.0f;
						wmRotMat._43 = 0.0f;
					}
					else
					{
						MatrixIdentity( &wmRotMat );

						WzVector vPos;
						vPos = wvWorldPos;

						WzVector vDest,vTemp;
						vDest = vPos + Direction*10.0f;
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


                    MatrixScaling( &wmScale, EffectParam.vScale.x, EffectParam.vScale.y, EffectParam.vScale.z );
                    pBody->SetResultMatrix( &wmPosMat, &wmRotMat, &wmScale );

				}
				else
				{
					//자동 NULL셋팅
					EffectParam.hEffect = INVALID_HANDLE_VALUE;
				}
			}
		}
	}
}
