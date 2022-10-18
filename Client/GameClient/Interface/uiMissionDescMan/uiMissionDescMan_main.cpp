//------------------------------------------------------------------------------
//  uiMissionDescMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
//#include "uiMissionDescMan.h"
//#include "Map.h"
//#include "MapInfoParser.h"
//#include "GraphicResourceList.h"
//#include "SceneBase.h"
//#include "Application.h"
//#include "interfacemanager.h"
////------------------------------------------------------------------------------
///**
//*/
//uiMissionDescMan::uiMissionDescMan(InterfaceManager *pUIMan) :
//    uiBaseMan(pUIMan)
//{
//    m_bShowMissionDesc = false;
//    m_bShowTexture = false;
//
//    m_hMissionDescTex = NULL;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//uiMissionDescMan::~uiMissionDescMan()
//{
//    this->Release();
//}
//
//void
//uiMissionDescMan::Init(CDrawBase* pDrawBase)
//{
//    uiBaseMan::Init(pDrawBase);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//uiMissionDescMan::Release()
//{
//    FreeMissionDesc();
//}

//------------------------------------------------------------------------------
/**
*/
//void
//uiMissionDescMan::FreeMissionDesc()
//{
//    if (m_hMissionDescTex)
//    {
//        g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hMissionDescTex );
//        m_hMissionDescTex = NULL;
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//uiMissionDescMan::RenderMissionDesc()
//{
//    // �̼� �� ��쿡�� ����~
//    if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_MISSION)
//        return;
//
//    if (m_bShowMissionDesc && m_hMissionDescTex)
//    {
//        ENUM_ALPHABLEND		m_eAlphablendMode;
//        m_eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
//
//        g_pApplication->Set2DMode();
//
//        g_pSunRenderer->SetAlphaBlendMode(AB_NEW_ALPHA);
//
//        POINT pt = g_pApplication->Get2DModeMapping();
//
//        long width = 760;
//        long height = 570;
//
//        // TODO:
//        // ��ü ȭ�� �ػ󵵸� �����ؼ� ���̾�α׸� ȭ�� �߾ӿ� ��ġ��Ų��.
//        // ex) 1024x768
//
//        float mw = (long)width * 0.5f;
//        float mh = (long)height * 0.5f;
//
//        long sw;
//        long sh;
//
//        sw = pt.x; sh = pt.y;
//
//        float pw = (sw * 0.5f) - mw;
//        float ph = (sh * 0.5f) - mh;
//
//        g_pSunRenderer->RenderTexture( 
//			m_hMissionDescTex, 
//			(float)pw, 
//			(float)ph, 
//			760.0f, 
//			570.0f, 
//			0.0f, 
//			0.0f );
//
//        g_pSunRenderer->Set2DMode( FALSE );
//
//        g_pSunRenderer->SetAlphaBlendMode( m_eAlphablendMode );
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//uiMissionDescMan::SetMissionDesc(MAPCODE MapCode)
//{
//    FreeMissionDesc();
//
//    const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(MapCode);
//    assert (mapInfo);
//    if (mapInfo)
//    {
//        for (int i=0; i<mapInfo->MAX_FIELD_NUM; ++i)
//        {
//            if (mapInfo->FCode[i] == g_pMap->GetCurrentFieldID())
//            {
//                DWORD dwImgCode = mapInfo->dwImageCode[i];
//
//                if (dwImgCode)
//                {
//                    RESOURCE_INFO* resInfo = GraphicResourceList::Instance()->GetItemInfo((CODETYPE)dwImgCode);
//                    assert (resInfo);
//                    
//                    if (resInfo)
//                    {
//                        this->m_hMissionDescTex =
//                            g_pSunRenderer->x_pManagerTexture->LoadTexture( resInfo->szFilename, TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP);
//                    }
//                }
//            }
//        }
//    }
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void
//uiMissionDescMan::ToggleMissionDesc()
//{
//    // �̼� �� ��쿡�� ����~
//    if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_MISSION)
//        return;
//
//    g_InterfaceManager.ShowGameInterface(TRUE);
//
//    this->m_bShowMissionDesc = !this->m_bShowMissionDesc;
//
//    if (this->m_bShowMissionDesc)
//    {
//        if (m_hMissionDescTex)
//            g_InterfaceManager.HideAllWindows();
//    }
//}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
