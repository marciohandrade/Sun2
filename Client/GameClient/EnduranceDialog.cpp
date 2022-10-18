
#include "SunClientPrecompiledHeader.h"
#include "EnduranceDialog.h"
#include "equipmentcontainer.h"
#include "HeroEquipmentContainer.h"
#include "SCItemSlot.h"
#include "Hero.h"

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
#include "StrengthPenaltyParser.h"
#include "StrengthUIParser.h"
#include "ItemInfoParser.h"
#include "InterfaceManager.h"
#endif//__NA_001062_20080623_STRENGTH_PENALTY
#ifdef _NA_000000_20130114_RENEWER_UI
#include "EnduranceInfoParser.h"
#define WM_UP_WINDOW WM_USER +2
#endif

WzID EnduranceDIalog::m_wzId[MAX_PICTURE_CONTROL] =
{
	StrToWzID("P001"),
	StrToWzID("P002"),
	StrToWzID("P003"),
	StrToWzID("P004"),
	StrToWzID("P005"),
	StrToWzID("P006"),
	StrToWzID("P007"),
	StrToWzID("P008"),
	StrToWzID("P009"),
	StrToWzID("P010"),
	StrToWzID("P011"),
	StrToWzID("P012"),
	StrToWzID("P013"),
	StrToWzID("P014"),
	StrToWzID("P015"),
	StrToWzID("P016"),
	StrToWzID("P017"),
	StrToWzID("P018"),
	StrToWzID("P019"),
	StrToWzID("P020"),
	StrToWzID("P021"),
	StrToWzID("P022"),
	StrToWzID("P023"),
	StrToWzID("P024"),
	StrToWzID("P025"),
	StrToWzID("P026"),
	StrToWzID("P027"),
};


//------------------------------------------------------------------------------
/**
*/
EnduranceDIalog::EnduranceDIalog()
{
	for( int i = 0 ; i < MAX_EQUIPMENT_TYPE; ++i)
	{
		m_iEquipmentStatus[i] = i;
	}
	m_kUpdate.bUpdate = FALSE;
	m_kUpdate.dwCurTick = 0;

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	m_hDuraTexture = INVALID_HANDLE_VALUE;	
#endif//__NA_001062_20080623_STRENGTH_PENALTY
}

//------------------------------------------------------------------------------
/**
*/
EnduranceDIalog::~EnduranceDIalog()
{

}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::Release()
{
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	if( m_hDuraTexture != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hDuraTexture );
		m_hDuraTexture = INVALID_HANDLE_VALUE;
	}
#endif//__NA_001062_20080623_STRENGTH_PENALTY
}
//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::Init( CDrawBase * pDrawBase )
{
	for (int i = 0; i < MAX_PICTURE_CONTROL; ++i)
	{
		m_pPicture[i] = static_cast<CCtrlPictureWZ*>(getControl(i));
	}

	//UpdatePosition();

	//기본 베이스 이미지 체크 체크

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
#ifdef _NA_000000_20130114_RENEWER_UI
	if(m_hDuraTexture==INVALID_HANDLE_VALUE)
	{	
		m_hDuraTexture  = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}
#else
	if(m_hDuraTexture==INVALID_HANDLE_VALUE)
		m_hDuraTexture  = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\endurance.tga", TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
#endif
#endif//__NA_001062_20080623_STRENGTH_PENALTY

#ifdef _NA_000000_20130114_RENEWER_UI
	//머리                    어깨                      갑옷
	equip_real_pos_[0].x = 12;equip_real_pos_[1].x = 5; equip_real_pos_[2].x = 10;
	equip_real_pos_[0].y = 0; equip_real_pos_[1].y = 5; equip_real_pos_[2].y = 7;
	//장갑                    벨트                       바지
	equip_real_pos_[3].x = 4; equip_real_pos_[4].x = 11; equip_real_pos_[5].x = 8;
	equip_real_pos_[3].y = 13;equip_real_pos_[4].y = 20; equip_real_pos_[5].y = 23;
    //부츠                   무기                       셔츠
	equip_real_pos_[6].x = 10;equip_real_pos_[7].x = 0; equip_real_pos_[8].x = 11;
	equip_real_pos_[6].y = 31; equip_real_pos_[7].y = 27; equip_real_pos_[8].y = 14;

#endif
}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::OnShowWindow(BOOL val)
{

}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::MessageProc( SI_MESSAGE * pMessage )
{

}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::NetworkProc( MSG_BASE * pMsg )
{

}

//------------------------------------------------------------------------------
/** 내구도 상태를 업데이트해서 이미지를 변경해주는부분
    디파인적용시:__NA_001062_20080623_STRENGTH_PENALTY
    DESC:이전에는 UI에 해당하는 인덱스이미지를 show,hide시켰었는데  	
         지금은 RenderTexuture로 뿌린다.하지만 기본바탕은 show시켜준다.
*/
void EnduranceDIalog::UpdateEquipEnduranceState()
{
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	AllPictureControlShowOff();	
#else
	if(!g_pHero)
	{
		return;
	}
	HeroEquipmentContainer * pEquipContainer = static_cast<HeroEquipmentContainer*>
		(g_pHero->GetHeroEquipContainer());
	if(pEquipContainer)
	{
		for(int i =0; i < MAX_EQUIPMENT_TYPE; ++i)
		{
			SCSlot & rSlot = pEquipContainer->GetSlot(GetEquipContainerPos(i));
			SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(rSlot);
			
			if(rItemSlot.WithDura())
			{
				m_iEquipmentStatus[i] = EachPictureControlSelectLevel(rItemSlot, i);
			}
			else
			{
				m_iEquipmentStatus[i] = i;
			}

		}
	}

	AllPictureControlShowOff();	

	if(IsWounded())
	{
		for (int i = 0; i < MAX_EQUIPMENT_TYPE; ++i)
		{
			if(m_iEquipmentStatus[i] < MAX_PICTURE_CONTROL)
			{
				if(m_pPicture[m_iEquipmentStatus[i]])
					m_pPicture[m_iEquipmentStatus[i]]->ShowWindowWZ(WZ_SHOW);
			}
		}
	}
#endif//__NA_001062_20080623_STRENGTH_PENALTY
}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::Process(DWORD dwTick)
{
	if(m_kUpdate.dwCurTick == 0)
	{
		m_kUpdate.bUpdate = FALSE;
	}
	m_kUpdate.dwCurTick += dwTick;

	if(m_kUpdate.dwCurTick > RENEW_TIME)
	{
		m_kUpdate.dwCurTick = 0;
		m_kUpdate.bUpdate = TRUE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::OnRenderSolarDialog()
{
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
	RenderDuraEquip();
#endif//__NA_001062_20080623_STRENGTH_PENALTY(이곳에서 프레임마다 랜더한다)
}
#ifdef __NA_001062_20080623_STRENGTH_PENALTY
//------------------------------------------------------------------------------
/** 내구도 장비그리기
*/
void EnduranceDIalog::RenderDuraEquip()
{
#ifdef _NA_000000_20130114_RENEWER_UI
	//널체크
	if(!g_pHero || !g_pSunRenderer)  return; 

	HeroEquipmentContainer * pEquipContainer = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());

	//널체크	
	if(!pEquipContainer) return; 

	ENUM_ALPHABLEND eOldAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

	RECT pDesc;

	GetDialogWZ()->GetClientRect(&pDesc);
	for(int i =0; i < MAX_EQUIPMENT_TYPE; ++i)
	{
		SCSlot & rSlot = pEquipContainer->GetSlot(GetEquipContainerPos(i));

		SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(rSlot);

		//기본컬러&인덱스
		WzColor		WzErrorColor = WzColor_RGBA( 255, 255, 255, 255 );
		int			IndexY		 = 0;

		//슬롯유효검사
		if(rItemSlot.IsContainValidItem() && rItemSlot.WithDura())
		{
			//변경컬러&인덱스
			WzErrorColor = GetEnduraLevelColor(rItemSlot);
			IndexY = GetEnduraPictureIndex(rItemSlot);
		}

		WzColor arTextureColor[4]={WzErrorColor,WzErrorColor,WzErrorColor,WzErrorColor};

		EnduranceInfo data;
		memset(&data,0,sizeof(EnduranceInfo));
		
		bool isdata = EnduranceInfoParser::Instance()->FindData(data,IndexY,i);

		if (isdata)
		{
			g_pSunRenderer->RenderTexture( m_hDuraTexture,
				(float)pDesc.left +equip_real_pos_[i].x, 
				(float)pDesc.top+equip_real_pos_[i].y
				,(float)data.width_
				,(float)data.height_,
				(float)data.x_position,
				(float)data.y_position
				,(float)data.width_
				,(float)data.height_,
				arTextureColor);
		}
		
	}
	g_pSunRenderer->SetAlphaBlendMode(eOldAlphablendMode);
#else
	//널체크
	if(!g_pHero || !g_pSunRenderer)  return; 

	HeroEquipmentContainer * pEquipContainer = static_cast<HeroEquipmentContainer*>(g_pHero->GetHeroEquipContainer());

	//널체크	
	if(!pEquipContainer) return; 
		
	ENUM_ALPHABLEND eOldAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
	g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);
	
	RECT pDesc;
	
	GetDialogWZ()->GetClientRect(&pDesc);

	//이큅을 돌면서 체크를한다.
	for(int i =0; i < MAX_EQUIPMENT_TYPE; ++i)
	{
		SCSlot & rSlot = pEquipContainer->GetSlot(GetEquipContainerPos(i));
		
		SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(rSlot);
		
		//기본컬러&인덱스
		WzColor		WzErrorColor = WzColor_RGBA( 255, 255, 255, 255 );
		int			IndexY		 = 0;

        //슬롯유효검사
        if(rItemSlot.IsContainValidItem() && rItemSlot.WithDura())
        {
            //변경컬러&인덱스
            WzErrorColor = GetEnduraLevelColor(rItemSlot);
            IndexY = GetEnduraPictureIndex(rItemSlot);
        }
		
		WzColor arTextureColor[4]={WzErrorColor,WzErrorColor,WzErrorColor,WzErrorColor};

		g_pSunRenderer->RenderTexture( m_hDuraTexture,
									  (float)pDesc.left, (float)pDesc.top,34.f, 49.f,
									  i*35.f,IndexY*50.f, 34.f, 49.f,
									  arTextureColor);
	}
	g_pSunRenderer->SetAlphaBlendMode(eOldAlphablendMode);
#endif
}
//------------------------------------------------------------------------------
/** 내구도 패널티레벨 색상
*/
sSTRENGTH_PENALTY* EnduranceDIalog::GetPenaltyInfo(SCItemSlot& rItemSlot)
{
	BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rItemSlot.GetCode());

	if(!pItemInfo)
	{
		assert(!"pItemInfo- NULL");
		return NULL;
	}

	sSTRENGTH_PENALTY* pPenaltyData = StrengthPenaltyParser::Instance()->GetStrengthPenaltyInfo( pItemInfo->m_dwStrengthPenaltyIdx, rItemSlot.GetMaxDura(), rItemSlot.GetDura() );

	if(!pPenaltyData) 
	{
		assert(!"pPenaltyData- NULL");
		return NULL;
	}
	return pPenaltyData;
}
//------------------------------------------------------------------------------
/** 내구도 패널티레벨 색상
*/
WzColor EnduranceDIalog::GetEnduraLevelColor(SCItemSlot& rItemSlot)
{
	
	sSTRENGTH_PENALTY* pPenaltyData = GetPenaltyInfo(rItemSlot);

	if(!pPenaltyData) return 0;
	
	return (WzColor_ChangeAlpha(pPenaltyData->m_StrengthColorCode,255));

}
//------------------------------------------------------------------------------
/** 내구도 패널티 이미지인덱스
*/
int EnduranceDIalog::GetEnduraPictureIndex(SCItemSlot& rItemSlot)
{
	sSTRENGTH_PENALTY* pPenaltyData = GetPenaltyInfo(rItemSlot);

	if(!pPenaltyData) return 0;

	 sSTRENGTH_UI_RECODE* pPenaltyUI= StrengthUIParser::Instance()->GetStrengthUIInfo(pPenaltyData->m_nStrengthUIIndex);
	
	if(!pPenaltyUI) return 0;

	return pPenaltyUI->m_YCoordinate;
}
#endif//__NA_001062_20080623_STRENGTH_PENALTY
//------------------------------------------------------------------------------
/**
*/
void EnduranceDIalog::AllPictureControlShowOff()
{
	for ( int i =0; i < MAX_PICTURE_CONTROL; ++i)
	{
		if(m_pPicture)
			m_pPicture[i]->ShowWindowWZ(WZ_HIDE);
	}
}

//------------------------------------------------------------------------------
/**
*/
int EnduranceDIalog::GetEquipContainerPos(int iNum)
{
	int iEquipContainerPos = 0;
	switch(iNum) 
	{
	case DUR_HELMET:
		iEquipContainerPos = EquipmentContainer::EQUIP_HELMET;
		break;
	case DUR_PROTECTOR:
		iEquipContainerPos = EquipmentContainer::EQUIP_PROTECTOR;
		break;
	case DUR_ARMOR:
		iEquipContainerPos = EquipmentContainer::EQUIP_ARMOR;
		break;
	case DUR_GLOVER:
		iEquipContainerPos = EquipmentContainer::EQUIP_GLOVE;
		break;
	case DUR_BELT:
		iEquipContainerPos = EquipmentContainer::EQUIP_BELT;
		break;
	case DUR_PANTS:
		iEquipContainerPos = EquipmentContainer::EQUIP_PANTS;
		break;
	case DUR_BOOTS:
		iEquipContainerPos = EquipmentContainer::EQUIP_BOOTS;
		break;
	case DUR_WEAPON:
		iEquipContainerPos = EquipmentContainer::EQUIP_WEAPON1;
		break;
	case DUR_SHIRTS:
		iEquipContainerPos = EquipmentContainer::EQUIP_SHIRTS;
		break;
	default:
		break;
	}
	return iEquipContainerPos;
}

//------------------------------------------------------------------------------
/**
*/
int	EnduranceDIalog::EachPictureControlSelectLevel(SCItemSlot& rItemSlot,int curEquip)
{
	if(rItemSlot.GetMaxDura() == 0)
	{
		return curEquip;
	}

	float fRatio = (float) rItemSlot.GetDura() / (float) rItemSlot.GetMaxDura();
	if(fRatio > 0.1f)
	{
		return curEquip;
	}
	else if (fRatio > 0.0f)
	{
		return (curEquip + MAX_EQUIPMENT_TYPE);
	}
	else
	{
		return (curEquip + MAX_EQUIPMENT_TYPE * 2);
	}
}


//------------------------------------------------------------------------------
/**
*/
CControlWZ * EnduranceDIalog::getControl( POSTYPE AtPos )
{
	assert( AtPos < MAX_PICTURE_CONTROL );
	return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
POSTYPE	EnduranceDIalog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < MAX_PICTURE_CONTROL; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return MAX_PICTURE_CONTROL;
}

BOOL EnduranceDIalog::IsWounded()
{
	for( int i =0; i < MAX_EQUIPMENT_TYPE; ++i )
	{
		if(m_iEquipmentStatus[i] >= MAX_EQUIPMENT_TYPE)
		{
			return TRUE;
		}
	}
	return FALSE;
}

#ifdef _NA_000000_20130114_RENEWER_UI
void EnduranceDIalog::GetMessage( DWORD messageID,LPARAM lParam,WPARAM wParam )
{
	switch(messageID)
	{
	case WM_UP_WINDOW:
		{
#if WZENGINEVER >= 590
		CDialogManager* pDialogManager = g_InterfaceManager.GetDialogManager();
		pDialogManager->MoveTail( GetDialogKey() );
#endif // WZENGINEVER >= 590
		}	
		break;
	}
}
#endif