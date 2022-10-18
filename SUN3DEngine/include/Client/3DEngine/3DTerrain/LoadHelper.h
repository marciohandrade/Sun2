// loadhelper.h

#ifndef _3DTERRAIN_LOADHELPER_H_
#define _3DTERRAIN_LOADHELPER_H_

//------------------------------------------------------------------------------
/**
    ���Ϸ� ���� �ʻ� �����ϴ� �������� ������Ʈ���� �ε��ϱ� ���� ����ϴ� 
    Ŭ�����̴�. ������Ʈ/SpeedTree/Grass/LocalLight���� �ε带 ���� ������
    ���̱� ������ �ε����Ŀ� �̸� �����ؼ� ������� �ʵ��� �Ѵ�.

    @modify
    @2006.3.7 ~ 4.19 : Script�� �̿��� ������Ʈ �ε� ����.
    @2006.4.24 : Sun/Wiki Studio�� �� ������Ʈ �ε� �κп� LoadHelper ���.

    @author: �����, ���ſ�(3D Engine Team)

    Copyright 2004 WEBZEN All rights.
*/

#include <programcommon/wzarray.h>
#include <programcommon/wzarchive.h>
#include <programcommon/wzsingleton.h>

#include <client/3dengine/common/wzcolor.h>
#include <client/3dengine/common/wzuvvert.h>
#include <client/3dengine/common/boundingvolume.h>

// ��ó�� ����
#ifdef CLIENT_ONLY
class CDrawBase;
#endif

class CWorldBase;

/// ������Ʈ ���� ������ ��� ����ü.
struct St_LHObjectWZU
{
	char*		m_pszFileName;
	WzID		m_WzID;
	BYTE		m_Attr[4];							///< 0: WZU������ �ν��Ͻ� ������� 1: ȭ�鿡�� �۰� ���� ��� ������ ����
													///< 2: ����						3: ����

	// ������ ���õ� ����
	int			m_iID;								///< �ε�� WZM�� �ĺ� �ε��� ��.
};

struct St_LHObjectSpecialTexture
{
	char*		m_pszFileName;
	WzID		m_WzID;

	int			m_iID;								///< �ε�� WZM�� �ĺ� �ε��� ��.
};

struct St_LHAtdEffect
{
    WzID    m_wzID;         ///< ������Ʈ�� �پ� �ִ� Loop Effect�� WzID
    short   m_iRotAngle[3]; ///< X,Y,Z �࿡ ���� ȸ����(0 ~ 360��)
    float   m_fScale;       ///< Effect�� ��ü Scale�� ���� ��Į��
};

struct St_LHObject
{
	WzID					m_WzID;
	DWORD					m_dwAttribute;
	WzVector				m_wvPos;
	WzVector				m_wvScale;
	WzVector				m_wvRot;
	WzBoundingVolume		m_wzBV;
	WzID					m_wiUser;				///< ������ �Է��� WZID.
	DWORD					m_dwExtraAttibue[3];	///< ??? ���� �۾��ڰ� �س��� �κ�. �뵵�� �𸣰� �ִ�.
	DWORD					m_dwParam[4];			///< �������� �Է��� �߰� ������ ���� �Ķ����.
    St_LHAtdEffect          m_AtdEffect[4];
	int						m_iID;					///< �ε�� WZM�� �ĺ� �ε��� ��.
	WzColor					m_wcDraw;				//_YJ_EDIT_OBJ_COLOR
    int					    m_iCollision;			//_GS_OBJ_COLLISION
};


/// Ǯ ������Ʈ ����ü
struct St_LHGrass
{
	WzVector		m_wvPos;
	WzVector		m_wvRot;
	WzVector		m_wvScale;
	BYTE			m_byAttribute;

	// ������ ���� ������.
	int				m_iID;
};

/// Ǯ ������Ʈ�� ���� WZU���� ���� ����ü
struct St_LHGrassWZU
{
	WzID			m_WzID;
	int 			m_nNumGrass;
	St_LHGrass*		m_pGrass;

	// ������ ���� ������.
	int				m_iLastIndex;
};


/// ���ǵ� Ʈ�� ����ü
struct St_LHSpeedTree
{
	WzID			m_WzID;
	DWORD			m_dwAttribute;
	WzVector		m_wvPos;
	WzVector		m_wvScale;
	WzColor			m_wcDraw;	//_YJ_EDIT_OBJ_COLOR

	// ������ ���� ������.
	int				m_iID;
};

/// ���ǵ� Ʈ�� ���� ���� ����ü
struct St_LHSpeedTreeFile
{
	char*		m_pszFileName;
	WzID		m_WzID;

	// ������ ���� ������.
	int				m_iID;
};

/// ���ǵ� Ʈ�� ��� ���� ����ü
struct St_LHSpeedTreeHeader
{
	char		m_szCompositeTexture[256];
	char		m_szCompositeShadowTexture[256];
};


/// ����Ʈ ���� ����ü
struct St_LHLight
{
	WzVector	m_wvPos;
	WzColor		m_wc;
	float		m_fRange;
	float		m_fIntensity;
	BYTE		m_byIrradianceRef;

	// ������ ���� ������.
	int				m_iID;
};


/// Mesh�� ���� ����Ʈ�� ������ ��� ���� ����ü
/// @author ������
typedef struct St_LHMeshGroupLM
{
	int			NumObj;					//������Ʈ ��ȣ
	char		WZU_Filename[MAX_PATH];	//WZU���� �̸�
	int			NumMeshGroup;			//�޽��׷� ��ȣ
	char		LM_Filename[MAX_PATH];	//����Ʈ�� ���� �̸�
	int			NumLMVertex;			//����Ʈ�� ���ؽ� ����
	int			*FaceTextureNum;		//�ش� Face�� ���� ����Ʈ�� �ؽ��� ��ȣ
	WzUVVert	*ListLMVertex;			//����Ʈ�� ���ؽ� ����Ʈ

} LMMeshGroup;

typedef struct St_LHTerrainLM
{
	char TerrainLMName[MAX_PATH];		//���� LM �̸�
} TerrainLM;

struct St_LHUnitPath
{
	WzVector wvStartPos;
	WzVector wvPath;
    BYTE     byWalkState;               // 0 �ȱ�, 1 �޸���
};
typedef CWzArray<St_LHUnitPath> LHPathList;

// Structure : St_LHUnit
//
// ���� �� ���� ����:
//  1) 0x0002: ���� �н� ���� �߰�.
//  2) 0x0003: Ʈ���� ���� ���ֿ� ���� ���� �߰�.
//  3) 0x0004: ���� ���� ���� ���� �߰�.
//  4) 0x0005: �� ��Ʈ�� ��λ󿡼� �ȱ�/�޸��� ���� �߰�
#define VERSION_UNITDATAFORMAT		(0x0005)
#define NUM_REGENUNITTYPE			(5)
struct St_LHUnit
{
	WORD						wVersion;

	// ����Ǵ� ������ ����
	char						name[128];			// �̸�	
	char						wzufilename[128];	// ������ �����ϴ� WZU ���ϸ�
	int							category;
	WzID						wID;
	
	float						radian;				// ������ �ü�����
	BYTE						unit_type;			// ��ġ Ÿ��
	BYTE						ai_type;			// AI ����
    /// ������ ����Ÿ�� (0:�Ϲݸ���, 1:�������� 2:��������Ÿ�� - ��Ʈüũ�Ͽ� ���, ���� 0,1 �� ��� ������� ����)
	BYTE						ucRegenType;
    /// ������ Ű���� ����(1���� ū��� ��������Ÿ�� �������� �Ѵ�)
	BYTE						ucNumKey;
	bool						bRefTrigger;		// ������ Ʈ���� �ý��ۿ� ����Ǿ��ִ�.
	DWORD						regentime;			// ��� �ð�
	float						regenProbability;	// ��� Ȯ��
	
	LHPathList					PathList;			// ������ ��ġ�� �� �ִ� ��ġ����.
	int							iKeyList[NUM_REGENUNITTYPE]; // ���� ���� ����
	float						fRegenRate[NUM_REGENUNITTYPE]; // ���� Ȯ��
	DWORD						userdata[2];
};

#define	VERSION_UNITGROUPDATAFORMAT	(0x0001)
struct St_LHUnitGroup
{
	WzID						wID;
	WzID						leader;				// ���� ������ ID
	DWORD						userdata[2];		// ������� ����

	// ���� �׷쿡 ���� ���ֵ鿡 ���� ������ ���.
	CWzArray<WzID>				unitlist;
};


typedef struct St_LHMapOption
{
	WzVector	vPosition;
	WzVector	vLookAt;
				
	// ���� 
	float		fBrightArg[3];          // Brightness Function�� ���� Arguments(���� ������)
	WzColor		wcShadow;

	// Projection params...
	float		fFov;
	float		fNear;
	float		fFar;

	// Sight params...
	float		fVisibleRange;	        // ���� ����
	float		fAlphaRange;	        // Alpha���� ����Ǿ� �������� �Ǵ� ����
	float		fVisibleRate;	        // ����� ������ ����

	// Render option
	float		fBrightness;	        // ���� �������� ��� ������ ���� ��.

	// Params for bloom...
	int			iBlurCount;				// �� Ƚ��
	float		fMinLuminance;			// ������ �ּ� �� 0.0~1.0
	float		fSelectHighLight;		// ���õ� ���� HighLight�� 
	WzColor		wcSelectColor;			// ������ ����
	float		fFinalColorBlend;		// ���� �������� �󸶸�ŭ ���� �Ҳ���.
	float		fOriginalColorBlend;	// �����̹��� ���� ����

    WzID        WeatherEffect;          // ����ȿ�� ����Ʈ�� WzID

	float		fDOF_Rate;
	float		fDOF_Range;
} LHMapOption;

typedef struct St_LHFileHeader
{
	char  cIdentify[4];
	WORD  wFirstVersion;
	WORD  wSecondVersion;
	DWORD dwCheckSum;

} LHFileHeader;

enum Enum_FileType
{
	FILETYPE_MAP,
	FILETYPE_MAPOBJECT,
	FILETYPE_MAPSOUND,
	FILETYPE_MAPUNIT,
	FILETYPE_MAPOPTION,
};


class CLoadHelper : public CWzSingleton<CLoadHelper>  
{

public:
	CLoadHelper();
	virtual ~CLoadHelper();
	/** �ε�� ������ �����ϰ� �Ҵ�� ��� �޸𸮸� �����Ѵ�. */
	virtual void DeleteAll();

	/**	@name Object
	 *	
	 *	SpeedTree�� ���õ� �Լ���.
	 *	@{
	 */

	virtual BOOL						LoadObj(CWzArchive* pArchive);
	virtual int							GetObjWzuCount();
	virtual St_LHObjectWZU				*GetObjWzu();
	virtual int							GetSpecialTextureCount();
	virtual St_LHObjectSpecialTexture*  GetSpecialTexture();
	virtual int							GetObjectCount();
	virtual St_LHObject					*GetObject();
	/** ������Ʈ�� ���� ������ �����ϰ� �޸𸮸� ��ȯ�Ѵ�. */
	virtual void						DeleteObject();

	virtual int							GetLastIndexObjWzu() { return m_iLastIndexObjWZU; }
	virtual int							GetLastIndexSpecTex() { return m_iLastIndexSpecTex; }
	virtual int							GetLastIndexObj() { return m_iLastIndexObj; }

	/** ������Ʈ �ν��Ͻ����� ���� �޸� ������ �Ҵ��Ѵ� */
	virtual void						AllocateArrayForWzu(int size);
	virtual void						AllocateArrayForSpecailTexture(int size);
	virtual void						AllocateArrayForObject(int size);
	virtual void						ExpandArrayForObject(int size);
	/** ������Ʈ�� �ε��Ѵ�. */
	virtual void						LoadWzu(int index, LPSTR lpFilename, WzID wID);	
	virtual void						LoadSpecailTexture(int index, LPSTR lpFilename, WzID wID);
	
	virtual int							ReallocateArrayForWzu(int size);

	/**
	 * �־��� �̸��� WzID�� WZU object�� �ִ��� ã�´�.
	 * @return : 0�̻��� ��� ���� ��Ͽ��� Wzu object�� �ε����̴�.
	 */
	virtual int							FindObjectWzu(LPSTR lpFilename, WzID wID);
	/**
	 * �־��� �̸��� WzID�� SpecialTexture�� �ִ��� ã�´�.
	 * @return : 0�̻��� ��� ���� ��Ͽ��� SpecialTexture�� �ε����̴�.
	 */
	virtual int							FindSpecailTexture(LPSTR lpFilename, WzID wID);

	/**	@}	*/ // end of Object


	/**	@name Grass
	 *	
	 *	Grass�� ���õ� �Լ���.
	 *	@{
	 */

	/** Map������ Grass ������ �ε��Ѵ�. */
	virtual BOOL						LoadGrass(CWzArchive* pArchive);
	virtual int							GetGrassWZUCount();
	virtual int							GetGrassCount(int iWZU);
	virtual St_LHGrassWZU				*GetGrassWZU();
	virtual int							GetTotalGrassCount();
	/** Grass�� ���� ������ �����ϰ� �޸𸮸� ��ȯ�Ѵ�. */
	virtual void						DeleteGrass();

	virtual BOOL CreateGrassWzuList(int nGrassTotal, int nGrassWZU);
	virtual BOOL CreateGrassList(int iWZU, WzID wID, int nNumGrass, int iLastGrass);
	/**
	 * �ش� �ε����� �ν��Ͻ��� ���� ������ �����Ѵ�.
	 * @param index : �迭�� �ε���
	 * @param iWzu : Grass ��ü�� ����ϴ� Wzu ������ �ε�����
	 * @param iID : �ĺ� �ε���
	 * @param vPos :
	 * @param sRot : 
	 * @param sScale :
	 * @param byAttrubte :
	 */
	virtual void SetGrass(int index, int iWZU, int iID, WzVector *vPos, short sRot, short sScale, BYTE byAttribute);
	/**
	 * ���� �ε�� Grass ����� �ش� �ν��Ͻ��� ���� ������ �����Ѵ�.
	 * @param index : �迭�� �ε���
	 * @param iWzu : Grass ��ü�� ����ϴ� Wzu ������ �ε�����
	 * @param vPos :
	 * @param sRot : 
	 * @param sScale :
	 * @param byAttrubte :
	 */
	virtual void ModifyGrass(int index, int iWZU, WzVector *vPos, short sRot, short sScale, BYTE byAttribute);
	virtual int FindGrassWzu(WzID wID);
	virtual int FindGrass(int iGrassWZU, int iID, WzVector *vPos, WzVector *vRot, WzVector *vScale);
	/** �־��� ũ��� Grass�� ���Ǵ� Wzu ��Ͽ� ���� �޸𸮸� ���Ҵ��Ѵ�. */
	virtual int ReallocGrassWzu(int size);
	/** �־��� ũ��� Grass�� ���� �޸𸮸� ���Ҵ��Ѵ�. */
	virtual int ReallocGrass(int iGrassWZU, int size);
	
	/**	@}	*/ // end of Grass


	/**	@name SpeedTree
	 *	
	 *	SpeedTree�� ���õ� �Լ���.
	 *	@{
	 */

	/** Map���Ϸκ��� SpeedTree ������ �д´�. */
	virtual BOOL LoadSpeedTree(CWzArchive* pArchive);
	virtual St_LHSpeedTreeHeader* GetSpeedTreeHeader();
	virtual int GetSpeedTreeCount();
	virtual St_LHSpeedTree* GetSpeedTree();
	virtual int GetSpeedTreeFileCount();	
	virtual St_LHSpeedTreeFile* GetSpeedTreeFile();
	/** SpeedTree�� ���� ������ �����ϰ� �޸𸮸� ��ȯ�Ѵ�. */
	virtual void DeleteSpeedTree();

	virtual BOOL CreateSpeedTreeHeader(LPSTR lpCompositeTexture, LPSTR lpCompositeShadowTexture);
	virtual BOOL CreateSpeedTreeFileList(int nSpeedTreeFile, int iLastSpeedTreeFile);
	virtual  void SetSpeedTreeFile(int index, int iID, LPSTR filename, WzID wID);
	virtual BOOL CreateSpeedTreeList(int nSpeedTree, int iLastSpeedTree);
	virtual void SetSpeedTree(int index, int iID, WzID wID, DWORD dwAttr, WzVector *vPos, WzVector *vScale);
	virtual void ModifySpeedTree(int index, DWORD dwAttr, WzVector *vPos, WzVector *vScale);

	virtual int	GetLastIndexSpeedTreeFile() { return m_iLastSpeedTreeFile; }
	virtual int	GetLastIndexSpeedTree() { return m_iLastSpeedTree; }

	bool FindIndexSpeedTreeFile(int iID);
	/** �־��� ������ ���� Ʈ�� ������ �ִ��� Ȯ���Ѵ�. */
	virtual bool FindSpeedTreeFile(LPSTR filename, WzID wID);
	bool FindIndexSpeedTree(int iID);
	/** �־��� ������ ���� Ʈ���� �ִ��� Ȯ���Ѵ�. */
	virtual bool FindSpeedTree(WzID wID, WzVector *vPos, WzVector *vScale);
	/** ���ǵ� Ʈ�� ���� ����� ���Ҵ��Ѵ�. */
	virtual int ReallocSpeedTreeFileList(int size);
	/** ���ǵ� Ʈ�� ����� ���Ҵ��Ѵ�. */
	virtual int ReallocSpeedTreeList(int size);

	/**	@}	*/ // end of SpeedTree


	/**	@name Light
	 *	
	 *	����Ʈ�� ���õ� �Լ���.
	 *	@{
	 */

	/** Map���Ϸκ��� Light ������ �д´�. */
	virtual BOOL LoadLight(CWzArchive* pArchive);
	virtual int  GetLightCount();
	virtual St_LHLight* GetLight();
	/** Light������ �����ϰ� �޸𸮸� �����Ѵ�. */
	virtual void DeleteLight();

	virtual bool CreateLocalLightList(int nLight);
	/** 
	 * �������� �迭�� �������� ���� ������ �����Ѵ�. 
	 * @param index : �迭�� �ε���
	 * @param iID : �ĺ� �ε���
	 * @param wc : ����
	 * @param vPos : ��ġ��
	 * @param fRange : 
	 * @paranm byIrradianceRef : 
	 * @param fIntensity :
	 */
	virtual void SetLocalLight(int index, int iID, WzColor wc, WzVector *vPos, float fRange, BYTE byIrradianceRef, float fIntensity);
	/** 
	 * �ش� �������� ������ �����Ѵ�. 
	 * @param index : ��� �������� �迭 �ε���
	 * @param wc : ����
	 * @param vPos : ��ġ��
	 * @param fRange : 
	 * @paranm byIrradianceRef : 
	 * @param fIntensity :
	 */
	virtual void ModifyLocalLight(int index, WzColor wc, WzVector *vPos, float fRange, BYTE byIrradianceRef, float fIntensity);
	virtual int GetLastLight() { return m_iLastLight; }

	virtual bool FindLocalLight(const int iID, WzVector *vPos, const float fRange);
	/** �־��� ũ��� �������� ���� �޸𸮸� ���Ҵ��Ѵ�. */
	virtual int ReallocLocalLight(int size);

	/**	@}	*/ // end of Light


	/**	@name Light
	 *	
	 *	����Ʈ�� ���õ� �Լ���.
	 *	@{
	 */

	/** 
	 * WLU ���� ������ �а� Mesh�� LIghtMap�����鿡 ���� ����� �����Ѵ�.
	 *
	 * @param Filename : Lightmap ���� �����̸�(*.wlu) - I
	 * @param ppLMUV : �޽��� ����Ʈ�� ������ ���� ���ۿ� ���� �����͸� ��´�. - O
	 * @param NumLMMeshGroup : �� �޽� �׷��� ���� - O
	 * @param pTLM : ������ ����Ʈ�� ������ ���� ���ۿ� ���� �����͸� ��´�. - O
	 * @param NumTLM : ���� ����Ʈ�� ���� - O
	 */
	#ifdef CLIENT_ONLY
	virtual BOOL ParseWLU(char *FileName, St_LHMeshGroupLM **ppLMUV, DWORD &NumLMMeshGroup, TerrainLM** ppTLM, DWORD &NumTLM, CDrawBase *pDrawBase );
	#endif
	virtual void ReleaseTerrainWLU();
	virtual void ReleaseUnitWLU();

	virtual St_LHMeshGroupLM *GetMeshGroupLM() { return	m_LMUV;	}
	virtual DWORD GetNumMeshGroupLM() { return m_NumLMMeshGroup; }

	/**	@}	*/ // end of Light


	/**	@name unit
	 *	
	 *	���� ������ �д´�.
	 *	@{
	 */
	
private:
	void ReadUnitData(CWzArchive *pArchive, St_LHUnit *pUnit);
	void ReadUnitGroupData(CWzArchive *pArchive, St_LHUnitGroup *pUnitGroup);

	BOOL ReadMapSubFileHeader(Enum_FileType type, CWzArchive *pArchive, St_LHFileHeader &fhInfo);

public:
	/** ���� ������ �д´�. */
	virtual BOOL LoadUnit(CWzArchive *pArchive, St_LHFileHeader &fhInfo);
	/** ���� ������ �����Ѵ�. */
	virtual void DestroyUnit();

	CWzArray< St_LHUnit* >			*GetUnits() { return &m_Units; }
	CWzArray< St_LHUnitGroup* >		*GetUnitGroups() { return &m_UnitGroups; }

	/** @} */ // end of unit


	/**	@name MapOption
	 *	
	 *	�������� �ɼ� �κп� �ش��ϴ� ������ �д´�.
	 *	@{
	 */

	virtual BOOL LoadMapOption(CWzArchive *pArchive, St_LHFileHeader &fhInfo);
	
	virtual St_LHMapOption *GetMapOption();

	/**	@}	*/ // end of MapOption

	virtual BOOL LoadMapObjectInfoFromMem(CWorldBase *pWorld, CWzArchive *pArchive);

	int		GetReference() { return m_iRef; }
	void	AddReference() { ++m_iRef; }
	void	SubReference() { --m_iRef; }

	//------------------------------------------------------
	// Data.
	//
private:
	static int						m_iRef;

protected:
	St_LHObjectWZU*					m_pObjectWZU;					// ������Ʈ�� ���� WZU ���� ���
	St_LHObjectSpecialTexture*		m_pObjectSpecialTexture;		// ������Ʈ �ؽ��Ŀ� ���� ���
	St_LHObject*					m_pObject;						// ������Ʈ ���Ͽ� ���� ���
	St_LHGrassWZU*					m_pGrassWZU;					// Ǯ�� ���� WZU ���� ���
	St_LHSpeedTreeHeader*			m_pSpeedTreeHeader;				// SpeedTree�� ���� ��� ����
	St_LHSpeedTree*					m_pSpeedTree;					// SpeedTree ���� ���
	St_LHSpeedTreeFile*				m_pSpeedTreeFile;				// SpeedTree ���� ���
	St_LHLight*						m_pLight;						// ����Ʈ ���� ���

	St_LHTerrainLM*					m_TLM;							// ���� ����Ʈ �� ���� ���
	St_LHMeshGroupLM*				m_LMUV;							// �޽��� ����Ʈ�� ����

	CWzArray< St_LHUnit* >			m_Units;						// ������ ��Ͽ� ���� �迭
	CWzArray< St_LHUnitGroup* >		m_UnitGroups;					// ���� �׷쿡 ���� �迭

	St_LHMapOption					m_MapOption;					// �� �ɼ� ���� ����ü

	int								m_nObjectWZU;					// WZU ���� ����
	int								m_nObjectSpecialTexture;		// ������Ʈ�� ���� �ؽ��� ����
	int								m_nObject;						// ������Ʈ ����
	int								m_nGrassWZU;					// Ǯ�� ���� WZU ���� ����
	int								m_nGrassTotal;					// Ǯ�� �� �ν��Ͻ� ����
	int								m_nSpeedTree;					// SpeedTree ����
	int								m_nSpeedTreeFile;				// SpeedTree ���� ����
	int								m_nLight;						// ����Ʈ ����
	DWORD							m_NumLMMeshGroup;				// �޽� �׷쿡 ���� ����Ʈ ������ ����.

	int								m_iLastIndexObjWZU;				// WZU�� �Ҵ�� ������ �ε���
	int								m_iLastIndexSpecTex;			// �ؽ��Ŀ� �Ҵ�� ������ �ε���
	int								m_iLastIndexObj;				// ������Ʈ�� �Ҵ�� ������ �ε���
	int								m_iLastSpeedTreeFile;			// ���ǵ� Ʈ�� ���Ͽ� �Ҵ�� ������ �ε���
	int								m_iLastSpeedTree;				// ���ǵ� Ʈ���� �Ҵ�� ������ �ε���
	int								m_iLastLight;					// ����Ʈ�� ���� ������ �ε���
};

// ��ũ��
#define WZMAPLOADER					CLoadHelper::GetSingleton()

#endif // _3DTERRAIN_LOADHELPER_H_