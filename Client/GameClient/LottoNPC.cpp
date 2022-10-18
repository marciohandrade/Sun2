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

	//step1:숫자오브젝트 리소스코드(숫자판+숫자유닛)
	m_ResInfo.m_rNumberCode  = ENGINE_RESOURCE_ID_OFFSET + 5045;
	m_ResInfo.m_rBaseCode    = ENGINE_RESOURCE_ID_OFFSET + 5044;

	//step2:위에 리소스코드를 로드
	LoadNumberObject();

	//step3:숫자판때기에 그려주기위에 판때기의 숫자본위치 추출
	ExtractBoneIndex();	

	//step4:디폴트로 숫자초기화
	RefreshNumber();

	//step5:서버에서는 갱신시에만 숫자정보를보내기때문에 맵입장 npc초기화시 클라에서 정보 요청해준다.
	uiLottoMan* pLottoMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
	
	if(pLottoMan)
	{
		pLottoMan->SEND_LOTTO_INFO_SYN(false);
	}
	
	//step6:로또매니저에 로또오브젝트키를등록한(로또정보갱신(숫자)는 매니저를통해 이뤄진다)
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
/** 리소스해제
*/
void LottoNPC::Destroy( void )
{
	FreeNumberObject();

	NPC::Destroy();
}

//------------------------------------------------------------------------------
/** 리소스로드
*/
void LottoNPC::LoadNumberObject()	
{
	//step1:베이스 파츠(숫자판때기)
	m_pBasePart = ResourceManager::Instance()->AllocResourceUnitDraw(m_ResInfo.m_rBaseCode);

	int i=0;

	//step2:wzu(유닛파일로드)
	for( i = 0 ; i < NUM_MAX_UNIT ; ++i )
	{
		m_ppNumberUnit[i] = ResourceManager::Instance()->AllocResourceUnit(m_ResInfo.m_rNumberCode+i);
		assert(m_ppNumberUnit[i] && "리소스가 없습니다.");
	}

	//step3:유닛을 그려줄 UnitDraw로드
	for( i = 0;i<MAX_BONE_POS;++i)
	{
        m_ppRenderPart[i] = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0);

		assert(m_ppRenderPart[i]);
	}
}
//------------------------------------------------------------------------------
/** 숫자판때기+숫자리소스해제
*/
void LottoNPC::FreeNumberObject()
{
	//step1:베이스 파츠(숫자판때기)
	if(m_pBasePart)
	{
		ResourceManager::Instance()->FreeResourceUnitDraw(m_pBasePart);
	}
	
	//step2:wzu(유닛파일로드)
	int i=0;
	
	for( ; i < NUM_MAX_UNIT ; ++i )
	{
		ResourceManager::Instance()->FreeResourceUnit(m_ResInfo.m_rNumberCode+i);
		m_ppNumberUnit[i]= NULL;
	}

	//step3:유닛을 그려줄 UnitDraw로드
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
/** 로또 활성.비활성
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
	 //비활성(정산):추첨채팅메시지와함께 숫자판이 숫자가 계속바뀐다
	if(m_bLottoActive)
	{
		m_iOldNumber=-1;
		SetDrawNumber(pLottoMan->GetLottoInfo()->m_i64TotalAccum);
	}
	else
	{
		//5286 %d회 로또 추첨이 시작되었습니다.
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
/** NPC셋팅정보
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
/** 업데이트
*/
BOOL LottoNPC::Process( DWORD dwTick )
{
	//기본 숫자판그리기
	if(!m_pBasePart)
	{
		return FALSE;
	}

	g_pSunRenderer->ProgressUnitFrame( m_pBasePart,0.0f);
	g_pSunRenderer->AnimateUnit(m_pBasePart);

	//숫자판위의 숫자들
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

		//본위치 숫자를 부친다
		if(pBoneMatrix)
		{
			g_pSunRenderer->AnimateUnit(m_ppRenderPart[i],0,&pBoneMatrix[m_arBonPosition[i]]);
		}
	}


	SetBillBoard();

	BOOL bRet =  Character::Process(dwTick);

	//비활성상태일경우(정산중)
	if(!m_bLottoActive)
	{
		SetChatDisplayTime(1000);
		AnimationNumberChaos(dwTick);
		RefreshNumber();
	}
	else
	{
		//숫자애니메이션
		if(m_bNumberAni)
		{
			AnimationNumber(dwTick);
			RefreshNumber();
		}
	}

	return bRet;
}
//------------------------------------------------------------------------------
/**  랜더링
*/
BOOL LottoNPC::Render()
{
	//숫자판
	if(!m_pBasePart)
	{
		return FALSE;
	}

	g_pSunRenderer->RenderUnit( m_pBasePart);

	//숫자들
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
/** 본인덱스추출(각자릿수 숫자위치)
*/
void LottoNPC::ExtractBoneIndex()
{
	if(!m_pBasePart) 
	{
		return;
	}

	CWzUnit* pUnit  = m_pBasePart->GetWzUnit();	

	//유닛없으면 리턴
	if(!pUnit)
	{
		return;
	}

	TCHAR   szNumber[30];

	for(int i=0;i<MAX_BONE_POS;++i)
	{
		//arg1: 10의자리	arg2:1의자리
		Sprintf (szNumber, _T("NO%d%d"), i/10,i%10);
		WzBoneInfo *pInfo  = pUnit->GetBoneInfo(StrToWzID(szNumber));
		m_arBonPosition[i] = (pInfo ? (pInfo->m_iNode):(-1));
	}

}
//------------------------------------------------------------------------------
/** 숫자애니메이션(숫자가 또로록올라가는 효과)
*/
void LottoNPC::AnimationNumber(DWORD dwTick)
{
	static DWORD dwUpdateTick = 0;

	//일정간격마다 갱신한다.
	dwUpdateTick+=dwTick;
	if(dwUpdateTick < 50) 
	{
		return;
	}
	
	dwUpdateTick = 0;

	int nFindTopIndex = -1;

	//최상위숫자를 찾는다.
	int i=0;
	for(;i<MAX_BONE_POS;++i)
	{
		//숫자 낮으면 즉각적용
		if(m_arCurNumber[i] == -1)
		{
			m_arOldNumber[i] = m_arCurNumber[i];
		}
		else
		{
			//높은숫자 밑으로는 죄다 올린다.
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

	//마지막으르 제외한 나머지는 ++시켜준다.
	for( i=nFindTopIndex+1;i<MAX_BONE_POS;++i)
	{
		if(++m_arOldNumber[i]>9)
		{
			m_arOldNumber[i] = 0;
		}
	}

	//더이상 애니할게 없다면 애니종료
	if(nFindTopIndex==-1) 
	{
		memcpy(&m_arOldNumber,&m_arCurNumber,sizeof(m_arCurNumber));
		m_bNumberAni = FALSE;
	}
}
//------------------------------------------------------------------------------
/** 숫자애니메이션(규칙없이 모든자릿수가 계속돈다)
*/
void LottoNPC::AnimationNumberChaos(DWORD dwTick)
{
	static DWORD dwUpdateTickChaos = 0;

	//일정간격마다 갱신한다.
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
/**	숫자를 문자로 변환해 각자릿수를 분리해 배열에 담는다
*/
void LottoNPC::SetparateNumber(INT64 Number)
{
	//바뀌지않았다면 리턴!
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

	//자연스럽게 올라가는 애니를 위해
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
		//끝자리숫자 추출
		util::_tstring tempStr = str.substr((iLen-1)-i,1);
		int nNeedNum = atoi(tempStr.c_str());

		int nNumPos = (MAX_BONE_POS-1)-i;

		//15자리이상은 취급안한다.
		if(nNumPos<0)
		{
			continue;
		}

		m_arCurNumber[nNumPos] = nNeedNum;
	}

}
//------------------------------------------------------------------------------
/**	숫자를 셋팅한다( NPC::SetDrawNumber에의해 호출)
*/
void LottoNPC::SetDrawNumber(INT64 Number)
{
	//활성화상태에서만 받겠다
	if(m_bLottoActive)
	{
		m_bNumberAni = TRUE;
		SetparateNumber(Number);
	}
}
//------------------------------------------------------------------------------
/** UnitDraw의 물고있는 Unit를 교체해준다(숫자교체)
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
/** 숫자판 빌보드
*/
void LottoNPC::SetBillBoard()
{
	static WzVector vOldAxisCam; 

	//카메라 행렬얻기
	WzMatrix wmViewMat;
	g_pSunRenderer->GetViewMatrix(&wmViewMat);

	//X축에대한 빌보딩
	WzVector vAxisOrg,vAxisCam;

	//카메라 메트릭스의 Right(x축)성분 추출
	SetVector(&vAxisCam,wmViewMat._11, wmViewMat._21,wmViewMat._31);
	
	//값이 바뀌었을대만 한다.
	if(vAxisCam==vOldAxisCam)
	{
		return;
	}
	
	SetVector(&vAxisOrg,1.0f,0.0f,0.0f);

	//회전축얻기
	WzVector vRotAxis;	
	VectorCrossProduct( &vRotAxis, &vAxisOrg, &vAxisCam);
	VectorNormalize( &vRotAxis, &vRotAxis);

	//최종회전메트릭스얻기
	WzMatrix wmFinalRot;
	MatrixRotation(&wmFinalRot, &vRotAxis, acosf( VectorDotProduct( &vAxisOrg, &vAxisCam)));
	m_pBasePart->SetRotationByMatrix(&wmFinalRot);

	vOldAxisCam = vAxisCam;
}



