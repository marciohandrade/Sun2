// autocameraplay.h

#ifndef _3DTERRAIN_AUTOCAMERAPLAY_H_
#define _3DTERRAIN_AUTOCAMERAPLAY_H_

class CWzArchive;

#define  ZCAMERA_CHANGE_SPEED (0.0005f)	///< 다른 카메라 동선,혹은 점 사이에. 천천히 바꿀 속도
#define  ZCAMERA_CHAINGE_TIME (1500)	///< 바꿀 시간 
#define  ZCAMERA_CHUNK			(0x1782)
#define  INVALID_CAMERA_TILE	(-1)

#define ZCAMERA_MAX_DIST	(10.0f)

enum EN_CAMERA_ATTRIBUTE///< 카메라 속성
{
	ECA_LINK=0x01,///< 링크된 카메라
	ECA_KEEP_DISTANCE=0x01<<1,///< 카메라 일정거리 유지
	ECA_LINE=0x01<<2,///< 직선 카메라 (디폴트는 곡선)
};
enum EN_MAPTILE_ATTRIBUTE
{
	EMA_ONEPOINT=0x01,
};
enum EN_PALYCAMERA_OPTION
{
	ENPO_FALL_BACK=0x01,///< eye가 뒤로 물러 나야 할때날때.
};

struct StC_Options {
	/// @name 마우스 거리 옵션
	BOOL		bMouseDist;
	float		fMouseDistFrontAccel;
	float		fMouseDistBackAccel;
	float		fMouseDistMax;
	float		fMouseDistReactTime;
	float		fMouseDistMinReactDist;

	/// @name 마우스 회전 옵션
	BOOL		bMouseRotate;
	float		fMouseRotateXmax;
	float		fMouseRotateYmax;
	float		fMouseRotateAccel;
	float		fMouseRotateReactTime;
	float		fMouseRotateMinReactDist;

	/// @name Spline정보 
	BOOL		bSpline;
	BOOL		blerfByTime;
	float		fSplineFrontAccel;
	float		fSplineBackAccel;
	float		fSplineReactTime;
	float		fSplineStartSlope;
	
};

struct StC_ControlPoint///제어점 
{
	WzVector			m_wvPosition;
	WzVector			m_wvTargetPt;
	WzVector			m_wvAddPt;
	DWORD				m_dwA;///<카메라 속성 상위 16비트는 그룹 인덱스 (이값이 같으면 같은 리스트)
};

struct StC_MapTile {
	int					m_iIndexStart;	///<카메라 리스트 시작 위치, 이값이  INVALID_CAMERA_TILE면 현재 링크된 카메라 정보가 없다.
	int					m_iIndexEnd;	///<카메라 리스트 끝점 위치
	DWORD				m_dwA;			///<카메라 타일 속성 
};
struct STC_HERMITE_CURVE///허밋 곡석
{
	WzVector		P0,P1,T0,T1;
	float			fGaussFactor;// f(-0.5773)+f(0.5773)
	float			fTotalLenth;

};
class CAutoCameraPlay  
{
protected:
	struct _PRE_CAMERA {
		int	m_iTile;
		int m_iCamera;

		
		float m_fPreT;					///<이전에 제어점 사이에서 얻어진 비율
		int	m_iCurrTile;
		BOOL m_bSameGroup;				
	};

	struct _HERMITE_VECTOR
	{
		STC_HERMITE_CURVE	HCurve;
		DWORD			mode;
		float			fT;
		float			fStressTime;
		float			fFrontSpeed;
		float			fFallBackSpeed;
		
		
		
		float			fCurrentLenth;
		

		float			fPassedTime;
		
		//float
		
	};
	//곡선 관련 param
	float				m_fCurveTension;		//긴장
	float				m_fCurveSkewness;		//왜도
	float				m_fCurveContinuity;		//연속성


	float				m_fCurveConstant[4];		
	
public:
	WzVector			m_vNextEye;		///<다음으로 옮길 eye좌표
	WzVector			m_vNextAt;		///<다음으로 옮길 at좌표
	WzVector			m_vPrePos;		///<이전 발위치
	WzVector			m_vFoot;
	

	StC_ControlPoint*	m_pPixedPoiont;		///<에디터로 부터 넘오면 제어점
	int					m_nPixedPoint;
	StC_MapTile*		m_pMapTile;			///<타일 정보 인덱스.
	int					m_nTileNum;			///<타일 갯수

	float				m_fIerpSpeed;		///<보간 속도
	DWORD				m_dwPreTime;		///<이전에 체크된 시간.
	_PRE_CAMERA			m_PreInfo;
	
	float				m_fMinDist;			///<최소 유지 거리.


//{ Test
public:
	_HERMITE_VECTOR		m_HermiteVector;
	float				m_fAngleStress;
	float				m_fDistStress;
	float				m_fZrotCurr;
	float				m_fZrotDest;
	float				m_fStraftCurr;
	float				m_fStraftDest;
	float				m_fDistDest;
	float				m_fDistCurr;

	int					m_iAngleType;
	int					m_iDistType;
	DWORD				m_dwOption;
	float				m_fDistMouse;
	StC_Options			m_stOtion;
	DWORD				m_dwDifferTime;


	
	

	int IncreaseStress(float *pfStess,float fDesireTime, DWORD dwDifferTime);
	void _CheckAngle(int iTile,WzVector* dest,float fScreenX,float fScreenY,float fFootX,float fFootY,DWORD dwTime);
	void _CheckHermite(DWORD dwTime);
//} Test

protected:
	
	void _InitHermiteCurve(STC_HERMITE_CURVE& hc,int iCameraIdx);
	float _GetHermiteSpeed(STC_HERMITE_CURVE& hc,float t);
	float _GetHermiteLengh(STC_HERMITE_CURVE& hc,float t);
	float _SolveHermiteEquation(STC_HERMITE_CURVE& hc,float fT,float fLenth,bool bReverse=false);
	void  _GetHermiteCurveTangent0(int iCameraIdx,WzVector* pWVRes);
	void  _GetHermiteCurveTangent1(int iCameraIdx,WzVector* pWVRes);

	void _Release();						///<에디터로 부터 넘오면 구조 해제
	void _SatisfyVector(WzVector* pPre,WzVector* pNext,float fUpdate);///<현재와 다음사이에 보간. 
	int	 _GetFirstValidIndex();

	//현재와 다음사이에 보간. ==>카메라 이동 속도 제한용 현재는 테스트중.
	void _SatisfyVector2(WzVector* pPre,WzVector* pNext,float fUpdate,DWORD dwTime);
	BOOL _IsLinked(StC_ControlPoint* pSpp){return pSpp->m_dwA & ECA_LINK;}
	BOOL _IsValidTile(StC_MapTile* pMaptile){return pMaptile->m_iIndexStart != INVALID_CAMERA_TILE;}
	int  _GetProperIndexFromCameraList(WzVector wvDest,int iTile);;///<만족하는 카메라 링크를 얻는다.
	BOOL _IsSameGroup(int index1,int index2);///<두타일이 같은 그룹인지 본다.
	void _PlayTwo( int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt,float t);
	float _GetCameraTvalue(int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt);
	void _CameraLenthIncrease(int* pCameraIdx,float* pT);
	void _CameraLenthDecrease(int* pCameraIdx,float* pT);


public:
	virtual void SetInvaildLink(){m_PreInfo.m_iTile = INVALID_CAMERA_TILE;	}
	virtual BOOL IsValidLink(){return m_PreInfo.m_iTile != INVALID_CAMERA_TILE;}
	virtual const StC_MapTile* GetMapTileBuff(){return m_pMapTile;}
	virtual BOOL Serialize( CWzArchive *pArchive);
	virtual const StC_ControlPoint* GetCameraBuff(){return m_pPixedPoiont;}
	virtual int GetNumCamera(){return m_nPixedPoint;}
	virtual int GetNumTile(){return m_nTileNum;}
	virtual void Play(int iCovexNum,WzVector* dest,WzVector* pEye,WzVector* pAt
		,float fScreenX,float fScreenY,float fFootX,float fFootY,DWORD dwTime); ///<메인 카메라 플레이

	virtual void PlayOne(int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt);///<한점 카메라
	virtual void PlayTwo(int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt);///<두점 카메라

	virtual void CopyMember(StC_ControlPoint* pPixedPoint,int point,StC_MapTile* pMapTile,int maptile);	//에디트 정보 설정
	virtual void SetMoveSpeed(float fSpeed){m_fIerpSpeed=fSpeed;}	///<보간 속도 결정
	virtual void StartCamera(int iTileNum); ///<초기값 설정

	virtual void SetMinDist(float fDist){m_fMinDist=fDist;}

	virtual void Reset(DWORD dwTime);
	virtual void SetOtion(StC_Options* pStOption);
	virtual StC_Options* GetOtion(){return &m_stOtion;}

//에디터에서 사용
	virtual void SetCurveParam(float fT,float fS,float fC);
	virtual void GetCurveParam(float *pfT,float *pfS,float *pfC){*pfT=m_fCurveTension;*pfS=m_fCurveSkewness;
																	*pfC=m_fCurveContinuity;}
	virtual void CalculateCurve(STC_HERMITE_CURVE *pOutHC,const WzVector *pwv);
	virtual void GenHermiteSpline(STC_HERMITE_CURVE& hc,float t,WzVector* wvResult);

	
	
	
	
	CAutoCameraPlay();
	virtual ~CAutoCameraPlay();

};

#endif // _3DTERRAIN_AUTOCAMERAPLAY_H_
