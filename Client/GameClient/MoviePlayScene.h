

//=======================================================================================================================
/** MoviePlayScene
@author
	�Ӵ���
@since

@remarks
*/

#pragma once
#include "scenebase.h"
#include "bink.h"

class MoviePlayScene : public SceneBase
{
public:
	MoviePlayScene();
	virtual ~MoviePlayScene();

	BOOL	Init();							//	Scene Ȱ��ȭ�Ǿ��� ��, ��ǻ� activate()�� ���� ��Ȱ��.
	BOOL	Process( DWORD dwTick );		//	���� ���
	BOOL	Render( DWORD dwTick );			//	���� ������
	void	Release();						//	����
	void	ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	BOOL	OpenMovieFile( char *szFileName );

private:

	void	InitData( void );
	BOOL	GenerateEmptyTexture( void );

	BOOL	GetNextFrame( void );
	void	RenderText( void );
	void	DecideScene();

	BOOL	IsStopable( void );
	BOOL	IsQuitable( void );
	void	RenderFrameNo( void );

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	virtual void	RenderGameRatingMark(); // ���� ��� ��ũ ǥ��
	virtual void	LoadingGameRatingMarkTexture();
	virtual void	ReleaseGameRatingMarkTexture();
	void			RenderGameRatingMarkAndText();
	
#endif

private:

	HBINK	m_hBink;
	RECT	m_rcScreenRect;
	HANDLE	m_hTexture;
	WzColor	m_wzPrevClearColor;
	HANDLE  m_hTexGameRatingText;

};

extern MoviePlayScene g_MoviePlayScene;

