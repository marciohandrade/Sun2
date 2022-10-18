#ifndef		TRIGGERPARSER_H
#define		TRIGGERPARSER_H

#pragma once

#include <vector>
#include <TriggerHeader.h>
#include <SolarHashTable.h>
#include <Singleton.h>
#include <ProgramCommon/WzArchive.h>
#include <ProgramCommon/WzDataType.h>

enum eTRIGGER_TYPE;
struct sTRIGGER_CONTAINER;
struct sTRIGGERLIST;
struct sTRIGGERCATEGORY;
struct sTRIGGERGROUP;

#define MAX_TRIGGER_GROUPNAME_LENGTH             256
#define MAX_TRIGGER_CATEGORYNAME_LENGTH          256

typedef		std::vector<TRIGGER_TYPE *>			TriggerType_vector;
typedef		TriggerType_vector::iterator		TriggerType_iterator;
typedef		std::vector<sTRIGGER_CONTAINER *>	TriggerList_vector;
typedef		TriggerList_vector::iterator		TriggerList_iterator;

typedef		std::vector<sTRIGGERCATEGORY *>			TriggerCategory_vector;
typedef		TriggerCategory_vector::iterator		TriggerCategory_iterator;

typedef		std::vector<sTRIGGERGROUP *>			TriggerGroup_vector;
typedef		TriggerGroup_vector::iterator		TriggerGroup_iterator;

//---------------------------------------------------------------------------------------
//		트리거 데이터
//---------------------------------------------------------------------------------------

struct sTRIGGER_CONTAINER
{
	WzID                    m_TriggerListID;                   
	WORD					m_wTriggerListAtt;

	WORD					m_wNumConTrigger;
	TriggerType_vector		m_ConditionTrigger;

	WORD					m_wNumActTrigger;
	TriggerType_vector		m_ActionTrigger;

	void Load(CWzArchive *pArchive);
	void Destroy();
};


struct sTRIGGERLIST
{
	CODETYPE				m_TriggerCode;
	WORD					m_wNumberOfTriggerContainer;
	TriggerList_vector		m_TriggerListVector;
};


struct sTRIGGERCATEGORY
{
	char                    m_szName[MAX_TRIGGER_CATEGORYNAME_LENGTH];
	WORD                    m_wNumberOfTriggerList;
	TriggerList_vector		m_TriggerListVector;

	void Load(CWzArchive *pArchive);
	void Destroy();

};

struct sTRIGGERGROUP
{
	char                    m_szName[MAX_TRIGGER_GROUPNAME_LENGTH];
	WORD                    m_wNumberOfTriggerCategory;
	TriggerCategory_vector  m_TriggerCategoryList;

	void Load(CWzArchive *pArchive);
	void Destroy();

};

class MapTrigger
{
public:
	MapTrigger();
	~MapTrigger();

	void Init();
	void Destroy();
    
	int  GetGroupNum();
	void Load(CWzArchive *pArchive);

	sTRIGGERGROUP *GetGroup(int iGroupIndex);
	

protected:
	WORD  m_wGroupNum;
	TriggerGroup_vector     m_TriggerGroupList;

};

//---------------------------------------------------------------------------------------
//		트리거 파서( 로더 )
//---------------------------------------------------------------------------------------

class TriggerParser : public util::Singleton<TriggerParser>
{
	enum { _MAX_TRIGGERLIST_SIZE = 30, };
public:
	TriggerParser();
	virtual ~TriggerParser();


	BOOL			Init();
	void			Release( void );
	BOOL			Load( char * pszRootDir, char *pTriggerScriptFileName );
	sTRIGGERLIST *	GetTriggerList( CODETYPE tcode ) { return m_TriggerListForMap.GetData( tcode );	}

	MapTrigger *    Load(CWzArchive *pArchive);

    static BYTE*	SetTriggerType( 
                        WORD Type, 
                        DWORD * pdwSize, 
                        eTRIGGER_TYPE eCondition = eTRIGGER_CONDITION );
	
protected:
	BOOL			Load( char * pTriggerFileName, sTRIGGERLIST * pTriggerList );
	
private:
	util::SolarHashTable<sTRIGGERLIST *>	m_TriggerListForMap;
};

#endif	//	TRIGGERPARSER_H

