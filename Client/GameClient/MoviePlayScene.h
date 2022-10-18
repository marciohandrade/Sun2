

//=======================================================================================================================
/** MoviePlayScene
@author
	임대진
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

	BOOL	Init();							//	Scene 활성화되었을 때, 사실상 activate()와 같은 역활임.
	BOOL	Process( DWORD dwTick );		//	메인 계산
	BOOL	Render( DWORD dwTick );			//	메인 랜더링
	void	Release();						//	종료
	void	ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

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
	virtual void	RenderGameRatingMark(); // 게임 등급 마크 표시
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

