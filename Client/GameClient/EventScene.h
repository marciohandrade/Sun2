
#ifndef		EVENTSCENE_H
#define		EVENTSCENE_H

#include "SceneBase.h"


#define		EVENT_SCENE_COUNT			5


struct EVENT_CAMERA
{
	int			iFrame;

	WzVector	wvCameraPos;
	WzVector	wvLookAt;
	WzVector	wvUp;
};



typedef		std::vector<EVENT_CAMERA *>			EventCamera_vector;
typedef		EventCamera_vector::iterator		EventCamera_iterator;



class EventCamera
{
	WzVector				m_wvPos;
    WzVector			    m_wvLook;
	WzVector				m_wvUp;
	int						m_iLastFrame;
	int						m_iCurrentFrame;
	float					m_fCurrentFrame;
	EventCamera_vector		m_vectorEventCamera;


public:
	EventCamera( void );
	~EventCamera( void );

	void		Init( void );
	void		Destroy( void );
	BOOL		Process( DWORD dwTick );

	int			GetCurrentFrame( void ) { return m_iCurrentFrame; }
	int			GetLastFrame( void ) { return m_iLastFrame; }

    int			fGetCurrentFrame( void ) { return (int)m_fCurrentFrame; }
};



class EventScene : public SceneBase
{
public:
	EventScene(void);
	virtual ~EventScene(void);

	void Activate();						//	활성화 되었을 때
	BOOL Init();							//	초기화, 로딩
	BOOL Process( DWORD dwTick );			//	메인 계산
	BOOL Render( DWORD dwTick );			//	메인 랜더링
	void Release();							//	종료
	void ParsePacket( MSG_BASE * pMsg );	//  네트워크 패킷 처리

	// 텍스트 출력
protected:
	void RenderText( void );

protected:
	void OnRecvCG_CONNECTION( MSG_BASE * pMsg );


protected:
	WzVector			m_wvPos;
    WzVector			m_wvLook;

	DWORD				m_dwCharacter[EVENT_SCENE_COUNT];
	CWzUnitDraw			*m_pCharacter[EVENT_SCENE_COUNT];

	int					m_iCurrentScene;
	EventCamera			m_EventCamera;

};

extern EventScene g_EventScene;


#endif	//	EVENTSCENE_H

