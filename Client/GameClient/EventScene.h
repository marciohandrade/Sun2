
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

	void Activate();						//	Ȱ��ȭ �Ǿ��� ��
	BOOL Init();							//	�ʱ�ȭ, �ε�
	BOOL Process( DWORD dwTick );			//	���� ���
	BOOL Render( DWORD dwTick );			//	���� ������
	void Release();							//	����
	void ParsePacket( MSG_BASE * pMsg );	//  ��Ʈ��ũ ��Ŷ ó��

	// �ؽ�Ʈ ���
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

