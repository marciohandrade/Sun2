#pragma once

#include <string>
#include <list>
#include <hash_map>

typedef std::string STD_STRING;
typedef std::list<DWORD> STD_LIST;
typedef stdext::hash_map<DWORD,VOID*> STD_ID_MAP;

class CBaseManager  //. ��ϵ� �Ƶ� ����Ʈ�� �������ش�. std::list�� �����Ǿ� �ִ�.
{
public:
	CBaseManager();
	virtual ~CBaseManager();
	
	virtual bool OnCreate();
	virtual void OnDestroy();

public:
	bool			IsCtrlID(DWORD dwID);				//. ���� ���� �Ƶ� ��� �Ǿ� �ֳ� üũ�� �ش�.
	bool			CreateID(DWORD dwID, VOID* pPtr);	//. �־��� �Ƶ�� �����͸� ��� ��Ų��..
	DWORD 			CreateID(VOID* pPtr);				//. ������ �Ƶ� �����͸� ��� ��Ų��.
	
	bool			RemoveCtrlID(DWORD dwID);			//. �־��� �Ƶ� ����.
	bool			RemoveAllCtrlID();					//. ��� ��ϵ� �Ƶ� �����Ѵ�.
	
	bool			ChangeCtrlID(DWORD dwPrevID,DWORD dwNextID);
	bool			MoveToTail(DWORD dwID);
    bool            MoveToHead(DWORD dwID);
	bool			MoveToListIndex(int nOrig, int nDes);//. list�� ��ϵ� �ε��� ��ġ�� �̵����ش�.
	DWORD			GetCount();

	DWORD				FindIndexID(int nIndex);
	VOID*				FindData(DWORD dwID);
	VOID*				FindIndexData(DWORD dwIndex);
	STD_LIST::iterator	FindDrawList(DWORD dwID);

	//. ��ϵ� Head - > Tail �� üũ�ϸ鼭 �����͸� �Ѱ��ش�.
	virtual void			SetHeadPosition();	
	virtual VOID*			GetNextDrawInfo();

	//. ��ϵ� Tail - > Head �� üũ�ϸ鼭 �����͸� �Ѱ��ش�.
	void			SetTailPosition();
	VOID*			GetPrevDrawInfo();

    STD_LIST& GetDrawList() { return m_sStdDrawList; }
protected:

	STD_LIST::iterator			m_sPosition;
	STD_LIST::reverse_iterator	m_sRePosition;

	STD_ID_MAP m_stdIDMap;								//. �Ƶ�� �����͸� ���
	

	STD_LIST m_sStdDrawList;							//. ��ϵ� �Ƶ��� ���� ����Ʈ.
	DWORD m_dwIDCnt;									//���� ID ������ ��Ÿ����.
	
};

