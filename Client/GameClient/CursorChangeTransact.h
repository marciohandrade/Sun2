//----------------------------------------------------------------------------
/**
	@author	: À¯ Àç ¿µ < y2jinc@webzen.co.kr >
	@since	2004. 11. 30
	@remark
*/

#pragma once

#include <Singleton.h>
#include "MouseCursor.h"


#define CURSOR_CHANGE_LOG	FALSE
#if defined ( _DEBUG )
#	define CURSOR_CHANGE_DEBUG	TRUE
#endif

typedef std::map<DWORD, DWORD>			CURSOR_PRIORITY_MAP;
typedef	std::pair<DWORD, DWORD>				CURSOR_PRIORITY_MAP_PAIR;
typedef CURSOR_PRIORITY_MAP::iterator	CURSOR_PRIORITY_MAP_ITR;

enum eCURSOR_PRIORITY_TYPE
{
	eCURSOR_PRIORITY_TYPE_HIGH		=  0,
	eCURSOR_PRIORITY_TYPE_SOME_HIGH		,
	eCURSOR_PRIORITY_TYPE_MEDIUM		,
	eCURSOR_PRIORITY_TYPE_SOME_LOW		,
	eCURSOR_PRIORITY_TYPE_LOW			,
};

class CursorChangeTransact : public Singleton< CursorChangeTransact >
{
public:
	CursorChangeTransact(void);
	virtual ~CursorChangeTransact(void);

	void Init();
	void Release();

	void ForceChange( MouseCursor::eMouseCursor eCursor );
	void ChangeTranaction( MouseCursor::eMouseCursor eCursor );

private:
	CURSOR_PRIORITY_MAP						m_PriorityMap;		
	std::vector<MouseCursor::eMouseCursor>	m_eCursorVec;
};
