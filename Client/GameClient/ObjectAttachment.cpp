#include "SunClientPrecompiledHeader.h"
#include "ObjectAttachment.h"
#include "ResourceManager.h"
#include "GameFunc.h"
#include "GlobalFunc.h"


const DWORD   ciInterpolateTick		= 400;
const float	  cAttachmentAccelater = 2.0f;


ObjectAttachment::ObjectAttachment()
{
	m_pUnitDraw = NULL;
	m_bAnimate = FALSE;
	m_animID = 0;
}

BOOL ObjectAttachment::InitializeAttachment( BOOL bPlayer, const AttachmentInfo& AttachmentInfo, float fRequestRateResizeTexture)
{
	if (AttachmentInfo.GetResourceID() == 0)
	{
		ReleaseAttachment();
		return FALSE;
	}

	// ������ �Ҵ�Ȱ��� �ִٸ� Delete �����ش�.
	if (m_pUnitDraw)
	{
		if ((m_pUnitDraw->GetWzUnit() == NULL) || 
            (m_pUnitDraw->GetWzUnit()->GetType() != AttachmentInfo.GetResourceID()) )
		{
			ResourceManager::Instance()->FreeResourceUnitDraw(m_pUnitDraw);

			if( bPlayer == TRUE )
				m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw( AttachmentInfo.GetResourceID(), fRequestRateResizeTexture, false);
			else
				m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw( AttachmentInfo.GetResourceID(), fRequestRateResizeTexture, true );
		}		
	}
	else
	{
		if( bPlayer == TRUE )
			m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw( AttachmentInfo.GetResourceID(), fRequestRateResizeTexture, false);
		else
			m_pUnitDraw = ResourceManager::Instance()->AllocResourceUnitDraw( AttachmentInfo.GetResourceID(), fRequestRateResizeTexture, true );
	}

	//���ҽ��ڵ� & ��þƮ & ����� �� ���ٸ� ���������ʿ����
	if (( m_AttachmentInfo.GetResourceID() == AttachmentInfo.GetResourceID() ) &&
		( m_AttachmentInfo.GetSubType() == AttachmentInfo.GetSubType() ) &&
		( m_AttachmentInfo.IsSpecularVariation() == AttachmentInfo.IsSpecularVariation() ) )
	{
		return FALSE;
	}

	if ( m_pUnitDraw != NULL )
	{
		if ((m_pUnitDraw->GetWzUnit() != NULL) && 
            (AttachmentInfo.GetSubType() < m_pUnitDraw->GetWzUnit()->GetNumSubType()))
		{
			//����Ÿ���� �ٸ����� ���ش�
			m_pUnitDraw->SetSubType( AttachmentInfo.GetSubType() );			
		}
		else
		{
			m_pUnitDraw->SetSubType(0);
		}
	}

	// Set Info
	m_AttachmentInfo = AttachmentInfo;

	if ( m_AttachmentInfo.GetAttachType() == ATTACHTYPE_INTERPOLATION )
	{
		SetVector(&m_wvInterpolateOffsetAttachment, 0.f, 0.f, 0.f);			
		m_iInterpolateTick = 0;
		m_fAccelerate = cAttachmentAccelater;
	}	

	return TRUE;

}

BOOL ObjectAttachment::ReleaseAttachment()
{
	if ( m_pUnitDraw != NULL )
	{	
		ResourceManager::Instance()->FreeResourceUnitDraw(m_pUnitDraw);
		m_pUnitDraw = NULL;
		m_animID = 0;
		m_bAnimate = FALSE;

		m_AttachmentInfo.SetResourceID(0);
		m_AttachmentInfo.SetBoneIndex(-1);
        m_AttachmentInfo.SetItemCode(0);
	}

	return TRUE;
}

void ObjectAttachment::Animate( CWzUnitDraw* pBasePart, const WzVector& VisiblePosition, float fHeightOffset )
{
	if ( m_pUnitDraw == NULL )
		return;


	int BoneIndex = m_AttachmentInfo.GetBoneIndex();
    const WzMatrix& rotate_matrix = m_AttachmentInfo.GetRotateMatrix();

    WzMatrix offset_matrix;
    MatrixTranslation(&offset_matrix, 
                        m_AttachmentInfo.GetOffset().x,
                        m_AttachmentInfo.GetOffset().y,
                        m_AttachmentInfo.GetOffset().z);

	// ������ ���������ʴ´� attachment(������ Ư��)
	if ( BoneIndex == -1 )
	{
		//���� offsetMatrix������ ���� z���� �ɼ¸� �����ϰ��ִ�
		WzMatrix mat;
		MatrixIdentity(&mat);

        // ��ġ�� �ɼ�
        WzMatrix ret_mat;
        MatrixMultiply(&ret_mat, &offset_matrix, &mat);
        mat = ret_mat;

        //! ȸ���� �����Ų��.i
        MatrixMultiply(&ret_mat, &rotate_matrix, &mat);
        mat = ret_mat;

		mat._41 = VisiblePosition.x;
		mat._42 = VisiblePosition.y;
		mat._43 += VisiblePosition.z + fHeightOffset;
		g_pSunRenderer->AnimateUnit(m_pUnitDraw, 0, &mat);	

	}
	else if( BoneIndex < pBasePart->GetTransformBufferSize() )
	{
		//������ �����̴� Attachment
		WzMatrix* pBoneMatrix = pBasePart->GetTransformBuffer();	

		if ( pBoneMatrix == NULL)
		{
			return;
		}

		WzMatrix mat = pBoneMatrix[BoneIndex];

        // ��ġ�� �ɼ�
        WzMatrix ret_mat;
        MatrixMultiply(&ret_mat, &offset_matrix, &mat);
        mat = ret_mat;

        //! ȸ���� �����Ų��.
        MatrixMultiply(&ret_mat, &rotate_matrix, &mat);
        mat = ret_mat;

		if ( GetAttachmentInfo().GetAttachType() == ATTACHTYPE_INTERPOLATION  )
		{ 	
			if (m_iInterpolateTick)
			{					
				WzVector dest_pos;
				dest_pos.x = mat._41;
				dest_pos.y = mat._42;
				dest_pos.z = mat._43;

				float rate = 
					(float)(ciInterpolateTick - m_iInterpolateTick) / (float) ciInterpolateTick;

				rate = min(rate, 1.0f);
				rate = max(rate, 0.0f);
				WzVector cur_pos = dest_pos;

				//���� ����
				cur_pos = m_vOldPos * (1.0f - rate) + dest_pos * (rate);
				mat._41 =cur_pos.x;
				mat._42 =cur_pos.y;
				mat._43 =cur_pos.z;
				g_pSunRenderer->AnimateUnit(m_pUnitDraw, 0, &mat);	

			}
			else
			{											
				g_pSunRenderer->AnimateUnit(m_pUnitDraw, 0, &mat);	
			}						
		}				
		else
		{
			g_pSunRenderer->AnimateUnit(m_pUnitDraw, 0, &mat);	
		}
	}
}

void ObjectAttachment::SetAnimation(WzID animID, BOOL bLoop, BOOL bRestart)
{
	if (animID == 0) 
	{
		m_animID = animID;
		m_bAnimate = FALSE;
		if (m_pUnitDraw)
		{
			m_pUnitDraw->SetFrame(0.0f);
		}
	}
	else
	{
		m_animID = animID;
		m_bAnimate = TRUE;

        if (bLoop == TRUE)
        {
            bRestart = FALSE;
        }

		if (m_pUnitDraw && m_pUnitDraw->GetWzUnit())
		{
			//wzu���Ͽ� �ش� �ִ�ID�� �ִٸ� SetCurrentAnimation
			BOOL bYes = FALSE;
			int iCnt = m_pUnitDraw->GetWzUnit()->GetCountAnimationInfo();
			for(int i = 0; i < iCnt; i++)
			{
				WzAnimationInfo* pAI = m_pUnitDraw->GetWzUnit()->GetAnimationInfoByIndex(i);
				if( (pAI) && (animID == pAI->m_wiAction) )
				{
					bYes = TRUE;
					break;
				}
			}

			if(bYes)
				m_pUnitDraw->SetCurrentAnimation(animID, bLoop, bRestart);
			else
				m_pUnitDraw->SetCurrentAnimation(0, bLoop, bRestart);

		}
	}
}

void ObjectAttachment::ProcessAnimation( DWORD dwTick )
{
	if( m_pUnitDraw != NULL )
	{
		int BoneIndex = m_AttachmentInfo.GetBoneIndex();
		// �Ŀ� boneindex�� ��ȿ���� üũ�ϴ� ��ƾ �ʿ�.

		if (BoneIndex != -1) 
		{					
			if (m_bAnimate) 
			{
				g_pSunRenderer->ProgressUnitFrame( m_pUnitDraw, (float)dwTick);	
			}
			else
			{
				g_pSunRenderer->ProgressUnitFrame( m_pUnitDraw, 0.0f);	
			}
		}
		else
		{
			if (m_bAnimate) 
			{
				g_pSunRenderer->ProgressUnitFrame( m_pUnitDraw, (float)dwTick);	
			}
			else
			{
				g_pSunRenderer->ProgressUnitFrame( m_pUnitDraw, 1);	                       
			}
		}
	}
}

void ObjectAttachment::ProcessInterpolation( CWzUnitDraw* pBasePart, DWORD dwTick )
{
	if ( m_pUnitDraw == NULL ) 
	{
		return;
	}

	if ( m_AttachmentInfo.GetAttachType() != ATTACHTYPE_INTERPOLATION ) 
	{
		return;
	}

	// ����
	WzVector vDestPos;
	if ( GameFunc::GetBonePos( pBasePart, m_AttachmentInfo.GetBoneIndex(), &vDestPos) )
	{
		const WzVector& wvCurPos = m_pUnitDraw->GetPosition();			
		m_wvInterpolateOffsetAttachment = vDestPos - wvCurPos;

		if (m_iInterpolateTick <= 0)
		{
			m_iInterpolateTick = 0;
		}
		else
		{
			//ƽ�� Ŀ���� ���� �̵� �Ѵ�.
			// ApplyInterpolateAccesory ���� �� ���� �־�� ��.�ʱ�ȭ ���ִµ��� 				
			m_fAccelerate -= 0.084f * dwTick;
			m_fAccelerate = max(m_fAccelerate, 0.5f);
			dwTick = DWORD(float(dwTick) * m_fAccelerate);
			m_iInterpolateTick -= dwTick;
		}
	}
}

void ObjectAttachment::Render()
{
	if ( m_pUnitDraw == NULL ) 
		return;


	if ( g_pSunRenderer->CheckUnitLoaded(m_pUnitDraw) == FALSE )
		return;

	if ( m_AttachmentInfo.IsSpecularVariation() )
	{
		m_pUnitDraw->SetSpecularIntensity(g_fSpecularVariation);
	}

	BOOL bReslt = g_pSunRenderer->RenderUnit( m_pUnitDraw );				
	if ( bReslt == FALSE )
	{
		//COMMENT:�α�
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CONSOLE, "ObjectAttachment::Render - %s(%d)",  __FILE__, __LINE__ );
	}
}

void ObjectAttachment::ApplyInterpolateAccesory( CWzUnitDraw* pBasePart )
{
	if ( pBasePart == NULL )
		return;

	if ( m_pUnitDraw == NULL )
		return;

	if ( m_AttachmentInfo.GetAttachType() != ATTACHTYPE_INTERPOLATION )
		return;

	if ( m_iInterpolateTick > 0 )
		return;

	int BoneIndex = m_AttachmentInfo.GetBoneIndex();
	if ( BoneIndex == -1 )
		return;

	if ( WzMatrix* pBoneMatrix = pBasePart->GetTransformBuffer() )
	{
		WzMatrix mat = pBoneMatrix[BoneIndex];
		
        WzMatrix offset_matrix;
        MatrixTranslation(&offset_matrix, 
            m_AttachmentInfo.GetOffset().x,
            m_AttachmentInfo.GetOffset().y,
            m_AttachmentInfo.GetOffset().z);
        WzMatrix ret_mat;
        MatrixMultiply(&ret_mat, &offset_matrix, &mat);
        mat = ret_mat;

		m_iInterpolateTick = ciInterpolateTick;
		m_fAccelerate = cAttachmentAccelater;

		m_vOldPos.x = mat._41;
		m_vOldPos.y = mat._42;
		m_vOldPos.z = mat._43;
	}
}


