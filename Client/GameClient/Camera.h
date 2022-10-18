#ifndef		_CAMERA_H
#define		_CAMERA_H
//------------------------------------------------------------------------------
/**

@author

@since

@remarks
    -2005.06.22 카메라 상태 추가 -지완 
    -2005.06.22 Process 함수에 타임 틱 추가-지완 
    -2005.06.23 오브젝트 상속 제거(꼭필요한가요?)-지완 
    -2005.06.30 Todo:시간 단위 이동으로 바꾸자.-지완 
    -2005.06.30 스프링 카메라 작업 시작(연출용) - 지완 

*/

#define INIT_SWITCH                     0.0 
#define INIT_POWER                      0.05

#define	CAMERA_YEW_RANGE_MIN			0.00f
#define	CAMERA_YEW_RANGE_MAX			( WZ_PI * 2.0f )
#define	CAMERA_YEW_RANGE_CENTER			WZ_PI


#define CAMERA_DEFAULT_FOV              ( WZ_PI / 5.14f ) // 35도.

#define CAMERA_DEFAULT_NEAR_CLIP        1.0f
#define CAMERA_DEFAULT_FAR_CLIP         150.0f

#define CAMERA_DEFAULT_MOVE_SPEED		1.0f
#define CAMERA_DEFAULT_ROTATE_SPEED		0.005f

#define CAMERA_DISTANCE                 7.0f
#define CAMERA_DISTANCE_RANGE_MIN       1.0f
#define CAMERA_DISTANCE_RANGE_ALPHA_MIN 1.5f


enum{

	TARGET_LEFT=0,
	TARGET_REVERSE_LEFT,

	TARGET_RIGHT,
	TARGET_REVERSE_RIGHT,
};

class Camera 
{

public:
    Camera( void );
    ~Camera( void );

    enum CAMERA_STATUS 
    { 
        CAMERA_STATUS_NONE				= 0, 
        CAMERA_STATUS_VIBE				= 1,        //진동 상태 -타격 연출에 쓸것임 
        CAMERA_STATUS_EVENT             = (2 << 1), //이벤트 상태
        CAMERA_STATUS_SPRING            = (2 << 2), //스프링 연출용 
    };

	enum VIBE_TYPE
	{  
		VIBE_TYPE_NONE=0,
		VIBE_TYPE_ALL,
		VIBE_TYPE_VERTICAL,
		VIBE_TYPE_HORISONTAL,
	};

	enum
	{
		CAMERA_ROTATE_NONE,
		CAMERA_ROTATE_LMOUSE,
		CAMERA_ROTATE_RMOUSE,
	};

	enum
	{
		CAMERA_NOT_TURN		= 0,
		CAMERA_LEFT_TURN	= 1,
		CAMERA_RIGHT_TURN	= 2,
	};

protected:
    DWORD	        m_status;			           //현재 상태.		
    float	        m_twVibe;			           //진동 시간  
    float           m_vibePower;
    BOOL            m_twSwitch;
    CPathExplorer   *m_pPathExplorer;
	WORD            m_vibeType;
	int				m_iAngleCheckX;
	int				m_iAngleCheckZ;
	BOOL            m_bForceSpringCamera;

	float			m_fRotationSpeed;
	int				m_iRotationLevel;
	sScope			m_RotationScrollRange;

	BOOL 			m_bRidingTurn;


public:
	WORD GetVibeType() { return m_vibeType; }
	void SetVibeType(WORD value) { m_vibeType = value; }


    void            StartVibration(float time,float fPower=INIT_POWER);       // 일단 1초만 
	void            SetVibrationPower(float fPower){m_vibePower=fPower;}

    void            EndVibration();
	void            SetVibeType(VIBE_TYPE type) { m_vibeType = type; }

	BOOL            Create( float fMin, float fMax ,float fFOV);	
	virtual void    Destroy( void );
    virtual BOOL    Process( DWORD dwTick );

	const WzVector& GetCameraDirection() const { return m_CameraDirection; }
	const WzVector& GetCameraPosition() const { return m_CameraFrom; }
	const WzVector& GetCameraFrom() const { return m_CameraFrom; }
	const WzVector& GetCameraTo() const { return m_CameraTo; }

	float GetAngleYaw() const { return m_fAngleYaw; }
	float GetAnglePitch() const { return m_fAnglePitch; }
	void SetAngleYaw(float value) { m_fAngleYaw = value; }
	void SetAnglePitch(float value) { m_fAnglePitch = value; }

	void            SetCameraFromTo( const WzVector& wzFrom, const WzVector& wzTo)	{ m_CameraFrom = wzFrom; m_CameraTo = wzTo; }
	void            SetForceSpringCamera(BOOL bForce){m_bForceSpringCamera=bForce;}
	void            SetProjection( float fMin, float fMax ,float fFOV);
	void            RestoreProjection();
	void            SetCameraPosition( WzVector *WzTarget);
	void            SetCamera( WzVector* wvPos = NULL, BOOL bHeightCheck = FALSE ,DWORD dwTick=0);	
	void            SetTargetDirection( WzVector *wvDir );
	void            SetYaw(float fYaw);
	void            SetPitch(float fPitch);
	float           GetYaw();
    void            UpdateCamera();
	float           GetFarClip();
	float           GetNearClip();
	float           GetFOV();	
    float CameraHeight(){ return (m_fUp + m_CameraHeight); }		

	void            StartMove( void );
	void            InterpolateAngle();	
	void            KeyboardModeInterpolateAngle(float fAngle);	
	
	void            Rotate( int iAngleX, int iAngleZ = 0.0f, int iState = CAMERA_ROTATE_NONE );
	void            RotateYaw( int iAngle );

	void            MoveFrontBack( int iDist );
	void            SetCameraDistance(float fDist) { m_fDistance = fDist; }
	float           GetCameraDistance();
	void            MoveUpDown( float fDist ) { m_CameraHeight = fDist; }
	void            SetFocus(WzVector wzFocus);
    float           eye_height() const { return m_fUp; }
    void            set_eye_height(float val) { m_fUp = val; }

	void            SetAngle( float fAngle ) { m_fTargetDirectionAngle = fAngle; }
	void            SetAngle( WzVector* wvDir ) { SetAngle( Math_GetAngleFromVector( wvDir ) ); }

	float			GetAngle() { return m_fAngleYaw; }
	void            RotateAroundFocus(float fAngle);

	WzRay           GenerateCameraRay();

	void               SetAspectRatio(ENUM_SCREEN_RATIO eRatio);
	ENUM_SCREEN_RATIO  GetAspectRatio();



	void            AutoTurnState();
	void            CheckTurnState();
	void			SetRotateState( BOOL bState );
	BOOL			IsRotate() {return m_bIsRotate;}
	int				RotateMouseState() { return m_iRotateState;	}
	void			RotateAngleCheck( int iCheck ) { m_iAngleCheckX = iCheck; }
	int				GetStartRotateCheck() { return m_bStartRotate; }
	void			SetStartRotateCheck( BOOL bStartRotate ) { m_bStartRotate = bStartRotate; m_bStartRotateX = bStartRotate; m_bStartRotateZ = bStartRotate; }

	void            FreeCameraMoveFrontBack( int iDist );

	float           GetCurrentCameraFocusRange();

	void            InitSpringCamera(BOOL bSpring=FALSE);
	void            WarpFrom();


#ifdef _KST_CREATECHARACTER_SPRINGCAMERA_
	// 스프링 카메라를 직접 제어하기 위한 함수
	void InitSpringCameraPosition( const WzVector& From, const WzVector& To );
	void SetSpringCameraPosition( const WzVector& From, const WzVector& To, DWORD dwTick );

#else // _KST_CREATECHARACTER_SPRINGCAMERA_

	void            AttachToBone(const WzVector& From, const WzVector& To );
#endif // _KST_CREATECHARACTER_SPRINGCAMERA_


protected:
	void            SetBasicCamera( WzVector* wvPos, BOOL bHeightCheck,DWORD dwTick);	
	void            SetSpringCamera( WzVector* wvPos, BOOL bHeightCheck,DWORD dwTick);	

private:
    WzVector	    m_CameraPosition;
    WzVector	    m_CameraDirection;
    WzVector	    m_CameraFrom;
    WzVector	    m_CameraTo;

    WzVector	    m_wvX;
    WzVector	    m_wvY;
    WzVector    	m_wvZ;

	BOOL			m_bIsRotate;
	BOOL			m_bIsRotateX;
	BOOL			m_bIsRotateZ;

	float			m_fMoveSpeed;

    float		    m_fUp;


    float		    m_fDistance;
	float		    m_fPrevAngleYaw;
	float		    m_fAngleYaw;
    float		    m_fAngleYawStep;
    float		    m_fAngleYawDirection;
	float		    m_fPrevAnglePitch;
    float		    m_fAnglePitch;
	int				m_iPreYaw;
	int				m_iPrePitch;
	int				m_iRotateState;

    WzVector	    m_CameraTargetDirection;
    float		    m_fTargetDirectionAngle;

    BOOL		    m_bStartMove;
    float		    m_fAngle;
	float           m_fTurnAccelerate;

	float           m_fFOV;
    float           m_fNearClip;
    float           m_fFarClip;
	DWORD           m_dwWarpTime;
	ENUM_SCREEN_RATIO m_eRatio;
    
public:
	int				GetCameraTurnState() { return m_iTurnState; }
	int				GetRotationLevel();
	void			SetRotationLevel(int iLevel);
	void			SetRotationRange(int min, int max);
	sScope			GetRotationRange();
	int				GetRotationRangeMin();
	int				GetRotationRangeMax();

private:
	int				m_iTurnState;

public:
	float GetSpringConst() const { return m_springConst; }
	void SetSpringConst(float value) { m_springConst = value; }

	float GetDampConst() const { return m_dampConst; }
	void SetDampConst(float value) { m_dampConst = value; }

	float GetSpringLen() const { return m_springLen; }
	void SetSpringLen(float value) { m_springLen = value; }

private:

    vector3         SpringDamp( vector3 currPos,			// Current Position
                                const vector3& trgPos,		// Target Position
                                const vector3& prevTrgPos,	// Previous Target Position
                                float deltaTime,			// Change in Time
                                float springConst,			// Hooke's Constant
                                float dampConst,			// Damp Constant
                                float springLen);

	void			SetRotationSpeed();
	float			GetRotationSpeed();


protected:
    // 스프링 카메라 관련  
    WzVector         m_targetOldPos;
    WzVector         m_chasePos;
	BOOL			m_bStartRotate;
	BOOL			m_bStartRotateX;
	BOOL			m_bStartRotateZ;
	BOOL            m_bSpring;

	float			m_fRotateLevelX;
	float			m_fRotateSlowDownX;
	float			m_fRotateLevelZ;
	float			m_fRotateSlowDownZ;
	float			m_fDistanceLevel;
	float			m_fDistanceSlowDown;

	/// spiring constant
	float           m_springConst;
	/// damping constant
	float           m_dampConst;
	/// spring length constant
	float           m_springLen;
    
    float m_CameraHeight;
};

//------------------------------------------------------------------------------
/**
*/
inline void Camera::RotateYaw( int iAngle )
{
    m_fAngleYaw -= (float)(iAngle)  * 0.005f;
    if( m_fAngleYaw < CAMERA_YEW_RANGE_MIN )
    {
        m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
    }
    else if( m_fAngleYaw > CAMERA_YEW_RANGE_MAX )
    {
        m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
    }
}


extern	Camera					g_Camera;

#endif	//	_CAMERA_H

