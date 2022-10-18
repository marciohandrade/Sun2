

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

	//�ε����� �����ϰ� ó���� ������\
	//����� ��Ʋ�� -> �ε��� -> �̼Ǿ� ������ ����ȴ�.
	//������ ó���Ǿ�� �� �κ��� �ε����� ����� ��.
	//����� ��Ʋ�� -> �̼Ǿ� ������ ������� �����..
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


