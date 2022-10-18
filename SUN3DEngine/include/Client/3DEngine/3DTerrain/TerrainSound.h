// terrainsound.h

#ifndef _3DTERRAIN_TERRAINSOUND_H_
#define _3DTERRAIN_TERRAINSOUND_H_

//------------------------------------------------------------------------------
/**
    Project: WzMapEditor 

    Date:   ~ 2005.11.25
    Rework: 2005.12.5 ~
    Author: �����, Won ShinYeon (3D Engine Team)

    LastModify:
    @2006.05.18
    ; TCHAR�� ������ ������ ��� char�� ������.

    Copyright 2004 WEBZEN All rights.
*/

//----------------------------------------------------------------------------------------
// ��� ���� ���� ����
//
#include "octree.h"
#include <programcommon/wzarchive.h>
#include <client/3dengine/common/boundingvolume.h>
#include <vector>
#include <map>

//----------------------------------------------------------------------------------------
// ��ó�� ����
//
class COctree;
class CWorldBase;

//----------------------------------------------------------------------------------------
// ��ũ�� ����� ����
//
#define VALID_VERSION_FOR_SOUNDFILE			157
#define VERSION_SOUNDAREA					0x0001
#define VERSION_SOUNDDATA					0x0001

//#define CHUNK_ID_SOUND					0x1791 ///< �� ���Ͽ� ����Ǵ� ûũ �ĺ���

//#define CHUNK_SOUNDAREA_HEADER			0x1811 ///< ���� ���Ͽ� ����Ǵ� ûũ �ĺ���
//#define CHUNK_SOUNDAREA					0x1812
//#define CHUNK_SOUNDDATA_HEADER			0X1821
//#define CHUNK_SOUNDDATA					0X1822

#define MAX_SOUNDFILENAMELEN				128
#define MAX_ACTIVE_SOUND					2

#define DEFAULT_SOUNDFILENAME				"NOSOUNDFILE"
#define EXTENSION_SOUNDFILE					".sdt"



/*---------------------------------------------------------------------------------------------------

											CTerrainSound2

 ----------------------------------------------------------------------------------------------------*/


// �ڵ��� ���Ǹ� ���� ������ ������ Ÿ��
typedef std::vector<WzID>					StlVWzIDList;
typedef std::vector<WzID>::iterator			StlVWzIDListIter;
typedef int									SOUND_INDEX;
typedef std::vector<SOUND_INDEX>			StlVSoundGroupList;
typedef std::vector<SOUND_INDEX>::iterator  StlVSoundGroupListIter;

/// ���� ���� ��� ������
enum EN_SOUNDAREATYPE 
{
	SVT_AABB,
	SVT_SPHERE,
	SVT_LAST
};

/// ���� ������ ������ ����� ������ ���� ����ü
struct St_SoundVolume
{
	WzID			m_WzID;			  ///< WzID�� ǥ���Ǵ� �ĺ���	
	WzOctIndex		m_OctIndex;		  ///< Octree�� ��Ͻ� �޴� �ε�����
	WORD			m_wIndex;		  ///< �迭���� �ε��� ��
	WORD			m_Reserved;	
	float			m_fDistance;	  ///< ���� ����ø��� �������� ��谣�� ������������ �Ÿ�

	WzBoundingVolume m_BoundVolume;	  ///< ���� ������ ���� ��� ����

	StlVWzIDList	m_SoundDataList;  ///< ���� ������ ������ ���� �׷� ���
};

typedef std::map< int, St_SoundVolume* > StlMSoundVolume;


/// ���� �������� �Ӽ��� ��Ÿ���� �÷���(Sound Data Attribute)
#define	SDA_NONE     0x00
#define	SDA_AMBIENT	 0x01
#define	SDA_3DSOUND	 0x02
#define SDA_EAX		 0x04
#define	SDA_ALL		 (SDA_AMBIENT | SDA_3DSOUND | SDA_EAX)

/// @brief ���� ������ ������ ���� ����ü
///
/// ������ ȯ�� ����� ���� ������ �����ϰ� ����Ѵ�.
struct St_SoundFile
{
	// ����/�ε� �Ǵ� ���
	int			m_iSoundIdx;					///< ���� �ĺ� �ε���
	float		m_fMinDist;						///< 3D ����� �ּ� ��ȿ �Ÿ�
	float		m_fMaxDist;						///< 3 D����� �ִ� ��ȿ �Ÿ�

	// ���� �ÿ� ����ϴ� ���
	int			m_iSoundHandle;					///< ���� ����� ���Ϲ޾� �����ϰ� �־���ϴ� �ڵ鰪

	char		m_szName[MAX_SOUNDFILENAMELEN]; ///< ���ϸ� ���� ����
};

// ���� ���� ���� ����� ���� ������ Ÿ�� ����
typedef std::vector< St_SoundFile > StlVSoundList;

/// ȯ�� ���� ���� ����ü
struct St_AmbientSound2
{
	//------------------------------------
	// Methods
	//
	~St_AmbientSound2()
	{
		m_SoundList.clear();
	}
	
	//------------------------------------
	// Data
	//
	StlVSoundList m_SoundList;
};

struct St_3DSound
{
	StlVSoundList m_SoundList;

	~St_3DSound()
	{
		m_SoundList.clear();
	}
};

struct St_EAX
{
	unsigned long	m_ulID;			/// sound environment ID
	float			m_fVolume;
	float			m_fDamping;
	float			m_fDecay;
};

struct St_Link
{
	WzID			m_idArea;		///< ���� ���� ID
	WzID			m_idSndData;	///< �����ϰ��� �ϴ� ���� ������ ID
};


/// ����� ���� ������ ���� ����ü
struct St_SoundData
{
	//------------------------------------
	// Methods
	//

	St_SoundData()
		: m_iIndex(-1)
		, m_WzID(INVALID_WZID)
		, m_Flags(SDA_NONE)
		, m_pAmbient(NULL)
		, m_p3DSound(NULL)
		, m_pEAX(NULL)
	{}
	~St_SoundData()
	{
		if(m_pAmbient) delete m_pAmbient;
		if(m_p3DSound) delete m_p3DSound;
		if(m_pEAX)	   delete m_pEAX;
	}


	//------------------------------------
	// Data
	//

	int					m_iIndex;		///< ���� �׷��� �ε���
	WzID				m_WzID;			///< ���� �������� WzID
	BYTE				m_Flags;		///< ���� �׷��� ������ �ִ� ���� ������ ���� �÷���
	char				m_Reserved[3];	///< 4byte ������ ���ؼ�

	St_AmbientSound2*	m_pAmbient;		///< ȯ�� ���� ���� ����ü�� ���� ������
	St_3DSound*			m_p3DSound;		///< 3D ���忡 ���� ���� ����ü�� ���� ������
	St_EAX*				m_pEAX;			///< EAX ���� ���� ����ü�� ���� ������
};

struct St_GlobalSoundData
{
	St_SoundFile		m_GlobalAmbient; ///< ���� ȯ�� ���� ���� ����
};

// ��ó�� ����.
struct St_LHFileHeader;

/// ������ ���� ���� ������ �����ϴ� Ŭ����
/// ������ CTerrainSound�� ���̻� ������� �ʴ´�.
///
/// @modify
//	 2005.12.14
//		- ��Ī����: SoundVolume -> SoundArea
//		- ��Ī����: SoundGroup  -> SoundData
/// @wranning
///
class CTerrainSound2
{
private:
	BOOL Serialize_SoundArea(CWzArchive *pArchive);
	BOOL Serialize_SoundData(CWzArchive *pArchive);

	int FindEmptySlot_SoundArea();
	int FindEmptySlot_SoundData();

	/// WzID�� �̿��Ͽ� ���� ���� �ν��Ͻ��� ã�´�.
	St_SoundVolume* FindSoundArea(WzID id);
	float CalcDistance(WzBoundingVolume* pbvTarget, WzVector &wvFrom, float *pfDistToCenter);

	void RemoveAllSoundArea();
	void RemoveAllSoundData();

	BOOL LoadSoundFile(LPCSTR lpFilename, St_LHFileHeader &fhInfo);
	BOOL SaveSoundFile(LPCSTR lpFilename, St_LHFileHeader &fhInfo);

public:
	//--------------------------------------------
	// �ܺη� �����Ǵ� �Լ���
	//

	/// ���� ������ �ʱ�ȭ
	virtual void Initialize(CWorldBase* pWorld, COctree* pOctree);
	/// ���� �������� ��� �ν��Ͻ� ����� ����
	virtual void Clear();
	/// ���Ͽ� ������ �����ϰų�/�о�´�.
	/// @param bSkip : ���� ������ �ε�/���̺� ���� �ʰ� �ǳʶ��.
	virtual BOOL Serialize( CWzArchive *pArchive, St_LHFileHeader &fhInfo, bool bSkip=false);
	
	/// �����Ӻ� ��鿡 �����Ǿ� Ȱ��ȭ/��Ȱ��ȭ�� ���� ����� �����
	virtual void BuildSoundList(CDrawBase* pDraw, WzFrustum &wfFrustum, WzVector &wvFrom, float fRange=30.f);

	/// ���� ������ �߰��Ѵ�.
	/// @param index: ����Ʈ�󿡼��� �ε�����(���)
	/// @return return a handle of sound area instance.
	virtual int AddSoundArea(WzID id, WzBoundingVolume& bv, int *handle);
	/// ���� ������ �ν��Ͻ��� �߰��Ѵ�.
	virtual int AddSoundData(WzID id);

	virtual void DelSoundData(WzID id);

	/// index�� �ش��ϴ� ���� ���� ��ü�� WzID�� �����Ѵ�.
	virtual void SetWzID_SoundArea(const int index, const WzID id);
	///
	virtual void SetWzID_SoundData(const int index, const WzID id);

	/// index�� �ش��ϴ� ���� ���� ��ü�� �����Ѵ�.
	virtual void UpdateSoundArea(const int index, const WzBoundingVolume& bv);
	///
	virtual void UpdateSoundArea_SoundGroupList(const int index, StlVWzIDList& list);
	///
	virtual void UpdateSoundData_AmbientSoundList(const int index, StlVSoundList &list);
	virtual void UpdateSoundData_3DSoundList(const int index, StlVSoundList &list);
	virtual void UpdateSoundData_EAX(const int index, unsigned long ulID, float fVolume,  float fDamping, float fDecay);

	/// ���� Frustum�ȿ� �ִ� Octree ��忡 ����� ���� �����鿡 ���� ����� ���� �迭�� ���Ϲ޴´�.
	CWzArray<DWORD>			*GetSoundObjectList_InFrustum() { return m_parSoundObject; }
	/// �ֱ� Ȱ��ȭ�� ���� ������ ���
	StlMSoundVolume			*Enum_ActiveSoundArea() { return &m_LastSndAreas; }
	/// Frustum���κ��� ����� ���̻� Ȱ��ȭ ���°� ���� �ʴ� ���� ���� ���
	StlMSoundVolume			*Enum_DeactiveSoundArea() { return &m_DeactiveSndAreas; }

	// �־��� ���ϸ����� ���� ���� ���ϸ��� �����.
	virtual void			SetFilename(LPCSTR pFilename);
	/// �־��� ���ϸ����� ���� ���� ���ϸ��� �����. (== SetFilename())
	virtual void			MakeFilename(LPCSTR pFilename);
	/// ���� ȯ�� ���带 �����Ѵ�.
	virtual void			SetGlobalAmbientSound(int index, LPCSTR lpFilename);
	virtual void			GetGlobalAmbientSound(int &index, LPSTR lpFilename);

	virtual int				SoundAreaCount() { return m_arSoundArea.GetCount(); }
	virtual int				SoundDataCount() { return m_arSoundData.GetCount(); }
	virtual St_SoundVolume	*GetSoundArea(int index) { return m_arSoundArea.Get(index); }
	virtual St_SoundData	*GetSoundData(int index) { return m_arSoundData.Get(index); }
	virtual void			Get_AmbientSoundList(const int index, StlVSoundList **ppList);
	virtual void			Get_3DSoundList(const int index, StlVSoundList **ppList);
	virtual St_SoundData	*FindSoundData(WzID id);
	virtual St_EAX			*GetCurEAX() { return m_pCurEAX; }
	virtual BOOL			RemoveSoundArea(const int index);
	virtual BOOL			RemoveSoundData(const int index);
	virtual void			RenewalSoundArea(const int handle);

	// ���� ������(SD)�� ���� ����(SA)�� �����Ѵ�.
	virtual BOOL LinkSDToSV(WzID sd, WzID sa);
	virtual BOOL UnlinkSDFromSV(WzID sd, WzID sa);
	/// �־��� ���ϸ��� ������ �����ϴ��� Ȯ���Ѵ�.
	/// @return true�̸� ������ �����Ѵ�.
	virtual bool FileExists(LPCSTR lpFilename);

	CTerrainSound2(void);
	virtual ~CTerrainSound2(void);


	//--------------------------------------------
	// Data
	//
private:
	COctree							*m_pOctree;					///< ���� Octree ��ü�� ���� ������
	CWorldBase						*m_pWorld;					///< World ��ü�� ���� ������
	
	StlMSoundVolume					m_LastSndAreas;				///< �ֱ� Ȱ��ȭ�� ���� ���� ���
	StlMSoundVolume					m_DeactiveSndAreas;			///< ��Ȱ��ȭ�� ���� ���� ���
	char							m_szFilename[_MAX_PATH];	///< ���� ������ ����� ���� �̸�

	CWzArray<DWORD>					*m_parSoundObject;

public:
	CWzArray<St_SoundVolume*>		m_arSoundArea;
	CWzArray<St_SoundData*>			m_arSoundData;
	St_EAX							*m_pCurEAX;
	St_GlobalSoundData				m_GlobalSound;
};






/*---------------------------------------------------------------------------------------------------

											CTerrainSound

 ----------------------------------------------------------------------------------------------------*/




//----------------------------------------------------------------------------------------
// ��ũ�� ����� ����
//
#define MAX_TERRAIN_SOUND_EVENT_PARAM  (4)

enum ENTERRAIN_SOUNDMSG
{
	//�Ϲ� 2D�� ����
	eTSM_PLAYSOUND,

	eTSM_DEACTIVE_AS_GROUP,
	eTSM_ACTIVE_AS_GROUP,

	//���� ����.
	eTSM_LOOP_SOUND_PLAY,
	eTSM_LOOP_SOUND_STOP,
	eTSM_LOOP_SOUND_SET_VOLUME,// Fade out/in  ���� ���
};

struct ST_TerrainSoundEvent
{
	ENTERRAIN_SOUNDMSG		m_EnventType;
	void*					m_avParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
	int						m_aiParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
	float					m_afParam[MAX_TERRAIN_SOUND_EVENT_PARAM];
};


// ������ ���� ���� ��ü. �ܼ��� ���� ������ ���� ȣȯ�� ��������
// �����Ѵ�.
class CTerrainSound
{
public:
	CTerrainSound(void){}
	virtual ~CTerrainSound(void) {}
	
	virtual BOOL Serialize( CWzArchive *pArchive);
};

#endif // _3DTERRAIN_TERRAINSOUND_H_