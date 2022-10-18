#ifndef __QUERY_H__
#define __QUERY_H__

#pragma once


#define		MAX_QUERY_LENGTH		256

class Query
{
	
public:
	Query() : m_pVoidObject( NULL ) {}
	~Query() {}

	inline VOID						Init( DWORD dwIdx, char * pszQuery ) { m_dwIndex = dwIdx; strncpy( m_pszQuery, pszQuery, MAX_QUERY_LENGTH ); }
	inline VOID						SetIndex( DWORD dwIdx ) { m_dwIndex = dwIdx; }

	inline DWORD					GetIndex() { return m_dwIndex;	}
	inline const char * const		GetQuery() { return m_pszQuery;	}

	inline VOID *					GetVoidObject() { return m_pVoidObject;	}
	inline VOID						SetVoidObject( VOID * pVoidObject ) { m_pVoidObject = pVoidObject; }
	inline VOID						SetQuery( char * pszQuery ) { strncpy( m_pszQuery, pszQuery, MAX_QUERY_LENGTH ); }

private:
	DWORD							m_dwIndex;
	VOID *							m_pVoidObject;
	char							m_pszQuery[MAX_QUERY_LENGTH];
};


#endif // __QUERY_H__