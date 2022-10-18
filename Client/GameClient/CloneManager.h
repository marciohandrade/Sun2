#ifndef	__CLONE_MANAGER_H__
#define	__CLONE_MANAGER_H__
//------------------------------------------------------------------------------
/**

    CLONE 객체를 관리  
    @author

    @since

    @remarks
        -2005.06.07 소스 정리(지완)
        - 
*/

#pragma once

class Object;
class Clone;
class Character;

typedef		stdext::hash_map<DWORD, Clone *>		Clone_Map;
typedef		Clone_Map::iterator			    Clone_Map_Itr;
typedef		std::pair<DWORD, Clone *>		    Clone_Map_Pair;

class CloneManager
{
public:
	CloneManager(void);
	~CloneManager(void);

	void				Clear();
	DWORD				CreateClone( Object *pObject);
	DWORD				CreateAttachmentClone( Object *pObject);

	Clone               *CreateNotManagedClone(Object *pObject);
	BOOL				Delete( DWORD dwObjectKey );
	Clone *				GetClone(DWORD dwKey);
	int					GetCloneCnt(){return m_hashmapClone.size();}

	Clone_Map::iterator GetBegin()
	{
		return m_hashmapClone.begin();
	}

	Clone_Map::iterator GetEnd()
	{
		return m_hashmapClone.end();
	}
	void				Process(DWORD dwTick);

protected:
	DWORD				m_dwLastKey;
	Clone_Map			m_hashmapClone;
};

extern CloneManager g_CloneManager;
#endif 
