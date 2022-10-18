//------------------------------------------------------------------------------
//  ProjecttileManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
//#include "Projectilemanager.h"
//#include "Projectile.h"
//#include ".\resourcemanager.h"
//#include "Application.h"
//
//ProjectileManager g_ProjectileManager;
//
////------------------------------------------------------------------------------
///**
//*/
//ProjectileManager::ProjectileManager(void)
//{
//	m_dwLastKey = 1;
//	m_hTrailTexture = INVALID_HANDLE_VALUE;
//
//}
//
////------------------------------------------------------------------------------
///**
//*/
//ProjectileManager::~ProjectileManager(void)
//{
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void ProjectileManager::Clear( void )
//{
//	Projectile_Map::iterator	it;
//	for( it = m_hashmapProjectile.begin() ; it != m_hashmapProjectile.end() ; ++it )
//	{
//		(it->second)->Destroy();
//		delete (it->second);
//	}
//
//	m_hashmapProjectile.clear();
//	m_dwLastKey = 1;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//DWORD ProjectileManager::CreateProjectile( const PROJECTILE_PARAM & param )
//{
//	DWORD dwProjectileKey = m_dwLastKey++;
//
//	Projectile * pProjectile = new Projectile();	
//	
//    // Todo:수정할것 
//
//	if(param.dwImageCode)
//	{
//		RESOURCECODE code = param.dwImageCode + ENGINE_RESOURCE_ID_OFFSET;
//		CWzUnitDraw * pUnitDraw = 
//                ResourceManager::Instance()->AllocResourceUnitDraw(code); //화살 
//		pProjectile->SetUnitDraw(pUnitDraw);
//	}
//
//	pProjectile->Create( param );
//	pProjectile->SetKey(dwProjectileKey);
//
//	pProjectile->SetTexture(m_hTrailTexture);
//	m_hashmapProjectile.insert( Projectile_Map_Pair(dwProjectileKey, pProjectile) );
//	return dwProjectileKey;
//}
//
////------------------------------------------------------------------------------
///**
//*/
//BOOL ProjectileManager::Delete( DWORD dwProjectileKey )
//{
//	Projectile_Map::iterator	it,temp;
//	for( it = m_hashmapProjectile.begin() ; it != m_hashmapProjectile.end() ;)
//	{
//		DWORD dwObjKey = (it->second)->GetKey();
//
//		if (dwObjKey==dwProjectileKey)
//		{
//			temp = it;
//			DWORD dwKey = (temp->first);
//
//			it++;
//			CWzUnitDraw *pUnit=(temp->second)->GetUnitDraw();
//
//			//리소스가 있는 객체 만 삭제 
//			if(pUnit)
//				ResourceManager::Instance()->FreeResourceUnitDraw(pUnit);
//
//			(temp->second)->Destroy();
//			delete (temp->second);
//			m_hashmapProjectile.erase( dwKey );
//
//			return TRUE;
//		}
//		else
//		{
//			it++;
//		}
//	}
//
//
//	return FALSE;
//}
//
//cProjectileBase* ProjectileManager::GetProjectile(DWORD dwKey)
//{
//    return g_ProjectileManagerEx.GetProjectile(dwKey);
//}
//
//
////
//////------------------------------------------------------------------------------
/////**
////*/
////Projectile * ProjectileManager::GetProjectile(DWORD dwKey)
////{	
////	Projectile_Map_Itr	itr;
////	itr = m_hashmapProjectile.find( dwKey );
////	if (itr == m_hashmapProjectile.end())
////	{
////		return NULL;
////	}
////	else
////	{	
////		return itr->second;
////	}
////}
//
////------------------------------------------------------------------------------
///**
//*/
//void ProjectileManager::Process(DWORD dwTick)
//{
//	Projectile_Map::iterator	it,temp;
//	for( it = m_hashmapProjectile.begin() ; it != m_hashmapProjectile.end() ;)
//	{
//		BOOL bKill = (it->second)->Process(dwTick);
//		if (bKill)
//		{
//			temp = it;
//			DWORD dwKey = (temp->first);
//
//			it++;
//            CWzUnitDraw *pUnit=(temp->second)->GetUnitDraw();
//            
//            //리소스가 있는 객체 만 삭제 
//            if(pUnit)
//                ResourceManager::Instance()->FreeResourceUnitDraw(pUnit);
//
//			(temp->second)->Destroy();
//			delete (temp->second);
//			m_hashmapProjectile.erase( dwKey );
//		}
//		else
//		{
//			it++;
//		}
//	}
//
//    g_ProjectileManagerEx.Process( dwTick );
//
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void ProjectileManager::Render()
//{
//	ENUM_CULL_FACE      eCullFaceMode;
//	eCullFaceMode = g_pSunRenderer->GetCullFaceMode();
//	ENUM_ALPHABLEND eAlphablendMode = g_pSunRenderer->GetAlphaBlendMode();
//	g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA, FALSE );
//	WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
//	g_pSunRenderer->SetCullFaceMode( CF_DRAW_ALL );
//	Projectile_Map::iterator	it;
//	for( it = m_hashmapProjectile.begin() ; it != m_hashmapProjectile.end() ; ++it)
//	{
//		(it->second)->Render();		
//		
//	}
//	g_pSunRenderer->SetCullFaceMode( eCullFaceMode );
//	g_pSunRenderer->SetColor(oldcolor);
//	g_pSunRenderer->SetAlphaBlendMode( eAlphablendMode, FALSE );
//
//    g_ProjectileManagerEx.Render();
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void ProjectileManager::Init()
//{
//    g_ProjectileManagerEx.Init();
//
//	m_hTrailTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Arrow\\Alpha_Gradient.TGA" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//void ProjectileManager::Destroy()
//{	
//	if (m_hTrailTexture != INVALID_HANDLE_VALUE)
//	{
//		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTrailTexture );
//		m_hTrailTexture = INVALID_HANDLE_VALUE;
//	}
//
//	Clear();
//
//    g_ProjectileManagerEx.Destroy();
//
//}
//
////------------------------------------------------------------------------------
///**
//*/
//HANDLE ProjectileManager::GetTrailTexture()
//{
//	return m_hTrailTexture;
//}