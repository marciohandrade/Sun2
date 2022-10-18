//------------------------------------------------------------------------------
//  LottNPC.cpp
//  (C) 2009 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"


#include "LottoNPC.h"
#include "NPC.h"
#include "uiLottoMan/uiLottoMan.h"
#include "ResourceManager.h"
#include "objectmanager.h"

//------------------------------------------------------------------------------
/** 
*/
LottoNPC::LottoNPC( void )
:m_bNumberAni(FALSE),
m_bLottoActive(TRUE),
m_iOldNumber(0)
{
	SetObjectType(LOTTO_NPC_OBJECT);
	
	memset(&m_ResInfo,NULL,sizeof(m_ResInfo));
	memset(&m_ppRenderPart,NULL,sizeof(m_ppRenderPart));
	memset(&m_ppNumberUnit,NULL,sizeof(m_ppNumberUnit));
	memset(&m_arBonPosition,-1,sizeof(m_arBonPosition));
	memset(&m_arCurNumber,-1,sizeof(m_arCurNumber));
	memset(&m_arOldNumber,-1,sizeof(m_arOldNumber));
}
//------------------------------------------------------------------------------
/** 
*/
LottoNPC::~LottoNPC( void )
{

}
//------------------------------------------------------------------------------
/** 
*/
BOOL LottoNPC::Create( DWORD dwObjectKey, DWORD dwID)
{
	BOOL bRet =  NPC::Create(dwObjectKey,dwID);

	//step1:���ڿ�����Ʈ ���ҽ��ڵ�(������+��������)
	m_ResInfo.m_rNumberCode  = ENGINE_RESOURCE_ID_OFFSET + 5045;
	m_ResInfo.m_rBaseCode    = ENGINE_RESOURCE_ID_OFFSET + 5044;

	//step2:���� ���ҽ��ڵ带 �ε�
	LoadNumberObject();

	//step3:�����Ƕ��⿡ �׷��ֱ����� �Ƕ����� ���ں���ġ ����
	ExtractBoneIndex();	

	//step4:����Ʈ�� �����ʱ�ȭ
	RefreshNumber();

	//step5:���������� ���Žÿ��� ���������������⶧���� ������ npc�ʱ�ȭ�� Ŭ�󿡼� ���� ��û���ش�.
	uiLottoMan* pLottoMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
	
	if(pLottoMan)
	{
		pLottoMan->SEND_LOTTO_INFO_SYN(false);
	}
	
	//step6:�ζǸŴ����� �ζǿ�����ƮŰ�������(�ζ���������(����)�� �Ŵ��������� �̷�����)
	uiLottoMan* pLoginMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
	if(pLoginMan)
	{
		pLoginMan->RegisterLottoKey(this->GetObjectKey());
	}

	SetShadowDraw(TRUE);
	if(m_pBasePart)
	{	
		m_pBasePart->EnableShadowCast(TRUE);
		m_pBasePart->EnableShadowReceive(TRUE);
	}

    InitMeshHeight();

	return bRet;
}
//------------------------------------------------------------------------------
/** ���ҽ�����
*/
void LottoNPC::Destroy( void )
{
	FreeNumberObject();

	NPC::Destroy();
}

//------------------------------------------------------------------------------
/** ���ҽ��ε�
*/
void LottoNPC::LoadNumberObject()	
{
	//step1:���̽� ����(�����Ƕ���)
	m_pBasePart = ResourceManager::Instance()->AllocResourceUnitDraw(m_ResInfo.m_rBaseCode);

	int i=0;

	//step2:wzu(�������Ϸε�)
	for( i = 0 ; i < NUM_MAX_UNIT ; ++i )
	{
		m_ppNumberUnit[i] = ResourceManager::Instance()->AllocResourceUnit(m_ResInfo.m_rNumberCode+i);
		assert(m_ppNumberUnit[i] && "���ҽ��� �����ϴ�.");
	}

	//step3:������ �׷��� UnitDraw�ε�
	for( i = 0;i<MAX_BONE_POS;++i)
	{
        m_ppRenderPart[i] = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0);

		assert(m_ppRenderPart[i]);
	}
}
//------------------------------------------------------------------------------
/** �����Ƕ���+���ڸ��ҽ�����
*/
void LottoNPC::FreeNumberObject()
{
	//step1:���̽� ����(�����Ƕ���)
	if(m_pBasePart)
	{
		ResourceManager::Instance()->FreeResourceUnitDraw(m_pBasePart);
	}
	
	//step2:wzu(�������Ϸε�)
	int i=0;
	
	for( ; i < NUM_MAX_UNIT ; ++i )
	{
		ResourceManager::Instance()->FreeResourceUnit(m_ResInfo.m_rNumberCode+i);
		m_ppNumberUnit[i]= NULL;
	}

	//step3:������ �׷��� UnitDraw�ε�
	for( i = 0 ; i < MAX_BONE_POS ; ++i )
	{
		if(m_ppRenderPart[i])
		{
			g_pSunRenderer->DestroyResourceForWzUnitDraw(m_ppRenderPart[i]);

            Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_ppRenderPart[i] );

			m_ppRenderPart[i] =NULL;
		}
	}

}
//------------------------------------------------------------------------------
/** �ζ� Ȱ��.��Ȱ��
*/
void LottoNPC::SetLottoActive(BOOL bActive)
{
	if(bActive==m_bLottoActive)
	{
		return;
	}

	m_bLottoActive = bActive;

	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	if(!pLottoMan)
	{
		return;
	}
	 //��Ȱ��(����):��÷ä�ø޽������Բ� �������� ���ڰ� ��ӹٲ��
	if(m_bLottoActive)
	{
		m_iOldNumber=-1;
		SetDrawNumber(pLottoMan->GetLottoInfo()->m_i64TotalAccum);
	}
	else
	{
		//5286 %dȸ �ζ� ��÷�� ���۵Ǿ����ϴ�.
		TCHAR	szTemp[INTERFACE_STRING_LENGTH];
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(5286, szTemp);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH, szTemp,pLottoMan->GetLottoInfo()->m_wLottoDrawNum);
		AddChatMessage(szMessage);
	
		for(int a=0;a<MAX_BONE_POS;++a)
		{
			m_arOldNumber[a]=a%9;
		}
	}
}
//------------------------------------------------------------------------------
/** NPC��������
*/
void LottoNPC::SetExtraInfo( EXTRA_NPCINFO	* pExtraInfo)
{
	NPC::SetExtraInfo(pExtraInfo);

	if(m_pBasePart)
	{
		m_pBasePart->SetPosition(pExtraInfo->m_fNPCPos[0],pExtraInfo->m_fNPCPos[1],pExtraInfo->m_fNPCPos[2]);
	}

}
//------------------------------------------------------------------------------
/** ������Ʈ
*/
BOOL LottoNPC::Process( DWORD dwTick )
{
	//�⺻ �����Ǳ׸���
	if(!m_pBasePart)
	{
		return FALSE;
	}

	g_pSunRenderer->ProgressUnitFrame( m_pBasePart,0.0f);
	g_pSunRenderer->AnimateUnit(m_pBasePart);

	//���������� ���ڵ�
	for( int i = 0 ; i < MAX_BONE_POS ; ++i )
	{
		if(!m_ppRenderPart[i])
		{
			continue;
		}

		g_pSunRenderer->ProgressUnitFrame(m_ppRenderPart[i],0.0f);

		if(m_arBonPosition[i]==-1)
		{
			continue;
		}

		WzMatrix *pBoneMatrix = m_pBasePart->GetTransformBuffer();

		//����ġ ���ڸ� ��ģ��
		if(pBoneMatrix)
		{
			g_pSunRenderer->AnimateUnit(m_ppRenderPart[i],0,&pBoneMatrix[m_arBonPosition[i]]);
		}
	}


	SetBillBoard();

	BOOL bRet =  Character::Process(dwTick);

	//��Ȱ�������ϰ��(������)
	if(!m_bLottoActive)
	{
		SetChatDisplayTime(1000);
		AnimationNumberChaos(dwTick);
		RefreshNumber();
	}
	else
	{
		//���ھִϸ��̼�
		if(m_bNumberAni)
		{
			AnimationNumber(dwTick);
			RefreshNumber();
		}
	}

	return bRet;
}
//------------------------------------------------------------------------------
/**  ������
*/
BOOL LottoNPC::Render()
{
	//������
	if(!m_pBasePart)
	{
		return FALSE;
	}

	g_pSunRenderer->RenderUnit( m_pBasePart);

	//���ڵ�
	for( int i = 0 ; i < MAX_BONE_POS ; ++i )
	{
		if(!m_ppRenderPart[i])
		{
			continue;
		}
		
		g_pSunRenderer->RenderUnit( m_ppRenderPart[i] );
	}

	return NPC::Render();
}


//------------------------------------------------------------------------------
/** ���ε�������(���ڸ��� ������ġ)
*/
void LottoNPC::ExtractBoneIndex()
{
	if(!m_pBasePart) 
	{
		return;
	}

	CWzUnit* pUnit  = m_pBasePart->GetWzUnit();	

	//���־����� ����
	if(!pUnit)
	{
		return;
	}

	TCHAR   szNumber[30];

	for(int i=0;i<MAX_BONE_POS;++i)
	{
		//arg1: 10���ڸ�	arg2:1���ڸ�
		Sprintf (szNumber, _T("NO%d%d"), i/10,i%10);
		WzBoneInfo *pInfo  = pUnit->GetBoneInfo(StrToWzID(szNumber));
		m_arBonPosition[i] = (pInfo ? (pInfo->m_iNode):(-1));
	}

}
//------------------------------------------------------------------------------
/** ���ھִϸ��̼�(���ڰ� �ǷηϿö󰡴� ȿ��)
*/
void LottoNPC::AnimationNumber(DWORD dwTick)
{
	static DWORD dwUpdateTick = 0;

	//�������ݸ��� �����Ѵ�.
	dwUpdateTick+=dwTick;
	if(dwUpdateTick < 50) 
	{
		return;
	}
	
	dwUpdateTick = 0;

	int nFindTopIndex = -1;

	//�ֻ������ڸ� ã�´�.
	int i=0;
	for(;i<MAX_BONE_POS;++i)
	{
		//���� ������ �ﰢ����
		if(m_arCurNumber[i] == -1)
		{
			m_arOldNumber[i] = m_arCurNumber[i];
		}
		else
		{
			//�������� �����δ� �˴� �ø���.
			if( nFindTopIndex == -1 && m_arCurNumber[i]!=m_arOldNumber[i]) 
			{
				nFindTopIndex = i;

				if(++m_arOldNumber[i]>9)
				{
					m_arOldNumber[i] = 0;
				}

				break;
			}
		}
	}

	//���������� ������ �������� ++�����ش�.
	for( i=nFindTopIndex+1;i<MAX_BONE_POS;++i)
	{
		if(++m_arOldNumber[i]>9)
		{
			m_arOldNumber[i] = 0;
		}
	}

	//���̻� �ִ��Ұ� ���ٸ� �ִ�����
	if(nFindTopIndex==-1) 
	{
		memcpy(&m_arOldNumber,&m_arCurNumber,sizeof(m_arCurNumber));
		m_bNumberAni = FALSE;
	}
}
//------------------------------------------------------------------------------
/** ���ھִϸ��̼�(��Ģ���� ����ڸ����� ��ӵ���)
*/
void LottoNPC::AnimationNumberChaos(DWORD dwTick)
{
	static DWORD dwUpdateTickChaos = 0;

	//�������ݸ��� �����Ѵ�.
	dwUpdateTickChaos+=dwTick;
	if(dwUpdateTickChaos < 100) 
	{
		return;
	}

	dwUpdateTickChaos = 0;
	for(int i=0;i<MAX_BONE_POS;++i)
	{
		if(++m_arOldNumber[i]>9)
		{
			m_arOldNumber[i] = 0;
		}
	}
}
//------------------------------------------------------------------------------
/**	���ڸ� ���ڷ� ��ȯ�� ���ڸ����� �и��� �迭�� ��´�
*/
void LottoNPC::SetparateNumber(INT64 Number)
{
	//�ٲ����ʾҴٸ� ����!
	if(m_iOldNumber==Number && Number!=0)
	{
		m_bNumberAni = FALSE;
		return;
	}

	m_iOldNumber = Number;

	TCHAR szNumber[100]={0};	
	_i64toa(Number,szNumber,10);

	int iLen = strlen(szNumber);
	util::_tstring str(szNumber);

	memset(m_arCurNumber,-1,sizeof(m_arCurNumber));

	//�ڿ������� �ö󰡴� �ִϸ� ����
	if(Number!=0)
	{
		for(int a=0;a<MAX_BONE_POS;++a)
		{
			m_arOldNumber[a]=a%9;
		}
	}

	int nInsertPos=0;
	int	nCnt=0;

	for(int i=0;i<iLen;++i)
	{
		//���ڸ����� ����
		util::_tstring tempStr = str.substr((iLen-1)-i,1);
		int nNeedNum = atoi(tempStr.c_str());

		int nNumPos = (MAX_BONE_POS-1)-i;

		//15�ڸ��̻��� ��޾��Ѵ�.
		if(nNumPos<0)
		{
			continue;
		}

		m_arCurNumber[nNumPos] = nNeedNum;
	}

}
//------------------------------------------------------------------------------
/**	���ڸ� �����Ѵ�( NPC::SetDrawNumber������ ȣ��)
*/
void LottoNPC::SetDrawNumber(INT64 Number)
{
	//Ȱ��ȭ���¿����� �ްڴ�
	if(m_bLottoActive)
	{
		m_bNumberAni = TRUE;
		SetparateNumber(Number);
	}
}
//------------------------------------------------------------------------------
/** UnitDraw�� �����ִ� Unit�� ��ü���ش�(���ڱ�ü)
*/
void LottoNPC::RefreshNumber()
{
	for( int i = 0;i<MAX_BONE_POS;++i)
	{
		if(!m_ppRenderPart[i])
		{
			continue;
		}

		if(m_arOldNumber[i]== -1 || m_arOldNumber[i]>(NUM_MAX_UNIT-2) )
		{
			m_ppRenderPart[i]->SetWzUnit(m_ppNumberUnit[0],FALSE);
		}
		else
		{
			m_ppRenderPart[i]->SetWzUnit(m_ppNumberUnit[m_arOldNumber[i]+1],FALSE);
		}
	}
}
//------------------------------------------------------------------------------
/** ������ ������
*/
void LottoNPC::SetBillBoard()
{
	static WzVector vOldAxisCam; 

	//ī�޶� ��ľ��
	WzMatrix wmViewMat;
	g_pSunRenderer->GetViewMatrix(&wmViewMat);

	//X�࿡���� ������
	WzVector vAxisOrg,vAxisCam;

	//ī�޶� ��Ʈ������ Right(x��)���� ����
	SetVector(&vAxisCam,wmViewMat._11, wmViewMat._21,wmViewMat._31);
	
	//���� �ٲ�����븸 �Ѵ�.
	if(vAxisCam==vOldAxisCam)
	{
		return;
	}
	
	SetVector(&vAxisOrg,1.0f,0.0f,0.0f);

	//ȸ������
	WzVector vRotAxis;	
	VectorCrossProduct( &vRotAxis, &vAxisOrg, &vAxisCam);
	VectorNormalize( &vRotAxis, &vRotAxis);

	//����ȸ����Ʈ�������
	WzMatrix wmFinalRot;
	MatrixRotation(&wmFinalRot, &vRotAxis, acosf( VectorDotProduct( &vAxisOrg, &vAxisCam)));
	m_pBasePart->SetRotationByMatrix(&wmFinalRot);

	vOldAxisCam = vAxisCam;
}



