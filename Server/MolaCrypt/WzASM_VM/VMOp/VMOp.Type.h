#pragma once

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
#include <list>
#include <map>
#include <algorithm>

using namespace std;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
enum eMOLASIG_TYPE
{
	SIGMSG_NORMAL = 0x4400
};

struct eMOLASIG_MSG
{
	eMOLASIG_TYPE	_TYPE;
};

class IVMOp
{
public:
	virtual BOOL	DoInit() = 0;
	virtual BOOL	DoProcess() = 0;
	virtual BOOL	IsComplete() = 0;
	virtual BOOL	DoRelease() = 0;
	// Control Interface
	virtual VOID	OnMsg( eMOLASIG_MSG* pSIG_MSG ) {}
	//virtual BOOL	IsPending() = 0;
	//virtual VOID	SetPending( BOOL bPending ) = 0;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//
template<typename T>
class Delegator
{
public:
	Delegator() : m_POINTER(NULL) {}
	Delegator( T* ptr ) : m_POINTER(ptr) {}
	~Delegator() { m_POINTER = NULL; }

	inline BOOL operator == ( PVOID ptr ) const { return m_POINTER == ptr; }
	inline BOOL operator != ( PVOID ptr ) const { return !(m_POINTER == ptr); }

	inline BOOL operator == ( const Delegator<T>& ref ) const { return ref.m_POINTER == m_POINTER; }
	inline BOOL operator != ( const Delegator<T>& ref ) const { return !(ref.m_POINTER == m_POINTER); }

	inline T* operator-> ()	{ return m_POINTER; }

	inline T& operator* ()	{ return *m_POINTER; }

	inline T* operator() ()	{ return m_POINTER; }

	inline T*& REFERENCE()	{ return m_POINTER; }

private:

	// <FIELDs>
	T*		m_POINTER;
};
