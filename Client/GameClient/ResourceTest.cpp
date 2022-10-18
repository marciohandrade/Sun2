#include "SunClientPrecompiledHeader.h"

#include "resourcetest.h"
#include "SolarHashTable.h"
#include "GraphicResourceList.h"
#include "Application.h"
#include "LoadScene.h"
#include "GameFramework.h"
#include "Map.h"
#include "ObjectManager.h"
#include "MapInfoParser.h"

ResourceTest	g_ResourceTestScene;

//-------------------------------------------------------------------------------------------
/**
*/
ResourceTest::ResourceTest(void) : m_eCurStatus(eTEST_STATUS_NONE), 
                                   m_eNextStatus(eTEST_STATUS_START),
                                   m_pfTestFunc(NULL),
                                   m_pXMLLog(NULL)
{
    this->m_SceneType = SCENE_TYPE_RESOURCETEST;
}

//-------------------------------------------------------------------------------------------
/**
*/
ResourceTest::~ResourceTest(void)
{
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::Init( void )
{
    // 초기화 및 로그 남길 준비
    
    SAFE_DELETE(m_pXMLLog);

    m_pXMLLog = new CMarkupArchive( CMarkupArchive::ArchiveModeNotZipped | 
                                    CMarkupArchive::ArchiveModeCreate | 
                                    CMarkupArchive::ArchiveModeStoring );

    m_pXMLLog->SetEncodingString("euc-kr");

    m_pXMLLog->Open("log\\ResourceTest.xml");

    m_pXMLLog->AddElem( "RESOURCE_TEST", "Resource Load Test Log");
    m_pXMLLog->IntoElem();

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::Process( DWORD dwTick )
{
    Testing();
    forwardTest();
    return TRUE;    
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::Render( DWORD dwTick )
{
    // 준비 과정
    g_pSunRenderer->Set2DMode( TRUE );
    g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "DFLT" );

    switch(m_eCurStatus) 
    {
    case eTEST_STATUS_START:
        g_pSunRenderer->x_pManagerTextOut->DrawText( "Resource Testing Start", 5, 5, WzColor_RGB( 255, 255, 255 ) );
        break;
    case eTEST_STATUS_GRAPHIC_RESOURCE:
        g_pSunRenderer->x_pManagerTextOut->DrawText( "Resource Testing GraphicResource", 5, 5, WzColor_RGB( 255, 255, 255 ) );
        break;
    case eTEST_STATUS_MAP:
        g_pSunRenderer->x_pManagerTextOut->DrawText( "Resource Testing Map", 5, 5, WzColor_RGB( 255, 255, 255 ) );
        break;
    case eTEST_STATUS_INTERFACE:
        g_pSunRenderer->x_pManagerTextOut->DrawText( "Resource Testing Interface", 5, 5, WzColor_RGB( 255, 255, 255 ) );
        break;
    case eTEST_STATUS_END:
        g_pSunRenderer->x_pManagerTextOut->DrawText( "Resource Testing End", 5, 5, WzColor_RGB( 255, 255, 255 ) );
        break;
    default:
        break;
    }   

    // 마무리
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    g_pSunRenderer->Set2DMode( FALSE );

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void ResourceTest::Release( void )
{          
    m_pXMLLog->Close();
    SAFE_DELETE(m_pXMLLog);
}

//-------------------------------------------------------------------------------------------
/**
*/
void ResourceTest::Testing()
{
    if ( m_pfTestFunc )
    {
        (this->*m_pfTestFunc)(&m_TokenData);
    }

}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::TestingGraphicResource(TestProgressInfoData * pData)
{   
    BOOL bRet = FALSE;

    m_pXMLLog->IntoElem();

    util::SolarHashTable<RESOURCE_INFO *>::iterator itr = 
                        GraphicResourceList::Instance()->Begin();

    for ( ; itr != GraphicResourceList::Instance()->End(); ++itr)
    {
        RESOURCE_INFO * pInfo = *itr;
        if ( !pInfo )
            continue;

        m_pXMLLog->AddElem("Entity");

        char * filename = &pInfo->szFilename[0];
        BOOL bExist = FALSE;
        if (pInfo)
        {
            if( GENERALPARAM->IsUsePackFile() )
            {
                DA_FILE hFile = g_pApplication->GetDiscAccess()->OpenFile(ePFN_RESOURCE, pInfo->szFilename, DA_READ, SM_READ, OD_OPEN_EXISTING);
                if (INVALID_DA_FILE	!= hFile ) 
                {
                    bExist = TRUE;
                    g_pApplication->GetDiscAccess()->CloseFile( hFile );
                }
            }
            else
            {
                FILE *fp = fopen(pInfo->szFilename,"rb");
                if (fp)
                {
                    bExist = TRUE;
                    fclose(fp);
                }
            }
        }

        if (!bExist)
        {
            // failed exist not file  
            writeLogGraphicResource(eLOAD_STATUS_FAILED_NOT_EXIST, pInfo );
            continue;
        }

        TCHAR szBufStr[_MAX_PATH] = { 0, };
        TCHAR szDrive[_MAX_PATH] = { 0, };
        TCHAR szDir[_MAX_PATH] = { 0, };
        TCHAR szFName[_MAX_FNAME] = { 0, };
        TCHAR szExt[_MAX_EXT] = { 0, };
        
        _splitpath(filename, NULL, NULL, szFName, szExt);

        eFILETYPE eType;
        if ( _Strincmp( szExt, _tcslen(szExt), _T(".wzu"), _tcslen(szExt) ) )
        {
            eType = eFILETYPE_WZU;
        }
        else if ( _Strincmp( szExt, _tcslen(szExt), _T(".tga"), _tcslen(szExt) ) )
        {
            eType = eFILETYPE_TGA;
        }
        else if ( _Strincmp( szExt, _tcslen(szExt), _T(".cur"), _tcslen(szExt) ) )
        {
            eType = eFILETYPE_CUR;
        }
        else 
        {
            eType = eFILETYPE_NONE;
        }

        switch ( eType )
        {
        case eFILETYPE_WZU:
            {
                DWORD resourceCode = 0;

                if( pInfo->dwCode != 
                        g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( pInfo->dwCode, filename, TRUE, ULS_CHECK_VALID_ANIMATION ) )
                {
                    //로드 실패
                    writeLogGraphicResource(eLOAD_STATUS_FAILED, pInfo );
                    continue;
                }              

                g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( pInfo->dwCode );
            }
            break;
        case eFILETYPE_TGA:
            {
                HANDLE hTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( filename );
                if ( INVALID_HANDLE_VALUE == hTexture )
                {
                    // 로드 실패
                    writeLogGraphicResource(eLOAD_STATUS_FAILED, pInfo);
                    continue;
                }

                g_pSunRenderer->x_pManagerTexture->DestroyTexture( hTexture );
            }
            break;
        case eFILETYPE_CUR:
            {
                HCURSOR hCursor = ::LoadCursorFromFileA( filename );
                if ( !hCursor )
                {
                    writeLogGraphicResource(eLOAD_STATUS_FAILED, pInfo, filename);
                    // 로드 실패
                    continue;
                }  
            }
            break;

        default:
            break;
        }        

        // ok 
		if(!GENERALPARAM->IsTestResourceFailedOnly())//제네럴.INI에서 값 입력.
		{
			writeLogGraphicResource(eLOAD_STATUS_OK, pInfo );
		}
	}

    m_pXMLLog->OutOfElem();

    changeTest( eTEST_STATUS_MAP );
    
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::TestingMap(TestProgressInfoData * pData)
{
    m_pXMLLog->IntoElem();

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
#endif

    if ( g_pMap )
    {
        g_pMap->Destroy();            
    }

    char szRootDir[MAX_PATH];
    ::GetCurrentDirectoryA(MAX_PATH, szRootDir);     

    sMAPINFO_HASH::const_iterator itr = 
        MapInfoParser::Instance()->GetMapInfoHash().begin();
    for(; itr != MapInfoParser::Instance()->GetMapInfoHash().end(); ++itr )
    {
        const sMAPINFO * pMapInfo = static_cast<const sMAPINFO *> (itr->second);    
        
        for ( int i = 0; i < sMAPINFO::MAX_FIELD_NUM; ++i )
        {    
//Archive New/Delete 관련.
//////////////////////////////////////////////////////////////////////////
			g_pMap->Create(g_pSunRenderer, szRootDir);
//////////////////////////////////////////////////////////////////////////
            if ( !pMapInfo->FCode[i] )
                continue;

            m_pXMLLog->AddElem("Entity");
                    
#ifndef _MOD_CHAGNE_DELETE_HERO
			g_ObjectManager.Create();
#endif
			BOOL bRet = FALSE;
			bRet = g_pMap->Load( pMapInfo->MapCode, pMapInfo->FCode[i] );   

            if (bRet)
            {
				if(!GENERALPARAM->IsTestResourceFailedOnly())
				{
                	writeLogMap(eLOAD_STATUS_OK, pMapInfo->MapCode, pMapInfo->FCode[i] );
				}
			}
            else
            {
                writeLogMap(eLOAD_STATUS_FAILED, pMapInfo->MapCode, pMapInfo->FCode[i] );
            }  

            g_pMap->Destroy();
#ifdef _MOD_CHAGNE_DELETE_HERO
			g_ObjectManager.DeleteAllObjectWithoutHero();
#else
			g_ObjectManager.Destroy();
#endif
        } 
    }

    m_pXMLLog->OutOfElem();

    changeTest(eTEST_STATUS_EFFECT);

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::TestingEffect(TestProgressInfoData * pData)
{
    changeTest( eTEST_STATUS_INTERFACE );
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::TestingInterface(TestProgressInfoData * pData)
{
    changeTest( eTEST_STATUS_END );
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::TestingEnd(TestProgressInfoData * pData)
{
    if ( pData->dwTemp[0] > clock_function::GetTickCount() )
    {       
        SCENE_TYPE iSceneType = SCENE_TYPE_NONE;

        if (GENERALPARAM->IsTestUIScene())
        {
            iSceneType = SCENE_TYPE_TESTUI;
        }
        else
        {
            if (GENERALPARAM->IsNet())
            {
                // multi
                iSceneType = SCENE_TYPE_LOAD;	
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
            }
            else
            {
                // single
                iSceneType = SCENE_TYPE_LOAD;	
                g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );
            }
        }

        GameFramework::ChangeScene(iSceneType);
    }   

    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
void ResourceTest::forwardTest()
{
    if ( m_eCurStatus != m_eNextStatus )    
    {
        m_eCurStatus = m_eNextStatus;

        ZeroMemory(&m_TokenData, sizeof(m_TokenData));

        switch(m_eCurStatus) 
        {
        case eTEST_STATUS_START:
            {                
                changeTest(eTEST_STATUS_GRAPHIC_RESOURCE);
                m_pfTestFunc = NULL;
            }            
            break;
        case eTEST_STATUS_GRAPHIC_RESOURCE:
            {           
                if (GENERALPARAM->IsTestResourceGraphic())
                {
                    m_pXMLLog->AddElem( "GRAPHIC_RESOURCE");
                    m_pfTestFunc = &ResourceTest::TestingGraphicResource;
                }
                else
                {
                    m_pXMLLog->AddElem( "GRAPHIC_RESOURCE", "Skip");
                    m_pfTestFunc = NULL;
                    changeTest(eTEST_STATUS_MAP);
                }                                
            }
            break;
        case eTEST_STATUS_EFFECT:
            {
                if (GENERALPARAM->IsTestResourceEffect())
                {
                    m_pXMLLog->AddElem( "Effect");
                    m_pfTestFunc = &ResourceTest::TestingEffect;
                }
                else
                {
                    m_pXMLLog->AddElem( "Effect", "Skip");
                    m_pfTestFunc = NULL;
                    changeTest(eTEST_STATUS_INTERFACE);
                }                                                
            }
            break;
        case eTEST_STATUS_MAP:
            {
                if (GENERALPARAM->IsTestResourceMap())
                {
                    m_pXMLLog->AddElem( "MAP");
                    m_pfTestFunc = &ResourceTest::TestingMap;
                }
                else
                {
                    m_pXMLLog->AddElem( "MAP", "Skip");
                    m_pfTestFunc = NULL;
                    changeTest(eTEST_STATUS_EFFECT);
                } 
            }
            break;
        case eTEST_STATUS_INTERFACE:
            {               
                if (GENERALPARAM->IsTestResourceInterface())
                {
                    m_pXMLLog->AddElem( "INTERFACE");
                    m_pfTestFunc = &ResourceTest::TestingInterface;
                }
                else
                {
                    m_pXMLLog->AddElem( "INTERFACE", "Skip");
                    m_pfTestFunc = NULL;
                    changeTest(eTEST_STATUS_END);
                } 
            }
            break;
        case eTEST_STATUS_END:
            {
                m_pfTestFunc = &ResourceTest::TestingEnd;

                m_TokenData.dwTemp[0] = clock_function::GetTickCount() + DELAY_END_TICK;
            }
            break;
        default:
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
void ResourceTest::changeTest(eTEST_STATUS eTestType)
{
    m_eNextStatus = eTestType;
}

BOOL ResourceTest::writeLogGraphicResource( 
        eLOAD_STAUTS eLoadStatus, 
        RESOURCE_INFO * pInfo, TCHAR* filename)
{
    if ( !pInfo )
    {
        return FALSE;
    }

    BOOL bRet = FALSE;    
    
    m_pXMLLog->AddAttrib("ID", pInfo->dwCode);
	if(NULL == filename)
		m_pXMLLog->AddAttrib("FileName", pInfo->szFilename);
	else
		m_pXMLLog->AddAttrib("FileName", filename);

    if (eLoadStatus & eLOAD_STATUS_OK)
    {
        m_pXMLLog->AddAttrib("Check", "OK");
        return TRUE;
    }
    else if (eLoadStatus & eLOAD_STATUS_FAILED)
    {
        m_pXMLLog->AddAttrib("Check", "FAILED");

        return TRUE;
    }
    else if (eLoadStatus & eLOAD_STATUS_FAILED_NOT_EXIST)
    {
        m_pXMLLog->AddAttrib("Check", "Not Exist");
        return TRUE;
    }
    else 
    {
        assert(0);
    }
    
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL ResourceTest::writeLogMap( eLOAD_STAUTS eLoadStatus, MAPID MapID, FIELDID FieldID)
{
    const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( MapID );
    if ( !pMapInfo )
    {
        return FALSE;
    }
   
    sFIELDINFO * pFieldInfo = MapInfoParser::Instance()->GetFieldInfo(FieldID);
    if ( !pFieldInfo )
    {
        return FALSE;
    }

    TCHAR TriggerCode[MAX_MAPNAME_LENGTH];
    for ( int i = 0; i < sMAPINFO::MAX_FIELD_NUM; ++i)
    {
        if ( pMapInfo->FCode[i] && 
             pMapInfo->FCode[i] == FieldID)
        {
            ZeroMemory(TriggerCode, sizeof(TriggerCode));
            StrnCopy(TriggerCode, pMapInfo->FGCode[i], MAX_MAPNAME_LENGTH);
        }
    }    

    m_pXMLLog->AddAttrib("MAPID", MapID);
    m_pXMLLog->AddAttrib("FIELD", FieldID);
    m_pXMLLog->AddAttrib("TRIGERID", TriggerCode);

    if (eLoadStatus & eLOAD_STATUS_OK)
    {
        m_pXMLLog->AddAttrib("Check", "OK");
        return TRUE;
    }
    else if (eLoadStatus & eLOAD_STATUS_FAILED)
    {
        m_pXMLLog->AddAttrib("Check", "FAILED");

        return TRUE;
    }
    else if (eLoadStatus & eLOAD_STATUS_FAILED_NOT_EXIST)
    {
        m_pXMLLog->AddAttrib("Check", "Not Exist");
        return TRUE;
    }
    else 
    {
        assert(0);
    }

    return TRUE;
}          

