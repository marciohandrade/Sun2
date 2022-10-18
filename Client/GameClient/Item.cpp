#include "SunClientPrecompiledHeader.h"

#include "Character.h"
#include "ObjectManager.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "item.h"
#include "AppearanceManager.h"
#include "Gamefunc.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "ItemInfoParser.h"
#include "state\StateItemDrop.h"
#include "GameConst_Color.h"

#include "Hero.h"
#include "uiPartyMan/uiPartyMan.h"

static const int ITEM_DISAPPEAR_DELAY = 10000;

extern DWORD g_CurTime;

// 아이템 파츠 식별을 위한
enum ItemPart
{
	IP_BODY = 0,
	IP_COUNT,
};

Item::Item()
: m_bHide(FALSE)
, m_dwOwnerKey(0)
, m_dwMonsterID(0)
, m_dwMoney(0)
, m_bIsSendPickPacket(FALSE)
, m_bProcessTick( FALSE )
, expire_time_for_owner_(0)
, is_can_get_item_(true)
{

}

Item::~Item()
{
}

BOOL Item::Create( DWORD dwObjectKey, DWORD dwID )
{
	m_bIsSendPickPacket = FALSE;

	SetObjectType(ITEM_OBJECT);
	SetAngle(0.0f,TRUE);

    BOOL bRet = Object::Create(dwObjectKey, dwID);


	//m_fVisibleAngleModifier = 0.0f;

	//SetVector(&m_wvVisiblePos,0.0f,0.0f,0.0f);

	//m_dwInterpolateInterval = 0;
	//m_dwInterpolateCurTime = 0;

	APPEARANCEINFO appearInfo;
	ZeroMemory(&appearInfo,sizeof(appearInfo));
	appearInfo.ObjectType = APPEARANCEINFO::ITEM;
	appearInfo.ItemCode = dwID;

	
	if (appearInfo.ItemCode != 0) 
	{
        // 아이템
		appearInfo.dwMoney = 0;
		m_dwMoney = 0;
        SetItemType(ITEM_RENDER_INFO::eFIELDITEM_ITEM);
	}
	else
	{
        // 하임
		appearInfo.dwMoney = 1;
		m_dwMoney = 1;
        SetItemType(ITEM_RENDER_INFO::eFIELDITEM_MONEY);
	}

    InitRender(IP_COUNT, IP_BODY, &appearInfo,0,FALSE);
	InitState(STATE::ITEM_DROP_STATE, g_CurTime);
	
 	m_dwMonsterID = 0;

	m_bOpened = FALSE;

	m_ItemSlot.Clear();

	return bRet;
}

BOOL Item::Process( DWORD dwTick )
{
	if( m_bOpened )
	{
		m_dwOpenDelay += dwTick;

		if( m_dwOpenDelay > ITEM_DISAPPEAR_DELAY )
		{
			return FALSE;
		}
		
	}

    if (GAMEOPTION->GetNotifyDropItem() == TRUE)
	{
		if( m_bProcessTick == TRUE )
			m_dwDisplayTime += dwTick;

		if( m_dwDisplayTime >= 5000 )
		{
			m_dwDisplayTime = 0;
			m_bDisappear = TRUE;
			m_bProcessTick = FALSE;
		}

		if( m_bDisappear )
		{
			m_dwHideTick += dwTick;

			float fRate = 1.0f - (float)m_dwHideTick / (float)3000;

			if( fRate <= 0.0f )
			{
				fRate = 0.0f;
				m_bDisappear = FALSE;
			}

			m_Alpha				= (BYTE)( m_Alpha * fRate );
			m_BackgroundAlpha	= (BYTE)( m_BackgroundAlpha * fRate );
		}
	}

	if (!m_bHide)
	{
		ProcessInterpolation(dwTick);
		ProcessAnimation(dwTick);
		//	상태 처리
		StateProcess( g_CurTime, dwTick );
	}
	else
	{
		if (m_dwMonsterID)
		{
			Monster *pMon = (Monster *)g_ObjectManager.GetObject(m_dwMonsterID);
			if (pMon)
			{
				if (pMon->GetHP() <= 0)
				{
					m_bHide = FALSE;
				}

			}
			else
			{
				m_bHide = FALSE;
			}
		}
	}

	return TRUE;
}

BOOL Item::ProcessInterpolation( DWORD dwTick )
{
    if (m_pPathExplorer)
    {
	    SetVisiblePos( m_pPathExplorer->GetVisiblePos() );
    }

	return TRUE;
}

BOOL Item::Render( void )
{
	if( m_bHide )
	{
		return FALSE;	
	}
	else
	{
		return Object::Render();
	}
}

BOOL Item::SetPosition( const WzVector& wvPos, int iTileIndex )
{
	g_pSunTerrain->PathExp_Reset( m_pPathExplorer, wvPos, -1);

	GetBasePart()->SetPosition(m_pPathExplorer->GetVisiblePos());

	if (g_pMap&&g_pMap->IsLoaded())
	{		
		UpdateOctree();
	}

	return TRUE;
}


WzID Item::GetCurrentDropAnimation()
{
	return StrToWzID("N001");
}

WzID Item::GetCurrentIdleAnimation()
{
	return StrToWzID("N002");
}

WzID Item::GetCurrentOpenAnimation()
{
	return StrToWzID("N002");
}

void Item::Open()
{
	m_dwOpenDelay = 0;
	m_bOpened = TRUE;
	SetAnimation(GetCurrentOpenAnimation(),FALSE);
}

void Item::SetItem( const ITEMSTREAMEX & ItemStream)
{
	m_ItemSlot.Copy(ItemStream);
}

DWORD Item::_getItemNameRGB(const char* szNumber)
{
	return WzColor_ChangeAlpha(strtol(szNumber,0,16),255);
}

void Item::RenderInfo( bool bMouseOver )
{
	if (g_InterfaceManager.GetShowFlag()) 
	{
		if (!IsMoney()) 
		{
			if (IsOpened())
			{
				return;
			}
		}
	
		if ( !(GAMEOPTION->GetNotifyDropItem()) && g_InterfaceManager.ShowItemName() )
		{
			m_Alpha = 255;
			m_BackgroundAlpha = 100;
		}
		else if ( m_bProcessTick == FALSE && m_bDisappear == FALSE && bMouseOver == false )
		{
			return;
		}

		WzVector wzVisualPos = GetVisiblePos();
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
		g_pSunRenderer->Set2DMode( TRUE );
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

		WzBoundingVolume *pV = NULL;
		if(GetBasePart()->GetCurrentAnimationInfo())
		{
			// 애니매이션이 있으면 픽킹용 바운딩박스 이용
			pV  = GetPickingBoundingVolume(0);			
		}
		else
		{
			// 애니매이션이 없으면 기본 바운딩박스 이용
			pV  = GetBoundingVolume();		
		}

		if(pV && pV->m_eBvType==BVT_AABB)
		{
			wzVisualPos.z = (pV->m_aabb.m_wvMax.z+0.5f);
		}
		else
		{
			// 예전에 찍던스타일로찍기
			wzVisualPos.z += 0.7f;
		}

		POINT pt2;
		float fDepth;
		g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos,&fDepth);

		TCHAR szString[256];
		ZeroMemory(szString,sizeof(szString));
		TCHAR szFinal[256] = {0,};

		BYTE alpha = m_Alpha;
		WzColor ItemColor = WzColor_RGBA( 255, 255, 255, alpha );

		if (IsMoney())
		{
			TCHAR	szMoney[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString( eST_HIME, szMoney, INTERFACE_STRING_LENGTH );
			Sprintf(szFinal,_T("%I64u %s"),m_dwMoney,szMoney);
		}
		else
		{
			BASE_ITEMINFO * pBaseItemInfo = (BASE_ITEMINFO *)ItemInfoParser::Instance()->GetItemInfo(m_ItemSlot.GetCode());
			if (pBaseItemInfo) 
			{
                TCHAR szBuf[128] = {0, };

                TCHAR string_socket_number[128] = {0,};

#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
				if (pBaseItemInfo->IsWeapon() || pBaseItemInfo->IsArmor() || m_ItemSlot.IsWingItem())
#else
                if (pBaseItemInfo->IsWeapon() || pBaseItemInfo->IsArmor())
#endif //_NA002823_WINGS_EQUIPMENTS_EXTENTION
				{
                    GameFunc::GetItemName(&m_ItemSlot, pBaseItemInfo, szBuf, 128);

                    int socket_number = -1;
                    switch(m_ItemSlot.GetSocketIdentifyType())
                    {
                    case eSOCKET_UNIDENTIFIED_ITEM://미확인
                    case eSOCKET_IDENTIFIED_ITEM://소켓갯수확인됨
                        {
                            socket_number = m_ItemSlot.GetSocketNum();
                            /* 2187: 미확인
                                2186: 1S
                                2185: 2S
                                2184: 3S
                                2183: 4S
                                2182: 5S
                            */
                            g_InterfaceManager.GetInterfaceString((2187 - socket_number), 
                                string_socket_number, 128);
                        }
                        break;
                    default: // nop
                        break;
                    }                    

					switch (m_ItemSlot.GetSet())
					{
					// 디바인
					case 1:
						{
							TCHAR szBuf4[128];
							ZeroMemory(szBuf4,sizeof(szBuf4));

							const int DIVINE_STRING_INDEX = 11246;


							g_InterfaceManager.GetInterfaceString(DIVINE_STRING_INDEX,szBuf4,128);	

							if(m_ItemSlot.IsLimited())
								Sprintf(szString,_T("+%d %s %s"),m_ItemSlot.GetEnchant(),szBuf4,szBuf);
							else
                                Sprintf(szString,_T("+%d %s %s"),m_ItemSlot.GetEnchant(),szBuf4,szBuf);

							ItemColor = WzColor_RGBA( 176, 118, 242, alpha );
						}
						break;

					default:
						{
							if( m_ItemSlot.IsLimited() || m_ItemSlot.IsFateItem())
								Sprintf(szString,_T("+%d %s"),m_ItemSlot.GetEnchant(),szBuf);	
							else
                                Sprintf(szString,_T("+%d %s"),m_ItemSlot.GetEnchant(),szBuf);
						}
						break;
					}

                    if (socket_number > -1)
                    {
                        TCHAR string_temp_name[128] = {0,};
                        Sprintf(string_temp_name, _T("%s %s"), szString, string_socket_number);
                        strcpy(szString, string_temp_name);
                    }
				}
				else
				{	
                    GameFunc::GetItemName(&m_ItemSlot, pBaseItemInfo, szString, 128);
				}

				BOOL bSetItemOpt = (BOOL)pBaseItemInfo->GetApplySetItemOptionType();

				if(TRUE == bSetItemOpt)
				{
					Sprintf(szFinal, _T("%s %s"), "(S)", szString);
				}
				else
				{
					StrnCopy(szFinal, szString, 255);
				}

                //파티중에 자신이 먹을 수 있는 아이템은 [*]로 표시
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
                // [?]속성이 있으면 [?]로 표시
                if (is_dice_item() == true)
                {
                    StrnCopy(szString, szFinal, 255);
                    Sprintf(szFinal, _T("%s %s"), "[?]", szString);
					//Sprintf(szFinal, _T("%s %s"), "[::]", szString);
                }
                else
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
                if (is_can_get_item() == true)
                {
                    StrnCopy(szString, szFinal, 255);
                    Sprintf(szFinal, _T("%s %s"), "[*]", szString);
                }
			}
		}


		const BASE_ITEMINFO * pItemInfo = m_ItemSlot.GetItemInfo();

		if ( m_ItemSlot.IsLimited() )
		{
			if(m_ItemSlot.GetSet()!=1)
            {
				ItemColor = WzColor_RGBA( 167, 167, 167, alpha );//WzLimitedItemColor;
            }
			
		}
		else if( m_ItemSlot.IsFateItem() )
		{
			ItemColor = WzColor_RGBA( 0, 255, 240, alpha );//c_LuckyItemColor;
		}
		else if( pItemInfo )
		{
			switch ( (eITEMSUBTYPE)pItemInfo->m_wSubType )
			{
            case eITEMSUBTYPE_ELITE_SSQ:
			case eITEMSUBTYPE_ELITE:
				ItemColor = WzColor_RGBA( 250,248, 75, alpha );//WzEliteNameColor;
				break;
			case eITEMSUBTYPE_UNIQUE:
				ItemColor = WzColor_RGBA( 183, 0, 0, alpha );//WzUniqueNameColor;
				break;

			default:
				break;
			}
		}


		//조건1:아이템색지정이되있을경우 (유니크,엘리트,럭키,디바)의색은 무시된다
		if(pItemInfo && pItemInfo->m_bUse)
		{
			WzColor color = _getItemNameRGB(pItemInfo->m_szColor);
			ItemColor = WzColor_RGBA( Red_WzColor(color), Green_WzColor(color), Blue_WzColor(color), alpha );
		}

		//조건2:색지정이있는데두 리미티드에 색이지정될경우 제일우선순위이다
		if(m_ItemSlot.IsLimited())
		{
			sLIMITED_ITEM_INFO *pLimitItemInfo  = LimitedItemInfoParser::Instance()->GetLimitedItemInfo(m_ItemSlot.GetLimitedIndex());

			if(pLimitItemInfo)
			{
				if(pLimitItemInfo->m_bUse)
				{
					WzColor color = _getItemNameRGB(pLimitItemInfo->m_szColor);
					ItemColor = WzColor_RGBA( Red_WzColor(color), Green_WzColor(color), Blue_WzColor(color), alpha );
				}
			}
			else
			{
				assert(pLimitItemInfo);
			}
		}


		RECT rc;
		SIZE textsize;

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szFinal,lstrlen(szFinal),&textsize);

		rc.left = pt2.x - (textsize.cx / 2);
		rc.top = pt2.y;
		rc.right = rc.left + textsize.cx;
		rc.bottom = rc.top + 50;

		BYTE BackgndAlpha = 100;

		if( bMouseOver )
		{
			ItemColor = WzColor_RGBA( 42, 200, 194, 255 );
			BackgndAlpha = 100;
		}
		else
		{	
			BackgndAlpha = m_BackgroundAlpha;
		}

		WzColor BackColor = WzColor_RGBA( 0, 0, 0, BackgndAlpha );

		RenderBlockedTextWithDepthTest(szFinal,&rc, ItemColor, BackColor,fDepth);
		g_pSunRenderer->Set2DMode( FALSE );
		g_pSunRenderer->SetColor(oldcolor);
	}

}

void Item::SetDisplayTime( BOOL bProcess )
{
	m_dwDisplayTime = 0;
	m_Alpha = 255;
	m_BackgroundAlpha = 100;
	m_dwHideTick = 0;
	m_bDisappear = FALSE;
	m_bProcessTick = bProcess;
}

void Item::SetSendPickPacket( BOOL bSendPacket )
{
	m_bIsSendPickPacket = bSendPacket;
}

BOOL Item::GetSendPickPacket( void )
{
	return m_bIsSendPickPacket;
}

void Item::set_expire_time_for_owner(DWORD remain_time)
{
    expire_time_for_owner_ = remain_time + g_CurTime;
}

bool Item::UpdateOwner()
{
    bool Is_changed = false;

    if (GetOwner() == 0)
    {
        return Is_changed;
    }
    if (g_CurTime > expire_time_for_owner_)
    {
        SetOwner(0);
        Is_changed = true;
    }
    return Is_changed;
}

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
void Item::SetDiceAppearance(APPEARANCEINFO* appearance_info)
{
    //InitRender(IP_COUNT, IP_BODY, appearance_info, 0, FALSE );
    RefreshRender(appearance_info);
    InitState(STATE::ITEM_DROP_STATE, g_CurTime);
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING


