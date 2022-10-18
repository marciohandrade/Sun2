#pragma once

#include <string>
#include <list>
#include <hash_map>

typedef std::string STD_STRING;
typedef std::list<DWORD> STD_LIST;
typedef stdext::hash_map<DWORD,VOID*> STD_ID_MAP;

class CBaseManager  //. 등록된 아디 리스트를 관리해준다. std::list로 구성되어 있다.
{
public:
	CBaseManager();
	virtual ~CBaseManager();
	
	virtual bool OnCreate();
	virtual void OnDestroy();

public:
	bool			IsCtrlID(DWORD dwID);				//. 인자 값의 아디가 등록 되어 있나 체크해 준다.
	bool			CreateID(DWORD dwID, VOID* pPtr);	//. 주어진 아디와 포인터를 등록 시킨다..
	DWORD 			CreateID(VOID* pPtr);				//. 임의의 아디에 포인터를 등록 시킨다.
	
	bool			RemoveCtrlID(DWORD dwID);			//. 주어진 아디를 제거.
	bool			RemoveAllCtrlID();					//. 모든 등록된 아디를 제거한다.
	
	bool			ChangeCtrlID(DWORD dwPrevID,DWORD dwNextID);
	bool			MoveToTail(DWORD dwID);
    bool            MoveToHead(DWORD dwID);
	bool			MoveToListIndex(int nOrig, int nDes);//. list에 등록된 인덱스 위치를 이동해준다.
	DWORD			GetCount();

	DWORD				FindIndexID(int nIndex);
	VOID*				FindData(DWORD dwID);
	VOID*				FindIndexData(DWORD dwIndex);
	STD_LIST::iterator	FindDrawList(DWORD dwID);

	//. 등록된 Head - > Tail 로 체크하면서 포인터를 넘겨준다.
	virtual void			SetHeadPosition();	
	virtual VOID*			GetNextDrawInfo();

	//. 등록된 Tail - > Head 로 체크하면서 포인터를 넘겨준다.
	void			SetTailPosition();
	VOID*			GetPrevDrawInfo();

    STD_LIST& GetDrawList() { return m_sStdDrawList; }
protected:

	STD_LIST::iterator			m_sPosition;
	STD_LIST::reverse_iterator	m_sRePosition;

	STD_ID_MAP m_stdIDMap;								//. 아디와 포인터를 등록
	

	STD_LIST m_sStdDrawList;							//. 등록된 아디의 순서 리스트.
	DWORD m_dwIDCnt;									//현재 ID 갯수를 나타낸다.
	
};

