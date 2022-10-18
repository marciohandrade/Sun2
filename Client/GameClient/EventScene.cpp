
#include "SunClientPrecompiledHeader.h"
#include "EventScene.h"
#include "LoadScene.h"
#include "Monster.h"
#include "objectmanager.h"
#include "ItemManager.h"
#include "InterfaceManager.h"
#include "SoundBGM.h"
#include "Map.h"
EventScene g_EventScene;

// ���� ���� ��ġ
WzVector g_vTargetPos[] =
{
    { 21.21f, 49.00f, 06.58f }, // cut scene 01
    { 21.21f, 46.00f, 06.58f }, // cut scene 02
    { 21.21f, 49.00f, 06.58f }, // cut scene 03
    { 21.21f, 49.00f, 06.58f }, // cut scene 04
    { 21.21f, 44.00f, 06.58f }, // cut scene 05
};

// ī�޶��� LookAt
WzVector g_vLookat[] =
{
    { 22.21f, 49.00f, 07.10f }, // cut scene 01
    { 21.21f, 47.00f, 06.00f }, // cut scene 02
    { 21.21f, 48.00f, 09.20f }, // cut scene 03
    { 21.21f, 49.00f, 06.58f }, // cut scene 04
    { 21.30f, 44.00f, 06.58f }, // cut scene 05
};


//---------------------------------------------------------------------------------------
//
//		�̺�Ʈ ī�޶�
//
//---------------------------------------------------------------------------------------

EventCamera::EventCamera( void )
{
}

EventCamera::~EventCamera( void )
{
	Destroy();
}

void EventCamera::Init( void )
{
    SetVector( &m_wvUp, 0.0f, 0.0f, 1.0f );

	FILE			*fp;
	char			szFileName[256] = {0,};
	int				iFrameCount;
	DWORD			dwFrame;
	float			x, y, z;
	int				iTotalFrame = 0;

	for( int i = 0 ; i < EVENT_SCENE_COUNT ; ++i )
	{
		_snprintf( szFileName,256-1, "Data\\Event\\Camera%d.txt", i );
		fp = fopen( szFileName, "rt" );
		if( fp == NULL )
		{
			assert( !"�̺�Ʈ ī�޶� ���� �ε� ����" );
			return;
		}

		//	������ ����
		fscanf( fp, "%d", &iFrameCount );
		for( int j = 0 ; j < iFrameCount ; ++j )
		{
			EVENT_CAMERA	*pEvent_Camera = new EVENT_CAMERA;

			//	ī�޶� ������
			fscanf( fp, "%d", &dwFrame );
			pEvent_Camera->iFrame			= iTotalFrame++;

            // ����: �ϵ��ڵ� ����

            static WzVector _vTargetPos;
            static WzVector _vLookat;

            _vTargetPos = g_vTargetPos[i];
            _vLookat = g_vLookat[i];

            // �ʿ��ϸ� ����������Ʈ �Ѵ�.
            if ( i == 0 )
            {
                _vTargetPos.x += +1.0f;
                _vTargetPos.y += +3.0f;
                _vTargetPos.z += +0.8f;
                _vLookat.z +=  ((float)j*0.03f);
            }
            else
            if ( i == 1 )
            {
                _vTargetPos.x += +0.0f;
                _vTargetPos.y += +6.0f + ((float)j*0.002f);
                _vTargetPos.z += +1.3f;
                
                _vLookat.x += -((float)j*0.01f);
                _vLookat.z +=  ((float)j*0.012f);
            }
            else
            if ( i == 2 )
            {
                _vTargetPos.x += +0.1f;
                _vTargetPos.y += +3.0f;
                _vTargetPos.z += +0.5f;
            }
            else
            if ( i == 3 )
            {
            }
            else
            if ( i == 4 )
            {
                _vTargetPos.x += +0.4f;
                _vTargetPos.y += +5.5f;
                _vTargetPos.z += -0.8f;
            }

			//	ī�޶� ��ġ �б�
			fscanf( fp, "%f", &x );
			fscanf( fp, "%f", &y );
			fscanf( fp, "%f", &z );
			pEvent_Camera->wvCameraPos.x	= x + _vTargetPos.x;
			pEvent_Camera->wvCameraPos.y	= y + _vTargetPos.y;
			pEvent_Camera->wvCameraPos.z	= z + _vTargetPos.z;

			//	ī�޶� Ÿ�� �б�
			fscanf( fp, "%f", &x );
			fscanf( fp, "%f", &y );
			fscanf( fp, "%f", &z );
            pEvent_Camera->wvLookAt.x		= _vLookat.x;
            pEvent_Camera->wvLookAt.y		= _vLookat.y;
            pEvent_Camera->wvLookAt.z		= _vLookat.z;

			m_vectorEventCamera.push_back( pEvent_Camera );
		}

		fclose( fp );
	}
                                                               

	m_iLastFrame = iTotalFrame;
	m_iCurrentFrame = 0;
	m_fCurrentFrame = 0.0f;
}

void EventCamera::Destroy( void )
{
	EventCamera_iterator it;
	for( it = m_vectorEventCamera.begin() ; it != m_vectorEventCamera.end() ; ++it )
	{
		delete *it;
	}
	m_vectorEventCamera.clear();
}

BOOL EventCamera::Process( DWORD dwTick )
{
	++m_iCurrentFrame;
	if( m_iLastFrame <= m_iCurrentFrame )
	{
		m_iCurrentFrame = m_iLastFrame - 1;
	}

	EVENT_CAMERA	*pEVENT_CAMERA = m_vectorEventCamera.at( m_iCurrentFrame );

    g_pSunRenderer->SetCameraMatrix( &pEVENT_CAMERA->wvCameraPos,
                                     &pEVENT_CAMERA->wvLookAt,
                                     &m_wvUp );

    return TRUE;
}


//---------------------------------------------------------------------------------------
//
//		�̺�Ʈ ��
//
//---------------------------------------------------------------------------------------

EventScene::EventScene(void)
{
    m_SceneType = SCENE_TYPE_EVENT;
}

EventScene::~EventScene(void)
{
}

void EventScene::Activate()
{
}


BOOL EventScene::Init()
{
	m_iCurrentScene = 0;
	m_dwCurFadeTime=0;
	m_eFadeState=eFADE_STATE_NONE;
	m_bFristRender=TRUE;

	// e3���� �ƹ��͵� �����������..
	g_SoundBGM.Stop();
    
	//	ī�޶� �ʱ�ȭ
	m_EventCamera.Init();

	char szFileName[256] = {0,};
	TCHAR str[5] = { 'X', '0', '0', '0', NULL };
	CWzUnit	*pWzUnit;

	for( int i = 0 ; i < EVENT_SCENE_COUNT ; ++i )
	{
		m_dwCharacter[i] = StrToWzID( str );	str[3] += 1;
		_snprintf( szFileName, 256-1, "Data\\Event\\%s.WZU", str );
		g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( m_dwCharacter[i], szFileName );

        m_pCharacter[i] = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );

		pWzUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit( m_dwCharacter[i] );
		m_pCharacter[i]->SetWzUnit( pWzUnit,FALSE );

		m_pCharacter[i]->SetCurrentAnimation( StrToWzID( str ), FALSE );

        if (i==4)
            m_pCharacter[i]->SetPosition( g_vTargetPos[4].x, g_vTargetPos[4].y, g_vTargetPos[4].z );
        else
            m_pCharacter[i]->SetPosition( g_vTargetPos[0].x, g_vTargetPos[0].y, g_vTargetPos[0].z );
	}


	return TRUE;
}

void EventScene::Release()
{
	//	�߼��� ���� �ı�
	for( int i = 0 ; i < EVENT_SCENE_COUNT ; ++i )
	{
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit( m_dwCharacter[i] );

        Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pCharacter[i] );

	}

	//	������ �÷��� ����
	m_bEnableRender = FALSE;

	g_SoundBGM.Stop();
}


BOOL EventScene::Process( DWORD dwTick )
{
	//	��� ������Ʈ
	g_pMap->Update( dwTick );

	//	To Do: �߼��� ���� ���ϸ��̼�
	float	fPercent;
	g_pSunRenderer->ProgressUnitFrame( m_pCharacter[m_iCurrentScene], (float)dwTick, NULL, &fPercent );
	g_pSunRenderer->AnimateUnit( m_pCharacter[m_iCurrentScene] );
	if( 100.0f == fPercent )
	{
		++m_iCurrentScene;
		if( m_iCurrentScene >= EVENT_SCENE_COUNT )
		{
			//	���� : ���� ������
			GameFramework::ChangeScene( SCENE_TYPE_MISSION );
			return TRUE;
		}
		else
		{
			g_pSunRenderer->ProgressUnitFrame( m_pCharacter[m_iCurrentScene], 0.0f, NULL, &fPercent );
			g_pSunRenderer->AnimateUnit( m_pCharacter[m_iCurrentScene] );
		}
	}

	//	ī�޶� ��ŷ
	m_EventCamera.Process( dwTick );

	//	������ �÷��� �ѱ�
	m_bEnableRender = TRUE;

	return TRUE;
}


BOOL EventScene::Render( DWORD dwTick )
{
	m_eFadeState=eFADE_STATE_NONE;

	//	������ �÷��װ� ���� ������ ������ ���� ����
	if( m_bEnableRender == FALSE )
	{
		return TRUE;
	}

	if( m_iCurrentScene >= EVENT_SCENE_COUNT )
	{
		return TRUE;
	}

	//	���
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_FRONT );

	//	��� ������
	g_pMap->Render(dwTick);

	//	�߼��� ���� ������
	m_pCharacter[m_iCurrentScene]->EnableNormalMap(TRUE);
	g_pSunRenderer->RenderUnit( m_pCharacter[m_iCurrentScene] );

	return TRUE;
}


void EventScene::ParsePacket( MSG_BASE * pMsg )
{
	switch( pMsg->m_byCategory )
	{
	case 0:
	default:
		_asm { int 3 }
		break;
	}
}

void EventScene::RenderText( void )
{
}

void EventScene::OnRecvCG_CONNECTION( MSG_BASE * pMsg )
{
}
