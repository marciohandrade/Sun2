#pragma once

//=============================================================================================================================
/// SolarAuth Interfaces
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	@history 
	-  
*/
//=============================================================================================================================

// 객체 외부 제거 목적 인터페이스
class IDisposer
{
public:
	virtual VOID	Dispose() = 0;
};

//
//
//

enum eSIG_TYPE
{
	SIGMSG_NORMAL = 0x4400
};

struct sSIG_MSG
{
	eSIG_TYPE	_TYPE;
	sSIG_MSG() : _TYPE(SIGMSG_NORMAL) {}
	sSIG_MSG( const eSIG_TYPE sig_type ) : _TYPE(sig_type) {}
};

struct sSIG_EXTEND : sSIG_MSG 
{
	DWORD		_SUBTYPEorSIZE;
	sSIG_EXTEND() : sSIG_MSG(), _SUBTYPEorSIZE(0) {}
	sSIG_EXTEND( const eSIG_TYPE sig_type ) : sSIG_MSG(sig_type), _SUBTYPEorSIZE(0) {}
	sSIG_EXTEND( const eSIG_TYPE sig_type, DWORD sig_subtype ) : sSIG_MSG(sig_type), _SUBTYPEorSIZE(sig_subtype) {} 
};

struct sSIG_USRDFL : sSIG_MSG
{
	BYTE		_PTR[1];
};


//
//
//

class IWorkNode
{
public:
	virtual ~IWorkNode() {}
	virtual BOOL	DoInit() = 0;
	virtual BOOL	DoProcess() = 0;
	virtual BOOL	IsComplete() = 0;
	virtual BOOL	DoRelease() = 0;
	// Control Interface
	virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG ) { __UNUSED(pSIG_MSG); }
	//virtual BOOL	IsPending() = 0;
	//virtual VOID	SetPending( BOOL bPending ) = 0;
};


