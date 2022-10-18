#include "SunClientPrecompiledHeader.h"
#include "TerrainObject.h"
#include "Map.h"
#include "AppearanceManager.h"
#include "Camera.h"
#include "GameFunc.h"
#include "Mouse.h"
#include "interfacemanager.h"
#include "HeroActionInput.h"
#include "MapEditScene.h"
#include "ObjectManager.h"
#include "ObjectFactory.h"
#include "PlayerMonster.h"
#include "Item.h"

TerrainObject::TerrainObject() 	
: m_bStartFallDown( FALSE )
{
}

TerrainObject::~TerrainObject(void)
{
	m_iTileIndexArray.Clear();
}


BOOL TerrainObject::Create(DWORD dwObjectKey, APPEARANCEINFO * pAppearInfo)
{
	BOOL rt = MapObject::Create(dwObjectKey,pAppearInfo);

	m_bStartFallDown=FALSE;
	
	ForceStateChange( STATE::IDLE, g_CurTime);

	UpdateOctree();

	return rt;
}

//------------------------------------------------------------------------------
/**
*/
WzID TerrainObject::GetCurrentIdleAnimation()
{
	WzID animationID = 0;	
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'D';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '0';

	animationID = StrToWzID(szAniCode);

	return animationID;
}

void TerrainObject::Destroy( void )
{
	MapObject::Destroy();
	m_iTileIndexArray.Clear();
	m_dwPlayerIdArray.Clear();
}

BOOL TerrainObject::Render()
{
#ifdef MAP_EDIT_MODE

	UpdateOctree();

	if(!g_mapEditScene.IsHideAllObject())
	{
		MapObject::Render();
	}

	if(g_HeroInput.GetEditMapObjKey()==GetObjectKey())//선택 됐을때 
	{
		if(g_Input.GetState(DIK_LSHIFT, KS_KEY))
		{
			g_pSunRenderer->RenderBoundingVolume(GetBoundingVolume());
		}
	}

#else

	MapObject::Render();
#endif 

	return TRUE;
}

void TerrainObject::RenderFocus()
{

	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

	for(int i=0;i<m_iTileIndexArray.Size();i++)
	{
		int iTileIndex=m_iTileIndexArray[i];

		WzColor color;

		if (iTileIndex >=0&&iTileIndex<g_pMap->GetWorldBase()->x_pPathFinder->GetNumPathTile()) 
		{
			ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();
			BOOL bZTest = g_pSunRenderer->IsEnableZTest();
			BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
			g_pSunRenderer->SetZBufferTest(FALSE);
			g_pSunRenderer->SetZBufferWrite(FALSE);


			if(g_pMap)
			{
				if ((g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileIndex) & (PTA_NO_WALK)))
				{
					color = WzColor_RGBA(255,0,0,255);
					g_pMap->SetMeshDrawMode(MD_WIREFRAME);
				}
				else
				{
					color = color = GetColor();
					g_pMap->SetMeshDrawMode(MD_WIREFRAME);

				}

				g_pSunRenderer->SetColor(color);

				g_pMap->GetWorldBase()->x_pPathFinder->RenderPathTile(g_pSunRenderer,0 ,&iTileIndex,1);			
				g_pMap->SetMeshDrawMode(MD_POLYGON);

			}

			g_pSunRenderer->SetZFunc(eZFunc);
			g_pSunRenderer->SetZBufferTest(bZTest);
			g_pSunRenderer->SetZBufferWrite(bZWrite);
		}
	}

	g_pSunRenderer->SetColor(oldcolor);
	
}

void TerrainObject::RenderInfo()
{
#ifdef MAP_EDIT_MODE

	if(g_mapEditScene.IsShowAllTile())
	{
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

		for(int i=0;i<m_iTileIndexArray.Size();i++)
		{
			int iTileIndex=m_iTileIndexArray[i];

			WzColor color;

			if (iTileIndex != -1) 
			{
				ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();
				BOOL bZTest = g_pSunRenderer->IsEnableZTest();
				BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
				g_pSunRenderer->SetZBufferTest(FALSE);
				g_pSunRenderer->SetZBufferWrite(FALSE);


				if(g_pMap)
				{
					color =WzColor_RGBA(255,255,255,255);
					g_pSunRenderer->SetColor(color);
					g_pMap->SetMeshDrawMode(MD_WIREFRAME);
					g_pMap->GetWorldBase()->x_pPathFinder->RenderPathTile(g_pSunRenderer,0 ,&iTileIndex,1);			
					g_pMap->SetMeshDrawMode(MD_POLYGON);
				}

				g_pSunRenderer->SetZFunc(eZFunc);
				g_pSunRenderer->SetZBufferTest(bZTest);
				g_pSunRenderer->SetZBufferWrite(bZWrite);
			}
		}

		g_pSunRenderer->SetColor(oldcolor);
	}


	if(g_HeroInput.GetEditMapObjKey() == GetObjectKey())//선택 됐을때 
	{
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();

		for(int i=0;i<m_iTileIndexArray.Size();i++)
		{
			int iTileIndex=m_iTileIndexArray[i];

			WzColor color;

			if (iTileIndex != -1) 
			{
				ENUM_CMP_FUNC eZFunc = g_pSunRenderer->GetZFunc();
				BOOL bZTest = g_pSunRenderer->IsEnableZTest();
				BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
				g_pSunRenderer->SetZBufferTest(FALSE);
				g_pSunRenderer->SetZBufferWrite(FALSE);


				if(g_pMap)
				{
					g_pMap->SetMeshDrawMode(MD_WIREFRAME);

					if ((g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileIndex) & (PTA_NO_WALK)))
					{
						
						color =WzColor_RGBA(255,0,0,255);
					}
					else
					{
						color =WzColor_RGBA(255,255,255,255);
					}

					g_pSunRenderer->SetColor(color);
				
					g_pMap->GetWorldBase()->x_pPathFinder->RenderPathTile(g_pSunRenderer,0 ,&iTileIndex,1);			
					g_pMap->SetMeshDrawMode(MD_POLYGON);

				}

				g_pSunRenderer->SetZFunc(eZFunc);
				g_pSunRenderer->SetZBufferTest(bZTest);
				g_pSunRenderer->SetZBufferWrite(bZWrite);
			}
		}

		g_pSunRenderer->SetColor(oldcolor);
	}

	if(g_mapEditScene.IsShowAllLayer())
	{
		char lpszText[256] = {0,};

		sprintf(lpszText,"%d",GetID());


		WzVector wzVisualPos = GetVisiblePos();
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
		g_pSunRenderer->Set2DMode( TRUE );
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

		WzVector wzPos = GetPosition();				
		wzVisualPos.z += GetMeshHeight();
		wzVisualPos.z += 0.3f;

		POINT pt2;
		float fDepth;
		g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos ,&fDepth);		

		RECT rc;
		SIZE textsize;

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(lpszText,lstrlen(lpszText),&textsize);


		rc.left = pt2.x - (textsize.cx / 2);
		rc.top = pt2.y ;
		rc.right = rc.left + textsize.cx;
		rc.bottom = rc.top + 50;

		WzColor color =WzColor_RGBA(255,0,0,255);

		RenderShadowedText(lpszText,&rc,color);


		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		g_pSunRenderer->Set2DMode( FALSE );
		g_pSunRenderer->SetColor(oldcolor);

	}

#endif
}

BOOL TerrainObject::AddTileIndex(int iIndex)
{
#ifdef MAP_EDIT_MODE
	//걸을수 있는 곳만 
	if ((g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iIndex) & (PTA_NO_WALK)))
	{
		g_InterfaceManager.ShowMessageBox("지정할수없는 타일",1000, NULL, 0);
		return FALSE;
	}

	for(int i=0;i<m_iTileIndexArray.Size();i++)
	{
		if(m_iTileIndexArray[i]==iIndex)
			return FALSE;
	}
#endif

	m_iTileIndexArray.Append(iIndex);
	return TRUE;
}


BOOL TerrainObject::AddFallDownPlayer(DWORD dwId)
{
	m_dwPlayerIdArray.Append(dwId);
	return TRUE;
}

BOOL TerrainObject::RemoveAllFallDownPlayer()
{

	m_dwPlayerIdArray.Clear();
	return TRUE;
}

BOOL TerrainObject::DeleteTileIndex(int iIndex)
{
	for(int i=0;i<m_iTileIndexArray.Size();i++)
	{
		if(m_iTileIndexArray[i]==iIndex)
		{
#ifdef MAP_EDIT_MODE
			g_pMap->GetWorldBase()->x_pPathFinder->UnsetAttribute(iIndex, PTA_NO_WALK);
#endif 			
			m_iTileIndexArray.Erase(i);
			return TRUE;
		}
	}

	return FALSE;
}

void TerrainObject::DeleteAllTileIndex()
{
#ifdef MAP_EDIT_MODE
	for(int i=0;i<m_iTileIndexArray.Size();i++)
	{
		g_pMap->GetWorldBase()->x_pPathFinder->UnsetAttribute(m_iTileIndexArray[i], PTA_NO_WALK);			
	}
#endif 

	m_iTileIndexArray.Clear();
}


//------------------------------------------------------------------------------
/**
*/
BOOL TerrainObject::IsFallDown()
{
	return m_bStartFallDown;
}

//------------------------------------------------------------------------------
/**
*/
void TerrainObject::SetFallDown(BOOL bStart)
{
	 m_bStartFallDown = bStart;

	 if (m_bStartFallDown)
	 {
        WORD attribute = PTA_PLAYER_FALLDOWN_TILE;

        if (g_pMap)
        {
	        for (int i = 0; i < m_iTileIndexArray.Size(); i++)
	        {
		        int iIndex = m_iTileIndexArray[i];

		        g_pMap->GetWorldBase()->x_pPathFinder->SetAttribute(iIndex, attribute);
        		
	        }

            m_dwPlayerIdArray.Clear();

            Object_Map_Itr tempitr;
            Object_Map_Itr itr = g_ObjectManager.GetBegin();

            while (itr != g_ObjectManager.GetEnd())
            {		
	            Object* pObject = static_cast<Object*>(itr->second);

	            if (pObject == NULL)
                {
		            continue;
                }

	            if (pObject->IsKindOfObject(CHARACTER_OBJECT))
	            {
		            Character* pChr = (Character*)pObject;

		            DWORD dwTileAtt = pChr->GetCurrentTileAtt(TRUE);
            		
		            if (BIT_CHECK(dwTileAtt, attribute))
		            {
			            for (int i = 0; i < m_iTileIndexArray.Size(); i++)
			            {
				            int iIndex = m_iTileIndexArray[i];

				            if (pChr->GetPathExplorer()->GetTile() == iIndex)
                            {
					            m_dwPlayerIdArray.Append(pChr->GetObjectKey());
                            }
			            }
		            }

	            }
	            else if (pObject->IsKindOfObject(ITEM_OBJECT))
	            {						
		            DWORD dwPlayerAtt = pObject->GetCurrentTileAtt();

		            if (BIT_CHECK(dwPlayerAtt, attribute))
		            {
			            for (int i = 0; i < m_iTileIndexArray.Size(); i++)
			            {
				            int iIndex = m_iTileIndexArray[i];

				            if (pObject->GetPathExplorer()->GetTile() == iIndex)
                            {
					            m_dwPlayerIdArray.Append(pObject->GetObjectKey());
                            }
			            }
		            }
	            }
            		
	            itr++;
            }

	        //Todo: 리스트를 돌며 떨어뜨리는 처리 
	        for (int i = 0; i < m_dwPlayerIdArray.Size(); i++)
	        {
		        DWORD dwId = m_dwPlayerIdArray[i];
		        Object* pObject = g_ObjectManager.GetObject(dwId);

		        if (pObject == NULL)
                {
			        continue;
                }

		        if (pObject->IsKindOfObject(CHARACTER_OBJECT))
		        {
			        Character* pChr = (Character*)pObject;

			        PLAYER_ACTION action;
			        ZeroMemory(&action, sizeof(action));
			        action.ActionID = ACTION_AIR_FALLDOWN;
			        pChr->DoAction(&action);
			        pChr->CommitChangeState(g_CurTime);
		        }
		        else if (pObject->IsKindOfObject(ITEM_OBJECT))
		        {
                    g_ObjectManager.Delete(dwId);

			        //Item* pItem = (Item*)pObject;

			        //pItem->Destroy();
		        }
	        }

	        m_dwPlayerIdArray.Clear();
        }
    }
    else
    {
        for (int i = 0; i < m_iTileIndexArray.Size(); i++)
        {
	        g_pMap->GetWorldBase()->x_pPathFinder->UnsetAttribute(m_iTileIndexArray[i], PTA_NO_WALK);
        }

    }

}


