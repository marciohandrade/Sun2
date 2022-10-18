// autocameraplay.h

#ifndef _3DTERRAIN_AUTOCAMERAPLAY_H_
#define _3DTERRAIN_AUTOCAMERAPLAY_H_

class CWzArchive;

#define  ZCAMERA_CHANGE_SPEED (0.0005f)	///< �ٸ� ī�޶� ����,Ȥ�� �� ���̿�. õõ�� �ٲ� �ӵ�
#define  ZCAMERA_CHAINGE_TIME (1500)	///< �ٲ� �ð� 
#define  ZCAMERA_CHUNK			(0x1782)
#define  INVALID_CAMERA_TILE	(-1)

#define ZCAMERA_MAX_DIST	(10.0f)

enum EN_CAMERA_ATTRIBUTE///< ī�޶� �Ӽ�
{
	ECA_LINK=0x01,///< ��ũ�� ī�޶�
	ECA_KEEP_DISTANCE=0x01<<1,///< ī�޶� �����Ÿ� ����
	ECA_LINE=0x01<<2,///< ���� ī�޶� (����Ʈ�� �)
};
enum EN_MAPTILE_ATTRIBUTE
{
	EMA_ONEPOINT=0x01,
};
enum EN_PALYCAMERA_OPTION
{
	ENPO_FALL_BACK=0x01,///< eye�� �ڷ� ���� ���� �Ҷ�����.
};

struct StC_Options {
	/// @name ���콺 �Ÿ� �ɼ�
	BOOL		bMouseDist;
	float		fMouseDistFrontAccel;
	float		fMouseDistBackAccel;
	float		fMouseDistMax;
	float		fMouseDistReactTime;
	float		fMouseDistMinReactDist;

	/// @name ���콺 ȸ�� �ɼ�
	BOOL		bMouseRotate;
	float		fMouseRotateXmax;
	float		fMouseRotateYmax;
	float		fMouseRotateAccel;
	float		fMouseRotateReactTime;
	float		fMouseRotateMinReactDist;

	/// @name Spline���� 
	BOOL		bSpline;
	BOOL		blerfByTime;
	float		fSplineFrontAccel;
	float		fSplineBackAccel;
	float		fSplineReactTime;
	float		fSplineStartSlope;
	
};

struct StC_ControlPoint///������ 
{
	WzVector			m_wvPosition;
	WzVector			m_wvTargetPt;
	WzVector			m_wvAddPt;
	DWORD				m_dwA;///<ī�޶� �Ӽ� ���� 16��Ʈ�� �׷� �ε��� (�̰��� ������ ���� ����Ʈ)
};

struct StC_MapTile {
	int					m_iIndexStart;	///<ī�޶� ����Ʈ ���� ��ġ, �̰���  INVALID_CAMERA_TILE�� ���� ��ũ�� ī�޶� ������ ����.
	int					m_iIndexEnd;	///<ī�޶� ����Ʈ ���� ��ġ
	DWORD				m_dwA;			///<ī�޶� Ÿ�� �Ӽ� 
};
struct STC_HERMITE_CURVE///��� �
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

		
		float m_fPreT;					///<������ ������ ���̿��� ����� ����
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
	//� ���� param
	float				m_fCurveTension;		//����
	float				m_fCurveSkewness;		//�ֵ�
	float				m_fCurveContinuity;		//���Ӽ�


	float				m_fCurveConstant[4];		
	
public:
	WzVector			m_vNextEye;		///<�������� �ű� eye��ǥ
	WzVector			m_vNextAt;		///<�������� �ű� at��ǥ
	WzVector			m_vPrePos;		///<���� ����ġ
	WzVector			m_vFoot;
	

	StC_ControlPoint*	m_pPixedPoiont;		///<�����ͷ� ���� �ѿ��� ������
	int					m_nPixedPoint;
	StC_MapTile*		m_pMapTile;			///<Ÿ�� ���� �ε���.
	int					m_nTileNum;			///<Ÿ�� ����

	float				m_fIerpSpeed;		///<���� �ӵ�
	DWORD				m_dwPreTime;		///<������ üũ�� �ð�.
	_PRE_CAMERA			m_PreInfo;
	
	float				m_fMinDist;			///<�ּ� ���� �Ÿ�.


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

	void _Release();						///<�����ͷ� ���� �ѿ��� ���� ����
	void _SatisfyVector(WzVector* pPre,WzVector* pNext,float fUpdate);///<����� �������̿� ����. 
	int	 _GetFirstValidIndex();

	//����� �������̿� ����. ==>ī�޶� �̵� �ӵ� ���ѿ� ����� �׽�Ʈ��.
	void _SatisfyVector2(WzVector* pPre,WzVector* pNext,float fUpdate,DWORD dwTime);
	BOOL _IsLinked(StC_ControlPoint* pSpp){return pSpp->m_dwA & ECA_LINK;}
	BOOL _IsValidTile(StC_MapTile* pMaptile){return pMaptile->m_iIndexStart != INVALID_CAMERA_TILE;}
	int  _GetProperIndexFromCameraList(WzVector wvDest,int iTile);;///<�����ϴ� ī�޶� ��ũ�� ��´�.
	BOOL _IsSameGroup(int index1,int index2);///<��Ÿ���� ���� �׷����� ����.
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
		,float fScreenX,float fScreenY,float fFootX,float fFootY,DWORD dwTime); ///<���� ī�޶� �÷���

	virtual void PlayOne(int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt);///<���� ī�޶�
	virtual void PlayTwo(int iIndex,WzVector* dest,WzVector* pEye,WzVector* pAt);///<���� ī�޶�

	virtual void CopyMember(StC_ControlPoint* pPixedPoint,int point,StC_MapTile* pMapTile,int maptile);	//����Ʈ ���� ����
	virtual void SetMoveSpeed(float fSpeed){m_fIerpSpeed=fSpeed;}	///<���� �ӵ� ����
	virtual void StartCamera(int iTileNum); ///<�ʱⰪ ����

	virtual void SetMinDist(float fDist){m_fMinDist=fDist;}

	virtual void Reset(DWORD dwTime);
	virtual void SetOtion(StC_Options* pStOption);
	virtual StC_Options* GetOtion(){return &m_stOtion;}

//�����Ϳ��� ���
	virtual void SetCurveParam(float fT,float fS,float fC);
	virtual void GetCurveParam(float *pfT,float *pfS,float *pfC){*pfT=m_fCurveTension;*pfS=m_fCurveSkewness;
																	*pfC=m_fCurveContinuity;}
	virtual void CalculateCurve(STC_HERMITE_CURVE *pOutHC,const WzVector *pwv);
	virtual void GenHermiteSpline(STC_HERMITE_CURVE& hc,float t,WzVector* wvResult);

	
	
	
	
	CAutoCameraPlay();
	virtual ~CAutoCameraPlay();

};

#endif // _3DTERRAIN_AUTOCAMERAPLAY_H_
