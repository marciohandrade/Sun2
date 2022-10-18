

#ifndef		_GameFramework_H
#define		_GameFramework_H

//#include <Protocol_IGC.h>




class SceneBase;

class GameFramework
{

private:

	static SceneBase	*s_pCurrentScene;
	static SceneBase	*s_pNextScene;
	static SceneBase	*s_pOldNextScene;

	//로딩신을 제외하고 처리할 다음씬\
	//현재는 배틀씬 -> 로딩씬 -> 미션씬 순으로 변경된다.
	//하지만 처리되어야 될 부분은 로딩씬이 없어야 됨.
	//현재는 배틀씬 -> 미션씬 순으로 변경됨을 기록함..
	static SCENE_TYPE   post_Scene_;

public:

	GameFramework() {}
	~GameFramework() { Destroy(); }

	BOOL CommitChangeState( void );
	static BOOL ChangeScene( SCENE_TYPE eScene );
	static SCENE_TYPE GetCurScene();
	static SCENE_TYPE GetPostScene();

	static SCENE_TYPE GetCurSceneType();
	static SCENE_TYPE GetPostSceneType();

	BOOL Create( SCENE_TYPE eScene );
	void Destroy( void );

	BOOL Update( DWORD dwTick );
	void SetViewport();
	void Set2DViewport();
	BOOL Render(  DWORD dwTick  );
	BOOL PreInterfaceRender( DWORD dwTick );
	BOOL PostInterfaceRender( DWORD dwTick );
	void Activate( void );
	void DeleteDeviceObjects( void );
	void InvalidateDeviceObjects( void );
	void RestoreDeviceObjects( void );
	void ParsePacket( MSG_BASE * pMsg );
	void ParsePacketInChat( MSG_BASE * pMsg );
	void OnDeviceReset();

   	static SceneBase	*GetCurrentScene();

	int OnEndRendering( DWORD dwTick );

#ifdef __NA_0_20080415_PROCESS_END_PROECSS
	int OnEndProcess( DWORD dwTick );
#endif

};






#endif	//	_GameFramework_H


