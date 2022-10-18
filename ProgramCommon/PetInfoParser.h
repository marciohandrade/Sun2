#pragma  once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>
#include <hash_map>

#define MAX_PET_OPTION_EXTENT_COUNT		5
#define MAX_PET_OPTION_COUNT			3
#define MAX_PET_INTIMACY				100
#define MIN_PET_INTIMACY				0
#define MAX_PET_FULLNESS				100
#define MIN_PET_FULLNESS				0

struct sPetOptionInfo
{
	WORD				m_wOptionKind[MAX_PET_OPTION_COUNT];	
	BYTE				m_byValueType[MAX_PET_OPTION_COUNT];
	INT					m_iValue[MAX_PET_OPTION_COUNT];	
};

struct sPet_Script_Info
{
#ifdef __CN_PET_OPTION_APPLY_FLAG
	enum ePET_OPTION_FLAG
	{
		ePET_OPTION_FLAG_NONE		= (0),			//< 0
		ePET_OPTION_FLAG_1			= (1<<0),		//< 1
		ePET_OPTION_FLAG_2			= (1<<1),		//< 2
		ePET_OPTION_FLAG_3			= (1<<2),		//< 4
		ePET_OPTION_FLAG_4			= (1<<3),		//< 8
		ePET_OPTION_FLAG_5			= (1<<4),		//< 16
		//(shogen)(20090825)
		ePET_OPTION_FLAG_MAX		= (1<<5),		//< 32
		
	};
#endif

	WORD				m_wIndexCode;
	
	char				m_pszName[MAX_MONSTERNAME_LENGTH];
	DWORD				m_dwNameCode;
	MONSTERCODE			m_MonsterCode;

#ifdef _GS_ADD_PET_ENCHANT_EFFECT
    BYTE			    effect_subtype;
#endif//_GS_ADD_PET_ENCHANT_EFFECT
	
	BYTE				m_byIntimacyMin[MAX_PET_OPTION_EXTENT_COUNT];
	BYTE				m_byIntimacyMax[MAX_PET_OPTION_EXTENT_COUNT];

	sPetOptionInfo		m_PetOptionInfo[MAX_PET_OPTION_EXTENT_COUNT];

	BYTE				m_byFullnessUp;
	BYTE				m_byFullnessDown[2];

	BYTE				m_byIntimacyUp;
	BYTE				m_byIntimacyDown[2];

	BYTE				m_byAIType;

#ifdef __CN_PET_OPTION_APPLY_FLAG
	ePET_OPTION_FLAG	m_eApplyOptionFlag;
	BOOL				CanUseOption(BYTE byOptionIndex)
	{
		INT Flag = ePET_OPTION_FLAG_NONE;
		switch(byOptionIndex)
		{
		case 0: Flag = ePET_OPTION_FLAG_1;
			break;
		case 1:	Flag = ePET_OPTION_FLAG_2;
			break;
		case 2:	Flag = ePET_OPTION_FLAG_3;
			break;
		case 3:	Flag = ePET_OPTION_FLAG_4;
			break;
		case 4:	Flag = ePET_OPTION_FLAG_5;
			break;
		}

		return (m_eApplyOptionFlag & Flag);
	}
#endif

	BYTE				m_byPdisplayMove;
	BYTE				m_byPpossition;
};

typedef stdext::hash_map<DWORD, sPet_Script_Info *>		PET_SCRIPT_INFO_HASH;

class PetInfoParser : public util::Singleton<PetInfoParser>, public IParser
{
public:
	PetInfoParser(void);
	virtual ~PetInfoParser(void);

	VOID							Init( char * pszPackFileName );
	VOID							Release();

	VOID							Unload();

	virtual	BOOL					LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

	sPet_Script_Info *				GetPetInfo( DWORD Index );
	int 							GetOptionLevel( WORD wPetIndex , int iCurIntimacy  );

	VOID							SetFirst() { m_HashPetInfoIterator = m_HashPetInfo.begin();	}
	sPet_Script_Info*				GetNext();

protected:
	BOOL							ValidCheck(sPet_Script_Info *pInfo);
private:
	BOOL							_Load( BOOL bReload );

	PET_SCRIPT_INFO_HASH			m_HashPetInfo;
	PET_SCRIPT_INFO_HASH::iterator	m_HashPetInfoIterator;
	char						m_pszFileName[MAX_PATH];
};
