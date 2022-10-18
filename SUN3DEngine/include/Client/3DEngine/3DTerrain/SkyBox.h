// skybox.h

#ifndef _3DTERRAIN_SKYBOX_H_
#define _3DTERRAIN_SKYBOX_H_

#ifdef CLIENT_ONLY
#include <client/3dengine/3dfileaccess/3dfileaccess.h>
#include <client/3dengine/3dfileaccess/wzunit/wzunitdraw.h>
#else
#include <programcommon/wzarchive.h>
#endif



class CDrawBase;


#ifdef CLIENT_ONLY



enum EN_SKYBOX_ATTR
{
	eSBA_HIDE=0x01,

};
// Sky Box Object
#define MAXNUM_SKYBOX_OBJECT	( 128)
#define MAX_SKYMAP_FILE_NAME_LENGTH 256 +1

typedef struct _VolumeLightInfo
{
	BOOL m_bSunSky;				// 태양SkyBox
	BOOL m_bArea;				// 태양영역
	int m_iWeight;				// 가중치
	int m_iExposure;			// 확산
	int m_iDecay;				// 감소
	int m_iDensity;				// 길이
	int m_iBoneIndex;			// 본인덱스
	int m_iAreaIndex;			// 영역인덱스
	int m_iFadeTime;			// 페이드타임
	WzColor m_wLightColor;		// 볼륨라이트컬러
}VolumeLightInfo;

typedef struct
{
	char *m_lpszFileName;		// 파일명
	WzID m_wiUnit;				// WzUnit 핸들
	CWzUnitDraw *m_pwzUnitDraw;	// WzUnitDraw
	DWORD m_dwAttribute;		// 속성
	float m_fScale;				// 스케일
	WzColor m_wcDraw;			// 칼라
	WzVector m_wvRotateAxis;	// 회전축
	float m_fRotateRate;		// 회전속도
	float m_fAccumulateZRotate;	// Z축회전 축적 
	VolumeLightInfo m_SunSky;
	
} St_SkyBoxObjectInfo;

typedef struct _SkySunInfo
{
	BOOL m_bExgistSun;
	BOOL m_bNeedUpdate;
	int m_iSunIndex;
	int m_iTick;
	float m_fAlpha;
	POINT m_Pos;
	VolumeLightInfo* m_pConstant;
}SkySunInfo;

#endif


// Sky Box Object 속성
#define SBOA_HIDE	( 0x00000001)

// SkyBox class
class CSkyBox  
{
public:
	CSkyBox();
	virtual ~CSkyBox();
	virtual void Clear();

	// 생성/제거
#ifdef CLIENT_ONLY
	virtual BOOL Create( CDrawBase *pDraw, char *lpszFolderPath);
	virtual void Destroy();
#endif

	virtual BOOL Serialize( CWzArchive *pArchive);

#ifdef CLIENT_ONLY
	// 기본 데이터
protected:
	SkySunInfo m_SunInfo;
	CDrawBase *m_pDraw;
	char m_lpszFolderPath[MAX_PATH];
	char m_lpszFolderPath4Sky[MAX_PATH + 1];

	// 객체 리스트
protected:
	int m_iNumSkyBoxObject;
	St_SkyBoxObjectInfo m_SkyBoxObjects[MAXNUM_SKYBOX_OBJECT];

public:
	virtual int GetNumberOfSkyBoxObject()	{ return ( m_iNumSkyBoxObject); }
	virtual St_SkyBoxObjectInfo* GetSkyBoxObjectInfo( int iIndex);
	virtual void AddSkyBoxObject( char *lpszFileName , BOOL bLoadDefualt=true);
	virtual void RemoveSkyBoxObject( int iIndex);
	virtual void UpdateSkyBox(St_SkyBoxObjectInfo* pSkyBoxObjectInfo);
	virtual void UpdateSunInfo();
   
protected:
	BOOL LoadSkyBoxObject( int iIndex);
	void DeleteSkyBoxObject( int iIndex);
	
#endif

#ifdef CLIENT_ONLY
	// 계산/출력
public:
	virtual void HeartBeat( DWORD dwTick);
	virtual void Render();
	void GetFolderPath(TCHAR* szVal){strncpy(szVal, m_lpszFolderPath, MAX_PATH-1);}

	virtual void VolumeLightMasking();
	SkySunInfo& GetVolumeLightinfo(){ return m_SunInfo; }
	void ProcessVolumeLight(DWORD tick);
#endif
};
#endif // _3DTERRAIN_SKYBOX_H_
