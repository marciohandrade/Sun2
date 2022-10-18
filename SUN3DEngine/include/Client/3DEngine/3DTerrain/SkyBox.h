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
	BOOL m_bSunSky;				// �¾�SkyBox
	BOOL m_bArea;				// �¾翵��
	int m_iWeight;				// ����ġ
	int m_iExposure;			// Ȯ��
	int m_iDecay;				// ����
	int m_iDensity;				// ����
	int m_iBoneIndex;			// ���ε���
	int m_iAreaIndex;			// �����ε���
	int m_iFadeTime;			// ���̵�Ÿ��
	WzColor m_wLightColor;		// ��������Ʈ�÷�
}VolumeLightInfo;

typedef struct
{
	char *m_lpszFileName;		// ���ϸ�
	WzID m_wiUnit;				// WzUnit �ڵ�
	CWzUnitDraw *m_pwzUnitDraw;	// WzUnitDraw
	DWORD m_dwAttribute;		// �Ӽ�
	float m_fScale;				// ������
	WzColor m_wcDraw;			// Į��
	WzVector m_wvRotateAxis;	// ȸ����
	float m_fRotateRate;		// ȸ���ӵ�
	float m_fAccumulateZRotate;	// Z��ȸ�� ���� 
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


// Sky Box Object �Ӽ�
#define SBOA_HIDE	( 0x00000001)

// SkyBox class
class CSkyBox  
{
public:
	CSkyBox();
	virtual ~CSkyBox();
	virtual void Clear();

	// ����/����
#ifdef CLIENT_ONLY
	virtual BOOL Create( CDrawBase *pDraw, char *lpszFolderPath);
	virtual void Destroy();
#endif

	virtual BOOL Serialize( CWzArchive *pArchive);

#ifdef CLIENT_ONLY
	// �⺻ ������
protected:
	SkySunInfo m_SunInfo;
	CDrawBase *m_pDraw;
	char m_lpszFolderPath[MAX_PATH];
	char m_lpszFolderPath4Sky[MAX_PATH + 1];

	// ��ü ����Ʈ
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
	// ���/���
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
