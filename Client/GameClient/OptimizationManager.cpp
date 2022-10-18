#include "SunClientPrecompiledHeader.h"

#include "OptimizationManager.h"
#include "ItemInfoParser.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "Hero.h"

OptimizationManager g_OptimizationManager;

OptimizationManager::OptimizationManager()
{
	m_bPerformanceMode=FALSE;
}

OptimizationManager::~OptimizationManager()
{
	Destory();
}

void OptimizationManager::Init()
{
#ifdef SKILL_EDIT
	ItemInfoParser::Instance()->SetFirst();
	BASE_ITEMINFO *pBASE_ITEMINFO = NULL;

	while( true )
	{
		pBASE_ITEMINFO = ItemInfoParser::Instance()->GetNext();
		if( pBASE_ITEMINFO == NULL )
		{
			break;
		}

		AddPartsInfo(pBASE_ITEMINFO);
	
	}
#endif 

	m_bPerformanceMode=FALSE;

}

float OptimizationManager::GetTextureRate(ObjectBase *pObj)
{
    const OptionParam& param = GAMEOPTION->GetGameOption();

    float fRate=1.0f;

    int TextureQuiality = param.GetOptionValue(nOptionType::TEXTURE_QUALITY);

    if( TextureQuiality == eOPTION_LEVEL_HIGH )
	{	
		fRate = 1.0f;
	}
	else if( TextureQuiality == eOPTION_LEVEL_MIDDLE )
	{		
		fRate = 0.5f;
	}
	else
	{
		fRate = 0.25f;
	}

	//현재 모드가 공성전 이거나 마을 일경우 다른유저는 낮춘다.
	if(GameFramework::GetCurrentScene()&&GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE&&pObj&&pObj->IsKindOfObject(PLAYER_OBJECT))
	{
		float fUsedMem=( float)g_pSunRenderer->GetUsedTextureMem() / 1024.0f;
		float fRemainMem=( float)g_pSunRenderer->GetAvailableTextureMem()/ 1024.0f;
		float fTotalMem= fRemainMem+fUsedMem;
		float fPercent=(fUsedMem/fTotalMem)*100.0f;

		if(fTotalMem<=300000.0f)
		{
			if(fPercent>=40.0f)
			{
				Player *pPlayer=(Player *)pObj;

				if(fRate>=0.25f&&!pPlayer->IsHero())
				{
					fRate/=2.0f;
				}
			}
		}
		else
		{
			if(fPercent>=80.0f)
			{
				Player *pPlayer=(Player *)pObj;

				if(fRate>=0.25f&&!pPlayer->IsHero())
				{
					fRate/=2.0f;
				}
			}

		}
	}

	return fRate;
}

void OptimizationManager::Destory()
{
	m_partsInfoArray.Clear();
}

BOOL OptimizationManager::GetAllPartInfo(eCHAR_TYPE charType,ALL_ARMOR_CODE &getInfo)
{
	ALL_ARMOR_CODE temp;

	// 임시로 버서커에게 칼을 들려준다.
	if (charType == eCHAR_BERSERKER) 
	{
		if(rand()%2)
		{
			temp.dwWeapon=1;
			temp.dwStyle=eSTYLE_FUSS_BREAKER;
		}
		else
		{
			temp.dwWeapon=16;
			temp.dwStyle=eSTYLE_BALTURS_DEFENCE;
		}



		temp.dwArmorCode=51000;

		temp.dwProtectorCode=51028;

		temp.dwPantsCode=51002;


		temp.dwBootsCode=51003;

		temp.dwGloveCode=51004;

		temp.dwBeltCode=51005;

		temp.dwShirtsCode=51006;

		temp.dwHelmetCode=51001;

	}

	// 드래곤도 칼주자
	if (charType== eCHAR_DRAGON) 
	{
		

		if(rand()%2)
		{
			temp.dwWeapon=31;
			temp.dwStyle=eSTYLE_RESTROS_SPEED;
		}
		else
		{
			temp.dwWeapon=46;
			temp.dwStyle=eSTYLE_ELEMEOUS_KNOCKBACK_LANCE;
		}



		temp.dwArmorCode=51007;

		temp.dwProtectorCode=51029;

		temp.dwPantsCode=51009;

		temp.dwBootsCode=51010;

		temp.dwGloveCode=51011;

		temp.dwBeltCode=51012;

		temp.dwShirtsCode=51013;

		temp.dwHelmetCode=51008;


	}
	// 발키리 석궁주기
	if (charType == eCHAR_VALKYRIE) 
	{

		if(rand()%2)
		{
			temp.dwWeapon=112;
			temp.dwStyle=eSTYLE_ETHER_NORMAL;
		}
		else
		{
			temp.dwWeapon=91;
			temp.dwStyle=eSTYLE_ONEHANDCROSSBOW_NORMAL;
		}

		temp.dwArmorCode=51021;

		temp.dwProtectorCode=51031;

		temp.dwPantsCode=51023;

		temp.dwBootsCode=51024;

		temp.dwGloveCode=51025;

		temp.dwBeltCode=51026;

		temp.dwShirtsCode=51027;

		temp.dwHelmetCode=51022;

	}

	if (charType== eCHAR_MAGICIAN) 
	{

		if(rand()%2)
		{
			temp.dwWeapon=154;
			temp.dwStyle=eSTYLE_ORB_NORMAL;
		}
		else
		{
			temp.dwWeapon=145;
			temp.dwStyle=eSTYLE_STAFF_NORMAL;
		}

		temp.dwArmorCode=51014;

		temp.dwProtectorCode=51030;

		temp.dwPantsCode=51016;

		temp.dwBootsCode=51017;

		temp.dwGloveCode=51018;

		temp.dwBeltCode=51019;

		temp.dwShirtsCode=51020;

		temp.dwHelmetCode=51015;
	}

	getInfo=temp;

	return TRUE;
}

BOOL OptimizationManager::AddPartsInfo(BASE_ITEMINFO *pInfo)
{

	ARMOR_PARTS_INFO tempInfo;

	if( pInfo->m_wEqClass1 )
	{
		tempInfo.charType=eCHAR_BERSERKER;
	}
	else if( pInfo->m_wEqClass2 )
	{
		tempInfo.charType=eCHAR_DRAGON;
	}
	else if(pInfo->m_wEqClass4 )
	{
		tempInfo.charType=eCHAR_VALKYRIE;
	}
	else if( pInfo->m_wEqClass5 )
	{
		tempInfo.charType=eCHAR_MAGICIAN;
	}
	else
	{
		return FALSE;
	}

	if(pInfo->m_wType==eITEMTYPE_ARMOR)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_PROTECTOR)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_HELMET)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_PANTS)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_BOOTS)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_GLOVE)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_BELT)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else if(pInfo->m_wType==eITEMTYPE_SHIRTS)
	{
		tempInfo.dwItemType=pInfo->m_wType;
	}
	else
	{
		return FALSE;
	}

	tempInfo.dwCode=pInfo->m_Code;

	m_partsInfoArray.Append(tempInfo);

	return TRUE;
}

void OptimizationManager::SetOriginalMode()
{
	if(g_pSunRenderer)
	{
		float fLODDistance[4] = { 20.0f, 21.0f, 40.0f, 41.0f};
		g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
	}
}

BOOL OptimizationManager::Process( DWORD dwTick )
{

	return TRUE;
}

BOOL OptimizationManager::ChangePerfMode(BOOL bHero)
{
	if(!g_pSunRenderer)
		return FALSE;

	if(m_bPerformanceMode)
	{
		if(bHero)
		{
			float fLODDistance[4] = { 20.0f, 21.0f, 40.0f, 41.0f};
			g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
		}
		else
		{
			float fLODDistance[4] = { 5.0f, 6.0f, 10.0f, 11.0f};	
			g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
		}
		g_pSunRenderer->x_pManagerShadowMap->SetShadowRange(25.0f);

	}
	else
	{
		float fLODDistance[4] = { 20.0f, 21.0f, 40.0f, 41.0f};
		g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
		g_pSunRenderer->x_pManagerShadowMap->SetShadowRange(30.0f);
	
		return FALSE;
	}

	return TRUE;
}


